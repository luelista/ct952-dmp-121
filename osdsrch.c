#include    "winav.h"

#ifndef NO_SEARCH_UI //++CoCo2.37p

#include    "input.h"
#include    "osd.h"
#include    "hal.h"
#include    "cc.h"
#include    "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include    "dvdinfo.h"
#include    "infofilter.h"
#include    "linear.h"
#include    "navigate.h"
#include    "utl.h"
#include    "osdsrch.h" 
#include    "cell.h"
#include    "vcd20.h"
#include    "digest.h"
#include    "thumb.h"
#include    "srcfilter.h"
#include    "media.h"
#include    "osdnd.h" 
#include    "setup.h"
#include    "parser.h"
#ifdef SIMPLE_SEARCH_UI
#include    "osddsply.h"
#endif

#include "notransl.h"

/* alan test
WORD aTitleIcon[] = {1, CHAR_N_TITLE};  
WORD aChapterIcon[] = {1, CHAR_N_CHAP};             
WORD aTimeIcon[] = {1, CHAR_N_TIME};       
*/


//alex909,20040909 move osdsrch.c
/***********************************/
//          DEFINE
/***********************************/

#define FRAME_THICKNESS     2
#define TOTAL_BUTTON_NUM_IN_SRCH      4
#define TOTAL_SP_NUM_IN_VIR           2  


#define BUTTON_PREV         0
#define BUTTON_PLAY         1
#define BUTTON_PAUSE        2
#define BUTTON_STOP         3
#define BUTTON_NEXT         4
#define BUTTON_SPST_CTL     5
#define BUTTON_AST_CTL      6
#define BUTTON_AUDIO_LR     BUTTON_AST_CTL
#define BUTTON_ANGLE_CTL    7
#define BUTTON_EXIT         8

#define SEARCH_TITLE        0
#define SEARCH_TRACK        SEARCH_TITLE
#define SEARCH_CHAPTER      1
#define SEARCH_TIME         2


#define BUTTON_GAP          8

#ifndef SIMPLE_SEARCH_UI

#define SRCH_FRAME_X0     25
#define SRCH_FRAME_Y0     40 
#define SRCH_FRAME_X1     375
#define SRCH_FRAME_Y1     SRCH_FRAME_Y0+52

#define SRCH_SEPARATOR_WIDTH     2*BUTTON_GAP
#define SRCH_BUTTON_WIDTH        5*BUTTON_GAP

//Search Title/Track Button
#define BUTTON_T_X0  (SRCH_FRAME_X0 + BUTTON_GAP + 15)
#define BUTTON_T_Y0  (SRCH_FRAME_Y0 + BUTTON_GAP + 5)
#define BUTTON_T_X1  (BUTTON_T_X0   + SRCH_BUTTON_WIDTH + 5)
#define BUTTON_T_Y1  (SRCH_FRAME_Y1 - BUTTON_GAP)

//Search Chapter Button
#define BUTTON_C_X0  (BUTTON_T_X1 + 2*SRCH_BUTTON_WIDTH + 3*BUTTON_GAP + 15)
#define BUTTON_C_Y0  BUTTON_T_Y0
#define BUTTON_C_X1  (BUTTON_C_X0 + SRCH_BUTTON_WIDTH + 5)
#define BUTTON_C_Y1  BUTTON_T_Y1

#define SOLIDUS_T_X    (BUTTON_T_X1+BUTTON_GAP*2+40)
#define SOLIDUS_C_X    (BUTTON_C_X1+BUTTON_GAP*2+40)

#else    //  #ifndef SIMPLE_SEARCH_UI

#define SRCH_FRAME_X0     25
#define SRCH_FRAME_Y0     OSDDSPLY_LD_START_V
#define SRCH_FRAME_X1     (352 + SRCH_FRAME_X0)
#define SRCH_FRAME_Y1     (SRCH_FRAME_Y0+26)

#define SRCH_SEPARATOR_WIDTH     BUTTON_GAP
#define SRCH_BUTTON_WIDTH        (4*BUTTON_GAP)

//Search Title/Track Button
#define BUTTON_T_X0  (SRCH_FRAME_X0 + BUTTON_GAP*3) 
#define BUTTON_T_Y0  SRCH_FRAME_Y0
#define BUTTON_T_X1  (BUTTON_T_X0   + SRCH_BUTTON_WIDTH + 20)
#define BUTTON_T_Y1  SRCH_FRAME_Y1

//Search Chapter Button
#define BUTTON_C_X0  (BUTTON_T_X1 + 2*SRCH_BUTTON_WIDTH + BUTTON_GAP + 15)
#define BUTTON_C_Y0  BUTTON_T_Y0
#define BUTTON_C_X1  (BUTTON_C_X0 + SRCH_BUTTON_WIDTH + 20)
#define BUTTON_C_Y1  BUTTON_T_Y1

#define SOLIDUS_T_X    (BUTTON_T_X1+BUTTON_GAP+20)
#define SOLIDUS_C_X    (BUTTON_C_X1+BUTTON_GAP+20)

#endif

//Search Time Button
#define BUTTON_E_X0  BUTTON_C_X0
#define BUTTON_E_Y0  BUTTON_C_Y0
#define BUTTON_E_X1  BUTTON_C_X1
#define BUTTON_E_Y1  BUTTON_C_Y1

//Kevin1.07, VCD/CDDA time postion for absolute gototime
#define BUTTON_EABS_X0  BUTTON_T_X0
#define BUTTON_EABS_Y0  BUTTON_T_Y0
#define BUTTON_EABS_X1  BUTTON_T_X1
#define BUTTON_EABS_Y1  BUTTON_T_Y1
  



typedef struct
{             
    WORD wX0; 
    WORD wY0;
    WORD wX1;
    WORD wY1;               
    WORD *wpOSDVIRFONT;
} BUTTON_INFO;



BUTTON_INFO aSRCHButton[TOTAL_BUTTON_NUM_IN_SRCH] =
{ {BUTTON_T_X0, BUTTON_T_Y0, BUTTON_T_X1, BUTTON_T_Y1, aTitleIcon},
  {BUTTON_C_X0, BUTTON_C_Y0, BUTTON_C_X1, BUTTON_C_Y1, aChapterIcon},
  {BUTTON_E_X0, BUTTON_E_Y0, BUTTON_E_X1, BUTTON_E_Y1, aTimeIcon},
  {BUTTON_EABS_X0, BUTTON_EABS_Y0, BUTTON_EABS_X1, BUTTON_EABS_Y1, aTimeIcon}};  



/***********************************/
//          Variable
/***********************************/
PARM_FRAME          _OSDSRCHFrame;
PARM_DRAW_STRING    _OSDSRCHStringAttr;
PARM_RECT           _OSDSRCHGRect;
WORD                _wOSDSRCHString[GDI_STRING_LENGTH+1];


/***********************************/
//          Function
/***********************************/				

static BYTE _OSDSRCH_SearchKey(BYTE bKey);
static void _OSDSRCH_InitializeGDI(void);
static void _OSDSRCH_Process_KeyNumber ( WORD wValue );
static void _OSDSRCH_ConfirmAction(WORD wValue);
void _OSDSRCH_ShowButton(BYTE bButtonIndex); //CoCo1.20, reduce code size.
static void _OSDSRCH_ShowSrchFrame(void);
static void _ShowOneEntry(BYTE bColor); //rename later
//static void _ShowOneEntryNumber(BYTE bSize, BYTE bNumber);
static void _ShowOneEntryNumber(BYTE bSize, WORD wNumber);
#ifndef NO_DISC_MODE //CoCo2.37p
static void _OSDSRCH_ShowTwoEntry(BYTE bColor1 , BYTE bColor2); // 
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
static void _OSDSRCH_ClearOSDRegion(void); //CoCo0.76
void _OSDSRCH_DrawFrame(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom); //CoCo1.20, reduce code size.
void _OSDSRCH_DrawString(void);  //CoCo1.20, reduce code size.
static void _OSDSRCH_Exit(void);
extern void _InitRect(PURECT pRect, WORD wLeft, WORD wTop, WORD wRight, WORD wBottom); //CoCo1.20, reduce code size.

//alex080,20041231,

void _OSDSRCH_Entry_Initial(void);
#ifndef NO_DISC_MODE //CoCo2.37p
void _OSDSRCH_Entry_TitleChapter(void);
void _OSDSRCH_Entry_TitleTime(void);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

void _OSDSRCH_Entry_PureTime(void);
void _OSDSRCH_SetCommonFrameAttr(void);

WORD    __wMaxNumber;
WORD    _wPreValue;
DWORD   __dwTimeNumber;


extern WORD code aDigit[];

//Global variables 
extern  BYTE    __bGoToTimeNO; 
//current pos 
extern BYTE    __bSearchCursor; 



//  *************************************************************************
//  Function    :   _OSDSRCH_ShowTwoEntry
//  Description :   Show twoe Entry Number in either SEARCH_TITLE, SEARCH_CHAPTER, or SEARCH_TIME field. a Button in the VIR Frame//  Arguments   :   
//                  bColor: First Color in Title
//                  bColor: Second Color in Chapter
//  Return      :   None
//  Side Effect :   None
//  *************************************************************************
#ifndef NO_DISC_MODE //CoCo2.37p
static void _OSDSRCH_ShowTwoEntry(BYTE bColor1 , BYTE bColor2)
{
    if (__bSearchCursor == SEARCH_CURSOR_TITLE)
	{
	    __bSearchCursor = SEARCH_CURSOR_CHAPTER;             
            _ShowOneEntry(bColor1);            
	    __bSearchCursor = SEARCH_CURSOR_TITLE;            
	    _ShowOneEntry(bColor2);
	}        
    else if (__bSearchCursor == SEARCH_CURSOR_CHAPTER)
	{
		__bSearchCursor = SEARCH_CURSOR_TITLE;            
		_ShowOneEntry(bColor1);
		__bSearchCursor = SEARCH_CURSOR_CHAPTER;             
		_ShowOneEntry(bColor2);        
    }
    
}
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

//  *************************************************************************
//  Function    :   _OSDSRCH_ShowButton
//  Description :   Show a Button in the VIR Frame
//  Arguments   :   bButtonIndex: Button Index ( 0 ~ (TOTAL_BUTTON_NUM_IN_SRCH-1) )
//                  bColor: Center Color & Text BG Color
//  Return      :   None
//  Side Effect :   None
//  *************************************************************************
void _OSDSRCH_ShowButton(BYTE bButtonIndex) //CoCo1.20, reduce code size.
{
   
#ifdef ABSOLUTE_GOTOTIME //Kevin1.07, VCD/CDDA shows track# only for relative gototime only                
    if ( !(__wDiscType & BOOK_DVD) )                
        bButtonIndex ++;
#endif    
    

#ifndef SIMPLE_SEARCH_UI
    if (SETUP_CUSTOM_UI_STYLE_1 == __SetupInfo.bUIStyle)
    {
        _OSDSRCHFrame.bColorBright=  OSDSRCH_ENTRY_COLOR_FRAME_BRIGHT;
        _OSDSRCHFrame.bColorCenter=  OSDSRCH_ENTRY_COLOR_ICON_BG;
        _OSDSRCHFrame.bColorDark=    OSDSRCH_ENTRY_COLOR_FRAME_DARK;
        _OSDSRCHFrame.bStyle=        FRAME_POP_SOLID; 
        _OSDSRCHFrame.bThickness =   FRAME_THICKNESS;

        _OSDSRCH_DrawFrame(aSRCHButton[bButtonIndex].wX0,
                          aSRCHButton[bButtonIndex].wY0,
                          aSRCHButton[bButtonIndex].wX1,
                          aSRCHButton[bButtonIndex].wY1); //CoCo1.20, reduce code size.

        _OSDSRCHStringAttr.wX = aSRCHButton[bButtonIndex].wX0+8;
    }
    else if (SETUP_CUSTOM_UI_STYLE_2 == __SetupInfo.bUIStyle)
    {
        _OSDSRCHGRect.bColor = OSDSRCH_ENTRY_COLOR_ICON_BG;
        _InitRect(&(_OSDSRCHGRect.rect), //CoCo1.20, reduce code size.    
                          aSRCHButton[bButtonIndex].wX0,
                          aSRCHButton[bButtonIndex].wY0,
                          aSRCHButton[bButtonIndex].wX1 + 12,
                          aSRCHButton[bButtonIndex].wY1);
        GDI_FillRoundRect(0, 17, &_OSDSRCHGRect);

        _OSDSRCHStringAttr.wX = aSRCHButton[bButtonIndex].wX0+13;
    }
    else if (SETUP_CUSTOM_UI_STYLE_3 == __SetupInfo.bUIStyle)
    {
        _OSDSRCHGRect.bColor = OSDSRCH_ENTRY_COLOR_ICON_BG;
        _InitRect(&(_OSDSRCHGRect.rect), //CoCo1.20, reduce code size.
                          aSRCHButton[bButtonIndex].wX0,
                          aSRCHButton[bButtonIndex].wY0,
                          aSRCHButton[bButtonIndex].wX1 + 12,
                          aSRCHButton[bButtonIndex].wY1);
        GDI_FillRoundRect(0, 13, &_OSDSRCHGRect);
        _OSDSRCHStringAttr.wX = aSRCHButton[bButtonIndex].wX0+13;
    }
    else if (SETUP_CUSTOM_UI_STYLE_4 == __SetupInfo.bUIStyle)
    {
        _OSDSRCHFrame.bColorBright=  OSDSRCH_ENTRY_COLOR_FRAME_DARK;
        _OSDSRCHFrame.bColorCenter=  OSDSRCH_ENTRY_COLOR_ICON_BG;
        _OSDSRCHFrame.bColorDark=    OSDSRCH_ENTRY_COLOR_FRAME_BRIGHT;
        _OSDSRCHFrame.bStyle=        FRAME_POP_SOLID; 
        _OSDSRCHFrame.bThickness =   FRAME_THICKNESS;

        _OSDSRCH_DrawFrame(aSRCHButton[bButtonIndex].wX0,
                           aSRCHButton[bButtonIndex].wY0,
                           aSRCHButton[bButtonIndex].wX1,
                           aSRCHButton[bButtonIndex].wY1); //CoCo1.20, reduce code size.

        _OSDSRCHStringAttr.wX = aSRCHButton[bButtonIndex].wX0+8;
    }
    _OSDSRCHStringAttr.wY = aSRCHButton[bButtonIndex].wY0 + 3;
#else
    _OSDSRCHStringAttr.wX = aSRCHButton[bButtonIndex].wX0+8;
    _OSDSRCHStringAttr.wY = aSRCHButton[bButtonIndex].wY0;
#endif 

    _OSDSRCHStringAttr.bTextColor =  OSDSRCH_ENTRY_COLOR_ICON_FG;
    _OSDSRCHStringAttr.bBackground = OSDSRCH_ENTRY_COLOR_ICON_BG;
    _OSDSRCHStringAttr.bColorKey =   0;
    _OSDSRCHStringAttr.bShadow1 =    PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDSRCHStringAttr.bShadow2 =    PAL_ENTRY_COLOR_TRANSPARENT; 

    GDI_DrawString_909(0, &_OSDSRCHStringAttr, aSRCHButton[bButtonIndex].wpOSDVIRFONT );
            
} //end of _OSDSRCH_ShowButton()



//  *************************************************************************
//  Function    :   _OSDSRCH_ShowSrchFrame 
//  Description :   Show the search frame 
//  Arguments   :   
//  Return      :   None
//  Side Effect :   None
//  *************************************************************************
static void _OSDSRCH_ShowSrchFrame(void)
{
    _OSDSRCHGRect.bColor = OSDSRCH_ENTRY_COLOR_FRAME_CENTER;

#ifndef SIMPLE_SEARCH_UI
    if (SETUP_CUSTOM_UI_STYLE_1 == __SetupInfo.bUIStyle)
    {
        _OSDSRCH_SetCommonFrameAttr(); //CoCo1.20, reduce code size.
        _OSDSRCH_DrawFrame(SRCH_FRAME_X0, SRCH_FRAME_Y0, //CoCo1.20, reduce code size.
                          SRCH_FRAME_X1, SRCH_FRAME_Y1);
    }
    else if (SETUP_CUSTOM_UI_STYLE_2 == __SetupInfo.bUIStyle)
    {
        _InitRect(&(_OSDSRCHGRect.rect), SRCH_FRAME_X0, SRCH_FRAME_Y0, //CoCo1.20, reduce code size.
                          SRCH_FRAME_X1, SRCH_FRAME_Y1);
        GDI_FillRoundRect(0, 25, &_OSDSRCHGRect);
    }
    else if (SETUP_CUSTOM_UI_STYLE_3 == __SetupInfo.bUIStyle)
    {
        _InitRect(&(_OSDSRCHGRect.rect), SRCH_FRAME_X0, SRCH_FRAME_Y0, //CoCo1.20, reduce code size.
                          SRCH_FRAME_X1, SRCH_FRAME_Y1);
        GDI_FillRoundRect(0, 15, &_OSDSRCHGRect);
    }
    else if (SETUP_CUSTOM_UI_STYLE_4 == __SetupInfo.bUIStyle)
    {
        _OSDSRCH_SetCommonFrameAttr(); //CoCo1.20, reduce code size.
        _OSDSRCH_DrawFrame(SRCH_FRAME_X0, SRCH_FRAME_Y0, //CoCo1.20, reduce code size.
                          SRCH_FRAME_X1, SRCH_FRAME_Y1);
    }
#else
    _InitRect(&(_OSDSRCHGRect.rect), SRCH_FRAME_X0, SRCH_FRAME_Y0,
                                     SRCH_FRAME_X1, SRCH_FRAME_Y1); 
    GDI_FillRoundRect(0, 12, (PPARM_RECT)&_OSDSRCHGRect.rect);
#endif
} //end of _OSDSRCH_ShowSrchFrame()


//  *************************************************************************
//  Function    :   _ShowOneEntryNumber
//  Description :   Show One Entry Number in either SEARCH_TITLE, SEARCH_CHAPTER, or SEARCH_TIME field. 
//  Arguments   :   bSize : Display Size , bNumber: Display Number 
//  Return      :   None
//  Side Effect :
//  *************************************************************************
static void _ShowOneEntryNumber(BYTE bSize, WORD wNumber)
{
    _wOSDSRCHString[0] = bSize; //2 digits  or 3 digits
    _wOSDSRCHString[1] = CHAR_SOLIDUS;                
    
    if ( wNumber <100) 
    {
    	_wOSDSRCHString[bSize-1]   =  aNDNumbers[(wNumber/10)%10];
    	_wOSDSRCHString[bSize]     =  aNDNumbers[wNumber%10];       	
    }
    else if  ( wNumber <1000) 
    {
        _wOSDSRCHString[bSize-2]   =  aNDNumbers[(wNumber/100)%10];
    	_wOSDSRCHString[bSize-1]   =  aNDNumbers[(wNumber/10)%10];
    	_wOSDSRCHString[bSize]     =  aNDNumbers[wNumber%10];       
    }   
/* Alan removes 
    _wOSDSRCHString[bSize+1] = CHAR_SPACE; //to clear 3 digits remaining
    _wOSDSRCHString[bSize+2] = CHAR_SPACE; //to clear 3 digits remaining
    _wOSDSRCHString[0] = bSize + 2 ; //2 digits  or 3 digits
*/
}


//  *************************************************************************
//  Function    :   _ShowOneEntry
//  Description :   Show One Entry in either SEARCH_TITLE, SEARCH_CHAPTER, or SEARCH_TIME field. 
//  Arguments   :   bColor: Center Color & Text BG Color
//  Return      :   None
//  Side Effect :
//  *************************************************************************
static void _ShowOneEntry(BYTE bColor)
{ 
    //Kevin1.00, fix the video hang/audio broken when invoking Search GUI
    //HAL_PreFillBuffer(HAL_FILL_AV_BUFFER, NULL);

    _OSDSRCHStringAttr.bBackground = bColor;
    if (bColor == OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG)
        _OSDSRCHStringAttr.bTextColor =  OSDSRCH_ENTRY_COLOR_WORK_TEXT_FG;
    else
    {
        _OSDSRCHStringAttr.bTextColor =  OSDSRCH_ENTRY_COLOR_STILL_TEXT_FG;
    }

    
    if (__bSearchCursor == SEARCH_CURSOR_TITLE)
    {
	if(__wDiscType & BOOK_DVD)
	{     
#ifndef NO_DISC_MODE //CoCo2.37p
            _ShowOneEntryNumber(2, __bSearchTitle); 
            _OSDSRCHStringAttr.wX = SOLIDUS_T_X - GDI_GetStringWidth(_wOSDSRCHString);
            _OSDSRCHStringAttr.wY = BUTTON_T_Y0;
            _OSDSRCH_DrawString(); //CoCo1.20, reduce code size.
            _ShowOneEntryNumber(3, __bTitle_Ns);            
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        }

        //Kevin0.87b, add to fix "Current track# / Total track# is wrong in GotoTime UI" 
    	else if (__wDiscType & BOOK_CDDA)
        {            
            //alex1.21,20031217use _ShowOneEntryNumber replace double command//
            _ShowOneEntryNumber(2, __wPlayItem);
            _OSDSRCHStringAttr.wX = SOLIDUS_T_X - GDI_GetStringWidth(_wOSDSRCHString);
            _OSDSRCHStringAttr.wY = BUTTON_T_Y0;
            _OSDSRCH_DrawString(); //CoCo1.20, reduce code size.
            _ShowOneEntryNumber(3, __wTrackEnd);
            
        }
	else //BOOK_VCD/BOOK_CVD/BOOK_SVCD 
	{
            //alex1.21,20031217use _ShowOneEntryNumber replace double command//
            _ShowOneEntryNumber(2, __wPlayItem);  
            _OSDSRCHStringAttr.wX = SOLIDUS_T_X - GDI_GetStringWidth(_wOSDSRCHString);
            _OSDSRCHStringAttr.wY = BUTTON_T_Y0;
            _OSDSRCH_DrawString(); //CoCo1.20, reduce code size.
            _ShowOneEntryNumber(3, __wTrackEnd-1);
            
	}                  

        _OSDSRCHStringAttr.bBackground = OSDSRCH_ENTRY_COLOR_FRAME_CENTER;   
        _OSDSRCHStringAttr.bTextColor =  OSDSRCH_ENTRY_COLOR_STILL_TEXT_FG;
        _OSDSRCHStringAttr.wX = SOLIDUS_T_X;
        _OSDSRCHStringAttr.wY = BUTTON_T_Y0;
        _OSDSRCH_DrawString(); //CoCo1.20, reduce code size.
    }   
    else if (__bSearchCursor == SEARCH_CURSOR_CHAPTER)
    {          
	//Damon2.37, fix the problem the range of chapter display exceeds the search UI
      //_OSDSRCHGRect.bColor = OSDSRCH_ENTRY_COLOR_FRAME_CENTER;
      //_InitRect(&(_OSDSRCHGRect.rect), SOLIDUS_C_X - 36, SRCH_FRAME_Y0,  SOLIDUS_C_X + 44, SRCH_FRAME_Y1);
      //GDI_FillRect_909(0, (PPARM_RECT)&_OSDSRCHGRect.rect);
      //Kevin1.00, automatically adjust chapter display to 2 or 3 digits
      if (__wSearchChapter_Ns_Title < 100)
      {
        //alex1.21,20031217use _ShowOneEntryNumber replace double command//
        _ShowOneEntryNumber(2, __wSearchChapter);
        _OSDSRCHStringAttr.wX = SOLIDUS_C_X - GDI_GetStringWidth(_wOSDSRCHString);
        _OSDSRCHStringAttr.wY = BUTTON_C_Y0;
        _OSDSRCH_DrawString(); //CoCo1.20, reduce code size.
        _ShowOneEntryNumber(3, __wSearchChapter_Ns_Title);
      }
      else
      {
        //Kevin1.00, expand to 3 digits 
        _ShowOneEntryNumber(3, __wSearchChapter);
        _wOSDSRCHString[1] = aNDNumbers[__wSearchChapter/100]; //CoCo1.20, reduce code size
        _OSDSRCHStringAttr.wX = SOLIDUS_C_X - GDI_GetStringWidth(_wOSDSRCHString);
        _OSDSRCHStringAttr.wY  = BUTTON_C_Y0;
        _OSDSRCH_DrawString(); //CoCo1.20, reduce code size.
        _ShowOneEntryNumber(4, __wSearchChapter_Ns_Title);
      }
     
      _OSDSRCHStringAttr.bBackground = OSDSRCH_ENTRY_COLOR_FRAME_CENTER;   
      _OSDSRCHStringAttr.bTextColor =  OSDSRCH_ENTRY_COLOR_STILL_TEXT_FG;
      _OSDSRCHStringAttr.wX = SOLIDUS_C_X;
      _OSDSRCHStringAttr.wY  = BUTTON_C_Y0;
      _OSDSRCH_DrawString(); //CoCo1.20, reduce code size.
      
    }  
    else //SEARCH_CURSOR_TIME   
    {       
     
        if (!__bGoToTimeNO) //not set yet
        {   
            for (__bTemp=1;__bTemp<8;__bTemp++)
                _wOSDSRCHString[__bTemp] = CHAR_LOW_LINE;    
        }  
        else
        { 
            _wOSDSRCHString[1] = aNDNumbers[__bTimeHour];
            _wOSDSRCHString[3] = aNDNumbers[__bTimeMin/10];
            _wOSDSRCHString[4] = aNDNumbers[__bTimeMin%10];
            _wOSDSRCHString[6] = aNDNumbers[__bTimeSec/10];
            _wOSDSRCHString[7] = aNDNumbers[__bTimeSec%10];              
        }
        _wOSDSRCHString[0] = 7; //7 digits            
        _wOSDSRCHString[2] = CHAR_COLON;        
        _wOSDSRCHString[5] = CHAR_COLON;        


#ifdef ABSOLUTE_GOTOTIME //Kevin1.07, VCD/CDDA shows track# only for relative gototime only                
        if ( !(__wDiscType & BOOK_DVD ))   
        {   
            _OSDSRCHStringAttr.wX =  BUTTON_EABS_X1+BUTTON_GAP*4;
            _OSDSRCHStringAttr.wY  = BUTTON_EABS_Y0;
            _OSDSRCH_DrawString(); //CoCo1.20, reduce code size.
        }
        else
#endif
        {
            _OSDSRCHStringAttr.wX =  BUTTON_E_X1+BUTTON_GAP*3;
            _OSDSRCHStringAttr.wY  = BUTTON_E_Y0;
            _OSDSRCH_DrawString(); //CoCo1.20, reduce code size.
        }
    }    
    
    //Kevin1.00, fix the video hang/audio broken when invoking Search GUI
    //HAL_PreFillBuffer(HAL_FILL_AV_BUFFER, NULL);

} //end of _ShowOneEntry()

//alex909,20040909
//  *********************************************************************************
//  Function    :   OSDSRCH_ProcessKey
//  Description :   Process Key in CC , to judge which key needs to recieve
//  Arguments   :   bKey    : bKey
//  Return      :   BYTE    : if the key doesn't be belong to this module , remove it.
//  Side Effect :   None
//  *********************************************************************************
BYTE OSDSRCH_ProcessKey(void)
{
    //alex070,20041207 fix the bug #156Search e KEY_Next ==> Program, 
    //Program can not get any KEY. Search->Program, can't receive keys in program mode.
                
    
    if (__bKey==KEY_SEARCH )
    {
    	 //alex072,20041214,fix the bug, It need show the "Invalid Key" when
    	 //Enter Thum/Degist"
         //bKey=_OSDSRCH_SearchKey(__bKey);        
         //return bKey;         
         return _OSDSRCH_SearchKey(__bKey);        
          
    }

    if (__bSearchMode != SEARCH_MODE_OFF)
    {
        switch(__bKey)
        {
            //alex075,20041217,Search Mode can't use Display	
        case    KEY_DISPLAY:            
            	return INVALID_KEY;
        case    KEY_STOP:
        case    KEY_NEXT:
        case    KEY_PREV:
        case    KEY_RESUME:                                
        case    KEY_BOOKMARK:           
        case    KEY_PROGRAM:        
            //alex080,20050110 avoid special vcd error, like JVC
        case    KEY_PBC:
        case    KEY_MENU:
            //alex080,20050108 add some key can by pass
            _OSDSRCH_Exit();            
            //alex080,20041231, according micky's comment , it need to split to "KEY_TODO" , "KEY_BYPASS"                      
            return KEY_BYPASS;                          
            
        case    KEY_UP:       
        case    KEY_DOWN:           
        case    KEY_RIGHT:
        case    KEY_LEFT:          
            return _OSDSRCH_SearchKey(__bKey);        
            //bKey=_OSDSRCH_SearchKey(bKey);        
            //return bKey;                            
        case    KEY_ENTER:
        case    KEY_PLAY:
        case    KEY_PLAY_PAUSE:
            if (__bRepeatAB!=REPEAT_NONE)    //alan091
	    {
                __bRepeatAB = REPEAT_NONE;
                //alex,20040126,fix test report 076,M115
                OSD_OUTPUT_MACRO(MSG_REPEATAB, REPEAT_NONE, 1);
	    }
            //OSDSRCH_Search(MSG_SETSEARCH,OSD_COMPLETE_TIME,0xff);
            _CC_GoTimeAction(GOTIME_CALCULATE);                
#ifndef NO_DISC_MODE //CoCo2.37p            
            if  (__bSearchMode==SEARCH_MODE_TITLE_CHAPTER)
            {
                __bSearchMode=SEARCH_MODE_OFF;
                if (__bSearchTitle==0)
                    __bSearchTitle=1;
                if (__wSearchChapter==0)
                    __wSearchChapter=1;                
                //bKey = KEY_SEARCH_PLAY;
                //alex080,20041231, according micky's comment , it need to split to "KEY_TODO" , "KEY_BYPASS"                      
                _OSDSRCH_Exit();            
                __bKey = KEY_SEARCH_PLAY;
                return KEY_TODO;                          
            }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
            if  (__bSearchMode==SEARCH_MODE_TIME)
            {
                __bSearchMode=SEARCH_MODE_OFF;
                //bKey = KEY_GOTOTIME_PLAY;                
                //alex080,20041231, according micky's comment , it need to split to "KEY_TODO" , "KEY_BYPASS"                      
                _OSDSRCH_Exit();            
                __bKey = KEY_GOTOTIME_PLAY;                
                return KEY_TODO;                          
                
            }
            //OSDSRCH_Exit(OSD_UI_EXIT_ALL);            
            //return KEY_NO_KEY;
        case    KEY_N0:
        case    KEY_N1:
        case    KEY_N2:
        case    KEY_N3:
        case    KEY_N4:
        case    KEY_N5:
        case    KEY_N6:
        case    KEY_N7:
        case    KEY_N8:
        case    KEY_N9:
        case    KEY_N10:
        case    KEY_N11:
        case    KEY_N12:
        case    KEY_N13:
        case    KEY_N14:
        case    KEY_N15:
        case    KEY_N16:
        case    KEY_N17:
        case    KEY_N18:
        case    KEY_N19:
        case    KEY_N20:
            _OSDSRCH_Process_KeyNumber ( (__bKey- KEY_N0) );
            return _OSDSRCH_SearchKey(KEY_NUMBER);    
            //bKey=_OSDSRCH_SearchKey(KEY_NUMBER);    
            //return bKey;                              	
        case    KEY_N10PLUS:
            _OSDSRCH_Process_KeyNumber ( (10 | 0x8000 ) );    // 0x8000 means it's a "PLUS" Key
            return _OSDSRCH_SearchKey(KEY_NUMBER);    
            //bKey=_OSDSRCH_SearchKey(KEY_NUMBER);    
            //return bKey;                                  
        case    KEY_N20PLUS:
            _OSDSRCH_Process_KeyNumber ( (20 | 0x8000 ) );
            return _OSDSRCH_SearchKey(KEY_NUMBER);    
            //bKey=_OSDSRCH_SearchKey(KEY_NUMBER);    
            //return bKey;                              
        case    KEY_CLEAR:
            return _OSDSRCH_SearchKey(__bKey);        
        default:
            return KEY_BYPASS;
            //return bKey;          
            
        }
    }
    else
    {
        //return bKey;
        return KEY_BYPASS;
    }
      

}

//  *********************************************************************************
//  Function    :   _OSDSRCH_ClearUI
//  Description :   Clear OSDSRCH UI
//  Arguments   :   None
//  Return      :   TRUE /FALSE ,
//  Side Effect :   None
//  *********************************************************************************
void _OSDSRCH_ClearUI(void)
{	
    _InitRect(&(_OSDSRCHGRect.rect), SRCH_FRAME_X0, SRCH_FRAME_Y0, SRCH_FRAME_X1, SRCH_FRAME_Y1); //CoCo1.20, reduce code size.
    _OSDSRCHGRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;    
    GDI_FillRect_909(0, (PPARM_RECT)&_OSDSRCHGRect.rect);
	
}

//  *********************************************************************************
//  Function    :   OSDSRCH_Exit
//  Description :   Exit OSDSRCH Control
//  Arguments   :   None
//  Return      :   TRUE /FALSE ,
//  Side Effect :   None
//  *********************************************************************************
void OSDSRCH_Exit(BYTE bExitMode)
{
    __bSearchMode=SEARCH_MODE_OFF;	
    if (bExitMode==OSD_UI_EXIT_ALL)
    {
        _OSDSRCH_ClearUI();
    }
}



//  *************************************************************************
//  Function    :   _OSDSRCH_Exit
//  Description :   Exit OSDSRCH Control
//  Arguments   :   None
//  Return      :   TRUE /FALSE ,
//  Side Effect :   None
//  *************************************************************************
void _OSDSRCH_Exit(void)
{
    __bSearchMode=SEARCH_MODE_OFF;	

    _OSDSRCH_ClearUI();
    OSD_ChangeUI(OSD_UI_SEARCH, OSD_EXIT_UI); //CoCo.076, Notify OSD to exit the UI here.
}



//  *********************************************************************************
//  Function    :   OSDSRCH_Entry
//  Description :   Enter OSDSRCH UI
//  Arguments   :   bSearchMode , 
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************************
void  OSDSRCH_Entry(BYTE bSearchMode)
{

    
    __bSearchMode = bSearchMode;       
    
    if(__bSearchMode == SEARCH_MODE_TIME)
    {        
        if ( !(__wDiscType & BOOK_DVD ))   
        {                   
            _OSDSRCH_Entry_Initial();            
            _OSDSRCH_Entry_PureTime();       
        }
        else
        {
#ifndef NO_DISC_MODE //CoCo2.37p
            _OSDSRCH_Entry_TitleTime();
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        }
    } 
#ifndef NO_DISC_MODE //CoCo2.37p
    else if(__bSearchMode == SEARCH_MODE_TITLE_CHAPTER)
    {
        _OSDSRCH_Entry_Initial(); 
        _OSDSRCH_Entry_TitleChapter();   
    }    
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
}


//  *********************************************************************************
//  Function    :   OSDSRCH_ShowUI
//  Description :   Show Information in UI
//  Arguments   :   bSearchMode , 
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************************
void  OSDSRCH_ShowUI(BYTE bSeachMode)
{
        _OSDSRCH_InitializeGDI();
        __bSearchMode=bSeachMode;  
#ifndef NO_DISC_MODE //CoCo2.37p    
        if(__bSearchMode==SEARCH_MODE_TITLE_CHAPTER)
	{	    
            //alex1.24,20040301,use ShowTwoEntry to reduce source code 
            _OSDSRCH_ShowTwoEntry(OSDSRCH_ENTRY_COLOR_FRAME_CENTER, OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG);		        
	}
	else if(__bSearchMode==SEARCH_MODE_TIME)
#else
    if(__bSearchMode==SEARCH_MODE_TIME)
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        {
	    __bSearchCursor = SEARCH_CURSOR_TIME;             
            _ShowOneEntry(OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG); 
        }    		
}


//  *********************************************************************************
//  Function    :   _OSDSRCH_SearchKey
//  Description :   Process Key in CC , to judge which key needs to recieve
//  Arguments   :   bKey    : bKey
//  Return      :   BYTE    : if the key doesn't be belong to this module , remove it.
//  Side Effect :   None
//  *********************************************************************************
BYTE _OSDSRCH_SearchKey(BYTE bKey)
{
    				
    DWORD   dwTemp;
    switch(bKey)
    {
        case    KEY_SEARCH: 
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
            if (__btInputProgram)
            {
                return KEY_INVALID;
            }
#endif
            //alex070,20041205,fix the bug #85 for cdrom file manager show search  
            // JPEG slide show can enter Search UI and Search UI shows the "time" search mode.
            //CDROM_M1                   0x0100
            //DROM_XA                    0x0200
	    //DROM_PICTURECD             0x0400  
	    //BOOK_M1                    0x0102  // BOOK_CDROM | CDROM_M1
	    //BOOK_XA                    0x0302  // BOOK_CDROM | CDROM_M1 | CDROM_XA
	    //BOOK_PICTURECD             0x0702  // BOOK_CDROM | CDROM_M1 | CDROM_XA | CDROM_PICTURECD. 
        if ( (__bAttrPlay <= ATTR_AV_END && __bAttrPlay >= ATTR_AV_BEGIN) &&   (__btPlaying || __btPause)  )
	    {
            // LLY0.96, don't support GoTime action while non-exist DivX index table
            // LLY1.01, don't support GoTime action while audio only mode (video resolution > 720*576)            
            // LLY1.02, enable GoTime action for DivX w/ skip video stream case
            if( (__bAttrPlay == ATTR_AVI) && (__bNoDivXIndexTbl) )//|| __bVideoID==HAL_THROW_VIDEO) )            
            {
                return (INVALID_KEY);
            }

            // LLY2.22, Dis-allow time search while CDROM/DVDROM title w/ file total time is zero.
            // Otherwise, it can't calculate the correct postion
            if( (__bAttrPlay & TYPE_CDROM_AV) && (!__dwFileTime) )
            {
                return (INVALID_KEY);
            }

            /*__dwTemp = PARSER_QueryContainerAudioType(__dwSFStreamID, __bASTNO);
            if ((__bAttrPlay == ATTR_AVI) && ((__dwTemp == PARSER_ESFMT_WMA_V1) || (__dwTemp == PARSER_ESFMT_WMA_V2)))
                return (INVALID_KEY);*/
	    }
            else if ((__wDiscType & BOOK_PICTURECD) || (__wDiscType == BOOK_CDDA && __bCDDA_DTS == CDDA_CDG)) //CoCo0.76, Please Alex don't use 0x700 directly. It's dangerous.
            {
                return INVALID_KEY;            	
            }           
            
            else if ( 
                //alex080,20050110 only pbc off ,can use search..
                __btPBC            
            	//alex070,20041205 fix the bug #106 for Thumbnail UI can enter Search UI
                //||
                ||  __bThumbnailMode                
                //alex070,20041205 fix the bug #90 for MM UI can enter Search UI
                ||( __bNavigateMode ==  MEDIA_NAVIGATE_MODE)         
// Micky1.20, 4M flash
#ifndef NO_DIGEST
                || __btDIGEST 
#endif
               )
            {
                //__bSearchMode=SEARCH_MODE_OFF;//XULI0106                
                //OSDSRCH_Search(MSG_SETSEARCH,OSD_COMPLETE_TIME,0xff);
                //OSDSRCH_Exit(OSD_UI_EXIT_ALL);
                //OSD_ChangeUI(OSD_UI_SEARCH, OSD_EXIT_UI);                
                return INVALID_KEY;
            }            
                       
            __bSearchMode++;  // toggle switch for search mode          
            
            if(__bSearchMode > SEARCH_MODE_END)
	    {
                __bSearchMode=SEARCH_MODE_BEGIN;
	    }
            // nonDVD only have SEARCH_MODE_TIME
#ifndef NO_DISC_MODE //CoCo2.37p    
	    if ((!(__wDiscType&BOOK_DVD)&&(__bSearchMode==SEARCH_MODE_TITLE_CHAPTER)))//XULI0106
#endif //#ifndef NO_DISC_MODE //CoCo2.37p    
	    {
		    __bSearchMode+=1;//XULI0106
	    }	    

            switch(__bSearchMode)
            {
                case    SEARCH_MODE_OFF:  // leave search mode                    
                    //OSDSRCH_Search(MSG_SETSEARCH,OSD_COMPLETE_TIME,0xff);
                    _OSDSRCH_Exit();                                        
                    break;
#ifndef NO_DISC_MODE //CoCo2.37p
                case    SEARCH_MODE_TITLE_CHAPTER:  // enter search mode
    
                    //DVD_051Micky, Number >10
                    //Use this variable to record the input value and number of digits
                    __wNumberQueue=0;

                    //Kevin1.23DMTech, default serach cursor is on Chapter or Title  
                    __bSearchCursor=SEARCH_CURSOR_TITLE;
                    //__bSearchCursor=SEARCH_CURSOR_CHAPTER;

                    // LLY.037, initialize the search item value
                    __bSearchTitle=__bTitle;
                    __wSearchChapter=__wChapter;
                    //alex,2005012
                    __wSearchChapter_Ns_Title=__wChapter_Ns_Title;

                   
                    // don't allow Title/PTT play and PTT_Search
                    if ( !NV_QueryUOP (UOP_CAN_TITLE_PLAY) &&
                         !NV_QueryUOP (UOP_CAN_PTT_SEARCH)
                    )
                    {
                        //__bSearchMode=SEARCH_MODE_OFF;                   ;
                        //OSDSRCH_Search(MSG_SETSEARCH,OSD_COMPLETE_TIME,0xff);
//                        _OSDSRCH_Exit();
                        OSDSRCH_Exit(OSD_UI_EXIT_CONTROL_ONLY);
                        //OSD_ChangeUI(OSD_UI_SEARCH, OSD_EXIT_UI); //--CoCo.076, Notify OSD to exit the UI in OSDSRCH_Exit().
                        return INVALID_KEY;
                    }

                    // check if Title selection not allowed
                    if ( !NV_QueryUOP (UOP_CAN_TITLE_PLAY) )
                    {
                        // can only choose chapter
                        __bSearchCursor=SEARCH_CURSOR_CHAPTER;
                    }

                    // check if chapter selection not allowed
                    if ( !NV_QueryUOP (UOP_CAN_PTT_SEARCH) )
                    {
                        // can only choose title
                        __bSearchCursor=SEARCH_CURSOR_TITLE;
                    }
                
                
                    //OSDSRCH_Search(MSG_SETSEARCH,OSD_INPUT_TITLE_CHAPTER,0xff);
                    OSDSRCH_Entry(__bSearchMode);
	            break;	            
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
                
            case    SEARCH_MODE_TIME:	
#ifndef NO_DISC_MODE //CoCo2.37p    
            	if ( __wDiscType & BOOK_DVD )
            	{
                    __bUOP_Parameter = __bTitle;
			 // Damon2.37, when search mode change from TITLE_CHAPTER to TIME, the value of tiltle need to reset
			 __bSearchTitle=__bTitle;
                    if ( ! NV_QueryUOP (UOP_CAN_TIME_SEARCH) )
                    {
                        _OSDSRCH_Exit();                        
#ifdef  GOTOTIME_SHOW_INVALID
                        return INVALID_KEY;
#else
                        return KEY_NO_KEY;
#endif
                    }
                }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p    

                if ( !LINEAR_Action (KEY_GOTOTIME, NULL_0) )
           	{
    			//__bSearchMode=SEARCH_MODE_OFF;//XULI0106                
                //OSDSRCH_Search(MSG_SETSEARCH,OSD_COMPLETE_TIME,0xff);
                    _OSDSRCH_Exit();
                    //OSD_ChangeUI(OSD_UI_SEARCH, OSD_EXIT_UI); //--CoCo.076, Notify OSD to exit the UI in OSDSRCH_Exit().
    			return INVALID_KEY;
           	}
		break;

        } //switch(__bSearchMode)

        // Micky1.07a, fix RepeatAB-> GoTo TT/CC, won't clear repeat AB.
        // Let GoTo TT/CC, Time <-> RepeatAB be exclusive
        // can't call UTL_ModeInitial, as it will clear __bSearchMode.
        if(__bSearchMode!=SEARCH_MODE_OFF)
	{
            // Micky1.24a, LINEAR_Action(KEY_GOTO_TIME) will remove UTL_ModeInitial
            // so need to celar necessary mode here.
            // This is a temporary solution, wait for UTL_ModeInitial review.
            __btRandom= FALSE;
            __btIntroMode=FALSE;
#ifdef SUPPORT_BOOKMARK //--CoCo1.20, reduce code size.
            __btBookMark= FALSE;
#endif
	    // wyc.pro when execute gototime, must clear program mode information
	    LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
	        // LLY.037 end
	    if(__bSearchMode==SEARCH_MODE_TIME)
	    {                
                //OSDSRCH_Search(MSG_SETSEARCH,OSD_INPUT_TIME,0xff);
                OSDSRCH_Entry(__bSearchMode);
	    }
        }
	return KEY_NO_KEY;
        //break;
    case    KEY_UP:
#ifndef NO_DISC_MODE //CoCo2.37p    
        if( __bSearchMode==SEARCH_MODE_TITLE_CHAPTER )    
        {
            if( __bSearchCursor==SEARCH_CURSOR_TITLE )
            {
                if (__bSearchTitle<__bTitle_Ns)
                {
                    __bSearchTitle++;
                }    
		else 
		{
		    __bSearchTitle=1;
		}    
                DVD_ParseInfo ( DVD_TOTAL_CHAPTERS, & dwTemp, __bSearchTitle );
                __wSearchChapter_Ns_Title= (WORD) dwTemp;
                __wSearchChapter=0x1;					
            }
            else if ( __bSearchCursor==SEARCH_CURSOR_CHAPTER )   
            {
                if (__wSearchChapter<__wSearchChapter_Ns_Title)
                {
                    __wSearchChapter++;            
                }    
                else 
                {
		    __wSearchChapter=1;
                }   
            }       			
            //OSDSRCH_Search(MSG_SEARCH,OSD_INPUT_TITLE_CHAPTER,0xff);
            OSDSRCH_ShowUI(__bSearchMode);
            return  KEY_NO_KEY;
        }
        else 
#endif //#ifndef NO_DISC_MODE //CoCo2.37p    
        {
            return  INVALID_KEY;    
        }    
        break;
    case    KEY_DOWN:
#ifndef NO_DISC_MODE //CoCo2.37p    
        if( __bSearchMode==SEARCH_MODE_TITLE_CHAPTER )    
        {
            if( __bSearchCursor==SEARCH_CURSOR_TITLE )
            {
                if (__bSearchTitle>1)
                {
                    __bSearchTitle--;
                }
		else
		{
		    __bSearchTitle=__bTitle_Ns;
		}    
                DVD_ParseInfo ( DVD_TOTAL_CHAPTERS, & dwTemp, __bSearchTitle );
                __wSearchChapter_Ns_Title= (WORD) dwTemp;
                __wSearchChapter=0x1;                					
            }
            else if ( __bSearchCursor==SEARCH_CURSOR_CHAPTER )   
            {
                if (__wSearchChapter>1)
                {
                    __wSearchChapter--; 
                }    
		else
		{
		    __wSearchChapter=__wSearchChapter_Ns_Title;
		}			
            }                			
            //OSDSRCH_Search(MSG_SEARCH,OSD_INPUT_TITLE_CHAPTER,0xff);
            OSDSRCH_ShowUI(__bSearchMode);
            return  KEY_NO_KEY;
        }
        else 
#endif //#ifndef NO_DISC_MODE //CoCo2.37p    
        {
            return  INVALID_KEY;    
        }      

    case    KEY_RIGHT:
    case    KEY_LEFT:
//DVD_051Micky, Number >10
        // Use this variable to record the input value and number of digits
        // must reset the value when change field
        __wNumberQueue=0;

        __dwTimeKey=NULL_0;  // must clear N10PLUS key reference while change cursor
        // move the cursor depending on right/left
        // LLY.037, let cursor can move right/left anytime
        if(bKey==KEY_RIGHT)
        {
#ifndef NO_DISC_MODE //CoCo2.37p    
            if( (__bSearchCursor==SEARCH_CURSOR_TITLE) &&
                (__bSearchMode==SEARCH_MODE_TIME) )
                __bSearchCursor+=2;
            else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p    
                __bSearchCursor++;
        }
        else
        {

            if( (__bSearchCursor==SEARCH_CURSOR_TIME) &&  // LLY.039
                (__bSearchMode==SEARCH_MODE_TIME) )
                __bSearchCursor-=2;
            else
                __bSearchCursor--;
        }
        // LLY.037 end

#ifndef NO_DISC_MODE //CoCo2.37p    
        // DVD_099Micky, add GOTOTIME UOP check
        // check if Title selection not allowed
        if ( !NV_QueryUOP (UOP_CAN_TITLE_PLAY) )
        {
            // can only choose chapter
            __bSearchCursor=SEARCH_CURSOR_CHAPTER;
        }

        // check if chapter selection not allowed
        if ( !NV_QueryUOP (UOP_CAN_PTT_SEARCH) )
        {
            // can only choose title
            __bSearchCursor=SEARCH_CURSOR_TITLE;
        }

        // DVD_099Micky, add 0 protection
        if (__bSearchTitle==0)
            __bSearchTitle=1;
        if (__wSearchChapter==0)
            __wSearchChapter=1;

        // depending on search mode, to toggle the search cursor while out of range
        if(__bSearchMode==SEARCH_MODE_TITLE_CHAPTER)
        {
            if(__bSearchCursor>SEARCH_CURSOR1_END)
                __bSearchCursor=SEARCH_CURSOR_BEGIN;
            else if(__bSearchCursor<SEARCH_CURSOR_BEGIN)
                __bSearchCursor=SEARCH_CURSOR1_END;
            //OSDSRCH_Search(MSG_SEARCH,OSD_INPUT_TITLE_CHAPTER,0xff);
            //OSDSRCH_ShowUI(SEARCH_MODE_TITLE_CHAPTER);
        }
        else if(__bSearchMode==SEARCH_MODE_TIME)
#else
        if(__bSearchMode==SEARCH_MODE_TIME)
#endif //#ifndef NO_DISC_MODE //CoCo2.37p    
        {
            if(__bSearchCursor>SEARCH_CURSOR2_END)
                __bSearchCursor=SEARCH_CURSOR_BEGIN;
            else if(__bSearchCursor<SEARCH_CURSOR_BEGIN)
                __bSearchCursor=SEARCH_CURSOR2_END;
            //OSDSRCH_Search(MSG_SEARCH,OSD_INPUT_TITLE_CHAPTER,0xff);
            //OSDSRCH_ShowUI(SEARCH_MODE_TITLE_CHAPTER);
        }
        OSDSRCH_ShowUI(__bSearchMode);
        //OSD_OUTPUT_MACRO(MSG_SEARCH, NULL, 0xFF);  // LLY.050        
        return  KEY_NO_KEY;;
        
    case    KEY_NUMBER: 
/*    
     	if ( (__wDiscType & BOOK_DVD) )
        {
            // DVD_099bMicky, must give title parameter for UOP query
            __bUOP_Parameter = __bTitle;
            // must check Time Search
            // Alex1.24, don't record bookmark time when non-title domain
            if ( ! NV_QueryUOP (UOP_CAN_TIME_SEARCH) || (__bDomain !=TT_DOM) )
            {        
                return INVALID_KEY;                            
            }
        }// Alex1.24, don't record bookmark time when segment play item
        else if (__wPlayItem > 999 )    // segment playitem
        {
            return INVALID_KEY;            
        }    
*/        
      	OSDSRCH_ShowUI(__bSearchMode);            
        return  KEY_NO_KEY;;
    case KEY_CLEAR:
        __dwNumberPrevious = 0;
#ifndef NO_DISC_MODE //CoCo2.37p
        if (__bSearchCursor == SEARCH_CURSOR_TITLE)
        {
            if(__wDiscType & BOOK_DVD)
            {
                __bSearchTitle = __bTitle;
            }
            else if (__wDiscType & BOOK_CDDA)
            {
            }
            else  //BOOK_VCD/BOOK_CVD/BOOK_SVCD
            {
            }
        }
        else if (__bSearchCursor == SEARCH_CURSOR_CHAPTER)
        {
            __wSearchChapter = __wChapter;
        }
        else //SEARCH_CURSOR_TIME
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        {
            __bTimeHour = 0;
            __bTimeMin = 0;
            __bTimeSec = 0;
            __bGoToTimeNO = 0;
        }
        _ShowOneEntry(OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG);
        return  KEY_NO_KEY;;
    default:
        return  INVALID_KEY; 
    }






}



// LLY.036 begin, one function to process search mode action
//  *****************************************************************
//  Function    :   _OSDSRCH_ProcessSearchAction
//  Description :   Process search mode action
//  Argument    :
//  Return      :   TRUE, the action has processed and is valid
//                  FALSE, the action is invalid
//  Side Effect :
//  *****************************************************************
BYTE OSDSRCH_ProcessSearchAction(BYTE bKey, WORD wParam)
{
    DWORD   dwTemp;
    switch(bKey)
    {

    
    case    KEY_NUMBER:
        // wyc1.10, add a independent define to control the on/off of CDROM titles number confirm key
#ifndef  SUPPORT_NUMBER_CONFIRM_KEY
#ifdef    SUPPORT_CDROM_NUMBER_CONFIRM_KEY
        if (__wDiscType & BOOK_M1)
#else
        if (0)
#endif // 
#endif //
//DVD_051Micky, Number >10
        {
            // the 1st digit input
            // bit 15=0, means this input number is the first number
            if ( !(__wNumberQueue & 0x8000) )
            {
                // save the first value
                __wNumberQueue = wParam;
                __wNumberQueue |=0x8000;
            }
            else
            {
            //DVD_099Micky, clear the flag later, as still need check
            /*
                // clear the flagO
                __wNumberQueue &=0x7fff;
                // second number, must calculate 1st & 2nd number as one value
                wParam += __wNumberQueue*10;
            */
                __wNumberQueue |=0x4000;    // set the second digit flag
                // second number, must calculate 1st & 2nd number as one value
                wParam += (__wNumberQueue&0x3fff)*10;
            }
            // DVD_099Micky, don't do protection here, must check final data.
            // os it will cause OSD won't display the value in time
            // protect, Title/Chapter can't be 0
            if (__wNumberQueue &0x4000)    // second value has been input
            {
                __wNumberQueue &=0x3fff;
                if (wParam==0)  // 0 can be final digit only
                    wParam=1;
            }
        }

        switch(__bSearchCursor)
        {
#ifndef NO_DISC_MODE //CoCo2.37p
        case    SEARCH_CURSOR_TITLE:
            __bSearchTitle=(BYTE)wParam;
            if(__bSearchTitle > __bTitle_Ns)
                __bSearchTitle=__bTitle_Ns;  // set default value as max title
			//Kevin0.86, add 
            if(!__bSearchTitle)
                __bSearchTitle=1;
            DVD_ParseInfo ( DVD_TOTAL_CHAPTERS, & dwTemp, __bSearchTitle );
            __wSearchChapter_Ns_Title= (WORD) dwTemp;
            __wSearchChapter=0x1;
            break;

        case    SEARCH_CURSOR_CHAPTER:
            __wSearchChapter=(BYTE)wParam;
            if(__wSearchChapter > __wSearchChapter_Ns_Title)
                __wSearchChapter=__wSearchChapter_Ns_Title;  // set defai;t va;ie as max chapter
			//Kevin0.86, add 
            if(!__wSearchChapter)
                __wSearchChapter=1;
            break;

#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        case    SEARCH_CURSOR_TIME:
            break;
        }
        //OSD_OUTPUT_MACRO(MSG_SEARCH, NULL, 0xFF); // LLY.050
        OSDSRCH_ShowUI(__bSearchMode);
        return TRUE;
    default:
        return  FALSE;
    }
}


// DVD_160Micky, re-arrange the whole function
//  *********************************************************************
//  Function    :   _OSDSRCH_ConfirmAction
//  Description :   Process the input number
//  Arguments   :   wValue, the input numver value
//              :   must be a decided value, i.e. no time key.
//  Return      :   none.
//  Side Effect :
//  *********************************************************************
void _OSDSRCH_ConfirmAction(WORD wValue)
{
// DVD_160Micky, number 0 is invalid under some case.
// [NUMBER] CHECK NUMBER 0's validity
    if (! wValue )  // input value is 0
    {
#ifndef    KEY_N0_SAME_NPLUS    // at this case, 0 still means 0.
        // 0 is valid in GOTOTIME
        // NUM_N0 is valid in inputprogram mode in new program mode
//alex1.23,20040218 combine gototime , search
//        if (__btGoToTime || __btInputProgram
        if ( (__bSearchMode == SEARCH_MODE_TIME) || __btInputProgram
#ifdef  SUPPORT_NUMBER_CONFIRM_KEY
        // allow 0 as input in tt/cc serach mode
        // DVD_160.aMicky, fix can't input 10/20/N0 in TT/CC search mode
        || (__bSearchMode == SEARCH_MODE_TITLE_CHAPTER)
#endif
          )
        {
            // gototime/programinput allow 0 as input
        }
        else
        // other modes don't allow 0 as input.
        {
            // menu-> 0x is processed in _CC_Process_KeyNumber (0) first
            OSD_Output(MSG_INVALID, OSD_ERR_NUMBER, 1 );
            return;
        }
#else       // at this case, 0 means 10 or 20 or x0.
        // DVD_276Micky, don't allow 10+ in __bSearchMode.
//        if ( !__btGoToTime && !__btInputProgram )    // "0" assuem KEY_NPLUS
//alex1.23,20040218 combine gototime , search
//        if ( !__btGoToTime && !__btInputProgram && !__bSearchMode)    // "0" assuem KEY_NPLUS
        if ( !__btInputProgram && (__bSearchMode == SEARCH_MODE_OFF) )    // "0" assuem KEY_NPLUS
        {
            __dwNumberPrevious= KEY_NPLUS;
            wValue = KEY_NPLUS;
        }
#endif
    }

// [NUMBER] GOTOTIME/PROGRAM
    // DVD_160Micky, make number input mode has top priority
    // fix VCD2.0 menu-> Program-> input number
    // for DVD/VCD/MP3/CD-DA.. are all same.
//alex1.23,20040218 combine gototime , search
//    if ( __btGoToTime || __btInputProgram )
    if ( (__bSearchMode==SEARCH_MODE_TIME)|| __btInputProgram )
    {
        if ( LINEAR_Action(KEY_NUMBER, wValue) )
            return;
    }
// [NUMBER] SEARCH TT/CC
    else
//alex1.23,20040218 combine gototime , search
//    if(__bSearchMode)  // process number key in search mode
    if(__bSearchMode==SEARCH_MODE_TITLE_CHAPTER)  // process number key in search mode
    {
        //alex0909,use osdsrch_ProcessSearchAction to replace _CC_ProcessSearchAction
        if( OSDSRCH_ProcessSearchAction(KEY_NUMBER, wValue)== KEY_NO_KEY )
            return;
    }
// [NUMBER] DIGEST
// Micky1.20, 4M flash
#ifndef NO_DIGEST
    else
    if ( __btDIGEST )
    {
        if ( DIGEST_Number(LOBYTE(wValue)) )
            return;
    }
#endif
#ifndef NO_DISC_MODE //CoCo2.37p
    else
// [NUMBER] DVD BUTTON number
    if ( __wDiscType & BOOK_DVD )
    {
        OSD_OUTPUT_MACRO ( MSG_NUMBER, wValue, 1 );
// Micky2.78b-2, Support using number key to select chapter play.
// Only when in title domain and and no selective buttons.
// only need check no selective button case.
// (NF-200, Dinosaur has the case that TT_DOM, but have selective buttons)
#ifdef  SUPPORT_PLAY_CHAPTER_BY_NUMBER
        if ( !NV_Action(ACTION_NUMBER_CHECK, NULL_0) )
        {
            // input number is out of chapter range.
            if ( wValue > __wChapter_Ns_Title )
            {
                OSD_Output(MSG_INVALID, OSD_ERR_NUMBER, 1 );
                return;
            }
            //Kevin1.24a, fix "program mode is not cleared when inputting number to select chapter"
            if (__btPlayingProgram)
                LINEAR_Program(LINEAR_PROGRAM_CLEAR, NULL_0);

            NV_TitlePlay(__bTitle, wValue);
            HAL_Reset(HAL_RESET_AUDIO);
            HAL_Reset(HAL_RESET_SPST);
            HAL_Reset(HAL_RESET_VIDEO);
            // DVD_103Micky, fix GoToTime will hang issue
            // need restore the play command
            UTL_TransferCommandID(__bModeCmd);

            //HAL_Display (HAL_DISPLAY_ONE_FIELD, 0);
            return;
        }
        else
#endif
        if ( NV_Action ( ACTION_NUMBER, wValue) )
            return;
    }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
    else
    {
    // In PBC  Stop->Key number
    // will directly play the track number in linear mode
#ifdef  AUTO_PBCOFF
#ifndef NO_DISC_MODE //++CoCo2.37p
        if ( __btPBC && (__bModeCmd==KEY_STOP) )
        {
            // directly play the number if in PBC stop mode
            // just turn PBC off
            __btPBC=FALSE;
            //OSD_OUTPUT_MACRO(MSG_PBC, FALSE, 0);
        }
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
#endif  //#ifdef  AUTO_PBCOFF

#ifndef NO_DISC_MODE //++CoCo2.37p
        if (__btPBC)
        {
            // show the input number
            OSD_OUTPUT_MACRO ( MSG_NUMBER, wValue, 1 );
            if ( __wStatus != ID_ENDLIST )
            {
                VCD20_KeyNumber ( wValue );
                return;
            }
        }
        else
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
        {
            if ( LINEAR_Action(KEY_NUMBER, wValue) )    //__dwNumberPrevious) )
                return;
        }
    }

    // Only invalid number will go here
    //OSD_Output(MSG_INVALID, OSD_ERR_KEY, 1 );
}


//***************************************************************************
//  Function    :   _OSDSRCH_InitializeGDI
//  Abstract    :   This function will initialize GDI-relative attributes like palette.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDSRCH_InitializeGDI(void)
{
    if (SETUP_CUSTOM_UI_STYLE_1 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_FRAME_CENTER,
                           OSDSRCH_COLOR_CENTER1,  TRUE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_ICON_FG,
                           OSDSRCH_VALUE_COLOR_ICON_FG1,       FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_ICON_BG,
                           OSDSRCH_VALUE_COLOR_ICON_BG1,       FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG,
                           OSDSRCH_VALUE_COLOR_WORK_TEXT_BG1,  FALSE);
    }
    else if (SETUP_CUSTOM_UI_STYLE_2 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_FRAME_CENTER,
                           OSDSRCH_COLOR_CENTER2,  FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_ICON_FG,
                           OSDSRCH_VALUE_COLOR_ICON_FG2,       FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_ICON_BG,
                           OSDSRCH_VALUE_COLOR_ICON_BG2,       FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG,
                           OSDSRCH_VALUE_COLOR_WORK_TEXT_BG2,  FALSE);
    }
    else if (SETUP_CUSTOM_UI_STYLE_3 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_FRAME_CENTER,
                           OSDSRCH_COLOR_CENTER3,  FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_ICON_FG,
                           OSDSRCH_VALUE_COLOR_ICON_FG3,       FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_ICON_BG,
                           OSDSRCH_VALUE_COLOR_ICON_BG3,       FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG,
                           OSDSRCH_VALUE_COLOR_WORK_TEXT_BG3,  FALSE);
    }
    else if (SETUP_CUSTOM_UI_STYLE_4 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_FRAME_CENTER,
                           OSDSRCH_COLOR_CENTER4,  FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_ICON_FG,
                           OSDSRCH_VALUE_COLOR_ICON_FG4,       FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_ICON_BG,
                           OSDSRCH_VALUE_COLOR_ICON_BG4,       FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG,
                           OSDSRCH_VALUE_COLOR_WORK_TEXT_BG4,  FALSE);
    }

#ifndef SIMPLE_SEARCH_UI
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_FRAME_BRIGHT,
                           OSDSRCH_VALUE_COLOR_FRAME_BRIGHT,  FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_FRAME_DARK,
                           OSDSRCH_VALUE_COLOR_FRAME_DARK,    FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_STILL_TEXT_FG,
                           OSDSRCH_VALUE_COLOR_STILL_TEXT_FG, FALSE);
        GDI_ChangePALEntry(OSDSRCH_ENTRY_COLOR_WORK_TEXT_FG,
                           OSDSRCH_VALUE_COLOR_WORK_TEXT_FG,  FALSE);
#endif 


    GDI_WaitPaletteComplete();
    GDI_ActivateRegion(0); //open GDI
}




//  *********************************************************************
//  Function    :   _OSDSRCH_Process_KeyNumber
//  Description :   Process input number
//  Arguments   :   bValue, Number Key (Numerical)
//                  if ( bVlaue & 0x80 )  means it's 10PLUS or ..
//  Return      :   NONE
//  Side Effect :   __bISRKey may changed
//  *********************************************************************
void _OSDSRCH_Process_KeyNumber ( WORD wValue )
{
    // Micky2.81, don't give initial value while declaration.
    BYTE    bRealNumber;
    bRealNumber=0;

    // Number input was allowed when:
    // [AVSYS]
    //  - Tuner/ Program Channel.(Others will be invalidated by AVSYS_ProcessKey)
    // [DVD]
    // 1. Program input
    // 2. GoToTime input
    // 3. TT/CC search
    // 4. Digest mode 
    // 5. Has selectable buttons
    // 6. No selectable buttons, but support play chapter by number
    // [Other titles]
    //  - Always.

#ifndef NO_DISC_MODE //CoCo2.37p
    if (__wDiscType & BOOK_DVD)
    {
         if (
// Micky1.20, 4M flash
#ifndef NO_DIGEST
             __btDIGEST || 
#endif
//alex1.23,20040218 combine gototime , search
//            __btInputProgram || __btGoToTime || __bSearchMode || __btBookMark)
            __btInputProgram || (__bSearchMode != SEARCH_MODE_OFF) 
#ifdef SUPPORT_BOOKMARK //--CoCo1.20, reduce code size.
            || __btBookMark
#endif
            )
         {
            // Always allow KEY_Nx
         }
         // check if can allow number selection
         else if ( !NV_Action(ACTION_NUMBER_CHECK, 0) )
         {
            // don't allow KEY_Nx when no selectable button.
            // except support play chapter by number
// Micky2.78b-2, Support using number key to select chapter play.
// Only when in title domain and and no selective buttons.
// (NF-200, Dinosaur has the case that TT_DOM, but have selective buttons)
#ifdef  SUPPORT_PLAY_CHAPTER_BY_NUMBER
            if ( ( __bDomain & TT_DOM ) )
            {
            }
            else
#endif  //#ifdef  SUPPORT_PLAY_CHAPTER_BY_NUMBER
            {
                OSD_OUTPUT_MACRO(MSG_INVALID, OSD_ERR_KEY, 0x1);
                return ;
            }
         }

    }   // end if (__wDiscType & BOOK_DVD)

#endif //#ifndef NO_DISC_MODE //CoCo2.37p

    // Micky2.81, support different number of digits.
    // must give proper value for different title/mode.
    // VCD/DVD, only allow 2 digits
    // CDROM title ??
    // AVSYS, 4 digits
    if (__wDiscType & CDROM_M1)
        __wMaxNumber = FM_MAX_NUMBER;
    else
    //Micky1.00, fix "The Perfect Storm"(Region 1) can't select > 100 chapter
    // the Title 6 has 249 chapters.
    // need allow the input number to be 999.
    {
        if ( __bSearchMode )
            __wMaxNumber = 999;
        else
        __wMaxNumber = 90;
    }

    // below will do input number value calculation
    // KEY_10+ is not allowed when:
    // 1. Program input
    // 2. GoToTime input
    if (wValue & 0x8000)  // a time key
    {
        // no time key for GOTOTIME
        // wyc.101, not allow 10+ in program input mode
//alex1.23,20040218 combine gototime , search
//        if (( __btGoToTime ) || (__btInputProgram))
        if (( __bSearchMode==SEARCH_MODE_TIME) || (__btInputProgram))
        {   // invalid
            OSD_Output(MSG_INVALID,  OSD_ERR_KEY, 1 );
            return;
        }

        // the actual value is in lower 7 bits
        wValue &= 0x7FFF;
        if (__dwTimeKey)    // second time key
        {
            __dwNumberPrevious +=(DWORD)wValue;     // really number
            // check if the number out of max. digits.
            // Micky2.81, support different number of digits.
            // when over the max. number, will reset as 0
            if ( __dwNumberPrevious > (DWORD)__wMaxNumber )
                __dwNumberPrevious = 0;
        }
        else                // first time key
        {
            
            //alex fix the bug plus +10
            //__dwNumberPrevious += (DWORD)wValue;
            __dwNumberPrevious = (DWORD)wValue;
        }
        // record the time that KEY_10+ was pressed.
        __dwTimeKey= OS_GetSysTimer();
    }
    else                // key value is decided
    {
        if (__dwTimeKey)    // key value is decided
        {
            __dwNumberPrevious+= wValue; // increase number
        }
        else
           //alex fix the bug plus +10
           //__dwNumberPrevious+= wValue;
            __dwNumberPrevious= wValue;
        bRealNumber=TRUE;

    // wyc1.10, add a independent define to control the on/off of CDROM titles number confirm key
#ifndef  SUPPORT_NUMBER_CONFIRM_KEY
#ifdef    SUPPORT_CDROM_NUMBER_CONFIRM_KEY
    if (__wDiscType & BOOK_M1)
#else
    if (0)
#endif
#endif  //#ifndef  SUPPORT_NUMBER_CONFIRM_KEY
    {
//alex1.23,20040218 combine gototime , search
//        if (!__btInputProgram && !__btGoToTime && !__bSearchMode)
        if (!__btInputProgram && (__bSearchMode == SEARCH_MODE_OFF) )
        {
            if (!__dwTimeKey)
            {
                if (_wPreValue == 0xffff)
                    _wPreValue=0;

                // Micky2.81, support different number of digits.
                _wPreValue = (_wPreValue % ((__wMaxNumber+10)/10) ) *10;
                if ( (_wPreValue + wValue) > (__wMaxNumber+10) )
                    _wPreValue = 0;
                __dwNumberPrevious=(DWORD)(_wPreValue+wValue);
            }
            _wPreValue=(WORD)__dwNumberPrevious;
            __dwTimeNumber=OS_GetSysTimer();
            // DVD_099Micky, the time value must be divided by COUNT_1_SEC
            OSD_OUTPUT_MACRO(MSG_NUMBER,(WORD) __dwNumberPrevious, COUNT_NUMBER_GAP/COUNT_1_SEC);
        }
    }
        __dwTimeKey=0;
        }

//DVD_047-1. Number confirm
    //ppp
    if (bRealNumber)
    {
        // wyc1.10, support number confirm key to select file when disc is CDROM title.
#ifdef  SUPPORT_NUMBER_CONFIRM_KEY
        // only program, gototime, chapter search
//alex1.23,20040218 combine gototime , search
//        if (__bSearchMode || __btInputProgram || __btGoToTime)
        if ( (__bSearchMode != SEARCH_MODE_OFF) || __btInputProgram)
#else
#ifdef SUPPORT_CDROM_NUMBER_CONFIRM_KEY
        if (  (!(__wDiscType & BOOK_M1)) 
              ||( (__bAttrPlay <= ATTR_AV_END && __bAttrPlay >= ATTR_AV_BEGIN) && (__btPlaying || __btPause) )
        ) 
        
#endif // 
#endif
        {
            _OSDSRCH_ConfirmAction(__dwNumberPrevious);
            _wPreValue=0xffff;
        }
        return;
    }
    else    // 10+
    {
//alex1.23,20040218 combine gototime , search
//        if (__btGoToTime || __bSearchMode)
        if ( __bSearchMode != SEARCH_MODE_OFF)
        {
            _OSDSRCH_ConfirmAction(__dwNumberPrevious);
            _wPreValue=0xffff;
            return;
        }
    }

    // Micky1.23, below checking seems not necessary.
    //if((!__bSearchMode) && (!__btInputProgram))
    // seems search/program will now go here.

    // LLY.036, don't process key 10+ while in search mode
    // wyc.101, to avoid number 10+ in program input mode
    if((!__bSearchMode) && (!__btInputProgram))
    {
        // Key 10+, still need input the other digit
        if (!bRealNumber)
        {
            // show the number value
            OSD_OUTPUT_MACRO ( MSG_KEY10,(WORD) __dwNumberPrevious, COUNT_NUMBER_GAP/COUNT_1_SEC );
            return;
        }
    }
}

//  *********************************************************************************
//  Function    :   OSDSRCH_Recover
//  Description :   The function is called by OSD to do the recovery of Program UI 
//  Arguments   :   bRecoverRegion, the recover region 
//		:   OSD_RECOVER_ALL 
//		:   OSD_RECOVER_UPPER_REGION, invalid
//		:   OSD_RECOVER_BOTTOM_REGION, invalid
//  Return      :   TRUE, The region has been recovered.
//              :   FALSE, The region doesn't need to be recovered in this UI.
//  Side Effect :
//  *********************************************************************************
BYTE OSDSRCH_Recover(BYTE bRecoverRegion)
{
    if (OSD_RECOVER_ALL != bRecoverRegion) 
    {
    	return FALSE;
    }    

    _OSDSRCH_InitializeGDI();

    if(__bSearchMode == SEARCH_MODE_TIME)
    {        
        if ( !(__wDiscType & BOOK_DVD ))   
        {                               
            _OSDSRCH_Entry_PureTime();       
        }
        else
        {
#ifndef NO_DISC_MODE //CoCo2.37p
            _OSDSRCH_Entry_TitleTime();
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        }
    } 
#ifndef NO_DISC_MODE //CoCo2.37p    
    else if(__bSearchMode == SEARCH_MODE_TITLE_CHAPTER)
    {
        _OSDSRCH_Entry_TitleChapter();   
    }    
#endif //#ifndef NO_DISC_MODE //CoCo2.37p    
    
    return TRUE;
}


void OSDSRCH_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;

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

#ifdef SIMPLE_SEARCH_UI 
    RegionInfo.bColorMode = GDI_OSD_4B_MODE;
#else
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;
#endif
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;

    /*
    GDI_DeactivateRegion();
    GDI_ConfigRegionInfo(0, &RegionInfo);
    GDI_InitialRegion(0);

    // Alan2.22, doesn't need to clear region for Mantin bug #1832
    // Clear region
    // GDI_ClearRegion(0);

    GDI_ActivateRegion(0);
    */

    OSD_SetRegion(0, FALSE, &RegionInfo);
}


//CoCo0.76
//  *********************************************************************************
//  Function    :   _OSDSRCH_ClearOSDRegion
//  Description :   This function will clear the OSD region except the NU/NB region.
//  Arguments   :   
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************************
void _OSDSRCH_ClearOSDRegion(void)
{
    _OSDSRCHGRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    
    _InitRect(&(_OSDSRCHGRect.rect), 0, OSDND_UPPER_BACKGROUND_BOTTOM+1, GDI_REGION_WIDTH, OSDND_BOTTOM_BACKGROUND_START_V - 1); //CoCo1.20, reduce code size
    
    GDI_FillRect_909(0, &_OSDSRCHGRect);
}



//Alex 0.80
//  *************************************************************************
//  Function    :   _OSDSRCH_Entry_Initial
//  Description :   This function will initial some setting before entry
//  Arguments   :   
//  Return      :   None
//  Side Effect :   None
//  *************************************************************************
void _OSDSRCH_Entry_Initial(void)
{
    OSD_ChangeUI(OSD_UI_SEARCH, OSD_ENTER_UI);

    //CoCo0.76, Clear the OSD region 
    _OSDSRCH_ClearOSDRegion();    
    _OSDSRCH_InitializeGDI();
    
    //alex070,20041208,fix the bug  #157
    // Mute e Search, mute icon is cleared (it clear whole screen)
    //GDI_ClearRegion(0); 
    //GDI_SetMixRatio(GDI_GENERAL_MIX_RATIO);                        
    GDI_ActivateRegion(0);
}


//Alex 0.80
//  *********************************************************************************
//  Function    :   _OSDSRCH_Entry_TiTleChapter
//  Description :   This function will show  Title and Chapter
//  Arguments   :   
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************************
#ifndef NO_DISC_MODE //CoCo2.37p
void _OSDSRCH_Entry_TitleChapter(void)
{
    _OSDSRCH_ShowSrchFrame();
    _OSDSRCH_ShowButton(SEARCH_CURSOR_TITLE-SEARCH_CURSOR_BEGIN); //CoCo1.20, reduce code size.
    _OSDSRCH_ShowButton(SEARCH_CURSOR_CHAPTER-SEARCH_CURSOR_BEGIN); //CoCo1.20, reduce code size.
    _OSDSRCH_ShowTwoEntry(OSDSRCH_ENTRY_COLOR_FRAME_CENTER, OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG);      
}
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

//  *************************************************************************
//  Function    :   _OSDSRCH_Entry_TitleTime
//  Description :   This function will show Title and Time
//  Arguments   :   
//  Return      :   None
//  Side Effect :   None
//  *************************************************************************
#ifndef NO_DISC_MODE //CoCo2.37p
void _OSDSRCH_Entry_TitleTime(void)
{
    _OSDSRCH_ShowSrchFrame();
    _OSDSRCH_ShowButton(SEARCH_CURSOR_TITLE-SEARCH_CURSOR_BEGIN); //CoCo1.20, reduce code size.
    __bSearchCursor = SEARCH_CURSOR_TITLE;                    
    _ShowOneEntry(OSDSRCH_ENTRY_COLOR_FRAME_CENTER); 

    _OSDSRCH_ShowButton(SEARCH_CURSOR_TIME-SEARCH_CURSOR_BEGIN); //CoCo1.20, reduce code size.
    __bSearchCursor = SEARCH_CURSOR_TIME;         
    _ShowOneEntry(OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG);         
}
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

//  *************************************************************************
//  Function    :   _OSDSRCH_Entry_PureTime
//  Description :   This function will show Pure Time
//  Arguments   :   
//  Return      :   None
//  Side Effect :   None
//  *************************************************************************
void _OSDSRCH_Entry_PureTime(void)
{
#ifndef SIMPLE_SEARCH_UI
    _OSDSRCH_SetCommonFrameAttr(); //CoCo1.20, reduce code size.
    _OSDSRCH_DrawFrame(SRCH_FRAME_X0, SRCH_FRAME_Y0, BUTTON_C_X1, SRCH_FRAME_Y1); //CoCo1.20, reduce code size.
#else
    _OSDSRCHGRect.bColor = OSDSRCH_ENTRY_COLOR_FRAME_CENTER;
    _InitRect(&(_OSDSRCHGRect.rect), SRCH_FRAME_X0, SRCH_FRAME_Y0,
                                     BUTTON_C_X1, SRCH_FRAME_Y1); 
    GDI_FillRect_909(0, (PPARM_RECT)&_OSDSRCHGRect.rect);
#endif
    
    _OSDSRCH_ShowButton(SEARCH_CURSOR_TIME-SEARCH_CURSOR_BEGIN); //CoCo1.20, reduce code size.
    __bSearchCursor = SEARCH_CURSOR_TIME;         
    _ShowOneEntry(OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG);        
   
}


//CoCo1.20, reduce code size.
#ifndef SIMPLE_SEARCH_UI
void _OSDSRCH_SetCommonFrameAttr(void)
{
    _OSDSRCHFrame.bColorBright= OSDSRCH_ENTRY_COLOR_FRAME_BRIGHT;
    _OSDSRCHFrame.bColorCenter= OSDSRCH_ENTRY_COLOR_FRAME_CENTER;
    _OSDSRCHFrame.bColorDark=   OSDSRCH_ENTRY_COLOR_FRAME_DARK;
    _OSDSRCHFrame.bStyle=FRAME_POP_SOLID; // bStyle                        
    _OSDSRCHFrame.bThickness = FRAME_THICKNESS;// bThickness
}
#endif #ifndef SIMPLE_SEARCH_UI

//CoCo1.20, reduce code size.    
void _OSDSRCH_DrawFrame(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom)
{
    _InitRect(&(_OSDSRCHFrame.rect), wLeft, wTop, wRight, wBottom); //CoCo1.20, reduce code size.    
    GDI_DrawFrame_909(0, &_OSDSRCHFrame);
}

//CoCo1.20, reduce code size.
void _OSDSRCH_DrawString(void)
{
    GDI_DrawString_909(0, &_OSDSRCHStringAttr, _wOSDSRCHString );
}

#endif //#ifndef NO_SEARCH_UI //++CoCo2.37p









