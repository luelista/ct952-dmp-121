#define CALENDAR_MAIN_FILE

#include "winav.h"
#include "menu.h"
#include "calenui.h"
#include "osdnd.h"
#include "setup.h"
#include "fontindex.h"
#include "cc.h"
#include "poweronmenu.h"
#include "disp.h"
#include "rtcdrv.h"
#include "clock.h"
#include "osd.h"

#ifdef SUPPORT_STB
#include "dvb_sys.h"
#include "ui_lmp.h"
#include "nvram.h"
#endif

#include "OSDString\\strextrn.h"

BYTE _bCALENDARMode;
WORD _wCALENDARUpdateTimePosX;
WORD _wCALENDARUpdateTimePosY;
RTC_TM _CALENDARCurrnetRTCTime;
DWORD _dwCALENDARTriggerTime;

WORD _wCurrentYear;
BYTE _bCurrentMonth, _bCurrentDate;
BYTE _bLastDay;
BYTE _bCalX, _bCalY;
WORD *_pwBuffer=NULL;
WORD _wStartX, _wStartY, _wTotalH, _wTotalW;

WORD _wFocusYear ;
BYTE _bFocusMonth;

BYTE        _bBodyBackColor, _bBodyFront1Color, _bBodyBorderColor;
BYTE        _bHeadBackColor, _bHeadFront1Color, _bHeadFront2Color,_bHeadBorderColor;
BYTE        _bTailBackColor, _bTailFront1Color;  

WIN_PARAMETER stFullScreen;

typedef struct calendaInfo_
{
    WORD		u16CalenderYear;    /** : solar year for calender */    
    BYTE         u8CalenderMonth;    /**: solar month for calender */
    BYTE         u8CalenderDate;	    /** : solar date for calender */    
} CALENDAINFO;
CALENDAINFO Calenda_Info;
BYTE  __bCalSelectMenuID;
BYTE  __bCalselectdata=0;
/////////////////////////////////////////////////////////////////////////////////////
#ifdef SUPPORT_SET_DATE_BY_MENU
HMENU _hCALENDARDateMenu = MENU_HANDLE_INVALID;
SETDATE_DATE _CALENDARSetDate;
HDIALOG _hCALENDARDlg = DIALOG_HANDLE_INVALID;
HSETDATE _hCALENDARSetDate = SETDATE_HANDLE_INVALID;
BYTE _bCALENDARExitKey[] = {KEY_LEFT};
#endif //SUPPORT_SET_DATE_BY_MENU
/////////////////////////////////////////////////////////////////////////////////////

WORD aCALENDARColon[] = {1, CHAR_COLON};
//#ifdef NL_SOLUTION
WORD aCALENDARDistance[] = {3, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_SPACE};
//#endif //NL_SOLUTION

WORD aCalenda_Solidus[2] = {1, CHAR_SOLIDUS}; //fhfeng 1012
WORD *aWeeklyDay[7] = 
{
    aWeeklyDay_Sun,
    aWeeklyDay_Mon,
    aWeeklyDay_Tue,
    aWeeklyDay_Wed,
    aWeeklyDay_Thu,
    aWeeklyDay_Fri,
    aWeeklyDay_Sat,     
};

void Calendar_InitialPalEntry(void)
{
    // set entries used by Menu, Setup, Equalizer and AV System
    GDI_ChangePALEntry(CALENDAR_MENU_ENTRY_TITLE_BG_COLOR, CALENDAR_MENU_VALUE_TITLE_BG_COLOR, FALSE);
    GDI_ChangePALEntry(CALENDAR_MENU_ENTRY_LARGEFORM_BG_COLOR, CALENDAR_MENU_VALUE_LARGEFORM_BG_COLOR, FALSE);
    GDI_ChangePALEntry(CALENDAR_MENU_ENTRY_COLOR_DARK, CALENDAR_MENU_VALUE_COLOR_DARK, FALSE);
    GDI_ChangePALEntry(CALENDAR_MENU_ENTRY_COLOR_HIGHLIGHT, CALENDAR_MENU_VALUE_COLOR_HIGHLIGHT, FALSE);
    GDI_ChangePALEntry(CALENDAR_MENU_ENTRY_TETRIS_BODY_COLOR, CALENDAR_MENU_VALUE_TETRIS_BODY_COLOR, FALSE);
    GDI_ChangePALEntry(CALENDAR_MENU_ENTRY_TITLE_BODY_HIGHLIGHT_COLOR, CALENDAR_MENU_VALUE_TITLE_BODY_HIGHLIGHT_COLOR, FALSE);
    GDI_ChangePALEntry(CALENDAR_MENU_ENTRY_SUNDAY_COLOR, CALENDAR_MENU_VALUE_SUNDAY_COLOR, FALSE);
    GDI_ChangePALEntry(CALENDAR_MENU_ENTRY_SATURDAY_COLOR, CALENDAR_MENU_VALUE_SATURDAY_COLOR, FALSE);
}
void Calendar_DrawString(PPARM_DRAW_STRING pStrAttr, WORD wStartX, WORD wStartY, WORD * pwString)
{
    pStrAttr->wX = wStartX;
    pStrAttr->wY = wStartY;
    GDI_DrawString_909(0, pStrAttr, pwString);
}

void Calendar_OutputNumber(WORD wPosX, WORD wPosY, WORD wNumber, PPARM_DRAW_STRING GmMsg)
{
    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex(wNumber ); 
    Calendar_DrawString(GmMsg,wPosX, wPosY, __wOSDUTLStringBuf);
}

void Calendar_ShowBoxWithArc(WORD wPosX, WORD wPosY, WORD wWidth, WORD wHeight, BYTE bBackCIdx)
{
    PARM_RECT rectArcBox;
    rectArcBox.rect.wLeft = wPosX;
    rectArcBox.rect.wTop = wPosY;
    rectArcBox.rect.wRight = wPosX+wWidth;
    rectArcBox.rect.wBottom = wPosY+wHeight;
    rectArcBox.bColor = bBackCIdx;
    GDI_FillRoundRect(0, 13, &rectArcBox);
}


void Calendar_DrawRect(WORD wPosX, WORD wPosY, WORD wWidth, WORD wHeight, BYTE bColor)
{
    PARM_RECT rectGMRect;
    rectGMRect.bColor = bColor;
    rectGMRect.rect.wLeft = wPosX;
    rectGMRect.rect.wRight = rectGMRect.rect.wLeft + wWidth-1;
    rectGMRect.rect.wTop = wPosY;
    rectGMRect.rect.wBottom = rectGMRect.rect.wTop + wHeight-1;
    GDI_FillRect(0, &rectGMRect);
}

//  ***************************************************************************
//  Function    :   _Calendar_ConfigOSDRegion
//  Description :   
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void _Calendar_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;
#ifdef  BORROW_FRAME_BUFFER
    DWORD dwBufferAddr;
#endif //

    GDI_ClearRegion(0);

#ifdef  BORROW_FRAME_BUFFER

    if ((POWERPNMENU_WIDTH == __RegionList[0].wWidth) && (POWERPNMENU_HEIGHT <= __RegionList[0].wHeight) &&
        (GDI_OSD_8B_MODE == __RegionList[0].bColorMode)) 
    {
        return;
    }
    else if (__RegionList[0].bColorMode != GDI_OSD_8B_MODE)   // Thumbnail mode & preview mode
    {
        // Use Frame buffer 1 or 2
        dwBufferAddr = DISP_QueryAvailableBuffer();
        if (0 == dwBufferAddr)
        {
            RegionInfo.dwTAddr = DS_OSDFRAME_ST;
        }
        else
        {
            RegionInfo.dwTAddr = dwBufferAddr;
        }
    } 
    else
    {
        // Use OSD buffer and Frame buffer 0
        RegionInfo.dwTAddr = DS_OSDFRAME_ST;   // Use OSD buffer and Frame buffer 0 
    }

#else
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
#endif // #ifdef  BORROW_FRAME_BUFFER

    RegionInfo.wWidth = POWERPNMENU_WIDTH;
    RegionInfo.wHeight = POWERPNMENU_HEIGHT;
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;

#ifdef  BORROW_FRAME_BUFFER
    OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    OSD_SetRegion(0, FALSE, &RegionInfo);
#endif
}

void Calendar_GetVisibleScreen(PWIN_PARAMETER pScreenSize)
{
    extern BYTE __bTVType;
    
    if( __bTVType==SETUP_SCREEN_TV_SYSTEM_PAL )
    {
        pScreenSize->sHStart = 8;// must be multiple of 4
        pScreenSize->sVStart= 24;
        //pScreenSize->sVStart= 0;
    }
    else
    {
        pScreenSize->sHStart = 8;// must be multiple of 4
        pScreenSize->sVStart= 0; // 24;
    }
    
    //CoCo1.10, fix the bug about the OSD region exceeds the GDI region.
    pScreenSize->wHWidth = GDI_REGION_WIDTH; //624;// must be multiple of 4
    pScreenSize->wVWidth = GDI_REGION_HEIGHT;//78L
    
    return ;
}
static BYTE Calendar_GetLeap( WORD year )
{
    if ( year % 400 == 0 )
    {
        return 1;
    }
    else if ( year % 100 == 0 )
    {
        return 0;
    }
    else if ( year % 4 == 0 )
    {
        return 1;
    }
    return 0;
}

static BYTE SolarCal[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
BYTE Calendar_GetDaysofMonth(WORD u16SolarYear, WORD u8SolarMonth)
{
    
    if ( Calendar_GetLeap(u16SolarYear)!=0 && u8SolarMonth==2)
    {
        return SolarCal[u8SolarMonth-1]+1;
    }
    if ( u8SolarMonth>12 )
    {
        return 0;
    }
    
    return SolarCal[u8SolarMonth-1];
}

BYTE Calendar_SolarCalendar(WORD u16Year, BYTE u8Month,BYTE u8Day)
{
    BYTE u8Week, u8idx;
    WORD u16TheDayInYear, u16TheDayInPeriod;
    
    u16TheDayInYear = u8Day;
    for(u8idx=0; u8idx<u8Month-1; u8idx++)
    {
        if(u8idx==1)
        {
            if ( Calendar_GetLeap(u16Year)==1)
                u16TheDayInYear++;
        }
        u16TheDayInYear += SolarCal[u8idx];
    }
    
    u16TheDayInPeriod=u16Year-1+(u16Year-1)/4-(u16Year-1)/100+(u16Year-1)/400+u16TheDayInYear;
    
    u8Week = u16TheDayInPeriod%7;
    
    return (u8Week);
}



//color = 'true',highlight
//false. normal
void calendar_draw_title(BYTE flag)
{
    WORD wYearX, wYearY;
    PARM_DRAW_STRING strTitle;
    
    if ((_bCALENDARMode == CALENDAR_MODE_SETTING) || (_bCALENDARMode == CALENDAR_MODE_DISPLAY_PERPETUAL_CALENDAR))
    {
        wYearX = _wStartX + CALENDER_ITEM_X_OFFSET+150; //fhfeng 1012
    }
    else //Display mode
    {
        wYearX = _wStartX + CALENDER_ITEM_X_OFFSET+80;
    }

    wYearY = _wStartY + 8;
    
    Calendar_DrawRect(_wStartX, _wStartY , _wTotalW, CALENDER_HEAD_HEIGHT_OF_YEAR, _bHeadBackColor); //clear the title region
    strTitle.bBackground = _bHeadBackColor;
    strTitle.bColorKey = 0;
    if(flag==TRUE) //fhfeng 1012
        strTitle.bTextColor =CALENDAR_MENU_ENTRY_TITLE_BODY_HIGHLIGHT_COLOR;
    else
        strTitle.bTextColor =_bHeadFront2Color;
    
    strTitle.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    strTitle.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;

//#ifdef NL_SOLUTION
    switch (_bCurrentMonth)
    {
    case 1:
        __pOSDUTLOutputString = aMonthJanuary;
        //GDI_DrawString(0, &strTitle, aMonthJanuary);
        break;
    case 2:
        __pOSDUTLOutputString = aMonthFebruary;
        //GDI_DrawString(0, &strTitle, aMonthFebruary);
        break;
    case 3:
        __pOSDUTLOutputString = aMonthMarch;
        //GDI_DrawString(0, &strTitle, aMonthMarch);
        break;
    case 4:
        __pOSDUTLOutputString = aMonthApril;
        //GDI_DrawString(0, &strTitle, aMonthApril);
        break;
    case 5:
        __pOSDUTLOutputString = aMonthMay;
        //GDI_DrawString(0, &strTitle, aMonthMay);
        break;
    case 6:
        __pOSDUTLOutputString = aMonthJune;
        //GDI_DrawString(0, &strTitle, aMonthJune);
        break;
    case 7:
        __pOSDUTLOutputString = aMonthJuly;
        //GDI_DrawString(0, &strTitle, aMonthJuly);
        break;
    case 8:
        __pOSDUTLOutputString = aMonthAugust;
        //GDI_DrawString(0, &strTitle, aMonthAugust);
        break;
    case 9:
        __pOSDUTLOutputString = aMonthSeptember;
        //GDI_DrawString(0, &strTitle, aMonthSeptember);
        break;
    case 10:
        __pOSDUTLOutputString = aMonthOctober;
        //GDI_DrawString(0, &strTitle, aMonthOctober);
        break;
    case 11:
        __pOSDUTLOutputString = aMonthNovember;
        //GDI_DrawString(0, &strTitle, aMonthNovember);
        break;
    case 12:
        __pOSDUTLOutputString = aMonthDecember;
        //GDI_DrawString(0, &strTitle, aMonthDecember);
        break;
    default:
        break;
    }

    strTitle.wX = wYearX;
    strTitle.wY = wYearY;
    GDI_DrawString(0, &strTitle, __pOSDUTLOutputString);
    strTitle.wX += GDI_GetStringWidth(__pOSDUTLOutputString);

    GDI_DrawString(0, &strTitle, aCALENDARDistance);
    strTitle.wX += GDI_GetStringWidth(aCALENDARDistance);

    wYearX = strTitle.wX;
    Calendar_OutputNumber(wYearX, wYearY, _bCurrentDate, &strTitle);
    wYearX += GDI_GetStringWidth(&__wOSDUTLStringBuf[0]);

    strTitle.wX = wYearX;
    GDI_DrawString(0, &strTitle, aCALENDARDistance);
    wYearX += GDI_GetStringWidth(aCALENDARDistance);

    Calendar_OutputNumber(wYearX, wYearY, _wCurrentYear, &strTitle);
//#else    
    /*
    Calendar_OutputNumber(wYearX, wYearY, _wCurrentYear, &strTitle);
    wYearX += GDI_GetStringWidth(&__wOSDUTLStringBuf[0]);

    wYearX +=2;  //draw "/"
    Calendar_DrawString(&strTitle,wYearX, wYearY,&aCalenda_Solidus[0]);
    wYearX += GDI_GetStringWidth(&aCalenda_Solidus[0])+2;

    Calendar_OutputNumber(wYearX, wYearY, _bCurrentMonth, &strTitle);
    wYearX += GDI_GetStringWidth(&__wOSDUTLStringBuf[0]);
    
    wYearX +=2; //draw "/"
    Calendar_DrawString(&strTitle,wYearX, wYearY,&aCalenda_Solidus[0]);
    wYearX += GDI_GetStringWidth(&aCalenda_Solidus[0])+2;

    Calendar_OutputNumber(wYearX, wYearY, _bCurrentDate, &strTitle);
    */
//#endif //NL_SOLUTION

    _wCALENDARUpdateTimePosX = wYearX+GDI_GetStringWidth(&__wOSDUTLStringBuf[0]);
    _wCALENDARUpdateTimePosY = wYearY;
}

void calendar_draw_item_HighLight(BYTE bDate, BYTE bFocus)
{
    WORD wContentX, wContentY, wContentW, wContentH;
    BYTE bIdx;
    BYTE bHighLightColor;
    
    _bCalX = Calendar_SolarCalendar(_wCurrentYear,_bCurrentMonth,bDate);
    for(bIdx=1;bIdx<8;bIdx++)//check the 1st satday
    {
        if(Calendar_SolarCalendar(_wCurrentYear,_bCurrentMonth,bIdx)==6)
            break;
    }
    
    //DVB_Msg(MSG_4_AP, "The first satday is %d\n",(int)idx, __FFLL__);
    if(bDate<=bIdx)
        _bCalY=0;
    else
    {
        if((bDate-bIdx)%7==0)
            _bCalY=(bDate-bIdx)/7;
        else
            _bCalY=(bDate-bIdx)/7+1;
    }
    
    wContentX = _wStartX + CALENDER_ITEM_X_OFFSET + _bCalX*CALENDER_ITEM_WIDTH;
    wContentY = _wStartY + CALENDER_HEAD_HEIGHT + CALENDER_ITEM_BORDER + _bCalY*CALENDER_ITEM_HEIGHT;
    wContentW = CALENDER_ITEM_WIDTH;
    wContentH = CALENDER_ITEM_HEIGHT;
    
    if(bFocus)
        bHighLightColor = CALENDAR_MENU_ENTRY_TITLE_BODY_HIGHLIGHT_COLOR;
    else
        bHighLightColor = _bBodyBackColor;
    {
        Calendar_DrawRect( wContentX, wContentY, wContentW, 2, 
            bHighLightColor);
        Calendar_DrawRect( wContentX, wContentY, 2, wContentH, 
            bHighLightColor);
        Calendar_DrawRect( wContentX + wContentW-2, wContentY, 2, wContentH, 
            bHighLightColor);
        Calendar_DrawRect( wContentX, wContentY + wContentH-2, wContentW, 2, 
            bHighLightColor);
    }
}


void calendar_draw_item(BYTE bDate, BYTE bFocus)
{
    WORD wContentX, wContentY, wContentW, wContentH;
    WORD wNumberX, wNumberY;
    BYTE bIdx;
    PARM_DRAW_STRING strItem;
    
    _bCalX = Calendar_SolarCalendar(_wCurrentYear,_bCurrentMonth,bDate);
    for(bIdx=1;bIdx<8;bIdx++)//check the 1st satday
    {
        if(Calendar_SolarCalendar(_wCurrentYear,_bCurrentMonth,bIdx)==6)
            break;
    }
    
    if(bDate<=bIdx)
        _bCalY=0;
    else
    {
        if((bDate-bIdx)%7==0)
            _bCalY=(bDate-bIdx)/7;
        else
            _bCalY=(bDate-bIdx)/7+1;
    }
    
    wContentX = _wStartX + CALENDER_ITEM_X_OFFSET + _bCalX*CALENDER_ITEM_WIDTH;
    wContentY = _wStartY + CALENDER_HEAD_HEIGHT + CALENDER_ITEM_BORDER + _bCalY*CALENDER_ITEM_HEIGHT;
    wContentW = CALENDER_ITEM_WIDTH;
    wContentH = CALENDER_ITEM_HEIGHT;
    
    if(bFocus)
    {
        Calendar_DrawRect( wContentX, wContentY, wContentW, 2, 
            CALENDAR_MENU_ENTRY_TITLE_BODY_HIGHLIGHT_COLOR);
        Calendar_DrawRect( wContentX, wContentY, 2, wContentH, 
            CALENDAR_MENU_ENTRY_TITLE_BODY_HIGHLIGHT_COLOR);
        Calendar_DrawRect( wContentX + wContentW-2, wContentY, 2, wContentH, 
            CALENDAR_MENU_ENTRY_TITLE_BODY_HIGHLIGHT_COLOR);
        Calendar_DrawRect( wContentX, wContentY + wContentH-2, wContentW, 2, 
            CALENDAR_MENU_ENTRY_TITLE_BODY_HIGHLIGHT_COLOR);
    }
    strItem.bBackground = _bBodyBackColor;
    strItem.bColorKey = 0;
    strItem.bTextColor = _bBodyFront1Color;
    strItem.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    strItem.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;    
    
    if(_bCalX==0)
    {
        //strItem.bTextColor = CALENDAR_MENU_ENTRY_COLOR_HIGHLIGHT;
        strItem.bTextColor = CALENDAR_MENU_ENTRY_SUNDAY_COLOR;
    }
    else if (_bCalX == 6)
    {
        strItem.bTextColor = CALENDAR_MENU_ENTRY_SATURDAY_COLOR;
    }
    else
    {
        strItem.bTextColor = _bBodyFront1Color; //mon ~ Fri normal
    }   
    
    if(bDate<10)
        wNumberX = wContentX + (wContentW-/*AP_OSDGetNumberFontWidth()*/8)/2;
    else
        wNumberX = wContentX + (wContentW-2*8)/2;
    wNumberY = wContentY + 6;
    Calendar_OutputNumber(wNumberX, wNumberY, (WORD) bDate,&strItem);    
}

void calendar_draw_background(void)
{
    BYTE bIdx;
    WORD wWeeklyX, wWeeklyY;
    PARM_DRAW_STRING strText;    
    
    Calendar_ShowBoxWithArc( _wStartX-CALENDER_BACKGROUND_BORDER, _wStartY, _wTotalW+2*CALENDER_BACKGROUND_BORDER,
        _wTotalH+CALENDER_BACKGROUND_BORDER, _bHeadBackColor);
    
    Calendar_DrawRect( _wStartX, _wStartY + CALENDER_HEAD_HEIGHT_OF_YEAR, _wTotalW, CALENDER_HEAD_HEIGHT_OF_WEEKLY, 
        _bHeadBorderColor);
    
    
    for(bIdx=0; bIdx<7; bIdx++)
    {
        //avoid message length larger than CALENDER_ITEM_WIDTH
        wWeeklyX = GDI_GetStringWidth(aWeeklyDay[bIdx]);
        if(CALENDER_ITEM_WIDTH >= wWeeklyX)
            wWeeklyX = _wStartX + CALENDER_ITEM_X_OFFSET + (WORD)(bIdx)*CALENDER_ITEM_WIDTH + (CALENDER_ITEM_WIDTH-wWeeklyX)/2;
        else
            wWeeklyX = _wStartX + CALENDER_ITEM_X_OFFSET + (WORD)(bIdx)*CALENDER_ITEM_WIDTH;
        
        wWeeklyY = _wStartY + CALENDER_HEAD_HEIGHT - GDI_GetStringHeight(aWeeklyDay[bIdx]) - 2;
        
        strText.bBackground = _bHeadBorderColor;
        strText.bColorKey = 0;
        if (bIdx == 0)
        {
            strText.bTextColor = CALENDAR_MENU_ENTRY_SUNDAY_COLOR;
        }
        else if (bIdx == 6)
        {
            strText.bTextColor = CALENDAR_MENU_ENTRY_SATURDAY_COLOR;
        }
        else
        {
            strText.bTextColor = _bHeadFront1Color;
        }
        strText.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
        strText.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
        Calendar_DrawString(&strText,wWeeklyX, wWeeklyY,aWeeklyDay[bIdx]);
    }
    
}


void calendar_draw_all(void)
{
    BYTE bIdx;
    
    // clean working area
    Calendar_DrawRect(_wStartX, _wStartY+CALENDER_HEAD_HEIGHT, _wTotalW, CALENDER_BODY_HEIGHT, _bBodyBackColor);
    if(__bCalSelectMenuID==YEAR_MONTH_CHANGE)
	{
        calendar_draw_title(TRUE);
    }
    else
	{
        calendar_draw_title(FALSE);
    }
    
    for(bIdx=1; bIdx<=_bLastDay; bIdx++)
    {
        if ((bIdx==__bCalselectdata) && ( __bCalSelectMenuID==DATA_CHANGE))
        {
            calendar_draw_item(bIdx, TRUE);
        }
        else
        {
            calendar_draw_item(bIdx, FALSE);
        }
    }
}
//get data from clock,now use constant
BYTE  CALENDA_Get_YYMMDD(void)
{
    RTC_TM _Calendar;

    RTC_GetTime(&_Calendar);

#ifndef CT950_STYLE
    Calenda_Info.u16CalenderYear  = 2007;
    Calenda_Info.u8CalenderMonth = 1;
    Calenda_Info.u8CalenderDate = 1;
#else
    if ((_Calendar.bMon > 12) || (_Calendar.bMon < 1))
    {
        Calenda_Info.u16CalenderYear  = 2007;
        Calenda_Info.u8CalenderMonth = 1;
        Calenda_Info.u8CalenderDate = 1;
    }
    else
    {
        Calenda_Info.u16CalenderYear  = _Calendar.bYear+2000;
        Calenda_Info.u8CalenderMonth = _Calendar.bMon;
        Calenda_Info.u8CalenderDate = _Calendar.bDate;
    }
#endif //CT950_STYLE

   return TRUE;
}
//save current year/month/data to EEprom
void  CALENDA_Save_YYMMDD(void)
{

       
}

void CALENDAR_Entry(BYTE bMode)
{
    _bCALENDARMode = bMode;

    if(CALENDA_Get_YYMMDD()==FALSE)
    {	
        //set default value
        _wCurrentYear = CAL_DEFAULT_YEAR;
        _bCurrentMonth =CAL_DEFAULT_MONTH;
        _bCurrentDate = CAL_DEFAULT_DATA;
        
    }
    else
    {	       //set get data from clock
        _wCurrentYear = Calenda_Info.u16CalenderYear;
        _bCurrentMonth =Calenda_Info.u8CalenderMonth;
        _bCurrentDate = Calenda_Info.u8CalenderDate;
    }

    _wFocusYear = _wCurrentYear;
    _bFocusMonth = _bCurrentMonth;
    
    _bLastDay = Calendar_GetDaysofMonth(_wCurrentYear, _bCurrentMonth);

    //Config the OSD to 8 bit mode.
    _Calendar_ConfigOSDRegion();

    switch (bMode)
    {
    case CALENDAR_MODE_SETTING:
        _CALENDAR_ShowCalendar(FALSE);
        break;
    case CALENDAR_MODE_DISPLAY_CLOCK_AND_TIME:
        _CALENDAR_ShowCalendar(TRUE);
        break;
    case CALENDAR_MODE_MENU:
#ifdef SUPPORT_SET_DATE_BY_MENU
        _hCALENDARDateMenu = MENU_HANDLE_INVALID;
        _CALENDAR_ShowMenu();
#endif //SUPPORT_SET_DATE_BY_MENU
        break;
    case CALENDAR_MODE_DISPLAY_PERPETUAL_CALENDAR:
        _CALENDAR_ShowCalendar(FALSE);
        break;
    case CALENDAR_MODE_SET_DATE:
        break;
    default:
        break;
    }           
}

void _CALENDAR_ShowCalendar(BYTE bShowTime)
{
    Calendar_InitialPalEntry();   
    
    Calendar_GetVisibleScreen(&stFullScreen);
    _wTotalH = 6*CALENDER_ITEM_HEIGHT+CALENDER_HEAD_HEIGHT_OF_YEAR+CALENDER_HEAD_HEIGHT_OF_WEEKLY;//stStPos->u16Height;
    _wTotalW =2*CALENDER_X_OFFSET+7*CALENDER_ITEM_WIDTH;// stStPos->u16Width-2*CALENDER_X_OFFSET;
    _wStartX = stFullScreen.sHStart + (stFullScreen.wHWidth-_wTotalW)/2;
    _wStartY = stFullScreen.sVStart+50; //+ (stFullScreen.wVWidth-_wTotalH)/2;
    
    _bBodyBackColor = CALENDAR_MENU_ENTRY_TETRIS_BODY_COLOR;
    _bBodyFront1Color = CALENDAR_MENU_ENTRY_COLOR_DARK;
    _bBodyBorderColor = CALENDAR_MENU_ENTRY_TITLE_BODY_HIGHLIGHT_COLOR;
    _bHeadBackColor = CALENDAR_MENU_ENTRY_LARGEFORM_BG_COLOR;
    _bHeadFront1Color = CALENDAR_MENU_ENTRY_COLOR_HIGHLIGHT;
    _bHeadFront2Color = CALENDAR_MENU_ENTRY_COLOR_DARK;
    _bHeadBorderColor = CALENDAR_MENU_ENTRY_TITLE_BG_COLOR;
    _bTailBackColor = CALENDAR_MENU_ENTRY_TITLE_BG_COLOR;
    _bTailFront1Color = CALENDAR_MENU_ENTRY_COLOR_HIGHLIGHT;        
        
    calendar_draw_background();

    //fhfeng 1012
    __bCalSelectMenuID=DATA_CHANGE;
    __bCalselectdata=_bCurrentDate; 
    calendar_draw_all();

    if (bShowTime)
    {
        _dwCALENDARTriggerTime = OS_GetSysTimer();
        RTC_GetTime(&_CALENDARCurrnetRTCTime);

        _CALENDAR_ShowTime(TRUE);
    }
}

void CALENDAR_Recover(void)
{
    //Config the OSD to 8 bit mode.
    _Calendar_ConfigOSDRegion();

    switch (_bCALENDARMode)
    {
    case CALENDAR_MODE_SETTING:
        _CALENDAR_ShowCalendar(FALSE);
        break;
    case CALENDAR_MODE_DISPLAY_CLOCK_AND_TIME:
        _CALENDAR_ShowCalendar(TRUE);
        break;
    case CALENDAR_MODE_MENU:
#ifdef SUPPORT_SET_DATE_BY_MENU
        _CALENDAR_ShowMenu();
#endif //SUPPORT_SET_DATE_BY_MENU
        break;
    case CALENDAR_MODE_DISPLAY_PERPETUAL_CALENDAR:
        _CALENDAR_ShowCalendar(FALSE);
        break;
    case CALENDAR_MODE_SET_DATE:
#ifdef SUPPORT_SET_DATE_BY_MENU
        _CALENDAR_ShowMenu();
        SETDATE_Recover(_hCALENDARSetDate);
#endif //SUPPORT_SET_DATE_BY_MENU
        break;
    default:
        break;
    }           
}

BYTE CALENDAR_ProcessKey(void)
{
#ifdef SUPPORT_SET_DATE_BY_MENU
    DWORD dwMenuReturn;
#endif //SUPPORT_SET_DATE_BY_MENU

    if (_bCALENDARMode == CALENDAR_MODE_DISPLAY_CLOCK_AND_TIME)
    {
        if ((__bKey == KEY_FUNCTION) || (__bKey == KEY_EXIT))
        {
            CALENDAR_Exit(TRUE);
            __bCLOCKShowClock = FALSE;
            POWERONMENU_BackToMainMenu();
        }
    }
#ifdef SUPPORT_SET_DATE_BY_MENU
    else if (_bCALENDARMode == CALENDAR_MODE_MENU)
    {
        if ((__bKey == KEY_FUNCTION) || (__bKey == KEY_EXIT))
        {
            CALENDAR_Exit(TRUE);
            POWERONMENU_BackToMainMenu();            
        }

        dwMenuReturn = MENU_ProcessKey(_hCALENDARDateMenu, __bKey);
        
        if (HIWORD(dwMenuReturn) == MENU_ACTION_EXIT)
        {
            CALENDAR_Exit(FALSE);
            POWERONMENU_BackToMainMenu();                        
        }
        else if (HIWORD(dwMenuReturn) == MENU_ACTION_PROCESS_MENU_ITEM)
        {
            switch (LOWORD(dwMenuReturn))
            {
            case CALENDAR_MAIN_DISPLAY:
                CALENDAR_Entry(CALENDAR_MODE_DISPLAY_PERPETUAL_CALENDAR);
                break;
            case CALENDAR_MAIN_SET_DATE:
                _bCALENDARMode = CALENDAR_MODE_SET_DATE;
                _CALENDAR_SET_DATE.wYear = _wCurrentYear;
                _CALENDAR_SET_DATE.bMonth = _bCurrentMonth;
                _CALENDAR_SET_DATE.bDate = _bCurrentDate;

                _hCALENDARSetDate  = SETDATE_Create(&_CALENDAR_SET_DATE, &_CALENDAR_SET_DATE_Action);
                if (_hCALENDARSetDate != SETDATE_HANDLE_INVALID)
                {
                    SETDATE_Show(_hCALENDARSetDate, MAINMENU_PALETTE_ENTRY_CONTENT_REGION, __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
                }
                break;
            case CALENDAR_MAIN_EXIT:
                CALENDAR_Exit(FALSE);
                POWERONMENU_BackToMainMenu();                        
                break;
            default:
                break;
            }            
        }
    }
    else if (_bCALENDARMode == CALENDAR_MODE_SET_DATE)
    {
        if ((__bKey == KEY_FUNCTION) || (__bKey == KEY_EXIT))
        {
            CALENDAR_Exit(TRUE);
            POWERONMENU_BackToMainMenu();            
        }
        SETDATE_ProcessKey(_hCALENDARSetDate, __bKey);
    }
#endif //SUPPORT_SET_DATE_BY_MENU
    else
    {
        switch (__bKey)
        {        
        case KEY_STOP: //go to the change month mode.
            if (_bCALENDARMode == CALENDAR_MODE_SETTING)
            {
                if(__bCalSelectMenuID==DATA_CHANGE)
                {
                    calendar_draw_item_HighLight(__bCalselectdata, FALSE);
                    __bCalSelectMenuID=YEAR_MONTH_CHANGE;
                    calendar_draw_title(TRUE);
                }
                else
                {
                    calendar_draw_title(FALSE);
                    __bCalSelectMenuID=DATA_CHANGE;
                    if(__bCalselectdata>_bLastDay)
                        __bCalselectdata=_bLastDay;
                    calendar_draw_item_HighLight(__bCalselectdata, TRUE);
                }
            }
            break;
        case KEY_DOWN:
            if (_bCALENDARMode == CALENDAR_MODE_DISPLAY_PERPETUAL_CALENDAR)
            {
                __bCalSelectMenuID = YEAR_MONTH_CHANGE;
            }

            if(__bCalSelectMenuID==YEAR_MONTH_CHANGE)
            {
                if(_bCurrentMonth>1)
                {
                    _bCurrentMonth--;
                }
                else
                {
                    if(_wCurrentYear>CAL_FIRSTYEAR)
                    {
                        _wCurrentYear--;
                        _bCurrentMonth = 12;
                    }
                }
                
                _bLastDay = Calendar_GetDaysofMonth(_wCurrentYear, _bCurrentMonth);
                calendar_draw_all();
            }
            else  if(__bCalSelectMenuID==DATA_CHANGE)
            {
                if(__bCalselectdata<=(_bLastDay-7))
                {
                    calendar_draw_item_HighLight(__bCalselectdata, FALSE);
                    __bCalselectdata +=7;
                    calendar_draw_item_HighLight(__bCalselectdata, TRUE);
                }
            }
            break;
        case KEY_UP:
            if (_bCALENDARMode == CALENDAR_MODE_DISPLAY_PERPETUAL_CALENDAR)
            {
                __bCalSelectMenuID = YEAR_MONTH_CHANGE;
            }

            if(__bCalSelectMenuID==YEAR_MONTH_CHANGE)
            {
                if(_bCurrentMonth<12)
                    _bCurrentMonth++;
                else
                {
                    if(_wCurrentYear<CAL_LASTYEAR)
                    {
                        _wCurrentYear++;
                        _bCurrentMonth = 1;
                    }
                }
                
                _bLastDay = Calendar_GetDaysofMonth(_wCurrentYear, _bCurrentMonth);
                calendar_draw_all();
            }
            else  if(__bCalSelectMenuID==DATA_CHANGE)
            {
                if( __bCalselectdata>7 )
                {
                    calendar_draw_item_HighLight(__bCalselectdata, FALSE);
                    __bCalselectdata -=7;
                    calendar_draw_item_HighLight(__bCalselectdata, TRUE);
                }
            }
            
            break;
        case KEY_LEFT:
            if (_bCALENDARMode == CALENDAR_MODE_DISPLAY_PERPETUAL_CALENDAR)
            {
                __bCalSelectMenuID = YEAR_MONTH_CHANGE;
            }

            if(__bCalSelectMenuID==YEAR_MONTH_CHANGE)
            {
                if(_wCurrentYear>CAL_FIRSTYEAR)
                    _wCurrentYear--;
                else
                    _wCurrentYear=CAL_LASTYEAR;
                _bLastDay = Calendar_GetDaysofMonth(_wCurrentYear, _bCurrentMonth);
                calendar_draw_all();
            }
            else  if(__bCalSelectMenuID==DATA_CHANGE)
            {
                _bCalX = Calendar_SolarCalendar(_wCurrentYear,_bCurrentMonth,__bCalselectdata);
                if((_bCalX>0)&&(__bCalselectdata>1))
                {
                    
                    calendar_draw_item_HighLight(__bCalselectdata, FALSE);
                    __bCalselectdata --;
                    calendar_draw_item_HighLight(__bCalselectdata, TRUE);
                }
            }
            
            break;
            
        case KEY_RIGHT:
            if (_bCALENDARMode == CALENDAR_MODE_DISPLAY_PERPETUAL_CALENDAR)
            {
                __bCalSelectMenuID = YEAR_MONTH_CHANGE;
            }

            if(__bCalSelectMenuID==YEAR_MONTH_CHANGE)
            {
                if(_wCurrentYear<CAL_LASTYEAR)
                    _wCurrentYear++;
                else
                    _wCurrentYear=CAL_FIRSTYEAR;
                
                _bLastDay = Calendar_GetDaysofMonth(_wCurrentYear, _bCurrentMonth);
                calendar_draw_all();
            }
            else  if(__bCalSelectMenuID==DATA_CHANGE)
            {
                _bCalX = Calendar_SolarCalendar(_wCurrentYear,_bCurrentMonth,__bCalselectdata);
                if((_bCalX<6)&&(__bCalselectdata<_bLastDay))
                {
                    calendar_draw_item_HighLight(__bCalselectdata, FALSE);
                    __bCalselectdata ++;
                    calendar_draw_item_HighLight(__bCalselectdata, TRUE);
                }
            }
            break;
            
        case KEY_PLAY:
        case KEY_ENTER:
        case KEY_PLAY_PAUSE:
            if (_bCALENDARMode == CALENDAR_MODE_SETTING)
            {
                _bCurrentDate = __bCalselectdata;
                _CALENDAR_Set_YYMMDD();
                calendar_draw_title(FALSE);
            }
            break;           
        case KEY_FUNCTION:
        case KEY_EXIT:
            if (_bCALENDARMode == CALENDAR_MODE_SETTING)
            {
                CALENDAR_Exit(TRUE);
                POWERONMENU_BackToMainMenu();
            }
#ifdef SUPPORT_SET_DATE_BY_MENU
            else if (_bCALENDARMode == CALENDAR_MODE_DISPLAY_PERPETUAL_CALENDAR)
            {
                _bCALENDARMode = CALENDAR_MODE_MENU;
                _CALENDAR_ShowMenu();
            }
#endif //SUPPORT_SET_DATE_BY_MENU
            return KEY_NO_KEY;            
        default:
            break;
        }
    }
    
    return KEY_NO_KEY;    
}

void CALENDAR_Exit(BYTE bClearBackground)
{
    __bCalselectdata = 0;
    __bCalSelectMenuID =0;
    _bCurrentDate= 0;
    _bCurrentMonth =0;
    _wCurrentYear=0;

    if (bClearBackground)
    {
        GDI_ClearRegion(0);
    }

#ifdef SUPPORT_SET_DATE_BY_MENU
    if (_hCALENDARDateMenu != MENU_HANDLE_INVALID)
    {
        MENU_DeleteMenu(_hCALENDARDateMenu, FALSE);
        _hCALENDARDateMenu = MENU_HANDLE_INVALID;
    }

    if (_hCALENDARSetDate != SETDATE_HANDLE_INVALID)
    {
        SETDATE_Exit(_hCALENDARSetDate, FALSE);
    }
#endif //SUPPORT_SET_DATE_BY_MENU
}

//get data from clock,now use constant
void  _CALENDAR_Set_YYMMDD(void)
{
    RTC_TM Calendar;

    RTC_GetTime(&Calendar);

#ifdef CT950_STYLE
    Calendar.bYear = _wCurrentYear-2000;
    Calendar.bMon = _bCurrentMonth;
    Calendar.bDate = _bCurrentDate;

    RTC_SetTime(&Calendar);
#endif //CT950_STYLE
}

BYTE _CALENDAR_ShowTime(BYTE bImmediate)
{
    RTC_TM RTCTime;

    if (!bImmediate)
    {        
        if ((OS_GetSysTimer() - _dwCALENDARTriggerTime) > COUNT_500_MSEC)
        {
            RTC_GetTime(&RTCTime);
            
            if ((_CALENDARCurrnetRTCTime.bSec == RTCTime.bSec) &&
                (_CALENDARCurrnetRTCTime.bMin == RTCTime.bMin) &&
                (_CALENDARCurrnetRTCTime.bHr == RTCTime.bHr))
            {
                return FALSE;
            }
            else
            {   
                if ((RTCTime.bSec == 0) && (RTCTime.bMin == 0) && (RTCTime.bHr == 0))
                {
                    if (CALENDA_Get_YYMMDD())
                    { 
                        _wCurrentYear = Calenda_Info.u16CalenderYear;
                        _bCurrentMonth =Calenda_Info.u8CalenderMonth;
                        _bCurrentDate = Calenda_Info.u8CalenderDate;
                    }

                    __bCalSelectMenuID=DATA_CHANGE;
                    __bCalselectdata=_bCurrentDate; 
                    calendar_draw_all();
                }   

                _CALENDAR_ShowTimeContent(&RTCTime);
                
                _CALENDARCurrnetRTCTime.bSec = RTCTime.bSec;
                _CALENDARCurrnetRTCTime.bMin = RTCTime.bMin;
                _CALENDARCurrnetRTCTime.bHr = RTCTime.bHr;
                
                _dwCALENDARTriggerTime = OS_GetSysTimer();
            }
        }
    }
    else
    {
        RTC_GetTime(&RTCTime);
        _CALENDAR_ShowTimeContent(&RTCTime);
    }
        
    return TRUE;
}

void _CALENDAR_ShowTimeContent(PRTC_TM pRTCTime)
{
    PARM_DRAW_STRING strTime;

    if ((pRTCTime->bHr == 0) && (pRTCTime->bMin == 0) && (pRTCTime->bSec == 0))
    {
         calendar_draw_title(FALSE);
    }

    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex(pRTCTime->bHr/10);    
    OSDUTL_OutputNumFromIndex(pRTCTime->bHr%10);
    OSDUTL_CopyStringFromIndex(aCALENDARColon);
    OSDUTL_OutputNumFromIndex(pRTCTime->bMin/10);    
    OSDUTL_OutputNumFromIndex(pRTCTime->bMin%10);
    OSDUTL_CopyStringFromIndex(aCALENDARColon);
    OSDUTL_OutputNumFromIndex(pRTCTime->bSec/10);    
    OSDUTL_OutputNumFromIndex(pRTCTime->bSec%10);
    
    strTime.bBackground = _bHeadBackColor;
    strTime.bColorKey = 0;
    strTime.bTextColor =_bHeadFront2Color;    
    strTime.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    strTime.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    strTime.wX = _wCALENDARUpdateTimePosX+CALENDAR_DATA_TIME_DISTANCE_H;
    strTime.wY = _wCALENDARUpdateTimePosY;
    
    GDI_DrawString_909(0, &strTime, __pOSDUTLOutputString);
}


void CALENDAR_Trigger(void)
{
    if (_bCALENDARMode == CALENDAR_MODE_DISPLAY_CLOCK_AND_TIME)
    {
        //update time
        _CALENDAR_ShowTime(FALSE);
    }
}

#ifdef SUPPORT_SET_DATE_BY_MENU
void _CALENDAR_ShowMenu(void)
{    
    MAINMENU_InitializeGDI(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
    MAINMENU_DrawMainBackground();
    MAINMENU_DrawSelectionHelp(FALSE, FALSE);
    
    //Draw the title
    MAINMENU_DrawTitleString(aPOWERONIconCalendar, NULL, NULL);
    
    if (_hCALENDARDateMenu == MENU_HANDLE_INVALID)
    {
        _hCALENDARDateMenu = MENU_CreateMenu(&_MENU_CALENDAR, CALENDAR_MAX_VISIBLE_MENU_ITEM_NUM, _bCALENDARExitKey);
    }
    
    MENU_ShowMenu(_hCALENDARDateMenu, TRUE, MAINMENU_PALETTE_ENTRY_CONTENT_REGION);        
}

void CALENDAR_SetDate(WORD wYear, BYTE bMonth, BYTE bDate)
{
    Calenda_Info.u16CalenderYear  = wYear;
    Calenda_Info.u8CalenderMonth = bMonth;
    Calenda_Info.u8CalenderDate = bDate;

    _wCurrentYear = Calenda_Info.u16CalenderYear;
    _bCurrentMonth =Calenda_Info.u8CalenderMonth;
    _bCurrentDate = Calenda_Info.u8CalenderDate;

    _CALENDAR_Set_YYMMDD();
        
    SETDATE_Exit(_hCALENDARSetDate, TRUE);
    MENU_ShowMenu(_hCALENDARDateMenu, TRUE, MAINMENU_PALETTE_ENTRY_CONTENT_REGION); 

    _bCALENDARMode = CALENDAR_MODE_MENU;
}

void CALENDAR_CancelSetDate(void)
{
    SETDATE_Exit(_hCALENDARSetDate, TRUE);
    MENU_ShowMenu(_hCALENDARDateMenu, TRUE, MAINMENU_PALETTE_ENTRY_CONTENT_REGION); 

    _bCALENDARMode = CALENDAR_MODE_MENU;
}

#endif //SUPPORT_SET_DATE_BY_MENU