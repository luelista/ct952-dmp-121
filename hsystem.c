#include "winav.h"
#include "ctkav.h"
#include "ctkav_mcu.h"
#include "hal.h"
#include "hdecoder.h"
#include "disp.h"
#include "hio.h"
#include "hsys.h"
#include "hdevice.h"
#include "romld.h"
// LLY2.36, include "spflash.h" to adjust SPI Flash clock
#include "spflash.h"
// LLY2.36, include "input.h" to change IR checking mode
#include "input.h"

#ifdef EMU_RTC
#include "rtcdrv.h"
#endif
#ifdef SUPPORT_USB
#include "srcfilter.h"
#include "usbwrap.h"
#endif //
extern void    INT_Init( void );

// LLY2.15, add varible to keep system clock
DWORD   __dwSysClk;
WIN_PARAMETER   MainVideoInfo;  // declare here, as gdi will use it.


void    HAL_StartSystem(void)
{
    // LLY2.22, give CPU_SPEED directly for MPLL setting
    HAL_ClockSet(MODE_MPLL, CPU_SPEED);

    // LLY2.15, give default value for system clock
    // And, prescaler reload value will reference it.
    __dwSysClk = CPU_SPEED;

    // LLY2.13, base on desired CPU working speed to program prescaler reload value
    // prescaler_reload = (cpu_speed-1)/2
    // Notice: only necessary to give the value of MHz part
    REG_PLAT_PRESCALER_RELOAD = ((__dwSysClk/1000000)-1)/2;

    // LLY2.21, porting Chuan's code to calculate time1 reload value base on system tick
    REG_PLAT_TIMER1_RELOAD = (1000 * SYSTEM_TICK) - 1;

    // LLY1.05, always enable cache power saving feature
    REG_PLAT_CACHE_CONTROL |= CACHE_DCACHE_POWER_SAVING;

    // J500CSC_111, clear the semaphore at first start
    MACRO_PLAT_KEY_UNLOCK( );

    // LLY2.16, remove the procedure from HAL_StartSystem(),
    // Notice: it must be initialize as earily as possible since servo init may use expand GPIO
    HAL_Init_ExpGPIO();

    // LLY2.61, initial necessary register to control SCART PWM
#ifdef  SUPPORT_SCART_IF
#ifdef  SUPPORT_SCART_PWM_CTRL
    HAL_SCART_PWM_Init(PIN_SCART_PWM);
#endif  // #ifdef SUPPORT_SCART_PWM_CTRL
#endif  // #ifdef SUPPORT_SCART_IF

    // LLY2.60, only do DVD/DVB pin switching while solution need this pin to switch mode
    // ex. it's un-necessary and no pin to do it @ DMP952A solution.
    // Notice: can't use SUPPORT_SERVO_SOURCE && SUPPORT_STB for checking
    //         since CT909R system only servo source, not NIM solution
    //         but it still need do this pin switching.
    // LLY2.38, remove DVD/ DVB switching procedure from HAL_InitPlatForm()
    // Since, it must be done as earily as possible than servo init.
    // Otherwise, servo action can't work.
    // LLY2.15, set DVD/ DVB switch mode as DVD while power-on
    // Otherwise, servo signal can't come-in
#ifndef  NO_DVD_DVB_SWITCHING
    // Notice: must check why the action will fail @ CT909S EVAL board.
#if (DECODER_SYSTEM != DVD909S_EVAL)
    HAL_WriteGPIO(DVD_DVB_SWITCH_GRP, PIN_DVD_DVB_SWITCH, 1);
#endif  // #if (DECODER_SYSTEM != DVD909S_EVAL)
#endif  // #ifdef  DO_DVD_DVB_SWITCHING

}


//  *********************************************************************
//  Function    :   INITIAL_uProcessor
//  Description :   The initalization of uProcessor; Set IRQ and Timer
//  Arguments   :   NONE
//  Return      :   NONE
//  Side Effect :   After the function, will enable Interrupt...
//  *********************************************************************
BYTE HAL_InitPlatForm(void)
{
#ifdef  SYSTEM_ECOS
    DWORD   dwAck=0;
    DWORD   dwSaveInt;
    BYTE    bIndx;
    DWORD   dwTimeStart;
    DWORD   dwREG_PLAT_TIMER1_RELOAD;

// LLY2.17, porting Chuan's code to remove un-necessary code if not AP_FRAMEWORK
#ifndef AP_FRAMEWORK

    // Chuan1.20,  Disable PROC2 debug information.
    // Serial debug tool wll resync again later
    REG_DBG_PROC2FLAG = 0;

    //kevin0.95, clear shared memory only when H/W power on
    for ( bIndx=0; bIndx<32; bIndx++)
        (*(volatile DWORD *)(0x80000780+bIndx*4)) = 0;

    // J500CSC_2.05, when current is boot from small boot code, enable or disable
    // audio code protection depends on RUNTIME_UNZIP_AUDIO_CODE is defined or not.
    if( PLAT_CODE_PROTECTION == SPARC_NOP_INSTRUCTION )
    {
#ifdef  RUNTIME_UNZIP_AUDIO_CODE
        PLAT_CODE_PROTECTION |= PLAT_CODE_PROTECT_PROC2;
#else
        PLAT_CODE_PROTECTION &= (~PLAT_CODE_PROTECT_PROC2);
#endif  // RUNTIME_UNZIP_AUDIO_CODE
    }

    // J500CSC_108, arrange the memory protection mechanism
    // Added following setting for preserve the compliance with previous version
    if( PLAT_CODE_PROTECTION & PLAT_CODE_PROTECT_PROC2 )
    {
        (*(volatile DWORD *)0x80000784) |= 0x80000000;
    }
    else
    {
        (*(volatile DWORD *)0x80000784) &= (~0x80000000);
    }

    // Chuan0.64, Set The start address of PROC2 and SP
    // After release PROC2, the boot code will look the two address and do the setting.
    // If the address isn't 0x40xxxxxx, boot code will be trapped.
    PROC2_SP = DS_PROC2_SP;
    PROC2_STARTADR = DS_PROC2_STARTADDR;

#ifndef NO_PROC2
// J500CSC_2.05, for supporting runtime decompressed audio code
    HAL_LoadAudioCode( HAL_AUDIO_AC3, (PDWORD) DS_AUDIO_TABLE_ST, (PBYTE) DS_PROC2_STARTADDR,
            (PBYTE) DS_UNZIP_BUF_ST );

#endif  //#ifndef NO_PROC2

    // LLY2.21, Porting Chuan's code to keep time1 value first
    // Since, it will be updated by PROC2
    dwREG_PLAT_TIMER1_RELOAD = REG_PLAT_TIMER1_RELOAD;

    // LLY1.06, it's un-necessary to set PROC2 SP & STARTADDR since nobody reference it
    // ie. After standard library (eCos package v1.1.1)
    //(*(volatile DWORD *) (0x403ffff8)) = DS_PROC2_SP;
    //(*(volatile DWORD *) (0x403ffffc)) = DS_PROC2_STARTADDR;

    // Chuan0.64, Due Flash and ATAPI use the same control pin, the initialization of ATAPI need
    // wait PROC2 located at DRAM(0x40xxxxxx).
    // Therefore, we need wait the ack of HAL_AM_AUDIO_START.
    // Lack the timeout flow?
    // Notice: The clock doesn't attach yet! Use real timer for timeout control or attach first.
    (*(volatile DWORD *)(0x800007e4)) = 0x10003;

    // LLY0.65, porting Chuan code to support DSU 2 debug
#ifdef DEBUG_PROC2
    (*(volatile DWORD *)(DS_PROC2_STARTADDR)) = 0x91d02000;     // ta 0
    (*(volatile DWORD *)(DS_PROC2_STARTADDR+4)) = 0x01000000;   // nop
    (*(volatile DWORD *)(DS_PROC2_STARTADDR+8)) = 0x01000000;   // nop
#endif  // #ifdef DEBUG_PROC2

    // LLY2.15, remove following procedure, since current use API to replace it.
#if 0
    // LLY0.80a, add return code within the last address of our code
    // Just for tester setting breakpointer easiler
#ifdef  SUPPORT_PSNR_TEST
    (*(volatile DWORD *)(0x407FFFF0)) = 0x81c3e008;     // retl
    (*(volatile DWORD *)(0x407FFFF0+4)) = 0x01000000;   // nop
    (*(volatile DWORD *)(0x407FFFF0+8)) = 0x01000000;   // nop
#endif  // #ifdef SUPPORT_PSNR_TEST
#endif  // #if 0

    // LLY1.21-1, set a default address for area 1 code protection. (Used for Proc2)
    // Since, PROC1 will enable area 1 protection early than PROC2 set the desired address.
    MACRO_MCU_WRITE_PROTECTION( 1, 0x40000000, 0x40000000);

    // J500CSC_108, arrange the memory protection mechanism
    // WP0 -> 0x40000008 ~ 0x400AFFF8: .text & .rodata (Proc1 & Video Decoder)
    // WP1 -> 0x400B0000 ~ 0x4011FFF8: Audio Decoder, but just .text & rodata, this is protected
    //                                 by PROC2
    // WP2 -> 0x40800000 ~ 0x4FFFFFF8: out of DRAM size.
    // WP3 -> 0x407E0000 ~ 0x40800000: DS_OSD_FONTTABLE_ST ~ DS_OSD_FONTTABLE_END
    // FONT_TABLE will be protected always.
    // .text & .rodata of PROC1 and PROC2 protected or not depends on the settings at
    // PLAT_CODE_PROTECTION.
    // disable all memory protection
    REG_MCU_WPROTEN = 0;

    // set correct memory ranges for code protection.
    // LLY2.17, porting Chuan's code since re-arrange the memory layout
    // [1] Always protect trap table.
    MACRO_MCU_WRITE_PROTECTION(0, 0x40000000, 0x40001000);

    // [2] Protect PROC1 text, rodata section.
    // MACRO_MCU_WRITE_PROTECTION( 2, DS_PROC1_TEXTADDR_ST, DS_PROC1_TEXTADDR_END );
    {
        //Aron2.36, Get the code protection by __dwCoPoStartAddr, __dwCoPoEndAddr
        //Both two are resided in linker script file
        extern DWORD __dwCoPoStartAddr, __dwCoPoEndAddr;//READ ONLY
        DWORD dwStartAddr, dwEndAddr;
        dwStartAddr = (DWORD)(&__dwCoPoStartAddr);
        dwEndAddr = (DWORD)(&__dwCoPoEndAddr);
        if ((dwStartAddr & 0x40000000) && (dwEndAddr & 0x40000000))
        {
            MACRO_MCU_WRITE_PROTECTION( 2, dwStartAddr, dwEndAddr );
        }
     }

    // [3] LLY1.20, base on DRAM size to do out-of range protection
    // Ref. Design Target Spec-platform.pdf: system configure register 1
    // Notice: 909 supprt 32M ~ 512M
    //         909S support 16M ~ 256M
    // LLY2.56, porting Chuan's code since code protection mechanism is different from CT909G
#ifdef CT909R_IC_SYSTEM
    MACRO_MCU_WRITE_PROTECTION( 4, HAL_GetDRAMSize(), 0x50000000 );
#endif  // #ifdef CT909R_IC_SYSTEM

    // [4] Protect OSD font table area.
    // LLY2.20, porting Alan's code not to protect the font table if necessary
#ifndef UNPROTECT_FONT_TABLE
    MACRO_MCU_WRITE_PROTECTION( 3, DS_OSD_FONTTABLE_ST, DS_OSD_FONTTABLE_END ); //Alan1.00 adds for protecting font table
#endif  // #ifndef UNPROTECT_FONT_TABLE

    // check how many ranges need to be protected
    dwSaveInt = MCU_WRITE_PROTECT_AREA0_ENABLE | MCU_WRITE_PROTECT_AREA3_ENABLE | MCU_WRITE_PROTECT_AREA4_ENABLE;

    // Base on PLAT_CODE_PROTECTION value to check if protect PROC1
    if( PLAT_CODE_PROTECTION & PLAT_CODE_PROTECT_PROC1 )
    {
        dwSaveInt |= MCU_WRITE_PROTECT_AREA2_ENABLE;
    }

    // enable memory protection
    MACRO_MCU_ENABLE_PROTECTION( dwSaveInt );


    OS_REORDER_BARRIER();
    // LLY0.80, add protection while accessing REG_PLAT_RESET_CONTROL
    // Since, it maybe access by another thread or processor
    OS_DISABLE_INTERRUPTS( dwSaveInt );	// Disable INT to avoid different threads accessing

    // LLY2.01, porting Chuan code for NO_PROC2 code
#ifdef NO_PROC2
    REG_PLAT_RESET_CONTROL_ENABLE = PLAT_RESET_PROC2_ENABLE;
    (*(volatile DWORD *)(0x800007e4)) = 0x0;
#else   // #ifdef NO_PROC2
    REG_PLAT_RESET_CONTROL_DISABLE = PLAT_RESET_PROC2_DISABLE;
#endif  // #ifdef NO_PROC2
    OS_RESTORE_INTERRUPTS( dwSaveInt );	// Restore INT.
    OS_REORDER_BARRIER();

// Micky_PORTING_OSWRAP
// Move to INITIAL_System
/*
    // Set Timer earily since next statement will use counter
    __RTC = cyg_real_time_clock();
    cyg_clock_to_counter(__RTC, &__ClockHandle);
*/
    dwTimeStart=OS_GetSysTimer();

#ifdef DEBUG_PROC2
    while(1)
#else  // #ifdef DEBUG_PROC2
    while( (OS_GetSysTimer() - dwTimeStart) < PROC2_RESET_ACK_TIME_OUT)
#endif // #ifdef DEBUG_PROC2
    {
        dwAck = (*(volatile DWORD *)(0x800007e4));
        dwAck >>= 16;

        // Process 2 booting ok
        if(dwAck==0)
        {
            break;
        }
    }

    // LLY2.21, Porting Chuan's code to restore time1 value first
    // Since, it will be updated by PROC2
    REG_PLAT_TIMER1_RELOAD = dwREG_PLAT_TIMER1_RELOAD;

    // LLY1.21a, enable area 1 protection for PROC2
    // Notice: this procedure must be done after PROC2 booting ok
    if( PLAT_CODE_PROTECTION & PLAT_CODE_PROTECT_PROC2 )
    {
        // protect any wrong settings by PROC2
        if ((REG_MCU_WP1BASE >= (DS_PROC2_STARTADDR&0x7FFFFFF)) && (REG_MCU_WP1BASE <= (DS_PROC2_SP&0x7FFFFFF)) &&
            (REG_MCU_WP1SIZE >= (DS_PROC2_STARTADDR&0x7FFFFFF)) && (REG_MCU_WP1SIZE <= (DS_PROC2_SP&0x7FFFFFF)) &&
            (REG_MCU_WP1SIZE > REG_MCU_WP1BASE))
        {
            MACRO_MCU_ENABLE_PROTECTION( MCU_WRITE_PROTECT_AREA1_ENABLE );
        }
    }

#endif // #ifndef AP_FRAMEWORK

    REG_PLAT_PROC1_1ST_INT_MASK_ENABLE = INT_SET_ALL;
    REG_PLAT_PROC1_2ND_INT_MASK_ENABLE = INT_SET_ALL;

    REG_PLAT_PROC1_1ST_INT_CLEAR = INT_SET_ALL;
    REG_PLAT_PROC1_2ND_INT_CLEAR = INT_SET_ALL;

    // Chuan0.64, Don't initial the INT of PROC2. Let PROC2 do it.
    /*
    REG_PLAT_PROC2_1ST_INT_MASK_PRIORITY = INT_CLEAR_ALL;
    REG_PLAT_PROC2_1ST_INT_CLEAR = INT_SET_ALL;
    REG_PLAT_PROC2_INT_MASK_PRIORITY = INT_CLEAR_ALL;
    REG_PLAT_PROC2_INT_CLEAR = INT_SET_ALL;
    */

    REG_PLAT_INT_MASK_PRIORITY = (INT_CLEAR_ALL|INT_TIMER1);
    REG_PLAT_INT_CLEAR = (INT_SET_ALL&(~(INT_TIMER1)));

    // Clear STC and Enable.
    // LLY1.05, using API to set STC value
    //REG_PLAT_TIMER3_VALUE = 0;
    DISP_UpdateSTC(0, FALSE);
    REG_PLAT_TIMER3_CONTROL = 2;

#ifndef AP_FRAMEWORK

    // initialize INT for BIU
    REG_BIU_BIUCTL |= BIU_INT_BPINT_EN;
    REG_PLAT_PROC1_2ND_INT_MASK_DISABLE = INT_PROC1_2ND_BIU;

    // initialize INT for MCU_BSRD
    REG_PLAT_PROC1_2ND_INT_MASK_DISABLE = INT_PROC1_2ND_MCU_BSRD;

    REG_MCU_REMEN = (REG_MCU_REMEN & MCU_BIT_STREAM_TYPE_UNMASK) | MCU_REMAINDER_ENABLE_VIDEO_BUF | MCU_REMAINDER_ENABLE_AUDIO_BUF0 | MCU_REMAINDER_ENABLE_SP1_BUF
            | MCU_REMAINDER_ENABLE_SP2_BUF;
    REG_MCU_VREM = MCU_REMAINDER_INIT | 0x00;
    REG_MCU_A0REM = MCU_REMAINDER_INIT | 0x00;
    REG_MCU_SP1REM = MCU_REMAINDER_INIT | 0x00;
    REG_MCU_SP2REM = MCU_REMAINDER_INIT | 0x00;

    REG_PLAT_PROC1_2ND_INT_MASK_DISABLE = (INT_PROC1_2ND_VBUF_UNDERFLOW | INT_PROC1_2ND_VBUF_OVERFLOW);

    // Chuan0.85, Give the overflow threshold.
    REG_MCU_VOTH = MCU_VOTH_VALUE / 4;
    REG_MCU_VUTH = MCU_VUTH_VALUE / 4;

    // LLY1.21-1, it's un-necessary to enable PROC1_2ND_WR_PROTECTION
    // Since, nobody to write the interrupte service routine.
    //REG_PLAT_PROC1_2ND_INT_MASK_PRIORITY |= INT_PROC1_2ND_MCU_WR_PROTECTION;

// Brian0.67
#ifdef  SUPPORT_SERVO_SOURCE
    REG_PLAT_PROC1_2ND_INT_MASK_DISABLE = INT_PROC1_2ND_SERVO; // Brian0.67
#endif  // #ifdef SUPPORT_SERVO_SOURCE

    // Chuan0.90, Set the SP T/B/C Fifo threshold.
    REG_MCU_VCR04 = 0x0F0F;
    REG_MCU_VCR0C = 0x0F0F;
    REG_MCU_VCR14 = 0x0C04;

    // Micky2.11a, move form HAL_InitVariables
    // kevin0.91, set REG_PLAT_SYSTEM_CONFIGURATION1 in one place
    // Since, F/W must disable interrupt and do key lock before accessing
    OS_DISABLE_INTERRUPTS( dwSaveInt );
    MACRO_PLAT_KEY_LOCK( );

    // Disable external ADC
    REG_PLAT_SYSTEM_CONFIGURATION1 &= ~(1<<31);

    // Following procedure is used to calculate final system configuration1 value
    REG_PLAT_SYSTEM_CONFIGURATION1 = ( REG_PLAT_SYSTEM_CONFIGURATION1
    // LLY2.56, DMA arbiter control is necessary only for CT909R
#ifdef  CT909R_IC_SYSTEM
    // Set bit[10] as 1, just for CT909BB IC (DMA arbiter Control Register)
                                        + (0x1 << 10)
#endif  // #ifdef CT909R_IC_SYSTEM

    // LLY2.51, it's un-necessary to config system configuration 1 for external Audio DAC @ CT909P
    // Since, it was removed to GPE Mux selection register.
#ifdef CT909R_IC_SYSTEM
#if (AUDIO_DAC != DAC_INTERNAL)
#ifdef  SUPPORT_2_CHANNEL_ONLY
    // LLY2.20, only enable bit[24] for PCMSD0 while using external auido DAC + 2 channel only
                                        + (0x1<<24)
#else   // #ifdef SUPPORT_2_CHANNEL_ONLY
    // LLY2.20, enable bit[24, 25, 26, 27] for PCMSD0/1/2 while 5.1 channel solution.
                                        + (0xf<<24)
#endif  // #ifdef   SUPPORT_CHANNEL_ONLY
#endif  // #if (AUDIO_DAC != DAC_INTERNAL)
#endif  // #ifdef CT909R_IC_SYSTEM

    // LLY2.56, 2 channel only configuration is different @ CT909G
#ifdef CT909G_IC_SYSTEM
#if (AUDIO_DAC != DAC_INTERNAL)
#ifdef  SUPPORT_2_CHANNEL_ONLY
    // enable bit[25]: 5.1ch L/R and bit[24]: DAC aclk/ bclk/ ws while using external auido DAC + 2 channel only
                                        + (0x3<<24)
#else   // #ifdef SUPPORT_2_CHANNEL_ONLY
    // enable bit[27:25]: 5.1 ch and bit[24]: DAC aclk/ bclk/ ws while 5.1 channel solution.
                                        + (0xf<<24)
#endif  // #ifdef   SUPPORT_CHANNEL_ONLY
#endif  // #if (AUDIO_DAC != DAC_INTERNAL)
#endif  // #ifdef CT909G_IC_SYSTEM


#ifdef  SUPPORT_CCIR656
    // CCIR656: Set DVI[22]=1
    // CCIR601: Set DVI[22]=1 and SyncOut[23]=1
                                        + (1<<22)
// Alan2.18, Invert Pixel Clock, if not define "not to inverse VCLK" (default is off)
#ifndef VCLK_NO_INVERSE
                                        + (1<<21)
#endif  // #ifndef VCLK_NO_INVERSE
#endif  // #ifdef SUPPORT_CCIR656
                                        );

    MACRO_PLAT_KEY_UNLOCK( );
    OS_RESTORE_INTERRUPTS( dwSaveInt );

    // LLY2.51, must configuration GPE Mux selection register for external audio DAC @ CT909P
#ifdef  CT909P_IC_SYSTEM
#if (AUDIO_DAC != DAC_INTERNAL)
    // Always enable GPEMux[2:1] as 1 to enable PCMWS/ PCMCLK/ ACLK for external audio DAC
    REG_PLAT_GPE_MUX |= (0x3L<<1);
#ifdef  SUPPORT_2_CHANNEL_ONLY
    // Enable GPEMux[6]=1 to enable PCMSD0 for 2 channel only
    REG_PLAT_GPE_MUX |= (0x1L<<6);
#else   // #ifdef  SUPPORT_2_CHANNEL_ONLY
    // Enable GPEMux[6:3]=1 to enable PCMSD0/1/2/3 for 5.1 channel
    REG_PLAT_GPE_MUX |= (0xfL<<3);
#endif  // #ifdef  SUPPORT_2_CHANNEL_ONLY
#endif  // #if (AUDIO_DAC != DAC_INTERNAL)

#endif  // #ifdef CT909P_IC_SYSTEM

    // LLY2.16, remove the procedure to HAL_StartSystem(), since servo init may use expand GPIO
#if 0
    // LLY2.13, initial expand GPIO while using it
    // Notice: must put the procedure as earily as possible since servo init may use expand GPIO
    HAL_Init_ExpGPIO();
#endif  // #if 0

    // LLY2.38, remove DVD/ DVB switching procedure as earily as possible
    // ie. must earier than servo init.
    // Otherwise, servo action can't work.
    // LLY2.15, set DVD/ DVB switch mode as DVD while power-on
    // Otherwise, servo signal can't come-in
#if 0
#if (DECODER_SYSTEM != DVD909S_EVAL)
    HAL_WriteGPIO(DVD_DVB_SWITCH_GRP, PIN_DVD_DVB_SWITCH, 1);
#endif  // #if (DECODER_SYSTEM != DVD909S_EVAL)
#endif  // #if 0

#endif // #ifndef AP_FRAMEWORK

    // Initial OSD FIFO threshold @ REG_MCU_VCR24
    // And, multiple 2 for CT909S
    // LLY2.10-1, Chuan says 0x2004005 will cause previous video flash while DivX do pre-scan period
    // LLY2.12, porting Chuan's code to modify MCR4/ VCR24/ SYS_CONFIG1 for CT909S BB version IC
    // To fix OSD flick issue.
#if 0
    REG_MCU_MCR4 = 0x2004004; //0x2004005;  // DMA Arbiter Control Register
    REG_MCU_VCR24 = 0x3A20;
#endif  // #if 0
    // LLY2.56, DMA arbiter control is necessary only for CT909R
#ifdef  CT909R_IC_SYSTEM
    REG_MCU_MCR4 = 0x2002005;  // DMA Arbiter Control Register
#endif  // #ifdef CT909R_IC_SYSTEM
    REG_MCU_VCR24 = 0x3A20;

    // LLY1.21-1, remove code protection procedure before PROC2 go.
    INT_Init( );

    // LLY2.15, porting YC's code to enable watch-dog
    HAL_WatchDog_Enable(TRUE);


    if (dwAck)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

#endif  // #ifdef SYSTEM_ECOS
}


void HAL_PowerControl(BYTE bDevice, BYTE bAction)
{
    DWORD dwSaveInt;
    switch(bDevice)
    {
        // LLY2.36, add new case to power down PROC2, Not Test !!
        case    HAL_POWER_PROC2:
            if (bAction == HAL_POWER_NORMAL)
            {
                OS_DISABLE_INTERRUPTS( dwSaveInt );
                REG_PLAT_CLK_GENERATOR_CONTROL &= ~(PLAT_MCLK_PROC2_DISABLE);
                OS_RESTORE_INTERRUPTS( dwSaveInt );
                // Notice: delay a while to wait PROC2 enable ok
                OS_DelayTime(COUNT_50_MSEC);
            }
            else
            if (bAction == HAL_POWER_SAVE)
            {
                OS_DISABLE_INTERRUPTS( dwSaveInt );
                REG_PLAT_CLK_GENERATOR_CONTROL |= (PLAT_MCLK_PROC2_DISABLE);
                OS_RESTORE_INTERRUPTS( dwSaveInt );

            }
            break;

        // LLY2.36, must control VDEC2 too
        case    HAL_POWER_VDEC:
            if (bAction == HAL_POWER_NORMAL)
            {
                if (REG_PLAT_CLK_GENERATOR_CONTROL & (PLAT_MCLK_VDEC_DISABLE|PLAT_MCLK2_VDEC_DISABLE))
                {
                    OS_DISABLE_INTERRUPTS( dwSaveInt );
                    REG_PLAT_CLK_GENERATOR_CONTROL &= ~(PLAT_MCLK_VDEC_DISABLE | PLAT_MCLK2_VDEC_DISABLE);
                    OS_RESTORE_INTERRUPTS( dwSaveInt );
                    // Notice: delay a while to wait VDEC enable ok
                    OS_DelayTime(COUNT_50_MSEC);
                }
            }
            else if (bAction == HAL_POWER_SAVE)
            {
                OS_DISABLE_INTERRUPTS( dwSaveInt );
                REG_PLAT_CLK_GENERATOR_CONTROL |= (PLAT_MCLK_VDEC_DISABLE | PLAT_MCLK2_VDEC_DISABLE);
                OS_RESTORE_INTERRUPTS( dwSaveInt );
            }
            break;

        case    HAL_POWER_PROM:
            if (bAction == HAL_POWER_NORMAL)
            {
                OS_DISABLE_INTERRUPTS( dwSaveInt );
                REG_PLAT_CLK_GENERATOR_CONTROL &= ~(PLAT_MCLK_PROM_DISABLE);
                OS_RESTORE_INTERRUPTS( dwSaveInt );
                // Notice: delay a while to wait PROM enable ok
                OS_DelayTime(COUNT_50_MSEC);
            }
            else
            if (bAction == HAL_POWER_SAVE)
            {
                OS_DISABLE_INTERRUPTS( dwSaveInt );
                REG_PLAT_CLK_GENERATOR_CONTROL |= (PLAT_MCLK_PROM_DISABLE);
                OS_RESTORE_INTERRUPTS( dwSaveInt );
            }
            break;

// LLY2.56, don't support USB path for CT909G
// Notice: using ifndef CT909G IC to instead of USB source temporally
//         since compiler error if disable "SUPPORT_USB_SOURCE"
//#ifdef  SUPPORT_USB_SOURCE
#ifndef CT909G_IC_SYSTEM
#ifdef SUPPORT_USB
        case    HAL_POWER_USB:
            if (bAction == HAL_POWER_NORMAL)
            {
#ifdef SUPPORT_USB_POWER_BY_IO
                if (SrcFilter_GetOTGMode() == USB_MODE_HOST)
                    HAL_WriteGPIO(USB_POWER_GRP, PIN_USB_POWER, 1); //Jeff 20050602 Add for USB_POWER
#endif
                OS_DISABLE_INTERRUPTS( dwSaveInt );
                REG_PLAT_CLK_GENERATOR_CONTROL &= ~(PLAT_UCLK48M_USB_DISABLE | PLAT_HCLK_USB_DISABLE);
                OS_RESTORE_INTERRUPTS( dwSaveInt );
                OS_DelayTime(COUNT_50_MSEC);
            }
            else if (bAction == HAL_POWER_SAVE)
            {

#ifdef SUPPORT_USB_POWER_BY_IO
                HAL_WriteGPIO(USB_POWER_GRP, PIN_USB_POWER, 0); //Jeff 20050602 Add for USB_POWER
#endif //
                OS_DISABLE_INTERRUPTS( dwSaveInt );
                // wyc2.05-909S, update the code to enable USB clock and exit USB HW when entering DVD mode, also enable USB when
                // enter USB mode. This can avoid the USB HW to affect other modules.
                REG_PLAT_CLK_GENERATOR_CONTROL |= (PLAT_UCLK48M_USB_DISABLE | PLAT_HCLK_USB_DISABLE);
                OS_RESTORE_INTERRUPTS( dwSaveInt );
                OS_DelayTime(COUNT_50_MSEC);
            }
            break;
#endif //
#endif  // #ifdef SUPPORT_STB

        // LLY2.13, add the procedure to power down all available MPEG part
        // LLY2.15, remove UART related setting.
        case    HAL_POWER_PLL:
            OS_DISABLE_INTERRUPTS( dwSaveInt );

            if (bAction == HAL_POWER_NORMAL)
            {
                // Now, 909S wake up procedure are:
                // (1) Power on MPLL/ APLL/ UPLL
                // (2) Set MCLK to select MPLL
                // (3) Readjust system time & UART baudrate since MCLK is updated.

                // LLY2.22, give CPU_SPEED directly
                // LLY2.36, do nothing while current MPLL clock is same as user desired
                if(__dwSysClk != CPU_SPEED)
                {
                    // LLY2.56, porting Chuan's code for CT909P/ CT909G power down control
                    // Since, they are different than CT909R
#ifdef  CT909R_IC_SYSTEM
                    // Restore all PLL as previous, bit[20]=0
                    REG_PLAT_MPLL_CONTROL &= ~(1 << 20);
                    REG_PLAT_APLL_CONTROL &= ~(1<< 20);
                    REG_PLAT_UPLL_CONTROL &= ~(1<< 20);
#else   // #ifdef CT909R_IC_SYSTEM
                    REG_PLAT_MPLL_CONTROL &= ~(1 << 8); // Power down MPLL/APLL/UPLL at the same time.
#endif  // #ifdef CT909R_IC_SYSTEM
                    HAL_ClockSet(MODE_MPLL, CPU_SPEED);
#ifdef EMU_RTC   
                    //Aron2.77, added for DMP SW-RTC
                    RTC_Emu_ChkTimerReload(RTC_EMU_WAKEUP);
#endif                
                }


                // Re-adjust system timer w/ normal system clock
                // Notice: must update the __dwSysClk value together
                __dwSysClk=CPU_SPEED;
                REG_PLAT_PRESCALER_RELOAD = ((__dwSysClk/1000000)-1)/2;

                // LLY2.15, reset baud rate for DSU1/ UART1/ UART2
                // Notice: need double how to reset DSU1 baudrate, since can't work
                //HAL_DSU_BaudRateSet((__dwSysClk/2), BAUDRATE_115200, HAL_DSU1);
                HAL_UART_BaudRateSet((__dwSysClk/2), BAUDRATE_115200, HAL_UART1);
                HAL_UART_BaudRateSet((__dwSysClk/2), BAUDRATE_115200, HAL_UART2);

            }
            else if(bAction == HAL_POWER_SAVE)
            {
                // Now, 909S power down method are:
                // (1) Set MCLK to select extra OSCI (27MHz)
                // (2) Power down MPLL/ APLL/ UPLL
                // (3) Readjust system time & UART baudrate since MCLK is updated.

                // LLY2.16, system clock must be work >= 54MHz
                // Otherwise, we will miss IR interrupt
                // IR work @ 27MHz, interrupt work @ pclk = system clock/2, and must >= 27MHz
                // LLY2.36, set power down CPU speed as 27M for power consumption issue
                // So, must change IR working mode from interrupt to polling @ 27M
                // LLY2.38, using global define to set desired CPU speed for power down mode.
                //if(HAL_ClockSet(MODE_MPLL, CPU_27M))
                if(HAL_ClockSet(MODE_MPLL, POWERDOWN_CPU_SPEED))
                {
                    // update the corresponding __dwSysClk value
                    //__dwSysClk=CPU_27M;
                    __dwSysClk=POWERDOWN_CPU_SPEED;

                    // Power down all PLL, bit[20]=1 while set as 27M
                    if(__dwSysClk==CPU_27M)
                    {
                    // LLY2.56, porting Chuan's code for CT909P/ CT909G power down control
                    // Since, they are different than CT909R
#ifdef  CT909R_IC_SYSTEM
                        REG_PLAT_MPLL_CONTROL |= (1 << 20);
                        REG_PLAT_APLL_CONTROL |= (1<< 20);
                        REG_PLAT_UPLL_CONTROL |= (1<< 20);
#else   // #ifdef CT909R_IC_SYSTEM
                        REG_PLAT_MPLL_CONTROL |= (1 << 8);
#endif  // #ifdef CT909R_IC_SYSTEM
#ifdef EMU_RTC
                        //Aron2.77, added for DMP SW-RTC
                        RTC_Emu_ChkTimerReload(RTC_EMU_SLEEP);
#endif
                    }
                }
                else
                {
                    // LLY2.36, must restore the original MPLL clock
                    // Since user desired setting is not work
                    __dwSysClk=CPU_SPEED;
                }


                // Re-adjust system timer w/ new system clock
                REG_PLAT_PRESCALER_RELOAD = ((__dwSysClk/1000000)-1)/2;

                // LLY2.15, reset baud rate for DSU1/ UART1/ UART2
                // Notice: need double how to reset DSU1 baudrate, since can't work
                //HAL_DSU_BaudRateSet((__dwSysClk/2), BAUDRATE_115200, HAL_DSU1);
                HAL_UART_BaudRateSet((__dwSysClk/2), BAUDRATE_115200, HAL_UART1);
                HAL_UART_BaudRateSet((__dwSysClk/2), BAUDRATE_115200, HAL_UART2);

            }

            OS_RESTORE_INTERRUPTS( dwSaveInt );
            break;

#ifndef  SUPPORT_SERVO_SOURCE
        case    HAL_POWER_SERVO:
            if (bAction == HAL_POWER_NORMAL)
            {
                OS_DISABLE_INTERRUPTS( dwSaveInt );
                REG_PLAT_CLK_GENERATOR_CONTROL &= ~(PLAT_CDCLK_DISABLE | PLAT_DVDCLK_DISABLE);
                OS_RESTORE_INTERRUPTS( dwSaveInt );
                OS_DelayTime(COUNT_50_MSEC);
            }
            else
            if (bAction == HAL_POWER_SAVE)
            {
                // (*(volatile DWORD *)(0x800026F8) = (DWORD)(1<<20); // Power down ADC6
                // MACRO_IDLE();
                OS_DISABLE_INTERRUPTS( dwSaveInt );
                REG_PLAT_CLK_GENERATOR_CONTROL |= (PLAT_CDCLK_DISABLE | PLAT_DVDCLK_DISABLE);
                OS_RESTORE_INTERRUPTS( dwSaveInt );
            }
            break;
#endif	// #ifndef  SUPPORT_SERVO_SOURCE

        default:
                break;

    }
}

//**************************************************************************
// Description  :   Use an IO pin to act as H/W circuit mute in parallel with P_MUTE/D_MUTE
// Arguments    :   bMute: TRUE: mute
//                           FALSE: unmute
// Return       :   None
//***************************************************************************
void    HAL_IOMute(BYTE bMute) //test OK
{
    // LLY2.11, disable UART2 while IOMUTE pin is shared w/ UART2
    // LLY2.15, don't disble UART2 here since programmer maybe use it for debug and skip IOMUTE
#if 0//def IOMUTE_UART2_SAME_PORT
    HAL_UART_Disable(HAL_UART2);
#endif  // #ifdef IOMUTE_UART2_SAME_PORT

    // LLY2.13, using general define for HWMUTE GPIO group/ pin
    // LLY2.18, add procedure for new IO mute control
#ifdef  IOMUTE_ACTIVE_LOW
    // Mute: send value 0
    // DeMute: set pin as input mode while it w/ poll-high 5V or
    // DeMute: send value 1
    if(bMute)
    {
        HAL_WriteGPIO(HWMUTE_GPIO_GRP, PIN_HWMUTE, 0);
    }
    else
    {
        // LLY2.38, support different method for de-mute action base on if add pull-high 5V circuit
#ifdef  IOMUTE_NOT_ADD_PULL_HIGH_5V
        // If not add pll-high 5V circuit, just set output mode w/ value 1
        HAL_WriteGPIO(HWMUTE_GPIO_GRP, PIN_HWMUTE, 1);
#else   // #ifdef IOMUTE_NOT_ADD_PULL_HIGH_5V
        // Use "read GPIO" procedure to configure the pin as input mode
        // while IO mute circuit w/ poll-high 5V
        HAL_ReadGPIO(HWMUTE_GPIO_GRP, PIN_HWMUTE);
#endif  // #ifdef IOMUTE_NOT_ADD_PULL_HIGH_5V
    }
#else   // #ifdef IOMUTE_ACTIVE_LOW
    // Mute: send value as 1
    // DeMute: send value as 0
    if (bMute)
    {
        HAL_WriteGPIO( HWMUTE_GPIO_GRP, PIN_HWMUTE, 1);
    }
    else
    {
        HAL_WriteGPIO( HWMUTE_GPIO_GRP, PIN_HWMUTE, 0);
    }
#endif  // #ifdef IOMUTE_ACTIVE_LOW
}


// LLY2.04 create
//  ************************************************************************************
//  Function    :   HAL_GetDRAMSize
//  Description :   Get DRAM Size
//  Arguments   :   None
//  Return      :   HAL_DRAM_XXX, specify the DRAM size, the final size after down-grade
//                  The value is real DRAM end address
//  Notice      :
//  ************************************************************************************
DWORD HAL_GetDRAMSize(void)
{
    BYTE    bIndx;

    // Abstract DRAM type, bit[4:0]
    bIndx = (BYTE)(REG_PLAT_SYSTEM_CONFIGURATION1 & 0x1F);

    // If bit[4:3]=11, normal DRAM
    if( (bIndx & 0x18) == 0x18)
    {
        switch(bIndx)
        {
        case    0x1A:   // 32Mb, 2Mb x 16 (1x12x8 dram type)
        case    0x1F:   // 32Mb, 1Mb x 16 x 2 (1x11x8 cascade)
#ifdef CT909G_IC_SYSTEM//peteryu275, for support DRAM type, 2007/3/23 11:56AM
        case    0x18:   //32Mb, 2Mb x 16 ( 1x11x9 dram type)
#endif
            return (HAL_DRAM_32M);

        case    0x1E:   // 64Mb, 4Mb x 16 (2x12x8 dram type)
            return (HAL_DRAM_64M);

        case    0x1D:   // 128Mb, 8Mb x 16 (2x12x9 dram type)
            return (HAL_DRAM_128M);

        case    0x1C:   // 256Mb, 16Mb x 16 (2x13x9 dram type)
            return (HAL_DRAM_256M);

        case    0x1B:   // 16Mb, 1Mb x 16  (1x11x8 dram type)
            return (HAL_DRAM_16M);

        default:
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Unknown DRAM type to decide DRAM size !\n");
            return (HAL_DRAM_UNKNOWN);
        }
    }
    // If bit[4:3]=01, 64M downgrade, let setting same as 32M
    else if(bIndx & 0x08)
    {
        return (HAL_DRAM_32M);
    }
    // If bit[4:3]=10, 128M downgrade, let setting same as 64M
    else if(bIndx & 0x10)
    {
       //return (HAL_DRAM_32M);
       return (HAL_DRAM_64M);//peteryu275, for support DRAM type, 2007/3/23 11:56AM
    }
    else
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Unknown DRAM type to decide DRAM size !\n");
        return (HAL_DRAM_UNKNOWN);
    }

}
//************************************************************************
// Description  :   Checksum
// Argument     :   dwStart: start addr (DWORD aligned)
//                  dwEnd: end addr (DWORD aligned)
// Return       :   checksum value
//************************************************************************
WORD HAL_CheckSum(DWORD dwStart, DWORD dwEnd) //test OK
{
    DWORD   dwAddr, dwMemValue;
    WORD    wResult = 0;
    for ( dwAddr=dwStart; dwAddr<dwEnd; dwAddr+=4)
    {
        dwMemValue = *((volatile DWORD *)dwAddr );
        wResult += (BYTE)((dwMemValue&0xFF000000)>>24);
        wResult += (BYTE)((dwMemValue&0x00FF0000)>>16);
        wResult += (BYTE)((dwMemValue&0x0000FF00)>> 8);
        wResult += (BYTE)( dwMemValue&0x000000FF);
    }
    return wResult;
}


// LLY2.15, define watch dog enable control bit
#define WATCHDOG_SINGAL_ENABLE  (1L<<28) //  REG_PLAT_SYSTEM_CONFIGURATION1 [28]

// LLY2.15, define the watch dog down count value
#define WATCHDOG_DOWN_COUNT_VALUE   0xA00000

// LLY2.15 create
//  ***************************************************************************
//  Function    :   HAL_WatchDog_Enable
//  Description :   Control Watch-Dog mechanism enable/ disable
//  Argument    :   TRUE, enable watch dog
//                  FALSE, disable watch dog
//  ***************************************************************************
void HAL_WatchDog_Enable(BYTE bEnable)
{
    DWORD   dwSaveInt;
#ifdef SUPPORT_WATCH_DOG
    DWORD   dwInitClk, dwCurClk, dwGap;

    //OS_DISABLE_INTERRUPTS( dwSaveInt );
    //MACRO_PLAT_KEY_LOCK( );

    if (bEnable)
    {
        REG_PLAT_WATCHDOG = WATCHDOG_DOWN_COUNT_VALUE;
        dwInitClk = REG_PLAT_TIMER1_COUNTER;

        // Here, we need idle time, (1 tick + 1/27MHz) ~= 2 ticks, to wait counter valid.
        while( TRUE )
        {
            MACRO_IDLE( );
            dwCurClk = REG_PLAT_TIMER1_COUNTER;
            dwGap = dwInitClk - dwCurClk;
            if( dwCurClk > dwInitClk )
            {
                dwGap += (REG_PLAT_PRESCALER_RELOAD + 1);
            }

            if( dwGap >= 2 )
            {
                break;
            }
        }

        OS_DISABLE_INTERRUPTS( dwSaveInt );
        MACRO_PLAT_KEY_LOCK( );

        REG_PLAT_SYSTEM_CONFIGURATION1 |= (WATCHDOG_SINGAL_ENABLE);
    }
    else
#endif // #ifdef SUPPORT_WATCH_DOG
    {
        OS_DISABLE_INTERRUPTS( dwSaveInt );
        MACRO_PLAT_KEY_LOCK( );
        REG_PLAT_SYSTEM_CONFIGURATION1 &= ~(WATCHDOG_SINGAL_ENABLE);
    }
    MACRO_PLAT_KEY_UNLOCK( );
    OS_RESTORE_INTERRUPTS( dwSaveInt );

}


//  ***************************************************************************
//  Function    :   HAL_WatchDog_Status
//  Description :   Report Watch-Dog disable/ enable status.
//  Arguments   :   None
//  Return      :   TRUE, watch dog is enabled.
//                  FALSE, watch dog is disabled
//  ***************************************************************************
BYTE HAL_WatchDog_Status(void)
{
#ifdef  SUPPORT_WATCH_DOG
    if(REG_PLAT_SYSTEM_CONFIGURATION1 & WATCHDOG_SINGAL_ENABLE)
    {
        return TRUE;
    }
#endif  // #ifdef SUPPORT_WATCH_DOG
    return FALSE;
}


//  ***************************************************************************
//  Function    :   HAL_WatchDog_Reset
//  Description :   Reset watch dog down count value as default
//  Argument    :   None
//  Return      :   None
//  ***************************************************************************
void HAL_WatchDog_Reset(void)
{
#ifdef  SUPPORT_WATCH_DOG
    REG_PLAT_WATCHDOG = WATCHDOG_DOWN_COUNT_VALUE;
#endif  // #ifdef SUPPORT_WATCH_DOG
}


// ***********************************************************************
//      Function        :   HAL_ClockSet
//      Description     :   Adjust Clock setting
//      Arguments       :   bMode:
//      Return          :   TRUE, the desired action is done
//                          FALSE, the desired action is fail
//      Side Effect     :
// ***********************************************************************
// LLY2.22, re-modify 2nd parameter type from BYTE to DWORD
// Since, caller may give CPU_SPEEd directly
// LLY2.36, re-modify this API since need return value to know the action ok or fail.
//void    HAL_ClockSet(BYTE bMode, DWORD dwType)
DWORD   HAL_ClockSet(BYTE bMode, DWORD dwType)
{
    DWORD dwCLK;

    switch(bMode)
    {
        case MODE_MPLL:
            // LLY2.36, call SPF_MPLLSet() to while using SPI Flash
            dwCLK = SPF_MPLLSet(dwType);
            if(dwCLK)
            {
                // Let IR checking method as polling while MPLL = 27MHz
                // Since, IR interupt must work >= 27MHz, and get from MPLL/2
                if(dwType == CPU_27M)
                {
                    INPUT_SetIRCheckMode(IR_CHECK_MODE_POLLING);
                }
                else
                {
                    INPUT_SetIRCheckMode(IR_CHECK_MODE_INT);
                }
            }
            return dwCLK;


        case MODE_APLL:
            // Ext_ACLK = 1, PD = 0, PF = 0, MF = 1, NF = 16 : Fout = 324
            REG_PLAT_APLL_CONTROL = (1 << 20) + (0 << 20) + (0 << 18) + (1 << 11) + (16);
            break;

// LLY2.56, don't support USB path for CT909G
// Notice: using ifndef CT909G IC to instead of USB source temporally
//         since compiler error if disable "SUPPORT_USB_SOURCE"
//#ifdef  SUPPORT_USB_SOURCE
#ifndef CT909G_IC_SYSTEM
        case MODE_UPLL:
            // PD = 0, PF = 0, MF = 1, NF = 14 : Fout = 288
            REG_PLAT_UPLL_CONTROL = (0 << 20) + (0 << 18) + (1 << 11) + (14);
            break;
#endif  // #ifdef SUPPORT_USB_SOURCE

        case MODE_CLKCTL_VIDEO:
            dwCLK = REG_PLAT_CLK_FREQ_CONTROL1;
            if ((dwType&TYPE_PSCAN) != TYPE_PSCAN)
            {
                dwCLK |= (1 << 24);
            }
            else
            {
                dwCLK &= ~(1 << 24);
            }
            REG_PLAT_CLK_FREQ_CONTROL1 = dwCLK;
            break;

        case MODE_CLKCTL_SERVO:
            break;
    }

    return TRUE;
}


// LLY2.61 create ...
#ifdef  SUPPORT_SCART_IF
#ifdef  SUPPORT_SCART_PWM_CTRL
//  *******************************************************************
//  Function    :   HAL_SCART_PWM_Init
//  Description :   Do SCART PWM related register initial
//  Arguments   :   bPort, specify the desired PWM port
//  Return      :   TRUE, the action is successfully
//                  FALSE, the action fail
//  *******************************************************************
BYTE HAL_SCART_PWM_Init(BYTE bPort)
{
    // Clear bit[5:4] first
    REG_PLAT_SYS_PIN_USE1 &= ~(0x3L<<4);

    if(bPort==SCART_PWM0)
    {
        // enable bit[4] to use SCART_PWM0
        REG_PLAT_SYS_PIN_USE1 |= (0x1L<<4);
    }
    else if(bPort==SCART_PWM1)
    {
        // enable bit[4] to use SCART_PWM0
        REG_PLAT_SYS_PIN_USE1 |= (0x1L<<5);
    }
    else
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: not defined PWM ID\n");
        return FALSE;
    }

    return TRUE;
}


//  ******************************************************************************************
//  Function    :   HAL_SCART_PWM_Ctrl
//  Description :   Control SCART PWM related register
//  Arguments   :   bPort, specify the desired PWM port
//                  bVal, the value from 0 ~ 0xFF, used to specify the ratio while output 1
//                        ex, if give 0x7F, output ratio of 1 is 0x7F/0xFF = 1/2
//                            if give 0xFF, output ratio of 1 is 0xFF/0xFF = 1
//                            if give 0x0, output ratio of 1 is 0x0/0xFF = 0
//  Return      :   TRUE, the action is successfully
//                  FALSE, the action fail
//  ******************************************************************************************
BYTE HAL_SCART_PWM_Ctrl(BYTE bPort, BYTE bVal)
{
    if(bPort==SCART_PWM0)
    {
        REG_PLAT_SCART_PWM_CONFIGURATION &= ~(0xFFL);
        REG_PLAT_SCART_PWM_CONFIGURATION |= (0x1L<<12) + bVal;
    }
    else if(bPort==SCART_PWM1)
    {
        REG_PLAT_SCART_PWM_CONFIGURATION &= ~(0xFFL<<16);
        REG_PLAT_SCART_PWM_CONFIGURATION |= (0x1L<<28) + ((DWORD)bVal<<16);
    }
    else
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: not defined PWM ID\n");
        return FALSE;
    }

    printf("PWM: %lx\n", bVal);
    return TRUE;
}

#endif  // #ifdef SUPPORT_SCART_PWM_CTRL
#endif  // #ifdef  SUPPORT_SCART_IF
