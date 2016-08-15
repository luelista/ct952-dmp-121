#include "winav.h"
#include "aploader.h"
#include "ctkav.h"
#include "int_gbl.h"
#include "input.h"
#include "ir.h"
#include "utl.h"
#include "cc.h"

volatile BYTE __bISRKey = KEY_NO_KEY; 

IR_INFO _IRInfo;

#if (IR_CODE_TYPE == SW_RC5 || IR_CODE_TYPE == SW_NEC)
BYTE bIRCheck = 0;
#endif  // #if (IR_CODE_TYPE == SW_RC5 || IR_CODE_TYPE == SW_NEC)

//hwtan1209 begin, for fine tune SW IR receiving
#if (IR_CODE_TYPE == SW_KONKA)
BYTE bIRStep = 0;  // hwtan1123
BYTE bDataCode = 0;    //wx.051123
BYTE bIRFlag = 0;  // 1:  has one IR key input   0: none
BYTE bFallingFlag = 0;  // 
BYTE bPrevEdge = 0xff;
BYTE bIRCheck = 0; // hwtan1213test
WORD    IR_code[19]; // hwtan1209
#endif  // #if (IR_CODE_TYPE == SW_KONKA)

BYTE    _bKeyGet;
BYTE    _bISRKeyPrev = KEY_NO_KEY; 
BYTE    _bCustomerCode = 0;
BYTE    _bCustomerCode1 = 0;

// LLY2.36, keep current IR check mode.
BYTE    _bIRCheckMode=IR_CHECK_MODE_INT;
// LLY2.36, keep previous IR repeat flag
BYTE    _bPreRepeatFlag=1;

DWORD   _dwTimeISRKeyPrev = 0;
DWORD   _dwTimeISRKeyPrev2 = 0;

extern PAP_INFO_ADDITIONAL     AP_Info_Additional;

#ifdef SUPPORT_GAME_IR
#ifdef IR_CODE_TYPE
    #undef  IR_CODE_TYPE
    #define IR_CODE_TYPE    HW_NEC
#endif  // #ifdef IR_CODE_TYPE
BYTE    _blGameIRMode = FALSE;
volatile WORD __wISRGameKey = 0x0000;
#endif  // #ifdef SUPPORT_GAME_IR

// LLY2.38, keep IR working mode when power down.
#if (IR_CODE_TYPE != HW_NEC)
#define KEEP_IR_WORKING_MODE_WHEN_POWERDOWN
#endif  // #if (IR_CODE_TYPE == HW_NEC)

VOID    _ResetIR(VOID); 

#if (PLATFORM == ECOS_OS)
VOID    ISR_IRSaveClearStatus( VOID )
{
// =============================================================================
#if (IR_CODE_TYPE == SW_RC5)
    #define FULL_WINDOW_TIME    (178)
    #define MAX_TIME            (FULL_WINDOW_TIME)
    #define HALT_WINDOW_TIME    ((FULL_WINDOW_TIME + 1) / 2)
    #define GAP                 ((FULL_WINDOW_TIME + 2) / 4)
    #define GREAT(x, y)         ((x) > ((y) + (GAP)))
    #define LESS(x, y)          ((x) < ((y) - (GAP)))
    #define EQUAL(x, y)         ((!GREAT((x), (y))) && (!LESS((x), (y))))

    #define IR_STATUS_RISING    (0x10)
    #define IR_STATUS_FALLING   (0x20)
    #define IR_STATUS_WIN_BEGIN (0x40)

    static BYTE bBitCnt = 0, bPrevIRStatus;
    static WORD wIRCode;
    BYTE bIRStatus;
    WORD wIRTime;

    bIRStatus = REG_PLAT_IR_STATUS;
    if(bIRStatus & 0x10)
    {
        wIRTime = REG_PLAT_IR_COUNTER >> 16;
    }
    else
    {
        wIRTime = REG_PLAT_IR_COUNTER;
    }

    do
    {
        if(((bIRStatus & bPrevIRStatus) & 0x30) == 0)    // edge rising or falling must be alternative
        {
            if(EQUAL(wIRTime, HALT_WINDOW_TIME))
            {
                if(!(bPrevIRStatus & IR_STATUS_WIN_BEGIN))
                {
                    bIRStatus |= IR_STATUS_WIN_BEGIN;
                    wIRCode |= ((bIRStatus & IR_STATUS_RISING) ? 0 : 1) << bBitCnt;
                    ++bBitCnt;
                }
                break;
            }
            else if(EQUAL(wIRTime, FULL_WINDOW_TIME))
            {
                if(!(bPrevIRStatus & IR_STATUS_WIN_BEGIN))
                {
                    wIRCode |= ((bIRStatus & IR_STATUS_RISING) ? 1 : 0) << bBitCnt;
                    ++bBitCnt;
                    break;
                }
            }
        }

        // begin of code or error status
        wIRCode = bBitCnt = 1;
    }
    while(0);

    bPrevIRStatus = bIRStatus;

    if(bBitCnt >= 14)
    {
        if(((wIRCode >> 3) & 0x1f) == CUSTOMER_CODE)
        {
            DWORD dwTimeISRKey = OS_GetSysTimer();
            if((dwTimeISRKey - _dwTimeISRKeyPrev) >= COUNT_200_MSEC)
            {
                BYTE bIRDataCode = wIRCode >> 8;
                _bKeyGet = bIRDataCode < _IRInfo.bIRMapSize ? _IRInfo.aIRMap[bIRDataCode] : KEY_NO_KEY;

                bIRCheck = !((dwTimeISRKey - _dwTimeISRKeyPrev) < COUNT_300_MSEC);
                _dwTimeISRKeyPrev = dwTimeISRKey;

                __pINTDSRRoutine[INT_NO10][__dwINTDSRCounter[INT_NO10] ++] = DSR_IR;
            }
        }
        wIRCode = bBitCnt = 0;
    }
    return;
#endif  // #if (IR_CODE_TYPE == SW_RC5)
// =============================================================================

// =============================================================================
#if (IR_CODE_TYPE == SW_KONKA)
  WORD  wIRTemp;
  DWORD dwTimeISRKey;  // hwtan1213
    
//***********************************************
// the waveform of Konka IR is below:
//   ___       
//  |    |   |    leader code:  3ms high + 3ms low
//  |    |___|
//  _
// ||  |
// ||__|         "0" :   0.5ms high + 1.5ms low
//  _
// ||    |
// ||____|     "1" :  0.5ms high + 2.5ms low
//***********************************************

    if (bIRFlag)  return; // if DSR is processing , do nothing
    if (!bIRStep)    // 0 : just begin receive IR
    {   
        wIRTemp =LOWORD(REG_PLAT_IR_COUNTER);
        if (wIRTemp<320 && wIRTemp >280)  // high 3ms means the valid leader code
            bIRStep = 1; 
     }
     else if (bIRStep==1)  //   1:  check the leader code's low 3ms
     {
         wIRTemp =  HIWORD(REG_PLAT_IR_COUNTER);
         if (wIRTemp<320 && wIRTemp >280)  // low 3ms match  300
             bIRStep++;     // begine receive IR
         else 
             bIRStep = 0;   // if the leader code is invalid, its noise, reset the step to 0
    }
    else   // receive the customer/data/sync code
    {
        wIRTemp =  HIWORD(REG_PLAT_IR_COUNTER); // only focus on the low level 1.5ms for 0, 2.5ms for 1
        if (wIRTemp > 130 && wIRTemp < 270)    // its valid data bit:  0  or   1
        {  
            bIRStep ++;
            IR_code[bIRStep] = wIRTemp;
        }
        else if (wIRTemp > 330 && wIRTemp < 420)  // the sync bit, low lenght = 4ms, means one IR key finish
        {
            if (bIRStep == 18)   //  2bit leader + 8 bit customer code + 8bit data code = 18
            { 
                bIRStep ++; // 19 is a flag, means one IR key finish, let DSR_IR  parse the data in IR_Code[] to get key
                dwTimeISRKey = OS_GetSysTimer(); // get current time
                if ( (dwTimeISRKey- _dwTimeISRKeyPrev) < COUNT_200_MSEC ) 
                    bIRCheck = 0; // for repeat IR key, do not receive it
                else
                    bIRCheck = 1;   
               _dwTimeISRKeyPrev = dwTimeISRKey;
            }
        }
    }
    __pINTDSRRoutine[INT_NO10][__dwINTDSRCounter[INT_NO10] ++] = DSR_IR;
#endif  // #if (IR_CODE_TYPE == SW_KONKA)
// =============================================================================

// =============================================================================
#if (IR_CODE_TYPE == SW_NEC)
    #define IR_STATUS_RISING    (0x10)
    #define IR_STATUS_FALLING   (0x20)

    static BYTE bState = 0;
    static BYTE bBitCnt = 0;
    static DWORD dwIRCode;
    BYTE bIRStatus;
    WORD wIRTime;

    bIRStatus = REG_PLAT_IR_STATUS;
    wIRTime = REG_PLAT_IR_COUNTER >> ((bIRStatus & IR_STATUS_RISING)? 16: 0);

    if((bIRStatus & IR_STATUS_FALLING) && wIRTime > 800)
    {
        bState = 2;
    }
    else
    {
        switch(bState)
        {
/*
            case 0: // wait for rising (L >= 25ms)
                if((bIRStatus & IR_STATUS_RISING) && wIRTime > 2500)
                {
                    ++bState;
                }
                break;
    
            case 1: // wait for falling (H == 9ms)
                if((bIRStatus & IR_STATUS_FALLING) && wIRTime > 800 && wIRTime < 1000)
                {
                    ++bState;
                }
                else
                {
                    bState = 0;
                }
                break;
*/
            case 2: // wait for rising (L = 4.5ms for normal, L = 2.25ms for repeat)
                if((bIRStatus & IR_STATUS_RISING) && wIRTime > 112 && wIRTime < 563)
                {
                    if(wIRTime > 337)
                    {
                        ++bState;
                        bBitCnt = 0;    // prepare for bit receiving
                    }
                    else
                    {
                        bState = 5; // code completed for repeat
                    }
                }
                else
                {  
                    bState = 0;
                }
                break;
    
            case 3: // wait for falling (H = 0.56ms)
                if((bIRStatus & IR_STATUS_FALLING) && wIRTime > 28 && wIRTime < 84)
                {
                    ++bState;
                }
                else
                {
                    bState = 0;
                } 
                break;
    
            case 4: // wait for rising (L = 0.56ms for '0', L = 1.69ms for '1')
                if((bIRStatus & IR_STATUS_RISING) && wIRTime < 225)
                {
                    dwIRCode = (dwIRCode >> 1) | ((wIRTime > 112) << 31);
    
                    if(++bBitCnt < 32)
                    {
                        --bState;
                    }
                    else
                    {
                        ++bState;
                    }
                }
                else
                {
                    bState = 0;
                }
                break;

            case 5: // code completed for normal and repeat
                if(bBitCnt >= 32 && (dwIRCode & 0xffff) == (CUSTOMER_CODE | (CUSTOMER_CODE1 << 8)))
                {
                    DWORD dwTimeISRKey = OS_GetSysTimer();
                    if((dwTimeISRKey - _dwTimeISRKeyPrev) >= COUNT_200_MSEC)
                    {
                        BYTE bIRDataCode = dwIRCode >> 16;
                        bIRCheck = !((dwTimeISRKey - _dwTimeISRKeyPrev) < (COUNT_250_MSEC + COUNT_300_MSEC) / 2);
                        _dwTimeISRKeyPrev = dwTimeISRKey;
                        _bKeyGet = bIRDataCode < _IRInfo.bIRMapSize ? _IRInfo.aIRMap[bIRDataCode] : KEY_NO_KEY;
                        __pINTDSRRoutine[INT_NO10][__dwINTDSRCounter[INT_NO10] ++] = DSR_IR;
                    }
                }
                bState = 0;
                break;
        }
    }
#endif  // #if (IR_CODE_TYPE == SW_NEC)
// =============================================================================

// =============================================================================
#if (IR_CODE_TYPE == HW_NEC)
    // save status
    __INTISRintProc12ndStatus.dwPLAT_IR_DATA = REG_PLAT_IR_DATA;
    __INTISRintProc12ndStatus.dwPLAT_IR_RAW_CODE = REG_PLAT_IR_RAW_CODE;
#ifdef RESET_IR_AFTER_IR_INT
#ifdef SUPPORT_GAME_IR
    if (_blGameIRMode)
    {
        INPUT_SetGameIR();
    }
    else
    {
        _ResetIR();
    }
#else   // #ifdef SUPPORT_GAME_IR
    //kevin0.75, reset IR
    _ResetIR();
#endif  // #ifdef SUPPORT_GAME_IR
#endif  // #ifdef RESET_IR_AFTER_IR_INT
    // set function pointer for ISR and DSR
    __pINTDSRRoutine[INT_NO10][__dwINTDSRCounter[INT_NO10] ++] = DSR_IR;
#endif  // #if (IR_CODE_TYPE == HW_NEC)
// =============================================================================
}
#endif  // #if (PLATFORM == ECOS_OS)

// ***********************************************************************
//  Function    :   DSR_IR
//  Description :   Read remote controller input when IR INT is triggered
//  Arguments   :   NONE
//  Return      :   input key
//  Side Effect :
// *********************************************************************** 
VOID    DSR_IR(VOID)
{
// =============================================================================
#if (IR_CODE_TYPE == SW_KONKA)
    BYTE i;

    if(bIRStep!=19)  
          return;   // not reach 19, do nothing
    bIRFlag = 1;      // means in receiving IR code now{
    bIRStep = 0;  // reach 19, reset the step first
    bDataCode = 0;
    _bCustomerCode = 0;
// the timing: 100 = 1ms
    for (i=0;i<8;i++)
    {
        if (IR_code[i+3] >= 230 && IR_code[i+3] <= 270)  // 2.5ms low  = 1   1.5ms low = 0
            _bCustomerCode |=  (0x80 >> i);
        else if(IR_code[i+3] >= 130 && IR_code[i+3] <= 200) // 170
            _bCustomerCode &=  ~(0x80 >> i);
        else  
    { 
        _bKeyGet = KEY_NO_KEY;
        bIRFlag = 0;   // process finish
            return ;
    }
    }

    if (_bCustomerCode == 0x06)  // the customer code for Konka IR
    {
        for (i=0;i<8;i++)
        {
            if (IR_code[i+11] >= 230 && IR_code[i+11] <= 270)  // is 1  2.5ms  low, 250
                bDataCode |=  (0x80 >> i);
            else if(IR_code[i+11] >= 130 && IR_code[i+11] <= 200)  // 170
                _bCustomerCode &=  ~(0x80 >> i);
            else  
            { 
                _bKeyGet = KEY_NO_KEY;
        bIRFlag = 0;   // process finish
                return;
            }
        }
//Alan2.05        _bKeyGet = aIRMap[bDataCode]; 
        _bKeyGet = _IRInfo.aIRMap[bDataCode]; 
    }
    else    //if(_bCustomerCode == 0x06)
        _bKeyGet = KEY_NO_KEY;
#endif  // #if (IR_CODE_TYPE == SW_KONKA)
// =============================================================================

// =============================================================================
#if (IR_CODE_TYPE == HW_NEC)
    DWORD dwTimeISRKey; 

    dwTimeISRKey = OS_GetSysTimer();
#ifdef SUPPORT_GAME_IR
    if (_blGameIRMode)
    {
        _bKeyGet = INPUT_Game_RemoteScan();
    }
    else
#endif  // #ifdef SUPPORT_GAME_IR
    {
        _bKeyGet = INPUT_RemoteScan();
    }
//    _dwTimeISRKeyPrev2 = dwTimeISRKey;
#endif  // #if (IR_CODE_TYPE == HW_NEC)
// =============================================================================

    if (_bKeyGet != KEY_NO_KEY)
    {
        if ( _bKeyGet == _bISRKeyPrev )
        {
            // re-push the same key within 250 msec
#if (IR_CODE_TYPE == HW_NEC)
            if ( (dwTimeISRKey- _dwTimeISRKeyPrev) < COUNT_250_MSEC )
#else   // #if (IR_CODE_TYPE == HW_NEC)
            if(!bIRCheck)
#endif  // #if (IR_CODE_TYPE == HW_NEC)
            {
                // only these keys have continue capability
                if (    (_bKeyGet!=KEY_VOL_UP) &&
                        (_bKeyGet!=KEY_VOL_DOWN) &&
                        (_bKeyGet!=KEY_KEYUP) &&
                        (_bKeyGet!=KEY_KEYDOWN)     )
                {
                    _bKeyGet = KEY_NO_KEY;
                }
                
                //kevin0.72, adjust continuous key timing to let it less sensitive
                //kevin0.68, discard continuous keys (KEY_VOL_UP....) within 150ms; otherwise there will be 2 KEY_VOL_UPs when pressing a single KEY_VOL_UP
#if (IR_CODE_TYPE == HW_NEC)
                else if( (dwTimeISRKey-_dwTimeISRKeyPrev) < COUNT_200_MSEC )  
                {
                    _bKeyGet = KEY_NO_KEY;  
                    //return; //not update _dwTimeISRKeyPrev
                }
#endif  // #if (IR_CODE_TYPE == HW_NEC)
            }
        }
               
        if ( _bKeyGet != KEY_NO_KEY )
        {
//            printf("Key: %lx\n", _bKeyGet);

#if (IR_CODE_TYPE == HW_NEC)
            _dwTimeISRKeyPrev = dwTimeISRKey;
            _dwTimeISRKeyPrev2 = dwTimeISRKey;
#endif  // #if (IR_CODE_TYPE == HW_NEC)

            //__bISRKeyRead = TRUE;
            __bISRKey = _bKeyGet;
            _bISRKeyPrev = _bKeyGet;

        }
    }

#if (IR_CODE_TYPE == HW_NEC)
//  _dwTimeISRKeyPrev = dwTimeISRKey; //UTL_GetSysTimer();
#endif  // #if (IR_CODE_TYPE == HW_NEC)

#if (IR_CODE_TYPE == SW_KONKA)
    bIRFlag = 0;    // hwtan.nec.0331 for SW_IR
#endif  // #if (IR_CODE_TYPE == SW_NEC)

    return ;
}



// ***********************************************************************
//  Function    :   INPUT_RemoteScan
//  Description :   Read remote controller input when IR INT is triggered
//  Arguments   :   NONE
//  Return      :   input key
//  Side Effect :
// ***********************************************************************
#if (IR_CODE_TYPE == HW_NEC)
BYTE    INPUT_RemoteScan(VOID)
{
//IR stage
#define IR_STAGE_IDLE   0x1
#define IR_STAGE_DATA   0x2
#define IR_STAGE_REPEAT 0x4

//IR INT source
#define IR_INT_INVALID  0x1 //data code invalid/customer code invalid/repeat code timeout
#define IR_INT_DATA     0x2
#define IR_INT_REPEAT   0x4

    static BYTE bIRstage = IR_STAGE_IDLE;
    BYTE    bIRINT;
    DWORD   dwData;
    DWORD   dwRawCode;
    BYTE    bData;

    // LLY2.36, just polling the rising edge of IR repeat flag to check if any key input.
    if(_bIRCheckMode==IR_CHECK_MODE_POLLING)
    {
        BYTE    bRepeat;
        dwData = REG_PLAT_IR_DATA;
        dwRawCode = REG_PLAT_IR_RAW_CODE;

        bRepeat = (BYTE)((dwData&0x00000100)>>8);
//        printf("%lx - %lx\n", _bPreRepeatFlag, bRepeat);

        if(!_bPreRepeatFlag && bRepeat)
        {
            if( !(dwData & 0x00000400L) // bit[10] = 1, means invalid data code
#ifdef CHECK_IR_CUSTOMER_CODE
                // check customer code (To Do: LSB first)
                && ( (BYTE)(dwRawCode>>24) == _bCustomerCode )
                && ( (BYTE)(dwRawCode>>16) == _bCustomerCode1 )
#endif  // #ifdef CHECK_IR_CUSTOMER_CODE
                )
            {
                bIRINT = IR_INT_DATA;
            }
            else
            {
                bIRINT = IR_INT_INVALID;
            }
            //printf("0 to 1: %lx\n", bIRINT);
        }
        /*
        else if( (_bPreRepeatFlag == bRepeat)
             && ( (OS_GetSysTimer() - _dwTimeISRKeyPrev2) < COUNT_250_MSEC) )
        {
//            printf("Same\n");

            bIRINT = IR_INT_REPEAT;
        }
        */
        else
        {
            bIRINT = IR_INT_INVALID;
        }

        _bPreRepeatFlag = bRepeat;
//        if(!_bPreRepeatFlag)
//        {
//            printf("%lx\n", _bPreRepeatFlag);
//        }
    }
    else
    {
#if (PLATFORM == ECOS_OS)
    dwData = __INTISRintProc12ndStatus.dwPLAT_IR_DATA;
    dwRawCode = __INTISRintProc12ndStatus.dwPLAT_IR_RAW_CODE;
#else
    dwData = REG_PLAT_IR_DATA;
    dwRawCode = REG_PLAT_IR_RAW_CODE;
#endif

    //Decide IR INT source
    if ( ( dwData & 0x00000100) 
         //&& ( (UTL_GetSysTimer() - _dwTimeISRKeyPrev2) < COUNT_150_MSEC ) //kevin0.85, fix next repeat KEY is regarded as repeat key of prev KEY when moving out of and again to IR receiver range 
         && ( (OS_GetSysTimer() - _dwTimeISRKeyPrev2) < COUNT_250_MSEC ) //kevin0.91, fix cont. KEY VOL-/+ may stop in the middle, but not at min/max
        )
    {
        bIRINT = IR_INT_REPEAT;
    }
    else if (  ( !(dwData & 0x00000500)  ) //0x00000400 | 0x00000100
#ifdef    CHECK_IR_CUSTOMER_CODE
                // check customer code (To Do: LSB first)
                && ( (BYTE)(dwRawCode>>24) == _bCustomerCode )
                && ( (BYTE)(dwRawCode>>16) == _bCustomerCode1 ) 
#endif      
            )
    {
        bIRINT = IR_INT_DATA;
    }
    else
    {
        bIRINT = IR_INT_INVALID;        
    }

    }

    //Read data
    if ( bIRstage & IR_STAGE_IDLE)  
    {
        if (bIRINT & IR_INT_DATA)   //data code
        {
            bData = (BYTE)(dwData & 0x000000ff);
#ifdef   SHOW_IR_MAP
            printf("IR Raw Code = %8x\n", dwData);
#endif
            bIRstage = IR_STAGE_DATA;
        }
        else //IR_INT_REPEAT/IR_INT_INVALID
        {
            bData = 0xFF;
            //bIRstage remains unchanged
        }
    }
    else //IR_STAGE_DATA/IR_STAGE_REPEAT
    {
        if (bIRINT & IR_INT_DATA) //data code
        {
            bData = (BYTE)(dwData & 0x000000ff);
#ifdef   SHOW_IR_MAP
            printf("IR Raw Code = %8x\n", dwData);
#endif
            //bIRstage remains unchanged
        }
        else if (bIRINT & IR_INT_REPEAT) //repeat code
        {
            bData = (BYTE)(dwData & 0x000000ff);
#ifdef   SHOW_IR_MAP
            printf("IR Repeat Code = %2x\n", bData);
#endif
            bIRstage = IR_INT_REPEAT;
        }
        else //IR_INT_INVALID
        {
            bData = 0xFF;
            bIRstage = IR_STAGE_IDLE;       
        }
    }

    if ( bData < _IRInfo.bIRMapSize)
    {
        return _IRInfo.aIRMap[bData];
    }
    else
    {
        return KEY_NO_KEY;
    }

/* Alan2.05
    if ( bData < (sizeof(aIRMap)/sizeof(BYTE)) )
        return aIRMap[bData];
    else
        return KEY_NO_KEY;
*/

}
#endif  // #if (IR_CODE_TYPE == HW_NEC)

#if (IR_CODE_TYPE == SW_KONKA)
VOID   INPUT_SW_IR_Detect(VOID)
{
    #define FALLING_EDGE   0x00000020
    #define RISING_EDGE    0x00000010
    extern cyg_handle_t    __ClockHandle;

    DWORD dwTemp;
    DWORD dwTemp1;

    // get the system timer when enter interrupt
    dwTemp= (DWORD) cyg_counter_current_value(__ClockHandle); //UTL_GetSysTimer();
    // printf("sys timer=%lx \n", dwTemp);    
    //dwTemp1 = (dwTemp-_dwTimeISRKeyPrev2);                  
    dwTemp1  =  REG_PLAT_IR_COUNTER;                          

    switch(bIRStep)                                                                         
    {                                                                                       
     case 0:                                                                                        
                 if(REG_PLAT_IR_STATUS & RISING_EDGE)   //rising todo                               
                                           break;                                                   
          else          goto INVALID_IR;                                                            
                                                                                                    
     case 1:                                                                                        
            if((dwTemp-_dwTimeISRKeyPrev2)==3 && (REG_PLAT_IR_STATUS & FALLING_EDGE))               
                 break;                                                                             
       else                                                                                         
        goto INVALID_IR;                                                                            
                                                                                                    
     case 2:                                                                                        
             if((dwTemp-_dwTimeISRKeyPrev2)==3 &&(REG_PLAT_IR_STATUS & RISING_EDGE))                
               break;                                                                               
       else                                                                                         
        goto INVALID_IR;                                                                            
                                                                                                    
     case 3:                                                                                        
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // customer code MSB 8                            
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                _bCustomerCode &= ~0x80;                                                            
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               _bCustomerCode |= 0x80;                                                              
             }                                                                                      
       else                                                                                         
        goto INVALID_IR;                                                                            
            break;                                                                                  
                                                                                                    
     case 4:                                                                                        
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 7                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                _bCustomerCode &= ~0x40;                                                            
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               _bCustomerCode |= 0x40;                                                              
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
              break;                                                                                
                                                                                                    
     case 5:                                                                                        
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 6                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                _bCustomerCode &= ~0x20;                                                            
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               _bCustomerCode |= 0x20;                                                              
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
    case 6:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 5                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                _bCustomerCode &= ~0x10;                                                            
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               _bCustomerCode |= 0x10;                                                              
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
    case 7:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 4                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                _bCustomerCode &= ~0x08;                                                            
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               _bCustomerCode |= 0x08;                                                              
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
    case 8:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 3                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                _bCustomerCode &= ~0x04;                                                            
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               _bCustomerCode |= 0x04;                                                              
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
    case 9:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 2                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                _bCustomerCode &= ~0x02;                                                            
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               _bCustomerCode |= 0x02;                                                              
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
    case 10:                                                                                        
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 1                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                _bCustomerCode &= ~0x01;                                                            
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               _bCustomerCode |= 0x01;                                                              
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
    case 11:                                                                                        
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 8         data code start                      
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                bDataCode&= ~0x80;                                                                  
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               bDataCode|= 0x80;                                                                    
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
   case 12:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 7                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                bDataCode&= ~0x40;                                                                  
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               bDataCode|= 0x40;                                                                    
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               

   case 13:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 6                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                bDataCode&= ~0x20;                                                                  
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               bDataCode|= 0x20;                                                                    
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
   case 14:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 5                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                bDataCode&= ~0x10;                                                                  
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               bDataCode|= 0x10;                                                                    
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
   case 15:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 4                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                bDataCode&= ~0x08;                                                                  
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               bDataCode|= 0x08;                                                                    
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
   case 16:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 3                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                bDataCode&= ~0x04;                                                                  
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               bDataCode|= 0x04;                                                                    
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               

               break;                                                                               
                                                                                                    
   case 17:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 2                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                bDataCode&= ~0x02;                                                                  
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               bDataCode|= 0x02;                                                                    
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
                                                                                                    
               break;                                                                               
                                                                                                    
   case 18:                                                                                         
           if(REG_PLAT_IR_STATUS & FALLING_EDGE)  // 1                                              
        return;                                                                                     
             if((dwTemp-_dwTimeISRKeyPrev2)==2)  // it is 0                                         
            {                                                                                       
                bDataCode&= ~0x01;                                                                  
             }                                                                                      
       else if((dwTemp-_dwTimeISRKeyPrev2)==3)  // it is 1                                          
            {                                                                                       
               bDataCode|= 0x01;                                                                    
             }                                                                                      
       else                                                                                         
        goto INVALID_IR; // or break?                                                               
               break;                                                                       
    }                                                                                       
//    printf("time gap = %lx \n",dwTemp1);                                                  
  
    if (bIRStep==18) // && _bCustomerCode == 0x06 )  // finish one key receiving, clear flag, map the real key, and return
    {
//Alan2.05        _bKeyGet = aIRMap[bDataCode];  // get the key map
        _bKeyGet = _IRInfo.aIRMap[bDataCode];  // get the key map
        bIRStep = 0;  // must clear the flag
        bIRFlag = 1;  // get one ir key
// printf("time gap = %lx \n",dwTemp1);           
         return;
    }
    else if(bIRStep<18)   // continue to detect next edge
    {
        _dwTimeISRKeyPrev2 = dwTemp;  
        bIRStep++;  // 0 -> 1 none --> leader begin
 //     printf("time gap = %lx \n",dwTemp1);
        return;
    }

INVALID_IR:   //its not valid IR singal
    bIRStep = 0;
    _bKeyGet = KEY_NO_KEY;
    return;
}
#endif  // #if (IR_CODE_TYPE == SW_KONKA)

/*** Many of the following codes will be moved out of the INPUT module ***/

//  *********************************************************************
//  Function    :   INPUT_InitIR
//  Description :   Set IR parameters
//  Arguments   :   None
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************
VOID    INPUT_InitIR(PIR_INFO pIRInfo)
{
    BYTE i;

    if (NULL == pIRInfo)
    {
        _IRInfo.bIR_Type = IR_TYPE;
        _IRInfo.bCustomerCode = CUSTOMER_CODE;
        _IRInfo.bCustomerCode1 = CUSTOMER_CODE1;
        _IRInfo.aIRMap = aIRMap;
        _IRInfo.bIRMapSize = sizeof(aIRMap) / sizeof(BYTE);

        for (i = 0; i < 8; i++)
        {
            _bCustomerCode |= ( (CUSTOMER_CODE>>i) & 1 ) << (7-i);
            _bCustomerCode1 |= ( (CUSTOMER_CODE1>>i) & 1 ) << (7-i);
        }
    }
    else
    {
        INPUT_SetIRInfo(pIRInfo);
    }
        
    // LLY2.36, call API to enable IR interrupt and config IR checking mode
    //REG_PLAT_PROC1_2ND_INT_MASK_DISABLE = INT_PROC1_2ND_IR;//some of interrupt is not available yet. ->mask off
    INPUT_SetIRCheckMode(IR_CHECK_MODE_INT);

    _ResetIR();
}

VOID    _ResetIR(VOID) //kevin0.75
{
    DWORD dwSaveInt;
    HAL_DISABLE_INTERRUPTS( dwSaveInt );    // Disable INT to avoid different threads accessing
    REG_PLAT_RESET_CONTROL_ENABLE = PLAT_RESET_IR_ENABLE;
    REG_PLAT_RESET_CONTROL_DISABLE = PLAT_RESET_IR_DISABLE;
    HAL_RESTORE_INTERRUPTS( dwSaveInt );    // Restore INT. 
        
    //IR control registers
    //REG_PLAT_IR_TIMING_CONTROL1 = 0x13880bb8;   
    REG_PLAT_IR_TIMING_CONTROL1 = 0x13880008;
    REG_PLAT_IR_TIMING_CONTROL2 = 0x01c20384;
    REG_PLAT_IR_TIMING_CONTROL3 = 0x232800e1;

#if (IR_CODE_TYPE == HW_NEC)
    REG_PLAT_IR_TIMING_CONTROL4 = 0x0040105a
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
    | (0x3 << 23)
#endif
    ;
#else    
    REG_PLAT_IR_TIMING_CONTROL4 = 0x0060105a; 
#endif    

#ifdef SUPPORT_GAME_IR
    _blGameIRMode = FALSE;
#endif
}

VOID    INPUT_SetIRInfo(PIR_INFO pIRInfo)
{
    BYTE i;

    if (pIRInfo->bIRMapSize > 255)
    {
#ifdef DEBUG_IR
        printf("IR size is overflow.\n");
#endif
        return;
    }
    _IRInfo.bIR_Type = pIRInfo->bIR_Type;
    _IRInfo.bCustomerCode = pIRInfo->bCustomerCode;
    _IRInfo.bCustomerCode1 = pIRInfo->bCustomerCode1;
    _IRInfo.aIRMap = pIRInfo->aIRMap;
    _IRInfo.bIRMapSize = pIRInfo->bIRMapSize;

    for (i = 0; i < 8; i++)
    {
        _bCustomerCode |= ( (pIRInfo->bCustomerCode>>i) & 1 ) << (7-i);
        _bCustomerCode1 |= ( (pIRInfo->bCustomerCode1>>i) & 1 ) << (7-i);
    }
}

PIR_INFO INPUT_GetIRInfo()
{
    return &_IRInfo;
}

#ifdef SUPPORT_GAME_IR
VOID    INPUT_SetGameIR(VOID)
{
    DWORD dwSaveInt;

    HAL_DISABLE_INTERRUPTS( dwSaveInt );        // Disable INT to avoid different threads accessing
    REG_PLAT_RESET_CONTROL_ENABLE = PLAT_RESET_IR_ENABLE;
    REG_PLAT_RESET_CONTROL_DISABLE = PLAT_RESET_IR_DISABLE;
    HAL_RESTORE_INTERRUPTS( dwSaveInt );        // Restore INT.

    //IR control registers
    REG_PLAT_IR_TIMING_CONTROL1 = 0x0FA0000A;
    REG_PLAT_IR_TIMING_CONTROL2 = 0x00C80190;
    REG_PLAT_IR_TIMING_CONTROL3 = 0x0A140064;
    REG_PLAT_IR_TIMING_CONTROL4 = 0x0040103C
    // port aron's code, 20070315
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
        | (0x3 << 23) // Set 32bits data mode
#endif
        ;
    _blGameIRMode = TRUE;
}


BYTE INPUT_Game_RemoteScan(VOID)
{
    static  BYTE    bGameIRstage = IR_STAGE_IDLE;
    DWORD   dwData;
    DWORD   dwRawCode;
    BYTE    bIRINT;

#if (PLATFORM == ECOS_OS)
    dwData = __INTISRintProc12ndStatus.dwPLAT_IR_DATA;
    dwRawCode = __INTISRintProc12ndStatus.dwPLAT_IR_RAW_CODE;
#else
    dwData = REG_PLAT_IR_DATA;
    dwRawCode = REG_PLAT_IR_RAW_CODE;
#endif

//printf("dwData = %lx, dwRawCode = %lx\n", dwData, dwRawCode);

        //Decide IR INT source
        if ( (OS_GetSysTimer() - _dwTimeISRKeyPrev2) < COUNT_50_MSEC )
        {
                bIRINT = IR_INT_REPEAT;
        }
        else if ( !(dwData & 0x00000500) )
        {
                bIRINT = IR_INT_DATA;
        }
        else
        {
                bIRINT = IR_INT_INVALID;
        }


        if (IR_INT_INVALID != bIRINT)
        {
            if (( (BYTE)(dwRawCode>>24) == _bCustomerCode ) &&          // Custom code : 00 FF
                ( (BYTE)(dwRawCode>>16) == _bCustomerCode1 ))
            {
                // Operation Key
                __wISRGameKey = (WORD) ((~dwData) & 0x000000FF);
            }
            else if (( (BYTE)(dwRawCode>>24) == _bCustomerCode1 ) &&
                     ( (BYTE)(dwRawCode>>16) == _bCustomerCode ))
            {
                // External Key
                if (0x0 == (dwData & 0x000000FF))
                {
                    _ResetIR();
                    bIRINT = IR_INT_INVALID;
                }
                else
                {
                    __wISRGameKey = 0x0001 << ((dwData & 0x000000FF) + 7);
                }
            }
            else
            {
                bIRINT = IR_INT_INVALID;
            }
        }

        //Read data
        if ( bGameIRstage & IR_STAGE_IDLE)
        {
                if (bIRINT & IR_INT_DATA)       //data code
                {
                        bGameIRstage = IR_STAGE_DATA;
                }
                else //IR_INT_REPEAT/IR_INT_INVALID
                {
                        __wISRGameKey = 0xFFFF;
                        //bGameIRstage remains unchanged
                }
        }
        else // IR_STAGE_DATA/IR_STAGE_REPEAT
        {
                if (bIRINT & IR_INT_DATA) //data code
                {
                        //bGameIRstage remains unchanged
                }
                else if (bIRINT & IR_INT_REPEAT) //repeat code
                {
                        bGameIRstage = IR_INT_REPEAT;
                }
                else //IR_INT_INVALID
                {
                        __wISRGameKey = 0xFFFF;
                        bGameIRstage = IR_STAGE_IDLE;
                }
        }

    return KEY_NO_KEY;
}
#endif   // ifdef SUPPORT_GAME_IR


// LLY2.36 create
//  ********************************************************************************
//  Function    :   INPUT_SetIRCheckMode
//  Description :   Config IR key checking mode
//  Arguments   :   bMode, specify the desired mode
//  Return      :   None
//  ********************************************************************************
void    INPUT_SetIRCheckMode(BYTE bMode)
{
// LLY2.38, keep IR working mode when power down.
#ifdef  KEEP_IR_WORKING_MODE_WHEN_POWERDOWN

    // Must change mode first, then enable IR_INT
    _bIRCheckMode=IR_CHECK_MODE_INT;
    REG_PLAT_PROC1_2ND_INT_MASK_DISABLE = INT_PROC1_2ND_IR; // enable IR interrupt

#else   // #ifdef  KEEP_IR_WORKING_MODE_WHEN_POWERDOWN

    if(bMode==IR_CHECK_MODE_INT) // interrupt mode
    {
        // Must change mode first, then enable IR_INT
        _bIRCheckMode=IR_CHECK_MODE_INT;
        REG_PLAT_PROC1_2ND_INT_MASK_DISABLE = INT_PROC1_2ND_IR; // enable IR interrupt
    }
    else    // Polling mode
    {
        // Must disable IR_INT first, then change mode
        REG_PLAT_PROC1_2ND_INT_MASK_ENABLE = INT_PROC1_2ND_IR; //ddisable IR interrupt
        _bIRCheckMode=IR_CHECK_MODE_POLLING;
    }

#endif  //  #ifdef  KEEP_IR_WORKING_MODE_WHEN_POWERDOWN
}


//  ********************************************************************************
//  Function    :   INPUT_GetIRCheckMode
//  Description :   Get current IR key checking mode
//  Arguments   :   None
//  Return      :   IR_CHECK_MODE_INT, interrupt mode
//                  IR_CHECK_MODE_POLLING, polling mode
//  ********************************************************************************
BYTE    INPUT_GetIRCheckMode(void)
{
    return _bIRCheckMode;
}
