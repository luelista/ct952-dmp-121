// ** TCH1.24a; Move Some APIs to this files. It's a extenstion of CC Module
// ** TCH0.60; Build CC_KEY.C for Key Processing.

// The file "winav.h" must be included in first line
#include "winav.h"
#include "ctkav.h"  // ** TCH0.60; 

#include    <string.h>

// Micky2.80p, declare malloc.h into platform.h
#include "initial.h"
#include "utl.h"

#include "hal.h"

#include "SrcFilter.h"
#include "input.h"
#include "chips.h"
#include "thumb.h"
#include "comutl.h"
//#include "amp.h"
#include "gdi.h" //CoCo.CT909
#include "osdmm.h" //CoCo.CT909


#include "setup.h"
#include "navigate.h"
#include "cell.h"
#include "dvdinfo.h"    

#include "infofilter.h"
#include "vcd20.h"

#include "digest.h"

#include "cc.h"
#include "osd.h"
#include "panel.h" 

#include "linear.h"
#include "monitor.h"
//#include "sysdebug.h" 

#ifndef NO_MP3
#include "Mmanager.h" //   "mp3.h"
#endif

#include "disp.h"
#include "parser.h"
//#include "mpeg2dec.h"
#include "subpict.h"

#ifdef SUPPORT_CHAR_SUBPICTURE
#include "par_sptx.h"
#include "char_subpict.h"  //Alan1.10, support char-based subpicture
#endif

// Micky0.62-909
#include    "media.h"
#include    "osdsrch.h"

// #include "logoaddr.h"
#include "aploader.h"

#define NULL_0      0   // ** TCH0.60; 
// wyc1.02a-909, add SP1 & SP2 debug information.
//#define CCKEY_SP_DEBUG
// ** TCH0.60; begin...  

extern  BYTE            __bAutoScans;   

extern  BIT             __btPlayFromBegin;// Told F/W it should play Track-Begin first
extern  BIT             __btFromStopMode;// GOTO position comes from a "STOP" mode

// ** TCH0.60; end... 

//CoCo1.07, for auto-test
#ifdef AUTO_TEST
extern WORD aDVDRule[];
extern WORD aCDROMRule[];
extern DWORD __dwAutoTestTime;
extern BYTE __bRuleIndex;
extern BYTE __bAutoTest;
extern WORD *_wpRule;
#endif

// Micky0.62-909, process number key, then convert as KEY_NUMBER
WORD    __wNumberQueue;

// The number need time out control
BYTE    bNumberTimeOut=FALSE;
//the time that the number key was pressed
DWORD   _dwTimeNumber;
// Micky0.72, change number to DWORD to avoid overflow.
DWORD    __dwNumberPrevious;
DWORD   __dwTimeKey;

// Micky0.62-909, process number key, then convert as KEY_NUMBER
// __dwTimeKey  : record the time that the last KEY_N10/N20_PLUS was pressed(NULL means none)
//              : used for number calculation only instead of time out.
// __dwNumberPrevious:   The current value
// __wNumberQueue !=0, means has number input.
// __dwTimeNumber: Used for number time out.
// each ProcessKey must DO:
// When the value out of max. value, and reset to default, must call CC_NumberUtility(NUMBER_CLEAR)
// When the value out of max. digits, must call CC_NumberUtility(NUMBER_SHIFT) to discard the first input number

// DVD_161Micky, add key arrays for release still state
// whenever a new key for release is added, must update
// !!! only physical keys are allowed(compound keys are not allowed)
// RELEASE_STILL_KEY_NUMBER and _bReleaseStillKeys[]
#define RELEASE_STILL_KEY_NUMBER    2
BYTE    code _bReleaseStillKeys[RELEASE_STILL_KEY_NUMBER] =
        {
            KEY_PLAY, KEY_PAUSE
        };
// wyc2.37-909s, replace NO_DVD to NO_DISC_MODE
#ifndef NO_DISC_MODE //CoCo2.37p
BYTE    _CC_ReleaseStillTrigger(void);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

// Brian.170
//  *********************************************************************
//  Function    :   _AutoVocal()
//  Description :   Accoding to the __sbMICDetect to turn on/off the vocal
//  Arguments   :   bValidKey
//  Return      :   bValidKey
//  Side Effect :
//  *********************************************************************
// This part of code is moved from the main loop, to simplify the flow and for easy reading
#ifdef  ENABLE_AUTOVOCAL
BYTE _AutoVocal(BYTE bValidKey)
{
    // DVD_104Micky, check mic control only when karaoke is on.
    if (__SetupInfo.bAudioKaraoke == SETUP_AUDIO_KARAOKE_ON)
    {
    if (OS_GetSysTimer() - __dwMICDectectInterval> COUNT_100_MSEC)
    {
#ifdef  SW_MIC_DETECT
        CHIPS_MICDetect();
#endif
        __dwMICDectectInterval = OS_GetSysTimer();

        if (__sbMICDetect ==_btPreMICDetect)
        {
#ifdef  SUPPORT_MIC_DETECT_INV
            if ( ! __sbMICDetect )
#else
            if ( __sbMICDetect )
#endif
                    _bMICOffCount++;
        }
        else
        {
            _bMICOffCount = 0;
        }
    if ( __bAutoVocal )
    {
#ifdef  SUPPORT_MIC_DETECT_INV      // ** TCH1.54; Active= HIGH. Mic has I/P.
        if ( ! __sbMICDetect )  // ** TCH1.54; Active= 1.
#else
        if ( __sbMICDetect )  // ** TCH1.54; Active= 0.
#endif
        {   // NO MIC Input
            // DVD_104Micky, enhance the vocal mode define
            if ( __bVocal == VOCAL_OFF && _bMICOffCount > 10)
            {
                // DVD_103Micky, fix auto vocal won't have effect
                {
// DVD_172Micky, FLOW_CONTROL begin
                    __bFWKey = KEY_VOCAL;
                    __bKeyMessage=MSG_VOCAL;    // force the message to display
// DVD_172Micky, FLOW_CONTROL end
                    _bMICOffCount = 0;
                }
            }
        }
        else
        {
            // DVD_104Micky, enhance the vocal mode define
//            if ( __btVocalON )
            if ( __bVocal == VOCAL_ON)
            // DVD_103Micky, fix auto vocal won't have effect
            {
// DVD_172Micky, FLOW_CONTROL begin
                __bFWKey = KEY_VOCAL;
                __bKeyMessage=MSG_VOCAL;    // force the message to display
// DVD_172Micky, FLOW_CONTROL end
            }
        }
    } // end if ( __bAutoVocal )

        _btPreMICDetect = __sbMICDetect; // keep the previous detect result
    } // end if (__dwCountSystem - __dwMICDectectInterval> COUNT_100_MSEC)

    } // end if (__SetupInfo.bAudioKaraoke == SETUP_AUDIO_KARAOKE_ON)

    return bValidKey;
}
#endif  // ENABLE_AUTOVOCAL
// Micky2.81, put show IR map codes into a function.
#ifdef  _SHOW_IR_MAP
void _CC_ShowIRMap()
{
    if ( _bIRGet )
    {
        _bIRGet= FALSE;
        COMUTL_BYTE2HexStr ( (__bMsgString ), _bIRMap[0] );
        COMUTL_BYTE2HexStr ( (__bMsgString +3), _bIRMap[1] );
        COMUTL_BYTE2HexStr ( (__bMsgString +6), _bIRMap[2] );
        COMUTL_BYTE2HexStr ( (__bMsgString +9), _bIRMap[3] );
        __bMsgString[2]= '-';
        __bMsgString[5]= '-';
        __bMsgString[8]= '-';
        __bMsgString [11]= NULL_0;
        OSD_Output ( MSG_STRING, NULL_0, 1 );
        OS_DelayTime ( COUNT_1_SEC);
    }
}
#endif  //#ifdef  _SHOW_IR_MAP

// Micky1.05, add auto test for IC sorting by CoCo
#ifdef AUTO_TEST
void _CC_AutoTest(void)
{
    if (__bAutoTest)
    {        
        if (__wDiscType != NO_DISC_IN_DRIVE)
        {
            if (__wDiscType & BOOK_DVD)
                _wpRule = aDVDRule;
            else if (__wDiscType & BOOK_CDROM)
                _wpRule = aCDROMRule;
            else
                __bAutoTest = FALSE;
            
            if (__bAutoTest)
            {
                if ((OS_GetSysTimer() - __dwAutoTestTime) > (DWORD)_wpRule[__bRuleIndex])
                {
                    if ((_wpRule[__bRuleIndex] == 0) && (_wpRule[__bRuleIndex+1] == 0))
                    {                    
                        __bAutoTest = FALSE;
                    }   
                    else
                    {      
                        if (__bKey == KEY_NO_KEY)
                        {
                            __bRuleIndex++; 
                            __bISRKey = (BYTE)_wpRule[__bRuleIndex++];                            
                            //__bISRKeyRead = TRUE; 
                            __dwAutoTestTime = OS_GetSysTimer();                            
                        }
                    }
                }
            }
        }
    }
}

void  CC_AutoTestInitial(void)
{
   __bRuleIndex = 0;
   __bAutoTest = FALSE;
}

#endif

BYTE    _KeyGroupVideo(BYTE bKey)
{
switch(bKey)
{
// KEY_GROUP7, Video Keys
    case    KEY_ZOOM:
        // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
        if ( !HAL_Zoom(VIEW_ZOOM_SUPPORT) )
            return INVALID_KEY;

        if ( __wDiscType & BOOK_CDDA )  // ** 0.21;
            return INVALID_KEY;
        // wyc2.37-909s, fix MPEG-CD can zoom problem.
#ifdef SUPPORT_FUSS_MPEG_CD
        if (INFOFILTER_FUSSMPEGCD(IF_MODE_CHECK_MPEG_CD,0))
            return INVALID_KEY;
#endif
// Micky1.20, 4M flash
#ifndef NO_DIGEST
        if ( __btDIGEST )
            return INVALID_KEY;
#endif
        {
            //initial factor no Zoom in)
            switch(__bZoomFactor)
            {
            case    VIEW_NORMAL:
                __bZoomFactor=VIEW_ZOOMX2;
                // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
                if ( !HAL_Zoom(VIEW_ZOOMX2) )
                    return INVALID_KEY;
                break;

            case    VIEW_ZOOMX2:
                __bZoomFactor=VIEW_ZOOMX4;
                // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
                if ( !HAL_Zoom(VIEW_ZOOMX4) )
                    return INVALID_KEY;
                break;
            case    VIEW_ZOOMX4:
                __bZoomFactor=VIEW_NORMAL;
                // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
                if ( !HAL_Zoom(VIEW_NORMAL) )
                    return INVALID_KEY;
                break;
            }
        }
        // Micky0.85, re-define the ZOOM ID.
        __bTemp = __bZoomFactor;
        if ( __bZoomFactor == VIEW_NORMAL )
            __bTemp = VIEW_ZOOMX1;
        // wyc2.30-909s, use new zoom message
#ifdef SUPPORT_GRAPH_ZOOM
        OSD_OUTPUT_MACRO ( MSG_ZOOM, __bTemp, 0x3 );
#endif //
        OSD_OUTPUT_MACRO ( MSG_ZOOM_TEXT, __bTemp, 0x3 );
        break;

    case    KEY_ZOOM_LEFT:
        // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
        if ( !HAL_Zoom(VIEW_ZOOM_LEFT) )
            return INVALID_KEY;
        // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
        // MS_ZOOMMOVE will be removed.
        /// OSD_Output(MSG_ZOOMMOVE, OSD_ZOOM_LEFT, 0x3);
#ifdef SUPPORT_GRAPH_ZOOM
        OSD_Output(MSG_ZOOM, VIEW_ZOOM_LEFT, 0x3);
#endif //
        OSD_Output(MSG_ZOOM_TEXT, VIEW_ZOOM_LEFT, 0x3);
        break;

    case    KEY_ZOOM_RIGHT:
         // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
        if ( !HAL_Zoom(VIEW_ZOOM_RIGHT) )
            return INVALID_KEY;
        // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
        // MS_ZOOMMOVE will be removed.
#ifdef SUPPORT_GRAPH_ZOOM
        OSD_Output(MSG_ZOOM, VIEW_ZOOM_RIGHT, 0x3);
#endif //
        OSD_Output(MSG_ZOOM_TEXT, VIEW_ZOOM_RIGHT, 0x3);
        break;

    case    KEY_ZOOM_UP:
        // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
        if ( !HAL_Zoom(VIEW_ZOOM_UP) )
            return INVALID_KEY;
        // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
        // MSG_ZOOMMOVE will be removed.
#ifdef SUPPORT_GRAPH_ZOOM
        OSD_Output(MSG_ZOOM, VIEW_ZOOM_UP, 0x3);
#endif //
        OSD_Output(MSG_ZOOM_TEXT, VIEW_ZOOM_UP, 0x3);
        break;

    case    KEY_ZOOM_DOWN:
        // LLY.274p-3, use command debug 1 to readback SP buffer cnt
        // And, using new method to output debug info.
        // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
        if ( !HAL_Zoom(VIEW_ZOOM_DOWN) )
            return INVALID_KEY;
        // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
        // MSG_ZOOMMOVE will be removed.
#ifdef SUPPORT_GRAPH_ZOOM
        OSD_Output(MSG_ZOOM, VIEW_ZOOM_DOWN, 0x3);
#endif //
        OSD_Output(MSG_ZOOM_TEXT, VIEW_ZOOM_DOWN, 0x3);
        break;

    case    KEY_SPST_CTL:
    		// Micky0.68, merge SP change into an API for navigator and key switch share use.
    		// parameter 1 means by KEY_SPST_CTL
    		// Micky0.76, must return the value.
        	// Micky0.80,
    		return CC_SPST_Change(CC_TOGGLE_SP1);
    		break;   
    // Micky0.80, add SP2 control key    		
    case    KEY_SPST2_CTL:
    		return CC_SPST_Change(CC_TOGGLE_SP2);
    		break;     		
    // DVD_101-2, add VIDEO_OUTPUT_CTL for switch normal/yuv/rgb directly
   //Alex1.07a, 20031013 Support OSD SHOW PSCAN PROMPT, OSDPROMPT_ProcessKey will handle this KEY
#ifndef SUPPORT_PSCAN_PROMPT_OSD
    case    KEY_VIDEO_OUTPUT_CTL:
            __SetupInfo.bVideoOutput++;
            if (__SetupInfo.bVideoOutput > SETUP_SCREEN_VIDEO_OUTPUT_END)
                __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_BEGIN;
            // DVD_104Micky. convert setup value ID
            SETUP_ChangeValue(SETUP_SCREEN_VIDEO_OUTPUT);

            //Brian1.06, P-SCAN is integrated into Video Output
            // SETUP_ChangeValue will set __SetupInfo.bProgressiveScan according to __bSetupInfo.VideoOutput
            SETUP_ChangeValue(SETUP_SCREEN_PROGRESSIVE_SCAN);
            OSD_OUTPUT_MACRO(MSG_PSCAN, (WORD)NULL, 0); //CoCo1.50, send PSCAN information here to prevent the information in Display UI is not updated.

            //HAL_AdjustTVMode(); // Brian.172r call SETUP_AdjustSetting instead
            SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);

            OSD_Output(MSG_VIDEO_OUTPUT, __SetupInfo.bVideoOutput, 0x3); // LLY.101-2
            break;
#endif // #ifndef SUPPORT_PSCAN_PROMPT_OSD
    default:
    // Micky0.65-909, must return INVALID_KEY;
        return INVALID_KEY;
}
    return TRUE;
}

//  *****************************************************************
//  Function    :   _KeyGroupOthers
//  Description :   Process Video Group
//  Return      :   None
//  Side Effect :
//  *****************************************************************
BYTE    _KeyGroupOthers(BYTE bKey)
{
    if (bKey != KEY_NO_KEY)
        return INVALID_KEY;
    else
        return KEY_NO_KEY;
}

#ifdef SUPPORT_AV_IN
BYTE    _KeyGroupTFT(BYTE bKey)  //Jeff 20060705 Add for TFT
{
 if ( !_btTFT_PowerDown )
 {
    if (bKey == KEY_AV_IN)
    {
    	if ( !_btAVIN )
    	{
    	    _btAVIN=1;
    	}
    	else
    	{
    		_btAVIN=0;
    	}
        __bFWKey = KEY_POWER;
        TFT_Trigger();
    }	
 }
     return bKey;        
}
#endif  //#ifdef SUPPORT_AV_IN

BYTE    _KeyGroupPlay(BYTE bKey)
{
#ifdef SUPPORT_NPF_TRIGGER_ONLY
// LLY.0992, don't allow non-play playable macro command key in stop mode
    if(__bModeCmd==KEY_STOP)
    {
        if( (bKey>=NONNPF_PLAYABLEKEY_BEGIN) && (bKey<=NONNPF_PLAYABLEKEY_END) )
            return INVALID_KEY;
    }
#endif  // #ifdef SUPPORT_NPF_TRIGGER_ONLY

switch(bKey)
{
// KEY_GROUP3, Play back Command
    case    KEY_SCF:
    case    KEY_SCB:
    case    KEY_FREEZE:
    case    KEY_STEP:
    // Micky2.81, Merge KEY_STOP's action together
///    case    KEY_STOP:
// DVD_16a1Micky, fix repeat A-> fast-> can't repeat AB
// don't allow repeat A mode change macro command to fast
// now repeat ab only allow npf and slow.
    case    KEY_FF:
        {
        extern  DWORD   _dwTimePrev;    // [UTL]
        _dwTimePrev= 0xfffffff; // ** TCH1.60-1; Let Time Display can show current time.
        }

            // Do NOT allow REPEAT AB ->  scan/freeze/step
            if (__bRepeatAB != REPEAT_NONE)
            {
                return  INVALID_KEY;
            }
            if (bKey==KEY_STEP)
            {
                //do NOT allow stop/scan/freeze -> step
                if ( (__bModeCmd==KEY_STOP) || (__bModeCmd==KEY_FREEZE) ||
                     (__bModeCmd==KEY_SCF) || (__bModeCmd==KEY_SCB) )
                {
                    return  INVALID_KEY;
                }
            }

            // Only allow playing/freeze->freeze
            // what about !__btPlaying
            if ( __bModeCmd != KEY_FREEZE )
            {
                if ( bKey==KEY_FREEZE )
                if ( ( __bModeCmd != KEY_PLAY ) || !__btPlaying )
                {
                    return  INVALID_KEY;
                }
            }
    case    KEY_PLAY:
// DVD_16a1Micky, fix repeat A-> fast-> can't repeat AB
//    case    KEY_FF:
    // wyc2.17-909S, add one key for language study.
    case    KEY_LAN_STUDY:
    case    KEY_SF:
        if (__btIntroMode)
        if ( (bKey != KEY_PLAY) )
            return INVALID_KEY;
        // wyc2.37-909s, fix mpeg-cd can slow problem.
#ifdef SUPPORT_FUSS_MPEG_CD
        if ((INFOFILTER_FUSSMPEGCD(IF_MODE_CHECK_MPEG_CD,0)) && (bKey == KEY_SF))
            return INVALID_KEY;
#endif

        // wyc1.06-909, only press KEY_SF can make SLOW message active.
#if (SLOW_MODE_CONFIG != NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
        if (bKey == KEY_SF)
            __bActiveMessage = MSG_SLOW;
#else
        if ((bKey == KEY_SF) || (bKey == KEY_LAN_STUDY))
            __bActiveMessage = MSG_SLOW;
#endif //

#ifndef NO_DISC_MODE //CoCo2.37p
        // **** TCH0.99E-XQ, Begin...
        if (__wDiscType & BOOK_DVD)
        {
            if( (bKey>=NONNPF_PLAYABLEKEY_BEGIN) && (bKey<=NONNPF_PLAYABLEKEY_END) )
                // DVD_155Micky, check cell still and vobu still
                if (CELL_QueryStatus(CELL_IS_STILL) ||
                    (CELL_QueryStatus (CELL_VOBU_STILL_ON) )
                    ) // DVD_100Micky, query cell still status
                {
                    return INVALID_KEY;
                }
        }
        // **** TCH0.99E-XQ, end...
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

// wyc2.17-909S, add differenr case for language study function.
#if (SLOW_MODE_CONFIG == LANGUAGE_STUDY_MODE)
        if (bKey == KEY_SF)
        {
            if ( !(__wDiscType & BOOK_DVD) && !( (__wDiscType & BOOK_VCD) && !(__wDiscType & BOOK_SVCD) && !(__wDiscType & BOOK_CVD) ) ) //for DVD/VCD only now //kevin1,10, fix SVCD freq wrong when language study  
                return INVALID_KEY;                    
        }
#elif (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
        if (bKey == KEY_LAN_STUDY)
        {
            if ( !(__wDiscType & BOOK_DVD) && !( (__wDiscType & BOOK_VCD) && !(__wDiscType & BOOK_SVCD) && !(__wDiscType & BOOK_CVD) ) ) //for DVD/VCD only now //kevin1,10, fix SVCD freq wrong when language study  
                return INVALID_KEY;                    
        }

#endif

            // ** S-TCH;
            if ( bKey == KEY_FF  )
            {
                // ** 0.17; Don't allow FF  within SCAN mode
                if ( __bRepeatAB == REPEAT_AB )
                    return  INVALID_KEY;
            }

            // DVD_160Micky, make number input mode has top priority begin
            // fix VCD2.0 menu-> Program-> input number-> key_play
            // move (bKey==KEY_PLAY) before PBC check
            //Check trigger GOTOTIME->PLAY or PROGRAM->PLAY
            if ( bKey == KEY_PLAY )
            {
                //KEY_PLAY will trigger PROGRAM to play
                // wyc.pro , in DVD playing, NV_Trigger will enter follow code to change titles.
                // So we must avoid enter follow code by not IR key operation.
                // wyc1.10-909, when pause + program edit mode, we treat KEY_PLAY to exit pause mode, not edit program entries.
                if ( (__btInputProgram) && ( __bKeySource == KEY_FROM_ISR ) && (__btPause == FALSE))
                {
                    // wyc.102 , to count the program entry number for program playing usage
                    // if in playing mode and not input digit in program input menu, not affect the program playing
                    // and move clear program OSD to up place
                    __btInputProgram=FALSE;

                    // LLY2.78b-3, support "Program Play" message
#ifdef  SUPPORT_PROGRAM_PLAY_INFO
                    __bKeyMessage=MSG_PROGRAM_PLAY;
                    OSD_OUTPUT_MACRO(MSG_PROGRAM_PLAY, NULL_0, 0x3);
#endif  // SUPPORT_PROGRAM_PLAY_INFO

                    // wyc1.10-909, because when no program entries + program edit mode and press KEY_PLAY, then MM will process clear program
                    // mode, so it won't need to process follow code in CC. And when MP3 pre-stop + program edit mode, then press KEY_RESUME.
                    // The key will be transfer to KEY_PLAY here. So we don't do follow code to let code can do CC_Keycommand(KEY_PLAY) tyo avoid
                    // KEY_RESUME in MP3 pre-stop + program edit mode -> KEY_RESUME -> Can't play problem.
                    if ((__bAttrPlay & TYPE_CDROM_AUDIO) && (__bProgramMaxEntry == 0) && (__wDiscType & CDROM_M1))
                    {
                    }
                    else
                    {
    					// Micky0.75, modify Program-> KEY_PLAY checking
                        if ( ! __bProgramMaxEntry )
                        {
                            LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
                            // Micky0.75, fix Program-> no input-> KEY_PLAY will show invalid.
    	                    OSD_Output(MSG_PROGRAM, 0, 0);	// let display mode clear Program icon
                            break;
                    	}
                        __btPlayingProgram = TRUE;
                        OSD_Output(MSG_PROGRAM, 0, 0);	// let display mode show Program icon
                        __dwTimeKey=NULL_0;
    					if (__btPlaying ||__btPause )
    					{
    						__btNextPlayProgram = TRUE;
#ifdef    PROGRAM_PLAY_DIRECT
                            __bFWKey= KEY_NEXT;
#endif    // PROGRAM_PLAY_DIRECT
    						break;			
    					}
                    }
                }
            }   //check trigger PLAY
            // DVD_160Micky, make number input mode has top priority end

            // wyc2.37-909s, replace to NO_DISC_MODE
#ifndef NO_DISC_MODE //++CoCo2.37p
            if ( __btPBC )
            {
                if ( ( bKey == KEY_PLAY ) )
                {
                    if ( __bModeCmd == KEY_STOP )
                    {
                        if ( __wStatus & ID_ENDLIST )
                        {
                            __wStatus= NULL_0;
                            __btPlaying= FALSE;
                            __btPause= FALSE;
// ** TCH1.61-2;                             __bModeCmd= KEY_PLAY;       //force it into play mode
                            __bModeCmd= KEY_PAUSE;  // ** TCH1.61-2;  Pause is using by VCD20 Module
                            VCD20_Action ( LIST_FIRST );
                            break;
                        }
                    }
                    else
                    {
                        // VCD20 default play function lost
                        // when menu playend, __bModeCmd will change to KEY_PAUSE
                        if ( (__bModeCmd == KEY_SF) || (__bModeCmd == KEY_FF) ||
                            (__bModeCmd == KEY_SCB) || (__bModeCmd == KEY_SCF) ||
                            (__bModeCmd == KEY_STEP) || (__bModeCmd == KEY_FREEZE) )
                            //                        if (__bModeCmd!=KEY_PLAY)
                        {
                            //do motion switching
                        }
                        // Pause-> Play, taken as normal case(non-PBC)
                        else if ( ! __btPause )
                        {
                            // Check LIST_DEFAULT is valid or not
                            // Current Invalid will by-pass KEY_PLAY, don't show "INVALID"
                            if ( VCD20_Action ( LIST_DEFAULT ) )
                                break;
                        }
                    }   // ! KEY_STOP
                }   // KEY_PLAY
                else if (bKey != KEY_STOP)
                {
                    // KEY_SCF/KEY_SCB/KEY_FREEZE/KEY_STEP/KEY_FF/KEY_SF

                    // only enable KEY_PLAY to trigger VCD 2.0 play
                    if (!__btPlaying && !__btPause)
                        return INVALID_KEY;
                }
            }   //__btPBC
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
            return CC_KeyCommand ( bKey );
            break;

    // Micky2.81, Merge KEY_STOP's action together
    // also move in the part that are outside CC_DoKeyAction(Show LOGO and MSG_STOP)
    case    KEY_STOP:
        {
        extern  DWORD   _dwTimePrev;    // [UTL]
        _dwTimePrev= 0xfffffff; // ** TCH1.60-1; Let Time Display can show current time.
        }
        
        if (bKey==KEY_STOP)
        {
            // wyc1.00-909, temply not set __btPlayEnd to TRUE when VCD2.0 disc to fix VCD2.0 stop->resume but can't play problem.
            if (!(__wDiscType & VCD_2_0))
                __btPlayEnd=TRUE;
            OSD_OUTPUT_MACRO ( MSG_SELECTIONLIST, 0xFFFF,  0 );
        }

#ifndef NO_DISC_MODE //CoCo2.37p
        // ** TCH0.452; begin...
        // One DVD-Title "Gloden Eye" from mustek.
        // The play is inhibited as region-code (?),
        // We can do STOP-> Play.....
        // Micky2.81, only check UOP_CAN_STOP when DVD channel
#ifdef  SUPPORT_AV_SYSTEM
        if (__bAUXChannel == AUX_CHANNEL_DVD)
#endif  //#ifdef  SUPPORT_AV_SYSTEM
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
        if(!__bSetupMode)       //Alan2.02, if SETUP cause stop, force it enter stop mode.
#endif
        {
            if ( ! NV_QueryUOP ( UOP_CAN_STOP ) )
            {
                return INVALID_KEY;
            }
        }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

// Micky2.79, tune the usage of __bCountStop,
// only the process KEY_STOP will reference DOUBLE_STOP define
// if DOUBLE_STOP is not defined, __bCountStop will be 2(real stop) only.
#ifdef DOUBLE_STOP
        if ( __bKeySource == KEY_FROM_ISR)
        {
            __bCountStop++;
            if ( __bCountStop >= 2 )
            {
#ifdef SET_REPEAT_OFF_WHEN_FULL_STOP
                __bRepeat = REPEAT_NONE;
#endif //
                __bCountStop = 2;
            }

            // MP3(w/o resume)/ Digest don't have pre-stop state.
            if ( 
// Micky1.20, 4M flash
#ifndef NO_DIGEST
                __btDIGEST ||
#endif
//Micky0.66-909, don't support stop/resume, pre-stop for CDG.
#ifndef NO_CDG                
        		(__bCDDA_DTS == CDDA_CDG) ||
#endif
#ifndef SUPPORT_MP3_RESUME
                (__bAttrPlay == ATTR_MP3) ||
                // LLY2.13, don't assign __bCountStop = 2 while support MP3/ MP2 resume feature
                // Otherwise, the action maybe fail by press KEY_RESUME
                (__bAttrPlay == ATTR_MP2) ||                
#endif  //#ifndef SUPPORT_MP3_RESUME
//Alex1.10-2, 20031105 System will support MP2 format audio
               0 
               )
               __bCountStop = 2;
        }
        // some case will call DoKeyAction(KEY_STOP),such as KEY_SCB
        // __bKeySource will be KEY_FROM_NONE
        else if ( __bKeySource == KEY_FROM_FW )
#endif  //#ifdef  DOUBLE_STOP
// If DOUBLE_STOP is not defined, __bCountStop will be 2(real stop) only.
        {
            __bCountStop = 2;
        }

        // Micky2.79-2, support FullScreen function
        // always clear Full screen status when stop.
        __bFullScreen = 0;

        // DVD_273.2, for encrpyted DVD begin
        // clear the auto scan next 2 cell's command.
        __bAutoScans = 0;

        //must clear repeat AB and GOTO time
        //must be done before ModeInitial
        __btFromStopMode=FALSE;
        __btPlayFromBegin=FALSE;

// Micky1.24, add a define to clet REPEAT status when real stop
#ifdef CLEAR_REPEAT_WHEN_REAL_STOP		
        if( (__bCountStop==2)&&__bRepeat )
        {
	    	__bRepeat=REPEAT_NONE;
        }
#endif
        if (__bRepeatAB!=REPEAT_NONE)
        {
            // Micky0.85
            __bRepeatAB = REPEAT_NONE;
            OSD_OUTPUT_MACRO ( MSG_REPEATAB, REPEAT_NONE, 0);
        }

//alex1.23,20040218 combine gototime , search
//        if (__btGoToTime)
        if (__bSearchMode != SEARCH_MODE_OFF)
        {
            // Micky0.62-909
            OSD_RemoveUI(OSD_UI_SEARCH);    // it will cal Exit(control_only)
        }

        if (__btProgram )
//DVD_099dMicky, don't clear PROGRAM/RANDOM when pre-stop
    	{
            if (  __bCountStop >=2 )
            {
                //Alan2.32, bug #2025. When it is in Program mode, don't clear it.
                // wyc2.32a, update Alan's code and modify to always clear program entries in CDROM mode.
                if ((!__btInputProgram) || (__wDiscType & CDROM_M1))
                {
                    LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
                }
                // wyc1.10, update FM UI to normal because of real stop need to enter normal UI now.
                if (__wDiscType & CDROM_M1)
                {
                    // wyc2.32-909S, when issue stop command in program list mode, need to back to normal mode.
                    if (__bMMMenuID != MM_MENU_PROGLIST)
                    {
                        MM_InitialMenu(MM_MENU_NORMAL, TRUE);
                    }
                    else
                    {
                        MM_ProgramListToNormal();
                    }
                }
            }
    	}

// Micky1.20, 4M flash
#ifndef NO_DIGEST
        if (__btDIGEST)
        {
            OSD_OUTPUT_MACRO ( MSG_DIGEST, NULL_0, NULL_0 );    // clear Digest
            __btDIGEST=FALSE;
//LJY1.24-3, restore DSP ucode after exited digest mode.
#ifdef  USE_32M_SDRAM
            //UTL_DSPuCodeBackup(FALSE);
#endif            
        }
#endif
        // wyc1.02-909, need to clear PIP when KEY_STOP.
        if (!(__wDiscType & CDROM_M1))
        {
            if (__bZoomFactor != VIEW_NORMAL)
            {
                DISP_DisplayCtrl(DISP_PIP, FALSE);
#ifdef SUPPORT_GRAPH_ZOOM
                OSD_OUTPUT_MACRO ( MSG_ZOOM, VIEW_ZOOMX1, 0x3 );
#endif //
                OSD_OUTPUT_MACRO ( MSG_ZOOM_TEXT, VIEW_ZOOMX1, 0x3 );
            }
        }

        if ( ! __btPBC )
        {
            // wyc1.10, make CDROM key stop in program playing same as other titles.
            /*
            // wyc1.05-2, not clear program entries in KEY_STOP in program playing mode.
            if (__wDiscType & BOOK_M1)
                UTL_ModeInitial( MODE_NOT_CLEAR_PROGRAM );
            else
            */
                UTL_ModeInitial ( MODE_LINEAR );
            // re-set __wplaytem to first Track of the title
            // Micky0.91, keep the track number when pre-stop.
            if ( __bCountStop==2 )
            {
                __wPlayItem= __wTrackBegin;
            }
        }

        CHIPS_OpenAudio(FALSE);

        {
            // wyc2.53-909P,
            SrcFilter_Stop (__dwSFStreamID);  // avoid CDROM operation when pseudo-STOP
#ifndef    NO_DISC_MODE
            NV_Action ( ACTION_STOP, NULL_0 );  // ** TCH0.37; Let NV stay on STOP mode
#endif    // NO_DISC_MODE
        }

        CC_KeyCommand ( bKey );
        break;    

    case    KEY_PAUSE:
        // to avoid the CD-IF is colsed before I frame is detected.
        // prohibit scan pause
        // wyc2.35-909S. mark these code to support scan mode pause.
        /*if ( (__bModeCmd==KEY_SCF) || (__bModeCmd==KEY_SCB)
// DVD_160Micky, don't allow pause when KEY_FF mode
#ifdef  SAME_FAST_AS_SCF2
            || (__bModeCmd == KEY_FF)
#endif
            )
            return  INVALID_KEY;*/

#ifndef NO_DISC_MODE //CoCo2.37p
        if (__wDiscType & BOOK_DVD)
        {
            // 1. Only check pause on UOP when non-setup mode
            if (!__bSetupMode)
            {
               // DVD_170aMicky, ref. Annex J VIX-62.
                // only pause on can do UOP check
                // for pause off, it's not applicable.
                if (!__btPause)
                {
                    // check pause UOP.
                    if ( ! NV_QueryUOP (UOP_CAN_PAUSE_ON) )
                        return INVALID_KEY; // don't allow pause.
                }
            }
            else
            // 2. Do nothing for cell/vobu still when setup.
            {
                // DVD_170aMicky, add protection  for Setup call CC_KeyCommand(KEY_PAUSE)
                // When enter setup mode, can't do anything to cell/vobu still
                // cell/vobu still release will be done by _CC_ReleaseStillTrigger
                if (CELL_QueryStatus (CELL_VOBU_STILL_ON) || CELL_QueryStatus(CELL_IS_STILL) )
                    return TRUE;
            }
        }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

        if ( __btPause )
        {
            CC_KeyCommand ( __bModeCmd );
        }
        else
        {   // enter Pause mode
            if ( ! __btPlaying )
                return  INVALID_KEY;

            // For PLAY/SLOW, using STEP command to pause
            __btPause= TRUE;
            __btPlaying=FALSE;

            CHIPS_OpenAudio(FALSE);
            // Micky0.80a, issue pause command for all
            UTL_TransferCommandID(KEY_PAUSE);
            
/*            
            {
                if ( (__bModeCmd==KEY_PLAY) ||
                    (__bModeCmd==KEY_SF) )
                {
                    EX1=0;
                    // DVD_155Micky, use hal new define API
                    // Brian.161A, write a zero value for PAUSE mode to make sure SP display
                    HAL_ControlAVSync(HAL_AVSYNC_STEP, 0);

                  //Kevin1.23, close CDIF before issuing pause command
                    HAL_CheckCDIF(HAL_CDIF_FREEZE_TIME, NULL_0);  

                  UTL_TransferCommandID(KEY_PAUSE);
                  HAL_ReadInfo (HAL_INFO_STC, &__dwSTCStep);// keep the STC when pause for later case if pause-->step
                }
                else
                if (__bModeCmd != KEY_STEP)
                {
                    HAL_CheckCDIF(HAL_CDIF_FREEZE_TIME, NULL_0);
                }
            }
    */            
            UTL_GetCurrentTime ();
            __dwTimeNow++;
// f/w 0.43 fix(01.19.98) begin
// show pause message at last
// so it won't have the effect that the OSD already show pause,
// but the time just go ahead 1 sec.
            UTL_OutputShowTime ();  // The function will call OSD/PANEL_TRIGGER
            // LLY.099, don't show message to fix "MSG_PAUSE" will be seen
            // while enter/exit SETUP menu
            if(!__bSetupMode)
            {
                OSD_OUTPUT_MACRO ( MSG_PAUSE, NULL_0, 0xFF );
            }
// f/w 0.43 fix(01.19.98) end
        }
        break;

    default:
        // Micky0.65-909, must return INVALID_KEY;
        return INVALID_KEY;
}
    return TRUE;
}

//  *****************************************************************
//  Function    :   _KeyGroupTest
//  Description :   Process Video Group
//  Return      :   None
//  Side Effect :
//  *****************************************************************
BYTE    _KeyGroupTest ( BYTE bKey)
{
    // Micky0.62-909, process number key
    //put at the group temporarily, as KEY_NUMBER is defined before GROUP2
    if ( bKey == KEY_NUMBER )
    {
    	// Micky0.72
    	// KEY_NUMBER will be issued when 1) Any number is pressed, 2)number is confirmed(or time out accept)
        if (CC_NumberUtility(NUMBER_SET_INPUT)) // number is not decided
        {
        	// do nothing
            return KEY_NO_KEY;
        }
        if ( !LINEAR_Action(KEY_NUMBER, __dwNumberPrevious) )
        {
	      	// Micky0.72, number action failed, clear current display number
            OSD_OUTPUT_MACRO ( MSG_NUMBER, 0, 0 );        
            bKey = INVALID_KEY;
        }
        else
        {
        	// Micky0.72, fix number can't be display when VCD number action.
        	// number action is done. let the number OSD last for 1s.
	        OSD_OUTPUT_MACRO ( MSG_NUMBER, __dwNumberPrevious, 1 );
            bKey = KEY_NO_KEY;
        }
        CC_NumberUtility(NUMBER_CLEAR);
        return bKey;
    }
    
    
    //kevin0.76, debug IR hang
    // wyc2.32-909S, remove un-used testing code.
    /*else if ( bKey == KEY_TEST1 )
    {   
#ifdef SUPPORT_CC_DEBUG_INFO
        printf("REG_PLAT_RESET_CONTROL_DISABLE=%lx", REG_PLAT_RESET_CONTROL_DISABLE);
        printf("REG_PLAT_IR_TIMING_CONTROL1=%lx", REG_PLAT_IR_TIMING_CONTROL1);
        printf("REG_PLAT_IR_TIMING_CONTROL2=%lx", REG_PLAT_IR_TIMING_CONTROL2);
        printf("REG_PLAT_IR_TIMING_CONTROL3=%lx", REG_PLAT_IR_TIMING_CONTROL3);
        printf("REG_PLAT_IR_TIMING_CONTROL4=%lx", REG_PLAT_IR_TIMING_CONTROL4);
        printf("REG_PLAT_IR_DATA=%lx", REG_PLAT_IR_DATA);        
        printf("REG_PLAT_IR_RAW_CODE=%lx", REG_PLAT_IR_RAW_CODE); 
        printf("REG_PLAT_IR_COUNTER=%lx", REG_PLAT_IR_COUNTER);                 
        printf("REG_PLAT_IR_STATUS=%lx", REG_PLAT_IR_STATUS);         
#endif //
        return KEY_NO_KEY;                  
    }        
    else if ( bKey == KEY_TEST2 )
    {    
        extern VOID _ResetIR(VOID); 
        //printf("RS IR ");
        _ResetIR();  
        return KEY_NO_KEY;            
    }
    else if ( bKey == KEY_TEST4 )
    {
        //printf("\nDIS Par EDC");
        PARSER_Command(0, PARSER_CMD_IGNORE_EDC, (DWORD*)1);
        return KEY_NO_KEY;            
    }*/
    
    return INVALID_KEY;        
}

// Micky1.00_909
// Auto Sleep > Auto powerown
// 1. Need to check the status after process each key.
//    - After title detection, it was set as STOP(__dwTimeWaitPowerDown=__dwCountSystem) 
//    - The status of STOP : none of item was playing.(__bModecmd=STOP(except Digest/ Thumbnail))
//    - There will have problem if the status flag was changed after Trigger
//    - (Thumbnail-> __btPlaying=0-> __dwTimeWaitPowerDownset-> Trigger-> __btPlaying=1->
//    - after thumbnail play end, it till enter power down mode
// 2. If the status was kept at STOP, record the time(__dwTimeWaitPowerDown)
// 3. If the status was not STOP, reset the time(__dwTimeWaitPowerDown=0xffffffff)
// 4. Just need to monitor __dwCountSystem-__dwTimeWairPowerDown>WAITTIME_POWERDOWN
#ifdef  AUTO_POWERDOWN
void    _CC_Trigger_AutoPowerDown(void)
{
        if (!__btPowerDown)
        {
            if (__dwTimeWaitPowerDown != 0xFFFFFFFF)
            {
                // Micky1.25, add auto powerdown for screen saver mode
#ifdef  POWERDOWN_AFTER_SCREENSAVER
                if ( __bScreenSaver != TRIGGER_SCREEN_SAVER )
                {
                    __dwTimeWaitPowerDown=0xFFFFFFFF;    
                }
#else
                // after start trigger time
                // and the key trigger into play state
                // must clear the wait time
                if (
// Micky1.20, 4M flash
#ifndef NO_DIGEST
                    __btDIGEST || 
#endif
                    __btPlaying || __btPause || __btNewTime ||
                // DVD_101-2Micky, reference __bSetupMode instead of __bSetupID
                    (__bSetupMode) && __btDriveOpen)
                    __dwTimeWaitPowerDown=0xFFFFFFFF;
                else
                {
                    // after start trigger time
                    // and the key don't trigger into play state
                    // must reset the wait time
                    // just reset the start trigger time
                    __dwTimeWaitPowerDown=OS_GetSysTimer();
                }
            }
            else
            {
                // not in trigger time state
                // if key pressed, but still don't trigger play status
                // just reset the start trigger time
                if (
// Micky1.20, 4M flash
#ifndef NO_DIGEST
                    !__btDIGEST && 
#endif
                    !__btPlaying && !__btPause && !__btNewTime &&
                // DVD_101-2Micky, reference __bSetupMode instead of __bSetupID
                    (!__bSetupMode) && !__btDriveOpen)
                {
                    __dwTimeWaitPowerDown=OS_GetSysTimer();
                }
#endif  //#ifdef  POWERDOWN_AFTER_SCREENSAVER
            }
        }
}
#endif  //#ifdef  AUTO_POWERDOWN

//  *********************************************************************
//  Function    :   CC_NumberUtility
//  Description :   This function will do related number action
//  Arguments   :   NUMBER_CLEAR,    reset current number input control.
//              :   NUMBER_SHIFT,    shift the number value left one digit
//		        :   NUMBER_DIGITS,   count the number of digits.
//              :   NUMBER_SET_INPUT,    query the number input status    
//              :   __dwNumberPrevious, __wNumberQueue
//  Return      :   the status
//  Side Effect :
//  *********************************************************************
BYTE    CC_NumberUtility(BYTE bAction)
{
    WORD    wCheckDigit;
    WORD    wNumberBase;
    BYTE    i;
    // Micky0.72
    BYTE bFindZero;
    
    if (bAction == NUMBER_CLEAR)
    {
    	// Micky0.72, let OSD auto clear
        //OSD_OUTPUT_MACRO(MSG_NUMBER, 0, 0);
        __dwTimeKey=NULL_0;
        __wNumberQueue=0;
        __dwNumberPrevious=0;
        bNumberTimeOut=FALSE;
    }
    else if ( bAction == NUMBER_SHIFT )
    {
    	// 10045 shift will become 45, __wNumberQueue = 0x3;
        wCheckDigit=0x10;   // Max. 5 digits
        wNumberBase=MAX_DIGITS_NUMBER;
        // Micky0.72
        bFindZero = FALSE;
        for (i=MAX_DIGITS;i>=1;i--)
        {
        	// find the next non-zero digit
        	if (bFindZero)
    		{
    		
        		if (__dwNumberPrevious/ wNumberBase)
    			{
        			break;
    			}
        		else
    			{
	                __wNumberQueue >>=1;
    			}
    		}
        	else
            if ( __wNumberQueue & wCheckDigit )  // search the highest digit
            {
                __dwNumberPrevious -= (__dwNumberPrevious/wNumberBase) *wNumberBase;
                __wNumberQueue >>=1;
                bFindZero= TRUE;
            }
	        wCheckDigit >>=1;
            wNumberBase/=10;
        }
    }
    else if ( bAction == NUMBER_DIGITS )
    {
    	// Micky0.72, return number of digits by __wNumberQueue
    	// can't use the method for KEY_10+
    	/*
        wCheckDigit=0x10;   // Max. 5 digits
        for (i=MAX_DIGITS;i>=1;i--)
    	{
    		if (wCheckDigit & __wNumberQueue)
    			return i;
    		else
    			wCheckDigit >>=1;
    	}
        return i;
        */
    	// count from highest non-zero digit.
        wNumberBase = MAX_DIGITS_NUMBER;
        for (i=MAX_DIGITS;i>=1;i--)
        {
            if (__dwNumberPrevious/ wNumberBase)
        	{
                break;
        	}
            else
        	{
                wNumberBase/=10;
        	}
        }
        return i;
    }
    else if ( bAction == NUMBER_SET_INPUT )
    {
        if (bNumberTimeOut)
            return TRUE;    // number input state
        else
            return FALSE;
    }
    else if ( bAction == NUMBER_SET_TIMEOUT)
    {
        bNumberTimeOut=TRUE;
        _dwTimeNumber = OS_GetSysTimer();
    }
    else if ( bAction == NUMBER_CLEAR_TIMEOUT)
    {
        bNumberTimeOut=FALSE;
    }
    // Micky0.72, update number queue
    else if (bAction == NUMBER_QUEUE)
	{
      	wNumberBase = MAX_DIGITS_NUMBER;
        for (i=MAX_DIGITS;i>=1;i--)
        {
            if (__dwNumberPrevious/ wNumberBase)
        	{
                break;
        	}
            wNumberBase/=10;
        }
        __wNumberQueue = 1;
        if (i)
    	{
    		for (; i>=1; i--)
    		{
    			__wNumberQueue *=2;
	    	}
    	}
    	__wNumberQueue-=1;
       	return TRUE;		
	}

    return  TRUE;
}
//  *********************************************************************
//  Function    :   _CC_NumberProcessKey
//  Description :   This function will do number calculation for input number
//  Arguments   :   bKey,   input number key
//  Return      :   KEY_NUMBER, plus __dwTimeKey, __dwNumberPrevious
//  Side Effect :
//  *********************************************************************
// Micky0.80, remove parameter
BYTE    _CC_NumberProcessKey(void)
{
    WORD wValue;
    BYTE bNumberMode;
    BYTE    bNumberMaxDigits;
#define NUMBER_NORMAL   1
#define NUMBER_SHIFT_LEFT    2

//    BYTE    bKey= __bKey;
    if  ( ( __bKey >= KEY_N0 ) && (__bKey <= KEY_N20 ) )
    {
       wValue = __bKey - KEY_N0;
    }
    else if (__bKey == KEY_N10PLUS)
    {
        wValue = 0x800A;
    }
    else if (__bKey == KEY_N20PLUS)
    {
        wValue = 0x8014;
    }
    else
    {
        // must clear number related if any ISR key pressed
        if ( __bKeySource == KEY_FROM_ISR )
        {
            if ( CC_NumberUtility(NUMBER_SET_INPUT) )
            {
                if ( ( __bKey == KEY_PLAY) || (__bKey == KEY_PLAY_PAUSE) || (__bKey == KEY_ENTER) || (__bKey == KEY_ACTIVE) )
                {
                    CC_NumberUtility(NUMBER_CLEAR_TIMEOUT);
                    // Micky0.80, since the key is not active as original value, clear the message
                    __bKeyMessage = MSG_NULL;
                    // Micky0.80, return new status
                    __bKey = KEY_NUMBER;
                    return KEY_BYPASS;
                    //return KEY_NUMBER; 
                }
                CC_NumberUtility(NUMBER_CLEAR);
                // Micky0.72, give up current number, clear the number OSD
		        OSD_OUTPUT_MACRO(MSG_NUMBER, 0, 0);
            }
        }
        // Micky0.80, return new status
        return KEY_BYPASS;        
        //return bKey;
    }
#ifdef  SUPPORT_NUMBER_CONFIRM_KEY
    bNumberMode= NUMBER_SHIFT_LEFT;
#else
    bNumberMode= NUMBER_NORMAL;
#endif
    bNumberMaxDigits = 2;

    // means media manager, reference other define for number input mode
    // InfoFilter will update __bNavigateMode
    if ( __bNavigateMode == MEDIA_NAVIGATE_MODE )
    {
#ifdef    SUPPORT_CDROM_NUMBER_CONFIRM_KEY
        bNumberMode= NUMBER_SHIFT_LEFT;
#endif
        bNumberMaxDigits = 4;
    }
// For general display mode(VCD/CD-DA/DVD)
    if (bNumberMode == NUMBER_NORMAL)
    {
        if ( __dwTimeKey )                          // previous key is KEY_N10PLUS
        {
            __dwNumberPrevious +=wValue & 0x7FFF;    // accumulate the number value
        }
        else
        {
            __dwNumberPrevious = wValue & 0x7FFF;
        }
        if (wValue & 0x8000)
        {
            CC_NumberUtility(NUMBER_SET_TIMEOUT);
        }
        else
        {
            CC_NumberUtility(NUMBER_CLEAR_TIMEOUT);
        }

    }
    else
    {
        // __wNumberQueue, bit 0=1, 1st digit is on, bit 1, 2nd digit is on, bit 2, 3rd digit is on
        if ( __dwTimeKey )                          // previous key is KEY_N10PLUS
        {
            // record current number of digits
            __bTemp = CC_NumberUtility(NUMBER_DIGITS); // original digits
            __dwNumberPrevious += wValue & 0x7FFF;
            // Micky0.72, re-calculate __wNumberQueue
            CC_NumberUtility(NUMBER_QUEUE);
            /*
            if (wValue & 0x8000)
            {
            	// Micky0.72, remove the end of ';', bug.
                if ( __bTemp != CC_NumberUtility(NUMBER_DIGITS) )
                {
                    // continue KEY_N10PLUS, don't increase the number digits except carry(90->100)
                    __wNumberQueue <<=1;
                    __wNumberQueue |=1;
                }
            }
            */
        }
        else
        {
            if (wValue & 0x8000)                    // KEY_N10/N20_PLUS.. was pressed
            {
                // First time press KEY_N10PLUS, always set number digit as 2
                __wNumberQueue = 0x03;
                // reset previous number
                __dwNumberPrevious =0;
            }
            else
            {
                // increase digit number for general number
                // wyc2.38-909s, don't increase the checking digit when pressing 0 first. Otherwise will happen user can input to 4-digit 
                // in CDDA problem.
                if ((wValue != 0) || (__dwNumberPrevious != 0))
                {
                    __wNumberQueue <<=1;
                    __wNumberQueue |=1;
                    __dwNumberPrevious *= 10;    // shift the last value one digit left
                }
            }
            __dwNumberPrevious += wValue & 0x7FFF;   
        }
        // set time out
        CC_NumberUtility(NUMBER_SET_TIMEOUT);
    }

    if ( CC_NumberUtility(NUMBER_DIGITS) > bNumberMaxDigits )
    {
    	// Micky0.72, fix number out of max. digits, NUMBER_SHIFT will do it.
        //__wNumberQueue = bNumberMaxDigits+1;
       CC_NumberUtility(NUMBER_SHIFT);
   }

    if (wValue & 0x8000)                    // KEY_N10/N20_PLUS.. was pressed
    {
        __dwTimeKey = OS_GetSysTimer();
    }
    else
    {
        __dwTimeKey = NULL_0;
    }

    if (__dwTimeKey)
    {
        OSD_OUTPUT_MACRO ( MSG_KEY10, (WORD)__dwNumberPrevious, COUNT_NUMBER_GAP/COUNT_1_SEC );
    }
    else
    {
        __bKeyMessage = MSG_NUMBER;
        OSD_OUTPUT_MACRO(MSG_NUMBER, (WORD)__dwNumberPrevious, COUNT_NUMBER_GAP/COUNT_1_SEC);
    }

    // must check if allow number input, if not allow, reset the number
    // if allowed, & not time out, return KEY_NO_KEY.
    // Micky0.80, return new status
    __bKey = KEY_NUMBER;
    return KEY_BYPASS;
    //return KEY_NUMBER;
}
// Micky0.62-909, process number key, then convert as KEY_NUMBER
//  **************************************************************************
//  Function    :   _CC_Trigger_TimeOut_Nummber
//  Description :   Do the action when number input time out
//  Arguments   :   None
//  Return      :   None
//  Side Effect :   
//  **************************************************************************
void _CC_Trigger_NumberTimeOut(void) 

{
    if (bNumberTimeOut)
    {
        if ( ( OS_GetSysTimer()- _dwTimeNumber ) > COUNT_NUMBER_GAP )
        {
#ifdef  ACCEPT_TIMEOUT_NUMBERKEY
            OSD_OUTPUT_MACRO(MSG_NUMBER, (WORD)__dwNumberPrevious, 1);
            bNumberTimeOut = FALSE;
            // Micky0.62-909
            // ie. don't simulate it by assign F/W key again
            // To avoid IR key and F/W key conflict issue
            __bFWKey = KEY_NUMBER;  // means to active the number value
            CC_MainProcessKey();
#else
            CC_NumberUtility(NUMBER_CLEAR);
            // Micky0.72, give up current number, clear the number OSD
   	        OSD_OUTPUT_MACRO(MSG_NUMBER, 0, 0);
#endif
        }
    }
}
// Micky0.80, remove it.
/*
// Micky0.62-909, add VCD/DVD Process Key
//  **************************************************************************
//  Function    :   _CC_DVD_PreProcessKey()
//  Description :   
//  Arguments   :   __bKey
//  Return      :   TRUE, the key value is decided.
//  Side Effect :   
//  **************************************************************************
BYTE    _CC_DVD_PreProcessKey(void)
{
    if ( !( __wDiscType & BOOK_DVD ) )
        return FALSE;

//-------------------------------------------------------------------
// :: Release DVD still
// DVD_161Micky, add general keys for releasing CELL still/ VOBU still
// don't enter pause state for these modes
// make sure the keys defined within _bReleaseStillKeys
// won't return earlier than the checking for release still.
        if ( _CC_ReleaseStillTrigger() == KEY_ACCEPT)
        {
            __bKey = KEY_NO_KEY;
            return TRUE;
        }
// DVD has buttons. (SUPPORT_KEY_PLAY_AS_KEY_ACTIVE)
// when KEY_PLAY is shared use with active
// the rule is to active button first
#ifdef  SUPPORT_KEY_PLAY_AS_KEY_ACTIVE
    if ( (__bKey == KEY_PLAY_PAUSE) || (__bKey == KEY_PLAY) )
    {
        if ( NV_Action(ACTION_ACTIVE_CHECK, NULL_0) )
        {
            // Fix Title menu(motion w/ buttons)-> pause-> KEY_PLAY will hang.
                        // HolySound(travel around the world-8, -3)
                // let non-NPF-> play has top priority
            // i.e. Button active only when normal playing
            if ( __bModeCmd != KEY_PLAY || __btPause )
            {
                __bKey = KEY_PLAY;
            }
            else    // act as confirm key
            {
                __bKey = KEY_ACTIVE;
            }
            return TRUE;    // KEY is decided.
        }
        return FALSE;
    }
#endif  //#ifdef  SUPPORT_KEY_PLAY_AS_KEY_ACTIVE

// :: Shared Keys(DVD only)
//      - KEY_DIGEST---> KEY_TITLE
//      - KEY_AUDIO_LR--->KEY_AST_CTL
//      - KEY_ENTER--->KEY_ACTIVE
//      - KEY_GOTO_TIME--->KEY_SEARCH
#ifdef NO_DVDDIGEST
        if ( __bKey == KEY_DIGEST )
        {
            __bKey = KEY_TITLE;
            return TRUE;
        }
#endif  //#ifdef NO_DVDDIGEST    
#ifdef  KEY_AUDIO_LR_SAME_KEY_AST_CTL
        if ( __bKey == KEY_AUDIO_LR )
        {
            __bKey = KEY_AST_CTL;
            return TRUE;
        }
#endif  //#ifdef  KEY_AUDIO_LR_SAME_KEY_AST_CTL
    return FALSE;
}

BYTE    _CC_VCD_PreProcessKey(void)
{
        if ( !( __wDiscType & BOOK_VCD ) )
        {
            return FALSE;
        }
    
        // ** TCH0.60-1; begin... 
        // below code is for PBC ON mode.
        if ( !__btPBC )     
            return FALSE;


    // these codes almost same as _CC_DVD_PreProcessKey ??
    if ( ( __bKey == KEY_PLAY_PAUSE ) || (__bKey == KEY_PLAY) )
    {
        if ( VCD20_IsValid_Command ( LIST_DEFAULT ) )
        {
            // let non-NPF-> play has top priority
            if ( __bModeCmd != KEY_PLAY || __btPause )
            {
                __bKey = KEY_PLAY;
            }
            else    // act as confirm key
            {
                __bKey = KEY_DEFAULT;
            }
            return TRUE;
        }
        return FALSE;
    }

    return FALSE;
}
*/
//  **************************************************************************
//  Function    :   CC_VCD_ProcessKey()
//  Description :   Process VCD2.0 related KEY
//  Arguments   :   bKey
//  Return      :   None
//  Side Effect :   
//  **************************************************************************
#ifndef NO_DISC_MODE //++CoCo2.37p
// Micky0.80, remove parameter
BYTE    CC_VCD_ProcessKey(void)//BYTE bKey)
{
    if ( !( __wDiscType & VCD_2_0 ) )
        return KEY_BYPASS;  // Micky0.80, return new status
        //return bKey;
    if ( !__btPBC && (__bKey != KEY_PBC) && (__bKey != KEY_MENU) )
        return KEY_BYPASS;  // Micky0.80, return new status
//        return bKey;
// Micky0.77, just return the same key when program input mode.
#ifdef FORCE_SWITCH_NONPBC
	if( __btInputProgram )
        return KEY_BYPASS;  // Micky0.80, return new status
//		return bKey;
#endif    
    // Micky0.80, move from CC_PreProcessKey
if ( __bKeySource == KEY_FROM_ISR )    
{
 // these codes almost same as _CC_DVD_PreProcessKey ??
    if ( ( __bKey == KEY_PLAY_PAUSE ) || (__bKey == KEY_PLAY) )
    {
        if ( VCD20_IsValid_Command ( LIST_DEFAULT ) )
        {
            // Micky0.80, always show PLAY.
            _CC_MapKeyMessage(KEY_PLAY);
            // let non-NPF-> play has top priority
            if ( __bModeCmd != KEY_PLAY || __btPause )
            {
                __bKey = KEY_PLAY;
                return KEY_TODO;
            }
            else    // act as confirm key
            {
                __bKey = KEY_DEFAULT;
            }
        }
        else
            return KEY_BYPASS;
    }        
        
    // only enable KEY_PLAY to trigger VCD 2.0 play
    if ( ( __bKey >= NONNPF_PLAYABLEKEY_BEGIN ) && ( __bKey <= NONNPF_PLAYABLEKEY_END ) )
    {
        if (__bModeCmd == KEY_STOP )
            return INVALID_KEY;
    }
}
    switch(__bKey)
    {
        case    KEY_PLAY:
            if ( __bModeCmd == KEY_STOP )
            {
                if ( __wStatus & ID_ENDLIST )
                {
                    __wStatus= NULL_0;
                    __btPlaying= FALSE;
                    __btPause= FALSE;

                    __bModeCmd= KEY_PAUSE;  // ** TCH1.61-2;  Pause is using by VCD20 Module
                    VCD20_Action ( LIST_FIRST );
                    break;
                }
            }
            else
            {
                // VCD20 default play function lost
                // when menu playend, __bModeCmd will change to KEY_PAUSE
                if ( (__bModeCmd == KEY_SF) || (__bModeCmd == KEY_FF) ||
                     (__bModeCmd == KEY_SCB) || (__bModeCmd == KEY_SCF) ||
                     (__bModeCmd == KEY_STEP) || (__bModeCmd == KEY_FREEZE) )
                {
                    //do motion switching
                }
                // Pause-> Play, taken as normal case(non-PBC)
                else
                if ( ! __btPause )
                {
                    // Check LIST_DEFAULT is valid or not
                    // Current Invalid will by-pass KEY_PLAY, don't show "INVALID"
                    if ( VCD20_Action ( LIST_DEFAULT ) )
                        break;
                }
                return KEY_BYPASS;  // Micky0.80, return new status
//                return bKey;
            }
            break;
        case    KEY_DEFAULT:
            if ( __wStatus != ID_ENDLIST )
            {
                if ( VCD20_Action ( LIST_DEFAULT ) )
                    break;
            }
            return INVALID_KEY;
            break;
        case    KEY_MENU:
        case    KEY_PBC:
        CC_KeyCommand ( KEY_STOP );
#ifndef NO_DISC_MODE //++CoCo2.37p
        if ( __btPBC )
        {
            __btPBC= FALSE;
            OSD_OUTPUT_MACRO(MSG_PBC, FALSE, 3);

            __wStatus= NULL_0;            // clear the record to normal PBC value
            __wPlayItem= __wTrackBegin; // from first Track

#ifdef    AUTO_PLAY
            CC_KeyCommand ( KEY_PLAY );
#else
			// Micky0.65-909. 
            UTL_ShowLogo();
			SrcFilter_Stop(__dwSFStreamID);
#endif    // AUTO_PLAY
        }
        else
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
        {
            __dwResumeTime=NULL_0;
            //CC_KeyCommand ( KEY_STOP );

            UTL_ModeInitial ( MODE_PBC );
            // DVD_101bMicky, show the PBC OSD after mode initial
            // or Program->PBC has problem.
            // OSD need adjust.
            OSD_OUTPUT_MACRO(MSG_PBC, TRUE, 3);

#ifdef    AUTO_PLAY
            __wStatus= NULL_0;
            VCD20_Action ( LIST_FIRST );
#else
            //wait user input key
            __wStatus= ID_ENDLIST;
			// Micky0.65-909. 
            UTL_ShowLogo();
			SrcFilter_Stop(__dwSFStreamID);
#endif    // AUTO_PLAY
        }
            break;
        case    KEY_STOP_RETURN:
//-------------------------------------------------------------------
// :: Composite KEY_STOP_RETURN, KEY_STOP/ KEY_RETURN
// :: Act as KEY_RETURN
//      - PBC has return list
// :: Act as KEY_STOP
//      - Exclusive with above cases
            if ( !VCD20_IsValid_Command ( LIST_RETURN ) )
            {
                __bKey = KEY_STOP;
                return KEY_BYPASS;  // Micky0.80, return new status
//                return KEY_STOP;
            }
        case    KEY_RETURN: 
            {
                if ( __wStatus != ID_ENDLIST )
                {
                    //It's ok to clear __wStatus, because VCD20_Trigger retrun value
                    //will set __wStatus again
                    __wStatus=NULL_0;
                    VCD20_Action ( LIST_RETURN );
                }     
                else
                    return  INVALID_KEY;
            }
            break;
// Micky0.62-909, process number key
    case    KEY_NUMBER:
#ifdef  AUTO_PBCOFF
        if (__bModeCmd == KEY_STOP )    // number key will switch PBC off
        { 
            if (CC_NumberUtility(NUMBER_SET_INPUT)) // number is not decided
                return KEY_NO_KEY;
            __btPBC=FALSE;
            OSD_OUTPUT_MACRO(MSG_PBC, FALSE, 0);
            __bKey = KEY_NUMBER;
            return KEY_TODO;  // Micky0.80, return new status
//            return KEY_NUMBER;
        }
#endif  //#ifdef  AUTO_PBCOFF
        
        if (CC_NumberUtility(NUMBER_SET_INPUT)) // number is not decided
            return KEY_NO_KEY;
        // Micky0.72, check the return value
        if ( VCD20_KeyNumber((WORD)__dwNumberPrevious) == TRUE )
    	{
          	// Micky0.72, number action is done. clear display number
	        OSD_OUTPUT_MACRO(MSG_NUMBER, (WORD)__dwNumberPrevious, 1);
      	}
        else
    	{
            OSD_OUTPUT_MACRO ( MSG_INVALID, OSD_ERR_KEY, 1 );
    	}
        CC_NumberUtility(NUMBER_CLEAR);
        return KEY_NO_KEY;
        break;  
        
        default:
            return KEY_BYPASS;  // Micky0.80, return new status
//            return bKey;
    }
    return KEY_NO_KEY;
}
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
//  **************************************************************************
//  Function    :   CC_DVD_ProcessKey()
//  Description :   Do the DVD title related KEY processing
//  Arguments   :   bKey
//  Return      :   None
//  Side Effect :   
//  **************************************************************************
// Micky0.80, remove parameter
#ifndef NO_DISC_MODE //CoCo2.37p
BYTE    CC_DVD_ProcessKey(void)//BYTE bKey)
{
    // non-DVD, just return the key value
    if ( !( __wDiscType & BOOK_DVD ) )
        return KEY_BYPASS;  // Micky0.80, return new status
//        return bKey;

    // Micky0.80
if ( __bKeySource == KEY_FROM_ISR )    
{
//-------------------------------------------------------------------
// :: Release DVD still
// DVD_161Micky, add general keys for releasing CELL still/ VOBU still
// don't enter pause state for these modes
// make sure the keys defined within _bReleaseStillKeys
// won't return earlier than the checking for release still.
        if ( _CC_ReleaseStillTrigger() == KEY_ACCEPT)
        {
            return KEY_NO_KEY;
        }
// DVD has buttons. (SUPPORT_KEY_PLAY_AS_KEY_ACTIVE)
// when KEY_PLAY is shared use with active
// the rule is to active button first
#ifdef  SUPPORT_KEY_PLAY_AS_KEY_ACTIVE
    if ( (__bKey == KEY_PLAY_PAUSE) || (__bKey == KEY_PLAY) )
    {
        if ( NV_Action(ACTION_ACTIVE_CHECK, NULL_0) )
        {
            // Fix Title menu(motion w/ buttons)-> pause-> KEY_PLAY will hang.
                        // HolySound(travel around the world-8, -3)
                // let non-NPF-> play has top priority
            // i.e. Button active only when normal playing
            if ( __bModeCmd != KEY_PLAY || __btPause )
            {
                __bKey = KEY_PLAY;
                _CC_MapKeyMessage(__bKey);
                return KEY_TODO;
            }
            else    // act as confirm key
            {
                __bKey = KEY_ACTIVE;
                _CC_MapKeyMessage(__bKey);
            }
        }
        else
            return KEY_BYPASS;
    }
#endif  //#ifdef  SUPPORT_KEY_PLAY_AS_KEY_ACTIVE

// :: Shared Keys(DVD only)
//      - KEY_DIGEST---> KEY_TITLE
//      - KEY_AUDIO_LR--->KEY_AST_CTL
#ifdef NO_DVDDIGEST
        if ( __bKey == KEY_DIGEST )
        {
            __bKey = KEY_TITLE;
        }
#endif  //#ifdef NO_DVDDIGEST    
#ifdef  KEY_AUDIO_LR_SAME_KEY_AST_CTL
        if ( __bKey == KEY_AUDIO_LR )
        {
            // Micky0.80, let main flow process it.
            __bKey = KEY_AST_CTL;
            // Micky0.85, fix multi-audio can't switch
            return KEY_TODO;            
        }
#endif  //#ifdef  KEY_AUDIO_LR_SAME_KEY_AST_CTL
   if ( __bKey == KEY_NEXT )
    {
        if ( __bModeCmd == KEY_STOP )
        {
#ifdef   SUPPORT_NEXT_TRIGGER_FROM_STOP                   // ** TCH1.72;
            // Suppose it play from first Track
            __bKey=KEY_PLAY;
            // Micky0.80, assign __bKeyMessage
            _CC_MapKeyMessage(__bKey);
            return KEY_TODO;
#endif  //#ifdef   SUPPORT_NEXT_TRIGGER_FROM_STOP                   // ** TCH1.72;
        }
    }
}        
switch ( __bKey )
{
    case    KEY_ENTER:    
    case    KEY_ACTIVE:
#ifdef SUPPORT_PRINTF
            PRINTF ( " * DVD Button Active;" );
#endif  //#ifdef SUPPORT_PRINTF
            __bTemp= NV_Action ( ACTION_ACTIVE, NULL_0 );
            if ( ! __bTemp )
                return  INVALID_KEY;
            break;

    case    KEY_PBC:    // PlayBack Control switch
            __bNVPlayMode= ! __bNVPlayMode;
            //Aron1.11-909, constrained repeat, disable repeat mode when switch PBC ON/OFF
            __bRepeat= REPEAT_NONE;            
            OSD_OUTPUT_MACRO(MSG_REPEAT, __bRepeat, 0 );
            // wyc.pro , in DVD title, going to NV mode must clear program variables and then goto NV playing when press KEY_PBC
            if (__bNVPlayMode)
                UTL_ModeInitial ( MODE_LINEAR );
            OSD_OUTPUT_MACRO ( MSG_PBC, __bNVPlayMode, 3 );
            if ( ! __bNVPlayMode )
            {
                if ( __bDomain != TT_DOM )
                    NV_TitlePlay ( 1, 1 );
            }
            break;
    case    KEY_NEXT:
    case    KEY_PREV:
            if ( __bKey == KEY_NEXT )
                __bTemp= ACTION_NEXT;
            else
                __bTemp= ACTION_PREV;

            // ** TCH1.03; begin... move from below. Done before call NV_Action.
            // ** TCH1.00; begin... Enhance the XQ request. Push KEY_NEXT will clear REPEAT_CHAPTER only
            if ( __bKeySource == KEY_FROM_ISR )    // KEY_FROM_FW
            {
                if ( __bDomain & TT_DOM )
                {
#ifdef FORCE_SET_REPEAT_MODE_AFTER_REAL_NEXTPREV
                    if ( __bRepeat == (REPEAT_CHAPTER) )
                    {
                        __bRepeat= REPEAT_NONE;
                        OSD_OUTPUT_MACRO(MSG_REPEAT, __bRepeat, 0 );
                    }   // Only clear REPEAT_CHAPTER
#endif  // FORCE_SET_REPEAT_MODE_AFTER_REAL_NEXTPREV
                }   // if ( __bDomain & TT_DOM )

                if(__bSearchMode != SEARCH_MODE_OFF)
                {
                    // Micky0.62-909
                    OSD_RemoveUI(OSD_UI_SEARCH);    // it will cal Exit(control_only)
                }
            }   // Key from ISR

            // ** TCH0.39; Check ACTION_NEXT/ PREV Value for INVALID_KEY
            // wyc.101 , when not playing program mode, enter this flow
            if (!__btPlayingProgram)
            {
#ifdef     SUPPORT_NEXT_OVERRULE_NVFLOW                 
                if ( __bKeySource == KEY_FROM_ISR )    // KEY_FROM_FW
                    __wTemp=1;
                else
                    __wTemp= NULL;
                if ( ! NV_Action ( __bTemp, __wTemp) ) // ** TCH2.30; 
                    return INVALID_KEY;
#else                    
                if ( ! NV_Action ( __bTemp, NULL_0 ) )
                    return INVALID_KEY;
#endif                  // SUPPORT_NEXT_OVERRULE_NVFLOW     
            }
            // wyc.101 , for DVD title go changing TT/CC in program playing mode
            else if (__btPlayingProgram)
            {
                LINEAR_ChangeTrack ( __bKey, NULL_0 );
            }

// DVD_099cMicky, support show OSD only when IR key is pressed
#ifdef  SHOW_IR_KEY_OSD_ONLY
            if (__bKey==KEY_NEXT)
                __bTemp=MSG_NEXT;
            else
                __bTemp=MSG_PREV;
            OSD_OUTPUT_MACRO(__bTemp, OSD_NEXT_PREV_CHAPTER, MSG_SHOWTIME_NORMAL ); // LLY.099c
#endif
            //Fix the bug about the video will flash
            //HAL_Display (HAL_DISPLAY_ONE_FIELD, 0); //CoCo.100
            break;

    case    KEY_ANGLE_CTL:
    	// Micky0.80.
   		return CC_Angle_Change(CC_TOGGLE_ANGLE);
    	break;

    case    KEY_TITLE:
        {   // Do TitleMenu
            if ( ! NV_MenuCall ( MENU_TITLE ) )
            {
                if ( __bModeCmd == KEY_STOP )
                {
                   	UTL_ShowLogo();
               	}
                return KEY_INVALID;
            }

            {
                OSD_OUTPUT_MACRO(MSG_MENU, OSD_MENU_TITLE, 0x3);
            }

            // DVD_160Micky, Program-> title, will clear Program
            if (__btProgram)
                LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
        }
        break;

    case    KEY_MENU:
        {   // Do RootMenu
            if ( ! NV_MenuCall ( MENU_ROOT ) )
            {
                if ( __bModeCmd == KEY_STOP )
               	{
                	UTL_ShowLogo();
               	}
                return KEY_INVALID;
            }

            {
                OSD_OUTPUT_MACRO(MSG_MENU, OSD_MENU_ROOT, 0x3);
            }

            // DVD_160Micky, Program-> title, will clear Program
            if (__btProgram)
                LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
        }
        break;
    case    KEY_FP_PGC:
        {   // Do FP_PGC
            if ( ! NV_TitlePlay ( NULL_0, NULL_0 ) )
            {
                return KEY_INVALID;
            }
            else
            {
            }
        }
        break;
//Aron1.11-909, add the feature: smart play by IR
#ifdef  SUPPORT_DVD_SMRTPB            
    case    KEY_SMARTPLAY:
        {
            if ( __bSmartPB_ABLE && __bTitle != __bMaxPBT_TT)
            {
                //Aron1.11-909, need to clear belowing modes when press smart 
                // play button

                // Clear Repeat Mode
                __bRepeat= REPEAT_NONE;            
                OSD_OUTPUT_MACRO(MSG_REPEAT, __bRepeat, 0 );

                // Clear Repeat AB Mode
                __bRepeatAB= REPEAT_NONE;
                OSD_OUTPUT_MACRO(MSG_REPEATAB, REPEAT_NONE, 0);    
                
                // Clear program related variables
                __btProgram= FALSE;
                __btInputProgram=FALSE;    
                __btPlayingProgram=FALSE;                
                OSD_Output(MSG_PROGRAM, 0, 0);
                __btNextPlayProgram=FALSE;
                
                // Cancel Random Mode
                __btRandom= FALSE;
                OSD_OUTPUT_MACRO(MSG_RANDOM, FALSE, 0);
               
                NV_TitlePlay(__bMaxPBT_TT, 1);
                SMAN_PushStateInfo ( STATE_PG, STATUS_READY );
            }
            else
            {
                return INVALID_KEY;
            }
        }
        break;
#endif  

    case    KEY_UP:
    case    KEY_DOWN:
    case    KEY_LEFT:
    case    KEY_RIGHT:
        {
#ifdef SUPPORT_PRINTF
        PRINTF ( " * DVD Button Move [%d];", bKey );
#endif  //#ifdef SUPPORT_PRINTF
        if ( ! NV_Action ( (BYTE)(ACTION_UP+ __bKey- KEY_UP), NULL_0 ) )
        // Micky0.80, bypass the key, it may be for zoom.        
        {
            return KEY_BYPASS;
        }
        return KEY_NO_KEY;
        }
        break;
        // ** TCH0.29; end...

// Micky2.79-2, support FullScreen function
// allowed only when DVD && Playing && Title Domain && no Highlight.
// FullScreen is exclusive with ZOOM function.
#ifdef  SUPPORT_FULLSCREEN 
    case    KEY_FULLSCREEN:
            if (!__btPlaying && !__btPause )
                return INVALID_KEY;
            if ( ! ( __bDomain & TT_DOM ) )
                return INVALID_KEY;
             // Read HighLight Status, if have HightLight, just disable ZOOM.

			HAL_ReadInfo(HAL_INFO_HLI_STATUS, &__dwTemp);
			if (__dwTemp)
                return INVALID_KEY;
            // ZOOM and Full screen are exclusive
            __bZoomFactor= VIEW_NORMAL;
            __bFullScreen = !__bFullScreen;
            OSD_Output(MSG_FULLSCREEN, (WORD)__bFullScreen, 3);
            // re-setting Video display
            // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
            HAL_Zoom (VIEW_NORMAL);
            // force to re-select subpicture,
            // ON: select WIDE, OFF: accord to SETUP apect ratio
            NV_Setting ( SET_SPST, 1 );
            break;
#endif  //#ifdef  SUPPORT_FULLSCREEN
    case    KEY_ZOOM:
            if ( ! ( __bDomain & TT_DOM ) )
                return INVALID_KEY;
            // Micky2.79-2, support FullScreen function
            // FullScreen is exclusive with ZOOM function.
            __bFullScreen = 0;
            return KEY_TODO;  // Micky0.80, return new status
//            return bKey;    // ZOOM is common for all
            break;
// let main flow process it.            
/*            
    case    KEY_SPST_CTL:
    		// Micky0.68, merge SP change into an API for navigator and key switch share use.
    		// parameter 1 means by KEY_SPST_CTL
    		CC_SPST_Change(1);
    		break;
    case    KEY_AST_CTL:
            // Micky2.37,  still put within GroupAudio, as too complicated.
            return bKey;
*/    		
// Micky0.62-909, process number key
    case    KEY_NUMBER:
        {
            BYTE bActiveButton=0;
            //if check if allow number input, play tt/chapter by number
            // has button
            // don't has button, but support play TT/CC by chapter
            bActiveButton = 0;
            // 1. Check if allow number input.
            if ( !NV_Action(ACTION_NUMBER_CHECK, NULL_0) )
            {
#ifdef  SUPPORT_PLAY_CHAPTER_BY_NUMBER
                if ( ( __bDomain & TT_DOM ) )
                {
                    // support only for title domain
                    bActiveButton=2;
                }
                else
#endif  //#ifdef  SUPPORT_PLAY_CHAPTER_BY_NUMBER
                {
                    CC_NumberUtility(NUMBER_CLEAR_TIMEOUT);
                    __bKey = INVALID_KEY;
                }
            }
            else
            {
                bActiveButton=1;
            }
            
            if (CC_NumberUtility(NUMBER_SET_INPUT)) // number is not decided
                return KEY_NO_KEY;

            if (bActiveButton == 1)
            {
                if ( !NV_Action ( ACTION_NUMBER, (WORD)__dwNumberPrevious) )
                {
                    __bKey = INVALID_KEY;
                }
            }
#ifdef  SUPPORT_PLAY_CHAPTER_BY_NUMBER
            else if (bActiveButton==2)
            {
                // input number is out of chapter range.
                if ( __dwNumberPrevious > __wChapter_Ns_Title )
                {
                    __bKey = INVALID_KEY;
                }
                else
                {
                    //Kevin1.24a, fix "program mode is not cleared when inputting number to select chapter"
                    if (__btPlayingProgram)
                        LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);

                    NV_TitlePlay(__bTitle, (WORD)__dwNumberPrevious);
                    //Aron1.20-909, Fix bug reported by Mustek when enableing definition " SUPPORT_PLAY_CHAPTER_BY_NUMBER"
                    //Play 遊走8 女王頭 按數字鍵跳章節，再按數字鍵跳章節會停留在同一章節 
                    SMAN_PushStateInfo ( STATE_PG, STATUS_READY );
                    HAL_Reset(HAL_RESET_AUDIO);
                    HAL_Reset(HAL_RESET_SPST);

                    HAL_Reset(HAL_RESET_VIDEO);
                    // DVD_103Micky, fix GoToTime will hang issue
                    // need restore the play command
                    UTL_TransferCommandID(__bModeCmd);

                    //HAL_Display (HAL_DISPLAY_ONE_FIELD, 0);
                }
            }
#endif  //#ifdef  SUPPORT_PLAY_CHAPTER_BY_NUMBER
			// Micky0.72, number action is done, let the number OSD last for 1s.
			if (__bKey == KEY_NUMBER)
			{
		        OSD_OUTPUT_MACRO(MSG_NUMBER, __dwNumberPrevious, 1);				
			}
			else
			{
	        	OSD_OUTPUT_MACRO(MSG_NUMBER, 0, 0);
	        }
            CC_NumberUtility(NUMBER_CLEAR);

            if (__bKey == INVALID_KEY)
            {
                return INVALID_KEY;
            }
            return KEY_NO_KEY;
        }
        break;
    default:
        return KEY_BYPASS;  // Micky0.80, return new status
//        return bKey;    // common keys, let main flow process
}   // switch ( bKey )
    // For those keys are processed, just return KEY_NO_KEY.
    return KEY_NO_KEY;
}

#endif //#ifndef NO_DISC_MODE //CoCo2.37p

// DVD_161Micky, add general keys for releasing CELL still/ VOBU still
//  *********************************************************************
//  Function    :   _CC_ReleaseStillTrigger()
//  Description :   It will first check if the input key allow to release
//                  the VOBU or CELL still state, then release it.
//                  The available keys are defined in _bReleaseStillKeys.
//  Arguments   :   __bKey, the input key value
//  Return      :   KEY_ACCEPT, the still state is released and key is accept.
//              :   KEY_DISALLOW, the key is reject for still
//  Side Effect :
//  *********************************************************************
#ifndef NO_DISC_MODE //CoCo2.37p
BYTE    _CC_ReleaseStillTrigger(void)
{
    __btTemp = FALSE;
// 1. Check if the key allow to release still
    for (__bTemp=0;__bTemp<RELEASE_STILL_KEY_NUMBER;__bTemp++)
    {
        if ( __bKey == _bReleaseStillKeys[__bTemp] )
        {
            __btTemp = TRUE;
            break;
        }
    }
    if (!__btTemp)
        return KEY_DISALLOW;

// 2. Check if in cell still/ vobu state

    // cell still, "nf200" and "Life is beautiful", menu has buttons
    // uop check can still off, so must add button control check
    // for cell still but with button selections, can't release cell still.
    if ( ( CELL_QueryStatus(CELL_IS_STILL) && !NV_Action(ACTION_ACTIVE_CHECK, NULL_0))
         || CELL_QueryStatus (CELL_VOBU_STILL_ON) )
    {
        if ( NV_QueryUOP (UOP_CAN_STILL_OFF) ) // user can release still
        {
            // Micky0.95, suppose no need it for 909, as servo won't auto stop.

            // release vobu still
            CELL_QueryStatus (CELL_VOBU_STILL_OFF);
            // release cell still
            __bStillTime = 0;
            return KEY_ACCEPT;
        }
    }
    return KEY_DISALLOW;
}
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

// Micky0.68, merge SP change into an API for navigator and key switch share use.
// parameter 1 means by KEY_SPST_CTL, 0 means by friendly navigator(and assign correct __bSPSTNO)
BYTE	CC_SPST_Change(BYTE bSwitch)
{
    // LLY1.11, parser support CVD OGT from v1.10
#if 0    
    if (__wDiscType & BOOK_CVD)
        return INVALID_KEY;
#endif  // #if 0 

	// don't allow spst switch when vidoe freeze
    if ( __bModeCmd == KEY_FREEZE )
    	return INVALID_KEY;
    //Aron1.11-909, added to prevent enable displaying SPST when in scan mode
    if ( __bModeCmd == KEY_SCB )
        return INVALID_KEY;

    if ( __bModeCmd == KEY_SCF )
        return INVALID_KEY;
                        
    if ( !__bSPSTNs  )
        return INVALID_KEY;

    if ( __bZoomFactor != VIEW_NORMAL )
        return INVALID_KEY;

#ifndef NO_DISC_MODE //CoCo2.37p
    if (__wDiscType & BOOK_DVD)
    {
        if ( ! NV_QueryUOP ( UOP_CAN_CHANGE_SUBPICTURE_STREAM ) )
            return  INVALID_KEY;
    }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

    // Micky0.90-2, don't allow SP2 change when SP1 is off
    if ( (bSwitch == CC_SWITCH_QUERY_SP2) || (bSwitch & CC_SWITCH_SP2) )
    {
        if ( __bSP_Disp_OFF & 0x01) // SP1 off
            return INVALID_KEY;
    }
// Micky0.80
    if (bSwitch == CC_SWITCH_QUERY)
        return KEY_NO_KEY;

    // toggle SP1		
	if (bSwitch == CC_TOGGLE_SP1)        
	{
    	    __bSPSTNO++;
	}
    // wyc1.00-909, re-allocate the layout.
    // Micky0.90a, user toggle SP2
	if (bSwitch == CC_TOGGLE_SP2)        
	{
        __bSPST2NO++;
        /*
        if (__bSPST2NO >= __bSPSTNs )
        {
            __bSPST2NO = 0;
        }  
        */
    }

    if (__wDiscType & BOOK_DVD)
    {
#ifndef NO_DISC_MODE //CoCo2.37p
        // change SP1
        if ( bSwitch & CC_SWITCH_SP1 )
        {
#ifdef CCKEY_SP_DEBUG
            printf("\nDbg1: %hx, %hx",bSwitch,__bSP_Disp_OFF);
#endif //
            if ( (__bSP_Disp_OFF & 0xF0) )
            // Check the case: SP Disable by NV-CMD and do not choose SP stream
            {
                // user toggle SP1 from NV off
                if (bSwitch & CC_TOGGLE)        
                {
                    // just don't increase the SP index
                    __bSPSTNO--;    //Play current SPSTNO.
                }
                __bSP_Disp_OFF &= 0x0F; // clear the SP off by NV cmd status
            }
#ifdef CCKEY_SP_DEBUG
            printf("\nDbg2: %hx, %hx",__bSPSTNO,__bSPSTNs);
#endif //

            if ( __bSPSTNO >= __bSPSTNs )   // 0~__bSPSTNs-1 is valid SP index
            {
                if ( (__bDomain & TT_DOM) )     // only valid when TT_DOM
                {
                    // wyc1.02-909, update the flow to make SP2 enable go right path. Otherwise SP2 will be enable after switch SP1 from off to on.
                    // Also fix SP2 won't show even user enable SP2 and switch SP1 from off to first SP. SP2 will show when switch to 2nd SP1.
                    // wyc1.01-909, add comment, __wSPRMs [2]: bit 0-5: SP ID. bit6: SP enable or disable.
#ifdef CCKEY_SP_DEBUG
                    printf("\nDbg3: SPRM[2]: %x",__wSPRMs [2]);
#endif
                    __wTemp= __wSPRMs [2]; 
                    __wSPRMs[2] |= 0x0040;  // ** TCH1.61-1; // Enable SP_disp_flag
                    // Micky0.76, let ID keep as max.
                    // as when calling from SP2 change, it may enable the last SP.
                    // check SP1 display status, 1 means enable
                    // wSPRMs[2] &0x40, SP is disabled by NV cmd or user switch SP1 off form navigator menu
                    // Micky0.90a, 0x40 means SP display flag is on.
                    if ( ( __wTemp & 0x40 )  ||  !( bSwitch & CC_TOGGLE ) ) // means SP_disp_flag = 1;
                    {
                        // SP1 off
                        __bSP_Disp_OFF |= 0x01;     // mean sp_disp_flag is 0b, SP_OFF by user
                        __wSPRMs [2]= 63;    // Dummy stream.
                        HAL_Display ( HAL_DISPLAY_SPST2, FALSE );
                        // Micky0.90a, just turn off SP2 display flag
                        // wyc1.06-909, only set __bSP2_Disp_OFF |= 0x10 when SP is enabled mode, otherwise SP2 will be turn on when switch 
                        // SP1 to enable mode even SP2 is not enabled originally.
                        if (!(__bSP2_Disp_OFF & 0x1))
                            __bSP2_Disp_OFF |= 0x10;
                    }
                    else
                    {
                        // SP1 off-> On 1st SP
                        __bSPSTNO= 0;
                        {
                            __wSPRMs[2] |= 0x0040;  // ** TCH1.61-1; // Enable SP_disp_flag                
                            // When SP1 exist, and SP2 is disabled by SP1 off, turn SP2 on again
                            // wyc1.02-909, only open SP2 when SP is forced disbale by SP and user have enable SP2.
#ifdef CCKEY_SP_DEBUG
                            printf("\nDbg4: %hx",__bSP2_Disp_OFF);
#endif //
                            if (( __bSP2_Disp_OFF & 0x10 ) && (!(__bSP2_Disp_OFF & 0x1)))
                            {
                                __bSP2_Disp_OFF = 0;                    
                                HAL_Display ( HAL_DISPLAY_SPST2, TRUE );                        
                            }
                        }
                    }
                    //Aron1.03-909, store user SPST setting for current title.
                    NV_Usersetting.__bUS_Vaild = TRUE;
                    NV_Usersetting.__bUS_SPSTNO = __bSPSTNO;
                    NV_Usersetting.__bUS_TTNO = __bTitle;                      
                }
                else
                {
                    __bSPSTNO= 0;
                    __bSP_Disp_OFF &= 0xFE;
                    HAL_Display ( HAL_DISPLAY_SPST2, FALSE );
                    // Micky0.90a, just turn off SP2 display flag
                    __bSP2_Disp_OFF |= 0x10;
                }
            }                    
            // wyc1.02a-909, when use NV to enable SP1, need to enable SP1/SP2 control here. Otherwise SP won't appear even user want to 
            // enable it in NV.
            else
            {
#ifdef CCKEY_SP_DEBUG
                printf("\nDbg5: %hx, %hx, %hx, %hx, %hx",__bSPSTNO,__bSPST2NO, __bSP_Disp_OFF,__bSP2_Disp_OFF,bSwitch);
#endif //
                if (!(bSwitch & CC_TOGGLE))
                {
                    __bSP_Disp_OFF = 0;
                    __wSPRMs[2] |= 0x0040;  // ** TCH1.61-1; // Enable SP_disp_flag                
                    // When SP1 exist, and SP2 is disabled by SP1 off, turn SP2 on again
                    if ( __bSP2_Disp_OFF & 0x10 )
                    {
                        __bSP2_Disp_OFF = 0;                    
                        HAL_Display ( HAL_DISPLAY_SPST2, TRUE );                        
                    }
                }
            }
            // Micky0.90a
        }

        // Micky0.80, add switch SP2 and toggle SP2.
        if ( ( bSwitch & CC_SWITCH_SP2 ) )
        {
            if ( bSwitch & CC_TOGGLE )
            {
                if (__bSP2_Disp_OFF)    // SP2 off-> on 1st SP
                {
                    __bSPST2NO = 0;
                }
            }
            // Chuan0.75, Need check more..
            if ( __bSPST2NO >= __bSPSTNs )  // turn SP2 off
            {
                // User turn SP2 off
                __bSP2_Disp_OFF =0x01; 

                // Micky0.90-2, must check if same SP1 & SP2
                // must keep parser and display when same ID
                if ( __bSPST2ID != __bSPSTID )
                {
                    HAL_Display ( HAL_DISPLAY_SPST2, FALSE );
                }
                // Micky0.90-2, clear must check if same SP1 & SP2
                //__bSPST2ID = 0xFF;	// means no SP2 selected      
            }
            else
            {
                // Micky0.90a
                // only enable SP2 when SP1 is on
                // Aron1.01, NV_Setting will get correct __bSPST2ID
                //__bSPST2ID= __bSPST2NO;
                // Micky0.76, enable SP2 parsing.
                //HAL_ControlParser ( HAL_PARSER_SP2_ID, __bSPST2ID+0x20 );
                if ( __bSP2_Disp_OFF )
                {
                    __bSP2_Disp_OFF =0x00; 
                    HAL_Display ( HAL_DISPLAY_SPST2, TRUE );
                }
            }               
        }

        // NV_Setting will set SP1 ID in __bSPSTID, SP2 ID in __bSPST2ID 
        if ( ! NV_Setting ( SET_SPST, 1 ) )   // ** TCH0.38;
            return  INVALID_KEY;
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

    }
    // LLY1.20, remove SPST switching for char base subpicture procedure
    // Since, it will go the same flow of non-DVD disc
#if 0//def SUPPORT_CHAR_SUBPICTURE   //Alan1.10, for char-based subpicture
    else if (__bAttrPlay == ATTR_AVI && PARSER_SPTXT_SUCCESS == __dwParResult_CharSP) 
    {
        if (__bSPSTNO == __bSPSTNs)
        {
            __bSP_Disp_OFF = 1;
            HAL_Display ( HAL_DISPLAY_SPST1, FALSE );
        }
        else if (__bSPSTNO > __bSPSTNs)
        {
            __bSPSTNO = 0; 
            __bSP_Disp_OFF = 0;
            HAL_Display ( HAL_DISPLAY_SPST1, TRUE);
        }
    }
#endif
    else    // non-DVD disc
    {
        // LLY1.20, modify the SPST on/ off procedure
        // Since, old flow will cause SPST can't turn-on while only one SPST and off --> on
        if(__bSP_Disp_OFF)  // turn on SPST
        {
            __bSP_Disp_OFF=0;
            __bSPSTNO=0;
        }
        else if(__bSPSTNO >= __bSPSTNs) // turnoff SPST 
        {
            __bSPSTNO=0;
            HAL_Display(HAL_DISPLAY_SPST1, FALSE);
            __bSP_Disp_OFF=1;
        }
#if 0       
        if ( __bSPSTNO >= __bSPSTNs )
        {
            // Micky0.90a, support SP off for non-DVD disc
            __bSPSTNO= NULL_0;
            HAL_Display ( HAL_DISPLAY_SPST1, FALSE );
            __bSP_Disp_OFF = 1;
        }
        else
        {
            if (__bSP_Disp_OFF )
            {
                __bSP_Disp_OFF= 0;
                __bSPSTNO = 0;
            }
        }
#endif  // #if 0        
        __bSPSTID= __bSPSTNO;
    }
    if (bSwitch & CC_TOGGLE)        
    {    
        OSD_Output ( MSG_SPST_CTL, __bSPSTNO, 0x3 );    // ** TCH0.451;
    }
    if ( !(__wDiscType & BOOK_DVD) && (!__bSP_Disp_OFF))
    {
        // wyc1.20-909, need to call it to avoid SP match ID with SP2. Then it will show SP2 a while then disappearred.
        HAL_ControlParser(HAL_PARSER_SP2_ID, SP_NULL_ID);
        // wyc1.07-909, call HAL_ControlParser to replace PARSER_ChangeID.
        HAL_ControlParser(HAL_PARSER_SP_ID, __bSPSTID);
        //PARSER_ChangeID( 0, PARSER_ES_SP1, __bSPSTID );	
        HAL_Display ( HAL_DISPLAY_SPST1, TRUE );
        
    }
    return KEY_NO_KEY;
}

#ifndef NO_DISC_MODE //CoCo2.37p 
BYTE	CC_Angle_Change(BYTE bSwitch)
{

    // Micky1.10-2, video freeze or pause or step, don't allow angle change
    if ( ! __bAGL_Ns || __btPause || (__bModeCmd== KEY_STEP) || (__bModeCmd == KEY_FREEZE))
        return INVALID_KEY;

    if ( ! NV_QueryUOP ( UOP_CAN_CHANGE_ANGLE ) )
        return  INVALID_KEY;

	// Micky0.80
	if (bSwitch == CC_SWITCH_QUERY)
		return KEY_NO_KEY;
		
	if (bSwitch & CC_TOGGLE)
	{
		__bAGLN++;
	}
    // ** TCH0.29; Different from AST and SPST as AGLN comes from "1"
    if ( __bAGLN > __bAGL_Ns )
        __bAGLN= 1;

    NV_Setting ( SET_AGL, 1 );      // ** TCH0.47;
	return KEY_NO_KEY;
}
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

// Micky0.80, add audio control into an API
BYTE    CC_Audio_Change(BYTE bSwitch)
{
    if ( ! __bASTNs )
        return INVALID_KEY;

#ifndef NO_DISC_MODE //CoCo2.37p
    if ( __wDiscType & BOOK_DVD )
    {
        if ( ! NV_QueryUOP ( UOP_CAN_CHANGE_AUDIO_STREAM ) )
            return  INVALID_KEY;
    }                
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

    if (bSwitch == CC_SWITCH_QUERY)
	    return KEY_NO_KEY;

	if ( bSwitch == CC_TOGGLE_AUDIO )
	{
        if ( __bASTNs == 1 )
        {
            OSD_Output ( MSG_AST_CTL, __bASTNO, 0x3 );      // ** TCH0.451;
            return KEY_NO_KEY;
        }

        __bASTNO++;
        __dwTemp = PARSER_QueryContainerAudioType(__dwSFStreamID, __bASTNO);
        if ((__bAttrPlay == ATTR_AVI))// && ((__dwTemp == PARSER_ESFMT_WMA_V1) || (__dwTemp == PARSER_ESFMT_WMA_V2)))
            HAL_ControlParser(HAL_PARSER_AUDIO_ID, HAL_THROW_AUDIO);
        if ( __bASTNO >= __bASTNs )
            __bASTNO= NULL_0;
    }            

    //Kevin0.87, avoid the noise when 2CH->5.1CH
    CHIPS_OpenAudio(FALSE);

#ifndef NO_DISC_MODE //CoCo2.37p
    if ( __wDiscType & BOOK_DVD )
    {
        // Micky1.25, fix multi-audio switcing (Panassonic 11/1) vidoe unsmooth
       CELL_Trigger(NULL_0);
       CELL_Trigger(NULL_0);
       CELL_Trigger(NULL_0);
       CELL_Trigger(NULL_0);
       CELL_Trigger(NULL_0);

        if ( ! NV_Setting ( SET_AST, 1 ) )  // ** TCH0.371; // ** TCH0.38;
            return  INVALID_KEY;
    }
    else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
    {
        // LLY2.21a, get corresponding audio stream type, ID and do setting for CDROM/ DVDROM motion file
        //if(__bAttrPlay == ATTR_AVI))
        if(__bAttrPlay & TYPE_CDROM_AV)
        {
            // LLY1.20, rename it as UTL_MovieASTSetting()
            // Since it's same between AVI/ DivX/ MP4/ NeroDigital
            UTL_MovieASTSetting(__bASTNO);
        }
        else // Others, just assign the index to audio ID
        {
            __bASTID= __bASTNO; // ** TCH0.451;
        }
    }

    OSD_Output ( MSG_AST_CTL, __bASTNO, 0x3 );      // ** TCH0.451;
    if ( (__bModeCmd >= KEY_THROWAUDIO_BEGIN) && (__bModeCmd <= KEY_THROWAUDIO_END) )        // ** TCH1.00a;
    {
            // ** TCH0.60; 
        HAL_ControlParser(HAL_PARSER_AUDIO_ID, HAL_THROW_AUDIO);    // ** TCH1.00a;
    }
    else
    {
        // Micky1.25, fix multi-audio switcing (Panassonic 11/1) vidoe unsmooth
        // NV_Setting will do the action.
        if (!(__wDiscType & BOOK_DVD))
        {
            // ** TCH0.60; 909 & 908 are same 
            HAL_ControlParser ( HAL_PARSER_AUDIO_ID, __bASTID );
            HAL_ControlParser ( HAL_PARSER_ENABLE, NULL_0 );
        }

        // DVD_103Micky, return a/v sync to video master.
        // DVD_155Micky, use hal new define API
        HAL_ReadInfo(HAL_INFO_STILL, &__dwTemp);
        if (__dwTemp == TRUE)
        {
            HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, 0 );                      
        }
        else
        {
            HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, 1 );                      
            // Chuan0.85, Inform Proc2 to wait STC if Audio PTS lead STC.
            HAL_WriteAM(HAL_AM_PROC2_WAIT_STC, 1);        
        }
    }
        
    //kevin0.68, restore the original play command (reset audio will stop audio)
    if (__btPause) //KEY_PAUSE: __bModeCmd remains unchanged
        HAL_WriteAM(HAL_AM_PLAY_COMMAND, 6); //pause
    else if ( __bModeCmd==KEY_STEP || __bModeCmd==KEY_FF )    
        HAL_WriteAM(HAL_AM_PLAY_COMMAND, 5); //step       
    else if ( __bModeCmd==KEY_SF )
        // wyc2.17-909S,
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_MODE)
        HAL_WriteAM(HAL_AM_PLAY_COMMAND, 5); //step          
#else
        //kevin1.10, fix slow->AST => no sound
        //HAL_WriteAM(HAL_AM_PLAY_COMMAND, 5); //step
        UTL_TransferCommandID(KEY_SF);  //kevin1.10, fix slow->AST=> freq is faster (NV_Setting will call HAL_SetAudioDAC again) 
#endif //
    else                  
        HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1); //play                 
        
    //Kevin0.87, avoid the noise when 2CH->5.1CH
    CHIPS_OpenAudio(TRUE);
    return KEY_NO_KEY;
}
