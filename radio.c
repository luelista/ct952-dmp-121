#define RADIO_MAIN_FILE

#include "winav.h"
#include "gdi.h"
#include "cc.h"
#include "hal.h"
#include "radiodrv.h"
#include "radio.h"
#include "input.h"
#include "osd.h"
#include "poweronmenu.h"
#include "mainmenu.h"
#include "rtcdrv.h"
#include "disp.h"
#include "osd.h"
#include "osdnd.h"
#include "fontindex.h"
#include "OSDString\\strextrn.h"
#include "storage.h"
#include "chips.h"
#include "oswrap.h"
#include "romld.h"
#include "convchar.h"
#ifdef CT950_STYLE
#include "dvdsetup_op.h"
#endif //CT950_STYLE

#ifdef SUPPORT_RADIO_TUNER
/* 
const BYTE _aRADIO_Frame_BG[] =
{
#include "bmp/Radio_FrameBG.txt"
};

const BYTE _aRADIO_Title_BG[] =
{
#include "bmp/Radio_TitleBG.txt"
};

const BYTE _aRADIO_Freq_BG[] =
{
#include "bmp/Radio_FreqBG.txt"
};

const BYTE _aRADIO_Info_BG[] =
{
#include "bmp/Radio_InfoBG.txt"
};
*/

/*
const BYTE _aRADIO_H_Button[] =
{
#include "bmp/HButton.txt"
};

const BYTE _aRADIO_N_Button[] =
{
#include "bmp/NButton.txt"
};
*/

BYTE *_aRADIO_Frame_BG;
BYTE *_aRADIO_Title_BG;
BYTE *_aRADIO_Freq_BG;
BYTE *_aRADIO_Info_BG;
BYTE *_aRADIOArray[4];

/*
const BYTE *_aRADIOArray[] = 
{
    _aRADIO_Frame_BG,
    _aRADIO_Title_BG,
    _aRADIO_Freq_BG,
    _aRADIO_Info_BG
    //_aRADIO_H_Button,
    //_aRADIO_N_Button
};
*/

const DWORD _aRADIOBG_Palette[] =  
{
#include "bmp/palRadioBG.txt"
};
/*
const DWORD _aRADIOHButton_Palette[] =  
{
#include "bmp/palHButtonBG.txt"
};

const DWORD _aRADIONButton_Palette[] =  
{
#include "bmp/palNButtonBG.txt"
};
*/
/*
WORD aRADIOSpace[] = {1, CHAR_SPACE};
WORD aRADIODot[] = {1, CHAR_FULL_STOP};
WORD aRADIOColon[] = {1, CHAR_COLON};
WORD aRADIOSolidus[] = {1, CHAR_SOLIDUS};
//WORD aRADIO76[] = {2, CHAR_7, CHAR_6}; 
//WORD aRADIO795[] = {4, CHAR_7, CHAR_9, CHAR_FULL_STOP, CHAR_5};
//WORD aRADIO83[] = {2, CHAR_8, CHAR_3};
//WORD aRADIO84[] = {2, CHAR_8, CHAR_4}; 
WORD aRADIO875[] = {4, CHAR_8, CHAR_7, CHAR_FULL_STOP, CHAR_5}; 
//WORD aRADIO90[] = {2, CHAR_9, CHAR_0};
//WORD aRADIO92[] = {2, CHAR_9, CHAR_2};
WORD aRADIO93[] = {2, CHAR_9, CHAR_3};
WORD aRADIO98[] = {2, CHAR_9, CHAR_8};
//WORD aRADIO100[] = {3, CHAR_1, CHAR_0, CHAR_0}; 
//WORD aRADIO1029[] = {5, CHAR_1, CHAR_0, CHAR_2, CHAR_FULL_STOP, CHAR_9};
WORD aRADIO103[] = {3, CHAR_1, CHAR_0, CHAR_3};
WORD aRADIO108[] = {3, CHAR_1, CHAR_0, CHAR_8};
*/

WORD *_aRADIOUSEuropeBand[RADIO_BAND_DRAW_NUM] = 
{
    aRADIO875,
    aRADIO93,
    aRADIO98,
    aRADIO103,
    aRADIO108,
};

/*
WORD *_aRADIOJapanWideBand[RADIO_BAND_DRAW_NUM] = 
{
    aRADIO76,
    aRADIO84,
    aRADIO92,
    aRADIO100,
    aRADIO108,
};

WORD *_aRADIOJapanNormalBand[RADIO_BAND_DRAW_NUM] = 
{
    aRADIO76,
    aRADIO795,
    aRADIO83,
    aRADIO865,
    aRADIO90,
};
*/

BYTE __bPlayRadio;	// Save current radio playing status

WORD _wRADIOFavorite[RADIO_FAVORITE_NUM];	// An array for save favorite stations
BYTE _bRADIOMode;	// Save current radio mode
BYTE _bRADIOBackupMode;	// Backup current radio mode
BYTE _bRADIOFavoritePlay;	// Save playing favorite item
BYTE _bRADIOButtonCursorIndex;	// Save cursor position of mode button 
BYTE _bRADIOFreqRange;	// 0: US/Europe, 1: Japan Wide, 2: Japan Normal
BYTE _bRADIOSpace;	// 0: 200kHz, 1: 100kHz, 2: 50kHz
WORD _wRADIOBackupFreq;	// Save the last playing station
BYTE _bRADIOStationNum;	// Save the number of found stations
BYTE _bRecordHighlightPos;	// Save the selected record item
DWORD _dwRadioFlashTime;	// Save time for flash something (freq. arrow, record list, mode button)
BYTE _bRADIOFlash;	// Save the flash status
#ifdef SUPPORT_RADIO_RDS
DWORD _dwRDSFlashTime;	// Save time for RDS information update
DWORD _dwRDSWaitTime;		// Save time as a delay count for the next RDS info. display    
WORD _wRDSDataPos;	// Save current RDS data position
BYTE _bRDSDataRemainNum;	// Save the number of non-displayed RDS data
BYTE _bCurRDSType;	// Save current displaying RDS info.
#endif

// Define channel start freq.
//WORD _wRADIOStartFreq[3] = {8750, 7600, 7600};
WORD _wRADIOStartFreq = 8750;
// Define channel end freq.
//WORD _wRADIOEndFreq[3] = {10800, 10800, 9000};
WORD _wRADIOEndFreq = 10800;
// Define real range of channel band.
//WORD _wRADIOFreqRange[3] = {10800-8750, 10800-7600, 9000-7600};
WORD _wRADIOFreqRange = 10800 - 8750;
// Define real value of channel space.
//BYTE _bRADIOSpaceValue[3] = {20, 10, 5};
BYTE _bRADIOSpaceValue = 10;

const  RADIO_MODE_BUTTON RADIOModeButton[RADIO_MODE_NUM] = 
{
    {aMENUNormal, RADIO_BUTTON_NORMAL_START_H, RADIO_BUTTON_NORMAL_START_V, RADIO_MODE_NORMAL},
    {aRADIORecord, RADIO_BUTTON_RECORD_START_H, RADIO_BUTTON_RECORD_START_V, RADIO_MODE_RECORD},    		
    {aPOWERONIconFavorite, RADIO_BUTTON_FAVORITE_START_H, RADIO_BUTTON_FAVORITE_START_V, RADIO_MODE_FAVORITE},    
    {aRADIOScan, RADIO_BUTTON_SCAN_START_H, RADIO_BUTTON_SCAN_START_V, RADIO_MODE_SCAN}, 
};

extern  DWORD __dwVolAudio;

#ifdef SUPPORT_RADIO_RDS
//BYTE aRDSTemplate[] = {28, 'F', 'M', ' ', 'R', 'a', 'd', 'i', 'o', ' ', 'W', 'e', 'l', 'c', 'o', 'm', 'e', ' ', 't', 'o', ' ', 'C', 'h', 'e', 'e', 'r', 't', 'e', 'k'}; 
BYTE aGetRDSData[65];
WORD aConvRDSCT[30]; // clock time and date
WORD aConvRDSPS[9]; //program service name has 8 characters from RDS spec.
WORD aConvRDSPTY[10]; //Program type code has 8 character from RDS spec. the bRADIODRV_RDS_PTY[0] using PTY length
//WORD aConvRDSCT[65]; // clock time and date
//WORD aConvRDSPS[65]; //program service name has 8 characters from RDS spec.
//WORD aConvRDSPTY[65]; //Program type code has 8 character from RDS spec. the bRADIODRV_RDS_PTY[0] using PTY length
WORD aConvRDSRT[65]; //RADIO TEXT has 64 characters from RDS spec and bRADIODRV_RWORD aConvRDSRT[65]; //RADIO TEXT has 64 characters from RDS spec and bRADIODRV_R
WORD aShowRDSData[65];

WORD *aConvRDSData[] =
{
    aConvRDSCT,
    aConvRDSPS,
    aConvRDSPTY,
    aConvRDSRT,    
};

BYTE SupportRDSType[] = 
{   
    RDS_CT,
    RDS_PS,    
    RDS_PTY,
    RDS_RT,    
};

// The order that save each RDS update status is the same as SupportRDSType[].
BYTE RDSUpdateStatus[RDS_TYPE_NUMBER];  // Save each RDS info. update status.

WORD *aWeekDay[7] = 
{    
    aWeeklyDay_Mon,
    aWeeklyDay_Tue,
    aWeeklyDay_Wed,
    aWeeklyDay_Thu,
    aWeeklyDay_Fri,
    aWeeklyDay_Sat,    
    aWeeklyDay_Sun
};
#endif


//*********************************************************************************************************
//  Function    :   RADIO_Entry
//  Abstract    :   This is the entry function of radio.
//  Arguments   :   none.
//  Return		:   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void RADIO_Entry(BYTE bRadioMode)
{  
#ifdef SUPPORT_RADIO_RDS
    BYTE bTemp;
    //GDI_REGION_INFO RegionInfo;
#endif
	
    //RADIO_ConfigOSDRegion();
    OSD_ChangeUI(OSD_UI_RADIO_TUNER, OSD_ENTER_UI);

    if (!RADIO_Initial(TRUE))
    {
        POWERONMENU_Initial();
        return; 
    }
    
    _RADIO_ReadData();	
    
    if (bRadioMode == RADIO_MODE_FAVORITE)
    {
        RADIODRV_tune(_wRADIOFavorite[0]);
    }
    else
    {
        // Broadcast backup freq.
        // Backup freq. save the last played freq. before user leave radio function.    
        RADIODRV_tune(_wRADIOBackupFreq);
    }

    __bPlayRadio = TRUE;
    _bRADIOButtonCursorIndex = bRadioMode;
    _bRADIOFavoritePlay = 0;
    _bRecordHighlightPos = 0;
    _bRADIOMode = bRadioMode;
    _bRADIOBackupMode = _bRADIOMode;
#ifdef SUPPORT_RADIO_RDS
    _bRDSDataRemainNum = 0;
    _wRDSDataPos = RADIO_RDS_INFO_START_H;
    _bCurRDSType = 0;
    _dwRDSWaitTime = OS_GetSysTimer();
	
    for (bTemp = 0; bTemp < RDS_TYPE_NUMBER; bTemp++)
    {
        RDSUpdateStatus[bTemp] = 0; //Initial the RDS update status.
    }    
	
    // Config RDS display region.
    //RegionInfo.wWidth = RADIO_RDS_REGION_WIDTH;
    //RegionInfo.wHeight = RADIO_RDS_REGION_HEIGHT;
    //RegionInfo.bColorMode = GDI_OSD_8B_MODE;
    //RegionInfo.dwTAddr = RADIO_RDS_REGION_ADDR;    
    //GDI_ConfigRegionInfo(2, &RegionInfo);    //Borrow Region 2 (SP OSD). Need recover when exit radio.  
#endif	
    // Load radio background BMP data. 
    if (!_RADIO_LoadBmpData())
    {
        printf("Can't find radio BMP data from flash!!\n");
        POWERONMENU_Initial();
        return; 
    }
   
    RADIO_ShowFrame();    
}

//*********************************************************************************************************
//  Function    :   RADIO_ProcessKey
//  Abstract    :   Call this function to process the keys.
//  Arguments  :   none.
//  Return	 :   KEY_NO_KEY: The input key is processed.
//                 :   INVALID_KEY: The input key is not allowed.
//                 :   KEY_BYPASS: Will not process this key and pass it to main flow to process it.
//                 :   KEY_XXX: The main program need do more process for the key.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE RADIO_ProcessKey(void)
{
    if (!__bPlayRadio)
    {
        return KEY_BYPASS;
    }

    if (__bKey == KEY_POWER)
    {
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        // Save current playing radio frequency.
        _wRADIOBackupFreq = RADIODRV_GetFreq();
        HAL_WriteStorage(RADIO_ADDR_BACKUP_FREQ, (BYTE *)&_wRADIOBackupFreq, EPROM_BACKUP_FREQ_BYTE);
#endif //#ifdef WRITE_EEPROM_ENABLE 
        return KEY_BYPASS;
    }

    _dwRadioFlashTime = OS_GetSysTimer();

    if ((__bKey == KEY_VOL_UP) || (__bKey == KEY_VOL_DOWN) || (__bKey == KEY_MUTE))
    {
        _RADIO_VolumeControl(__bKey);
        return KEY_NO_KEY;
    }

#ifdef SUPPORT_RADIO_RDS	
    // When radio frequency are changed, reset RDS status.
    if ((_bRADIOMode == RADIO_MODE_NORMAL) || (_bRADIOMode == RADIO_MODE_FAVORITE))
    {
        if ((__bKey == KEY_UP) || (__bKey == KEY_DOWN) || (__bKey == KEY_RIGHT) || (__bKey == KEY_LEFT))
        {
             _RADIO_ResetRDS(TRUE);
        }
    }
#endif	

    switch (_bRADIOMode)
    {
    case RADIO_MODE_NORMAL:
        _RADIO_NormalModeProcess(__bKey);
        break;
    case RADIO_MODE_FAVORITE:
        _RADIO_FavoriteModeProcess(__bKey);
        break;
    case RADIO_MODE_RECORD:        
        _RADIO_RecordModeProcess(__bKey);
        break;
    case RADIO_MODE_SCAN:	        
        break;
     case RADIO_MODE_SELECTION:		
        _RADIO_ModeSelectionProcess(__bKey);
        break;
    default:
        break;
    }

    return KEY_NO_KEY;
}

//*********************************************************************************************************
//  Function    :   RADIO_Recover
//  Abstract    :   The function is called by OSD to do the recovery of radio UI 
//  Arguments   :   bRecoverRegion, the recover region 
//                   :   OSD_RECOVER_ALL 
//                   :   OSD_RECOVER_UPPER_REGION, invalid
//                   :   OSD_RECOVER_BOTTOM_REGION, invalid
//  Return      :   TRUE, The region has been recovered.
//                 :   FALSE, The region doesn't need to be recovered in this UI.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE RADIO_Recover(BYTE bRecoverRegion)
{
    if (bRecoverRegion != OSD_RECOVER_ALL) 
    {
    	return FALSE;
    }    

    //RADIO_ConfigOSDRegion();
    RADIO_Initial(FALSE);
    RADIO_ShowFrame();
    //Recover forever message (mute).
    OSD_RecoverNBForeverMsg();        	

    switch (_bRADIOMode)
    {
    case RADIO_MODE_NORMAL:       
    case RADIO_MODE_FAVORITE:
    case RADIO_MODE_SCAN:  
        break;
    case RADIO_MODE_RECORD:        
        _RADIO_UpdateFrequencyBar(TRUE, RADIO_COLOR_HIGHLIGHT);
        break;
     case RADIO_MODE_SELECTION:		
        _RADIO_UpdateFrequencyBar(TRUE, RADIO_COLOR_HIGHLIGHT);
        _RADIO_UpdateButtonCursor(_bRADIOButtonCursorIndex, RADIO_COLOR_HIGHLIGHT, RADIO_COLOR_HIGHLIGHT);	
        break;
    default:
        break;    
    }

    return TRUE;
}

//*********************************************************************************************************
//  Function    :   RADIO_Trigger
//  Abstract    :   This function will flash freq. arrow, record list, mode button, and RDS info.
//  Arguments   :   
//  Return	:               
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void RADIO_Trigger(void)
{
    switch (_bRADIOMode)
    {
    case RADIO_MODE_NORMAL:       
    case RADIO_MODE_FAVORITE:
        //Flash the frequency arrow.            
        if ((OS_GetSysTimer()-_dwRadioFlashTime) > RADIO_CURSOR_FLASH_TIME)
        {
            if (_bRADIOFlash)
            {
                _RADIO_UpdateFrequencyBar(FALSE, RADIO_COLOR_NORMAL);
            }
            else
            {
                _RADIO_UpdateFrequencyBar(FALSE, RADIO_COLOR_HIGHLIGHT);
            }            
        }
#ifdef SUPPORT_RADIO_RDS		
        //Update RDS information.
        if ((OS_GetSysTimer() - _dwRDSFlashTime) > RADIO_RDS_FLASH_TIME)
        {           
            _RADIO_UpdateRDSInfo(FALSE);		
        }	
#endif		
        break;
    case RADIO_MODE_RECORD:        
        //Flash the record list.            
        if ((OS_GetSysTimer()-_dwRadioFlashTime) > RADIO_CURSOR_FLASH_TIME)
        {
            if (_bRADIOFlash)
            {
                _RADIO_UpdateRecordInfo(FALSE, RADIO_COLOR_HIGHLIGHT, RADIO_COLOR_NORMAL);
            }
            else
            {
                _RADIO_UpdateRecordInfo(FALSE, RADIO_COLOR_HIGHLIGHT, RADIO_COLOR_HIGHLIGHT);
            }            
        }
        break;
     case RADIO_MODE_SELECTION:		
        //Flash the mode button.            
        if ((OS_GetSysTimer()-_dwRadioFlashTime) > RADIO_CURSOR_FLASH_TIME)
        {
            if (_bRADIOFlash)
            {
                _RADIO_UpdateButtonCursor(_bRADIOButtonCursorIndex, RADIO_COLOR_HIGHLIGHT, RADIO_COLOR_NORMAL);
            }
            else
            {
                _RADIO_UpdateButtonCursor(_bRADIOButtonCursorIndex, RADIO_COLOR_HIGHLIGHT, RADIO_COLOR_HIGHLIGHT);
            }            
        }   
#ifdef SUPPORT_RADIO_RDS		
        if ((_bRADIOBackupMode == RADIO_MODE_NORMAL) || (_bRADIOBackupMode == RADIO_MODE_FAVORITE))	
        {
            //Update RDS information.
            if ((OS_GetSysTimer() - _dwRDSFlashTime) > RADIO_RDS_FLASH_TIME)
            {             
                _RADIO_UpdateRDSInfo(FALSE);		
            }	
        }
#endif		
        break;
    default:
        break;    
    }    
}

//*********************************************************************************************************
//  Function    :   RADIO_Exit
//  Abstract    :   This function will be called by OSD kernel to exit radio UI.
//  Arguments   :  bExitMode: OSD_UI_EXIT_ALL/OSD_UI_EXIT_CONTROL_ONLY
//  Return	:               
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void RADIO_Exit(BYTE bExitMode)
{
#ifdef SUPPORT_RADIO_RDS
    //GDI_REGION_INFO RegionInfo;
#endif

    if (bExitMode == OSD_UI_EXIT_ALL)
    {
        //Clear all radio OSD region.
        GDI_ClearRegion(0);
    }
	
    __bPlayRadio = FALSE;
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
    // Save current playing radio frequency.
    _wRADIOBackupFreq = RADIODRV_GetFreq();
    HAL_WriteStorage(RADIO_ADDR_BACKUP_FREQ, (BYTE *)&_wRADIOBackupFreq, EPROM_BACKUP_FREQ_BYTE);
#endif //#ifdef WRITE_EEPROM_ENABLE 
    RADIODRV_PowerDown();

#ifdef SUPPORT_RADIO_RDS
    // Recover Region 2 (SP OSD).
    //RegionInfo.wWidth = GDI_SP_OSD_REGION_WIDTH;
    //RegionInfo.wHeight = GDI_SP_OSD_REGION_HEIGHT;
    //RegionInfo.bColorMode = GDI_OSD_2B_MODE;
    //RegionInfo.dwTAddr = DS_SP_OSD_ST;    
    //GDI_ConfigRegionInfo(2, &RegionInfo);     
#endif

    //Enable the main video.
    //DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
    OSD_ChangeUI(OSD_UI_RADIO_TUNER, OSD_EXIT_UI);
}

//*********************************************************************************************************
//  Function    :   RADIO_Initial
//  Abstract    :   Call this function to initial radio device and set attribute for display volume status.
//  Arguments   :   bPowerUp: (TRUE) enter radio function in first time
//                                      (FALSE) enter radio function not in first time
//  Return	:   TRUE: initial radio device successful
//                    FALSE: initial radio device failed
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE RADIO_Initial(BYTE bPowerUp)
{
    if (bPowerUp)
    {
        if (!RADIODRV_Init()) // When tuner initial failed, return false.
        {
            printf("\n tuner initial failed!!");
            return FALSE;
        }
    
        _bRADIOFreqRange = RADIO_FREQ_RANGE_US_EUROPE;
        _bRADIOSpace = RADIO_FREQ_SPACE_100K;
    	
        RADIODRV_SETBand(_bRADIOFreqRange);
        RADIODRV_SETChanSpace(_bRADIOSpace);
#ifdef SUPPORT_RADIO_RDS	
        RADIODRV_RDSInit();
#endif

        HAL_IOMute(FALSE);

        if (__btMute)
        {
            RADIODRV_SetMute(!__btMute);
            OSD_OUTPUT_MACRO(MSG_MUTE, __btMute, 0xFF);
        } 
    
        if ((__dwVolAudio/VOLUME_GAP) < 2)
        {
            RADIODRV_SetVol((BYTE)(__dwVolAudio/VOLUME_GAP)&0x0F);
        }
        else
        {
            RADIODRV_SetVol((BYTE)((__dwVolAudio/VOLUME_GAP)-1)&0x0F);
        }		
    }

     //Disable the main video.
    //DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
    POWERONMENU_ClearScreen();
	 
    //Set the attributes for volume message display.    
    __OSDNDBottomMsgAttr.wX = RADIO_VOLUME_START_H;
    __OSDNDBottomMsgAttr.wY = RADIO_VOLUME_START_V;
    //__OSDNDBottomMsgAttr.bBackground = RADIO_PALETTE_ENTRY_FRAME_BG;
    //__OSDNDBottomMsgAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_BLACK;
    //__OSDNDBottomMsgAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    //__OSDNDBottomMsgAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    //__OSDNDBottomMsgAttr.bColorKey = 0;

    return TRUE;
}

void RADIO_ShowFrame(void)
{    
    _RADIO_InitPalette();
    _RADIO_DrawFrameBackground();
    _RADIO_UpdateContent(RADIO_ALL_REGION, FALSE, RADIO_COLOR_HIGHLIGHT);
}

//*********************************************************************************************************
//  Function    :   RADIO_AutoScan
//  Abstract    :   This function will do radio auto scan and save found station to favorite.
//  Arguments   :   
//  Return	:               
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void RADIO_Scan(void)
{
    BYTE bIndex, bLoop;
    WORD wFreqTemp1, wFreqTemp2;

    if (!__btMute)
    {
        RADIODRV_SetMute(0); // Enable mute
    }
    
    bLoop = FALSE;
    wFreqTemp1 = RADIODRV_GetFreq();	// Save current frequency.
    wFreqTemp2 = wFreqTemp1;
    for (bIndex = 0; bIndex < RADIO_FAVORITE_NUM; bIndex++)
    {
        if (!RADIODRV_Seek(TRUE)) // Seek successful.
        {
            // Re-scan from the start point of the loop (8700~10800).
            if (RADIODRV_GetFreq() <= wFreqTemp2 && bLoop == FALSE)
            {
                bLoop = TRUE; 
            }
            // Already scan all frequency.
            else if (RADIODRV_GetFreq() <= wFreqTemp2 && bLoop == TRUE)
            {
                break;
            }
			
            // Already scan all frequency. 
            if (bLoop == TRUE && RADIODRV_GetFreq() >= wFreqTemp1)
            {
                if (RADIODRV_GetFreq() == wFreqTemp1)
                {
                    _bRADIOStationNum++;
                    _wRADIOFavorite[bIndex] = RADIODRV_GetFreq();
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
                    HAL_WriteStorage(RADIO_ADDR_FAVORITE+2*bIndex, (BYTE *)(_wRADIOFavorite)+2*bIndex, 2);
#endif //#ifdef WRITE_EEPROM_ENABLE 
                    _RADIO_UpdateContent(RADIO_FREQ_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);
                    _RADIO_UpdateInfoRegion(FALSE, FALSE, RADIO_COLOR_NORMAL);
                }
				
                break; 
            }
			
            wFreqTemp2 = RADIODRV_GetFreq();
            _bRADIOStationNum++;
            _wRADIOFavorite[bIndex] = RADIODRV_GetFreq();
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
            HAL_WriteStorage(RADIO_ADDR_FAVORITE+2*bIndex, (BYTE *)(_wRADIOFavorite)+2*bIndex, 2);
#endif //#ifdef WRITE_EEPROM_ENABLE 
            _RADIO_UpdateContent(RADIO_FREQ_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);
            _RADIO_UpdateInfoRegion(FALSE, FALSE, RADIO_COLOR_NORMAL);
        }
        else // Seek failed
        {
            break;
        }
    }

    if (!__btMute)
    {
        RADIODRV_SetMute(1); // Disable mute
    }	
    
    if (_bRADIOStationNum == 0) // Not find any stations.
    {
        _bRADIOFavoritePlay = _bRADIOStationNum;
    }
    else
    {
        _bRADIOFavoritePlay = _bRADIOStationNum - 1;
    }
    _bRADIOButtonCursorIndex = RADIO_MODE_FAVORITE; // Set the cursor index to favorite button.
    RADIODRV_tune(_wRADIOFavorite[_bRADIOFavoritePlay]);
    _bRADIOMode = RADIO_MODE_FAVORITE;
    _RADIO_UpdateContent(RADIO_ALL_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);	
}

//********************************************************************************************************
//  Function    :   RADIO_ClearRegion
//  Abstract    :   This function will be called by OSD to clear the normal bottom 
//              :   region if the UI accepts the normal display message is displayed in its UI.
//  Arguments   :   bRegion: OSD_ND_BOTTOM_REGION.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void RADIO_ClearRegion(BYTE bRegion)
{
    PARM_RECT NBRegionRect;

    if (bRegion == OSD_ND_BOTTOM_REGION)
    {
         NBRegionRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
         NBRegionRect.rect.wLeft = OSDND_BOTTOM_BACKGROUND_START_H;
         NBRegionRect.rect.wTop = RADIO_VOLUME_START_V;
         NBRegionRect.rect.wRight = OSDND_BOTTOM_BACKGROUND_START_RIGHT;
         NBRegionRect.rect.wBottom = NBRegionRect.rect.wTop+OSDND_BOTTOM_BACKGROUND_HEIGHT;
         // Clear the normal bottom message.
         GDI_FillRect(0, &NBRegionRect);        
    }
}

//  ***************************************************************************
//  Function    :   _RADIO_ConfigOSDRegion
//  Description :   This function will config the OSD region. And it is exported for OSD kernel usage.
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void RADIO_ConfigOSDRegion(void)
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

//*********************************************************************************************************
//  Function    :   _RADIO_NormalModeProcess
//  Abstract    :   Call this function to process the keys in normal mode.
//  Arguments  :   bKey: input key
//  Return	 :   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _RADIO_NormalModeProcess(BYTE bKey)
{
    WORD RadioFreq;
	
    switch (bKey)
    { 
    case KEY_STOP:
        _bRADIOBackupMode = _bRADIOMode;
        _bRADIOMode = RADIO_MODE_SELECTION;
        _RADIO_UpdateFrequencyBar(FALSE, RADIO_COLOR_HIGHLIGHT);
        _RADIO_UpdateButtonCursor(_bRADIOButtonCursorIndex, RADIO_COLOR_HIGHLIGHT, RADIO_COLOR_HIGHLIGHT);        
        break;
    case KEY_UP:
        if (!RADIODRV_Seek(TRUE)) // Seek successful.
        {            
            _RADIO_UpdateContent(RADIO_FREQ_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);          
        }
        // If seek failure, do noting.       
        break;
    case KEY_DOWN:
        if (!RADIODRV_Seek(FALSE)) // Seek successful.
        {            
            _RADIO_UpdateContent(RADIO_FREQ_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);          
        }
        // If seek failure, do noting.
        break;
    case KEY_LEFT:
        RadioFreq = RADIODRV_GetFreq();
        RadioFreq = RadioFreq - _bRADIOSpaceValue;
        if (RadioFreq < _wRADIOStartFreq)
        {
            RadioFreq = _wRADIOEndFreq;
        }
        RADIODRV_tune(RadioFreq);
        _RADIO_UpdateContent(RADIO_FREQ_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);
        break;
    case KEY_RIGHT:
        RadioFreq = RADIODRV_GetFreq();
        RadioFreq = RadioFreq + _bRADIOSpaceValue;
        if (RadioFreq > _wRADIOEndFreq)
        {
            RadioFreq = _wRADIOStartFreq;
        }
        RADIODRV_tune(RadioFreq);
        _RADIO_UpdateContent(RADIO_FREQ_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);     
        break;
    default:
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _RADIO_FavoriteModeProcess
//  Abstract    :   Call this function to process the keys in favorite mode.
//  Arguments  :   bKey: input key
//  Return	 :   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _RADIO_FavoriteModeProcess(BYTE bKey)
{
    switch (bKey)
    { 
    case KEY_STOP:  
        _bRADIOBackupMode = _bRADIOMode;
        _bRADIOMode = RADIO_MODE_SELECTION;
        _RADIO_UpdateFrequencyBar(FALSE, RADIO_COLOR_HIGHLIGHT);
        _RADIO_UpdateButtonCursor(_bRADIOButtonCursorIndex, RADIO_COLOR_HIGHLIGHT, RADIO_COLOR_HIGHLIGHT);        
        break;
    case KEY_UP:
    case KEY_RIGHT:
        if (_bRADIOFavoritePlay < RADIO_FAVORITE_NUM-1)
        {
            _bRADIOFavoritePlay++;
        }
        else
        {
            _bRADIOFavoritePlay = 0;
        }
        RADIODRV_tune(_wRADIOFavorite[_bRADIOFavoritePlay]);       
        _RADIO_UpdateContent(RADIO_FREQ_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);        
        _RADIO_UpdateContent(RADIO_MODE_NAME_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);
        break;
    case KEY_DOWN:
    case KEY_LEFT:
        if (_bRADIOFavoritePlay > 0)
        {
            _bRADIOFavoritePlay--;
        }
        else
        {
            _bRADIOFavoritePlay = RADIO_FAVORITE_NUM-1;
        }
        RADIODRV_tune(_wRADIOFavorite[_bRADIOFavoritePlay]);            
        _RADIO_UpdateContent(RADIO_FREQ_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);        
        _RADIO_UpdateContent(RADIO_MODE_NAME_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);
        break;       
    default:
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _RADIO_RecordModeProcess
//  Abstract    :   Call this function to process the keys in record mode.
//  Arguments  :   bKey: input key
//  Return	 :   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _RADIO_RecordModeProcess(BYTE bKey)
{
    switch (bKey)
    { 
    case KEY_STOP:  
        _bRADIOBackupMode = _bRADIOMode;
        _RADIO_UpdateInfoRegion(FALSE, FALSE, RADIO_COLOR_NORMAL);
        _bRADIOMode = RADIO_MODE_SELECTION;
        //_RADIO_UpdateFrequencyBar(FALSE, RADIO_COLOR_NORMAL);        
        _RADIO_UpdateButtonCursor(_bRADIOButtonCursorIndex, RADIO_COLOR_HIGHLIGHT, RADIO_COLOR_HIGHLIGHT);        
        break;
    case KEY_UP:
        if (_bRecordHighlightPos > 0)
        {
            _bRecordHighlightPos--;
        }
        else
        {
            _bRecordHighlightPos = RADIO_FAVORITE_NUM-1;
        }
        _RADIO_UpdateInfoRegion(FALSE, FALSE, RADIO_COLOR_HIGHLIGHT);
        break;
    case KEY_DOWN:
        if (_bRecordHighlightPos < RADIO_FAVORITE_NUM-1)
        {
            _bRecordHighlightPos++;
        }       
        else
        {
            _bRecordHighlightPos = 0;
        }
        _RADIO_UpdateInfoRegion(FALSE, FALSE, RADIO_COLOR_HIGHLIGHT);
        break;
    case KEY_RIGHT:        
        _bRecordHighlightPos = (_bRecordHighlightPos + 5) % 20;       
        _RADIO_UpdateInfoRegion(FALSE, FALSE, RADIO_COLOR_HIGHLIGHT);        
        break;
    case KEY_LEFT:
        _bRecordHighlightPos = (_bRecordHighlightPos + RADIO_FAVORITE_NUM - 5) % 20;
        _RADIO_UpdateInfoRegion(FALSE, FALSE, RADIO_COLOR_HIGHLIGHT);
        break;
    case KEY_ENTER:
    case KEY_PLAY:
        _bRADIOButtonCursorIndex = RADIO_MODE_NORMAL; // Set the cursor index to normal button.
        _wRADIOFavorite[_bRecordHighlightPos] = RADIODRV_GetFreq();
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(RADIO_ADDR_FAVORITE+2*_bRecordHighlightPos, (BYTE *)(_wRADIOFavorite)+2*_bRecordHighlightPos, 2);
#endif //#ifdef WRITE_EEPROM_ENABLE 
        _bRADIOMode = RADIO_MODE_NORMAL;
        _RADIO_UpdateContent(RADIO_ALL_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);
        break;
    default:
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _RADIO_ModeSelectionProcess
//  Abstract    :   Call this function to process the keys in mode selection mode.
//  Arguments  :   bKey: input key
//  Return	 :   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _RADIO_ModeSelectionProcess(BYTE bKey)
{
    switch (bKey)
    {
    case KEY_UP:
        _RADIO_GotoUpButton();
        break;
    case KEY_DOWN:
        _RADIO_GotoDownButton();
        break;
/*
    case KEY_LEFT:
        _RADIO_GotoLeftButton();
        break;
    case KEY_RIGHT:
        _RADIO_GotoRightButton();
        break;
*/
    case KEY_ENTER:
    case KEY_PLAY:
        _RADIO_ChangeMode();
        break;
    default:
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _RADIO_InitPalette
//  Abstract    :   This function will initialize the palette for radio UI.
//  Arguments  :   
//  Return	 :   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _RADIO_InitPalette(void)
{ 
    
    GDI_PALETTE_INFO PaletteInfo_Radio_BG = {RADIO_BITMAP_PALETTE_INDEX_START, PAL_ADDR, _aRADIOBG_Palette}; 
    //GDI_PALETTE_INFO PaletteInfo_Radio_NButton = {RADIO_NBUTTON_PALETTE_INDEX_START, PAL_ADDR, _aRADIONButton_Palette};	
    //GDI_PALETTE_INFO PaletteInfo_Radio_HButton = {RADIO_HBUTTON_PALETTE_INDEX_START, PAL_ADDR, _aRADIOHButton_Palette};	

    // Load palette for radio background photo.
    GDI_LoadPalette(&PaletteInfo_Radio_BG);    
    GDI_ChangePALEntry(RADIO_BITMAP_PALETTE_BG, 0x5a000000, FALSE);
    GDI_WaitPaletteComplete();
/*
    // Load palette for radio normal button photo.
    GDI_LoadPalette(&PaletteInfo_Radio_NButton);    
    GDI_ChangePALEntry(RADIO_NBUTTON_PALETTE_BG, PAL_ENTRY_COLOR_TRANSPARENT, FALSE);
    GDI_WaitPaletteComplete();      
    // Load palette for radio highlight button photo.
    GDI_LoadPalette(&PaletteInfo_Radio_HButton);    
    GDI_ChangePALEntry(RADIO_HBUTTON_PALETTE_BG, PAL_ENTRY_COLOR_TRANSPARENT, FALSE);
    GDI_WaitPaletteComplete();
*/
    MAINMENU_LoadGroupPalette(RADIO_PALETTE_ENTRY_NORMAL_BUTTON_BEGIN, _dwRADIONormalButtonPalette, FALSE);
    MAINMENU_LoadGroupPalette(RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BEGIN, _dwRADIOHighlightButtonPalette, FALSE);

    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_HIGHLIGHT_BG, RADIO_PALETTE_COLOR_HIGHLIGHT_BG, FALSE);
    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_TEXT_BLACK, RADIO_PALETTE_COLOR_TEXT_BLACK, FALSE);
    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_TEXT_WHITE, RADIO_PALETTE_COLOR_TEXT_WHITE, FALSE);
    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_TEXT_GRAY, RADIO_PALETTE_COLOR_TEXT_GRAY, FALSE);
    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_TEXT_NAVYBLUE, RADIO_PALETTE_COLOR_TEXT_NAVYBLUE, FALSE);
    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_FREQ_BAR_HIGHLIGHT, RADIO_PALETTE_COLOR_FREQ_BAR_HIGHLIGHT, FALSE);
    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_FREQ_BAR_NORMAL, RADIO_PALETTE_COLOR_FREQ_BAR_NORMAL, FALSE);
    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_FREQ_FRAME_BG, RADIO_PALETTE_COLOR_FREQ_FRAME_BG, FALSE);
    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_FREQ_FRAME_BG_OUTLINE, RADIO_PALETTE_COLOR_FREQ_FRAME_BG_OUTLINE, FALSE);
    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_NBUTTON_OUTLINE, RADIO_PALETTE_COLOR_NBUTTON_OUTLINE, FALSE);   	
    GDI_ChangePALEntry(RADIO_PALETTE_ENTRY_HBUTTON_OUTLINE, RADIO_PALETTE_COLOR_HBUTTON_OUTLINE, FALSE);  
	
    GDI_WaitPaletteComplete();
}

//*********************************************************************************************************
//  Function    :   _RADIO_ReadData
//  Abstract    :   read backup freq. & favorite data from EEPROM/Flash and do some protection.
//  Arguments   :   none.
//  Return		:   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _RADIO_ReadData(void)
{
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
    BYTE bIndex;

    HAL_ReadStorage(RADIO_ADDR_BACKUP_FREQ, (BYTE *)&_wRADIOBackupFreq, 2);
    HAL_ReadStorage(RADIO_ADDR_FAVORITE, (BYTE *)_wRADIOFavorite, RADIO_FAVORITE_NUM*2);

    if ((_wRADIOBackupFreq < _wRADIOStartFreq) || (_wRADIOBackupFreq > _wRADIOEndFreq))
    {
        _wRADIOBackupFreq = RADIO_DEFAULT_FREQ;
    }

    for (bIndex = 0; bIndex < RADIO_FAVORITE_NUM; bIndex++)
    {
        if ((_wRADIOFavorite[bIndex] < _wRADIOStartFreq) || (_wRADIOFavorite[bIndex] > _wRADIOEndFreq))
        {
            _wRADIOFavorite[bIndex] = RADIO_DEFAULT_FREQ;
        }
    }
#endif //#ifdef WRITE_EEPROM_ENABLE 
}

void _RADIO_DrawFrameBackground(void)
{    
    PARM_DRAW_BITMAP DrawBitmap;          
   
    // Draw radio background photo.
    DrawBitmap.bBitmapID = 0;        
    DrawBitmap.wDestX = RADIO_BG_PHOTO_START_H;
    DrawBitmap.wDestY = RADIO_BG_PHOTO_START_V;        
    DrawBitmap.bDestRegion = 0;
    
    GDI_DrawZipBitmap(&DrawBitmap, _aRADIOArray, (PBYTE)RADIO_UNZIP_ADDR, RADIO_UNZIP_WINDOW_ADDR);
 
    _RADIO_DrawFreqFrame();
    _RADIO_ShowModeButton();
}

void _RADIO_DrawFreqFrame(void)
{
    BYTE i;
    PARM_RECT FreqRect;
    PARM_DRAW_STRING FreqStrAttr;
    WORD BarWidthTemp, BarStartHTemp;

    FreqRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
    FreqRect.rect.wLeft = RADIO_BAND_FRAME_START_H;
    FreqRect.rect.wTop = RADIO_BAND_FRAME_START_V;
    FreqRect.rect.wRight = FreqRect.rect.wLeft+RADIO_BAND_FRAME_WIDTH-1;
    FreqRect.rect.wBottom = FreqRect.rect.wTop+RADIO_BAND_FRAME_HEIGHT-1;
    // Fill band frame.
    GDI_FillRoundRect(0, RADIO_BAND_FRAME_ARC_VALUE, &FreqRect);

    FreqRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG_OUTLINE;
    FreqRect.rect.wLeft = FreqRect.rect.wLeft-RADIO_ARC_PIXEL;
    FreqRect.rect.wTop = FreqRect.rect.wTop-RADIO_ARC_PIXEL;
    FreqRect.rect.wRight = FreqRect.rect.wRight+RADIO_ARC_PIXEL;
    FreqRect.rect.wBottom = FreqRect.rect.wBottom+RADIO_ARC_PIXEL;
    // Draw band frame ourter line.
    GDI_DrawRoundRect(0, RADIO_BAND_FRAME_OUTER_ARC_VALUE, RADIO_BAND_FRAME_ARC_VALUE, &FreqRect);

    FreqRect.bColor = RADIO_PALETTE_ENTRY_FREQ_BAR_NORMAL;
    FreqRect.rect.wLeft = RADIO_FREQ_BAR_START_H;
    FreqRect.rect.wTop = RADIO_FREQ_BAR_START_V;
    FreqRect.rect.wRight = FreqRect.rect.wLeft+RADIO_FREQ_BAR_WIDTH+RADIO_DISTANCE_OFFSET20-1;
    FreqRect.rect.wBottom = FreqRect.rect.wTop+RADIO_FREQ_BAR_HEIGHT-1;
    // Fill frequency bar.
    GDI_FillRect(0, &FreqRect);

    FreqRect.bColor = RADIO_PALETTE_ENTRY_FREQ_BAR_HIGHLIGHT;
    FreqRect.rect.wLeft = RADIO_FREQ_BAR_START_H-RADIO_DISTANCE_OFFSET20;
    FreqRect.rect.wRight = RADIO_FREQ_BAR_START_H-1;
    // Fill frequency bar.
    GDI_FillRect(0, &FreqRect);

    FreqRect.bColor = RADIO_PALETTE_ENTRY_TEXT_BLACK;    
    FreqRect.rect.wTop = RADIO_FREQ_BAR_START_V+RADIO_FREQ_BAR_HEIGHT;
    FreqRect.rect.wBottom = FreqRect.rect.wTop+12-1;

    FreqStrAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_BLACK;
    FreqStrAttr.bBackground = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
    FreqStrAttr.bColorKey = 0;
    FreqStrAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    FreqStrAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    FreqStrAttr.wY = FreqRect.rect.wTop+RADIO_DISTANCE_OFFSET10;    

    BarWidthTemp = RADIO_FREQ_BAR_WIDTH*200/205;
    BarStartHTemp = RADIO_FREQ_BAR_START_H+RADIO_FREQ_BAR_WIDTH-BarWidthTemp-1;

    FreqRect.rect.wLeft = RADIO_FREQ_BAR_START_H;
    FreqRect.rect.wRight = FreqRect.rect.wLeft+1;
    // Fill the first frequency large scale (87.5MHz).
    GDI_FillRect(0, &FreqRect);
    FreqStrAttr.wX = FreqRect.rect.wLeft-(GDI_GetStringWidth(_aRADIOUSEuropeBand[0])>>1);        
    // Show the value of frequency scale.
    GDI_DrawString_909(0, &FreqStrAttr, _aRADIOUSEuropeBand[0]); 

    for (i = 1; i < 5; i++)
    {        
        FreqRect.rect.wLeft = BarStartHTemp+(i*(BarWidthTemp>>2));       
        FreqRect.rect.wRight = FreqRect.rect.wLeft+1;        
        // Fill frequency large scale.
        GDI_FillRect(0, &FreqRect);       
        FreqStrAttr.wX = FreqRect.rect.wLeft-(GDI_GetStringWidth(_aRADIOUSEuropeBand[i])>>1);        
        // Show the value of frequency scale.
        GDI_DrawString_909(0, &FreqStrAttr, _aRADIOUSEuropeBand[i]);      
    }
    
    FreqRect.rect.wBottom = FreqRect.rect.wTop+5-1;
	
    for (i = 0; i < 4; i++)
    {        
        FreqRect.rect.wLeft = BarStartHTemp+(BarWidthTemp>>3)+(i*(BarWidthTemp>>2));       
        FreqRect.rect.wRight = FreqRect.rect.wLeft+1;        
        // Fill frequency small scale.
        GDI_FillRect(0, &FreqRect);
    }  
}

void _RADIO_ShowModeButton(void)
{
    //PARM_DRAW_BITMAP DrawButton;
    PARM_RECT ButtonRect;
    PARM_DRAW_STRING ButtonStrAttr;
    BYTE bButtonIndex;         

    MAINMENU_UseColorKey(&ButtonStrAttr);
    ButtonStrAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_WHITE;
    ButtonRect.bColor = RADIO_BUTTON_HEIGHT;
    ButtonRect.bShadePtr = _bRADIONormalButtonPalette;

    //DrawButton.bBitmapID = 4;
    for (bButtonIndex = 0; bButtonIndex < RADIO_MODE_NUM; bButtonIndex++)
    {
/*
        // Draw radio background photo.                
        DrawButton.wDestX = RADIOModeButton[bButtonIndex].wStartH;
        DrawButton.wDestY = RADIOModeButton[bButtonIndex].wStartV;        
        DrawButton.bDestRegion = 0;
    
        GDI_DrawZipBitmap(&DrawButton, _aRADIOArray, (PBYTE)RADIO_UNZIP_ADDR, RADIO_UNZIP_WINDOW_ADDR);
*/             
        ButtonRect.rect.wLeft = RADIOModeButton[bButtonIndex].wStartH;
        ButtonRect.rect.wTop = RADIOModeButton[bButtonIndex].wStartV;
        ButtonRect.rect.wRight = ButtonRect.rect.wLeft+RADIO_BUTTON_WIDTH-1;
        ButtonRect.rect.wBottom = ButtonRect.rect.wTop+RADIO_BUTTON_HEIGHT-1;
        // Fill button.
        GDI_FillRoundRect(0|GDI_SHADING_OPERATION, RADIO_BUTTON_ARC_VALUE, &ButtonRect);  

        ButtonRect.bColor = RADIO_PALETTE_ENTRY_NBUTTON_OUTLINE;
        ButtonRect.rect.wLeft = ButtonRect.rect.wLeft-RADIO_ARC_PIXEL;
        ButtonRect.rect.wTop = ButtonRect.rect.wTop-RADIO_ARC_PIXEL;
        ButtonRect.rect.wRight = ButtonRect.rect.wRight+RADIO_ARC_PIXEL;
        ButtonRect.rect.wBottom = ButtonRect.rect.wBottom+RADIO_ARC_PIXEL;
        // Draw button ourter line.
        GDI_DrawRoundRect(0, RADIO_BUTTON_OUTER_ARC_VALUE, RADIO_BUTTON_ARC_VALUE, &ButtonRect);

        ButtonStrAttr.wX = RADIOModeButton[bButtonIndex].wStartH+((RADIO_BUTTON_WIDTH-GDI_GetStringWidth(RADIOModeButton[bButtonIndex].pwText))>>1);
        ButtonStrAttr.wY = RADIOModeButton[bButtonIndex].wStartV+((RADIO_BUTTON_HEIGHT-GDI_GetStringHeight(RADIOModeButton[bButtonIndex].pwText))>>1);    
        // Show mode name.
        GDI_DrawString_909(0, &ButtonStrAttr, RADIOModeButton[bButtonIndex].pwText);	

/*		
        ButtonStrAttr.wX = DrawButton.wDestX+((RADIO_BUTTON_WIDTH-GDI_GetStringWidth(RADIOModeButton[bButtonIndex].pwText))>>1);
        ButtonStrAttr.wY = DrawButton.wDestY+((RADIO_BUTTON_HEIGHT-GDI_GetStringHeight(RADIOModeButton[bButtonIndex].pwText))>>1);    
        // Show mode name.
        GDI_DrawString_909(0, &ButtonStrAttr, RADIOModeButton[bButtonIndex].pwText);  
*/
    }    
}

void _RADIO_UpdateContent(BYTE bRegion, BYTE bClear, BYTE bArrowType)
{
    switch (bRegion)
    {    
    case RADIO_MODE_NAME_REGION:
        _RADIO_UpdateTitle(bClear);        
        break;
    case RADIO_FREQ_REGION:
        _RADIO_UpdateFreqString(bClear);
        _RADIO_UpdateFrequencyBar(bClear, bArrowType);
        break;
    case RADIO_INFO_REGION:
        _RADIO_UpdateInfoRegion(bClear, TRUE, RADIO_COLOR_HIGHLIGHT);
        break;
    case RADIO_ALL_REGION:
        _RADIO_UpdateTitle(bClear);
        _RADIO_UpdateFreqString(bClear);
        _RADIO_UpdateFrequencyBar(bClear, bArrowType);
        _RADIO_UpdateInfoRegion(bClear, TRUE, RADIO_COLOR_HIGHLIGHT);
        break;		
    default:
        break;
    }    
}

void _RADIO_UpdateTitle(BYTE bClear)
{
    PARM_DRAW_BITMAP DrawBitmap;
    PARM_DRAW_STRING TitleStrAttr;

    if (bClear)
    {
        // Draw title background photo.
        DrawBitmap.bBitmapID = 1;        
        DrawBitmap.wDestX = RADIO_TITLE_START_H;
        DrawBitmap.wDestY = RADIO_TITLE_START_V;        
        DrawBitmap.bDestRegion = 0;
    
        GDI_DrawZipBitmap(&DrawBitmap, _aRADIOArray, (PBYTE)RADIO_UNZIP_ADDR, RADIO_UNZIP_WINDOW_ADDR);
    }

    MAINMENU_UseColorKey(&TitleStrAttr);
    TitleStrAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_GRAY;   

    TitleStrAttr.wX = RADIO_TITLE_START_H;
    TitleStrAttr.wY = RADIO_TITLE_START_V; 
	
    switch (_bRADIOMode)
    {
    case RADIO_MODE_NORMAL:
        GDI_DrawString_909(0, &TitleStrAttr, aMENUNormal);
        break;
    case RADIO_MODE_FAVORITE:
        GDI_DrawString_909(0, &TitleStrAttr, aPOWERONIconFavorite);
        OSDUTL_PrepareOutputString();
        OSDUTL_CopyStringFromIndex(aRADIOColon);
        OSDUTL_CopyStringFromIndex(aRADIOSpace);
        if (_bRADIOFavoritePlay < 9)
        {
            OSDUTL_OutputNumFromIndex(0);
        }
        OSDUTL_OutputNumFromIndex(_bRADIOFavoritePlay+1);
        TitleStrAttr.wX = TitleStrAttr.wX+(GDI_GetStringWidth(aPOWERONIconFavorite));
        GDI_DrawString_909(0, &TitleStrAttr, __pOSDUTLOutputString);
        break;
    case RADIO_MODE_RECORD:        
        GDI_DrawString_909(0, &TitleStrAttr, aRADIORecord);
        break;
    case RADIO_MODE_SCAN:		
        GDI_DrawString_909(0, &TitleStrAttr, aRADIOScan);
        break;
    case RADIO_MODE_SELECTION:
        GDI_DrawString_909(0, &TitleStrAttr, RADIOModeButton[_bRADIOBackupMode].pwText);
        if (_bRADIOBackupMode == RADIO_MODE_FAVORITE)
        {
            OSDUTL_PrepareOutputString();
            OSDUTL_CopyStringFromIndex(aRADIOColon);
            OSDUTL_CopyStringFromIndex(aRADIOSpace);
            if (_bRADIOFavoritePlay < 9)
            {
                OSDUTL_OutputNumFromIndex(0);
            }
            OSDUTL_OutputNumFromIndex(_bRADIOFavoritePlay+1);
            TitleStrAttr.wX = TitleStrAttr.wX+(GDI_GetStringWidth(aPOWERONIconFavorite));
            GDI_DrawString_909(0, &TitleStrAttr, __pOSDUTLOutputString);
        }
        break;
    default:
        break;
    }
}

void _RADIO_UpdateFreqString(BYTE bClear)
{  
    PARM_DRAW_BITMAP DrawBitmap;
    PARM_DRAW_STRING FreqStrAttr;
    WORD RadioFreq;
	
    if (bClear)
    {
        // Draw frequency string background photo.
        DrawBitmap.bBitmapID = 2;        
        DrawBitmap.wDestX = RADIO_FREQ_STRING_START_H;
        DrawBitmap.wDestY = RADIO_FREQ_STRING_START_V;        
        DrawBitmap.bDestRegion = 0;
    
        GDI_DrawZipBitmap(&DrawBitmap, _aRADIOArray, (PBYTE)RADIO_UNZIP_ADDR, RADIO_UNZIP_WINDOW_ADDR);       
    }

    RadioFreq = RADIODRV_GetFreq();
	
    OSDUTL_PrepareOutputString();    
    OSDUTL_OutputNumFromIndex(RadioFreq/100);
    if ((RadioFreq%100) != 0)
    {
        OSDUTL_CopyStringFromIndex(aRADIODot);
        OSDUTL_OutputNumFromIndex((RadioFreq/10)%10);	
        if ((RadioFreq%10) != 0)
        {
            OSDUTL_OutputNumFromIndex(RadioFreq%10);
        }
    }

    MAINMENU_UseColorKey(&FreqStrAttr);
    FreqStrAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_NAVYBLUE;
	
    FreqStrAttr.wX = RADIO_FREQ_STRING_START_H+((RADIO_FREQ_STRING_WIDTH-GDI_GetStringWidth(__pOSDUTLOutputString))>>1)-2;
    FreqStrAttr.wY = RADIO_FREQ_STRING_START_V;    
    // Show current playing frequency.
    GDI_DrawString_909(0, &FreqStrAttr, __pOSDUTLOutputString);  
}

void _RADIO_UpdateFrequencyBar(BYTE bClear, BYTE bArrowType)
{
    PARM_RECT FreqRect;
    DWORD RadioFreq;
    PARM_DRAW_STRING FreqStrAttr;

    if (bClear)
    {
        FreqRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
        FreqRect.rect.wLeft = RADIO_BAND_FRAME_START_H+RADIO_DISTANCE_OFFSET10;
        FreqRect.rect.wTop = RADIO_FREQ_ARROW_START_V;
        FreqRect.rect.wRight = RADIO_BAND_FRAME_START_H+RADIO_BAND_FRAME_WIDTH-RADIO_DISTANCE_OFFSET10-1;
        FreqRect.rect.wBottom = FreqRect.rect.wTop+GDI_GetStringHeight(aDSPLYDownArrowSymbol)-1;
        // Clear band arrow region.
        GDI_FillRect(0, &FreqRect);
		
        FreqRect.bColor = RADIO_PALETTE_ENTRY_FREQ_BAR_NORMAL;
        FreqRect.rect.wLeft = RADIO_FREQ_BAR_START_H;
        FreqRect.rect.wTop = RADIO_FREQ_BAR_START_V;
        FreqRect.rect.wRight = FreqRect.rect.wLeft+RADIO_FREQ_BAR_WIDTH-1;
        FreqRect.rect.wBottom = FreqRect.rect.wTop+RADIO_FREQ_BAR_HEIGHT-1;
        // Clear frequency bar.
        GDI_FillRect(0, &FreqRect);
    }
    
    RadioFreq = (DWORD)(RADIODRV_GetFreq());

    RadioFreq = (DWORD)(RADIO_FREQ_BAR_WIDTH*(RadioFreq-_wRADIOStartFreq)/_wRADIOFreqRange);
    FreqRect.bColor = RADIO_PALETTE_ENTRY_FREQ_BAR_HIGHLIGHT;
    FreqRect.rect.wLeft = RADIO_FREQ_BAR_START_H;
    FreqRect.rect.wTop = RADIO_FREQ_BAR_START_V;
    FreqRect.rect.wRight = FreqRect.rect.wLeft+(WORD)RadioFreq-1;
    FreqRect.rect.wBottom = FreqRect.rect.wTop+RADIO_FREQ_BAR_HEIGHT-1;
    // Fill frequency bar.
    GDI_FillRect(0, &FreqRect);

    if (bArrowType == RADIO_COLOR_NORMAL)
    {
        FreqStrAttr.bTextColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
        _bRADIOFlash = FALSE;
    }
    else
    {
        FreqStrAttr.bTextColor = RADIO_PALETTE_ENTRY_FREQ_BAR_HIGHLIGHT;
        _bRADIOFlash = TRUE;
    }
    
    FreqStrAttr.bBackground = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
    FreqStrAttr.bColorKey = 0;
    FreqStrAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    FreqStrAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;

    FreqStrAttr.wX = FreqRect.rect.wRight-(GDI_GetStringWidth(aDSPLYDownArrowSymbol)>>1);
    FreqStrAttr.wY = RADIO_FREQ_ARROW_START_V;    
    // Show arrow incidate current playing frequency.
    GDI_DrawString_909(0, &FreqStrAttr, aDSPLYDownArrowSymbol);   

    _dwRadioFlashTime = OS_GetSysTimer();	
}

void _RADIO_UpdateInfoRegion(BYTE bClear, BYTE bDrawBG, BYTE bListBGType)
{
//    PARM_RECT InfoRect;
    PARM_DRAW_BITMAP DrawBitmap;

    if (bClear)
    {
/*
        InfoRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
        InfoRect.rect.wLeft = RADIO_INFO_START_H;
        InfoRect.rect.wTop = RADIO_INFO_START_V-2;
        InfoRect.rect.wRight = InfoRect.rect.wLeft+RADIO_INFO_WIDTH-1;
        InfoRect.rect.wBottom = InfoRect.rect.wTop+RADIO_INFO_HEIGHT+3;
        // Clear info region.
        GDI_FillRect(0, &InfoRect);
*/        
        DrawBitmap.bBitmapID = 3;        
        DrawBitmap.wDestX = RADIO_INFO_START_H;
        DrawBitmap.wDestY = RADIO_INFO_START_V;        
        DrawBitmap.bDestRegion = 0;
        // Clear info region.   
        GDI_DrawZipBitmap(&DrawBitmap, _aRADIOArray, (PBYTE)RADIO_UNZIP_ADDR, RADIO_UNZIP_WINDOW_ADDR);       
    }

    switch (_bRADIOMode)
    {
#ifdef SUPPORT_RADIO_RDS    
    case RADIO_MODE_NORMAL:
    case RADIO_MODE_FAVORITE:
        _RADIO_UpdateRDSInfo(bDrawBG);
        break;
#endif		
    case RADIO_MODE_RECORD:        
        _RADIO_UpdateRecordInfo(bDrawBG, bListBGType, RADIO_COLOR_HIGHLIGHT);
        break;
    case RADIO_MODE_SCAN:		
        _RADIO_UpdateScanInfo(bDrawBG);
        break;
    case RADIO_MODE_SELECTION:
#ifdef SUPPORT_RADIO_RDS		
        if ((_bRADIOBackupMode == RADIO_MODE_NORMAL) || (_bRADIOBackupMode == RADIO_MODE_FAVORITE))
        {
            _RADIO_UpdateRDSInfo(bDrawBG);
        }
        else
#endif			
        if (_bRADIOBackupMode == RADIO_MODE_RECORD)
        {
            _RADIO_UpdateRecordInfo(bDrawBG, RADIO_COLOR_NORMAL, RADIO_COLOR_HIGHLIGHT);
        }
        break;
    default:
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _RADIO_UpdateRDSInfo
//  Abstract    :   Call this function to update and display RDS info.
//  Arguments  :   bDrawBG: 1: draw background  0: not draw background
//  Return	 :   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
#ifdef SUPPORT_RADIO_RDS
void _RADIO_UpdateRDSInfo(BYTE bDrawBG)
{
    PARM_RECT RDSRect;
    PARM_DRAW_STRING RDSStrAttr;
    BYTE bTemp1, bTemp2;
    WORD wNum;
    WORD RDSDataLength;
    BYTE bIndex;
    BYTE bCharWidth;
    PARM_BITMAP_COPY RDSBitmapCopy;

    if (bDrawBG)
    {
        RDSRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
        RDSRect.rect.wLeft = RADIO_RDS_START_H;
        RDSRect.rect.wTop = RADIO_RDS_START_V;
        RDSRect.rect.wRight = RDSRect.rect.wLeft+RADIO_RDS_WIDTH-1;
        RDSRect.rect.wBottom = RDSRect.rect.wTop+RADIO_RDS_HEIGHT-1;
        // Fill RDS frame.
        GDI_FillRoundRect(0, RADIO_BAND_FRAME_ARC_VALUE, &RDSRect);
    
        RDSRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG_OUTLINE;
        RDSRect.rect.wLeft = RDSRect.rect.wLeft-RADIO_ARC_PIXEL;
        RDSRect.rect.wTop = RDSRect.rect.wTop-RADIO_ARC_PIXEL;
        RDSRect.rect.wRight = RDSRect.rect.wRight+RADIO_ARC_PIXEL;
        RDSRect.rect.wBottom = RDSRect.rect.wBottom+RADIO_ARC_PIXEL;
        // Draw RDS frame ourter line.
        GDI_DrawRoundRect(0, RADIO_BAND_FRAME_OUTER_ARC_VALUE, RADIO_BAND_FRAME_ARC_VALUE, &RDSRect);        
    }
    // Update each RDS info.
    for (bTemp1 = 0; bTemp1 < SUPPORT_RDS_TYPE_NUMBER; bTemp1++)
    {
        if ((bTemp1 == _bCurRDSType) && (_bRDSDataRemainNum != 0)) // Current displaying RDS info. can't update.
            continue;
        if (RADIODRV_RDSTYPE(SupportRDSType[bTemp1]))  // If support this type?
        { 		
            if (RADIODRV_UpdateRDS(SupportRDSType[bTemp1], aGetRDSData)) // RDS data ready?
            {
                if (SupportRDSType[bTemp1] == RDS_CT) // Data need change to ASCII
                {
                    OSDUTL_PrepareOutputString();	
                    wNum = ((WORD)aGetRDSData[1]) << 8;
                    wNum |= (WORD)aGetRDSData[2];
                    OSDUTL_OutputNumFromIndex(wNum);  // Year
                    OSDUTL_CopyStringFromIndex(aRADIOSolidus); // "/"
                    OSDUTL_OutputNumFromIndex(aGetRDSData[3]); // Month	
                    OSDUTL_CopyStringFromIndex(aRADIOSolidus); // "/"
                    OSDUTL_OutputNumFromIndex(aGetRDSData[4]); // Date
                    OSDUTL_CopyStringFromIndex(aRADIOSpace); // " "
                    OSDUTL_CopyStringFromIndex(aWeekDay[aGetRDSData[5]-1]);
                    OSDUTL_CopyStringFromIndex(aRADIOSpace); // " "
                    OSDUTL_OutputNumFromIndex(aGetRDSData[6]); // Hour                    
                    OSDUTL_CopyStringFromIndex(aRADIOColon); // " "
                    OSDUTL_OutputNumFromIndex(aGetRDSData[7]); // Minute                      
                    
                    aConvRDSData[bTemp1][0] = __wOSDUTLStringBuf[0];                    
    		
                    for (bTemp2 = 1; bTemp2 <= aConvRDSData[bTemp1][0]; bTemp2++)
                    {                  
                        aConvRDSData[bTemp1][bTemp2] = __wOSDUTLStringBuf[bTemp2];
                    }	
                }
                else  // ASCII
                {
                    aConvRDSData[bTemp1][0] = aGetRDSData[0];
    		
                    for (bTemp2 = 1; bTemp2 <= aConvRDSData[bTemp1][0]; bTemp2++)
                    {                  
                        aConvRDSData[bTemp1][bTemp2] = CONVCHAR_ASCII_To_CTK(aGetRDSData[bTemp2]);
                    }	
                }
				
                RDSUpdateStatus[bTemp1] = 1;				
            }
        }
    }

    if (_bRDSDataRemainNum == 0) // Prepare RDS info. display.
    {
        if ((OS_GetSysTimer()-_dwRDSWaitTime) < RADIO_RDS_WAIT_TIME) // Wait for RDS data ready.
            return;
        if (_bCurRDSType >= SUPPORT_RDS_TYPE_NUMBER)
        {
            _bCurRDSType = 0;				
        }	

        if (RDSUpdateStatus[_bCurRDSType] == 0) // Non updated.
        {
            _bCurRDSType++;
            return;
        }
		
        RDSUpdateStatus[_bCurRDSType] = 0; // Clear the update status. the RDS info. will be showed.		
        _bRDSDataRemainNum = aConvRDSData[_bCurRDSType][0];
        _wRDSDataPos = RADIO_RDS_INFO_START_H;           
    }
    else // RDS info. display.
    {
        if ((_wRDSDataPos-RADIO_RDS_MOVE_STEP) < RADIO_RDS_INFO_BOUNDARY_LEFT) // The first character have reached the left boundry.
        {
            _bRDSDataRemainNum--;
            if (_bRDSDataRemainNum == 0) // Finish current RDS string display. 
            {
                _dwRDSWaitTime = OS_GetSysTimer();
                _bCurRDSType++;                       
                /*				
                RDSRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
                RDSRect.rect.wLeft = RADIO_RDS_START_H;
                RDSRect.rect.wTop = RADIO_RDS_START_V;
                RDSRect.rect.wRight = RDSRect.rect.wLeft+RADIO_RDS_WIDTH-1;
                RDSRect.rect.wBottom = RDSRect.rect.wTop+RADIO_RDS_HEIGHT-1;
                // Clear RDS frame.        
                GDI_FillRoundRect(0, RADIO_BAND_FRAME_ARC_VALUE, &RDSRect);
                */
                return;
            }
      			
            _wRDSDataPos = _wRDSDataPos + GDI_GetCharWidth(aShowRDSData[1]) - RADIO_RDS_MOVE_STEP;            
            bTemp1 = 1;	
            // Check if the following characters also reached left boundy. 
            while (_wRDSDataPos < RADIO_RDS_INFO_BOUNDARY_LEFT)
            {
                _bRDSDataRemainNum--;                
                if (_bRDSDataRemainNum == 0) // Finish current RDS string display. 
                {
                    _dwRDSWaitTime = OS_GetSysTimer();
                    _bCurRDSType++;
                    /*
                    RDSRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
                    RDSRect.rect.wLeft = RADIO_RDS_START_H;
                    RDSRect.rect.wTop = RADIO_RDS_START_V;
                    RDSRect.rect.wRight = RDSRect.rect.wLeft+RADIO_RDS_WIDTH-1;
                    RDSRect.rect.wBottom = RDSRect.rect.wTop+RADIO_RDS_HEIGHT-1;
                    // Clear RDS frame.        
                    GDI_FillRoundRect(0, RADIO_BAND_FRAME_ARC_VALUE, &RDSRect);
                    */
                    return;
                }
                bTemp1++;	
                _wRDSDataPos = _wRDSDataPos + GDI_GetCharWidth(aShowRDSData[bTemp1]);	
            }
        }
        else
        {
            _wRDSDataPos = _wRDSDataPos - RADIO_RDS_MOVE_STEP;
        }

        RDSDataLength = RADIO_RDS_INFO_BOUNDARY_RIGHT - _wRDSDataPos;
        bIndex = 0;
        // Grab characters for display.
        for (bTemp2 = 1; bTemp2 <= _bRDSDataRemainNum; bTemp2++)
        {
            bCharWidth = GDI_GetCharWidth(aConvRDSData[_bCurRDSType][aConvRDSData[_bCurRDSType][0]-_bRDSDataRemainNum+bTemp2]);
            if (bCharWidth <= RDSDataLength)
            {
                RDSDataLength -= bCharWidth;                
                bIndex++;
                aShowRDSData[bIndex] = aConvRDSData[_bCurRDSType][aConvRDSData[_bCurRDSType][0]-_bRDSDataRemainNum+bTemp2];
            }
            else
                break;
        }   
		
        aShowRDSData[0] = bIndex;        
	
        RDSRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
        RDSRect.rect.wLeft = RADIO_RDS_INFO_DISPLAY_LEFT;
        RDSRect.rect.wTop = RADIO_RDS_INFO_DISPLAY_TOP;
        RDSRect.rect.wRight = RADIO_RDS_INFO_DISPLAY_RIGHT;
        RDSRect.rect.wBottom = RADIO_RDS_INFO_DISPLAY_BOTTOM;
        // Clear RDS frame.        
        GDI_FillRect(4, &RDSRect);

        RDSStrAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_BLACK;       
        RDSStrAttr.bBackground = RDSRect.bColor;
        RDSStrAttr.bColorKey = 0;
        RDSStrAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
        RDSStrAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;    
        RDSStrAttr.wX = _wRDSDataPos;
        RDSStrAttr.wY = RADIO_RDS_INFO_DISPLAY_TOP+((RADIO_RDS_HEIGHT-GDI_GetStringHeight(aShowRDSData))>>1);
        // Show RDS info.
        GDI_DrawString_909(4, &RDSStrAttr, aShowRDSData);

        RDSBitmapCopy.bSrcRegion = 4;
        RDSBitmapCopy.bDestRegion = 0;        
        RDSBitmapCopy.SrcRect.wLeft = RADIO_RDS_INFO_DISPLAY_LEFT;
        RDSBitmapCopy.SrcRect.wTop = RADIO_RDS_INFO_DISPLAY_TOP;
        RDSBitmapCopy.SrcRect.wRight = RADIO_RDS_INFO_DISPLAY_RIGHT;
        RDSBitmapCopy.SrcRect.wBottom = RADIO_RDS_INFO_DISPLAY_BOTTOM;
        RDSBitmapCopy.wDestX = RADIO_RDS_START_H+15;
        RDSBitmapCopy.wDestY = RADIO_RDS_START_V;
        // Show RDS info.
        GDI_BitmapCopy(&RDSBitmapCopy);
/*

        RDSRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
        RDSRect.rect.wLeft = RADIO_RDS_START_H;
        RDSRect.rect.wTop = RADIO_RDS_START_V;
        RDSRect.rect.wRight = RDSRect.rect.wLeft+RADIO_RDS_WIDTH-1;
        RDSRect.rect.wBottom = RDSRect.rect.wTop+RADIO_RDS_HEIGHT-1;
        // Clear RDS frame.        
        GDI_FillRoundRect(0, RADIO_BAND_FRAME_ARC_VALUE, &RDSRect);

        RDSStrAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_BLACK;       
        RDSStrAttr.bBackground = RDSRect.bColor;
        RDSStrAttr.bColorKey = 0;
        RDSStrAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
        RDSStrAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;    
        RDSStrAttr.wX = _wRDSDataPos;
        RDSStrAttr.wY = RADIO_RDS_START_V+((RADIO_RDS_HEIGHT-GDI_GetStringHeight(aShowRDSData))>>1);
        // Show RDS info.
        GDI_DrawString_909(0, &RDSStrAttr, aShowRDSData);

        RDSRect.rect.wLeft = RADIO_RDS_INFO_BOUNDARY_LEFT;
        RDSRect.rect.wTop = RDSStrAttr.wY;
        RDSRect.rect.wRight = RDSRect.rect.wLeft+20-1;
        RDSRect.rect.wBottom = RDSRect.rect.wTop+GDI_GetStringHeight(aShowRDSData)-1;
        // Clear left part of the string that exceed display region.
        GDI_FillRect(0, &RDSRect);

        RDSRect.rect.wLeft = RADIO_RDS_INFO_START_H;       
        RDSRect.rect.wRight = RDSRect.rect.wLeft+20-1;     
        // Clear right part of the string that exceed display region.
        GDI_FillRect(0, &RDSRect);
*/
/*
        RDSRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG_OUTLINE;
        RDSRect.rect.wLeft = RADIO_RDS_START_H-RADIO_ARC_PIXEL;
        RDSRect.rect.wTop = RADIO_RDS_START_V-RADIO_ARC_PIXEL;
        RDSRect.rect.wRight = RADIO_RDS_START_H+RADIO_RDS_WIDTH-1+RADIO_ARC_PIXEL;
        RDSRect.rect.wBottom = RADIO_RDS_START_V+RADIO_RDS_HEIGHT-1+RADIO_ARC_PIXEL;
        // Draw RDS frame ourter line.
        GDI_DrawRoundRect(0, RADIO_BAND_FRAME_OUTER_ARC_VALUE, RADIO_BAND_FRAME_ARC_VALUE, &RDSRect);        
*/        
    }
	
    _dwRDSFlashTime = OS_GetSysTimer();
}

//*********************************************************************************************************
//  Function    :   _RADIO_ResetRDS
//  Abstract    :   Call this function to reset RDS status
//  Arguments  :   bClear: 1: Clear the displaying RDS scrolling text  0: not Clear
//  Return	 :   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _RADIO_ResetRDS(BYTE bClear)
{
    PARM_RECT RDSRect;
    BYTE bTemp;

    _bRDSDataRemainNum = 0;
    _bCurRDSType = 0;	
			
    for (bTemp = 0; bTemp < RDS_TYPE_NUMBER; bTemp++)
    {
        RDSUpdateStatus[bTemp] = 0; //Initial the RDS update status.
    }   

    if (bClear)		
    {
        RDSRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
        RDSRect.rect.wLeft = RADIO_RDS_START_H;
        RDSRect.rect.wTop = RADIO_RDS_START_V;
        RDSRect.rect.wRight = RDSRect.rect.wLeft+RADIO_RDS_WIDTH-1;
        RDSRect.rect.wBottom = RDSRect.rect.wTop+RADIO_RDS_HEIGHT-1;
        // Clear RDS frame.        
        GDI_FillRoundRect(0, RADIO_BAND_FRAME_ARC_VALUE, &RDSRect);
    }

    //_dwRDSFlashTime = OS_GetSysTimer();	
}
#endif	

void _RADIO_UpdateRecordInfo(BYTE bDrawBG, BYTE bListBGType, BYTE bTextType)
{
    PARM_RECT RecordRect;
    PARM_DRAW_STRING RecordStrAttr;

    if (bDrawBG)
    {
        RecordRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
        RecordRect.rect.wLeft = RADIO_RECORD_START_H;
        RecordRect.rect.wTop = RADIO_RECORD_START_V;
        RecordRect.rect.wRight = RecordRect.rect.wLeft+RADIO_RECORD_WIDTH-1;
        RecordRect.rect.wBottom = RecordRect.rect.wTop+RADIO_RECORD_HEIGHT-1;
        // Fill record frame.
        GDI_FillRoundRect(0, RADIO_BAND_FRAME_ARC_VALUE, &RecordRect);
    
        RecordRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG_OUTLINE;
        RecordRect.rect.wLeft = RecordRect.rect.wLeft-RADIO_ARC_PIXEL;
        RecordRect.rect.wTop = RecordRect.rect.wTop-RADIO_ARC_PIXEL;
        RecordRect.rect.wRight = RecordRect.rect.wRight+RADIO_ARC_PIXEL;
        RecordRect.rect.wBottom = RecordRect.rect.wBottom+RADIO_ARC_PIXEL;
        // Draw record frame ourter line.
        GDI_DrawRoundRect(0, RADIO_BAND_FRAME_OUTER_ARC_VALUE, RADIO_BAND_FRAME_ARC_VALUE, &RecordRect);
    
        RecordRect.rect.wLeft = RADIO_RECORD_START_H+RADIO_DISTANCE_OFFSET50+20;
        RecordRect.rect.wRight = RecordRect.rect.wLeft+1;
        // Draw record separate line.
        GDI_FillRect(0, &RecordRect);
    }

    OSDUTL_PrepareOutputString();
	
    if (_bRecordHighlightPos < 9)
    {
        OSDUTL_OutputNumFromIndex(0);
    }
    OSDUTL_OutputNumFromIndex(_bRecordHighlightPos+1);     

    if (bListBGType)
    {
        RecordRect.bColor = RADIO_PALETTE_ENTRY_HIGHLIGHT_BG;
    }
    else
    {
        RecordRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
    }
    
    RecordRect.rect.wLeft = RADIO_RECORD_START_H+RADIO_DISTANCE_OFFSET10+4;
    RecordRect.rect.wTop = RADIO_RECORD_START_V+((RADIO_RECORD_HEIGHT-GDI_GetStringHeight(__pOSDUTLOutputString))>>1);
    RecordRect.rect.wRight = RecordRect.rect.wLeft+RADIO_DISTANCE_OFFSET50-9;
    RecordRect.rect.wBottom = RecordRect.rect.wTop+GDI_GetStringHeight(__pOSDUTLOutputString)-1;
    // Draw favorite list background.
    GDI_FillRect(0, &RecordRect);

    if (bTextType)
    {
        RecordStrAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_BLACK;
        _bRADIOFlash = TRUE;
    }
    else
    {
        RecordStrAttr.bTextColor = RADIO_PALETTE_ENTRY_HIGHLIGHT_BG;
        _bRADIOFlash = FALSE;
    }
    
    RecordStrAttr.bBackground = RecordRect.bColor;
    RecordStrAttr.bColorKey = 0;
    RecordStrAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    RecordStrAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;

    RecordStrAttr.wX = RADIO_RECORD_START_H+((RADIO_RECORD_LIST_WIDTH-GDI_GetStringWidth(__pOSDUTLOutputString))>>1);
    RecordStrAttr.wY = RecordRect.rect.wTop;
    // Show the position of favorite list.
    GDI_DrawString_909(0, &RecordStrAttr, __pOSDUTLOutputString);

    RecordRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
    RecordRect.rect.wLeft = RADIO_RECORD_FREQ_START_H;   
    RecordRect.rect.wRight = RecordRect.rect.wLeft+RADIO_RECORD_FREQ_WIDTH-RADIO_DISTANCE_OFFSET10;    
    // Draw favorite freq. background.
    GDI_FillRect(0, &RecordRect);

    OSDUTL_PrepareOutputString();
	
    OSDUTL_OutputNumFromIndex(_wRADIOFavorite[_bRecordHighlightPos]/100);
    if ((_wRADIOFavorite[_bRecordHighlightPos]%100) != 0)
    {
        OSDUTL_CopyStringFromIndex(aRADIODot);
        OSDUTL_OutputNumFromIndex((_wRADIOFavorite[_bRecordHighlightPos]/10)%10);	
        if ((_wRADIOFavorite[_bRecordHighlightPos]%10) != 0)
        {
            OSDUTL_OutputNumFromIndex(_wRADIOFavorite[_bRecordHighlightPos]%10);
        }
    } 

    RecordStrAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_BLACK;
    RecordStrAttr.bBackground = RecordRect.bColor;	
    RecordStrAttr.wX = RADIO_RECORD_FREQ_START_H+((RADIO_RECORD_FREQ_WIDTH-GDI_GetStringWidth(__pOSDUTLOutputString))>>1);
    // Show the freq. of the position of favorite list.
    GDI_DrawString_909(0, &RecordStrAttr, __pOSDUTLOutputString);

    _dwRadioFlashTime = OS_GetSysTimer();
}

void _RADIO_UpdateScanInfo(BYTE bDrawBG)
{
    PARM_RECT ScanRect;
    PARM_DRAW_STRING ScanStrAttr;

    if (bDrawBG)
    {
        ScanRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
        ScanRect.rect.wLeft = RADIO_SCAN_START_H;
        ScanRect.rect.wTop = RADIO_SCAN_START_V;
        ScanRect.rect.wRight = ScanRect.rect.wLeft+RADIO_SCAN_WIDTH-1;
        ScanRect.rect.wBottom = ScanRect.rect.wTop+RADIO_SCAN_HEIGHT-1;
        // Fill scan frame.
        GDI_FillRoundRect(0, RADIO_BAND_FRAME_ARC_VALUE, &ScanRect);
    
        ScanRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG_OUTLINE;
        ScanRect.rect.wLeft = ScanRect.rect.wLeft-RADIO_ARC_PIXEL;
        ScanRect.rect.wTop = ScanRect.rect.wTop-RADIO_ARC_PIXEL;
        ScanRect.rect.wRight = ScanRect.rect.wRight+RADIO_ARC_PIXEL;
        ScanRect.rect.wBottom = ScanRect.rect.wBottom+RADIO_ARC_PIXEL;
        // Draw scan frame ourter line.
        GDI_DrawRoundRect(0, RADIO_BAND_FRAME_OUTER_ARC_VALUE, RADIO_BAND_FRAME_ARC_VALUE, &ScanRect);
    }

    ScanRect.bColor = RADIO_PALETTE_ENTRY_FREQ_FRAME_BG;
    ScanRect.rect.wLeft = RADIO_SCAN_START_H+RADIO_DISTANCE_OFFSET20;
    ScanRect.rect.wTop = RADIO_SCAN_START_V;
    ScanRect.rect.wRight = ScanRect.rect.wLeft+RADIO_SCAN_WIDTH-2*RADIO_DISTANCE_OFFSET20-1;
    ScanRect.rect.wBottom = ScanRect.rect.wTop+RADIO_SCAN_HEIGHT-1;
    // Fill scan frame.
    GDI_FillRect(0, &ScanRect);

    OSDUTL_PrepareOutputString();
	
    if (_bRADIOStationNum < 10)
    {
        OSDUTL_OutputNumFromIndex(0);
    }
    OSDUTL_OutputNumFromIndex(_bRADIOStationNum);

    OSDUTL_CopyStringFromIndex(aRADIOSpace);
    OSDUTL_CopyStringFromIndex(aRADIOStationsFound);
		
    ScanStrAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_BLACK;
    ScanStrAttr.bBackground = ScanRect.bColor;
    ScanStrAttr.bColorKey = 0;
    ScanStrAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    ScanStrAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;

    ScanStrAttr.wX = RADIO_SCAN_START_H+((RADIO_SCAN_WIDTH-GDI_GetStringWidth(__pOSDUTLOutputString))>>1);
    ScanStrAttr.wY = RADIO_SCAN_START_V+((RADIO_SCAN_HEIGHT-GDI_GetStringHeight(__pOSDUTLOutputString))>>1);
    // Show the number of found stations.
    GDI_DrawString_909(0, &ScanStrAttr, __pOSDUTLOutputString);
}

//*********************************************************************************************************
//  Function    :   _RADIO_ChangeMode
//  Abstract    :   Call this function to change radio mode
//  Arguments  :   
//  Return	 :   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _RADIO_ChangeMode(void)
{
    _RADIO_UpdateButtonCursor(_bRADIOButtonCursorIndex, RADIO_COLOR_NORMAL, RADIO_COLOR_HIGHLIGHT);
	
    switch (RADIOModeButton[_bRADIOButtonCursorIndex].bMode)
    {
    case RADIO_MODE_NORMAL:
        if (_bRADIOBackupMode != RADIO_MODE_NORMAL)
        {
            _bRADIOMode = RADIO_MODE_NORMAL;
            _RADIO_UpdateContent(RADIO_ALL_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);
#ifdef SUPPORT_RADIO_RDS			
            _RADIO_ResetRDS(TRUE);	
#endif
        }
        else
        {
            _bRADIOMode = RADIO_MODE_NORMAL;
            //_RADIO_UpdateFrequencyBar(FALSE, RADIO_COLOR_HIGHLIGHT);        
        }
        break;
    case RADIO_MODE_FAVORITE:
        if (_bRADIOBackupMode != RADIO_MODE_FAVORITE)
        {
            _bRADIOMode = RADIO_MODE_FAVORITE;
            RADIODRV_tune(_wRADIOFavorite[_bRADIOFavoritePlay]);             
            _RADIO_UpdateContent(RADIO_ALL_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);
#ifdef SUPPORT_RADIO_RDS			
            _RADIO_ResetRDS(TRUE);	
#endif
        }
        else
        {
            _bRADIOMode = RADIO_MODE_FAVORITE;        
            //_RADIO_UpdateFrequencyBar(FALSE, RADIO_COLOR_HIGHLIGHT);        	  	
        }
        break;
    case RADIO_MODE_RECORD:
        if (_bRADIOBackupMode != RADIO_MODE_RECORD)
        {
            _bRADIOMode = RADIO_MODE_RECORD;                         
            _RADIO_UpdateContent(RADIO_ALL_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);
        }
        else
        {
            _bRADIOMode = RADIO_MODE_RECORD;
            _RADIO_UpdateRecordInfo(FALSE, RADIO_COLOR_HIGHLIGHT, RADIO_COLOR_HIGHLIGHT);                    	  	
        }        	
        break;
    case RADIO_MODE_SCAN:
        _bRADIOMode = RADIO_MODE_SCAN;
        _bRADIOStationNum = 0;
        _RADIO_UpdateContent(RADIO_ALL_REGION, TRUE, RADIO_COLOR_HIGHLIGHT);
#ifdef SUPPORT_RADIO_RDS
        _RADIO_ResetRDS(FALSE);	
#endif
        RADIO_Scan();	        		
        break;
    default:
        break;
    }    
}

void _RADIO_GotoUpButton(void)
{
    BYTE bButtonIndex;
    DWORD dwDistance = 0;
    DWORD dwShortestDistance = 0xFFFFFFFF;
    WORD wH, wV;
    BYTE bLatestIndex = _bRADIOButtonCursorIndex;

    for (bButtonIndex = 0; bButtonIndex < RADIO_MODE_NUM; bButtonIndex++)
    {
        if (RADIOModeButton[bButtonIndex].wStartV < RADIOModeButton[_bRADIOButtonCursorIndex].wStartV)
        {
            if (RADIOModeButton[bButtonIndex].wStartH >= RADIOModeButton[_bRADIOButtonCursorIndex].wStartH)
            {
                wH = RADIOModeButton[bButtonIndex].wStartH-RADIOModeButton[_bRADIOButtonCursorIndex].wStartH;
            }
            else
            {
                wH = RADIOModeButton[_bRADIOButtonCursorIndex].wStartH-RADIOModeButton[bButtonIndex].wStartH;
            }
            
            wV = RADIOModeButton[_bRADIOButtonCursorIndex].wStartV-RADIOModeButton[bButtonIndex].wStartV;
            dwDistance = wH*wH+wV*wV;

            if (dwDistance < dwShortestDistance)
            {
                dwShortestDistance = dwDistance;
                bLatestIndex = bButtonIndex;
            }
        }
    }

    _RADIO_MoveButtonCursor(bLatestIndex);
}

void _RADIO_GotoDownButton(void)
{
    BYTE bButtonIndex;
    DWORD dwDistance = 0;
    DWORD dwShortestDistance = 0xFFFFFFFF;
    WORD wH, wV;
    BYTE bLatestIndex = _bRADIOButtonCursorIndex;

    for (bButtonIndex = 0; bButtonIndex < RADIO_MODE_NUM; bButtonIndex++)
    {
        if (RADIOModeButton[bButtonIndex].wStartV > RADIOModeButton[_bRADIOButtonCursorIndex].wStartV)
        {
            if (RADIOModeButton[bButtonIndex].wStartH >= RADIOModeButton[_bRADIOButtonCursorIndex].wStartH)
            {
                wH = RADIOModeButton[bButtonIndex].wStartH-RADIOModeButton[_bRADIOButtonCursorIndex].wStartH;
            }
            else
            {
                wH = RADIOModeButton[_bRADIOButtonCursorIndex].wStartH-RADIOModeButton[bButtonIndex].wStartH;
            }
            
            wV = RADIOModeButton[bButtonIndex].wStartV-RADIOModeButton[_bRADIOButtonCursorIndex].wStartV;
            dwDistance = wH*wH+wV*wV;

            if (dwDistance < dwShortestDistance)
            {
                dwShortestDistance = dwDistance;
                bLatestIndex = bButtonIndex;
            }
        }
    }

    _RADIO_MoveButtonCursor(bLatestIndex);
}

/*
void _RADIO_GotoRightButton(void)
{
    BYTE bButtonIndex;
    DWORD dwDistance = 0;
    DWORD dwShortestDistance = 0xFFFFFFFF;
    WORD wH, wV;
    BYTE bLatestIndex = _bRADIOButtonCursorIndex;

    for (bButtonIndex = 0; bButtonIndex < RADIO_MODE_NUM; bButtonIndex++)
    {
        if (RADIOModeButton[bButtonIndex].wStartH > RADIOModeButton[_bRADIOButtonCursorIndex].wStartH)
        {
            if (RADIOModeButton[bButtonIndex].wStartV >= RADIOModeButton[_bRADIOButtonCursorIndex].wStartV)
            {
                wV = RADIOModeButton[bButtonIndex].wStartV-RADIOModeButton[_bRADIOButtonCursorIndex].wStartV;
            }
            else
            {
                wV = RADIOModeButton[_bRADIOButtonCursorIndex].wStartV-RADIOModeButton[bButtonIndex].wStartV;
            }
            
            wH = RADIOModeButton[bButtonIndex].wStartH-RADIOModeButton[_bRADIOButtonCursorIndex].wStartH;
            dwDistance = wH*wH+wV*wV;

            if (dwDistance < dwShortestDistance)
            {
                dwShortestDistance = dwDistance;
                bLatestIndex = bButtonIndex;
            }
        }
    }

    _RADIO_MoveButtonCursor(bLatestIndex);
}

void _RADIO_GotoLeftButton(void)
{
    BYTE bButtonIndex;
    DWORD dwDistance = 0;
    DWORD dwShortestDistance = 0xFFFFFFFF;
    WORD wH, wV;
    BYTE bLatestIndex = _bRADIOButtonCursorIndex;

    for (bButtonIndex = 0; bButtonIndex < RADIO_MODE_NUM; bButtonIndex++)
    {
        if (RADIOModeButton[bButtonIndex].wStartH < RADIOModeButton[_bRADIOButtonCursorIndex].wStartH)
        {
            if (RADIOModeButton[bButtonIndex].wStartV >= RADIOModeButton[_bRADIOButtonCursorIndex].wStartV)
            {
                wV = RADIOModeButton[bButtonIndex].wStartV-RADIOModeButton[_bRADIOButtonCursorIndex].wStartV;
            }
            else
            {
                wV = RADIOModeButton[_bRADIOButtonCursorIndex].wStartV-RADIOModeButton[bButtonIndex].wStartV;
            }
            
            wH = RADIOModeButton[_bRADIOButtonCursorIndex].wStartH - RADIOModeButton[bButtonIndex].wStartH;
            dwDistance = wH*wH+wV*wV;

            if (dwDistance < dwShortestDistance)
            {
                dwShortestDistance = dwDistance;
                bLatestIndex = bButtonIndex;
            }
        }
    }

    _RADIO_MoveButtonCursor(bLatestIndex);
}
*/

void _RADIO_MoveButtonCursor(BYTE bNewCursorIndex)
{
     if (bNewCursorIndex != _bRADIOButtonCursorIndex)
    {
        _RADIO_UpdateButtonCursor(_bRADIOButtonCursorIndex, RADIO_COLOR_NORMAL, RADIO_COLOR_HIGHLIGHT);
        _bRADIOButtonCursorIndex = bNewCursorIndex;
        _RADIO_UpdateButtonCursor(_bRADIOButtonCursorIndex, RADIO_COLOR_HIGHLIGHT, RADIO_COLOR_HIGHLIGHT);
    }    
}

void _RADIO_UpdateButtonCursor(BYTE bCursorIndex, BYTE bCursorType, BYTE bTextType)
{ 
    //PARM_DRAW_BITMAP DrawButton;
    PARM_RECT ButtonRect;
    PARM_DRAW_STRING ButtonStrAttr;
	
    MAINMENU_UseColorKey(&ButtonStrAttr);
    ButtonRect.bColor = RADIO_BUTTON_HEIGHT;    
	
    if (bCursorType == RADIO_COLOR_NORMAL)
    {
        ButtonRect.bShadePtr = _bRADIONormalButtonPalette; // Normal button
    }
    else
    {
        ButtonRect.bShadePtr = _bRADIOHighlightButtonPalette; // Highlight button
    }
/*    
    // Draw radio background photo.                
    DrawButton.wDestX = RADIOModeButton[bCursorIndex].wStartH;
    DrawButton.wDestY = RADIOModeButton[bCursorIndex].wStartV;        
    DrawButton.bDestRegion = 0;    
    GDI_DrawZipBitmap(&DrawButton, _aRADIOArray, (PBYTE)RADIO_UNZIP_ADDR, RADIO_UNZIP_WINDOW_ADDR);
*/
    ButtonRect.rect.wLeft = RADIOModeButton[bCursorIndex].wStartH;
    ButtonRect.rect.wTop = RADIOModeButton[bCursorIndex].wStartV;
    ButtonRect.rect.wRight = ButtonRect.rect.wLeft+RADIO_BUTTON_WIDTH-1;
    ButtonRect.rect.wBottom = ButtonRect.rect.wTop+RADIO_BUTTON_HEIGHT-1;
    // Fill button.
    GDI_FillRoundRect(0|GDI_SHADING_OPERATION, RADIO_BUTTON_ARC_VALUE, &ButtonRect);
 
    if (bCursorType == RADIO_COLOR_NORMAL)
    {
        ButtonRect.bColor = RADIO_PALETTE_ENTRY_NBUTTON_OUTLINE; // Normal button
    }
    else
    {
        ButtonRect.bColor = RADIO_PALETTE_ENTRY_HBUTTON_OUTLINE; // Highlight button
    }	

    ButtonRect.rect.wLeft = ButtonRect.rect.wLeft-RADIO_ARC_PIXEL;
    ButtonRect.rect.wTop = ButtonRect.rect.wTop-RADIO_ARC_PIXEL;
    ButtonRect.rect.wRight = ButtonRect.rect.wRight+RADIO_ARC_PIXEL;
    ButtonRect.rect.wBottom = ButtonRect.rect.wBottom+RADIO_ARC_PIXEL;
    // Draw button ourter line.
    GDI_DrawRoundRect(0, RADIO_BUTTON_OUTER_ARC_VALUE, RADIO_BUTTON_ARC_VALUE, &ButtonRect);

    if (bTextType)
    {
        ButtonStrAttr.bTextColor = RADIO_PALETTE_ENTRY_TEXT_WHITE;
        ButtonStrAttr.wX = RADIOModeButton[bCursorIndex].wStartH+((RADIO_BUTTON_WIDTH-GDI_GetStringWidth(RADIOModeButton[bCursorIndex].pwText))>>1);
        ButtonStrAttr.wY = RADIOModeButton[bCursorIndex].wStartV+((RADIO_BUTTON_HEIGHT-GDI_GetStringHeight(RADIOModeButton[bCursorIndex].pwText))>>1);    
        // Show mode name.
        GDI_DrawString_909(0, &ButtonStrAttr, RADIOModeButton[bCursorIndex].pwText);
        _bRADIOFlash = TRUE;
    }
    else
    {        
        _bRADIOFlash = FALSE;
    }
/*    
    ButtonStrAttr.bBackground = PAL_ENTRY_COLOR_TRANSPARENT;
    ButtonStrAttr.bColorKey = 0;
    ButtonStrAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    ButtonStrAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;

    ButtonStrAttr.wX = DrawButton.wDestX+((RADIO_BUTTON_WIDTH-GDI_GetStringWidth(RADIOModeButton[bCursorIndex].pwText))>>1);
    ButtonStrAttr.wY = DrawButton.wDestY+((RADIO_BUTTON_HEIGHT-GDI_GetStringHeight(RADIOModeButton[bCursorIndex].pwText))>>1);    
    // Show mode name.
    GDI_DrawString_909(0, &ButtonStrAttr, RADIOModeButton[bCursorIndex].pwText);
*/
    
    _dwRadioFlashTime = OS_GetSysTimer();
}

//*********************************************************************************************************
//  Function    :   _RADIO_VolumeControl
//  Abstract    :   Call this function to do volume control and output the information.
//  Arguments  :   bKey: input key
//  Return	 :   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _RADIO_VolumeControl(BYTE bKey)
{
    BYTE    bTime;
	
    switch(bKey)
    {
    case KEY_VOL_UP:
        if (__btMute)
        {           
            __btMute= FALSE;
            RADIODRV_SetMute(!__btMute);
            OSD_OUTPUT_MACRO(MSG_MUTE, __btMute, 0);
        }
        __dwVolAudio += VOLUME_GAP;
        if(__dwVolAudio > VOLUME_MAX)
        {
            __dwVolAudio= VOLUME_MAX;
        }
#ifdef CT950_STYLE
        DVDSETUP_SaveVolume();
#endif //CT950_STYLE
        if ((__dwVolAudio / VOLUME_GAP) < 2)
        {
            RADIODRV_SetVol((BYTE)(__dwVolAudio/VOLUME_GAP)&0x0F);
        }
        else
        {
            RADIODRV_SetVol((BYTE)((__dwVolAudio/VOLUME_GAP)-1)&0x0F);
        }
        OSD_OUTPUT_MACRO(MSG_VOLUME, __dwVolAudio/VOLUME_GAP, 3);        
        break;
    case KEY_VOL_DOWN:
        if (__btMute)
        {           
            __btMute= FALSE;
            RADIODRV_SetMute(!__btMute);
            OSD_OUTPUT_MACRO(MSG_MUTE, __btMute, 0);
        }
        if( __dwVolAudio < VOLUME_MIN + VOLUME_GAP)
        {
            __dwVolAudio = VOLUME_MIN;
        }
        else
        {
            __dwVolAudio -= VOLUME_GAP;
        }
#ifdef CT950_STYLE
        DVDSETUP_SaveVolume();
#endif //CT950_STYLE
        if ((__dwVolAudio / VOLUME_GAP) < 2)
        {
            RADIODRV_SetVol((BYTE)(__dwVolAudio/VOLUME_GAP)&0x0F);
        }
        else
        {
            RADIODRV_SetVol((BYTE)((__dwVolAudio/VOLUME_GAP)-1)&0x0F);
        }
        OSD_OUTPUT_MACRO(MSG_VOLUME, __dwVolAudio/VOLUME_GAP, 3);        
        break;
    case KEY_MUTE: 
        if (__btMute)
        {           
            __btMute= FALSE;
        }
        else
        {            
            __btMute= TRUE;
        }
        RADIODRV_SetMute(!__btMute);
        if (__btMute)
        {
            bTime=0xFF;
        }
        else
        {
            bTime=1;
        }
        OSD_OUTPUT_MACRO(MSG_MUTE, __btMute, bTime);
        break;    
    default:
        break;
    }
}

BYTE _RADIO_LoadBmpData(void)
{

    PSECTION_ENTRY pDataRoot, pBmpData; 

    // point to radio bmp data root
    pDataRoot = ROMLD_GetSectionByName(0, "RADR");

    if (!pDataRoot)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find radio BMP data root\n");
        return (FALSE);
    }

    // load radio frame background
    pBmpData = ROMLD_GetSectionByName((PSECTION_ENTRY)pDataRoot->dwRMA, "FRAM");

    if (!pBmpData)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find radio frame background BMP data\n");
        return (FALSE);
    }

    _aRADIO_Frame_BG = (BYTE *)(pBmpData->dwRMA + pDataRoot->dwRMA);	    

    // load radio title background
    pBmpData = ROMLD_GetSectionByName((PSECTION_ENTRY)pDataRoot->dwRMA, "TITL");

    if (!pBmpData)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find radio title background BMP data\n");
        return (FALSE);
    }

    _aRADIO_Title_BG = (BYTE *)(pBmpData->dwRMA + pDataRoot->dwRMA);

    // load radio frequency background
    pBmpData = ROMLD_GetSectionByName((PSECTION_ENTRY)pDataRoot->dwRMA, "FREQ");

    if (!pBmpData)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find radio frequency background BMP data\n");
        return (FALSE);
    }

    _aRADIO_Freq_BG = (BYTE *)(pBmpData->dwRMA + pDataRoot->dwRMA);

    // load radio information background
    pBmpData = ROMLD_GetSectionByName((PSECTION_ENTRY)pDataRoot->dwRMA, "INFO");

    if (!pBmpData)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find radio information background BMP data\n");
        return (FALSE);
    }

    _aRADIO_Info_BG = (BYTE *)(pBmpData->dwRMA + pDataRoot->dwRMA);    

    _aRADIOArray[0] = _aRADIO_Frame_BG;
    _aRADIOArray[1] = _aRADIO_Title_BG;
    _aRADIOArray[2] = _aRADIO_Freq_BG;
    _aRADIOArray[3] = _aRADIO_Info_BG;

    return (TRUE); 
}

#endif //#ifdef SUPPORT_RADIO_TUNER
