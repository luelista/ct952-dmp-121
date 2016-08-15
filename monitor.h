#ifndef __MONITOR_H__
#define __MONITOR_H__

///////////////////////////////////////////////////////////////////
// define
// The define for recover stage
#define RECOVER_STAGE_PREPARE           0
#define RECOVER_STAGE_IDLE              1
#define RECOVER_STAGE_SW_RESET          2
#define RECOVER_STAGE_JUMP_SECTOR       3
#define RECOVER_STAGE_HW_RESET          4
#define RECOVER_STAGE_RESET_SERVO       5
#define RECOVER_STAGE_NEXT_ITEM         6

///////////////////////////////////////////////////////////////////
// extern variables
extern  DWORD   __dwMONRecoverStage;
extern  BYTE    __bResetRecover;

///////////////////////////////////////////////////////////////////
// Function Prototype
void MONITOR_CheckWatchDog(void);
void    MONITOR_RecoverSystem(void);
void    MONITOR_Debug(void);
// wyc0.95, add functions.
void    MONITOR_AlarmInit(void);
// Micky2.17c
VOID    MONITOR_INTERRUPT(HANDLE_T alarmH, WORD data1);
void    MONITOR_CheckHang(void);
// Protected function

#endif  // #ifndef __MONITOR_H__
