#ifndef __CTKAV_BIU_H__
#define __CTKAV_BIU_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

// define BIU registers
#define REG_BIU_BASE            (CT909_IO_START+0x800)                      // memory address base for BIU

#define REG_BIU_BIUCTL          (*((volatile DWORD *) REG_BIU_BASE))        // BIU Control Register
#define REG_BIU_BIUCTL_ADDR     ((PDWORD) REG_BIU_BASE)                     // BIU Control Register
#define REG_BIU_BIURRD          (*((volatile DWORD *) (REG_BIU_BASE+0x04))) // BIU Data Port for Read
#define REG_BIU_BIUWRD          (*((volatile DWORD *) (REG_BIU_BASE+0x08))) // BIU Data Port for Write
#define REG_BIU_SECTORLEN       (*((volatile DWORD *) (REG_BIU_BASE+0x0c))) // Set Input Sector Length
#define REG_BIU_MEDIAKEYLSB     (*((volatile DWORD *) (REG_BIU_BASE+0x10))) // LSB of Encrypted Disc Key/Media Key
#define REG_BIU_MEDIAKEYMSB     (*((volatile DWORD *) (REG_BIU_BASE+0x14))) // MSB of Encrypted Disc Key/Media Key
#define REG_BIU_IDALBUMLSB      (*((volatile DWORD *) (REG_BIU_BASE+0x18))) // LSB of Encrypted Reference Disc Key/Album ID
#define REG_BIU_IDALBUMMSB      (*((volatile DWORD *) (REG_BIU_BASE+0x1c))) // MSB of Encrypted Reference Disc Key/Album ID
#define REG_BIU_FRAMELENGTH     (*((volatile DWORD *) (REG_BIU_BASE+0x20))) // Input Frame length/keep length
#define REG_BIU_BIUBPCNT        (*((volatile DWORD *) (REG_BIU_BASE+0x24))) // BIU By-pass Mode Counter
#define REG_BIU_BIUBPCNT_ADDR   ((PDWORD) (REG_BIU_BASE+0x24))              // BIU By-pass Mode Counter
#define REG_BIU_BIUBPREM        (*((volatile DWORD *) (REG_BIU_BASE+0x28))) // BIU By-pass Mode Bytes Remainder
#define REG_BIU_BIUBPREM_ADDR   ((PDWORD) (REG_BIU_BASE+0x28))              // BIU By-pass Mode Bytes Remainder
#define REG_BIU_BIUCLIPLEN      (*((volatile DWORD *) (REG_BIU_BASE+0x2c))) // BIU clip length for prefix/suffix of BIU_SECTORLEN
#define REG_BIU_BIUFNPEC        (*((volatile DWORD *) (REG_BIU_BASE+0x30))) // BIU By-pass/Skip Mode Byte counter
#define REG_BIU_BIUBSCNT        (*((volatile DWORD *) (REG_BIU_BASE+0x34))) // BIU By-pass/Skip Mode Word counter
#define REG_BIU_BIUTGTPAT0      (*((volatile DWORD *) (REG_BIU_BASE+0x38))) // BIU target pattern0 to be found
#define REG_BIU_BIUTGTPAT1      (*((volatile DWORD *) (REG_BIU_BASE+0x3c))) // BIU target pattern1 to be found
#define REG_BIU_BIUPATMASK      (*((volatile DWORD *) (REG_BIU_BASE+0x40))) // BIU target pattern mask
#define REG_BIU_BIUTGRAB01      (*((volatile DWORD *) (REG_BIU_BASE+0x44))) // BIU choose which bit should be keep for BIUDT0/1
#define REG_BIU_BIUTGRAB23      (*((volatile DWORD *) (REG_BIU_BASE+0x48))) // BIU choose which bit should be keep for BIUDT2/3
#define REG_BIU_BIUDT0          (*((volatile DWORD *) (REG_BIU_BASE+0x4c))) // BIU data register 0 to keep clipped bits
#define REG_BIU_BIUDT1          (*((volatile DWORD *) (REG_BIU_BASE+0x50))) // BIU data register 1 to keep clipped bits
#define REG_BIU_BIUDT2          (*((volatile DWORD *) (REG_BIU_BASE+0x54))) // BIU data register 2 to keep clipped bits
#define REG_BIU_BIUDT3          (*((volatile DWORD *) (REG_BIU_BASE+0x58))) // BIU data register 3 to keep clipped bits
#define REG_BIU_BIUCGEN         (*((volatile DWORD *) (REG_BIU_BASE+0x5c))) // BIU Clock
#define REG_BIU_BIUCGDIS        (*((volatile DWORD *) (REG_BIU_BASE+0x60))) // BIU data register 3 to keep clipped bits

// define BIU control bits at REG_BIU_BIUCTL
#define BIU_CTL_PATSTS                  (0x80000000)    // status of which pattern to be found
#define BIU_CTL_CNT_EN                  (0x40000000)    // enable contine run after find start code
#define BIU_CTL_OUT_LITTLE_ENDIAN       (0x0c000000)    // enable output at little endian mode BOEdnSel[11]
#define BIU_CTL_IN_LITTLE_ENDIAN        (0x03000000)    // enable input  at little endian mode BIEdnSel[11]
#define BIU_CTL_PADDING                 (0x10000000)    // push last bypass data to DRAM by padding to 64-bit
#define BIU_CTL_BIUSKIP_EN              (0x00008000)    // enable skip data at by-pass mode
#define BIU_CTL_CPRMDSC_EN              (0x00000100)    // enable CPRM descramble mode
#define BIU_CTL_CPPMDSC_EN              (0x00000080)    // enable CPPM descramble mode
#define BIU_CTL_CSSDSC_EN               (0x00000008)    // enable CSS descramble mode
#define BIU_CTL_DVDDSC_EN               (0x00000004)    // enable DVD descramble mode
#define BIU_CTL_BOFF_EN                 (0x00000002)    // enable output FIFO
#define BIU_CTL_BIFF_EN                 (0x00000001)    // enable input FIFO
#define BIU_CTL_CNT_EN                  (0x40000000)    // enable continue run after find start code

// define BIU Operation bits at REG_BIU_BIUCTL
#define BIU_OP_CALC2G2                  (0x20000000)    // start to do C2_G() operation for processing 2nd to 5th data portions
#define BIU_OP_FNP_GO                   (0x00010000)    // start to find next pattern
#define BIU_OP_BIUBP_GO                 (0x00000800)    // start by-pass mode
#define BIU_OP_CALC2D                   (0x00000400)    // start to do C2_D() operation
#define BIU_OP_CALC2G                   (0x00000200)    // start to do C2_G() operation
#define BIU_OP_CALCSSTKEY               (0x00000040)    // start to calculate CSS Title key
#define BIU_OP_CALCSSRDKEY              (0x00000020)    // start to calculate CSS Reference Disc key
#define BIU_OP_CALCSSDKEY               (0x00000010)    // start to calculate CSS Disc key

// define BIU interrupt bits at REG_BIU_BIUCTL
#define BIU_INT_BIUINTCLR               (0x00800000)    // clear all BIU interrupts
#define BIU_INT_CSSERR                  (0x00400000)    // error occurred while calculating CSS Title Key
#define BIU_INT_FNPINT                  (0x00200000)    // find next pattern is done
#define BIU_INT_BPINT                   (0x00100000)    // by-pass mode is done
#define BIU_INT_CSSINT_EN               (0x00080000)    // enable CSS_ERR interrupt
#define BIU_INT_FNPINT_EN               (0x00040000)    // enable find next pattern interrupt
#define BIU_INT_BPINT_EN                (0x00020000)    // enable by-pass mode interrupt
 
#define BIU_MASK_INPUT_FIFO_READY       (0x00001000)
// J500KWShih_121, fixed
#define BIU_MASK_OUTPUT_FIFO_REMAINDER  (0x00004000)
#define BIU_MASK_OUTPUT_FIFO_READY      (0x00002000)

// define BIU status bits at REG_BIU_BIUCTL
#define BIU_STATUS_BOBUFREM             (0x00004000)    // report if there is a 32-bit data at BIU output FIFO
#define BIU_STATUS_MBOWRRDY             (0x00002000)    // report if BIU output FIFO is ready
#define BIU_STATUS_BIURDDRDY            (0x00001000)    // report if BIU is ready for reading data

// define BIU remainder mask
#define BIU_REMAINDER_MASK              (0x03)          // remainder mask bit

#define BIU_COUNTER_MASK                (0x0000ffff)

// define default settings for REG_BIU_FRAMELENGTH
#define BIU_FRAME_DEFAULT               (0x00080008)    // input is 8 bytes --> output is 8 bytes
#define BIU_FRAME_DVD                   (0x00040004)    // input is 4 bytes --> output os 4 bytes


// define default settings for REG_BIU_BIUCLIPLEN
// <data comes from internal servo>
// -- one DVD block is 0xa000 and contains 16 sectors
// -- one CD block is 0xa00 and contains 1 sector only, its format is:
//       [2352 bytes][96 bytes subcode][112 bytes dummy]
//    ** we can clip the tail, subcode+dummy or dummy, through MCU H/W support
// <data comes from ATAPI (DVD-ROM,...)
// -- one block is one sector no matter CD or DVD
#define BIU_DEFAULT_CLIP                (0x00000000)    // no clip at all
#define BIU_DVD_CLIP                    (0x0004000c)    // clip 12 bytes at front and 4 byte at rear
#define BIU_DVD_BLOCK_LEN               (0xa000)        // length of a dvd block
#define BIU_DVD_SECTOR_LEN              (2064)
#define BIU_DVD_DATA_LEN                (2048)
#define BIU_DVD_SECTORS_PER_BLOCK       (16)

#define BIU_CD_BLOCK_LEN                (0xa00)
#define BIU_CD_SECTOR_LEN               (2352)
#define BIU_CD_SUBCODE_LEN              (96)
#define BIU_CD_SECTOR_SUBCODE_LEN       (BIU_CD_SECTOR_LEN+BIU_CD_SUBCODE_LEN)

#define BIU_CD_M1_DATA_CLIP             (0x01200010)    // clip 16+288 bytes at front+rear and data field is 2048 bytes
                                                        // clip tail 208 bytes, subcode+dummy, via MCU H/W
                                                        // (???RESERVED) clip tail 496 bytes, EDC+Space+ECC+subcode+dummy, via MCU H/W
#define BIU_CD_M1_DATA_LEN              (2048)

#define BIU_CD_M2_DATA_CLIP             (0x00000010)    // clip 16 bytes at front and data field is 2336 bytes
                                                        // clip tail 208 bytes, subcode+dummy, via MCU H/W
#define BIU_CD_M2_DATA_LEN              (2336)

#define BIU_CD_M2F1_SECTOR_LEN          (2072)

#define BIU_CD_M2F1_DATA_CLIP           (0x01180018)    // clip 24+280 bytes at front+rear and data field is 2048 bytes
                                                        // clip tail 208 bytes, subcode+dummy, via MCU H/W
                                                        // (???RESERVED) clip tail 488 bytes, EDC+ECC+subcode+dummy, via MCU H/W
#define BIU_CD_M2F1_DATA_LEN            (2048)

#define BIU_CD_M2F2_DATA_CLIP           (0x00040018)    // clip 24+4 bytes at front+rear and data field is 2324 bytes
                                                        // clip tail 208 bytes, subcode+dummy, via MCU H/W
#define BIU_CD_M2F2_DATA_LEN            (2324)

#define BIU_DEFAULT_GRAB01              (0x00040000)
#define BIU_DEFAULT_GRAB23              (0x080c0008)

#define BIU_DVB_CD_DATA_CLIP            (0x01c80010)    // clip 16+288+168 bytes at front+rear and data field is 2048 bytes
#define BIU_DVB_CD_DATA_LEN             (1880)

#define BIU_DVB_DATA_LEN                (43 * 188)
#define BIU_DVB_BLOCK_LEN               (0x2000)
#define BIU_DVB_SECTOR_LEN              BIU_DVB_DATA_LEN

// define structures
typedef struct  _tagBIU_CLIP_STRU
{
    SHORT       sSuffix;
    SHORT       sPrefix;
} BIU_CLIP_STRU, *PBIU_CLIP_STRU;

union   BIU_CLIP_UNION
{
    DWORD           dwClipReg;
    BIU_CLIP_STRU   ClipStru;
};

typedef struct  _tagTEST_BIU_CSS_STRU
{
    DWORD       dwBitsDRAMAddr;         // bitstream input buffer address
    LONG        lBitsLen;               // in 8 bytes (64-bit) unit
    DWORD       dwWriteDRAMAddr;        // output stream buffer address
    DWORD       dwPatternDRAMAddr;      // pattern buffer address
    LONG        lSecterLen;             // sector length in byte unit
    DWORD       dwEncDiscKeyL;          // 31:0 of disc key
    DWORD       dwEncDiscKeyU;          // 39:32 of disc key
    DWORD       dwEncDiscRefKeyL;       // 31:0 of disc reference key
    DWORD       dwEncDiscRefKeyU;       // 39:32 of disc reference key
    DWORD       dwEncTitleKeyL;         // 31:0 of title key
    DWORD       dwEncTitleKeyU;         // 39:32 of title key
    union BIU_CLIP_UNION    BIUClip;    // the sum of prefix and suffix should be modulated by 8
} TEST_BIU_CSS_STRU, *PTEST_BIU_CSS_STRU;

typedef struct  _tagTEST_BIU_CPPM_STRU
{
    DWORD       dwBitsDRAMAddr;         // bitstream input buffer address
    LONG        lBitsLen;               // in 8 bytes (64-bit) unit
    DWORD       dwWriteDRAMAddr;        // output stream buffer address
    DWORD       dwPatternDRAMAddr;      // pattern buffer address
    LONG        lSecterLen;             // sector length in byte unit
    DWORD       dwMediaKeyL;            // 31:0 of Media key
    DWORD       dwMediaKeyU;            // 55:32 of Media key
    DWORD       dwAlbumIDL;             // 31:0 of Album ID
    DWORD       dwAlbumIDU;             // 63:32 of Album ID
    union BIU_CLIP_UNION    BIUClip;    // the sum of prefix and suffix should be modulated by 8
} TEST_BIU_CPPM_STRU, *PTEST_BIU_CPPM_STRU;

// define MACROs
//#define MACRO_BIU_SW_RESET      {REG_PLAT_RESET_CONTROL &= 0xfffffffb;}
#define MACRO_BIU_READ_32BIT(dwBit)     \
    {dwBit = REG_BIU_BIURRD;}
#define MACRO_BIU_WRITE_32BIT(dwBit)    \
    {REG_BIU_BIUWRD = dwBit;}

#define MACRO_BIU_INPUT_ENABLE      (REG_BIU_BIUCTL |= BIU_CTL_BIFF_EN)
#define MACRO_BIU_OUTPUT_ENABLE     (REG_BIU_BIUCTL |= BIU_CTL_BOFF_EN)
#define MACRO_BIU_INPUT_READY       (REG_BIU_BIUCTL & BIU_STATUS_BIURDDRDY)
#define MACRO_BIU_OUTPUT_READY      (REG_BIU_BIUCTL & BIU_STATUS_MBOWRRDY)
#define MACRO_BIU_INPUT_DISABLE     (REG_BIU_BIUCTL &= (~(BIU_CTL_BIFF_EN)))
#define MACRO_BIU_OUTPUT_DISABLE    (REG_BIU_BIUCTL &= (~(BIU_CTL_BOFF_EN)))

#define MACRO_BIU_REMAINDER(dwBit)      \
    {REG_BIU_BIUBPREM |= dwBit;}

#define MACRO_BIU_GET_REMAINDER (REG_BIU_BIUBPREM & BIU_REMAINDER_MASK)

#define MACRO_BIU_GET_COUNTER   (REG_BIU_BIUFNPEC) // can be used only in find next pattern

#define MACRO_BIU_INIT_READ(dwSectorLen, dwClipUnion)       \
    {   \
        REG_BIU_SECTORLEN = dwSectorLen;    \
        REG_BIU_BIUCLIPLEN = dwClipUnion;   \
        REG_BIU_BIUCTL |= BIU_CTL_BIFF_EN;  \
    }

#define MACRO_BIU_INIT(dwSectorLen, dwClipUnion, dwState)   \
    {   \
        REG_BIU_SECTORLEN = dwSectorLen;    \
        REG_BIU_BIUCLIPLEN = dwClipUnion;   \
        REG_BIU_BIUCTL |= (dwState);    \
    }

//====================================================================
// iRemainder is 3-bit value
// iSize is total number of bytes needed to be by-pass. If this size is not divided by 64-bit,
//  padding mechanism will be turn on automatically.
// iSectorLen is a number of bytes for a sector/block
// iClipUnion stores the suffix and prefix needed to be clipped
// iPadding is a byte indicates what value to be padded at padding mode
//---------------------------------------
// By-pass mode INT bit will be turn on
//====================================================================
#define MACRO_BIU_BYPASS(dwRemainder, dwSize, dwSectorLen, dwClipUnion)  \
    {   \
        REG_BIU_BIUBPREM = dwRemainder;     \
        REG_BIU_SECTORLEN = dwSectorLen;    \
        REG_BIU_BIUCLIPLEN = dwClipUnion;   \
        REG_BIU_BIUBPCNT = dwSize;          \
        REG_BIU_BIUCTL &= (~BIU_CTL_PADDING);   \
        REG_BIU_BIUCTL |= (BIU_INT_BPINT_EN|BIU_OP_BIUBP_GO|BIU_CTL_BOFF_EN);   \
    }

#define MACRO_BIU_BYPASS_PADDING(dwRemainder, dwSize, dwSectorLen, dwClipUnion, dwPadding)  \
    {   \
        REG_BIU_BIUBPREM = (dwPadding << 8) | dwRemainder;      \
        REG_BIU_SECTORLEN = dwSectorLen;    \
        REG_BIU_BIUCLIPLEN = dwClipUnion;   \
        REG_BIU_BIUBPCNT = dwSize;      \
        REG_BIU_BIUCTL |= (BIU_CTL_PADDING|BIU_INT_BPINT_EN|BIU_OP_BIUBP_GO|BIU_CTL_BOFF_EN);   \
    }


//====================================================================
// iRemainder is 3-bit value
// iSize is total number of bytes needed to be skipped.
// iSectorLen is a number of bytes for a sector/block
// iClipUnion stores the suffix and prefix needed to be clipped
//---------------------------------------
// By-pass mode INT bit will be turn on
//====================================================================
#define MACRO_BIU_SKIP(dwRemainder, dwSize, dwSectorLen, dwClipUnion)       \
    {   \
        REG_BIU_BIUBPREM = dwRemainder;     \
        REG_BIU_SECTORLEN = dwSectorLen;    \
        REG_BIU_BIUCLIPLEN = dwClipUnion;   \
        REG_BIU_BIUBPCNT = dwSize;      \
        REG_BIU_BIUCTL |= (BIU_CTL_BIUSKIP_EN|BIU_INT_BPINT_EN|BIU_OP_BIUBP_GO|BIU_CTL_BIFF_EN|BIU_CTL_BOFF_EN);    \
    }


//====================================================================
// iRemainder is 3-bit value
// iSize is total number of bytes needed to be skipped.
// iSectorLen is a number of bytes for a sector/block
// iClipUnion stores the suffix and prefix needed to be clipped
//---------------------------------------
// By-pass mode INT bit will be turn on
//====================================================================
#define MACRO_BIU_FIND_PATTERN(dwSectorLen, dwClipUnion, dwPattern0, dwPattern1, dwPatternMask)  \
    {   \
        REG_BIU_SECTORLEN = dwSectorLen;    \
        REG_BIU_BIUCLIPLEN = dwClipUnion;   \
        REG_BIU_BIUTGTPAT0 = dwPattern0;    \
        REG_BIU_BIUTGTPAT1 = dwPattern1;    \
        REG_BIU_BIUPATMASK = dwPatternMask; \
        REG_BIU_BIUCTL |= (BIU_INT_FNPINT_EN|BIU_OP_FNP_GO|BIU_CTL_BIFF_EN|BIU_CTL_BOFF_EN);    \
    }

#define MACRO_BIU_FIND_NEXT_PATTERN     (REG_BIU_BIUCTL |= BIU_OP_FNP_GO)


#define MACRO_BIU_FNP_STATUS            (REG_BIU_BIUCTL >> 31)

// Select BIU input/output format (Big or little endian)
#define MACRO_BIU_FMT_OUTPUT_BIG_ENDIAN	\
	{	\
		REG_BIU_BIUCTL &= (~(0x0c000000));	\
	}	\

#define MACRO_BIU_FMT_OUTPUT_BYTE_SWAP	\
	{	\
		REG_BIU_BIUCTL &= (~(0x0c000000));	\
		REG_BIU_BIUCTL |= (0x04000000);	\
	}	\

#define MACRO_BIU_FMT_OUTPUT_LITTLE_ENDIAN_2BYTE	\
	{	\
		REG_BIU_BIUCTL &= (~(0x0c000000));	\
		REG_BIU_BIUCTL |= (0x08000000);	\
	}	\

#define MACRO_BIU_FMT_OUTPUT_LITTLE_ENDIAN	\
	{	\
		REG_BIU_BIUCTL |= (0x0c000000);	\
	}	\

#define MACRO_BIU_FMT_INPUT_BIG_ENDIAN	\
	{	\
		REG_BIU_BIUCTL &= (~(0x03000000));	\
	}	\

#define MACRO_BIU_FMT_INPUT_BYTE_SWAP	\
	{	\
		REG_BIU_BIUCTL &= (~(0x03000000));	\
		REG_BIU_BIUCTL |= (0x01000000);	\
	}	\

#define MACRO_BIU_FMT_INPUT_LITTLE_ENDIAN_2BYTE	\
	{	\
		REG_BIU_BIUCTL &= (~(0x03000000));	\
		REG_BIU_BIUCTL |= (0x02000000);	\
	}	\

#define MACRO_BIU_FMT_INPUT_LITTLE_ENDIAN	\
	{	\
		REG_BIU_BIUCTL |= (0x03000000);	\
	}	\

#ifdef __cplusplus
}
#endif //__cplusplus

#endif  // __CTKAV_BIU_H__
