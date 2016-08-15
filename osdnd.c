#define OSDND_MAIN_FILE

#include "winav.h"
#include "ctkav.h"
#include "infofilter.h"
#include "osd.h"
#include "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include "osddsply.h"
#include "osdnd.h"
#include "cc.h"
#include "hal.h"
#include "setup.h"
#include "chips.h"
#include "utl.h"
#include "navigate.h"
#include "haljpeg.h"
#include "dvdinfo.h"
#include "initial.h"
#include "disp.h"
#include "haljpeg.h"
#include "mmanager.h"
#include "navigate.h"
// wyc1.21-909, 
#include "media.h"
#include "notransl.h"

#define OSDND_MAIN_FILE
//#include "string.lan"
#include "OSDString\\strextrn.h"

PARM_DRAW_STRING __OSDNDUpMsgAttr;
PARM_DRAW_STRING __OSDNDBottomMsgAttr;
PARM_DRAW_STRING __OSDNDTempStringAttr;
PARM_FRAME _OSDNDFrame;

BYTE _bOSDNDMsg;
WORD _wOSDNDInfo;
BYTE _bOSDNDTemp;
WORD _wOSDNDTemp;
DWORD _dwOSDNDTemp, _dwOSDNDTemp1;
WORD _wOSDNDHPos;
PARM_RECT _OSDNDRect;
PARM_VOLUME_BAR _OSDNDVolumeBar;
BYTE __bOSDUTLIndex;
WORD *__pOSDUTLOutputString;
WORD __wOSDUTLStringBuf[OSDUTL_STRING_LENGTH+1];
extern BYTE __bSurround;

WORD aSPACE[] = {1, CHAR_SPACE};

//CoCo.CT909, add it for linkage issue. Will remove them.
BYTE __bOSDFg;
BYTE __bOSDBg;
WORD __wOSDUpVPos;
WORD __wOSDUpHPos;
WORD __wOSDBottomVPos;
WORD __wOSDBottomHPos;
WIN_COLOR _OSDNDZoomPIPColor;

//CoCo1.07, the variables for displaying progress bar.
WORD __wProgressTotal;
WORD __wProgressCurrent;

extern WORD __wJPEGFMTExifImageWidth;
extern WORD __wJPEGFMTExifImageHeight;
extern CHAR __cJPEGFMTExifDate[11];
extern DWORD	__dwPicHSize;
extern DWORD	__dwPicVSize;
extern WORD    __wHCW, __wVCW;
extern SHORT   __wXStart, __wYStart;
extern BYTE _bOSDCurrentUI;
extern BYTE    __bChooseMedia; //0==>CDROM/DVDROM, 1==>USB
//***************************************************************************
//  Function    :   OSDND_GetMessagePos
//  Abstract    :   Call this function to get the type of the message.
//  Arguments   :   bMessage: the message.
//  Return		:   OSD_ND_UPPER_RIGHT_REGION: The upper right message.
//              :   OSD_ND_BOTTOM_REGION: The bottom message.
//              :   OSDND_UNKNOWN_REGION: unknown region.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE OSDND_GetMessagePos(BYTE bMessage)
{
    switch (bMessage)
    {
    case MSG_DISCTYPE:
    case MSG_WAITING: 
    case MSG_OPEN:
    case MSG_NEXT:
    case MSG_PREV:
    case MSG_RETURN:
    case MSG_PLAYING:
    case MSG_STOP:
    case MSG_PAUSE:
    case MSG_FAST:
    case MSG_STEP:
    case MSG_FREEZE:
    case MSG_REPEAT:
    case MSG_REPEATAB:
    case MSG_RANDOM:
    case MSG_PBC:
    case MSG_TVMODE:
    case MSG_RESUME:
    case MSG_MENU:
        /*
#ifndef SUPPORT_GRAPH_ZOOM
    case MSG_ZOOM:
#endif
    */
    case MSG_ZOOM_TEXT:
    case MSG_VIDEO_OUTPUT:
    case MSG_SCANFORWARD:
    case MSG_SCANBACKWARD:
    case MSG_SLOW:        
    case MSG_INVALID:
    //case MSG_TRACK:
    case MSG_NUMBER:
    case MSG_VOCAL:
    case MSG_AUTOVOCAL:
    //case MSG_VOICECANCEL:
    case MSG_KARAOKE:
    case MSG_ROTATE:
    case MSG_FLIP:
    case MSG_JPEG_EFFECT:
    case MSG_ABSTIME:
    case MSG_RELTIME:
    case MSG_REMAIN_ABSTIME:
    case MSG_REMAIN_RELTIME:
    case MSG_TITLE_TIME:
    case MSG_REMAIN_TITLE_TIME:
    case MSG_CHAPTER_TIME:
    case MSG_REMAIN_CHAPTER_TIME:
    case MSG_KEY10:
    case MSG_PSCAN:
    case MSG_STRING:
    case MSG_VERSION:
    case MSG_ERRCODE:
    case MSG_ABNORMAL_DISC:
    case MSG_SELECTIONLIST: 
#ifdef  SUPPORT_PROGRAM_PLAY_INFO
	case MSG_PROGRAM_PLAY:
#endif //SUPPORT_PROGRAM_PLAY_INFO
	case MSG_INTRO:
	case MSG_FULLSCREEN:
    case MSG_NO_PLAYABLE_FILE:
    case MSG_BROWSE_MODE:
        return OSD_ND_UPPER_RIGHT_REGION;
    //The bottom message
    case MSG_AUDIO:
    case MSG_VOLUME:
    case MSG_MUTE:
    case MSG_KEY:
    case MSG_ECHO:
    case MSG_SURROUND:
    case MSG_3D:
    case MSG_ANGLE_CTL:
    case MSG_SOUND_EFFECT:
    case MSG_AST_CTL:
    case MSG_SPST_CTL:
	case MSG_AUDIO_OUTPUT_CTL:
    case MSG_PROGRESS: //CoCo1.10, support progress bar
        return OSD_ND_BOTTOM_REGION;
#ifdef SUPPORT_GRAPH_ZOOM
    case MSG_ZOOM:
        return OSD_ND_RIGHT_BOTTOM_REGION;
#endif
    default:
        return OSD_ND_UNKNOWN_REGION;
    }   
}

//***************************************************************************
//  Function    :   OSDND_Update
//  Abstract    :   This fnction is called by OSD to show normal upper message
//              :   or normal bottom message.
//  Arguments   :   bMessage: the message.
//              :   wParam: The parameter for the message. 
//  Return		:   TRUE: The message has been showed.
//              :   FALSE: The message is not processed by normal display.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE OSDND_Update(BYTE bMessage, WORD wParam)
{
    _bOSDNDMsg = bMessage;
    _wOSDNDInfo = wParam;    

    if (OSDND_GetMessagePos(bMessage) == OSD_ND_UPPER_RIGHT_REGION)
    {
        _OSDND_ShowUpperRightMessage();
    }
    else if (OSDND_GetMessagePos(bMessage) == OSD_ND_BOTTOM_REGION)
    {
        _OSDND_ShowBottomMessage();
    }    
    else
    {
        switch (bMessage)
        {
        case MSG_DEFAULT_STATE:
            _OSDND_DefaultState();
            break;
        case MSG_INITIAL_PALETTE:        
            _OSDND_InitializeGDI();
            break;
#ifdef SUPPORT_GRAPH_ZOOM
        case MSG_ZOOM:
            _OSDND_ShowZoom();
            break;
#endif
        default:            
            return FALSE;
        }
    }

    GDI_ActivateRegion(0);

    return TRUE;
}

//***************************************************************************
//  Function    :   _OSDND_ShowUpperRightMessage
//  Abstract    :   Show upper-right message.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowUpperRightMessage(void)
{
    switch (_bOSDNDMsg)
    {
    case MSG_DISCTYPE:
        if (__wDiscType == NO_DISC_IN_DRIVE)
        {
            // wyc1.21-909, use new define.
            if (__bChooseMedia != MEDIA_SELECT_DVD) //0==>CDROM/DVDROM, 1==>USB
            {
                __pOSDUTLOutputString = aNDNoUSBDevice;
            }
            else
            {
                __pOSDUTLOutputString = aNDNoDisc;
            }
        }
        else
        {
#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
            if (__bMMPlayMediaDirectlyMode == TRUE)
            {
                return;
            }
            else
#endif //#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
            {
                _OSDND_ShowDiscType();
            }
        }
        break;
    case MSG_WAITING:
        _OSDND_ShowWaitingState();
        break;
    case MSG_OPEN:
    case MSG_NEXT:
    case MSG_PREV:
    case MSG_RETURN:
    case MSG_PLAYING:
    case MSG_STOP:
    case MSG_PAUSE:
    case MSG_FAST:
    case MSG_STEP:
    case MSG_FREEZE:
    case MSG_REPEAT:
    case MSG_REPEATAB:
    case MSG_RANDOM:
    case MSG_PBC:
    case MSG_TVMODE:
    case MSG_RESUME:
    case MSG_MENU:
        /*
#ifndef SUPPORT_GRAPH_ZOOM
    case MSG_ZOOM:
#endif
    */
    case MSG_ZOOM_TEXT:
    case MSG_VIDEO_OUTPUT:
    case MSG_VOCAL:
    case MSG_AUTOVOCAL:
    //case MSG_VOICECANCEL:
    case MSG_KARAOKE: 
    case MSG_INVALID:
    case MSG_SCANFORWARD:
    case MSG_SCANBACKWARD:
    case MSG_SLOW:
    case MSG_JPEG_EFFECT:
    case MSG_ROTATE:
    case MSG_FLIP:
    case MSG_PSCAN:
	case MSG_INTRO:
        _OSDND_OutputKeyString();
        break; 
    case MSG_ABNORMAL_DISC:
        _OSDND_OutputErrorDisc();
        break;
    case MSG_KEY10:
        _OSDND_Show10Plus();
        break;  
    //case MSG_TRACK:
    case MSG_NUMBER:
        OSDUTL_PrepareOutputString();
        OSDUTL_OutputNumFromIndex(_wOSDNDInfo);
        break;
    case MSG_SELECTIONLIST:
        _OSDND_ShowSelection();
        break;
    //show time
    case MSG_RELTIME:
    case MSG_REMAIN_ABSTIME:
    case MSG_REMAIN_RELTIME:
    case MSG_TITLE_TIME:
    case MSG_REMAIN_TITLE_TIME:
    case MSG_CHAPTER_TIME:
    case MSG_REMAIN_CHAPTER_TIME:
        _OSDND_ShowTime();
        break;
    case MSG_STRING:
        _OSDND_ShowString();
        break;
    case MSG_VERSION:        // will show "VER xxxxx"
    case MSG_ERRCODE:        // will show "ERR xxx"
        _OSDND_ShowDebugNum();
        break;
#ifdef  SUPPORT_PROGRAM_PLAY_INFO
	case MSG_PROGRAM_PLAY: 
		__pOSDUTLOutputString = aNDProgramPlay;
		break;
#endif //SUPPORT_PROGRAM_PLAY_INFO
        /*
#ifdef SUPPORT_GRAPH_ZOOM
    case MSG_ZOOM:
        _OSDND_ShowZoom();
        return;
#endif
        */
    case MSG_NO_PLAYABLE_FILE:
        __pOSDUTLOutputString = aNDNoPlayableFile;
        break;
    case MSG_BROWSE_MODE:
#ifdef SUPPORT_PHOTO_BROWSE_MODE
        __pOSDUTLOutputString = aNDBrowseMode;
#endif //SUPPORT_PHOTO_BROWSE_MODE
        break;
    default:
        return;
    }

    _OSDND_OutputUpString();
}

//***************************************************************************
//  Function    :   _OSDND_OutputUpString
//  Abstract    :   This function will update the horizontal position to output the string.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_OutputUpString(void)
{
    WORD wBackupStringWidth;

    wBackupStringWidth = __OSDNDUpMsgAttr.wX;

    __OSDNDUpMsgAttr.wX -= GDI_GetStringWidth_909(__pOSDUTLOutputString);
    GDI_DrawString_909(0, &__OSDNDUpMsgAttr, __pOSDUTLOutputString);

    __OSDNDUpMsgAttr.wX = wBackupStringWidth; //recover the horizontal position.
}

//***************************************************************************
//  Function    :   _OSDND_OutputBottomString
//  Abstract    :   
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_OutputBottomString(void)
{
    GDI_DrawString_909(0, &__OSDNDBottomMsgAttr, __pOSDUTLOutputString);
}

//***************************************************************************
//  Function    :   _OSDND_ShowBottomMessage
//  Abstract    :   
//  Arguments   :   bMessage: the message.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowBottomMessage(void)
{
    switch (_bOSDNDMsg)
    {
    case MSG_AUDIO:
        _OSDND_ShowAudio();
        break;
    case MSG_VOLUME:
        _OSDND_ShowVolume();
        return;
    case MSG_MUTE:
        __OSDNDTempStringAttr.bTextColor = __OSDNDBottomMsgAttr.bTextColor;
        __OSDNDTempStringAttr.bShadow1 = __OSDNDBottomMsgAttr.bShadow1;
        __OSDNDTempStringAttr.bShadow2 = __OSDNDBottomMsgAttr.bShadow2;
        if (_wOSDNDInfo)
        {
            __OSDNDBottomMsgAttr.bTextColor = OSDND_PALETTE_ENTRY_MUTE_ICON_TEXT_FOREGROUND;
            __OSDNDBottomMsgAttr.bShadow1 = OSDND_PALETTE_ENTRY_MUTE_ICON_SHADOW_1;
            __OSDNDBottomMsgAttr.bShadow2 = OSDND_PALETTE_ENTRY_MUTE_ICON_SHADOW_2;
            __pOSDUTLOutputString = aNDMuteIcon;
        }
        else
        {
            __OSDNDBottomMsgAttr.bTextColor = OSDND_PALETTE_ENTRY_MUTEOFF_ICON_TEXT_FOREGROUND;
            __OSDNDBottomMsgAttr.bShadow1 = OSDND_PALETTE_ENTRY_MUTEOFF_ICON_SHADOW_1;
            __OSDNDBottomMsgAttr.bShadow2 = OSDND_PALETTE_ENTRY_MUTEOFF_ICON_SHADOW_2;
            __pOSDUTLOutputString = aNDMuteOffIcon;
        }
        break;
	case MSG_AUDIO_OUTPUT_CTL:
		_OSDND_ShowAudioOutput();
		break;
    case MSG_KEY:
        _OSDND_ShowAudioKeyControl();
        return;
    case MSG_ECHO:
        _OSDND_ShowEcho();
        return;
    case MSG_SURROUND:
        _OSDND_ShowSurround();
        break;
    case MSG_3D:
        _OSDND_Show3D();
        break;
    case MSG_ANGLE_CTL:
        if (__SetupInfo.bAngleMarkDisplay == SETUP_CUSTOM_ANGLE_MARK_ON)
        {
            _OSDND_ShowAngle();
        }
        else
            return;
        break;    
    case MSG_AST_CTL:
        _OSDND_ShowAST();
        break;
    case MSG_SPST_CTL:
        if (__bSPSTNs) // exist SP
        {
            _OSDND_ShowSPST();
        }
        else
            return;
        break;
    case MSG_PROGRESS: //CoCo1.10, support progress bar
        _OSDND_ShowProgressBar();
        return;
    default:
        break;
    }

    //GDI_DrawString_909(0, &__OSDNDBottomMsgAttr, __pOSDUTLOutputString);
    _OSDND_OutputBottomString();

    if (_bOSDNDMsg == MSG_MUTE)
    {
        //Set to default state to prevent the string attributes are modified by icon.
        __OSDNDBottomMsgAttr.bShadow1 = __OSDNDTempStringAttr.bShadow1;
        __OSDNDBottomMsgAttr.bShadow2 = __OSDNDTempStringAttr.bShadow2;
        __OSDNDBottomMsgAttr.bTextColor =  __OSDNDTempStringAttr.bTextColor;
    }
}


//***************************************************************************
//  Function    :   OSDND_ClearRegion
//  Abstract    :   This function will clear the region.
//  Arguments   :   bRegion: The region you want to clear.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSDND_ClearRegion(BYTE bRegion)
{
#ifdef SUPPORT_STB
    if (__bChooseMedia == MEDIA_SELECT_STB)
        return;
#endif //

    switch (bRegion)
    {
    case OSD_ND_UPPER_RIGHT_REGION:
        _OSDNDRect.rect.wLeft = OSDND_UPPER_BACKGROUND_START_H;
        //_OSDNDRect.rect.wTop = OSDND_UPPER_BACKGROUND_START_V;
        //CoCo1.10, support OSD dynamic region
        _OSDNDRect.rect.wTop = __OSDNDUpMsgAttr.wY;
        _OSDNDRect.rect.wRight = OSDND_UPPER_BACKGROUND_RIGHT;
        _OSDNDRect.rect.wBottom = OSDND_UPPER_BACKGROUND_BOTTOM;
        break;
    case OSD_ND_BOTTOM_REGION:
        _OSDNDRect.rect.wLeft = OSDND_BOTTOM_BACKGROUND_START_H;
        //_OSDNDRect.rect.wTop = OSDND_BOTTOM_BACKGROUND_START_V;
        //CoCo1.10, support OSD dynamic region
        _OSDNDRect.rect.wTop = __OSDNDBottomMsgAttr.wY;
        _OSDNDRect.rect.wRight = OSDND_BOTTOM_BACKGROUND_START_RIGHT;
        _OSDNDRect.rect.wBottom = _OSDNDRect.rect.wTop+ OSDND_BOTTOM_BACKGROUND_HEIGHT; //CoCo1.20, put the normal bottom meesage to be lower in PAL mode.
        break;
    default:
        return;
    }

    _OSDNDRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;

    GDI_FillRect_909(0, &_OSDNDRect);
}

//***************************************************************************
//  Function    :   _OSDND_ShowDiscType
//  Abstract    :   This function will show disc type according to __wDiscType.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowDiscType(void)
{
    if (__wDiscType & BOOK_DVD)
    {
        __pOSDUTLOutputString = aNDDVD;
    }
    else if (__wDiscType & BOOK_VCD) // include VCD1.0/ VCD1.1/ VCD2.0/ SVCD/ CVD
    {
        // check VCD title version
        if (__wDiscType & BOOK_SVCD)
            __pOSDUTLOutputString = aNDSVCD;
        else if(__wDiscType & BOOK_CVD)
            __pOSDUTLOutputString = aNDCVD;
        else if(__wDiscType & VCD_2_0)
        {
            __pOSDUTLOutputString = aNDVCD20;
        }
        else if(__wDiscType & VCD_1_1)
        {
            __pOSDUTLOutputString = aNDVCD11;
        }
        else
            __pOSDUTLOutputString = aNDVCD10;
    }
    else if (__wDiscType & BOOK_CDDA)
    {
        //if (__bCDDA_DTS & CDDA_DISC_DTS)
        if (__bCDDA_DTS == CDDA_DTS)
            __pOSDUTLOutputString = aNDCDDTS;
        //else if (__bCDDA_DTS & CDDA_DISC_HDCD)
        else if (__bCDDA_DTS == CDDA_HDCD)
            __pOSDUTLOutputString = aNDHDCD;
        //else if (__bCDDA_DTS & CDDA_DISC_CDG)
        else if (__bCDDA_DTS == CDDA_CDG)
            __pOSDUTLOutputString = aNDCDG;
        else
            __pOSDUTLOutputString = aNDCDDA;
    }
    else if (__wDiscType & CDROM_PICTURECD) //Kodak Picture CD
    {
        __pOSDUTLOutputString = aNDPictureCD;
    }
    else if (__wDiscType & CDROM_M1)
    {
#ifdef SUPPORT_POWERON_MENU
        if (__bMMExtFilterMode & EXT_BIT_MP3)
        {
            if (__bMMExtFilterMode & EXT_BIT_JPG)
            {
                __pOSDUTLOutputString = aPOWERONIconPhotoMusic;
            }
            else
            {
                __pOSDUTLOutputString = aPOWERONIconMusic;
            }
        }
        else if (__bMMExtFilterMode & EXT_BIT_JPG)
        {
            __pOSDUTLOutputString = aPOWERONIconPhoto;
        }
        else
        {
            __pOSDUTLOutputString = aPOWERONIconMovie;
        }
#else
        // wyc0.80, this variable need to use bit resolution here. Also remove AC3 & MPG ext type.
        switch (__bMMExtFilterMode)
        {
        case EXT_BIT_MP2:
            __pOSDUTLOutputString = aNDMP2;
            break;
        case EXT_BIT_MP3:
            __pOSDUTLOutputString = aNDMP3;
            break;
        case EXT_BIT_JPG:
            __pOSDUTLOutputString = aNDJPEG;
            break;
        case EXT_BIT_DAT:
            __pOSDUTLOutputString = aNDDAT;
            break;
        case EXT_BIT_VOB:
            __pOSDUTLOutputString = aNDVOB;
            break;
        case EXT_BIT_WMA:
            __pOSDUTLOutputString = aNDWMA;
            break;
        default:
            // wyc1.21-909, use new define.
            if (__bChooseMedia != MEDIA_SELECT_DVD) //0==>CDROM/DVDROM, 1==>USB
            {
                __pOSDUTLOutputString = aNDUSB;
            }
            else
            {
                if (__bDVDROM)
                    __pOSDUTLOutputString = aNDDVDROM;
                else           
                    __pOSDUTLOutputString = aNDCDROM;
            }
            break;
        }

#endif //SUPPORT_POWERON_MENU
    }
}

//***************************************************************************
//  Function    :   _OSDND_OutputKeyString
//  Abstract    :   This function will output the string of the keys according to the message.
//  Arguments   :   bMessage: The message.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_OutputKeyString(void)
{
    switch (_bOSDNDMsg)
    {
    case MSG_OPEN:
        if (_wOSDNDInfo)
        {
            __pOSDUTLOutputString = aNDOpen;        
        }
        else
        {
            __pOSDUTLOutputString = aNDClose;        
        }
        break;
    case MSG_NEXT:
        _OSDND_ShowNext();
        break;
    case MSG_PREV:
        _OSDND_ShowPrevious();
        break;
    case MSG_RETURN:
        __pOSDUTLOutputString = aNDReturn;
        break;
    case MSG_PLAYING:
        __pOSDUTLOutputString = aNDPlay;
        break;
    case MSG_STOP:
#ifdef  DOUBLE_STOP
        if (__bCountStop==1)
        {
            __pOSDUTLOutputString = aNDPreStop;
        }
        else        
#endif  // #ifdef DOUBLE_STOP
        {
            __pOSDUTLOutputString = aNDStop;
        }
        break;
    case MSG_PAUSE:
         __pOSDUTLOutputString = aNDPause;
        break;
    case MSG_FAST:
         __pOSDUTLOutputString = aNDFast;
        break;
    case MSG_STEP:
         __pOSDUTLOutputString = aNDStep;
        break;
    case MSG_FREEZE:
         __pOSDUTLOutputString = aNDFreeze;
        break;
    case MSG_REPEAT:
        _OSDND_ShowRepeatState();
        break;
    case MSG_REPEATAB:
        _OSDND_ShowRepeatABState();
        break;
    case MSG_RANDOM:
        if (_wOSDNDInfo)
        {
            __pOSDUTLOutputString = aNDRandomOn;
        }
        else
        {
            __pOSDUTLOutputString = aNDRandomOff;
        }
        break;
    case MSG_PBC:
#ifndef NO_DISC_MODE
        _OSDND_ShowPBC();
#endif //NO_DISC_MODE
        break;
    case MSG_TVMODE:
        _OSDND_ShowTVMode();
        break;
    case MSG_RESUME:
         __pOSDUTLOutputString = aNDResume;
        break;
#ifndef NO_DISC_MODE //CoCo2.37
    case MSG_MENU:
        if (__bDomain == VTSM_DOM) // Root Menu
        {
            __pOSDUTLOutputString = aNDRootMenu;
        }
        else if (__bDomain == VMGM_DOM)  // Title Menu
        {
            __pOSDUTLOutputString = aNDTitleMenu;
        }
        break;
#endif  //#ifndef NO_DISC_MODE //CoCo2.37   
        /*
#ifndef SUPPORT_GRAPH_ZOOM
    case MSG_ZOOM:
        _OSDND_ShowZoomState();
        break;
#endif    
        */
    case MSG_ZOOM_TEXT:
        _OSDND_ShowZoomState();
        break;
    case MSG_VIDEO_OUTPUT:
        _OSDND_ShowVideoOutput();
        break;
    case MSG_VOCAL:
    case MSG_AUTOVOCAL:
        _OSDND_ShowVocal();
        break;
        /*
    case MSG_VOICECANCEL:
        // voice cancel on
        if (_wOSDNDInfo)
        {
            __pOSDUTLOutputString = aNDVoiceCancelOn;
        }
        // voice cancel off
        else
        {
            __pOSDUTLOutputString = aNDVoiceCancelOff;
        }
        break;
        */
    case MSG_KARAOKE:        
        _OSDND_ShowKaraoke();
        break;
    case MSG_INVALID:
		if (_wOSDNDInfo == OSD_ERR_DATA)
		{
			__pOSDUTLOutputString = aNDErrorData;	
		}
		else if (_wOSDNDInfo == OSD_ERR_PARENTAL)
		{
			__pOSDUTLOutputString = aNDErrorParental;
		}
		else
		{
			__pOSDUTLOutputString = aNDInvalid;
		}
        break;
    //case MSG_TRACK:
    case MSG_NUMBER:
        OSDUTL_PrepareOutputString();
        OSDUTL_OutputNumFromIndex(_wOSDNDInfo);
        break;
    case MSG_JPEG_EFFECT:
        _OSDND_ShowJPEGEffect();
        break;
    case MSG_ROTATE:
        if (_wOSDNDInfo == HALJPEG_OPERATION_RC) //rotate right 90 degree
        {
            __pOSDUTLOutputString = aNDRotateRight;
        }
        else
        {
            __pOSDUTLOutputString = aNDRotateLeft;
        }
        break;
    case MSG_FLIP:
        if (_wOSDNDInfo == HALJPEG_OPERATION_FH)
        {
            __pOSDUTLOutputString = aNDFlipHorizontally;
        }
        else //vertial
        {
            __pOSDUTLOutputString = aNDFlipVertically;
        }
        break;
    case MSG_PSCAN:
        if (__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
        {
            __pOSDUTLOutputString = aNDpscan;
        }
        else
        {
            __pOSDUTLOutputString = aNDInterlace;
        }
        break;
    case MSG_SCANFORWARD:
    case MSG_SCANBACKWARD:
    case MSG_SLOW:
        if ((__wDiscType&CDROM_M1) && (__bAttrPlay == ATTR_JPG))
        {
            _OSDND_ShowSlideTransitionTime();
        }
        else
        {
            _OSDND_ShowPlayAction();
        }
        break;
	case MSG_INTRO:
		__pOSDUTLOutputString = aNDIntro;
		break;
	case MSG_FULLSCREEN:
        if (_wOSDNDInfo)
        {
            __pOSDUTLOutputString = aNDFullScreenOn;        
        }
        else
        {
            __pOSDUTLOutputString = aNDFullScreenOff;        
        }
        break;
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowNext
//  Abstract    :   This function will show "Next" info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowNext(void)
{
    switch (_wOSDNDInfo)
    {
    case OSD_NEXT_PREV_CHAPTER:
        __pOSDUTLOutputString = aNDNextChapter;
        break;
    case OSD_NEXT_PREV_PAGE:
        __pOSDUTLOutputString = aNDNextPage;
        break;
    case OSD_NEXT_PREV_ALBUM:
        __pOSDUTLOutputString = aNDNextAlbum;
        break;
    case OSD_NEXT_PREV_TRACK:
    default:
        __pOSDUTLOutputString = aNDNextTrack;
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowPrevious
//  Abstract    :   This function will show "Previous" info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowPrevious(void)
{
    switch (_wOSDNDInfo)
    {
    case OSD_NEXT_PREV_CHAPTER:
        __pOSDUTLOutputString = aNDPrevChapter;
        break;
    case OSD_NEXT_PREV_PAGE:
        __pOSDUTLOutputString = aNDPrevPage;
        break;
    case OSD_NEXT_PREV_ALBUM:
        __pOSDUTLOutputString = aNDPrevAlbum;
        break;
    case OSD_NEXT_PREV_TRACK:
    default:
        __pOSDUTLOutputString = aNDPrevTrack;
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowRepeatState
//  Abstract    :   This function will show the state of "repeat".
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowRepeatState(void)
{
    OSDUTL_PrepareOutputString();

    OSDUTL_CopyStringFromIndex(aNDRepeatSymbol);

    switch (_wOSDNDInfo)
    {
    case REPEAT_NONE:
        OSDUTL_CopyStringFromIndex(aNDRepeatNone);
        break;
    case REPEAT_TRACK:
        OSDUTL_CopyStringFromIndex(aNDRepeatTrack);
        break;
    case REPEAT_DISC:
        //OSDUTL_CopyStringFromIndex(aNDRepeatDisc);
        OSDUTL_CopyStringFromIndex(aNDRepeatAll);
        break;
    case REPEAT_ALLDISCS:
        OSDUTL_CopyStringFromIndex(aNDRepeatAll);
        break;
    case REPEAT_TITLE:
        OSDUTL_CopyStringFromIndex(aNDRepeatTitle);
        break;
    case REPEAT_CHAPTER:
        OSDUTL_CopyStringFromIndex(aNDRepeatChapter);
        break;
    case REPEAT_FOLDER:
        OSDUTL_CopyStringFromIndex(aNDRepeatFolder);
        break;
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowRepeatABState
//  Abstract    :   This function will show the state of "Repeat A-B".
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowRepeatABState(void)
{
    switch (_wOSDNDInfo)
    {
    case REPEAT_A:
        __pOSDUTLOutputString = aNDRepeatA;
        break;    
    case REPEAT_AB:
        __pOSDUTLOutputString = aNDRepeatAB;
        break;
    default:
        __pOSDUTLOutputString = aNDRepeatOff;
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowPBC
//  Abstract    :   This function will show the state of PBC according to gcShowStatus.bPBCMode
//              :   and __wDiscType.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowPBC(void)
{
    if (__wDiscType & BOOK_DVD)
    {
        if (gcShowStatus.bPBCMode)
        {
            __pOSDUTLOutputString = aNDNavigateOn;
        }
        else
        {
            __pOSDUTLOutputString = aNDNavigateOff;
        }
    }
    else
    {
        if (gcShowStatus.bPBCMode)
        {
            __pOSDUTLOutputString = aNDPBCOn;
        }
        else
        {
            __pOSDUTLOutputString = aNDPBCOff;
        }
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowTVMode
//  Abstract    :   This function will show the TV mode according to gcShowStatus.bTVMODE.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowTVMode(void)
{
    switch (__SetupInfo.bTVSystem)
    {
    case SETUP_SCREEN_TV_SYSTEM_NTSC:
        __pOSDUTLOutputString = aNDNTSC;
        break;
    case SETUP_SCREEN_TV_SYSTEM_AUTO:
        __pOSDUTLOutputString = aNDAuto;
        break;
    case SETUP_SCREEN_TV_SYSTEM_PAL:
        __pOSDUTLOutputString = aNDPAL;
        break;
    case SETUP_SCREEN_TV_SYSTEM_PAL_M:
        __pOSDUTLOutputString = aNDPALM;
        break;
    case SETUP_SCREEN_TV_SYSTEM_PAL_N:
        __pOSDUTLOutputString = aNDPALN;
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowZoomState
//  Abstract    :   This function will show the state of the zoom.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowZoomState(void)
{
    switch (_wOSDNDInfo)
    {
    case VIEW_ZOOMX1:
        __pOSDUTLOutputString = aNDZoom1;
        break;
    case VIEW_ZOOMX2:
        __pOSDUTLOutputString = aNDZoom2;
        break;
    case VIEW_ZOOMX4:
        __pOSDUTLOutputString = aNDZoom4;
        break;
        //For JPEG
    case VIEW_ZOOM25:
        __pOSDUTLOutputString = aNDZoom25;
        break;
    case VIEW_ZOOM50:
        __pOSDUTLOutputString = aNDZoom50;
        break;
    case VIEW_ZOOM100:
        __pOSDUTLOutputString = aNDZoom100;
        break;
    case VIEW_ZOOM150:
        __pOSDUTLOutputString = aNDZoom150;
        break;
    case VIEW_ZOOM200:
        __pOSDUTLOutputString = aNDZoom200;
        break;
    case VIEW_ZOOM_LEFT:
        __pOSDUTLOutputString = aNDMoveLeft;
        break;
    case VIEW_ZOOM_RIGHT:
        __pOSDUTLOutputString = aNDMoveRight;
        break;
    case VIEW_ZOOM_UP:
        __pOSDUTLOutputString = aNDMoveUp;
        break;
    case VIEW_ZOOM_DOWN:
        __pOSDUTLOutputString = aNDMoveDown;
        break;
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowVideooutput
//  Abstract    :   This function will show video output according to SETUP's value.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowVideoOutput(void)
{
    switch (__SetupInfo.bVideoOutput)
    {
    case SETUP_SCREEN_VIDEO_OUTPUT_NORMAL:
        __pOSDUTLOutputString = aNDNormal;
        break;
    case SETUP_SCREEN_VIDEO_OUTPUT_RGB:
        __pOSDUTLOutputString = aNDRGB;
        break;
    case SETUP_SCREEN_VIDEO_OUTPUT_YUV:
        __pOSDUTLOutputString = aNDYUV;
        break;
    case SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN:
        __pOSDUTLOutputString = aNDYUVPSCAN;
        break;
    case SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN:
        __pOSDUTLOutputString = aNDRGBPSCAN;
        break;
    case SETUP_SCREEN_VIDEO_OUTPUT_VGA:
        __pOSDUTLOutputString = aNDVGA;
        break;
    default:
        break;
    }    
}

//***************************************************************************
//  Function    :   _OSDND_ShowVocal
//  Abstract    :   This function will show "vocal" info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowVocal(void)
{  
    switch (_wOSDNDInfo)
    {
    case VOCAL_ON:
        __pOSDUTLOutputString = aNDVocalOn;
        break;
    case VOCAL_LL:
        __pOSDUTLOutputString = aNDVocalLL;
        break;
    case VOCAL_RR:
        __pOSDUTLOutputString = aNDVocalRR;
        break;
    case VOCAL_OFF:
        __pOSDUTLOutputString = aNDVocalOff;
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowKaraoke
//  Abstract    :   Display Karaoke info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowKaraoke(void)
{
#ifdef KARAOKE  
    switch (_wOSDNDInfo)
    {
    case 0:
        __pOSDUTLOutputString = aNDKaraokeOff;
        break;
    case 1:
        __pOSDUTLOutputString = aNDMic1;
        break;
    case 2:
        __pOSDUTLOutputString = aNDMic2;
        break;
    case 3:
        __pOSDUTLOutputString = aNDMic3;
        break;
    case 4:
        __pOSDUTLOutputString = aNDMic4;
        break;
    case 5:
        __pOSDUTLOutputString = aNDMic5;
        break;
    case 6:
        __pOSDUTLOutputString = aNDMic6;
        break;
    case 7:
        __pOSDUTLOutputString = aNDMic7;
        break;
    case 8:
        __pOSDUTLOutputString = aNDMic8;
        break;
    default:
        break;
    }
#else
    // LLY.104, refer CHIPS define
    switch (_wOSDNDInfo)
    {
    case CHIPS_ON:
        __pOSDUTLOutputString = aNDKaraokeOn;
        break;
    case CHIPS_OFF:
        __pOSDUTLOutputString = aNDKaraokeOff;
        break;
    default:
        break;
    }
#endif
}


//***************************************************************************
//  Function    :   _OSDND_ShowJPEGEffect
//  Abstract    :   This function will show the effect of slide show for JPEG.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowJPEGEffect(void)
{
    switch (_wOSDNDInfo)
    {
    case HALJPEG_EFFECT_NONE:
        __pOSDUTLOutputString = aNDJPEGEffectNone;
        break;
    case HALJPEG_EFFECT_COVER_UP:
        __pOSDUTLOutputString = aNDJPEGEffectCoverUp;
        break;
    case HALJPEG_EFFECT_COVER_DOWN:
        __pOSDUTLOutputString = aNDJPEGEffectCoverDown;
        break;
    case HALJPEG_EFFECT_COVER_LEFT:
        __pOSDUTLOutputString = aNDJPEGEffectCoverLeft;
        break;
    case HALJPEG_EFFECT_COVER_RIGHT:
        __pOSDUTLOutputString = aNDJPEGEffectCoverRight;
        break;
    case HALJPEG_EFFECT_UNCOVER_UP:
        __pOSDUTLOutputString = aNDJPEGEffectUncoverUp;
        break;
    case HALJPEG_EFFECT_UNCOVER_DOWN:
        __pOSDUTLOutputString = aNDJPEGEffectUncoverDown;
        break;
    case HALJPEG_EFFECT_UNCOVER_LEFT:
        __pOSDUTLOutputString = aNDJPEGEffectUncoverLeft;
        break;
    case HALJPEG_EFFECT_UNCOVER_RIGHT:
        __pOSDUTLOutputString = aNDJPEGEffectUncoverRight;
        break;
    case HALJPEG_EFFECT_WIPE_UP:
        __pOSDUTLOutputString = aNDJPEGEffectWipeUp;
        break;
    case HALJPEG_EFFECT_WIPE_DOWN:
        __pOSDUTLOutputString = aNDJPEGEffectWipeDown;
        break;
    case HALJPEG_EFFECT_WIPE_LEFT:
        __pOSDUTLOutputString = aNDJPEGEffectWipeLeft;
        break;
    case HALJPEG_EFFECT_WIPE_RIGHT:
        __pOSDUTLOutputString = aNDJPEGEffectWipeRight;
        break;
    case HALJPEG_EFFECT_PUSH_UP:
        __pOSDUTLOutputString = aNDJPEGEffectPushUp;
        break;
    case HALJPEG_EFFECT_PUSH_DOWN:
        __pOSDUTLOutputString = aNDJPEGEffectPushDown;
        break;
    case HALJPEG_EFFECT_PUSH_LEFT:
        __pOSDUTLOutputString = aNDJPEGEffectPushLeft;
        break;
    case HALJPEG_EFFECT_PUSH_RIGHT:
        __pOSDUTLOutputString = aNDJPEGEffectPushRight;
        break;
    case HALJPEG_EFFECT_RANDOM:
        __pOSDUTLOutputString = aNDJPEGEffectRandom;
        break;
	default:
		break;	
    }
}


//***************************************************************************
//  Function    :   _OSDND_ShowSlideTransitionTime
//  Abstract    :   Show the time of slide transition.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowSlideTransitionTime(void)
{
    if (_bOSDNDMsg == MSG_SLOW)
        return;

    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aNDSlideTransitionTime);
    OSDUTL_OutputNumFromIndex(_wOSDNDInfo);
}

//***************************************************************************
//  Function    :   _OSDND_ShowPlayAction
//  Abstract    :   show play action.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowPlayAction(void)
{    
    OSDUTL_PrepareOutputString();
    
    switch (_bOSDNDMsg)
    {
    case MSG_SCANFORWARD:
        OSDUTL_CopyStringFromIndex(aNDScanF);
        break;
    case MSG_SCANBACKWARD:
        OSDUTL_CopyStringFromIndex(aNDScanB);
        break;
    case MSG_SLOW:
        OSDUTL_CopyStringFromIndex(aNDSlow);
        break;
    default:
        break;
    }

    OSDUTL_CopyStringFromIndex(aNDSpeed);

    /*
    if (_bOSDNDMsg == MSG_SLOW)
    {
#ifdef SUPPORT_LANGUAGE_STUDYING //kevin1.10         
        extern  WORD    __wSlowSpeed;
        if (__wSlowSpeed == 2)
            OSDUTL_OutputNumFromIndex(75);     
        else //4            
            OSDUTL_OutputNumFromIndex(50); 
#else 
        OSDUTL_OutputNumFromIndex(1);
        OSDUTL_CopyStringFromIndex(aNDSlash);        
#endif                
    }

#ifdef SUPPORT_LANGUAGE_STUDYING  //kevin1.10    
    if (_bOSDNDMsg == MSG_SLOW) 
        OSDUTL_CopyStringFromIndex(aNDPercent);   
    else        
#endif     
    OSDUTL_OutputNumFromIndex(_bSpeedFactor);           

    */

    if (_bOSDNDMsg == MSG_SLOW)
    {
#if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)
        extern  WORD    __wSlowSpeed;
        extern  BYTE    __bLangStudyFlag;

        // wyc2.17-909S, show different OSD in SLOW or language study mode.
        if (__bLangStudyFlag == FALSE) //normal slow
        {
            OSDUTL_OutputNumFromIndex(1);
            OSDUTL_CopyStringFromIndex(aNDSlash);        
        }
        else //language studying.
        {
            if (__wSlowSpeed == 2)
            {
                OSDUTL_OutputNumFromIndex(75);     
            }
            else //4            
            {
                OSDUTL_OutputNumFromIndex(50);
            }
            
            OSDUTL_CopyStringFromIndex(aNDPercent);

            return;
        }
#else
            OSDUTL_OutputNumFromIndex(1);
            OSDUTL_CopyStringFromIndex(aNDSlash);        
#endif //
    }

    OSDUTL_OutputNumFromIndex(_bSpeedFactor);
}

//***************************************************************************
//  Function    :   _OSDND_OutputErrorDisc
//  Abstract    :   This function will Output error disc info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_OutputErrorDisc(void)
{
#ifdef NO_DISC_MODE
    if(__bDiscInfo & DISCINFO_UNRECOGNIZE)
    {
        // wyc1.21-909
        if (__bChooseMedia != MEDIA_SELECT_DVD) //0==>CDROM/DVDROM, 1==>USB
        {
            __pOSDUTLOutputString = aNDNoPlayableFile;
        }
    }
    else
    {
        // wyc1.21-909
        if (__bChooseMedia != MEDIA_SELECT_DVD) //0==>CDROM/DVDROM, 1==>USB
        {
            __pOSDUTLOutputString = aNDNoUSBDevice;
        }
    }
#else
    if (__bDiscInfo & DISCINFO_ENCRYPT)
    {
        __pOSDUTLOutputString = aNDEncrypt;
    }
    else if(__bDiscInfo & DISCINFO_REGION_ERROR)
    {
        __pOSDUTLOutputString = aNDErrRegionCode;
    }
    else if(__bDiscInfo & DISCINFO_UNRECOGNIZE)
    {
        // wyc1.21-909
        if (__bChooseMedia != MEDIA_SELECT_DVD) //0==>CDROM/DVDROM, 1==>USB
		{
			__pOSDUTLOutputString = aNDNoPlayableFile;
		}
		else
		{
			__pOSDUTLOutputString = aNDWrongDisc;
		}
	}
    else if(__bDiscInfo & DISCINFO_DAMAGED)
    {
        __pOSDUTLOutputString = aNDDamageDisc;
    }
    else
    {
        // wyc1.21-909
        if (__bChooseMedia != MEDIA_SELECT_DVD) //0==>CDROM/DVDROM, 1==>USB
		{
			__pOSDUTLOutputString = aNDNoUSBDevice;
		}
		else
		{
			__pOSDUTLOutputString = aNDNoDisc;
		}
    }
#endif //NO_DISC_MODE
}

//***************************************************************************
//  Function    :   _OSDND_Show10Plus
//  Abstract    :   This function will show the info. for 10+.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_Show10Plus(void)
{
    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex((WORD)(_wOSDNDInfo/10));
    OSDUTL_CopyStringFromIndex(aNDDash);
}

//***************************************************************************
//  Function    :   _OSDND_ShowSelection
//  Abstract    :   Show "SELECT XX-XX" on screen.
//  Arguments   :   wParam, HIBYTE means the 1st number
//                          LOBYTE means the 2nd number
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowSelection(void)
{
    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aNDSelect);
    OSDUTL_OutputNumFromIndex(LOBYTE(_wOSDNDInfo));
    OSDUTL_CopyStringFromIndex(aNDDash);
    OSDUTL_OutputNumFromIndex(HIBYTE(_wOSDNDInfo));
    OSDUTL_CopyStringFromIndex(aNDSpace);
}

//***************************************************************************
//  Function    :   _OSDND_ShowTime
//  Abstract    :   This function will show the time.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowTime(void)
{
    OSDUTL_PrepareOutputString();

    switch (_bOSDNDMsg)
    {
        // absolute time/ absolute remain time, ie disc unit
    case MSG_ABSTIME:
    case MSG_REMAIN_ABSTIME:
        OSDUTL_CopyStringFromIndex(aNDDisc);
        break;
        // relative playing time/ relative remain time, ie. track unit
    case MSG_RELTIME:
    case MSG_REMAIN_RELTIME:
        OSDUTL_CopyStringFromIndex(aNDTrack);
        break;
        // title playing time/ title remain time
    case MSG_TITLE_TIME:
    case MSG_REMAIN_TITLE_TIME:
        OSDUTL_CopyStringFromIndex(aNDTitle);
        break;
        // chapter playing time/ chapter remain time
    case MSG_CHAPTER_TIME:
    case MSG_REMAIN_CHAPTER_TIME:
        OSDUTL_CopyStringFromIndex(aNDChapter);
        break;
    }  

    switch (_bOSDNDMsg)
    {
    case    MSG_REMAIN_ABSTIME:
    case    MSG_REMAIN_RELTIME:
    case    MSG_REMAIN_TITLE_TIME:
    case    MSG_REMAIN_CHAPTER_TIME:
        OSDUTL_CopyStringFromIndex(aNDRemain);
        break;
    case    MSG_ABSTIME:
    case    MSG_RELTIME:
    case    MSG_TITLE_TIME:
    case    MSG_CHAPTER_TIME:
        OSDUTL_CopyStringFromIndex(aNDPlus);
        break;
    default:
        break;
    }

    OSDUTL_ShowHourMinSec();
}

//***************************************************************************
//  Function    :   _OSDND_ShowDebugNum
//  Abstract    :   This function will show the number for debug. (VER xxx, ERR xxx)
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowDebugNum(void)
{
    OSDUTL_PrepareOutputString();

    if (_bOSDNDMsg == MSG_VERSION)
        OSDUTL_CopyStringFromIndex(aNDVersion);
    else //MSG_ERROR
        OSDUTL_CopyStringFromIndex(aNDError);

    OSDUTL_OutputNumFromIndex(_wOSDNDInfo);
}

//***************************************************************************
//  Function    :   _OSDND_ShowAudio
//  Abstract    :   Call this funciton to show audio info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowAudio(void)
{
	switch (__bModeChannel)
    {
    case AUDIO_STEREO:
        __pOSDUTLOutputString = aNDLR;
        break;
    case AUDIO_LL:
        __pOSDUTLOutputString = aNDLL;
        break;
    case AUDIO_RR:
        __pOSDUTLOutputString = aNDRR;
        break;
#ifdef DOLBY_CERTIFICATION
    case AUDIO_MIXED:
        __pOSDUTLOutputString = aNDMixed;
        break;
#endif
    case AUDIO_3D:
        __pOSDUTLOutputString = aND3DAudio;
        break;
    }  
}

//***************************************************************************
//  Function    :   _OSDND_ShowVolume
//  Abstract    :   Call this function to show volume bar.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowVolume(void)
{
    __pOSDUTLOutputString = aNDVolume;

    _wOSDNDHPos = __OSDNDBottomMsgAttr.wX;

    //GDI_DrawString_909(0, &__OSDNDBottomMsgAttr, __pOSDUTLOutputString);
    _OSDND_OutputBottomString();
    _wOSDNDHPos += GDI_GetStringWidth_909(__pOSDUTLOutputString);

    //show volume bar
    if (__bOSDClearNDBRegion)
    {
        _OSDNDRect.rect.wLeft = _wOSDNDHPos;
        _OSDNDRect.rect.wRight = _wOSDNDHPos+OSDND_DISPLAY_VOLUME_BAR_WIDTH+OSDND_DISTANCE_FONT_VOLUME_BAR*2;
        _OSDNDRect.rect.wTop = __OSDNDBottomMsgAttr.wY;
        _OSDNDRect.rect.wBottom = __OSDNDBottomMsgAttr.wY+OSDND_FONT_HEIGHT-1;
        _OSDNDRect.bColor = __OSDNDBottomMsgAttr.bBackground;
        
        GDI_FillRect_909(0, &_OSDNDRect);
    }

    _wOSDNDHPos += OSDND_DISTANCE_FONT_VOLUME_BAR;

    _bOSDNDTemp = LOBYTE(_wOSDNDInfo);

    _OSDNDVolumeBar.bBgColor = __OSDNDBottomMsgAttr.bBackground;
    _OSDNDVolumeBar.bBarColor = OSDND_PALETTE_ENTRY_BAR_FG;
    _OSDNDVolumeBar.bFrameColor = OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING;  //Alan2.36, Mantis #2241
    _OSDNDVolumeBar.bOriention = ORIENTION_HORIZONTAL;
    _OSDNDVolumeBar.rect.wLeft = _wOSDNDHPos;
    _OSDNDVolumeBar.rect.wRight = _wOSDNDHPos + OSDND_DISPLAY_VOLUME_BAR_WIDTH;
    _OSDNDVolumeBar.rect.wTop = __OSDNDBottomMsgAttr.wY + (OSDND_FONT_HEIGHT-OSDND_DISPLAY_VOLUME_BAR_HEIGHT)/2;
    _OSDNDVolumeBar.rect.wBottom = _OSDNDVolumeBar.rect.wTop + OSDND_DISPLAY_VOLUME_BAR_HEIGHT;
    _OSDNDVolumeBar.wVolume = (WORD)_bOSDNDTemp*100/OSDND_DISPLAY_VOLUME_BAR_RANGE; // percent

    GDI_DrawVolumeBar_909(0, &_OSDNDVolumeBar);

    OSDUTL_PrepareOutputString();

    if (_bOSDNDTemp < 10)
    {
        OSDUTL_OutputNumFromIndex(0);
    }

    OSDUTL_OutputNumFromIndex(_bOSDNDTemp);
        
    _wOSDNDHPos += OSDND_DISPLAY_VOLUME_BAR_WIDTH+OSDND_DISTANCE_FONT_VOLUME_BAR;

    _wOSDNDTemp = __OSDNDBottomMsgAttr.wX;
    __OSDNDBottomMsgAttr.wX = _wOSDNDHPos;

    //GDI_DrawString_909(0, &__OSDNDBottomMsgAttr, __pOSDUTLOutputString);
    _OSDND_OutputBottomString();

    __OSDNDBottomMsgAttr.wX = _wOSDNDTemp;
}

//***************************************************************************
//  Function    :   _OSDND_ShowAudioOutput
//  Abstract    :   Show auio output.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowAudioOutput(void)
{
	switch (__SetupInfo.bAudioAnalogOut)
	{
	case SETUP_AUDIO_ANALOG_OUT_51CH:
		__pOSDUTLOutputString = aND51Channel;
		break;
	case SETUP_AUDIO_ANALOG_OUT_OFF:
		__pOSDUTLOutputString = aNDAnalogOff;
		break;
	case SETUP_AUDIO_ANALOG_OUT_2CH:
	default:		
		__pOSDUTLOutputString = aND2Channel;
		break;
	}		
}

//***************************************************************************
//  Function    :   _OSDND_ShowAudioKeyControl
//  Abstract    :   Call this function to show audio key bar.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowAudioKeyControl(void)
{
    __pOSDUTLOutputString = aNDTone;

    _wOSDNDHPos = __OSDNDBottomMsgAttr.wX;
    //GDI_DrawString_909(0, &__OSDNDBottomMsgAttr, __pOSDUTLOutputString);
    _OSDND_OutputBottomString();
    _wOSDNDHPos += GDI_GetStringWidth_909(__pOSDUTLOutputString);
    
    if (__bOSDClearNDBRegion)
    {
        _OSDNDRect.rect.wLeft = _wOSDNDHPos;
        _OSDNDRect.rect.wRight = _wOSDNDHPos+OSDND_DISPLAY_AUDIO_KEY_BAR_WIDTH+OSDND_DISTANCE_FONT_AUDIO_KEY_BAR*2;
        _OSDNDRect.rect.wTop = __OSDNDBottomMsgAttr.wY;
        _OSDNDRect.rect.wBottom = __OSDNDBottomMsgAttr.wY + +OSDND_FONT_HEIGHT-1;
        _OSDNDRect.bColor = __OSDNDBottomMsgAttr.bBackground;
        
        GDI_FillRect_909(0, &_OSDNDRect);
    }

    _wOSDNDHPos += OSDND_DISTANCE_FONT_AUDIO_KEY_BAR;

    _OSDNDVolumeBar.bBgColor = __OSDNDBottomMsgAttr.bBackground;
    _OSDNDVolumeBar.bBarColor = OSDND_PALETTE_ENTRY_BAR_FG;
    _OSDNDVolumeBar.bFrameColor = OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING;  //Alan2.36, Mantis #2241
    _OSDNDVolumeBar.bOriention = ORIENTION_HORIZONTAL;
    _OSDNDVolumeBar.rect.wLeft = _wOSDNDHPos;
    _OSDNDVolumeBar.rect.wRight = _wOSDNDHPos + OSDND_DISPLAY_AUDIO_KEY_BAR_WIDTH;
    _OSDNDVolumeBar.rect.wTop = __OSDNDBottomMsgAttr.wY + (OSDND_FONT_HEIGHT-OSDND_DISPLAY_AUDIO_KEY_BAR_HEIGHT)/2;
    _OSDNDVolumeBar.rect.wBottom = _OSDNDVolumeBar.rect.wTop + OSDND_DISPLAY_AUDIO_KEY_BAR_HEIGHT;
    _OSDNDVolumeBar.wVolume = (WORD)_wOSDNDInfo*100/OSDND_DISPLAY_AUDIO_KEY_BAR_RANGE; // percent

    GDI_DrawVolumeBar_909(0, &_OSDNDVolumeBar);

    _wOSDNDHPos +=OSDND_DISPLAY_AUDIO_KEY_BAR_WIDTH+OSDND_DISTANCE_FONT_AUDIO_KEY_BAR;

    _wOSDNDTemp = __OSDNDBottomMsgAttr.wX;
    __OSDNDBottomMsgAttr.wX = _wOSDNDHPos;

    OSDUTL_PrepareOutputString();
    //show "+" or "-"
    if (_wOSDNDInfo > NORMALKEY_OFFSET)
    {
        OSDUTL_CopyStringFromIndex(aNDPlus);
        _bOSDNDTemp = _wOSDNDInfo - NORMALKEY_OFFSET;
    }
    else if (_wOSDNDInfo < NORMALKEY_OFFSET)
    {
        OSDUTL_CopyStringFromIndex(aNDDash);
        _bOSDNDTemp = NORMALKEY_OFFSET - _wOSDNDInfo;
    }
    else
    {
        _bOSDNDTemp = 0;
        //OSDUTL_CopyStringFromIndex(aNDSpace);
		OSDUTL_CopyStringFromIndex(aNDPlus);
		//Clear the region.
		_OSDNDRect.rect.wLeft = _wOSDNDHPos;
        _OSDNDRect.rect.wRight = _OSDNDRect.rect.wLeft+GDI_GetStringWidth(__pOSDUTLOutputString)-1;
        _OSDNDRect.rect.wTop = __OSDNDBottomMsgAttr.wY;
        _OSDNDRect.rect.wBottom = __OSDNDBottomMsgAttr.wY + +OSDND_FONT_HEIGHT-1;
        _OSDNDRect.bColor = __OSDNDBottomMsgAttr.bBackground;
        
        GDI_FillRect_909(0, &_OSDNDRect);

		__OSDNDBottomMsgAttr.wX += GDI_GetStringWidth(__pOSDUTLOutputString);

		OSDUTL_PrepareOutputString();
    }
    
    //show value
    OSDUTL_OutputNumFromIndex(_bOSDNDTemp);
    //GDI_DrawString_909(0, &__OSDNDBottomMsgAttr, __pOSDUTLOutputString);
    _OSDND_OutputBottomString();

    __OSDNDBottomMsgAttr.wX = _wOSDNDTemp; //restore the 
}

//***************************************************************************
//  Function    :   _OSDND_ShowEcho
//  Abstract    :   Call this function to show Echo bar.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowEcho(void)
{
    __pOSDUTLOutputString = aNDEcho;
    
    _wOSDNDHPos = __OSDNDBottomMsgAttr.wX;
    //GDI_DrawString_909(0, &__OSDNDBottomMsgAttr, __pOSDUTLOutputString);
    _OSDND_OutputBottomString();
    _wOSDNDHPos += GDI_GetStringWidth_909(__pOSDUTLOutputString);
 
    //show bolume bar
    if (__bOSDClearNDBRegion)
    {
        _OSDNDRect.rect.wLeft = _wOSDNDHPos;
        _OSDNDRect.rect.wRight = _wOSDNDHPos+OSDND_DISPLAY_ECHO_BAR_WIDTH+OSDND_DISTANCE_FONT_ECHO_BAR*2;
        _OSDNDRect.rect.wTop = __OSDNDBottomMsgAttr.wY;
        _OSDNDRect.rect.wBottom = __OSDNDBottomMsgAttr.wY + +OSDND_FONT_HEIGHT-1;
        _OSDNDRect.bColor = __OSDNDBottomMsgAttr.bBackground;
        
        GDI_FillRect_909(0, &_OSDNDRect);
    }

    _wOSDNDHPos += OSDND_DISTANCE_FONT_ECHO_BAR;

    _OSDNDVolumeBar.bBgColor = __OSDNDBottomMsgAttr.bBackground;
    _OSDNDVolumeBar.bBarColor = OSDND_PALETTE_ENTRY_BAR_FG;
    _OSDNDVolumeBar.bFrameColor = OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING;  //Alan2.36, Mantis #2241
    _OSDNDVolumeBar.bOriention = ORIENTION_HORIZONTAL;
    _OSDNDVolumeBar.rect.wLeft = _wOSDNDHPos;
    _OSDNDVolumeBar.rect.wRight = _wOSDNDHPos+OSDND_DISPLAY_ECHO_BAR_WIDTH;
    _OSDNDVolumeBar.rect.wTop = __OSDNDBottomMsgAttr.wY + (OSDND_FONT_HEIGHT-OSDND_DISPLAY_ECHO_BAR_HEIGHT)/2;
    _OSDNDVolumeBar.rect.wBottom = _OSDNDVolumeBar.rect.wTop + OSDND_DISPLAY_ECHO_BAR_HEIGHT;
    _OSDNDVolumeBar.wVolume =  (WORD)_wOSDNDInfo*100/OSDND_DISPLAY_ECHO_BAR_RANGE; // percent

    GDI_DrawVolumeBar_909(0, &_OSDNDVolumeBar);

    __pOSDUTLOutputString = __wOSDUTLStringBuf;
    _wOSDNDTemp = __OSDNDBottomMsgAttr.wX;

    _wOSDNDHPos += OSDND_DISPLAY_ECHO_BAR_WIDTH+OSDND_DISTANCE_FONT_ECHO_BAR;
    __OSDNDBottomMsgAttr.wX = _wOSDNDHPos;

    //show value
    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex(_wOSDNDInfo);
    //GDI_DrawString_909(0, &__OSDNDBottomMsgAttr, __pOSDUTLOutputString);
    _OSDND_OutputBottomString();

    __OSDNDBottomMsgAttr.wX = _wOSDNDTemp; //restore the 
}

//***************************************************************************
//  Function    :   _OSDND_Surround
//  Abstract    :   Call this function to show surround info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowSurround(void)
{
	switch (__bSurround)
    {
    case CHIPS_SURROUND_NONE:
        __pOSDUTLOutputString = aNDNoSurround;
        break;
    case CHIPS_SURROUND_LIVE:
        __pOSDUTLOutputString = aNDLiveSurround;
        break;
    case CHIPS_SURROUND_THEATER:
        __pOSDUTLOutputString = aNDTheaterSurround;
        break;
    case CHIPS_SURROUND_STEREO:
        __pOSDUTLOutputString = aNDStereoSurround;
        break;
    case CHIPS_SURROUND_MOVIE:
        __pOSDUTLOutputString = aNDMovieSurround;
        break;
    case CHIPS_SURROUND_KARAOKE:
        __pOSDUTLOutputString = aNDKaraSurround;
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_Show3D
//  Abstract    :   Call this function to show the 3D info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_Show3D(void)
{
#ifdef SUPPORT_3D_FEATURE
    switch (_wOSDNDInfo)
    {
    case 1:
        __pOSDUTLOutputString = aND3D1;
        break;
    case 2:
        __pOSDUTLOutputString = aND3D2;
        break;
    case 3:
        __pOSDUTLOutputString = aND3D3;
        break;
    case 4:
        __pOSDUTLOutputString = aND3D4;
        break;
    case 5:
        __pOSDUTLOutputString = aND3D5;
        break;
    case 6:
        __pOSDUTLOutputString = aND3D6;
        break;
    }
#endif
}

//***************************************************************************
//  Function    :   _OSDND_ShowAngle
//  Abstract    :   Call this function to show the angle info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowAngle(void)
{
    if (__bAGL_Ns)
    {
        OSDUTL_PrepareOutputString();
        OSDUTL_CopyStringFromIndex(aNDAngle);        
        OSDUTL_OutputNumFromIndex(__bAGLN);
        OSDUTL_CopyStringFromIndex(aNDSlash);
        OSDUTL_OutputNumFromIndex(__bAGL_Ns);
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowAST
//  Abstract    :   Show AST and audio output info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowAST(void)
{
    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aNDLanguage);
    
    _OSDND_OutputAudioType(__bAudioType);

#ifndef NO_DISC_MODE //CoCo2.37
    if (__wDiscType & BOOK_DVD)
    {
        OSDUTL_CopyStringFromIndex(aNDAudioDistance);
        
        _OSDDND_OutputAudioChannel(__bACHN);
    }
#endif //#ifndef NO_DISC_MODE //CoCo2.37

    OSDUTL_CopyStringFromIndex(aNDAudioDistance);

    if (__bASTNs) // AST exist
    {
        if (!(__wDiscType & BOOK_2X)) //SVCD and CVD
        {
            _OSDND_CodeToLanguage(__wA_LCD);
            OSDUTL_CopyStringFromIndex(aNDAudioDistance);
        }

        OSDUTL_OutputNumFromIndex((WORD)(__bASTNO+1));
        OSDUTL_CopyStringFromIndex(aNDSlash); 

        OSDUTL_OutputNumFromIndex(__bASTNs);
    }
    else            
        OSDUTL_CopyStringFromIndex(aNDPUnknownLanguage);   
}

//***************************************************************************
//  Function    :   _OSDND_CodeToLanguage
//  Abstract    :   This function will output the country according to wCode.
//  Arguments   :   wCode: The code that you want to transform.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_CodeToLanguage(WORD wCode)
{
    switch(wCode)
    {
    case 0x6a61: // Japanese, 'ja'
        OSDUTL_CopyStringFromIndex(aNDLanguageJapanese);
        break;
    case 0x656e: // English, 'en'
        OSDUTL_CopyStringFromIndex(aNDLanguageEnglish);
        break;
    case 0x6672: // French, 'fr'
        OSDUTL_CopyStringFromIndex(aNDLanguageFrench);
        break;
    case 0x6465: // German, 'de'
        OSDUTL_CopyStringFromIndex(aNDLanguageGerman);
        break;
    case 0x7275: // Russian, 'ru'
        OSDUTL_CopyStringFromIndex(aNDLanguageRussian);
        break;
    case 0x7a68: // Chinese, 'zh'
        OSDUTL_CopyStringFromIndex(aNDLanguageChinese);
        break;
    case 0x6b6f: // Korean, 'ko'
        OSDUTL_CopyStringFromIndex(aNDLanguageKorean);
        break;
    case 0x6869: // Hindi, 'hi'
        OSDUTL_CopyStringFromIndex(aNDLanguageHindi);
        break;
    case 0x7468: // Thai, 'th'
        OSDUTL_CopyStringFromIndex(aNDLanguageThai);
        break;
    case 0x6974: // Italian, 'it'
        OSDUTL_CopyStringFromIndex(aNDLanguageItalian);
        break;
    case 0x6573: // Spanish, 'es'
        OSDUTL_CopyStringFromIndex(aNDLanguageSpanish);
        break;
    case 0x7074: // Portuguese, 'pt'
        OSDUTL_CopyStringFromIndex(aNDLanguagePortuguese);
        break;
    case 0x6e6c: // Dutch, 'nl'
        OSDUTL_CopyStringFromIndex(aNDLanguageDutch);
        break;
    case 0x656c: // Greek, 'el'
        OSDUTL_CopyStringFromIndex(aNDLanguageGreek);
        break;
    case 0x6c61: // Latin, 'la'
        OSDUTL_CopyStringFromIndex(aNDLanguageLatin);
        break;
    case 0x6e6f: // Norwegian, 'no'
        OSDUTL_CopyStringFromIndex(aNDLanguageNorwegian);
        break;
    case 0x6461: // Danish, 'da'
        OSDUTL_CopyStringFromIndex(aNDLanguageDanish);
        break;
    case 0x6669: // Finnish, 'fi'
        OSDUTL_CopyStringFromIndex(aNDLanguageFinnish);
        break;
    case 0x7376: // Swedish, 'sv'
        OSDUTL_CopyStringFromIndex(aNDLanguageSwedish);
        break;
    case 0x6875: // Hungarian, 'hu'
        OSDUTL_CopyStringFromIndex(aNDLanguageHungarian);
        break;
    case 0x706c: // Polish, 'pl'
        OSDUTL_CopyStringFromIndex(aNDLanugagePolish);
        break;
    case 0x6973: // Icelandic, 'is'
        OSDUTL_CopyStringFromIndex(aNDLanguageIcelandic);
        break;
    case 0x6977: // Hebrew, 'iw'
        OSDUTL_CopyStringFromIndex(aNDLanguageHebrew);
        break;
    case 0x6373: // Czech, 'cs'
        OSDUTL_CopyStringFromIndex(aNDLanguageCzech);
        break;
    case 0x6172: // Arabic, 'ar'
        OSDUTL_CopyStringFromIndex(aNDLanguageArabic);
        break;
    case 0x6d73: // Malay, 'ms'
        OSDUTL_CopyStringFromIndex(aNDLanguageMalay);
        break;
    case 0x696e: // Indonesian, 'in'
        OSDUTL_CopyStringFromIndex(aNDLanguageIndonesian);
        break;
    case 0x6865: // Hebrew, 'he' **** Is it 'heb' or 'her'
        OSDUTL_CopyStringFromIndex(aNDLanguageHebrew);
        break;
    case 0x7472: // Turkish, 'tr'
        OSDUTL_CopyStringFromIndex(aNDLanguageTurkish);
        break;
    case 0x6872: // Croatian, 'hr'
        OSDUTL_CopyStringFromIndex(aNDLanguageCroatian);
        break;
    case 0x6267: // Bulgarian, 'bg'
        OSDUTL_CopyStringFromIndex(aNDLanguageBulgarian);
        break;
    default:
        OSDUTL_CopyStringFromIndex(aNDLanguageOthers);
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowSPST
//  Abstract    :   Show SPST info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowSPST(void)
{
    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aNDSubtitle);

    // spst is on, ie. __bSP_Disp_OFF is 0 or 128 ~255
    if ((__bSP_Disp_OFF==0) || (__bSP_Disp_OFF&0x80))
    {
        if (!(__wDiscType & BOOK_2X)) //SVCD and CVD
        {
            /*
            _wOSDNDTemp = DVD_GetLangCD(SET_SPST, __bSPSTNO);
            _OSDDSPLY_CodeToLanguage(_wOSDNDTemp);
            */
            _OSDDSPLY_CodeToLanguage(__wSP_LCD); //CoCo1.20, use __wSP_LCD to show SPST language directly.
            
            OSDUTL_CopyStringFromIndex(aNDAudioDistance);
        }

        OSDUTL_OutputNumFromIndex((WORD)(__bSPSTNO+1));
        OSDUTL_CopyStringFromIndex(aNDSlash);
        OSDUTL_OutputNumFromIndex(__bSPSTNs);
    }
    else // spst is off
        OSDUTL_CopyStringFromIndex(aNDOff);   
}

//***************************************************************************
//  Function    :   _OSDND_DefaultState
//  Abstract    :   This function will set the default value about forground,
//              :   background, and vertical starting position for upper right 
//              :   message.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_DefaultState(void)
{
    __OSDNDUpMsgAttr.bBackground = OSDND_PALETTE_ENTRY_BACKGROUND;
    __OSDNDUpMsgAttr.bColorKey = 0;
    __OSDNDUpMsgAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDUpMsgAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDUpMsgAttr.bTextColor = OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING;
    __OSDNDUpMsgAttr.wX = OSDND_DISPLAY_UPPER_RIGHT_POS_H;
    __OSDNDUpMsgAttr.wY = OSDND_DISPLAY_UPPER_RIGHT_POS_V;

    __OSDNDBottomMsgAttr.bBackground = OSDND_PALETTE_ENTRY_BACKGROUND;
    __OSDNDBottomMsgAttr.bColorKey = 0;
    __OSDNDBottomMsgAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDBottomMsgAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDBottomMsgAttr.bTextColor = OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING;
    __OSDNDBottomMsgAttr.wX = OSDND_DISPLAY_BOTTOM_START_H;

    //CoCo1.20, put the normal bottom meesage to be lower in PAL mode.
#ifdef ENLARGE_OSD_FOR_PAL_MODE
    if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
        __OSDNDBottomMsgAttr.wY = OSDND_DISPLAY_BOTTOM_START_V;
    else
        __OSDNDBottomMsgAttr.wY = OSDND_DISPLAY_BOTTOM_START_V_PAL;
#else
    __OSDNDBottomMsgAttr.wY = OSDND_DISPLAY_BOTTOM_START_V;
#endif
        
    GDI_SetMixRatio(OSDND_MIX_RATIO_BACKGROUND); //The mix ratio just effects the background
}

//***************************************************************************
//  Function    :   _OSDND_InitializeGDI
//  Abstract    :   This function will initialize the palettes and draw the background of
//              :   the normal display.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_InitializeGDI(void)
{    
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_BACKGROUND, OSDND_PALETTE_COLOR_BACKGROUND, TRUE);
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING, OSDND_PALETTE_COLOR_UPPER_RIGHT_STRING, FALSE);
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_BOTTOM_STRING, OSDND_PALETTE_COLOR_BOTTOM_STRING, FALSE);
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_BAR_FG, OSDND_PALETTE_COLOR_BAR_FG, FALSE);    
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_MUTE_ICON_TEXT_FOREGROUND, OSDND_PALETTE_COLOR_MUTE_ICON_TEXT_FOREGROUND, FALSE);    
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_MUTE_ICON_SHADOW_1, OSDND_PALETTE_COLOR_MUTE_ICON_SHADOW_1, FALSE);    
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_MUTE_ICON_SHADOW_2, OSDND_PALETTE_COLOR_MUTE_ICON_SHADOW_2, FALSE);    
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_MUTEOFF_ICON_TEXT_FOREGROUND, OSDND_PALETTE_COLOR_MUTEOFF_ICON_TEXT_FOREGROUND, FALSE);    
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_MUTEOFF_ICON_SHADOW_1, OSDND_PALETTE_COLOR_MUTEOFF_ICON_SHADOW_1, FALSE);    
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_MUTEOFF_ICON_SHADOW_2, OSDND_PALETTE_COLOR_MUTEOFF_ICON_SHADOW_2, FALSE);  
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_PROGRESS_COLOR, OSDND_PALETTE_COLOR_PROGRESS_COLOR, TRUE);

#ifndef USE_4B_MODE_OSD    
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_ZOOM_COLOR, OSDND_PALETTE_COLOR_ZOOM_COLOR, TRUE);

#ifdef NO_PIP
    GDI_ChangePALEntry(OSDND_PALETTE_ENTRY_NORMAL_FRAME, OSDND_PALETTE_COLOR_NORMAL_FRAME, FALSE);   
#endif

#endif

    GDI_SetMixRatio(OSDND_MIX_RATIO_BACKGROUND); //The mix ration just effects the background

    //CoCo0.95
    GDI_WaitPaletteComplete();
}

//***************************************************************************
//  Function    :   _OSDND_ShowWaitingState
//  Abstract    :   This function will show waiting
//  Arguments   :   none.
//  Return		:   The end of the position to show "Loading".
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowWaitingState(void)
{
#ifdef  SHOW_LOADING_STATUS
    _bWaitingCnt++;
    if (_bWaitingCnt > 3)
        _bWaitingCnt = 1;
#else   
    _bWaitingCnt = 3;
#endif

    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aNDWait);

    __wOSDUTLStringBuf[0] += OSDND_MAX_DOT_NUM_FOR_LOADING << 1;

    for (__bTemp = 0; __bTemp < OSDND_MAX_DOT_NUM_FOR_LOADING; __bTemp++)
    {
        __wOSDUTLStringBuf[__bOSDUTLIndex] = CHAR_SPACE;
        __wOSDUTLStringBuf[__bOSDUTLIndex+1] = CHAR_FULL_STOP;
        __bOSDUTLIndex += 2;
    }

    _bOSDNDTemp = _bWaitingCnt;

    while (_bOSDNDTemp < OSDND_MAX_DOT_NUM_FOR_LOADING)
    {
        __wOSDUTLStringBuf[__bOSDUTLIndex--] = CHAR_SPACE;
        __wOSDUTLStringBuf[__bOSDUTLIndex--] = CHAR_SPACE;
        _bOSDNDTemp++;
    }
}

//***************************************************************************
//  Function    :   _OSDND_OutputAudioType
//  Abstract    :   This function will Output audio type.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_OutputAudioType(BYTE bAudioType)
{
    switch (bAudioType)
    {
    case HAL_AUDIO_MPG:
        OSDUTL_CopyStringFromIndex(aNDMPEG);
        break;
    case HAL_AUDIO_AC3:
#ifdef SUPPORT_DMP
        OSDUTL_CopyStringFromIndex(aSPACE);
#else
        OSDUTL_CopyStringFromIndex(aNDAC3);
#endif //SUPPORT_DMP
        break;
    case HAL_AUDIO_LPCM:
        OSDUTL_CopyStringFromIndex(aNDLPCM);
        break;
    case HAL_AUDIO_DTS:
        OSDUTL_CopyStringFromIndex(aNDDTS);
        break;
    /*case HAL_AUDIO_SDDS:
        OSDUTL_CopyStringFromIndex(aSDDS);
        break;*/
    case HAL_AUDIO_PCM:
        OSDUTL_CopyStringFromIndex(aNDPCM);
        break;
    case HAL_AUDIO_MP3:
        OSDUTL_CopyStringFromIndex(aNDMP3);
        break;
    case HAL_AUDIO_WMA:
        OSDUTL_CopyStringFromIndex(aNDWMA);
        break;
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDDND_OutputAudioChannel
//  Abstract    :   This function will Output audio channel.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDND_OutputAudioChannel(BYTE bAudioChannel)
{
    switch (bAudioChannel)
    {
    case 2:
        OSDUTL_CopyStringFromIndex(aND2Channel);        
        break;
    case 6:
        OSDUTL_CopyStringFromIndex(aND51Channel);      
        break;
    case 8:
        OSDUTL_CopyStringFromIndex(aND71Channel);
        break;
    default:        
        __wOSDUTLStringBuf[__bOSDUTLIndex++] = aNDNumbers[__bACHN];
        OSDUTL_CopyStringFromIndex(aNDChannel);
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDND_ShowString
//  Abstract    :   
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDND_ShowString(void)
{
    BYTE bIndex;

    OSDUTL_PrepareOutputString();

    _bOSDNDTemp = strlen((char *)__bMsgString);
    if (_bOSDNDTemp > OSDUTL_STRING_LENGTH)
        _bOSDNDTemp = OSDUTL_STRING_LENGTH;

    __wOSDUTLStringBuf[0] = _bOSDNDTemp;

    for (bIndex = 0; bIndex <_bOSDNDTemp; bIndex++)
    {
        __wOSDUTLStringBuf[bIndex+1] = OSDUTL_TransformChar(__bMsgString[bIndex]);
    }
}

//**********************************************************************
//  Function    :   _OSDND_ShowZoom
//  Abstract    :   This function will use PIP+OSD to show "graph" zoom.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************
void _OSDND_ShowZoom(void)
{
#ifdef SUPPORT_GRAPH_ZOOM

    WORD wWidth = 0, wHeight = 0;
#ifndef NO_PIP
    WORD wTuneStartH = 0, wTuneStartV = 0;
#endif

    //if ((_bOSDCurrentUI == OSD_UI_NONE) || (_bOSDCurrentUI == OSD_UI_DISPLAY)) //Only "Normal display mode" and "Display mode" support graphics zoom.
    {
#ifdef NO_PIP

        //Clear the rectangle of PIP part.
        _OSDNDRect.rect.wLeft = OSDND_DISPLAY_ZOOM_START_H - OSDND_DISPLAY_ZOOM_BORDER_THICKNESS;
        _OSDNDRect.rect.wTop = OSDND_DISPLAY_ZOOM_START_V - OSDND_DISPLAY_ZOOM_BORDER_THICKNESS;
        _OSDNDRect.rect.wRight = _OSDNDRect.rect.wLeft + OSDND_DISPLAY_ZOOM_WIDTH + (OSDND_DISPLAY_ZOOM_BORDER_THICKNESS << 1)-1;
        _OSDNDRect.rect.wBottom = _OSDNDRect.rect.wTop + OSDND_DISPLAY_ZOOM_HEIGHT + (OSDND_DISPLAY_ZOOM_BORDER_THICKNESS << 1)-1;    
        _OSDNDRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;

        switch (_bOSDCurrentUI)
        {
        case OSD_UI_DISPLAY:
        case OSD_UI_NONE:
        case OSD_UI_NAVIGATOR:
        case OSD_UI_SEARCH:
            GDI_FillRect_909(0, &_OSDNDRect);
            break;
        default:
            break;
        }

        _OSDNDFrame.bColorBright = OSDND_PALETTE_ENTRY_NORMAL_FRAME;
        _OSDNDFrame.bColorDark = OSDND_PALETTE_ENTRY_NORMAL_FRAME;    
        _OSDNDFrame.bColorCenter = PAL_ENTRY_COLOR_TRANSPARENT; //"FRAME_DOUBLE" doesn't need to assign the center color?
        _OSDNDFrame.bStyle = FRAME_PUSH;
        _OSDNDFrame.bThickness = OSDND_DISPLAY_ZOOM_BORDER_THICKNESS;
        _OSDNDFrame.rect.wLeft = OSDND_DISPLAY_ZOOM_START_H - OSDND_DISPLAY_ZOOM_BORDER_THICKNESS;
        _OSDNDFrame.rect.wTop = OSDND_DISPLAY_ZOOM_START_V - OSDND_DISPLAY_ZOOM_BORDER_THICKNESS;
        _OSDNDFrame.rect.wRight = _OSDNDFrame.rect.wLeft + OSDND_DISPLAY_ZOOM_WIDTH + (OSDND_DISPLAY_ZOOM_BORDER_THICKNESS << 1)-1;
        _OSDNDFrame.rect.wBottom = _OSDNDFrame.rect.wTop + OSDND_DISPLAY_ZOOM_HEIGHT + (OSDND_DISPLAY_ZOOM_BORDER_THICKNESS << 1)-1;
                
        if (__bLOGO == LOGO_JPEG)
        {
            if (_HALJPEG_GetRotateStatus())
            {
                if (_wHALJPEGClipWidth > _wHALJPEGScalingHeight)
                    wWidth = OSDND_DISPLAY_ZOOM_WIDTH;
                else
                    wWidth = _wHALJPEGClipWidth*OSDND_DISPLAY_ZOOM_WIDTH/_wHALJPEGScalingHeight;
                
                if (_wHALJPEGClipHeight > _wHALJPEGScalingWidth)
                    wHeight = OSDND_DISPLAY_ZOOM_PIP_HEIGHT;
                else
                    wHeight = _wHALJPEGClipHeight*OSDND_DISPLAY_ZOOM_HEIGHT/_wHALJPEGScalingWidth;
                
                if (_wHALJPEGClipHStart > ((HALJPEG_DISPLAY_BUF_WIDTH-_wHALJPEGScalingHeight)/2))
                    _OSDNDRect.rect.wLeft = OSDND_DISPLAY_ZOOM_START_H+(_wHALJPEGClipHStart-(HALJPEG_DISPLAY_BUF_WIDTH-_wHALJPEGScalingHeight)/2)*wWidth/_wHALJPEGClipWidth;
                else
                    _OSDNDRect.rect.wLeft = OSDND_DISPLAY_ZOOM_START_H;
                
                if (_wHALJPEGClipVStart > ((HALJPEG_DISPLAY_BUF_HEIGHT-_wHALJPEGScalingWidth)/2))
                    _OSDNDRect.rect.wTop = OSDND_DISPLAY_ZOOM_START_V+(_wHALJPEGClipVStart-(HALJPEG_DISPLAY_BUF_HEIGHT-_wHALJPEGScalingWidth)/2)*wHeight/_wHALJPEGClipHeight;
                else
                    _OSDNDRect.rect.wTop = OSDND_DISPLAY_ZOOM_START_V;
            }
            else
            {
                if (_wHALJPEGClipWidth > _wHALJPEGScalingWidth)
                    wWidth = OSDND_DISPLAY_ZOOM_WIDTH;
                else
                    wWidth = _wHALJPEGClipWidth*OSDND_DISPLAY_ZOOM_WIDTH/_wHALJPEGScalingWidth;
                
                if (_wHALJPEGClipHeight > _wHALJPEGScalingHeight)
                    wHeight = OSDND_DISPLAY_ZOOM_HEIGHT;
                else
                    wHeight = _wHALJPEGClipHeight*OSDND_DISPLAY_ZOOM_HEIGHT/_wHALJPEGScalingHeight;
                
                if (_wHALJPEGClipHStart > ((HALJPEG_DISPLAY_BUF_WIDTH-_wHALJPEGScalingWidth)/2))
                    _OSDNDRect.rect.wLeft = OSDND_DISPLAY_ZOOM_START_H+(_wHALJPEGClipHStart-(HALJPEG_DISPLAY_BUF_WIDTH-_wHALJPEGScalingWidth)/2)*wWidth/_wHALJPEGClipWidth;
                else
                    _OSDNDRect.rect.wLeft = OSDND_DISPLAY_ZOOM_START_H;
                
                if (_wHALJPEGClipVStart > ((HALJPEG_DISPLAY_BUF_HEIGHT-_wHALJPEGScalingHeight)/2))
                    _OSDNDRect.rect.wTop = OSDND_DISPLAY_ZOOM_START_V+(_wHALJPEGClipVStart-(HALJPEG_DISPLAY_BUF_HEIGHT-_wHALJPEGScalingHeight)/2)*wHeight/_wHALJPEGClipHeight;
                else
                    _OSDNDRect.rect.wTop = OSDND_DISPLAY_ZOOM_START_V;
            }
        }
        else
        {
            wWidth = __wHCW*OSDND_DISPLAY_ZOOM_WIDTH/__dwPicHSize;
            wHeight = __wVCW*OSDND_DISPLAY_ZOOM_HEIGHT/__dwPicVSize;
            _OSDNDRect.rect.wLeft = OSDND_DISPLAY_ZOOM_START_H+__wXStart*wWidth/__wHCW;
            _OSDNDRect.rect.wTop = OSDND_DISPLAY_ZOOM_START_V+__wYStart*wHeight/__wVCW;
        }
        
        switch (_wOSDNDInfo)
        {
        case VIEW_ZOOMX1:
        case VIEW_ZOOM25:
        case VIEW_ZOOM50:
        case VIEW_ZOOM100:
            break;
        default:
            GDI_DrawFrame_909(0, &_OSDNDFrame);

            _OSDNDRect.rect.wRight = _OSDNDRect.rect.wLeft+wWidth-1;
            _OSDNDRect.rect.wBottom = _OSDNDRect.rect.wTop+wHeight-1;    

            if (__bZoomPos & POS_LAST_RIGHT)
            {
                _OSDNDRect.rect.wRight = _OSDNDFrame.rect.wRight - OSDND_DISPLAY_ZOOM_BORDER_THICKNESS;
                _OSDNDRect.rect.wLeft = _OSDNDRect.rect.wRight - wWidth;
            }

            if (__bZoomPos & POS_LAST_LEFT)
            {
                _OSDNDRect.rect.wLeft = OSDND_DISPLAY_ZOOM_START_H;
                _OSDNDRect.rect.wRight = _OSDNDRect.rect.wRight = _OSDNDRect.rect.wLeft+wWidth-1;
            }

            if (__bZoomPos & POS_LAST_UP)
            {
                _OSDNDRect.rect.wTop = OSDND_DISPLAY_ZOOM_START_V;
                _OSDNDRect.rect.wBottom = _OSDNDRect.rect.wTop+wHeight-1;
            }

            if (__bZoomPos & POS_LAST_DOWN)
            {
                _OSDNDRect.rect.wTop = _OSDNDRect.rect.wBottom - wHeight;
                _OSDNDRect.rect.wBottom = _OSDNDFrame.rect.wBottom-OSDND_DISPLAY_ZOOM_BORDER_THICKNESS;
            }

            _OSDNDRect.bColor = OSDND_DISPLAY_ZOOM_OSD_MOVE_COLOR;

            GDI_FillRect_909(0, &_OSDNDRect);
            break;
        }
#else
        //set PIP border
        _OSDNDZoomPIPColor.Color.bWeight = OSDND_DISPLAY_ZOOM_PIP_COLOR_WEIGHT;
        _OSDNDZoomPIPColor.Color.bY = OSDND_DISPLAY_ZOOM_PIP_COLOR_Y;
        _OSDNDZoomPIPColor.Color.bU = OSDND_DISPLAY_ZOOM_PIP_COLOR_U;
        _OSDNDZoomPIPColor.Color.bV = OSDND_DISPLAY_ZOOM_PIP_COLOR_V;
        DISP_BGColorSet(_OSDNDZoomPIPColor, DISP_PIP_BORDER);
        DISP_PIPBorderSet(OSDND_DISPLAY_ZOOM_PIP_BORDER_THICKNESS, OSDND_DISPLAY_ZOOM_PIP_BORDER_THICKNESS, TRUE);
        
        HAL_ReadInfo(HAL_INFO_HSIZE, &_dwOSDNDTemp);
        HAL_ReadInfo(HAL_INFO_VSIZE, &_dwOSDNDTemp1);
        
        switch (_wOSDNDInfo)
        {
        case VIEW_ZOOMX1:
        case VIEW_ZOOM25:
        case VIEW_ZOOM50:
        case VIEW_ZOOM100:
            //Remove the PIP
            PIPVideoInfo.bEnable = FALSE;
            break;
        default:
            PIPVideoInfo.bEnable = TRUE;
            break;
        }

        PIPVideoInfo.sHStart = OSDND_DISPLAY_ZOOM_PIP_START_H;
        PIPVideoInfo.sVStart = OSDND_DISPLAY_ZOOM_PIP_START_V;
        PIPVideoInfo.wHWidth = OSDND_DISPLAY_ZOOM_PIP_WIDTH;
        PIPVideoInfo.wVWidth = OSDND_DISPLAY_ZOOM_PIP_HEIGHT;    
        
        DISP_PIPSet(&PIPVideoInfo, _dwOSDNDTemp, _dwOSDNDTemp1, DISP_NEXTVSYNC);
        
        if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
        {
            wTuneStartH = OSDND_DISPLAY_ZOOM_OSD_TUNE_NTSC_START_H;
            wTuneStartV = OSDND_DISPLAY_ZOOM_OSD_TUNE_NTSC_START_V;
        }
        else
        {
            wTuneStartH = OSDND_DISPLAY_ZOOM_OSD_TUNE_PAL_START_H;
            wTuneStartV = OSDND_DISPLAY_ZOOM_OSD_TUNE_PAL_START_V;
        }
        
        //Clear the rectangle of PIP part.
        _OSDNDRect.rect.wLeft = PIPVideoInfo.sHStart-wTuneStartH-OSDND_DISPLAY_ZOOM_OSD_TUNE_CLEAR_REGION;
        _OSDNDRect.rect.wTop = PIPVideoInfo.sVStart-wTuneStartV-OSDND_DISPLAY_ZOOM_OSD_TUNE_CLEAR_REGION;
        _OSDNDRect.rect.wRight = _OSDNDRect.rect.wLeft+OSDND_DISPLAY_ZOOM_PIP_WIDTH+(OSDND_DISPLAY_ZOOM_OSD_TUNE_CLEAR_REGION<<1);
        _OSDNDRect.rect.wBottom = _OSDNDRect.rect.wTop+OSDND_DISPLAY_ZOOM_PIP_HEIGHT+(OSDND_DISPLAY_ZOOM_OSD_TUNE_CLEAR_REGION<<1);    
        _OSDNDRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;

        switch (_bOSDCurrentUI)
        {
        case OSD_UI_DISPLAY:
        case OSD_UI_NONE:
        case OSD_UI_NAVIGATOR:
        case OSD_UI_SEARCH:
            GDI_FillRect_909(0, &_OSDNDRect);
            break;
        default:
            break;
        }
        
        if (__bLOGO == LOGO_JPEG)
        {
            if (_HALJPEG_GetRotateStatus())
            {
                if (_wHALJPEGClipWidth > _wHALJPEGScalingHeight)
                    wWidth = OSDND_DISPLAY_ZOOM_PIP_WIDTH;
                else
                    wWidth = _wHALJPEGClipWidth*OSDND_DISPLAY_ZOOM_PIP_WIDTH/_wHALJPEGScalingHeight;
                
                if (_wHALJPEGClipHeight > _wHALJPEGScalingWidth)
                    wHeight = OSDND_DISPLAY_ZOOM_PIP_HEIGHT;
                else
                    wHeight = _wHALJPEGClipHeight*OSDND_DISPLAY_ZOOM_PIP_HEIGHT/_wHALJPEGScalingWidth;
                
                if (_wHALJPEGClipHStart > ((HALJPEG_DISPLAY_BUF_WIDTH-_wHALJPEGScalingHeight)/2))
                    _OSDNDRect.rect.wLeft = PIPVideoInfo.sHStart+(_wHALJPEGClipHStart-(HALJPEG_DISPLAY_BUF_WIDTH-_wHALJPEGScalingHeight)/2)*wWidth/_wHALJPEGClipWidth-wTuneStartH;
                else
                    _OSDNDRect.rect.wLeft = PIPVideoInfo.sHStart-wTuneStartH;
                
                if (_wHALJPEGClipVStart > ((HALJPEG_DISPLAY_BUF_HEIGHT-_wHALJPEGScalingWidth)/2))
                    _OSDNDRect.rect.wTop = PIPVideoInfo.sVStart+(_wHALJPEGClipVStart-(HALJPEG_DISPLAY_BUF_HEIGHT-_wHALJPEGScalingWidth)/2)*wHeight/_wHALJPEGClipHeight-wTuneStartV;
                else
                    _OSDNDRect.rect.wTop = PIPVideoInfo.sVStart-wTuneStartV;
            }
            else
            {
                if (_wHALJPEGClipWidth > _wHALJPEGScalingWidth)
                    wWidth = OSDND_DISPLAY_ZOOM_PIP_WIDTH;
                else
                    wWidth = _wHALJPEGClipWidth*OSDND_DISPLAY_ZOOM_PIP_WIDTH/_wHALJPEGScalingWidth;
                
                if (_wHALJPEGClipHeight > _wHALJPEGScalingHeight)
                    wHeight = OSDND_DISPLAY_ZOOM_PIP_HEIGHT;
                else
                    wHeight = _wHALJPEGClipHeight*OSDND_DISPLAY_ZOOM_PIP_HEIGHT/_wHALJPEGScalingHeight;
                
                if (_wHALJPEGClipHStart > ((HALJPEG_DISPLAY_BUF_WIDTH-_wHALJPEGScalingWidth)/2))
                    _OSDNDRect.rect.wLeft = PIPVideoInfo.sHStart+(_wHALJPEGClipHStart-(HALJPEG_DISPLAY_BUF_WIDTH-_wHALJPEGScalingWidth)/2)*wWidth/_wHALJPEGClipWidth-wTuneStartH;
                else
                    _OSDNDRect.rect.wLeft = PIPVideoInfo.sHStart-wTuneStartH;
                
                if (_wHALJPEGClipVStart > ((HALJPEG_DISPLAY_BUF_HEIGHT-_wHALJPEGScalingHeight)/2))
                    _OSDNDRect.rect.wTop = PIPVideoInfo.sVStart+(_wHALJPEGClipVStart-(HALJPEG_DISPLAY_BUF_HEIGHT-_wHALJPEGScalingHeight)/2)*wHeight/_wHALJPEGClipHeight-wTuneStartV;
                else
                    _OSDNDRect.rect.wTop = PIPVideoInfo.sVStart - wTuneStartV;
            }
        }
        else
        {
            wWidth = __wHCW*OSDND_DISPLAY_ZOOM_PIP_WIDTH/__dwPicHSize;
            wHeight = __wVCW*OSDND_DISPLAY_ZOOM_PIP_HEIGHT/__dwPicVSize;
            _OSDNDRect.rect.wLeft = PIPVideoInfo.sHStart+__wXStart*wWidth/__wHCW-wTuneStartH;
            _OSDNDRect.rect.wTop = PIPVideoInfo.sVStart+__wYStart*wHeight/__wVCW-wTuneStartV;
        }
        
        switch (_wOSDNDInfo)
        {
        case VIEW_ZOOMX1:
        case VIEW_ZOOM25:
        case VIEW_ZOOM50:
        case VIEW_ZOOM100:
            //Remove the PIP
            PIPVideoInfo.bEnable = FALSE;
            DISP_PIPSet(&PIPVideoInfo, _dwOSDNDTemp, _dwOSDNDTemp1, DISP_NEXTVSYNC);
            //DISP_DisplayCtrl(DISP_PIP, FALSE);
            break;
        default:
            _OSDNDRect.rect.wRight = _OSDNDRect.rect.wLeft+wWidth;
            _OSDNDRect.rect.wBottom = _OSDNDRect.rect.wTop+wHeight;    
            _OSDNDRect.bColor = OSDND_DISPLAY_ZOOM_OSD_MOVE_COLOR;

            GDI_FillRect_909(0, &_OSDNDRect);
            break;
        }
#endif //NO_PIP
    }
#endif //SUPPORT_GRAPH_ZOOM
}

//CoCo1.10, support progress bar
//**********************************************************************
//  Function    :   _OSDND_ShowProgressBar
//  Abstract    :   
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************
void _OSDND_ShowProgressBar(void)
{
    _wOSDNDHPos = __OSDNDBottomMsgAttr.wX;

    if (__bOSDClearNDBRegion || _wOSDNDInfo)
    {
        _OSDNDRect.rect.wLeft = _wOSDNDHPos;
        _OSDNDRect.rect.wRight = _wOSDNDHPos+OSDND_DISPLAY_PROGRESS_BAR_WIDTH+OSDND_DISTANCE_FONT_PROGRESS_BAR*2;
        _OSDNDRect.rect.wTop = __OSDNDBottomMsgAttr.wY;
        _OSDNDRect.rect.wBottom = __OSDNDBottomMsgAttr.wY+OSDND_FONT_HEIGHT-1;
        _OSDNDRect.bColor = __OSDNDBottomMsgAttr.bBackground;
        
        GDI_FillRect_909(0, &_OSDNDRect);
    }

    if (__wProgressTotal == 0)    
        return;

    if (__wProgressCurrent == __wProgressTotal)
        _bOSDNDTemp = 100;
    else
        _bOSDNDTemp = (BYTE)(__wProgressCurrent*100/__wProgressTotal);

    //CoCo1.21, Fix the bug about the progress bar may overlap the button because the value is larger than 100% for some case.
    if (_bOSDNDTemp > 100)
        _bOSDNDTemp = 100;

    _wOSDNDHPos += OSDND_DISTANCE_FONT_PROGRESS_BAR;

    _OSDNDVolumeBar.bBgColor = __OSDNDBottomMsgAttr.bBackground;
    _OSDNDVolumeBar.bBarColor = OSDND_PALETTE_ENTRY_PROGRESS_COLOR;
    _OSDNDVolumeBar.bFrameColor = OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING;  //Alan2.36, Mantis #2241
    _OSDNDVolumeBar.bOriention = ORIENTION_HORIZONTAL;
    _OSDNDVolumeBar.rect.wLeft = _wOSDNDHPos;
    _OSDNDVolumeBar.rect.wRight = _wOSDNDHPos + OSDND_DISPLAY_PROGRESS_BAR_WIDTH;
    _OSDNDVolumeBar.rect.wTop = __OSDNDBottomMsgAttr.wY + (OSDND_FONT_HEIGHT-OSDND_DISPLAY_PROGRESS_BAR_HEIGHT)/2;
    _OSDNDVolumeBar.rect.wBottom = _OSDNDVolumeBar.rect.wTop + OSDND_DISPLAY_PROGRESS_BAR_HEIGHT;
    _OSDNDVolumeBar.wVolume = (WORD)_bOSDNDTemp; // percent

    GDI_DrawVolumeBar_909(0, &_OSDNDVolumeBar);

    OSDUTL_PrepareOutputString();

    if (_bOSDNDTemp < 10)
    {
        OSDUTL_OutputNumFromIndex(0);
        OSDUTL_OutputNumFromIndex(0);
    }
    else if (_bOSDNDTemp < 100)
    {
        OSDUTL_OutputNumFromIndex(0);
    }

    OSDUTL_OutputNumFromIndex(_bOSDNDTemp);
    OSDUTL_CopyStringFromIndex(aNDPercent);
        
    _wOSDNDHPos += (OSDND_DISPLAY_PROGRESS_BAR_WIDTH+OSDND_DISTANCE_FONT_PROGRESS_BAR);

    _wOSDNDTemp = __OSDNDBottomMsgAttr.wX;
    __OSDNDBottomMsgAttr.wX = _wOSDNDHPos;

    //GDI_DrawString_909(0, &__OSDNDBottomMsgAttr, __pOSDUTLOutputString);
    _OSDND_OutputBottomString();

    __OSDNDBottomMsgAttr.wX = _wOSDNDTemp;
}

//***************************************************************************
//  Function    :   OSDND_ConfigOSDRegion
//  Abstract    :   This function will config the OSD region. And it is exported for OSD kernel usage.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSDND_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;

#ifdef USE_4B_MODE_OSD
    RegionInfo.bColorMode = GDI_OSD_4B_MODE;
#else
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;        
#endif        
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
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;    
    
    /*
    GDI_DeactivateRegion();
    GDI_ConfigRegionInfo(0, &RegionInfo);
    
    GDI_InitialRegion(0);
    
    // Clear region
    //GDI_ClearRegion(0);
    
    GDI_ActivateRegion(0);
    */
    OSD_SetRegion(0, FALSE, &RegionInfo);
}


//***************************************************************************
//  Function    :   OSDUTL_PrepareOutputString
//  Abstract    :   This function will initialize __bOSDUTLIndex and __wOSDUTLStringBuf[] 
//              :   for later output string to __wOSDUTLStringBuf[].
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSDUTL_PrepareOutputString(void)
{
    __bOSDUTLIndex = 1;
    __pOSDUTLOutputString = __wOSDUTLStringBuf;
}

//***************************************************************************
//  Function    :   OSDUTL_OutputNumFromIndex
//  Abstract    :   This function will output the number to __wOSDUTLStringBuf[].
//  Arguments   :   wNum: The number you want to output.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSDUTL_OutputNumFromIndex(WORD wNum)
{
    WORD wBase;
    WORD wTemp;

    wTemp = wNum;
    
    wBase = 10000; //set the base to 10000 because the maximum value for a "WORD" is 65535
    
    if (wTemp == 0)
    {
        __wOSDUTLStringBuf[__bOSDUTLIndex++] = aNDNumbers[wTemp];
    }
    else
    {
        while (!(wTemp / wBase))
        {
            wBase = wBase / 10;
        }
        
        while (1)
        {
            __wOSDUTLStringBuf[__bOSDUTLIndex++] = aNDNumbers[wTemp / wBase];
            wTemp = wTemp % wBase;
            wBase = wBase / 10;
            
            if (wBase == 0)
                break;
        }
    }

    __wOSDUTLStringBuf[0] = __bOSDUTLIndex - 1;
}

//***************************************************************************
//  Function    :   OSDUTL_CopyStringFromIndex
//  Abstract    :   This function will copy the string to the __bOSDUTLIndex of __wOSDUTLStringBuf[].
//  Arguments   :   pString: the string to be copyed.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   Should assign __bOSDUTLIndex before calling this function.
//***************************************************************************
void OSDUTL_CopyStringFromIndex(WORD *pString)
{
    BYTE bOSDUTLTemp;

    for (bOSDUTLTemp=1; (bOSDUTLTemp <= pString[0]) && (bOSDUTLTemp <= OSDUTL_STRING_LENGTH); bOSDUTLTemp++)
    {
        __wOSDUTLStringBuf[__bOSDUTLIndex++] = pString[bOSDUTLTemp];
    }

    __wOSDUTLStringBuf[0] = __bOSDUTLIndex-1;
}

//***************************************************************************
//  Function    :   OSDUTL_ShowHourMinSec
//  Abstract    :   This function will show time.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
// LLY2.53, modify the procedure since current time value unit is expand from WORD to DWORD
// [23:16]: hour, [15:8]: min, [7:0]: sec
void OSDUTL_ShowHourMinSec(void)
{    
    //show "hour"
    //_OSDUTL_ShowTimeNum((BYTE)(HIBYTE(gcShowStatus.wCurrentTime)/60));
    _OSDUTL_ShowTimeNum((BYTE)HIWORD(gcShowStatus.dwCurrentTime));
    
    //show ":"
    __wOSDUTLStringBuf[__bOSDUTLIndex++] = aNDMouhow[1];

    //show "Minute"
    //_OSDUTL_ShowTimeNum((BYTE)(HIBYTE(gcShowStatus.wCurrentTime)%60));
    _OSDUTL_ShowTimeNum(HIBYTE(LOWORD(gcShowStatus.dwCurrentTime)));

    //show ":"
    __wOSDUTLStringBuf[__bOSDUTLIndex++] = aNDMouhow[1];

    //show "second"
    //_OSDUTL_ShowTimeNum((BYTE)(LOBYTE(gcShowStatus.wCurrentTime)));
    _OSDUTL_ShowTimeNum((BYTE)(LOWORD(gcShowStatus.dwCurrentTime)));
}

//***************************************************************************
//  Function    :   _OSDUTL_ShowTimeNum
//  Abstract    :   This function will show the section of the time.
//  Arguments   :   bNum: The time you want to display.
//  Return		:   The final position after displaying the time.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDUTL_ShowTimeNum(BYTE bNum)
{
    if (bNum < 10)
        OSDUTL_OutputNumFromIndex(0);
    
    OSDUTL_OutputNumFromIndex((WORD)bNum);
}

//**********************************************************************
//  Function    :   OSD_TransformChar
//  Abstract    :   This function will transform the character to the value that GDI can display.
//  Arguments   :   bChar: The character that you want to transform.
//  Return      :   The transformed value.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************
WORD OSDUTL_TransformChar(char chCode)
{
    switch (chCode) //for punctuation
    {
    case 92: //CoCo1.20, add "\\" for common usage.
        return CHAR_REVERSE_SOLIDUS;
    case 95: //"_"
        return CHAR_LOW_LINE;
    case 47: //"/"
        return CHAR_SOLIDUS;
    case 46: //"."
        return CHAR_FULL_STOP;
    case 45: //"-"
        return CHAR_HYPHEN_MINUS;
    case 43: //"+"
        return CHAR_PLUS_SIGN;
    case 58: //":"
        return CHAR_COLON;
    case 32:
    case 39: //CoCo1.26, "'" show space temporarily
        return CHAR_SPACE;
    default:
        break;
    }

    if (chCode <= 57) 
    {
        //0~9 ==>48~57
        return aNDNumbers[chCode-48];
    }
    else if (chCode <= 90) //A~Z==>65~90
    {
        return aNDUppercase[chCode - 65];
    }
    else if (chCode <= 122)
    {
        //a~z==>97~122
        return aNDLowercase[chCode - 97];
    }
    else
        return CHAR_SPACE;
}

//**********************************************************************
//  Function    :   OSDUTL_ShowJPEGDate
//  Abstract    :   
//  Arguments   :   
//  Return      :   
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************
void OSDUTL_ShowJPEGDate(WORD wDirID, WORD wFileIndex)
{
    OSDUTL_PrepareOutputString();

    _bOSDNDTemp = 0;

    if (__cMMJPEGFMTExifDate[0] == '\0')
    {
		_dwOSDNDTemp = MM_ID_RelativeToAbsolute(wDirID, wFileIndex);
		__pMMFileRecord = __pMMFileRecordStart;
		__pMMFileRecord += _dwOSDNDTemp;

        //__wOSDUTLStringBuf[0] = 0;
		OSDUTL_OutputNumFromIndex(MM_GetFileInfo(MM_GET_FILE_YEAR));
		OSDUTL_CopyStringFromIndex(aNDSlash);		
		OSDUTL_OutputNumFromIndex(MM_GetFileInfo(MM_GET_FILE_MONTH));
		OSDUTL_CopyStringFromIndex(aNDSlash);		
		OSDUTL_OutputNumFromIndex(MM_GetFileInfo(MM_GET_FILE_DATE));
    }
    else
    {
        while (__cMMJPEGFMTExifDate[_bOSDNDTemp] != '\0')
        {
            __wOSDUTLStringBuf[_bOSDNDTemp+1] = OSDUTL_TransformChar(__cMMJPEGFMTExifDate[_bOSDNDTemp]);
            if (__wOSDUTLStringBuf[_bOSDNDTemp+1] == CHAR_COLON)
            {
                __wOSDUTLStringBuf[_bOSDNDTemp+1] = CHAR_SOLIDUS;
            }
            _bOSDNDTemp++;
        }

        __wOSDUTLStringBuf[0] = _bOSDNDTemp;
    }
}

//**********************************************************************
//  Function    :   OSDUTL_ShowJPEGResolution
//  Abstract    :   
//  Arguments   :   
//  Return      :   
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************
void OSDUTL_ShowJPEGResolution(void)
{
    OSDUTL_PrepareOutputString();

    OSDUTL_OutputNumFromIndex(__wJPEGFMTExifImageWidth);
    OSDUTL_CopyStringFromIndex(aNDMultiply);
    OSDUTL_OutputNumFromIndex(__wJPEGFMTExifImageHeight);               
}

