
/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved.

        Product : WinAV  Firmware

        Date    : 1998.10.10
        Author  : Cheertek (D300 TCH)
        Purpose : INITIALIZATION Module
                  The moudle contains all kernel-module initial function
        Sources : initial.h / inital.c

****************************************************************************/

#ifndef __INITIAL_H__
#define __INITIAL_H__

#ifdef __cplusplus
        extern  "C" {
#endif  // #ifdef __cplusplus


// Micky_PORTING_OSWRAP
/*
//////////////////////////////////////////////////////////////////////////
//  Structure Define
typedef struct  tagTHREADINFO
{
    cyg_addrword_t  priority;   // scheduler-specific information, priority value for the thread.
    cyg_handle_t    handle;     // the handle for the thread
    cyg_thread      space;      // thread information is stored in the thread menmory object pointed to by this parameter
} THREAD, *PTHREAD;
*/


//////////////////////////////////////////////////////////////////////////
// *******   Extern variable Area   *****
// [1] the real sectors that will jump for each SCAN
// LLY2.80, remove to CC module
//extern  WORD    __wScanGap;

// [3] Reserve a buffer for SP palette -- LLY0.61
extern  BYTE    __bSPPaletteBuffer[64];

extern  THREAD_INFO  ThreadCTKDVD;
extern  THREAD_INFO  ThreadDec;
extern  THREAD_INFO  ThreadPARSER;
// LLY0.95, only define ATAPI thread stack while support ATAPI interface
#ifdef  SUPPORT_ATAPI_SOURCE
extern  THREAD_INFO  ThreadATAPI;
#endif  // #ifdef SUPPORT_ATAPI_SOURCE
extern  THREAD_INFO  ThreadInfoFilter;
// wyc1.01, for USB thread.
#ifdef  SUPPORT_USB_SOURCE
extern THREAD_INFO  ThreadUSBSRC;
#endif  // #ifdef SUPPORT_ATAPI_SOURCE

extern CHAR    __cCTKDVDStack[];
extern CHAR    __cDecStack[];
extern CHAR    __cPARSERStack[];
extern CHAR    __cInfoFilterStack[];
#ifdef  SUPPORT_ATAPI_SOURCE
extern CHAR    __cATAPIStack[];
#endif  // #ifdef SUPPORT_ATAPI_SOURCE
#ifdef  SUPPORT_USB_SOURCE
extern CHAR    __cUSBSRCStack[];
#endif  // #ifdef SUPPORT_USB_SOURCE

//extern  HANDLE_T    __ClockHandle;

// LLY0.62, extern them for other module reference.
extern  WIN_PARAMETER   PIPVideoInfo;
extern  WIN_COLOR       MainBGColor;
extern  WIN_COLOR       PIPBGColor;
extern  WIN_COLOR       PIPBorderColor;


// LLY0.63, define a cyg_flag to keep every thread initial action
extern  FLAG_T      __fThreadInit;




///////////////////////////////////////////////////////////////////
// ***** Some definition for global variabe usage Area ****
/// [2] Define the ID for __fThreadInit
// Decorder thread within BYTE 0, ie bit[7:0]
#define     INIT_DEC_THREAD_MPEG_DONE       0x00000001  // MPEG decoder thread initial done
#define     INIT_DEC_THREAD_JPEG_DONE       0x00000002  // JPEG decoder thread initial done
#define     INIT_DEC_THREAD_DIVX_DONE       0x00000004  // DivX decoder thread initial done

// Misc thread within BYTE 1, ie. bit[15:8]
#define	    INIT_PARSER_THREAD_DONE         0x00000100	// Parser thread initial done
#define	    INIT_INFO_FILTER_THREAD_DONE    0x00000200	// Information Filter thread initial done

// Source thread within BYTE 2 & 3, ie. bit[31:16]
// LLY0.80, reserve two bits for servo thread since there are two threads for servo
#define     INIT_SRC_THREAD_ATAPI_DONE      0x00010000	// ATAPI thread initial done
#define     INIT_SRC_THREAD_SERVO1_DONE     0x00020000	// Servo thread #1 initial done
#define     INIT_SRC_THREAD_SERVO2_DONE     0x00040000  // Servo thread #2 initial done
#define     INIT_SRC_THREAD_USB_DONE        0x00080000	// USB thread initial done

// LLY2.36, define the decoder error ratio.
// It will skip the picture while this picture is destroyed > 1/DEC_ERROR_RATIO
#define     DEC_ERROR_RATIO     8

// Integrate all thread for power on action.
// LLY1.01, define the default necessary thread w/o source, ex.
// MPEG thread, Parser thread, information filter(eCos909 don't include information filter)
// Then, add necessary source thread, ex. ATAPI, Servo, USB within INITIAL_PowerOnStatus()
#ifdef  SIMP_INITIAL
#define INIT_POWER_ON_THREAD_X_SOURCE_DONE  (INIT_DEC_THREAD_MPEG_DONE | INIT_PARSER_THREAD_DONE )
#else   // #ifdef SIMP_INITIAL
#define INIT_POWER_ON_THREAD_X_SOURCE_DONE  (INIT_DEC_THREAD_MPEG_DONE | INIT_PARSER_THREAD_DONE | INIT_INFO_FILTER_THREAD_DONE)
#endif  // #ifdef SIMP_INITIAL

///////////////////////////////////////////////////////////////////////
// ***** Parameter definition for function API usage Area *****
// [1] parameters for INITIAL_PowerONStatus()
#define INITIAL_POWER_HARDWARE      0
#define INITIAL_POWER_SOFTWARE      1

// [2] parameters for INITIAL_ThreadInit()
#define THREAD_ATAPI            1
#define THREAD_PARSER           2
#define THREAD_MPEG_DECODER     3
#define THREAD_DIVX_DECODER     4
#define THREAD_JPEG_DECODER     5
//#define THREAD_SRC_FILTER     6
#define THREAD_SERVO            7
#define THREAD_INFO_FILTER      8
// wyc1.01-909, add USB thread.
#define THREAD_USBSRC           9

#define STACK_MAGIC_NUMBER                  (0x5A)

/////////////////////////////////////////////////////////
// ***** Function prototype definition Area *****
// LLY.171-1, give parameter for INITIAL_System(), and the parameter are put in winav.h
void    INITIAL_System(BYTE bMode);
// DVD_100Micky, merge I/R power on sequence with h/w power on.
void    INITIAL_PowerONStatus (BYTE bPower);
void    INITIAL_RunSystem(void);    // LLY2.17, add function prototype
#ifdef  SIMP_INITIAL
void    INITIAL_InitialState(void);
#endif  // #ifdef  SIMP_INITIAL
void    INITIAL_Variables(void);
WORD    INITIAL_CheckServo(void);
void    INITIAL_ThreadInit(BYTE bThread);
BYTE    INITIAL_ThreadDelete(BYTE bThread); // LLY0.72


// protected
void    _INITIAL_gcShowStatus(BYTE bFlag);
void    _INITIAL_Variables_PowerON(void);  // ** TCH1.60-1;
void    _GetCDDAFormat(void);  // LLY.161
// wyc1.24a, one function to display first time LOGO.
void    _INITIAL_ShowFirstLOGO(void);
void    _INITIAL_ClearStack(void);

// Micky1.23 support resume info from EEPROM
// LLY2.14, porting Aron's code to support STB system
#if defined(SUPPORT_MULTI_DISC_RESUME) //||defined(SUPPORT_STB)
BYTE    _Detect_Resume_Information(void);
BYTE    _Save_Resume_Information(BYTE bAction);

// Define the ID for _Save_Resume_Information() parameter -- LLY0.95
#define SAVE_INFO   (TRUE)
#define CLEAR_INFO  (FALSE)

#endif  //#ifdef  SUPPORT_MULTI_DISC_RESUME
BYTE INITIAL_MediaSetting(void);


#ifdef __cplusplus
        }
#endif  // #ifdef __cplusplus


#endif  // __INITIAL_H__


