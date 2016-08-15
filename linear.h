
/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved.

        Product : WinAV  Firmware

        Date    : 1998.10.10
        Author  : Cheertek (D300 TCH)
        Purpose : LINEAR module
                  The moudle will control the flow of Linear Playing
        Sources : linear.h/ linear.c

****************************************************************************/

#ifndef __LINEAR_H__
#define __LINEAR_H__

#define LINEAR_PROGRAM_MODE             0
#define LINEAR_PROGRAM_CLEAR            1
#define LINEAR_PROGRAM_ADD_ENTRY        2
#define LINEAR_PROGRAM_CLEAR_ENTRY      3
#define LINEAR_COUNT_ENTRY              4
#define LINEAR_LEFT_RIGHT_OPERATION     5
#define LINEAR_PROGRAM_CLEAR_ALL_ENTRY  6
#define LINEAR_PROGRAM_CLEAR_MIN        7
//wyc.pro1
//#define PROGRAM_LEFTTORIGHT
#define LINEAR_CLEARENTRY           0
#define LINEAR_CLEARNUMBER          1
#define BACKWARD                    0
#define FORWARD                     1
// wyc.pro1
#define LINEAR_FIND_ENTRY           0
#define LINEAR_DIGIT_TO_ENTRY       1
#define LINEAR_ENTRY_TO_DIGIT       2
#define TRACK_NUMBER                0
#define TITLE_NUMBER                1
#define CHAPTER_NUMBER              2

//Kevin0.86, add
#define LINEAR_CLEAR_ALL_ENTRY                      0
#define LINEAR_QUERY_INDEX                          1
#define LINEAR_ADD_ENTRY_BY_INDEX                   2
#define LINEAR_DELETE_ENTRY_BY_INDEX_W_REARRANGE    3
#define LINEAR_DELETE_ENTRY_BY_INDEX_WO_REARRANGE   4

// support_more_mp3
BYTE    LINEAR_ChangeTrack(BYTE bKey, WORD wTrackNO);
BYTE    LINEAR_Trigger(void);
BYTE     LINEAR_Program(BYTE bFunction, BYTE bKey);
BYTE     LINEAR_Action(BYTE bKey, DWORD   dwParam);
// wyc.pro
void    LINEAR_ClearProgramEntry(BYTE  bParam, BYTE bNumBegin, BYTE bNumEnd);
// wyc.pro1
BYTE _LinearEntryOperation(BYTE bType, BYTE bParam);
WORD _LinearReturnMaxTrack(BYTE bParam);

// protected
BYTE    _CheckRepeatMode(BYTE  bKey);
BYTE     LINEAR_Number( WORD wNumber );
void    LINEAR_PlayItem(void);
//winnie2.51, for play jpg from SPI flash
#ifdef  SUPPORT_ENCODE_JPG_PICTURE                        
BYTE LINEAR_JPGAction(BYTE bKey,BYTE bPlayIdx);
#endif //
//Kevin0.86, program index list array for font mode / GDI mode  
typedef struct tagPROG_INDEX
{
    BYTE    bMainIndex;     // main index of program data
    WORD    wSubIndex;      // sub-index relative to main index
}   PROG_INDEX, * PPROG_INDEX;
#ifdef SUPPORT_ENCODE_JPG_PICTURE
#define MAX_PROG_ITEM       15
#else
#define MAX_PROG_ITEM       20
#endif //
extern  PROG_INDEX      __ProgIdxList[MAX_PROG_ITEM];

//Kevin1.00, add
extern BIT             __btProgram;
extern BIT             __btInputProgram;
extern BIT             __btPlayingProgram;
extern BIT             __btNextPlayProgram;

//Kevin0.86, add
extern  BYTE    LINEAR_ProgramEntryOperation(BYTE bAction, BYTE bArrayIndex, BYTE bMainIndex, WORD wSubIndex);
extern  BYTE    __bProgramCurrentIndex;
extern BYTE     __bProgramMaxEntry;//Micky2.80, review global variables init. = 0;

// wyc2.80, move the variables' decalre from CC to LINEAR.
extern BYTE     __bProgramCurrentIndex;
extern BYTE     __bProgramPlayingIndex;
//winnie2.51, added for play jpg from SPI flash
#ifdef SUPPORT_ENCODE_JPG_PICTURE
extern BYTE __bMMJPGEncodeNum;
extern BYTE __bOSDSSPicIdx;
#endif 


#endif  // __LINEAR_H__
