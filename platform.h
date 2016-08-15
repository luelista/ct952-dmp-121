// This header file is used for system define
// Also define some capability for different system
#ifndef _PLATFORM_H
#define _PLATFORM_H

// wyc0.60-909, re-config the system define for PC908 & CT909 mode.
#define SYSTEM_ECOS
#define _WINDVD
// wyc0.60-909, add file include here. This header file is used for thread control.
// wyc2.16-909S, remove it by Micky's request.
//#include <cyg/kernel/kapi.h>
//#include <cyg/hal/hal_arch.h>          // Architecture specific definitions

// OS is Win2000. WIN2000 will be enabled within makefile.
// When enabled, will use the PCI library from 2000(through WDM driver).
// Else will use direct I/O PCI library(D300 build)
// Don't open the define WIN2000 here
//#define   WIN2000

#ifdef  WIN2000
// through WDM driver
#define IO_PCI_WDM
#endif

// Follow define is used to select system IC. Only can choose one of them can't define more than 2 IC in below define.
//#define CT909R_IC_SYSTEM
#define CT909P_IC_SYSTEM

// LLY2.56, add new define for CT909G IC system
//#define CT909G_IC_SYSTEM

// VC 32bit compiler will auto add the define WIN32.
// When enabled, use 32 bits APIs for Timer/ RS232
// Don't open the define WIN32
//#define   WIN32

// add a OS type define here
#define OS_ECOS         1
#define OS_DOS          2

// add CPU type define here
#define CPU_SPARC       1
#define CPU_X86         2

// add I/O type
#define IO_SPARC         1   // 8051 bus
#define IO_PCI          2   // PCI bus
// Micky2.17
#define IO_NONE     3

// [Selected by System Configuration]
// !!! can't open the define directly !!!!
/// define it, means the CPU is little endian
/// else, default is big endian
// #define USE_LITTLE_ENDIAN

/// define it, means can use printf to print some message.
/// else, default don't printf the message.
// #define SUPPORT_PRINTF

/// define it, means will use standard C library
/// ekse, default don't support it.
// #define USE_STD_C_LIB

/// define it, means the application is under Windows.
/// else, default is DOS prompt or 8051 application.
// #define APPLICATION_WINDOWS

// must add a block for each system description
// include OS, CPU type, system capability
/// [System Configuration]

#ifdef  SYSTEM_ECOS
#define OS      OS_ECOS
#define CPU     CPU_SPARC
//#undef  NO_PANEL               //jeffsun 20040103
//#define SUPPORT_PRINTF
#define IO  IO_SPARC
#endif

// Micky2.17
#ifdef  APPLICATION_WINDOWS
#define OS      OS_DOS
#define CPU     CPU_X86
#define IO      IO_NONE
#include	<windows.h>
#include	<stdio.h>

#endif  //#ifdef  APPLICATION_WINDOWS

// adding header file here
#include    "comdef.h"

#ifdef  USE_STD_C_LIB
#include    <stdio.h>
#include    <stdlib.h>
#include    <conio.h>
#include    <malloc.h>
#endif


#if CPU == CPU_X86
#define USE_LITTLE_ENDIAN
#endif

#define NOP
// only 8051 has these define
extern      BYTE    EA, EX0, EX1;
#if OS ==OS_DOS
#define data
#endif //
#define xdata
// for data type define
#define IDATA
#define DATA
#define code
#define pragma
#define NOAREGS
#define DISABLE
//#define OPTIMIZE(a, b)
#define bit BYTE

#if ( IO == IO_PCI)
// Chuan0.83a, Remapping PCI I/O function
// This part should be after C's standard header files, ex. stdio.h, conio.h, ...
// Otherwise, it will remapping declarations of those functions, _inpdw, _inp, _outpdw, and _outp, and caused errors reported.

#ifdef  IO_PCI_WDM
#include "PCIBridge.h"
// Remapping PCI I/O function
// This part should be after C's standard header files, ex. stdio.h, conio.h, ...
// Otherwise, it will remapping declarations of those functions, _inpdw, _inp, _outpdw, and _outp, and caused errors reported.
#define _inpdw          PCI_ReadDWORD
#define _inpd           PCI_ReadDWORD
#define _inp            PCI_ReadByte
#define _outpdw         PCI_WriteDWORD
#define _outpd          PCI_WriteDWORD
#define _outp           PCI_WriteByte
#else
#include    "pci.h"
#endif

#endif  // End  


// Brian.276p, support _printf
#ifdef SUPPORT_PRINTF
#define PRINTF printf

#endif

// wyc0.66, move printf re-define function to winav.h
// Micky2.80p, move to winav.h when SERIAL_DEBUG is defined.
// define this to let emulator also use _printf to output data
//#define printf _printf

//brian0.85-3rd
// DTS-GXA workaround
#define ACK_FROM_OTHERS

#endif  //_PLATFORM_H
