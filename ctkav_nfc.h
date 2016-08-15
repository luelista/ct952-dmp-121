
#ifndef __CTKAV_NFC_H__
#define __CTKAV_NFC_H__
#ifdef __cplusplus
    extern "C" {
#endif  // __cplusplus

// =================================================================================================
#define REG_NFC_BASE                            (0xa0002000L)
// -------------------------------------------------------------------------------------------------
#define REG_NFC_CFG0                            (*(volatile DWORD*)(REG_NFC_BASE+0x00))
#define REG_NFC_CFG1                            (*(volatile DWORD*)(REG_NFC_BASE+0x04))
#define REG_NFC_CMD                             (*(volatile DWORD*)(REG_NFC_BASE+0x08))
#define REG_NFC_TRAN_MODE                       (*(volatile DWORD*)(REG_NFC_BASE+0x0c))
#define REG_NFC_COL_ADDR                        (*(volatile DWORD*)(REG_NFC_BASE+0x10))
#define REG_NFC_ROW_ADDR                        (*(volatile DWORD*)(REG_NFC_BASE+0x14))
#define REG_NFC_SW_RESET                        (*(volatile DWORD*)(REG_NFC_BASE+0x18))
#define REG_NFC_RAND_ACC_CMD                    (*(volatile DWORD*)(REG_NFC_BASE+0x1c))
#define REG_NFC_INT_ENABLE                      (*(volatile DWORD*)(REG_NFC_BASE+0x20))
#define REG_NFC_INT_STAT                        (*(volatile DWORD*)(REG_NFC_BASE+0x24))
#define REG_NFC_DATA_PORT                       (*(volatile DWORD*)(REG_NFC_BASE+0x28))
#define REG_NFC_XTRA_DATA0                      (*(volatile DWORD*)(REG_NFC_BASE+0x30))
#define REG_NFC_XTRA_DATA1                      (*(volatile DWORD*)(REG_NFC_BASE+0x34))
#define REG_NFC_XTRA_DATA2                      (*(volatile DWORD*)(REG_NFC_BASE+0x38))
#define REG_NFC_XTRA_DATA3                      (*(volatile DWORD*)(REG_NFC_BASE+0x3c))
#define REG_NFC_DIRECT_CTRL                     (*(volatile DWORD*)(REG_NFC_BASE+0x40))
#define REG_NFC_STAT                            (*(volatile DWORD*)(REG_NFC_BASE+0x44))
#define REG_NFC_HM_ECC0                         (*(volatile DWORD*)(REG_NFC_BASE+0x48))
#define REG_NFC_HM_ECC1                         (*(volatile DWORD*)(REG_NFC_BASE+0x4c))
#define REG_NFC_DMA_ADDR                        (*(volatile DWORD*)(REG_NFC_BASE+0x50))
#define REG_NFC_DMA_CTRL                        (*(volatile DWORD*)(REG_NFC_BASE+0x54))
#define REG_NFC_RS_ECC0                         (*(volatile DWORD*)(REG_NFC_BASE+0x60))
#define REG_NFC_RS_ECC1                         (*(volatile DWORD*)(REG_NFC_BASE+0x64))
#define REG_NFC_RS_ECC2                         (*(volatile DWORD*)(REG_NFC_BASE+0x68))
#define REG_NFC_RS_ECC3                         (*(volatile DWORD*)(REG_NFC_BASE+0x6c))
#define REG_NFC_HM_ECC0_ERR_STAT                (*(volatile DWORD*)(REG_NFC_BASE+0x70))
#define REG_NFC_HM_ECC1_ERR_STAT                (*(volatile DWORD*)(REG_NFC_BASE+0x74))

// =================================================================================================
// REG_NFC_CFG0
#define NFC_CFG0_TWH(n)                         ((0xf & (n)) << 28)
#define NFC_CFG0_TWL(n)                         ((0xf & (n)) << 24)
#define NFC_CFG0_TRH(n)                         ((0xf & (n)) << 20)
#define NFC_CFG0_TRL(n)                         ((0xf & (n)) << 16)
#define NFC_CFG0_TWW(n)                         ((0xf & (n)) << 12)
#define NFC_CFG0_TAR(n)                         ((0xf & (n)) << 8)
#define NFC_CFG0_TCLS_TCS_TALS_TALH(n)          ((0xf & (n)) << 4)
#define NFC_CFG0_16BIT                          (1 << 2)
#define NFC_CFG0_ROW_ADDR_3CYCLES               (1 << 1)
#define NFC_CFG0_COL_ADDR_2CYCLES               (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_CFG1
#define NFC_CFG1_READY_TO_BUSY_TIMEOUT(n)       ((0xf & (n)) << 24)
#define NFC_CFG1_LITTLE_ENDIAN_XTRA             (1 << 19)
#define NFC_CFG1_LITTLE_ENDIAN                  (1 << 18)
#define NFC_CFG1_RS_ECC_ENABLE                  (1 << 16)
#define NFC_CFG1_BUSY_TO_READY_TIMEOUT(n)       (0xffff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_CMD
#define NFC_CMD_KEEP_CE_WHEN_NO_DATA_TRAN       (1 << 31)
#define NFC_CMD_CE_IDX(n)                       ((7 & (n)) << 28)
#define NFC_CMD_WP_DISABLE                      (1 << 27)
#define NFC_CMD_KEEP_WP_WHEN_NO_DATA_TRAN       (1 << 26)
#define NFC_CMD_DATA_PRESENT                    (1 << 23)
#define NFC_CMD_ADDR_CYCLE(n)                   ((7 & (n)) << 20)
#define NFC_CMD_ADDR_CYCLE_NONE                 NFC_CMD_ADDR_CYCLE(0)
#define NFC_CMD_ADDR_CYCLE_DUMMY                NFC_CMD_ADDR_CYCLE(1)
#define NFC_CMD_ADDR_CYCLE_COL                  NFC_CMD_ADDR_CYCLE(2)
#define NFC_CMD_ADDR_CYCLE_ROW                  NFC_CMD_ADDR_CYCLE(4)
#define NFC_CMD_ADDR_CYCLE_COL_ROW              NFC_CMD_ADDR_CYCLE(6)
#define NFC_CMD_END_WAIT_BUSY_TO_RDY            (1 << 19)
#define NFC_CMD_END_WAIT_RDY_TO_BUSY            (1 << 18)
#define NFC_CMD_CYCLE(n)                        ((3 & (n)) << 16)
#define NFC_CMD_CYCLE_NONE                      NFC_CMD_CYCLE(0)
#define NFC_CMD_CYCLE_ONE                       NFC_CMD_CYCLE(1)
#define NFC_CMD_CYCLE_TWO                       NFC_CMD_CYCLE(2)
#define NFC_CMD_CODE1(n)                        ((0xff & (n)) << 8)
#define NFC_CMD_CODE0(n)                        (0xff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_TRAN_MODE
#define NFC_TRAN_MODE_KEEP_CE                   (1 << 31)
#define NFC_TRAN_MODE_CE_IDX(n)                 ((7 & (n)) << 28)
#define NFC_TRAN_MDOE_RAND_ACC_CMD_CYCLE(n)     ((3 & (n)) << 24)
#define NFC_TRAN_MDOE_RAND_ACC_CMD_CYCLE_NONE   NFC_TRAN_MDOE_RAND_ACC_CMD_CYCLE(0)
#define NFC_TRAN_MDOE_RAND_ACC_CMD_CYCLE_ONE    NFC_TRAN_MDOE_RAND_ACC_CMD_CYCLE(2)
#define NFC_TRAN_MDOE_RAND_ACC_CMD_CYCLE_TWO    NFC_TRAN_MDOE_RAND_ACC_CMD_CYCLE(3)
#define NFC_TRAN_MDOE_XTRA_DATA_COUNT(n)        ((3 & (n)) << 22)
#define NFC_TRAN_MDOE_XTRA_DATA_COUNT_NONE      NFC_TRAN_MDOE_XTRA_DATA_COUNT(0)
#define NFC_TRAN_MDOE_XTRA_DATA_COUNT_8_LOW     NFC_TRAN_MDOE_XTRA_DATA_COUNT(1)
#define NFC_TRAN_MDOE_XTRA_DATA_COUNT_8_HIGH    NFC_TRAN_MDOE_XTRA_DATA_COUNT(2)
#define NFC_TRAN_MDOE_XTRA_DATA_COUNT_16        NFC_TRAN_MDOE_XTRA_DATA_COUNT(3)
#define NFC_TRAN_MDOE_END_WAIT_BUSY_TO_RDY      (1 << 21)
#define NFC_TRAN_MDOE_ECC_CHK                   (1 << 19)
#define NFC_TRAN_MDOE_ECC_CMB                   (1 << 18)
#define NFC_TRAN_MDOE_BLK_SIZE(n)               ((0x1ff & ((n) - 1)) << 8)
#define NFC_TRAN_MDOE_ECC_ENABLE                (1 << 7)
#define NFC_TRAN_MDOE_ECC_RESET                 (1 << 6)
#define NFC_TRAN_MDOE_DATA_SEL(n)               ((3 & (n)) << 4)
#define NFC_TRAN_MDOE_DATA_SEL_DATA_PORT        NFC_TRAN_MDOE_DATA_SEL(0)
#define NFC_TRAN_MDOE_DATA_SEL_DMA              NFC_TRAN_MDOE_DATA_SEL(1)
#define NFC_TRAN_MDOE_DATA_SEL_XTRA             NFC_TRAN_MDOE_DATA_SEL(2)
#define NFC_TRAN_MDOE_DATA_SEL_XTRA_ECC_CMB     NFC_TRAN_MDOE_DATA_SEL(3)
#define NFC_TRAN_MDOE_END_WAIT_RDY_TO_BUSY      (1 << 3)
#define NFC_TRAN_MDOE_START_WAIT_RDY            (1 << 2)
#define NFC_TRAN_MDOE_WRITE                     (1 << 1)
#define NFC_TRAN_MDOE_FIRE                      (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_SW_RESET
#define NFC_SW_RESET_DMA                        (1 << 1)
#define NFC_SW_RESET_INTERFACE                  (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_RAND_ACC_CMD
#define NFC_RAND_ACC_CMD_CODE1(n)               ((0xff & (n)) << 24)
#define NFC_RAND_ACC_CMD_CODE0(n)               ((0xff & (n)) << 16)
#define NFC_RAND_ACC_CMD_COL_ADDR(n)            (0xffff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_INT_ENABLE, REG_NFC_INT_STAT
#define NFC_INT_ERR                             ((0xf7f << 8))
// | NFC_INT_ERR_ECC)
#define NFC_INT_INT_ENABLE                      (1 << 31)
#define NFC_INT_RDY_REQ                         (1 << 24)
#define NFC_INT_ERR_REG_DMA                     (1 << 19)
#define NFC_INT_ERR_REG_TRAN_MODE               (1 << 18)
#define NFC_INT_ERR_REG_CMD                     (1 << 17)
#define NFC_INT_ERR_REG_DIRECT_CTRL             (1 << 16)
#define NFC_INT_ERR_TRAN_WAIT_RDY1_TIMEOUT      (1 << 14)
#define NFC_INT_ERR_TRAN_WAIT_BUSY_TIMEOUT      (1 << 13)
#define NFC_INT_ERR_TRAN_WAIT_RDY0_TIMEOUT      (1 << 12)
#define NFC_INT_ERR_DATA_PORT_WRITE             (1 << 11)
#define NFC_INT_ERR_DATA_PORT_READ              (1 << 10)
#define NFC_INT_ERR_CMD_WAIT_BUSY_TIMEOUT       (1 << 9)
#define NFC_INT_ERR_CMD_WAIT_RDY_TIMEOUT        (1 << 8)
#define NFC_INT_RS_ECC_DEC_COMPLETE             (1 << 6)
#define NFC_INT_DIRECT_CTRL_DATA_COMPLETE       (1 << 5)
#define NFC_INT_ERR_ECC                         (1 << 4)
#define NFC_INT_DATA_PORT_BUFF_RDY              (1 << 2)
#define NFC_INT_MEM_TRAN_COMPLETE               (1 << 2)
#define NFC_INT_DATA_TRAN_COMPLETE              (1 << 1)
#define NFC_INT_CMD_COMPLETE                    (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_DIRECT_CTRL
#define NFC_DIRECT_CTRL_WP                      (1 << 31)
#define NFC_DIRECT_CTRL_ECC_OPT                 (3 << 29)
#define NFC_DIRECT_CTRL_CE                      (1 << 28)
#define NFC_DIRECT_CTRL_ALE                     (1 << 25)
#define NFC_DIRECT_CTRL_CLE                     (1 << 24)
#define NFC_DIRECT_CTRL_XTRA_DATA_ADDR(n)       ((1 & (n)) << 23)
#define NFC_DIRECT_CTRL_XTRA_DATA_ADDR_LOW      NFC_DIRECT_CTRL_XTRA_DATA_ADDR(0)
#define NFC_DIRECT_CTRL_XTRA_DATA_ADDR_HIGH     NFC_DIRECT_CTRL_XTRA_DATA_ADDR(1)
#define NFC_DIRECT_CTRL_CE_IDX(n)               ((7 & (n)) << 20)
#define NFC_DIRECT_CTRL_BLK_SIZE(n)             ((0x1ff & (n)) << 8)
#define NFC_DIRECT_CTRL_RS_ECC_DEC_STAGE        (1 << 6)
#define NFC_DIRECT_CTRL_RS_ECC_DEC              (1 << 5)
#define NFC_DIRECT_CTRL_RS_ECC_ACTIVE           (1 << 4)
#define NFC_DIRECT_CTRL_DATA_SEL(n)             ((3 & (n)) << 2)
#define NFC_DIRECT_CTRL_DATA_SEL_DATA_PORT      NFC_DIRECT_CTRL_DATA_SEL(0)
#define NFC_DIRECT_CTRL_DATA_SEL_DMA            NFC_DIRECT_CTRL_DATA_SEL(1)
#define NFC_DIRECT_CTRL_DATA_SEL_XTRA           NFC_DIRECT_CTRL_DATA_SEL(2)
#define NFC_DIRECT_CTRL_DATA_SEL_XTRA_ECC_CMB   NFC_DIRECT_CTRL_DATA_SEL(3)
#define NFC_DIRECT_CTRL_WRITE                   (1 << 1)
#define NFC_DIRECT_CTRL_DATA_PRESENT            (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_STAT
#define NFC_STAT_RB                             (1 << 31)
#define NFC_STAT_RS_ECC_ERR(n)                  ((3 & (n)) << 20)
#define NFC_STAT_RS_ECC_ERR_MASK                NFC_STAT_RS_ECC_ERR(-1)
#define NFC_STAT_RS_ECC_ERR_NONE                NFC_STAT_RS_ECC_ERR(0)
#define NFC_STAT_RS_ECC_ERR_CORRECTABLE         NFC_STAT_RS_ECC_ERR(1)
#define NFC_STAT_RS_ECC_ERR_NONCORRECTABLE      NFC_STAT_RS_ECC_ERR(2)
#define NFC_STAT_RS_ECC_DEC_ERR_ADDR_DATA_MASK  (1 << 19)
#define NFC_STAT_RS_ECC_ERR_BYTE_COUNT          (7 << 16)
#define NFC_STAT_HM_ECC_ERR_IDX1                (3 << 10)
#define NFC_STAT_HM_ECC_ERR_IDX0                (3 << 8)
#define NFC_STAT_DMA_TRAN_ACTIVE                (1 << 5)
#define NFC_STAT_CMD_ACTIVE                     (1 << 4)
#define NFC_STAT_DATA_TRAN_ACTIVE               (1 << 3)
#define NFC_STAT_DIRECT_CTRL_DATA_TRAN_ACTIVE   (1 << 2)
#define NFC_STAT_DATA_PORT_BUFF_FULL            (1 << 1)
#define NFC_STAT_DATA_PORT_BUFF_EMPTY           (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_HM_ECC0, REG_NFC_HM_ECC1
#define NFC_HM_ECC_PARITY_COL                   (0xff << 16)
#define NFC_HM_ECC_PARITY_LINE                  (0xffff)

// -------------------------------------------------------------------------------------------------
// REG_NFC_DMA_CTRL
#define NFC_DMA_CTRL_READ                       (1 << 16)
#define NFC_DMA_CTRL_TRAN_BYTE_COUNT(n)         (0xffff & ((n) - 1))

// -------------------------------------------------------------------------------------------------
// REG_NFC_RS_ECC0, REG_NFC_RS_ECC1, REG_NFC_RS_ECC2, REG_NFC_RS_ECC3
#define NFC_RS_ECC_CORRECTABLE_DATA_ADDR        (0x3ff << 16)
#define NFC_RS_ECC_CORRECTABLE_DATA_MASK        (0xff)

// -------------------------------------------------------------------------------------------------
// REG_NFC_HM_ECC0_ERR_STAT, REG_NFC_HM_ECC1_ERR_STAT
#define NFC_HM_ECC_ERR_STAT_IDX                 (3 << 16)
#define NFC_HM_ECC_ERR_STAT_DATA_ADDR           (0xff << 8)
#define NFC_HM_ECC_ERR_STAT_DATA_MASK           (7)

// =================================================================================================
#ifdef __cplusplus
    }
#endif  // __cplusplus
#endif  // __CTKAV_NFC_H__
