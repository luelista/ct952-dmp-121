#ifndef     __COMDEF_H__
#define     __COMDEF_H__
// ****** Common definitions and typedefs **********************************

// Added by D300 Chern Shyh-Chain, 4/11/2002
// Rename and redefine the 16-bit huge environment since it is necessary at 32-bit.
#ifdef  WIN32

#define _halloc( size, dummy )  malloc( size )
#define _hfree( pointer )       free( pointer )

// wyc.host
#ifndef _huge
#define _huge
#endif // _huge
#ifndef __huge
#define __huge
#endif // _huge

#define far
#define _far

#endif  // WIN32
#define BIT                 BYTE

// Micky2.80p, don't reference OS_XXX directly
#ifndef  APPLICATION_WINDOWS
// below identifier/ macros are only for Windows
#define VOID                void
#define FALSE               0
#define TRUE                1
typedef unsigned char       BOOL;

// wyc0.60-909, add and merge some define from typedef.h
typedef char                CHAR;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;

typedef unsigned long long  ULONGLONG;
typedef long long           LONGLONG;
typedef ULONGLONG * PULONGLONG;
typedef LONGLONG *  PLONGLONG;

typedef short               SHORT;
typedef long                LONG;
typedef int                 INT;

typedef unsigned long       ULONG;
typedef unsigned short      USHORT;
typedef unsigned int        UINT;


typedef CHAR *      PCHAR;
typedef SHORT *     PSHORT;
typedef LONG *      PLONG;
typedef VOID *      PVOID;


// cat't define as (WORD *)!!!
typedef WORD *      PWORD;
typedef BYTE *      PBYTE;
typedef DWORD *     PDWORD;

#ifndef NULL
#define NULL        0
#endif //

#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((DWORD)(w) >> 8) & 0xFF)) 

#define MAKEWORD(low, high) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8 )))

#define LOWORD(l)           ((WORD)(DWORD)(l))
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))

#define MAKELONG(low, high) ((LONG)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))
// LLY.049-1, define rectangle structure
#endif  //#ifndef  APPLICATION_WINDOWS
#define NULL_0                0

typedef struct tagRECT_
{
    WORD    wLeft;
    WORD    wTop;
    WORD    wRight;
    WORD    wBottom;
} URECT, *PURECT;

typedef struct  tagWIN_PARAMETER
{
    SHORT   sHStart;
    WORD    wHWidth;
    SHORT   sVStart;
    WORD    wVWidth;
    BYTE    bEnable;
} WIN_PARAMETER, *PWIN_PARAMETER;

typedef union   tagWIN_COLOR
{
    struct
    {
        BYTE    bWeight; // 6 bits resolution
        BYTE    bY;
        BYTE    bU;
        BYTE    bV;
    } Color;    
    DWORD       dwColorReg;
}WIN_COLOR , *PWIN_COLOR;

typedef union   tagULONGLONG_UNION
{
    BYTE        b8bit[8];
    WORD        w16bit[4];
    DWORD       dw32bit[2];
    ULONGLONG   ull64bit;
} ULONGLONG_UNION, *PULONGLONG_UNION;

typedef union   tagLONGLONG_UNION
{
    CHAR        c8bit[8];
    SHORT       s16bit[4];
    LONG        l32bit[2];
    LONGLONG    ll64bit;
} LONGLONG_UNION, *PLONGLONG_UNION;

typedef union   tagULONG_UNION
{
    BYTE        b8bit[4];
    WORD        w16bit[2];
    DWORD       dw32bit;
} ULONG_UNION, *PULONG_UNION;

typedef union   tagLONG_UNION
{
    CHAR        c8bit[4];
    SHORT       s16bit[2];
    LONG        l32bit;
} LONG_UNION, *PLONG_UNION;

typedef union   tagWORD_UNION
{
    BYTE        b8bit[2];
    WORD        w16bit;
} WORD_UNION, *PWORD_UNION;

typedef union   tagSHORT_UNION
{
    CHAR        c8bit[2];
    SHORT       s16bit;
} SHORT_UNION, *PSHORT_UNION;


#ifndef NOMINMAX

#define MIN(a,b)                (((a) < (b)) ? (a) : (b))
#define MAX(a,b)                (((a) > (b)) ? (a) : (b))

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#endif  // NOMINMAX

// ***** Common pointer types ***********************************************

// time format conversion macros ***************************
#define MSF_MINUTE(msf)             ((BYTE)(msf))
#define MSF_SECOND(msf)             ( (BYTE) ( ( (WORD) (msf) ) >> 8 ) )
#define MSF_FRAME(msf)              ((BYTE)((msf)>>16))
#define MAKE_MSF(m, s, f)           ((DWORD)(((BYTE)(m) | ((WORD)(s)<<8)) | (((DWORD)(BYTE)(f))<<16)))

#define TMSF_TRACK(tmsf)            ((BYTE)(tmsf))
#define TMSF_MINUTE(tmsf)           ((BYTE)(((WORD)(tmsf)) >> 8))
#define TMSF_SECOND(tmsf)           ((BYTE)((tmsf)>>16))
#define TMSF_FRAME(tmsf)            ((BYTE)((tmsf)>>24))

#define MAKE_TMSF(t, m, s, f)       ((DWORD)(((BYTE)(t) | ((WORD)(m)<<8)) | (((DWORD)(BYTE)(s) | ((WORD)(f)<<8))<<16)))

// ** TCH0.36; begin...
#define HMSF_HOUR(hmsf)             ((BYTE)(hmsf))
#define HMSF_MINUTE(hmsf)           ((BYTE)(((WORD)(hmsf)) >> 8))
#define HMSF_SECOND(hmsf)           ((BYTE)((hmsf)>>16))
#define HMSF_FRAME(hmsf)            ((BYTE)((hmsf)>>24))

#define MAKE_HMSF(h, m, s, f)       ((DWORD)(((BYTE)(h) | ((WORD)(m)<<8)) | (((DWORD)(BYTE)(s) | ((WORD)(f)<<8))<<16)))
// ** TCH0.36; end...

// LLY.043, YUV <--> RGB conversion macros
// [31:24]:don't care, [23:16]:R value, [15:8]: G value, [7:0]: B value
#define RGB_R(rgb)              ((BYTE)((DWORD)rgb>>16))
#define RGB_G(rgb)              ((BYTE)((DWORD)rgb>>8))
#define RGB_B(rgb)              ((BYTE)rgb)
#define MAKE_RGB(r, g, b)       ( ((DWORD)r<<16) | ((DWORD)g<<8) | (DWORD)b ) // LLY.102

// [Notice] YUV format is defined as OSD spec.
// [15:10]<-- Y[7:2]; [9:8]<-- 0x0
// [7:4]<-- U[7:4]; [3:0]<-- V[7:4]
#define YUV_Y(yuv)              ((BYTE)((WORD)yuv>>10)<<2)
#define YUV_U(yuv)              ((BYTE)(yuv&0x00F0))
#define YUV_V(yuv)              ((BYTE)(yuv&0x000F)<<4)
//Brian2.81
#define MAKE_YUV(y, u, v)       ((((DWORD)(y&0xFE))<<11) | (((DWORD)(u&0xFC))<<4) | (((DWORD)(v&0xFC))>>2))
// LLY.043 end ...


#endif      // __COMDEF_H__


