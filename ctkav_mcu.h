#ifndef __CTKAV_MCU_H__
#define __CTKAV_MCU_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define REG_MCU_BASE        (CT909_IO_START+0x880)

#define REG_MCU_MR0         (*((volatile DWORD *) (REG_MCU_BASE+0x000)))    // MCU Configuration Register
#define REG_MCU_MCR1        (*((volatile DWORD *) (REG_MCU_BASE+0x004)))    // SDRAM Base Address
#define REG_MCU_MCR2        (*((volatile DWORD *) (REG_MCU_BASE+0x008)))    // SDRAM AC Timing Setting
#define REG_MCU_MCR3        (*((volatile DWORD *) (REG_MCU_BASE+0x00c)))    // DMA Refresh/Initialization Register
#define REG_MCU_MCR4        (*((volatile DWORD *) (REG_MCU_BASE+0x010)))    // DMA Arbiter Control Register
#define REG_MCU_MCR5        (*((volatile DWORD *) (REG_MCU_BASE+0x014)))    // DMA channel FIFO clear Register
                                                                            // D13 BIU Read, D12 BIU Write
#define REG_MCU_MCR6        (*((volatile DWORD *) (REG_MCU_BASE+0x018)))    // Interrupt Control Register 1
#define REG_MCU_MCR7        (*((volatile DWORD *) (REG_MCU_BASE+0x01c)))    // Interrupt Control Register 2
#define REG_MCU_CCR00       (*((volatile DWORD *) (REG_MCU_BASE+0x020)))    // Clean Buffer Write Channel Base Address
#define REG_MCU_CCR01       (*((volatile DWORD *) (REG_MCU_BASE+0x024)))    // Clean Buffer Fill Data and Byte Number

#define REG_MCU_ACR40       (*((volatile DWORD *) (REG_MCU_BASE+0x060)))    // AIU MIC2 Write Channel Base Address
#define REG_MCU_ACR41       (*((volatile DWORD *) (REG_MCU_BASE+0x064)))    // AIU MIC2 Write Channel Write Address
#define REG_MCU_ACR42       (*((volatile DWORD *) (REG_MCU_BASE+0x068)))    // AIU MIC2 Write Address Upper Boundary
#define REG_MCU_ACR43       (*((volatile DWORD *) (REG_MCU_BASE+0x06c)))    // AIU MIC2 Write Address Lower Boundary
#define REG_MCU_ACR44       (*((volatile DWORD *) (REG_MCU_BASE+0x070)))    // AIU MIC2 Write Channel FIFO Status

#define REG_MCU_ACR00       (*((volatile DWORD *) (REG_MCU_BASE+0x080)))    // AIU SPDIF Write Channel Base Address of SPDIF_BUF 1
#define REG_MCU_ACR01       (*((volatile DWORD *) (REG_MCU_BASE+0x084)))    // AIU SPDIF Write Channel Write Address
#define REG_MCU_ACR02       (*((volatile DWORD *) (REG_MCU_BASE+0x088)))    // AIU SPDIF Write Address Upper Boundary of SPDIF_BUF 1
#define REG_MCU_ACR03       (*((volatile DWORD *) (REG_MCU_BASE+0x08c)))    // AIU SPDIF Write Address Lower Boundary of SPDIF_BUF 1
#define REG_MCU_ACR04       (*((volatile DWORD *) (REG_MCU_BASE+0x090)))    // AIU SPDIF Write Channel FIFO Status Register

#define REG_MCU_ACR08       (*((volatile DWORD *) (REG_MCU_BASE+0x0a0)))    // AIU Unpacker0 Read Channel Base Address
#define REG_MCU_ACR09       (*((volatile DWORD *) (REG_MCU_BASE+0x0a4)))    // AIU Unpacker0 Read Channel Read Address
#define REG_MCU_ACR0A       (*((volatile DWORD *) (REG_MCU_BASE+0x0a8)))    // AIU Unpacker0 Read Address Upper Boundary
#define REG_MCU_ACR0B       (*((volatile DWORD *) (REG_MCU_BASE+0x0ac)))    // AIU Unpacker0 Read Address Lower Boundary
#define REG_MCU_ACR0C       (*((volatile DWORD *) (REG_MCU_BASE+0x0b0)))    // AIU Unpacker0 Read Channel FIFO Status Register
#define REG_MCU_ACR10       (*((volatile DWORD *) (REG_MCU_BASE+0x0c0)))    // AIU Unpacker1 Read Channel Base Address
#define REG_MCU_ACR11       (*((volatile DWORD *) (REG_MCU_BASE+0x0c4)))    // AIU Unpacker1 Read Channel Read Address
#define REG_MCU_ACR12       (*((volatile DWORD *) (REG_MCU_BASE+0x0c8)))    // AIU Unpacker1 Read Address Upper Boundary
#define REG_MCU_ACR13       (*((volatile DWORD *) (REG_MCU_BASE+0x0cc)))    // AIU Unpacker1 Read Address Lower Boundary
#define REG_MCU_ACR14       (*((volatile DWORD *) (REG_MCU_BASE+0x0d0)))    // AIU Unpacker1 Read Channel FIFO Status Register
#define REG_MCU_ACR18       (*((volatile DWORD *) (REG_MCU_BASE+0x0e0)))    // AIU PCM Read Channel Base Address
#define REG_MCU_ACR19       (*((volatile DWORD *) (REG_MCU_BASE+0x0e4)))    // AIU PCM Read Channel Read Address
#define REG_MCU_ACR1A       (*((volatile DWORD *) (REG_MCU_BASE+0x0e8)))    // AIU PCM Read Address Upper Boundary
#define REG_MCU_ACR1B       (*((volatile DWORD *) (REG_MCU_BASE+0x0ec)))    // AIU PCM Read Address Lower Boundary
#define REG_MCU_ACR1C       (*((volatile DWORD *) (REG_MCU_BASE+0x0f0)))    // AIU PCM Read Channel FIFO Status Register
#define REG_MCU_ACR20       (*((volatile DWORD *) (REG_MCU_BASE+0x100)))    // AIU SPDIF Read Channel Base Address
#define REG_MCU_ACR21       (*((volatile DWORD *) (REG_MCU_BASE+0x104)))    // AIU SPDIF Read Channel Read Address
#define REG_MCU_ACR22       (*((volatile DWORD *) (REG_MCU_BASE+0x108)))    // AIU SPDIF Read Address Upper Boundary
#define REG_MCU_ACR23       (*((volatile DWORD *) (REG_MCU_BASE+0x10c)))    // AIU SPDIF Read Address Lower Boundary
#define REG_MCU_ACR24       (*((volatile DWORD *) (REG_MCU_BASE+0x110)))    // AIU SPDIF Read Channel FIFO Status Register
#define REG_MCU_ACR28       (*((volatile DWORD *) (REG_MCU_BASE+0x120)))    // AIU Write Channel Base Address
#define REG_MCU_ACR29       (*((volatile DWORD *) (REG_MCU_BASE+0x124)))    // AIU Write Channel Write Address
#define REG_MCU_ACR2A       (*((volatile DWORD *) (REG_MCU_BASE+0x128)))    // AIU Write Address Upper Boundary
#define REG_MCU_ACR2B       (*((volatile DWORD *) (REG_MCU_BASE+0x12c)))    // AIU Write Address Lower Boundary
#define REG_MCU_ACR2C       (*((volatile DWORD *) (REG_MCU_BASE+0x130)))    // AIU Write Channel FIFO Status Register
#define REG_MCU_ACR30       (*((volatile DWORD *) (REG_MCU_BASE+0x140)))    // AIU Read Channel Base Address
#define REG_MCU_ACR31       (*((volatile DWORD *) (REG_MCU_BASE+0x144)))    // AIU Read Channel Read Address
#define REG_MCU_ACR32       (*((volatile DWORD *) (REG_MCU_BASE+0x148)))    // AIU Read Address Upper Boundary
#define REG_MCU_ACR33       (*((volatile DWORD *) (REG_MCU_BASE+0x14c)))    // AIU Read Address Lower Boundary
#define REG_MCU_ACR34       (*((volatile DWORD *) (REG_MCU_BASE+0x150)))    // AIU Read Channel FIFO Status Register

#define REG_MCU_ACR35       (*((volatile DWORD *) (REG_MCU_BASE+0x154)))    // AIU Read Channel x, y direction increment
#define REG_MCU_ACR36       (*((volatile DWORD *) (REG_MCU_BASE+0x160)))    // AIU MIC Read Channel Base Address
#define REG_MCU_ACR37       (*((volatile DWORD *) (REG_MCU_BASE+0x164)))    // AIU MIC Read Channel current Read Address
#define REG_MCU_ACR38       (*((volatile DWORD *) (REG_MCU_BASE+0x168)))    // AIU MIC Read Address Upper Boundary
#define REG_MCU_ACR39       (*((volatile DWORD *) (REG_MCU_BASE+0x16c)))    // AIU MIC Read Address Lower Boundary
#define REG_MCU_ACR3A       (*((volatile DWORD *) (REG_MCU_BASE+0x170)))    // AIU MIC Read Channel FIFO Status
#define REG_MCU_ACR3B       (*((volatile DWORD *) (REG_MCU_BASE+0x174)))    // AIU MIC Read Channel read address offset

// BIU write channel
#define REG_MCU_BCR00       (*((volatile DWORD *) (REG_MCU_BASE+0x180)))    // BIU write channel base address
#define REG_MCU_BCR00_ADDR  ((PDWORD) (REG_MCU_BASE+0x180))                 // BIU write channel base address
#define REG_MCU_BCR01       (*((volatile DWORD *) (REG_MCU_BASE+0x184)))    // BIU write channel current write address
#define REG_MCU_BCR01_ADDR  ((PDWORD) (REG_MCU_BASE+0x184))                 // BIU write channel current write address
#define REG_MCU_BCR02       (*((volatile DWORD *) (REG_MCU_BASE+0x188)))    // BIU write channel upper boundary
#define REG_MCU_BCR03       (*((volatile DWORD *) (REG_MCU_BASE+0x18c)))    // BIU write channel lower boundary
#define REG_MCU_BCR04       (*((volatile DWORD *) (REG_MCU_BASE+0x190)))    // BIU write channel FIFO status


// BIU bit stream read channel for ATAPI (linear buffer)
#define REG_MCU_BCR08       (*((volatile DWORD *) (REG_MCU_BASE+0x1a0)))    // BIU bit stream read channel base address
#define REG_MCU_BCR08_ADDR  ((PDWORD) (REG_MCU_BASE+0x1a0))                 // BIU bit stream read channel base address
#define REG_MCU_BCR09       (*((volatile DWORD *) (REG_MCU_BASE+0x1a4)))    // BIU bit stream read channel current read address
#define REG_MCU_BCR09_ADDR  ((PDWORD) (REG_MCU_BASE+0x1a4))                 // BIU bit stream read channel current read address
#define REG_MCU_BCR0A       (*((volatile DWORD *) (REG_MCU_BASE+0x1a8)))    // BIU bit stream read channel x, y direction increment
#define REG_MCU_BCR0B       (*((volatile DWORD *) (REG_MCU_BASE+0x1ac)))    // BIU bit stream read channel x, y direction sub-increment
// remainder is at [19:16], range from 0 to 15
#define REG_MCU_BCR0C       (*((volatile DWORD *) (REG_MCU_BASE+0x1b0)))    // BIU bit stream read channel FIFO status
#define REG_MCU_BCR0D       (*((volatile DWORD *) (REG_MCU_BASE+0x1b4)))    // [31:30] bsrdtype "10", [23:16] bsrdheight "1",
                                                                            // [9:0] bsrdwidth (i.e. # of 64-bit)
#define REG_MCU_BCR0D_ADDR  ((PDWORD) (REG_MCU_BASE+0x1b4))                 // [31:30] bsrdtype "10", [23:16] bsrdheight "1",
#define REG_MCU_BCR0E       (*((volatile DWORD *) (REG_MCU_BASE+0x1b8)))    // BIU Bit-Stream Read Channel Sub-Block Width/Height

#define REG_MCU_SW_TRIGGER  (*((volatile DWORD *) (REG_MCU_BASE+0x1bc)))    // trigger DMA starting

#define REG_MCU_BCR10       (*((volatile DWORD *) (REG_MCU_BASE+0x1c0)))    // ATAPI Write Channel Base Address
#define REG_MCU_BCR11       (*((volatile DWORD *) (REG_MCU_BASE+0x1c4)))    // ATAPI Write Channel Write Address
#define REG_MCU_BCR12       (*((volatile DWORD *) (REG_MCU_BASE+0x1c8)))    // ATAPI Write Channel Write Count
#define REG_MCU_BCR13       (*((volatile DWORD *) (REG_MCU_BASE+0x1cc)))    // ATAPI Write Channel X Direction Increment
#define REG_MCU_BCR14       (*((volatile DWORD *) (REG_MCU_BASE+0x1d0)))    // ATAPI Write Channel FIFO Status Register

#define REG_MCU_DCR00       (*((volatile DWORD *) (REG_MCU_BASE+0x380)))    // VDEC VLD Read Channel Base Address
#define REG_MCU_DCR01       (*((volatile DWORD *) (REG_MCU_BASE+0x384)))    // VDEC VLD Read Channel Read Address
#define REG_MCU_DCR02       (*((volatile DWORD *) (REG_MCU_BASE+0x388)))    // VDEC VLD Read Channel upper boundary
#define REG_MCU_DCR03       (*((volatile DWORD *) (REG_MCU_BASE+0x38c)))    // VDEC VLD Read Channel lower boundary
#define REG_MCU_DCR04       (*((volatile DWORD *) (REG_MCU_BASE+0x390)))    // VDEC VLD Read Channel FIFO Status

#define REG_MCU_VCR00       (*((volatile DWORD *) (REG_MCU_BASE+0x480)))    // VOU SP Top Read Channel Base Address of SP1
#define REG_MCU_VCR01       (*((volatile DWORD *) (REG_MCU_BASE+0x484)))    // VOU SP Top Read Channel Read Address
#define REG_MCU_VCR02       (*((volatile DWORD *) (REG_MCU_BASE+0x488)))    // VOU SP Read Address Upper Boundary of SP1
#define REG_MCU_VCR03       (*((volatile DWORD *) (REG_MCU_BASE+0x48c)))    // VOU SP Read Address Lower Boundary of SP1
#define REG_MCU_VCR04       (*((volatile DWORD *) (REG_MCU_BASE+0x490)))    // VOU SP Top Read Channel FIFO Status Register
#define REG_MCU_VCR05       (*((volatile DWORD *) (REG_MCU_BASE+0x494)))    // VOU SP Top Read Channel Base Address of SP2
#define REG_MCU_VCR06       (*((volatile DWORD *) (REG_MCU_BASE+0x498)))    // VOU SP Read Address Upper Boundary of SP2
#define REG_MCU_VCR07       (*((volatile DWORD *) (REG_MCU_BASE+0x49c)))    // VOU SP Read Address Lower Boundary of SP2
#define REG_MCU_VCR08       (*((volatile DWORD *) (REG_MCU_BASE+0x4a0)))    // VOU SP Bottom Read Channel Base Address of SP1
#define REG_MCU_VCR09       (*((volatile DWORD *) (REG_MCU_BASE+0x4a4)))    // VOU SP Bottom Read Channel Read Address
#define REG_MCU_VCR0C       (*((volatile DWORD *) (REG_MCU_BASE+0x4b0)))    // VOU SP Bottom Read Channel FIFO Status Register
#define REG_MCU_VCR0D       (*((volatile DWORD *) (REG_MCU_BASE+0x4b4)))    // VOU SP Bottom Read Channel Base Address of SP2
#define REG_MCU_VCR10       (*((volatile DWORD *) (REG_MCU_BASE+0x4c0)))    // VOU SP Color Data Read Channel Base Address of SP1
#define REG_MCU_VCR11       (*((volatile DWORD *) (REG_MCU_BASE+0x4c4)))    // VOU SP Color Data Read Channel Read Address
#define REG_MCU_VCR14       (*((volatile DWORD *) (REG_MCU_BASE+0x4d0)))    // VOU SP Color Data Read Channel FIFO Status Register
#define REG_MCU_VCR15       (*((volatile DWORD *) (REG_MCU_BASE+0x4d4)))    // VOU SP Color Data Read Channel Base Address of SP2
#define REG_MCU_VCR18       (*((volatile DWORD *) (REG_MCU_BASE+0x4e0)))    // VOU Teletext Read Channel Base Address
#define REG_MCU_VCR19       (*((volatile DWORD *) (REG_MCU_BASE+0x4e4)))    // VOU Teletext Read Channel Read Address
#define REG_MCU_VCR1A       (*((volatile DWORD *) (REG_MCU_BASE+0x4e8)))    // VOU Teletext Read Address Upper Boundary
#define REG_MCU_VCR1B       (*((volatile DWORD *) (REG_MCU_BASE+0x4ec)))    // VOU Teletext Read Address Lower Boundary
#define REG_MCU_VCR1C       (*((volatile DWORD *) (REG_MCU_BASE+0x4f0)))    // VOU Teletext Read Channel FIFO Status Register
#define REG_MCU_VCR20       (*((volatile DWORD *) (REG_MCU_BASE+0x500)))    // VOU OSD Read Channel Base Address
#define REG_MCU_VCR21       (*((volatile DWORD *) (REG_MCU_BASE+0x504)))    // VOU OSD Read Channel Base Address
#define REG_MCU_VCR22       (*((volatile DWORD *) (REG_MCU_BASE+0x508)))    // VOU OSD Read Channel Width/Height
#define REG_MCU_VCR23       (*((volatile DWORD *) (REG_MCU_BASE+0x50c)))    // VOU OSD Read Channel X, Y Direction Increment
#define REG_MCU_VCR24       (*((volatile DWORD *) (REG_MCU_BASE+0x510)))    // VOU OSD Read Channel FIFO Status Register
#define REG_MCU_VCR25       (*((volatile DWORD *) (REG_MCU_BASE+0x51c)))    // VOU OSD upscalling register

#define REG_MCU_SCR10       (*((volatile DWORD *) (REG_MCU_BASE+0x5c0)))    // SERVO EDC Read Channel Base Address
#define REG_MCU_SCR11       (*((volatile DWORD *) (REG_MCU_BASE+0x5c4)))    // SERVO EDC Read Channel Read Address
#define REG_MCU_SCR12       (*((volatile DWORD *) (REG_MCU_BASE+0x5c8)))    // SERVO EDC Read Channel X, Y Direction Increment
#define REG_MCU_SCR13       (*((volatile DWORD *) (REG_MCU_BASE+0x5cc)))    // SERVO EDC Read Channel X, Y Direction Sub-Increment
#define REG_MCU_SCR14       (*((volatile DWORD *) (REG_MCU_BASE+0x5d0)))    // SERVO EDC Read Channel FIFO Status Register
#define REG_MCU_SCR15       (*((volatile DWORD *) (REG_MCU_BASE+0x5d4)))    // SERVO EDC Read Channel Width/Height
#define REG_MCU_SCR16       (*((volatile DWORD *) (REG_MCU_BASE+0x5d8)))    // SERVO EDC Read Channel Sub-Block Width/Height
#define REG_MCU_SCR18       (*((volatile DWORD *) (REG_MCU_BASE+0x5e0)))    // SERVO ECC Read Channel Base Address
#define REG_MCU_SCR19       (*((volatile DWORD *) (REG_MCU_BASE+0x5e4)))    // SERVO ECC Read Channel Base Address
#define REG_MCU_SCR1A       (*((volatile DWORD *) (REG_MCU_BASE+0x5e8)))    // SERVO ECC Read Channel X, Y Direction Increment
#define REG_MCU_SCR1B       (*((volatile DWORD *) (REG_MCU_BASE+0x5ec)))    // SERVO ECC Read Channel X, Y Direction Sub-Increment
#define REG_MCU_SCR1C       (*((volatile DWORD *) (REG_MCU_BASE+0x5f0)))    // SERVO ECC Read Channel FIFO Status Register
#define REG_MCU_SCR1D       (*((volatile DWORD *) (REG_MCU_BASE+0x5f4)))    // SERVO ECC Channel Setting / Q-Parity Offset
#define REG_MCU_SCR1E       (*((volatile DWORD *) (REG_MCU_BASE+0x5f8)))    // SERVO ECC Read Channel Width/Height
#define REG_MCU_SCR1F       (*((volatile DWORD *) (REG_MCU_BASE+0x5fc)))    // SERVO ECC Read Channel Sub-Block Width/Height


// Write Protection Registers
// Area 0~3 won't let data write to dram but area 4~5 will.
#define REG_MCU_WPROTEN     (*((volatile DWORD *) (REG_MCU_BASE+0x580)))    // write protection enable register
#define REG_MCU_WP0BASE     (*((volatile DWORD *) (REG_MCU_BASE+0x584)))    // write protection area 0 address base address, 64-bit alignment
#define REG_MCU_WP0SIZE     (*((volatile DWORD *) (REG_MCU_BASE+0x588)))    // write protection area 0 address upper boundary, 64-bit alignment
#define REG_MCU_WP1BASE     (*((volatile DWORD *) (REG_MCU_BASE+0x58c)))    // write protection area 1 address base address, 64-bit alignment
#define REG_MCU_WP1SIZE     (*((volatile DWORD *) (REG_MCU_BASE+0x590)))    // write protection area 1 address upper boundary, 64-bit alignment
#define REG_MCU_WP2BASE     (*((volatile DWORD *) (REG_MCU_BASE+0x594)))    // write protection area 2 address base address, 64-bit alignment
#define REG_MCU_WP2SIZE     (*((volatile DWORD *) (REG_MCU_BASE+0x598)))    // write protection area 2 address upper boundary, 64-bit alignment
#define REG_MCU_WP3BASE     (*((volatile DWORD *) (REG_MCU_BASE+0x59c)))    // write protection area 3 address base address, 64-bit alignment
#define REG_MCU_WP3SIZE     (*((volatile DWORD *) (REG_MCU_BASE+0x5a0)))    // write protection area 3 address upper boundary, 64-bit alignment
#define REG_MCU_WP4BASE     (*((volatile DWORD *) (REG_MCU_BASE+0x5a4)))    // write protection area 4 address base address, 64-bit alignment
#define REG_MCU_WP4SIZE     (*((volatile DWORD *) (REG_MCU_BASE+0x5a8)))    // write protection area 4 address upper boundary, 64-bit alignment
#define REG_MCU_WP5BASE     (*((volatile DWORD *) (REG_MCU_BASE+0x5ac)))    // write protection area 5 address base address, 64-bit alignment
#define REG_MCU_WP5SIZE     (*((volatile DWORD *) (REG_MCU_BASE+0x5b0)))    // write protection area 5 address upper boundary, 64-bit alignment
#define REG_MCU_WPSTATUS    (*((volatile DWORD *) (REG_MCU_BASE+0x5b4)))    // write protection status register

#define PREG_MCU_WP0BASE    ((volatile DWORD *) (REG_MCU_BASE+0x584))
#define PREG_MCU_WP0SIZE    ((volatile DWORD *) (REG_MCU_BASE+0x588))


#define REG_MCU_REMEN       (*((volatile DWORD *) (REG_MCU_BASE+0x680)))    // Buffer Remainder Enable Register
#define REG_MCU_REMEN_ADDR  ((PDWORD) (REG_MCU_BASE+0x680))                 // Buffer Remainder Enable Register

#define REG_MCU_VEDIO_REMCTL      (*((volatile DWORD *) (REG_MCU_BASE+0x684)))    // Vedio Buffer Remainder Control Register
#define REG_MCU_VEDIO_REMCTL_ADDR ((PDWORD) (REG_MCU_BASE+0x684))                 // Vedio Buffer Remainder Control Register
#define REG_MCU_AUDIO_REMCTL      (*((volatile DWORD *) (REG_MCU_BASE+0x688)))    // Audio Buffer Remainder Control Register
#define REG_MCU_AUDIO_REMCTL_ADDR ((PDWORD) (REG_MCU_BASE+0x688))                 // Audio Buffer Remainder Control Register

#define REG_MCU_A0REM       (*((volatile DWORD *) (REG_MCU_BASE+0x690)))    // Audio Bit-stream Buffer 0 Remainder
#define REG_MCU_A0REM_ADDR  ((PDWORD) (REG_MCU_BASE+0x690))                 // Audio Bit-stream Buffer 0 Remainder
#define REG_MCU_A0OTH       (*((volatile DWORD *) (REG_MCU_BASE+0x694)))    // Audio Bit-stream Buffer 0 Overflow Threshold
#define REG_MCU_A0UTH       (*((volatile DWORD *) (REG_MCU_BASE+0x698)))    // Audio Bit-stream Buffer 0 Underflow Threshold
#define REG_MCU_A1REM       (*((volatile DWORD *) (REG_MCU_BASE+0x6a0)))    // Audio Bit-stream Buffer 1 Remainder
#define REG_MCU_A1REM_ADDR  ((PDWORD) (REG_MCU_BASE+0x6a0))                 // Audio Bit-stream Buffer 1 Remainder
#define REG_MCU_A1OTH       (*((volatile DWORD *) (REG_MCU_BASE+0x6a4)))    // Audio Bit-stream Buffer 1 Overflow Threshold
#define REG_MCU_A1UTH       (*((volatile DWORD *) (REG_MCU_BASE+0x6a8)))    // Audio Bit-stream Buffer 1 Underflow Threshold
#define REG_MCU_AADD        (*((volatile DWORD *) (REG_MCU_BASE+0x6ac)))    // Audio Buffer 0 Addend/Subtrahend
#define REG_MCU_VREM        (*((volatile DWORD *) (REG_MCU_BASE+0x6b0)))    // Video Bit-stream Buffer Remainder
#define REG_MCU_VREM_ADDR   ((PDWORD) (REG_MCU_BASE+0x6b0))                 // Video Bit-stream Buffer Remainder
#define REG_MCU_VOTH        (*((volatile DWORD *) (REG_MCU_BASE+0x6b4)))    // Video Bit-stream Buffer Overflow Threshold
#define REG_MCU_VUTH        (*((volatile DWORD *) (REG_MCU_BASE+0x6b8)))    // Video Bit-stream Buffer Underflow Threshold
#define REG_MCU_VADD        (*((volatile DWORD *) (REG_MCU_BASE+0x6bc)))    // Video Buffer 0 Addend/Subtrahend
#define REG_MCU_SP1REM      (*((volatile DWORD *) (REG_MCU_BASE+0x6c0)))    // SP1 Bit-stream Buffer Remainder
#define REG_MCU_SP1REM_ADDR ((PDWORD) (REG_MCU_BASE+0x6c0))                 // SP1 Bit-stream Buffer Remainder
#define REG_MCU_SP1OTH      (*((volatile DWORD *) (REG_MCU_BASE+0x6c4)))    // SP1 Bit-stream Buffer Overflow Threshold

#define REG_MCU_VSETVALUE      (*((volatile DWORD *) (REG_MCU_BASE+0x6c8)))    // Video bit-stream buffer initial setting value

#define REG_MCU_SP1SUB      (*((volatile DWORD *) (REG_MCU_BASE+0x6cc)))    // SP1 Bit-stream Buffer Remainder Subtrahend
#define REG_MCU_SP2REM      (*((volatile DWORD *) (REG_MCU_BASE+0x6d0)))    // SP2 Bit-stream Buffer Remainder
#define REG_MCU_SP2REM_ADDR ((PDWORD) (REG_MCU_BASE+0x6d0))                 // SP2 Bit-stream Buffer Remainder
#define REG_MCU_SP2OTH      (*((volatile DWORD *) (REG_MCU_BASE+0x6d4)))    // SP2 Bit-stream Buffer Overflow Threshold
#define REG_MCU_SP2SUB      (*((volatile DWORD *) (REG_MCU_BASE+0x6dc)))    // SP2 Bit-stream Buffer Remainder Subtrahend
#define REG_MCU_PCMREM      (*((volatile DWORD *) (REG_MCU_BASE+0x6e0)))    // PCM Buffer Remainder
#define REG_MCU_PCMOTH      (*((volatile DWORD *) (REG_MCU_BASE+0x6e4)))    // PCM Buffer Overflow Threshold
#define REG_MCU_PCMUTH      (*((volatile DWORD *) (REG_MCU_BASE+0x6e8)))    // PCM Buffer Underflow Threshold
#define REG_MCU_SPDIFREM    (*((volatile DWORD *) (REG_MCU_BASE+0x6f0)))    // SPDIF Output Buffer Remainder
#define REG_MCU_SPDIFOTH    (*((volatile DWORD *) (REG_MCU_BASE+0x6f4)))    // SPDIF Output Buffer Overflow Threshold
#define REG_MCU_SPDIFUTH    (*((volatile DWORD *) (REG_MCU_BASE+0x6f8)))    // SPDIF Output Buffer Underflow Threshold

#define REG_MCU_SPDIFADD    (*((volatile DWORD *) (REG_MCU_BASE+0x6fc)))    // SPDIF buffer addend/subtrahend

#define REG_MCU_PCM0REM     (*((volatile DWORD *) (REG_MCU_BASE+0x700)))    // PCM Buffer 0 Remainder
#define REG_MCU_PCM0OTH     (*((volatile DWORD *) (REG_MCU_BASE+0x704)))    // PCM Buffer 0 Overflow Threshold
#define REG_MCU_PCM0UTH     (*((volatile DWORD *) (REG_MCU_BASE+0x708)))    // PCM Buffer 0 Underflow Threshold
#define REG_MCU_PCM0ADD     (*((volatile DWORD *) (REG_MCU_BASE+0x70c)))    // PCM Buffer 0 Addend/Subtrahend

#define REG_MCU_STBREM     (*((volatile DWORD *) (REG_MCU_BASE+0x710)))    // STB Buffer Remainder
#define REG_MCU_STBOTH     (*((volatile DWORD *) (REG_MCU_BASE+0x714)))    // STB Buffer Overflow Threshold
#define REG_MCU_STBUTH     (*((volatile DWORD *) (REG_MCU_BASE+0x718)))    // STB Buffer Underflow Threshold

// define write protection is occurred by which module
#define MCU_WRITE_PROTECT_STATUS_SI_WRITE                           (0x001)
#define MCU_WRITE_PROTECT_STATUS_VPU_WRITE                          (0x002)
#define MCU_WRITE_PROTECT_STATUS_MC_WRITE                           (0x004)
#define MCU_WRITE_PROTECT_STATUS_READ_MODIFY_WRITE_CHANNEL_WRITE    (0x008)
#define MCU_WRITE_PROTECT_STATUS_STB_WRITE                          (0x010)
#define MCU_WRITE_PROTECT_STATUS_DUMP_IN_WRITE                      (0x020)
#define MCU_WRITE_PROTECT_STATUS_BIU_WRITE                          (0x040)
#define MCU_WRITE_PROTECT_STATUS_MIC2_WRITE                         (0x080)
#define MCU_WRITE_PROTECT_STATUS_MIC_WRITE                          (0x100)
#define MCU_WRITE_PROTECT_STATUS_AIU_WRITE                          (0x200)
#define MCU_WRITE_PROTECT_STATUS_CLB_WRITE                          (0x400)
#define MCU_WRITE_PROTECT_STATUS_AHB_WRITE                          (0x200)

// define write protection area enable bit
#define MCU_WRITE_PROTECT_AREA0_ENABLE              (0x00000001)
#define MCU_WRITE_PROTECT_AREA1_ENABLE              (0x00000002)
#define MCU_WRITE_PROTECT_AREA2_ENABLE              (0x00000004)
#define MCU_WRITE_PROTECT_AREA3_ENABLE              (0x00000008)
#define MCU_WRITE_PROTECT_AREA4_ENABLE              (0x00000010)
#define MCU_WRITE_PROTECT_AREA5_ENABLE              (0x00000020)
#define MCU_WRITE_PROTECTION_DISABLE_ALL            (0x00000000)
#define MCU_WRITE_PROTECTION_ENABLE_ALL             (0x0000003f)

// define bitstream type of REMCTL
#define MCU_BIT_STREAM_TYPE_MASK                    (0x00000007)
#define MCU_BIT_STREAM_TYPE_UNMASK                  (~MCU_BIT_STREAM_TYPE_MASK)
#define MCU_BIT_STREAM_AUDIO                        (0x00000000)
#define MCU_BIT_STREAM_VIDEO                        (0x00000001)
#define MCU_BIT_STREAM_SP1                          (0x00000002)
#define MCU_BIT_STREAM_SP2                          (0x00000003)
#define MCU_BIT_STREAM_DATA                         (0x00000005)

// define enable bit of REMEM
#define MCU_REMAINDER_ENABLE_AUDIO_BUF0             (0x00000001)
#define MCU_REMAINDER_ENABLE_AUDIO_BUF1             (0x00000002)
#define MCU_REMAINDER_ENABLE_VIDEO_BUF              (0x00000004)
#define MCU_REMAINDER_ENABLE_SP1_BUF                (0x00000008)
#define MCU_REMAINDER_ENABLE_SP2_BUF                (0x00000010)
#define MCU_REMAINDER_ENABLE_PCM_BUF                (0x00000020)
#define MCU_REMAINDER_ENABLE_SPDIF_OUTPUT           (0x00000040)
#define MCU_REMAINDER_ENABLE_PCM0_BUF               (0x00000080)

#define MCU_REMAINDER_ENABLE_STB_BUF                (0x00000100)
#define MCU_REMAINDER_ENABLE_ALL                    (0x000001ff)
#define MCU_A0_REMAINDER_ADD                        (0x00000200)
#define MCU_A0_REMAINDER_SUB                        (0x00000100)
#define MCU_A1_REMAINDER_ADD                        (0x02000000)
#define MCU_A1_REMAINDER_SUB                        (0x01000000)
#define MCU_AADD_REMAINDER_ACRCSET                  (0x10000000) // acrcset : if set (1), the upk0_rdpt-18 => upk1_rdpt, abuf0rem-18 => abuf1rem
#define MCU_SP_REMAINDER_SUB                        (0x00010000)

#define MCU_REMAINDER_ADD                           (0x00000200)
#define MCU_REMAINDER_SUB                           (0x00000100)
#define MCU_REMAINDER_INIT                          (0x01000000)

// define channel select of REMCTL
#define MCU_AIU_CHANNEL_SELECT_MASK                 (0x00007f00)
#define MCU_AIU_CHANNEL_SELECT_UNMASK               (~MCU_AIU_CHANNEL_SELECT_MASK)
#define MCU_AIUWR_CHANNEL_SELECT_MASK               (0x00000300)
#define MCU_AIUWR_CHANNEL_SELECT_UNMASK             (~MCU_AIUWR_CHANNEL_SELECT_MASK)
#define MCU_AIUWR_CHANNEL_SELECT_PCM                (0x00000000)
#define MCU_AIUWR_CHANNEL_SELECT_SPDIF_OUTPUT       (0x00000100)
#define MCU_AIUWR_CHANNEL_SELECT_AUDIO              (0x00000200)
#define MCU_AUDIO_BUF0_CHANNEL_SELECT_MASK          (0x00000c00)
#define MCU_AUDIO_BUF0_CHANNEL_SELECT_UNMASK        (~MCU_AUDIO_BUF0_CHANNEL_SELECT_MASK)
#define MCU_AUDIO_BUF0_CHANNEL_SELECT_BIUWR         (0x00000000)
#define MCU_AUDIO_BUF0_CHANNEL_SELECT_SPDIF_IN      (0x00000400)
#define MCU_AUDIO_BUF0_CHANNEL_SELECT_AIUWR         (0x00000800)
#define MCU_AUDIO_BUF1_CHANNEL_SELECT_MASK          (0x00003000)
#define MCU_AUDIO_BUF1_CHANNEL_SELECT_UNMASK        (~MCU_AUDIO_BUF0_CHANNEL_SELECT_MASK)
#define MCU_AUDIO_BUF1_CHANNEL_SELECT_BIUWR         (0x00000000)

#define MCU_AUDIO_BUF1_CHANNEL_SELECT_AIUWR         (0x00002000)
#define MCU_UNPACKER1_CHANNEL_SELECT_MASK           (0x00004000)
#define MCU_UNPACKER1_CHANNEL_SELECT_UNMASK         (~MCU_UNPACKER1_CHANNEL_SELECT_MASK)
#define MCU_UNPACKER1_CHANNEL_SELECT_AUDIO_BUF1     (0x00000000)
#define MCU_UNPACKER1_CHANNEL_SELECT_PCM0           (0x00004000)

#define MCU_MASK_BIU_BIT_STREAM_FIFO_REMAINDER      (0x00000000)
#define MCU_MASK_BIU_WRITE_CHANNEL_FIFO_REMAINDER   (0x000f0000)
#define MCU_SHIFT_BIU_WRITE_CHANNEL_FIFO_REMAINDER  (16)

#define MCU_BIU_WRITE_CHANNEL_FIFO_REMAINDER        (REG_MCU_BCR04 >> 16)

// DMA FIFO channel FIFO clear Register (MCU_MCR5)
#define MCU_RESET_AIU_MIC_WRITE_FIFO                (0x00000001)
#define MCU_RESET_STB_WRITE_FIFO                    (0x00000400)
#define MCU_RESET_AIU_MIC2_WRITE_FIFO               (0x04000000)
#define MCU_RESET_AIU_UPK0_FIFO                     (0x00000002)
#define MCU_RESET_AIU_UPK1_FIFO                     (0x00000004)
#define MCU_RESET_AIU_PCM_READ_FIFO                 (0x00000008)
#define MCU_RESET_AIU_SPDIF_READ_FIFO               (0x00000010)
#define MCU_RESET_AIU_WRITE_FIFO                    (0x00000020)
#define MCU_RESET_AIU_READ_FIFO                     (0x00000040)
#define MCU_RESET_BIU_WRITE_FIFO                    (0x00000100)
#define MCU_RESET_BIU_READ_FIFO                     (0x00000200)
#define MCU_RESET_VDEC_READ_FIFO                    (0x00001000)
#define MCU_RESET_SP_TOP_FIFO                       (0x00010000)
#define MCU_RESET_SP_BOTTOM_FIFO                    (0x00020000)
#define MCU_RESET_SP_COLOR_FIFO                     (0x00040000)
#define MCU_RESET_TELETEXT_FIFO                     (0x00080000)
#define MCU_RESET_OSD_READ_FIFO                     (0x00100000)
#define MCU_RESET_SERVO_EDC_FIFO                    (0x01000000)
#define MCU_RESET_SERVO_ECC_FIFO                    (0x02000000)

// BIU Bit-Stream Format (MCU_BCR0D)
#define MCU_BSTYPE_DVD                              (0x00000000)
#define MCU_BSTYPE_CD                               (0x40000000)
#define MCU_BSTYPE_ATAPI                            (0x80000000)

// define interrupt enable bit of MCU_MCR6 Interrupt Control Register 1
#define MCU_INTCTL1_AUDIO_BUF0_OVERFLOW_ENABLE      (0x00000001)
#define MCU_INTCTL1_AUDIO_BUF1_OVERFLOW_ENABLE      (0x00000002)
#define MCU_INTCTL1_BIUBR_CHANNEL_DONE_ENABLE       (0x00000004)
#define MCU_INTCTL1_BIUBR_FIFO_FULL_ENABLE          (0x00000008)
#define MCU_INTCTL1_AUDIO_BUF0_OVERFLOW_STATUS      (0x00000010)
#define MCU_INTCTL1_AUDIO_BUF1_OVERFLOW_STATUS      (0x00000020)
#define MCU_INTCTL1_AUDIO_BUF_STATUS_UPDATE         (0x00000040)
#define MCU_INTCTL1_BIUBR_CHANNEL_DONE_STATUS       (0x00000080)
#define MCU_INTCTL1_BIUBR_FIFO_FULL_STATUS          (0x00000100)
#define MCU_INTCTL1_BIUBR_STATUS_UPDATE             (0x00000200)
#define MCU_INTCTL1_BIUBR_STATUS_MASK               (MCU_INTCTL1_BIUBR_FIFO_FULL_STATUS|MCU_INTCTL1_BIUBR_CHANNEL_DONE_STATUS)
#define MCU_INTCTL1_AUDIO_STATUS_MASK               (MCU_INTCTL1_AUDIO_BUF0_OVERFLOW_STATUS|MCU_INTCTL1_AUDIO_BUF1_OVERFLOW_STATUS)

// define interrupt enable bit of MCU_MCR7 Interrupt Control Register 2
#define MCU_INTCTL2_AUDIO_BUF0_UNDERFLOW_ENABLE     (0x00000001)
#define MCU_INTCTL2_AUDIO_BUF1_UNDERFLOW_ENABLE     (0x00000002)
#define MCU_INTCTL2_PCM_BUF0_UNDERFLOW_ENABLE       (0x00000004)
#define MCU_INTCTL2_UNPACK0_FIFO_EMPTY_ENABLE       (0x00000008)
#define MCU_INTCTL2_UNPACK1_FIFO_EMPTY_ENABLE       (0x00000010)
#define MCU_INTCTL2_AIURD_FIFO_EMPTY_ENABLE         (0x00000020)
#define MCU_INTCTL2_AUDIO_BUF0_UNDERFLOW_STATUS     (0x00000040)
#define MCU_INTCTL2_AUDIO_BUF1_UNDERFLOW_STATUS     (0x00000080)
#define MCU_INTCTL2_PCM_BUF0_UNDERFLOW_STATUS       (0x00000100)
#define MCU_INTCTL2_UNPACK0_FIFO_EMPTY_STATUS       (0x00000200)
#define MCU_INTCTL2_UNPACK1_FIFO_EMPTY_STATUS       (0x00000400)
#define MCU_INTCTL2_AIURD_FIFO_EMPTY_STATUS         (0x00000800)

#define MCU_INTCTL2_PCM_BUF_UNDERFLOW_STATUS        (0x00001000)
#define MCU_INTCTL2_SPDIF_BUF_UNDERFLOW_STATUS      (0x00002000)
#define MCU_INTCTL2_AIU_PCM_RD_FIFO_EMPTY_STATUS    (0x00004000)
#define MCU_INTCTL2_AIU_SPDIF_RD_FIFO_EMPTY_STATUS  (0x00008000)
#define MCU_INTCTL2_AIU_MIC_RD_FIFO_EMPTY_STATUS    (0x01000000)
#define MCU_INTCTL2_INT_FLAG_STATUS_UPDATE          (0x00100000)

// define remainder for each buffer
#define MCU_MAX_FIFO_LEN                (0x20)      // maximum FIFO length, 32x32, for each buffer except VLD
#define MCU_VLD_MAX_FIFO_LEN            (0x10)      // maximum FIFO length, 16x32, for VLD
#define MCU_BSRD_FIFO_REMAINDER         ((REG_MCU_BCR0C >> 16) & 0x0000003f)
#define MCU_VLD_FIFO_REMAINDER          ((REG_MCU_DCR04 >> 16) & 0x0000001f) // [4:0]
#define MCU_BSWR_FIFO_REMAINDER         ((REG_MCU_BCR04 >> 16) & 0x0000003f)
#define MCU_AIU_UNPACK0_FIFO_REMAINDER  ((REG_MCU_ACR0C >> 16) & 0x0000003f)
#define MCU_AIU_UNPACK1_FIFO_REMAINDER  ((REG_MCU_ACR14 >> 16) & 0x0000003f)

#define MCU_BUF_EMPTY                   (0x00000000)

#define MCU_A0_BS_BUF_REMAINDER         ((REG_MCU_A0REM    & 0x00ffffff) << 2)
#define MCU_A1_BS_BUF_REMAINDER         ((REG_MCU_A1REM    & 0x00ffffff) << 2)
#define MCU_VIDEO_BS_BUF_REMAINDER      ((REG_MCU_VREM     & 0x00ffffff) << 2)
#define MCU_SP1_BS_BUF_REMAINDER        ((REG_MCU_SP1REM   & 0x00ffffff) << 2)
#define MCU_SP2_BS_BUF_REMAINDER        ((REG_MCU_SP2REM   & 0x00ffffff) << 2)
#define MCU_PCM_BUF_REMAINDER           ((REG_MCU_PCMREM   & 0x00ffffff) << 2)
#define MCU_PCM0_BUF_REMAINDER          ((REG_MCU_PCM0REM  & 0x00ffffff) << 2)
#define MCU_SPDIF_BUF_REMAINDER         ((REG_MCU_SPDIFREM & 0x00ffffff) << 2)

#define MACRO_MCU_SUB_SP1_REMAINDER(dwBytes)                        \
    {   \
        REG_MCU_SP1SUB = (MCU_SP_REMAINDER_SUB | (dwBytes + 3) >> 2);  \
    }
#define MACRO_MCU_SUB_SP2_REMAINDER(dwBytes)                        \
    {                                                               \
        REG_MCU_SP2SUB = (MCU_SP_REMAINDER_SUB | (dwBytes + 3) >> 2);  \
    }

#define MCU_INIT_BUFFER_REMAINDER       (0x01000000)
#define MCU_BUFFER_REMAINDER_MASK       (0x00ffffff)
//#define MCU_REMAINDER_VIDEO_BUF_ADD     (0x00000200)
//#define MCU_REMAINDER_VIDEO_BUF_SUB     (0x00000100)

#define MCU_CHANNEL_DUMMY_LEN           (8)     // this is a dummy length since A?V buffer remainder start from 0xFFFFFFFF (-1)

// define clear buffer
#define MCU_CLEAR_BUFFER_DONE           (~(REG_MCU_CCR01>>31))  // MSB bit indicates clear buffer still doing

// define structure
typedef struct  _tagMCU_TRANSFER
{
    DWORD   dwMemBase;
    DWORD   dwMemUpper;
    DWORD   dwMemLower;
} MCU_TRANSFER, *PMCU_TRANSFER;

// define MACROs
#define MACRO_MCU_RESET_BIU_READ_FIFO   {REG_MCU_MCR5 = MCU_RESET_BIU_READ_FIFO;}
#define MACRO_MCU_RESET_BIU_WRITE_FIFO  {REG_MCU_MCR5 = MCU_RESET_BIU_WRITE_FIFO;}
#define MACRO_MCU_RESET_ALL_FIFO        {REG_MCU_MCR5 = 0xFFFFFFFF;}
#define MACRO_MCU_RESET_AUDIO_FIFO      {REG_MCU_MCR5 = (0x0000007F);}
#define MACRO_MCU_RESET_VIDEO_FIFO      {REG_MCU_MCR5 = MCU_RESET_VDEC_READ_FIFO;}

#define MACRO_MCU_BSINPUT_START         (REG_MCU_SW_TRIGGER = 0x01)

#define MACRO_MCU_SET_BIU_WRITE_BUFFER(dwMemBase, dwMemUpper, dwMemLower)   \
    {   \
        REG_MCU_BCR00 = dwMemBase;   \
        REG_MCU_BCR02 = dwMemUpper - DATA_WIDTH;  \
        REG_MCU_BCR03 = dwMemLower;  \
    }

//==================================================
// iMemAddr is the address starts to read of linear buffer, it should be 64-bit alignment
// iSize is the number of data in unit of 64-bit
//---------------------------------------------
// start to transfer is activated here, so it is necessary to make sure BIU is ready
//==================================================
#define MACRO_MCU_SET_BIU_READ_LINEAR_BUFFER(dwMemAddr, dw32BitSize)     \
    {   \
        REG_MCU_BCR08 = dwMemAddr;  \
        REG_MCU_BCR0D = (MCU_BSTYPE_ATAPI|0x00010000|dw32BitSize);  \
    }

#define MACRO_MCU_SET_BIU_READ_DVD_BLOCK_BUFFER(dwMemAddr)      \
    {   \
        REG_MCU_BCR08 = dwMemAddr;  \
        REG_MCU_BCR0A = 0x00400004; \
        REG_MCU_BCR0D = (MCU_BSTYPE_DVD|0x00c0002b);    \
    }

#define MACRO_MCU_SET_BIU_READ_CD_BLOCK_BUFFER(dwMemAddr, dwNumSectors, dwNumValid32Bit)    \
    {   \
        REG_MCU_BCR08 = dwMemAddr;  \
        REG_MCU_BCR0A = 0x0a000004; \
        REG_MCU_BCR0D = (MCU_BSTYPE_CD|(dwNumSectors << 16)|dwNumValid32Bit);   \
    }

#define MACRO_MCU_BIU_BSTYPE( dwBSType )    \
{   \
    REG_MCU_VEDIO_REMCTL &= MCU_BIT_STREAM_TYPE_UNMASK;   \
    REG_MCU_VEDIO_REMCTL |= dwBSType; \
}

#define MACRO_MCU_BIU_WRITE_FIFO_REMAINDER  (REG_MCU_BCR04 >> 16)

#define MACRO_MCU_WRITE_PROTECTION(dwOrder, dwMemBase, dwMemUpper)  \
    {   \
        *(PREG_MCU_WP0BASE + (dwOrder << 1)) = dwMemBase;   \
        *(PREG_MCU_WP0SIZE + (dwOrder << 1)) = dwMemUpper - DATA_WIDTH;  \
    }

#define MACRO_MCU_WRITE_PROTECTION_ENABLE(dwFlag)   \
{   \
    REG_MCU_WPROTEN = dwFlag;   \
}

//Alan1.01
#define MACRO_MCU_ENABLE_PROTECTION(dwFlag)   \
{   \
    REG_MCU_WPROTEN |= dwFlag;   \
}

//Alan1.01
#define MACRO_MCU_DISABLE_PROTECTION(dwFlag)   \
{   \
    REG_MCU_WPROTEN &= ~dwFlag;   \
}

#define MACRO_MCU_CLEAR_BUFFER(dwMemBase, dwFillBytes, dwFillPattern)   \
{   \
    REG_MCU_CCR00 = dwMemBase;  \
    REG_MCU_CCR01 = (((dwFillBytes) << 8) | dwFillPattern) | 0x80000000;    \
}

#define MACRO_MCU_SET_VLD_READ_BUFFER(dwMemBase, dwMemUpper, dwMemLower)    \
{   \
    REG_MCU_DCR00 = dwMemBase;  \
    REG_MCU_DCR02 = dwMemUpper - DATA_WIDTH; \
    REG_MCU_DCR03 = dwMemLower; \
}

#define MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(dwMemBase, dwMemUpper, dwMemLower)  \
{   \
    REG_MCU_ACR08 = dwMemBase;  \
    REG_MCU_ACR0A = dwMemUpper - DATA_WIDTH; \
    REG_MCU_ACR0B = dwMemLower; \
}

#define MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(dwMemBase, dwMemUpper, dwMemLower)  \
{   \
    REG_MCU_ACR10 = dwMemBase;  \
    REG_MCU_ACR12 = dwMemUpper - DATA_WIDTH; \
    REG_MCU_ACR13 = dwMemLower; \
}

#define MCU_VUTH_VALUE      (5 * 8)
#define MCU_VOTH_VALUE      (0x1000 * 8)

#ifdef __cplusplus
}
#endif //__cplusplus

#endif  // __CTKAV_MCU_H__
