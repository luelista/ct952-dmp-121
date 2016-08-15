//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2004,
//          D300, all right reserved.
//
//      Product : CT909
//
//      Date    : 2004.05.03
//      Author  : Cheertek (D300 Chuan)
//      Purpose :
//      Sources :
//  **************************************************************************

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

//#define SUPPORT_PROC2_DBG
#define DBG_DELAY_TIME          20000

#define DEBUG_AVSYNC

// For PROC2 Used
#define REG_DBG_R_IDX       REG_AIU_GR(28)
#define REG_DBG_W_IDX       REG_AIU_GR(30)
#define REG_DBG_PROC2FLAG   REG_AIU_GR(29)

#define DBG_CHAR_LENGTH     90

//For displaying auto-upgrade strings.
#define DBG_AUTOUPGRADE_STRING_START_H  80
#define DBG_AUTOUPGRADE_STRING_START_V  200

typedef struct  tagDBG_INFO
{
    DWORD   dwTickTime;
    BYTE    bThreadID;
    BYTE    bSize;
    BYTE    bData[DBG_CHAR_LENGTH];
} DBG_INFO, *PDBG_INFO;

// MPEG General Debug Information: Bit[7:0]
#define DBG_INFO_PRINTF             (0x00000001)    // BIT[0]
#define DBG_INFO_AVREM              (0x00000002)    // BIT[1]
#define DBG_INFO_PROC2_PC           (0x00000004)    // BIT[2]
#define DBG_INFO_SPREM              (0x00000008)    // BIT[3]
#define DBG_INFO_PCMREM             (0x00000010)    // BIT[4]
#define DBG_INFO_JPEGDEC            (0x00000020)    // BIT[5]
#define DBG_INFO_MPEG               (0x00000020)    // BIT[5]
#define DBG_INFO_MPEG4              (0x00000020)    // BIT[5]
#define DBG_INFO_PARSER             (0x00000040)    // BIT[6]
#define DBG_INFO_MM_INFOFTR         (0x00000080)    // BIT[7]

#define DBG_INFO_REVERSED_1         (0x00000000)

// SERVO General Debug Information: Bit[15:8]
// Andy 050131
#define DBG_INFO_REVERSED_2         (0x00002000)    // BIT[13]
#define DBG_INFO_SRVINFO            (0x00000100)    // BIT[08]
#define DBG_INFO_SRVCMD             (0x00000200)    // BIT[09]
#define DBG_INFO_SRVBM              (0x00000400)    // BIT[10]
#define DBG_INFO_SRVBMERR           (0x00000800)    // BIT[11]
#define DBG_INFO_SRVBMERRRATE       (0x00001000)    // BIT[12]
#define DBG_INFO_SRVSERVOERR        (0x00004000)    // BIT[14]
#define DBG_INFO_SRVSERVO           (0x00008000)    // BIT[15]

// User Define Debug Information: Bit[31:16]
#define DBG_INFO_VIDEOSYNC          (0x40000000)    // BIT[30]
#define DBG_INFO_AVSYNC             (0x00010000)    // BIT[16]
#define DBG_INFO_PTS_MONITOR        (0x00020000)    // BIT[17]
#define DBG_INFO_PRINT_KEY          (0x00040000)    // BIT[18]
#define DBG_INFO_USB_REQUEST        (0x00080000)    // BIT[19]
#define DBG_INFO_USB_STATUS         (0x00100000)    // BIT[20]
#define DBG_INFO_KEY_FLOW           (0x00200000)    // BIT[21]
#define DBG_INFO_NV_FLOW            (0x00400000)    // BIT[22]
#define DBG_INFO_PGC_FLOW           (0x00800000)    // BIT[23]
#define DBG_INFO_CELL_FLOW          (0x01000000)    // BIT[24]

//DVB
#define DBG_INFO_DVB_USRIF          (0x02000000)    // BIT[25]
#define DBG_INFO_DVB_NIM            (0x04000000)    // BIT[26]
#define DBG_INFO_DVB_SYS            (0x08000000)    // BIT[27]
#define DBG_INFO_DVB_TTXT           (0x10000000)    // BIT[28]
#define DBG_INFO_DVB_DB             (0x20000000)    // BIT[29]

// For Error informtaion: use this to output error inform which indicate the "Must check condition"
#define DBG_INFO_ASSERTION          (0x80000000)    // BIT[31]

#define DBG_INFO_NULL               (0x00L)

#ifdef SUPPORT_PROC2_DBG
#define DS_DBG_SIZE                 ((DS_DEBUG_BUF_END - DS_DEBUG_BUF_ST)/2)
#else
#define DS_DBG_SIZE                 (DS_DEBUG_BUF_END - DS_DEBUG_BUF_ST)
#endif
#define DS_DBG_PROC1_AREA_ST        (DS_DEBUG_BUF_ST)
#define DS_DBG_PROC1_AREA_END       (DS_DBG_PROC1_AREA_ST + DS_DBG_SIZE)

#define DBG_MAX_IDX                 ((DS_DBG_PROC1_AREA_END - DS_DBG_PROC1_AREA_ST)/sizeof(DBG_INFO))

#ifdef SUPPORT_PROC2_DBG
#define DS_DBG_PROC2_AREA_ST        (DS_DBG_PROC1_AREA_END + 0x8)
#define DS_DBG_PROC2_AREA_END       (DS_DEBUG_BUF_END - 0x8)
#define DBG_MAX_IDX2                ((DS_DBG_PROC2_AREA_END - DS_DBG_PROC2_AREA_ST)/sizeof(DBG_INFO))
#endif

// Thread ID
#define DBG_THREAD_ATAPI            (0)
#define DBG_THREAD_PARSER           (1)
#define DBG_THREAD_DECODER          (2)
#define DBG_THREAD_SERVO            (3)
#define DBG_THREAD_INFO_FILTER      (4)
#define DBG_THREAD_MONITOR          (5)
#define DBG_THREAD_CHEERDVD         (6)
#define DBG_ISR                     (7)
#define DBG_DSR                     (8)
#define DBG_DEBUG                   (9)
// wyc1.01-909
#define DBG_THREAD_USB              (10)
#define DBG_PROC2                   (11)
#define DBG_THREAD_NULL             (0xFF)

#define DBG_PRIORITY_HIGH           (0x1000)

#define DBG_THREAD_ATAPI_HI         (DBG_THREAD_ATAPI|DBG_PRIORITY_HIGH)
#define DBG_THREAD_PARSER_HI        (DBG_THREAD_PARSER|DBG_PRIORITY_HIGH)
#define DBG_THREAD_DECODER_HI       (DBG_THREAD_DECODER|DBG_PRIORITY_HIGH)
#define DBG_THREAD_SERVO_HI         (DBG_THREAD_SERVO|DBG_PRIORITY_HIGH)
#define DBG_THREAD_INFO_FILTER_HI   (DBG_THREAD_INFO_FILTER|DBG_PRIORITY_HIGH)
#define DBG_THREAD_MONITOR_HI       (DBG_THREAD_MONITOR|DBG_PRIORITY_HIGH)
#define DBG_THREAD_CHEERDVD_HI      (DBG_THREAD_CHEERDVD|DBG_PRIORITY_HIGH)
#define DBG_INT_HI                  (DBG_DEBUG|DBG_PRIORITY_HIGH)
#define DBG_DEBUG_HI                (DBG_DEBUG|DBG_PRIORITY_HIGH)
#define DBG_ISR_HI                  (DBG_ISR|DBG_PRIORITY_HIGH)
#define DBG_DSR_HI                  (DBG_DSR|DBG_PRIORITY_HIGH)

//The following defines are command-relative usage for Serial Debug Tool
#define DBG_RX_COMMAND_SIZE         30
#define DBG_RX_DATA_SIZE            8

//[Define commands to communicate with "Serial Debug Tool"]
//Note: Don't use the following values because of the UDE command
//49M Hz: 0xFE, 0x1E, 0xFC, 0x1C, 0xE0, 0, 0x7E, 0xE
//33M Hz: 0x7F, 0x3C, 0, 0xF8, 0xF0, 0x7, 0xE0, 0xFC, 0xFF, 0xC0

//Write data
#define WRITE_DATA_CMD                  0x81 //PC request 8051 to write data to flash
#define ACK_WRITE_DATA_CMD              0x82 //8051 ack receive WRITE_DATA_CMD to PC
#define ERROR_WRITE_DATA_CMD            0x83 //8051 error receive WRITE_DATA_CMD from PC

//Joey2.60: Support Vipor Tunning Tools
//Sequence Read/Write DRAM
#define START_SEQ_RW_DRAM_CMD           0x84
#define ACK_START_SEQ_RW_DRAM_CMD       0x85
#define ERROR_START_SEQ_RW_DRAM_CMD     0x86

#define END_SEQ_RW_DRAM_CMD             0x87
#define ACK_END_SEQ_RW_DRAM_CMD         0x88
#define ERROR_END_SEQ_RW_DRAM_CMD       0x89

#define SEQ_READ_DRAM_SUBCMD            0x8a
#define ACK_SEQ_READ_DRAM_SUBCMD        0x8b
#define ERROR_SEQ_READ_DRAM_SUBCMD      0x8c

#define SEQ_WRITE_DRAM_SUBCMD           0x8d
#define ACK_SEQ_WRITE_DRAM_SUBCMD       0x8e
#define ERROR_SEQ_WRITE_DRAM_SUBCMD     0x8f

//Read DRAM data
#define READ_DRAM_CMD                   0x91
#define ACK_READ_DRAM_CMD               0x92
#define ERROR_READ_DRAM_CMD             0x93

//Write DRAM data
#define WRITE_DRAM_CMD                  0x94
#define ACK_WRITE_DRAM_CMD              0x95
#define ACK_COMPLETE_WRITE_DRAM         0x96
#define ERROR_WRITE_DRAM_CMD            0x97

//download bin file data
#define ACK_RECEIVE_ROM_DATA            0xA0
#define ACK_RECEIVE_WRONG_DATA          0xA1

//Auto Upgrade
#define AUTO_UPGRADE_CMD                0xB0
#define PREPARE_AUTO_UPGRADE_CMD        0xB1
#define ACK_PREPARE_AUTO_UPGRADE_CMD    0xB2
#define ERROR_PREPARE_AUTO_UPGRADE_CMD  0xB3
#define EXIT_AUTO_UPGRADE_CMD           0xB4
#define ACK_EXIT_AUTO_UPGRADE_CMD       0xB5
#define ERROR_EXIT_AUTO_UPGRADE_CMD     0xB6
#define AUTO_UPGRADE_CODE_ADDRESS       0xB7
#define ACK_AUTO_UPGRADE_CODE_ADDRESS   0xB8
#define ERROR_AUTO_UPGRADE_CODE_ADDRESS 0xB9

//Mechanism for text command.
#define START_TEXT_MODE_CMD             0xE3
#define ACK_START_TEXT_MODE_CMD         0xE4
#define ACK_TEXT_MODE_CMD_LOST          0xE5

//Notify Serial Debug Tool that the system reboots.
#define RESYNC_DEBUG_LEVEL              0xC1

//Notify Serial Debug Tool to save the DRAM data to a file.
#define START_DUMP_DRAM_DATA            0xD1
#define END_DUMP_DRAM_DATA              0xD2
#define DUMP_DRAM_START_ADDRESS         0xD3
#define DUMP_DRAM_END_ADDRESS           0xD4
#define DUMP_DRAM_FILE_NAME             0xD5
#define DUMP_START_DRAM_DATA_TRANSFER   0xD6

//Binary mode command.
#define START_BIN_MODE_CMD              0xF1
#define START_BIN_MODE_ACK              0xF2
#define ERROR_START_BIN_MODE            0xF3
#define END_BIN_MODE_CMD                0xF4
#define END_BIN_MODE_ACK                0xF5
#define ERROR_END_BIN_MODE              0xF6
#define ERROR_RECOVER_CMD               0xFB
#define END_CMD                         0xFF     //PC end sending command to 8051

//The definition for communication
#define SERIAL_DEBUG_READ_DRAM      0
#define SERIAL_DEBUG_READ_DM        1
#define SERIAL_DEBUG_WRITE_DRAM     2
#define SERIAL_DEBUG_WRITE_DM       3

#define SERIAL_DEBUG_OPERATE_NONE   0
#define SERIAL_DEBUG_OPERATE_AND    1
#define SERIAL_DEBUG_OPERATE_OR     2

#define SERIAL_DEBUG_FAIL           0
#define SERIAL_DEBUG_OK             1
#define SERIAL_DEBUG_CANCEL         2

#define SERIAL_DEBUG_SIZE_BYTE      1
#define SERIAL_DEBUG_SIZE_WORD      2
#define SERIAL_DEBUG_SIZE_DWORD     4

#define ROW_SIZE                    256

//Define the value for the command has "SPACE"
#define DBG_COMMAND_SPACE           0x0100
#define DBG_EMPTY_DATA_IN_BUFFER    0

//extern cyg_handle_t     __AlarmHandle;
//extern cyg_alarm        __AlarmObj;
extern DWORD            __dwDebugFlag;
extern BYTE _bAutoUpgrade;

// Export functions
BOOL    DBG_Printf(DWORD dwThreadID, DWORD dwDBGType, char *fmt, ...);
VOID    DBG_Init(DWORD dwSysCLK, DWORD dwBaudRate, DWORD dwUartPort, DWORD dwDBGMode);
VOID    DBG_INT(VOID);
VOID    DBG_Polling(VOID);
VOID    DBG_OutputDRAMData(DWORD dwStartAddress, DWORD dwEndAddress, BYTE bFileName);
VOID    DBG_Disable(VOID);


#ifdef __cplusplus
        }
#endif

#endif //__DEBUG_H__
