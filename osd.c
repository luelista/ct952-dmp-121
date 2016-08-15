//#include  "ctkav.h"
#include "winav.h"
#include "infofilter.h"
#include "srcfilter.h"
#include "osd.h"
#include "gdi.h"
#include "osddsply.h"
#include "osdnd.h"
#include "cc.h"
#include "setup.h"
#include "hal.h"
#include "thumb.h"
#include "osdmm.h"
#include "osddlg.h"
#include "utl.h"
#include "osdsrch.h"
#include "osdprgm.h"
#include "media.h"
#include "osdbmark.h"
#include "mmanager.h"
#include "osddg.h"
#include "osdss.h"
#include "digest.h"
#include "char_subpict.h"
#include "osd3.h"
#if defined(SUPPORT_STB) || defined(IMAGE_FRAME_SETUP) //CoCo2.38
#include "mainmenu.h"
#endif //
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
#include "poweronmenu.h"
#endif //#ifdef SUPPORT_POWERON_MENU

#ifdef SUPPORT_RADIO_TUNER
#include "radio.h"
#endif //SUPPORT_RADIO_TUNER

//The depth for each corner's message stack
#define OSD_MAX_DEPTH_OF_MSG_STACK      4

//The depth for UI's stack  
//Currently, we have 5 layers in CT909 UI. But layer 0 (normal display) and layer 4 (screen saver) will not be push to the stack.
//Therefore, the maximum depth for UI stack is 3. (for layer 1 to layer 3)
#define OSD_MAX_DEPTH_OF_UI             6

BYTE aMsgChk[MAX_MSGNO_FOR_CHECK] =
{
#include "msgcheck.txt" //CoCo, should check it for CT909 new message refinement
};

/////////////////////////////////////////////////////////////////////
// ***** Global Variable Area *****
STATUS  gcShowStatus;

BYTE _bOSDLang;     // keep current osd language
BYTE    __bMsgString[MAXNO_OF_MSG_STRING+1]; // CoCo, should remove it in CT909


OSDMESSAGE _OSDNUCurMsg;
OSDMESSAGE _OSDNBCurMsg;
OSDMESSAGE _OSDNUStack[OSD_MAX_DEPTH_OF_MSG_STACK];
OSDMESSAGE _OSDNBStack[OSD_MAX_DEPTH_OF_MSG_STACK];
BYTE _bOSDNUIndex;
BYTE _bOSDNBIndex;
BYTE _bOSDCurrentUI; //Keep the current UI.
BYTE _OSDUIStack[OSD_MAX_DEPTH_OF_UI]; //The UI's stack for recovery's usage.
BYTE _bOSDUIStackIndex; //The index to record the stack.
DWORD _dwOSDNUStartTickCount; // the reference start ticks for down count
WORD _wOSDNUDisplayTickCount; // OSD display ticks, down count in OSD_Trigger
DWORD _dwOSDNBStartTickCount; // the reference start ticks for down count
WORD _wOSDNBDisplayTickCount; // OSD display ticks, down count in OSD_Trigger

//Alan2.21, OSD Buffer mode info
OSDBUF_INFO _OSDBuffer_Info;

// record the Scan or slow factor
BYTE _bSpeedFactor; // only support 0 ~ 255
BYTE _bWaitingCnt; //CoCo, this variable is for "waiting..." usage. Need to move it to OSDND?
PARM_RECT _OSDRect;
BYTE _bOSDTemp, _bOSDTemp1;
BYTE __bOSDClearNDURegion;
BYTE __bOSDClearNDBRegion;
BYTE __bOSDForceToClearNBRegion;

//CoCo1.10, support OSD dynamic region
//The following variables are for OSD dynamic region.
BYTE __bOSDDynamicRegion;
pDynamicRegionRecoverFunc DynamicRegionRecoverFunction;

BYTE _bOSDSetRegion = FALSE;
BYTE _bOSDCurrentRegion;
GDI_REGION_INFO _bOSDCurrentRegionInfo;

extern  BYTE    __bKeyMessage;
extern BYTE _bGDIInit;

extern void OSDPROMPT_Trigger(void);
extern void SETUP_Trigger(void);
extern void SETUP_ConfigOSDRegion(void);
//***************************************************************************
//  Function    :   OSD_PANEL_Output
//  Abstract    :   This function will process the message output for OSD and Panel.
//  Arguments   :   bMessage    : the input message
//                  wParam      : the parameter for this message
//                  bSec        : the display tome for this message
//                  =0x0        : clear the pre-display message
//                  =0xFF       : means forever display
//                  =other value: unit is second.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSD_PANEL_Output(BYTE bMessage, WORD wParam, BYTE bSec)
{   
    OSD_Output(bMessage, wParam, bSec);
    PANEL_Output(bMessage, wParam);
}

//***************************************************************************
//  Function    :   OSD_Output
//  Abstract    :   This function will process the message output for normal 
//              :   display and display mode. 
//  Arguments   :   bMessage    : the input message
//                  wParam      : the parameter for this message
//                  bSec        : the display tome for this message
//                  =0x0        : clear the pre-display message
//                  =0xFF       : means forever display
//                  =other value: unit is second.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSD_Output(BYTE bMessage, WORD wParam, BYTE bSec)
{    
    if ((bMessage == MSG_NULL) || (_bGDIInit == FALSE))
        return;
    
    //1. Update gcShowStatus
    _OSD_UpdateStatus(bMessage, wParam);

    //2. Remove specific messages when receiving bMessage.
    _OSD_UpdateMessage(bMessage);

    if (bMessage == MSG_DISPLAY)
    {
        if (bSec == 0xFF)
        {
            OSDDSPLY_Update(MSG_INITIAL_PALETTE, 0);
            OSDDSPLY_Update(MSG_DISPLAY, OSD_CHANGE_UI);
        }

        return;
    }
    
    if (_bOSDCurrentUI == OSD_UI_DISPLAY)
    {
        _OSD_ClearMsg(MSG_TVMODE);

        if ((bMessage >= MSG_PLAYING) && (bMessage <= MSG_FREEZE))
        {
            //Update status. Send MSG_STATUS to Display mode to update the playing status.
            OSDDSPLY_Update(MSG_STATUS, wParam);
        }
        
        //Judge if this is Display mode message.
        if (OSDDSPLY_Update(bMessage, wParam)) //This is a Display mode message.
        {
            if ((bSec == 0) && (OSDND_GetMessagePos(bMessage) == OSD_ND_BOTTOM_REGION))
            {
                if (_OSD_SearchMsg(bMessage))
                {
                    _OSD_ClearMsg(bMessage); //Clear the forever message from the stack even Display mode has processed it.
                }
            }
         
            if (bMessage != MSG_ANGLE_CTL) //CoCo1.05, Let normal display process "MSG_ANGLE_CTL" even the virtual panel has processed it.
                return;
        }        
    }

#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
	//CoCo1.11, when title/chapter
	if (_bOSDCurrentUI == OSD_UI_NAVIGATOR)
	{
		OSDDSPLY_NAVIGATOR_ClearMenuForNVChange(bMessage);
	}    
#endif //#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
    
    if (bSec == 0) //Our F/W forces to clear the message 
    {
        if ((bMessage != _OSDNUCurMsg.bMessage) && (bMessage != _OSDNBCurMsg.bMessage))
        {
            //The message maybe a forever message and has been put to the stack before.
            //Therefore, need to clear it if is is in the stack.
            if (_OSD_SearchMsg(bMessage))
            {
                _OSD_ClearMsg(bMessage);
            }
            
            //CoCo0.72, MSG_NUMBER needs to clear MSG_KEY10
            if ((bMessage == MSG_NUMBER) && (_OSDNUCurMsg.bMessage == MSG_KEY10))
            {
                //Let MSG_NUMBER to clear MSG_KEY10
            }
            else if (bMessage == MSG_PLAYING) //CoCo2.17, fix the bug about KEY_REPEAT_AB==>KEY_REPEAT_AB==>KEY_PAUSE==>KEY_PLAY can't recover MSG_REPEATAB.
            {
                if (_bOSDNUIndex > 0) //The stack has forever message to be recovered.
                {
                    _OSD_RecoverNURegion();
                }
                return;
            }   
            else            
                return;
        }

        _bOSDTemp = OSDND_GetMessagePos(bMessage);
        
        //Notify the OSDND to clear the message.        
        _OSD_ClearNUNBRegion(_bOSDTemp);

        if (_bOSDTemp == OSD_ND_UPPER_RIGHT_REGION)
        {   
            //Set message to MSG_NULL first. If there is any forever message to be recovered, it will be set later.
            _OSDNUCurMsg.bMessage = MSG_NULL;

            //Recover the normal upper region.
            _OSD_RecoverNURegion();
        }
        else if (_bOSDTemp == OSD_ND_BOTTOM_REGION)
        {
            //Set message to MSG_NULL first. If there is any forever message to be recovered, it will be set later.
            _OSDNBCurMsg.bMessage = MSG_NULL;

            //Recover the normal bottom region.
            _OSD_RecoverNBRegion();
        }
    }
    else //bSec != 0=>F/W send the message to show it.
    {
        if (OSDND_GetMessagePos(bMessage) == OSD_ND_UNKNOWN_REGION)
            return;

        //CoCo, need to check the following flow for __bKeyMessage
        if(__bKeyMessage!=MSG_ALL && bSec!=0xFF)
        {
            // Must check the message in aMsgChk[] array
            if(aMsgChk[bMessage]==1)
            {
                // The message is sent by correspond key:
                // only clear __bKeyMessage, other flow is same as original case
                if(bMessage==__bKeyMessage)
                {
                    __bKeyMessage=MSG_NULL;
                }
                else
                {
                    // filter special slide show case KEY_NEXT/PREV and then "MSG_PLAYING"
                    // in this case we hope retrun, not to clear the OSD "NEXT" or "PREV"
                    if ((_OSDNUCurMsg.bMessage == MSG_NEXT || _OSDNUCurMsg.bMessage == MSG_PREV)
                        && bMessage == MSG_PLAYING)
                    {
                        return;
                    }

                    //clear the message.
                    if ((_OSD_MessageGroup(_OSDNUCurMsg.bMessage) != _OSD_MessageGroup(bMessage)))
                    {
                        // it is just a call to set some MSG to NULL
                        return;
                    }
                    else
                    {
                        _OSD_ClearDisplayRegion(bMessage);

                        //CoCo0.63, add the code to clear the curernt message.
                        _bOSDTemp = OSDND_GetMessagePos(bMessage);
                        if (_bOSDTemp == OSD_ND_UPPER_RIGHT_REGION)
                        {
                            _OSDNUCurMsg.bMessage = MSG_NULL;
                        }
                        else if (_bOSDTemp == OSD_ND_UPPER_RIGHT_REGION)
                        {
                            _OSDNBCurMsg.bMessage = MSG_NULL;
                        }
                        
                        return;
                    }
                }
            }
        }

        //If the normal upper or normal bottom message is a forever message, we will push it to the stack.
        if (OSDND_GetMessagePos(bMessage) == OSD_ND_UPPER_RIGHT_REGION)
        {
            if (_OSDNUCurMsg.bMessage != MSG_NULL)
            {
                if (_OSD_MessageGroup(_OSDNUCurMsg.bMessage) != _OSD_MessageGroup(bMessage))
                    _OSD_PushForeverMsg(_OSDNUCurMsg.bMessage);
                
                //judge if the message should be cleared in normal upper region or normal bottom region.
                if (OSDND_GetMessagePos(_OSDNUCurMsg.bMessage) == OSDND_GetMessagePos(bMessage))
                {
                    _OSD_ClearDisplayRegion(bMessage);            
                }
            }
            else
            {
                //Always clear the region to prevent some UI has info. in that region. ex. Thumbnail UI.
                if (_bOSDCurrentUI != OSD_UI_NONE)
                {
                    _OSD_ClearNUNBRegion(OSD_ND_UPPER_RIGHT_REGION);
                }             
            }
        }
        else if (OSDND_GetMessagePos(bMessage) == OSD_ND_BOTTOM_REGION)
        {
            if (_OSDNBCurMsg.bMessage != MSG_NULL)
            {
                if (_OSD_MessageGroup(_OSDNBCurMsg.bMessage) != _OSD_MessageGroup(bMessage))
                {
                    if (OSDND_GetMessagePos(_OSDNBCurMsg.bMessage) == OSDND_GetMessagePos(bMessage))
                        _OSD_PushForeverMsg(_OSDNBCurMsg.bMessage);
                }            
                
                //judge if the message should be cleared in normal upper region or normal bottom region.
                if (OSDND_GetMessagePos(_OSDNBCurMsg.bMessage) == OSDND_GetMessagePos(bMessage))
                {
                    _OSD_ClearDisplayRegion(bMessage); //Just clear the position of the input message because the current message's position is the same as input message.
                }
            }
            else
            {
                //Always clear the region to prevent some UI has info. in that region. ex. Thumbnail UI.
                if (_bOSDCurrentUI != OSD_UI_NONE)
                {
                    _OSD_ClearNUNBRegion(OSD_ND_BOTTOM_REGION);
                }             
            }
        }

        //Set the input message to current message.
        if (bSec != 0)
        {
            _OSD_SetCurrentMsg(bMessage, wParam, bSec);

            //Call normal display entry to show the message.
            OSDND_Update(bMessage, wParam);
        }
    }
}

//  *********************************************************************
//  Function    :   _OSD_MessageGroup
//  Abstract    :   
//  Arguments   :   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//  *********************************************************************
BYTE _OSD_MessageGroup(BYTE bMessage)
{
    if(((bMessage>=MSG_PLAYING) && (bMessage<=MSG_FREEZE)) || 
        ((bMessage>=MSG_NEXT) && (bMessage<=MSG_PREV)) )
        return MSG_PLAYING;
    else
        return bMessage;
}

//  *********************************************************************
//  Function    :   _OSD_UpdateStatus
//  Abstract    :   To update the gcShowStatus information.
//  Arguments   :   bMessage: the input message
//                  wParam  : parameter for this message
//                  bSec    : the message display time
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//  *********************************************************************
void _OSD_UpdateStatus(BYTE bMessage, WORD wParam)
{
    switch (bMessage)
    {
    case MSG_TOTALTRACKS:
        gcShowStatus.wTotalTracks=wParam;
        break;
    case MSG_STOPWAIT:
        _bWaitingCnt=2;
        if(wParam)
        {
            // set the system information to gcShowStatus
            gcShowStatus = *((STATUS *) __bpDataPointer);
            __bpDataPointer=NULL;
        }
        break;
    case MSG_ENDLIST:
        gcShowStatus.bPlayingMode=MSG_STOP;
        break;
    case MSG_PBC:
        // update the PBC status
        gcShowStatus.bPBCMode=LOBYTE(wParam);
#ifndef NO_DISC_MODE //CoCo2.37
        if(__wDiscType & BOOK_DVD)
        {
        }
        else
#endif //#ifndef NO_DISC_MODE //CoCo2.37
        {
            gcShowStatus.bStill=LOBYTE(wParam);
        }
        break;
    case MSG_PLAYING:
    case MSG_STEP:
    case MSG_FAST:
    case MSG_STOP:
    case MSG_PAUSE:
    case MSG_SLOW:
    case MSG_SCANFORWARD:
    case MSG_SCANBACKWARD:
    case MSG_FREEZE:
        // update the play mode
        gcShowStatus.bPlayingMode=bMessage;
        // update the speed factor
        // now the speed factor is valid for SCF/SCB/SLOW only
        // OSD display is level * 2
        //_bSpeedFactor=LOBYTE(wParam);
        _bSpeedFactor=1;
        for(_bOSDTemp=0; _bOSDTemp<(BYTE)(wParam); _bOSDTemp++)
        {
            _bSpeedFactor*=2;
        }

        //If the status is SCFx? and cross the track, main flow will send MSG_PLAY to update the playing mode.
        //At this time, we will delete the MSG_SCANFORWARD/MSG_SCANBACKWARD/MSG_FAST/MSG_SLOW if they are in the stack.

        break;
    case MSG_MOTION:
        // update the play item mode
        // wParam=TRUE, means play motion, otherwise play STILL
        gcShowStatus.bStill=!LOBYTE(wParam);
        break;
    case MSG_OPEN:
    case MSG_POWER:
        if (bMessage == MSG_POWER)
            gcShowStatus.bPowerDown=LOBYTE(wParam);
        
        // set play mode as STOP
        gcShowStatus.bPlayingMode=MSG_STOP;
        if (bMessage == MSG_POWER)
        {
            //CoCo, update __bLOGO variable in OSD kernel?
            __bLOGO=LOGO_NONE;
            
            // clear OSD when power down
            if (wParam)
            {
                GDI_ClearRegion(0);
            }
        }
        break;
    case MSG_TRACK:
        // update the current play track
        gcShowStatus.wCurrentTrack=wParam;
        // LLY2.53, expand the time value from WORD to DWORD unit
        // Since, we will use [23:16]:hour, [15:8]:min, [7:0]: sec
        //gcShowStatus.wCurrentTime=0;
        gcShowStatus.dwCurrentTime=0;
        break;		
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   _OSD_UpdateMessage
//  Abstract    :   This function will some specific messages when receiving 
//              :   bMessage.
//  Arguments   :   bMessage: the input message
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_UpdateMessage(BYTE bMessage)
{
	switch(bMessage)
    {
    case MSG_WAITING:           
        _OSD_ClearMsg(MSG_OPEN); //Clear MSG_OPEN because MSG_WAITING and MSG_OPEN are excursive
        break;		
    case MSG_STOPWAIT:        
        _OSD_ClearMsg(MSG_WAITING); //Clear MSG_WAITING
        break;		
    case MSG_STOP:        
        _OSD_ClearMsg(MSG_ANGLE_CTL); //Clear MSG_ANGLE_CTL
        break;
    case MSG_OPEN:
    case MSG_POWER:
        _OSD_ClearMsg(MSG_WAITING); //Clear MSG_WAITING
        _OSD_ClearMsg(MSG_DISCTYPE); //Clear MSG_DISCTYPE
        _OSD_ClearMsg(MSG_ANGLE_CTL); //Clear MSG_ANGLE_CTL
        _OSD_ClearMsg(MSG_ABNORMAL_DISC); //Clear MSG_ABNORMAL_DISC
        break;
        //CoCo0.76, clear the playing message from the stack.
    case MSG_PLAYING:
    case MSG_STEP:
    case MSG_FAST:
    case MSG_PAUSE:
    case MSG_SLOW:
    case MSG_SCANFORWARD:
    case MSG_SCANBACKWARD:
    case MSG_FREEZE:
        _OSD_ClearMsg(MSG_STEP);
        _OSD_ClearMsg(MSG_FAST);
        _OSD_ClearMsg(MSG_PAUSE);
        _OSD_ClearMsg(MSG_SLOW);
        _OSD_ClearMsg(MSG_SCANFORWARD);
        _OSD_ClearMsg(MSG_SCANBACKWARD);
        _OSD_ClearMsg(MSG_FREEZE);
        break;
    case MSG_REPEATAB: 
        //CoCo0.91, Clear MSG_REPEATAB because our flow will not send bSec == 0 to clear MSG_REPEATAB. 
        //If MSG_REPEATAB is overwritten by other forever message, it may not be cleared in the stack.
        _OSD_ClearMsg(MSG_REPEATAB);
        break;
    case MSG_MUTE:
        _OSD_ClearMsg(MSG_MUTE);
        break;
    default:
        break;
    }	
}

//***************************************************************************
//  Function    :   _OSD_PushForeverMsg
//  Abstract    :   This function will push the forever message to the stack.
//  Arguments   :   bMessage: the input message.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_PushForeverMsg(BYTE bMessage)
{
    //Get message's position
    _bOSDTemp = OSDND_GetMessagePos(bMessage);
    
    switch (_bOSDTemp)
    {
    case OSD_ND_UPPER_RIGHT_REGION:
        if (_OSDNUCurMsg.bMessage != MSG_NULL)
        {
            if (_OSDNUCurMsg.bSec == 0xFF)
            {
                if (_OSD_SearchMsg(bMessage))
                    return;

                //push the forever message to the stack.
                _OSDNUStack[_bOSDNUIndex].bMessage = _OSDNUCurMsg.bMessage;
                _OSDNUStack[_bOSDNUIndex].wParam = _OSDNUCurMsg.wParam;
                _OSDNUStack[_bOSDNUIndex].bSec = _OSDNUCurMsg.bSec;
                _bOSDNUIndex++;
            }
        }
        break;
    case OSD_ND_BOTTOM_REGION:
        if (_OSDNBCurMsg.bMessage != MSG_NULL)
        {
            if (_OSDNBCurMsg.bSec == 0xFF)
            {
                if (_OSD_SearchMsg(bMessage))
                    return;

                //push the forever message to the stack.
                _OSDNBStack[_bOSDNBIndex].bMessage = _OSDNBCurMsg.bMessage;
                _OSDNBStack[_bOSDNBIndex].wParam = _OSDNBCurMsg.wParam;
                _OSDNBStack[_bOSDNBIndex].bSec = _OSDNBCurMsg.bSec;
                _bOSDNBIndex++;
            }
        }
        break;
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   _OSD_PushForeverMsg
//  Abstract    :   This function will push the forever message to the stack.
//  Arguments   :   bMessage: the input message.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSD_SearchMsg(BYTE bMessage)
{
    //Get message's position
    _bOSDTemp = OSDND_GetMessagePos(bMessage);
    
    switch (_bOSDTemp)
    {
    case OSD_ND_UPPER_RIGHT_REGION:
        for (_bOSDTemp = 0; _bOSDTemp < _bOSDNUIndex; _bOSDTemp++)
        {
            if (_OSDNUStack[_bOSDTemp].bMessage == bMessage)
                return TRUE;
        }
        break;
    case OSD_ND_BOTTOM_REGION:
        for (_bOSDTemp = 0; _bOSDTemp < _bOSDNBIndex; _bOSDTemp++)
        {
            if (_OSDNBStack[_bOSDTemp].bMessage == bMessage)
                return TRUE;
        }
        break;
    default:
        break;
    }

    return FALSE;
}


//***************************************************************************
//  Function    :   _OSD_SetCurrentMsg
//  Abstract    :   This function will set the message to normal upper or bottom message.
//  Arguments   :   bMessage: the input message.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_SetCurrentMsg(BYTE bMessage, WORD wParam, BYTE bSec)
{
    //Get message's position
    _bOSDTemp = OSDND_GetMessagePos(bMessage);

    switch (_bOSDTemp)
    {
    case OSD_ND_UPPER_RIGHT_REGION:
        //Save current normal upper message
        _OSDNUCurMsg.bMessage = bMessage;
        _OSDNUCurMsg.wParam = wParam;
        _OSDNUCurMsg.bSec = bSec;
        //Save the display time for normal upper message.
        _wOSDNUDisplayTickCount = (WORD)((WORD)bSec*COUNT_1_SEC); 
        _dwOSDNUStartTickCount = OS_GetSysTimer();
        break;
    case OSD_ND_BOTTOM_REGION:
        //Save current normal bottom message
        _OSDNBCurMsg.bMessage = bMessage;
        _OSDNBCurMsg.wParam = wParam;
        _OSDNBCurMsg.bSec = bSec;
        //Save the display time for normal bottom message.
        _wOSDNBDisplayTickCount = (WORD)((WORD)bSec*COUNT_1_SEC); 
        _dwOSDNBStartTickCount = OS_GetSysTimer();
        break;
    default:
        break;
    }
}


//***************************************************************************
//  Function    :   OSD_Initial
//  Abstract    :   This function will initialize OSD-relative variables.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSD_Initial(void)
{
    //Initialize GDI. The palette is also initialized in this function.
    GDI_Initial();
		
    //Initialize normal display status.
    OSDND_Update(MSG_INITIAL_PALETTE, 0);
    OSDND_Update(MSG_DEFAULT_STATE, 0);

    //Set OSD language.    	
    //_bOSDLang = (__SetupInfo.bOSDLanguage - SETUP_LANGUAGE_OSD_BASE); //--CoCo2.38
	
    _OSD_ResetState();

    GDI_ClearRegion(0); //CoCo.CT909
}

//***************************************************************************
//  Function    :   _OSD_RemoveMsgFromStack
//  Abstract    :   This function will remove the message from stack.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_RemoveMsgFromStack(BYTE bMessage)
{
    //Get message's position
    _bOSDTemp = OSDND_GetMessagePos(bMessage);

    switch (_bOSDTemp)
    {
    case OSD_ND_UPPER_RIGHT_REGION:
        for (_bOSDTemp=0; _bOSDTemp < OSD_MAX_DEPTH_OF_MSG_STACK; _bOSDTemp++)
        {
            if (bMessage == _OSDNUStack[_bOSDTemp].bMessage)
            {
                _OSDNUStack[_bOSDTemp].bMessage = MSG_NULL;
                
                if (_bOSDTemp == (_bOSDNUIndex-1))
                {
                    // we just clear the topest msg from the stack
                    // so decrease index by 1
                    _bOSDNUIndex--;
                }
            }
        }
        break;
    case OSD_ND_BOTTOM_REGION:
        for (_bOSDTemp=0; _bOSDTemp < OSD_MAX_DEPTH_OF_MSG_STACK; _bOSDTemp++)
        {
            if (bMessage == _OSDNBStack[_bOSDTemp].bMessage)
            {
                _OSDNBStack[_bOSDTemp].bMessage = MSG_NULL;
                
                if (_bOSDTemp == (_bOSDNBIndex-1))
                {
                    // we just clear the topest msg from the stack
                    // so decrease index by 1
                    _bOSDNBIndex--;
                }
            }
        }
        break;
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   _OSD_ClearMsg
//  Abstract    :   This function will clear the message and remove the message from the stack.
//  Arguments   :   bMessage: the message that we want to clear.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_ClearMsg(BYTE bMessage)
{
    if ((bMessage == _OSDNUCurMsg.bMessage) || (bMessage == _OSDNBCurMsg.bMessage))
    {
        //clear the message.
        _OSD_ClearNUNBRegion(OSDND_GetMessagePos(bMessage));
        
        if (bMessage == _OSDNUCurMsg.bMessage)
        {
            _OSDNUCurMsg.bMessage = MSG_NULL;
            _OSDNUCurMsg.bSec = 0;
        }
        else if (bMessage == _OSDNBCurMsg.bMessage)
        {
            _OSDNBCurMsg.bMessage = MSG_NULL;
            _OSDNBCurMsg.bSec = 0;
        }
    }

    _OSD_RemoveMsgFromStack(bMessage);
}



//***************************************************************************
//  Function    :   OSD_ChangeUI
//  Abstract    :   This function will change the UI and do recovery for each UI.
//  Arguments   :   bUI: The UI
//              :   bAction : OSD_ENTER_UI==>The UI enters.
//              :           : OSD_EXIT_UI==>The UI exits.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSD_ChangeUI(BYTE bUI, BYTE bAction)
{
    BYTE bRecoverDynamicRegion = FALSE;

    //Recover the normal display attributes.
    OSDND_Update(MSG_DEFAULT_STATE, 0);

    //Reset this flag to prevent volume bar/Key/Echo bar is not re-drawed again.
    //__bOSDClearNDBRegion = TRUE;
    __bOSDForceToClearNBRegion = TRUE;

    if (bAction == OSD_ENTER_UI)
    {
        //judge the UI's layer
        if (_OSD_GetUILayer(bUI) > _OSD_GetUILayer(_bOSDCurrentUI))
        {
            //Push current UI to UI's stack.
            _OSDUIStack[_bOSDUIStackIndex] = _bOSDCurrentUI;
            _bOSDUIStackIndex++;
        }
        else if (_OSD_GetUILayer(bUI) == _OSD_GetUILayer(_bOSDCurrentUI))
        {
            //Call current UI's "exit" function to exit the UI and don't do the recovery.(The UI can't call OSD_ChangeUI(bUI, OSD_EXIT_UI))
            //_OSD_ExitUI(bUI, OSD_UI_EXIT_ALL);
            _OSD_ExitUI(_bOSDCurrentUI, OSD_UI_EXIT_ALL);
        }
        else // The new UI's layer < current UI. This is an illegal case
        {
            return;
        }

        _bOSDCurrentUI = bUI;

#ifdef USE_MINI_OSD_REGION
        if ((_bOSDCurrentUI == OSD_UI_SETUP) || (_bOSDCurrentUI == OSD_UI_MAIN_MENU) || (_bOSDCurrentUI == OSD_UI_SCREEN_SAVER) ||
            (_bOSDCurrentUI == OSD_UI_DVD_PROGRAM) || (_bOSDCurrentUI == OSD_UI_BOOKMARK) || (_bOSDCurrentUI == OSD_UI_MEDIA_SELECT_DLG))
#else
        if ((_bOSDCurrentUI == OSD_UI_SETUP) || (_bOSDCurrentUI == OSD_UI_MAIN_MENU) || (_bOSDCurrentUI == OSD_UI_SCREEN_SAVER))
#endif //USE_MINI_OSD_REGION
        {
            if (_OSDNBCurMsg.bMessage != MSG_NULL && _OSDNBCurMsg.bSec != 0xFF)
            {
                __bOSDClearNDBRegion = TRUE;                    
                _OSDNBCurMsg.bMessage = MSG_NULL;

                if (_bOSDNBIndex > 0) //The stack has forever message to be recovered.
                {
                    _OSDNBCurMsg.bMessage = _OSDNBStack[_bOSDNBIndex-1].bMessage;
                    _OSDNBCurMsg.wParam = _OSDNBStack[_bOSDNBIndex-1].wParam;
                    _OSDNBCurMsg.bSec = _OSDNBStack[_bOSDNBIndex-1].bSec;
                    _bOSDNBIndex--;
                }
            }

            if (_OSDNUCurMsg.bMessage != MSG_NULL && _OSDNUCurMsg.bSec != 0xFF)
            {
                _OSDNUCurMsg.bMessage = MSG_NULL;

                if (_bOSDNUIndex > 0) //The stack has forever message to be recovered.
                {
                    _OSDNUCurMsg.bMessage = _OSDNUStack[_bOSDNUIndex-1].bMessage;
                    _OSDNUCurMsg.wParam = _OSDNUStack[_bOSDNUIndex-1].wParam;
                    _OSDNUCurMsg.bSec = _OSDNUStack[_bOSDNUIndex-1].bSec;
                    _bOSDNUIndex--;
                }
            }
        }
        
        //Config each UI's region
        _OSD_ConfigRegion(_bOSDCurrentUI);

#ifdef SUPPORT_GRAPH_ZOOM
        //Clear PIP
        if ((_bOSDCurrentUI != OSD_UI_DISPLAY) && (__bZoomFactor != VIEW_NORMAL))
        {
            OSDND_Update(MSG_ZOOM, VIEW_NORMAL);
        }
#endif
    }
    else if (bAction == OSD_EXIT_UI)
    {
        //Recover the normal display attributes.
        //OSDND_Update(MSG_DEFAULT_STATE, 0);

        //Setup UI is a special UI that we will recover NU/NB message always.
#ifdef USE_MINI_OSD_REGION
        if ((_bOSDCurrentUI == OSD_UI_SETUP) || (_bOSDCurrentUI == OSD_UI_MAIN_MENU) || (_bOSDCurrentUI == OSD_UI_SCREEN_SAVER) ||
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
            (_bOSDCurrentUI == OSD_UI_POWERON_MENU) ||
#endif
            (_bOSDCurrentUI == OSD_UI_DVD_PROGRAM) || (_bOSDCurrentUI == OSD_UI_BOOKMARK) || (_bOSDCurrentUI == OSD_UI_MEDIA_SELECT_DLG))
#else //USE_MINI_OSD_REGION
        if ((_bOSDCurrentUI == OSD_UI_SETUP) || (_bOSDCurrentUI == OSD_UI_MAIN_MENU) || (_bOSDCurrentUI == OSD_UI_SCREEN_SAVER)
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
            || (_bOSDCurrentUI == OSD_UI_POWERON_MENU)
#endif            
            )
#endif //USE_MINI_OSD_REGION
        {
            //CoCo1.10, support OSD dynamic region
            bRecoverDynamicRegion = TRUE;
            
            if (_OSDNUCurMsg.bSec == 0xFF)
            {
                _OSD_ConfigRegion(OSD_UI_NONE);
                OSDND_Update(_OSDNUCurMsg.bMessage, _OSDNUCurMsg.wParam);
            }
            
            if (_OSDNBCurMsg.bSec == 0xFF)
            {
                _OSD_ConfigRegion(OSD_UI_NONE);
                OSDND_Update(_OSDNBCurMsg.bMessage, _OSDNBCurMsg.wParam);
            }
        }

        //Pop-up the UI from UI's stack.
        //Check if any UI needs to be recovered.
        if (_bOSDUIStackIndex != 0) //Some UI has been pushed to the stack.
        {            
            //Set current UI to the pop-up UI.
            _bOSDCurrentUI = _OSDUIStack[_bOSDUIStackIndex-1];
            _bOSDUIStackIndex--;

            //CoCo2.56, fix the problem about subtitle will be recovered abnormally in 64M solution.
            if ((_bOSDCurrentUI == OSD_UI_SETUP) || (_bOSDCurrentUI == OSD_UI_MAIN_MENU)
#ifdef SUPPORT_POWERON_MENU 
                || (_bOSDCurrentUI == OSD_UI_POWERON_MENU)
#endif            
                )
            {
                bRecoverDynamicRegion = FALSE;
            }

            //Call the pop-up UI's OSDXXX_Recover().
            if (!_OSD_RecoverUI(_bOSDCurrentUI, OSD_RECOVER_ALL))
            {
                //CoCo, fix the problem about "Repeat A-B" message is not recovered after SETUP==>SETUP==>MM UI.
                if (_OSDNUCurMsg.bSec == 0xFF)
                {
                    OSDND_Update(_OSDNUCurMsg.bMessage, _OSDNUCurMsg.wParam);
                }

                if (_OSDNBCurMsg.bSec == 0xFF)
                {
                    OSDND_Update(_OSDNBCurMsg.bMessage, _OSDNBCurMsg.wParam);
                }
            }
        }
        else //The stack doesn't have any UI.
        {
            _bOSDCurrentUI = OSD_UI_NONE; //Normal Display mode.
            _OSD_ConfigRegion(_bOSDCurrentUI);
        }

        if (bRecoverDynamicRegion)
        {
            _OSD_RecoverDynamicRegion();
        }

#ifdef SUPPORT_GRAPH_ZOOM
        //Recover PIP
        //if ((_bOSDCurrentUI != OSD_UI_DISPLAY) && (__bZoomFactor != VIEW_NORMAL))
        if (((_bOSDCurrentUI == OSD_UI_DISPLAY) || (_bOSDCurrentUI == OSD_UI_NONE)) && (__bZoomFactor != VIEW_NORMAL)) //CoCo1.02
        {
            OSDND_Update(MSG_ZOOM, __bZoomFactor);
        }
#endif
    }
}

//***************************************************************************
//  Function    :   _OSD_GetUILayer
//  Abstract    :   This function will get the layer of the UI.
//  Arguments   :   bUI: The UI
//  Return      :   The layer of the UI.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSD_GetUILayer(BYTE bUI)
{
    switch (bUI)
    {
    case OSD_UI_NONE:                   //Normal Display
        _bOSDTemp = OSD_UI_LAYER_0;
        break;
    case OSD_UI_DISPLAY:                //Display mode
        _bOSDTemp = OSD_UI_LAYER_1;
        break;
    case OSD_UI_MEDIA_MANAGER:          //Media Manager
    case OSD_UI_THUMBNAIL:              //Thumbnail
    case OSD_UI_SEARCH:                 //Search UI
    case OSD_UI_DVD_PROGRAM:            //VCD/DVD Program
    case OSD_UI_DIGEST:                 //Digest
    case OSD_UI_BOOKMARK:               //Bookmark
    case OSD_UI_PASSWORD:               //DVD Password Dialog
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
    case OSD_UI_POWERON_MENU:           //Power on Menu //CoCo2.38
#endif //
#ifdef SUPPORT_INDEPENDENT_NAVIGATOR //CoCo1.0, support independent Navigator UI
    case OSD_UI_NAVIGATOR:
#endif
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
    case OSD_UI_COPY_DELETE_DLG:
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
#ifdef SUPPORT_RADIO_TUNER
    case OSD_UI_RADIO_TUNER:
#endif //SUPPORT_RADIO_TUNER		
        _bOSDTemp = OSD_UI_LAYER_2;
        break;
    case OSD_UI_MEDIA_SELECT_DLG:       //Media Select Dialog box
    case OSD_UI_SETUP:                  //SETUP
    case OSD_UI_MAIN_MENU:              //Main Menu
        _bOSDTemp = OSD_UI_LAYER_3;
        break;
    case OSD_UI_PSCAN_PROMPT_DLG:
    case OSD_UI_COMMON_DLG:
        _bOSDTemp = OSD_UI_LAYER_4;
        break;
    case OSD_UI_SCREEN_SAVER:           //Screen Saver
        _bOSDTemp = OSD_UI_LAYER_5;
        break;
    default:                            //unknown UI
        _bOSDTemp = OSD_UI_LAYER_0;
        break;
    }

    return _bOSDTemp;
}

//***************************************************************************
//  Function    :   _OSD_ExitUI
//  Abstract    :   This function will call the UI's "exit" function to notify 
//              :   the UI to exit and don't do recovery.
//  Arguments   :   bUI: the UI. bExitMode: OSD_UI_EXIT_ALL/OSD_UI_EXIT_CONTROL_ONLY.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_ExitUI(BYTE bUI, BYTE bExitMode)
{
    switch (bUI)
    {
    case OSD_UI_DISPLAY:                //Display mode
        OSDDSPLY_Exit(bExitMode);
        break;
    case OSD_UI_MEDIA_MANAGER:          //File Manager        
        MM_Exit(bExitMode);
        break;
    case OSD_UI_THUMBNAIL:              //Thumbnail        
        THUMB_Exit(bExitMode);
        break;
#ifndef NO_SEARCH_UI //++CoCo2.37p
    case OSD_UI_SEARCH:                 //Search UI
        OSDSRCH_Exit(bExitMode);
        break;
#endif //#ifndef NO_SEARCH_UI //++CoCo2.37p
#ifndef NO_DVD_PASSWORD_DLG //CoCo2.38
    case OSD_UI_PASSWORD:               //DVD Password Dialog
        OSDPASSWORD_Exit(bExitMode);
        break;
#endif //NO_DVD_PASSWORD_DLG
#ifndef NO_PROGRAM_UI //++CoCo2.37p
    case OSD_UI_DVD_PROGRAM:            //VCD/DVD Program
        OSDPRGM_Exit(bExitMode);
        break;
#endif //#ifndef NO_PROGRAM_UI //++CoCo2.37p
    case OSD_UI_DIGEST:                 //Digest
        break;
    case OSD_UI_MEDIA_SELECT_DLG:       //Media Select Dialog box
        MEDIA_MediaSelection_Exit(bExitMode);
        break;
    case OSD_UI_PSCAN_PROMPT_DLG:       //The PSCAN prompt dialog box
        break;
    case OSD_UI_BOOKMARK:               //Bookmark
#ifdef SUPPORT_BOOKMARK
        OSDBOOKMARK_Exit(bExitMode);
#endif //
        break;
#ifndef SUPPORT_STB
    case OSD_UI_SETUP:                  //SETUP
#ifdef IMAGE_FRAME_SETUP
        MAINMENU_Exit(bExitMode);
#else
        SETUP_Exit(bExitMode);
#endif //IMAGE_FRAME_SETUP
        break;
#endif //SUPPORT_STB

#ifdef SUPPORT_STB
    case OSD_UI_MAIN_MENU:              //Main Menu
        MAINMENU_Exit(bExitMode);
        break;
#endif //SUPPORT_STB
    case OSD_UI_SCREEN_SAVER:           //Screen Saver
#ifndef NO_SCREEN_SAVER
        OSDSS_Exit(bExitMode);
#endif //
        break;
    case OSD_UI_COMMON_DLG:
        OSDCOMDLG_Exit(bExitMode);         //For common dialog box.
        break;
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
#ifdef SUPPORT_INDEPENDENT_NAVIGATOR //CoCo1.0, support independent Navigator UI
    case OSD_UI_NAVIGATOR:
        OSDDSPLY_NAVIGATOR_Exit(bExitMode);
        break;
#endif //SUPPORT_INDEPENDENT_NAVIGATOR
#endif //#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
    case OSD_UI_POWERON_MENU:           //Power on Menu
        POWERONMENU_Exit(bExitMode);
        break;
#endif //SUPPORT_POWERON_MENU
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
    case OSD_UI_COPY_DELETE_DLG:
        OSDMM_CopyDeleteDlg_Exit(bExitMode);
        break;
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
#ifdef SUPPORT_RADIO_TUNER
    case OSD_UI_RADIO_TUNER:		//Radio Tuner
        RADIO_Exit(bExitMode);
        break;
#endif //SUPPORT_RADIO_TUNER
    default:                            //unknown UI
        break;
    }    
}

//***************************************************************************
//  Function    :   _OSD_RecoverUI
//  Abstract    :   This function will call the UI's OSDXXX_Recover function to 
//              :   recover the UI.
//  Arguments   :   bUI: The UI needs to be recovered.
//              :   bRecoverType:   OSD_RECOVER_ALL==>Recover all region.
//              :                   OSD_RECOVER_UPPER_REGION==>Recover upper region.
//              :                   OSD_RECOVER_BOTTOM_REGION==>Recover bottom region.
//  Return      :   TRUE: It has been recoverd. FALSE: The UI doesn't do recovery for this region.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSD_RecoverUI(BYTE bUI, BYTE bRecoverType)
{
    _bOSDTemp = FALSE;

    if (bRecoverType == OSD_RECOVER_ALL)
    {
        _OSD_ConfigRegion(bUI);
    }

    switch (bUI)
    {
    case OSD_UI_NONE:
        if (_bOSDNBIndex != 0)
        {
            //If we have another forever message displayed currently. Don't do recovery.
            //For example, KEY_ANGLE (push to stack by Display mode UI) ==>KEY_MUTE==>Leave Display mode UI
            if (_OSDNBCurMsg.bMessage == _OSDNBStack[_bOSDNBIndex-1].bMessage)
            {
                _OSD_RecoverNDMessage(OSD_ND_BOTTOM_REGION);
            }
        }

        //CoCo0.90, recover the zoom OSD
        _OSD_RecoverSpecialGraphZoom();

        //CoCo1.10, support OSD dynamic region
        _OSD_RecoverDynamicRegion();
        break;
    case OSD_UI_DISPLAY:                //Display mode
        _bOSDTemp = OSDDSPLY_Recover(bRecoverType);
        //CoCo0.90, recover the zoom OSD
        _OSD_RecoverSpecialGraphZoom();
        break;
    case OSD_UI_MEDIA_MANAGER:          //Media Manager        
        _bOSDTemp = OSDMM_Recover(bRecoverType);
        break;
    case OSD_UI_THUMBNAIL:              //Thumbnail
        _bOSDTemp = THUMB_Recover(bRecoverType);
        break;
#ifndef NO_SEARCH_UI //++CoCo2.37p
    case OSD_UI_SEARCH:                 //Search UI
        _bOSDTemp = OSDSRCH_Recover(bRecoverType);
        break;
#endif //#ifndef NO_SEARCH_UI //++CoCo2.37p
#ifndef NO_DVD_PASSWORD_DLG //CoCo2.38
    case OSD_UI_PASSWORD:               //DVD Password Dialog
        _bOSDTemp = OSDPASSWORD_Recover(bRecoverType);
        break;
#endif //NO_DVD_PASSWORD_DLG
#ifndef NO_PROGRAM_UI //++CoCo2.37p
    case OSD_UI_DVD_PROGRAM:            //VCD/DVD Program
        _bOSDTemp = OSDPRGM_Recover(bRecoverType);
        break;
#endif //#ifndef NO_PROGRAM_UI //++CoCo2.37p
    case OSD_UI_DIGEST:                 //Digest
    // wyc0.95, notify CoCo to update it.
#ifndef NO_DIGEST
        _bOSDTemp = OSDDG_Recover(bRecoverType);
#endif //
        break;
    case OSD_UI_MEDIA_SELECT_DLG:       //Media Select Dialog box
        _bOSDTemp = OSDDLG_MEDIA_SELECT_Recover(bRecoverType);
        break;
    case OSD_UI_BOOKMARK:               //Bookmark
#ifdef SUPPORT_BOOKMARK
        _bOSDTemp = OSDBOOKMARK_Recover(bRecoverType);
#endif //
        break;
#ifndef SUPPORT_STB
    case OSD_UI_SETUP:                  //SETUP
#ifdef IMAGE_FRAME_SETUP //CoCo2.38
        _bOSDTemp = MAINMENU_Recover(bRecoverType);
#else
        _bOSDTemp = SETUP_Recover(bRecoverType);
#endif
        break;
#endif //#ifndef SUPPORT_STB
#ifdef SUPPORT_STB
    case OSD_UI_MAIN_MENU:
        MAINMENU_DVD_Recover(bRecoverType);
        break;
#endif //
    case OSD_UI_PSCAN_PROMPT_DLG:       //The PSCAN prompt dialog box
        //The "PSCAN prompt dialog box" doesn't need to be recovered because it will not be overwritten by other UI.
        break;
    case OSD_UI_SCREEN_SAVER:           //Screen Saver
        //The screen saver UI doesn't need to be recovered because it will not be overwritten by other UI.
        //Each time is a "new start" when screen saver UI is appeared.
        break;
    case OSD_UI_COMMON_DLG:
        _bOSDTemp = OSDCOMDLG_Recover(bRecoverType);
        break;
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
#ifdef SUPPORT_INDEPENDENT_NAVIGATOR //CoCo1.0, support independent Navigator UI
    case OSD_UI_NAVIGATOR:
        _bOSDTemp = OSDNAVIGATOR_Recover(bRecoverType);
        break;
#endif
#endif //#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
    case OSD_UI_POWERON_MENU:
        _bOSDTemp = POWERONMENU_Recover(bRecoverType);
        break;
#endif //#ifdef SUPPORT_POWERON_MENU //CoCo2.38
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
    case OSD_UI_COPY_DELETE_DLG:
        _bOSDTemp = OSDMM_CopyDeleteDlg_Recover(bRecoverType);
        break;
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
#ifdef SUPPORT_RADIO_TUNER
    case OSD_UI_RADIO_TUNER:		//Radio Tuner
        _bOSDTemp = RADIO_Recover(bRecoverType);	
        break;
#endif //SUPPORT_RADIO_TUNER
    default:                            //unknown UI
        break;
    }
    
    return _bOSDTemp;
}

//***************************************************************************
//  Function    :   OSD_Trigger
//  Abstract    :   This function will call the UI's OSDXXX_Recover function to 
//              :   recover the UI.
//  Arguments   :   bType: OSD_TRIGGER_CONTROL/OSD_TRIGGER_TIME
//              :   wTime: It's valid only when OSD_TRIGGER_TIME
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
// LLY2.53, expand time value unit from WORD to DWORD
// Since, we will use [23:16]:hour, [15:8]: min, [7:0]: sec
//void OSD_Trigger(BYTE bType, WORD wTime)
void OSD_Trigger(BYTE bType, DWORD dwTime)
{
    // LLY2.53, also need check 0xFFFFFFFF since OSD time unit is expand from WORD to DWORD
    //if ((bType == OSD_TRIGGER_TIME) && (wTime != 0xFFFF))
    if ((bType == OSD_TRIGGER_TIME) && (dwTime != 0xFFFFFFFF) && (dwTime != 0xFFFF) )
    {
        // update current time
        // LLY2.53, expand the time value from WORD to DWORD unit
        //gcShowStatus.wCurrentTime=wTime;
        gcShowStatus.dwCurrentTime=dwTime;
    }

    //Call each UI's trigger function
    if (_bOSDCurrentUI != OSD_UI_NONE)
    {
        // LLY2.53, expand the time value from WORD to DWORD unit
        //_OSD_UITrigger(bType, wTime);
        _OSD_UITrigger(bType, dwTime);
    }

    //Normal upper message time out?
    if (_OSDNUCurMsg.bMessage != MSG_NULL && _OSDNUCurMsg.bSec != 0xFF)
    {
        // count display time
        if (OS_GetSysTimer() > (_dwOSDNUStartTickCount+_wOSDNUDisplayTickCount))   
        {
            if (!(
                (_bOSDCurrentUI == OSD_UI_SETUP) 
                || (_bOSDCurrentUI == OSD_UI_MAIN_MENU) 
                || (_bOSDCurrentUI == OSD_UI_SCREEN_SAVER)
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
                || (_bOSDCurrentUI == OSD_UI_POWERON_MENU)
#endif
                ))
            {
                // Current message is time out.
                //_OSD_ClearNUNBRegion(OSDND_GetMessagePos(_OSDNUCurMsg.bMessage));
                _OSD_ClearNUNBRegion(OSD_ND_UPPER_RIGHT_REGION);
                
                _OSDNUCurMsg.bMessage = MSG_NULL;
                
                //Recover the normal upper region.
                _OSD_RecoverNURegion();
            }
        }
    }
    
    //Normal bottom message time out?
    if (_OSDNBCurMsg.bMessage != MSG_NULL && _OSDNBCurMsg.bSec != 0xFF)
    {
        // count display time
        if (OS_GetSysTimer() > (_dwOSDNBStartTickCount+_wOSDNBDisplayTickCount))   
        {
            // current message is time out
            //_OSD_ClearNUNBRegion(OSDND_GetMessagePos(_OSDNBCurMsg.bMessage));
            _OSD_ClearNUNBRegion(OSD_ND_BOTTOM_REGION);

            //Reset this flag to prevent volume bar/Key/Echo bar is not re-drawed again.
            __bOSDClearNDBRegion = TRUE;

            _OSDNBCurMsg.bMessage = MSG_NULL;
            
            //Recover the normal bottom region.
            _OSD_RecoverNBRegion();
            
            //CoCo, Need to notify Panel module to clear the message?
        }
    }
}

//***************************************************************************
//  Function    :   _OSD_UITrigger
//  Abstract    :   This function will call each UI's trigger function to update the UI.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
// LLY2.53, expand time value unit from WORD to DWORD
// Since, we will use [23:16]:hour, [15:8]: min, [7:0]: sec
//void _OSD_UITrigger(BYTE bType, WORD wTime)
void _OSD_UITrigger(BYTE bType, DWORD dwTime)
{
    switch (_bOSDCurrentUI)
    {
    case OSD_UI_DISPLAY:                //Display mode
        // LLY2.53, expand time value unit from WORD to DWORD
        //OSDDSPLY_Trigger(bType, wTime);
        OSDDSPLY_Trigger(bType, dwTime);
        break;
    case OSD_UI_MEDIA_MANAGER:          //Media Manager
        // LLY2.53, expand time value unit from WORD to DWORD
        //OSDMM_Trigger(bType, wTime);
        OSDMM_Trigger(bType, dwTime);
        break;
    case OSD_UI_THUMBNAIL:              //Thumbnail
        // LLY2.53, expand time value unit from WORD to DWORD
        //THUMB_UI_Trigger(bType, wTime);
        THUMB_UI_Trigger(bType, dwTime);
        break;
    case OSD_UI_SEARCH:                 //Search UI
        break;
    case OSD_UI_PASSWORD:               //DVD Password Dialog
        break;
    case OSD_UI_DVD_PROGRAM:            //VCD/DVD Program
        break;
    case OSD_UI_DIGEST:                 //Digest
        break;
    case OSD_UI_MEDIA_SELECT_DLG:       //Media Select Dialog box
        break;
    case OSD_UI_PSCAN_PROMPT_DLG:       //The PSCAN prompt dialog box
#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
        OSDPROMPT_Trigger();
#endif
        break;
    case OSD_UI_BOOKMARK:               //Bookmark
        break;
#ifndef SUPPORT_STB
    case OSD_UI_SETUP:                  //SETUP
        SETUP_Trigger();
        break;
#endif //#ifndef SUPPORT_STB
#ifdef SUPPORT_STB
    case OSD_UI_MAIN_MENU:              //Main Menu
    	MAINMENU_Trigger();
        break;
#endif        
    case OSD_UI_SCREEN_SAVER:           //Screen Saver
#ifndef NO_SCREEN_SAVER
        // LLY2.53, expand time value unit from WORD to DWORD
        //OSDSS_Trigger(bType, wTime);
        OSDSS_Trigger(bType, dwTime);
#endif //
        break;
    case OSD_UI_COMMON_DLG:
        // LLY2.53, expand time value unit from WORD to DWORD
        //OSDCOMDLG_Trigger(bType, wTime);
        OSDCOMDLG_Trigger(bType, dwTime);
        break;
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
#ifdef SUPPORT_INDEPENDENT_NAVIGATOR //CoCo1.0, support independent Navigator UI
    case OSD_UI_NAVIGATOR:
        // LLY2.53, expand time value unit from WORD to DWORD
        //OSDNAVIGATOR_Trigger(bType, wTime);
        OSDNAVIGATOR_Trigger(bType, dwTime);
        break;
#endif
#endif //#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p

#ifdef SUPPORT_POWERON_MENU //CoCo2.38
    case OSD_UI_POWERON_MENU:
        // LLY2.53, expand time value unit from WORD to DWORD
        //POWERONMENU_Trigger(bType, wTime);
        POWERONMENU_Trigger(bType, dwTime);
        break;
#endif //#ifdef SUPPORT_POWERON_MENU //CoCo2.38
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
    case OSD_UI_COPY_DELETE_DLG:
        break;
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
#ifdef SUPPORT_RADIO_TUNER
    case OSD_UI_RADIO_TUNER:		//Radio Tuner
        RADIO_Trigger();	
        break;
#endif //SUPPORT_RADIO_TUNER
    default:                            //unknown UI
        break;
    }    
}

//***************************************************************************
//  Function    :   _OSD_RecoverNDMessage
//  Abstract    :   This function will recover normal display message.
//  Arguments   :   bMessagePos: OSD_ND_UPPER_RIGHT_REGION==>normal upper message.
//              :                OSD_ND_BOTTOM_REGION==>normal bottom message.   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSD_RecoverNDMessage(BYTE bMessagePos)
{
	BYTE bRecover = FALSE;

    switch (bMessagePos)
    {
    case OSD_ND_UPPER_RIGHT_REGION:
        if (_bOSDNUIndex > 0) //The stack has forever message to be recovered.
        {
            _OSDNUCurMsg.bMessage = _OSDNUStack[_bOSDNUIndex-1].bMessage;
            _OSDNUCurMsg.wParam = _OSDNUStack[_bOSDNUIndex-1].wParam;
            _OSDNUCurMsg.bSec = _OSDNUStack[_bOSDNUIndex-1].bSec;
            _bOSDNUIndex--;

            OSDND_Update(_OSDNUCurMsg.bMessage, _OSDNUCurMsg.wParam);

			bRecover = TRUE;
        }
        break;
    case OSD_ND_BOTTOM_REGION:
        if (_bOSDNBIndex > 0) //The stack has forever message to be recovered.
        {
            _OSDNBCurMsg.bMessage = _OSDNBStack[_bOSDNBIndex-1].bMessage;
            _OSDNBCurMsg.wParam = _OSDNBStack[_bOSDNBIndex-1].wParam;
            _OSDNBCurMsg.bSec = _OSDNBStack[_bOSDNBIndex-1].bSec;
            _bOSDNBIndex--;

            OSDND_Update(_OSDNBCurMsg.bMessage, _OSDNBCurMsg.wParam);
			
			bRecover = TRUE;
        }
        break;
    default:
        break;
    }

	return bRecover;
}

//***************************************************************************
//  Function    :   _OSD_RecoverNURegion
//  Abstract    :   This function will recover normal upper region.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_RecoverNURegion(void)
{
    if (_bOSDCurrentUI == OSD_UI_NONE)
    {
        //Forever normal upper message needs to be recovered?
        _OSD_RecoverNDMessage(OSD_ND_UPPER_RIGHT_REGION);
    }
    else //Call other UI's recover function to recover the normal upper region.
    {
        //Some UI doesn't have to recover the normal upper message like Search, Bookmark, Program. They coexist with the normal display mode.
        //Therefore, the normal upper message needs to be recovered when current UI is the UI.
        if (!_OSD_RecoverUI(_bOSDCurrentUI, OSD_RECOVER_UPPER_REGION))
            _OSD_RecoverNDMessage(OSD_ND_UPPER_RIGHT_REGION);
    }
}

//***************************************************************************
//  Function    :   _OSD_RecoverNBRegion
//  Abstract    :   This function will recover normal bottom region.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_RecoverNBRegion(void)
{
    if (_bOSDCurrentUI == OSD_UI_NONE)
    {
        //Forever normal bottom message needs to be recovered?
        _OSD_RecoverNDMessage(OSD_ND_BOTTOM_REGION);
    }
    else //Call other UI's recover function to recover the normal bottom region.
    {
        //Some UI doesn't have to recover the normal bottom message like Search, Bookmark, Program. They coexist with the normal display mode.
        //Therefore, the normal bottom message needs to be recovered when current UI is the UI.
        if (!_OSD_RecoverUI(_bOSDCurrentUI, OSD_ND_BOTTOM_REGION))       
            _OSD_RecoverNDMessage(OSD_ND_BOTTOM_REGION);
    }
}

//***************************************************************************
//  Function    :   OSD_AdjustRegion
//  Abstract    :   This function will adjust the OSD region. When the TV mode
//              :   is changed, need to call this function to adjust the OSD region.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSD_AdjustRegion(void)
{	
#ifdef ENLARGE_OSD_FOR_PAL_MODE
#ifdef SUPPORT_STB
    if (__bChooseMedia != MEDIA_SELECT_STB)
#endif //SUPPORT_STB
    {
        GDI_ChangeRegionHeight(0); // Brian1.20, enlarge OSD for PAL mode, call this before OSD_AdjustOSDPos otherwise the OSD position will be wrong
    }
#endif //ENLARGE_OSD_FOR_PAL_MODE

#ifdef SUPPORT_STB
    if (__bChooseMedia == MEDIA_SELECT_STB)
    {
        extern void AP_ConfigGDIRegion(BYTE bClearRegion);
        AP_ConfigGDIRegion(FALSE);
    }
#endif

    
    OSD_AdjustOSDPos(); // Brian0.75

    //CoCo1.20, support large OSD region for PAL mode.
#ifdef ENLARGE_OSD_FOR_PAL_MODE
    switch (_bOSDCurrentUI)
    {
    case OSD_UI_THUMBNAIL:
    case OSD_UI_DIGEST:
    case OSD_UI_MEDIA_MANAGER:
        break;
    case OSD_UI_SETUP:
    case OSD_UI_MAIN_MENU:
        OSDND_Update(MSG_DEFAULT_STATE, 0);
        break;
    default:
#ifdef SUPPORT_STB
    if (__bChooseMedia != MEDIA_SELECT_STB)
#endif //SUPPORT_STB
    {
        OSDND_ClearRegion(OSD_ND_BOTTOM_REGION);
        __bOSDClearNDBRegion = TRUE;
        OSDND_Update(MSG_DEFAULT_STATE, 0);
        OSDND_Update(_OSDNBCurMsg.bMessage, _OSDNBCurMsg.wParam);
        }
        break;
    }
#endif //ENLARGE_OSD_FOR_PAL_MODE
    
    //Some UIs needs to be updated when changing the TV mode.
    //Therefore, OSD will call it's "recover all" function to recover the UI.
    switch (_bOSDCurrentUI)
    {
    case OSD_UI_THUMBNAIL:
        THUMB_Recover(OSD_RECOVER_ALL);
        break;
    case OSD_UI_DIGEST:
        break;
    case OSD_UI_MEDIA_MANAGER:
        //Need to tune the position of the JPEG preview.
        MM_RecoverPreviewWindow();
        OSDMM_TuneRegionForPALMode();
        break;
    default:
        break;
    }

#ifdef SUPPORT_CHAR_SUBPICTURE 
    CHAR_SP_AdjustRegion();  // Alan1.20, Set the position for the sub-title
#endif

}
 // Brian0.75
void OSD_AdjustOSDPos(void)
{	
    WORD wStartX, wStartY; 

    __bCurrentRegionId = 0;
    if (__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
    {
        if (__bMPEGVideoMode == MPEG_VIDEO_MODE_NTSC)
        {
            wStartX = REGION_X_NTSC;
            wStartY = REGION_Y_NTSC;//-5 //CoCo0.90, Doesn't need to adjust it because DISP will adjust it?
        }
        else
        {
            wStartX = REGION_X_PAL;//-4 //CoCo0.90, Doesn't need to adjust it because DISP will adjust it?
            wStartY = REGION_Y_PAL; //- 4 //CoCo0.90, Doesn't need to adjust it because DISP will adjust it?
        }
    }
    else
    {
        if (__bMPEGVideoMode == MPEG_VIDEO_MODE_NTSC)
        {
            wStartX = REGION_X_NTSC;
            wStartY = REGION_Y_NTSC;
        }
        else
        {
            wStartX = REGION_X_PAL;
            wStartY = REGION_Y_PAL;
        }
    }
    
    //CoCo, Need to modify it as new GDI API.
    //GDI_MOVE_REGION(wStartX, wStartY);		
    GDI_MoveRegion(0, wStartX, wStartY);
	    
}

//***************************************************************************
//  Function    :   OSD_NormalMode
//  Abstract    :   This function will clear all UIs and return to normal display mode.
//              :   When user presses KEY_OPEN_CLOSE/KEY_POWER, main control module (CC) 
//              :   needs to call this function before it sends OSD_OUTPUT_MACRO( MSG_OPEN, ....);
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSD_NormalMode(void)
{
    //Initialize the palette for normal display
    OSDND_Update(MSG_INITIAL_PALETTE, 0);

    //Recover the normal display attributes.
    OSDND_Update(MSG_DEFAULT_STATE, 0);

    _OSD_ResetState();

    //Clear the region.
    GDI_ClearRegion(0);
}

//***************************************************************************
//  Function    :   _OSD_ResetUI
//  Abstract    :   This function will reset the UI's stack and set current UI
//              :   to normal display mode.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_ResetUI(void)
{
    //Initialize current UI to normal display mode.
    _bOSDCurrentUI = OSD_UI_NONE;

    //Initialize the UI stack.
    for (_bOSDTemp = 0; _bOSDTemp < OSD_MAX_DEPTH_OF_UI; _bOSDTemp++)
    {
        _OSDUIStack[_bOSDTemp] = OSD_UI_NONE;
    }

    //Initialize the index of the UI stack.
    _bOSDUIStackIndex = 0;
}

//***************************************************************************
//  Function    :   _OSD_ClearDisplayRegion
//  Abstract    :   This function will clear the normal upper/bottom region for normal display.
//  Arguments   :   bMessage: the message.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_ClearDisplayRegion(BYTE bMessage)
{
    switch (bMessage)
    {
    case MSG_VOLUME:
    case MSG_KEY:
    case MSG_ECHO:
    case MSG_PROGRESS: //CoCo1.10
    case MSG_WAITING:
        {
            _bOSDTemp = OSDND_GetMessagePos(bMessage);
            
            if (_bOSDTemp == OSD_ND_UPPER_RIGHT_REGION)
            {               
                if (bMessage != _OSDNUCurMsg.bMessage)
                {
                    //send MSG_CLEAR to normal display or other UI to clear normal upper message.
                    _OSD_ClearNUNBRegion(OSD_ND_UPPER_RIGHT_REGION);
                    __bOSDClearNDURegion = TRUE;
                }
                else
                    __bOSDClearNDURegion = FALSE;
            }
            else if (_bOSDTemp == OSD_ND_BOTTOM_REGION)
            {
                if (bMessage != _OSDNBCurMsg.bMessage)
                {
                    //send MSG_CLEAR to normal display or other UI to clear normal bottom message.
                    _OSD_ClearNUNBRegion(OSD_ND_BOTTOM_REGION);
                    __bOSDClearNDBRegion = TRUE;
                }
                else
                {
                    if (!__bOSDForceToClearNBRegion)
                    {
                        __bOSDClearNDBRegion = FALSE;
                    }
                    else
                    {
                        _OSD_ClearNUNBRegion(OSD_ND_BOTTOM_REGION);
                        __bOSDClearNDBRegion = TRUE;
                        __bOSDForceToClearNBRegion = FALSE;
                    }
                }
            }
        }
        break;
    default:
        //send MSG_CLEAR to normal display or other UI to clear normal upper/bottom message.
        _OSD_ClearNUNBRegion(OSDND_GetMessagePos(bMessage));
        __bOSDClearNDURegion = TRUE;
        __bOSDClearNDBRegion = TRUE;
        break;
    }
}

//***************************************************************************
//  Function    :   _OSD_ClearNUNBRegion
//  Abstract    :   This function will clear normal upper/bottom region.
//  Arguments   :   bMessage: the message.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_ClearNUNBRegion(BYTE bRegion)
{
    switch (_bOSDCurrentUI)
    {
    case OSD_UI_MEDIA_MANAGER:
        OSDMM_ClearRegion(bRegion);
        return;
    case OSD_UI_THUMBNAIL:
        THUMB_ClearRegion(bRegion);
        return;
#ifdef SUPPORT_RADIO_TUNER
    case OSD_UI_RADIO_TUNER:		//Radio Tuner
        RADIO_ClearRegion(bRegion);
        return;
#endif //SUPPORT_RADIO_TUNER
    default:
        break;
    }

#ifdef USE_MINI_OSD_REGION
        if ((_bOSDCurrentUI != OSD_UI_SETUP) && (_bOSDCurrentUI != OSD_UI_MAIN_MENU) &&
            (_bOSDCurrentUI != OSD_UI_DVD_PROGRAM) && (_bOSDCurrentUI != OSD_UI_BOOKMARK) && (_bOSDCurrentUI != OSD_UI_MEDIA_SELECT_DLG)
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
            && (_bOSDCurrentUI != OSD_UI_POWERON_MENU)
#endif
            )
#else //USE_MINI_OSD_REGION
    //CoCo0.80a, Don't clear the normal display when current UI is SETUP because SETUP will occuply whole scrren and it doesn't show normal display message.
    if ((_bOSDCurrentUI != OSD_UI_SETUP) && (_bOSDCurrentUI != OSD_UI_MAIN_MENU)
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
        && (_bOSDCurrentUI != OSD_UI_POWERON_MENU)
#endif
        )
#endif
    {
        OSDND_ClearRegion(bRegion);
    }
}


//***************************************************************************
//  Function    :   OSD_RemoveUI
//  Abstract    :   This function will remove the UI.
//  Arguments   :   bUI: OSD_UI_DISPLAY/OSD_UI_MEDIA_MANAGER/OSD_UI_SETUP/
//              :        OSD_UI_THUMBNAIL/OSD_UI_SEARCH/OSD_UI_DVD_PROGRAM/
//              :        OSD_UI_DIGEST/OSD_UI_MEDIA_SELECT_DLG/OSD_UI_PSCAN_PROMPT_DLG/
//              :        OSD_UI_BOOKMARK/OSD_UI_SCREEN_SAVER
//  Return      :   TRUE: Remove the UI successfully. FALSE: Can't remove the UI.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE OSD_RemoveUI(BYTE bUI)
{
    if (bUI == _bOSDCurrentUI)
    {
        _OSD_ExitUI(bUI, OSD_UI_EXIT_ALL);

        //Check if any UI needs to be recovered.
        if (_bOSDUIStackIndex != 0) //Some UI has been pushed to the stack.
        {                                      
            _bOSDCurrentUI = _OSDUIStack[_bOSDUIStackIndex-1];
            _bOSDUIStackIndex--;

            //Call the pop-up UI's OSDXXX_Recover().
            _OSD_RecoverUI(_bOSDCurrentUI, OSD_RECOVER_ALL);
        }
        else //The stack doesn't have any UI.
        {
            _bOSDCurrentUI = OSD_UI_NONE; //Normal Display mode.
        }
        
        return TRUE;
    }
    else
    {
        //Search if the UI in the stack.
        for (_bOSDTemp = 0; _bOSDTemp < _bOSDUIStackIndex; _bOSDTemp++)
        {
            if (bUI == _OSDUIStack[_bOSDTemp])
            {
                _OSD_ExitUI(bUI, OSD_UI_EXIT_CONTROL_ONLY);

                for (_bOSDTemp1 = _bOSDTemp; _bOSDTemp1 < (_bOSDUIStackIndex-1); _bOSDTemp1++)
                {
                    _OSDUIStack[_bOSDTemp1] = _OSDUIStack[_bOSDTemp1+1];  
                }

                _bOSDUIStackIndex--;

                return TRUE;
            }
        }
    }

    return FALSE;
}

//***************************************************************************
//  Function    :   OSD_PushCurrentNBForeverMsg
//  Abstract    :   This function will push the current NB forever message to the stack.
//  Arguments   :   bMessage: the message we want to push to the stack.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSD_PushCurrentNBForeverMsg(BYTE bMessage)
{
    if (bMessage == _OSDNBCurMsg.bMessage)
    {
        _OSD_ClearNUNBRegion(OSD_ND_BOTTOM_REGION);
        _OSD_PushForeverMsg(_OSDNBCurMsg.bMessage);
    }
}

//***************************************************************************
//  Function    :   _OSD_ResetState
//  Abstract    :   This function will reset the stack, variables, and UI.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_ResetState(void)
{	
    //Initialize current normal upper and normal bottom message.
    _OSDNUCurMsg.bMessage = MSG_NULL;
    _OSDNBCurMsg.bMessage = MSG_NULL;

    //Initialize the index of normal upper and normal bottom stack.
    _bOSDNUIndex = 0;
    _bOSDNBIndex = 0;

    //Initialize normal upper and normal bottom stack
    for (_bOSDTemp = 0; _bOSDTemp < OSD_MAX_DEPTH_OF_MSG_STACK; _bOSDTemp++)
    {
        _OSDNUStack[_bOSDTemp].bMessage = MSG_NULL;
        _OSDNBStack[_bOSDTemp].bMessage = MSG_NULL;		
    }	

    //Clear the UI's stack and set current UI to normal display mode.
    OSD_RemoveAllUI();
    _OSD_ResetUI();

    //Initialize normal display relative variables.
    _dwOSDNUStartTickCount = 0;
    _wOSDNUDisplayTickCount = 0;
    _dwOSDNBStartTickCount = 0;
    _wOSDNBDisplayTickCount = 0;    

    __bOSDClearNDURegion = TRUE;
    __bOSDClearNDBRegion = TRUE;

    //CoCo1.10, support OSD dynamic region
    __bOSDDynamicRegion = FALSE;

    //CoCo1.20, add this flag to cler normal bottom region
    __bOSDForceToClearNBRegion = FALSE;

//#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16) 
    OSDND_ConfigOSDRegion();
//#endif
}

//***************************************************************************
//  Function    :   _OSD_RecoverSpecialGraphZoom
//  Abstract    :   This function will recover 
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_RecoverSpecialGraphZoom(void)
{
    //CoCo0.86, recover the zoom OSD
#ifdef SUPPORT_GRAPH_ZOOM
    if (__bZoomFactor != VIEW_NORMAL)
    {
        OSDND_Update(MSG_ZOOM, __bZoomFactor);
    }
#endif
}

//***************************************************************************
//  Function    :   OSD_RecoverNBForeverMsg
//  Abstract    :   This function will recover the current NB message.
//  Arguments   :   none.
//  Return      :   TRUE: There is a forever message and will be drawed.
//              :   FALSE: No NB forever message currently.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE OSD_RecoverNBForeverMsg(void)
{
    if ((_OSDNBCurMsg.bMessage != MSG_NULL) && (_OSDNBCurMsg.bSec == 0xFF))
    {
        _OSD_ClearNUNBRegion(OSD_ND_BOTTOM_REGION);
        OSDND_Update(_OSDNBCurMsg.bMessage, _OSDNBCurMsg.wParam);
        return TRUE;
    }
	else //pop-up stack
	{
		//Recover the normal bottom region.
		return _OSD_RecoverNDMessage(OSD_ND_BOTTOM_REGION);
	}

    return FALSE;
}

//***************************************************************************
//  Function    :   OSD_UpdateZoomState
//  Abstract    :   This function will Update the zoom mode.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSD_UpdateZoomState(void)
{
#ifdef SUPPORT_GRAPH_ZOOM
    //Clear PIP
    switch (_bOSDCurrentUI)
    {
    case OSD_UI_DISPLAY:
    case OSD_UI_NONE:
    case OSD_UI_NAVIGATOR:
    case OSD_UI_SEARCH:
        if (_bGDIInit)
            OSDND_Update(MSG_ZOOM, __bZoomFactor);
        break;
    default:
        break;
    }
    
#endif
}

//***************************************************************************
//  Function    :   OSD_RemoveAllUI
//  Abstract    :   This function will remove all UIs.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSD_RemoveAllUI(void)
{
    /*
    OSD_RemoveUI(OSD_UI_DISPLAY);
    OSD_RemoveUI(OSD_UI_SETUP);
    OSD_RemoveUI(OSD_UI_MAIN_MENU);
    OSD_RemoveUI(OSD_UI_THUMBNAIL);
    OSD_RemoveUI(OSD_UI_SEARCH);
    OSD_RemoveUI(OSD_UI_DVD_PROGRAM);
    OSD_RemoveUI(OSD_UI_DIGEST);
    OSD_RemoveUI(OSD_UI_MEDIA_SELECT_DLG);
    OSD_RemoveUI(OSD_UI_PSCAN_PROMPT_DLG);
    OSD_RemoveUI(OSD_UI_BOOKMARK);
    OSD_RemoveUI(OSD_UI_COMMON_DLG);
    OSD_RemoveUI(OSD_UI_NAVIGATOR);
    */
    //CoCo2.17, fix the bug about Navigator==>KEY_FUNCTION==>USB==>Show wrong Namvigator in the MM UI.
    _OSD_ExitUI(_bOSDCurrentUI, OSD_UI_EXIT_CONTROL_ONLY);
        
    for (_bOSDTemp = 0; _bOSDTemp < _bOSDUIStackIndex; _bOSDTemp++)
    {
        _OSD_ExitUI(_OSDUIStack[_bOSDTemp], OSD_UI_EXIT_CONTROL_ONLY);        
    }

    _OSD_ResetUI();
    
    GDI_ClearRegion(0);    
}

//CoCo1.10, support OSD dynamic region.
//***************************************************************************
//  Function    :   OSD_NewDynamicRegion
//  Abstract    :   This function will new a dymanic region.
//  Arguments   :   none.
//  Return      :   TRUE: The dynamic region is created OK.
//              :   FALSE: The dynamic region is created failed.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE OSD_NewDynamicRegion(pDynamicRegionRecoverFunc pRecoveryFunc)
{
    if (!__bOSDDynamicRegion)
    {
        __bOSDDynamicRegion = TRUE;

        DynamicRegionRecoverFunction = pRecoveryFunc;
    }
    else
        return FALSE;

    return TRUE;
}

//***************************************************************************
//  Function    :   OSD_DeleteDynamicRegion
//  Abstract    :   This function will delete the dymanic region.
//  Arguments   :   none.
//  Return      :   none. 
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSD_DeleteDynamicRegion(void)
{
    //Set the "Dynamic Region" flag to FALSE.
    __bOSDDynamicRegion = FALSE;

    OSDND_Update(MSG_DEFAULT_STATE, 0);
}

//***************************************************************************
//  Function    :   OSD_CanUpdateDynamicRegionInfo
//  Abstract    :   Call this function to check if you can update the information
//              :   in the dynamic region or not.
//  Arguments   :   none,
//  Return      :   TRUE: You can update the information.
//              :   FALSE: You can't update the information. 
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE OSD_CanUpdateDynamicRegionInfo(void)
{
    if ((_bOSDCurrentUI == OSD_UI_SETUP) || (_bOSDCurrentUI == OSD_UI_MAIN_MENU) || (_bOSDCurrentUI == OSD_UI_SCREEN_SAVER)
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
        || (_bOSDCurrentUI == OSD_UI_POWERON_MENU)
#endif
        )
    {
        return FALSE;
    }
    else
        return TRUE;   
}

//***************************************************************************
//  Function    :   _OSD_RecoverDynamicRegion
//  Abstract    :   This function will recover the dynamic region.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_RecoverDynamicRegion(void)
{
    if (__bOSDDynamicRegion)
    {
        DynamicRegionRecoverFunction();
    }
}

//***************************************************************************
//  Function    :   _OSD_ConfigRegion
//  Abstract    :   This function will config each UI's region.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSD_ConfigRegion(BYTE bUI)
{
    switch (bUI)
    {
    case OSD_UI_NONE:                   //Normal Display
        OSDND_ConfigOSDRegion();
        break;
    case OSD_UI_DISPLAY:
    case OSD_UI_NAVIGATOR:
        //Display mode
        OSDDSPLY_ConfigOSDRegion();
        break;
    case OSD_UI_MEDIA_MANAGER:          //Media Manager
        OSDMM_ConfigOSDRegion();
        break;
    case OSD_UI_THUMBNAIL:              //Thumbnail
        THUMB_ConfigOSDRegion();
        break;
#ifndef NO_SEARCH_UI //++CoCo2.37p
    case OSD_UI_SEARCH:                 //Search UI
        OSDSRCH_ConfigOSDRegion();
        break;
#endif //#ifndef NO_SEARCH_UI //++CoCo2.37p
#ifndef NO_DVD_PASSWORD_DLG //CoCo2.38
    case OSD_UI_PASSWORD:               //DVD Password Dialog
        OSDPASSWORD_ConfigOSDRegion();
        break;
#endif //#ifndef NO_DVD_PASSWORD_DLG //CoCo2.38
#ifndef NO_PROGRAM_UI //++CoCo2.37p
    case OSD_UI_DVD_PROGRAM:            //VCD/DVD Program
        OSDPRGM_ConfigOSDRegion();
        break;
#endif //#ifndef NO_PROGRAM_UI //++CoCo2.37p
#ifdef SUPPORT_BOOKMARK
    case OSD_UI_BOOKMARK:               //Bookmark
        OSDBOOKMARK_ConfigOSDRegion();
        break;
#endif
    case OSD_UI_DIGEST:                 //Digest
        break;
    case OSD_UI_MEDIA_SELECT_DLG:       //Media Select Dialog box
        OSDDLG_ConfigOSDRegion();
        break;
#ifndef SUPPORT_STB
    case OSD_UI_SETUP:                  //SETUP
        SETUP_ConfigOSDRegion();
        break;
#endif
    case OSD_UI_MAIN_MENU:              //Main Menu        
        break;
    case OSD_UI_PSCAN_PROMPT_DLG:
    case OSD_UI_COMMON_DLG:
        OSDCOMDLG_ConfigOSDRegion();
        break;
    case OSD_UI_SCREEN_SAVER:           //Screen Saver
        break;
#ifdef SUPPORT_RADIO_TUNER
    case OSD_UI_RADIO_TUNER:		//Radio Tuner
        RADIO_ConfigOSDRegion();
        break;
#endif //SUPPORT_RADIO_TUNER
    default:                            //unknown UI
        break;
    }
}

//Alan2.21, to save the buffer mode and bit mode information
void OSD_SetBufferModeInfo(BYTE bBuffMode)
{
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
    switch (bBuffMode)
    {
    case OSDBUFFER_MODE_NORMAL:      // DVD & VCD
        _OSDBuffer_Info.bColorMode = GDI_OSD_4B_MODE;
        _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_NORMAL;
        break;
    case OSDBUFFER_MODE_MM_MOTION:   // VOB 
        _OSDBuffer_Info.bColorMode = GDI_OSD_4B_MODE;
        _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_MM_MOTION;
        break;
    case OSDBUFFER_MODE_MM_AVI:      // AVI
        _OSDBuffer_Info.bColorMode = GDI_OSD_4B_MODE;
        _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_MM_AVI;
        break;
    case OSDBUFFER_MODE_MM_VIDEO_ONLY:        // JPEG slide show
        _OSDBuffer_Info.bColorMode = GDI_OSD_4B_MODE;
        _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_MM_VIDEO_ONLY;
        break;
    case OSDBUFFER_MODE_MM_AUDIO_ONLY:        // MP3
        _OSDBuffer_Info.bColorMode = GDI_OSD_4B_MODE;
        _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_MM_AUDIO_ONLY;
        break;
    case OSDBUFFER_MODE_MM_JPEG_PREVIEW:      // JPEG Preview
        _OSDBuffer_Info.bColorMode = GDI_OSD_8B_MODE;
        _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_MM_JPEG_PREVIEW;
        break;
    case OSDBUFFER_MODE_MM_JPEG_THUMBNAIL:    // JPEG slide show
        _OSDBuffer_Info.bColorMode = GDI_OSD_8B_MODE;
        _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_MM_JPEG_THUMBNAIL;
        break;
    case OSDBUFFER_MODE_MM_UI: // First time to display MM UI after disc-detected for non-autoplay  
        _OSDBuffer_Info.bColorMode = GDI_OSD_8B_MODE;
        _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_MM_UI;
        break;
    case OSDBUFFER_MODE_AP:          // AP
        _OSDBuffer_Info.bColorMode = GDI_OSD_4B_MODE;
        _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_AP;
        break;
    case OSDBUFFER_MODE_DVB:         // DVB
        _OSDBuffer_Info.bColorMode = GDI_OSD_8B_MODE;
        _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_DVB;
        break;
    default:
        break;
    }
#else
    _OSDBuffer_Info.bColorMode = GDI_OSD_8B_MODE;
    _OSDBuffer_Info.bBufferMode = OSDBUFFER_MODE_NORMAL;
#endif
}

void OSD_GetBufferModeInfo(POSDBUF_INFO pOSDBuffer_Info)
{
    pOSDBuffer_Info->bColorMode = _OSDBuffer_Info.bColorMode;
    pOSDBuffer_Info->bBufferMode = _OSDBuffer_Info.bBufferMode;
}

void OSD_ConfigOSDBuffer(BYTE bMode)
{
}

void OSD_SetRegion(BYTE bRegion, BYTE bClearRegion, GDI_REGION_INFO *RegionInfo)
{
    if (!_bOSDSetRegion)
    {
        _bOSDSetRegion = TRUE;
    }
    else
    {
        //compare the region.
        if ((_bOSDCurrentRegion != bRegion) ||
            (_bOSDCurrentRegionInfo.wWidth != RegionInfo->wWidth) ||
            (_bOSDCurrentRegionInfo.wHeight != RegionInfo->wHeight) ||
            (_bOSDCurrentRegionInfo.bColorMode != RegionInfo->bColorMode) ||
            (_bOSDCurrentRegionInfo.dwTAddr != RegionInfo->dwTAddr))        
        {
            //Set the region
        }
        else
        {
            if (bClearRegion)
            {
                GDI_ClearRegion(0);
            }

            return;
        }
    }

    GDI_DeactivateRegion();
    GDI_ConfigRegionInfo(bRegion, RegionInfo);    
    GDI_InitialRegion(bRegion);    

    // Clear region
    if (bClearRegion)
    {
        GDI_ClearRegion(bRegion);
    }

    GDI_ActivateRegion(bRegion);

    _bOSDCurrentRegion = bRegion;
    _bOSDCurrentRegionInfo.wWidth = RegionInfo->wWidth;
    _bOSDCurrentRegionInfo.wHeight = RegionInfo->wHeight;
    _bOSDCurrentRegionInfo.bColorMode = RegionInfo->bColorMode;
    _bOSDCurrentRegionInfo.dwTAddr = RegionInfo->dwTAddr;
}
