
#ifndef __CTKAV_SDC_H__
#define __CTKAV_SDC_H__
#ifdef __cplusplus
    extern "C" {
#endif  // __cplusplus


#ifndef REG_FCR_BASE

// =================================================================================================
#define REG_FCR_BASE                            (0xa0001000)
// -------------------------------------------------------------------------------------------------
#define REG_FCR_FUNC_CTRL                       (*(volatile DWORD*)(REG_FCR_BASE+0x00))
#define REG_FCR_CPBLT                           (*(volatile DWORD*)(REG_FCR_BASE+0x04))

// =================================================================================================
// REG_FCR_FUNC_CTRL
#define FCR_FUNC_CTRL_SW_MS_CD                  (1 << 27)
#define FCR_FUNC_CTRL_SW_SD_CD                  (1 << 26)
#define FCR_FUNC_CTRL_SW_SD_WP                  (1 << 25)
#define FCR_FUNC_CTRL_SW_CDWP_ENABLE            (1 << 24)
#define FCR_FUNC_CTRL_LITTLE_ENDIAN             (1 << 23)
#define FCR_FUNC_CTRL_SD_FLEXIBLE_CLK           (1 << 22)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(n)     (((n) & 3) << 20)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_MASK   FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(-1)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_512    FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(3)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_256    FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(2)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_128    FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(1)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_64     FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(0)
#define FCR_FUNC_CTRL_INT_OPT_STAT              (1 << 19)
#define FCR_FUNC_CTRL_INT_OPT_DMA               (1 << 18)
#define FCR_FUNC_CTRL_SD_PULLUP_RESISTOR        (1 << 17)
#define FCR_FUNC_CTRL_MMC_8BIT                  (1 << 16)
#define FCR_FUNC_CTRL_CD_DEBOUNCE_TIME(n)       (((n) & 0xf) << 12)
#define FCR_FUNC_CTRL_CD_DEBOUNCE_TIME_MASK     FCR_FUNC_CTRL_CD_DEBOUNCE_TIME(-1)
#define FCR_FUNC_CTRL_PW_UP_TIME(n)             (((n) & 0xf) << 8)
#define FCR_FUNC_CTRL_PW_UP_TIME_MASK           FCR_FUNC_CTRL_PW_UP_TIME(-1)
#define FCR_FUNC_CTRL_SD_SIG_PULLUP_TIME(n)     (((n) & 0xf) << 4)
#define FCR_FUNC_CTRL_SD_SIG_PULLUP_TIME_MASK   FCR_FUNC_CTRL_SD_SIG_PULLUP_TIME(-1)
#define FCR_FUNC_CTRL_MS_SIG_DELAY(n)           (((n) & 3) << 2)
#define FCR_FUNC_CTRL_MS_SIG_DELAY_MASK         FCR_FUNC_CTRL_MS_SIG_DELAY(-1)
#define FCR_FUNC_CTRL_SD_SIG_DELAY(n)           ((n) & 3)
#define FCR_FUNC_CTRL_SD_SIG_DELAY_MASK         FCR_FUNC_CTRL_SD_SIG_DELAY(-1)

// -------------------------------------------------------------------------------------------------
// REG_FCR_CPBLT
#define FCR_CPBLT_VOL_18V                       (1 << 18)
#define FCR_CPBLT_VOL_30V                       (1 << 17)
#define FCR_CPBLT_VOL_33V                       (1 << 16)
#define FCR_CPBLT_SD_BASE_CLK_FREQ(n)           (((n) & 0x3f) << 8)
#define FCR_CPBLT_SD_BASE_CLK_FREQ_MASK         FCR_CPBLT_SD_BASE_CLK_FREQ(-1)
#define FCR_CPBLT_SD_MAX_CURR_CPBLT(n)          ((n) & 0xff)
#define FCR_CPBLT_SD_MAX_CURR_CPBLT_MASK        FCR_CPBLT_SD_MAX_CURR_CPBLT(-1)

#endif  // REG_FCR_BASE


// =================================================================================================
#define REG_SDC_BASE                            (0xa0001100L)
// -------------------------------------------------------------------------------------------------
#define REG_SDC_DMA_ADDR                        (*(volatile DWORD*)(REG_SDC_BASE+0x00))
#define REG_SDC_BLK_SIZE                        (*(volatile  WORD*)(REG_SDC_BASE+0x04))
#define REG_SDC_BLK_COUNT                       (*(volatile  WORD*)(REG_SDC_BASE+0x06))
#define REG_SDC_ARG                             (*(volatile DWORD*)(REG_SDC_BASE+0x08))
#define REG_SDC_TRAN_MODE                       (*(volatile  WORD*)(REG_SDC_BASE+0x0c))
#define REG_SDC_CMD                             (*(volatile  WORD*)(REG_SDC_BASE+0x0e))
#define REG_SDC_RESP(n)                         (*(volatile DWORD*)(REG_SDC_BASE+0x10+4*((n)&3)))
#define REG_SDC_RESP0                           REG_SDC_RESP(0)
#define REG_SDC_RESP1                           REG_SDC_RESP(1)
#define REG_SDC_RESP2                           REG_SDC_RESP(2)
#define REG_SDC_RESP3                           REG_SDC_RESP(3)
#define REG_SDC_DATA_PORT                       (*(volatile DWORD*)(REG_SDC_BASE+0x20))
#define REG_SDC_STAT                            (*(volatile DWORD*)(REG_SDC_BASE+0x24))
#define REG_SDC_HOST_CTRL                       (*(volatile  BYTE*)(REG_SDC_BASE+0x28))
#define REG_SDC_PW_CTRL                         (*(volatile  BYTE*)(REG_SDC_BASE+0x29))
#define REG_SDC_BLK_GAP_CTRL                    (*(volatile  BYTE*)(REG_SDC_BASE+0x2a))
#define REG_SDC_WAKEUP_CTRL                     (*(volatile  BYTE*)(REG_SDC_BASE+0x2b))
#define REG_SDC_CLK_CTRL                        (*(volatile  WORD*)(REG_SDC_BASE+0x2c))
#define REG_SDC_TIMEOUT_CTRL                    (*(volatile  BYTE*)(REG_SDC_BASE+0x2e))
#define REG_SDC_SW_RESET                        (*(volatile  BYTE*)(REG_SDC_BASE+0x2f))
#define REG_SDC_INT_STAT                        (*(volatile DWORD*)(REG_SDC_BASE+0x30))
#define REG_SDC_INT_STAT_ENABLE                 (*(volatile DWORD*)(REG_SDC_BASE+0x34))
#define REG_SDC_INT_ENABLE                      (*(volatile DWORD*)(REG_SDC_BASE+0x38))
#define REG_SDC_AUTO_CMD12_ERR_STAT             (*(volatile  WORD*)(REG_SDC_BASE+0x3c))
#define REG_SDC_CPBLT(n)                        (*(volatile DWORD*)(REG_SDC_BASE+0x40+4*(n)))
#define REG_SDC_CPBLT0                          REG_SDC_CPBLT(0)
#define REG_SDC_CPBLT1                          REG_SDC_CPBLT(1)
#define REG_SDC_MAX_CURR_CPBLT(n)               (*(volatile DWORD*)(REG_SDC_BASE+0x48+4*(n)))
#define REG_SDC_MAX_CURR_CPBLT0                 REG_SDC_MAX_CURR_CPBLT(0)
#define REG_SDC_MAX_CURR_CPBLT1                 REG_SDC_MAX_CURR_CPBLT(1)
#define REG_SDC_SLOT_INT_STAT                   (*(volatile  WORD*)(REG_SDC_BASE+0xfc))
#define REG_SDC_HOST_VER                        (*(volatile  WORD*)(REG_SDC_BASE+0xfe))

// =================================================================================================
// REG_SDC_BLK_SIZE
#define SDC_BLK_SIZE_DMA_BUFF_BND(n)            ((WORD)((n) & 7) << 12)
#define SDC_BLK_SIZE_DMA_BUFF_BND_MASK          SDC_BLK_SIZE_DMA_BUFF_BND(-1)
#define SDC_BLK_SIZE_DMA_BUFF_BND_512K          SDC_BLK_SIZE_DMA_BUFF_BND(7)
#define SDC_BLK_SIZE_DMA_BUFF_BND_256K          SDC_BLK_SIZE_DMA_BUFF_BND(6)
#define SDC_BLK_SIZE_DMA_BUFF_BND_128K          SDC_BLK_SIZE_DMA_BUFF_BND(5)
#define SDC_BLK_SIZE_DMA_BUFF_BND_64K           SDC_BLK_SIZE_DMA_BUFF_BND(4)
#define SDC_BLK_SIZE_DMA_BUFF_BND_32K           SDC_BLK_SIZE_DMA_BUFF_BND(3)
#define SDC_BLK_SIZE_DMA_BUFF_BND_16K           SDC_BLK_SIZE_DMA_BUFF_BND(2)
#define SDC_BLK_SIZE_DMA_BUFF_BND_8K            SDC_BLK_SIZE_DMA_BUFF_BND(1)
#define SDC_BLK_SIZE_DMA_BUFF_BND_4K            SDC_BLK_SIZE_DMA_BUFF_BND(0)
#define SDC_BLK_SIZE_TRAN_BLK_SIZE(n)           ((WORD)((n) & 0xfff))

// -------------------------------------------------------------------------------------------------
// REG_SDC_TRAN_MODE
#define SDC_TRAN_MODE_MULT_BLK                  (1 << 5)
#define SDC_TRAN_MODE_READ                      (1 << 4)
#define SDC_TRAN_MODE_AUTO_CMD12                (1 << 2)
#define SDC_TRAN_MODE_BLK_COUNT                 (1 << 1)
#define SDC_TRAN_MODE_DMA                       (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_CMD
#define SDC_CMD_IDX(n)                          ((((WORD)(n)) & 0x3f) << 8)
#define SDC_CMD_TYPE_ABORT                      (3 << 6)
#define SDC_CMD_TYPE_RESUME                     (2 << 6)
#define SDC_CMD_TYPE_SUSPEND                    (1 << 6)
#define SDC_CMD_TYPE_NORMAL                     (0 << 6)
#define SDC_CMD_DATA_PRESENT                    (1 << 5)
#define SDC_CMD_IDX_CHK                         (1 << 4)
#define SDC_CMD_CRC_CHK                         (1 << 3)
#define SDC_CMD_RESP_TYPE_LEN_0                 (0 << 0)
#define SDC_CMD_RESP_TYPE_LEN_136               (1 << 0)
#define SDC_CMD_RESP_TYPE_LEN_48                (2 << 0)
#define SDC_CMD_RESP_TYPE_LEN_48_BUSY_CHK       (3 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_STAT
#define SDC_STAT_CMD_SIG                        (1 << 24)
#define SDC_STAT_DAT_SIG(n)                     (1 << (20 + (n)))
#define SDC_STAT_DAT_SIG0                       SDC_STAT_DAT_SIG(0)
#define SDC_STAT_DAT_SIG1                       SDC_STAT_DAT_SIG(1)
#define SDC_STAT_DAT_SIG2                       SDC_STAT_DAT_SIG(2)
#define SDC_STAT_DAT_SIG3                       SDC_STAT_DAT_SIG(3)
#define SDC_STAT_WP_PIN                         (1 << 19)
#define SDC_STAT_CD_PIN                         (1 << 18)
#define SDC_STAT_CARD_STAT_STABLE               (1 << 17)
#define SDC_STAT_CARD_INS                       (1 << 16)
#define SDC_STAT_BUFF_READ_ENABLE               (1 << 11)
#define SDC_STAT_BUFF_WRITE_ENABLE              (1 << 10)
#define SDC_STAT_READ_TRAN_ACTIVE               (1 << 9)
#define SDC_STAT_WRITE_TRAN_ACTIVE              (1 << 8)
#define SDC_STAT_DAT_LINE_ACTIVE                (1 << 2)
#define SDC_STAT_CMD_INHIBIT_DAT                (1 << 1)
#define SDC_STAT_CMD_INHIBIT_CMD                (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_HOST_CTRL
#define SDC_HOST_CTRL_HIGH_SPEED                (1 << 2)
#define SDC_HOST_CTRL_4BIT                      (1 << 1)
#define SDC_HOST_CTRL_LED_ON                    (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_PW_CTRL
#define SDC_PW_CTRL_BUS_VOL(n)                  ((((WORD)(n)) & 7) << 1)
#define SDC_PW_CTRL_BUS_VOL_33V                 SDC_PW_CTRL_BUS_VOL(7)
#define SDC_PW_CTRL_BUS_VOL_30V                 SDC_PW_CTRL_BUS_VOL(6)
#define SDC_PW_CTRL_BUS_VOL_18V                 SDC_PW_CTRL_BUS_VOL(5)
#define SDC_PW_CTRL_BUS_PW_ON                   (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_BLK_GAP_CTRL
#define SDC_BLK_GAP_CTRL_INT_AT_BLK_GAP         (1 << 3)
#define SDC_BLK_GAP_CTRL_READ_WAIT_CTRL         (1 << 2)
#define SDC_BLK_GAP_CTRL_CONT_REQ               (1 << 1)
#define SDC_BLK_GAP_CTRL_STOP_AT_BLK_GAP_REQ    (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_WAKEUP_CTRL
#define SDC_WAKEUP_CTRL_CARD_REM                (1 << 2)
#define SDC_WAKEUP_CTRL_CARD_INS                (1 << 1)
#define SDC_WAKEUP_CTRL_CARD_INT                (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_CLK_CTRL
#define SDC_CLK_CTRL_SDCLK_FREQ_SEL(n)          ((((WORD)(n)) & 0xff) << 8)
#define SDC_CLK_CTRL_SDCLK_ENABLE               (1 << 2)
#define SDC_CLK_CTRL_INCLK_STABLE               (1 << 1)
#define SDC_CLK_CTRL_INCLK_ENABLE               (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_TIMEOUT_CTRL
#define SDC_TIMEOUT_CTRL_DATA_TIMEOUT(n)        ((n) & 0xf)

// -------------------------------------------------------------------------------------------------
// REG_SDC_SW_RESET
#define SDC_SW_RESET_DAT_LINE                   (1 << 2)
#define SDC_SW_RESET_CMD_LINE                   (1 << 1)
#define SDC_SW_RESET_ALL                        (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_INT_STAT, REG_SDC_INT_STAT_ENABLE, REG_SDC_INT_ENABLE
#define SDC_INT_ERR_INT                         (1 << 31)
#define SDC_INT_CARD_INT                        (1 << 24)
#define SDC_INT_CARD_REM                        (1 << 23)
#define SDC_INT_CARD_INS                        (1 << 22)
#define SDC_INT_BUFF_READ_RDY                   (1 << 21)
#define SDC_INT_BUFF_WRITE_RDY                  (1 << 20)
#define SDC_INT_DMA_INT                         (1 << 19)
#define SDC_INT_BLK_GAP_EVENT                   (1 << 18)
#define SDC_INT_TRAN_COMPLETE                   (1 << 17)
#define SDC_INT_CMD_COMPLETE                    (1 << 16)
#define SDC_INT_ERR_VENDOR_SPEC_STAT(n)         ((1 << ((n) + 12)) & 0xf000)
#define SDC_INT_ERR_AUTO_CMD12                  (1 << 8)
#define SDC_INT_ERR_CURR_LIMIT                  (1 << 7)
#define SDC_INT_ERR_DATA_END_BIT                (1 << 6)
#define SDC_INT_ERR_DATA_CRC                    (1 << 5)
#define SDC_INT_ERR_DATA_TIMEOUT                (1 << 4)
#define SDC_INT_ERR_CMD_IDX                     (1 << 3)
#define SDC_INT_ERR_CMD_END_BIT                 (1 << 2)
#define SDC_INT_ERR_CMD_CRC                     (1 << 1)
#define SDC_INT_ERR_CMD_TIMEOUT                 (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_AUTO_CMD12_ERR_STAT
#define SDC_AUTO_CMD12_ERR_STAT_CMD_NOT_ISSUED_BY_AUTO_CMD12_ERR (1 << 7)
#define SDC_AUTO_CMD12_ERR_STAT_IDX_ERR         (1 << 4)
#define SDC_AUTO_CMD12_ERR_STAT_END_BIT_ERR     (1 << 3)
#define SDC_AUTO_CMD12_ERR_STAT_CRC_ERR         (1 << 2)
#define SDC_AUTO_CMD12_ERR_STAT_TIMEOUT_ERR     (1 << 1)
#define SDC_AUTO_CMD12_ERR_STAT_NOT_EXECUTED    (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_CPBLT
#define SDC_CPBLT_VOL_18V                       (1 << 26)
#define SDC_CPBLT_VOL_30V                       (1 << 25)
#define SDC_CPBLT_VOL_33V                       (1 << 24)
#define SDC_CPBLT_SUSPEND_RESUME                (1 << 23)
#define SDC_CPBLT_DMA                           (1 << 22)
#define SDC_CPBLT_HIGH_SPEED                    (1 << 21)
#define SDC_CPBLT_MAX_BLK_LEN_MASK              (3 << 16)
#define SDC_CPBLT_MAX_BLK_LEN_2048              (2 << 16)
#define SDC_CPBLT_MAX_BLK_LEN_1024              (1 << 16)
#define SDC_CPBLT_MAX_BLK_LEN_512               (0 << 16)
#define SDC_CPBLT_BASE_CLK_FREQ_MASK            (0x3f << 8)
#define SDC_CPBLT_TIMEOUT_CLK_UNIT              (1 << 7)
#define SDC_CPBLT_TIMEOUT_CLK_FREQ_MASK         (0x3f << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_MAX_CURR_CPBLT
#define SDC_MAX_CURR_CPBLT_18V_MASK             (0xff << 16)
#define SDC_MAX_CURR_CPBLT_30V_MASK             (0xff << 8)
#define SDC_MAX_CURR_CPBLT_33V_MASK             (0xff << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_SLOT_INT_STAT
#define SDC_SLOT_INT_STAT_SLOT(n)               ((1 << (n)) & 0xff)

// -------------------------------------------------------------------------------------------------
// REG_SDC_HOST_VER
#define SDC_HOST_VER_VEND_VER_MASK              (0xff << 8)
#define SDC_HOST_VER_SPEC_VER_MASK              (0xff << 0)


// =================================================================================================
#ifdef __cplusplus
    }
#endif  // __cplusplus
#endif  // __CTKAV_SDC_H__

