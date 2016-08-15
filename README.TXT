
        (C)Copyright CHEERTEK INC. 2003-2005,
            D300, all right reserved

// This is a read-me file for WinAV Firmware. It'll describe some improvements.
// For detail explanation, please look the comments in source files.

// ******** For different system, please modify "customer.h"
// If release to customer.
// [1] Please run release.bat to build release directory. Then rename to version name.
// [2] Please define "RELEASE_SETTING" in "customer.h"
// [3] WB_WINBOND-Key in "ir.h". 
// [4] WB_PANASONIC PANEL-Key in "key.h" 
// [5] Check "SW_VERSION" in "customer.h"

// To make servo codes, please keep in mind that select your loader in makefile "servo_module".
// If you use released version, please modify "servo_module" in makefile.

// The MPEG "RELEASE_SETTING" will open.
// [1] Enable PANEL_KEYSCAN.
// [2] Disable some DEBUG operations. [3] Enable the mechanism of Hang-Recover.

// Sub-Directorys are:
//  [1]  BMP      - put all CT908 btimap data.
//  [2]  boot	  - put booting code.
//  [3]  decoder  - put MPEG1/2 and JPG decoder.
//  [4]  display  - put display code.
//  [5]  info     - put the file system code.
//  [6]  mp4      - put DivX decoder code.
//  [7]  navigate - put CheerDVD Navigate protected code.
//  [8]  OBJS	  - put the onject code.
//  [9]  OSDChina - Chinese+English OSD files
//  [10] OSDEUR   - Europe OSD files. 
//  [11] parser	  - put parser code.
//  [12] servo    - put all servo code & compile in this sub-dir.
//  [13] tve      - put the TV encoder code.
//  [14] usb      - put the usb code.

// To build different language version:
//  1)Copy the files of directory "OSDEur" or "OSDChina" onto version directory.
//  2)Remove all string *.C files in dir OSDString 
//  3)#define SELL_DESTINATION  to your want within "customer.h".

// Want to set different configuration
//      . MPEG part    - "customer.h"- DECODER_SYSTEM

// Want to switch CT909S/CT909R platform.
//      . MPEG part    - "customer.h"- DECODER_SYSTEM

// Want to switch CT909R 64M/32M solution. Default is 32M now.

//      . MPEG part    - "internal.h"- DRAM_CONFIGURATION_TYPE
//                     - Copy font/string files in OSDString\OSDXX_XXM\*.* to OSDString\*.*.
//                     - Copy DVD909_XXM.ld to DVD909.ld                        

// Configure 8M flash solution
//  - Mark the define //#define SUPPORT_4M_FLASH from customer.h
//  - issue "make" to build code.

// Want to disable SERIAL_DEBUG (Don't disable it now)
// "customer.h"- SERIAL_DEBUG

// Modify "ir.h" for different IR-Mapping.
// Modify "key.h" for different VFD KeyScan-Mapping.
// Modify "panel.h" for different VFD DISPLAY.
// Modify "vfd_ctrl.h" for different VFD controller.

// **************   Default Setting     ******************
// The default setting is: SUPPORT_SERVO_SOURCE (CT909)
// Default Setting
//  *** [customer.h]
//      SELL_DESTINATION    CHINA
//      SUPPORT_LETTERBOX_SUBPICTURE

/**************************************************************************
Version :    2.78
Date    :    2007.05.24 / servo 2.77a(909R/P/G)
Chip    :    CT909P/G
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 05152007
. JPG Decoder Code Date
    - 05172007
. Display Code Date
    - 05102007
. DivX Code Date
    - 05152007
. Navigate Code Date
    - 05142007
. Parser Code Date
    - 05152007
. Usb Code Date
    - 05232007
. Info Code Date
    - 05232007
. Card Code Date
    - 05232007
. Audio Code Date
    - 04272007
. Servo Code version and date
    - 05242007
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix the bug 0003124: slide show時，支持每幅圖的進度條顯示，但有時進度條不能到100%. {mm_play.c}
    - Fix the bug 0002996: black screen in PHOTO+MUSIC ICON, if open define SUPPORT_PHOTO_BROWSE_MODE. {mm_play.c/thumb.c}
    - Fix bug: define DSU1_RX_SOURCE NOT_USED Error on CT956_DEV. 預設值應define DSU1_RX_SOURCE NIM_PATH
    - Fix bug: define DSU/UART error for CT909R_IC_SYSTEM. 
    - Fix debug: set DSU/UART port error on CT909R_IC_SYSTEM. 
    - Fix bug 0003100: 外掛字幕顯示不正確
    - Fix ABEX 540CD40/70 title OSD time display abnormal issue
    - Fix the MJPG AVI files, captured by DCs, Casio/ Canon/ Aqua/ Polaroid/ Kyocera/ Fuji, can not playback issue reported by EnSky.
    - Fix bug 0003131: 有的圖片在我們的公闆平臺（950，952，955）上能夠直接播放，但是在thumbnail mode 下卻無法看到。
    - Fix bug 0003104: color abnormal when zoom out to 25% or 50%
    - Fix the MJPG AVI files, can not AV sync issue reported by EnSky.
        Media.c/ Pio.h/ Srcfilter.c/ Usbsrc.c Usb.a/ usbwrap.h}
    - Fix 0003107: "Checksum Miss match" when SD card Upgrade code 
    - Fix 0003125: 播放影片,按SCFx2一直到影片結束播放下一段影片時,OSD時間會顯示異常
    - Fix 0003120: 播放碼流vatan.mpg中的節目"ORDU TV"，Unpause時畫面固定會有短暫的Blocking [Reported from K000 CT216U, CT956 also has the same problem
    - Fix 0002993: 含有DVB-T的Code在Auto upgrade時會有問題
    - Fix [MTC] （高）客?有?DivX5的碟片播放的?候按?停止，再按恢复播放，綠屏3秒种( Just fix "綠屏3秒种")

. Enhance Items
    - Support general register setting in booting code.
    - Add new API in card for supporting of usb device.
    - Add Nand Flash related module.
    - Support thumbnail with background JPEG image
    - Support USB host + device mode in CT95x. {Cc.c / Dvd_dram_16m.h / Dvd_dram_32m.h / Dvd_dram_64m.h/ Hal.c/ Initial.c/
    - Increase 13 kinds of new effect
    - Support Thumbnail with background JPEG image. { thumb.c/thumb.h/utl.c/utl.h}
    - Support SUPPORT_CHOOSE_MEDIA features for CT95x
    - Support error concealment mechanism for CT95x
    - Add a function, PARSER_MPEG_GetCurrentSCR, to get most current MPEG SCR value (33-bit) found.
    - Enhance error concealment: Don't do KEY_STOP directly. Pass the error condition to UTL module 


. Audio code
    - No update.

. Servo code
    [F/W] 
    (YHChen)
    *. Add ADJUST_FS_TO_1P1V for PI voltage as offset-adj state.(ADJUST_FS_TO_1P1V) 
    *. Fix bug of offset-adj. It will case fatel error of TE gain adj.(Srvservo.c, SRVSV_OffsetAdjustment())
    (Adam)
    *. Set macro for monitor pin usage.(MNTR1_VAL, MO_XXXX.c)
    *. Remove protection of D9 judge D5 title during LS process. Maybe it will cause side-effect(Srvstate.c, SRVST_LayerSwitchState())
    *. Remove protection of long seek touch home-sensor.(SRVST_Seek())
    *. Remove protection of touch home-sensor of offocus-drop recover.(Srvstate.c, SRVST_ServoRecover()) 
    (Eric)
    *. Modify printfQA releated debug message. 
    (Kevin)
    *. Add protection code for fast forward x32 hang 10 sec problem in DVCD. Update the under-run protection threshold and add “BMFUNC_SendInfoToParserISR”when ISR status is “dump-in complete”. ( Srv_dram.c , Srvude.c, pls check make "//KevinKen 070517") 
    (Kenny)
    *. Update auto-tuning tools()
    
    [Ucode]
    *. Add some burst register to monitor short seek performance. Please search “(;kenny 070425 for short seek monitor”) 


.Flash code Version
    - Version 1311.

/**************************************************************************
Version :    2.77
Date    :    2007.04.30 / servo 2.76b(909R/P/G)
Chip    :    CT909P/G
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 04272007
. JPG Decoder Code Date
    - 04272007
. Display Code Date
    - 04232007
. DivX Code Date
    - 03142007
. Navigate Code Date
    - 04102007
. Parser Code Date
    - 04252007
. Usb Code Date
    - 10262006
. Info Code Date
    - 04242007
. Card Code Date
    - 04192007
. Audio Code Date
    - 04272007
. Servo Code version and date
    - 03092007
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix setting error for still image @ digital panel mode. { [UTL] }
    - Fix mantis bug #2671: 連續按快進鍵,結果卻會跳到時間點00:00:00開始快進  { [UTL] }
    - Fix mantis bug #2995: press key_next under BROWSE MODE, musci will play for a while  (remove the modification from HAL.C to UTL.C) { [UTL] }
    - Fix error: nobody to release interrupt while user want to readback un-supported GPB port (only happen @ CT909P) { [HIO] }
    - Porting Grievous's code to fix mantis bug #2896: CT909R + 64M SDRAM solution has audio problem on Slow/ Step --> normal, when playing divx files. { [HAL] }
    - Porting CoCo's code to fix “播放Photo+Music,按Photo+Music和Photo(RMC上)鍵 ,第一張圖片播出來上方有殘影.------->機率性1/10” { [UTL] }
    - Fix mantis bug #3013, fix spst/ast can be set as setup even if  there is no spst/ast setting nv command in pre-command.
    - Fix condition of WMA playing end (Use A0 remainder instead of A1 remainder)
    - Add code to fix mantis bug #3000 which user need to clean the video as black screen if skip DRM/AES file since it may stay dialog box and don't play next { [mm_play.c] }
    - Add code to fix mantis bug #3046 which play movie un-continually when key volume up/down quickly { [cc.c/chips.c/winav.h] }
    - Fix bug : SCBx2 is same as SCF while playback DVD title "心空空"
    - Fixed 952/955 backlight on delay.
    - Fixed power off not turn off backlight.(952/955)
    - Fixed PAL table fail .
    - Fixed OSD tuning page display time to 5 sec, 
    - Fix TDV-MW521CW Masoic @ 1:04:49.
    - Fix CGMS-A bug for MPAA.
    - Fix abnormal playing for DAT files in 95x series
    - Fix usb won't work when auto upgrade failed. {infofilter.c}
    - Fix issue: VOD Clip - This device can read any V03 clip and one view will be counted down, even if the activation file is not for this device. Other V03 clips are for devices that have different registration code and model ID, but it still plays. It seems like it can read anything that is User 1 and still counts down. The different registration code and model ID does not matter.
    - Fix Mantis bug [3069] One Divx5.0 avi file "002.avi" hang at 20%
    - Fix Mantis bug [3026] <<RACE.avi>>不能正常播放,在prescan時顯示失敗信息. 文件<<RACE.avi>>放在ToHQ/NL中.
    - Fix Mantis bug [3067] Motion JPEG<<news_paper.avi>>不能播放,OSD顯示Skip file since prescan fail. 此文件已經放在ToHQ/NL中.
    
. Enhance Items
    - Modify the debug mode setting for CT956A_EVAL system board. { [PIO.H] }
    - Porting CoCo's code to add new AM case(HAL_AM_WMA_L1_DECODE) and delete a un-used one (HAL_AM_WMA_DRM) { [HDECODER] }
    - Add new SW RTC mechanism for DMP (Use Timer2 as timing counter)
    - Add card auto clock downgrade and error recovery.
    - Support displaying UNIDOE dir and file name. {osddsply.c/osddsply.h}
    - Move all include table from DECODER_SYSTEM item to WinAv.h
    - Modify Brightness mapping table
    - Turning color and sharpness(disable).
    - Power down DAC for 909P IC.
    - Provide one field display for MainVideo/ OSD.
    - Enhance the usb performance in 16M DRAM solution. {usb code/makefile/DVD909_16M.ld/DVD909_16MDBG.ld}
    - Add usb performance testing code. {cc.c/usbsrc.c}
    - Enhance the MP3 hang problem when quickly next/prev in 64M solution in usb mode. {dvd_dram_64m.h / internal.h/ srcfilter.h}
    - Add four functions for DVB support.
    - Always power down DD/ DC/ DY DAC @ Digital Panel Mode for "直條紋" issue { [HDEVICE] }
    - Fix 1st LOGO display fail @ CT950 solution only issue { [INITIAL] }
    - Porting SY's code to support new DSU/ UART debug configuration method.

. Audio code
    - 909R
      (1)Add A/V sync control for audio STC is great than STC 2 second.
	      #define 	HAL_AM_AV_SYNC_2SEC_CONDITION	0x71 	// 1 bit
	      0: disable skip
	      1: enable skip
	      audio reset will set to 1
      (2) Add CT909R code label and date at the end of *.bin.
      (3) Fix cycle count of PTS write point bug.	
  
    - 909P
      (1) Add CT909P or CT909G code label and date at the end of *.bin.
      (2) Fix cycle count of PTS write point bug.

    - 909G
      (1) Add CT909P or CT909G code label and date at the end of *.bin.
      (2) Fix cycle count of PTS write point bug.

. Servo code
    [F/W]
		 (Adam)
    *. Modify offset_adjustment flow.(SRVSV_OffsetAdjustment())
    *. Modify LS skill in ucode
    *. Remove TE mask in Jumpsetting function(FOGR[3]). default set high.(SV_XXXX.C SRVSV_InitRFIC(), srvservo.c SRVSV_jumpsetting())
    (CYChen)
    *. Modify D5 ID limited (SRVST_ServoCMDAccept)
    (YHChen)
    *. Define gbRFVpp to signed-char
    *. Remove bug of refill reg. CGR(ATT). (SRVSV_RefillCalibration())
    (David)
    *. Update pcb_system for CT909G_AAI_DV34_SAM_DL6
    *. Add protection and modification for new short seek in ucode
    (Allen)
    *. Update pcb_system for CT909P_CMS_S76C_SAM_DL6FS
    (Eason)
    *. Update pcb_system for CT909G_AAI_DV34_SAM_DL6
    (Sakura)
    *. Update pcb_system for CT909P_DM520_HTC_1200W 22

    [Ucode]
    Update version 207 for LS and short-seek protection

.Flash code Version
    - Version 1311.

/**************************************************************************
Version :    2.76
Date    :    2007.04.04 / servo 2.76(909R/P/G)
Chip    :    CT909R/P/G
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 02052007
. JPG Decoder Code Date
    - 02072007
. Display Code Date
    - 03292007
. DivX Code Date
    - 01192007
. Navigate Code Date
    - 02012007
. Parser Code Date
    - 04032007
. Usb Code Date
    - 10262006
. Info Code Date
    - 02092007
. Audio Code Date
    - 04022007
. Servo Code version and date
    - 03092007
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix USB power control pin define incorrect for CT952A system board { [PIO.H] }
    - Fixed mantis bug #2847 "The pause icon still exist while play mp3--> pause -->play jpg" { [mmanager] }
    - Avoid jpg hang issue while jpg decode process { [mmanager] }
    - Fix bug: set digital panel as same as still image for motion JPEG.  { [UTL] }
    - Porting Peter's code to fix DRAM type incorrect issue. { [HSYSTEM] }
    - Fixed mantis bug #2940 進入選單內Photo播放JPEG檔中途,重複按按STOP/DIGEST鍵至瀏覽圖片,再按ENTER鍵,會發生黑屏

. Enhance Items
    - Support audio effect feature { [HDECODER] }
    - Support CT952 16M solution. {cc.c/Dvd_dram_16m.h/Internal.h/Srcfilter.h/Usbsrc.c/Usbwrap.h/Utl.c}
    - New MAKE mechanism for CT909G/CT909P/CT95x

. Audio code
    - 909R
        - Fix MP3 VBR detect error for some ID3 format.
  
    - 909P
        Fix MP3 VBR detect error for some ID3 format.
        Fix key shift 爆音
        Fix WMA 爆音及系統死機 bug
        Add CDDA, LPCM de-emphasis 
            #define 	HAL_AM_CDDA_DEEMPHASIS			0x70  //1 bit
            0: disable
            1: enable

    - 909G
        Fix MP3 VBR detect error for some ID3 format.
        Fix key shift 爆音
        Fix WMA 爆音及系統死機 bug
        Add CDDA, LPCM de-emphasis 
            #define 	HAL_AM_CDDA_DEEMPHASIS			0x70  //1 bit
            0: disable
            1: enable

. Servo code
    [F/W]
    (Adam)
    *. Update LD-On delay time (MO_HD65G.H)
    (YHChen)
    *. Update TrackingBalance Offset table
    *. Modify TE offset adjustment(#define CT901C_NEW_TE_OFFSET_CANCELLATION)
    (David)
    *. Fix bug of veriable gbMLPF as system refill. 
   
    [Ucode]
    CT909G version keep 205

.Flash code Version
    - Version 1311.
    
/**************************************************************************
Version :    2.60
Date    :    2007.02.09 / servo 2.53(909R/P/G)
Chip    :    CT909R/P/G
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 02052007
. JPG Decoder Code Date
    - 02072007
. Display Code Date
    - 01262007
. DivX Code Date
    - 01192007
. Navigate Code Date
    - 02012007
. Parser Code Date
    - 02022007
. Usb Code Date
    - 10262006
. Info Code Date
    - 02092007
. Audio Code Date
    - 02082007
. Servo Code version and date
    - 01262007
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix AVI's "strf" header with 0 size data length.
    - Fix servo can't work @ CT909R system board issue. { [HSYSTEM], [PIO] }
    - Fix setup ID difference between DVD and DVB solution { [UTL] }
    - Fix NV packet may lost sometime issue  { [CELL] }
    - Fix motion JPEG zoom incorrect issue { [HAL] }
    - Fix motion JPEG decoding not completely issue { [UTL] }
    - Fix CGMS @ PSCAN + PAL/ PALN mode signal incorrect issue. { [HDEVICE] }
    - Fixed some picture get error YUV component ID that can cause decoding error
    - Fix scan mode failure when playing AVI files
    - Fix the bug 0002721: 當不小心關閉Power之後再播放檔案時會有問題. {cc.c}
    - Fix the problem about 32M JPEG screen saver has garbage. {osdss.c}
    - Fix Mantis Bug [0002606].
    - Fix one DivX disc abnormal when doing index table parsign problem. {mmanager.c}

. Enhance Items
    - Support FourCC code, MP4S/ M4S2 (MPEG-4) & MJPG (Motion JPEG).
    - Provide AVI information about audio's bits per sample.
    - Get and config bits per sample information for AVI w/ PCM audio stream { [UTL], [HAL] }
    - Support AVI w/ PCM mono audio stream { [UTL] }
    - Add the necessary pin and system platform define for CT909P-A/ CT909P-B/ DMO956A { [PIO.H], [INTERNAL.H], [WINAV.H] }
    - Support ADC key function.
    - 8 KEY for left/right/down/up/brightness/rotate/mode/play
    - TOUCH_KEY for touch pad
    - Support enabling DVD smartplay depend on the setting of customer.h
    - Add a definition to support "auto smart play by setup setting"
    - Modify navigate module to get current setup value via UTL_QueryCapability()
    - Support multi-AP selection UI. It only works when system enables the definition SUPPORT_PARSE_ALL_AP_FILES.
    - Support Serial Debugger plug-in
    - Support 909G
    - Support “full screen” in 16:9 panel. {haljpeg.c/haljpeg.h}
    - Add STB entry code. {media.c}
    - Translate the MACRO name.
    - Provide TS packet parsing for DVB solution.
    - Reserve the dir/file name when it is unicode file. {cdinfo.c/udf.c/mmanager.c/thumb.c}

. Audio code
    - 909R
        - #define HAL_AM_WMA_L1_DECODE 0x3
	    - WMA bitstream if WMA_L1 decoder can decode
	    - 0: can decode
	    - 1: can't decode
    
    - 909P
        - Fix AC3 S/PDIF RAW/PCM switch bug.
        - Modify PCM SCALE multiple to unsigned.
        - #define HAL_AM_WMA_L1_DECODE 0x3
	    - WMA bitstream if WMA_L1 decoder can decode
	    - 0: can decode
	    - 1: can't decode

    - 909G
        - Fix AC3 S/PDIF RAW/PCM switch bug.
        - #define HAL_AM_WMA_L1_DECODE 0x3
	    - WMA bitstream if WMA_L1 decoder can decode
	    - 0: can decode
	    - 1: can't decode
   


. Servo code
    [BM]
    -

    [Servo]
    - add new long seek profile.
    - add new four instructions INCX, DECX, INCY and DECY.
    - remove feov/teov related REGB.
    - remove redundant ramx/ramy.
    - add CT909G servo code(include RF part)(ok)
    - update LS ucode (code size: 1800)

    [ucode] 

.Flash code Version
    - Version 1310.
    
/**************************************************************************
Version :    2.37
Date    :    2006.10.2 / servo 2.36e(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 09282006
. JPG Decoder Code Date
    - 09282006
. Display Code Date
    - 09282006
. DivX Code Date
    - 09282006
. Navigate Code Date
    - 09282006
. Parser Code Date
    - 09282006
. Usb Code Date
    - 08252006
. Info Code Date
    - 09292006
. Audio Code Date
    - 09282006
. Servo Code version and date
    - 09132006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix "servo 喀拉喀拉響 while auto-upgrade from USB" issue
    - Fix "Pansonic TT/CC: 03/01 Stop/ Resume always playback from chapter beginning" issue. Now, it will playback from previous still picture.  { [CELL] }
    - Fix the bug #2341, "在編輯畫面下,若空一格在編輯,回到影片中,按PREV,畫面會先hang一下後取消PRG模式" issue. { [osdprgm.c] }
    - Fix the bug #2359, After play and full stop, set parental level from "8" to "4", will see abnormal "Error Parenal" OSD when password dialog appear issue { [osd3.c] }
    - Fix the OSD cllear uncompletely of input-password dialog { [osd3.c] }
    - Fix the translation error in Spanish { [Str_spanish.c] }
    - Fix Mantis 0002348: "金歌總動員" after enter menu, play TT/CC:3/1, if set "repeat chapter" is useless, it will auto play to next chapter. 
    - Fix resuming from still picture; it always start playing from the beginning. It is caused by updating wrong code from 908.
    - Fix the bug to detect FAT filesystem type
    - Fix decoder hang for J500 scratched title test.
    - Fix two WMA file that one packet size is 0x22f2 can't playback @ 16M solution.
    - Fix mpeg-cd can zoon problem. {cckey.c}
    - Fix mpeg-cd can SF problem. {cckey.c}
    - Fix quickly next/prev, then DVD sometimes entering LOGO problem. {infofilter.c}
    - Fix DVD <-> USB switch sometimes system hang problem. {media.c/usbsrc.c/srcfilter.c}

. Enhance Items
    - Use define NO_DISC_MODE to replace NO_VCD20. NO_DVD, NO_CDDISC define.
    - Update audio PTS to correct position. {[PARSER]}
    - Do not add index entry when file format is MP4 (Nero Digital). {[PARSER]}
    - Disallow do bookmark when the content is still picture to prevent video is abnormal after user return the point by using bookmark.
    - Enhance the watchdog mechanism to adjust time configuration
    - Support  MPLL 142Mhz
    - Support the AC3/WMA audio L/R function. {cc.c}

. Audio code
    - Enhance A/V sync and add debug informance.

. Servo code
    [BM]
    - Add 142MHz clock supporting.

    [Servo]

    [ucode] 

.Flash code Version
    - Version 1310.

/**************************************************************************
Version :    2.35a
Date    :    2006.9.1 / servo 2.35(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 08232006
. JPG Decoder Code Date
    - 08312006
. Display Code Date
    - 08232006
. DivX Code Date
    - 08232006
. Navigate Code Date
    - 08302006
. Parser Code Date
    - 08312006
. Usb Code Date
    - 08252006
. Info Code Date
    - 08312006
. Audio Code Date
    - 08242006
. Servo Code version and date
    - 08242006

/**************************************************************************
Version :    2.36
Date    :    2006.9.14 / servo 2.36b(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 09112006
. JPG Decoder Code Date
    - 09122006
. Display Code Date
    - 09142006
. DivX Code Date
    - 09112006
. Navigate Code Date
    - 09122006
. Parser Code Date
    - 09132006
. Usb Code Date
    - 08252006
. Info Code Date
    - 09122006
. Audio Code Date
    - 09132006
. Servo Code version and date
    - 09132006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix OSD will abnormal if OSD height w/odd line. {Disp.c}
    - Fix the problem about "Repeat A-B" message will be cleared by MSG_PLAYING in MM UI. {osd.c}
    - Fix the problem about NTSC/Auto mode JPEG will flash when power on is PAL mode in 32M/64M solution. {haljpeg.c}
    - Fix some thumbnail picture 會看到雜色. {JPEGFMT.c/JPEGFMT.h}
    - Fix the bug #2306: “進入Media Manager UI, 按function鍵再取消，當前播放的曲目仍然繼續，但是信息欄的OSD消失。” {osdmm.c}
    - Fix DivX Scan to Play or Stop to Resume 會先播一下前面再繼續的問題. { [LINEAR], [CC], [UTL] }
    - Porting Alan's code to fix bug #2025: "When program UI is turn-on, don't exit it while auto-playback to disc end" { [UTL] }
    - Porting Aron's code to fix J500 Scratched DVD title playback hang issue { [CELL] }
    - Reset __bFileSystem as none within INITIAL_Variables() ie, open/close or IR power to fix non DVD SCAN action abnormal after playback USB file first. { [INITIAL] }
    - Fix the bug #1716: Media manager will not refresh when usb un-plugin in setup mode
    - Fix the CVD issues when there are only sp data but no av data, the system will unstable
    - Fix the system will hang from USB mode to VCD x4 mode
    - Fix "It shows key invaild instead parental error when the parental level is not match"
    - Fix Mantis #0002242: Parental control abnormal which is caused by invaild nv info
    - Fix J500 Scratch %40 DVD title hang issue. 
    - Fix J500 Scratch %50 DVD title repeatthe same vobu issue.
    - Fix WMA can't resume when enter/exit media select UI in 16M solution. {cc.c}
    - Fix play one AVI disc and then the other AVI disc, then only see 76 files problem. {mmanager.c}
    - Fix MP3 scan in program mode, then press KEY_PLAY, will cancel audio scan mode but OSD still keep in scan mode problem. {mmanager.c}
    - Fix when issue play in MP3 scan mode + program edit mode, then will cancel audio scan mode but OSD keep in scan mode. {mmanager.c}
    - Fix preview mode but button is disabled problem. {mmanager.c}
    - Fix when resume from JPG preview to MP3, then JPG image don't clear problem. {mmanager.c}
    - Fix KEY_MENU in DAT file, then resume from DAT will fail. {mmanager.c}
    - Fix FUSS scratched VCD will hang in SERVO problem when play end. {monitor.c}
    - Fix FUSS scratched VCD will hang in monitor recover code problem. {monitor.c}
    - Fix the bug when the word length is over the OSD width. {char_subpict.c}
    - Fix when the VCD menu is motion, the track number is wrong after enter and exit the media-select UI for Mantis #2307. {cc.c}
    - Fix the bug that the colors of volume bar is wrong in 16M solution after Search UI has been invoked. {gdi.c}
    - Fix the bug when the password dialog is display, it is overlaped with OSDND. {osd3.c}
    - Fix the UI is not clear competely problem when the chapter number has 3 digit. {osdsrch.c}
    - Fix "DivX UI" will be cleared while random play. { [UTL] }
    - Fix ES buffer overflow and cause video mosaic at DivX Certification CD-ROM
    - Fix one case for "PARSER stop fail" issue.
    - Fix AVI audio switching cause "AVI ID error" issue.
    - Fix the Chip DivX capability incorrect.
    - Fix "SAMI" file parsing incorrect problem.


    
. Enhance Items
    - Provide Programmable Error Ratio. {Mpeg2dec.c/ mpeg2dec.h/ mpg_dram.c/ Comdec.c/ Comdec.h}
    - Add the new feature to support thumbnail==>KEY_MENU to enter MM UI. {mmanager.c/mmanager.h/thumb.c/thumb.h}
    - Support power down @ 27MHz . {Spflash.c/spflash.h}
    - Support power down mode @ 16M solution { [INTERNAL.H], [SPFLASH], [HSYSTEM] }
    - Support power down working frequency is 27MHz { [INPUT], [HSYSTEM], [HAL], [CC], [SPFLASH] }
    - Support MP3/ WMA repeat A-B feature { [MMANAGER] }
    - Config AVI index table/ SP buffer/ char-based SP output buffer/ Servo buffer runtime to enhance 16M solution DivX performance is not enough issue { [INTERNAL.H], [UTL], [MMANAGER], [DVD_DRAM_16M.H] }
    - Always stop @ current file while DivX SCAN to file end during DivX certification case. { [UTL] }
    - Just do GoTime 0:00:00 while DivX scan to file beginning to fix DRM limit will decrease by 1 while scan to file beginning issue { [UTL] }
    - Enhance “parser stop fail” issues in monitor recover mechanism
    - Change the method to get code protection range
    - Enhance the CDROM disc recognize time. {cdinfo.c}
    - Support the define, SUPPORT_NUMBER_CONFIRM_KEY for Mantis #1722. {digest.c}
    - Support the DRM feature in 16M solution for Mantis #2309. {osddlg.c}
    - Enhance the MPEG-2 header parsing error concealment
    - Enhance parser hang while trapping to read data from BIU.

. Audio code
    - Fix AC3 hang while A1 buffer can't find header for long time.
    - Fix MPEG A/V sync bug for initial PTS is not one.
    - Fix STB audio hang bug and increase A0 remainder starting decode frame to 32 frames.
    - Reduce WMA run-level talbe for wma.bin and wma_l1.bin

. Servo code
    [BM]
    - Fixed the bug for SPECIAL_ERR_DECOD_RECOVER_SmallPILargePO
    - In the power down mode,Servo clcok is controled by #define SW_EXPAND_GPIO 
    - Enable #define TURN_OFF_RFIC_ADC6_POWER_MANAGEMENT
    - Delete #define CT909_SAVE_POWER
    - #define NEW_READIN_RETRY for CD disc(defalut:enable)

    [Servo]
    - Move D_CNT from 0x304 to 0x229 since 0x304 register is not available.
    - Support #define AutoTuning_SERVO(If define Autotuning_SERVO, please must also define Autotuning_MPEG at MPEG code)
    - Update qobserver.c file to fix the bug that Qcode will be wrong in lead out area.
    - Add a protection on case 3 of Disc ID state to avoid to misjudge CD title as "no disc".
    - Remove the PLL unlock check in SRVTB_SctClose2Target function.
    - Enable noise shaper as default setting.  
    - Swap MIRR calibration variables when the recorded varialbe is confilct to real layer number.
    (gbRF_MRCR2, gbRF_MRCR3, gbRF_MRCR21, gbRF_MRCR31)
    - Enable RF Amp. TE mask function for both seek and following.

    [ucode] 
    - Update ucod version to U950905_V20 
    - Move D_CNT from 0x304 to 0x229 since 0x304 register is not available.
    - Add auto tuning part. 
    - Change BIT err_tv

.Flash code Version
    - Version 1310.

/**************************************************************************
Version :    2.35a
Date    :    2006.9.1 / servo 2.35(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 08232006
. JPG Decoder Code Date
    - 08312006
. Display Code Date
    - 08232006
. DivX Code Date
    - 08232006
. Navigate Code Date
    - 08302006
. Parser Code Date
    - 08312006
. Usb Code Date
    - 08252006
. Info Code Date
    - 08312006
. Audio Code Date
    - 08242006
. Servo Code version and date
    - 08242006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix can't do DSU auto upgrade in 16M solution
    - Fix don't do recover in scratched VCD sometimes.
    - Fix quickly insert/remove USB, then susyem sometimes hang problem.
    - Fix system hang in parser problem.
    - Fix MPEG PTS problem for AV sync.
    - Fix DVD gototime abnormal problem.
    - Fix some DVD parent control abnormal problem.
    - Fix some JPG color abnormal problem.

. Enhance Items
    - Enable watch dog in default.
    - Enable UDF in 16M solution.
    
. Audio code

. Servo code
    [BM]

    [Servo]

    [ucode] 

.Flash code Version
    - Version 1310.

/**************************************************************************
Version :    2.35
Date    :    2006.8.25 / servo 2.35(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 08232006
. JPG Decoder Code Date
    - 08232006
. Display Code Date
    - 08232006
. DivX Code Date
    - 08232006
. Navigate Code Date
    - 08232006
. Parser Code Date
    - 08242006
. Usb Code Date
    - 08252006
. Info Code Date
    - 08232006
. Audio Code Date
    - 08242006
. Servo Code version and date
    - 08242006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix the bug when pressing KEY_DIGEST in the digest dialog can’t exit. {digest.c}
    - Fix the bug when in the mute mode and enter/exit the digest mode, the mute icon is there but it has sound. {digest.c}
    - Fix Mantis #0002047: parental control abnormal. {dvdinfo.c / navigate.c}
    - Fix Mantis #0002109: Title "FIN.K.L" can't play. {cell.c}
    - Fix Mantis #0002115 AV hang when play DVD title "DOOM". {cell.c}
    - Fix side-effects of media.c in DVD_232+DVB_108. {media.c}
    - Fix can't playback anything after detection DVD title "Doom"must assign cell action as VOBU_END while the VOBU only has NV sector { [CELL.C] }
    - Fix data can't save into SPI Flash sometime issue { [HDEVICE] }
    - Fix mantis bug @ 2109: Title "FIN.K.L" can't play. { [CELL] }
    - Fix LOGO do scaling after play CDG title. {disp.c/ disp.h}
    - Fix white dot appear left-up corner. {disp.c}
    - Fix Mantis #0002191: DIVX "smi_test.smi" char-based subtitle show strange font : "&nbsp;". {par_sptx.c}
    - Fix Mantis #0002158: MTC DIVX TEST disc, 9_03/9_04/9_06 char-based subtitle show strange font. {par_sptx.c}
    - Fix Mantis #0002051: DIVX test title, "TEST_9_05", show abnormal char-based subtitle, such as <font color="#e8e8e8">. {par_sptx.c}
    - Fix Mantis #0001910: The display char of one SRT sub-title is abnormal. {par_sptx.c}
    - Fix real STOP won't clear MM program entries problem. {cckey.c}
    - Fix one disc can't recogize OK problem. {infofilter.c}
    - Fix one FUSS recognize disc wrong because of EDC error problem. {infofilter.c}
    - Fix one CD-DTS play with noise problem by Kevin's code. {infofilter.c}
    - Fix char sub-title maybe use wrong buffer problem. {mmanager.c}
    - Fix stop from AVI scan mode, then play MP3, then can't pause problem. {mmanager.c}
    - Fix the problem about the message can't be cleared when time out if the current message is not normal upper or bottom message. {osd.c}
    - Fix JPEG slideshow hangs when pressing KEY_NEXT or KEY_PREV continuously and quickly. {JPEGFMT.C/JPEGFMT.H}

. Enhance Items
    - Remove some limitation and beautify output message of makerom tools. {Bootcode/Makerom}
    - Support the chroma setting in Setup menu. {setup.c}
    - Give 2nd parameter as __dwTimeEnd for UTL_PlayFomTime() since UTL_PlayFromTime() & UTL_FATPlayFromTime() integration. { [UTL], [DIGEST], [CELL], [MMANAGER], [MONITOR] }
    - Support WMA SCAN, Stop/ Resume feature { [CC], [UTL], [MMANAGER], [LINEAR], [HDECODER] }
    - Support run-time configure A/V buffer address for AVI file { [HDECODER], [UTL] }
    - Support TVE 7.5IRE/ chrom adjustment setting { [HDEVICE] }
    - Only use one audio buffer for MPEG/ MP3 audio stream { [HDECODER], [HAL] }
    - Enhance PTS calculation formula for MicroDVD. {par_sptx.c}
    - Support issue pause when SCF/SCB mode. {cc.c/cckey.c/mmanager.c}
    - Add protection code to avoid system hang in while loop when servo send wrong data. {cdinfo.c}
    - Disable program list button when program palying mode. {mmanager.c}
    - Set display when switching between JPEG preview, slide show and thumbnail. {haljpeg.c/haljpeg.h}
    - Force to open display in thumbnail mode to prevent the screen black until the first thumbnail is decoded. {thumb.c}
    - Enhanced JPEG quality for some resolution. {JPEGFMT.C/JPEGFMT.H}
    - Update the DivX DRM version to v3.0.2. {parser.h/ pardrm.a/ parnodrm.a}
    - Change the PTS calculation formula for fixing PTS rounding issue after playback long time (3 hours). {parser.c/ par_avi.c/ par_avi.h/ par_dram.c}
    
. Audio code
    - Fix DTS will hang while long data has no sync words.
    - Add AC3 skip decode PCM command.
    - #define???? HAL_AM_DECODE_DTS?????????????? 0x2b
    - Release MPEG one buffer.
    - Fix WMA bug while ACBUF2 is small.
    - Remove WMA write FRAME_COUNT to zero while wma initialization
    - Add WMA L1 vresion release

. Servo code
    [BM]

    [Servo]

    [ucode] 

.Flash code Version
    - Version 1309.

/**************************************************************************
Version :    2.32
Date    :    2006.8.8 / servo 2.31a(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 07142006
. JPG Decoder Code Date
    - 07122006
. Display Code Date
    - 07142006
. DivX Code Date
    - 07142006
. Navigate Code Date
    - 07142006
. Parser Code Date
    - 07122006
. Usb Code Date
    - 07122006
. Info Code Date
    - 07142006
. Audio Code Date
    - 07132006
. Servo Code version and date
    - 07122006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix the bug #2066: "angle mark 在setup menu 取消後,繼續撥放時,並無消失. {osdnd.c}
    - Fix the bug #2064: "Program list Jpeg選擇四張以上圖片,按下play delete中間部分的一張圖,前一張圖小圖會不見" {thyumb.c/thumb.h}
    - Fix the bug #1950: “Navigator bar dual subtitle function abnormal: cannot display the 2nd subtitle of this title”
    - Fix Mantis #0002047: parental control abnormal 	
    - Fix 0002044: 六宮格由A節目按左右鍵至B節目,小畫面下半部會閃一下. {mpeg2dec.c}
    - Fix 0002070: HT1-edit1-5.03-b.avi如果一開始就撥放此檔案,會無法撥放,但是如先撥放其他檔案,此檔可撥放 {decore.c}
    - Fix 0002087: 切換頻道會頓. {disp.c/ disp.h}
    - Fix Mantis bug #1898: DVD 刻錄 title, 播放過程停頓 { [CELL] }
    - Fix Mantis bug #1405: DVD Menu can't playback to end issue while release mode, ex. "Harry Potter", "Alexander" { [CELL] }
    - Fix GoTime fail while playback "奇聲" DVD title -- The transporter w/ search time < 5:00 { [CELL] }
    - Fix Mantis bug #1949: Hang @ Toshiba screen while playback "Toshiba_2004" DVD title since NV information continue wrong. { [CELL], [UTL] }
    - Fix mantis bug #1916: Title"mission impossible 2", play TT/CC:1/4, 0:44:00, OSD time error(亂跳), AV normal { [UTL] }
    - Fix scan mode to end of program file, then UI abnormal problem. {cckey.c/mmanager.c}
    - Fix one disc will loss 140 files in one dir problem. {cdinfo.c}
    - Fix seeing OSD when entering media select UI in thumbnail mode. {media.c}
    - Fix sharing memory mechanism maybe destroy file record problem. {mmanager.c}
    - Fix next/prev in program list mode will see scan OSD problem. {mmanager.c}
    - Fix 16M effect abnormal after entering thumbnail mode. {mmanager.c}
    - Fix bug 0002039: 播放照片,有1/3處會播不出來
    - Fix bug 0002055: first show no USB device, then loading…
    - Fix the problem about slie show with some effect will 抖動 in 16M solution. {haljpeg.c/haljpeg.h}
    - Fix mantis bug 2048, AVI file "Just Married", go-to time to 1:00:00, the char-based subtitle 不同步 w/ audio. {par_avi.c}
    - Fix zero size video chunk leads frame rate incorrect issue. {par_avi.c}
    - Fix mantis bug 2051, DIVX test title, "TEST_9_05", show abnormal char-based subtitle, such as <font color="#e8e8e8">. {par_sptx.c}
    - Fix mantis bug 2050, DIVX test title, "TEST_8_09" switch to 2nd audio language have no audio output. {lparser.h/parser.h/parser.c}

. Enhance Items
    - Customer reauest #2076: "PAL mode File manager畫面要置中". {osdmm.c/osd.c}
    - Customer reauest #2080: "Divx 顯示剩餘時間". {osddsply.c/osddsply.h}
    - Customer reauest #2083: "navigator每一個子項目要做xy調整,調整到對齊". {osddsply.c/osddsply.h/osdnd.h}
    - Add "AV In" button. {osddlg.c/osddlg.h/notransl.c/notransl.h}
    - Add DVD_Load_VTSTMAP to get TMAP entris to do error concealment for DSI with incorrect SRI data.
    - Enable/disable SUPPORT_TMAP_SEARCH in dvdinfo.h to turn on the function
    - Set motor mute when disabling servo for DVB mode to prevent DVB->USB auto-upgrade affects loader motor.
    - Add AV IN code. {cc.c/cckey.c/media.c}
    - Enhance PTS method for MicroDVD. {par_sptx.c}

. Audio code
    
. Servo code
    [BM]

    [Servo]

    [ucode] 

.Flash code Version
    - Version 1309.

/**************************************************************************
Version :    2.31
Date    :    2006.7.28 / servo 2.31(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 07142006
. JPG Decoder Code Date
    - 07122006
. Display Code Date
    - 07142006
. DivX Code Date
    - 07142006
. Navigate Code Date
    - 07142006
. Parser Code Date
    - 07122006
. Usb Code Date
    - 07122006
. Info Code Date
    - 07142006
. Audio Code Date
    - 07132006
. Servo Code version and date
    - 07122006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix mantis bug #1934: 當Parental Level 1時,欲播放有Parental control之碟片,而"Please input password"之OSD顯示模糊.. {osd3.c}
    - Fix JPEG slideshow zoom 25% 會造成圖片顯示錯誤。
    - User could choose which function he wants to enable.
    - Fix mantis bug #001939: Repeat AB任一時間點皆會無聲音輸出;且取消Repeat AB後,時間亦會亂跳 { [CELL.C] }
    - Fix IR power on fail after IR power down @ 16M solution { [HAL.C], [INTERNAL.H] }
    - Fix one AVI title "008" playback un-smooth issue { [HDECODER.C], [UTL.C] }
    - Fix "閃一個奇怪畫面" while one CDROM motion playback --> stop { [HDECODER.C] }
    - Fix pre-decode fail issue while playback AVI from USB path by stop --> resume { [UTL.C] }
    - Fix mantis bug 0001930: Mona's *.mpg; 快進*2之速度較快進*4以上之速度快;且快進*4時畫面看起來像是步進 { [MMANAGER.C] }
    - Fix mantis bug #1932: OSD 多 2 sec while SCFx4 Mona's *.mpg file to file end  { [MMANAGER.C] }
    - Fix 0001935: 當Parental Level 1時,播放有Parental control之碟片,輸入錯誤之pass word,結果瞬間出現"Error Parental"之OSD. 
    - Fix J300 CHChen feedback some disc looks like hang when recognizing scratch DVD title
    - AVI files unable to play in USB. {FAT.C,usbsrc.c}
    - AVI files result in system hang in USB. {FAT.C,usbsrc.c}
    - MP3 stop -> resume, time is not correct in USB. {FAT.C,usbsrc.c}
    - AVI files stop ->resume, the waiting time is too long in USB. {FAT.C,usbsrc.c}
    - Fix the bug #1914: "Pressing KEY_VOLUME, when the volume bar is not disappeared, pressing KEY_PROGRAM. The program UI will be cleared by normal display message in 16M solution." {osd.c}
    - Fix XviD title can't play if part no. is zero.
    - Fix SVCD can't do audio switch problem. {cc.c}
    - Fix dir record maybe destroyed when decode char sub-title problem. {mmanager.c}
    - Fix Vestel UDF disc can't read problem. {udf.c}
    - Fix seeing scan message when return from MP3 program scan. {mmanager.c}
    - Fix when no define SUPPORT_TUNE_JPG_SHOW_TIME, then KEY_SCF in JPG will wrong. {mmanager.c}
    - Fix seeing wrong char when some char are unkkown in UDF disc. {mmanager.c}

. Enhance Items
    - Reduce MPEG decoder stack size since only AVI decoding need larger space. { [INITIAL.C] }
    - Modify internal audio DAC gain value for Dolby and 國標 certification (Default for 國標)  { [HDECODER.C] }
    - Add error protection while last SCR < lst SCR { [UTL.C] }
    - Always re-config A/V buffer address base on parser reported ratio for each AVI file playback { [HDECODER.C], [UTL.C] }
    - Support Resume of no index table AVI file after it has built some index table done  { [CC.C]. [MMANAGER.C], [Parser code], [Decoder code] }
    - Support repeat A-B of no index table AVI file after it has built some index table done { [LINEAR.C] }
    - Enhance navigation error concealment to try to linear playback after load menu fail.
    - Support more external subtitle format, Microsoft-SAMI, DVDSubripper, DVDSubtitle, TMPlayer Format 1/2,TMPlayer+ Format 1/2
    - Check invalid chunk size by movi chunk size
    - Support runtime create index table for AVI w/o index
    - Increase Video buffer to 74K DW @ 16M.
    - Support GotoTime w/o turn off mainvideo.
    - Enhance the scratched DVD loop in code problem. {infofilter.c}
    - Support serial flash auto test at first booting time

. Audio code
    
. Servo code
    [BM]

    [Servo]

    [ucode] 

.Flash code Version
    - Version 1309.

/**************************************************************************
Version :    2.30
Date    :    2006.7.17 / servo 2.21b(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 07142006
. JPG Decoder Code Date
    - 07122006
. Display Code Date
    - 07142006
. DivX Code Date
    - 07142006
. Navigate Code Date
    - 07142006
. Parser Code Date
    - 07122006
. Usb Code Date
    - 07122006
. Info Code Date
    - 07142006
. Audio Code Date
    - 07132006
. Servo Code version and date
    - 07122006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix the bug #1911: "When showing information, the last character of file name is overlap with the other field. " {osddsply.c}
    - Fix the bug #1900: "When leave "SETUP" on mm mode, JPEG picture' resolution and file date can not recover " {mmanager.c}
    - Fix the bug about there is some noise when open==>close or stop==>play. {cdg.c}
    - Add the mechanism to break the waiting bitstream if there is no data in. {cdg.c}
    - Add the mechanism to prevent to do "deactivate region" and "inactivate region" but the region info. is the same. {osd.c/osd.h/osddsply.c/osdprgm.c/osdbookmark.c/setup.c/thumb.c/osdsearch.c/osdmm.c}
    - Fix the bug #1779: "The CDG video is too large (clip and exceed the screen) after playing DVD==>CDG".  {cdg.c}
    - Fix the problem about clip width will be 0 for display and cause "divide 0" after detecting "Picture CD". {mmanager.c}
    - Rremove the un-translated strings and move them to notransl.c to reduce DRAM space. {notransl.c/notransl.h/osdnd.c/osddsply.c/thumb.c/osdmm.c}
    - Fix 16M DRAM autoupgrade fail. {Flash.bin/flash_dsu_main.upgrade}
    - Fix Decoder hang issue under BFR Mode. {mpeg2dec.c/ decore.c}
    - Fix unexpected frame display if switch among different width. {mpeg2dec.c/ decore.c/ disp.c}
    - Fix 0001901: JPEG 作effect過程中，按display，則正在作effect的那張圖會花掉. {disp.c}
    - Fix divided by zero bug. {disp.c}
    - Fix Video will hang at PSCAN TV. {disp.c}
    - Fix bug #1886: Some normal display OSD won't disappear.. {setup.c}
    - Fix bug #1893: The colors of "OPEN" and "CLOSE" are different, no matter in 16M or 64M. {char_subpict.c}
    - Fix bug #1892: No thumbnail picture is displayed after exiting from setup UI. {setup.c}
    - Fix bug #1807: zoom -> setup 出現綠色畫面. {setup.c}
    - Fix bug 1902: “JPEG作垂直方向的effect時，其畫面會變得比較模糊”
    - Fix bug 1885: “There are abnormal image in bottom on 64M JPG preview mode”
    - Fix 0001865: DVD判到DVD碟後，不再繼續播放而停在LOGO狀態，按PBC OFF就可以往前播，這時再按MENU可以回到菜單	 
    - Fix 0001864: DVD讀到WARNING部分時，OSD始終停在0:00:00,要按PLAY鍵才可以繼續播放 	
    - Fix "video crash while playback "Pansonic" DVD TT/CC:03/01 24bit/ 96KHz LPCM w/ language study operation { [HAL], [UTL], [HDECODER], [NAVIGATE] }
    - Fix "One DVD title's Menu can't display" -- (reported by Roadwell) { [CELL] }
    - Fix audio stream information is wrong while playback AVI w/ WMA audio (only happen @ 16M solution { [UTL] }
    - Fix "閃一個奇怪畫面 while < 480 witch motion --> MM UI { [HDECODER] }
    - Fix can't resume to CDG when exit setup/media select UI in 16M mode. {cc.c}
    - Fix JPG can't next when exit media select UI in 16M mode. {cc.c}
    - Fix setup disappear when entering setup during disc recognizing. {infofilter.c}
    - Fix next quickly when decoding in effect mode, then image abnormal. {mmamager.c}
    - Fix one UDF disc can't recognize dir in root when there are un-supported files in root problem. {udf.c}

. Enhance Items
    - Add MSG_ZOOM_TEXT for displaying zoomx? when displaying graphics zoom for Vestel. {osd.h/osdnd.c}
    - Support runtime generating Index table and let the AVI w/o Index can do resume operation.
    - Support A/V buffer ratio depends on current AVI streams' information, limited range is 2:8 <= A:V <= 5:5
    - Added filter to ignore describe tag. {PAR_SPTX.C}
    - Enhance GDI_DeactivateRegion wait Vsync a little. {Gdi.c}
    - Add error conceal ment for DVB AV hang
    - Cable to pre-scan action for avi files in usb
    - Support char-based sub-picture for avi files in usb
    - Support mpeg error concealment.
    - Modify SCART procedure to meet new SCART circuit (only use 3 transistor) { [HAL] }
    - Add new zoom message. {cckey.c/mamaner.c}
    - Support easier way to switch 16M/32M/64M DRAM config mode. {makefile}


. Audio code
    
. Servo code
    [BM]

    [Servo]

    [ucode] 

.Flash code Version
    - Version 1309.

/**************************************************************************
Version :    2.22
Date    :    2006.7.17 / servo 2.21b(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 07142006
. JPG Decoder Code Date
    - 07122006
. Display Code Date
    - 07142006
. DivX Code Date
    - 07142006
. Navigate Code Date
    - 07142006
. Parser Code Date
    - 07122006
. Usb Code Date
    - 07122006
. Info Code Date
    - 07142006
. Audio Code Date
    - 07132006
. Servo Code version and date
    - 07122006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix a AVI bug: ID will be incorrect when AVI has more than 10 streams included. {parser code}
    - Fix mantis bug #0001787: JPG preview will make background bottom image abnormal { [UTL], [HALJPEG], [HDECODER] }
    - Fix mantis bug #0001819: Display background abnormal after TV mode switched for MP3 playback { [UTL], [HALJPEG], [HDECODER] }
    - Fix mantis bug #0001793: JPEG picture will be corrupted when enter Setup Menu { [UTL], [HALJPEG], [HDECODER] }
    - Fix mantis bug #0001799: Enter setup mode, JPEG video doesn't be cleared. 	{ [UTL], [HALJPEG], [HDECODER] }
    - Fix "can't playback while press KEY_NEXT of 兆馳 No5 title DVD+RW" (外圈染料有問題) { [MMANAGER] }
    - Fix mantis bug #1822, #1830, #1832: The Search UI will clear the whole region including the mute, angel icons and volume bar. { [OSDSRCH] }
    - Fix mantis bug #1824: Hardly to see the words of GoToTime UI when style1  { [OSDSRCH] }
    - Fix mantis bug #1826: Strange UI(Invalid UI) disaply on screen  { [OSDPRGM] }
    - Fix mantis bug #1826: 當播放 CDG, 開啟 setep 與 program 選單, 造成 AV 停止不正確   { [OSDPRGM] [SETUP] }
    - Fix mantis bug #1843: Strange UI(Invalid UI) disaply on screen  { [OSDPRGM] }
    - Fix mantis bug #1780: In GotoTime mode, pressing KEY_MENU will be back to menu but doesn't exit GotoTime mode. { [OSDPRGM] }
    - Fix mantis bug #1792: Setup menu will be corrupted when JPEG decoding is finished at background and start to show picture. { [SETUP] }
    - Fix mantis bug #1775: External subtitle can't display if TV system is PAL   { [CHAR_SUBPICT] }
    - Fix mantis bug #1777, #1835: Can't triggle program play by KEY_PLAY   { [OSDPRGM] }
    - Fix mantis bug #1802: Long subtitle will exceed screen   { [CHAR_SUBPICT] }
    - Fix mantis bug #1825: Mute icon isn't display, but Audio is muted   { [OSDPRGM] }
    - Fix mantis bug #1834: It will auto-resume after exit program when STOP mode   { [OSDPRGM] }
    - Fix mantis bug #1837: Some Words of Program will out of border when choose French   { [OSDPRGM] }
    - Fix the bug #1836: "Volume UI is wrong when KEY_VOL -> KEY_SETUP -> KEY_SETUP(Exit) -> KEY_VOL" {osd.c}
    - Fix the bug #1778: "While a DVD title is playback, pressing KEY_MUTE and then enter and exit Program mode in 16M" {osd.c}
    - Fix the bug #1781: "The UI of volume bar is wrong. " {osd.c}
    - Fix the bug #1827: The brightness of Loading/Open/Close/No Disc/... UI are different between Power on and KEY_Open_Close. " {colordef.h/Char_subpict.c / char_subpict.h}
    - Fix the bug #1844: "System hang when play last track of CDG (TIP TOP)" {cdg.c}
    - Fix the bug #1859: "Enable display mode -> enter/exit setup -> volume OSD disappeared". {osdsrch.c/osddsply.c/osdprgm.c/osddlg.c}
    - Fix the bug #1849: "CDG 畫面右移 ". {cdg.c/utl.c}
    - Fix usb.a reference external #define DS_USB_BUFF_ST bug.  {usb.c, usb.h, usbsrc.c, media.c}
    - Fix the bug # 1794: “MM menu will be correpted when JPEG is decoding and press KEY_MENU”
    - Fix the bug # 1803: “Play MP3 from JPG slide show can see background image abnormal”
    - Fix the bug # 1801:”JPG resolution will be worser a while when starting doing effect mode changing”
    - Fix the bug # 1817: “Blink happened when do flipping/rotation for JPEG pictures”
    - Fix the bug # 1818: “Grayscale abnormal on 16M solution”
    - Fix the bug # 1788: “The positon of JPG preview is not center”
    - Fix some disc stack overflow problem when doing disc recognizing. {initial.c}
    - Fix JPG preview image not in center problem. {mmanager.h}
    - Fix one disc will destroy LOGO when doing disc recognizing. {mmanager.c}
    - Fix moving to some dir will destroy UI problem in one disc. {mmanager.c}
    - Fix sometimes MM UI will be destroyed when stop from AVI/DAT/VOB. {mmanager.c}
    - Fix MM UI will be destroyed when enter UI from JPG decoding. {mmanager.c}
    - Add code to disable main video when doing rotate to avoid seeing some abnormal image. {mmanager.c}
    - Fix Picture CD can't recognize correctly problem. {utl.c}
    - Fix decoder stopped automaticly if picture size error. {mpeg2dec.c/ decore.c}
    - Fix frame convertion bug. { disp.c }
    - Fix show the wrong video among different frame buffer address setting. 

. Enhance Items
    - Improve DivX DRM error concealment. {parser code}
    - Support reserved 4CC code, RES1/ RES2/ RES3, for DivX Ultra. {parser code}
    - Enhance the response time of CLEAR_FEATURE and CHK_TD case  {usb.c, usb.h}
    - Improve AVI error concealment
    - Support media select UI to show correctly and will stop the system when enter UI and resume when exit UI. {media.c/cc.c}
    - Add code to show the long filename more reliable. {fat.c}
    - Avoid to do BFR Mode if vertical size <= 480. Now lack the flow of DVD & DAT/MPG/ VOB, but unit test is ok. {mpeg2dec.c/ comdec.c/ comdec.h/ decore.c/ mp4_picture.c/ mp4_vars.h/ dvd_dram_16m.h}

. Audio code
    - Fix WMA noise bug causes by discontinuous bitstream.
    - Fix MPEG hang for A1REM zero.
    
. Servo code
    [BM]
    - add the protection for Dumpin timeout abort(BMST_SeekControlState(),srvbm.c)
    - add captureRF function(#define DEBUG_CAPTURE_RF)
    - add PI erasure count into Multi-ECC path filter(ISR_SRVBM(),srv_dram.c)
    - add API(BMST_SetServoClock() for MPEG setup the servo system tixck(srvbm.c)
    - add SONY_313_TONIC platform

    [Servo]
    - Fix the bug that BXPLLKP didn't update correctly after JE calibration. (srvcal.c)
    - Add a function for returning servo result code. (#define NEW_RET_SERVO_RESULT_CODE_SCHEME)
    - Update HD65 FCCR table. CD:(120%) RFCK=66.5MHz ; DVD (110%) RFCK=133MHz. (sv_hd65.c)
    - Update correct LJEND_TRK for long seek tuning. (sv_hd65.c)
    - For CD, LJEND_TRK cannot be greater than 511. For DVD, LJEND_TRK cannot be greater than 2047. 

    [ucode] 
    - Release U950712_V17 version.
    - Fix long seek bug for auto-tuning.

.Flash code Version
    - Version 1308.

/**************************************************************************
Version :    2.20
Date    :    2006.6.20 / servo 2.20(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 06162006
. JPG Decoder Code Date
    - 06152006
. Display Code Date
    - 06162006
. DivX Code Date
    - 06162006
. Navigate Code Date
    - 06022006
. Parser Code Date
    - 06192006
. Usb Code Date
    - 02092006
. Info Code Date
    - 06192006
. Audio Code Date
    - 06192006
. Servo Code version and date
    - 06192006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix audio output is wrong @ CT909S_EVAL exteranl Audio DAC  { [PIO.H], [WINAV.H] }
    - Fix "CDG font is destroyed @ 32M solution" { [UTL.C/ H] }
    - Fix audio noise issue @ language study mode { [HDECODER.C], [UTL.C] }
    - Fix the bug that external subtitle loads a wrong 1-bit font table.  { [char_subpict.h], [char_subpict.c] }
    - Fix the PROGRAM UI clipped by OSDND bug in 16M solution { [osdprgm.c] }
    - Fix CT219_J000 bug: [0001625] At Radio mode, it is easy to occur "CMD_START failed since PARSER is running" when on off instant-replay very quickly.
    - Fix customer give picture can cause UV shift. {jpg code}
    - Fix the mantuis bug [0001670] The service name will be "unknown" if interrupt the auto-search several times. {Parser.c/ par_dvb.c}
    - Fix the bug #1690: "When PAL mode, enter SETUP mode and then exit, press KEY_VOL_UP/DOWN or MUTE, OSD cannot show at left lower corner. NTSC mode will not occur. " {osd.c}
    - Fix the bug #1652: "Zoom and move AVI file, cannot move to the left or right border". {osdnd.c/hal.c}
    - Fix the bug #1704: "Navigate Bar repeat menu item "repeat disc" should be invalid when navigation on. " {osddsply.c/osddsply.h}
    - Fix the bug #1652: "Zoom and move AVI file, cannot move to the left or right border". {osdnd.c/hal.c/ hal.h}
    - Fix Mantis #1650, Video shift left: Use TV setting as our default.(HAL.C) But this issue need patch display module too.
    - Fix mantis bug #1636: Some AVI file REPEAT AB will pause several seconds and play { utl.c/ winav.h }

. Enhance Items
    - Support loading one OSD language to DRAM each time to support 32M Europe language mode.
    - The code is ready for buffer config @ different playback mode { [UTL.C/H], [HAL.H] }
    - Provide a new API to read back all port status of expand GPI once time. { [HIO.C/ H] }
    - Restore the reload scale value calculation method. (It's wrong only @ DVD_218) { [HSYSTEM.C] }
    - Don't to protect font table base on define "UNPORTECT_FONT_TABLE" { [HSYSTEM.C] }
    - Support an API to specify which 1-bit font table will be used. { [osdfont1.c] }
    - Support dynamic OSD language loading. [MAKEFILE / ROMLD]
    - Add MCLK configuration. [BOOT] 
    - Cross platform printf. [SUBPICT] 
    - Support a flexible method to map avi files to sub-picture files in iso9660 { [cdinfo.c], [mmanager.c] }
    - Shrink the size at sections .data and .rodata
    - Set down mix mode for SD0. {Chips.h/chips.c}
    - Add new VD code.
    - Support FAT12 file system. {infofilter/mmanager}

. Audio code
    - Fix MPEG LSF sampling  rate detect failed
    - Fix MPEG audio sound will be discontinuous at beginning
    - Fix language study bug in karaoke.c
    - Map karaoke and prologic bss at 0xb0000000 for MPEG
    - Reduce code size
    - Fix one AVI MPG water flow noise problem.
    - Fix A/V sync wait 2 second bug
    
. Servo code
    [BM]
    - Fixed the BM under buffer protection bug @32M,2 block
    - Support CD-DA Multi-session(disable #define XINGQIU_MULTI_SESSION_CDDA_PLAYBACK)
 
    [Servo]
    - For HD65 solustion, PBC_SYSTEM=2 is removed. Please choose PBC_SYSTEM=1 based on the correct hardware.
    - More retry times while getting layer number(srvservo.c)
    - add new long seek control option(#define NEW_LONG_SEEK)
 
    [ucode] 
    - Release U950614_V15 version.
    - Fix auto RMP_REG bug. Because KGLPF2 uses unavailable register, move this register from 0x303 to 0x228.

. Flash code Version
    - Version 1307.

/**************************************************************************
Version :    2.18
Date    :    2006.6.06 / servo 2.18a(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 05262006
. JPG Decoder Code Date
    - 05302006
. Display Code Date
    - 06062006
. DivX Code Date
    - 06022006
. Navigate Code Date
    - 06022006
. Parser Code Date
    - 06022006
. Usb Code Date
    - 02092006
. Info Code Date
    - 06022006
. Audio Code Date
    - 06012006
. Servo Code version and date
    - 06062006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix the problem about "mute"/"Open" will not recovered after exiting SETUP. {osdnd.c} 
    - Fix the problem about KEY_DISPLAY will clear the normal display message. {osddsply.c}
    - Add mutex for I2C and Expand GPIO action conflict issue { [HIO.C] [HIO.H], [INITIAL.C] }
    - Fix "scan mode abnormal while playback DivX w/o index table parsing done --> SCAN x2 --> SCAN x4" { [MMANAGER.C] }
    - Fix "warning message" is lost for DVD "2000 新年音樂會" { [DVDINFO.C] }
    - Fix "bo" noise from internal Audio DAC after title detection done { [INITIAL.C], [HDECODER.C] }
    - Fix the problem about remain OSD problem. {osdnd.c}
    - Fix the problem about the info. in thumbnail is incorrect for some special JPEG format. [thumb.c]
    - Fix mantis bug 1400 and 1563 “按Mute鍵->按Volume+/-(取消Mute);但仍會無聲音輸出. {chips.c}
    - Fix setup menu turn off SPDIF, but still out put problem. {chips.c}
    - Fix SP OSD region size "incorrect" issue. {gdi.h}
    - Fix font data doesn't update completely for teletext font vertical scaling. {gdi.c}
    - Fix “Code Protect” no effect problem. {boot code}
    - Fix AVI 2.0 data overwrite issue {parser code}
    - Fix one VCD title (from 奇聲) 1st track can not play issue. {parser code}
    - Fix video buffer overflow at 16M solution. {parser code}
    - Fix it play error  when one cluste is two usb sector. {usbsrc.c}
    - Don't clear BFRMode in DISP_FrameBufferMode(). Fix error video scaling when BFR Mode. {Disp.c}
    - Don't reset Servo when load AP. Because GPB will be reset when reset servo module. Fix Trap will close when Auto-Upgrade. {aploader.c}
    - Fix PSCAN mode video shake problem. {disp.c}

. Enhance Items
    - Add code to support cross platforms.
    - Add KEY_ENTER to pop menu for Navigator. Fix setup menu turn off SPDIF, but still out put problem 
    - Always enable S/W expand GPIO if necessary, ie. skip H/W expand GPIO procedure { [PIO.H], [WINAV.H] }
    - Add IOMute Active Low procedure. Default is disabled { [HSYSTEM.C] }
    - Add code to support JPEG 16M. {JPEGDEC.C}
    - Revise clean decoding buffer and coefficient buffer method. {JPEGDEC.C}
    - Support DVB Instant-Replay function. {parser code}
    - Support DVB Instant-Replay buffer ratio functions. {parser code}
    - Remove the structure member alignment issue. {parser code}
    - Update the function support (DivX) according to new bonding setting. {parser code}
    - Support new part number checking mechanism. {HDECODER.C/ HDECODER.H}
    - Correct the meaning of Memory protection register field. {Ctkav_mcu.h}
    - Add part no protection. {Decore.c}
    - Support 32M sollution in real 32M DRAM.

. Audio code
    - Modify karraoke buffer 8k when 64M solution.
. Servo code
    [BM]
    - mdify the refilled threshold rules:
        . DRAM Block=87 ->refiied threshold =8
        . DRAM Block=6,  -> refilled threshold=4
        . DRAM Block=3   -> refilled threshold=2
    - Modify the SRVTB_CheckSctID for improve the performance(sometimes miss the SRV_OS_INT2BM_SECTOR_ID_READY) 
    - Servo can not open tray when close tray status is SERVO_FAIL(fixed)
    - Define IGNORE_READLEADIN(srvcom.h, default is disable) for some dvd discs always leadin failed
    - Solved the conflict between I2C and ScanKey
 
    [Servo]
     - Add new platforms:DM520_X_HTC_1400 and FUS_DV34_FUS_FOP5000 platform
    - New SRVST_TestServoState function(#define RS232F_SEEK_TUNING)
    - Initialize global variable gbRF_RFAtt & gbRF_RFAtt1 when update RF Amp. ATT level.
      (SRVSV_AutoRFATT() of srvservo.c, SRVSV_InitRFIC of sc_cdv65.c)
    - Add focusing and tracking auto gain caliration by AGC and default is disable.(//#define SHWU_AUTO_GAIN_HJ)    
    - Initialize KOLEV_WIN as 0x58 (unit is 1/88k => 0x58*(1/88k)= 1 ms). (srvservo.c)
       For kolev kick period protection, please set suitable period for your jump 1 and >=2 track.
    - Add over flow and under flow protection for boost calibration in SRVST_ErrorRateTuningState.
      (SRVCAL_AdjustFactor() of srvcal.c) 
 
    [ucode] 
    - Release U950529_V13 version.
    - Add AGC function for focusing and tracking auto gain calibration
    - Add kolev window for open kick protection when meet defect.
    - For runout detection, change the criteria as below:
      if (remainder track < 4), judge as runout disc. 
    - Add Jitter error collection.   

. Flash code Version


/**************************************************************************
Version :    2.17a
Date    :    2006.5.17 / servo 2.17a(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 05162006
. JPG Decoder Code Date
    - 05112006
. Display Code Date
    - 05112006
. DivX Code Date
    - 05162006
. Navigate Code Date
    - 04112006
. Parser Code Date
    - 05112006
. Usb Code Date
    - 02092006
. Info Code Date
    - 05122006
. Audio Code Date
    - 05162006
. Servo Code version and date
    - 05172006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix DivX 3.11 can't play problem. {utl.c}
    - Fix some AVI files with sub title play hang problem. {parser code}
    - Fix some AVI files AV sync abnormal problem. {parser code}
    - Enlarge the DRAM for text to enhance the code overflow issue.

. Enhance Items

. Audio code
    - Mp3 upsample function was implemented by linear interpolation instead of repeat.
    - AC-3 SPDIF-RAW -> SPDIF-PCM -> next song will cause the SPDIF-PCM no sound sometimes.
. Servo code
    [BM]
    - Delete the #define DRAM_CONFIGURATION_TYPE, servo.a can merge in DRAM_32,DRAM_64,and DRAM_16 system
    - Create new CMD_CONFIGURE_EDC_ARRAY command for MPEG return the EDC buffer start address
    [Servo]
    - Add arima platform system 
    - Support new bounding chip (cahnge GPB[1] to GPB[7] SMB). Need to program GPB[1] and GPB[7] simultaneously.
    - Not reference the DECODER_SYSTEM, servo will reference the SERVO_SYSTEM to extend physical I/O,solution….
      Now, DECODER_SYSTEM is the same as SERVO_SYTEM (servocom.h)

/**************************************************************************
Version :    2.17
Date    :    2006.5.12 / servo 2.17(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 05132006
. JPG Decoder Code Date
    - 05112006
. Display Code Date
    - 05112006
. DivX Code Date
    - 05112006
. Navigate Code Date
    - 04112006
. Parser Code Date
    - 05112006
. Usb Code Date
    - 02092006
. Info Code Date
    - 05122006
. Audio Code Date
    - 05122006
. Servo Code version and date
    - 05122006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix the bug #1106: DVD mode打開Navigator,再直接切換至USB mode,且欲播放檔案=>結果會出現Navigator的殘影.	 {osd.c}
    - Fix the bug #933: 讀碟後->Normal Play(任一章節)->按"Repeat A_B"->按"Pause"後->再按一次"Pause"取消=>播放時,應該要恢復"Repeat A_B"的字體. {osd.c}
    - Fix the bug #1324: 在TV mode的OSD尚未消失前再一次切換TV mode,結果TV mode的OSD與Info顯示不同步.  {osd.c}
    - Fix the bug #1298: "KEY_GOTOTIME => KEY_MENU 使出現的Menu畫面有問題。 " {osdmm.c}
    - Fix the #1337 bug 任一Silde show模式,當正在換下一張圖時,即按Setup鍵,結果畫面會失真   {setup.c}
    - Fix top/bottom field exchange problem when start line is odd.
    - Fix CVD highlight disappear problem.
    - Fix mantis #1131 “按"Stop"鍵->切"PAL mode"->按"Resume"=>在"PAL mode"播放,2種字幕不能同時播放“
    - Fix mantis #1284 “在有字幕的畫面下Slow1/2約4.5秒->Play,回到畫面,字幕會停留在畫面中“
    - Fix mantis #1397 “進到MENU選單內,放置約1分鐘不去動作,Highlight會消失”
    - Fix mantis #1404 “Noraml Play影片,在有字幕的畫面下,按Next or Prev,字幕會未清乾淨”
    - Support adjusting video aspect ration 16:9 to LB or PAN SCAN for DVB
    - Fix when triggering in screensaver mode, the DVD navigation auto-play can't exit screensaver mode 
    - Fix PARSER hang when DVB signal is not good. {par_dram.c}
    - Fix top/open won't work when setup mode. {cc.c/media.c}
    - Fix sometimes FAT32 disk auto upgrade failed problem. {fat.c}
    - Fix some UPG909.AP size auto upgrade failed problem. {infofilter.c}
    - Fix AVI ->JPG effect mode, sometimes happen remain image problem. {mmanager.c}
    - Fix continue next in AVI with char sub-title, then always parse 1st char sub-title problem. {mmanager.c}
    - Fix menu will wrong when filter out files in not 1st pages. {mmanager.c}
    - Fix MP3 will keep in scan mode when press PLAY button in MP3 program + scan mode. {mmanager.c}
    - Fix UDF parsing too long problem. {udf.c}
    - Fix JPU’s bug that horizontal scaling cause UV shifting

. Enhance Items
    - Support internal/ external Audio DAC can exist together { [WINAV.H], [HADAC], [HDECODER] }
    - Collect frame buffer setting procedure together { [UTL], [CDG], [HALJPEG], [INITIAL] }
    - Porting Aron's code to support video aspect ratio setting @ DVB mode { [HDECODER] }
    - Enable AAC audio related setting { [UTL], [HAL] }
    - Always enable serial flash/ EEPROM R/W procedure { [HDEVICE], [HAL] }
    - CELL module can cross different DRAM configuration { [CELL], [INITIAL] }
    - Support Philips RC5 coding by Software IR
    - Support 16M UI. {osd.c/osddsply.c/osddsply.h/osdnd.c/osdnd.h/thumb.c/thumb.h/osddlg.c/osddlg.h/osdmm.c/osdmm.h}
    - Support mini OSD region (for 16M DRAM case) {GDI.C, GDI.H, Colordef.h)
    - Support new define for language studying in Navigator. {osddsply.c/osddsply.h}
    - Support 16M thumbnail. {thumb.c}
    - Support PAUSE + PLAY & reading external subtitle in multiple parts. {parser.c/ par_sptx.h/ par_sptx.c}
    - Arrange critical functions for 16M solution.
    - Add config code to dupport independent key to open language study mode. {cc.c/cckey.c/input.h/mmanager.c/osdnd.c/chips.c/linear.c/osddsply.c/thumb.c/utl.c}
    - Add code to support JPG single buffer mode. {internal.h/mmanager.c/haljpeg.c/thumb.c}
    - Enable AVI index table parsing in USB mode. {mmanager.c}
    - Support BFR/FBR Decode/Display Mode. {Mpeg2dec.c/ ctkav_vdec.h/ Disp.c/ disp.h/ Decore.c/ getbits.h/ mp4_dpart.c/ mp4_dpart.h/ mp4_header.c/ mp4_header_311.c/ mp4_picture.c/ mp4_picture_311.c/ mp4_recon_gmc.c/ mp4_tables.c/ mp4_vars.h/ Comdec.c/ Comdec.h}
    - Auto-tune the OSD mode when bitstream is different (NTSC/PAL mode.) and TV mode is Auto. {osd.c}
    - Set JPEG relative buffer for removing DRAM configuration in JPEG decoder. {haljpeg.c}
    - Add code to support JPEG 16M
    - Support CT675
    - For 16M ,turn off interrupt when write/read/erase  flash

. Audio code
    - pcmbuf/spdifbuf/upk buffer write protection by audio function
    - mp3 repeat h/w bug fixed by software repeat function
    - A/V sync. threshold value updated to scale up 16 times. 
. Servo code
    - Enhance servo bit extraction stabilization.
    - Support function SRVIF_ParserGetEDCInfo() & SRVIF_ParserUpdateSERVO()
    - Remove the limitation in SERVO alignment 0xa00(Max. DVD ofsset can't over 0x1FFF*4),SERVO DRAM address 不可超過 
    (0x40000000+0xFFF*0xa000)


/**************************************************************************
Version :    2.16b
Date    :    2006.4.14 / servo 2.16b(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 04062006
. JPG Decoder Code Date
    - 04072006
. Display Code Date
    - 04062006
. DivX Code Date
    - 04062006
. Navigate Code Date
    - 04112006
. Parser Code Date
    - 04142006
. Usb Code Date
    - 02092006
. Info Code Date
    - 04112006
. Audio Code Date
    - 04112006
. Servo Code version and date
    - 04122006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix DivX 6.0 play back abnormal problem.

. Enhance Items
    - Support new AP architectue, so the auto upgrade don't support between old version.
    - Add code to protect audio disappearrted problem.
    - Add DRM enhancement.
    - Add cross platform code.
    - Add macrovision code.

. Audio code
    
. Servo code
    - system hang for layer 0 jump to layer 1 (jump to error position of layer 1) [fixed]
    - Support turn off 901A in power down mode (#define TURN_OFF_RFIC_POWER_MANAGEMENT)
    - Initial Hi for GPB[10] & GPB[11] [fixed]

/**************************************************************************
Version :    2.16a
Date    :    2006.4.12 / servo 0412(909R)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 04062006
. JPG Decoder Code Date
    - 04072006
. Display Code Date
    - 04062006
. DivX Code Date
    - 04062006
. Navigate Code Date
    - 04112006
. Parser Code Date
    - 04062006
. Usb Code Date
    - 02092006
. Info Code Date
    - 04112006
. Audio Code Date
    - 04112006
. Servo Code version and date
    - 04122006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix USB-AVI hang bug.
    - Fix CGMS setting error in 625i.
    - Fix 909R release mode can't boot successfuly problem.

. Enhance Items
    - Support more SPI flash type.

. Audio code
    
. Servo code
    - Support anti-shock( max 0x58 ECC blocks) function
    - Quick open -> close -> open tray, but  disc not stop. [fixed]
    - Modify CD & DVD short seek parameters. (Mo_cdv65.h, sv_cdv65.c, pm_cdv65.h)
    - Modify CD long seek profile. (Mo_cdv65.h)
    - Update correct EQ_DVCD4X table. (pm_cdv65.h)
    - Disable integral control of spindle servo. (pm_cdv65.h)
    - Enable sigma-delta function correctly. (sv_cdv65.c)
    - Release U950411_V12 version.
    - Implement FE DC hold in long jump command.

/**************************************************************************
Version :    2.16
Date    :    2006.4.11 / servo 0406(909S)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 04062006
. JPG Decoder Code Date
    - 04072006
. Display Code Date
    - 04062006
. DivX Code Date
    - 04062006
. Navigate Code Date
    - 04112006
. Parser Code Date
    - 04062006
. Usb Code Date
    - 02092006
. Info Code Date
    - 04112006
. Audio Code Date
    - 04112006
. Servo Code version and date
    - 04062006
    
----------------------------------------------------------------------------
. Fixed Problem
    - Fix the bug about zoom rectangle will be cleared when calling HAL_AdjustTVMode() in DVB mode. {hal.c}
    - Fix the bug about displaying "Close" after user choose "DVB" mode in "Media Select" dialog box. {initial.c}
    - Fix bug: "file total time is incorrect if the AVI file don't do index table parsing" { [UTL] }
    - Fix DTS audio switch fail issue { [INITIAL] }
    - Fix tray can't auto close while tray open + H/W power on @ 909R system { [HSYSTEM] }
    - Fix clear coeff and decoding buffer functions for 32M. {jpegdec.c/ jpegdec.h/ jpegfmt.c/ jpegfmt.h}
    - Fix highlight shift problem. {Subpict.c / subpict.h}
    - Fix JPU's defect of writing first 4-byte when FR_OP != 0 or 7. {jpegdec.c/ jpegdec.h/ jpegfmt.c/ jpegfmt.h}
    - Fix Video hang sometimes when DivX3.11 -> DivX 4 or 5. {decore.c}
    - Fix mantis bug #1321: must press twice KEY_POWER can wake-up the system. { [HSYSTEM], [INTERNAL.H] }
    - Fix the bug about KEY_ENTER can't press button. {media.c}
    - Fix the bug about DVB normal bottom message will be cleared by DVD. {osdnd.c}

. Enhance Items
    - Add recovery relative code for STB. {osd.c/osd.h}
    - Enhance audio track bit-rate determination method. {par_avi.h / par_avi.c}
    - Modify AVI index table entry structure. {lparser.h / parser.h / parser.c}
    - Read subtitle data through BIU directly. {par_sptx.h / par_sptx.c}
    - Support S/W expand GPIO procedure for 908S_EVAL/ 908R_EVAL system { [PIO.H], [ErrChk.h], [HIO], [HSYSTEM] }
    - Support stack overflow checking mechanism { [INITIAL] }
    - Porting Micky's code to our code can cross platform easily { [HADAC], [HDEVICE], [HSYSTEM], [INITIAL] }
    - Porting CoCo's code: don't show "close" message while STB H/W power on w/ tray close { [INITIAL] }
    - Porting CoCo's: not do zoom status adjustment within HAL_AdjustTVMode() for STB mode { [HAL] }
    - Use JPU API to scaling the video for digest. { [DIGEST] }
    - Support a function to draw the scaling font. { [GDI] }
    - Support UDF file system in CT909R platform. {infofilter/mmanager/udf}
    - Support SPI bottom mode.
    - Enhance mp4 data partition. {mp4_dpart.c}
    - Add stack detection & report its size. #define STACK_OVERFLOW_DETECTION to turn on it.
    - Add some pin define for STB219_YHI/ HCN system solution.
    - Add IR type "YHI_IR" for YHI IR for CT219. {ir.h}
    - Support auto-upgrade from Serial Debug Tool for CT909S/CT909R/CT219. {debug.c/debug/h}

. Audio code
    - REF_PLAT_SYSTEM_CONFIGURATION (0x8000031c) bit[30:29] was setting by PROC1 to config spdifout mode,
        PROC2 comment all the setting about this.
    - The mute control setting : when playing from DTS spdifraw -> AC-3, always mute --> solved by mute control setting in audio code.
    
. Servo code
    - Enhance CD-G playability
    - Support DVD Multi-session (#define SUPPORT_DVD_MULTISESSION)
    - Solve the decode halt for CD-ROM
    - support CD ID_JUMP detection


/**************************************************************************
Version :    2.15a
Date    :    2006.3.30 / servo 0324(909S)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 02202006
. JPG Decoder Code Date
    - 03142005
. Display Code Date
    - 03062005
. DivX Code Date
    - 03062006
. Navigate Code Date
    - 12292005
. Parser Code Date
    - 03152005
. Usb Code Date
    - 02092006
. Info Code Date
    - 03142006
. Audio Code Date
    - 03062005
. Servo Code version and date
    - 03142006
----------------------------------------------------------------------------
. Fixed Problem
    - Fix Europe string wrong problem.

. Enhance Items
    - Support CGMS.
    - Fix CDG color abnormal problem.

/**************************************************************************
Version :    2.15
Date    :    2006.3.24 / servo 0324(909S)
Chip    :    CT909R
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 02202006
. JPG Decoder Code Date
    - 03142005
. Display Code Date
    - 03062005
. DivX Code Date
    - 03062006
. Navigate Code Date
    - 12292005
. Parser Code Date
    - 03152005
. Usb Code Date
    - 02092006
. Info Code Date
    - 03142006
. Audio Code Date
    - 03062005
. Servo Code version and date
    - 03142006
----------------------------------------------------------------------------
. Fixed Problem
    - Enter main menu's processkey when DVD mode for CT219.
    - Fix 909R_EVAL system platform servo and expand GPIO latch D0 shared same port issue { [HIO], [PIO] }
    - Fix NIM reset pin and Expand GPIO latch D1 shared same port issue { [HIO], [PIO] }
    - Fix AVI A/V asynchronous. {usbsrc.c}
    - FixMPG OSD time sometime freeze. {usbsrc.c}
    - Fix "serial debug can't work while power down mode" { [HSYSTEM] }

. Enhance Items
    - The path is ok for PSNR testing { [HSYSTEM], [DECORE] }
    - Add watch dog supporting. {hsystem.c/hsys.h}
    - Support 3 audio bin code. {hdecodr.c}
    - Support LOGO from flash directory { [UTL], [WINAV.H], [ROMCFG.TXT], [MAKEFILE] }
    - Support no-pip digest. {digest.c}
    - Add new command line option for making dsu_boot_audio.rom & dvd909.rom (debug version). {boot code}
    - Support progressive JPEG for 32M. {JPG code}
    - Support CDG playback

/**************************************************************************
Version :    2.15p
Date    :    2006.3.20 / servo 0320(909S)
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 02202006
. JPG Decoder Code Date
    - 03142005
. Display Code Date
    - 03062005
. DivX Code Date
    - 03062006
. Navigate Code Date
    - 12292005
. Parser Code Date
    - 03152005
. Usb Code Date
    - 02092006
. Info Code Date
    - 03142006
. Audio Code Date
    - 03062005
. Servo Code version and date
    - 03142006
----------------------------------------------------------------------------
. Fixed Problem

. Enhance Items
    - Support CT909R new system board config and playback.

/**************************************************************************
Version :    2.14
Date    :    2006.3.15 / servo 0314(909S)
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 02202006
. JPG Decoder Code Date
    - 03142005
. Display Code Date
    - 03062005
. DivX Code Date
    - 03062006
. Navigate Code Date
    - 12292005
. Parser Code Date
    - 03152005
. Usb Code Date
    - 02092006
. Info Code Date
    - 03142006
. Audio Code Date
    - 03062005
. Servo Code version and date
    - 03142006
----------------------------------------------------------------------------
. Fixed Problem
    - Fixed buffer loop counter issue. { [PARSER] }
    - Fixed system dead when play some DVD-Video. { [PARSER] }
    - Fixed bob noise when play some DVD-Video. { [PARSER] }
    - Initial default audio bit-rate to CBR. { [PARSER] }
    - Enhance _PARSER_UpdateSERVO( DWORD dwStreamIdx ) for 32M solution block alignment issue. { [PARSER] }

. Enhance Items
    - Support CT909S 32M solution.
    - Support STB code. 
    - Support the string move to one specified address and EEPROM allocation for CT219

/**************************************************************************
Version :    2.01
Date    :    2006.1.4 / servo 2.01(909S)
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 01032006
. JPG Decoder Code Date
    - 12292005
. Display Code Date
    - 12302005
. DivX Code Date
    - 01032006
. Navigate Code Date
    - 12292005
. Parser Code Date
    - 12272005
. Usb Code Date
    - 12282005
. Info Code Date
    - 01032006
. Audio Code Date
    - 12292005
. Servo Code version and date
    - 01032005 / servo 1.40(909A)/2.01(909S)
----------------------------------------------------------------------------
. Fixed Problem
    - Some adjustment for CT909S. {Mpeg2dec.c/ Decore.c/ Disp.c}
    - Some adjustment for CT909S. {gdi.c / ctkav_gpu.h}

. Enhance Items
    - Support different DRAM config in function INFOFILTER_DumpSector. {infofilter.c}
    - Modify serov/ audio/ video buffer setting for CT909S 64M & 32M solution { [APLoader], [HAL], [INITIAL], [UTL], [INFOFILTER] }
    - Extend HAL_ReadGPIO()/ HAL_WriteGPIO() w/ use can specify the GPIO group { [HAL], [TVE], [MEDIA], [VFD_CTRL] }
    - Support GPB control procedure for CT909S
    - Adjust DRAM configuration for FBR mode. {ctkav.h /ctkav_vdec.h}
    - For the CT909S, need to set USB clock select to UPLL. {usb.c}
    - Update the value for REG_TVE_GAIN_YC for 909S. {tve.c}
    - Update all the function of I2C functions to support GPIO group and PIN number setting. {tve.c}
    - Update all the macros of CS, SCK and SDA of VFD to set GPIO group and PIN number. {vfd_ctrl.h}
    
/**************************************************************************
Version :    2.00
Date    :    2005.12.30 / servo 2.00(909S)
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 12122005
. JPG Decoder Code Date
    - 12292005
. Display Code Date
    - 12122005
. DivX Code Date
    - 12122005
. Navigate Code Date
    - 12292005
. Parser Code Date
    - 12272005
. Usb Code Date
    - 11282005
. Info Code Date
    - 12302005
. Audio Code Date
    - 12292005
. Servo Code version and date
    - 12202005 / servo 1.40(909A)/0.90(909S)
----------------------------------------------------------------------------
. Fixed Problem
    - Fixed: HEIWID_SRC and HEIWID_DST registers should use actual width & height for U or V part. {jpegdec.c/ jpegfmt.c/ ctkav_jpu.h}
    - Fix the calling of APPacker.exe. {Working.boot/ Makefile}
    - Fix preview window content is not clear while JPEG preview --> MP3 { [HAL] }
    - Fix auto-upgrade fail from USB/ Servo { [INFOFILTER] }
    - Fix one DivX disc with no sub-title but will report having sub-title problem. {cdinfo.c}
    - Fix enter media select UI in repeat AB mode, then exit media select UI, then repeat AB OSD disappearred problem. {media.c}
    - Fix the background of progress bar disappear after entering media select UI and exit media select UI. {media.c}
    - Fix JPG picture will be scramble when enter and exit media select UI during JPG decoding. {media.c}
    - Fix enter setup during USB disk loading, then the loading OSD will destroy the setup UI. {media.c}
    - Fix enter media select UI during JPG preview, then preview picture will show even media select UI is enabled. But background should be no video when media select UI is enabled in MM mode. {mmanager.c}
    - Fix when press KEY_FUNCTION quickly when USB recognizing, then sometimes happen media select UI destroy the MM UI problem. {mmanager.c}
    - Fix play MP3 and prestop, then move cursor to last dir's JPG file to preview, then move back to original MP3 to play. Then it will play the first file in previous dir. {mmanager.c}
    - Fix when filter out all files in ROOT dir and then enter ROOT dir, it will show wrong file name in MM UI. {mmanager.c}
    - Restore last AP mode as DVD if can't find the AP from flash while DVD booting. {INITIAL.C}
    - Fix compiler error for HAL_DetectChipVersion() @ CT909S. {hal.c}
    - Restore last AP mode as DVD if AP_Loader() fail, and prepare to rebooting. {aploader.c}

. Enhance Items
    - Adjust HAL_ClearScreen() to support CT909S. {hal.c}
    - Add support for multiple audio decoders (integrate by Kevin's updates). {address.txt, address_rel.txt, Working.boot/ boot.c, extend909.sh, extend909.exe}
    - Support AP mode switching can work while non stop mode { [APLoader] }
    - Add protection code to disallow auto-upgrade (include AP) w/ mis-match chip version { [APLoader] }
    - Generate two kinds of font tables for 2 bits mode and 1 bit mode. {FontIndx_2bit.def / FontIndx_2bit.txt / FontIndx_1bit.def / Fontable_1bit.txt}
    - Support TEST909S. {gdi.c}
    - Support 1-bit / 2-bit font, load font from flash. {gdi.c}
    - Reduce USB DRAM buffer to 0.5K DW. {usb.c}

. Audio code
    - Support 909S audio code and separate into 5 BIN files.

/**************************************************************************
Version :    1.50
Date    :    2005.12.23 / servo 1.40(909A)/0.90(909S)
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 12122005
. JPG Decoder Code Date
    - 12222005
. Display Code Date
    - 12122005
. DivX Code Date
    - 12122005
. Navigate Code Date
    - 12202005
. Parser Code Date
    - 12072005
. Usb Code Date
    - 11232005
. Info Code Date
    - 12202005
. Audio Code Date
    - 12202005
. Servo Code version and date
    - 12202005 / servo 1.40(909A)/0.90(909S)
----------------------------------------------------------------------------
. Fixed Problem
    - Add "DONT_DO_COMMAND_IF_DATA_ERROR" for performance choice. If the define is enabled, will skip the error data from servo's report. If the define is disabled, will still do the command when servo find some data is error. {cdg.c/cdg.h}
    - Fix bug (subpict.c) : Mantis#0001020 “一進畫面就未有字幕出現”. {subpict.c}
    - Fix the bug #1106 "DVD mode打開Navigator,再直接切換至USB mode,且欲播放檔案=>結果會出現Navigator的殘影 " {media.c}
    - Fix the bug #1129: "Normal Play(OSD全開時)->按"Video"鍵,切"YUV mode"->按"Pscan"鍵->再按"Video"鍵(按2次),會切回S-Video mode=>此時,OSD會錯亂". {cckey.c}
    - Fix 0001044: 按ZOOM,畫面會抖動及有小頓: Display order: T1 T1' T2 T2' .... ; T2 will overwrite T1. {mpegdec.c}
    - Fix CT909 ROM code bug. Always force negative number to positive for trb / trbi. {mp4_header.c}
    - fix mantis #1125 by adding protection that don't send MSG_TITLE/ MSG_CHAPTER when playing the same TT/CC. {DVDINFO.C/ NAVIGATE.C/ NAVIGATE.H/ PGC.C}
    - Fix the bug #1106 "DVD mode打開Navigator,再直接切換至USB mode,且欲播放檔案=>結果會出現Navigator的殘影 " {media.c}
    - Fix the bug #1129: "Normal Play(OSD全開時)->按"Video"鍵,切"YUV mode"->按"Pscan"鍵->再按"Video"鍵(按2次),會切回S-Video mode=>此時,OSD會錯亂". {cckey.c}
    - Fix Title "the Bone collector" It always show parental error after doing Power-ON resume. {initial.c}

. Enhance Items
    - Support new subtitle formats PowerDivX and TMPlayer. {PAR_SPTX.C / WINAV.H}
    - Support ISO 14496-12 and Nero Digital
    - Add code to support 909S. {all code}
    - Add one more European character for the “Card Reader” string. {Fontindx.def / Fontable.txt}
    - Specify different bitstream formats, Nero digital or AVI, base on file type. {Char_spict.c}
    - Enhance "Long time to find the next correct NV while CELL's 1st NV fail  { [CELL] }
    - Support multiple AP { [INITIAL], [CC], [INPUT], [SETUP] }
    - Integrate Nero Digital/ AVI playback control procedure { [MMANAGER], [UTL], [CHAR_SUBPICT], [LINEAR], [CTKAV], [Parser related code] }
    - Change the position and size password-changed dialog to fix the “wrong password” clipped problem. {Setup.c / osd3.c / osd3.h}
    - Add one protect dir to move infofilter relative code to it. {cc.c/setup.c}

. Audio code
    - Support 909S audio code.

/**************************************************************************
Version :    1.21d
Date    :    2005.12.30 / servo 1.11a
Chip    :    CT909
Name    :    CheerTek (J200 LLY)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 11302005
. JPG Decoder Code Date
    - 11292005
. Display Code Date
    - 11302005
. DivX Code Date
    - 11302005
. Navigate Code Date
    - 11292005
. Parser Code Date
    - 12162005
. Usb Code Date
    - 11232005
. Audio Code Date
    - 12162005
. Servo Code version and date
    - 12082005 / servo1.11a
----------------------------------------------------------------------------
. Enhance items
  - Support AVI 2.0 playback. {parser code}
  - Fix one DivX file with no sub-title play un-smooth problem. {cdinfo.c}
  - Clear the media select UI when entering MM UI to avoid media select UI destroy MM UI. {mmanager.c}
  - Don't show waiting message when USB recognizing & setup mode is enabled. {media.c}
  
/**************************************************************************
Version :    1.21c
Date    :    2005.12.27 / servo 1.11a
Chip    :    CT909
Name    :    CheerTek (J200 LLY)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 11302005
. JPG Decoder Code Date
    - 11292005
. Display Code Date
    - 11302005
. DivX Code Date
    - 11302005
. Navigate Code Date
    - 11292005
. Parser Code Date
    - 12162005
. Usb Code Date
    - 11232005
. Audio Code Date
    - 12162005
. Servo Code version and date
    - 12082005 / servo1.11a
----------------------------------------------------------------------------
. Enhance items
  - Add protention code to dis-allow auto-upgrade (include AP) w/ mis-match chip version (CT909 vs. 
  CT909S) { [Aploader.c], [Aploader.h] }
  
/**************************************************************************
Version :    1.21b
Date    :    2005.12.16 / servo 1.11a
Chip    :    CT909
Name    :    CheerTek (J200 LLY)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 11302005
. JPG Decoder Code Date
    - 11292005
. Display Code Date
    - 11302005
. DivX Code Date
    - 11302005
. Navigate Code Date
    - 11292005
. Parser Code Date
    - 12162005
. Usb Code Date
    - 11232005
. Audio Code Date
    - 12162005
. Servo Code version and date
    - 12082005 / servo1.11a
----------------------------------------------------------------------------
. Fixed Problem
  - SCR value maybe incorrect issue (one *.mpg can't playback within Mustek given CDROM title (content is SVCD karaoke)) {[parser code]}
  
. Audio code
	- 909M & 909W : can't playback WMA. (In fact, WMA can be playback by those two kinds of chip part no) {[Audio code]}
  - 909V: can playback prologic II. (In fact, Prologic II is not support by this kind of chip part no) {[Audio code]}  


/**************************************************************************
Version :    1.21a
Date    :    2005.12.09 / servo 1.11a
Chip    :    CT909
Name    :    CheerTek (J200 LLY)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 11302005
. JPG Decoder Code Date
    - 11292005
. Display Code Date
    - 11302005
. DivX Code Date
    - 11302005
. Navigate Code Date
    - 11292005
. Parser Code Date
    - 12052005
. Usb Code Date
    - 11232005
. Audio Code Date
    - 12092005
. Servo Code version and date
    - 12082005 / servo1.11a
----------------------------------------------------------------------------
. Fixed Problem
	- PROC2 reset fail trap issue { [INITIAL], [Audio code] }

. Enhance Items
	- Using macro to read back PROC2 PC { [HAL] }
	- Modify DivX+WAM playback end checking mechanism { [MMANAGER] }



/**************************************************************************
Version :    1.21
Date    :    2005.12.08 / servo 1.11a
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 11302005
. JPG Decoder Code Date
    - 11292005
. Display Code Date
    - 11302005
. DivX Code Date
    - 11302005
. Navigate Code Date
    - 11292005
. Parser Code Date
    - 12052005
. Usb Code Date
    - 11232005
. Audio Code Date
    - 12072005
. Servo Code version and date
    - 12082005 / servo1.11a
----------------------------------------------------------------------------
. Fixed Problem
    - Fix mantis #1048: OSD time display info incorrect while SCAN --> PLAY (AVI only) { [CC], [UTL] }
    - Fix mantis #998, DVD Title "Cinderella", play motion then jump to chapter 25, the video has chance(4/10) to be displayed broken. {pgc.c}
    - Fix mantis #1047: JPEG playback fail if playback motion JPEG first { [UTL] }
    - Fix the video is dirty problem when KEY_BOOKMARK is pressing. {Osdbmark.c}
    - Prevent to enter bookmark mode while playing CDG title. {Osdbmark.c}
    - Fix some global variable declarations to 'volatile' to avoid compiler optimization problem. {jpegfmt.h/ jpegfmt.c}
    - Fix for thumbnail error-handling. {jpegfmt.h/ jpegfmt.c}
    - Fix switching between USB and Card Reader, can't detect mass storage device. {usb.c}
    - Fix if port 1 has device connected, device on port 2 may have problem in USB communication. {usb.c}
    - Force open tray & enter power down when auto-upgrade. Fix auto-upgrade fail issue for COBY platform. {Aploader.c}
    - Fix Resume -> Play, Video will disappear. Mustek issue. {Decore.c}
    - Fix FAT12 can recognize well but can't playproblem. {fat.c}
    - Fix some file name will show wrong char sometimes problem. {fat.c}
    - Add code to recognize wrong media control to avoid system abnormal after recognized as wrong media. {media.c/usbsrc.c}
    - Fix USB disk hang when removing during disk recognizing. {media.c}
    - Fix the bug #1000: Normal Play(JPEG)->按"Digest"鍵,Play"Digest"畫面每一頁,播放約2~3張圖後->按"Next"鍵(大約按10次以上,就會發生1次)=>到Page15/27時,Progress(殘影)出現在畫面右下方,會蓋住2個Button字體.(機率:3/10) {osdnd.c}
    - Fix the bug about the color palette transform is wrong. {cdg.}
    - Fix the bug about auto-upgrade is failed from Serial Debug Tool. {cc.c}
    - Fix the bug #816: 至工具箱mode,執行audio切換語言enter後,螢幕又背景殘影 {osddsply.c/osddsply.h}
    - Add "Card Reader" item in the "Media Select" dialog box to support build-in card reader. {osddlg.c/osddlg.h}
    - Fix upgrade to 8M flash from Servo/ USB/ Card Reader fail if code size < 4M (flash.bin)
    
. Enhanced Items:  
    - Support the CP-1250 character set. {Char_subpict.c}
    - Open #define PREFIX_INLINE_FLAG, #define TABLE_LOOKUP_VLD to gain JPEG decoder performance. {jpegfmt.h/ jpegfmt.c}
    - Add supports for SOF1 marker (extended sequential, baseline). {jpegfmt.h/ jpegfmt.c}
    - Add build in card reader supporting code. {cc.c/customer.h/media.c/secfilter.c/usbsrc.c/usb.c}

. Servo Fixed Items:
    - Please reference readme_srv.txt.

. Audio Fixed Items:
    - Fix MPEG clear PCM buffer error and it will make AVI gototome failed.
    - MPEG layer 2 read sampling rate error
    - System booting abnormal issue (destroy trap table)

/**************************************************************************
Version :    1.20a
Date    :    2005.11.11 / servo 1.10b
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 11042005
. JPG Decoder Code Date
    - 11112005
. Display Code Date
    - 11042005
. DivX Code Date
    - 11042005
. Navigate Code Date
    - 10282005
. Parser Code Date
    - 11112005
. Audio Code Date
    - 11112005
. Servo Code version and date
    - 11112005 / servo1.10b
----------------------------------------------------------------------------
. Fixed Problem
    - Fix DVD gototime failed problem. {infofilter.c}
    - Fix DVD A/V play un-smooth after 304 mins. {infofilter.c}
    - Update code to not show some error message problem. {jpegdec.c/jpegfmt.c}
    - Update audio version to correct format. {setup.c}
    - Fix when switch from SERVO to USB during opening tray when disc not recognized OK, then can't hear MP3 audio. {media.c}
    - Fix repeat AB message won't disappear when KEY_STOP during AVI repeat AB mode. {mmanager.c}
    - Fix the problem about JPEG progress will not have background if preview JPEG==>(Progress is not completed in the MMUI)Press KEY_PLAY to slide show. {osdnd.c/mmanager.c}
    - Fix the bug #970: "Transparency background of progressive bar disappear when the progress come to 100%". {osd.c}
    - Color palette of progressive bar is abnormal after existing the DivX dialog box". {osddlh.h}
    - Fix CDDA L/R channel swap sometimes problem. {parser.c}
    
. Servo Fixed Items:
    - Please reference readme_srv.txt.

. Audio Fixed Items:
    - Fix MPEG clear PCM buffer error and it will make AVI gototome failed.
    - MPEG layer 2 read sampling rate error

/**************************************************************************
Version :    1.20
Date    :    2005.11.7 / servo 1.10a
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 11042005
. JPG Decoder Code Date
    - 11042005
. Display Code Date
    - 11042005
. DivX Code Date
    - 11042005
. Navigate Code Date
    - 10282005
. Parser Code Date
    - 11032005
. Audio Code Date
    - 11072005
. Servo Code version and date
    - 11042005 / servo1.10a
----------------------------------------------------------------------------
. Fixed Problem
    - Enlarge OSD for PAL mode. {osdnd.c/osdnd.h/osd.c/osd.h/Ctkav.h/Gdi.c/gdi.h}
    - Fix the bug about "YUV P-SCAN" is not cleared completely in MM UI. {osdmm.h}
    - Fix the bug about SMI file may calculate wrong PTS. {mm.c}
    - Fix the bug when divide SMI file into 2 parts and the first SPU will get a wrong PTS during 2nd parsing. {char_subpict.c}
    - Fix bug#928 loading VTS fail leads to get wrong nv command data. {Dvdinfo.c / navigate.c}
    - Constrain the repeat mode: disabling repeat mode when doing search. {Cc.c}
    - Fix bug reported by Mustek when enableing definition " SUPPORT_PLAY_CHAPTER_BY_NUMBER" Using num key to play chapter but stall in the same chapter. {cckey.c}
    - Fix #595 for side-effect issues. {jpegfmt.c/ jpegfmt.h}
    - Fix #867 for add checking codes of  byte-stuffing, for DNL marker and corrupted bytes. {jpegfmt.c/ jpegfmt.h}
    - Fix a error handling from UNFINISH to UNSUPPORT in _JPEGFMT_ReadMarker(). {jpegfmt.c/ jpegfmt.h}
    - Fix a loop starting value of _JPEGFMT_ExecProgressiveIDCT(). {jpegfmt.c/ jpegfmt.h}
    - Fix _JPEGFMT_ClearBuffBeforeDecode(). {jpegfmt.c/ jpegfmt.h}
    - Fix SPST display time wrong for MicroDVD file format { [LINEAR], [MMANAGER] [PAR_SPTX] }
    - Fix parsing SP file action can't be done within _MM_MotionTrigger() issue { [LINEAR], [MMANAGER] }
    - Fix Mantis #913: system forever hang while SCAN mode switching quickly { [INITIAL] }
    - Fix > 64M SDRAM access fail if using 128M SDRAM { [INITIAL] }
    - Fix Mantis #912: OSD show "0:00:00" sometime during SCAN mode switching { [UTL] }
    - Fix Mantis #913: playback from track beginning sometime during SCAN mode switching quickly { [UTL] }
    - Fix SPST switching error while only exist one SPST case (only happen @ non-DVD title) { [CCKEY] }
    - Fix DVD title detection error while enable #define NO_DVD { [ INFOFILTER] }
    - Fix SPST display time wrong for MicroDVD file format. {Linear.c}
    - Fix the UI recover problem. {osddlg.h/osddlg.c}
    - When entering Program mode in Pause status, pressing KEY_PLAY will bypass to the main flow instead of being processed by Program module. It will have the same behavior with the Program mode of MM module. {osdprgm.c}
    - Fix one card can't recognize problem. {fat.c}
    - Fix one USB disk don't show JPG date information problem. {fat.c}
    - Fix Picture CD will hang after recognize disc when PROM disable. {infofilter.c}
    - Fix KEY_KARAOKE will show strange OSD problem. {cc.c}
    - Don't allow KEY_RESUME when auto play end. {cc.c}
    - Fix sometimes SP2 flash when play SVCD/CVD disc and switch SP. {cckey.c}
    - Fix when switch between USB & DVD, it will continue to play about 0.5s then switch problem. {media.c}
    - Fix play MP3 -> prestop -> go JPG to preview -> go this MP3 and play, then JPG image don't clear problem. {mmanager.c}
    - Fix next/prev in program list mode, then the help information will disappear problem. {mmanager.c}
    - Fix pause OSD don’t disappear when return from MP3 pause mode. {mmanager.c}
    - Fix quickly next/prev sometimes MP4 OSD will disappear problem. {mmanager.c}
    - Enhance system sometimes hang in same loop after playing overnight scratched DVD disc. {monitor.c}
    - Enhance decoder hang in wait VFIFO problem after scratched DVD overnight testing. {monitor.c}
    - Fix play from effect JPG to MP3, then JPG omage don't clear problem. {osdmm.c}
    - The last FW modification for passed Dolby Certification. {MM.c / WINAV.h}
    - Avoid MP3 low bit rate file from proceeding to next file too early. {MM.c}
    - Fix Decoder trap a loop when bitstream error. {Mpeg2dec.c}
    - Fix the bug #783: 按到最後一張讓其自動播放,換到第一張播放時,OSD上右邊會有數字殘留. {osddsply.h}
    - Fix the bug #834: 在播放歌曲時,按音量大小(VOl+/-),此時左下方的資訊欄的字和VOl的音量條會重疊.機率7/10. {osdmm.c/osd.c/osd.h}
    - Fix the bug #816:執行audio切換語言enter後,螢幕又背景殘影. {osddsply.c}
    - Fix the bug #871:打開Navigator,Audio Icon會有綠色邊框. {NG_Angle.txt/NG_Audio.txt/NG_Chap.txt/NG_Rep.txt/NG_RepAB.txt/NG_Slow.txt/NG_Sub1.txt/NG_Sub2.txt/NG_Title.txt/palNG.txt.}
    - Fix the bug #838: 在瀏覽模式下,在圖片未解完的狀態下,連續按PRG,有幾張圖片的3/4處會出現黑色部分.機率8/10. {thumb.c}
    - Fix partial of AP10 report #896. {jpegdec.c/ jpegdec.h/ jpegfmt.c/ jpegfmt.h}
    - Fix SCR value incorrect for MPEG-2. {Par_mpeg.c/ par_mpeg.h}
    - Fix video H/W power-down. {Parser.c/ parser.h}
    - Update SP1/SP2 control flow for fixing highlight missed issue. {Parser.c/ parser.h}
    - Fix buffer detect error for CD+G title. {Parser.c/ parser.h}
    - Leave a infinite loop when Stop. {Decore.c/ mp4_header.c/ mp4_recon_gmc.c}
    - Fix SCF/SCB/KEY_TITLE bo noise in HFY platform. {cc.c/hal.c/utl.c}
    
. Enhanced Items:
    - Support auto-upgrade from Serial Debug Tool. {debug.c/debug.h}
    - Support SubViewer 1.0 and 2.0 file format. {par_sptx.h / par_sptx.c}
    - Support the UTF-8 encoded string. {char_subpict.c/char_subpict.h}
    - Support the new version format. {osd3.c/osd3.h/setup.c}
    - Support file searching in file record for GPS application. {mmanager.c}
    - Support ZIP2006 compression. {Aploader.c/ Aploader.h/Comdec.c/osdfont1.c/Makefile/ boot.c/ unzipnew.ld/Makefile}
    - Support PROC2 Debug message. {Debug.c/ Debug.h}
    - Add one display character in Servo Version. {Flash.bin}
    - Add code to support multiple slot card reader. {fat.c/media.c/Srcfilter.c/srcfilter.h/usbsrc.c/usbsrc.h/USB.C/USB.H}
    - Add multi-session support for DVD-ROM. {Servo.c}
    - Add package 909W to support DivX playing and WMA playing. {decore.c/parser.c/hal.c}
    - Remove "make 8m" in makefile and only support makefile in CheerDVD code. {makefile}
    - Protect proc1 code in RELEASE code. {address_rel.txt}

. Servo Fixed Items:
    - Please reference readme_srv.txt.

. Audio Fixed Items:
    - Fix MP3 decode error cause noise at song beginning.
    - Add debug message from proc2.
    - For AV sync, audio update PTS will calculate PCM buffer data. PTS -> PTS - (PCMREM/3/sampling_rate)*90000
    - Fix MPEG bug for SVCD can't play. Cause by sync word is not byte align 
    - Add package version "909W" control to support DivX+WMA.

. Left Issues:

/**************************************************************************
Version :    1.15
Date    :    2005.10.21 / servo 1.01a
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 09232005
. JPG Decoder Code Date
    - 09282005
. Display Code Date
    - 09232005
. DivX Code Date
    - 09232005
. Navigate Code Date
    - 10212005
. Parser Code Date
    - 09282005
. Audio Code Date
    - 10072005
. Servo Code version and date
    - 09292005 / servo1.01a
----------------------------------------------------------------------------
. Fixed Problem
    
. Enhanced Items
    - Reduce code size of CheerDVD about 3.72K in zipped size.

/**************************************************************************
Version :    1.11c
Date    :    2005.10.19 / servo 1.01a
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 09232005
. JPG Decoder Code Date
    - 09282005
. Display Code Date
    - 09232005
. DivX Code Date
    - 09232005
. Navigate Code Date
    - 10072005
. Parser Code Date
    - 09282005
. Audio Code Date
    - 10072005
. Servo Code version and date
    - 09292005 / servo1.01a
----------------------------------------------------------------------------
. Fixed Problem
    - Fix MP3 sample/bitrate information wrong issue. {hal.c}
    - Fix DVD scan to play bo-bo noise problem. {hal.c}
    - Fix part no wrong problem to make 909V can't play DivX files. {hal.c}
    - Fix some SUB char sub-title file can't display problem. {parsptx.c}
    - Fix open tray video black during playing marcovision DVD disc in YUV PSCAN mode. {tve.c}
    - Fix when move cursor to dir part when JPG decode not OK, then press numberkey to re-play JPG, then only show preview picture problem. {mmanager.c}
    
. Enhanced Items
    - Support SubViewer 1.0/2.0 char sub-title format. {parsptx.c / char_subpict.c / par_sptx.h}

/**************************************************************************
Version :    1.11b
Date    :    2005.10.11 / servo 1.01a
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 09232005
. JPG Decoder Code Date
    - 09282005
. Display Code Date
    - 09232005
. DivX Code Date
    - 09232005
. Navigate Code Date
    - 10072005
. Parser Code Date
    - 09282005
. Audio Code Date
    - 10072005
. Servo Code version and date
    - 09292005 / servo1.01a
----------------------------------------------------------------------------
. Fixed Problem
    - Fix some DVDROM char sub-title can't display problem. {infofilter.c}
. Enhanced Items
    -    

/**************************************************************************
Version :    1.11a
Date    :    2005.10.07 / servo 1.01a
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 09232005
. JPG Decoder Code Date
    - 09282005
. Display Code Date
    - 09232005
. DivX Code Date
    - 09232005
. Navigate Code Date
    - 10072005
. Parser Code Date
    - 09282005
. Audio Code Date
    - 10072005
. Servo Code version and date
    - 09292005 / servo1.01a
----------------------------------------------------------------------------
. Fixed Problem
    - Fix no disc mode -> press invalid key to show invalid OSD -> enter setup mode -> exit setup mode -> then no disc OSD disappearred. {osd.c}
    - Fix KEY_OPEN when JPG slide show effect mode transfer, then LOGO will be abnormal. {cc.c/hal.c}
. Enhanced Items
    - Support SRT/SSA/ASS/SMI/SUB MP4 char based sub title. {char_subpict.c/cdinfo.c/par_sptx.c/mmanager.c}


/**************************************************************************
Version :    1.11
Date    :    2005.09.29 / servo 1.01a
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 09232005
. JPG Decoder Code Date
    - 09282005
. Display Code Date
    - 09232005
. DivX Code Date
    - 09232005
. Navigate Code Date
    - 09232005
. Parser Code Date
    - 09282005
. Audio Code Date
    - 09282005
. Servo Code version and date
    - 09292005 / servo1.01a
----------------------------------------------------------------------------
. Fixed Problem
    - Fix Scan/ GoTime action fail after playback DivX w/ char-base SPST --> fully stop --> Play. { [LINEAR], [MMANAGER] }
    - Fix Repeat AB seems not work while playback DivX w/ repeat AB < 3 sec { [LINEAR] }
    - Disable "resume feature" while DivX pre-scan action not done or non-exist index table { [MMANAGER] }
    - Disable "Auto playback feature" while enable "SUPPORT_DIVX_CERTIFICATION" or "SUPPORT_PSNR_TEST" { [WINAV] }
    - Fixed 遊走三..在一開始的女王頭按數字鍵跳章節, 此時使用navagiator功能跳章節會無法跳章節 [CCKEY.C]
    - Fixed Bug#811 按SCB_2X時,SUBTITLE切換多國語言,螢幕有時會出現字幕. {PGC.C}
    - Fixed Bug#812 進到影片中,按隨機播放,在按NEXT,會跑到MENU(DVD選單中). {PGC.C}
    - Fixed #595: Repeated column of blocks at right edge of JPEG pictures. {jpegdec.c jpegdec.h jpegfmt.c jpegfmt.h}
    - Fixed #762 & #764: 解不出來當機. {jpegdec.c jpegdec.h jpegfmt.c jpegfmt.h}
    - Fixed #741 & #765: 圖片左上方會有白色雜訊. {jpegdec.c jpegdec.h jpegfmt.c jpegfmt.h}
    - Fixed #742 & #769:  圖片的右邊會有橫條線. {jpegdec.c jpegdec.h jpegfmt.c jpegfmt.h}
    - Fix open tray -> enter media select UI -> KEY_CLOSE -> then black block appear when entering MM UI. {cc.c}
    - Fix some USB disk with null attr can't recogize correctly problerm. {fat.c}
    - Fix when undefine SUPPORT_POWER_SAVING, the USB mode will be abnormal problem. {media.c}
    - Fix sometimes MP3 continue playing a while when removing USB disk. {media.c}
    - Fix Picture CD with MP3 in dir PICTURES behavior abnormal problem. {mmanager.c}
    - Fix KEY_PLAY + random mode behavior abnormal problem. {mmanager.c}
    - Fix thumbnail picture can't decode OK in some specific USB disk problem. {thumb.c}
    - Fix fade out may not be completed [CHIPS.c]
    - Fix Play -> Slow %75 -> Slow %50 => unsmooth audio for a while [CC.c]
    - Support the KEY_CLEAR to clear entry in Search mode. {osdsrch.c}
    - Fix bug #684: 瀏覽模式下，圖片尚未全部顯示，按random後黑頻且hang住。{thumb.c}	 
    - Fix the bug about the screen is black when press KEY_ENTER in Thumbnail mode. {FullLuck issue.} {thumb.c}
    - Fix bug #771: Mute mode,從Jpeg檔播回Mp3檔時,檔案資訊的無聲符號會消失. {osd.c}
    - Fix the bug about the "invalid" symbol is displayed in SETUP UI when no disc==>Press any invalid key. {osd.c}
    - Fix [CT909_AP10]0000793: 章節結束點(00:26:58),人物會晃動.
    - Fix[CT909_AP10]0000775: 連續快進/快退後,畫面hang住及無聲音輸出約3秒後,即可正常播放.
    - Fix[CT909_AP10]0000773: repeat AB(3秒內) ,畫面有時也並沒有真正repeat AB,原本設定repeat AB(27"-29")但有時畫面卻只有repeat AB(27"). Item2: 畫面右下角會出現藍色長方形block.
    - Fix[CT909_AP10]0000774: search time,畫面右下角會出現長方形block. 
    - Fix the bug #777: 在目錄夾中編輯歌曲及圖片,編輯好後在進入瀏覽模式中的編輯清單內,按MP3 Play後,會出現黑色圖片背景. {thumb.c}	 
    - Fix the video buffer remainder overflow (count audio into video) case. {initial.c (base on fish’s update)/ ctkav_platform.h/ ctkav.h}
    - Fix bug that some MPEG stream has packet size 0 case. {par_mpeg.c/ parser.c}
    - Fix bug #768: 在瀏覽模式下,在圖片未解完時,按PRG 5~6次,此時,第1張圖片會變成第2張圖片一樣的圖.機率:5/10. {thumb.c}
    - Fix the bug #796: (Menu畫面)Normal Play->(MP3 03_PBB,第1個檔案)->選(JPEG 00MorrisroeR,第7個檔案,JPEG 00jpl,第9個檔案,容量較大的檔案),畫面下方"百分比"還在跑時->按"Tltle"鍵=>會有"Digest"畫面出現時,會有1個黃色的框出現,一直在閃動不停,都是在固定位置出現,右上角OSD也是固定,出現Pagina:205/27. {mmanager.c}
    - Update the AVI video compressor FourCC code detection mechanism and support more reasonable FourCC codes of MPEG-4 streams. {parser.c/ par_avi.c/ par_avi.h}
    - Fix when entering the pre-stop mode, the playing time is not clear. {fontindx.def}
    - Fix #765: For 444 sampling format, the block counts in horizontal should be aligned to 2, or there will be white dots displayed before pac02.jpg. {jpegfmt.c }

. Enhanced Items:
    - Support CVD OGT { [UTL], [HAL], [CCKEY] }
    - Add feature: DVD Smart play by IR using KEY_AMP. {NAVIGATE.C DVDINFO.H DVDINFO.C PGC.C }
    - Enlarge the font size. {osddsply.c/sokobanui.c/osddg.c/osdmm.c/thumb.c}
    - Tune region for new fonts. {thumb.c/osdmm.c/osddg.c/osddlg.c/sokobanui.c/tetrisui.c/minerui.c}
    - Support ASS and SSA subtitle format parsing.

. Servo Fixed Items:
    - Please reference readme_srv.txt.

. Audio Fixed Items:
    - Fix language study has noise while switch to normal play
    - Turn off timer to get brtter performance

. Left Issues:

/**************************************************************************
Version :    1.10
Date    :    2005.09.06 / servo 1.00
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 08262005
. JPG Decoder Code Date
    - 09022005
. Display Code Date
    - 08262005
. DivX Code Date
    - 08262005
. Navigate Code Date
    - 09062005
. Parser Code Date
    - 09062005
. Audio Code Date
    - 09062005
. Servo Code version and date
    - 09062005 / servo1.00a
----------------------------------------------------------------------------
. Fixed Problem
    - Fix NV SLOW 75%->50%->75%(should be Play). {osddsply.c}
    - Fix effect "Push down" is updating screen content, it's not smooth (a little lag). {mmanager.c}
    - Fix SVCD freq wrong when language study  by disable it. {cckey.c/utl.c}
    - Fix SPDIF raw is on when language study. {utl.c}
    - Fix "progress bar still on MM UI while abort pre-scan AVI file action. {mmanager.c}
    - Fix the KEY_RETURN from motion mode to make MM UI display two stage problem. {mmanager.c} 
    - Fix the bug "TV Mode test -- Use KEY_DIGEST to switch thumbnail mode and then use KEY_TVMODE to switch from NTSC to PAL (before the thumbnail page is finished), the progess bar will be placed between row 3 & row 4, and it's not cleared when the thumbnail page is finished.". {thumb.c}
    - Fix progress bar won't go 100% in some AVI file problem. {mmanager.c}
    - Fix some USB can't recopnize correctly after second switch from DVD. {media.c/usbsrc.c/usb.c}
    - Fix AVI ID error problem. {parser code}
    
. Enhanced Items:
    - 
. Servo Fixed Items:
    - Please reference readme_srv.txt.
. Audio Fixed Items:
    - No audio output for a long time after DivX file multi-audio switching.
    - spdif raw can't be turned off when play

. Left Issues:

/**************************************************************************
Version :    1.10.1st
Date    :    2005.09.02 / servo 1.00
Chip    :    CT909
Name    :    CheerTek (J200 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 08262005
. JPG Decoder Code Date
    - 09022005
. Display Code Date
    - 08262005
. DivX Code Date
    - 08262005
. Navigate Code Date
    - 08262005
. Parser Code Date
    - 09022005
. Audio Code Date
    - 08262005
. Servo Code version and date
    - 08262005 / servo1.00
----------------------------------------------------------------------------
. Fixed Problem
    - Fix part No wrong due to error part No. table. {hal.c}
    - Fix bug#723 the display time will become 0 when doing NSML Angles change. {cell.c}
    - Fix NF-250,Title menu-> Angle Test-> select seamless or non-seamless angle change-> after notmla play to 11s-> SCBx2->it will hang when play to 00:10:02. {cell.c}
    - Fix code to prevent servo is always in stop state when exiting from screen saver. {navigate.c}
    - Fix bug #632 DVD 限制級保母 program 2-2 ; 3-2 ; 4-2 ; 27-1, It repeats to play the 3-2 by pre-command issue. {pgc.c}
    - Fix CT909_AP10 issues: #398, #446, #689, #407, 431. {jpegfmt.c/ jpegfmt.h/ jpegdec.c/ jpegdec.h}
    - Fix counting variable issue for decoding progress. {jpegfmt.c/ jpegfmt.h/ jpegdec.c/ jpegdec.h}
    - Fix audio disapearred sometimes from scan -> play mode. {cc.c}
    - Fix the mute icon state is wrong after playing JPG and press KEY_MUTE. {cc.c}
    - Fix KEY_PLAY operation starge from program edit mode + pause mode. {cckey.c}
    - Fix when pressing KEY_RESUME from program edit mode + no entries, then system won't do resume function problem. {cckey.c}
    - Fix one DivX disc will spend more time in recognizing disc. {cdinfo.c}
    - Fix JPG video will appear when pressing KEY_FUNCTION from JPG effect mode. So now don't disable video when JPG & all motion mode. {media.c}
    - Fix when almost play end and enter pause mode, then wait about 5 mins, then go back to play mode again. Then system will automatically go next problem. {mmanager.c}
    - Adjust the trigger end flow to play more motion. {mmanager.c}
    - Fix sometimes see JPG scramble picture in background after enter MM UI & preview mode. {mmanager.c} 
    - Disable KEY_NUMBER in program edit mode. {mmanager.c}
    - Fix when repeat folder and then enter program edit mode to program some entries, then go program play mode. Then still keep in repeat folder mode but it won't go repeat folder flow problem. {mmanager.c}
    - Fix will see video a while after entering program edit mode problem. {mmanager.c}
    - Fix PIP don't clear when ZOOM mode -> KEY_PROGRAM -> play JPG again. {mmanager.c}
    - Fix play from motion to audio file, then play JPG again. Then the OSD information is in-correct in JPG slide show mode problem. {mmanager.c}
    - Fix system hang in sequence end code problem. {monitor.c}
    - Force mpegdec to enter stop mode to fix system hang in mpegdec loop problem. {monitor.c}
    - Fix DivX can't play anymore when SERVO enter STOP mode problem. {utl.c}
    - Clear progeam mode after entering random mode to fix two mode conflict problem. {utl.c}
    - Fix 0000656: 按SCB後 在14:44會發生block. {Mpeg2dec.c}
    - Fix 0000716: Normal play畫面不順. {Mpeg2dec.c}
    - Fix DivX subpict display time error.(one by one). {Subpict.c}
    - Fix part No wrong due to error part No. table. {Decore.c}
    - Fix bug #107: CDG=>Normal Play會down機. {cdg.c}	 
    - Fix bug #448:Pause->Play(數次)=>會有字體殘影. {cdg.c}
    - Fix bug #475: 設定"Repeat all"->此時先跳到最後一個章節(Ch12)Normal Play=>畫面黑頻,死機, {cdg.c}
    - Fix bug #496: 1.畫面播放出現Block 2. 畫面播放時,會有上一頁的字體殘影存在{cdg.c}
    - Fix bug #396: 播放CDG時,Info.秒數會亂跳. {cdg.c}
    - Fix bug #486: 在播放音樂,將Highlight移到左邊,再按prog按2次,左下角的資訊欄就不會顯示出音樂的資訊(若在右邊音樂欄處按prog則還是會出現).機率10/10 (MP3 YHI MP3 TEST TITLE) {osdmm.c}
    - Fix bug #483: "編輯模式"字樣的OSD不會清除 {osdmm.c}
    - Fix bug #529: Program取消->"編輯模式"字體還存在 {osdmm.c}
    - Fix bug #684: 瀏覽模式下，圖片尚未全部顯示，按random後黑頻且hang住。{thumb.c}
    - Fix bug #682: 進入PRG，但不設定直接選播放，會黑頻、hang住。{thumb.c}
    - Fix bug #681: PRG的圖播完回瀏覽模式，再按PRG、播放。結果每次都不一樣，但都有一連串的問題 {thumb.c}
    - Fix bug #680: 瀏覽模式發生殘影 {thumb.c}
    - Fix bug #664: Digest->圖片沒有依續出現 {thumb.c}
    - Fix FULLUCK's problem: ?片正在播放中，e.x. play item == 17，按KEY_STOP?入thumbnail mode,?上按program?入program mode,?thumbnail??有show到item 17?按key_play去program，在item 17的那?位置就?先出?“P”的??。然后再按key_program取消program mode，??尚未show出的thumbnail?有空的外框，而且“P”的??不?消失" {thumb.c}
    - Fix bug #661: 螢幕保護符號出現後，按pause，結果zoom右下角的預覽圖消失，只剩白框	{osdss}
    - Support FULLUCK's request: 播放碟片按"PAUSE"?很久才能停下?(6分?以上) {osdss}
    - Fix the bug about the mute icon will be recovered when angle==>mute==>angle==>mute off. {osd.c}
    - Fix MPEG-4 file SLOW operation not work. {parser code}
    - Fix AVI playback may hang sometime if pressing KEY_NEXT/ KEY_PREV frequently { [MMANAGER] }
    - Fix the bug #735: PRG模式下播放，按2次stop，結果檔案資訊會show "03:00. {osdmm.c}
    - Fix one DivX file hang in video buffer full when scan -> play. {parser.c}
    
. Enhanced Items:
    - Tune performance: flatten wait_vdfifo() in _JPEGFMT_Decode & _JPEGFMT_DecodeSequential -- about 25% speedup. {jpegfmt.c/ jpegfmt.h/ jpegdec.c/ jpegdec.h}
    - Support language study. {UTL.c / CHIPS.c / OSDND.c / CC.h / CCKEY.c / WINAV.h /osddsply/bitmaps}
    - Support char base (srt) subtitle file for DivX file. {par_sptx.c / cdinfo.c / mmanager.c / Gdi.h / char_subpict.c / osd3.h / setup.h / setup.c}
    - Make system just go next when A/V is both un-playable format in DivX mode. {mmanager.c}
    - Make system throw correct A/V bits when some format is not supported to fix some file play abnormal problem. {utl.c}
    - Enhance the error concealment of MP1/2/4. {Mpeg2dec.c / Decore.c / Comdec.c}
    - Enhance PROC2 reset flow. {Initial.c/ Debug.c/ ctkav_platform.h/ debug.h}
    - Change the  DISP_OSDEffect function argument , remove H/V duplicate parameters. DISPLAY module will check the duplication by OSD output window. {Gdi.c}
    - Support OSD dynamic region for DivX subtitle. {osdnd/osd}
    - Support JPEG/DivX progress bar. {mmanager/osd/osdnd}
    - Add error concealment mechanism for AVI file playback { [MMANAGER], [Parser related code] }
    - Support PARSER_CMD_IGNORE_EDC to enable/disable EDC flag detection, default is enable. {parser code}
    - Support DVD block not start from sector 0x?????0. {parser code}

    
. Servo Fixed Items:
    - Please reference readme_srv.txt.
. Audio Fixed Items:
    - MP3 PCM output noise
    - AC3 overnight testing will be hang.
    - Increase A/V sync buffer for Dvix certification
    - LPCM will be no sound while play it directly.
    - MPEG audio 'bo' noise at beginning while MPEG audio type switch.

. Left Issues:

/**************************************************************************
Version :    1.08
Date    :    2005.08.01 / servo 0.98
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. MPEG1/2 Decoder Code Date
    - 07272005
. JPG Decoder Code Date
    - 07272005
. Display Code Date
    - 07272005
. DivX Code Date
    - 07272005
. Navigate Code Date
    - 07152005
. Parser Code Date
    - 07282005
. Audio Code Date
    - 07262005
. Servo Code version and date
    - 07282005 / servo.098
----------------------------------------------------------------------------
. Fixed Problem
    - Fix SERVO spindle stop in SETUP mode and press KEY_SETUP, then spindle don't spinning problem. {cc.c}
    - Fix some DivX can't play problem. {ctkav.h}
    - Fix video blocky in B point when repeat AB mode. {mpeg2dec.c}

. Enhanced Items:
    - Support parser drop ECC wrong block function for Error Concealment Enhancement. {parser code}
    - Remove some GMC code and DivX 311 code in 4M solution for code 4M code size issue. {DivX code}
    - Support Power Saving code. {Aploader.c/Customer.h/Hal.c/Initial.c/Media.c/Parser.c/Utl.c}
    - Support new boot code to reset proc2 when boot code is boot_dbg.bin or boot_dbgsw3.bin. {boot code}
    
. Servo Fixed Items:
    - Please reference readme_srv.txt.
. Audio Fixed Items:
    - Fix 8M code don't reference DTS part NO problem.

. Left Issues:

/**************************************************************************
Version :    1.07
Date    :    2005.07.21 / servo 0.97c
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 07152005
. Display Code Date
    - 07152005
. DivX Code Date
    - 07152005
. Navigate Code Date
    - 07152005
. Parser Code Date
    - 06242005
. Audio Code Date
    - 07212005
. Servo Code version and date
    - 07152005 / servo.097c
----------------------------------------------------------------------------
. Fixed Problem
    - Fix bug#0000458 DVD can't do random play. Port 908 DVD random play feature to 909. {cell.c / navigate.c / navigate.h / pgc.c}
    - Fix bug#0000333: Confirmed for the white line in the right edge of picture. {jpegfmt.c/ jpegfmt.h/ jpegdec.c/ jpegdec.h}
    - Fix bug#0000446: It may be caused by reading bits before VLD FIFO is ready. Tuned the calling of wait_vdfifo(). {jpegfmt.c/ jpegfmt.h/ jpegdec.c/ jpegdec.h}
    - Tuned the calling of wait_vdfifo(). Now decoding JPEG pictures from USB-disc is ok. {jpegfmt.c/ jpegfmt.h/ jpegdec.c/ jpegdec.h}
    - Fix bug #435: "The thumbnails will be zoom in if slide show with effect (before the effect is completed)." {thumb.c}
    - Fix bug #464: "瀏覽模式中，圖未出來，所有白框先出來". {thumb.c}
    - Fix bug #453: "Play->切換(NTSC/PAL/Auto)=>畫面異常,字體都亂掉,聲音正常播放. " {cdg.c}
    - Fix 4M nanigate.a can't work well in 8M RELEASE code problem. {cdinfo.c/infofilter.h}
    - Support MM file auto playing. {mmanager.c}
    - Make OSD show "loading" immediately when USB media plug in. {media.c/usbsrc.c}
    - Fix when play MM-motion file and play end, then press KEY_PLAY, then will go resume and will stop immediately problem. {mmanager.c}
    - Fix when press KEY_PLAY in MP3 scan + program edit mode, then scan mode will be cancel problem. {mmanager.c}
    - Fix sometimes thumbnail picture don’t show in USB mode problem. {Thumb.c}
    - Fix some JPG file can't play well in FAT32 file system problem. {usbsrc.c}
    - Adjust timing define to fix sometimes error concealment won't active problem. {winav.h}
    - fix fade out/fade in noise (need Proc2's update) {chips.c}
    - fix hw power up TFT TV System will not be set :  if(bNewTarget!=__bTVType) will not hold in HAL_AdjustTVMode. {hal.c}
    - fix next TFT abnormal display. {hal.c}
    - fix audio L/R swap except I2S alignment. {hal.c}
    - Fix 0000357: Video 鋸齒狀 within Zoom Mode PIP area for some DivX file. {Disp.c/Mpeg2dec.c}
    - Fix Clip C04:  artifacts on the wall as they are walking down the hallway near the beginning of the clip. {Disp.c/Mpeg2dec.c}
    - Fix Video is cut off on all clips.  It looks like it is zoomed in. {Disp.c/Mpeg2dec.c}
    - Avoid SOSEN W571C161 44K audio (CDDA..) always noise when HW power on. {initial.c}
    - Fix pass wrong value of APST type to HAL_TVSetAPS. {cell.c}
    - Update the "Style" strings for Europe language. {setupstr.h/osdstr.h/UI_Europe.xls}
    - Fix SVCD display time loses some seconds e.g. no 4:48 -> 4:48 -> 4:50. {utl.c}
    - disable MV when IR power off & open tray. {initial.c}

. Enhanced Items:
    - Support displaying "Title Menu"/"Root Menu" in one-line Display UI. {osddsply.c/osddsply.h}
    - Support FAT32 file system. {cdinfo.c/fat.c/infofilter.h/mmanager.c}
    - Support USB auto upgrade. {fat.c/infofilter.c}
    - Move SUPPORT_4M_FLASH define from customer.h to makefile. {makefile/makefile_rel}
    - Decrease 1s for CDROM recognization time when close tray. {media.c}
    
. Servo Fixed Items:
    - Please reference readme_srv.txt.
. Audio Fixed Items:
    - AC3 Surround sound is small
    - AC3 Dowm-mix sound is too small for 2 -ch.
    - Bo-Bo noise wihle audio reset.
    - WMA + keydown noise.

. Left Issues:

/**************************************************************************
Version :    1.06
Date    :    2005.06.28 / servo 0.96a
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 06272005
. Display Code Date
    - 06272005
. DivX Code Date
    - 06272005
. Navigate Code Date
    - 06242005
. Parser Code Date
    - 06242005
. Audio Code Date
    - 06242005
. Servo Code version and date
    - 06292005 / servo.096a
----------------------------------------------------------------------------
. Fixed Problem
    - Fix "audio frequency 瞬間 incorrect issue while playback DivX w/ multi-audio switching { [UTL] }
    - Enhance "po-po" noise while playback DivX file w/ SCAN--> PLAY or repeat AB mode { [UTL] }
    - Fix Mantis #427: A/V playback maybe hang if playback NF-200 CELL Test --> VOBU Still & Navigate Timer Cell, then playback any DVD motion { [CELL] }
    - Fix "OSD time can't update while Pause --> FF/ SCAN" bug.  { [CC] }
    - Fix the bug #370: 按"Enter & L/R鍵"->互相連續切換=>會有殘影"H"字體在畫面中間. {osddsply.c}
    - Fix the bug #408: 刪增檔案後=>結果會讀錯檔位置. (Delay 500 Msec between each picture. Temporal solution to fix USB problem. YC will debug the real cause.)
    - Fix the bug #411: 進入Thumbnail Mode=>結果檔案無法全部show出來. (Delay 500 Msec between each picture. Temporal solution to fix USB problem. YC will debug the real cause.)
    - Fix the bug #206: The information in the information region of MM UI is wrong. {osdmm.c}
    - Fix the bug #107: CDG=>Normal Play會down機. {cdg.c}
    - Fix the bug #358: system crashes after one track playback {cdg.c}
    - Fix the bug #333: JPEG 圖形右邊有不規則細邊. {jpegfmt.c/ jpegfmt.h/ jpegdec.c/ jpegdec.h}
    - Tune decoding performance: (1) re-arrange orders of variable placement (2) improved IDCT flow and wait_vdfifo() callings. {jpegfmt.c/ jpegfmt.h/ jpegdec.c/ jpegdec.h}
    - Enhance the behavior from SERVO STOP mode. {cc.c/servo.c/osdss.c}
    - Fix AVI scan -> play OSD don't show play problem. {cc.c}
    - Fix AVI resume from JPG will play from start problem. {cc.c/manager.c}
    - Fix press KEY_REPEATAV from SLOW mode, then repeat AB OSD don't show problem. {cc.c}
    - Fix pause mode and press KEY_SCF, then OSD don’t show problem. {cc.c}
    - Fix SP2 will be turn on when switch SP1 to enable mode even SP2 is disabled originally. {cckey.c}
    - Fix MP3 won't go next file automatically when select UI is enabled mode. {media.c}
    - Fix press KEY_PLAY will disable zoom mode problem. {mmanager.c}
    - Fix when last file is JPG, then will enter thumbnail mode when play to disc end. {mmanager.c/linear.c}
    - Fix display mode abnormal when change to different extname file in playing mode. {mmanager.c}
    - Fix some JPG can’t display well and some MP3 will have bo noise when play end probmem. {usbsrc.c}
    - Fix TDL-0002: 19->20 have blocks at begin. {Mpeg2dec.c}
    - Fix ROM code bug for XviD 火影忍者. {Decore.c/ mp4_header.c/ mp4_header.h/ mp4_vars.h}
    - Support Anti-Shock buffer setting. {Ctkav.h/ HAL.C/ UTL.C}

. Enhanced Items:
    - Support 2 USB port. Fix the problem #391: JPEG檔的Date不會顯示資料 {osdnd.c/osdnd.h/osddsply.c/thumb.c}
    - Support auto-dump DRAM from system. {debug.c/debug.h}
    - Support USD wording "No USB Device" and "No playable file". {osdnd.c}
    - Support GDI Library. {gdi.c / gdi.h}
    - Support to remove the UI style options. {osd3.h}
    - Support TFT code. {osd3.h/setup.c/setup.h/input.h/cc.c/winav.h}
    - Support two feature while playback un-support A/V format DivX file { [UTL], [MMANAGER] }
    - Add function code for USB auto upgrade. {fat.c/infofilter.c}
    - Support Customer ID at Auto-Upgrade. {flash_dsu_main.upgrade/ flash.bin/ APLoader.c/ APLoader.h/ Appacker.exe/ MergeROM.exe/ Makefile/ Disp.c/ Customer.h/ Winav.h}
    - Support makefile switch "make 4m" to build 4M flash code and "make servo" to compile servo code in RELEASE dir.
    
. Servo Fixed Items:
    - Please reference readme_srv.txt.
. Audio Fixed Items:
    - HDCD can work.
    - Pcm data to do fade in and fade out.
    - Write MP3 bitrate every frame.

. Left Issues:

/**************************************************************************
Version :    1.05a
Date    :    2005.06.14 / servo 0.95e-2
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 05202005
. Display Code Date
    - 05312005
. DivX Code Date
    - 05312005
. Navigate Code Date
    - 05312005
. Parser Code Date
    - 06142005
. Audio Code Date
    - 06132005
. Servo Code version and date
    - 06142005 / servo.095e-2
----------------------------------------------------------------------------
. Fixed Problem
    - Fix resume can't work while STOP --> PLAY --> STOP before A/V start to playback --> PLAY. {utl.c}
    - Fix MM UI abnrmal while STOP --> PLAY --> STOP before A/V start to playback. {mmanager.c}
    - Enhance Po-Po noise problem. {hal.c/audio code}
    - Channel mapping wrong problem. {hal.c}
    - Audio is later than video while SCAN --> PLAY for some file, ex. E03. {parser code/audio code}
    - Fix CDROM auto upgrade failed problem. {aploader.c/falsh.bin}
    - Fix Window XP format U-disk can't read problem. {fat.c}
    - Fix one file read failed when play end in 256M U-Disk problem. {usbsrc.c}

. Enhanced Items:
    - Fix some DVD reading worse problem.
    
. Servo Fixed Items:
    
. DivX Fixed Items:

. Audio Fixed Items:
    - Fix Audio is later than video while SCAN --> PLAY for some file, ex. E03.
    - Enhance Po-Po noise problem. 

. Left Issues:

/**************************************************************************
Version :    1.05
Date    :    2005.06.8 / servo 0.95e
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 05202005
. Display Code Date
    - 05312005
. DivX Code Date
    - 05312005
. Navigate Code Date
    - 05312005
. Parser Code Date
    - 06072005
. Audio Code Date
    - 06032005
. Servo Code version and date
    - 06072005 / servo.095d
----------------------------------------------------------------------------
. Fixed Problem
    - Fix HW power on music -> play AV w/o audio => power on music appears again. {hal.c}
    - Fix no-sound bug when enter and exit Setup UI. {setup.c}
    - Fix the bug #330: 畫面就Hang住, 不能播放了 2.畫面重疊在一起(畫面就Hang住, 不能播放了). {gamemain.c}
    - Fix the bug #331: Game遊戲畫面,會有背景音樂出現. {gamemain.c}
    - Fix Ritek 4/1 --> KEY_NEXT, the "parental level N" still picture don't wait 3 sec issue. { [CELL] }
    - Fix prescan action is not issued for some special operation procedure { [LINEAR] }
    - Fix after selecting SP off in sp Menu, the user's SP selection will not be kept when user press SP change button. {NAVIGATE.C}
    - Fix the problem #312 of Mantis: Thumbnail mode 可自由移動游標. {thumb.c}
    - Fix the bug #341 of Mantis: VCD menu audio will lose about 2-3s audio when play end. {vcd20.c}

. Enhanced Items:
    - Macrovision Certification: avoid color burst start time bug when MV is on in CT909AC. {tve.c}
    - Enhance Dolby Certification code. {HAL.c/HAL.h/CHIPS.c/MM.c/UTL.c}
    - Support AutoUpgrade from CDROM. {Autoupgd.c/ Autoupgd.h/ Infofilter.c/ CC.c}
    - DRAM re-allocation. {Ctkav.h/ Makefile/ Makefile.rel/ DVD909.ld}
    - Support Data Cache power saving feature { [UTL], [INITIAL] }
    - Add protection code to fix playback abnormal issue if reading title key fail { [PGC], [SERVO] }
    - Support "Random" JPEG effect. {haljpeg.c/haljpeg.h/osdnd.c/osdstr.h}
    - Support Servo power down. {osdss.c/cc.c/servo code}
    - Support APLoader to upgrade Flash
    
. Servo Fixed Items:
    - Please reference servo.092 readme_srv.txt file.

. DivX Fixed Items:
    - Add #define BUG_ESCAPE_CODE to fix DivX_311 HW bug. The bug will be fix at CT909AC. Default setting: CT909AC. {Decore.c/ Mp4_picture_311.c}
. Audio Fixed Items:
    - Add audio code protection mechanism.

. Left Issues:

/**************************************************************************
Version :    1.05p
Date    :    2005.05.26 / servo 0.93
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 05202005
. Display Code Date
    - 05202005
. DivX Code Date
    - 05202005
. Navigate Code Date
    - 05202005
. Parser Code Date
    - 
. Audio Code Date
    - 02502005
. Servo Code version and date
    - 05262005 / servo.093
----------------------------------------------------------------------------
. Fixed Problem
    - Fix WMA total time is larger than real total time. {OSDMM.c / UTL.c}
    - Fix can’t input >= 10 track number within digest mode (seems don’t process 10+ key) {digest.c}
    - Item didn’t play to the end before jump(JVC test disc) {linear.c}
    - Fix (CDG) Play->Search time[00:11:11]=>結果無歌詞的部分背景顏色會變淡. {osdsrch.c}
    - Fix SETUP->按CODE出來(SETUP不要關掉)->直接OPEN->SETUP, 右上角會有殘影. {setup.c}
    - Fix (1) LVP0801 title 11 audio underflow issue; (2) Matrix highlight can't be seen { [CELL] }
    - Fix Mantis bug #92: "NF-200 desired menu don't appear while ROOT Menu --> choose "Presentation Test" --> Choose "9: Next Page" { [CELL] }
    - Fix Mantis bug #173: "video block appear during track 1 auto cross to track 2 while playback VCD - "xxx MTV" w/ PBC on mode  { [UTL] }
    - Avoid last data will be kept in buffer, and decoder not decode  { [UTL] }
    - Enhance GoTime action algorithm to fix audio lost a litter while DivX file playback --> SCB --> FF --> Play immediately{ [CC], [LINEAR], [UTL] }
    - Fix Repeat/ Program mode work abnormal while enable "DivX certification" define  { [MMANAGER] }
    - Fix no A/V playback by (1) any DivX file auto playend --> KEY_PLAY; (2) DRM checking dialog appear --> abort playback --> KEY_PLAY two operation while enable "DivX certification" define"  { [MMANAGER] }
    - Fixed thumbnail "central alignment horizontally" issue. {jpegdec.c, jpegdec.h, jpegfmt.c, jpegfmt.h}
    - Checked and make sure for continuous slideshow playing: 1) completed error handling 2) filed #325.{jpegdec.c, jpegdec.h, jpegfmt.c, jpegfmt.h}
    - Fix #105: CDG字體會超出畫面. {cdg.c}
    - Fix #109: CDG音樂會先撥放約5秒後,畫面才會開始show歌詞. {cdg.c}
    - Fix #118: CDG Play->Program完->Pause=>結果會down機. {cdg.c}	 
    - Fix #114: CDG Play->STOP=>會down機. {cdg.c}
    - Fix #283: "進入SETUP將Angle Mark設定為"On",結果沒有顯示Angle Mark. " {osd.c/osddsply.c}
    - Fix #265: "取消Display時不會顯示Angle的小符號. " {osd.c/osddsply.c}
    - Fix low bitrate WMA can't play end problem. {mmanager.c}
    - Fix when play scratched disc, then JPG maybe will hang and MM also won't go next file problem. {mmanager.c}
    - Fix when enter thumbnail from MM UI and KEY_PLAY immediately, then flow will be wrong. {mmanager.c}
    - Fix bitrate information disappearred after pause -> play. {mmanager.c}

. Enhanced Items:
    - Support general USB disc(64M ~ 2G) with FAT16 file system. {usb.c/usbsrc.c/fat.c/infofilter.c/mmanager.c/srcfilter.c}
    - Add select media UI opeartion and control flow to switch USB / SERVO. {cc.c / media.c}
    - Don’t parse in mm-motion files temply until speed is ready. {cdinfo.c}
    - Support power up music. {HAL.c / TONE.h / INITIAL.c/ SETUP.c / UTL.c}
    - Add one "Off" item in Setup menu "Parental Control". {setup.h / osd3.h}
    - Support keep user last SPST / AST selection. {NAVIGATE.C / NAVIGATE.H}
    - Improved decoding performance about 7% by: 1) removing or flattening some functions and macros. 2) re-arranging variables. {jpegdec.c, jpegdec.h, jpegfmt.c, jpegfmt.h}
    - Add ReadData failure recovery and retry. {usb.c/usb.h}
    - Add "Normal" and "Smart Play" strings. {setupstr.h}
    
. Servo Fixed Items:
    - Please reference servo.092 readme_srv.txt file.

. DivX Fixed Items:
    - Fix CT909_F000 #200: System maybe dead if pressing KEY_SCF x2 continue during pre-scan a big DivX file.
    - Fix decode error if switch between interlace / noninterlace bits.
    - Add release code control for DivX.
    - Add PSNR code.
    
. Audio Fixed Items:

. Left Issues:

/**************************************************************************
Version :    1.02a
Date    :    2005.05.11 / servo 0.92a
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 05112005
. Display Code Date
    - 05112005
. DivX Code Date
    - 05112005
. Navigate Code Date
    - 05062005
. Parser Code Date
    - 
. Audio Code Date
    - 
. Servo Code version and date
    - 05112005 / servo.092a
----------------------------------------------------------------------------
. Fixed Problem
    - Fix bug: Press KEY_SCB during TT/CC: 01/04, Time: 35:55 ~ 35:27 of DVD title "Pirates of the Ciribbean" -- Chuan reported  { [CELL] }
    - Fix trap table protection range error issue { [INITIAL] }
    - Fix #205 of DVD_102: "The mute icon is wrong." {thumb.c/thumb.h}
    - Fix PIP still appear after power down/open close when display mode is enabled. {cc.c}
    - Fix SP disappearred after scan -> play. {cc.c}
    - Fix enable SP1/SP2 and then disable SP1, then SP1 can't enable from navigator. {cckey.c}
    - Support preview picture with rotate status. {mmanager.c}
    - Fix DivX will play in resume mode in program edit mode after program playing. {mmanager.c}
    - Fix Thumbnail will get wrong file from MM mode. {mmanager.c}
    - Fix bug #207. The display UI can't be recovered. (osdsrch.c)
    - Fix bug #199 of CT909_F000: "Zoom mode curson don't appear on PIP center area while TV mode switching. ". {osd.c}
    - Fix EXIF parser for reading TestJPEG Disc#1 impress/ rwc1.jpg under thumbnail mode.
    - Fix zig-zaged blocks @ the left edge of progressive JPEG pictures.
    - Fix reserved coefficient buffer size for slideshow problem of  TestJPEG Disc#1 Kavalan Sc (2).
    - Fix some code can't boot successful problem. {boot.c}
    - Fix CT909_F000 #200: System maybe dead if pressing KEY_SCF x2 continue during pre-scan a big DivX file
    - Fix CT909_AP10 #78: 快進/快退後,SUBTITLE 1會瞬間show英文約1sec之後,SUBTITLE又會恢復為原設定輸出.
    - Fix CT909_AP10 #79: LOGO會變成鋸齒狀

. Servo Fixed Items:
    - Please reference servo.092 readme_srv.txt file.
. DivX Fixed Items:
. Left Issues:


/**************************************************************************
Version :    1.02
Date    :    2005.05.09 / servo 0.92
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 05065005
. Display Code Date
    - 05062005
. DivX Code Date
    - 05062005
. Navigate Code Date
    - 05062005
. Parser Code Date
    - 05062005
. Audio Code Date
    - 05062005
. Servo Code version and date
    - 05062005 / servo.092
----------------------------------------------------------------------------

. Fixed Problem
    - Modify DivX next scan jump position calculation algorigthm { [UTL] }
      [1]Enhance OSD time gap is no regular rule for different file w/ same scan level; 
      [2] Fix DivX testing report bug "B03" file SCF x4 直接跳播下一個檔案; 
      [3] Fix DivX testing report bug "C16" 按SCBx2畫面會Hang住約5~9秒才會快退.
      [4] Fix DivX_testing report bug: "B01" file SCB mode may hang issue
    - Fix no audio output while playback any DivX w/ no audio --> any CDROM motion { [UTL] }
    - Fix OSD time will show larger total file time value during file end {Fix DivX testing report bug:  "T03" 播到3:00:26時,秒數會播到3:00:31-32後再回到3:00:26-27時間點,再往下一章播 { [UTL] }
    - Fix DVD_101 bug #4 "DAT/VOB SCB x2 abnormal, seemed too slow and OSD will jump to bigger time sometimes. For example, OSD show 4:56, 4:55, 4:56 sometimes. { [UTL] }
    - Fix DVD_101 bug #6 "When gototime in VOB file, will see OSD show “0:00:00” then the requested jumping time. { [UTL] }
    - Fix DVD_101 bug #11 "OSD time is not clear to zero and keep the previous value while playback any file first ? KEY_STOP ? re-play the same file ? KEY_DISPLAY immediately. ex. DivX certification title, “t01_t02_xxx” { [UTL], [MMANAGER] }
    - Fix DVD_101 testing report bug #8: SCB x2 is too fast { [CELL] }
    - Fix can't get SCAN_DONE ack if issue SCAN command near DivX file end, since two I-frame distance maybe > 5 or more sec { [CC], [UTL], [CELL] }
    - Reduce the waiting time while start to playback the same DivX file { [INITIAL], [LINEAR], [MMANAGER] }
    - Fix F/W issue audio master mode for DivX multi-Audio switching { [DECORE] }
    - Add error recovery mechanism while 1st pre-scan fail for CDROM motion file. { [UTL] }
    - Disable "SCF/ SCB/ FF/ STEP/ FREEZE/ SLOW" key before CDROM motion pre-scan action done. { [MMANAGER] }
    - Fix DivX certification title's file "E05" audio stream index 5 playback sound like "卡通聲" { [UTL] }
    - Fix bug #40 of CT909_AP10: "頻譜顯示不完全,會有缺角." {osdmm.c}
    - Fix bug #46 of CT909_AP10: "編輯模式字樣不會消失". {osdmm.c}
    - Fix bug #183 of CT909_F000: "Mute icon disappearred after play MM-motion file." {osdmm.c}
    - Fix bug #179 of CT909_F000: "Button and info overlapped in thumbnail mode." {thumb.h}
    - Fix bug #71 of CT909_AP10: "1.一開始沒有畫面,有框框出現,後來畫面才慢慢出現, 2.有框框出現(框框一直重複閃爍), 但沒有畫面出現". {thumb.h}
    - Fix the bug about the red frame is wrong in Program mode for some composite folder. {thumb.c}
    - Fix CDG N<->P, display abnormal  [CDG/HAL.c]
    - Fix IR Power off-> power on => noise when reading disc [HAL.c]
    - Fix some CDDAs are detected as CDG [INFOFILTER.c]
    - Fix bug 花木蘭/松下測試碟讀到碟時,PBC OFF=>結果會回到CTK Logo. {NAVIGATE.C / NAVIGATE.H}
    - Fix title"親家路窄" , will show parental error for first play. {DVDINFO.C / DVDINFO.H}
    - Fix bug 0000096: Program完play=>結果Subtitle會"OFF";Audio會切換為"英文"1/5 AC3 5.1CH". {NVCMD.C}
    - Fix: Playing CDDA and enter digest mode, it will crash. {digest.c}
    - Fix: It can't paly to the end of VCD in linear mode. {linear.c}
    - Fix: OSD is ruined by the Search UI when it should not enter the Search mode. {osdsrch.c}
    - Fix " SP HLI appear on LOGO while playback Nero title menu --> Open Try { [INITIAL] }
    - Fix some CDDAs are detected as CDG by two consecutive pattern matchs { [UTL] }
    - Fix A/V hang while playback SVCD 跳跳虎 w/ repeat A-B { [UTL] }
    - Fix CDG detect hang sometimes problem. {cc.c}
    - Fix PIP don't clear after open/close problem. {cc.c}
    - Fix PIP don't clear after KEY_STOP problem. {cckey.c}
    - Fix second SP auto appear after continuous pressing KEY_SPST to switch SP1 in DVD disc. {cckey.c}
    - Fix AP10 mixed CDROM sometimes parse wrong problem. {cdinfo.c}
    - Fix AP10 mixed CDROM only parse to second deep dir problem. {cdinfo.c}
    - Fix auto upgrade entering pesudo thread problem. {infofilter.c}
    - Fix JPG may hang in continuous re-decode flow. {mmanager.c}
    - Fix JPG repeat folder from last picture to first picture video black problem. {mmanager.c}
    - Fix KEY_PLAY_PAUSE don’t work when scan mode. {mmanager.c}
    - Fix audio freq information disappearred when scan to play. {mmanager.c}
    - Fix PIP still appearred when KEY_STOP in MM-motion mode. {mmanager.c}
    - Fix audio noise when playing DVD. {usbsrc.c}
    - Fix Disc Key can not detect well problem. {pardrm.a/pardrm_4m.a}

. Enhanced Items:
    - Support 4M flash code define and config. {winav.h/pardrm.a/audio code}
    - Support Dutch and Polish language in Europe version. {fontable.txt/FontIndx.def/osdstr.h/setupstr.h/UI_Europe.xls/FontMapping.xls}
    - Support OSD game. {Makefile/cc.c/customer.h/gdi.c/gd.h/initial.c/osdgame}
    - Support 3ivX MPEG-4 format. {parser relative code}
    - Support Scan/ Repeat AB feature for DivX w/ skip video stream case { [CC], [OSDSRCH], [UTL], [INTERRUPT] }
    - Diable all DRM or AES related feature if enable "#define NO_DRM" { [UTL], [INITIAL], [CC], [MMANAGER], [SETUP], [OSD related code], [WINAV] }
    - Add MPEG-4 icon and string in MM UI.
    - Support GDI_CHECK_DISP_MEM_LINE to improve the OSD "horizontal line flash" due to Update-While-Displaying . {GDI.C}
    - Support the Servo information dump in backdoor. {setup.c}
    - Support the Polish & Dutch menu selection. {setup.c}
    - Support reading __bSupportFeature from package to decide parse in DivX and WMA or not. {cdinfo.c}    
    - Support DAT/VOB/DivX resume function. {mmanager.c}
    
. Servo Fixed Items:
    - Please reference servo.092 readme_srv.txt file.

. DivX Fixed Items:
    - Enhance the DivX flow. {DivX relative code}
    - Fix t01-t02 7:14 has some blocks.
    - Fix TDL0002: TT#18: After 00:26:00, the left side of character 'T' is disappeared.

. Audio Fixed Items:
    - Modify one code to remove WMA and DTS (only include DTS SPDIF-RAW) for 4M flash.
           
. Left Issues:

/**************************************************************************
Version :    1.01
Date    :    2005.04.20 / servo 0.91a
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 04182005
. Display Code Date
    - 04182005
. DivX Code Date
    - 04192005
. Navigate Code Date
    - 04182005
. Parser Code Date
    - 04182005
. Audio Code Date
    - 04182005
. Servo Code version and date
    - 04202005 / servo.091a
----------------------------------------------------------------------------

. Fixed Problem
    - Fix PIP bolck appearred when issuing KEY_TVMODE in MM mode. {osd.c/hal.c}
    - Fix DVD CSS disc platback failed problem. {infofilter.c}
    - Fix JPG preview -> KEY_NUMBER to MP3 file to play -> Then spectrum and preview picture appearred together. {mmanager.c}
    - Fix play CDROM motion files and press KEY_STOP three times, then system hang and show invalid OSD continuously. {mmanager.c}

. Enhanced Items:
    - Support JPG slide show playing from USB source. {mmanager.c}
    - Support macrovision certification. {tve.c}
    - Support Dolby certification.
    - Support DAT playing from USB source. {par_mpeg.c}
           
. Left Issues:

/**************************************************************************
Version :    1.01 1st
Date    :    2005.04.19 / servo 0.91
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 04182005
. Display Code Date
    - 04182005
. DivX Code Date
    - 04192005
. Navigate Code Date
    - 04182005
. Parser Code Date
    - 04182005
. Audio Code Date
    - 04182005
. Servo Code version and date
    - 04192005 / servo.091
----------------------------------------------------------------------------

. Fixed Problem
    - Fix AP10 report bug #246 SP2 display incorrect sp stream. {NAVIGATE.C / NAVIGATE.H / CC.C / CC.H / CCKEY.C}
    - Change the VTSI size definitation 540 to 542 bytes, Micky say it should be 542 bytes in current version. {dvdinfo.c}
    - Fix AP10 report bug #421. {dvdinfo.c}
    - Fix some blocking issues for decoding in thumbnail mode. {jpegfmt.h/ jpegfmt.c/ jpegdec.c}
    - Fix tray opened -> power down -> power on -> then disc recognized. {initial.c}
    - Fix "some thread not initial done" error message appear on serial-debug screen while H/W power on { [INITIAL], [Servo related code] }
    - Disable Repeat AB/ SCF/ SCB/ GoTime/ Resume feature for DivX w/ video resolution > 720*576 temporally { [CC], [OSDSRCH], [LINEAR] }
    - Fix OSD time is not updated while playback the DivX file w/ video resolution > 720*576 { [HAL] }
    - Fix bug: it take 5 sec while stop --> resume @ VCD 2.0 光良與品冠 track 1 { [UTL], [MPEG2DEC.C] }
    - Fix bug "Calcomp DivX #1 title : ROOT->DivX5->1K-BF/,按GOTOTIME到1分,畫面會Hang住不會動"  { [UTL] }
    - Fix bug "no video output, and only audio output while playback Lab Combo title: 0The Hunt (542th file) --> 099DAT (541th file) --> 0The Hunt (542 file" { [UTL] }
    - Fix the bug about PIP zoom coexit with other UIs. Fix the bug #384, #451, #455, #456 of M200 DVD_100 test report. {osd.c/osdnd.c}
    - Fix the bug #387 of M200 DVD_100 test report. {thumb.h}
    - Fix #400: SUBTITLE1設定為英文,SUBTITLE2設定為中文;當撥放至1/4,00:27:47與00:28:00時,SUBTITLE1[英文]會在SUBTITLE2[中文]下方一閃即過,
      且字體會破破的,類似殘影.(機率:1/10;DVD 神鬼奇航,Mona). {Disp.c/ Subpict.c/ Subpict.h}

. Enhanced Items:
    - Support playing MP3/DivX and JPG preview from USB source. {usb.c/usbsrc.c/media.c/infofilter.c/fat.c/mmanager.c}
    - Support WMA. {MM.c/UTL.c/HAL.c}
    - Support DivX DRM real-time decoding. {parser code}
    - Change flow for PRESCAN feature from API mode to Thread mode. {parser code}
    - Add the protecting for the font table. {osdfont1.c/initial.c}
    - Modify DivX pre-scan proceudre into several sub-routine, since pre-scan action must be done in stage by stage method. {utl}
    - Set 1stSCR value as zero while can't find NV sector during CELL_TimeSearch. {cell}
    - Enable Repeat A-B feature for DivX {cc/linear/utl/mmanager}
    - Implement CDROM motion pre-scan action in stage trigger procedure { [UTL], [LINEAR], [CC], [MMANAGER], [Parser related code] }
    - Modify the color of the PIP zoom. {osdnd.h}
    - Modify the method about updating the audio spectrum. {osdmm.c/osdmm.h}

. Servo Fixed Items:
    - Please reference servo.091 readme_srv.txt file.

. DivX Fixed Items:
    - Fixed the bug that the parser sends new bitstream which is not I frame when search command is issued

. Audio Fixed Items:
    - Modify MP3 power spectrum line
    - Support WMA playback.

           
. Left Issues:

/**************************************************************************
Version :    1.00a
Date    :    2005.04.14 / servo 0.90
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 03312005
. Display Code Date
    - 04062005
. DivX Code Date
    - 04062005
. Navigate Code Date
    - 04062005
. Parser Code Date
    - 04062005
. Audio Code Date
    - 04062005
. Servo Code version and date
    - 04062005 / servo.090
----------------------------------------------------------------------------
. Fixed Problem
    - Fix SP flash when switch audio in SP off mode. {subpict.c}
    - Fix DVD Navigator on -> open -> close -> Disc recognize failed problem. {osddsply.c}
    - Fix DVD Navigator on -> Open display mode -> .. -> display mode off -> Navigator will loss top image. {osddsply.c}
 
/**************************************************************************
Version :    1.00
Date    :    2005.04.06 / servo 0.90
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 03312005
. Display Code Date
    - 04062005
. DivX Code Date
    - 04062005
. Navigate Code Date
    - 04062005
. Parser Code Date
    - 04062005
. Audio Code Date
    - 04062005
. Servo Code version and date
    - 04062005 / servo.090
----------------------------------------------------------------------------

. Fixed Problem
    - Flash current frame to prompt user in Thumbnail UI. {thumb}
    - Fix the bug about "#6 把整個OSD開出來,按Pause,osd上的時間會有橫條紋在閃動" in DivX test report. {osddsply.c}
    - Fix VCD2.0 disc stop->resume failed problem. {cc.c}
    - Fix some bugs for "Disc Resume" feature. { [CC], [INITIAL], [SETUP], [NAVIGATER], [UTL] }
    - Fix audio maybe lost a little while SCAN to NPF mode { [CC] }
    - Fix the A/V sync.not correct issue while GoTime or SCAN to NPF mode  { [UTL] }
    - Fix progressive JPEG decoding with MC-scaling    	
    - Fix CDDA Stop->SCF: no sound [UTL.c]
    - Fix some files can't be parse in problem when these files have some same chars ahead. {mmanager}
    - Fix user can't go prev files when play some un-playable MP3 files. {mmanager}
    - Don't enable KEY_ANGLE_CTL when playing non-JPG files. {mmanager}
    - Fix when zoom-in in motion files and stop, then play. The picture will be CLIP problem. {mmanager}
    - Fix CDG data parsing error problem {parser code}
    - Fix a timing issue when parsing index table {parser code}
    - Fix DivX 311 titles bought several years ago. It uses "db" to indicate key frame (I frame) and "dc" 
      for others (P frame). {parser code}
    - Fix NF200: Presentation Test->SP Selection->Karaoke,字幕會閃爍且會有方格在跳動. {SUBPICT.C}
    - Provide a method to report the code size of CT909. If overflow, it will break the compiler. 
      {Script3.sh/ Makefile/ Makefile.rel/ codecheck.sh/ codecheck.exe/ DVD909.ld}
    - Let slideshow completed quickly when KEY_NEXT. {DISP.C/ DISP.H}
    - Fix OSD time still going before the desired point decode & display ok { [UTL], [Parser related code] }
    - Fix A/V still continue a moment for DivX GoTime value input done { [UTL] }
    - Fix trap into the same point for DivX SCAN mode { [UTL], [Parser related code]
    - Fix OSD display the file end time first then show the desired position while VCD GoTime action { [UTL] }
    
. Enhanced Items:
    - Support changing UI style from SETUP for Media Manager, Display mode, Thumbnail and Digest UI.
    - Support using "KEY_CLEAR" to start-up Navigator UI when define SUPPORT_INDEPENDENT_NAVIGATOR. {osddsply}
    - Support Disc Resume feature while enable "#define SUPPORT_RESUME_AFTER_POWERON" or "#define SUPPORT_RESUME_AFTER_OPENCLOSE"
      { [CC], [INITIAL], [SETUP], [NAVIGATER] }
    - Support CDG AV sync. {cdg.c/h}
    - JPEG & MPEG use diff TVE gains [TVE.c/MM.c/UTL.c]
    - Support DivX (1) index table parsing function , it helps to do tricky operation SCAN/ SEARCH, 
      (2) AV PTS for synchronization. {parser code}
    - Re-config the DRAM table for text/data/bss. {ctkav.h}
    - Enhance the performance while DivX GoTime action { [UTL] }

. Servo Fixed Items:
    - Please reference servo.090 readme_srv.txt file.

. DivX Fixed Items:
    - implemnet search command

. Audio Fixed Items:
    - Fix KEY+/- audio volume small problem.
    - fix AC3 mute on and scan-> play or next, then audio appearred but still in mute mode problem.
    - Fix audio shake when play DVD 神鬼奇航 problem.

           
. Left Issues:

/**************************************************************************
Version :    0.96
Date    :    2005.3.31 / servo 0.86
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 03312005
. Display Code Date
    - 03312005
. DivX Code Date
    - 03312005
. Navigate Code Date
    - 03232005
. Parser Code Date
    - 03312005
. Audio Code Date
    - 03312005
. Servo Code version and date
    - 03232005 / servo.086
----------------------------------------------------------------------------
. DivX Fixed Items:
    - Enhance the DivX playback ability.
    - Support DivX AV sync.
    - Support DivX gototime and scan function.

/**************************************************************************
Version :    0.95
Date    :    2005.3.23 / servo 0.86
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 03232005
. Display Code Date
    - 03232005
. DivX Code Date
    - 03232005
. Navigate Code Date
    - 03232005
. Parser Code Date
    - 03232005
. Audio Code Date
    - 03232005
. Servo Code version and date
    - 03232005 / servo.086
----------------------------------------------------------------------------

. Fixed Problem
    - Fix DVD-> Time search, if the search time is invalid, the A/V playback Hang(NF200 and  002)[cc.c/ navigate.c]
    - Fix NF250 highlight change should play to 25s only.[cell.c]
    - Fix NF250 Cell Test(VOBU still)Video hang a moment then continue the playback after last time KEY_PLAY pressing when VOBU still[cell.c]
    - Fix #230 of DVD_091a_M200 test report: "播到後面幾張圖片,再按Play(or stop)-> digest到瀏覽模式裡,右上角的頁數會沒有顯示出來[頁:2/2].機率10/10 (東亞 Picture CD) {thumb.c}
    - Fix #265 of DVD_091a_M200 test report: "進入LEVEL 1內->再進入其中一個JPEG檔後->按MENU鍵=>UI上方會有白色殘影,一閃即過.(機率:6/10;VCD MP3+JPEG+WMA+ DivX)" {mmanager.c}
    - Fix #229 of DVD_091a_M200 test report : "再切換下一張之前,切換Angle,畫面會有拖影的情形.機率8/10 (東亞 Picture CD)" {mmanager.c}
    - Fix #232 of DVD_091a_M200 test report: "Stop->Play到畫面中,OSD會有彩色殘影.機率3/10 (柯達Picture CD)". {osddsply.c}
    - Fix the bug in DVD_Forum test (NF200): It display SP2 OFF on OSD Navigator UI. But it don't choose "OFF" item of subpicture 2 selection list.  {osddsply.c}
    - Fix [M200 085a]Presentation Test->Test Signals->next page->previous page,Highligh會跑掉.機率10/10
    - Fix the bug that the reading password error from EEPROM {setup.h}
    - Fix Search Mode action abnormal while repeat AB run once time. { [UTL] }
    - Fixed some AVI files, Howel's Castle, can not be played
    - Fix the problem of "DVD will recognize wrong after playing multi-session disc
    - Fix SP off mode and scan, then KEY_PLAY to enter normal play mode. Then SP is show in screen
    	
. Enhanced Items:
    - Support new DRAM alloc. (512 fonts/ Index table of DivX/ Extra JPG original buffer)
    - support runtime reset Proc2 [HAL.c / INITIAL.c / Boot Code]
    - Add function GDI_WaitPaletteComplete, call it to wait set palette complete. {GDI}
    - Support DivX DRM UI & control flow { [UTL], [CC], [MMANAGER], [OSDDIVX], [SETUP], [Parser related code] }
    - Support AVI index table scan function
    - Support CDG playback, but still have problem in it.
    - Support 3ivx (MPEG-4 format AVI file)
    - Support DVD/VCD/CDROM audio files error concealment function. {monitor.c}
    - Support progressive JPG file decoding. {jpegdec/jpegfmt}
    - Support H/W reset proc2.


. Servo Fixed Items:
    - Please reference servo.086 readme_srv.txt file.

. DivX Fixed Items:
    - Fixed blocky artifacts due to the U,Y components
    - Smooth transitions between  video clips with differnt witdths or heights ( controlled by HAL_AdjustTVMode() )
    - FF mode  display order is correct


. Audio Fixed Items:
    - Audio decoding hang causes by SPDIF buffer full and can't read by S-port
    - AC3 sound overflow
    - AC3 can't decode advance bitstream
    - Remove A/V sync Divx mode
    - Change reset register from GR(23) to GR(25) and mask clear function for  GR(0~GR(31).
    - Move audio start address to 0x400b0000
           
. Left Issues:


/**************************************************************************
Version :    0.91
Date    :    2005.3.2 / servo 0.85a
Chip    :    CT909
Name    :    CheerTek (D300 WYC)
----------------------------------------------------------------------------
. Decoder Code Date
    - 03022005
. Display Code Date
    - 03022005
. DivX Code Date
    - 03022005
. Navigate Code Date
    - 03022005
. Parser Code Date
    - 03022005
. Audio Code Date
    - 03022005
. Servo Code version and date
    - 03022005 / servo.085a
----------------------------------------------------------------------------

. Fixed Problem
    - Fix "jump to next track directly while MP3 SCF to file end --> SCB" issue. {utl}
    - Fix "VCD GoTime" action error. {utl}
    - Fix "NF200 cell still time will display as 0". {utl}
    - Fixed some AVI file can not parse header correctly. {parser}
    - Fixed incorrect SCR value for file VOB/MPG at DVD-ROM. {parser}
    - Fixed audio only stream has no sound when PLAY->SCF->PLAY. {parser}
    - Fix one disc with more than 100 dirs that loss 10 dirs problem. {cdinfo}
    - Fix one disc that file name will parse less two chars and will dumplcate file number in each dir problem. {cdinfo}
    - Fix repeat one failed when program mode problem. {linear / mmanager}
    - Fix JPG zoom mode -> MM motion mode, then CLIP wrong problem. {mmanager}
    - Fix THUMB -> JPG slide show -> program, cursor disappearred problem. {mmanager}
    - Fix pause -> play then MP3 information about bitrate & freq disappeared problem. {mmanager}
    - Fix when entering program list mode when playing mode, then file don’t go next song. {mmanager}
    - Fix KEY_UP will go wrong position when cursor stay in top of dialog box. {mmanager}
    - Support correct behavior when playing mode in program list mode. {mmanager}
    - Avoid to resend NV_PCK when servo jumps a long range. {cell}
    - Fix the bug #213 of 085a_M200 test report: "Repeat A-B->Pause->Play->在暫停的符號消失前,將Repeat A-B取消,
      Repeat A-B還是會一直在右上角上不會消失,聲音畫面都正常播放,再按一次Repeat A-B才會消失.機率8/10 (VCD 國語金曲19)". {osd}
    - Fix the bug #20 of DVD_090 test report, "[VCD2.0 Digest] “曲目瀏覽” message can’t recover after display “invalid” 
      message. ex. NPF -> KEY_DIGEST to enter digest mode -> KEY_GOTIME". {osddg}
    - Fix the bug #53 of DVD_090 test report, "TV mode will see one black video block in right bottom position." {osdnd}
    - Fix error of "Huffman Table Index outside range". {jpg code}
    - Fix issues of decoding/ operation alignments. {jpg code}
    - Fix error of "Bad Huffman Code Length" {jpg code}
    - Fix "strange color around block shapes" {jpg code}
    - Change operation method -- append pixels to height for 32x-alignment jpg code and place the 
      1st-scaled picture in the center. {jpg code}
    - Fix the bug #7 of DVD_090 test report {osd3}
    - Fix the bug of Repeat AB which will be disabled while entering the search mode {osdsrch}
	
. Enhanced Items:
    - Support zoom pan mode in MM-motion mode. {mmanager}
    - Convert unknown file name to A-Z chars to avoid show FILE001 or DIR001. {mmanager}
    - Modify the palette usage for bitmap and normal OSD. {osd relative modules}
    - Add the functionality of speaker test. {setup}
    - Move the palette definitation from colordef.h to each module and change the starting entry number of the palette. {osd}
    - Support CDG detection and play with SERVO. {utl/infofilter/cdg}
    - Support Speaker Test BMP. {BMP txt files}
    - Support version information for related modules. {cc and all protected modules}
    - Support VCD20 auto-pause. {vcd20}

. Servo Fixed Items:
    - Implement timeout protection in SRVSV_OffsetAdjustment() to prevent from servo thread keeps sleepy.
    - Support data reliability information to parser.
    - Improve multiple buffer control.
    - Modify BMFUNC_SendInfoToParser(): for DVD, update __SF_SourceGBL[gbBMCurReqBuf].dwReadAddr aligned in block boundary.
    - Modify CMD_READ_CD control for initialization of data format change.

. DivX Fixed Items:
    - Resloved blocky effect problem during video clips transitions due to different frame sizes
    - The problem that PSNR results are incorrect at 2nd frame of 505b clip does not happen in new code.
    - Resolved wrong play order when SCAN back to normal PLAY
    - Added version control paramerters.

. Audio Fixed Items:
    - AC3 + karaoke sound is too small
    - AC3 + karaoke SPDIF Raw will 斷音 while playing 蟲蟲特攻隊 disk
    - The fisrt MP3 song will no sound while power on.
    - Mp3 water noise will play 11kHz sampling rate. 
    - MP3 spectrum line shows enhancement.
    - Analog PCM mute function.
           
. Left Issues:

