
/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved.

        Product : WinAV  Firmware

        Date    : 1998.10.10
        Author  : Cheertek (D300 TCH)
        Purpose : VIDEO CD 2.0 module
                  The moudle will control the flow of VCD 2.0
        Sources : vcd20.h/ vcd20.c

****************************************************************************/

#ifndef __VCD20_H__
#define __VCD20_H__

extern DWORD           __dwTimeVCD20;                  // record Auto-Pause and Waiting Count

extern WORD            __wAutoPauseTime;                    //SELLECTION/PLAY_LIST
extern BYTE            __bVCD20Status;   // for waiting time control

// definitions

// VCD20Status definitions
#define     VCD20_NORMAL                0x00
#define     VCD20_WAITTIME              0x01
#define     VCD20_AUTOPAUSE             0x02

#define     KEY_NOTRIGGER               128             // as JumpTiming == 1

#define     SUBMODE_EOF                 0x01
#define     SUBMODE_TRIGGER             0x02
#define     SUBMODE_EORN                0x04
#define     SUBMODE_EORH                0x08
#define     SUBMODE_EOR                 (SUBMODE_EORN | SUBMODE_EORH)
#define     SUBMODE_ALL                 0x0F

////////////////////////////////////////

BYTE       VCD20_Action ( WORD nCommand );
BYTE       VCD20_KeyNumber ( WORD nNumber );
WORD      VCD20_Trigger (void);
BYTE       VCD20_IsValid_Command ( WORD nCommand );
void      VCD20_ShowListType(void);
//////////////////////////////////////////////
// protected functions

BYTE      _VCD20_GetListInfor (void);
//BYTE      _VCD20_SegmentInitial (void);
BYTE       _VCD20_Check_PlayEnd (void);
BYTE       _VCD20_SelectionListEnd (void);
BYTE       _VCD20_PlayListEnd (void);
void       _VCD20_ErrorMenuRecovery(void);
void       _VCD20_PlayEnd(void);
void       _VCD20_AutoPause(void);


#endif  // __VCD20_H__
