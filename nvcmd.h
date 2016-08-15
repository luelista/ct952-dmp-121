#ifndef     __NVCMD_H__
#define     __NVCMD_H__

#ifdef __cplusplus
   extern "C"{
#endif

// ** TCH0.29; begin...
// There are 15 kinds of NV command, Ref VI 4-180
// the output after executing the NV command
extern  BYTE    __bReturnState;
// the parameters for output state
extern  WORD    __wReturnParameter;
extern  WORD    __wReturnParameter2;
// DVD_155Micky, must record the cell number for resume
// fix "dinosaur" 1/28-> back to menu-> can play from TT/CC 01/01
// the Cell number to resume
extern BYTE __bNVRSMCN;

//DVD_050Micky
extern WORD  __wStoreSPRM13;
// ** TCH0.29; end...

#define NV_GOTO_BASE        0
#define NV_GOTO_CMD         (NV_GOTO_BASE+1)
#define NV_NOP              (NV_GOTO_BASE+2)
#define NV_EXIT_CMD_AREA    (NV_GOTO_BASE+3)
#define NV_GOTO_LENGTH      10

#define NV_LINK_BASE        (NV_GOTO_BASE+NV_GOTO_LENGTH)
#define NV_LINK_PGCN        (NV_LINK_BASE+1)
#define NV_LINK_PTTN        (NV_LINK_BASE+2)
#define NV_LINK_PG          (NV_LINK_BASE+3)
#define NV_LINK_CN          (NV_LINK_BASE+4)
#define NV_LINK_NOLINK      (NV_LINK_BASE+5)
#define NV_LINK_TOPC        (NV_LINK_BASE+6)
#define NV_LINK_NEXTC       (NV_LINK_BASE+7)
#define NV_LINK_PREVC       (NV_LINK_BASE+8)
#define NV_LINK_TOPPG       (NV_LINK_BASE+9)
#define NV_LINK_NEXTPG      (NV_LINK_BASE+10)
#define NV_LINK_PREVPG      (NV_LINK_BASE+11)
#define NV_LINK_TOPPGC      (NV_LINK_BASE+12)
#define NV_LINK_NEXTPGC     (NV_LINK_BASE+13)
#define NV_LINK_PREVPGC     (NV_LINK_BASE+14)
#define NV_LINK_GOUPPGC     (NV_LINK_BASE+15)
#define NV_LINK_TAILPGC     (NV_LINK_BASE+16)
#define NV_LINK_RSM         (NV_LINK_BASE+17)
#define NV_LINK_LENGTH      20

#define NV_JUMP_BASE        (NV_LINK_BASE+NV_LINK_LENGTH)
#define NV_JUMP_EXIT        (NV_JUMP_BASE+1)
#define NV_JUMP_TT          (NV_JUMP_BASE+2)
#define NV_JUMP_VTSTT       (NV_JUMP_BASE+3)
#define NV_JUMP_VTSPTT      (NV_JUMP_BASE+4)
#define NV_JUMP_ROOT_MENU   (NV_JUMP_BASE+5)
#define NV_JUMP_SUPIC_MENU  (NV_JUMP_BASE+6)
#define NV_JUMP_AUDIO_MENU  (NV_JUMP_BASE+7)
#define NV_JUMP_ANGLE_MENU  (NV_JUMP_BASE+8)
#define NV_JUMP_PTT_MENU    (NV_JUMP_BASE+9)
#define NV_JUMP_TITLE_MENU  (NV_JUMP_BASE+10)
#define NV_JUMP_FP_PGC      (NV_JUMP_BASE+11)
#define NV_JUMP_VMGM_PGC    (NV_JUMP_BASE+12)
#define NV_JUMP_LENGTH      20

#define NV_CMD_ID_FIELD_MASK 0xF000
#define NV_CMD_BRANCH_FIELD_MASK 0x000F

enum NV_CMD_ID
{
    CMD_GRP_GOTO                             = 0x0000,
    CMD_GRP_LINK                              = 0x2000,
    CMD_GRP_JUMP                             = 0x3000,
    CMD_GRP_SETSYSTEM                   = 0x4000,    
    CMD_GRP_SETSYSTEM_I                = 0x5000,        
    CMD_GRP_SET                                = 0x6000,            
    CMD_GRP_SET_I                            = 0x7000,
    CMD_GRP_SET_COMLINKSINS       = 0x8000,   // set; if(compare){ LinkSIns;}
    CMD_GRP_SET_COMLINKSINS_I    = 0x9000,                
    CMD_GRP_COM_SETLINKSINS       = 0xA000,   // if(compare){set; LinkSIns;}                 
    CMD_GRP_COM_SETLINKSINS_I   = 0xB000,                        
    CMD_GRP_COMSET_LINKSINS       = 0xC000,   // if(compare){set;} LinkSIns;
    CMD_GRP_COMSET_LINKSINS_I   = 0xD000    
};

// Compare Field Aron added 2006.08.22 ================
enum NV_CMD_COMPARE_OPTION
{
    COMPARE_OPTION_BC           = 1, //001b Bitwise Compare
    COMPARE_OPTION_EQ           = 2, //010b Equal
    COMPARE_OPTION_NE           = 3, //011b Not Equal
    COMPARE_OPTION_GE           = 4, //100b Greater than or equal to
    COMPARE_OPTION_GT           = 5, //101b Greater than
    COMPARE_OPTION_LE           = 6, //110b Less than or equal to
    COMPARE_OPTION_LT           = 7  //111b Less than
};
// =========================================

// Branch Field Aron added 2006.08.22 =================
enum NV_CMD_GOTO_OPTION
{
    GOTO_OPTION_NOP               = 0, //0000b
    GOTO_OPTION_GOTO             = 1, //0001b
    GOTO_OPTION_BREAK           = 2, //0010b
    GOTO_OPTION_SETTMPPML   = 3  //0011b
};

enum NV_CMD_LINK_OPTION
{
    LINK_OPTION_LINKSINS         = 1,   //0001b
    LINK_OPTION_LINKPGCN        = 4,   //0100b
    LINK_OPTION_LINKPTTN        = 5,   //0101b
    LINK_OPTION_LINKPGN          = 6,   //0110b
    LINK_OPTION_LINKCN            = 7,   //0111b
    LINK_OPTION_LINKPGCNR      = 12, //1100b HDDVD ext
    LINK_OPTION_LINKPTTNR      = 13, //1101b HDDVD ext
    LINK_OPTION_LINKPGNR        = 14, //1110b HDDVD ext
    LINK_OPTION_LINKCNR          = 15  //1111b HDDVD ext    
};

enum NV_CMD_JUMP_OPTION
{
    JUMP_OPTION_EXIT                    = 1, //0001b
    JUMP_OPTION_JUMPTT               = 2, //0010b
    JUMP_OPTION_JUMPVTS_TT       = 3, //0011b
    JUMP_OPTION_JUMP_VTS_PTT   = 5, //0101b
    JUMP_OPTION_JUMPSS               = 6, //0110b
    JUMP_OPTION_CALLSS               = 8  //1000b
};
// =========================================

// SetSystem Field Aron added 2006.08.22 ================
enum NV_CMD_SETSYSTEM_OPTION
{
    SETSYSTEM_OPTION_SETSTN              = 1, //0001b 
    SETSYSTEM_OPTION_SETNVTMR         = 2, //0010b
    SETSYSTEM_OPTION_SETGPRMMD      = 3, //0011b
    SETSYSTEM_OPTION_SETHL_BTNN     = 6, //0110b
    SETSYSTEM_OPTION_SETM_LCD         = 8, //1000b
    SETSYSTEM_OPTION_SETM_STN         = 9, //1001b
    SETSYSTEM_OPTION_SETFP_STN       = 10  //1010b
};
// =========================================

// Set Field Aron added 2006.08.22 ================
enum NV_CMD_SET_OPTION
{
    SET_OPTION_MOV    = 1,  //0001b 
    SET_OPTION_SWP    = 2,  //0010b
    SET_OPTION_ADD    = 3,  //0011b
    SET_OPTION_SUB     = 4,  //0100b
    SET_OPTION_MUL    = 5,  //0101b
    SET_OPTION_DIV     = 6,  //1010b
    SET_OPTION_MOD    = 7,  //1011b
    SET_OPTION_RND    = 8,  //1000b
    SET_OPTION_AND    = 9,  //1001b
    SET_OPTION_OR      = 10, //1010b
    SET_OPTION_XOR    = 11  //1011b
};
// =========================================


BIT _NV_Compare(WORD wOPCode);
BIT _NV_Cmp_GoTo_Cmd(WORD wOPCode);
BIT _NV_SetCmp_LinkSIns(WORD wOPCode);
BIT _NV_Cmp_SetLinkSIns(WORD wOPCode);
BIT _NV_Cmp_Set_LinkSIns(WORD wOPCode);
BIT _NV_Cmp_Link_Cmd(WORD wOPCode);
BIT _NV_Cmp_Jump_Cmd(WORD wOPCode);
BIT _NV_Cmp_SetSystem(WORD wOPCode);
BIT _NV_SetSystemLink(WORD wOPCode);
BIT _NV_Cmp_Set(WORD wOPCode);
BIT _NV_SetLink(WORD wOPCode);
BIT _NV_Set(void);
BIT _NV_LinkSIns(void);
BIT _NV_Link(void);
BIT _NV_Jump(void);
BIT _NV_SetSystem(void);
WORD    _NV_Random(WORD wMaxValue);

BIT NV_ExecuteCommand(unsigned char *s8Command);
void NV_InitialParameters ( BYTE bCondition );  // ** TCH1.61a;  
#ifdef __cplusplus
   }
#endif

#endif      // __NVCMD_H__
