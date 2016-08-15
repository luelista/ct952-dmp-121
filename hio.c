#include "winav.h"
#include "ctkav.h"
#include "ctkav_mcu.h"
#include "hio.h"
#ifdef  SUPPORT_I2C

VOID _Delay_I2C(void);
VOID    _IIC_START (PI2C_CONFIG  pConfig);
VOID    _IIC_TxByte (PI2C_CONFIG  pConfig, BYTE bValue);
BYTE    _IIC_RxByte (PI2C_CONFIG  pConfig);
BYTE    _IIC_GetACK (PI2C_CONFIG  pConfig);
VOID    _IIC_SetACK (PI2C_CONFIG  pConfig);
VOID    _IIC_STOP (PI2C_CONFIG  pConfig);
VOID    _I2C_Set_SDAT_HIGH(PI2C_CONFIG  pConfig);
VOID    _I2C_Set_SCLK_HIGH(PI2C_CONFIG  pConfig);
VOID    _I2C_Set_SDAT_LOW(PI2C_CONFIG  pConfig);
VOID    _I2C_Set_SCLK_LOW(PI2C_CONFIG  pConfig);
BYTE    _I2C_Get_SDAT(PI2C_CONFIG  pConfig);
#endif  //#ifdef  SUPPORT_I2C
#ifdef  ENABLE_PANEL_KEYSCAN
extern BYTE __icVFDnow;
#endif  // #ifdef ENABLE_PANEL_KEYSCAN


// Define GPB register address -- LLY2.01
// LLY2.56, split GPB output data control to two different register,
// ie. one is for set 1, another is for clear 0
#ifdef  CT909G_IC_SYSTEM
#define REG_SERVO_GPB_SETDATA               (*(volatile DWORD *)(0x80002480+(DWORD)0x30*4))
#define REG_SERVO_GPB_CLEARDATA             (*(volatile DWORD *)(0x80002480+(DWORD)0x32*4))
#else   // #ifdef  CT909G_IC_SYSTEM
#define REG_SERVO_GPB_OUTDATA               (*(volatile DWORD *)(0x80002480+(DWORD)0x30*4))
#endif  // #ifdef  CT909G_IC_SYSTEM
#define REG_SERVO_GPB_INDATA                (*(volatile DWORD *)(0x80002480+(DWORD)0x31*4))
#define REG_SERVO_GPB_IO_DIR_CONTROL        (*(volatile DWORD *)(0x80002480+(DWORD)0x33*4))
#define REG_SERVO_GPB_FUNC_SEL              (*(volatile DWORD *)(0x80002480+(DWORD)0x36*4))


// LLY2.18, define a mutex for I2C and Expand GPIO procedure share same pin issue
MUTEX_T     __mutexI2CReady;


#ifdef  SW_EXPAND_GPIO

// LLY2.16 create
// LLY2.20, modify the procedure, since it will call lower level API to scan all key once time
//  ************************************************************************************
//  Function    :   _SW_Expand_GPI
//  Description :   Using S/W mechanism to do expand GPI feature
//  Arguments   :   dwPort, specify the desired port, index from '0'
//  Return      :   The port status
//  ************************************************************************************
BYTE  _SW_Expand_GPI(DWORD dwPort)
{
    DWORD   dwDesiredPort;

    // LLY2.60, add error protection base on supporting
#ifdef NO_KEY_DETECT_0
    if(dwPort < 8)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: not support key detect0, port: %lx\n", dwPort);
        return FALSE;
    }
#endif  // #ifdef NO_KEY_DETECT_0

#ifdef NO_KEY_DETECT_1
    if(dwPort >= 8)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: not support key detect1, port: %lx\n", dwPort);
        return FALSE;
    }
#endif  // #ifdef NO_KEY_DETECT_1

    // Calculate the desired port position.
    dwDesiredPort = (1<<dwPort);

    // Return the status of desired port
    if(HAL_Read_ExpGPI() & dwDesiredPort)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


DWORD   __dwExpGPOVal=0;
// LLY2.16 create
//  ************************************************************************************
//  Function    :   _SW_Expand_GPO
//  Description :   Using S/W mechanism to do expand GPO feature
//  Arguments   :   dwPort, specify the desired port
//  Return      :   The port status
//  ************************************************************************************
void _SW_Expand_GPO(DWORD dwPort, BYTE bValue)
{
    BYTE    bLatchVal;
    BYTE    bIdx;
    BYTE    bShiftBit;
    DWORD   dwSaveInt;

    // LLY2.60, add error protection while don't support latch D1
#ifdef  NO_LATCH_D0
    if(dwPort < 8)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: not support latch D0, port: %lx\n", dwPort);
        return;
    }
#endif  // #ifdef NO_LATCH_D0

#ifdef  NO_LATCH_D1
    if(dwPort >= 8)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: not support latch D1, port: %lx\n", dwPort);
        return;
    }
#endif  // #ifdef NO_LATCH_D1

    OS_DISABLE_INTERRUPTS(dwSaveInt);

    // Step 1: Set desired value to desired port
    if(bValue)
    {
        __dwExpGPOVal |= (1<<dwPort);
    }
    else
    {
        __dwExpGPOVal &= ~(1<<dwPort);
    }

    // Step 2: Calculate desired latch value (only 8 bit)
    if(dwPort>=8)   // port 8 ~ 15
    {
        bLatchVal = (BYTE)(__dwExpGPOVal>>8);
    }
    else    //  port 0 ~ 7
    {
        bLatchVal = (BYTE)(__dwExpGPOVal & 0xFF);
    }

    // Step 3: Set shift bit w/ 0x80 since bit[7] shift first
    bShiftBit=0x80;  // enable bit[7] as 1 first

    // Step 4: Generate clock and data one by one
    for(bIdx=0; bIdx<8; bIdx++)
    {
        if(bLatchVal & bShiftBit)
        {
            _KS_DAT_HI;
        }
        else
        {
            _KS_DAT_LO;
        }
        // Shift Register (164) is rising edge trigger, so clock is low --> high
        _KS_CLK_LO;
        _KS_CLK_HI;

        // shift right 1 bit for next data
        bShiftBit >>= 1;
    }

    // Step 5: Set latch signal as high since 373 register is high level trigger
    //         And, set the signal as low to release the latch
    if(dwPort>=8)   // port 8 ~ 15
    {
        _KS_LATCH1_HI;
        _KS_LATCH1_LO;
    }
    else    //  port 0 ~ 7
    {
        _KS_LATCH0_HI;
        _KS_LATCH0_LO;
    }
    OS_RESTORE_INTERRUPTS(dwSaveInt);

}


#endif  // #ifdef   SW_EXPAND_GPIO


// LLY2.20 create,
//  **************************************************************************************
//  Function    :   HAL_Read_ExpGPI
//  Description :   Read back Expand GPI all port status
//  Arguments   :   None
//  Return      :   All port status from expand GPI
//  **************************************************************************************
DWORD  HAL_Read_ExpGPI(void)
{
#ifdef  ENABLE_EXPAND_GPIO
    return (REG_PLAT_PANEL_KD);
#endif  // #ifdef ENABLE_EXPAND_GPIO

#ifdef  SW_EXPAND_GPIO
    BYTE    bIdx;
    DWORD   dwGPIValue=0;
    DWORD   dwSaveInt;
    BYTE    bDelay;

    // Disable interrupt first since following key scan action can't be interrupt.
    OS_DISABLE_INTERRUPTS(dwSaveInt);

#ifdef  ACTIVE_LOW

    // Step 0: initial first 7 port w/ high signal, ie. port 0 ~ 6, or port 8 ~ 14
    _KS_DAT_HI;
    for(bIdx=0; bIdx<7; bIdx++)
    {
        // Shift Register (164) is rising edge trigger, so clock is low --> high
        _KS_CLK_LO;
        _KS_CLK_HI;
    }

    // Step 2: output low signal (x1) + low/ high clock
    //         then output high signal later to generate a low pulse
    _KS_DAT_LO;
    _KS_CLK_LO;
    _KS_CLK_HI;
    _KS_DAT_HI;

    // Step 3: Read back the key detect 0/ 1 value for port 0 and 8
    // LLY2.60, add protection: only do operation while enable define.
    //dwGPIValue = ((DWORD)(!_KS_DET0_READ)) + (((DWORD)(!_KS_DET1_READ)) <<8);
#ifndef NO_KEY_DETECT_0
    dwGPIValue |= ((DWORD)(!_KS_DET0_READ));
#endif  // #ifndef NO_KEY_DETECT_0

#ifndef NO_KEY_DETECT_1
    dwGPIValue |= (((DWORD)(!_KS_DET1_READ)) <<8);
#endif  // #ifndef NO_KEY_DETECT_1

    // Step 4: Loop 7 time to generate low/ high clock to shift low pulse from port 1~7 or 9~ 15
    //         And, read back the key detect 0/ 1 value
    for(bIdx=1; bIdx<8; bIdx++)
    {
        // Must delay a while before next clock pulse
        // Since RC charge/ dis-charge effect for data pin ready.
        for(bDelay = 0;bDelay<128;bDelay++)
        {
            NOP;
        }

        _KS_CLK_LO;
        _KS_CLK_HI;

        // LLY2.60, add protection: only do operation while enable define.
        //dwGPIValue |= (((DWORD)(!_KS_DET0_READ))<<bIdx) + (((DWORD)(!_KS_DET1_READ))<<(bIdx+8));
#ifndef NO_KEY_DETECT_0
        dwGPIValue |= (((DWORD)(!_KS_DET0_READ))<<bIdx);
#endif  // #ifndef NO_KEY_DETECT_0

#ifndef NO_KEY_DETECT_1
        dwGPIValue |= (((DWORD)(!_KS_DET1_READ))<<(bIdx+8));
#endif  // #ifndef NO_KEY_DETECT_1

    }

#else   // #ifdef ACTIVE_LOW
    // Step 0: initial firt 7 port w/ low signal, ie port 0 ~ 6 or port 8 ~ 14
    _KS_DAT_LO;
    for(bIdx=0; bIdx<7; bIdx++)
    {
        // Shift Register (164) is rising edge trigger, so clock is low --> high
        _KS_CLK_LO;
        _KS_CLK_HI;
    }

    // Step 2: output high signal (x1) + low/ high clock
    //         then output low signal later to generate a high pulse
    _KS_DAT_HI;
    _KS_CLK_LO;
    _KS_CLK_HI;
    _KS_DAT_LO;

    // Step 3: Read back the key detect 0/ 1 value for port 0 and 8
    // LLY2.60, add protection: only do operation while enable define.
    //dwGPIValue = ((DWORD)_KS_DET0_READ) + (((DWORD)_KS_DET1_READ) <<8);
#ifndef NO_KEY_DETECT_0
    dwGPIValue |= ((DWORD)_KS_DET0_READ);
#endif  // #ifndef NO_KEY_DETECT_0

#ifndef NO_KEY_DETECT_1
    dwGPIValue |= (((DWORD)_KS_DET1_READ) <<8);
#endif  // #ifndef NO_KEY_DETECT_1

    // Step 4: Loop 7 time to generate low/ high clock to shift low pulse from port 1 ~ 7 or 9 ~ 15
    //         And, read back the key detect 0/ 1 value
    for(bIdx=1; bIdx<8; bIdx++)
    {
        // Must delay a while before next clock pulse
        // Since RC charge/ dis-charge effect for data pin ready.
        for(bDelay = 0;bDelay<128;bDelay++)
        {
            NOP;
        }

        _KS_CLK_LO;
        _KS_CLK_HI;

        // LLY2.60, add protection: only do operation while enable define.
        //dwGPIValue |= (((DWORD)_KS_DET0_READ)<<bIdx) + (((DWORD)_KS_DET1_READ)<<(bIdx+8));
#ifndef NO_KEY_DETECT_0
        dwGPIValue |= (((DWORD)_KS_DET0_READ)<<bIdx);
#endif  // #ifndef NO_KEY_DETECT_0

#ifndef NO_KEY_DETECT_1
        dwGPIValue |= (((DWORD)_KS_DET1_READ)<<(bIdx+8));
#endif  // #ifndef NO_KEY_DETECT_1

    }
#endif  // #ifdef ACTIVE_LOW


    // LLY2.50, Porting Jeff's code to send clock low/ high signal again
    // to reset 74138 for All ping = High output
    // If not , the  will always keep KS_DET0/1 pin as low,
    // and cause other pin shared it will work abnormal
    _KS_CLK_LO;
    _KS_CLK_HI;


    OS_RESTORE_INTERRUPTS(dwSaveInt);

    return dwGPIValue;

#endif  // #ifdef SW_EXPAND_GPIO

    return FALSE;

}


//  **********************************************************************************************
//  Function    :   HAL_ReadGPIO
//  Description :   Read the value from the desired GPA port
//  Arguments   :   bGroup: specify the GPIO group
//                  dwPort: port #
//  Return      :   1: High
//                  0: Low
//  Notice      :   Must disable interrupt first since GPIO maybe accessed by different thread
//  *********************************************************************************************
// LLY2.13, modify the procedure and add expand GPIO code
// LLY2.15, modify the procedure for GPIO CDEF group
// LLY2.50, modify the procedure to support extra CT909P GPIO group
BYTE    HAL_ReadGPIO(BYTE bGroup, DWORD dwPort)
{
    DWORD   dwSaveInt;
    // Used to keep desired port position, one port -> one position
    DWORD   dwDesiredPort;

#ifdef CT909R_IC_SYSTEM
    if(bGroup == GPIO_F)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Not implement GPIO group\n");
        return FALSE;
    }
#endif  // #ifndef CT909P_IC_SYSTEM

    // LLT2.77, remove the error protection before disable interrupt action.
#ifdef  CT909P_IC_SYSTEM
    if(bGroup == GPIO_B)
    {
        if(dwPort >= 6)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Not supported GPB port\n");
            return FALSE;
        }
    }
#endif  // #ifdef CT909P_IC_SYSTEM


    if(bGroup == GPIO_EXP)
    {
        // LLY2.16, go S/W Expand GPIO path
#ifdef  SW_EXPAND_GPIO
        return (_SW_Expand_GPI(dwPort));
#endif  // #ifdef SW_EXPAND_GPIO
    }

    // Shift the desired port to final position first
    dwDesiredPort = (1<<dwPort);

    // GPCDE @ same register for CT909R
#ifdef CT909R_IC_SYSTEM
    // Shift left again for GPIO CDE, since they shared the same register
    if(bGroup==GPIO_D)
    {
        // Shift left 8 bit since GPIO D @ bit[15:8] first
        dwDesiredPort = (dwDesiredPort << 8);
    }
    else if(bGroup==GPIO_E)
    {
        // Shift left 16 bit since GPIO E @ bit [24:16] first
        dwDesiredPort = (dwDesiredPort << 16);
    }
#endif  // #ifdef CT909R_IC_SYSTEM

    // Disable interrupt first
    OS_DISABLE_INTERRUPTS(dwSaveInt);

    // Set GPIO as input mode
    if(bGroup==GPIO_A)
    {
        // LLY2.50, config GPA Mux selection register for using GPA as GPIO @ CT909P
#ifdef  CT909P_IC_SYSTEM
        if(dwPort==0)
        {
            // GPAMux[0]=1, GPAMux[2:1]=0
            REG_PLAT_GPA_MUX &= ~(0x7L);
            REG_PLAT_GPA_MUX |= 0x1L;
        }
        else if(dwPort==1)
        {
            // GPAMux[3]=1, GPAMux[5:4]=0
            REG_PLAT_GPA_MUX &= ~(0x7L<<3);
            REG_PLAT_GPA_MUX |= (0x1L<<3);
        }
        else if(dwPort==2)
        {
            // GPAMux[6]=1, GPAMux[8:7]=0
            REG_PLAT_GPA_MUX &= ~(0x7L<<6);
            REG_PLAT_GPA_MUX |= (0x1L<<6);
        }
        else if(dwPort==3)
        {
            // GPAMux[9]=1, GPAMux[11:10]=0
            REG_PLAT_GPA_MUX &= ~(0x7L<<9);
            REG_PLAT_GPA_MUX |= (0x1L<<9);
        }
        else if(dwPort==4 || dwPort==5 || dwPort==12 || dwPort==13 || dwPort==24 || dwPort==25)
        {
            // GPAMux[12]=1; GPAMux[13, 16, 24]=0
            REG_PLAT_GPA_MUX &= ~((0x1L<<13) + (0x1L<<16) + (0x1L<<24));
            REG_PLAT_GPA_MUX |= (0x1L<<12);
        }
        else if(dwPort>=6 && dwPort<=11)
        {
            // GPAMux[14]=1; GPAMux[15]=0
            REG_PLAT_GPA_MUX &= ~(0x1L<<15);
            REG_PLAT_GPA_MUX |= (0x1L<<14);
        }
        else if(dwPort>=14 && dwPort<=19)
        {
            // GPAMux[17]=1; GPAMux[18]=0
            REG_PLAT_GPA_MUX &= ~(0x1L<<18);
            REG_PLAT_GPA_MUX |= (0x1L<<17);
        }
        else if(dwPort==20)
        {
            // GPAMux[20]=1; GPAMux[19]=0
            REG_PLAT_GPA_MUX &= ~(0x1L<<19);
            REG_PLAT_GPA_MUX |= (0x1L<<20);
        }
        else if(dwPort==21)
        {
            // GPAMux[22]=1; GPAMux[21]=0
            REG_PLAT_GPA_MUX &= ~(0x1L<<21);
            REG_PLAT_GPA_MUX |= (0x1L<<22);
        }
        else if(dwPort==22 || dwPort==23)
        {
            // GPAMux[23]=1
            REG_PLAT_GPA_MUX |= (0x1L<<23);
        }
        else if(dwPort>=26 && dwPort<=31)
        {
            // GPAMux[25]=1, GPAMux[26]=0
            REG_PLAT_GPA_MUX &= ~(0x1L<<26);
            REG_PLAT_GPA_MUX |= (0x1L<<25);
        }

#endif  // #ifdef CT909P_IC_SYSTEM

        // Set 1 to enable the desired port as input mode
        REG_PLAT_GPA_IO_DIR_CONTROL |= dwDesiredPort;
    }
    else if(bGroup==GPIO_B)
    {
        // LLY2.56, GPB control can be handled by MPEG part @ CT909G
        // And, must set bit[16]:GPB_EN, must set as 1, then can control GPB by MPEG part
        // Notice: default disable MPEG path otherwise can't access servo internal H/W block
#if 0//def  CT909G_IC_SYSTEM
        REG_PLAT_GPB_IO_DIR_CONTROL |= dwDesiredPort + (0x1L<<16);
#else   // #ifdef CT909G_IC_SYTEM
        // LLY2.51, do index map for GPB since it's different @ CT909P for function selection and IO Direction port
#ifdef  CT909P_IC_SYSTEM
        BYTE    aMap[6]={8, 6, 15, 16, 17, 18};

        // LLT2.77, remove the error protection before disable interrupt action.
        // Otherwise, nobody release the interrupt.
#if 0
        if(dwPort >= 6)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Not supported GPB port\n");
            return FALSE;
        }
#endif  // #if 0

        // LLY2.53, fix coding error (forget to do shift operation).
        dwDesiredPort = (1<<aMap[dwPort]);
#endif  // #ifdef CT909P_IC_SYSTEM

        // Set GPB output function selection register as GPIO mode by 0
        // LLY2.15, only can modify the desired pin
        REG_SERVO_GPB_FUNC_SEL &= ~(dwDesiredPort);

        // Set 1 to enable the desired port as input mode
        REG_SERVO_GPB_IO_DIR_CONTROL |= dwDesiredPort;
#endif  // #ifdef CT909G_IC_SYSTEM

    }
#ifdef  CT909P_IC_SYSTEM
    else if(bGroup==GPIO_C)
    {
        // Set 1 to enable the desired port as input mode
        REG_PLAT_GPC_IO_DIR_CONTROL |= dwDesiredPort;
    }
    else if(bGroup==GPIO_D)
    {
        // Set 1 to enable the desired port as input mode
        REG_PLAT_GPD_IO_DIR_CONTROL |= dwDesiredPort;
    }
    else if(bGroup==GPIO_E)
    {
        // Set 1 to enable the desired port as input mode
        REG_PLAT_GPE_IO_DIR_CONTROL |= dwDesiredPort;
    }
    else if(bGroup==GPIO_F)
    {
        // Set 1 to enable the desired port as input mode
        REG_PLAT_GPF_IO_DIR_CONTROL |= dwDesiredPort;
    }
    else if(bGroup==GPIO_G)
    {
        // Set 1 to enable the desired port as input mode
        REG_PLAT_GPG_IO_DIR_CONTROL |= dwDesiredPort;
    }
    else if(bGroup==GPIO_H)
    {
        // Set 1 to enable the desired port as input mode
        REG_PLAT_GPH_IO_DIR_CONTROL |= dwDesiredPort;
    }
#endif  // #ifdef CT909P_IC_SYSTEM
    // GPCDE same register of CT909R
#ifdef CT909R_IC_SYSTEM
    else if( (bGroup>=GPIO_C) && (bGroup<=GPIO_E) )
    {
        // Set 1 to enable the desired port as input mode
        REG_PLAT_GPCDE_IO_DIR_CONTROL |= dwDesiredPort;
    }
    else if(bGroup==GPIO_F)
    {
    }
#endif  // #ifdef CT909R_IC_SYSTEM

    // LLY2.56, support GPC @ CT909G
#ifdef  CT909G_IC_SYSTEM
    else if(bGroup == GPIO_C)
    {
        // Set 1 to enable the desired port as input mode
        REG_PLAT_GPC_IO_DIR_CONTROL |= dwDesiredPort;
    }
#endif  // #ifdef CT909G_IC_SYSTEM

    // Release the interrupt
    OS_RESTORE_INTERRUPTS(dwSaveInt);

    // Read-back the desired port value

    if(bGroup==GPIO_A)
    {
        if( REG_PLAT_GPA_CLEAR & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_B)
    {
        // LLY2.56, GPB control can be handled by MPEG part @ CT909G
        // Notice: default disable MPEG path otherwise can't access servo internal H/W block
#if 0//def  CT909G_IC_SYSTEM
        if( REG_PLAT_GPB_CLEAR & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
#else   // #ifdef CT909G_IC_SYTEM
        // LLY2.51, it's un-necessary to index map for GPB INDATA port
        // So, must re-calculate desired port again.
#ifdef  CT909P_IC_SYSTEM
        dwDesiredPort = (1<<dwPort);
#endif  // #ifdef CT909P_IC_SYSTEM

        if(REG_SERVO_GPB_INDATA & dwDesiredPort)
        {
            return 1;
        }
        else
        {
            return 0;
        }
#endif  // #ifdef CT909G_IC_SYSTEM

    }
#ifdef  CT909P_IC_SYSTEM
    else if(bGroup==GPIO_C)
    {
        if( REG_PLAT_GPC_CLEAR & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_D)
    {
        if( REG_PLAT_GPD_CLEAR & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_E)
    {
        if( REG_PLAT_GPE_CLEAR & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_F)
    {
        if( REG_PLAT_GPF_CLEAR & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_G)
    {
        if( REG_PLAT_GPG_CLEAR & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_H)
    {
        if( REG_PLAT_GPH_CLEAR & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
#endif  // #ifdef CT909P_IC_SYSTEM
    // GPCDE @ same register of CT909R
#ifdef CT909R_IC_SYSTEM
    else if( (bGroup >= GPIO_C) && (bGroup <= GPIO_E) )
    {
        if(REG_PLAT_GPCDE_CLEAR & dwDesiredPort)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
#endif  // #ifdef CT909R_IC_SYSTEM
    // LLY2.56, add GPC part for CT909G
#ifdef  CT909G_IC_SYSTEM
    else if(bGroup==GPIO_C)
    {
        if( REG_PLAT_GPC_CLEAR & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
#endif  // #ifdef CT909G_IC_SYSTEM

#ifdef  ENABLE_EXPAND_GPIO
    else if(bGroup==GPIO_EXP)
    {
        if(REG_PLAT_PANEL_KD & dwDesiredPort)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
#endif  // #ifdef  ENABLE_EXPAND_GPIO

    return 0;
 }


//  ************************************************************************************************
//  Function    :   HAL_WriteGPIO
//  Description :   PH or PL the desired GPA port
//  Arguments   :   bGroup: specify the GPIO group
//                  dwPort: port #
//                  bValue: 1: High 0: Low
//  Notice      :   if set direct value as 0, it means output mode
//                  if set direct value as 1, it means input mode
//                  Must disable interrupt first since GPIO maybe accessed by different thread
//  ***********************************************************************************************
// LLY2.13, add for expend GPIO
// LLY2.15, modify the procedure for GPIO CDEF group
// LLY2.50, modify the procedure to support extra CT909P GPIO group
void    HAL_WriteGPIO(BYTE bGroup, DWORD dwPort, BYTE bValue)
{
    DWORD   dwSaveInt;

    // Used to keep desired port position, one port -> one position
    DWORD   dwDesiredPort;

#ifdef CT909R_IC_SYSTEM
    if(bGroup == GPIO_F)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Not implement GPIO group\n");
        return;
    }
#endif  // #ifdef CT909R_IC_SYSTEM

    if(bGroup==GPIO_EXP)
    {
        // LLY2.16, go S/W Expand GPIO path
#ifdef  SW_EXPAND_GPIO
        _SW_Expand_GPO(dwPort, bValue);
        return;
#else   // #ifdef SW_EXPAND_GPIO
        // LLY2.15, must swap for desired port bit[7:0], bit[15:8]
        // ie. port 0 --> port7; port 8 --> port15
        // Since, IC design will output bit[0] first for Expand GPIO GPO mode
        dwPort = (dwPort&0x8) | ((~dwPort)&0x7);
        //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Port: %lx\n", dwPort);
#endif  // #ifdef SW_EXPAND_GPIO
    }

    // LLY2.50, do index map for GPB since it's different @ CT909P
#ifdef  CT909P_IC_SYSTEM
    if(bGroup==GPIO_B)
    {
        BYTE    aMap[6]={8, 6, 15, 16, 17, 18};

        if(dwPort >= 6)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Not supported GPB port\n");
            return;
        }
        dwPort = aMap[dwPort];
    }
#endif  // #ifdef CT909P_IC_SYSTEM

    // Shift the desired port to final position first
    dwDesiredPort = (1<<dwPort);

#ifdef CT909R_IC_SYSTEM
    // Shift left again for GPIO CDE, since they shared the same register @ CT909R
    if(bGroup==GPIO_D)
    {
        // Shift left 8 bit since GPIO D @ bit[15:8] first
        dwDesiredPort = (dwDesiredPort << 8);
    }
    else if(bGroup==GPIO_E)
    {
        // Shift left 16 bit since GPIO E @ bit [24:16] first
        dwDesiredPort = (dwDesiredPort << 16);
    }
#endif  // #ifdef CT909R_IC_SYSTEM

    OS_DISABLE_INTERRUPTS(dwSaveInt);
    if(bGroup==GPIO_A)
    {
        // LLY2.51, config GPA Mux selection register for using GPA as GPIO @ CT909P
#ifdef  CT909P_IC_SYSTEM
        if(dwPort==0)
        {
            // GPAMux[0]=1, GPAMux[2:1]=0
            REG_PLAT_GPA_MUX &= ~(0x7L);
            REG_PLAT_GPA_MUX |= 0x1L;
        }
        else if(dwPort==1)
        {
            // GPAMux[3]=1, GPAMux[5:4]=0
            REG_PLAT_GPA_MUX &= ~(0x7L<<3);
            REG_PLAT_GPA_MUX |= (0x1L<<3);
        }
        else if(dwPort==2)
        {
            // GPAMux[6]=1, GPAMux[8:7]=0
            REG_PLAT_GPA_MUX &= ~(0x7L<<6);
            REG_PLAT_GPA_MUX |= (0x1L<<6);
        }
        else if(dwPort==3)
        {
            // GPAMux[9]=1, GPAMux[11:10]=0
            REG_PLAT_GPA_MUX &= ~(0x7L<<9);
            REG_PLAT_GPA_MUX |= (0x1L<<9);
        }
        else if(dwPort==4 || dwPort==5 || dwPort==12 || dwPort==13 || dwPort==24 || dwPort==25)
        {
            // GPAMux[12]=1; GPAMux[13, 16, 24]=0
            REG_PLAT_GPA_MUX &= ~((0x1L<<13) + (0x1L<<16) + (0x1L<<24));
            REG_PLAT_GPA_MUX |= (0x1L<<12);
        }
        else if(dwPort>=6 && dwPort<=11)
        {
            // GPAMux[14]=1; GPAMux[15]=0
            REG_PLAT_GPA_MUX &= ~(0x1L<<15);
            REG_PLAT_GPA_MUX |= (0x1L<<14);
        }
        else if(dwPort>=14 && dwPort<=19)
        {
            // GPAMux[17]=1; GPAMux[18]=0
            REG_PLAT_GPA_MUX &= ~(0x1L<<18);
            REG_PLAT_GPA_MUX |= (0x1L<<17);
        }
        else if(dwPort==20)
        {
            // GPAMux[20]=1; GPAMux[19]=0
            REG_PLAT_GPA_MUX &= ~(0x1L<<19);
            REG_PLAT_GPA_MUX |= (0x1L<<20);
        }
        else if(dwPort==21)
        {
            // GPAMux[22]=1; GPAMux[21]=0
            REG_PLAT_GPA_MUX &= ~(0x1L<<21);
            REG_PLAT_GPA_MUX |= (0x1L<<22);
        }
        else if(dwPort==22 || dwPort==23)
        {
            // GPAMux[23]=1
            REG_PLAT_GPA_MUX |= (0x1L<<23);
        }
        else if(dwPort>=26 && dwPort<=31)
        {
            // GPAMux[25]=1, GPAMux[26]=0
            REG_PLAT_GPA_MUX &= ~(0x1L<<26);
            REG_PLAT_GPA_MUX |= (0x1L<<25);
        }

#endif  // #ifdef CT909P_IC_SYSTEM

        // Set 0 to enable the desired port as output mode
        REG_PLAT_GPA_IO_DIR_CONTROL &= ~(dwDesiredPort);

        // Set the value to corresponding port.
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            // Notice: only CT909R need read-back the value first then do operation
            //         and it's ok to access the corresponding bit directly by new chip
#ifdef  CT909R_IC_SYSTEM
            REG_PLAT_GPA_SET |= dwDesiredPort;
#else   // #ifdef CT909R_IC_SYSTEM
            REG_PLAT_GPA_SET = dwDesiredPort;
#endif  // #ifdef CT909R_IC_SYSTEM
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_PLAT_GPA_CLEAR = dwDesiredPort;
        }
    }
    else if(bGroup==GPIO_B)
    {
        // LLY2.56, GPB control can be handled by MPEG part @ CT909G
        // Notice: default disable MPEG path otherwise can't access servo internal H/W block
#if 0// def  CT909G_IC_SYSTEM
        // Notice: bit[16]:GPB_EN, must set as 1, then can control GPB by MPEG part
        REG_PLAT_GPB_IO_DIR_CONTROL |= (0x1L<<16);

        // Set 0 to enable the desired port as output mode
        REG_PLAT_GPB_IO_DIR_CONTROL &= ~(dwDesiredPort);


        // Set the value to corresponding port.
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            REG_PLAT_GPB_SET = dwDesiredPort;
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_PLAT_GPB_CLEAR = dwDesiredPort;
        }
#else   // #ifdef CT909G_IC_SYTEM

        // Set GPB output function selection register as GPIO mode by 0
        // LLY2.15, only can modify the desired port
        REG_SERVO_GPB_FUNC_SEL &= ~(dwDesiredPort);

        // Set 0 to enable the desired port as output mode
        REG_SERVO_GPB_IO_DIR_CONTROL &= ~(dwDesiredPort);

        // Set the value to corresponding port
        // LLY2.56, split GPB output data control to two different register,
        // ie. one is for set, another is for clear
        // And, only need to set the desired port, ie. un-necessary to read-back the value first
#ifdef  CT909G_IC_SYSTEM
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            REG_SERVO_GPB_SETDATA = dwDesiredPort;
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_SERVO_GPB_CLEARDATA = dwDesiredPort;
        }
#else   // #ifdef CT909G_IC_SYSTEM
        if(bValue)
        {
            REG_SERVO_GPB_OUTDATA |= (dwDesiredPort);
        }
        else
        {
            REG_SERVO_GPB_OUTDATA &= ~(dwDesiredPort);
        }
#endif  // #ifdef CT909G_IC_SYSTEM
#endif  // #ifdef CT909G_IC_SYSTEM
    }
#ifdef  CT909P_IC_SYSTEM
    else if(bGroup==GPIO_C)
    {
        // Set 0 to enable the desired port as output mode
        REG_PLAT_GPC_IO_DIR_CONTROL &= ~(dwDesiredPort);

        // Set the value to corresponding port.
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            REG_PLAT_GPC_SET = dwDesiredPort;
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_PLAT_GPC_CLEAR = dwDesiredPort;
        }
    }
    else if(bGroup==GPIO_D)
    {
        // Set 0 to enable the desired port as output mode
        REG_PLAT_GPD_IO_DIR_CONTROL &= ~(dwDesiredPort);

        // Set the value to corresponding port.
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            REG_PLAT_GPD_SET = dwDesiredPort;
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_PLAT_GPD_CLEAR = dwDesiredPort;
        }
    }
    else if(bGroup==GPIO_E)
    {
        // Set 0 to enable the desired port as output mode
        REG_PLAT_GPE_IO_DIR_CONTROL &= ~(dwDesiredPort);

        // Set the value to corresponding port.
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            REG_PLAT_GPE_SET = dwDesiredPort;
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_PLAT_GPE_CLEAR = dwDesiredPort;
        }
    }
    else if(bGroup==GPIO_F)
    {
        // Set 0 to enable the desired port as output mode
        REG_PLAT_GPF_IO_DIR_CONTROL &= ~(dwDesiredPort);

        // Set the value to corresponding port.
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            REG_PLAT_GPF_SET = dwDesiredPort;
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_PLAT_GPF_CLEAR = dwDesiredPort;
        }
    }
    else if(bGroup==GPIO_G)
    {
        // Set 0 to enable the desired port as output mode
        REG_PLAT_GPG_IO_DIR_CONTROL &= ~(dwDesiredPort);

        // Set the value to corresponding port.
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            REG_PLAT_GPG_SET = dwDesiredPort;
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_PLAT_GPG_CLEAR = dwDesiredPort;
        }
    }
    else if(bGroup==GPIO_H)
    {
        // Set 0 to enable the desired port as output mode
        REG_PLAT_GPH_IO_DIR_CONTROL &= ~(dwDesiredPort);

        // Set the value to corresponding port.
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            REG_PLAT_GPH_SET = dwDesiredPort;
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_PLAT_GPH_CLEAR = dwDesiredPort;
        }
    }
#endif  // #ifdef CT909P_IC_SYSTEM
    // GPCDE @ same register of CT909R
#ifdef  CT909R_IC_SYSTEM
    else if( (bGroup>=GPIO_C) && (bGroup<=GPIO_E) )
    {
        // Set 0 to enable the desired port as output mode
        REG_PLAT_GPCDE_IO_DIR_CONTROL &= ~(dwDesiredPort);

        // Set the value to corresponding port.
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            REG_PLAT_GPCDE_SET |= dwDesiredPort;
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_PLAT_GPCDE_CLEAR = dwDesiredPort;
        }
    }

#endif  // #ifdef CT909R_IC_SYSTEM
    // LLY2.56, add for CT909G GPC control procedure.
#ifdef  CT909G_IC_SYSTEM
    else if(bGroup==GPIO_C)
    {
        // Set 0 to enable the desired port as output mode
        REG_PLAT_GPC_IO_DIR_CONTROL &= ~(dwDesiredPort);

        // Set the value to corresponding port.
        if(bValue)
        {
            // Set 1 to desired port to write the value as 1
            REG_PLAT_GPC_SET = dwDesiredPort;
        }
        else
        {
            // Set 1 to desired port to clear the value as 1
            REG_PLAT_GPC_CLEAR = dwDesiredPort;
        }
    }

#endif  // #ifdef CT909G_IC_SYSTEM

#ifdef  ENABLE_EXPAND_GPIO
    else if(bGroup==GPIO_EXP)
    {
        // LLY2.15, if enable "ACTIVE_LOW", ie, means F/W must write 0, then output will be 1
        // So, if user want to see 1, F/W must write 0
#ifdef  ACTIVE_LOW
        if(bValue)
        {
            REG_PLAT_PANEL_DISP0 &= ~(dwDesiredPort);
        }
        else
        {
            REG_PLAT_PANEL_DISP0 |= (dwDesiredPort);
        }
#else   // #ifdef ACTIVE_LOW
        if(bValue)
        {
            REG_PLAT_PANEL_DISP0 |= (dwDesiredPort);
        }
        else
        {
            REG_PLAT_PANEL_DISP0 &= ~(dwDesiredPort);
        }
#endif  // #ifdef ACTIVE_LOW
    }
#endif  // #ifdef  ENABLE_EXPAND_GPIO
    else
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: not implemented GPIO group (W) : %lx\n", bGroup);
    }


    OS_RESTORE_INTERRUPTS(dwSaveInt);

}


// LLY2.51 create.
//  *******************************************************************************************
//  Function    :   HAL_ReadGPO_Value
//  Description :   Read back the desired GPO port current value
//  Arguments   :   bGroup: specify the GPIO group
//                  dwPort: port #
//  Return      :   The desired GPO port current value.
//  *******************************************************************************************
BYTE    HAL_ReadGPO_Value(BYTE bGroup, DWORD dwPort)
{
    // Used to keep desired port position, one port -> one position
    DWORD   dwDesiredPort;

#ifdef CT909R_IC_SYSTEM
    if(bGroup == GPIO_F)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Not implement GPIO group\n");
        return FALSE;
    }
#endif  // #ifdef CT909R_IC_SYSTEM

    if(bGroup==GPIO_EXP)
    {
        // LLY2.16, go S/W Expand GPIO path
#ifdef  SW_EXPAND_GPIO
        if(__dwExpGPOVal & (1<<dwPort))
        {
            return 1;
        }
        else
        {
            return 0;
        }
#else   // #ifdef SW_EXPAND_GPIO
        // LLY2.15, must swap for desired port bit[7:0], bit[15:8]
        // ie. port 0 --> port7; port 8 --> port15
        // Since, IC design will output bit[0] first for Expand GPIO GPO mode
        dwPort = (dwPort&0x8) | ((~dwPort)&0x7);
        //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Port: %lx\n", dwPort);
#endif  // #ifdef SW_EXPAND_GPIO
    }

    // LLY2.50, do index map for GPB since it's different @ CT909P
#ifdef  CT909P_IC_SYSTEM
    if(bGroup==GPIO_B)
    {
        BYTE    aMap[6]={8, 6, 15, 16, 17, 18};

        if(dwPort >= 6)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Not supported GPB port\n");
            return 0;
        }
        dwPort = aMap[dwPort];
    }
#endif  // #ifdef CT909P_IC_SYSTEM

    // Shift the desired port to final position first
    dwDesiredPort = (1<<dwPort);

#ifdef CT909R_IC_SYSTEM
    // Shift left again for GPIO CDE, since they shared the same register @ CT909R
    if(bGroup==GPIO_D)
    {
        // Shift left 8 bit since GPIO D @ bit[15:8] first
        dwDesiredPort = (dwDesiredPort << 8);
    }
    else if(bGroup==GPIO_E)
    {
        // Shift left 16 bit since GPIO E @ bit [24:16] first
        dwDesiredPort = (dwDesiredPort << 16);
    }
#endif  // #ifdef CT909R_IC_SYSTEM

    if(bGroup==GPIO_A)
    {
        // Return last GPO value
        if( REG_PLAT_GPA_SET & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_B)
    {
        // Return last GPO value
        // LLY2.56, GPB control can be handled by MPEG part @ CT909G
        // Notice: default disable MPEG path otherwise can't access servo internal H/W block
#if 0// def  CT909G_IC_SYSTEM
        if( REG_PLAT_GPB_SET & dwDesiredPort )
#else   // #ifdef CT909G_IC_SYSTEM
        // LLY2.56, split GPB output data control to two different register,
        // ie. one is for set, another is for clear
#ifdef  CT909G_IC_SYSTEM
        if(REG_SERVO_GPB_SETDATA & dwDesiredPort)
#else   // #ifdef  CT909G_IC_SYSTEM
        if(REG_SERVO_GPB_OUTDATA & dwDesiredPort)
#endif  // #ifdef CT909G_IC_SYSTEM
#endif  // #ifdef CT909G_IC_SYSTEM
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
#ifdef  CT909P_IC_SYSTEM
    else if(bGroup==GPIO_C)
    {
        // Return last GPO value
        if( REG_PLAT_GPC_SET & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_D)
    {
        // Return last GPO value
        if( REG_PLAT_GPD_SET & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_E)
    {
        // Return last GPO value
        if( REG_PLAT_GPE_SET & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_F)
    {
        // Return last GPO value
        if( REG_PLAT_GPF_SET & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_G)
    {
        // Return last GPO value
        if( REG_PLAT_GPG_SET & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(bGroup==GPIO_H)
    {
        // Return last GPO value
        if( REG_PLAT_GPH_SET & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
#endif  // #ifdef CT909P_IC_SYSTEM
    // GPCDE @ same register of CT909R
#ifdef  CT909R_IC_SYSTEM
    else if( (bGroup>=GPIO_C) && (bGroup<=GPIO_E) )
    {
        // Return last GPO value
        if( REG_PLAT_GPCDE_SET & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
#endif  // #ifdef CT909R_IC_SYSTEM
    // LLY2.56, add CT909G GPC control
#ifdef  CT909G_IC_SYSTEM
    else if(bGroup==GPIO_C)
    {
        // Return last GPO value
        if( REG_PLAT_GPC_SET & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
#endif  // #ifdef CT909G_IC_SYSTEM

#ifdef  ENABLE_EXPAND_GPIO
    else if(bGroup==GPIO_EXP)
    {
        // Return GPO current value
        // If "ACTIVE_LOW", means the value is 0, but output is 1, so must return 1
#ifdef  ACTIVE_LOW
        if( REG_PLAT_PANEL_DISP0 & dwDesiredPort )
        {
            return 0;
        }
        else
        {
            return 1;
        }
#else   // #ifdef ACTIVE_LOW
        if( REG_PLAT_PANEL_DISP0 & dwDesiredPort )
        {
            return 1;
        }
        else
        {
            return 0;
        }
#endif  // #ifdef ACTIVE_LOW
    }
#endif  // #ifdef  ENABLE_EXPAND_GPIO
    else
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: not implemented GPIO group (W) : %lx\n", bGroup);
    }

    return 0;


}


// LLY2.13, add expand GPIO related APIs
//  *******************************************************************************************
//  Function    :   HAL_Init_ExpIO
//  Description :   Configure the related register while using expand GPIO
//  Argument    :   None
//  Return      :   None
//  *******************************************************************************************
void    HAL_Init_ExpGPIO(void)
{
#ifdef  ENABLE_EXPAND_GPIO

    DWORD   dwSaveInt;

    // LLY2.51, config different register for expand GPIO
#ifdef  CT909P_IC_SYSTEM
    // CT909P expand GPIO control are moved to GPCMux
#ifndef NO_LATCH_D1
    // Using GPC[4], must enable GPCMux[3]=1
    REG_PLAT_GPC_MUX |= (1L<<3);
#endif  // #ifndef NO_LATCH_D1

#ifndef NO_LATCH_D0
    // Using GPC[3], must enable GPCMux[2]=1
    REG_PLAT_GPC_MUX |= (1L<<2);
#endif  // #ifndef NO_LATCH_D0

#ifndef NO_KEY_DETECT_1
    // Using GPC[5], must enable GPCMux[6]=1
    REG_PLAT_GPC_MUX |= (1L<<6);
#endif  // #ifndef NO_KEY_DETECT_1

#ifndef NO_KEY_DETECT_0
    // Using GPC[0], must enable GPCMux[0]=1
    REG_PLAT_GPC_MUX |= (1L);
#endif  // #ifndef NO_KEY_DETECT_0

    // Using GPC[1] as KS_DATA pin, GPC[2] as KS_CLK pin
    // So, must enable GPCMux[1]=1
    REG_PLAT_GPC_MUX |= (1L<<1);

#endif  // #ifdef CT909P_IC_SYSTEM

#ifdef  CT909R_IC_SYSTEM
    // LLY2.15, modify the procedure to enable each feature base on user selection.
    // Set bit[24] as 1 to support latch D1, using GPB[14]
#ifndef NO_LATCH_D1
    REG_PLAT_MPLL_CONTROL |= (1<<24);
#endif  // #ifndef NO_LATCH_D1

    // Set bit[23] as 1 to support latch D0, using GPB[1]
#ifndef NO_LATCH_D0
    REG_PLAT_MPLL_CONTROL |= (1<<23);
#endif  // #ifndef NO_LATCH_D0

    // Set bit[22] as 1 to support key detect 1, using GPB[11]
#ifndef NO_KEY_DETECT_1
    REG_PLAT_MPLL_CONTROL |= (1<<22);
#endif  // #ifndef NO_KEY_DETECT_1

    // Set bit[21] as 1 to support key detect 0, using GPB[10]
#ifndef NO_KEY_DETECT_0
    REG_PLAT_MPLL_CONTROL |= (1<<21);
#endif  // #ifndef NO_KEY_DETECT_0
#endif  // #ifdef CT909R_IC_SYSTEM

    // LLY2.16, need disable interrupt and do key lock before access system configure register 1
    OS_DISABLE_INTERRUPTS( dwSaveInt );
    MACRO_PLAT_KEY_LOCK( );

    // Set bit 15 as 1 to enable panel key scan (expand GPIO function)
    REG_PLAT_SYSTEM_CONFIGURATION1 |= (1<<15);   // set 15th bit as 1 to enable panel scan

    MACRO_PLAT_KEY_UNLOCK();
    OS_RESTORE_INTERRUPTS (dwSaveInt);

    // Set serial clock divider for expand GPIO
    REG_PLAT_PANEL_CLK = 0x00FF0000;

    // Config expand GPIO environment
    // bit[10:8]=0x1, set common port number =1 (2 port); 0 (1 port)
    // bit[1]=1, set active polarity of display is high-level trigger
    // bit[2]=0, set active polarity of keypad detection is low (since current circuit is pull high)
    // Notice: bit[2] & bit[0] value must be same
#ifdef  ACTIVE_LOW
    REG_PLAT_PANEL_CFG = 0x00000102;
#else   // #ifdef ACTIVE_LOW
    REG_PLAT_PANEL_CFG = 0x00000107;
#endif  // #ifdef ACTIVE_LOW

//    REG_PLAT_PROC1_2ND_INT_MASK_DISABLE = INT_PROC1_2ND_PANEL_IR;   // This is for Panel intrrupt
//    REG_PLAT_PANEL_INTCTRL = 0xFFFF0000;  // Edge trigger
//    REG_PLAT_PANEL_INTCTRL = 0x00000000;    // Level trigger

    // Set bit[15:0] as 1 to mask interrupt
    //     bit[31:16] as 0 to using level trigger
    // Notice: interrupt only can use edge trigger
    REG_PLAT_PANEL_INTCTRL = 0x0000FFFF;


//    REG_PLAT_PANEL_INT = 0x0;

//    REG_PLAT_PANEL_DISP0 = 0;

#else   // #ifdef ENABLE_EXPAND_GPIO
    // LLY2.60, do error protection while not support H/W expand GPIO

#ifdef  CT909P_IC_SYSTEM
    // [1] Disable GPC[0, 1, 2, 3, 4, 5] pin for H/W expand GPIO usage @ CT909P IC
    //     So, clear GPCMux[0, 1, 2, 3, 6]=0
    REG_PLAT_GPC_MUX &= ~(0x4fL);
#endif  //

#endif  // #ifdef  ENABLE_EXPAND_GPIO
}


#ifdef  SUPPORT_I2C
VOID _Delay_I2C(void)
{
    BYTE i;

    for(i = 0; i < 20; i++);
}
BOOL 	HAL_I2C_Write (BYTE DevSel, PI2C_CONFIG  pConfig, PTRANSFER_CONFIG pTransferConfig )
//BOOL _I2C_Write (PI2C_CONFIG  pConfig, BYTE DevSel, BYTE *pBuffer, BYTE count )
{
    BYTE    i;
    BOOL    bWriteResult = FALSE;

#ifdef  ENABLE_PANEL_KEYSCAN
    __icVFDnow = 0x01; //Disable Panel Keyscan
#endif  // #ifdef  ENABLE_PANEL_KEYSCAN

    HAL_I2CSrc_Ctrl(MUTEX_ACTION_LOCK);

    _IIC_START(pConfig);
    _IIC_TxByte (pConfig, DevSel );     // Write Slave Address
    if (_IIC_GetACK(pConfig))
    {
        goto WRITE_FUNC_END;
//        return FALSE;
    }

    // issue write address first
        _IIC_TxByte(pConfig, (*pTransferConfig).bAddr);
        if (_IIC_GetACK(pConfig))
    {
        goto WRITE_FUNC_END;
//        return FALSE;
    }

    for ( i = 0; i < (*pTransferConfig).bCount; i++ )
    {
        _IIC_TxByte(pConfig, *((*pTransferConfig).pbBuffer)++);
        if (_IIC_GetACK(pConfig))
        {
            goto WRITE_FUNC_END;
//            return FALSE;
        }
    }

    _IIC_STOP(pConfig);

    bWriteResult = TRUE;

WRITE_FUNC_END:

    HAL_I2CSrc_Ctrl(MUTEX_ACTION_UNLOCK);

#ifdef  ENABLE_PANEL_KEYSCAN
    __icVFDnow = 0x00; //Enable Panel Keyscan
#endif  // #ifdef  ENABLE_PANEL_KEYSCAN

    if (I2C_ID_EEPROM == DevSel)
        OS_DelayTime(COUNT_50_MSEC);  // The write cycle time needs to > 10 ms

//    return TRUE;

    return bWriteResult;
}
BOOL 	HAL_I2C_Read (BYTE DevSel, PI2C_CONFIG  pConfig, PTRANSFER_CONFIG pTransferConfig)
{
    BYTE i;
    BOOL bReadResult = FALSE;

#ifdef  ENABLE_PANEL_KEYSCAN
    __icVFDnow = 0x01; //Disable Panel Keyscan
#endif  // #ifdef  ENABLE_PANEL_KEYSCAN

    HAL_I2CSrc_Ctrl(MUTEX_ACTION_LOCK);

    _IIC_START(pConfig);

    _IIC_TxByte(pConfig, DevSel);
    if(_IIC_GetACK(pConfig))
    {
        goto READ_FUNC_END;
//        return FALSE;
    }

    _IIC_TxByte(pConfig, (*pTransferConfig).bAddr);
    if(_IIC_GetACK(pConfig))
    {
        goto READ_FUNC_END;
//        return FALSE;
    }

    // random read
    _IIC_START(pConfig);
    _IIC_TxByte(pConfig, (BYTE)(DevSel|0x01));
    if(_IIC_GetACK(pConfig))
    {
        goto READ_FUNC_END;
//        return FALSE;
    }

    *((*pTransferConfig).pbBuffer) ++ = _IIC_RxByte(pConfig);
    _IIC_STOP(pConfig);

    _Delay_I2C();

    for ( i = 0; i < (*pTransferConfig).bCount - 1; i++ )
    {
        _IIC_START(pConfig);
        _IIC_TxByte(pConfig, (BYTE)(DevSel|0x01));
        if(_IIC_GetACK(pConfig))
        {
            goto READ_FUNC_END;
//            return FALSE;
        }
        *((*pTransferConfig).pbBuffer) ++ = _IIC_RxByte(pConfig);
        _IIC_STOP(pConfig);
        _Delay_I2C();
    }

      bReadResult = TRUE;

READ_FUNC_END:

    HAL_I2CSrc_Ctrl(MUTEX_ACTION_UNLOCK);

#ifdef  ENABLE_PANEL_KEYSCAN
    __icVFDnow = 0x00; //Enable Panel Keyscan
#endif  // #ifdef  ENABLE_PANEL_KEYSCAN

//    return TRUE;

    return bReadResult;
}


VOID _IIC_START (PI2C_CONFIG  pConfig)
{
    _I2C_Set_SDAT_HIGH(pConfig);
    _I2C_Set_SCLK_HIGH(pConfig);
    _I2C_Set_SDAT_LOW(pConfig);
    _I2C_Set_SCLK_LOW(pConfig);
}

VOID _IIC_TxByte (PI2C_CONFIG  pConfig, BYTE bValue)
{
    BYTE    i;
    for(i = 0; i < 8; i++)
    {
        if( bValue & 0x80)
        {
            _I2C_Set_SDAT_HIGH(pConfig);
        }
        else
        {
            _I2C_Set_SDAT_LOW(pConfig);
        }
        _I2C_Set_SCLK_HIGH(pConfig);
        _I2C_Set_SCLK_LOW(pConfig);

        bValue = bValue << 1 ;
    }
}

BYTE _IIC_RxByte (PI2C_CONFIG  pConfig)
{
    BYTE i, bRet;

    bRet = 0x00;
    for ( i= 0x80; i; i >>= 1)
    {
        _I2C_Set_SCLK_HIGH(pConfig);

        if ( _I2C_Get_SDAT(pConfig) )
            bRet |= i;

        _I2C_Set_SCLK_LOW(pConfig);
    }

    return bRet;
}

BYTE _IIC_GetACK (PI2C_CONFIG  pConfig)
{
    BYTE    bRet;

    _Delay_I2C();
    HAL_ReadGPIO(pConfig->bIOGroup, pConfig->bSDAT_Pin);  //set input mode only
    _Delay_I2C();

    _I2C_Set_SCLK_HIGH(pConfig);
    bRet = _I2C_Get_SDAT(pConfig);
    _I2C_Set_SCLK_LOW(pConfig);
    return bRet;
}

VOID _IIC_SetACK (PI2C_CONFIG  pConfig)
{
    _I2C_Set_SDAT_LOW (pConfig);
    _I2C_Set_SCLK_HIGH (pConfig);
    _I2C_Set_SCLK_LOW (pConfig);
}


VOID _IIC_STOP (PI2C_CONFIG  pConfig)
{
    _I2C_Set_SDAT_LOW(pConfig);
    _I2C_Set_SCLK_HIGH(pConfig);
    _I2C_Set_SDAT_HIGH(pConfig);
}

VOID _I2C_Set_SDAT_HIGH(PI2C_CONFIG  pConfig)
{
    _Delay_I2C();
//Alan075    HAL_WriteGPC( PIN_SDAT, 1);
    HAL_WriteGPIO(pConfig->bIOGroup, pConfig->bSDAT_Pin, 1);
}

VOID _I2C_Set_SCLK_HIGH(PI2C_CONFIG  pConfig)
{
    _Delay_I2C();
//Alan075    HAL_WriteGPC( PIN_SCLK, 1);
    HAL_WriteGPIO(pConfig->bIOGroup, pConfig->bSCLK_Pin, 1);
}

VOID _I2C_Set_SDAT_LOW(PI2C_CONFIG  pConfig)
{
    _Delay_I2C();
//Alan075    HAL_WriteGPC( PIN_SDAT, 0);
    HAL_WriteGPIO(pConfig->bIOGroup, pConfig->bSDAT_Pin, 0);
}

VOID _I2C_Set_SCLK_LOW(PI2C_CONFIG  pConfig)
{
    _Delay_I2C();
//Alan075    HAL_WriteGPC( PIN_SCLK, 0);
    HAL_WriteGPIO(pConfig->bIOGroup, pConfig->bSCLK_Pin, 0);
}

BYTE _I2C_Get_SDAT(PI2C_CONFIG  pConfig)
{
    _Delay_I2C();
//Alan075    return ( HAL_ReadGPC( PIN_SDAT) );
    return (HAL_ReadGPIO(pConfig->bIOGroup, pConfig->bSDAT_Pin));
}
#endif  //#ifdef  SUPPORT_I2C

//**********************************************************************
// Description  :   DRAM Copy from one DRAM location to another DRAM location
// Arguments    :   dwSrc: the source DRAM location
//              :   dwDst: The destination DRAM location
//              :   dwLen: copy length in BYTE
// Return       :   None
//*********************************************************************
void    HAL_CopyDRAM(DWORD dwSrc, DWORD dwDst, DWORD dwLen) //test OK
{
    DWORD   dwIndx;

    //disable/flush cache
    //REG_PLAT_CACHE_CONTROL |= CACHE_FLUSH_DCACHE; //dump-in OK => do flush immediately?
    PLAT_PROC1_DCACHE_FLUSH;

    //JPU Bitmap copy is faster=> consider later ?

    if ( (dwSrc%4) || (dwDst%4) )
    {
        //Copy BYTE by BYTE
        for ( dwIndx=0; dwIndx<dwLen; dwIndx++, dwSrc++, dwDst++)
        {
            *((volatile BYTE *)dwDst ) = *((volatile BYTE *)dwSrc );
        }
    }
    else
    {
        //Copy DWORD by DWORD
        for ( dwIndx=0; dwIndx<dwLen/4; dwIndx++, dwSrc+=4, dwDst+=4)
        {
            *((volatile DWORD *)dwDst ) = *((volatile DWORD *)dwSrc );
        }
        for ( dwIndx=0; dwIndx<dwLen%4; dwIndx++, dwSrc++, dwDst++)
        {
            *((volatile BYTE *)dwDst ) = *((volatile BYTE *)dwSrc );
        }
    }
}

//dwLen: byte length; dwOffset: byte offset relative to the  beginning of User Data
void    HAL_DumpUserData( BYTE* pbBuf, DWORD dwLen, DWORD dwPosition ) //test OK
{
    DWORD   dwIndx;

    //disable/flush cache
    //REG_PLAT_CACHE_CONTROL |= CACHE_FLUSH_DCACHE; //dump-in OK => do flush immediately?
    PLAT_PROC1_DCACHE_FLUSH;

    //Copy BYTE by BYTE first
    //JPU Bitmap copy is faster=> consider later ?
    for ( dwIndx=0; dwIndx<dwLen; dwIndx++)
    {
        *(pbBuf+dwIndx) = *((volatile BYTE *) (dwPosition+dwIndx) );
    }
}

// SYuan2.77-909P: Suuport to set UART TX , UART RX indendently
//****************************************************************************
//Description :   Set UART baud rate value
//  Argument    :   dwSysCLK, current system clock
//                  dwBaudRate, desired baudrate value
//                  dwPort, desired UART port
// return      :     none.
//***************************************************************
VOID    HAL_UART_BaudRateSet(DWORD dwSysCLK, DWORD dwBaudRate, DWORD dwPort)
{
    BYTE bPort;

    //get UART port , ex: UART1 or UART2
    bPort = GET_UART_PORT(dwPort);
    bPort--;    
    //REG_PLAT_UART1_SCALER = (((SYSTEM_CLK*10)/(UART_BAUDRATE_QUAD_SPEED*8))-5)/10;
    REG_PLAT_UART_SCALER(bPort) = (((dwSysCLK*10)/(dwBaudRate*8))-5)/10;
}

// SYuan2.77-909P: Suuport to set UART TX , UART RX indendently
//**************************************************************
//function   :HAL_UART_Disable
//descriptor : Disable all possible UART path.
//arguments : dwPort :spcify UART port. ex: UART1_TX or UART1_RX .
//return     :none
//***************************************************************
VOID    HAL_UART_Disable(DWORD dwPort)
{
    BYTE bPort;

    //get UART port , ex: UART1 or UART2
    bPort = GET_UART_PORT(dwPort);
    bPort--;
    // LLY2.60, porting the necessary code for CT909P IC
    // UART are shared the same pin w/ other function,
    // So, if disable UART function, must release it to others.
    
#ifdef  CT909P_IC_SYSTEM    
    if(dwPort == HAL_UART1_TX)
    {
        //Disable all possible UART1 TX path
        //[1] from SPI -- GPA[[1], so clear GPAMux[4]=0
        REG_PLAT_GPA_MUX &= ~(1L<<4);
        //[2] from card reader -- GPC[7], so clear GPCMux[12]=0
        REG_PLAT_GPC_MUX &= ~(1L<<12);
        //[3] from NIM -- GPG[1], so clear GPG[6]=0
        REG_PLAT_GPG_MUX &= ~(1L<<6);  
        //REG_PLAT_UART_CONTROL(bPort) = 0;
        REG_PLAT_UART_CONTROL(bPort) &= ~(0xfffffffC+UART_TX_ENABLE);
    }
    else if(dwPort == HAL_UART1_RX)
    {
        //Disable all possible UART1 RX path
        //[1] from SPI -- GPA[0], so clear GPAMux[1]=0
        REG_PLAT_GPA_MUX &= ~(1L<<1);
        //[2] from card reader -- GPC[8], so clear GPCMux[16]=0
        REG_PLAT_GPC_MUX &= ~(1L<<16);
        //[3] from NIM -- GPG[0], so clear GPGMux[2]=0
        REG_PLAT_GPG_MUX &= ~(1L<<2);  
        //REG_PLAT_UART_CONTROL(bPort) = 0;
        REG_PLAT_UART_CONTROL(bPort) &= ~(0xfffffffC+UART_RX_ENABLE);
    }    
    else if(dwPort == HAL_UART2_TX)
    {
        //Disable all possible UART2 TX path
        //[1] from SPI -- GPA[3], so clear GPAMux[11]=0
        REG_PLAT_GPA_MUX &= ~(1L<<11);
        //[2] from NIM -- GPG[3], so clear GPGMux[15]=0
        REG_PLAT_GPG_MUX &= ~(1L<<15);
        //[3] from expand GPIO -- GPC[4], so clear GPCMux[5]=0
        REG_PLAT_GPC_MUX &= ~(1L<<5);  
        //REG_PLAT_UART_CONTROL(bPort) = 0;
        REG_PLAT_UART_CONTROL(bPort) &= ~(0xfffffffC+UART_TX_ENABLE);
    }
    else if(dwPort == HAL_UART2_RX)
    {
        //Disable all poosible UART RX path
        //[1] from SPI -- GPA[2], so clear GPAMux[8]=0
        REG_PLAT_GPA_MUX &= ~(1L<<8);
        //[2] from NIM -- GPG[2], so clear GPGMux[11]=0
        REG_PLAT_GPG_MUX &= ~(1L<<11);
        //[3] from expand GPIO -- GPC[5], so clear GPCMux[7]=0
        REG_PLAT_GPC_MUX &= ~(1L<<7); 
        //REG_PLAT_UART_CONTROL(bPort) = 0;
        REG_PLAT_UART_CONTROL(bPort) &= ~(0xfffffffC+UART_RX_ENABLE);
    }   
#endif  // #ifdef  CT909P_IC_SYSTEM

    // LLY2.75, disable UART pin control for CT909G    
#ifdef  CT909G_IC_SYSTEM
   #if 0
    if(dwPort == HAL_UART1)
    {
        // set bit[12:8] of System Pin-Use Register 0 to disable UART1
        REG_PLAT_SYS_PIN_USE0 &= ~(0x1fL<<8);
        REG_PLAT_UART_CONTROL(bPort) &= ~(UART_RX_ENABLE|UART_TX_ENABLE);
    }    
    else 
#endif
    if(dwPort == HAL_UART1_TX)
    {
        // set bit[12:10] of System Pin-Use Register 0 to disable UART1
        REG_PLAT_SYS_PIN_USE0 &= ~(0x1CL<<8);
        REG_PLAT_UART_CONTROL(bPort) &= ~(0xfffffffC+UART_TX_ENABLE);
    }
    else if (dwPort == HAL_UART1_RX)
    {
        // set bit[9:8] of System Pin-Use Register 0 to disable UART1
        REG_PLAT_SYS_PIN_USE0 &= ~(0x3L<<8);
        REG_PLAT_UART_CONTROL(bPort) &= ~(0xfffffffC+UART_RX_ENABLE);
    }   
#if 0
    else if(dwPort == HAL_UART2)
    {
        // set bit[1:0] of System Pin-Use Register 0 to disable UART2
        REG_PLAT_SYS_PIN_USE0 &= ~(0x3L);
        REG_PLAT_UART_CONTROL(bPort) &= ~(UART_RX_ENABLE|UART_TX_ENABLE);
    }    
#endif
    else if(dwPort == HAL_UART2_TX)
    {
        // set bit[1] of System Pin-Use Register 0 to disable UART2
        REG_PLAT_SYS_PIN_USE0 &= ~(0x2L);
        REG_PLAT_UART_CONTROL(bPort) &= ~(0xfffffffC+UART_TX_ENABLE);
    }
    else if (dwPort == HAL_UART2_RX)
    {
        // set bit[0] of System Pin-Use Register 0 to disable UART2
        REG_PLAT_SYS_PIN_USE0 &= ~(0x1L);
        REG_PLAT_UART_CONTROL(bPort) &= ~(0xfffffffC+UART_RX_ENABLE);
    }    
#endif  // #ifdef CT909G_IC_SYSTEM
    //REG_PLAT_UART_CONTROL(bPort) &= ~(UART_RX_ENABLE|UART_TX_ENABLE);
    //REG_PLAT_UART_CONTROL(bPort) = 0;
}

//SYuan2.77-909P: support to set UART2 TX & RX independently.
//**************************************************************************
//function   : HAL_UART_Enable
//description : Enable UART TX, RX path.
//arguments : dwport : specify UART port. ex UART1_TX or UART1_RX or UART2_TX or UART_RX
//           bPath  : specify UART GPIO path. ex: NIM_PATH ...etc.
//return    :none.
//****************************************************************************
VOID    HAL_UART_Enable(DWORD dwPort, BYTE bPath)
{
    BYTE bPort;
    DWORD dwBug;

    //get UART port , ex: UART1 or UART2
    bPort = GET_UART_PORT(dwPort);
    bPort--;
    
#ifdef  CT909G_IC_SYSTEM
    #if 0
    if(dwPort == HAL_UART1)
    {       
        // LLY2.56, default configuration for CT909G EV board
        // UART1_TX: using GPA[10], so set system pin-use0[12]=1
        // UART1_RX: using GPA[6], so set system pin-use0[8]=1, [24, 16, 0]=0
        REG_PLAT_SYS_PIN_USE0 |= (0x1L<<12) + (0x1L<<8);
        REG_PLAT_SYS_PIN_USE0 &= ~((0x1L<<24)+(0x1L<<16)+(0x1L));
        REG_PLAT_UART_CONTROL(bPort) = (UART_RX_ENABLE|UART_TX_ENABLE);
    }
    
    else 
#endif
    if(dwPort == HAL_UART1_TX)
    {
        //for Debug Mode, __dwDebugMode must be set first. [8~11] be used indicate UART1_TX.
        //bPath = (__dwDebugMode & 0xf00) >> 8;   
        
        if (bPath == VFD_CS_PATH)    //GPA_10
        {
            //  default configuration for CT909G EV board
            // UART1_TX: using GPA[10], so set system pin-use0[12]=1
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<12); 
            REG_PLAT_UART_CONTROL(bPort) |= (UART_TX_ENABLE);
        }
        else if (bPath == VFD_PATH) //GPA_9
        {
            // default configuration for CT909G EV board
            // UART1_TX: using GPA[9], so set system pin-use0[11]=1 , [27]=0
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<11); 
            REG_PLAT_SYS_PIN_USE0 &= ~(0x1L<<27);
            REG_PLAT_UART_CONTROL(bPort) |= (UART_TX_ENABLE);
        }
        else if (bPath == IO_MUTE_PATH) //GPA_4
        {
            // default configuration for CT909G EV board
            // UART1_TX: using GPA[4], so set system pin-use0[10]=1 , [26]=0, [17]=0, [10]=0, [1]=0
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<10); 
            REG_PLAT_SYS_PIN_USE0 &= ~((0x1L<<26)+(0x1L<<17)+(0x1L<<1));
            REG_PLAT_UART_CONTROL(bPort) |= (UART_TX_ENABLE);        
        }
    }    
    
    else if(dwPort == HAL_UART1_RX)
    {
        //for Debug Mode, __dwDebugMode must be set first. [12 ~ 15] be used indicate UART1_RX.
        //bPath = (__dwDebugMode & 0xf000) >> 12;
        if (bPath == SCART_PATH) //GPA_6
        {
            // default configuration for CT909G EV board
            // UART1_RX: using GPA[6], so set system pin-use0[8]=1, [24, 16, 0]=0
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<8);
            REG_PLAT_SYS_PIN_USE0 &= ~((0x1L<<24)+(0x1L<<16)+(0x1L));
            REG_PLAT_UART_CONTROL(bPort) |= (UART_RX_ENABLE);
        }
        else if (bPath == VFD_PATH) //GPA_11
        {
            // default configuration for CT909G EV board
            // UART1_RX: using GPA[11], so set system pin-use0[9]=1, [25]=0
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<9);
            REG_PLAT_SYS_PIN_USE0 &= ~(0x1L<<25);
            REG_PLAT_UART_CONTROL(bPort) |= (UART_RX_ENABLE);    
        }
    }  
    
    else if(dwPort == HAL_UART2_TX)
    {
        //for Debug Mode, __dwDebugMode must be set first. [16 ~ 19] be used indicate UART2_TX.
        //bPath = (__dwDebugMode & 0xf0000) >> 16; 
        if(bPath == IO_MUTE_PATH)
        {
            // default configuration for CT909G EV board
            // UART1_RX: using GPA[4], so set system pin-use0[1]=1, [10, 17, 26]=0
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<1);
            REG_PLAT_SYS_PIN_USE0 &= ~((0x1L<<26)+(0x1L<<17)+(0x1L<<10));
            REG_PLAT_UART_CONTROL(bPort) |= (UART_TX_ENABLE);   
        }    
    }
    else if(dwPort == HAL_UART2_RX)
    {
         //for Debug Mode, __dwDebugMode must be set first. [20 ~ 23] be used indicate UART2_RX.
        //bPath = (__dwDebugMode & 0xf00000) >> 20;
        if(bPath == SCART_PATH)
        {
            // default configuration for CT909G EV board
            // UART1_RX: using GPA[4], so set system pin-use0[0]=1, [8, 16, 24]=0
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<1);
            REG_PLAT_SYS_PIN_USE0 &= ~((0x1L<<24)+(0x1L<<16)+(0x1L<<8));
            REG_PLAT_UART_CONTROL(bPort) |= (UART_RX_ENABLE); 
        }
    }
#endif  // #ifdef CT909G_IC_SYSTEM


    // LLY2.11, disable DSU1 if enable UART1 only for DSU1/ UART1 shared case (128pin, ex 909R)
#ifdef  DSU1_UART1_SAME_PORT
    dwPort &= (0x0000ff00); //don't care about TX, RX port.
    if(dwPort == HAL_UART1)
    {
    	 //for Debug Mode, __dwDebugMode must be set first. [20 ~ 23] be used indicate UART2_RX.
        //bPath = (__dwDebugMode & 0x00ff00) >> 8;
        if(bPath != NOT_USED)
        {
	        // disable bit[27]: DSU1 pin enable
	        REG_PLAT_MPLL_CONTROL &= ~(1<<27);

	        // enable bit[25]: UART1 use GPA[3:2]
	        REG_PLAT_MPLL_CONTROL |= (1<<25);
       }
    }
    REG_PLAT_UART_CONTROL(bPort) = (UART_RX_ENABLE|UART_TX_ENABLE);
#endif  // #ifdef DSU1_UART1_SAME_PORT

    // LLY2.50, add for CT909P to config UART multi-function selection
#ifdef  CT909P_IC_SYSTEM    
    if(dwPort == HAL_UART1_TX)
    {       
        //for Debug Mode, __dwDebugMode must be set first. [8~11] be used indicate UART1_TX.
        //bPath = (__dwDebugMode & 0xf00) >> 8;        
        
        if (bPath == NIM_PATH)  
        {
            //USE GPG[1] for TX
            //Must clear GPGMux[7] as 0, and set GPGMux[6]=1 for UART1 TX
            REG_PLAT_GPG_MUX &= ~(0xf0L);
            REG_PLAT_GPG_MUX |= (1L<<6);
            //need disable other UART1 TX Path
            // [1] from SPI, so clear GPAMux[4]=0
            REG_PLAT_GPA_MUX &= ~(1L<<4);
            // [2] from card reader, so clear GPCMux[12]
            REG_PLAT_GPC_MUX &= ~(1L<<12);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 1
            // For simple method: just set GPAMux[22]=1 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX |= (1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 1);
#endif  // #ifdef USE_GPA21_CTL_74244
            
            REG_PLAT_UART_CONTROL(bPort) |= (UART_TX_ENABLE);
        }

        else if (bPath == SPI_PATH) 
        {
            //USE GPA[1] for TX
            //must clear GPAMux[5] as o and set GPAMux[4] =1 for UART1 TX
            REG_PLAT_GPA_MUX &= ~(0x38L);
            REG_PLAT_GPA_MUX |= (1L<<4);
            
            //need disable other UART1 TX path 
            // [1] from card reader, so clear GPCMux[12]
            REG_PLAT_GPC_MUX &= ~(1L<<12);
            // [2] from NIM, so clear GPG[6]=0
            REG_PLAT_GPG_MUX &= ~(1L<<6);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 1
            // For simple method: just set GPAMux[22]=1 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX |= (1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 1);
#endif  // #ifdef USE_GPA21_CTL_74244
            
            REG_PLAT_UART_CONTROL(bPort) |= (UART_TX_ENABLE);
        }   
        else if (bPath == CARD_READER_PATH) 
        {
            //USE GPC[7] for TX
            //Must clear GPCMux[11], and set GPCMux[12]=1 for UART1 TX
            REG_PLAT_GPC_MUX &= ~(0x3800L); 
            REG_PLAT_GPC_MUX |= (1L<<12); 
            
            //need disable other UART1 TX path
            // [1] from SPI, so clear GPAMux[4]=0 for UART1 TX
            REG_PLAT_GPA_MUX &= ~(1L<<4);
            // [2] from NIM, so clear GPG[6]=0 for UART1 TX
            REG_PLAT_GPG_MUX &= ~(1L<<6);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 0
            // For simple method: just set GPAMux[22]=0 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX &= ~(1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 0);
#endif  // #ifdef USE_GPA21_CTL_74244

            REG_PLAT_UART_CONTROL(bPort) |= (UART_TX_ENABLE);
        }
    }
    else if(dwPort == HAL_UART1_RX)
    {       
        //for Debug Mode, __dwDebugMode must be set first. [12 ~ 15] be used indicate UART1_RX.
        //bPath = (__dwDebugMode & 0xf000) >> 12;        
        if (bPath == NIM_PATH) 
        {
            //USE GPG[2] for RX
            //Must clear GPGMux[0] as o and set GPGMux[2]=1 for UART1 RX
            REG_PLAT_GPG_MUX &= ~(0xfL);
            REG_PLAT_GPG_MUX |= (1L<<2);        
            //need disable other UART1 RX path
            // [1] from SPI so clear GPAMux[1]=0
            REG_PLAT_GPA_MUX &= ~(1L<<1);
            // [2] from card reader, so clear GPCMux[16]
            REG_PLAT_GPC_MUX &= ~(1L<<16);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 1
            // For simple method: just set GPAMux[22]=1 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX |= (1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 1);
#endif  // #ifdef USE_GPA21_CTL_74244
            
            REG_PLAT_UART_CONTROL(bPort) |= (UART_RX_ENABLE);
        }

        else if (bPath == SPI_PATH) 
        {
            //USE GPA[0] for RX
            //must clear GPAMux[0] as o and set GPAMux[1]=1 for UART1 RX
            REG_PLAT_GPA_MUX &= ~(0x07L);
            REG_PLAT_GPA_MUX |= (1L<<1);

            //need disable other UART1 RX path
            // [1] from card reader, so clear GPCMux[16]
            REG_PLAT_GPC_MUX &= ~(1L<<16);
            // [2] from NIM, so clear GPG[2]=0
            REG_PLAT_GPG_MUX &= ~(1L<<2);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 1
            // For simple method: just set GPAMux[22]=1 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX |= (1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 1);
#endif  // #ifdef USE_GPA21_CTL_74244

            REG_PLAT_UART_CONTROL(bPort) |= (UART_RX_ENABLE);
        }

        else if (bPath == CARD_READER_PATH) 
        {
            //USE GPC[8] for RX
            //Must clear GPCMux[17], and set GPCMux[16]=1 for UART1 RX
            REG_PLAT_GPC_MUX &= ~(0x3c000L);
            REG_PLAT_GPC_MUX |= (1L<<16);
            
            //need disable other UART1 RX path
            // [1] from SPI, so clear GPAMux[1]=0 for UART1 RX
            REG_PLAT_GPA_MUX &= ~(1L<<1);
            //[2] from NIM, so clear GPG[2]=0 for UART1 RX
            REG_PLAT_GPG_MUX &= ~(1L<<2);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 0
            // For simple method: just set GPAMux[22]=0 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX &= ~(1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 0);
#endif  // #ifdef USE_GPA21_CTL_74244

            REG_PLAT_UART_CONTROL(bPort) |= (UART_RX_ENABLE);
        }
    } 
    else if (dwPort == HAL_UART2_TX)
    {
        //for Debug Mode, __dwDebugMode must be set first. [16 ~ 19] be used indicate UART2_TX.
        //bPath = (__dwDebugMode & 0xf0000) >> 16; 
       
        if (bPath ==  EXPAND_GPIO_PATH)    
        {
            dwBug=REG_PLAT_GPC_MUX;
            //USE Expand GPIO path for UART2 RX GPC[5]
            //must clear GPCMux[3], and set GPCMux[5]=1
            REG_PLAT_GPC_MUX &= ~(0x38L);
            REG_PLAT_GPC_MUX |= (1L<<5);

            // need disable other UART2 TX path
            // [1] from SPI -- GPA[3], so clear GPAMux[11]=0
            REG_PLAT_GPA_MUX &= ~(1L<<11);
            // [2] from NIM -- GPG[3], so clear GPGMux[15]=0
            REG_PLAT_GPG_MUX &= ~(1L<<15);

            REG_PLAT_UART_CONTROL(bPort) |= (UART_TX_ENABLE);
        }

        else if (bPath ==  NIM_PATH)
        {
            //USE GPG[3] for UART2 TX from NIM path
            //must clear GPGMux[15], and set GPGMux[15]=1
            REG_PLAT_GPG_MUX &= ~(0xf000L);
            REG_PLAT_GPG_MUX |= (1L<<15);

            //Need disable other UART2 TX path
            // [1] from SPI -- GPA[3], so clear GPAMux[11]=0
            REG_PLAT_GPA_MUX &= ~(1L<<11);
            // [2] from expand GPIO -- GPC[4], so clear GPCMux[5]=0
            REG_PLAT_GPA_MUX &= ~(1L<<5);

            REG_PLAT_UART_CONTROL(bPort) |= (UART_TX_ENABLE);
        }

        else if (bPath ==  SPI_PATH)
        {
            //USE GPA[3] for UART2 TX  from SPI path
            //must clear GPAMux[11], and set GPAMux[11]=1
            REG_PLAT_GPA_MUX &= ~(0x1c0L);
            REG_PLAT_GPA_MUX |= (1L<<11);

            //Need disable other path for UART2 TX
            // [1] from NIM -- GPG[3], so clear GPGMux[15]=0
            REG_PLAT_GPG_MUX &= ~(1L<<15);
            // [2] from Expand GPIO -- GPC[4], so clear GPCMux[5]=0
            REG_PLAT_GPA_MUX &= ~(1L<<5);

            REG_PLAT_UART_CONTROL(bPort) |= (UART_TX_ENABLE);
        }
        
    }
    else if (dwPort == HAL_UART2_RX)
    {
       
        //for Debug Mode, __dwDebugMode must be set first. [20 ~ 23] be used indicate UART2_RX.
        //bPath = (__dwDebugMode & 0xf00000) >> 20;        
        if (bPath ==  EXPAND_GPIO_PATH)     
        {
            
            //USE Expand GPIO path for UART2 RX GPC[5]
            //must clear GPCMux[7], and set GPC[7]=1
            REG_PLAT_GPC_MUX &= ~(0xc0L);
            REG_PLAT_GPC_MUX |= (1L<<7);
            dwBug=REG_PLAT_GPC_MUX;
            //need disable other UART2 RX path
            // [1] from SPI -- GPA[2], so clear GPAMux[8]=0
            REG_PLAT_GPA_MUX &= ~(1L<<8);
            // [2] from NIM -- GPG[2], so clear GPGMux[11]=0
            REG_PLAT_GPG_MUX &= ~(1L<<11);

            REG_PLAT_UART_CONTROL(bPort) |= (UART_RX_ENABLE);
        }
        else if (bPath ==  NIM_PATH)      
        {
            //USE GPG[2] for  UART2 RX from NIM path
            //must clear GPGMux[8],  and set GPGMux[11]=1
            REG_PLAT_GPG_MUX &= ~(0x0f00L);
            REG_PLAT_GPG_MUX |= (1L<<11) ;

            //Need disable other UART2 RX path
            // [1] from SPI -- GPA[2], so clear GPAMux[8]=0
            REG_PLAT_GPA_MUX &= ~(1L<<8);
            // [2] from expand GPIO -- GPC[5], so clear GPCMux[7]=0
            REG_PLAT_GPA_MUX &= ~(1L<<7);      

            REG_PLAT_UART_CONTROL(bPort) |= (UART_RX_ENABLE);
        }

        else if (bPath ==  SPI_PATH)
        {
            //USE GPA[3] for UART2 RX from SPI path
            //must clear GPAMux[6], and set GPAMux[8]=1
            REG_PLAT_GPA_MUX &= ~(0xe00L);
            REG_PLAT_GPA_MUX |= (1L<<8);

            //Need disable other path for UART2 RX
            // [1] from NIM -- GPG[2], so clear GPGMux[11]=0
            REG_PLAT_GPG_MUX &= ~(1L<<11);
            // [2] from Expand GPIO -- GPC[5], so clear GPCMux[7]=0
            REG_PLAT_GPA_MUX &= ~(1L<<7);

            REG_PLAT_UART_CONTROL(bPort) |= (UART_RX_ENABLE);
        }        
    } 
   // REG_PLAT_UART_CONTROL(bPort) = (UART_RX_ENABLE|UART_TX_ENABLE);
#endif  // #ifdef CT909P_IC_SYSTEM
    
    
}

//************************************************
// function :HAL_UART_ErrStatus
// argument:dwport : receive error message from UART1_RX or UART2_RX
// return          : Error Status
//****************************************************
BYTE    HAL_UART_ErrorStatus(DWORD dwPort)
{
    #define     HAL_UART_STATUS_NO_ERR      0
    #define     HAL_UART_STATUS_OVERRUN     1
    #define     HAL_UART_STATUS_PARITY_ERR  2
    #define     HAL_UART_STATUS_FRAME_ERR   4
    BYTE bUartStatus = HAL_UART_STATUS_NO_ERR;
    BYTE bPort;

    //get UART port , ex: UART1 or UART2  
    bPort = GET_UART_PORT(dwPort);
    bPort--;
    
     if (REG_PLAT_UART_STATUS(bPort) & UART_STATUS_OVERRUN)
        bUartStatus |= HAL_UART_STATUS_OVERRUN;

    if (REG_PLAT_UART_STATUS(bPort) & UART_STATUS_PARITY_ERROR)
        bUartStatus |= HAL_UART_STATUS_PARITY_ERR;

    if (REG_PLAT_UART_STATUS(bPort) & UART_STATUS_FRAME_ERROR)
        bUartStatus |= HAL_UART_STATUS_FRAME_ERR;

    // Clear Error Status.
    REG_PLAT_UART_STATUS(bPort) &= ~(UART_STATUS_OVERRUN | UART_STATUS_PARITY_ERROR | UART_STATUS_FRAME_ERROR);

    return bUartStatus;
}

//***************************************************************************
//  Function    :   IO_UART_ReceiveChar
//  Abstract    :   This function will detect if any data in UART data register and get it.
//  Arguments   :   pData : to be received data
//                 dwPort:  receive data from UART1 or UART2 port.
//  Return      :   success-->TRUE
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
BOOL    HAL_UART_ReceiveChar(PBYTE pData, DWORD dwPort)
{
    BYTE bPort;   
    
    //get UART port , ex: UART1 or UART2
    bPort = GET_UART_PORT(dwPort);
    bPort--;
    if (REG_PLAT_UART_STATUS(bPort) & UART_STATUS_DATA_READY)
    {
        *pData = REG_PLAT_UART_DATA(bPort);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//**********************************************************
// function  : HAL_UART_SendChar
// argument : bData : to be sent data
//           dwPort : to Choice UART1 or UART2 port.
// return    : Success->TRUE
//***********************************************************
BOOL    HAL_UART_SendChar(BYTE bData, DWORD dwPort)
{
    DWORD dwTimeStart;
    BYTE bPort;
    
    //get UART port , ex: UART1 or UART2
    bPort = GET_UART_PORT(dwPort);
    bPort--;  
    dwTimeStart=OS_GetSysTimer();
    while( (OS_GetSysTimer() - dwTimeStart) < COUNT_50_MSEC)
    {
        if (REG_PLAT_UART_STATUS(bPort) & UART_STATUS_TH_EMPTY)
        {
            REG_PLAT_UART_DATA(bPort) = bData;
            return TRUE;
        }
    }

    return FALSE;
}


// LLY2.15, define DSU pin control bits
#define DSU1_PIN_ENABLE                 (1L<<27)    // REG_PLAT_MPLL_CONTROL bit[27]
#define DSU2_PIN_ENABLE                 (1L<<26)    // REG_PLAT_MPLL_CONTROL bit[26]

// LLY2.15 create
// SYuan2.77-909P: suuport to set DSU TX , UART RX indendently
//  *****************************************************************************************
//  Function    :   HAL_DSU_Pin_Enable
//  Description :   Enable desired DSU port pin
//  Arguments   :   dwPort, specify the desired DSU port
//				    bPath, specify teh desired DSU TX, RX Path. ex: NIM_pATH ..etc
//  Return      :   None
//  *****************************************************************************************
VOID    HAL_DSU_Pin_Enable(DWORD dwPort, BYTE bPath)
{       
#ifdef  CT909G_IC_SYSTEM
      if(dwPort == HAL_DSU1_TX)
    {
        //for Debug Mode, __dwDebugMode must be set first. [0 ~ 3] be used indicate DSU1_TX.
        //bPath = (__dwDebugMode & 0xf); 
        if (bPath == VFD_PATH) //GPA_9
        {
            // default configuration for CT909G EV board
            // DSU1_TX: using GPA[9], so set system pin-use0[27]=1, [11]=0        
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<27);
            REG_PLAT_SYS_PIN_USE0 &= ~ (0x1L<<11);
        }
        else if ( bPath == IO_MUTE_PATH) //GPA_4
        {
            // default configuration for CT909G EV board
            // DSU1_TX: using GPA[4], so set system pin-use0[26]=1, [17]=0, [10]=0        
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<26);
            REG_PLAT_SYS_PIN_USE0 &= ~((0x1L<<17)+ (0x1L<<10));
        }
    }
    else if(dwPort == HAL_DSU1_RX)
    {
        //for Debug Mode, __dwDebugMode must be set first. [4 ~ 7] be used indicate DSU1_RX.
        //bPath = (__dwDebugMode & 0xf0) >> 4; 
        if(bPath == VFD_PATH) //GPA_11
        {
            // default configuration for CT909G EV board      
            // DSU1_RX: using GPA[11], so set system pin-use0[25]=1, [9]=0
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<25);
            REG_PLAT_SYS_PIN_USE0 &= ~(0x1L<<9);    
        }
        else if (bPath == SCART_PATH) //GPA_6
        {
            // default configuration for CT909G EV board      
            // DSU1_RX: using GPA[6], so set system pin-use0[24]=1, [16]=0, [8]=0, [0]=0
            REG_PLAT_SYS_PIN_USE0 |= (0x1L<<24);
            REG_PLAT_SYS_PIN_USE0 &= ~((0x1L<<16)+(0x1L<<8)+(0x1L));
        }
    }    
#endif  // #ifdef CT909G_IC_SYSTEM    

#ifndef CT909G_IC_SYSTEM
#ifdef  DSU1_UART1_SAME_PORT
	//get DSU port , ex: DSU1 or DSU2
    dwPort = GET_DSU_PORT(dwPort);
    if(dwPort == HAL_DSU1)
    {
    	 //for Debug Mode, __dwDebugMode must be set first. [20 ~ 23] be used indicate UART2_RX.
        //bPath = (__dwDebugMode & 0x0000ff) >> 8;
        if(bPath != NOT_USED)
       	{
	        // Must restore UART1 to GPA[9:8] first while enable DSU1
	        // Otherwise, the communication may wrong
	        // disable bit[25] to let UART1 use GPA[9:8]
	        REG_PLAT_MPLL_CONTROL &= ~(1<<25);
        }
    }
#endif  // #ifdef  DSU1_UART1_SAME_PORT

    if((dwPort == HAL_DSU1_TX) || (dwPort == HAL_DSU1_RX))
    {
        // enable REG_PLAT_MPLL_CONTROL bit[27]
        REG_PLAT_MPLL_CONTROL |= (DSU1_PIN_ENABLE);
    }
#endif  // #ifndef CT909G_IC_SYSTEM


        // LLY2.50, add for CT909P to config DSU1 multi-function selection
#ifdef  CT909P_IC_SYSTEM
    if(dwPort == HAL_DSU1_TX)
    {        
        //for Debug Mode, __dwDebugMode must be set first. [0 ~ 3] be used indicate DSU1_TX.
        //bPath = (__dwDebugMode & 0xf); 
        
        if (bPath == NIM_PATH)
        {
            //Must clear GPGMux[15], and set GPGMux[14]=1 for TX
            REG_PLAT_GPG_MUX &= ~(0xf000L);    
            REG_PLAT_GPG_MUX |= (1L<<14);

            //need disable other DUS1 TX path
            //[1]from SPI TX, so disable GPAMux[10]=0
            REG_PLAT_GPA_MUX &= ~(1L<<10);
            //[2]from card reader, so disable GPCMuX[9]=0
            REG_PLAT_GPC_MUX &= ~(1L<<9);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 1
            // For simple method: just set GPAMux[22]=1 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX |= (1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 1);
#endif  // #ifdef USE_GPA21_CTL_74244
        }  

        if (bPath == SPI_PATH) 
        {
            //Must clear GPAMux[11] , and set GPAMux[10]=1 for TX
            REG_PLAT_GPA_MUX &= ~(0xc00L);
            REG_PLAT_GPA_MUX |= (1L<<10);
            
            //Need disable other DUS1 TX path
            //[1] from card reader, so disable GPCMux[9]=0
            REG_PLAT_GPC_MUX &= ~(1L<<9);
            //[2] from NIM, so disable GPCMux[14]=0
            REG_PLAT_GPG_MUX &= ~(1L<<14);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 1
            // For simple method: just set GPAMux[22]=1 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX |= (1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 1);
#endif  // #ifdef USE_GPA21_CTL_74244

        }

        if (bPath == CARD_READER_PATH) 
        {
            //Must clear GPCMux[10:8] , and set GPCMux[9]=1 for TX
            REG_PLAT_GPC_MUX &= ~(0x700L);
            REG_PLAT_GPC_MUX |= (1L<<9);

            //need disable other DUS1 TX path
            //[1]from SPI TX -- GPA[3], so disable GPAMux[10]=0
            REG_PLAT_GPA_MUX &= ~(1L<<10);
            //[2]from NIM TX -- GPG[3] so disable GPGMux[14]=0
            REG_PLAT_GPG_MUX &= ~(1L<<14);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 0
            // For simple method: just set GPAMux[22]=0 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX &= ~(1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 0);
#endif  // #ifdef USE_GPA21_CTL_74244
        }
    }
    else if (dwPort == HAL_DSU1_RX)
    {
        
        //for Debug Mode, __dwDebugMode must be set first. [4 ~ 7] be used indicate DSU1_RX.
        //bPath = (__dwDebugMode & 0xf0) >> 4; 
        
        if (bPath == NIM_PATH) 
        {
            //Must clear GPGMux[8] , and set GPGMux[10]=1 for RX
            REG_PLAT_GPG_MUX &= ~(0x0f00L);
            REG_PLAT_GPG_MUX |= (1L<<10);

            //need disable other DUS1 path
            //[1]from SPI RX, so disable GPAMux[7]=0
            REG_PLAT_GPA_MUX &= ~(1L<<7);
            //[2]from card reader, so disable GPCMux[15]=0
            REG_PLAT_GPC_MUX &= ~(1L<<15);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 1
            // For simple method: just set GPAMux[22]=1 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX |= (1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 1);
#endif  // #ifdef USE_GPA21_CTL_74244
        }

        if (bPath == SPI_PATH) 
        {
            //must clean GPAMux[6] , and set GPAMux[7]=1 for RX
            REG_PLAT_GPA_MUX &= ~(0x1c0L);
            REG_PLAT_GPA_MUX |= (1L<<7);

            //Need disable other DUS1 RX path
            //[1] from card reader, so disable GPCMux[15]
            REG_PLAT_GPC_MUX &= ~(1L<<15);
            //[2] from NIM, so disable GPGMux[10]=0
            REG_PLAT_GPG_MUX &= ~(1L<<10);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 1
            // For simple method: just set GPAMux[22]=1 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX |= (1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 1);
#endif  // #ifdef USE_GPA21_CTL_74244
        }

        if (bPath == CARD_READER_PATH)
        {
            //Must clear GPCMux[17:14], and set GPCMux[15]=1
            REG_PLAT_GPC_MUX &= ~(0x3c000L);
            REG_PLAT_GPC_MUX |= (1L<<15);
            //need disable other DUS1 RX path
            //[1] from SPI RX -- GPA[2], so disable GPAMux[7]=0
            REG_PLAT_GPA_MUX &= ~(1L<<7);
            //[1] from NIM RX -- GPG[2], so disable GPGMux[10]=0
            REG_PLAT_GPG_MUX &= ~(1L<<10);
            // LLY2.75, if enable define "USE_GPA21_CTL_74244" then do following control.
            // Now, only CT909P_P3 and CT956A system board support it.
#ifdef  USE_GPA21_CTL_74244
            // LLY2.51, porting Chuan's code to enable GPA[21] as GPO and output 0
            // For simple method: just set GPAMux[22]=0 to reach this goal
            // Notice: must check if it's ok by the correct method.
            //REG_PLAT_GPA_MUX &= ~(1L << 22);
            HAL_WriteGPIO(GPIO_A, 21, 0);
#endif  // #ifdef USE_GPA21_CTL_74244
        }
    }
    else if(dwPort == HAL_DSU2)
    {
        // enable REG_PLAT_MPLL_CONTROL bit[26]
        REG_PLAT_MPLL_CONTROL |= (DSU2_PIN_ENABLE);
    }
#endif  // #ifdef CT909P_IC_SYSTEM
    
}


//  *****************************************************************************************
//  Function    :   HAL_DSU_Pin_Disable
//  Description :   Disable desired DSU port pin
//  Arguments   :   bPort, specify the desired DSU port
//  Return      :   None
//  *****************************************************************************************
VOID    HAL_DSU_Pin_Disable(DWORD dwPort)
{
    //get DSU port , ex: DSU1 or DSU2
    dwPort = GET_DSU_PORT(dwPort);
#ifndef CT909G_IC_SYSTEM    
    if((dwPort == HAL_DSU1))
    {
        // disable REG_PLAT_MPLL_CONTROL bit[27]        
        REG_PLAT_MPLL_CONTROL &= ~(DSU1_PIN_ENABLE);
    }
    else if(dwPort == HAL_DSU2)
    {
        // disable REG_PLAT_MPLL_CONTROL bit[26]
        REG_PLAT_MPLL_CONTROL &= ~(DSU2_PIN_ENABLE);
    }      
#endif  // #ifdef CT909G_IC_SYSTEM

    // LLY2.60, porting the necessary code for CT909P IC
    // DSU1 are shared the same pin w/ other function,
    // So, if disable DSU function, must release it to others.
#ifdef  CT909P_IC_SYSTEM
    if(dwPort == HAL_DSU1)
    {
        // Disable all possible path for DSU1
        // [1] from SPI -- GPA[2, 3], so disable GPAMux[7, 10]=0
        REG_PLAT_GPA_MUX &= ~((1L<<7) + (1L<<10));
        // [2] from card reader -- GPC[6, 8], so disable GPCMux[9, 15]=0
        REG_PLAT_GPC_MUX &= ~((1L<<9) + (1L<<15));
        // [3] from NIM -- GPG[2, 3], so disable GPGMux[10, 14]=0
        REG_PLAT_GPG_MUX &= ~((1L<<10) + (1L<<14));
    }
#endif  // #ifdef CT909P_IC_SYSTEM

    // LLY2.75, disable DSU pin control for CT909G    
#ifdef  CT909G_IC_SYSTEM
    if(dwPort == HAL_DSU1)
    {
        // set bit[27:24] of System Pin-Use Register 0 to disable DSU1
        REG_PLAT_SYS_PIN_USE0 &= ~(0xfL<<24);
    }
    else if(dwPort == HAL_DSU2)
    {
        // set bit[17:16] of System Pin-Use Register 0 to disable DSU2
        REG_PLAT_SYS_PIN_USE0 &= ~(0x3L<<16);
    }  
#endif  // #ifdef CT909G_IC_SYSTEM

}


// LLY2.15 create
//  *****************************************************************************************
//  Function    :   HAL_DSU_BaudRateSet
//  Description :   Set DSU baud rate value
//  Argument    :   dwSysCLK, current system clock
//                  dwBaudRate, desired baudrate value
//                  dwPort, desired DSU port
//  *****************************************************************************************
VOID    HAL_DSU_BaudRateSet(DWORD dwSysCLK, DWORD dwBaudRate, DWORD dwPort)
{
    ////get DSU port , ex: DSU1 or DSU2
    dwPort =GET_DSU_PORT(dwPort);
    if(dwPort == HAL_DSU1)
    {
        REG_PLAT_DSU_UART_SCALER = (((dwSysCLK*10)/(dwBaudRate*8))-5)/10;
    }
    else if(dwPort == HAL_DSU2)
    {
      REG_PLAT_PROC2_DSU_UART_SCALER = (((dwSysCLK*10)/(dwBaudRate*8))-5)/10;
    }
}



// LLY2.16 create, porting Aron's code to get NIM module I2C configure pin
#ifdef SUPPORT_STB
//  *****************************************************************************************
//  Function    :   HAL_NIM_GetI2CConfig
//  Description :   Get the NIM IIC SCLK DATA IO pin configuration
//  Argument    :   pI2C_Config, store the configuration in the struct pointer
//  *****************************************************************************************
VOID HAL_NIM_GetI2CConfig (PI2C_CONFIG pI2C_Config)
{
    pI2C_Config->bIOGroup =   NIM_I2C_GRP;
    pI2C_Config->bSDAT_Pin = NIM_I2C_DATPIN;
    pI2C_Config->bSCLK_Pin = NIM_I2C_SCLKPIN;
}
#endif // #ifdef SUPPORT_STB


// LLY2.18 create
//  *****************************************************************************************
//  Function    :   HAL_I2CSrc_Ctrl
//  Description :   Init/ Lock/ Unlock I2C resource by mutex
//  Arguments   :   ACTION_LOCK, lock the mutek
//                  ACTION_UNLOCK, unlock the mutek
//  Return      :   TRUE, the mutex has been locked
//                  FALSE, the mutex hst not been locked
//  Notice      :   Since, I2C control pin maybe shared w/ Expand GPIO pin, VFD Control pin
//  *****************************************************************************************
BOOL HAL_I2CSrc_Ctrl(BYTE bAction)
{
    switch(bAction)
    {
    case    MUTEX_ACTION_INIT:
        OS_InitMutex(&__mutexI2CReady);
        return TRUE;

    case    MUTEX_ACTION_LOCK:
        return (OS_LockMutex(&__mutexI2CReady));

    case    MUTEX_ACTION_UNLOCK:
        OS_UnlockMutex(&__mutexI2CReady);
        return TRUE;

    case    MUTEX_ACTION_TRYLOCK:
        return (OS_TryLockMutex(&__mutexI2CReady));

    default:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Not support I2C resouce control action\n");
        return FALSE;
    }
}
