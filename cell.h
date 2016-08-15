#ifndef     __CELL_H__
#define     __CELL_H__

#ifdef __cplusplus
   extern "C"{
#endif

// DVD_160Micky
// reference VI3-66, once transferred to Action state, any transfer shall not allowed
#define CELL_BUTTON_SELECTION   1
#define CELL_BUTTON_ACTION      2
#define CELL_BUTTON_NONE        3


#define CELL_SEARCH_1ST_NV  1   // must linear search nv (one by one sector)
#define CELL_LINEAR         2   // play cell as linear mode
#define CELL_NORMAL         3   // cell normal playback mode, ie. NV -> VOBU -> NV -> VOBU -> ...
#define CELL_SKIP_VOBU      4   // search nv by jump (skip VOBU)

//#define CELL_FORCE_LINEAR_PLAY_TEST
#ifdef CELL_FORCE_LINEAR_PLAY_TEST
#define CELL_LINEAR_NPF_OK          0x01
#define CELL_LINEAR_NPF_RUN         0x02
#define CELL_LINEAR_NPF_UPDATE_STC  0x04
#endif // CELL_FORCE_LINEAR_PLAY_TEST

// the skip levels to VOBOSRI
#define CELL_SKIP_SECTORS   6000

extern BYTE     __bCellState;
extern BYTE    _bNVRecover;
extern DWORD    __dwCellLVOBUEA;
extern  DWORD   __dwVOBUStart;
extern  BYTE    __bCellAction;
// LLY2.17, use a variable to keep NV buffer start address
// Since, the address maybe different base on different DRAM configuration.
// So, replace DS_NV_BUF_ADDR_ST w/ __dwNVStart
extern  DWORD   __dwNVStart;


// DVD_103Micky, reduce DSI
// will read DSI data only when needed
// read DSI_GI to SML_PBI's ILVU_EA
// SML_AGLI
// VOBU_SRI
// which might be affected are:
// 1) Detect number of angles(seamless angle change)
// 2) VOBU range
// 3) ILVU range
// 4) Angle change(nom-seamless angle change)
// 5) Time search
// 6) Scan
#define REDUCE_DSI

// PCI has 979 bytes, DSI_BASE will point to PCI RECI's last 3 bytes
// Micky0.65-909, use bytes unit.
// Micky0.66-909, DSI aligh at 64bits bouyndary(MCU request)
#define	PCI_BYTE		        (979)
#define	DSI_ALIGN_BYTE		    (5)//8-(PCI_BYTE%8))

#define DSI_BASE                (__dwNVStart + + PCI_BYTE + DSI_ALIGN_BYTE)

// first byte of DSI_GI will locate at 3 third byte of the 1st dword
// read 11 dwords a time for following data
#define DSI_GI_BASE             DSI_BASE
// DVD_156Micky, add reading PCK_LBN
#define DSI_GI_NV_PCK_LBN       (4)
#define DSI_GI_VOBUEA_OFFSET    (8)   // offset to DSI_GI
#define DSI_SML_PBI_VOBU_CAT    (32)  // offset to DSI_GI
#define DSI_SML_PBI_ILVU_EA     (34)  // offset to DSI_GI
#define DSI_SML_PBI_ILVU_SA     (38)  // offset to DSI_GI
#define	DSI_GI_BYTE			    (32)
// all DSI_GI=32 bytes, VOBUSML_CAT+ILVU_EA+ILVU_SA=10 bytes
#define DSI_GI_AND_SML_PBI_BYTE (DSI_GI_BYTE + 10 )
// 1st SML_AGLI base address

// Micky0.65, bytes offset

#define	SML_PBI_BYTE		    (148)
#define	SML_AGLI_BYTE		    (54)
#define DSI_SMLAGLI_BASE        (DSI_BASE + DSI_GI_BYTE + SML_PBI_BYTE)

// 1st VOBUSRI base address
// Micky0.65-909, use bytes unit.
#define DSI_VOBUSRI_BASE        (DSI_BASE + DSI_GI_BYTE + SML_PBI_BYTE + SML_AGLI_BYTE)  // 180+SML_AGLI=234


//DVD_045q-1, NV reduce
//#define  PCI_1_TIME
#define PCI_BASE                (__dwNVStart + 0)
#define	NSML_AGLI_BYTE		    (36)
#define	PCI_HLI_BASE		    (PCI_BASE + PCI_GI_BYTE + NSML_AGLI_BYTE )
#define PCI_VOBU_CAT            (PCI_BASE + 4)
#define PCI_VOBU_UOP_CTL        (PCI_BASE + 8)
// DVD_103Micky, add reading VOBU_S_PTM as 1st STC time.
#define PCI_VOBU_S_PTM          (PCI_BASE + 12)
// Micky0.65-909, use bytes unit
#define PCI_VOBU_E_PTM          (PCI_BASE + 16)
#define PCI_C_ELTM              (__dwNVStart + 24)

// Micky0.65-909, use bytes unit
#define	PCI_GI_BYTE		        (60)
#define PCI_NSML_AGLI           (__dwNVStart + PCI_GI_BYTE)

#define	HL_GI_BYTE		        (22)
#define	BTN_COLIT_BYTE	        (24)
#define	PCI_HLI_BTNIT_BASE	    (PCI_HLI_BASE + HL_GI_BYTE + BTN_COLIT_BYTE)

//DVD_045
// Refer to pge VI 4-115
#ifdef  PCI_1_TIME
#define PCI_OFFSET   96
#define APS_OFFSET  4
#define NSML_OFFSET 60
#else
#define PCI_OFFSET   0
#define APS_OFFSET  0
#define NSML_OFFSET 0
#endif

#define HL_GI_HLI_SS0   0       //2 bytes
#define HL_GI_HLI_SS1   1
#define HL_GI_S_PTM_0     2       //4 bytes
#define HL_GI_S_PTM_1     3
#define HL_GI_S_PTM_2     4
#define HL_GI_S_PTM_3     5
#define HL_GI_E_PTM_0     6       // 4 bytes
#define UOP_CAN_PTT_PLAY                0x00000002L     //UOP1, bit1    // ** TCH0.37;
#define HL_GI_E_PTM_1     7
#define HL_GI_E_PTM_2     8
#define HL_GI_E_PTM_3     9
#define HL_GI_BTN_SL_E_PTM_0  10  // 4 bytes
#define HL_GI_BTN_SL_E_PTM_1  11
#define HL_GI_BTN_SL_E_PTM_2  12
#define HL_GI_BTN_SL_E_PTM_3  13
#define HL_GI_BTN_MD_0      14  // 2 bytes
#define HL_GI_BTN_MD_1      15
#define HL_GI_BTN_OFN       16  // 1 byte
#define HL_GI_BTN_Ns        17  // 1 byte
#define HL_GI_NSL_BTN_Ns    18  // 1 byte
#define HL_GI_FOSL_BTNN     20  // 1 byte
#define HL_GI_FOAC_BTNN     21  // 1 byte


// DVD_103Micky, record last played VOBUSA that has video data
// for resume to play w/ video data
extern  DWORD   __dwCellVideo_VOBUSA;

extern  BYTE    __bBTNBaseOffset;       // BTN offset
extern  BYTE    __bSELBTN_Ns;           // number of selectable buttons
extern  BYTE    __bBTN_Ns;              // number of total buttons
extern  BYTE    __bDefaultHL_BTNN;      // default selection button number
extern  BYTE    __bForceActiveBTNN;     // forced active button number
extern  DWORD   __dwHLStartTime;        // button highlight start time
extern  DWORD   __dwHLEndTime;          // buttom highlight end time
extern  DWORD   __dwBTNSelEndTime;      // end of buttion selection time
extern  BYTE    __bNowBTNNumber;      // current selection BTNN
extern  BYTE    __bCellActiveBTN;

// states
#define PGC_STATE           1
#define CELL_STATE          2
#define NV_COMMAND_STATE    3

// DVD_171-2Micky, since 1.70, cell don't need do angle search.
/*
// DVD_100bMicky, define angle search ID
#define CELL_ANGLE_SEARCH_NONE      0
#define CELL_ANGLE_SEARCH_START     1
#define CELL_ANGLE_SEARCH_DONE      2
*/

//DVD_039-1
// re-order the CELL stages
#define CELL_NOACTION       0
#define VOBU_START          1
#define VOBU_PLAYING        2
#define VOBU_TIME           3
#define VOBU_STILL          4
#define VOBU_END            5
#define ILVU_END            6
#define VOBU_NEXT           7
// LLY2.32, porting Micky's code for using time map table
// Micky07312006 begin, for TMAP
#define	TIME_SEARCH			8


// DVD_099Micky, adjust the ID define
// !!! don't modify the sequence
//DVD_040-1
// add CELL_LAST_VOBU state
// for processing VOBU PTM control
#define CELL_LAST_VOBU      11
#define CELL_STILL          12
#define CELL_CHECK_FORCEACTIVE  13
#define CELL_END            14

// Ming0.75, Add a new state to wait the source filter has completed NV sector sending.
#define VOBU_WAIT_NV_SEND_COMPLETE  15

#define VOBU_PREU   0x80
#define VOBU_ILVU   0x40

// define the VOBU_UOP capability
//DVD_099Micky, modify the UOP_CAN bit, reference Annex J(VIX-62)
#define UOP_CAN_TIME_PLAY               0x00000001L     //UOP0, bit0    // ** Micky0.37;
#define UOP_CAN_UOP0                    0x00000001L
#define UOP_CAN_PTT_PLAY                0x00000002L     //UOP1, bit1    // ** Micky0.37;
#define UOP_CAN_UOP1                    0x00000002L
#define UOP_CAN_TITLE_PLAY              0x00000004L     //UOP2, bit2    // ** Micky0.37;

#define UOP_CAN_STOP                    0x00000008L     //UOP3, bit3
#define UOP_CAN_GOUP                    0x00000010L     //UOP4, bit4

//DVD_099Micky, define TimeSearch/PTTSearch query as combination
#define UOP_CAN_UOP5                    0x00000020L     //UOP5, Time/PTT Search
#define UOP_CAN_TIME_SEARCH             0x00000021L     //UOP5, bit5|bit 0
#define UOP_CAN_PTT_SEARCH              0x00000022L     //UOP5, bit5|bit 1

#define UOP_CAN_PREVPG_SEARCH           0x00000040L     //UOP6, bit6   // ** TCH0.39; "pre"-> "prev"
#define UOP_CAN_TOPPG_SEARCH            0x00000040L     //UOP6, bit6
#define UOP_CAN_NEXTPG_SEARCH           0x00000080L     //UOP7, bit7
#define UOP_CAN_FORWARD_SCAN            0x00000100L     //UOP8, bit8
#define UOP_CAN_BACKWARD_SCAN           0x00000200L     //UOP9, bit9
#define UOP_CAN_TITLE_MENU              0x00000400L     //UOP10, bit10
#define UOP_CAN_ROOT_MENU               0x00000800L     //UOP11, bit11
#define UOP_CAN_SUBPICTURE_MENU         0x00001000L     //UOP12, bit12
#define UOP_CAN_AUDIO_MENU              0x00002000L     //UOP13, bit13
#define UOP_CAN_ANGLE_MENU              0x00004000L     //UOP14, bit14
#define UOP_CAN_PTT_MENU                0x00008000L     //UOP15, bit15
#define UOP_CAN_RESUME                  0x00010000L     //UOP16, bit16
//DVD_051Micky
//add UOP check
#define UOP_CAN_BUTTON                  0x00020000L     //UOP17, bit 17
#define UOP_CAN_STILL_OFF               0x00040000L     //UOP18, bit18
#define UOP_CAN_PAUSE_ON                0x00080000L     //UOP19, bit19
#define UOP_CAN_CHANGE_AUDIO_STREAM     0x00100000L     //UOP20, bit20
#define UOP_CAN_CHANGE_SUBPICTURE_STREAM 0x00200000L     //UOP21, bit21 // ** TCH0.401;
#define UOP_CAN_CHANGE_ANGLE            0x00400000L     //UOP22, bit22
#define UOP_CAN_CHANGE_KARAOKE_AUDIO    0x00800000L     //UOP23, bit23
#define UOP_CAN_CHANGE_VIDEO_MODE       0x01000000L     //UOP24, bit24

// Micky1.00, used for _bCheckNVData
// bit [7:6], for same NV sector send count.
// the count will be cleared when 1) NV_INT got. 2) 2nd NV_INT fail after re-sending.
// bit [4], will be linear search NV sector
// bit [3:0], the NV sector state
// bit [7:4], the state for re-search NV action
#define CELL_NV_NONE    0
#define CELL_WAIT_NV    1       // desired sector has been sent, and wait NV_INT
#define CELL_GOT_NV     2       // Got NV_INT, ie. NV sector has been found
#define CELL_LINEAR_NV  0x10    // Search NV one by one sector
#define CELL_RESEND_NV  0x20    // Search NV by re-send same sector

// DVD_171-2Micky, re-define the parameter for _bEnableTimeSearch.
#define CELL_TIMESEARCH_NONE    0   // not doing any search
#define CELL_TIMESEARCH_FREEZE  1   // freeze cell playing
#define CELL_TIMESEARCH_TIME    2   // search VOBU for specified time
#define CELL_TIMESEARCH_VOBUSA  3   // serach specified VOBUSA

/*
#define CELL_VOBU_PLAY      1
#define CELL_VOBU_FREEZE    2
#define CELL_VOBU_SEARCH    3
*/

#define CELL_TIME_BEGIN     0   // ** TCH0.39;
#define CELL_TIME_FREEZE    1
#define CELL_TIME_PLAY      2
#define CELL_TIME_VOBU      3

//DVD_039-1
//Add CELL_Action
// DVD_102Micky, replace by CELL_VOBU_STILL_OFF
//#define CELL_STILL_OFF          1
#define CELL_BUTTON_SELECT      2
// DVD_100Micky, add cell status query
#define CELL_IS_STILL           3
// DVD_101-2Micky, add button numbers check
#define CELL_BUTTON_NUMBER      4
// DVD_102Micky, support PAUSE/PLAY to release VOBU still
#define CELL_VOBU_STILL_ON      5   // check if VOBU still is on
#define CELL_VOBU_STILL_OFF     6   // release VOBU still
// DVD_273.2Micky, add query for non-NOP command
#define CELL_SCAN_BTN_COMMAND  7

// DVD_171-2Micky, modify
typedef struct tagC_PBI
{
    char    C_CAT[4];
    char    C_PBTM[4];
    DWORD   C_FVOBU_SA;
    DWORD   C_FILVU_EA;
    DWORD   C_LVOBU_SA;
    DWORD   C_LVOBU_EA;
}   C_PBI;

//Ming0.80, use PCI and DSI directly.
typedef struct tagPCI_GINFO
{
    DWORD   NV_PCK_LBN;
    char    VOBU_CAT[2];
    char    Reserved1[2];
    DWORD   VOBU_UOP_CTL;
    DWORD   VOBU_S_PTM;
    DWORD   VOBU_E_PTM;
    DWORD   VOBU_SE_E_PTM;
    char    C_ELTM[4];
    char    Reserved2[32];
} PACK_FORMAT  PCI_GINFO;

typedef struct tagPCI_HLGINFO
{
    char    HLI_SS[2];
    DWORD   HLI_S_PTM;
    DWORD   HLI_E_PTM;
    DWORD   BTN_SL_E_PTM;
    WORD    BTN_MD;
    char    BTN_OFN;
    char    BTN_Ns;
    char    NSL_BTN_Ns;
    char    Reserved;
    char    FOSL_BTNN;
    char    FOAC_BTNN;
} PACK_FORMAT  PCI_HLGINFO;

typedef struct tagPCI_BTN_COLIT
{
    char        SL_COLI[4];
    char        AC_COLI[4];
} PACK_FORMAT  PCI_BTN_COLIT;

typedef struct tagPCI_BTNIT
{
    char            BTN_POSI[6];
    char            AJBTN_POSI[4];
    unsigned char   BTN_CMD[8];
} PACK_FORMAT  PCI_BTNIT;

typedef struct tagPCI_HLINFO
{
    PCI_HLGINFO     HLI_GI;
    PCI_BTN_COLIT   BTL_COLIT[3];
    PCI_BTNIT       BTNIT[36];
} PACK_FORMAT  PCI_HLINFO;

typedef struct tagPCI
{
    PCI_GINFO   PCI_GI;
    char        NSML_AGLI[9][4];
    PCI_HLINFO  HLI;
    char        RECI[189];
} PACK_FORMAT  PCI;

typedef struct tagDSI_GIINFO
{
    DWORD   NVPCK_SCR;
    DWORD   NV_PCK_LBN;
    DWORD   VOBU_EA;
    char    VOBU_1STREF_EA[4];
    char    VOBU_2NDREF_EA[4];
    char    VOBU_3RDREF_EA[4];
    char    VOBU_VOB_IDN[2];
    char    reserve[1];
    char    VOBU_C_IDN[1];
    char    C_ELTM[4];
} PACK_FORMAT  DSI_GIINFO;

typedef struct tagSML_PBIINFO
{
    char    VOBU_SML_CAT[2];
    DWORD   ILVU_EA;
    DWORD   NXT_ILVU_SA;
    char    NXT_ILVU_SZ[2];
    DWORD   VOB_V_S_PTM;
    DWORD   VOB_V_E_PTM;
    char    VOB_A_STP_PTM[64];
    char    VOB_A_GAP_LEN[64];
} PACK_FORMAT  SML_PBIINFO;

typedef struct tagDSI
{
    DSI_GIINFO  DSI_GI;
    SML_PBIINFO SML_PBI;
    char    SML_AGLI[9][6];
    DWORD   VOBU_SRI[42];
    char    SYNCI[144];
    char    reserve[471];
} PACK_FORMAT  DSI;

//Aron1.08-909, Record past NV's start/end presentation time and UOP field
typedef struct tagPAST_NV_UOP
{
    DWORD   VOBU_UOP_CTL;
    DWORD   VOBU_S_PTM;
    DWORD   VOBU_E_PTM;
} PACK_FORMAT  PAST_NV_UOP;

BYTE    CELL_CellInitial(C_PBI *pC_PBI);
void    CELL_AngleChange(BYTE bAngle);
WORD    CELL_Trigger(BYTE bParam);
BYTE    CELL_QueryUOP(DWORD  dwQuery, WORD *wParam);
BYTE    CELL_ButtonControl(BYTE bDirection, BYTE bState);
BYTE    CELL_ScanJump(void);
BYTE    CELL_TimeSearch(BYTE bFunction, DWORD dwTime);
//Aron1.15-909, not be used, comment it
//void    CELL_Recover(void);
DWORD   CELL_QueryTime(void);
// LLY2.36, add new API to get current playback cell begin time
DWORD   CELL_GetCellBeginTime(void);

//DVD_039-1
// DVD_100Micky, add cell status query
BYTE    CELL_QueryStatus(BYTE bQuery);

// protected
void    _DumpNVData(void);
void     _UpdatePCI(void);
BYTE    _CheckVOBUEnd(void);
BYTE     _ActiveButton(void);
void    _CellVOBUSearch(void);
//DVD_039-1
BYTE    _CheckForceActive(void);
// DVD_273.2Micky, add special function to execute non-NOP command
BYTE    _ScanButtonCommands(void);
// Micky0.67-909
void CELL_Debug(void);
extern  BYTE    __bGroupOffset;

// Ming0.68-909, Must wait for NV to update the SCR.
DWORD   _WaitParserEventNV(DWORD dwTime);

#ifdef __cplusplus
   }
#endif

#endif      // __CELL_H__
