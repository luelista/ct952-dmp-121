//////////////////////////////////////////////////////////////
// ***** Big Modification History Area *****
// LLY.274p, some big modification:
// [1] Rename all MP3 relative info. because module name has changed !!
//  - MP3_ProcessHighLight()  -->  FM_ProcessHighLight()  --> MM_ProcessHighLight
//  - __wTotalAlbums  --> __wFMTotalDirs;
// Micky2.80a
//  - remove MULTI_DISC_SYSTEM codes
// LLY2.80a
//  - remove ADD_BACKGND_FOR_CDDA codes
/////////////////////////////////////////////////////////////


#include "winav.h"
#include "digest.h"

//ccc
#include "cc.h"
#include "utl.h"
#include "input.h"
#include "hal.h"
#include "osd.h"
#include "infofilter.h"
#include "linear.h"
#include "chips.h"
#include "disp.h"
#include "dvdinfo.h" // wyc.pro
#include "navigate.h" // wyc.pro
#include "cell.h" // ** TCH1.61a; for REPEAT_AB
//#include "message.h"
//DVD_031, MP3
#include "Mmanager.h" //"mp3.h"
#include "SrcFilter.h"
#include "thumb.h"  // LLY2.80
#include "char_subpict.h"

#include "parser.h"
//Kevin0.86, program index list array for font mode / GDI mode
PROG_INDEX      __ProgIdxList[MAX_PROG_ITEM];

//Kevin1.00, move from cc.c to here
BIT             __btProgram;//Micky2.80, review global variables init.= FALSE;             // Program Mode ON/OFF (Only valid in PBC OFF)
BIT             __btInputProgram;//Micky2.80, review global variables init.= FALSE;        // PROGRAM in INPUT mode
BIT             __btPlayingProgram;//Micky2.80, review global variables init.= FALSE;      // Now is playing PROGRAM
BIT             __btNextPlayProgram;//Micky2.80, review global variables init.= FALSE;     // Ready for play PROPRAM;

//Kevin0.86, move frm cc.c to here
BYTE            __bProgramMaxEntry;//Micky2.80, review global variables init. = 0;
// wyc2.80, move the variables' decalre from CC to LINEAR.
BYTE     __bProgramCurrentIndex;
BYTE     __bProgramPlayingIndex;
extern DWORD    __dwGoToTime;                   // Time that use GOTO position
extern DWORD    __dwResumeTime;             // Time that record final stop point
//extern BIT      __btInputProgram;           // PROGRAM in INPUT mode
//extern BIT      __btPlayingProgram;         // Now is playing PROGRAM
//extern BIT      __btNextPlayProgram;        // Ready for play PROPRAM;
//Kevin0.86, comment
//extern WORD     __wProgramData [PROGRAM_TRACK];  // update max. program entry to PROGRAM_TRACK
//extern BYTE     __bProgramMaxEntry;
extern BYTE     __bNVPlayMode;

extern BYTE     __bSameItem;
extern WORD     __wPrevPlayDir;
extern WORD     __wPrevPlayItem;

// end ..
// wyc.172-CDDA, extern the flag to represent CDDA is play across tracks automatically.
extern BYTE     __bCDDAAutoChTrk;
// DVD_276Micky, bit reduce
///extern BIT      __btTrueKey;
// wyc1.07a. remove this variale.
//extern BYTE     __bKeyRepeat;        // in isr.c
extern DWORD IDATA     __dwTimeKey;          // time reference; First Number Key
extern BIT      __btPlayFromBegin;        // Told F/W it should play Track-Begin first
extern BIT      __btFromStopMode;         // GOTO position comes from a "STOP" mode
extern BIT      __btNewTime;              // A flag that means it's a NEW time
extern  BYTE    __bTimeMin, __bTimeSec;         // GOTOTime Usage
extern  BYTE    __bGoToTimeNO;                  // count the I/P Number in GoToTime
extern  BYTE    __bModePlay;
// LLY.036
extern  BYTE    __bTimeHour;    // GoTo Time usage

DWORD           __dwPreGetTime= 0;           // Previous get time, time reference
extern DWORD        __dwTimeA, __dwTimeB;           // record the times for repeatAB
//DVD_051Micky, Number >10
extern WORD    __wNumberQueue;
extern DWORD __dwNumberPrevious;        // record previous valid number key

#ifdef SUPPORT_MP4
#ifndef NO_DISC_MODE //CoCo2.37p
extern WORD __wChapter;
extern WORD __wChapter_Ns_Title;
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
#endif //SUPPORT_MP4


// wyc1.06-909, extern variable
//**********************************************************************
//  Function    : LINEAR_ChangeTrack
//  Description : Play the next/prev track or the specified track number
//              : the prev/next track is decided according to the REPEAT mode
//              : (Exception PROGRAM->input entries->PLAY)
//  Parameters  : bKey: KEY_NEXT/KEY_PREV
//              :       bTrackNO: 1 Means auto play to next track from LINEAR_Trigger, 0 means press key to next track.
//              :       KEY_NUMBER, must reference bTrackNO
//              :       bTrackNO: specified track number to play(logical track)
//              :                 0, means current __wPlayItem(current track)
//  Return      : TRUE, stay in playing mode
//              : FALSE, stop playing
//  Changed     : __wPlayItem, __bModePlay
//**********************************************************************
BYTE LINEAR_ChangeTrack(BYTE bKey, WORD wTrackNO)
{
    BYTE        bCmd;//unused, bTemp;

    // __btNextPlayProgram=TRUE, is set when PROGRAM->Entries->PLAY
    // When KEY_NEXT is pressed or current track play end
    // it must directly play the first program entry

// test, when repeat none
// play last track, then select track 1 or last
// PROGRAM->ANY TRACK, will cancel program mode????

    // 1. when KEY_NEXT or KEY_PREV is pressed
    // must check the repeat mode!!!
    // 2. for key number, it will directly play the specified track
    // don't care the current repeat mode

    if ( bKey == KEY_NUMBER)
    {
        if ( !wTrackNO) // KEY_N0, play the first track
            wTrackNO = 1;

        // Key number (KEY_Nx) will clear the random mode
// Micky2.79, tune the usage of __bCountStop, don't reference DOUBLE_STOP
// KEY_NUMBER only clear Random mode when playing and real stop.
// __btRandom is on when playing and pre-stop.
        if (__btRandom && (__bCountStop ==0) )
        {
            __btRandom= FALSE;
            OSD_OUTPUT_MACRO(MSG_RANDOM, __btRandom, 3);
        }
    }
    else
    {
        // ** 0.16; Random mode don't care of REPEATMODE
        // wyc0.91, if don't check repeat mode when programming, then repeat one will failed in program playing mode. So recover
        // to original code to fix it.
        if ( ! __btRandom )// && (!__btPlayingProgram))
        {
            // check current repeat mode
            bKey = _CheckRepeatMode( bKey );

            // ** TCH2.81; begin...
            if  ( bKey == KEY_REPEAT )
            {
                bKey= KEY_NUMBER;       // let's go original path
                wTrackNO= __wPlayItem;
            }
            else
            // ** TCH2.81; end...
            if ( bKey == KEY_NUMBER )   // REPEAT_ONE, repeat the current track
            {
            #ifdef  SUPPORT_ENCODE_JPG_PICTURE //winnie2.51, added to play jpg from SPI flash
               //means play current jpg from SPI flash
                if(__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI)
                __bOSDSSPicIdx=0;
                else
             #endif
                {
                // means play current __wPlayItem
                wTrackNO=0; // CD-DA track number same as play item number
                }

            }
            else
            if ( bKey == KEY_STOP)      // REPEAT_NONE, will enter STOP mode
                return FALSE;
        }
    }

    switch ( bKey )
    {
        case    KEY_NEXT:           // play the next track
        case    KEY_PREV:           // play the prev. track
            // wyc2.30-909s, when no program entries, don't fo program playing flow. Otherwise will happen problem when program one entry and
            // play, disable this program entry and prev, then no program entry now but still go program finding entry flow and will wrong.
            if ((__btPlayingProgram) && (__bProgramMaxEntry!=0))
            {
                if (__btNextPlayProgram)
                {
                    __btNextPlayProgram=FALSE;
                }
                else
                {
                    // wyc.pro __wProgramData[1] -> __bProgramCurrentIndex
                    // merge the key prev and next operation
                    if (bKey == KEY_NEXT)
                        __bProgramPlayingIndex++;
                    else if (bKey == KEY_PREV)
                    {
                        __bProgramPlayingIndex--;
                        //Alan2.37, for Mantis #2341
                        while (0 == __ProgIdxList[__bProgramPlayingIndex].wSubIndex)
                            __bProgramPlayingIndex--;
                    }
                }
                // wyc.pro __wProgramData[0] -> __bProgramMaxEntry , __wProgramData[1] -> __bProgramCurrentIndex
                // and set when __bProgramCurrentIndex >= PROGRAM_TRACK, goto stop mode
                //LJY0.60p, "__bProgramPlayingIndex < 0" always false.
                if (( __bProgramPlayingIndex >= __bProgramMaxEntry ))// || (__bProgramPlayingIndex < 0))
                {
                    // wyc.101a, to fix when program playing mode, repeae disc is not available problem
                    // wyc1.10, make KEY_NEXT / KEY_PREV repeat playing in program playing mode. Auto play will stop when program playing to last entry.
                    // Also add control to make repeat mode will continue playing and make .
                    // Also make playing program always repeat.
//                    if ( (!__btPlayingProgram) && (__bKeySource != KEY_FROM_ISR) && (__bRepeat != REPEAT_DISC) )
                    if ( (__bKeySource != KEY_FROM_ISR) && (__bRepeat == REPEAT_NONE) )
                    {
                        // wyc1.06-909, when playing to last dir/file in MM mode, set __bLastPlayItem to let MM can reference it to not
                        // enter thumbnail mode when last play item is JPG file.
                        // wyc1.20-909, rename
                        __bLastPlayItem = TRUE;
                        __bFWKey = KEY_STOP;
                        return TRUE;
                    }
                    else
                    {
                        if (bKey == KEY_NEXT)
                            __bProgramPlayingIndex = 0;
                        else if (bKey == KEY_PREV)
                            __bProgramPlayingIndex = __bProgramMaxEntry - 1;
                    }
                }

                // wyc.pro  __wProgramData[1] -> __bProgramCurrentIndex
                // wyc.pro1 check if array is null, if null go to next program entry, and modify the function name
                if (bKey == KEY_NEXT)
                    __bTemp = _LinearEntryOperation(LINEAR_FIND_ENTRY,1);
                else if (bKey == KEY_PREV)
                    __bTemp = _LinearEntryOperation(LINEAR_FIND_ENTRY,0);

                // confirm the porgram has data
                if (__bTemp == 1)
                {
#ifndef NO_DISC_MODE //CoCo2.37p
                    if (__wDiscType & BOOK_DVD)
                    {
//Kevin0.86, __bTitle/__wChapter for font mode / GDI mode program UI
                        __bTitle = __ProgIdxList[__bProgramPlayingIndex].bMainIndex;
                        __wChapter = __ProgIdxList[__bProgramPlayingIndex].wSubIndex;

                        NV_TitlePlay(__bTitle,__wChapter);

                        //Ming0.72, Must restore the command before next play in PROGRAM mode.
                        // TODO: Must review all the codes which call audio reset.
                        UTL_TransferCommandID(__bModeCmd);

                        // wyc.101 add the return code
                        return TRUE;
                    }
                    else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
                    {
//Kevin0.86, __wPlayItem for font mode / GDI mode program UI
                        __MMPlayingItem.wDirID = __ProgIdxList[__bProgramPlayingIndex].bMainIndex;   //for FM use only
                        __wPlayItem= __ProgIdxList[__bProgramPlayingIndex].wSubIndex;
                    }
                }
                else
                {
                    // wyc1.10-3, when select some entries to program playing and then remove them, then system will enter stop mode.
                    // Now make it go normal playing and clear program mode.
                    //__bISRKey = KEY_STOP;
                    LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
                    __wPlayItem++;
                }

                // for VCD, the __wPlayItem=TrackNO+1;
                if  ( __wDiscType & BOOK_VCD )
                    __wPlayItem++;
            }
            else
            {
                // directly play the next play track
                // __wPlayItem value is according to disc type
                // wyc.pro merge key prev and next
                if (bKey == KEY_NEXT)
                {
                    // LLY1.50, add next chapter control for Nero Digital
#ifdef  SUPPORT_MP4
                    if( MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem) == EXTNAME_MP4 )
                    {
                        // if exit next chapter, change to it
                        if(__wChapter < __wChapter_Ns_Title)
                        {
                            // [1] Calculate the desired time for next changer beginning
                            __dwGoToTime = PARSER_GetMP4ChapterTime(PARSER_MP4_CHAP_NEXT);
                            // LLY2.36, keep corresping STC value here to avoid STC/90K= sec, then *90K=STC again
                            __dwGoTimeSTC = __dwGoToTime*90000;

                            // [2] Let it go the same flow of GoTime Action
                            __btNewTime=TRUE;

                            // [3] Transfer the command as KEY_PLAY
                            __bModeCmd = KEY_PLAY;

                            return TRUE;
                        }
                    }
#endif  // #ifdef SUPPORT_MP4
                    // wyc1.10, because set this __dwTimeKey to 0 will make F/W to clear the input number, so now not to
                    // clear it to allow user can input number by 10+ without F/W clear
                    //__dwTimeKey= 0;

                    // LLY0.83b-2, Let MM_Action(KEY_NEXT) handle NEXT action
                    // Because, __wPlayItem meaning maybe different between CT9928AF & CT908
                    // CT9928AF: __wPlayItem index from 1 ~ Total files within disc
                    // CT908: __wPlayItem index from 1 ~ Total files within current directory
                    // if nothing is done within this API, then go original flow
                    // wyc0.60-909, rename it.
                    if(MM_ActionForChangeTrack(KEY_NEXT, __wPlayItem)==MM_ACTION_CONTINUE)
                    {//winnie2.51, added to show JPG from SPI flash and key press NEXT
                    #ifdef  SUPPORT_ENCODE_JPG_PICTURE
                       if(__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI)
                         LINEAR_JPGAction(KEY_NEXT,0);
                        else
                    #endif
                        {
                        // wyc1.07a. remove variable __bKeyRepeat, so update code to __wPlayItem++ to play next item.
                        __wPlayItem ++;
                        if ( __wPlayItem > __wTrackEnd )
                        {
                            __wPlayItem= __wTrackBegin;     // roll back
                            // ** 2.76-TCHM; begin... Force to reset ICs for long-time play under VCD title.
                            if ( __wDiscType & BOOK_VCD )
                                __bModePlay= MODE_PLAYUNKNOW;
                            // ** 2.76-TCHM; end...
                        }
                        }
                    }
                }
                else if (bKey == KEY_PREV)
                {
                    // LLY1.50, add previous chapter control for Nero Digital
#ifdef  SUPPORT_MP4
                    if( MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem) == EXTNAME_MP4 )
                    {
                        // if exist previous chapter, change to it
                        if(__wChapter > 1)
                        {
                            // [1] Calculate the desired time for previous changer beginning
                            __dwGoToTime = PARSER_GetMP4ChapterTime(PARSER_MP4_CHAP_PREV);;
                            // LLY2.36, keep corresping STC value here to avoid STC/90K= sec, then *90K=STC again
                            __dwGoTimeSTC = __dwGoToTime*90000;

                            // [2] Let it go the same flow of GoTime Action
                            __btNewTime=TRUE;

                            // [3] Transfer the command as KEY_PLAY
                            __bModeCmd = KEY_PLAY;

                            return TRUE;
                        }
                    }
#endif  // #ifdef SUPPORT_MP4

                    // wyc1.10, because set this __dwTimeKey to 0 will make F/W to clear the input number, so now not to
                    // clear it to allow user can input number by 10+ without F/W clear
                    //__dwTimeKey= 0;
                    // LLY0.83b-2, Let MM_Action(KEY_NEXT) handle NEXT action
                    // Because, __wPlayItem meaning maybe different between CT9928AF & CT908
                    // CT9928AF: __wPlayItem index from 1 ~ Total files within disc
                    // CT908: __wPlayItem index from 1 ~ Total files within current directory
                    // if nothing is done within this API, then go original flow
                    if(MM_ActionForChangeTrack(KEY_PREV, __wPlayItem)==MM_ACTION_CONTINUE)
                    {//winnie2.51, added to show JPG from SPI flash and key press PREV
                    #ifdef  SUPPORT_ENCODE_JPG_PICTURE
                       if(__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI)
                         LINEAR_JPGAction(KEY_PREV,0);
                        else
                    #endif
                        {
                        if ( __wPlayItem )
                        {
                        // wyc1.07a. remove variale __bKeyRepeat, so update code to __wPlayItem-- to play previous item.
                            __wPlayItem --;
                            if ( __wPlayItem < __wTrackBegin )
                                __wPlayItem= __wTrackEnd;
                        }
                        else    // why __wPlayItem = 0 ?, error protection
                            __wPlayItem= __wTrackEnd;
                        }
                    }
                }
            }
            break;

        case    KEY_NUMBER:     // play the track specified by "wTrackNO"
        #ifdef  SUPPORT_ENCODE_JPG_PICTURE
         if(__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI) //winnie2.51, added to show JPG from SPI flash and key press Number
                LINEAR_JPGAction(KEY_NUMBER,wTrackNO);
         else
        #endif
            {
            // mark the follow code
            if ( ! wTrackNO )   // means play current __wPlayItem
                break;          // do not change the __wPlayItem value

            // LLY0.83b-2, don't assign the input number to __wPlayItem
            // Because, it maybe out-of-range !!
            // new track number is assigned
            //__wPlayItem= wTrackNO;

            // LLY0.83b-2, Let MM_Action(KEY_NUMBER) handle NUMBER action
            // Because, __wPlayItem meaning maybe different between CT9928AF & CT908
            // And, the desired play item value will be kept in __wPlayItem
            // CT9928AF: __wPlayItem index from 1 ~ Total files within disc
            // CT908: __wPlayItem index from 1 ~ Total files within current directory
            // if nothing is done within this API, then go original flow
            __bTemp=MM_ActionForChangeTrack(KEY_NUMBER, wTrackNO);

            // wyc0.60-909, rename it.
            if(__bTemp==MM_ACTION_INVALID)
            {
                return FALSE;
            }
            else if(__bTemp==MM_ACTION_CONTINUE)
            {
                // for VCD, the __wPlayItem=TrackNO+1;
                if ( __wDiscType & BOOK_VCD )
                    wTrackNO++;          // Mapping to actual Track

                // Check the valid range in one title
                if ( wTrackNO > __wTrackEnd )
                {
// wyc.170a, add define to show invalid when input number out of range
#ifdef NUMBER_OUTRANGE_INVALID
                    OSD_Output(MSG_INVALID, OSD_ERR_KEY, 1 );
                    return FALSE;
#else
                    __wPlayItem= __wTrackEnd;
#endif //
                }
                else if( wTrackNO < __wTrackBegin )
                {
#ifdef NUMBER_OUTRANGE_INVALID
                    OSD_Output(MSG_INVALID, OSD_ERR_KEY, 1 );
                    return FALSE;
#else
                    __wPlayItem= __wTrackBegin;
#endif //
                }
                else
                    __wPlayItem=wTrackNO;
            }
            }
            break;

        default:
            return FALSE;
            break;
    }


// DVD_099bMicky, always swicth to NPF when change track
#ifdef FORCE_CHANGE_TRACK_NORMAL_PLAY
    if (__bModeCmd != KEY_PLAY)
#else
    // Clear STEP and FREEZE status when change track
    if ( (__bModeCmd == KEY_STEP) || (__bModeCmd == KEY_FREEZE) )
#endif
    {
        __bModeCmd=KEY_PLAY;
        //__btVideoFreeze=FALSE;
    }

    // LLY0.83b-2, integrated this procedure into KEY_NUMBER case
    // Because, __wPlayItem value has updated if the input key is out-of-range
    /*
    // Check the valid range in one title
    if ( __wPlayItem > __wTrackEnd )
    {
// wyc.170a, add define to show invalid when input number out of range
#ifdef NUMBER_OUTRANGE_INVALID
        OSD_Output(MSG_INVALID, OSD_ERR_KEY, 1 );
        return FALSE;
#else
        __wPlayItem= __wTrackEnd;
#endif //
    }

    if ( __wPlayItem < __wTrackBegin )
    {
#ifdef NUMBER_OUTRANGE_INVALID
        OSD_Output(MSG_INVALID, OSD_ERR_KEY, 1 );
        return FALSE;
#else
        __wPlayItem= __wTrackBegin;
#endif //
    }
    */
// end ..
    //////////////////////////////////

    bCmd= __bModeCmd;       // save current play mode
    if ( __bModeCmd == KEY_STOP )       // play the track from stop mode
    {
        // STOP->track->PLAY
        bCmd= KEY_PLAY;     // *TCH** 97.7.30;
    }
    else    // Playing->STOP->change track
    {
        // Simple the code, SCF just run through KEY_STOP

        // SCB->PREV track,  need take special process
        // 1. play the track beginning, for bitstream info.
        // 2. SCB from the track end
        if ( __bModeCmd == KEY_SCB )
        {
            // Do SCB operation
            BIT     btMute; // store it temporary

            btMute= __btMute;
            __btMute= TRUE;

            UTL_ForceChangeTrack ( __wPlayItem );
            // must show track number in scan backward cross track
            UTL_ShowTrack(MSG_TRACK, __wPlayItem, 3);

            __btPause= TRUE;
            __btPlaying= FALSE;

            // play from track end
            __dwTimeNow= __dwTimeEnd- 750L;
// DVD_276Micky, bit reduce
            //do not show play when scan back cross track
///            __btTrueKey=FALSE;

            // avoid two I-Frame display
            CC_KeyCommand ( KEY_PLAY ); //???
            OS_DelayTime ( COUNT_HALF_SEC ); //CoCo1.20, reduce code size
//DVD_049Micky
// TCH move from below.
            __btMute= btMute;   // ** TCH0.49; move from below, other KeyCommand Can't enable Audio
            CC_KeyCommand ( KEY_SCB );

//            __btMute= btMute;
            // wyc2.04-909S, replace by DISP_DisplayCtrl
            DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);

            return ERR_NOERR;
        }
        else    // Non KEY_SCB
        // wyc.172-CDDA, when meet CDDA medley title, we not execute stop command to save spend time when play CDDA auto across tracks.
        {
//LJY1.23, LJY1.11b-2, no need???(it will cause audio buffer won't be cleared while using number key to back to track1, and is not good for antishock mode)
// wyc2.37, MEDLEY pure CDDA can't do stop, otherwise audio not continuously.
#ifdef SUPPORT_PLAY_MEDLEYCDDA
            if ((__bCDDA_DTS != CDDA_PURE) || (__bKeySource != KEY_FROM_NONE))
#endif
                CC_KeyCommand ( KEY_STOP );
        }
    }
//LJY1.23, LJY1.11b-2, no need???
    // wyc.172-CDDA, when CDDA auto play across tracks we set some flags to make execute command play successful.
    // wyc2.37, MEDLEY pure CDDA can't do stop, otherwise audio not continuously.
#ifdef SUPPORT_PLAY_MEDLEYCDDA
    if ((__bKeySource == KEY_FROM_NONE) && (__bCDDA_DTS == CDDA_PURE))
    {
        __btPlaying = FALSE;
        __bModeCmd = KEY_STOP;
        __bCDDAAutoChTrk = TRUE;
    }
#endif //
    CC_KeyCommand ( bCmd );
    __btPlayEnd = FALSE ;

    return ERR_NOERR;
}

//  *********************************************************************
//  Function    :   LINEAR_Trigger
//  Description :   Check PlayEnd if player is playing...
//  Arguments   :   NONE
//  Return      :
//  Side Effect :
//  *********************************************************************
BYTE LINEAR_Trigger ()
{
    // LLY.099d, the invalid case for trigger while:
    // [1] F/W is non-playing, or
    // [2] Playing new time case, ex GOTOTIME, RESUME, REPEAT AB
    // Otherwise, it will cause GOTOTIME error while __dwTimeNow > __dwTimeEnd
    // __dwTimeNow: keep last playing time
    // __dwTimeEnd: keep the playing end time which will be played
    if ( ! __btPlaying || __btNewTime)
    {
        return ERR_ERROR;
    }

    // *TCH** 97.7.30; ??? How about UTL_OutputShowTime
//BCB begin
// wyc.172-CDDA, remove follow code to save time for CDDA medley title.
#ifndef SUPPORT_PLAY_MEDLEYCDDA
    if ( (OS_GetSysTimer()- __dwPreGetTime) > (COUNT_300_MSEC) ) // *** 0.45;
//BCB end
    {
        __dwPreGetTime=OS_GetSysTimer();
        UTL_GetCurrentTime ();

    }
    else
        return ERR_ERROR;
#else
    __dwPreGetTime=OS_GetSysTimer();
    UTL_GetCurrentTime ();
#endif
// end ..
    // LLY1.01, Why need to keep resume time each LINEAR_Trigger()
    // Record RESUME function
    //__dwResumeTime=__dwTimeNow;

/* Alan1.06 remove
//DVD_046-1DVDIF
#ifdef  SUPPORT_CDIF
    if (__bCurrentIF==BITSTREAM_CDIF)
        __wTemp= UTL_EstimateSectors ( COUNT_500_MSEC );
#endif
*/

    if (PARSER_QueryEvent( 0, PARSER_EVENT_COMPLETED, FALSE)) //LJY0.67, check parsing OK.(for bitstream end detection)
    {
        //LJY0.65, set it TURE for waiting buffer to be consumed.
//        __btPlayEnd = TRUE;

        HAL_ReadInfo(HAL_INFO_VIDEO_REMAIN, &__dwTemp);
        if(__dwTemp > 0x100)
        {
            return ERR_NOERR;
        }

        // DVD_161Micky, wait CD-DA data  buffer end.
        // so the track end display time won't less than one second.
        // LLY.274p, check playing mode method has updated
        //if (__bModePlay & MODE_PLAYCDDA)
        if(__bAttrPlay == ATTR_CDDA)
        {
            __dwSystemTimer=0;
//LJY1.21, LJY1.11, let CDDA can play till track end.

                HAL_ReadInfo(HAL_INFO_AUDIO_REMAIN, &__dwTemp);

        // wyc.172-CDDA, remove follow code to save time for playing CDDA medley title.
#ifndef SUPPORT_PLAY_MEDLEYCDDA
				if (__dwTemp > 0x30)
#else
				if (__dwTemp > 10000)
#endif //
					return ERR_ERROR; //LJY.pc908.060
            UTL_OutputShowTime();
        }

        //CHIPS_OpenAudio (FALSE);
/* Alan1.06 remove
        // wyc.172-CDDA, if play CDDA medley title we not mute the audio when across tracks.
//LJY1.21, LJY1.11, let VCD can play till track end.
#ifdef SUPPORT_ANTISHOCK
        else
        {
            //LJY1.11d, ~0.5sec tolerance for track change.
           //LJY1.24a, using UTL_EstimateBufferTime() for code refinement.
            __dwTemp = UTL_EstimateBufferTime();
            if((__dwTimeNow - __dwTemp) < (__dwTimeEnd- __wTemp))
                return ERR_ERROR;
        }
#endif
*/

#ifdef SUPPORT_PLAY_MEDLEYCDDA
        if (!(__wDiscType & BOOK_CDDA))
#endif //
        CHIPS_OpenAudio (FALSE);
        __dwTimeNow= __dwTimeBegin;
        __btPlaying= FALSE;
        __btPlayEnd= TRUE;
        PARSER_QueryEvent( 0, PARSER_EVENT_COMPLETED, TRUE); //LJY0.67, clear the flag of "PARSER_EVENT_COMPLETED".
        // PlayEnd, must check repeat AB
        //#if 1 to disable REPEATAB cross track
    }
    else if (__btIntroMode)
    {
        if ( __dwTimeNow > (__dwTimeBegin + __dwIntroPlayTime) )
        {
        __dwTimeNow= __dwTimeBegin;
        __btPlaying= FALSE;
        __btPlayEnd= TRUE;
        CHIPS_OpenAudio (FALSE);
        }
    }

    if ( ! __btPlayEnd )
        return ERR_ERROR;

    // STEP mode can't allow from the start of playing track
    if  ( (__bModeCmd == KEY_STEP) || (__bModeCmd==KEY_FREEZE) )
    {
        __bModeCmd=KEY_PLAY;
        //__btVideoFreeze=FALSE;
    }

    //Alan1.02a, Bug #286
    OS_DelayTime(COUNT_50_MSEC); //CoCo1.20, reduce code size

    // wyc.172-CDDA, we add a parameter for CDDA playing, when 1 to represent playing across
    // tracks automatically. 0 means press keys to across tracks.
    LINEAR_ChangeTrack(KEY_NEXT, 1);

    return ERR_NOERR;
}

//  *********************************************************************
//  Function    :   LINEAR_PlayItem
//  Description :   Will get the range of __wPlayItem, and start play
//  Arguments   :   None
//
//  Return      :   None
//  Change      :   __wPlayItem
//  Side Effect :
//  *********************************************************************
void LINEAR_PlayItem()
{
    // Pause->Playing
    // do not re-initial the play range
    // just resume from the current time(__dwTimeNow)

    if ( __btPause )
    {
//        if(__dwTimeNow==0)
//            printf("[1] Play Item w/ zero\n");

        UTL_PlayItem (__wPlayItem, __dwTimeNow );
    }
    else    // Stop->Playing
    {
        // Micky1.07-2, DVD only need the action that UTL_PlayItem.
        // DVD PGM->play->next(auto or user key)-> will call LINEAR_ChangeTrack-> NV_TitlePlay
        // CC_KeyCommand->Get_PlayRange-> LINEAR_PlayItem-> only need do reset+command.
#ifndef NO_DISC_MODE //CoCo2.37p
        if (__wDiscType & BOOK_DVD)
        {
            // Micky1.10, fix stop->program-> play, always play TT/CC 1/1.
            // For DVD, RESUME-> PLAY, use common path
            // For VCD, RESUME-> KEY_GOTOTIME_PLAY
            // PGM + RESUME, RESUME should be high priority
            if ( __bModeCELLTimeSearch != CELL_TIME_BEGIN )
            {
                UTL_PlayItem (__wPlayItem, 0 );
                return;
            }
        }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

        // 1. for PROGRAM mode,
        // first must get Program entry, and mapping to __wPlayItem
        // Must send program index for next track
        if ( __btPlayingProgram )
        {
            // wyc.pro __wProgramData[0] -> __bProgramMaxEntry
            if ( __bProgramMaxEntry )    // Program has entries
            {
                // wyc.pro to let DVD can run program playing from stop mode
#ifndef NO_DISC_MODE //CoCo2.37p
                if (__wDiscType & BOOK_DVD)
                {
                    __bTemp = _LinearEntryOperation(LINEAR_FIND_ENTRY,1);
                    if (__bTemp == 1)
                    {
//Kevin0.86, __bTitle/__wChapter for font mode / GDI mode program UI
                        __bTitle = __ProgIdxList[__bProgramPlayingIndex].bMainIndex;
                        __wChapter = __ProgIdxList[__bProgramPlayingIndex].wSubIndex;

                        NV_TitlePlay(__bTitle,__wChapter);

                        //Ming0.72, Must restore the command before next play in PROGRAM mode.
                        // TODO: Must review all the codes which call audio reset.
                        UTL_TransferCommandID(__bModeCmd);

                    }
                    else
                        __bFWKey = KEY_STOP;
                }
                else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
                {
//Kevin0.86, __wPlayItem for font mode / GDI mode program UI
                    __MMPlayingItem.wDirID = __ProgIdxList[__bProgramPlayingIndex].bMainIndex;   //for FM use only
			 __wPlayItem= __ProgIdxList[__bProgramPlayingIndex].wSubIndex;

                    // for VCD, the __wPlayItem=TrackNO+1;
                    if ( __wDiscType & BOOK_VCD )
                        __wPlayItem++;
                }
            }
//DEBUG            // must debug, will come here???
            else        // Program has no entry
            {
                // DVD_100aMicky, clear __btProgram first
                // as panel will reference it from panel 2.00
                /*__btProgram= FALSE;         // no recording program-data
                __btPlayingProgram = FALSE;
                __bMMMenuID = FM_MENU_NORMAL;*/
                // wyc1.10-3, clear all program variables to avoid sometimes the UI in program mode but __btProgram is FALSE.
                LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
//                OSD_OUTPUT_MACRO(MSG_CLEARPROGRAM, FALSE, 0); //CoCo.101
            }
        }

        // 2. for Random mode
        // must random give a __wPlayItem
        if ( __btRandom )
        {
#ifndef  NO_MP3
            if (__wDiscType & CDROM_M1)
            {
                // wyc1.07, because old method only search 100 times to try to find playable files dir, and __dwCountSystem will be similar
                // when searching this 100 times. So it is possible of finding no playable dir in this searching and __MMPlayingItem.wDirID will be assign
                // a unknown value. So add protection to make sure we can get a playable dir index after this loop.
                // wyc0.85a, use new structure because of supporting sort/filter now.
                __MMPlayingItem.wDirID = (BYTE)(OS_GetSysTimer() % __bMMTotalDirs);
                while (__pMMFilterFilesOfDir[__MMPlayingItem.wDirID] == 0)
                {
                    // wyc1.21, the old method to compute directory / files have bugs and will cause Picture CD go wrong
                    // dir / file in random mode. Modify to fix it.
                    __MMPlayingItem.wDirID++;
                    if ( __MMPlayingItem.wDirID >= __bMMTotalDirs)
                        __MMPlayingItem.wDirID = 0;
                }

                __wPlayItem = (WORD)(OS_GetSysTimer() % __pMMFilterFilesOfDir[__MMPlayingItem.wDirID]);
                __wPlayItem+= __wTrackBegin;
            }
            else
#endif
            {
                __wPlayItem= (WORD)(OS_GetSysTimer() % (__wTrackEnd- __wTrackBegin+ 1));
                __wPlayItem+= __wTrackBegin;
            }
        }
        //DVD_047
        // for DVD, don't call this, as it will clear current time as 0
        // cause KEY_NEXT/PREV will show time 0 then chapter time
        if ( !(__wDiscType & BOOK_DVD) )
            UTL_ShowTrack ( MSG_TRACK, __wPlayItem, 3 );

        // play the __wPlayItem from the beginning

#ifndef NO_MP3
        if (__wDiscType & CDROM_M1)
        {
#ifdef  SUPPORT_ENCODE_JPG_PICTURE
			if(__SF_SourceGBL[0].bSourceIndex!= SOURCE_SPI) //winnie2.51,the jpg can't do MM_PlayItem from SPI flash
				MM_PlayItem(__wPlayItem);
#else
            MM_PlayItem(__wPlayItem);
#endif //SUPPORT_ENCODE_JPG_PICTURE
			
        }
#endif  // #ifndef NO_MP3

        // LLY1.01, let CDROM motion go different path
        // Since, F/W need to pre-scan some necessary information first
        if(__wDiscType & CDROM_M1)
        {

            if( UTL_CheckPlayItemMode(__wPlayItem)==MODE_PLAYMOTION )
            {
                // Do initial before playback a new item
                UTL_Init_PlayItem(__wPlayItem);

                // LLY1.02, check previous and new play item & directory are same
                // And, keep desired play item & dir into __wPrevPlayItem and __wPrePlayDir if not same
                // to identify previous and new play item & direcory are same
                if( (__wPrevPlayDir==__MMPlayingItem.wDirID) && (__wPrevPlayItem==__wPlayItem))
                {
                    __bSameItem=TRUE;
                }
                else
                {
                    __bSameItem=FALSE;
                }

#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
                // LLY1.11, pre-start servo first to avoid servo @ stop state
                // Since, motion file pre-scan action will need servo read data action
                // And, our normal code will do servo pre-start @ UTL_Start_PlayItem()
                UTL_PreStartServo(__dwTimeNow, ACTION_SKIP);
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p


                // LLY1.20, integrate parsing SP file action into _MM_MotionTrigger()
#if 0//def  SUPPORT_CHAR_SUBPICTURE
                // LLY1.11, parsing AVI Subpicture File while playback the non-same AVI file
                // Notice: current code assume CHAR_SP_ParserSPFile will be done before AVI file parsing. Otherwise,
                // (1) Index table will be destroyed since INFOFILTER_DumpSector() using same DRAM of index table
                // (2) SP related setting will be wrong since it will be set within UTL_Read_PreScan_Info(PRESCAN_AVI_INFO)
                if(!__bSameItem && __bAttrPlayNew==ATTR_AVI)
                {
                    CHAR_SP_ParseSPFile();
                    printf("Parser SP File\n");
                }
#endif

                // LLY1.20, enable Data Disc motion state as prescan mode to let it can go _MM_MotionTrigger()
                // Notice: don't use __bAttrPlay = ATTR_PRESCAN_INFO method
                //         Otherwise, we can't integrate SP file parsing into pre-scan stage.
                __bMotionState = MOTION_STATE_PRESCAN;

#if 0

                // LLY1.02, set playing mode attribute as prescan CDROM motion information
                __bAttrPlay=ATTR_PRESCAN_INFO;
#endif  // #if 0

                return;
            }
        }

        UTL_PlayItem (__wPlayItem, 0);




    }
}

//  *********************************************************************
//  Function    :   LINEAR_Number
//  Description :
//  Arguments   :   bNumber     : The
//  Return      :   TRUE, number is valid, and perform action
//              :   FALSE, number is invalid or out of range
//  Change      :
//  Side Effect :
//  *********************************************************************
BYTE LINEAR_Number( WORD wNumber )
{
    // in Input GoToTime mode
//alex1.23,20040218 combine gototime , search,bookmark, Remove __btGotoTime , use __bSearhMode in Linear_Number function
    //if (__btGoToTime)
    if(__bSearchMode==SEARCH_MODE_TIME)
    {
        // do not allow the value larger
        if ( wNumber>=10 )
        {
//            OSD_Output(MSG_INVALID,  OSD_ERR_KEY, 1 );
            return FALSE;
        }
        else    // process this input
        {
            BYTE            bTemp;

            // *** MULTI_DISC;  Count the I/P NO. of Time
            __bGoToTimeNO++;
//DVD_046-1
// don;t support goto time number rounding
#ifdef      GOTOTIME_NUMBER_NOROUND
            if(__bGoToTimeNO>5) return FALSE ; // pxu 2000/09/05 for input number should not more than 5
#endif

#ifdef    GOTOTIME_LEFTTORIGHT
            // LLY.037, support H:MM:SS format input mode
            // and there is no limit for input digit
            if(__bGoToTimeNO == 1)
                __bTimeHour=(BYTE)wNumber;
            else if(__bGoToTimeNO == 2)
                __bTimeMin=(BYTE)wNumber*10;
            else if(__bGoToTimeNO == 3)
                __bTimeMin+=(BYTE)wNumber;
            else if(__bGoToTimeNO == 4)
                __bTimeSec=(BYTE)wNumber*10;
            else
                __bTimeSec+=(BYTE)wNumber;

/*          // only support MM:SS format input mode
            if ( __bGoToTimeNO == 1 )
                __bTimeMin= bNumber* 10;
            if ( __bGoToTimeNO == 2 )
                __bTimeMin+= bNumber;
            if ( __bGoToTimeNO == 3 )
                __bTimeSec= bNumber* 10;
            if ( __bGoToTimeNO == 4 )
                __bTimeSec+= bNumber;
*/
#else
            __bTimeHour = __bTimeMin/10; // LLY.0371, calculate hour part

            bTemp= __bTimeMin% 10;
            __bTimeMin= bTemp* 10+ __bTimeSec/ 10;
            bTemp= __bTimeSec% 10;
            __bTimeSec= bTemp* 10+ (BYTE)wNumber;

#endif    // GOTOTIME_NORMALSEQUENCE
            // LLY.037, it's unnecessary becasue MSG_GOTOTIME will reference
            // __bTimeHour, __bTimeMin, __bTimeSec directly
//            __wTemp= MAKEWORD ( __bTimeSec, __bTimeMin );
//            OSD_OUTPUT_MACRO ( MSG_GOTOTIME, __wTemp, 0xff );
        // DVD_099Micky, GOTOTIME clear
//            OSD_OUTPUT_MACRO ( MSG_GOTOTIME, 0, 0xff );
//LJY.pc908.060, removed            OSD_OUTPUT_MACRO ( MSG_GOTOTIME, __bGoToTimeNO, 0xff );

#ifdef    GOTOTIME_LEFTTORIGHT
            if ( __bGoToTimeNO == 5 )   // LLY.036
            {
            // DVD_277Micky,Fix DVD(GOTOTIME_LEFTTORIGHT) input 5 number-> always show invalid
            // update cc/linear/input.h
            // move the codes to KEY_GOTOTIME_VALUE
            // calculate __dwGoToTIme at one place.
                __bFWKey=KEY_GOTOTIME_VALUE;
            }
#endif    //#ifdef    GOTOTIME_LEFTTORIGHT
            return TRUE;
        }
    }

    // key 10+ still can't clear the program mode
    // Key number(KEY_Nx) will clear the program status
    // is playing program list
    if ( __btPlayingProgram && !__btInputProgram && !(wNumber & 0x8000))
    {
        LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
    }


//Kevin0.86, GDI mode program UI doesn't process number keys
    if ( __btProgram )
    {
    	return TRUE;
    }
    if ((__bAttrPlay == ATTR_JPG) && (__dwMMJPEGPreview == TRUE))
        MM_ResetPreviewMode();
    // LLY0.83b-2, return the status of LINEAR_ChangeTrack()
    //LINEAR_ChangeTrack ( KEY_NUMBER, wNumber);
    //return TRUE;
    return LINEAR_ChangeTrack(KEY_NUMBER, wNumber);
}

//  *********************************************************************
//  Function    :   LINEAR_Action
//  Description :
//  Arguments   :   KEY_GOTOTIME/ KEY_GOTOTIME_PLAY/ KEY_REPEAT_AB
//  Return      :
//  Side Effect :
//  *********************************************************************
BYTE LINEAR_Action(BYTE bKey, DWORD dwParam)
{
    __btIntroMode=FALSE;
    switch(bKey)
    {
// f/w 0.34
        case    KEY_NUMBER:
                return LINEAR_Number((WORD)dwParam);

        case    KEY_GOTOTIME:
                // PROGRAM / GOTIME can't exist at the same time
                if (__btProgram)
                {
                    LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
                }
                // Micky1.23, LINEAR_Action(KEY_GOTOTIME) must reference __bSearchMode do action, can't modify __bSearchMode.
/*
//                if ( __btGoToTime )
                if(__bSearchMode==SEARCH_MODE_TIME)//alex1.23,20040218 combine gototime , search,bookmark
                {
                    OSD_OUTPUT_MACRO ( MSG_SETGOTOTIME, OSD_CANCEL_TIME, 0 );
                    //__btGoToTime= FALSE;
                    __bSearchMode=SEARCH_MODE_OFF;//alex1.23,20040218 combine gototime , search,bookmark
                }
                else
*/
                {
                    // enable GOTOTIME in all kinds of play mode.
                    // But after KEY_PLAY trigger, will goto normal play mode.

                    // clear time key
                    // disable key 10+ before entering GOTOTIME mode
                    __dwTimeKey=0;
                    //LJY1.24a, marked for reviewing initialization of MODE_LINEAR.
                    //UTL_ModeInitial ( MODE_LINEAR );
                    //__btGoToTime= TRUE;
                    __bSearchMode=SEARCH_MODE_TIME;//alex1.23,20040218 combine gototime , search,bookmark

                    //Kevin2.80, move before OSD_OUTPUT_MACRO ( MSG_SETGOTOTIME, OSD_INPUT_TIME, 0xFF ), 'cause 908 GOTOTIME UI needs the reset of the following
                    __bTimeHour=0;  // LLY.036
                    __bTimeMin= 0;
                    __bTimeSec= 0;
                    __bGoToTimeNO= 0;

//LJY.pc908.060, removed                    OSD_OUTPUT_MACRO ( MSG_SETGOTOTIME, OSD_INPUT_TIME, 0xFF ); //always show goto time
                }
                return TRUE;
        case    KEY_GOTOTIME_PLAY:
        {
                //__dwGoToTime will be input at entry
                //__dwGoToTime is relative to the first playable track
//DVD_050SRAM
//                DWORD       dwTemp;
//                BYTE    bTemp;

//DVD_049Micky
// remove it, as now won't disable IR
//                EX0= 1;  //enable IR interrupt

                //check if out of range
                // DVD_161icky, remove it,(-150L)
                // fix A-Bex(TCD-784)T26, pre-stop at 2 second, can't resume.
                // as last track should be able to play to __dwTimeDiscEnd.
//                if ( dwParam >= (__dwTimeDiscEnd-150) )    //last 2 seconds
            // Micky1.25-2, move out the action to CC.c
            /*
                if ( dwParam >= (__dwTimeDiscEnd) )
                {

                    // ** 0.17; Add it, otherwise PANEL don't know the status
                    //if ( __btGoToTime )
                    if ( __bSearchMode==SEARCH_MODE_TIME)//alex1.23,20040218 combine gototime , search,bookmark
                    {
                    //__btGoToTime= FALSE;
            	        __bSearchMode=SEARCH_MODE_OFF;//alex1.23,20040218 combine gototime , search,bookmark
                        OSD_OUTPUT_MACRO ( MSG_SETGOTOTIME, OSD_CANCEL_TIME, 0 );
                    }
                    return FALSE;
                }
            */

                // ** 0.17; Move from above "EX0= 1";
                // Must clear GoToTime input mode
                //if (__btGoToTime)
                if (__bSearchMode==SEARCH_MODE_TIME)//alex1.23,20040218 combine gototime , search,bookmark
                {
//LJY1.20, clear it later in _PlayTimeA for fixing GOTOTIME OSD time incorrect issue.
//                    __btGoToTime= FALSE;
//LJY.pc908.060, removed                    OSD_OUTPUT_MACRO ( MSG_SETGOTOTIME, OSD_COMPLETE_TIME, 0 );
                }

                //enable GOTOTIME for any mode, special processing pause mode
                // wyc1.21
                /*if ( __bModeCmd == KEY_FF ) //must slow down CDROM speed
                {
                    UTL_ChangeCDROMSpeed (1);
                }*/

                // LLY0.85, support CDROM A/V gotime action.
                // LLY2.05, support CDROM/ DVDROM audio only file gotime action.
                if( (__bAttrPlay & TYPE_CDROM_AV) || (__bAttrPlay & TYPE_CDROM_AUDIO) )
                {
                    __dwTemp=__wPlayItem;
                }
                else
                {
                    // Micky1.10, Fix JVC-> segmeny play item-> resume-> will play wrong range begin
                    // Only need to query PlayItem number for Track/ Entries
                    if (__wPlayItem <= 599)
                    {
#ifndef NO_DISC_MODE //++CoCo2.37p
                        __dwTemp=dwParam;
                        // Get track number and new Play Range
                        CDINFO_ParseInfo ( VCD_ENTRIES_TRACK, & __dwTemp, 0 );

                        UTL_GetPlayRange ( (WORD)__dwTemp );

#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
                    }
                    else
                        __dwTemp = __wPlayItem;
                }
                // Micky1.08, Fix JVC-> segmeny play item-> resume-> will play wrong range end

                // f/w 0.45, remove these
                // since in VCD2.0, this process will have problem
                // In PBC mode, when __dwTimeBegin==__dwGoToTime, can't treas as KEY_Nx
                // it will destroy PBC info.(in KEY_PRACTICE)

                // go time cross another track
                // or go time from stop mode
                if ( (__wPlayItem != __dwTemp) || !__btPlaying )
                {
                    if (__wPlayItem != __dwTemp )
                    {
                        // Cross different track
                        __btPlayFromBegin=TRUE;
                        __btFromStopMode=TRUE;
                    }
                    else
                    {
                        if ( ! __btPause ) // From stop mode
                        {
                            __btPlayFromBegin=TRUE;
                            __btFromStopMode=TRUE;
                            __bModeCmd=KEY_PLAY;
                        }
                        else
                        {
                            __btPlayFromBegin=FALSE;
                            __btFromStopMode=FALSE;
                        }
                    }
                }
                else    //go time within same track
                {
                    __btPlayFromBegin=FALSE;
                    __btFromStopMode=FALSE;
                }

                __wPlayItem=(WORD)__dwTemp;
                __btNewTime=TRUE;

                // LLY1.05, set the desired playback command for _Play_TimeA() reference
                // Since, it don't transfer playback mode as NPF within _Play_TimeA()
                // Otherwise, the behavior maybe fail while FF --> GoTime
                __bModeCmd=KEY_PLAY;

                return TRUE;
        }

        case    KEY_REPEAT_AB:
            // LLY0.96, don't support Repeat AB action while non-exist DivX index table
            // LLY1.01, don't support Repeat AB action while audio only mode (video resolution > 720*576)
            // LLY2.31, call parser API to check if index table build done.
            __dwTemp = PARSER_QueryContainerAudioType(__dwSFStreamID, __bASTNO);
            //if( (__bAttrPlay == ATTR_AVI) && (__bNoDivXIndexTbl || __bVideoID==HAL_THROW_VIDEO ||__dwTemp == PARSER_ESFMT_WMA_V1 || __dwTemp == PARSER_ESFMT_WMA_V2) )
            if( (__bAttrPlay == ATTR_AVI) &&
                (!PARSER_QueryAVIIndexTable() || __bVideoID==HAL_THROW_VIDEO /*||__dwTemp == PARSER_ESFMT_WMA_V1 || __dwTemp == PARSER_ESFMT_WMA_V2*/) )
            {
                return (FALSE);
            }

                // only work in NPF and SF
                if (__btPlaying && ( (__bModeCmd == KEY_PLAY) || (__bModeCmd== KEY_SF)
                    // wyc2.17-909S, for language study.
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
                    || (__bModeCmd == KEY_LAN_STUDY)
#endif //
                ) )
                {
                    // Do not allow repeat AB for still picture and GOTOTIME
//alex1.23,20040218 combine gototime , search,bookmark, Remove __btGotoTime , use __bSearhMode in Linear_Acction function  (Key_Repeat AB)
//                    if ( ( __bModePlay & MODE_STILL) || __btGoToTime )
                    if ( ( __bModePlay & MODE_STILL) || __bSearchMode)
                    {
                        return FALSE;
                    }
                }
                else
                    return FALSE;

                switch ( __bRepeatAB )
                {
                    case    REPEAT_NONE:
                    default:
                        // first time
                         __bRepeatAB= REPEAT_A;
                         __dwTimeB= 0;
                        // LLY0.95, keep Time A value in second unit for DivX/ AVI file
                        // Since, we must give time for parser to get corresonding sectot ID (base on index table)
                        if(__bAttrPlay == ATTR_AVI)
                        {
                            // LLY1.01, keep time A in sec format from STC/90000.
                            HAL_ReadInfo(HAL_INFO_STC, &__dwTemp);
                            __dwTimeA=__dwTemp/90000;
                            // LLY2.36, keep corresping STC value here to avoid STC/90K= sec, then *90K=STC again
                            // LLY2.61, keep A point frame count in a independent variable
                            // Since, __dwGoTimeFrameCnt will be used for stop time
                            // ie. the value will be changed if repeat A-B --> enter/exit setup (do stop/ resume)
                            //__dwGoTimeSTC = __dwTemp;
                            __dwTimeA_STC = __dwTemp;
                            //printf("Time A STC: %lx\n", __dwTimeA_STC);
                        }
                        else
                        {
                            // LLY2.35, keep Time A value in second unit for WMA
                            // Since, we must give time for parser to get corresponding sector ID
                            if(__bAttrPlay == ATTR_WMA)
                            {
                                // Must /75 since _UTL_CalTime_FromFrameCnt() return value is second*75
                                __dwTimeA=_UTL_CalTime_FromFrameCnt()/75;
                            }
                            else // Others, keep current decoded sector ID
                            {
                                // LLY2.05, call API to get current decoded sector
                                __dwTimeA = UTL_GetDecodedTime();
                            }

                            // LLY2.05, also keep the time A's frame count for CDROM/ DVDROM audio only file
                            if(__bAttrPlay & TYPE_CDROM_AUDIO)
                            {
                                // LLY2.35, get decoded frame count from DSP directly
                                // LLY2.61, keep A point frame count in a independent variable
                                // Since, __dwGoTimeFrameCnt will be used for stop time
                                // ie. the value will be changed if repeat A-B --> enter/exit setup (do stop/ resume)
                                //__dwGoTimeFrameCnt = UTL_SectorsToFrameCnt(__dwTimeA-__dwTimeBegin);
                                //HAL_ReadAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, &__dwGoTimeFrameCnt);
                                HAL_ReadAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, &__dwTimeA_FrameCnt);
                            }
                        }

#ifndef NO_DISC_MODE //CoCo2.37p
                        if (__wDiscType & BOOK_DVD)
                        {
                            NV_Action ( ACTION_REPEATAB, __bRepeatAB );
                        }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
                        OSD_OUTPUT_MACRO ( MSG_REPEATAB, REPEAT_A, 0xFF );
                        break;

                    case    REPEAT_A:
                        // wyc1.06-909, only press KEY_PRPEATAB when REPEATA mode can make MSG_REPEATAB active.
                        __bActiveMessage = MSG_REPEATAB;
                        __bRepeatAB= REPEAT_AB;
#ifndef NO_DISC_MODE //CoCo2.37p
                        if (__wDiscType & BOOK_DVD)
                        {
                            NV_Action ( ACTION_REPEATAB, __bRepeatAB ); // ** TCH1.61a;
                        }
                        else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
                        {
                            // LLY2.05, call API to get current decoded sector
                            __dwTimeB = UTL_GetDecodedTime();
                            /*
                            UTL_GetCurrentTime ();
                            //LJY1.24a, using UTL_EstimateBufferTime() for code refinement.
                            __dwTemp = UTL_EstimateBufferTime();
                            __dwTimeB = __dwTimeNow - __dwTemp;
                            */

                            // LLY1.11, it's un-necessary to minus the time torelence
                            // Since, it maybe the old code for 8051 platform.
#if 0
                            // since in B point need do more check
                            // add time torelence
                            // so it's possible that the __dwTimeB < __dwTimeA for same track
    //                            __dwTimeB-=105L;
                            //__dwTimeB-=30L;
#endif  // #if 0
                        }

                        __btPlayFromBegin=FALSE;
                        __dwGoToTime=__dwTimeA;
                        // wyc1.06-909, when repeat AB message is active, then call OSD to show it.
                        if (__bActiveMessage == MSG_REPEATAB)
                        {
                        OSD_OUTPUT_MACRO(MSG_REPEATAB, REPEAT_AB, 0xFF);
                        }
                        break;

                    case    REPEAT_AB:
                            __bRepeatAB= REPEAT_NONE;
                            OSD_OUTPUT_MACRO(MSG_REPEATAB, REPEAT_NONE, 1);
                            break;
                }
                return TRUE;
    }

    return FALSE;
}

//  *********************************************************************
//  Function    :   LINEAR_Program
//  Description :   It will process PROGRAM mode ON/OFF, Add/Clear Entry
//  Arguments   :   bFunction   : the Program Function
//  Return      :
//  Side Effect :   It will use the value __wNumberPrevious
//  *********************************************************************
BYTE LINEAR_Program(BYTE bFunction, BYTE bKey)
{
    switch(bFunction)
    {
        case    LINEAR_PROGRAM_MODE:
                // wyc1.07, support KEY_PROGRAM when JPG slide show.
                if ( __btPBC
#ifndef NO_DIGEST //LJY1.20, support 4M flash, remove digest
                    || __btDIGEST
#endif
#ifndef NO_DISC_MODE //CoCo2.37p
                    || ((__wDiscType & BOOK_DVD)&&(__bNVPlayMode))
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
                    )
// wyc.101_2nd , add define FORCE_SWITCH_NONPBC and add code to support FORCE_SWITCH_NONPBC
#ifdef FORCE_SWITCH_NONPBC
                // wyc.160
#ifndef NO_DIGEST //LJY1.20, support 4M flash, remove digest
                if (__btDIGEST) return FALSE;
#endif
#else
                return FALSE;
#endif // FORCE_SWITCH_NONPBC

                // clear time key
                // disable key 10+ before entering PROGRAM mode
                __dwTimeKey=0;
/* LJY.pc908.060, removed
                // PROGRAM / GOTIME can't exist at the same time
//alex1.23,20040218 combine gototime , search,bookmark, Remove __btGotoTime , use __bSearhMode in Linear_Program function
         //if (__btGoToTime)
                if(__bSearchMode)
                {   // Clear GOTIME
                    //__btGoToTime= FALSE;
                    __bSearchMode=SEARCH_MODE_OFF;
                    OSD_OUTPUT_MACRO(MSG_SETGOTOTIME, OSD_CANCEL_TIME, 0);
                }
*/
                if (__btPlayingProgram)  //wyc.102, When program playing mode and press KEY_PROGRAM, enter program input/playing mode
                {
/* LJY.pc908.060, removed
                    //second PROGRAM key,
                    //already playing program list now
                    //show the existing program list now
                    // wyc.101 use define to replace number in OSD macro
                    //  press program key in program playing mode
                    if (__wDiscType & BOOK_DVD)
                    {
                        OSD_OUTPUT_MACRO(MSG_SETPROGRAM, OSD_DVD_CLEAR_BACKGROUND, 0xFF);
                    }
                    else
                    {
                        OSD_OUTPUT_MACRO(MSG_SETPROGRAM, OSD_VCD_CLEAR_BACKGROUND, 0xFF);
                    }
*/
                    // For panel
                    // Must set input program mode at last
                    __btInputProgram=TRUE;
                    return TRUE;
                }

                // enter program mode
                {
                    //DVD_051Micky, Number >10
                    __wNumberQueue=0x0000;

                    // initial the variable and array value for program
                    __btInputProgram=TRUE;
                    __btProgram= TRUE;
                    UTL_ModeInitial ( MODE_PROGRAM );
                    // only double program command to clear the program entry data
                    if (__bProgramMaxEntry == 0)
                    {
                        LINEAR_ProgramEntryOperation(LINEAR_CLEAR_ALL_ENTRY,NULL_0,NULL_0,NULL_0);
                    }

#ifdef    FORCE_CLEAR_LOGO
                    if ( __bModeCmd == KEY_STOP )
                        UTL_SetPlayMode ( MODE_PLAYMOTION );
#endif    // FORCE_CLEAR_LOGO
                    return TRUE;
                }
                return FALSE;

        case    LINEAR_PROGRAM_CLEAR:
        case    LINEAR_PROGRAM_CLEAR_MIN:
                // wyc1.05-2, when pressing KEY_STOP in FM, it will call LINEAR_Program(LINEAR_PROGRAM_CLEAR) to clear program entries. In FM mode we
                // don't clear program entries and only need to set __bProgramPlayingIndex to 0 because of we need to re-program playing from beginning.
                __bProgramPlayingIndex=0;   //Kevin1.00, add
                if (bFunction == LINEAR_PROGRAM_CLEAR_MIN)
                    return TRUE;
                LINEAR_ProgramEntryOperation(LINEAR_CLEAR_ALL_ENTRY,NULL_0,NULL_0,NULL_0);
                __bProgramMaxEntry = 0;
                __bProgramCurrentIndex = 0;
                __btProgram= FALSE;

                // To clear the display of program
                __dwTimeKey= 0;              // clear it if had pur "10+" key
                __btInputProgram=FALSE;
                __btPlayingProgram=FALSE;
                __btNextPlayProgram=FALSE;
                // wyc1.10-3, clear to normal mode to avoid program variable not identical
                // wyc1.10-pro, only clear to normal mode when original is program edit mode. Because JPG
                // playing mode don't need to clear to normal mode and need to stay in FM_MENU_NONE mode.
                // wyc1.21, fix input number in program playing mode, then UI incorrect problem.
                // wyc0.60-909, rename to MM_MENU_XXX
                // wyc0.80, remove variable __bMMHide
                if ((__wDiscType & CDROM_M1) && (__bMMMenuID == MM_MENU_EDITPROG))
                    MM_InitialMenu(MM_MENU_NORMAL, TRUE);

// LJY.pc908.060, removed
                // wyc0.80, need to call this function, otherwise JPG program playing -> open display mode -> KEY_PROGRAM -> normal JPG playing.
                // but program icon in slide show don't disappearred.

                OSD_OUTPUT_MACRO(MSG_PROGRAM, 0, 0);    //Kevin1.07a, update PlayingProgram display icon

//                OSD_OUTPUT_MACRO (MSG_CLEARPROGRAM, FALSE, 0);      //HLHAN1.01A

                return TRUE;

        case LINEAR_COUNT_ENTRY:
            // wyc.276, make Picture CD mode will not clear repear mode to avoid entering program mode in Picture CD then will not repeat all problem.
            if (__wDiscType & CDROM_PICTURECD)
                __bRepeat = REPEAT_DISC;
            else
                __bRepeat = REPEAT_NONE;
            __bProgramMaxEntry = 0;
            __bProgramCurrentIndex = 0;
            __bProgramPlayingIndex = 0;
            __wTemp = 0;
#ifdef FORCE_SWITCH_NONPBC
            // wyc.160
            __btPBC = FALSE;
#ifndef NO_DISC_MODE //CoCo2.37p
            __bNVPlayMode = FALSE;
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

            OSD_OUTPUT_MACRO(MSG_PBC,__btPBC,0x0);
#endif // FORCE_SWITCH_NONPBC
            for (__bTemp=0;__bTemp < MAX_PROG_ITEM;__bTemp++)
            {
                if (__ProgIdxList[__bTemp].wSubIndex)
                {
                    if (__wTemp == 0)
                    {
                        __bProgramPlayingIndex = __bTemp;
                        __wTemp = 1;
                    }
                    // use __bTemp+1 is to avoid when only one program entry in index 0,
                    // the __bProgramMaxEntry will be NULL_0. So __bProgramMaxEntry is the
                    // max. program item+1
                    __bProgramMaxEntry = __bTemp+1;
                }
            }
            return TRUE;
    }
    return FALSE;
}

/////////////////////////////   protected frunction
//  *********************************************************************
//  Function    :   _CheckRepeatMode
//  Description :   Check the repeat mode status to decide play next or not
//  Arguments   :   bKey    , the key action
//  Return      :   KEY_NEXT, increase the track number
//              :   KEY_PREV, decrease the track number
//              :   KEY_NUMBER, play the current track
//              :   KEY_STOP, stop playing
//  Side Effect :
//  *********************************************************************
BYTE _CheckRepeatMode(BYTE  bKey)
{
//WINDVD_027 add begin
    if ( (bKey!=KEY_NEXT) && (bKey!=KEY_PREV) && (bKey!=KEY_NUMBER) )
        return bKey;
//WINDVD_027 add end

#ifdef  SUPPORT_MP4
    // LLY1.50, don't do repeat mode check if current MP4 file playback is not @ boundary
    // Since, it still to do next/ prev chapter
    if(MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem)==EXTNAME_MP4)
    {
        if( (bKey==KEY_NEXT) || (bKey==KEY_PREV) )
        {
            if( (__wChapter > 1) && (__wChapter<__wChapter_Ns_Title) )
            {
                return bKey;
            }
        }
    }
#endif  // #ifdef SUPPORT_MP4


    switch(__bRepeat)
    {
    case    REPEAT_NONE:
        // do not play the next track when
        // play to last track or last program entry
        if (__btPlayingProgram)
        {
            // DVD_046-1
            // in PROGRAM mode, press KEY_PREV will always play
            if (bKey == KEY_PREV)
            {
                return bKey;
            }
            // current play index of program list
            // 2 means first index
            // already play to the last program index
            // wyc.pro __wProgramData[0] -> __bProgramMaxEntry, __wProgramData[1] -> __bProgramCurrentIndex
            // remove the code, becaues it is moved to anpther place to check
            //if ( __bProgramCurrentIndex == __bProgramMaxEntry - 1 )
            //    break;
        }
        else
        if ( bKey != KEY_PREV )
        {
            // *** MULTI_DISC;
            // ** 0.20; If KEY_NEXT by IR, just do it. Don't care of REPEAT Mode
            // LLY.274p-3, must use two item to check if IR or F/W key
            // [1] bKey==KEY_NEXT
            // [2] __bKeySource!=KEY_FROM_ISR
            //if ( __bKey != KEY_NEXT )
            if( (bKey==KEY_NEXT) && (__bKeySource != KEY_FROM_ISR) )
            {

                // play to the last track, and auto-stop
                // wyc1.07-3, because the __wPlayItem only update depending on forder, so we can match __wPlayItem and __wTrackEnd in FM mode.
                // So we need to provide a mechanism to check if this title have play to end of CDROM disc.
                if (__wDiscType & BOOK_M1)
                {
                    // wyc0.60-909, rename it.
                    // wyc2.38a-909s, add code to check filter mode when check end, otherwise the sort/filter mode won't stop when play to last file.
                    #ifdef  SUPPORT_ENCODE_JPG_PICTURE
                    if(__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI) //winnie 2.51,  the jpg can't do MM_IndexConvert  from SPI flash
                        return bKey;
                    #endif
                    if (MM_IndexConvert(MM_INDEX_CONVERT_DIR2DISC_BY_FILTER, __MMPlayingItem.wDirID, __wPlayItem) == __wMMTotalFilterFiles)
                    {
                        // wyc1.07-909, when playing to last file in MM mode, need to set this variable to TRUE to represent entering REAL stop mode.
                        __bMMEnterRealStop = TRUE;
                        break;
                    }
                }
                else
                {
                    if ( __wPlayItem == __wTrackEnd )
                        break;
                }
            }

        }   // bKey != KEY_PREV                   // *** 0.42; 12.2

        return bKey;
        break;

    case    REPEAT_DISC:
        // must play next/prev track
        // wyc0.75, when playing Picture CD, return MM_RepeatFolder to make its hehavior like repeat folder to fix Picture CD
        // repeat disc error problem.
        if (__wDiscType & CDROM_PICTURECD)
            return MM_RepeatFolder(bKey);
        else
        return bKey;

    case    REPEAT_FOLDER:
        // LLY0.83b-2, collect all procedure into MM_RepeatFolder()
        // Because, it will handle all thing for __wPlayItem in disc or directory view
        // if out-of the directory's up/low boundary, return KEY_REPEAT w/ desired __wPlayItem
        // otherwise, return the input key, and LINEAR module will handle it
        // ie. nothing is done within it
        return MM_RepeatFolder(bKey);
        break;

    case    REPEAT_TRACK:
    default:
        return KEY_NUMBER;  // just play current track
    }

    // stop playing according to REPEAT mode
    // must stop at current track
    UTL_ModeInitial ( MODE_LINEAR );

// recursive
// 07.21.99
    // wyc1.06-909, when playing to last dir/file in MM mode, set __bLastPlayItem to let MM can reference it to not
    // enter thumbnail mode when last play item is JPG file.
    __bLastPlayItem = TRUE;
    __bFWKey=KEY_STOP;
/*
    CC_DoKeyAction ( KEY_STOP );
    // do not re-draw LOGO again for CDDA disc
    if ( !( __wDiscType & BOOK_CDDA) )
        LOGO_Show(__bLogo);;
    OSD_OUTPUT_MACRO ( MSG_STOP, NULL_0, 3 );
*/

    return KEY_STOP;
}


// wyc.pro1, Modify this function to do the operation for entry control
//  *********************************************************************
//  Function    :   _LinearEntryOperation
//  Description :   Do entry operation
//  Arguments   :   bType  : The operation decision, one is to find entry, the other is to convert __bProgramInputNum[] to __wProgramData[]
//              :   bParam : decide to clear program entry array or number array
//  return      :   return 1, means find not empty entry. If 0 not finding
//  Side Effect :
//  *********************************************************************
BYTE _LinearEntryOperation(BYTE bType, BYTE bParam)
{
    switch (bType)
    {
    case LINEAR_FIND_ENTRY:
        __bTemp = TRUE;
        __wTemp = __ProgIdxList[__bProgramPlayingIndex].wSubIndex;

        if (__wTemp == NULL_0)
        {
            __bTemp = FALSE;

            // wyc1.10-3, need to decrease 2 is because decrese 1 will make __bProgramPlayingIndex increase to 20
            // and restore error data in __ProgIdxList.
            // LJY0.60p, "__bProgramPlayingIndex >= 0" always true.
            while (( __bProgramPlayingIndex<= (MAX_PROG_ITEM-2)))// && (__bProgramPlayingIndex >= 0))
            {
                if (bParam == 0)
                {
                    // wyc.160, fix the Repeat all problems in program mode when first entry is empty
                    if (__bProgramPlayingIndex != 0)
                        __bProgramPlayingIndex--;
                    else
                    {
                        if (__bRepeat == REPEAT_DISC)
                            __bProgramPlayingIndex = MAX_PROG_ITEM-1;
                        else
                            break;
                    }
                }
                else if (bParam == 1)
                    __bProgramPlayingIndex++;

                if (__ProgIdxList[__bProgramPlayingIndex].wSubIndex )
                {
                    __bTemp = TRUE;
                    break;
                }
            }
        }
//        return __bTemp;
        break;
    }
    return __bTemp;
}

//Kevin0.86, create
//  *********************************************************************
//  Function    :   LINEAR_ProgramEntryOperation
//  Description :   Query/ Add / Delete the program entry
//  Arguments   :   bAction : Query / Add / Delete
//                  bArrayIndex: Program data array index
//                  bMainIndex: DVD title   / VCD,CDDA NULL_0  / MP3,JPEG dirIndex
//				    wSubIndex:  DVD chapter / VCD,CDDA track / MP3,JPEG fileIndex
//  Return      :   value other than 0xff: Queried/Added/Deleted index
//              :   0xff: FALSE
//  Side Effect :	None
//  *********************************************************************
BYTE LINEAR_ProgramEntryOperation(BYTE bAction, BYTE bArrayIndex, BYTE bMainIndex, WORD wSubIndex)
{
    switch (bAction)
    {
    case LINEAR_CLEAR_ALL_ENTRY: //replace LINEAR_ClearProgramEntry()
        for(__bTemp=0; __bTemp<MAX_PROG_ITEM; __bTemp++)
        {
            __ProgIdxList[__bTemp].bMainIndex=0;
            __ProgIdxList[__bTemp].wSubIndex=0;
        }
        __bProgramMaxEntry=0;
        return 0;

	case LINEAR_QUERY_INDEX: //dirIndex, fileIndex,
        for(__bTemp=0; __bTemp<MAX_PROG_ITEM; __bTemp++)
        {
            if ( (__ProgIdxList[__bTemp].bMainIndex == bMainIndex) && (__ProgIdxList[__bTemp].wSubIndex == wSubIndex) )
                return __bTemp; //entry found in the array
        }
        return 0xff; //entry not found in the array

	case LINEAR_ADD_ENTRY_BY_INDEX:
        // wyc2.51-909P, add code to add/remove the entries in JPG encode mode.
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
        if ((__bMMMenuID == MM_MENU_ENCODE_FILE) && (__wDiscType & CDROM_M1))
        {
    	    if ((bArrayIndex+__bMMJPGEncodeNum)>=MAX_PROG_ITEM)
                return 0xff;
        }
        else
#endif //
        {
    	    if (bArrayIndex>=MAX_PROG_ITEM)
                return 0xff;
        }
        __ProgIdxList[bArrayIndex].bMainIndex=bMainIndex;
        __ProgIdxList[bArrayIndex].wSubIndex=wSubIndex;
        return bArrayIndex;

	case LINEAR_DELETE_ENTRY_BY_INDEX_W_REARRANGE:
	    if (bArrayIndex>=MAX_PROG_ITEM)
            return 0xff;
        for(__bTemp=bArrayIndex; __bTemp<(MAX_PROG_ITEM-1); __bTemp++)
        {
	        __ProgIdxList[__bTemp]=__ProgIdxList[__bTemp+1];
	    }
        __ProgIdxList[MAX_PROG_ITEM-1].bMainIndex=0;
        __ProgIdxList[MAX_PROG_ITEM-1].wSubIndex=0;
        return bArrayIndex;

    case LINEAR_DELETE_ENTRY_BY_INDEX_WO_REARRANGE:
	    if (bArrayIndex>=MAX_PROG_ITEM)
            return 0xff;
        __ProgIdxList[bArrayIndex].bMainIndex=0;
        __ProgIdxList[bArrayIndex].wSubIndex=0;
        return bArrayIndex;

    default:
        return 0xff;
    }
    return 0xff;
}

//Winnie, create
//  *********************************************************************
//  Function    :   LINEAR_JPGAction()
//  Description :   play key_next/key_prev/key_number for play jpg from SPI flash
//  Arguments   :
//                  bKey: key_next/key_prev/key_number
//                  bPlayIdx: mean play jpg item from SPI flash
//  Return      :   TRUE: Key_next/key_prev/key_number work
//                      FALSE: Key_next/key_prev/key_number can't work
//  Side Effect :	None
//  *********************************************************************
#ifdef  SUPPORT_ENCODE_JPG_PICTURE
BYTE LINEAR_JPGAction(BYTE bKey,BYTE bPlayIdx)
{
  switch(bKey)
        {
            case    KEY_NEXT:
                __wPlayItem++;
                if(__wPlayItem>__bMMJPGEncodeNum) //roll back
                    __wPlayItem=1;       // __bOSDSSPicIdx==0 is play 1th jpg play item
                break;
            case    KEY_PREV:
                if(__wPlayItem)
                {
                    __wPlayItem--;
                    if(__wPlayItem<=0)  // __bOSDSSPicIdx==0 is play 1th jpg play item
                    __wPlayItem=__bMMJPGEncodeNum; //roll back to final jpg play item
                }
                break;
            case    KEY_NUMBER:
                if(!bPlayIdx) break; //if play same item or repeat track, then return
                if(bPlayIdx>__bMMJPGEncodeNum) //check the vaild range when input number out of range
                    __wPlayItem=__bMMJPGEncodeNum;
                else if (bPlayIdx<0)
                    __wPlayItem=1;
                else
                    __wPlayItem=bPlayIdx;
                break;
            default:
                return FALSE;
                 break;

        }
    return TRUE; //no-error
}
#endif //

