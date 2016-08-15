#include "winav.h"
#include <cyg/hal/hal_intr.h>
#include "ctkav.h"
#include "int_gbl.h"
#include "parser.h"
#include "comdec.h"
#include "subpict.h"
#include "utl.h"
#include "hal.h"
// wyc2.71-portusb
#if (USB_SYSTEM == USB2_0_JUNGO)
#include "usbwrap.h"
#endif //

// #define MEASURE_INT_TIME
#if 0
    else if ( bKey == KEY_TEST1 )
    {
        DWORD dwSaveInt;
        DWORD dwCnt, dwTotal1, dwTotal2;        
extern DWORD   __dwINT_MaxTime_ISR[2];
extern DWORD   __dwINT_MaxTime_DSR[2];
extern DWORD   __dwINT_MaxTotalTime[2];
extern DWORD   __dwINT_MaxTotalTime_PENDING[2];
extern DWORD   __dwINT_MaxTime_PENDING[2];
extern PWORD   __wINT_TotalTime1;
extern DWORD   __dwTotalCnt[2];
extern PWORD   __wINT_TotalTime2;


        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "ISR)INT13:%lx(%lx) ; INT10:%lx(%lx)", 
            __dwINT_MaxTime_ISR[1], __dwINT_MaxTime_PENDING[1], __dwINT_MaxTime_ISR[0], __dwINT_MaxTime_PENDING[0]);
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DSR)INT13:%lx ; INT10:%lx", __dwINT_MaxTime_DSR[1], __dwINT_MaxTime_DSR[0]);
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Total)INT13:%lx(%lx) ; INT10:%lx(%lx)",
            __dwINT_MaxTotalTime[1], __dwINT_MaxTotalTime_PENDING[1], __dwINT_MaxTotalTime[0], __dwINT_MaxTotalTime_PENDING[0]);

        HAL_DISABLE_INTERRUPTS( dwSaveInt );
        dwTotal1 = 0;
        for (dwCnt = 0; dwCnt < __dwTotalCnt[1]; dwCnt++)
        {
            dwTotal1 += __wINT_TotalTime1[dwCnt];
        }
        dwTotal1 = dwTotal1 / __dwTotalCnt[1];        

        dwTotal2 = 0;
        for (dwCnt = 0; dwCnt < __dwTotalCnt[0]; dwCnt++)
        {
            dwTotal2 += __wINT_TotalTime2[dwCnt];
        }
        dwTotal2 = dwTotal2 / __dwTotalCnt[0];        

        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AVG)INT13:%lx(%lx) ; INT10:%lx(%lx)", 
            dwTotal1, __dwTotalCnt[1], dwTotal2, __dwTotalCnt[0]);
    
        __dwINT_MaxTime_ISR[0] = 0;
        __dwINT_MaxTime_ISR[1] = 0;
        __dwINT_MaxTime_DSR[0] = 0;
        __dwINT_MaxTime_DSR[1] = 0;
        __dwINT_MaxTotalTime[0] = 0;
        __dwINT_MaxTotalTime[1] = 0;
        __dwINT_MaxTime_PENDING[0] = 0;
        __dwINT_MaxTime_PENDING[1] = 0;
        __dwINT_MaxTotalTime_PENDING[0] = 0;
        __dwINT_MaxTotalTime_PENDING[1] = 0;
        __dwTotalCnt[0] = 0;
        __dwTotalCnt[1] = 0;
        HAL_RESTORE_INTERRUPTS( dwSaveInt );
        return KEY_NO_KEY;
    }
#endif

// declare global variables
INT_PROC1_1ST_STATUS    __INTISRintProc11stStatus;
INT_PROC1_2ND_STATUS    __INTISRintProc12ndStatus;
INT_PROC1_SW_STATUS     __INTISRintProc1SWStatus;

// counters for ISR and DSR
DWORD   __dwINTISRCounter[MAX_INT_COUNT], __dwINTDSRCounter[MAX_INT_COUNT];
// function pointer for DSR and ISR
PISRDSR_ROUTINE     __pINTISRRoutine[MAX_INT_COUNT][MAX_2ND_INT_COUNT];
PISRDSR_ROUTINE     __pINTDSRRoutine[MAX_INT_COUNT][MAX_2ND_INT_COUNT];

// interrupt handle and interrupt object for INT10 & INT13
cyg_interrupt   _INTPROC11st, _INTPROC1SW, _INTPROC12nd;
HANDLE_T	_INTPROC11stHandle, _INTPROC1SWHandle, _INTPROC12ndHandle;

#ifdef EMU_RTC
//Aron2.77, added for DMP SW-RTC
cyg_interrupt    __INTTIMER2;
HANDLE_T	__INTTIMER2Handle;
#endif

// Chuan2.21, Calculate INTs
// #define INT_STAT
#ifdef INT_STAT
DWORD   __dwINT1_Stat[8];
DWORD   __dwINT2_Stat[24];
#endif

#ifdef MEASURE_INT_TIME
DWORD   __dwINT_MaxTime_ISR[2] = { 0, 0 };
DWORD   __dwINT_MaxTime_DSR[2] = { 0, 0 };
DWORD   __dwINT_MaxTotalTime[2] = { 0, 0 };
DWORD   __dwINT_MaxTotalTime_PENDING[2] = { 0, 0 };
DWORD   __dwINT_MaxTime_PENDING[2] = { 0, 0 };
DWORD   __dwINT_Time_ISR[2] = { 0, 0 };
DWORD   __dwINT_PENDING[2] = { 0, 0 };

PWORD    __wINT_TotalTime1 = (PWORD)0x40400000;
DWORD   __dwTotalCnt[2] = { 0, 0 };
PWORD    __wINT_TotalTime2 = (PWORD)0x40500000;
#endif

// wyc2.71-portusb
#if (USB_SYSTEM == USB2_0_JUNGO)
extern void ISR_USBSaveClearStatus(void);
#endif //

//
// Interrupt service routine for interrupt 13.
//
cyg_uint32  INT_Proc1_1st_isr(
        cyg_vector_t vector,
        cyg_addrword_t data)
{
    DWORD   dwLoop;

#ifdef MEASURE_INT_TIME
    DWORD   dwStartTime;
    DWORD   dwEndTime;

    dwStartTime = REG_PLAT_TIMER2_COUNTER;
#endif

    // clear ISR counter and DSR counter
    __dwINTISRCounter[INT_NO13] = 0;
    __dwINTDSRCounter[INT_NO13] = 0;

    // save secondary level status and clear them for next interrupts
    __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING =
            (REG_PLAT_PROC1_1ST_INT_PENDING & REG_PLAT_PROC1_1ST_INT_MASK_ENABLE);

    //===========================================
    // save status & clear any pendding interrupt
    //===========================================
    // int_vsync (13:0)
    if( __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING & INT_PROC1_1ST_VSYNC )
    {
        ISR_DISPSaveClearStatus();
    }
    // int_nhsync (13:1) for 909S
    if( __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING & INT_PROC1_1ST_HSYNC )
    {
    }
    // int_screen_end (13:1) / (13:2)
    if( __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING & INT_PROC1_1ST_SCREEN_END )
    {
    }
    // int_main_end (13:2) / (13:3)
    if( __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING & INT_PROC1_1ST_MAIN_END )
    {
    }
    // int_osd_end (13:4)
    if( __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING & INT_PROC1_1ST_OSD_END )
    {
    }
    // rldone2s (13:5)
    if( __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING & INT_PROC1_1ST_RL_DONE )
    {
    }
    // mcdone (13:6)
    if( __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING & INT_PROC1_1ST_MC_DONE )
    {
    }
    // int_16l (13:7)
    if( __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING & INT_PROC1_1ST_INT_16L )
    {
#ifndef NO_DECODER
        ISR_16LSaveClearStatus();
#endif  //#ifndef NO_DECODER        
    }

    // clear secondary mask pendding interrupts
    REG_PLAT_PROC1_1ST_INT_CLEAR = __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING;

#ifdef INT_STAT
    dwLoop = 0;
    while (__INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING != 0)
    {
        __dwINT1_Stat[dwLoop] += __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING & 0x1;
        dwLoop++;
        __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING >>= 1;
    }
#endif

    // clear fisrt mask pendding interrupt 13
    REG_PLAT_INT_CLEAR = (1 << INT_NO_PROC1_1ST);

    // Tell the processor that we have received
    // the interrupt.
    // cyg_interrupt_acknowledge( vector );

    if( __dwINTISRCounter[INT_NO13] )
    {
        // process ISR routines one by one
        for( dwLoop = 0; dwLoop < __dwINTISRCounter[INT_NO13]; dwLoop ++ )
        {
            (*(__pINTISRRoutine[INT_NO13][dwLoop]))( );
        }
    }

#ifdef MEASURE_INT_TIME
    dwEndTime = REG_PLAT_TIMER2_COUNTER;
    __dwINT_Time_ISR[INT_NO13] = dwStartTime - dwEndTime;
    __dwINT_PENDING[INT_NO13] = __INTISRintProc11stStatus.dwPLAT_PROC1_1ST_INT_PENDING;
    
    if (dwEndTime > dwStartTime)
    {
        __dwINT_Time_ISR[INT_NO13] += 0xFFFFFF;
    }

    if (__dwINT_MaxTime_ISR[INT_NO13] < __dwINT_Time_ISR[INT_NO13])
    {
        __dwINT_MaxTime_ISR[INT_NO13] = __dwINT_Time_ISR[INT_NO13];
        __dwINT_MaxTime_PENDING[INT_NO13] = __dwINT_PENDING[INT_NO13];
    }
#endif

    // Tell the kernel that chained interrupt processing
    // is done and the DSR needs to be executed next.
    if( __dwINTDSRCounter[INT_NO13] )
    {
        // Block this interrupt from occurring until
        // the DSR completes.
        // Chuan2.17, Don't to mask 1st INT to avoid other DSR block 1st DSR
        // cyg_interrupt_mask( vector );

        return( CYG_ISR_HANDLED | CYG_ISR_CALL_DSR );
    }
    else
    {
        return( CYG_ISR_HANDLED );      // if no DSR needed to be executed
    }
}

//
// Deferred service routine for interrupt 13.
//
void    INT_Proc1_1st_dsr(
        cyg_vector_t vector,
        cyg_ucount32 count,
        cyg_addrword_t data)
{
    /*
    
    DWORD   dwLoop;

    // process DSR routines one by one
    for( dwLoop = 0; dwLoop < __dwINTDSRCounter[INT_NO13]; dwLoop ++ )
    {
        (*(__pINTDSRRoutine[INT_NO13][dwLoop]))( );
    }
    */
    DWORD   dwSaveInt;
#ifdef MEASURE_INT_TIME
    DWORD   dwStartTime;
    DWORD   dwEndTime;
    DWORD   dwTotalTime;
#endif

    HAL_DISABLE_INTERRUPTS( dwSaveInt );
#ifdef MEASURE_INT_TIME
    dwStartTime = REG_PLAT_TIMER2_COUNTER;
#endif

    (*(__pINTDSRRoutine[INT_NO13][0]))( );

#ifdef MEASURE_INT_TIME
    dwEndTime = REG_PLAT_TIMER2_COUNTER;
    dwTotalTime = dwStartTime - dwEndTime;
    if (dwEndTime > dwStartTime)
    {
        dwTotalTime += 0xFFFFFF;
    }

    if (__dwINT_MaxTime_DSR[INT_NO13] < dwTotalTime)
    {
        __dwINT_MaxTime_DSR[INT_NO13] = dwTotalTime;
    }

    if (__dwINT_MaxTotalTime[INT_NO13] < (dwTotalTime + __dwINT_Time_ISR[INT_NO13]))
    {
        __dwINT_MaxTotalTime[INT_NO13] = (dwTotalTime + __dwINT_Time_ISR[INT_NO13]);
        __dwINT_MaxTotalTime_PENDING[INT_NO13] = __dwINT_PENDING[INT_NO13];
    }    

    if (__dwTotalCnt[INT_NO13] < 10000)
    {
        __wINT_TotalTime1[__dwTotalCnt[INT_NO13]++] = (dwTotalTime + __dwINT_Time_ISR[INT_NO13]);
    }
#endif
    HAL_RESTORE_INTERRUPTS( dwSaveInt );

    // Allow this interrupt to occur again.
    // cyg_interrupt_unmask( vector );
}

//
// Interrupt service routine for interrupt 10.
//

//#define DUMP_IN_FORCE_STOP
cyg_uint32  INT_Proc1_2nd_isr(
        cyg_vector_t vector,
        cyg_addrword_t data)
{
    DWORD   dwLoop;

#ifdef DUMP_IN_FORCE_STOP
extern DWORD dwReqDetPerBlock;
extern void  ISR_SRVSaveClearStatus1( void );
#endif

#ifdef MEASURE_INT_TIME
    DWORD   dwStartTime;
    DWORD   dwEndTime;

    dwStartTime = REG_PLAT_TIMER2_COUNTER;
#endif

    // clear ISR counter and DSR counter
    __dwINTISRCounter[INT_NO10] = 0;
    __dwINTDSRCounter[INT_NO10] = 0;

    // save secondary level status and clear them for next interrupts
    __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING =
            (REG_PLAT_PROC1_2ND_INT_PENDING & REG_PLAT_PROC1_2ND_INT_MASK_ENABLE);
    //===========================================
    // save status & clear any pendding interrupt
    //===========================================
    // ohci_irq_o_n (10:0) / stb_overflow (10:0)
#if defined(CT909R_IC_SYSTEM) || defined(CT909P_IC_SYSTEM)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_USB_OHCI )
    {
// wyc2.71-portusb
#if (USB_SYSTEM == USB2_0_JUNGO)
        ISR_USBSaveClearStatus();
#endif //
    }
#endif
    // servo_int (10:1)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_SERVO )
    {
// Brian0.67
#ifdef SUPPORT_SERVO_SOURCE
#ifndef NO_SERVO
        ISR_SRVSaveClearStatus();   // Andy: for servo
#endif  //#ifndef NO_SERVO
#endif
    }

#ifdef DUMP_IN_FORCE_STOP
    else if(dwReqDetPerBlock)
    {
        ISR_SRVSaveClearStatus1();
    }
#endif

    // irint (10:2)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_IR )
    {
#ifndef NO_IR
        ISR_IRSaveClearStatus( );
#endif //#ifndef NO_IR
    }
    // atapi_int (10:3) / stb_underflow (10:4)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_STBBUF_UNDERFLOW )
    {
    }
    // biu_int (10:4)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_BIU )
    {
#ifndef NO_PARSER
        ISR_BIUSaveClearStatus( );
#endif  //#ifndef NO_PARSER
    }
    // bsrdint (10:5) and abufint_overflow (10:11)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & (INT_PROC1_2ND_MCU_BSRD|INT_PROC1_2ND_ABUF_OVERFLOW) )
    {
#ifndef NO_PARSER
        ISR_BSRD_ABUFSaveClearStatus( );
#endif  //#ifndef NO_PARSER
    }
    // eccrdint (10:6)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_MCU_ECCRD )
    {
    }
    // edcrdint (10:7)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_MCU_EDCRD )
    {
    }
    // osdrdint (10:8)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_MCU_OSDRD )
    {
    }
    // vbufint_overflow (10:9)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_VBUF_OVERFLOW )
    {
#ifndef NO_DECODER
        ISR_VBUF_OF_SaveClearStatus( );
#endif  //#ifndef NO_DECODER
    }
    // vbufint_underflow (10:10)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_VBUF_UNDERFLOW )
    {
#ifndef NO_DECODER
        ISR_VBUF_UF_SaveClearStatus( );
#endif  //#ifndef NO_DECODER
    }
    // sp1bufint_overflow (10:12)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_SP1BUF_OVERFLOW )
    {
    }
    // sp2bufint_overflow (10:13)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_SP2BUF_OVERFLOW )
    {
    }
    // stbbufint_overflow (10:14)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_STBBUF_OVERFLOW )
    {
    }
    // int_osd_underflow (10:15)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_OSD_UNDERFLOW )
    {
    }
    // int_osd_error (10:16)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_OSD_ERR )
    {
    }
    // int_main_under (10:17)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_MAIN_UNDER )
    {
    }
    // int_sp (10:18)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_SP_ERR )
    {
    }
    // pla_err (10:19)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_PLA_ERR )
    {
    }
    // wrprotint (10:20)
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_MCU_WR_PROTECTION )
    {
    }
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_VPU )
    {
    }
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_ERR_CNT )
    {
    }
    if( __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & INT_PROC1_2ND_PANEL_IR )
    {
    }


    // clear secondary mask pendding interrupts
    REG_PLAT_PROC1_2ND_INT_CLEAR = __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING;

#ifdef INT_STAT
    dwLoop = 0;
    while (__INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING != 0)
    {
        __dwINT2_Stat[dwLoop] += __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING & 0x1;
        dwLoop++;
        __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING >>= 1;
    }
#endif

    // clear fisrt mask pendding interrupt 10
    REG_PLAT_INT_CLEAR = (1 << INT_NO_PROC1_2ND);

    // Tell the processor that we have received
    // the interrupt.
    // cyg_interrupt_acknowledge( vector );

    if( __dwINTISRCounter[INT_NO10] )
    {
        // process ISR routines one by one
        for( dwLoop = 0; dwLoop < __dwINTISRCounter[INT_NO10]; dwLoop ++ )
        {
            (*(__pINTISRRoutine[INT_NO10][dwLoop]))( );
        }
    }

#ifdef MEASURE_INT_TIME
    dwEndTime = REG_PLAT_TIMER2_COUNTER;
    __dwINT_Time_ISR[INT_NO10] = dwStartTime - dwEndTime;
    __dwINT_PENDING[INT_NO10] = __INTISRintProc12ndStatus.dwPLAT_PROC1_2ND_INT_PENDING;    
    if (dwEndTime > dwStartTime)
    {
        __dwINT_Time_ISR[INT_NO10] += 0xFFFFFF;
    }

    if (__dwINT_MaxTime_ISR[INT_NO10] < __dwINT_Time_ISR[INT_NO10])
    {
        __dwINT_MaxTime_ISR[INT_NO10] = __dwINT_Time_ISR[INT_NO10];
        __dwINT_MaxTime_PENDING[INT_NO10] = __dwINT_PENDING[INT_NO10];
    }
#endif

    // Tell the kernel that chained interrupt processing
    // is done and the DSR needs to be executed next.
    if( __dwINTDSRCounter[INT_NO10] )
    {
        // Block this interrupt from occurring until
        // the DSR completes.
        cyg_interrupt_mask( vector );

        return( CYG_ISR_HANDLED | CYG_ISR_CALL_DSR );
    }
    else
    {
        return( CYG_ISR_HANDLED );      // if no DSR needed to be executed
    }
}

//
// Deferred service routine for interrupt 10.
//
void    INT_Proc1_2nd_dsr(
        cyg_vector_t vector,
        cyg_ucount32 count,
        cyg_addrword_t data)
{
    DWORD   dwLoop;

#ifdef MEASURE_INT_TIME
    DWORD   dwStartTime;
    DWORD   dwEndTime;
    DWORD   dwTotalTime;

    dwStartTime = REG_PLAT_TIMER2_COUNTER;
#endif

    // process DSR routines one by one
    for( dwLoop = 0; dwLoop < __dwINTDSRCounter[INT_NO10]; dwLoop ++ )
    {
        (*(__pINTDSRRoutine[INT_NO10][dwLoop]))( );
    }

#ifdef MEASURE_INT_TIME
    dwEndTime = REG_PLAT_TIMER2_COUNTER;
    dwTotalTime = dwStartTime - dwEndTime;
    if (dwEndTime > dwStartTime)
    {
        dwTotalTime += 0xFFFFFF;
    }

    if (__dwINT_MaxTime_DSR[INT_NO10] < dwTotalTime)
    {
        __dwINT_MaxTime_DSR[INT_NO10] = dwTotalTime;
    }
    
    if (__dwINT_MaxTotalTime[INT_NO10] < (dwTotalTime + __dwINT_Time_ISR[INT_NO10]))
    {
        __dwINT_MaxTotalTime[INT_NO10] = (dwTotalTime + __dwINT_Time_ISR[INT_NO10]);
        __dwINT_MaxTotalTime_PENDING[INT_NO10] = __dwINT_PENDING[INT_NO10];
    }    

    if (__dwTotalCnt[INT_NO10] < 10000)
    {
        __wINT_TotalTime2[__dwTotalCnt[INT_NO10]++] = (dwTotalTime + __dwINT_Time_ISR[INT_NO10]);
    }
#endif
    // Allow this interrupt to occur again.
    cyg_interrupt_unmask( vector );
}

cyg_uint32  INT_Proc1_SW_isr(
        cyg_vector_t vector,
        cyg_addrword_t data)
{
    MACRO_PLAT_KEY_LOCK( );
    __INTISRintProc1SWStatus.dwPLAT_PARAMETER3 = REG_PLAT_PARAMETER3;
    REG_PLAT_PARAMETER3 = PLAT_CMDID_NULL;
    MACRO_PLAT_KEY_UNLOCK( );

    // Block this interrupt from occurring until
    // the DSR completes.
    cyg_interrupt_mask( vector );

    // Tell the processor that we have received
    // the interrupt.
    // cyg_interrupt_acknowledge( vector );

    return( CYG_ISR_HANDLED | CYG_ISR_CALL_DSR );
}

//
// Deferred service routine for interrupt 11 (software interrupt).
//
void    INT_Proc1_SW_dsr(
        cyg_vector_t vector,
        cyg_ucount32 count,
        cyg_addrword_t data)
{
#ifndef AP_FRAMEWORK
    printf("\nSW_INT Issue: %lx", __INTISRintProc1SWStatus.dwPLAT_PARAMETER3);
    // Chuan0.68, PROC2 will interrupt PROC1 when STC Updated
    if (__INTISRintProc1SWStatus.dwPLAT_PARAMETER3 == PLAT_CMDID_STCUPDATED)
    {
        // LLY1.02, unfreeze the STC if CDROM motion w/ skip video stream mode
        // Since, it's audio master mode, must unfreeze STC, then DSP update can be done.
        if( (__bAttrPlay&TYPE_CDROM_AV) &&(__bVideoID==HAL_THROW_VIDEO) )
        {
            DISP_STCFreeze(0, DISP_UNFREEZE_STC);
        }
        __dwNewSTCStatus |= DISP_AUDIO_STC_UPDATED;
        __dwSTCUpdated = TRUE;
        __SPUCtrl.bEnable |= SPCTRL_REALPTS; // Enable SP Decoder before update STC.
#ifdef DEBUG_AVSYNC
        DBG_Printf(DBG_DSR, DBG_INFO_PRINTF, "AUDIO: STC Updated!!(%lx), VPTS: %lx", REG_PLAT_TIMER3_VALUE, __dwDISP_PTS);
#endif
    }
#endif  //#ifndef AP_FRAMEWORK

    // Allow this interrupt to occur again.
    cyg_interrupt_unmask( vector );
}

#ifdef EMU_RTC
//Aron2.77, added for DMP SW-RTC
cyg_uint32  INT_Timer2_isr(
        cyg_vector_t vector,
        cyg_addrword_t data)
{
    cyg_interrupt_mask( vector );
    return( CYG_ISR_HANDLED | CYG_ISR_CALL_DSR );
 }

void   INT_Timer2_dsr(
        cyg_vector_t vector,
        cyg_ucount32 count,
        cyg_addrword_t data)
{
    extern ULONGLONG __u64SystemTick2;
    __u64SystemTick2 += 1;
    cyg_interrupt_unmask( vector );    
}
#endif

void    INT_Init( void )
{
#ifdef MEASURE_INT_TIME
    REG_PLAT_TIMER2_RELOAD = 0x00ffffff;
    REG_PLAT_TIMER2_CONTROL = 0x3;
#endif
    
    //
    // Create interrupt 13.
    //
    cyg_interrupt_create(
            CYGNUM_HAL_INTERRUPT_13,
            CYGNUM_HAL_PRI_HIGH,
            0,
            &INT_Proc1_1st_isr,
            &INT_Proc1_1st_dsr,
            &_INTPROC11stHandle,
            &_INTPROC11st);

    // Attach the interrupt created to the vector.
    cyg_interrupt_attach( _INTPROC11stHandle );

    //
    // Create interrupt 11.
    //
    cyg_interrupt_create(
            CYGNUM_HAL_INTERRUPT_11,
            CYGNUM_HAL_PRI_HIGH,
            0,
            &INT_Proc1_SW_isr,
            &INT_Proc1_SW_dsr,
            &_INTPROC1SWHandle,
            &_INTPROC1SW);

    // Attach the interrupt created to the vector.
    cyg_interrupt_attach( _INTPROC1SWHandle );

    //
    // Create interrupt 10.
    //
    cyg_interrupt_create(
            CYGNUM_HAL_INTERRUPT_10,
            CYGNUM_HAL_PRI_HIGH,
            0,
            &INT_Proc1_2nd_isr,
            &INT_Proc1_2nd_dsr,
            &_INTPROC12ndHandle,
            &_INTPROC12nd);

    // Attach the interrupt created to the vector.
    cyg_interrupt_attach( _INTPROC12ndHandle );

#ifdef EMU_RTC
//Aron2.77, added for DMP SW-RTC
    cyg_interrupt_create(
            CYGNUM_HAL_INTERRUPT_9,
            CYGNUM_HAL_PRI_HIGH,
            0,
            &INT_Timer2_isr,
            &INT_Timer2_dsr,
            &__INTTIMER2Handle,
            &__INTTIMER2);
    
    cyg_interrupt_attach( __INTTIMER2Handle );
    cyg_interrupt_unmask( CYGNUM_HAL_INTERRUPT_9 );            
#endif    
    
    // Unmask the interrupt we just configured.
    cyg_interrupt_unmask( CYGNUM_HAL_INTERRUPT_13 );
    cyg_interrupt_unmask( CYGNUM_HAL_INTERRUPT_11 );
    cyg_interrupt_unmask( CYGNUM_HAL_INTERRUPT_10 );
}

VOID    INT_VSYNC_End(VOID)
{
#ifdef SUPPORT_STB
{
    extern VOID DVB_TTX_Vsync(VOID);
    DVB_TTX_Vsync();
}
#endif
}
