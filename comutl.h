
/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved.

        Product : WinAV Firmware

        Date    : 1999.8.25.
        Author  : Cheertek (D300 TCH)
        Purpose : Useful Common API
                  The moudle will support the API that independent with other Module
        Sources : comutl.h/ comutl.c
****************************************************************************/

#ifndef __COMUTL_H__
#define __COMUTL_H__

// ** TCH2.79; begin... Add it for independence
////////////////////////////////////////////////////////////
// structure definition
#ifndef     DEF_UNIONWORD
#define     DEF_UNIONWORD
typedef union tagUNIONWORD
{
    BYTE    cS[2];
    WORD    nVal;
}   UWORD;
#endif

#ifndef     DEF_UNIONDWORD
#define     DEF_UNIONDWORD
typedef union tagUNIONDWORD
{
    BYTE    cS[4];
    DWORD   dwVal;
}   UDWORD;
#endif
// ** TCH2.79; end... 


extern  DWORD   __dwCOMUTLTemp;      // ** TCH1.72r; for external parameter

BYTE    COMUTL_DecimaltoBCD ( BYTE bVal );
BYTE    COMUTL_BCDtoDecimal ( BYTE bVal );
DWORD   COMUTL_MSFtoHSG ( DWORD msfTime );
DWORD   COMUTL_HSGtoMSF ( DWORD hsgTime );

void    COMUTL_Delay ( WORD wLoop);

void    COMUTL_StrUpr ( char * pS );            // ** TCH0.38;
// ** TCH1.55; begin... 
///void    COMUTL_SwapWORD ( WORD * pwPtr );            
void    COMUTL_SwapDWORD ( DWORD * pdwPtr );         
// wyc1.01-909, add one function to swap WORD for FAT usage.
void COMUTL_SwapWORD ( WORD * pwPtr );
// ** TCH1.55; end... 

// ** TCH0.39; begin...
WORD    COMUTL_HMStoSecond ( BYTE * ptr );
WORD    COMUTL_HMSFtoSecond ( void );   // ** TCH1.72r; 
// ** TCH0.39; end...

DWORD   COMUTL_SecondtoHMSF ( void );       // ** TCH1.72r; 

//LLY.172r, void    COMUTL_Printf ( char * pS, ... );

// LLY.043, YUV <--> RGB conversion
//Brian2.81
DWORD COMUTL_RGB2YUV(DWORD dwRGB);
DWORD   COMUTL_YUV2RGB(WORD wYUV);
// LLY.043 end ...

//VCD_080
DWORD   COMUTL_ReadBufferDWORD ( char * pStr, int wOffset );
WORD    COMUTL_ReadBufferWORD ( char * pStr, int wOffset );
// ** TCH1.55; BYTE    COMUTL_ReadBufferBYTE ( char * pStr, int wOffset );

WORD     COMUTL_rand (void); // ** TCH1.72q; // ** TCH1.03-1; 

// ** TCH1.72;
void    COMUTL_BYTE2HexStr ( BYTE * pStr, BYTE bVal );

// ** LLY.275b-2
#ifdef  _DEBUG_INFO
void COMUTL_DWORD2HexStr(BYTE * pStr, DWORD dwVal);
#endif  // #ifdef _DEBUG_INFO


#endif  // __COMUTL_H__


