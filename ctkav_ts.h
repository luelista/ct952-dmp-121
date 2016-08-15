#ifndef __CTKAV_TS_H__
#define __CTKAV_TS_H__

//#define CT909_IO_START  (0x80000000)

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define REG_TS_BASE        (CT909_IO_START+0x2980)

#define REG_TS_NIM_CFG      (*((volatile DWORD *) (REG_TS_BASE+0x000)))    // 0x2980, NIM Configuration Register (R/W)
#define REG_TS_NIM_STS      (*((volatile DWORD *) (REG_TS_BASE+0x004)))    // 0x2984, NIM Status Register (R)
#define REG_TS_PID_STS      (*((volatile DWORD *) (REG_TS_BASE+0x008)))    // 0x2988, PID Status Register (R)

#define REG_TS_CRC_INI      (*((volatile DWORD *) (REG_TS_BASE+0x010)))    // 0x2990, CRC32 Value Intialization Register (W)
#define REG_TS_CRC_OP       (*((volatile DWORD *) (REG_TS_BASE+0x014)))    // 0x2994, CRC32 Operation Value Register (R/W)
#define REG_TS_CRC_REM      (*((volatile DWORD *) (REG_TS_BASE+0x018)))    // 0x2998, CRC32 Remainder Value Register (R)

#define REG_TS_PWM          (*((volatile DWORD *) (REG_TS_BASE+0x01c)))    // 0x299c, PWM Control Register (R/W)

#define REG_TS_PID_FLT_00   (*((volatile DWORD *) (REG_TS_BASE+0x020)))    // 0x29a0, PID #00 Filter Control Register (R/W)
#define REG_TS_PID_FLT_01   (*((volatile DWORD *) (REG_TS_BASE+0x024)))    // 0x29a4, PID #01 Filter Control Register (R/W)
#define REG_TS_PID_FLT_02   (*((volatile DWORD *) (REG_TS_BASE+0x028)))    // 0x29a8, PID #02 Filter Control Register (R/W)
#define REG_TS_PID_FLT_03   (*((volatile DWORD *) (REG_TS_BASE+0x02c)))    // 0x29ac, PID #03 Filter Control Register (R/W)
#define REG_TS_PID_FLT_04   (*((volatile DWORD *) (REG_TS_BASE+0x030)))    // 0x29b0, PID #04 Filter Control Register (R/W)
#define REG_TS_PID_FLT_05   (*((volatile DWORD *) (REG_TS_BASE+0x034)))    // 0x29b4, PID #05 Filter Control Register (R/W)
#define REG_TS_PID_FLT_06   (*((volatile DWORD *) (REG_TS_BASE+0x038)))    // 0x29b8, PID #06 Filter Control Register (R/W)
#define REG_TS_PID_FLT_07   (*((volatile DWORD *) (REG_TS_BASE+0x03c)))    // 0x29bc, PID #07 Filter Control Register (R/W)
#define REG_TS_PID_FLT_08   (*((volatile DWORD *) (REG_TS_BASE+0x040)))    // 0x29c0, PID #08 Filter Control Register (R/W)
#define REG_TS_PID_FLT_09   (*((volatile DWORD *) (REG_TS_BASE+0x044)))    // 0x29c4, PID #09 Filter Control Register (R/W)
#define REG_TS_PID_FLT_10   (*((volatile DWORD *) (REG_TS_BASE+0x048)))    // 0x29c8, PID #10 Filter Control Register (R/W)
#define REG_TS_PID_FLT_11   (*((volatile DWORD *) (REG_TS_BASE+0x04c)))    // 0x29cc, PID #11 Filter Control Register (R/W)
#define REG_TS_PID_FLT_12   (*((volatile DWORD *) (REG_TS_BASE+0x050)))    // 0x29d0, PID #12 Filter Control Register (R/W)
#define REG_TS_PID_FLT_13   (*((volatile DWORD *) (REG_TS_BASE+0x054)))    // 0x29d4, PID #13 Filter Control Register (R/W)
#define REG_TS_PID_FLT_14   (*((volatile DWORD *) (REG_TS_BASE+0x058)))    // 0x29d8, PID #14 Filter Control Register (R/W)
#define REG_TS_PID_FLT_15   (*((volatile DWORD *) (REG_TS_BASE+0x05c)))    // 0x29dc, PID #15 Filter Control Register (R/W)
#define REG_TS_PID_FLT(dwID_Num)        (*((volatile DWORD *) (REG_TS_BASE+0x020+(dwID_Num << 2))))

#if 0   // for debug only
PBYTE   REG_TS_NAME[24] =
    { "NIM_CFG", "NIM_STS", "PID_STS", "NULL", "CRC_INI", "CRC_OP", "CRC_REM", "PWM",
        "PID_FLT_00", "PID_FLT_01", "PID_FLT_02", "PID_FLT_03", "PID_FLT_04", "PID_FLT_05",
        "PID_FLT_06", "PID_FLT_07", "PID_FLT_08", "PID_FLT_09", "PID_FLT_10", "PID_FLT_11",
        "PID_FLT_12", "PID_FLT_13", "PID_FLT_14", "PID_FLT_15" };
#endif  // 0

// define PID Filter relative macro or bit field
#define TS_PID_FLT_EN                   (0x00002000)
#define TS_PID_FLT_ID                   (0x00001fff)

// define NIM_CFG relative macro or bit field
#define TS_NIMCFG_RESET_EN              (0x00010000)
#define TS_NIMCFG_CLK_EN                (0x00000080)
#define TS_NIMCFG_SERIAL                (0x00000001)
#define TS_NIMCFG_BLKERR                (0x00000008)
#define TS_NIMCFG_CLKINVERT             (0x00000004)
#define TS_NIMCFG_SER_D7                (0x00000002)

// define NIM_STS relative macro or bit field
#define TS_SM_S                         (0x00000300)        // State Machine Status
#define TS_PKTCNT_SYN_LOST              (0x000000f0)        // packet count for syn lost determination
#define TS_PKTCNT_SYN_HUNT              (0x0000000f)        // packet count for syn hunting

// define PID_STS relative macro or bit field
#define TS_PID_STATUS                   (0xffff0000)        // Each bit represent on PID_FLT.
                                                            // ¡§1¡¨ means current packet is filter by this PID_FLT.
#define TS_PID_PACKET_COUNT             (0x0000ffff)        // A free-run counter increament
                                                            // when a packet is filter by any PID_FLT.

#ifdef __cplusplus
}
#endif //__cplusplus

#endif  // __CTKAV_TS_H__
