// =================================================================================================
#ifndef __CTKAV_PLATFORM_H__
#define __CTKAV_PLATFORM_H__
// =================================================================================================
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
// =================================================================================================
// define PROC's on-chip registers
// H/W will remap following PROC2's addresses and let DSUMON (GRMON) work as normal
// -- DSU UART, CACHE CONTROL, POWER DOWN, CONFIGURATION
// -- DSU's internal registers from 0x90000000
// This makes PROC2 can not see the same register addresses for PROC1.
// =================================================================================================
#define IO_START                        (CT909_IO_START)
// =================================================================================================
#define REG_PLATFORM_ON_CHIP_BASE       (IO_START)                                                  // 80000000
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_AHB_FAILING_ADDR       (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x00c)))   // 000c
#define REG_PLAT_AHB_STATUS             (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x010)))   // 0010
#define REG_PLAT_CACHE_CONTROL          (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x014)))   // 0014
#define REG_PLAT_POWER_DOWN             (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x018)))   // 0018
#define REG_PLAT_WRITE_PROTECTION1      (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x01c)))   // 001c
#define REG_PLAT_WRITE_PROTECTION2      (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x020)))   // 0020
#define REG_PLAT_CONFIGURATION          (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x024)))   // 0024
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_TIMER1_COUNTER         (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x040)))   // 0040
#define REG_PLAT_TIMER1_RELOAD          (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x044)))   // 0044
#define REG_PLAT_TIMER1_CONTROL         (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x048)))   // 0048
#define REG_PLAT_WATCHDOG               (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x04c)))   // 004c
#define REG_PLAT_TIMER2_COUNTER         (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x050)))   // 0050
#define REG_PLAT_TIMER2_RELOAD          (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x054)))   // 0054
#define REG_PLAT_TIMER2_CONTROL         (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x058)))   // 0058
#define REG_PLAT_PRESCALER_COUNTER      (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x060)))   // 0060
#define REG_PLAT_PRESCALER_RELOAD       (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x064)))   // 0064
#define REG_PLAT_TIMER3_CONTROL         (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x068)))   // 0068
#define REG_PLAT_TIMER3_VALUE           (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x06c)))   // 006c
#define REG_PLAT_TIMER3_VALUE_ADDR      ((DWORD)(&REG_PLAT_TIMER3_VALUE))                           // =006c
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_UART1_DATA             (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x070)))   // 0070
#define REG_PLAT_UART1_STATUS           (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x074)))   // 0074
#define REG_PLAT_UART1_CONTROL          (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x078)))   // 0078
#define REG_PLAT_UART1_SCALER           (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x07c)))   // 007c
#define REG_PLAT_UART2_DATA             (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x080)))   // 0080
#define REG_PLAT_UART2_STATUS           (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x084)))   // 0084
#define REG_PLAT_UART2_CONTROL          (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x088)))   // 0088
#define REG_PLAT_UART2_SCALER           (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x08c)))   // 008c
#define REG_PLAT_UART_DATA(n)           (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x070+((n) << 4))))  // 0070 or 0080
#define REG_PLAT_UART_STATUS(n)         (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x074+((n) << 4))))  // 0074 or 0084
#define REG_PLAT_UART_CONTROL(n)        (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x078+((n) << 4))))  // 0078 or 0088
#define REG_PLAT_UART_SCALER(n)         (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x07c+((n) << 4))))  // 007c or 008c
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_INT_MASK_PRIORITY      (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x090)))   // 0090
#define REG_PLAT_INT_PENDING            (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x094)))   // 0094
#define REG_PLAT_INT_FORCE              (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x098)))   // 0098
#define REG_PLAT_INT_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x09c)))   // 009c
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_PROC1_1ST_INT_MASK_ENABLE      (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0b0)))   // 00b0 (RW)
#define REG_PLAT_PROC1_1ST_INT_MASK_DISABLE     (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0bc)))   // 00bc (W)
#define REG_PLAT_PROC1_1ST_INT_PENDING_ENABLE   REG_PLAT_PROC1_1ST_INT_PENDING  // (RW)
#define REG_PLAT_PROC1_1ST_INT_PENDING_DISABLE  REG_PLAT_PROC1_1ST_INT_CLEAR    // (W)
#define REG_PLAT_PROC1_1ST_INT_PENDING  (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0b4)))   // 00b4 (RW)
#define REG_PLAT_PROC1_1ST_INT_STATUS   (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0b8)))   // 00b8 (R)
#define REG_PLAT_PROC1_1ST_INT_CLEAR    (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0b8)))   // 00b8 (W)
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_DSU_UART_STATUS        (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0c4)))   // 00c4
#define REG_PLAT_DSU_UART_CONTROL       (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0c8)))   // 00c8
#define REG_PLAT_DSU_UART_SCALER        (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0cc)))   // 00cc
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_PROC1_2ND_INT_MASK_ENABLE      (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0d0)))   // 00d0 (RW)
#define REG_PLAT_PROC1_2ND_INT_MASK_DISABLE     (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0dc)))   // 00dc (W)
#define REG_PLAT_PROC1_2ND_INT_PENDING_ENABLE   REG_PLAT_PROC1_2ND_INT_PENDING  // (RW)
#define REG_PLAT_PROC1_2ND_INT_PENDING_DISABLE  REG_PLAT_PROC1_2ND_INT_CLEAR    // (W)
#define REG_PLAT_PROC1_2ND_INT_PENDING  (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0d4)))   // 00d4 (RW)
#define REG_PLAT_PROC1_2ND_INT_STATUS   (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0d8)))   // 00d8 (R)
#define REG_PLAT_PROC1_2ND_INT_CLEAR    (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x0d8)))   // 00d8 (W)
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_PROC2_CACHE_CONTROL    (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x114)))   // 0114
#define REG_PLAT_PROC2_POWER_DOWN       (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x118)))   // 0118
#define REG_PLAT_PROC2_CONFIGURATION    (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x124)))   // 0124
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_PROC2_INT_MASK_PRIORITY (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x190)))  // 0190
#define REG_PLAT_PROC2_INT_PENDING      (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x194)))   // 0194
#define REG_PLAT_PROC2_INT_FORCE        (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x198)))   // 0198
#define REG_PLAT_PROC2_INT_CLEAR        (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x19c)))   // 019c
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_PROC2_1ST_INT_MASK_ENABLE      (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x1b0)))   // 01b0 (RW)
#define REG_PLAT_PROC2_1ST_INT_MASK_DISABLE     (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x1bc)))   // 01bc (W)
#define REG_PLAT_PROC2_1ST_INT_PENDING_ENABLE   REG_PLAT_PROC2_1ST_INT_PENDING  // (RW)
#define REG_PLAT_PROC2_1ST_INT_PENDING_DISABLE  REG_PLAT_PROC2_1ST_INT_CLEAR    // (W)
#define REG_PLAT_PROC2_1ST_INT_PENDING          (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x1b4)))   // 01b4 (RW)
#define REG_PLAT_PROC2_1ST_INT_STATUS           (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x1b8)))   // 01b8 (R)
#define REG_PLAT_PROC2_1ST_INT_CLEAR            (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x1b8)))   // 01b8 (W)
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_PROC2_DSU_UART_STATUS  (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x1c4)))   // 01c4
#define REG_PLAT_PROC2_DSU_UART_CONTROL (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x1c8)))   // 01c8
#define REG_PLAT_PROC2_DSU_UART_SCALER  (*((volatile DWORD *) (REG_PLATFORM_ON_CHIP_BASE+0x1cc)))   // 01cc
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_PROC2_PC               (REG_PLAT_DSU2_PC)
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// define timer bits
#define TIMER_ENABLE                    (0x00000001)
#define TIMER_RELOAD_COUNTER            (0x00000002)
#define TIMER_LOAD_COUNTER              (0x00000004)
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// define interrupt bits
#define INT_AHB_ERROR                   (0x00000002)
#define INT_UART2                       (0x00000004)
#define INT_UART1                       (0x00000008)
#define INT_TIMER1                      (0x00000100)
#define INT_TIMER2                      (0x00000200)
#define INT_PROC1_2ND                   (0x00000400)
#define INT_SOFTWARE                    (0x00000800)
#define INT_PROC1_1ST                   (0x00002000)
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
    #define INT_TIMER_SPORT                 (0x00000040)  // 909P/909G: renum from 0x1000
    #define INT_PROC2_1ST                   (0x00000080)  // 909P/909G: renum from 0x4000
    #define INT_IR                          (0x00001000)  // 909P/909G: new
#else
    #define INT_GPIO1                       (0x00000010)
    #define INT_GPIO2                       (0x00000020)
    #define INT_GPIO3                       (0x00000040)
    #define INT_TIMER_SPORT                 (0x00001000)
    #define INT_PROC2_1ST                   (0x00004000)
#endif

// define interrupt number
#define INT_NO_AHB_ERROR                (0x00000001)
#define INT_NO_UART2                    (0x00000002)
#define INT_NO_UART1                    (0x00000003)
#define INT_NO_TIMER1                   (0x00000008)
#define INT_NO_TIMER2                   (0x00000009)
#define INT_NO_PROC1_2ND                (0x0000000a)
#define INT_NO_SOFTWARE                 (0x0000000b)
#define INT_NO_PROC1_1ST                (0x0000000d)
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
    #define INT_NO_TIMER_SPORT              (0x00000006)  // 909P/909G: renum from 0xc
    #define INT_NO_PROC2_1ST                (0x00000007)  // 909P/909G: renum from 0xe
    #define INT_NO_IR                       (0x0000000c)  // 909P/909G: new
#else
    #define INT_NO_GPIO1                    (0x00000004)
    #define INT_NO_GPIO2                    (0x00000005)
    #define INT_NO_GPIO3                    (0x00000006)
    #define INT_NO_TIMER_SPORT              (0x0000000c)
    #define INT_NO_PROC2_1ST                (0x0000000e)
#endif
// -------------------------------------------------------------------------------------------------
// define interrupt bit for INT_PROC1_1ST
#define INT_PROC1_1ST_VSYNC             (0x00000001)
#define INT_PROC1_1ST_HSYNC             (0x00000002)
#define INT_PROC1_1ST_SCREEN_END        (0x00000004)
#define INT_PROC1_1ST_MAIN_END          (0x00000008)
#define INT_PROC1_1ST_OSD_END           (0x00000010)
#define INT_PROC1_1ST_RL_DONE           (0x00000020)
#define INT_PROC1_1ST_MC_DONE           (0x00000040)
#define INT_PROC1_1ST_INT_16L           (0x00000080)
// -------------------------------------------------------------------------------------------------
// define interrupt bit for INT_PROC1_2ND
#ifndef CT909G_IC_SYSTEM
    #define INT_PROC1_2ND_USB_OHCI          (0x00000001)    // 909G: del
#endif
#define INT_PROC1_2ND_SERVO             (0x00000002)
#define INT_PROC1_2ND_IR                (0x00000004)
#define INT_PROC1_2ND_STBBUF_UNDERFLOW  (0x00000008)
#define INT_PROC1_2ND_BIU               (0x00000010)
#define INT_PROC1_2ND_MCU_BSRD          (0x00000020)
#define INT_PROC1_2ND_MCU_ECCRD         (0x00000040)
#define INT_PROC1_2ND_MCU_EDCRD         (0x00000080)
#define INT_PROC1_2ND_MCU_OSDRD         (0x00000100)
#define INT_PROC1_2ND_VBUF_OVERFLOW     (0x00000200)
#define INT_PROC1_2ND_VBUF_UNDERFLOW    (0x00000400)
#define INT_PROC1_2ND_ABUF_OVERFLOW     (0x00000800)
#define INT_PROC1_2ND_SP1BUF_OVERFLOW   (0x00001000)
#define INT_PROC1_2ND_SP2BUF_OVERFLOW   (0x00002000)
#define INT_PROC1_2ND_STBBUF_OVERFLOW   (0x00004000)
#define INT_PROC1_2ND_OSD_UNDERFLOW     (0x00008000)
#define INT_PROC1_2ND_OSD_ERR           (0x00010000)
#define INT_PROC1_2ND_MAIN_UNDER        (0x00020000)
#define INT_PROC1_2ND_SP_ERR            (0x00040000)
#define INT_PROC1_2ND_PLA_ERR           (0x00080000)
#define INT_PROC1_2ND_MCU_WR_PROTECTION (0x00100000)
#define INT_PROC1_2ND_VPU               (0x00200000)
#define INT_PROC1_2ND_ERR_CNT           (0x00400000)
#define INT_PROC1_2ND_PANEL_IR          (0x00800000)
#ifdef CT909P_IC_SYSTEM
    #define INT_PROC1_2ND_FCR               (0x01000000)  // 909P: new
    #define INT_PROC1_2ND_NFC               (0x02000000)  // 909P: new
#endif
// -------------------------------------------------------------------------------------------------
// define interrupt bit for INT_PROC2_1ST
#define INT_PROC2_1ST_MIC_UNDERFLOW         (0x00000100)
#define INT_PROC2_1ST_ABUF_UNPK0_UNDERFLOW  (0x00000200)
#define INT_PROC2_1ST_ABUF_UNPK1_UNDERFLOW  (0x00000400)
#define INT_PROC2_1ST_PCM_UNDERFLOW         (0x00000800)
#define INT_PROC2_1ST_SPDIFOUT_UNDERFLOW    (0x00001000)
#define INT_PROC2_1ST_PCM0_UNDERFLOW        (0x00002000)
#define INT_PROC2_1ST_CLB                   (0x00004000)
#define INT_PROC2_1ST_MCU_WR_PROTECTION     (0x00008000)
// -------------------------------------------------------------------------------------------------
// define INT default
#define INT_SET_ALL                     (0xffffffff)
#define INT_CLEAR_ALL                   (0x00000000)
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// define cache control bits
#define CACHE_ICACHE_ENABLE             (0x00000003)
#define CACHE_DCACHE_ENABLE             (0x0000000c)
#define CACHE_FLUSH_DCACHE_PENDING      (0x00004000)
#define CACHE_FLUSH_ICACHE_PENDING      (0x00008000)
#define CACHE_INSTRUCTION_BURST_FETCH   (0x00010000)
#define CACHE_DATA_BURST_FETCH          (0x00020000)
#define CACHE_DCACHE_POWER_SAVING       (0x00040000)
#define CACHE_FLUSH_ICACHE              (0x00200000)
#define CACHE_FLUSH_DCACHE              (0x00400000)
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// define UART control bits
#define UART_RX_ENABLE                  (0x00000001)
#define UART_TX_ENABLE                  (0x00000002)
#define UART_RX_INT_ENABLE              (0x00000004)
#define UART_TX_INT_ENABLE              (0x00000008)
#define UART_ODD_PARITY                 (0x00000010)
#define UART_PARITY_ENABLE              (0x00000020)
#define UART_FLOW_CONTROL               (0x00000040)
#define UART_LOOP_BACK                  (0x00000080)
// -------------------------------------------------------------------------------------------------
// define UART status bits
#define UART_STATUS_DATA_READY          (0x00000001)
#define UART_STATUS_TS_EMPTY            (0x00000002)    // Transmitter shift (TS) register empty
#define UART_STATUS_TH_EMPTY            (0x00000004)    // Transmitter hold (TH) register empty
#define UART_STATUS_BREAK_RECEIVE       (0x00000008)
#define UART_STATUS_OVERRUN             (0x00000010)
#define UART_STATUS_PARITY_ERROR        (0x00000020)
#define UART_STATUS_FRAME_ERROR         (0x00000040)
// -------------------------------------------------------------------------------------------------
typedef struct  tagUART_STRU
{
    DWORD   dwControl;
    DWORD   dwScaler;
} UART_STRU, *PUART_STRU;
// =================================================================================================
// define Clock Generator & Reset Controller registers
// -------------------------------------------------------------------------------------------------
#define REG_PLATFORM_RESET_CLOCK_BASE   (IO_START+0x300)                                                  // 80000300
// -------------------------------------------------------------------------------------------------
// define Clock Generator & Reset Controller registers
#define REG_PLAT_CLK_GENERATOR_CONTROL  (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x00)))  // 0300
#define REG_PLAT_RESET_CONTROL_DISABLE  (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x04)))  // 0304
#define REG_PLAT_CLK_FREQ_CONTROL1      (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x08)))  // 0308
#define REG_PLAT_MPLL_CONTROL           (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x10)))  // 0310
#define REG_PLAT_APLL_CONTROL           (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x14)))  // 0314
#ifndef CT909G_IC_SYSTEM
    #define REG_PLAT_UPLL_CONTROL           (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x18)))  // 0318 (909G: del)
#endif
#define REG_PLAT_SYSTEM_CONFIGURATION1  (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x1c)))  // 031c
#define REG_PLAT_SYSTEM_CONFIGURATION2  (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x20)))  // 0320
#define REG_PLAT_RESET_CONTROL_ENABLE   (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x24)))  // 0324
#ifdef CT909G_IC_SYSTEM
    #define REG_PLAT_SYS_PIN_USE0           (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x28)))  // 0328 (909G: new)
    #define REG_PLAT_SYS_PIN_USE1           (*((volatile DWORD *) (REG_PLATFORM_RESET_CLOCK_BASE+0x2c)))  // 032c (909G: new)
#endif
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// define CLOCK Generator Gating Control bits for REG_PLAT_CLK_GENERATOR_CONTROL
#define PLAT_MCLK_PROC2_DISABLE     (0x00000001)
#define PLAT_MCLK2_AIU_DISABLE      (0x00000002)
#define PLAT_MCLK2_BIU_DISABLE      (0x00000004)
#define PLAT_MCLK_VDEC_DISABLE      (0x00000008)
#define PLAT_MCLK2_VDEC_DISABLE     (0x00000010)
#define PLAT_MCLK_DSU_DISABLE       (0x00000020)
#define PLAT_MCLK2_SERVO_DISABLE    (0x00000040)
#define PLAT_DSSPCLK_DISABLE        (0x00000080)
#define PLAT_DCLK_DISABLE           (0x00000100)
#define PLAT_DVDCLK_DISABLE         (0x00000200)
#define PLAT_CDCLK_DISABLE          (0x00000400)
#define PLAT_MCLK2_VOU_DISABLE      (0x00000800)
#define PLAT_CLK27M_IR_DISABLE      (0x00001000)
#ifndef CT909G_IC_SYSTEM
    #define PLAT_MCLK2_DVDIF_DISABLE    (0x00002000)    // 909G: del
#endif
#define PLAT_MCLK2_VPU_DISABLE      (0x00004000)
#define PLAT_MCLK_PROM_DISABLE      (0x00008000)
#ifndef CT909G_IC_SYSTEM
    #define PLAT_MCLK_ACP_DISABLE       (0x00010000)    // 909G: del
#endif
#define PLAT_TCLK_DISABLE           (0x00020000)
#define PLAT_ACLK_DAC_DISABLE       (0x00040000)
#define PLAT_BCLK_DAC_DISABLE       (0x00080000)
#define PLAT_ACLK_ADC_DISABLE       (0x00100000)
#define PLAT_BCLK_ADC_DISABLE       (0x00200000)
#define PLAT_ACLK2_DAC_DISABLE      (0x00400000)
#ifndef CT909G_IC_SYSTEM
    #define PLAT_HCLK_USB_DISABLE       (0x00800000)    // 909G: del
    #define PLAT_UCLK48M_USB_DISABLE    (0x01000000)    // 909G: del
#endif
#define PLAT_MCLK_VOU_DISABLE       (0x02000000)
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// define PLATFORM Reset Control Disable bits for REG_PLAT_RESET_CONTROL_DISABLE
#define PLAT_RESET_PROC2_DISABLE        (0x00000001)
#define PLAT_RESET_AIU_DISABLE          (0x00000002)
#define PLAT_RESET_BIU_DISABLE          (0x00000004)
#define PLAT_RESET_UART1_DISABLE        (0x00000008)
#define PLAT_RESET_UART2_DISABLE        (0x00000010)
#define PLAT_RESET_IRQ1_DISABLE         (0x00000020)
#define PLAT_RESET_IRQ2_DISABLE         (0x00000040)
#define PLAT_RESET_TIMER_DISABLE        (0x00000080)
#define PLAT_RESET_VDEC_DISABLE         (0x00000100)
#define PLAT_RESET_VDEC2_DISABLE        (0x00000200)
#define PLAT_RESET_SERVO_DISABLE        (0x00000400)
#define PLAT_RESET_DSSP_DISABLE         (0x00000800)
#define PLAT_RESET_DVD_DISABLE          (0x00001000)
#define PLAT_RESET_CD_DISABLE           (0x00002000)
#define PLAT_RESET_ECC_DISABLE          (0x00004000)
#define PLAT_RESET_DPLL_DISABLE         (0x00008000)
#define PLAT_RESET_EDC_DISABLE          (0x00010000)
#define PLAT_RESET_RF_DISABLE           (0x00020000)
#define PLAT_RESET_DSU1_DISABLE         (0x00040000)
#define PLAT_RESET_DSU2_DISABLE         (0x00080000)
#define PLAT_RESET_VOU2_DISABLE         (0x00100000)
#define PLAT_RESET_IR_DISABLE           (0x00200000)
#ifndef CT909G_IC_SYSTEM
    #define PLAT_RESET_DVDIF_DISABLE        (0x00400000)    // 909G: del
#endif
#define PLAT_RESET_VPU_DISABLE          (0x00800000)
#define PLAT_RESET_PROM_DISABLE         (0x01000000)
#ifndef CT909G_IC_SYSTEM
    #define PLAT_RESET_ACP_DISABLE          (0x02000000)    // 909G: del
    #define PLAT_RESET_USB_DISABLE          (0x04000000)    // 909G: del
    #define PLAT_RESET_USBCLKCKT_DISABLE    (0x08000000)    // 909G: del
#endif
#define PLAT_RESET_VOU_DISABLE          (0x10000000)
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
    #define PLAT_RESET_AIU_DAC_DISABLE      (0x20000000)    // 909G: new
#endif
// -------------------------------------------------------------------------------------------------
// define PLATFORM Reset Control Enable bits for REG_PLAT_RESET_CONTROL_ENABLE
#define PLAT_RESET_PROC2_ENABLE         (0x00000001)
#define PLAT_RESET_AIU_ENABLE           (0x00000002)
#define PLAT_RESET_BIU_ENABLE           (0x00000004)
#define PLAT_RESET_UART1_ENABLE         (0x00000008)
#define PLAT_RESET_UART2_ENABLE         (0x00000010)
#define PLAT_RESET_IRQ1_ENABLE          (0x00000020)
#define PLAT_RESET_IRQ2_ENABLE          (0x00000040)
#define PLAT_RESET_TIMER_ENABLE         (0x00000080)
#define PLAT_RESET_VDEC_ENABLE          (0x00000100)
#define PLAT_RESET_VDEC2_ENABLE         (0x00000200)
#define PLAT_RESET_SERVO_ENABLE         (0x00000400)
#define PLAT_RESET_DSSP_ENABLE          (0x00000800)
#define PLAT_RESET_DVD_ENABLE           (0x00001000)
#define PLAT_RESET_CD_ENABLE            (0x00002000)
#define PLAT_RESET_ECC_ENABLE           (0x00004000)
#define PLAT_RESET_DPLL_ENABLE          (0x00008000)
#define PLAT_RESET_EDC_ENABLE           (0x00010000)
#define PLAT_RESET_RF_ENABLE            (0x00020000)
#define PLAT_RESET_DSU1_ENABLE          (0x00040000)
#define PLAT_RESET_DSU2_ENABLE          (0x00080000)
#define PLAT_RESET_VOU2_ENABLE          (0x00100000)
#define PLAT_RESET_IR_ENABLE            (0x00200000)
#ifndef CT909G_IC_SYSTEM
    #define PLAT_RESET_DVDIF_ENABLE         (0x00400000)    // 909G: del
#endif
#define PLAT_RESET_VPU_ENABLE           (0x00800000)
#define PLAT_RESET_PROM_ENABLE          (0x01000000)
#ifndef CT909G_IC_SYSTEM
    #define PLAT_RESET_ACP_ENABLE           (0x02000000)    // 909G: del
    #define PLAT_RESET_USB_ENABLE           (0x04000000)    // 909G: del
    #define PLAT_RESET_USBCLKCKT_ENABLE     (0x08000000)    // 909G: del
#endif
#define PLAT_RESET_VOU_ENABLE           (0x10000000)
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
    #define PLAT_RESET_AIU_DAC_ENABLE       (0x20000000)    // 909G: new
#endif
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// define System Configuration-1 bits for REG_PLAT_SYSTEM_CONFIGURATION1
#ifndef CT909G_IC_SYSTEM
    #define PLAT_SYSCFG1_DVDIF_EN           (0x00080000)    // 909G: del
    #define PLAT_SYSCFG1_NIM_SEL_GPB        (0x00002000)    // 909G: del
#endif
// =================================================================================================
// define GPIO registers
#ifdef CT909P_IC_SYSTEM
// -------------------------------------------------------------------------------------------------
    #define REG_PLATFORM_GPIO_BASE          (IO_START+0x4004)                                       // 80004004
// -------------------------------------------------------------------------------------------------
    #define REG_PLAT_GPA_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x00)))   // 4004 (909P)
    #define REG_PLAT_GPA_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x04)))   // 4008 (909P)
    #define REG_PLAT_GPA_IO_DIR_CONTROL     (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x08)))   // 400c (909P)
    #define REG_PLAT_GPC_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x0c)))   // 4010 (909P)
    #define REG_PLAT_GPC_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x10)))   // 4014 (909P)
    #define REG_PLAT_GPC_IO_DIR_CONTROL     (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x14)))   // 4018 (909P)
    #define REG_PLAT_GPD_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x18)))   // 401c (909P)
    #define REG_PLAT_GPD_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x1c)))   // 4020 (909P)
    #define REG_PLAT_GPD_IO_DIR_CONTROL     (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x20)))   // 4024 (909P)
    #define REG_PLAT_GPE_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x24)))   // 4028 (909P)
    #define REG_PLAT_GPE_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x28)))   // 402c (909P)
    #define REG_PLAT_GPE_IO_DIR_CONTROL     (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x2c)))   // 4030 (909P)
    #define REG_PLAT_GPF_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x30)))   // 4034 (909P)
    #define REG_PLAT_GPF_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x34)))   // 4038 (909P)
    #define REG_PLAT_GPF_IO_DIR_CONTROL     (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x38)))   // 403c (909P)
    #define REG_PLAT_GPG_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x3c)))   // 4040 (909P)
    #define REG_PLAT_GPG_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x40)))   // 4044 (909P)
    #define REG_PLAT_GPG_IO_DIR_CONTROL     (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x44)))   // 4048 (909P)
    #define REG_PLAT_GPH_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x48)))   // 404c (909P)
    #define REG_PLAT_GPH_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x4c)))   // 4050 (909P)
    #define REG_PLAT_GPH_IO_DIR_CONTROL     (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x50)))   // 4054 (909P)
    #define REG_PLAT_GPA_MUX                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x54)))   // 4058 (909P)
    #define REG_PLAT_GPB_MUX                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x58)))   // 405c (909P)
    #define REG_PLAT_GPC_MUX                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x5c)))   // 4060 (909P)
    #define REG_PLAT_GPD_MUX                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x60)))   // 4064 (909P)
    #define REG_PLAT_GPD_MUX_EXT            (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x64)))   // 4068 (909P)
    #define REG_PLAT_GPE_MUX                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x68)))   // 406c (909P)
    #define REG_PLAT_GPF_MUX                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x6c)))   // 4070 (909P)
    #define REG_PLAT_GPG_MUX                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x70)))   // 4074 (909P)
#else
// -------------------------------------------------------------------------------------------------
    #define REG_PLATFORM_GPIO_BASE          (IO_START+0x330)                                        // 80000330
// -------------------------------------------------------------------------------------------------
    #define REG_PLAT_GPA_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x00)))   // 0330
    #define REG_PLAT_GPA_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x04)))   // 0334
    #define REG_PLAT_GPA_IO_DIR_CONTROL     (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x08)))   // 0338
    #define REG_PLAT_GPIO_INT_CONFIGURATION (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x0c)))   // 033c
    #ifdef CT909G_IC_SYSTEM
        #define REG_PLAT_GPC_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x10)))   // 0340 (909G: new)
        #define REG_PLAT_GPC_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x14)))   // 0344 (909G: new)
        #define REG_PLAT_GPC_IO_DIR_CONTROL     (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x18)))   // 0348 (909G: new)
        #define REG_PLAT_SCART_PWM_CONFIGURATION (*((volatile DWORD *)(REG_PLATFORM_GPIO_BASE+0x1c)))   // 034c (909G: new)
        #define REG_PLAT_GPB_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x20)))   // 0350 (909G: new)
        #define REG_PLAT_GPB_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x24)))   // 0354 (909G: new)
        #define REG_PLAT_GPB_IO_DIR_CONTROL     (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x28)))   // 0358 (909G: new)
    #else
        #define REG_PLAT_GPCDE_CLEAR            (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x10)))   // 0340
        #define REG_PLAT_GPCDE_SET              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x14)))   // 0344
        #define REG_PLAT_GPCDE_IO_DIR_CONTROL   (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x18)))   // 0348
        #define REG_PLAT_GPF_CLEAR              (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x1c)))   // 034c
        #define REG_PLAT_GPF_SET                (*((volatile DWORD *) (REG_PLATFORM_GPIO_BASE+0x20)))   // 0350
    #endif
#endif
// =================================================================================================
// define Pad IO registers
#ifdef CT909G_IC_SYSTEM
// -------------------------------------------------------------------------------------------------
    #define REG_PLATFORM_PADIO_BASE         (IO_START+0x2980)                                           // 2980
// -------------------------------------------------------------------------------------------------
    #define REG_PLAT_MA_IEDS                (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x00)))      // 2980 (909G: new)
    #define REG_PLAT_MA_PEPS                (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x04)))      // 2984 (909G: new)
    #define REG_PLAT_MD_IEDS                (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x08)))      // 2988 (909G: new)
    #define REG_PLAT_MD_PEPS                (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x0c)))      // 298c (909G: new)
    #define REG_PLAT_MMISC_IEDS             (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x10)))      // 2990 (909G: new)
    #define REG_PLAT_MMISC_PEPS             (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x14)))      // 2994 (909G: new)
    #define REG_PLAT_GPA_IEDS               (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x18)))      // 2998 (909G: new)
    #define REG_PLAT_GPA_PEPS               (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x1c)))      // 299c (909G: new)
    #define REG_PLAT_GPB_IEDS               (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x20)))      // 29a0 (909G: new)
    #define REG_PLAT_GPB_PEPS               (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x24)))      // 29a4 (909G: new)
    #define REG_PLAT_GPC_IEDS               (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x28)))      // 29a8 (909G: new)
    #define REG_PLAT_GPC_PEPS               (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x2c)))      // 29ac (909G: new)
    #define REG_PLAT_MISC_PEPS              (*((volatile DWORD *) (REG_PLATFORM_PADIO_BASE+0x34)))      // 29b4 (909G: new)
#endif
// =================================================================================================
// define Mode Selection registers
#ifdef CT909P_IC_SYSTEM
// -------------------------------------------------------------------------------------------------
    #define REG_PLATFORM_MODE_SEL_BASE      (IO_START+0x4078)                                           // 80004078
// -------------------------------------------------------------------------------------------------
    #define REG_PLAT_VPLUS_MODE_SEL         (*((volatile DWORD *) (REG_PLATFORM_MODE_SEL_BASE+0x00)))   // 4078 (909P)
    #define REG_PLAT_CT909S_TEST_MODE_SEL   (*((volatile DWORD *) (REG_PLATFORM_MODE_SEL_BASE+0x04)))   // 407C (909P)
    #define REG_PLAT_DAC_MODE_SEL           (*((volatile DWORD *) (REG_PLATFORM_MODE_SEL_BASE+0x08)))   // 4080 (909P)
#endif
// =================================================================================================
// define Command registers
#define REG_PLATFORM_COMMAND_BASE       (IO_START+0x360)                                            // 80000360
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_COMMAND                (*((volatile DWORD *) (REG_PLATFORM_COMMAND_BASE+0x00)))    // 0360
#define REG_PLAT_COMMAND_ADDR           (REG_PLATFORM_COMMAND_BASE+0x00)
#define REG_PLAT_PARAMETER1             (*((volatile DWORD *) (REG_PLATFORM_COMMAND_BASE+0x04)))    // 0364
#define REG_PLAT_PARAMETER2             (*((volatile DWORD *) (REG_PLATFORM_COMMAND_BASE+0x08)))    // 0368
#define REG_PLAT_PARAMETER3             (*((volatile DWORD *) (REG_PLATFORM_COMMAND_BASE+0x0c)))    // 036c
#define REG_PLAT_PARAMETER4             (*((volatile DWORD *) (REG_PLATFORM_COMMAND_BASE+0x10)))    // 0370
// -------------------------------------------------------------------------------------------------
// REG_PLAT_PARAMETER3 [31:28]: ID. 1: PTS, 2: Skip, 3: STC Updated
// REG_PLAT_PARAMETER3 [27:24]: Loop Cnt: 0~7.
// REG_PLAT_PARAMETER3 [23:0]: Address. Ex 123456 indicate address(0x40123456)
// REG_PLAT_PARAMETER4 [31:0]: PTS Value
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#define PLAT_CMDID_NULL                 (0x00000000)
#define PLAT_CMDID_PTS                  (0x10000000)
#define PLAT_CMDID_SKIP                 (0x20000000)
#define PLAT_CMDID_STCUPDATED           (0x30000000)
#define PLAT_CMDID_STCUPDATED_VIDEO     (0x40000000)
// =================================================================================================
// define IR Control registers
// -------------------------------------------------------------------------------------------------
#define REG_PLATFORM_IR_BASE            (IO_START+0x380)                                      // 80000380
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_IR_TIMING_CONTROL1     (*((volatile DWORD *) (REG_PLATFORM_IR_BASE+0x00)))   // 0380
#define REG_PLAT_IR_TIMING_CONTROL2     (*((volatile DWORD *) (REG_PLATFORM_IR_BASE+0x04)))   // 0384
#define REG_PLAT_IR_TIMING_CONTROL3     (*((volatile DWORD *) (REG_PLATFORM_IR_BASE+0x08)))   // 0388
#define REG_PLAT_IR_TIMING_CONTROL4     (*((volatile DWORD *) (REG_PLATFORM_IR_BASE+0x0c)))   // 038c
#define REG_PLAT_IR_DATA                (*((volatile DWORD *) (REG_PLATFORM_IR_BASE+0x10)))   // 0390
#define REG_PLAT_IR_RAW_CODE            (*((volatile DWORD *) (REG_PLATFORM_IR_BASE+0x14)))   // 0394
#define REG_PLAT_IR_COUNTER             (*((volatile DWORD *) (REG_PLATFORM_IR_BASE+0x18)))   // 0398
#define REG_PLAT_IR_STATUS              (*((volatile DWORD *) (REG_PLATFORM_IR_BASE+0x1c)))   // 039c
// =================================================================================================
// define IC ID (Software version number) register. (ref. VPU Spec.)
#define REG_PLAT_IC_ID                  (*((volatile DWORD *) (IO_START+0x28c8)))             // 28c8
// =================================================================================================
// define PROM (Parallel/Serial Flash) registers
// -------------------------------------------------------------------------------------------------
#define REG_PLATFORM_PROM_BASE          (IO_START+0x2a00)                                           // 80002a00
// -------------------------------------------------------------------------------------------------
// define Parallel Flash Controller registers
#ifndef CT909G_IC_SYSTEM
    #define REG_PLAT_PROMCFG                (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x00)))   // 2a00 (909G: del)
    #ifndef CT909P_IC_SYSTEM
        #define REG_PLAT_KRAMADR                (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x04)))   // 2a04 (909G: del)
        #define REG_PLAT_KRAMDATA               (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x08)))   // 2a08 (909G: del)
        #define REG_PLAT_CRAMADR                (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x0c)))   // 2a0c (909G: del)
        #define REG_PLAT_CRAMDATAL              (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x10)))   // 2a10 (909G: del)
        #define REG_PLAT_CRAMDATAH              (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x14)))   // 2a14 (909G: del)
        #define REG_PLAT_CBADR                  (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x18)))   // 2a18 (909G: del)
        #define REG_PLAT_CBDATA                 (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x1c)))   // 2a1c (909G: del)
    #endif
#endif
// -------------------------------------------------------------------------------------------------
// define Serial (SPI) Flash Controller registers
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
    #define REG_PLAT_SPI_CMD                (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x24)))   // 2a24
    #define REG_PLAT_SPI_OP                 (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x28)))   // 2a28
    #define REG_PLAT_SPI_READ_TYPE          (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x2c)))   // 2a2c
    #define REG_PLAT_SPI_WRITE              (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x30)))   // 2a30
    #define REG_PLAT_SPI_READ               (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x34)))   // 2a34
    #define REG_PLAT_SPI_SCLK_CTRL          (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x38)))   // 2a38
#else
    #define REG_PLAT_SPI_CMD                (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x20)))   // 2a20
    #define REG_PLAT_SPI_OP                 (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x24)))   // 2a24
    #define REG_PLAT_SPI_READ_TYPE          (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x28)))   // 2a28
    #define REG_PLAT_SPI_WRITE              (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x2c)))   // 2a2c
    #define REG_PLAT_SPI_READ               (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x30)))   // 2a30
    #define REG_PLAT_SPI_SCLK_CTRL          (*((volatile DWORD *) (REG_PLATFORM_PROM_BASE+0x34)))   // 2a34
#endif
// =================================================================================================
// define register for External IO Config
#if !defined(CT909P_IC_SYSTEM) && !defined(CT909G_IC_SYSTEM)
    #define REG_PLAT_EXTIOCFG               (*((volatile DWORD *) (IO_START+0x2b00)))               // 2b00
#endif
// =================================================================================================
// define register for Write Protection
#ifdef CT909G_IC_SYSTEM
// -------------------------------------------------------------------------------------------------
    #define REG_PLATFORM_WPROT_BASE         (IO_START+0x2b00)                                       // 80002b00
// -------------------------------------------------------------------------------------------------
    #define REG_PLAT_PROC1_WPROT1_START     (*((volatile DWORD *) (REG_PLATFORM_WPROT_BASE+0x00)))  // 2b00 (909G: new)
    #define REG_PLAT_PROC1_WPROT1_END       (*((volatile DWORD *) (REG_PLATFORM_WPROT_BASE+0x04)))  // 2b04 (909G: new)
    #define REG_PLAT_PROC1_WPROT2_START     (*((volatile DWORD *) (REG_PLATFORM_WPROT_BASE+0x08)))  // 2b08 (909G: new)
    #define REG_PLAT_PROC1_WPROT2_END       (*((volatile DWORD *) (REG_PLATFORM_WPROT_BASE+0x0c)))  // 2b0c (909G: new)
    #define REG_PLAT_PROC2_WPROT1_START     (*((volatile DWORD *) (REG_PLATFORM_WPROT_BASE+0x10)))  // 2b10 (909G: new)
    #define REG_PLAT_PROC2_WPROT1_END       (*((volatile DWORD *) (REG_PLATFORM_WPROT_BASE+0x14)))  // 2b14 (909G: new)
    #define REG_PLAT_PROC2_WPROT2_START     (*((volatile DWORD *) (REG_PLATFORM_WPROT_BASE+0x18)))  // 2b18 (909G: new)
    #define REG_PLAT_PROC2_WPROT2_END       (*((volatile DWORD *) (REG_PLATFORM_WPROT_BASE+0x1c)))  // 2b1c (909G: new)
#endif
// =================================================================================================
// define register for Panel Key Scan
// -------------------------------------------------------------------------------------------------
#define REG_PLATFORM_PANEL_BASE         (IO_START+0x2c00)                                       // 80002c00
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_PANEL_CFG              (*((volatile DWORD *) (REG_PLATFORM_PANEL_BASE+0x00)))  // 2c00
#define REG_PLAT_PANEL_CLK              (*((volatile DWORD *) (REG_PLATFORM_PANEL_BASE+0x04)))  // 2c04
#define REG_PLAT_PANEL_DISP0            (*((volatile DWORD *) (REG_PLATFORM_PANEL_BASE+0x08)))  // 2c08
#define REG_PLAT_PANEL_KD               (*((volatile DWORD *) (REG_PLATFORM_PANEL_BASE+0x0c)))  // 2c0c
#define REG_PLAT_PANEL_INTCTRL          (*((volatile DWORD *) (REG_PLATFORM_PANEL_BASE+0x10)))  // 2c10
#define REG_PLAT_PANEL_INT              (*((volatile DWORD *) (REG_PLATFORM_PANEL_BASE+0x14)))  // 2c14
// =================================================================================================
// define DSU registers
// -------------------------------------------------------------------------------------------------
#define REG_PLATFORM_DSU_BASE           (0x90000000)                                                // 90000000
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_DSU_CONTROL            (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x00)))        // 90000000
#define REG_PLAT_DSU_TRACE_BUF_CONTROL  (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x04)))        // 90000004
#define REG_PLAT_DSU_TIME_TAG_COUNTER   (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x08)))        // 90000008
#define REG_PLAT_DSU_AHB_BREAK_ADDR1    (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x10)))        // 90000010
#define REG_PLAT_DSU_AHB_MASK1          (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x14)))        // 90000014
#define REG_PLAT_DSU_AHB_BREAK_ADDR2    (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x18)))        // 90000018
#define REG_PLAT_DSU_AHB_MASK2          (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x1c)))        // 9000001c
#define REG_PLAT_DSU_O(n, m)            (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x20000+(((DWORD)64*(n)+32+4*(m))&(64*8-1)))))
#define REG_PLAT_DSU_L(n, m)            (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x20000+(((DWORD)64*(n)+64+4*(m))&(64*8-1)))))
#define REG_PLAT_DSU_I(n, m)            (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x20000+(((DWORD)64*(n)+96+4*(m))&(64*8-1)))))
#define REG_PLAT_DSU_G(n)               (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x20200+4*(n))))// 90020200
#define REG_PLAT_DSU_Y                  (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80000)))     // 90080000
#define REG_PLAT_DSU_PSR                (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80004)))     // 90080004
#define REG_PLAT_DSU_WIM                (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80008)))     // 90080008
#define REG_PLAT_DSU_TBR                (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x8000c)))     // 9008000c
#define REG_PLAT_DSU_PC                 (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80010)))     // 90080010
#define REG_PLAT_DSU_NPC                (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80014)))     // 90080014
#define REG_PLAT_DSU_FSR                (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80018)))     // 90080018
#define REG_PLAT_DSU_TRAP               (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x8001c)))     // 9008001c
#define REG_PLAT_DSU_ASR16              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80040)))     // 90080040
#define REG_PLAT_DSU_ASR17              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80044)))     // 90080044
#define REG_PLAT_DSU_ASR18              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80048)))     // 90080048
#define REG_PLAT_DSU_ASR19              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x8004c)))     // 9008004c
#define REG_PLAT_DSU_ASR20              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80050)))     // 90080050
#define REG_PLAT_DSU_ASR21              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80054)))     // 90080054
#define REG_PLAT_DSU_ASR22              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80058)))     // 90080058
#define REG_PLAT_DSU_ASR23              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x8005c)))     // 9008005c
#define REG_PLAT_DSU_ASR24              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80060)))     // 90080060
#define REG_PLAT_DSU_ASR25              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80064)))     // 90080064
#define REG_PLAT_DSU_ASR26              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80068)))     // 90080068
#define REG_PLAT_DSU_ASR27              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x8006c)))     // 9008006c
#define REG_PLAT_DSU_ASR28              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80070)))     // 90080070
#define REG_PLAT_DSU_ASR29              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80074)))     // 90080074
#define REG_PLAT_DSU_ASR30              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x80078)))     // 90080078
#define REG_PLAT_DSU_ASR31              (*((volatile DWORD *) (REG_PLATFORM_DSU_BASE+0x8007c)))     // 9008007c
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// define DSU registers
// -------------------------------------------------------------------------------------------------
#define REG_PLATFORM_DSU2_BASE          (0x98000000)                                                // 98000000
// -------------------------------------------------------------------------------------------------
#define REG_PLAT_DSU2_CONTROL           (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x00)))       // 98000000
#define REG_PLAT_DSU2_TRACE_BUF_CONTROL (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x04)))       // 98000004
#define REG_PLAT_DSU2_TIME_TAG_COUNTER  (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x08)))       // 98000008
#define REG_PLAT_DSU2_AHB_BREAK_ADDR1   (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x10)))       // 98000010
#define REG_PLAT_DSU2_AHB_MASK1         (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x14)))       // 98000014
#define REG_PLAT_DSU2_AHB_BREAK_ADDR2   (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x18)))       // 98000018
#define REG_PLAT_DSU2_AHB_MASK2         (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x1c)))       // 9800001c
#define REG_PLAT_DSU2_O(n, m)           (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x20000+(((DWORD)64*(n)+32+4*(m))&(64*8-1)))))
#define REG_PLAT_DSU2_L(n, m)           (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x20000+(((DWORD)64*(n)+64+4*(m))&(64*8-1)))))
#define REG_PLAT_DSU2_I(n, m)           (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x20000+(((DWORD)64*(n)+96+4*(m))&(64*8-1)))))
#define REG_PLAT_DSU2_G(n)              (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x20200+4*(n))))// 90020200
#define REG_PLAT_DSU2_Y                 (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80000)))    // 98080000
#define REG_PLAT_DSU2_PSR               (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80004)))    // 98080004
#define REG_PLAT_DSU2_WIM               (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80008)))    // 98080008
#define REG_PLAT_DSU2_TBR               (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x8000c)))    // 9808000c
#define REG_PLAT_DSU2_PC                (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80010)))    // 98080010
#define REG_PLAT_DSU2_NPC               (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80014)))    // 98080014
#define REG_PLAT_DSU2_FSR               (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80018)))    // 98080018
#define REG_PLAT_DSU2_TRAP              (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x8001c)))    // 9808001c
#define REG_PLAT_DSU2_ASR16             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80040)))    // 98080040
#define REG_PLAT_DSU2_ASR17             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80044)))    // 98080044
#define REG_PLAT_DSU2_ASR18             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80048)))    // 98080048
#define REG_PLAT_DSU2_ASR19             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x8004c)))    // 9808004c
#define REG_PLAT_DSU2_ASR20             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80050)))    // 98080050
#define REG_PLAT_DSU2_ASR21             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80054)))    // 98080054
#define REG_PLAT_DSU2_ASR22             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80058)))    // 98080058
#define REG_PLAT_DSU2_ASR23             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x8005c)))    // 9808005c
#define REG_PLAT_DSU2_ASR24             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80060)))    // 98080060
#define REG_PLAT_DSU2_ASR25             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80064)))    // 98080064
#define REG_PLAT_DSU2_ASR26             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80068)))    // 98080068
#define REG_PLAT_DSU2_ASR27             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x8006c)))    // 9808006c
#define REG_PLAT_DSU2_ASR28             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80070)))    // 98080070
#define REG_PLAT_DSU2_ASR29             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80074)))    // 98080074
#define REG_PLAT_DSU2_ASR30             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x80078)))    // 98080078
#define REG_PLAT_DSU2_ASR31             (*((volatile DWORD *) (REG_PLATFORM_DSU2_BASE+0x8007c)))    // 9808007c
// =================================================================================================
// J500CSC_108, save code protection flag, text and rodata sections of PROC1 & PROC2,
// at following memory. Its mechanism is listed as follows.
// (1) Flash booting: BOOT code will update the correct setting to this address.
// (2) Its default setting is no protection at all, i.e. lower bits, 0-th and 1-th, are 0.
//     If the code is boot from memory, all code won't be protected.
// 0x40000005c is useless memory for trap window_overflow and its default value is
// 0x010000000, nop instruction.
// Here, we also treat PROC2 can be H/W reset if its code is protected.
// Joey: Because 0x4000005c is used by standard library (non-eCos), we change address from 0x4000005c to 0x4000008c,
//       and it's floating point exception.
#define PLAT_CODE_PROTECTION            (*((volatile DWORD *) (0x4000008c)))
#define PLAT_CODE_PROTECT_PROC1         (0x00000001)
#define PLAT_CODE_PROTECT_PROC2         (0x00000002)
#define SPARC_NOP_INSTRUCTION           (0x01000000)
// =================================================================================================
// define some global macros
#define PLAT_PROC1_DCACHE_DISABLE       (REG_PLAT_CACHE_CONTROL&=(~(CACHE_DCACHE_ENABLE|CACHE_DATA_BURST_FETCH)))
#define PLAT_PROC1_DCACHE_ENABLE        (REG_PLAT_CACHE_CONTROL|=(CACHE_DCACHE_ENABLE|CACHE_DATA_BURST_FETCH))
#define PLAT_PROC1_ICACHE_DISABLE       (REG_PLAT_CACHE_CONTROL&=(~(CACHE_ICACHE_ENABLE|CACHE_INSTRUCTION_BURST_FETCH)))
#define PLAT_PROC1_ICACHE_ENABLE        (REG_PLAT_CACHE_CONTROL|=(CACHE_ICACHE_ENABLE|CACHE_INSTRUCTION_BURST_FETCH))
#define PLAT_PROC1_CACHE_DISABLE        (REG_PLAT_CACHE_CONTROL&=(~(CACHE_DCACHE_ENABLE|CACHE_DATA_BURST_FETCH|CACHE_ICACHE_ENABLE|CACHE_INSTRUCTION_BURST_FETCH)))
#define PLAT_PROC1_CACHE_ENABLE         (REG_PLAT_CACHE_CONTROL|=(CACHE_DCACHE_ENABLE|CACHE_DATA_BURST_FETCH|CACHE_ICACHE_ENABLE|CACHE_INSTRUCTION_BURST_FETCH))
#define PLAT_PROC1_ICACHE_FLUSH         (REG_PLAT_CACHE_CONTROL|=(CACHE_FLUSH_ICACHE))
// -------------------------------------------------------------------------------------------------
#define PLAT_BYPASS_DCACHE_OFFSET       (0x80000000)
#define PLAT_BYPASS_DCACHE_STARTADR     (0xc0000000)
// =================================================================================================
// F100CSC_105, replace macro for supporting DCache power saving mode
// CT909 PROC1's cache size is 4-way 2KB (16 bytes per line).
// Flush one cache line needs around one cycle. So, we need to wait at least
// 2048/16 = 128 cycles.
#if 0
#define PLAT_PROC1_DCACHE_FLUSH         (REG_PLAT_CACHE_CONTROL|=(CACHE_FLUSH_DCACHE))
#else
#define PLAT_PROC1_DCACHE_FLUSH                                     \
    {                                                               \
        register DWORD dwIdle = 4;                                  \
        REG_PLAT_CACHE_CONTROL &= (~(CACHE_DCACHE_POWER_SAVING));   \
        REG_PLAT_CACHE_CONTROL |= CACHE_FLUSH_DCACHE;               \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );      \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );      \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );      \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );      \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );      \
        REG_PLAT_CACHE_CONTROL |= CACHE_DCACHE_POWER_SAVING;        \
    }
#endif  // 0
// -------------------------------------------------------------------------------------------------
#if 0
#define PLAT_PROC1_CACHE_FLUSH          (REG_PLAT_CACHE_CONTROL|=(CACHE_FLUSH_DCACHE|CACHE_FLUSH_ICACHE))
#else
#define PLAT_PROC1_CACHE_FLUSH                                              \
    {                                                                       \
        register DWORD dwIdle = 4;                                          \
        REG_PLAT_CACHE_CONTROL &= (~(CACHE_DCACHE_POWER_SAVING));           \
        REG_PLAT_CACHE_CONTROL |= (CACHE_FLUSH_DCACHE|CACHE_FLUSH_ICACHE);  \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );              \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );              \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );              \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );              \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );              \
        REG_PLAT_CACHE_CONTROL |= (CACHE_FLUSH_DCACHE|CACHE_FLUSH_ICACHE);  \
    }
#endif  // 0
// =================================================================================================
// J500CSC_111, update platform lock mechanism
// Since the REG_PLAT_COMMAND_ADDR, a APB register, has problem, we change to use
// DRAM space as semaphore. DS_PROC_SEMAPHORE is defined at ctkav.h and this address
// should be initialized to zero, done by MACRO_PLAT_KEY_UNLOCK, when H/W Power-on
// or PROC2 reset.
#if 0
    #define MACRO_PLAT_KEY_LOCK( )                                                          \
        {                                                                                   \
            register DWORD dwLock;                                                          \
            register DWORD dwSemaphoreAddr = REG_PLAT_COMMAND_ADDR;                         \
            register DWORD dwCounter = 100;                                                 \
                                                                                            \
            while( dwCounter -- )                                                           \
            {                                                                               \
                asm volatile("mov 0x01, %0": "=r" (dwLock));                                \
                asm volatile ("swap [%1], %0;" : "=r" (dwLock) : "r" (dwSemaphoreAddr) );   \
                if( dwLock == 0 )                                                           \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                asm volatile ("nop; nop; nop; nop;");                                       \
            };                                                                              \
        }

    #define MACRO_PLAT_KEY_UNLOCK( )                                        \
        {                                                                   \
            register DWORD dwSemaphoreAddr = REG_PLAT_COMMAND_ADDR;         \
                                                                            \
            asm volatile ("swap [%0], %%g0;" : : "r" (dwSemaphoreAddr) );   \
        }
#else
    #define MACRO_PLAT_KEY_LOCK( )                                                              \
        {                                                                                       \
            register DWORD dwCounter = 4000;                                                    \
                                                                                                \
            while( dwCounter -- )                                                               \
            {                                                                                   \
                if( (REG_PLAT_COMMAND & 1) == 0 )                                               \
                {                                                                               \
                    break;                                                                      \
                }                                                                               \
                                                                                                \
                asm volatile ("nop; nop; nop; nop;");                                           \
            };                                                                                  \
        }

    #define MACRO_PLAT_KEY_UNLOCK( ) { REG_PLAT_COMMAND = 0; }

    // J500CSC_120, retrieve eCos kernel library's version
    #define MACRO_ECOS_VER( dwVer )     (dwVer = ecos_lib_ver);

    // J500CSC_120, provide access the exception error interface
    // Here we have exception number function & content of each index.
    // Index is counting from 1 to ... (64 is maximum for current setting).
    #define MACRO_GET_EXCEPTION_NUM( dwExceptionNum )                   \
        {                                                               \
            dwExceptionNum = hal_vsr_trace[0];                          \
        }

    #define MACRO_GET_EXCEPTION_INDEX( dwIndex, dwExceptionTT, dwExceptionPC )  \
        {                                                                       \
            dwExceptionTT = hal_vsr_trace[(dwIndex << 1) - 1];                  \
            dwExceptionPC = hal_vsr_trace[(dwIndex << 1)];                      \
        }
#endif  // 0
// -------------------------------------------------------------------------------------------------
#define MACRO_IDLE( )                                           \
    {                                                           \
        register DWORD dwIdle = 4;                              \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );  \
    }
// -------------------------------------------------------------------------------------------------
#define DRAM_DUMMY      (*(volatile DWORD *)(0x403FFFF4L))
// -------------------------------------------------------------------------------------------------
#define MACRO_PLAT_RESET_WAITSTABLE( )          \
    {                                           \
        DRAM_DUMMY = 0xFFFFFFFF;                \
        DRAM_DUMMY = 0xFFFFFFFF;                \
        DRAM_DUMMY = 0xFFFFFFFF;                \
        DRAM_DUMMY = 0xFFFFFFFF;                \
        DRAM_DUMMY = 0xFFFFFFFF;                \
        DRAM_DUMMY = 0xFFFFFFFF;                \
    }
// =================================================================================================
// Define the meaning of DSU control bit (REG_PLAT_DSU_CONTROL/REG_PLAT_DSU2_CONTROL)
#define PLAT_DSU_CTL_TE         (0x00000001)
#define PLAT_DSU_CTL_DCM        (0x00000002)
#define PLAT_DSU_CTL_BT         (0x00000004)
#define PLAT_DSU_CTL_FT         (0x00000008)
#define PLAT_DSU_CTL_BE         (0x00000010)
#define PLAT_DSU_CTL_BW         (0x00000020)
#define PLAT_DSU_CTL_BS         (0x00000040)
#define PLAT_DSU_CTL_BN         (0x00000080)
#define PLAT_DSU_CTL_BB         (0x00000100)
#define PLAT_DSU_CTL_BX         (0x00000200)
#define PLAT_DSU_CTL_BZ         (0x00000400)
#define PLAT_DSU_CTL_DE         (0x00000800)
#define PLAT_DSU_CTL_DM         (0x00001000)
#define PLAT_DSU_CTL_EB         (0x00002000)
#define PLAT_DSU_CTL_EE         (0x00004000)
#define PLAT_DSU_CTL_PE         (0x00008000)
#define PLAT_DSU_CTL_SS         (0x00010000)
#define PLAT_DSU_CTL_LR         (0x00020000)
#define PLAT_DSU_CTL_DR         (0x00040000)
#define PLAT_DSU_CTL_RE         (0x00080000)
// -------------------------------------------------------------------------------------------------
#ifdef CT909P_IC_SYSTEM
#define PLAT_POWER_DOWN_HIV     (0x0200)
#define PLAT_POWER_DOWN_DD      (0x0400)
#define PLAT_POWER_DOWN_DC      (0x0800)
#define PLAT_POWER_DOWN_DY      (0x1000)
#define PLAT_POWER_DOWN_CVBS    (0x2000)
#define PLAT_POWER_DOWN_ALL     (0x4000)
#endif

#define MACRO_PLAT_GET_PROC2_PC( dwPC )                         \
    {                                                           \
        register DWORD dwIdle = 4;                              \
        REG_PLAT_PROC2_POWER_DOWN = 0xff;                       \
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );  \
        dwPC = REG_PLAT_PROC2_POWER_DOWN;                       \
    }
// -------------------------------------------------------------------------------------------------
#define MACRO_PLAT_HALT_PROC2( )                                    \
    {                                                               \
        register DWORD dwIdle = 4;                                  \
        REG_PLAT_DSU2_CONTROL = (PLAT_DSU_CTL_BN | PLAT_DSU_CTL_BW);\
        asm volatile ("udiv %0, %0, %%g0;" : : "r" (dwIdle) );      \
    }
// -------------------------------------------------------------------------------------------------
#define MACRO_PLAT_RELEASE_PROC2( )                                 \
    {                                                               \
        REG_PLAT_DSU2_CONTROL = (PLAT_DSU_CTL_RE);                  \
    }
// =================================================================================================
// [1] IC chip version
#define MACRO_PLAT_GET_CHIPVER      (((REG_PLAT_SYSTEM_CONFIGURATION2 & 0xFF000000) >> 16) | REG_PLAT_IC_ID)

// LLY2.75, add for CT909G IC
#ifdef  CT909G_IC_SYSTEM
// [2] Part Number
#define MACRO_PLAT_GET_PARTNO       ((REG_PLAT_MPLL_CONTROL >> 24) & 0x1f)
// -------------------------------------------------------------------------------------------------
// [3] define macro for chip supported capability
#define IC_CAP_MPEG4            (0x10)  // Support MPEG4, ex CT909G-A/-W/-X
#define IC_CAP_DIVX             (0x08)  // Support DivX, ex. CT909G-W.-X
#define IC_CAP_51CH             (0x04)  // Support 5.1 channel, ex. CT909G-X
#define IC_CAP_SPECIAL          (0x02)  // Support special application, ex. CT909G-B
#define IC_CAP_ALL              (0x1E)  // Support all feature.

// -------------------------------------------------------------------------------------------------
#define MACRO_IC_CAPABILITY     (MACRO_PLAT_GET_PARTNO)

#else   // #ifdef CT909G_IC_SYSTEM
// [2] Part Number
#define MACRO_PLAT_GET_PARTNO       ((REG_PLAT_MPLL_CONTROL >> 28) & 0xf)
// -------------------------------------------------------------------------------------------------
// [3] define macro for chip supported capability
#define IC_CAP_SPECIAL          (0x01)
// LLY2.75, bit[1] is used for multi-media supporting, ex. CT95x serial IC
#define IC_CAP_MULTIMEDIA       (0x02)
#define IC_CAP_DIVX             (0x04)
#define IC_CAP_DVB              (0x08)
#define IC_CAP_DIGITAL_PANEL    (0x10)
#define IC_CAP_NAND_FLASH       (0x20)
#ifdef  CT909P_IC_SYSTEM
#define IC_CAP_ALL              (0x1F)
// -----------------------------------------------------------------------------------------------
#define IC_CT956A               (0x0A)
#define MACRO_IC_CAPABILITY     ((MACRO_PLAT_GET_PARTNO == IC_CT956A) ? (IC_CAP_DIGITAL_PANEL|IC_CAP_NAND_FLASH|IC_CAP_DVB|IC_CAP_MULTIMEDIA) : MACRO_PLAT_GET_PARTNO)
#else   // #ifdef CT909P_IC_SYSTEM
#define IC_CAP_ALL              (0x0D)
// -------------------------------------------------------------------------------------------------
#define IC_CT909RZ              (0x06)
#define IC_CT909SZ              (0x02)
// -------------------------------------------------------------------------------------------------
#define MACRO_IC_CAPABILITY     (((MACRO_PLAT_GET_PARTNO == IC_CT909RZ) || (MACRO_PLAT_GET_PARTNO == IC_CT909SZ)) ? IC_CAP_ALL : MACRO_PLAT_GET_PARTNO)
#endif  // #ifdef CT909P_IC_SYSTEM
#endif  // #ifdef CT909G_IC_SYSTEM
// =================================================================================================
#ifdef __cplusplus
}
// =================================================================================================
#endif //__cplusplus
#endif  // __CTKAV_PLATFORM_H__
// =================================================================================================
