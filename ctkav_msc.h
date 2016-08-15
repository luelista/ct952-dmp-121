
#ifndef __CTKAV_MSC_H__
#define __CTKAV_MSC_H__
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
#define REG_MSC_BASE                            (0xa0001080L)
// -------------------------------------------------------------------------------------------------
#define REG_MSC_DMA_ADDR                        (*(volatile DWORD*)(REG_MSC_BASE+0x00))
#define REG_MSC_TRAN_MODE                       (*(volatile DWORD*)(REG_MSC_BASE+0x04))
#define REG_MSC_CMD                             (*(volatile DWORD*)(REG_MSC_BASE+0x08))
#define REG_MSC_DATA_PORT                       (*(volatile DWORD*)(REG_MSC_BASE+0x0c))
#define REG_MSC_DATA(n)                         (*(volatile DWORD*)(REG_MSC_BASE+0x10+4*(n)))
#define REG_MSC_DATA0                           REG_MSC_DATA(0)
#define REG_MSC_DATA1                           REG_MSC_DATA(1)
#define REG_MSC_INT_STAT                        (*(volatile  WORD*)(REG_MSC_BASE+0x1a))
#define REG_MSC_INT_STAT_ENABLE                 (*(volatile  WORD*)(REG_MSC_BASE+0x1c))
#define REG_MSC_INT_ENABLE                      (*(volatile  WORD*)(REG_MSC_BASE+0x1e))
#define REG_MSC_HOST_CTRL                       (*(volatile  BYTE*)(REG_MSC_BASE+0x20))
#define REG_MSC_WAKEUP_CTRL                     (*(volatile  BYTE*)(REG_MSC_BASE+0x21))
#define REG_MSC_SW_RESET                        (*(volatile  BYTE*)(REG_MSC_BASE+0x22))
#define REG_MSC_TIMEOUT_CTRL                    (*(volatile  BYTE*)(REG_MSC_BASE+0x23))
#define REG_MSC_CLK_CTRL                        (*(volatile DWORD*)(REG_MSC_BASE+0x24))
#define REG_MSC_STAT                            (*(volatile DWORD*)(REG_MSC_BASE+0x28))

// =================================================================================================
// REG_MSC_TRAN_MODE
#define MSC_TRAN_MODE_BLK_COUNT(n)              ((DWORD)((n) & 0xffff) << 16)
#define MSC_TRAN_MODE_DMA                       (1 << 15)
#define MSC_TRAN_MODE_DMA_BUFF_BND(n)           ((DWORD)((n) & 7) << 12)
#define MSC_TRAN_MODE_DMA_BUFF_BND_MASK         MSC_TRAN_MODE_DMA_BUFF_BND(-1)
#define MSC_TRAN_MODE_DMA_BUFF_BND_512K         MSC_TRAN_MODE_DMA_BUFF_BND(7)
#define MSC_TRAN_MODE_DMA_BUFF_BND_256K         MSC_TRAN_MODE_DMA_BUFF_BND(6)
#define MSC_TRAN_MODE_DMA_BUFF_BND_128K         MSC_TRAN_MODE_DMA_BUFF_BND(5)
#define MSC_TRAN_MODE_DMA_BUFF_BND_64K          MSC_TRAN_MODE_DMA_BUFF_BND(4)
#define MSC_TRAN_MODE_DMA_BUFF_BND_32K          MSC_TRAN_MODE_DMA_BUFF_BND(3)
#define MSC_TRAN_MODE_DMA_BUFF_BND_16K          MSC_TRAN_MODE_DMA_BUFF_BND(2)
#define MSC_TRAN_MODE_DMA_BUFF_BND_8K           MSC_TRAN_MODE_DMA_BUFF_BND(1)
#define MSC_TRAN_MODE_DMA_BUFF_BND_4K           MSC_TRAN_MODE_DMA_BUFF_BND(0)
#define MSC_TRAN_MODE_WRITE                     (1 << 11)
#define MSC_TRAN_MODE_BLK_SIZE(n)               ((DWORD)(n) & 0x3ff)

// -------------------------------------------------------------------------------------------------
// REG_MSC_CMD
#define MSC_CMD_TRAN_BYTE_COUNT(n)              ((DWORD)((n)&7)<<12)
#define MSC_CMD_TRAN_DATA_REG                   (1 << 9)
#define MSC_CMD_TRAN_WRITE                      (1 << 8)
#define MSC_CMD_TPC(n)                          ((DWORD)(n)&0xff)

// -------------------------------------------------------------------------------------------------
// REG_MSC_INT_STAT, REG_MSC_INT_STAT_ENABLE, REG_MSC_INT_ENABLE
#define MSC_INT_ERR                             (0x1f << 11)
#define MSC_INT_ERR_CURR_LIMIT                  (1 << 15)
#define MSC_INT_ERR_BUFF_ACCESS                 (1 << 14)
#define MSC_INT_ERR_WRITE_CMD                   (1 << 13)
#define MSC_INT_ERR_CRC                         (1 << 12)
#define MSC_INT_ERR_TIMEOUT                     (1 << 11)
#define MSC_INT_TRAN_COMPLETE                   (1 << 10)
#define MSC_INT_BUFF_READ_RDY                   (1 << 9)
#define MSC_INT_BUFF_WRITE_RDY                  (1 << 8)
#define MSC_INT_CARD_REM                        (1 << 7)
#define MSC_INT_CARD_INS                        (1 << 6)
#define MSC_INT_DMA                             (1 << 5)
#define MSC_INT_CMD_COMPLETE                    (1 << 4)
#define MSC_INT_INTREG_CMDNK                    (1 << 3)
#define MSC_INT_INTREG_BREQ                     (1 << 2)
#define MSC_INT_INTREG_ERR                      (1 << 1)
#define MSC_INT_INTREG_CED                      (1 << 0)
#define MSC_INT_INTREG_SIG                      (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_MSC_HOST_CTRL
#define MSC_HOST_CTRL_LITTLE_ENDIAN             (1 << 4)
#define MSC_HOST_CTRL_BUS_PW_ON                 (1 << 3)
#define MSC_HOST_CTRL_SDIO_TURN_AROUND          (1 << 2)
#define MSC_HOST_CTRL_4BIT                      (1 << 1)
#define MSC_HOST_CTRL_LED_ON                    (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_MSC_WAKEUP_CTRL
#define MSC_WAKEUP_CTRL_CARD_REM                (1 << 5)
#define MSC_WAKEUP_CTRL_CARD_INS                (1 << 4)
#define MSC_WAKEUP_CTRL_CARD_INT(n)             (1 << ((n) & 3))

// -------------------------------------------------------------------------------------------------
// REG_MSC_SW_RESET
#define MSC_SW_RESET_ALL                        (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_MSC_TIMEOUT_CTRL
#define MSC_TIMEOUT_CTRL_DATA_TIMEOUT(n)        ((n) & 0xf)

// -------------------------------------------------------------------------------------------------
// REG_MSC_CLK_CTRL
#define MSC_CLK_CTRL_MSCLK_FREQ_SEL(n)          (((DWORD)(n)&0xf)<<4)
#define MSC_CLK_CTRL_MSCLK_ENABLE               (1 << 2)
#define MSC_CLK_CTRL_INCLK_STABLE               (1 << 1)
#define MSC_CLK_CTRL_INCLK_ENABLE               (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_MSC_STAT
#define MSC_STAT_CMD_SIG                        (1 << 24)
#define MSC_STAT_DAT_SIG(n)                     (1 << (20 + (n)))
#define MSC_STAT_DAT_SIG0                       MSC_STAT_DAT_SIG(0)
#define MSC_STAT_DAT_SIG1                       MSC_STAT_DAT_SIG(1)
#define MSC_STAT_DAT_SIG2                       MSC_STAT_DAT_SIG(2)
#define MSC_STAT_DAT_SIG3                       MSC_STAT_DAT_SIG(3)
#define MSC_STAT_CD_PIN                         (1 << 18)
#define MSC_STAT_CARD_STAT_STABLE               (1 << 17)
#define MSC_STAT_CARD_INS                       (1 << 16)
#define MSC_STAT_BUFF_READ_ENABLE               (1 << 11)
#define MSC_STAT_BUFF_WRITE_ENABLE              (1 << 10)
#define MSC_STAT_READ_TRAN_ACTIVE               (1 << 9)
#define MSC_STAT_WRITE_TRAN_ACTIVE              (1 << 8)
#define MSC_STAT_INTERFACE_BUSY                 (1 << 2)


// =================================================================================================
#ifdef __cplusplus
    }
#endif  // __cplusplus
#endif  // __CTKAV_MSC_H__
