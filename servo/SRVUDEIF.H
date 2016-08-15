//===========================================================================
//         CheerTek CT909 Combo DVD-Player Servo Firmware
//
//         Filename    : srvudeif.h
//         Description : Definitions UDE interface
//===========================================================================
//---------------------------------------------------------------------------
#ifndef __SRVUDEIF_H__
//---------------------------------------------------------------------------
#define __SRVUDEIF_H__


//**********************************************
//		IC test define  
//***********************************************/
//#define		BM_FPGA_Only
#ifdef 		BM_FPGA_Only
//Add by Jessie Lin

#define		Wr_TEST	0x01
#define     Rd_TEST  0x02
/////////////////////////////////////////////////
#define		DISCTYPE	0x00
#define		DVD			0x00
#define		CD			0x01

////////////////////////////////////////////////
//Second level Define
///////////////////////////////////////////////
#define			READ_REG_ONLY
//#define		   	DVDmode
#define		   	Trigger_ECC_Test
#define        	COUNT_ERROR_TEST
#define			FOR_BM_FPGA
#define			BM_SYSTEM
//***********************************************
//		IC Test Define
//***********************************************
//Add by Jessie Lin for define IC test Function
//#define		REGISTER_TEST
//#define		DRAMTest
//#define		POWER_ON_TEST
//#define		Q_CODE_UPDATE_TACE_FUNCTION
//#define		CDDA_C3_TACE_FUNCTION

#define		BIU_EQUEL_DUMPIN_STOP_TEST
#define		DECODEICTEST
//#define		ENABLE_PI_ON_THE_FLY
//#define     ENABLE_CDDA_DUMPIN_RIGHTAWAY
//#define        ID_SOURCE_SELECT


#endif

//==========================================================================
// Firmware version setting
//==========================================================================
#define SRV_FIRMWARE_VERSION_MAIN    (277)
#define SRV_FIRMWARE_VERSION_MINOR   (1L)   // 0: none; 1: a; 2: b; 3: c; ...
#define SRV_FIRMWARE_VERSION_REV     (0L)   // 0: none; 1: a; 2: b; 3: c; ...
//#define SERVO_FIRMWARE_VERSION       ((SRV_FIRMWARE_VERSION_MAIN<<16)+(SRV_FIRMWARE_VERSION_MINOR<<8))
#define SERVO_FIRMWARE_VERSION       ((SRV_FIRMWARE_VERSION_MAIN<<16)+(SRV_FIRMWARE_VERSION_MINOR<<8) + SRV_FIRMWARE_VERSION_REV)
#define FIRMWARE_VERSION             (SERVO_FIRMWARE_VERSION)

//===========================================================================
//      FBCR/FTGR tuning relative setting
//===========================================================================
//===========================================================================
//      Media Format Type (Logical Viewpoint): gbDiscFormat
//===========================================================================
#define DISC_FORMAT_UNKNOW            0xFF
#define DISC_FORMAT_ERROR             0xF0

#define DISC_FORMAT_DVD_ROM           0x00
#define DISC_FORMAT_DVD_RAM           0x01
#define DISC_FORMAT_DVD_MINUS_R       0x02
#define DISC_FORMAT_DVD_MINUS_RW      0x03
#define DISC_FORMAT_DVD_PLUS_R        0x0A
#define DISC_FORMAT_DVD_PLUS_RW       0x09

//#define DISC_FORMAT_CDDA              0x80   // CD-DA
//#define DISC_FORMAT_MODE1             0x81   // CD Mode 1 (ex. CD-ROM)
//#define DISC_FORMAT_M2F1              0x82   // CD Mode 2 Form 1 (ex. VCD control data)
//#define DISC_FORMAT_M2F2              0x83   // CD Mode 2 Form 2 (ex. VCD AV data)

// Same value as MPEG's definition
#define DISC_FORMAT_VCD               0x94   // CD-VCD
#define DISC_FORMAT_CDI               0x8A   // CD-CDI
#define DISC_FORMAT_CDDA              0x84   // CD-CDDA

#define DISC_FORMAT_MODE1             0x81   // CD Mode 1 (ex. CD-ROM)
#define DISC_FORMAT_M2F1              0x82   // CD Mode 2 Form 1 (ex. VCD control data)
#define DISC_FORMAT_M2F2              0x83   // CD Mode 2 Form 2 (ex. VCD AV data)

//#define DISC_FORMAT_SACD              0xA0

//Cyyang1210
#define DISC_FORMAT                   0x01
#define START_BUFFER                  0x02
#define FLASH_END_BUFFER              0x04
#define INITIAL_BUFFER                0x08
#define EDC_BUFINITIAL 0x01
#define SECTORFORMAT   0x02
#define EDCRESULT_OK   0x03
#define EDCRESULT_FAIL 0x04
#define ABORT_DUMPIN   0x05
#define RIGHTAWAY_CDDA 0x06


//===========================================================================
//      Communication Command Definition
//===========================================================================
#define CMD_OPEN_TRAY                 0x01
#define CMD_CLOSE_TRAY                0x02
#define CMD_START_UNIT                0x03
#define CMD_CONFIGURE_EDC_ARRAY       0x04
#define CMD_CONFIGURE_BUFFER          0x05
#define CMD_READ_CD                   0x06
#define CMD_READ_DVD                  0x07
//#define CMD_READ_DVD_CONTROL_DATA     0x08
//#define CMD_READ_DVD_BCA              0x09
#define CMD_SEEK                      0x0A
#define CMD_GET_STATUS                0x0B
#define CMD_POWER_MANAGEMENT          0x0C
#define CMD_SERVO_DEBUG               0x0D
#ifdef AutoTuning_SERVO
 #define CMD_TEST_SHORT_SEEK         0x0E    //kenny 070417
#endif
//===========================================================================
//      Communication Command Check
//===========================================================================
#define CMD_CHECK_TRAY_OPEN           0x00000001
#define CMD_CHECK_TRAY_CLOSE          0x00000002
#define CMD_CHECK_DISC_EXIST          0x00000004
#define CMD_CHECK_SERVO_ON            0x00000008
#define CMD_CHECK_LEADIN_OK           0x00000010
#define CMD_CHECK_DISC_CD             0x00000020
#define CMD_CHECK_DISC_DVD            0x00000040
#define CMD_CHECK_PARA                0x00000080
#define CMD_CHECK_ID                  0x00000100

//********************************************************
//      GET_STATUS command
//********************************************************
#define TRAY_STATUS             0x07
#define TRAY_OPEN               0x00
#define TRAY_OPENING            0x01
#define TRAY_CLOSE              0x02
#define TRAY_CLOSING            0x03
#define USER_TRAY_IN            0x04
#define TRAY_OPEN_FAIL          0x05
#define TRAY_CLOSE_FAIL         0x06
#define TRAY_UNKNOWN            0x07   // power-on or close-tray initial value

#define SPINDLE_STATUS          0x08
#define SPINDLE_STOP            0x00
#define SPINDLE_ROTATE          0x08   // power-on or close-tray initial value

#define MEDIA_STATUS            0x30
#define MEDIA_ANALYZING         0x00   // power-on or close-tray initial value
#define MEDIA_PRESENT           0x10
#define MEDIA_NOT_PRESENT       0x20
#define MEDIA_UNKNOWN           0x30

//********************************************************
//      Result Codes
//********************************************************
#define NO_ERROR                        0x00
#define CMD_ABORT                       0x10
#define CMD_ERROR                       0x20
#define CMD_ERROR_UNDEFINED_PARAMETER   0x20
#define CMD_ERROR_SRV_NOT_READY         0x21
#define CMD_ERROR_NO_DISC               0x22
#define CMD_ERROR_UNKNOWN_DISC          0x23
#define CMD_ERROR_WRONG_DISC_TYPE       0x24
#define CMD_ERROR_WRONG_TRAY_STATUS     0x25
#define CMD_ERROR_WRONG_START_ADDR      0x26
#define CMD_ERROR_WRONG_STOP_ADDR       0x27
#define CMD_ERROR_WRONG_BUFFER_NUMBER   0x28

//===========================================================================
//      Servo machine definitions
//===========================================================================
#define WAIT_NEWCMD              0xFF    // set this value after result code returned

#define NEW_CMD_PACKET_LENGTH      12
#define RET_PACKET_LENGTH          12

#define MAX_TOC_ENTRY              50

#define READY                       0  // active low
#define BUSY                        1

//===========================================================================
//      Servo Command Definition
//===========================================================================
#define SRV_OPEN_TRAY           0x00
#define SRV_CLOSE_TRAY          0x01
#define SRV_START_UNIT          0x02
#define SRV_STOP_UNIT           0x03
#define SRV_SEEK                0x04
#define SRV_JUMP_TRACK          0x05
#define SRV_WAKEUP              0x06
#define SRV_DYNAMIC_TUNING      0x07

#ifdef SUPPORT_DYNAMIC_TUNING
#define SRV_KK                  0x08
#endif

//===========================================================================
//      Servo Command Error Code
//===========================================================================
//#define NO_ERROR              0x00
#define SRV_BUSY                0xFF
#define SRV_ERROR               0x10
#define SRV_ABORTED             0x20
#define SRV_FAIL                0x30

//===========================================================================
//      Buffer Management Command Definition
//===========================================================================
#define BMSTATE_OPEN_TRAY               0x00
#define BMSTATE_CLOSE_TRAY              0x01
#define BMSTATE_START_UNIT              0x02
#define BMSTATE_READ_LEADIN             0x03
#define BMSTATE_DUMPIN_PREPARE          0x04
#define BMSTATE_DUMPIN_TRIGGER          0x05
#define BMSTATE_DUMPOUT_PREPARE         0x06
#define BMSTATE_CD_OUTPUT_MONITOR       0x07
#define BMSTATE_SEND_RESULT_CODE        0x08
#define BMSTATE_CLV_SCALE               0x09
#define BMSTATE_SEEK_CONTROL            0x0A
#define BMSTATE_JUMP_CONTROL            0x0B
#define BMSTATE_WAKEUP_SEEK             0x0C
#define BMSTATE_DEBUG_PI_PO             0x0D    // Debug Mode
#define BMSTATE_POWER_MANAGEMENT        0x0E
 #ifdef BM_MONITOR_PI_PO
#define BMSTATE_MONITOR_PI_PO           0x0F
 #endif

#define BMSTATE_STANDBY                 0x10

 #ifdef RS232F_SEEK_TUNING
#define BMSTATE_TEST_SERVO              0x11 
 #endif

 #ifdef AutoTuning_SERVO
#define BMSTATE_TEST_SHORTSEEK			0x12    //kenny 070417
 #endif
//===========================================================================
//      Timer for each state
//===========================================================================
//#define BMTIMER_OPEN_TRAY                 (60   / CPU_TIME_GAP)
//#define BMTIMER_CLOSE_TRAY                (60   / CPU_TIME_GAP)
#define BMTIMER_CLV_SCALE                 (1000 / CPU_TIME_GAP)   // 3800 ms
//#define BMTIMER_CHECK_BUFFER              (60   / CPU_TIME_GAP)   //   60 ms, MAX 4 block Data required at 2x
#define BMTIMER_CHECK_BUFFER              (500   / CPU_TIME_GAP)   // Andy test  500 ms, MAX 4 block Data required at 2x

//#define BMTIMER_DUMPING_TRIGGER           ((3000 / CPU_TIME_GAP))   // 5000  //3000 Eric100402  // 1000 ms, !!!Must be less!!!
#define BMTIMER_DUMPING_TRIGGER           ((4000 / CPU_TIME_GAP))   // 5000  //3000 Eric100402  // 1000 ms, !!!Must be less!!!
#define BMTIMER_AUTO_DUMPING_TRIGGER      ((1000 / CPU_TIME_GAP))   //   60 ms, !!!Must be less!!!
#define BMTIMER_READ_LEADIN_TOC           ((4500 / CPU_TIME_GAP))  //4000/
#if 1 //def AUTO_PAUSE
 #define BMTIMER_WAIT_PAUSE_TIMER          ((3000 / CPU_TIME_GAP))   // 3000 ms
 #define BMTIMER_PAUSE_SEEK                ((1000 / CPU_TIME_GAP))
#endif
#define BMTIMER_CDDA_WIAT_FOR_START        (200  / CPU_TIME_GAP)   // 100 ms

#define BMTIMER_CDDA_WIAT_FOR_STOP       (500  / CPU_TIME_GAP)

//===========================================================================
//      eCos Flag Definition, map of gdwCMDBMPattern and gdwCMDBMFlag
//===========================================================================
#define SRV_OS_CMD_BM_ALL_FLAG           0x00000FFF //0x000007FF

#define SRV_OS_HOST2CMD_NEW_CMD          0x00000001   // host issues new command
#define SRV_OS_HOST2CMD_ABORT_CMD        0x00000002   // host aborts current command

#define SRV_OS_INT2BM_SECTOR_ID_READY    0x00000004   // demodulator sector ID is saved
#define SRV_OS_INT2BM_DIN                0x00000008   // prepare next dump-in process
#define SRV_OS_INT2BM_LEAD_IN_READY      0x00000010   // lead-in or TOC is ready to read
#define SRV_OS_INT2BM_TIMEOUT            0x00000020   // BM timeout
#define SRV_OS_INT2BM_LOCAL_TIMEOUT      0x00000040   // BM local timeout
#define SRV_OS_INT2BM_ECC_TIMEOUT        0x00000080   // ECC decode timeout
#define SRV_OS_INT2CMD                   0x00000100   // 
#define SRV_OS_INT2CMD_1ST_SECTOR_READY  0x00000200   // 1st requested sector is in buffer
#define REQUEST_CDG_DECODE               0x00000800   // Request CDG decode
#define SRV_OS_SERVO2BM_CMD_FINISH       0x00000400   // servo completes gbServoCMD

//Cyyang1112:Send to host flag 
#define SRV_OS_BM2HOST_CMD_FINISH        0x00001000   //BM finishes HOST cammand
#define SRV_OS_BM2HOST_ABORT_FINISH      0x00002000   //BM aborts HOST commamd
#define SRV_OS_BM2HOST_AWAKE_PARSER      0x00004000   //BM wake up PASER queue.
//Cyyang1122

#define SRV_OS_BM2SERVO_CMD              0x00800000   // BM issues gbServoCMD to servo

//----servo ecos flag----
#define SRV_OS_SERVO_ALL_FLAG          0xFFFF0000
#define SRV_OS_DSSP_INT                0x00010000
#define SRV_OS_SERVO_TIMER0_L          0x00020000
#define SRV_OS_SERVO_TIMER0_G          0x00040000
#define SRV_OS_SERVO_TIMER1            0x00080000
#define SRV_OS_DSSP_FNOK               0x00100000
#define SRV_OS_MAX_SPD_TIMER           0x00200000
#define SRV_OS_PSEUDO_FLAG             0x80000000

 #ifdef SUPPORT_PSNR_TEST
#define SRV_OS_PSNR_SERVO_IDLE         0x00400000
#define SRV_OS_PSNR_REL_SERVO_IDLE     0x00800000
 #endif
//----------------------

//===========================================================================
//      BM Base Address for DRAM Access
//===========================================================================
#define SRV_DRAM_DIN_BASE_ADDR         0x00000000
#define SRV_DRAM_ECC_BASE_ADDR         0x40000000
#define SRV_DRAM_USER_BASE_ADDR        0xC0000000   // Andy 050302: for CT909S non-cache DRAM access

//===========================================================================
//      BM Buffer Underrun Threshold, unit: sector, must NOT extend servo buffer size
//===========================================================================
#define SRV_BM_UNDERRUN_THRESHOLD_DVD  0x00000040
#define SRV_BM_UNDERRUN_THRESHOLD_CD   0x00000030

//===========================================================================
//      BM Sector ID Byte Mapping, for union SCT
//===========================================================================
#define ID0                            0
#define ID1                            1
#define ID2                            2
#define ID3                            3

//===========================================================================
//      Miscellaneous
//===========================================================================
#define SRV_BM_MAX_BUFFER_NUMBER       2                          // number of buffers
#define SRV_BM_BAK_BUFFER_NUMBER       SRV_BM_MAX_BUFFER_NUMBER   // buffer number for backup pointers
#define SRV_BM_TOTAL_BUFFER            (SRV_BM_MAX_BUFFER_NUMBER + 1)
#define SRV_BM_DVD                     0   // BM in DVD mode
#define SRV_BM_CD                      1   // BM in CD mode

//===========================================================================
//      C2 OFFSET DEFINITION (only for CD-DA and CD-ROM mode 2 form 2)
//===========================================================================
#define SRV_BM_C2_OFFSET               2

//===========================================================================
//      ERROR SEEK DEFINITION
//===========================================================================
#define  ERR_DUMPIN_SEEK              0x11
#define  ERR_DECODE_SEEK              0x21  
#define  ERR_JUMPFORWARD              0x41  
#ifdef STOP_DUMPIN_IN_LEADOUT  // When OPU in leadout area, stop dumpin and let the OPU move in, CYYang 1.01, 20050923
#define  ERR_SCAN_LEADOUT             0x81 
#endif
#define  ERR_CD_IDJUMP                0x81
#ifdef SUPPORT_DYNAMIC_TUNING
#define ERR_REQ_CALIBRATION           0x81
#endif


//===========================================================================
//      ERROR DUMPIN DEFINITION in aParserEDCInfo[][]
//===========================================================================
#define  DIN_ERR_SECTOR            0x8000   // this sector is erroneous
#define  DIN_NOT_AVAILABLE         0x4000   // this sector is not available (not dumped in buffer)

//===========================================================================
//      CD-G Control Definition
//===========================================================================
#ifdef CDG_ENABLE
#define CDG_SOURCE_NULL 0x00  //not enable cdg 
#define CDG_SOURCE_INT  0x01  //enable by interrupt
#define CDG_SOURCE_FUN  0x02  //enable by myself.        
#endif

//===========================================================================
//      CDDA dumpin rightaway Definition
//===========================================================================
#define CDDA_DUMPIN_RIGHTAWAY_ENABLE     0x01  
#ifdef ENABLE_CDDA_DUMPIN_RIGHTAWAY
#define CDDA_DUMPIN_RIGHTAWAY_PREPARE    0x00  
//#define CDDA_DUMPIN_RIGHTAWAY_ENABLE     0x01  
#define CDDA_DUMPIN_RIGHTAWAY_GETSUBQ    0x02  
#define CDDA_DUMPIN_RIGHTAWAY_POLLQ      0x03
#define CDDA_DUMPIN_RIGHTAWAY_FAILQ      0x04
#define CDDA_DUMPIN_RIGHTAWAY_TRAGETQ    0x05
#define CDDA_DUMPIN_RIGHTAWAY_SUCCESS    0x06    
#endif

#ifdef SUPPORT_DYNAMIC_TUNING
#define MAX_COUNT_VALUE_DVD                50
#define MAX_COUNT_VALUE_CD                 50
#define DEC_ERR_COUNT_VAULE_DVD            30
#define DEC_ERR_COUNT_VALUE_CD              5
#endif

#define BM_REQUIRE_OPEN  0x20
#define BM_STOP_OPEN     0x40
#define  PUSH_TRAY_IN_DEBOUNCE_COUNT 0x05

#define POWERUP_ALL 0xA6
#define POWERDOWN_ALL 0xA5
#define POWERDOWN_CD 0x01
#define POWERDOWN_DVD 0x02
#define POWERDOWN_DECODE 0x04
#define POWERUP_DECODE 0x05
#define ENABLE_SAVE_POWER (0x0640)




//---------------------------------------------------------------------------
#endif  // __SRVUDEIF_H__
//-------------------------------- End --------------------------------------
