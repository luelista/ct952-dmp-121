// Micky1.20, add NO_FLOAT_POINT, NO_DIRECT_OPERATION, NO_SHIFT
// Micky1.07, remove CHIP_W9928 for 28AF related codes

#include    "winav.h"

#include    "comutl.h"

// LLY.043, define some global variable for temporal usage
BYTE    _bCOMUTLTmp1, _bCOMUTLTmp2, _bCOMUTLTmp3;

//!!! __dwCOMUTLTemp _wCOMUTLTemp and _bCOMUTLTmp1/2/3, uDword
// will be used at the same time
DWORD   __dwCOMUTLTemp;
UDWORD      uDWord;
UWORD      uWord;
// porting from DVDINFO
//***************************************************************************
//  Function    :   ::COMUTL_ReadBufferDWORD
//  Abstract    :
//  Arguments   :   pStr                : Destination string pointer
//                  wOffset             : The starting offse of unit
//  Return      :   Getting Value (DWORD)
//  Side Effect :
//  Notes       :
//***************************************************************************
DWORD   COMUTL_ReadBufferDWORD ( char * pStr, int wOffset )
{
#ifdef    USE_LITTLE_ENDIAN
    uDWord. cS[3]= pStr[ wOffset ];
    uDWord. cS[2]= pStr[ wOffset+ 1];
    uDWord. cS[1]= pStr[ wOffset+ 2];
    uDWord. cS[0]= pStr[ wOffset+ 3];
#else
    uDWord. cS[3]= pStr[ wOffset+ 3];
    uDWord. cS[2]= pStr[ wOffset+ 2];
    uDWord. cS[1]= pStr[ wOffset+ 1];
    uDWord. cS[0]= pStr[ wOffset+ 0];
#endif    // USE_LITTLE_ENDIAN

    return  uDWord. dwVal;
}

//***************************************************************************
//  Function    :   ::COMUTL_ReadBufferWORD
//  Abstract    :
//  Arguments   :   pStr                : Destination string pointer
//                  wOffset             : The starting offse of unit
//  Return      :   Getting Value (WORD)
//  Side Effect :
//  Notes       :
//***************************************************************************
WORD    COMUTL_ReadBufferWORD ( char * pStr, int wOffset )
{

#ifdef    USE_LITTLE_ENDIAN
    uWord. cS[1]= pStr[ wOffset ];
    uWord. cS[0]= pStr[ wOffset+ 1];
#else
    uWord. cS[1]= pStr[ wOffset+ 1 ];
    uWord. cS[0]= pStr[ wOffset ];
#endif    // USE_LITTLE_ENDIAN

    return  uWord. nVal;
}

//  *********************************************************************
//  Function    :   COMUTL_DecimaltoBCD
//  Description :       Convert the decimal value into BCD value
//  Arguments   :   bVal        : decimal value
//  Return      :   BCD value
//  Side Effect :
//  *********************************************************************
BYTE    COMUTL_DecimaltoBCD ( BYTE bVal )
{
        return ((bVal%10) & 0x0F) | (((bVal/10) << 4) & 0xF0);
}

//  *********************************************************************
//  Function    :   COMUTL_BCDtoDecimal
//  Description :       Convert the BCD value into decimal
//  Arguments   :   bVal        : BCD value
//  Return      :   Decimal value
//  Side Effect :
//  *********************************************************************
BYTE    COMUTL_BCDtoDecimal ( BYTE bVal )
{
        return (bVal&0x0F) + ((bVal&0xF0) >> 4) * 10;
}

//  *********************************************************************
//  Function    :   COMUTL_MSFToHSG
//  Description :       Convert the time format from mm:ss:ff into HSG(sector)
//  Arguments   :       msftime : 0f:sm
//  Return      :       the converted HSG value
//  Side Effect :
//  *********************************************************************
DWORD       COMUTL_MSFtoHSG ( DWORD msfTime )
{

    uDWord.cS[0]= MSF_MINUTE ( msfTime );
    uDWord.cS[1]= MSF_SECOND ( msfTime );
    uDWord.cS[2]= MSF_FRAME ( msfTime );

    // HSG = mm*75L*60L + ss*75L + ff - 150L
    __dwCOMUTLTemp= uDWord.cS[0]* 75L* 60L+ uDWord.cS[1]* 75L+ uDWord.cS[2];

    if (__dwCOMUTLTemp >150)
        __dwCOMUTLTemp -= 150L;
    else
        __dwCOMUTLTemp=0L;

    return __dwCOMUTLTemp;
}

//  *********************************************************************
//  Function    :   COMUTL_HSGToMSF
//  Description :   Convert the time format from HSG(sector) into mm:ss:ff
//  Arguments   :
//  Return      :   The converted mm:ss:ff value, 0f:sm
//  Side Effect :
//  *********************************************************************
DWORD       COMUTL_HSGtoMSF ( DWORD hsgTime )
{

    hsgTime+= 150L;

    __dwCOMUTLTemp= hsgTime/ 75;

    uDWord.cS[0]=(BYTE)(__dwCOMUTLTemp/60);
    uDWord.cS[1]=(BYTE)(__dwCOMUTLTemp%60);     // MAX. is 59
    uDWord.cS[2]=(BYTE)(hsgTime%75);

    // convert mm:ss:ff into DWORD format xf:sm
    __dwCOMUTLTemp= MAKE_MSF ( uDWord.cS[0], uDWord.cS[1], uDWord.cS[2] );

    return __dwCOMUTLTemp;
}


// **************************************************************************
//  Function    :   COMUTL_Delay
//  Description :   Loop for delay a moment
//  Arguments   :   wLoop:the loop time
//  Return      :   none
//  Side Effect :
// **************************************************************************
#ifdef  NOT_SUPPORT_TIMER
void COMUTL_Delay(WORD wLoop)
{
    DWORD   i;
    for(i=0;i<wLoop;i++);
}
#endif      // NOT_SUPPORT_TIMER

// ** TCH0.38; begin...
// **************************************************************************
//  Function    :   COMUTL_StrUpr
//  Description :   Transfer Lowercase to Uppercase; other is no effect.
//  Arguments   :   pS      : char pointer
//  Return      :   none
//  Side Effect :
// **************************************************************************
void COMUTL_StrUpr ( char * pS )
{
    for ( _bCOMUTLTmp1=0; _bCOMUTLTmp1< 255; _bCOMUTLTmp1++ )
    {
        if ( ! (pS[_bCOMUTLTmp1] ) )
        {
            break;
        }

        if ( (pS[_bCOMUTLTmp1] >= 97) && (pS[_bCOMUTLTmp1] <= 122) )
        {
// Micky1.20, 4M flash, reduce common bank lib.
// use Array[]-=xx will invoke C51 lib (?C?CILDOPTR), 34 bytes
#ifdef  NO_DIRECT_OPERATION
            _bCOMUTLTmp3 = pS[_bCOMUTLTmp1];
            _bCOMUTLTmp3 -= 32;
            pS[_bCOMUTLTmp1] = _bCOMUTLTmp3;
#else
            pS[_bCOMUTLTmp1]-= 32;     // Lowercase to Uppercase.
#endif
        }
    }
}

// **************************************************************************
//  Function    :   COMUTL_SwapDWORD
//  Description :   Swap a DWORD by BYTE unit.
//  Arguments   :   pdwPtr   : DWORD pointer
//  Return      :   none
//  Side Effect :
// **************************************************************************
void COMUTL_SwapDWORD ( DWORD * pdwPtr )
{

    __dwCOMUTLTemp= *pdwPtr;
    for ( _bCOMUTLTmp1= 0; _bCOMUTLTmp1< 4 ;_bCOMUTLTmp1++ )
    {
        uDWord.cS[_bCOMUTLTmp1]= (BYTE) (__dwCOMUTLTemp>> (_bCOMUTLTmp1*8) );
    }

    __dwCOMUTLTemp= 0;
    for ( _bCOMUTLTmp1= 0; _bCOMUTLTmp1< 4 ;_bCOMUTLTmp1++ )
    {
        __dwCOMUTLTemp |= ((DWORD)uDWord.cS[_bCOMUTLTmp1])<< ((3-_bCOMUTLTmp1)*8);
    }

    * pdwPtr= __dwCOMUTLTemp;
}

// wyc1.01-909, add one function to swap WORD for FAT usage.
// **************************************************************************
//  Function    :   COMUTL_SwapWORD
//  Description :   Swap a WORD by BYTE unit.
//  Arguments   :   pwPtr   : WORD pointer
//  Return      :   none
//  Side Effect :
// **************************************************************************
void COMUTL_SwapWORD ( WORD * pwPtr )
{

    __dwCOMUTLTemp= *pwPtr;
    for ( _bCOMUTLTmp1= 0; _bCOMUTLTmp1< 2 ;_bCOMUTLTmp1++ )
    {
        uDWord.cS[_bCOMUTLTmp1]= (BYTE) (__dwCOMUTLTemp>> (_bCOMUTLTmp1*8) );
    }

    __dwCOMUTLTemp= 0;
    for ( _bCOMUTLTmp1= 0; _bCOMUTLTmp1< 2 ;_bCOMUTLTmp1++ )
    {
        __dwCOMUTLTemp |= ((WORD)uDWord.cS[_bCOMUTLTmp1])<< ((1-_bCOMUTLTmp1)*8);
    }

    * pwPtr= (WORD)__dwCOMUTLTemp;
}


// ** TCH0.39; begin...
//***************************************************************************
//  Function    :   ::COMUTL_HMStoSecond
//  Abstract    :
//  Arguments   :   ptr                 :
//  Return      :   Second NO.
//  Side Effect :
//  Notes       :
//***************************************************************************
WORD    COMUTL_HMStoSecond ( BYTE * ptr )
{
    uWord.nVal= 0;

    uDWord.cS[0]= COMUTL_BCDtoDecimal ( ptr[0] );
    uWord.nVal+= 3600* (WORD) uDWord.cS[0];

    uDWord.cS[0]= COMUTL_BCDtoDecimal ( ptr[1] );
    uWord.nVal+= 60* (WORD) uDWord.cS[0];

    uDWord.cS[0]= COMUTL_BCDtoDecimal ( ptr[2] );
    uWord.nVal+= (WORD) uDWord.cS[0];

    return  uWord.nVal;
}

//***************************************************************************
//  Function    :   ::COMUTL_HMSFtoSecond
//  Abstract    :
//  Arguments   :   ptr                 :
//  Return      :   Second NO.
//  Side Effect :
//  Notes       :
//***************************************************************************
// ** TCH1.72r; WORD    COMUTL_HMSFtoSecond ( DWORD dwHMSF )
WORD    COMUTL_HMSFtoSecond ( void )
{
    uWord.nVal= 0;

///    uDWord.cS[0]= COMUTL_BCDtoDecimal ( HMSF_HOUR (dwHMSF) );
    uDWord.cS[0]= ( HMSF_HOUR (__dwCOMUTLTemp) );
    uWord.nVal+= 3600* (WORD) uDWord.cS[0];

///    uDWord.cS[0]= COMUTL_BCDtoDecimal ( HMSF_MINUTE (dwHMSF) );
    uDWord.cS[0]= ( HMSF_MINUTE (__dwCOMUTLTemp) );
    uWord.nVal+= 60* (WORD) uDWord.cS[0];

///    uDWord.cS[0]= COMUTL_BCDtoDecimal ( HMSF_SECOND (dwHMSF) );
    uDWord.cS[0]= ( HMSF_SECOND (__dwCOMUTLTemp) );
    uWord.nVal+= (WORD) uDWord.cS[0];

    return  uWord.nVal;
}
// ** TCH0.39; end...

// ** TCH1.02; begin...
//***************************************************************************
//  Function    :   ::COMUTL_SecondtoHMSF
//  Abstract    :
//  Arguments   :   dwSecond    : Unit: Second
//  Return      :   HMSF Format
//  Side Effect :
//  Notes       :
//***************************************************************************
// ** TCH1.72r; DWORD    COMUTL_SecondtoHMSF ( DWORD dwSecond )
DWORD    COMUTL_SecondtoHMSF ( void )

{
    _bCOMUTLTmp1= (BYTE)(__dwCOMUTLTemp % 60);   // second
    __dwCOMUTLTemp= __dwCOMUTLTemp/ 60;    // min.

    return  MAKE_HMSF (__dwCOMUTLTemp/60, __dwCOMUTLTemp%60, _bCOMUTLTmp1, 0 );
}
// ** TCH1.02; end...

// LLY.043 modify ...
//  *************************************************************************
//  Function    :   COMUTL_RGB2YUV
//  Description :   Convert RGB to YUV format which match OSD spec.
//  Arguments   :   dwRGB, specify RGB value
//                  [31:24]: don't care, [23:16]: R value
//                  [15:8]: G value, [7:0]: B value
//  Return      :   YUV value, it has calculated to match OSD spec.
//  Side Effect :
//  *************************************************************************
//Brian2.81
DWORD COMUTL_RGB2YUV(DWORD dwRGB)
{
	BYTE _bCOMUTLTmp1, _bCOMUTLTmp2, _bCOMUTLTmp3;
	UDWORD uDWord;
	DWORD __dwCOMUTLTemp;
	
    // use uDWord.cS[0] ~ [3] to store YUV value
    // use _wCOMUTLTemp to store final value

    // Step 0: split RGB value
    _bCOMUTLTmp1=(RGB_R(dwRGB));
    _bCOMUTLTmp2=(RGB_G(dwRGB));
    _bCOMUTLTmp3=(RGB_B(dwRGB));

// Micky1.20, 4M flash, reduce common bank lib.
// don't use float point.
// can't use DWORD, must use signed operation
#ifdef  NO_FLOAT_POINT
    // Step 1: Calculate YUV value depending on input RGB

    uDWord.cS[0]=(BYTE)(16+(65738L*(long)_bCOMUTLTmp1
                      +129057L*(long)_bCOMUTLTmp2
                      +25064L*(long)_bCOMUTLTmp3)/256000);  // Y value
                       
    //uDWord.cS[1]=(BYTE)(128+(-37945L*(long)_bCOMUTLTmp1
    //                   -74494L*(long)_bCOMUTLTmp2
    //                   +112439L*(long)_bCOMUTLTmp3)/256000); // U value

    __dwCOMUTLTemp = 32768000 + (-37945L*(long)_bCOMUTLTmp1
                       -74494L*(long)_bCOMUTLTmp2
                       +112439L*(long)_bCOMUTLTmp3);
    uDWord.cS[1] =  (BYTE)(__dwCOMUTLTemp/256000);  // U value


    //uDWord.cS[2]=(BYTE)(128+(112439L*(long)_bCOMUTLTmp1
    //                   -94154L*(long)_bCOMUTLTmp2
    //                   -18285L*(long)_bCOMUTLTmp3)/256000);  // V value


    __dwCOMUTLTemp= 32768000+(112439L*(long)_bCOMUTLTmp1
                       -94154L*(long)_bCOMUTLTmp2
                       -18285L*(long)_bCOMUTLTmp3);  
    uDWord.cS[2] =  (BYTE)(__dwCOMUTLTemp/256000); // V value
                       
// debug the value between float and long
// float > long by 1.
/*
    uWord.cS[0]=(BYTE)(16+(65738*(float)_bCOMUTLTmp1
                      +129057*(float)_bCOMUTLTmp2
                      +25064*(float)_bCOMUTLTmp3)/256000);  // Y value
    uWord.cS[1]=(BYTE)(128+(-37945*(float)_bCOMUTLTmp1
                       -74494*(float)_bCOMUTLTmp2
                       +112439*(float)_bCOMUTLTmp3)/256000); // U value
    uWord.cS[1]=(BYTE)(128+(112439*(float)_bCOMUTLTmp1
                       -94154*(float)_bCOMUTLTmp2
                       -18285*(float)_bCOMUTLTmp3)/256000);  // V value
    if (uDWord.cS[2] != uWord.cS[1])
        printf("\nError");
*/
#else

    // Step 1: Calculate YUV value depending on input RGB
    uDWord.cS[0]=(BYTE)(16+(65738*(float)_bCOMUTLTmp1
                      +129057*(float)_bCOMUTLTmp2
                      +25064*(float)_bCOMUTLTmp3)/256000);  // Y value

    uDWord.cS[1]=(BYTE)(128+(-37945*(float)_bCOMUTLTmp1
                       -74494*(float)_bCOMUTLTmp2
                       +112439*((float)_bCOMUTLTmp3)/256000)); // U value
    uDWord.cS[2]=(BYTE)(128+(112439*(float)_bCOMUTLTmp1
                       -94154*(float)_bCOMUTLTmp2
                       -18285*((float)_bCOMUTLTmp3)/256000));  // V value
#endif

    // Step 2: merge YUV value into DWORD unit and match OSD spec.
	__dwCOMUTLTemp=(MAKE_RGB(uDWord.cS[0], uDWord.cS[1], uDWord.cS[2]));
    return  __dwCOMUTLTemp;

}


//  *************************************************************************
//  Function    :   COMUTL_YUV2RGB
//  Description :   Convert YUV which match OSD spec. to RGB format
//  Arguments   :   wYUV, specify YUV value (match OSD spec.)
//                  [15:10]: Y[7:2], [9:8]: 0x0
//                  [7:4]: U[7:4], [3:0]: V[7:4]
//  Return      :   RGV value, [31:24] is don't care, other is RGB in BYTE unit
//  Side Effect :
//  *************************************************************************
DWORD COMUTL_YUV2RGB(WORD wYUV)
{
    // use uDWord.cS[0] ~ [3] to store RGB value
    // use __dwCOMUTLTemp to store final value

    // Step 0: split YUV value
    _bCOMUTLTmp1=(YUV_Y(wYUV));
    _bCOMUTLTmp2=(YUV_U(wYUV));
    _bCOMUTLTmp3=(YUV_V(wYUV));

// Micky1.20, 4M flash, reduce common bank lib.
// don't use float point
// can't use DWORD, must use signed operation
#ifdef  NO_FLOAT_POINT
    // Step 1: Calculate RGB value depending on input YUV
    uDWord.cS[0]=(BYTE)((298082L*(long)_bCOMUTLTmp1
                    +408583L*(long)_bCOMUTLTmp3)/256000-16);  // R value
    uDWord.cS[1]=(BYTE)((298082L*(long)_bCOMUTLTmp1
                    -100291L*(long)_bCOMUTLTmp2
                    -208120L*(long)_bCOMUTLTmp3)/256000-128);  // G value
    uDWord.cS[2]=(BYTE)((298082L*(long)_bCOMUTLTmp1
                    +516411L*(long)_bCOMUTLTmp2)/256000-128);  // B value
// debug the value between float and long
/*
    uWord.cS[0]=(BYTE)((298082*(float)_bCOMUTLTmp1
                    +0*(float)_bCOMUTLTmp2
                    +408583*(float)_bCOMUTLTmp3)/256000-16);  // R value
    uWord.cS[1]=(BYTE)((298082*(float)_bCOMUTLTmp1
                    -100291*(float)_bCOMUTLTmp2
                    -208120*(float)_bCOMUTLTmp3)/256000-128);  // G value
    uDWord.cS[1]=(BYTE)((298082*(float)_bCOMUTLTmp1
                    +516411*(float)_bCOMUTLTmp2
                    +0*(float)_bCOMUTLTmp3)/256000-128);  // B value
    if (uDWord.cS[2] != uWord.cS[1])
        printf("\nError");
*/
#else
    // Step 1: Calculate RGB value depending on input YUV
    uDWord.cS[0]=(BYTE)((298082*(float)_bCOMUTLTmp1
                    +0*(float)_bCOMUTLTmp2
                    +408583*(float)_bCOMUTLTmp3)/256000-16);  // R value
    uDWord.cS[1]=(BYTE)((298082*(float)_bCOMUTLTmp1
                    -100291*(float)_bCOMUTLTmp2
                    -208120*(float)_bCOMUTLTmp3)/256000-128);  // G value
    uDWord.cS[2]=(BYTE)((298082*(float)_bCOMUTLTmp1
                    +516411*(float)_bCOMUTLTmp2
                    +0*(float)_bCOMUTLTmp3)/256000-128);  // B value
#endif

    // Step 2: merge RGB value into WORD unit and match OSD spec.
    __dwCOMUTLTemp=(MAKE_RGB(uDWord.cS[0], uDWord.cS[1], uDWord.cS[2]));

    return __dwCOMUTLTemp;
}
// LLY.043 end ...

//  *************************************************************************
//  Function    :   COMUTL_rand
//  Description :   generate a random value from
//  Arguments   :   NONE
//  Return      :   Range from 0~32767
//  Side Effect :
//  *************************************************************************
// ** TCH1.03-1; begin... 
long  _lHoldrand;//Micky2.80, review global variables init.=1L; 

WORD COMUTL_rand ()
{
// Micky1.20, 4M flash, reduce common bank lib.
// It will invoke C51 lib (?C?SLSHR), 20 bytes
#ifdef  NO_SHIFT
  return (WORD)(((_lHoldrand = _lHoldrand * 214013L + 2531011L) / 65536L) & 0x7fff);
#else
  return (WORD)(((_lHoldrand = _lHoldrand * 214013L + 2531011L) >> 16) & 0x7fff);
#endif
}
// ** TCH1.03-1; end... 

//  *************************************************************************
//  Function    :   COMUTL_BYTE2HexStr
//  Description :   generate a hexial string from BYTE value
//  Arguments   :   pStr    : String pointer
//                  bVal    : Value (BYTE)
//  Return      :   void
//  Side Effect :
//  ************************************************************************
// ** TCH1.72; begin... 
BYTE    code    MAP_Hex2ASCII[]= {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' }; 
void COMUTL_BYTE2HexStr ( BYTE * pStr, BYTE bVal )
{
    for ( _bCOMUTLTmp1=0 ; _bCOMUTLTmp1< 2; _bCOMUTLTmp1++ )
    {
        if ( ! _bCOMUTLTmp1 )
            _bCOMUTLTmp2= bVal >> 4;
        else
            _bCOMUTLTmp2= bVal;
        _bCOMUTLTmp2&= 0x0F;
        pStr [_bCOMUTLTmp1]= MAP_Hex2ASCII [_bCOMUTLTmp2];
    }
}
// ** TCH1.72; end...


// LLY.275b-2 create
//  *************************************************************************
//  Function    :   COMUTL_DWORD2HexStr
//  Description :   generate a hexial string from DWORD value
//  Arguments   :   pStr    : String pointer
//                  dwVal    : Value (DWORD unit)
//  Return      :   void
//  Side Effect :
//  ************************************************************************
#ifdef  _DEBUG_INFO
void COMUTL_DWORD2HexStr(BYTE * pStr, DWORD dwVal)
{
    for(_bCOMUTLTmp1=0; _bCOMUTLTmp1<8; _bCOMUTLTmp1++)
    {
        // Abstract the highest 4 bit by "right-shift" operation
        _bCOMUTLTmp2=(BYTE)(dwVal>>((7-_bCOMUTLTmp1)*4));

        // Only abstract the [3:0] bits from the BYTE unit
        _bCOMUTLTmp2 &= 0x0F;

        pStr[_bCOMUTLTmp1]=MAP_Hex2ASCII[_bCOMUTLTmp2];
    }
}
#endif  // #ifdef _DEBUG_INFO

