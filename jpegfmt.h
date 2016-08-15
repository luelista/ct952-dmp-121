//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2006,
//          J500, all right reserved.
//
//      Product : CT909
//
//      Author  : Cheertek (J500 Tommy)
//      Purpose : For JPEG format, parser, and decoder implementation
//      Sources : 
//  **************************************************************************
//
#ifndef __JPEG_FORMAT_H__
#define __JPEG_FORMAT_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus



//tommy0.95a: multiple debugging levels 
#undef JPEG_DECODER_DBGMSG_EXECFLOW
#undef JPEG_DECODER_DBGMSG_DETAILED

//Profiling Category
#undef JPEG_DECODER_DBGMSG_PROFILE
#ifdef JPEG_DECODER_DBGMSG_PROFILE
#define JPEG_PROFILE_DECODING   0
#define JPEG_PROFILE_HEADER     1
#define JPEG_PROFILE_CONTENT    2
#define JPEG_PROFILE_TABLE      3
#define JPEG_PROFILE_SCANALL    4
#define JPEG_PROFILE_APP1       5
#define JPEG_PROFILE_APPX       6
#define JPEG_PROFILE_POSTPROC   7
#define JPEG_PROFILE_IDCTALL    8
#define JPEG_PROFILE_BYTESTUFF  9
#define JPEG_PROFILE_RUNLENGTH  10
#define JPEG_PROFILE_OPERATION  11
#define JPEG_PROFILE_IDCTCOEF   12
#define JPEG_PROFILE_BYPASS     13
#define JPEG_PROFILE_GETBITS    14
#define JPEG_PROFILE_DECCALC    15
#define JPEG_PROFILE_WAITVLD    16
#define JPEG_PROFILE_MAXNUM     17
#endif



//#ifndef CT909P_IC_SYSTEM
#if defined(CT909P_IC_SYSTEM ) || defined(CT909G_IC_SYSTEM)
//#define CT909P_SUPPORT_MJPEG
#else
//senshong2.60:If no jpeg and mp3 mode and only jpeg mode, we can disable "#define CT909R_JPEG_AND_MP3"
//#define CT909R_JPEG_AND_MP3

#ifndef CT909R_JPEG_AND_MP3
#define JPEG_USE_TWO_PROCESSOR
#endif
#endif


//senshong: using strip decoding method
#define JPEG_STRIP_DECODING

#define JPEG_STRIP_DEC_PROGRESSIVE
//tommy1.05
#define JPEG_TUNE_WAIT_VDFIFO

//tommy1.07: for performance evaluation
//tommy1.10: fixed _wMonitorBitStuffCount-related lines for exit-bit-mode issue
#undef JPEG_NO_BYTESTUFFING

//tommy1.08: decode DC only threshold for both slideshow and thumbnail modes.
#define DECODE_DC_ONLY_THRESHOLD_SLIDE      (5000000)
//tommy2.17: this should be smaller than 100000 pixels
#define DECODE_DC_ONLY_THRESHOLD_THUMB      (100000)

//tommy1.10: count the times of calling WaitVLD() in run-length decoding
#undef JPEG_PROFILE_WAITVLD_CALLS
#define JPEG_PROFILE_WAITVLD_CYCLES         (11)
#define JPEG_PROFILE_READVLD_CYCLES         (14)

//tommy1.10: inline some functions for better performance. Default: on
//tommy2.16b: removed. it's always on.
//#define PREFIX_INLINE_FLAG
//#ifdef PREFIX_INLINE_FLAG
//#define PREFIX_INLINE                       inline
//#else
//#define PREFIX_INLINE                       
//#endif

//tommy1.11: switch for flatten wait_vdfifo part. Default: on
#define FLATTEN_CRITICAL_WAITVDFIFO

//#ifndef CT909P_IC_SYSTEM
#if defined(CT909P_IC_SYSTEM ) || defined(CT909G_IC_SYSTEM)
#else
//tommy1.11: for MC-scaling down vertical 1/8; ref issue: #742, #757, #769. Default: on
//tommy2.10: CT909S has fixed this. default: off
//tommy2.14: still has problems in CT909S; make it on defaultly
#define MC_SCALE_DOWN_VERTICAL_8X_PATCH
#endif

//tommy1.11: use Chuan's table-lookup for variable-length decoding. Default: on
#define TABLE_LOOKUP_VLD

//tommy1.20: define IDCT/MC-scaling and JPU timed-out counts
#define JPEG_IDCT_TIMEDOUT_COUNT            (COUNT_1_SEC)
#define JPEG_JPU_TIMEDOUT_COUNT             (COUNT_1_SEC)

//tommy1.20: register value validation for IDCT/MC-scaling and JPU. Default: off
//#define JPEG_REGVALUE_VALIDATION

//tommy1.20a: acceptable decoding ratio for display. Default: 50
#define JPEG_ACCEPTABLE_DECODE_RATIO        (50)

// *** PLATFORM dependent ***
//
// Use CT909 VLD to process datastream
// This definition will affect datastream consuming related utility functions.
#define USE_CT909_VLD

// switch of doing Y-deflickering
#undef APPLY_Y_DEFLICKERING

// ON:  calculate aligned size (width & height) for Forg-3 to Forg-1
// OFF: use wFirstScalingWidth & wFirstScalingHeight for Forg-3 to Forg-1
#undef JPEGFMT_CALCSIZE_FOR_FRAMEORIG

// *** JPEG Marker Definitions ***
//
// prefix of JPEG marker
#define JPEG_MARKER_PREFIX   (0xFF)

// --- Start Of Frame ---
//
// Baseline DCT
#define JPEG_MARKER_SOF0     (0xC0)

// Extended sequential DCT, Huffman coding
//tommy1.21: already support & open this marker
#define JPEG_MARKER_SOF1     (0xC1)

// Progressive DCT, Huffman coding
#define JPEG_MARKER_SOF2     (0xC2)

// Lossless (sequential), Huffman coding
//#define JPEG_MARKER_SOF3     (0xC3)

// Differential sequential DCT
//#define JPEG_MARKER_SOF5     (0xC5)

// Differential progressive DCT
//#define JPEG_MARKER_SOF6     (0xC6)

// Differential lossless (sequential)
//#define JPEG_MARKER_SOF7     (0xC7)

// Extended sequential DCT, arithmetic coding
//#define JPEG_MARKER_SOF9     (0xC9)

// Progressive DCT, arithmetic coding
//#define JPEG_MARKER_SOF10    (0xCA)

// Lossless (sequential), arithmetic coding
//#define JPEG_MARKER_SOF11    (0xCB)

// Differential, arithmetic coding
//#define JPEG_MARKER_SOF13    (0xCD)
//#define JPEG_MARKER_SOF14    (0xCE)
//#define JPEG_MARKER_SOF15    (0xCF)

// --- RSTm: Restart with modulo 8 count "m" ---
#define JPEG_MARKER_RST0     (0xD0)
#define JPEG_MARKER_RST1     (0xD1)
#define JPEG_MARKER_RST2     (0xD2)
#define JPEG_MARKER_RST3     (0xD3)
#define JPEG_MARKER_RST4     (0xD4)
#define JPEG_MARKER_RST5     (0xD5)
#define JPEG_MARKER_RST6     (0xD6)
#define JPEG_MARKER_RST7     (0xD7)

// --- Start/End Of Image ---
#define JPEG_MARKER_SOI      (0xD8)
#define JPEG_MARKER_EOI      (0xD9)

//  --- Start Of Scan ---
#define JPEG_MARKER_SOS      (0xDA)

// --- DNL: Define Number of Lines ---
#define JPEG_MARKER_DNL      (0xDC)

// --- DRI: Define Restart Interval segment ---
#define JPEG_MARKER_DRI      (0xDD)

// --- APPn ---
#define JPEG_MARKER_APP0     (0xE0)
#define JPEG_MARKER_APP1     (0xE1)
#define JPEG_MARKER_APP2     (0xE2)
#define JPEG_MARKER_APP3     (0xE3)
#define JPEG_MARKER_APP4     (0xE4)
#define JPEG_MARKER_APP5     (0xE5)
#define JPEG_MARKER_APP6     (0xE6)
#define JPEG_MARKER_APP7     (0xE7)
#define JPEG_MARKER_APP8     (0xE8)
#define JPEG_MARKER_APP9     (0xE9)
#define JPEG_MARKER_APPA     (0xEA)
#define JPEG_MARKER_APPB     (0xEB)
#define JPEG_MARKER_APPC     (0xEC)
#define JPEG_MARKER_APPD     (0xED)
#define JPEG_MARKER_APPE     (0xEE)
#define JPEG_MARKER_APPF     (0xEF)

// --- COM: comment ---
#define JPEG_MARKER_COM      (0xFE)

// --- DHT and DQT: Huffman and Quantization Table ---
#define JPEG_MARKER_DQT      (0xDB)
#define JPEG_MARKER_DHT      (0xC4)

//tommy1.20a: process corner case while exit bit mode.
#define JPEG_MARKER_BYTESTUFF   (0x00)

// *** Constant Definition for JPEG Decoder ***

//tommy0.66: buffer plan -- 720x480 *2 + 640x432 *2
//32M Solution
//#define JPEG_INCOMEBUFFSIZE     (1347840)     //3-to-1 incoming buffer 32M Solution
                                                //size=720x576x3(MPEG2 buffer) - 720x480@420
// Constants for Fdisp0 & Fdisp1
#define JPEG_DISPBUF_WIDTH      (720)
#define JPEG_DISPBUF_HEIGHT     (480)
#define JPEG_DISPBUF_SIZE       (0x7E900)       //518400
#define JPEG_DISPBUF_SIZE_Y     (0x54600)       //345600
#define JPEG_DISPBUF_SIZE_UV    (0x2A300)       //172800
#define JPEG_DISPBUF_COLS       (0x5A)          //columns for display buffer
#define JPEG_DISPBUF_ROWS       (0x3C)          //rows for display buffer

//tommy0.66 Constants for Ftemp & Forig
//tommy2.10: this section should be moved into CT909A-ONLY block
#define JPEG_ORIGBUF_WIDTH      (640)
#define JPEG_ORIGBUF_HEIGHT     (432)
#define JPEG_ORIGBUF_SIZE       (0x65400)       //414720
#define JPEG_ORIGBUF_SIZE_Y     (0x43800)       //276480
#define JPEG_ORIGBUF_SIZE_UV    (0x21C00)       //138240
#define JPEG_ORIGBUF_COLS       (0x50)
#define JPEG_ORIGBUF_ROWS       (0x36)
#define JPEG_TEST_16M 0
#define JPEG_ORIGBUF_SIZE_32M       (0x70800)   //460800


//senshong2.32: define JPEG original buffer width and height
#define JPEG_ORIGBUF_WIDTH_64M  (800)
#define JPEG_ORIGBUF_HEIGHT_64M (640)
#define JPEG_ORIGBUF_WIDTH_32M  (640)
#define JPEG_ORIGBUF_HEIGHT_32M (480)
#define JPEG_ORIGBUF_WIDTH_16M  (576)
#define JPEG_ORIGBUF_HEIGHT_16M (400)
#define JPEG_THUMB_ORIGBUF_WIDTH_16M  (320)
#define JPEG_THUMB_ORIGBUF_HEIGHT_16M (240)

//tommy1.21: define the use of extra incoming buffer for decoding.
//           if it's disable, we'll use 3-in-1 buffer for decoding.
//tommy2.10: use extra incoming buffer for JPEG decoding by DRAM configuration
//#if ((DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16) || (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_32)||(JPEG_TEST_16M ==1))
/*#if ((DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16) ||(JPEG_TEST_16M ==1))
    #undef USE_EXTRA_INCOMING_BUFFER
#else       //64M 
    #undef  USE_EXTRA_INCOMING_BUFFER
    //#define USE_EXTRA_INCOMING_BUFFER
#endif

#if((DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)||(JPEG_TEST_16M ==1))
    #define NOT_USE_JPU_SCFR_COMBO
    #undef USE_JPU_SCFR_COMBO_OPERATION_PATCH
#else
    #define NOT_USE_JPU_SCFR_COMBO
    //#undef NOT_USE_JPU_SCFR_COMBO
    #undef USE_JPU_SCFR_COMBO_OPERATION_PATCH
#endif*/

#undef USE_EXTRA_INCOMING_BUFFER
#undef NOT_USE_JPU_SCFR_COMBO
#undef USE_JPU_SCFR_COMBO_OPERATION_PATCH

//#define JPEG_STRIP_DECODING_1
#define JPEG_STRIP_DECODING_AND_EFFECT
//-------------------------------------------------------------------------------
//Test JPEG 16M
//-------------------------------------------------------------------------------
    //#define JPEGFMT_BUFF_THUMBNAIL_DISP (DS_JPG_ORG_BUFF_ST)//0x40450000
    //#define JPEGFMT_BUFF_SLIDESHOW_DISP (DS_FRAMEBUF_ST)//0x4014f800
    

    //#define JPEGFMT_BUFF_DISP0 (DS_FRAMEBUF_ST)//0x4014f800
    //#define JPEGFMT_BUFF_DISP1 (DS_JPG_ORG_BUFF_ST)//0x40450000

    /*#define JPEGFMT_BUFF_THUMBNAIL_ST_ADDR  (DS_FRAMEBUF_THUMBNAIL_ST)
    #define JPEGFMT_BUFF_SLIDESHOW_ST_ADDR  (DS_FRAMEBUF_SLIDESHOW_ST)
    #define JPEGFMT_BUFF_THUMBNAIL_END_ADDR (DS_FRAMEBUF_THUMBNAIL_END)
    #define JPEGFMT_BUFF_SLIDESHOW_END_ADDR (DS_FRAMEBUF_SLIDESHOW_END)
    
    //DISPLAY BUFFER STARTING ADDRESS
    #define JPEGFMT_BUFF_THUMBNAIL_DISP (JPEGFMT_BUFF_THUMBNAIL_ST_ADDR)//0x400dd000
    #define JPEGFMT_BUFF_SLIDESHOW_DISP (JPEGFMT_BUFF_SLIDESHOW_ST_ADDR)//0x400a0000


    
    #define JPEGFMT_BUFF_DISP0 (JPEGFMT_BUFF_SLIDESHOW_DISP)//0x400a0000
    #define JPEGFMT_BUFF_DISP1 (JPEGFMT_BUFF_THUMBNAIL_DISP)//0x400dd000*/
    //-------------------------------------------------------------------------

/*
#ifdef USE_EXTRA_INCOMING_BUFFER
    //use extra incoming buffer
    #define JPEG_INCOMEBUFFSIZE     (DS_JPG_ORG_BUFF_END - DS_JPG_ORG_BUFF_ST)
#else   //USE_EXTRA_INCOMING_BUFFER
    //use 3-in-1 buffer
    //#define JPEG_INCOMEBUFFSIZE     (738432)
    #define JPEG_INCOMEBUFFSIZE     (JPEG_ORIGBUF_SIZE * 2 + JPEG_DISPBUF_SIZE)
#endif  //USE_EXTRA_INCOMING_BUFFER
*/

// BUFFER ADDRESS DECLARATIONS
// specify buffer plan for different memory allocations
//
// ----------------------------------
/*#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16||JPEG_TEST_16M== 1)
//senshong2.16: define JPEG deocder for 16M

//THUMBNAIL total buffer size: 0x400DD000 + 0x7E900; size=231K DW
    //SLIDESHOW total buffer size: 0x400A0000 + 0x7E900; size=328K DW
    //THUMBNAIL decoding buffer size: 231*1024*4 - 0x7e900 = 0xe7000  - 0x7e900 = 0x68700 =427776
    //SLIDESHOW decoding buffer size: 328*1024*4 - 0x7e900 = 0x148000 - 0x7e900 = 0xC9700 =825088
#if(JPEG_TEST_16M== 1)//senshong2.16: this define is use 64M extra memory to simulate 16m solution
    //DISPLAY BUFFER STARTING ADDRESS
    #define JPEGFMT_BUFF_THUMBNAIL_DISP (DS_JPG_ORG_BUFF_ST)//0x40450000
    //#define JPEGFMT_BUFF_THUMBNAIL_DISP (DS_FRAMEBUF_ST)
    #define JPEGFMT_BUFF_SLIDESHOW_DISP (DS_FRAMEBUF_ST)//0x4014f800
    

    #define JPEGFMT_BUFF_DISP0 (DS_FRAMEBUF_ST)//0x4014f800
    #define JPEGFMT_BUFF_DISP1 (DS_JPG_ORG_BUFF_ST)//0x40450000
    //#define JPEGFMT_BUFF_DISP1 (DS_FRAMEBUF_ST)
   

    //decoding + original buffer
    #define JPEGFMT_BUFF_THUMBNAIL_DEC (DS_JPG_ORG_BUFF_ST + JPEG_DISPBUF_SIZE)//0x404ce900
    #define JPEGFMT_BUFF_SLIDESHOW_DEC (DS_FRAMEBUF_ST + JPEG_DISPBUF_SIZE)//0x401ce100
    //#define JPEGFMT_BUFF_THUMBNAIL_DEC (DS_FRAMEBUF_ST + JPEG_DISPBUF_SIZE)
    
    

    #define JPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_BASE  (0x68700)//427776

   
    //#define JPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_BASE (0xC9700)
    #define JPEGFMT_BUFF_SIZE_SLIDESHOW_DEC_BASE  (0xC9700)//825088

    

    #define JPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_PROG  (0x1D880)//120960

    #define JPEGFMT_BUFF_SIZE_SLIDESHOW_DEC_PROG  (0x7E900)//518400
    //#define JPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_PROG (0x7E900)


    //SLIDESHOW coeff buffer size: 0xC9700-0x7e900 =0x4ae00 = 306688
    //THUMBNAIL coeff buffer size:  0x68700-0x1D880(360*224*1.5)=0x4ae80 =306816
    #define JPEG_COEFFBUFF_SIZE_SLIDESHOW (JPEGFMT_BUFF_SIZE_SLIDESHOW_DEC_BASE-0x7e900)//306688

    #define JPEG_COEFFBUFF_SIZE_THUMBNAIL (JPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_BASE-0x1D880)//306816
    //#define JPEG_COEFFBUFF_SIZE_THUMBNAIL   (JPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_BASE-0x7e900)

    #define JPEG_COEFFBUFF_SLIDESHOW_ADDR (JPEGFMT_BUFF_SLIDESHOW_DEC+0x7e900)//0x4024ca00

    #define JPEG_COEFFBUFF_THUMBNAIL_ADDR (JPEGFMT_BUFF_THUMBNAIL_DEC+0x1D880)//0x404ec180
    //#define JPEG_COEFFBUFF_THUMBNAIL_ADDR (JPEGFMT_BUFF_THUMBNAIL_DEC+0x07e900)
    

#else//when DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16


    //THUMBNAIL total buffer size: 0x400DD000 + 0xe7000; size=231K DW
    //SLIDESHOW total buffer size: 0x400A0000 + 0x148000; size=328K DW
    //THUMBNAIL decoding buffer size: 231*1024*4 - 0x7e900 = 0xe7000  - 0x7e900 = 0x68700 =427776
    //SLIDESHOW decoding buffer size: 328*1024*4 - 0x7e900 = 0x148000 - 0x7e900 = 0xC9700 =825088
    //#define JPEGFMT_BUFF_THUMBNAIL_ST_ADDR (0x400DD000)//0x400dd000
    //#define JPEGFMT_BUFF_SLIDESHOW_ST_ADDR (0x400A0000)//0x400a0000
    //#define JPEGFMT_BUFF_THUMBNAIL_ST_ADDR
    //#define JPEGFMT_BUFF_SLIDESHOW_ST_ADDR
    //#define JPEGFMT_BUFF_THUMBNAIL_ST_ADDR	(JPEGFMT_BUFF_SLIDESHOW_ST_ADDR+0xe7000)
    //#define JPEGFMT_BUFF_SLIDESHOW_END_ADDR (JPEGFMT_BUFF_SLIDESHOW_ST_ADDR+0x148000)
    #define JPEGFMT_BUFF_THUMBNAIL_ST_ADDR  (DS_FRAMEBUF_THUMBNAIL_ST)
    #define JPEGFMT_BUFF_SLIDESHOW_ST_ADDR  (DS_FRAMEBUF_SLIDESHOW_ST)
    #define JPEGFMT_BUFF_THUMBNAIL_END_ADDR (DS_FRAMEBUF_THUMBNAIL_END)
    #define JPEGFMT_BUFF_SLIDESHOW_END_ADDR (DS_FRAMEBUF_SLIDESHOW_END)
    
    //DISPLAY BUFFER STARTING ADDRESS
    #define JPEGFMT_BUFF_THUMBNAIL_DISP (JPEGFMT_BUFF_THUMBNAIL_ST_ADDR)//0x400dd000
    #define JPEGFMT_BUFF_SLIDESHOW_DISP (JPEGFMT_BUFF_SLIDESHOW_ST_ADDR)//0x400a0000


    
    #define JPEGFMT_BUFF_DISP0 (JPEGFMT_BUFF_SLIDESHOW_DISP)//0x400a0000
    #define JPEGFMT_BUFF_DISP1 (JPEGFMT_BUFF_THUMBNAIL_DISP)//0x400dd000

    //decoding + original buffer
    #define JPEGFMT_BUFF_THUMBNAIL_DEC (JPEGFMT_BUFF_THUMBNAIL_DISP + JPEG_DISPBUF_SIZE)//0x4015b900
    #define JPEGFMT_BUFF_SLIDESHOW_DEC (JPEGFMT_BUFF_SLIDESHOW_DISP + JPEG_DISPBUF_SIZE)//0x4011e900
    

    #define JPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_BASE  (0x68700)//427776
    #define JPEGFMT_BUFF_SIZE_SLIDESHOW_DEC_BASE  (0xC9700)//825088
    //#define JPEGFMT_BUFF_SIZE_SLIDESHOW_DEC_BASE (0x68700)

    #define JPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_PROG  (0x1D880)// 360*224*1.5= 120960(0x1d880)
    #define JPEGFMT_BUFF_SIZE_SLIDESHOW_DEC_PROG  (0x7E900)//518400
    //#define JPEGFMT_BUFF_SIZE_SLIDESHOW_DEC_PROG  (0x1D880)


    //SLIDESHOW coeff buffer size: 0xC9700-0x7e900 =0x4ae00 = 306688
    //THUMBNAIL coeff buffer size:  0x68700-0x1D880(360*224*1.5)=0x4ae80 =306816
    #define JPEG_COEFFBUFF_SIZE_SLIDESHOW (JPEGFMT_BUFF_SIZE_SLIDESHOW_DEC_BASE-0x7e900)//306688
    #define JPEG_COEFFBUFF_SIZE_THUMBNAIL (JPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_BASE-0x1D880)//306816
    #define JPEG_COEFFBUFF_SLIDESHOW_ADDR (JPEGFMT_BUFF_SLIDESHOW_DEC+0x7e900)//0x4019d200
    #define JPEG_COEFFBUFF_THUMBNAIL_ADDR (JPEGFMT_BUFF_THUMBNAIL_DEC+0x1D880)//0x40179180
    //#define JPEG_COEFFBUFF_SIZE_SLIDESHOW (JPEGFMT_BUFF_SIZE_SLIDESHOW_DEC_BASE-0x1D880)

#endif//#if(JPEG_TEST_16M== 1)




#endif  //16M*/

//---------------------------------------
//senshong2.17a:16M solution
//---------------------------------------
    //THUMBNAIL total buffer size: 0x400A0000 + 0x1E8000; size=225K DW
    //SLIDESHOW total buffer size: 0x400A0000 + 0x181000; size=328K DW
    //THUMBNAIL decoding buffer size: 225*1024*4 - 0x7E900(640*432*1.5) = 0xe1000(921600)  - 0x65400(414720) = 0x7BC00 =506880
    //SLIDESHOW decoding buffer size: 328*1024*4 - 0xA8C00(640*432*1.5+640*432) = 0x148000(1343488) - 0xA8C00(691200) = 0x9f400 =652288

    //THUMBNAIL  coeff buffer size: 0x7BC00(506880) -0x36600 (464*320*1.5) = 0x7BC00(506880) - 0x36600(222720)=0x45600(284160)
    //SLIDESHOW coeff buffer size: 0x9f400(652288) - 0x5a300(592*416*1.5) = 0x9f400(652288) -0x5a300(369408)= 0x45100(282880)




//--------------------------------------------------------

// ----------------------------------
//#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_32)
//#endif  //32M

// ----------------------------------
// For 64M Solution
//
//senshong 2.17 remove
/*#if(JPEG_TEST_16M!=1 && DRAM_CONFIGURATION_TYPE_DVD!=DRAM_SIZE_16)//senshong2.16:avoid to simulate 16m,the define overlap 64m
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_64 || DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_32)
    //old-fashioned declartions, for CT909A
    //Assume total buffer = DISP*2 + ORIG*2
    //
    #define JPEGFMT_BUFF_TOTALSIZE      (JPEG_ORIGBUF_SIZE * 2 + JPEG_DISPBUF_SIZE * 2)

    #define JPEGFMT_BUFF_START          (DS_FRAMEBUF_ST)
    #define JPEGFMT_BUFF_DISP0          (JPEGFMT_BUFF_START)
    #define JPEGFMT_BUFF_ORIG           (JPEGFMT_BUFF_START + JPEG_DISPBUF_SIZE)
    #define JPEGFMT_BUFF_TEMP           (JPEGFMT_BUFF_ORIG  + JPEG_ORIGBUF_SIZE)
    #define JPEGFMT_BUFF_DISP1          (JPEGFMT_BUFF_TEMP  + JPEG_ORIGBUF_SIZE)

    #ifdef USE_EXTRA_INCOMING_BUFFER
        #define JPEG_DECBUFF_ADDR       (DS_JPG_ORG_BUFF_ST)
        #define JPEG_DECBUFF_SIZE       (DS_JPG_ORG_BUFF_END - DS_JPG_ORG_BUFF_ST)
        #define JPEG_COEFFBUFF_ADDR0    (JPEGFMT_BUFF_DISP0)
        #define JPEG_COEFFBUFF_ADDR1    (JPEGFMT_BUFF_ORIG)
        #define JPEG_COEFFBUFF_SIZE     (JPEG_ORIGBUF_SIZE * 2 + JPEG_DISPBUF_SIZE)
    #else   //USE_EXTRA_INCOMING_BUFFER
        #define JPEG_DECBUFF_ADDR0      (JPEGFMT_BUFF_DISP0)
        #define JPEG_DECBUFF_ADDR1      (JPEGFMT_BUFF_ORIG)
        //decoding buffer size for BASELINE
        #define JPEG_DECBUFF_SIZE_BASE  (JPEG_ORIGBUF_SIZE * 2 + JPEG_DISPBUF_SIZE)
        //decoding buffer size for PROGRESSIVE: aligned to 32x
        #define JPEG_DECBUFF_SIZE_PROG  (JPEG_ORIGBUF_SIZE * 2) //640x432x1.5*2

        #define JPEG_COEFFBUFF_ADDR0    (JPEG_DECBUFF_ADDR0 + JPEG_DECBUFF_SIZE_PROG)
        #define JPEG_COEFFBUFF_ADDR1    (JPEG_DECBUFF_ADDR1 + JPEG_DECBUFF_SIZE_PROG)
        #define JPEG_COEFFBUFF_SIZE     (JPEG_DECBUFF_SIZE_BASE - JPEG_DECBUFF_SIZE_PROG)
    #endif  //USE_EXTRA_INCOMING_BUFFER*/
    //

    // -----------------
    // Adopt new buffer plan, resulting in: (1) new SC+FR _JPEGDEC_Operate()
    // applied, and (2) use the whole buffer allocated by MPEG-2
    //
    // MPEG-2 buffer size = 720x576x1.5 = 0x97E00 --> 0x98000 (aligned to 0x800)
    /*
    #define JPEGFMT_BUFF_TOTALSIZE      (DS_FRAMEBUF_SIZE * 3)
    #define JPEGFMT_BUFF_START          (DS_FRAMEBUF_ST)
    #define JPEGFMT_BUFF_DISP0          (JPEGFMT_BUFF_START)
    #define JPEGFMT_BUFF_ORIG           (JPEGFMT_BUFF_START + JPEG_DISPBUF_SIZE)
    #define JPEGFMT_BUFF_DISP1          (JPEGFMT_BUFF_START + JPEGFMT_BUFF_TOTALSIZE - JPEG_DISPBUF_SIZE)

    #ifdef USE_EXTRA_INCOMING_BUFFER
        #define JPEG_DECBUFF_ADDR       (DS_JPG_ORG_BUFF_ST)
        #define JPEG_DECBUFF_SIZE       (DS_JPG_ORG_BUFF_END - DS_JPG_ORG_BUFF_ST)
        #define JPEG_COEFFBUFF_ADDR0    (JPEGFMT_BUFF_DISP0)
        #define JPEG_COEFFBUFF_ADDR1    (JPEGFMT_BUFF_ORIG)
        #define JPEG_COEFFBUFF_SIZE     (JPEGFMT_BUFF_TOTALSIZE - JPEG_DISPBUF_SIZE)
    #else   //USE_EXTRA_INCOMING_BUFFER
        #define JPEG_DECBUFF_ADDR0      (JPEGFMT_BUFF_DISP0)
        #define JPEG_DECBUFF_ADDR1      (JPEGFMT_BUFF_ORIG)
        //decoding buffer size for BASELINE
        #define JPEG_DECBUFF_SIZE_BASE  (JPEGFMT_BUFF_TOTALSIZE - JPEG_DISPBUF_SIZE)
        //decoding buffer size for PROGRESSIVE: aligned to 32x
        #define JPEG_DECBUFF_SIZE_PROG  (JPEG_ORIGBUF_SIZE * 2) //640x432x1.5*2

        #define JPEG_COEFFBUFF_ADDR0    (JPEG_DECBUFF_ADDR0 + JPEG_DECBUFF_SIZE_PROG)
        #define JPEG_COEFFBUFF_ADDR1    (JPEG_DECBUFF_ADDR1 + JPEG_DECBUFF_SIZE_PROG)
        #define JPEG_COEFFBUFF_SIZE     (JPEG_DECBUFF_SIZE_BASE - JPEG_DECBUFF_SIZE_PROG)
    #endif  //USE_EXTRA_INCOMING_BUFFER
    */

//senshong2.17:remove    
//#endif  //64M
//#endif//#if(JPEG_TEST_16M!=1 && DRAM_CONFIGURATION_TYPE_DVD!=DRAM_SIZE_16)

/*
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_32)
    #define JPEGFMT_BUFF_START      DS_FRAMEBUF_ST
    #define JPEGFMT_BUFF_DISP0      (JPEGFMT_BUFF_START)
    #define JPEGFMT_BUFF_ORIG       (JPEGFMT_BUFF_START + JPEG_DISPBUF_SIZE)
#ifdef SUPPORT_CT909S_JPU_MODIFICATIONS
    #define JPEGFMT_BUFF_DISP1      (JPEGFMT_BUFF_START + DS_FRAMEBUF_SIZE - JPEG_DISPBUF_SIZE)
#else   //SUPPORT_CT909S_JPU_MODIFICATIONS
    #define JPEGFMT_BUFF_TEMP       (JPEGFMT_BUFF_ORIG  + JPEG_ORIGBUF_SIZE)
    #define JPEGFMT_BUFF_DISP1      (JPEGFMT_BUFF_TEMP  + JPEG_ORIGBUF_SIZE)
#endif  //SUPPORT_CT909S_JPU_MODIFICATIONS

    //tommy2.16: support progressive for 32M
    #define JPEGFMT_COEFFBUFF_ADDR  (JPEGFMT_BUFF_ORIG  + JPEG_ORIGBUF_SIZE)
    #define JPEGFMT_COEFFBUFF_SIZE  (311040)
    //#define JPEGFMT_COEFFBUFF_SIZE ((DS_FRAMEBUF_SIZE - JPEG_ORIGBUF_SIZE) * 3)        //312576

    #undef  JPEG_INCOMEBUFFSIZE

    //decoding buffer size
    #define JPEGFMT_DECBUFF_SIZE_BASELINE       (JPEG_ORIGBUF_SIZE * 2 + JPEG_DISPBUF_SIZE)
    #define JPEGFMT_DECBUFF_SIZE_PROGRESSIVE    (JPEG_DISPBUF_SIZE)
    #define JPEGFMT_DECBUFF_ADDR_PROGRESSIVE    (JPEGFMT_BUFF_ORIG)

#elif (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
    //tommy2.15: for 16M solution, re-define these settings

    //#define JPEGFMT_BUFF_START      DS_FRAMEBUF_ST
    //#define JPEGFMT_BUFF_DISP0      (JPEGFMT_BUFF_START)
    //#define JPEGFMT_BUFF_ORIG       (JPEGFMT_BUFF_START + JPEG_DISPBUF_SIZE)

    #undef JPEG_INCOMEBUFFSIZE

    //THUMBNAIL total buffer size: 0x400DD000 + 0x7E900; size=231K DW
    //SLIDESHOW total buffer size: 0x400A0000 + 0x7E900; size=328K DW
    //THUMBNAIL decoding buffer size: 231*1024*4 - 0x7e900 = 0xe7000  - 0x7e900 = 0x68700
    //SLIDESHOW decoding buffer size: 328*1024*4 - 0x7e900 = 0x148000 - 0x7e900 = 0xC9700

    //DISPLAY BUFFER STARTING ADDRESS
    #define JPEGFMT_DISPBUFF_ADDR_THUMBNAIL (0x400DD000)
    #define JPEGFMT_DISPBUFF_ADDR_SLIDESHOW (0x400A0000)

    //decoding + original buffer
    #define JPEGFMT_DECBUFF_ADDR_THUMBNAIL  (0x400DD000 + JPEG_DISPBUF_SIZE)
    #define JPEGFMT_DECBUFF_ADDR_SLIDESHOW  (0x400A0000 + JPEG_DISPBUF_SIZE)

    #define JPEGFMT_DECBUFF_SIZE_THUMBNAIL  (0x68700)
    #define JPEGFMT_DECBUFF_SIZE_SLIDESHOW  (0xC9700)

//64M SOLUTION
#else
#ifndef SUPPORT_CT909S_JPU_MODIFICATIONS
    #define JPEGFMT_BUFF_START      DS_FRAMEBUF_ST
    #define JPEGFMT_BUFF_DISP0      (JPEGFMT_BUFF_START)
    #define JPEGFMT_BUFF_ORIG       (JPEGFMT_BUFF_START + JPEG_DISPBUF_SIZE)
    #define JPEGFMT_BUFF_TEMP       (JPEGFMT_BUFF_ORIG  + JPEG_ORIGBUF_SIZE)
    #define JPEGFMT_BUFF_DISP1      (JPEGFMT_BUFF_TEMP  + JPEG_ORIGBUF_SIZE)

#else   //SUPPORT_CT909S_JPU_MODIFICATIONS
    #define JPEGFMT_BUFF_START      DS_FRAMEBUF_ST
    #define JPEGFMT_BUFF_DISP0      (JPEGFMT_BUFF_START)
    #define JPEGFMT_BUFF_ORIG       (JPEGFMT_BUFF_START + JPEG_DISPBUF_SIZE)
    #define JPEGFMT_BUFF_DISP1      (JPEGFMT_BUFF_START + DS_FRAMEBUF_SIZE - JPEG_DISPBUF_SIZE)
#endif  //SUPPORT_CT909S_JPU_MODIFICATIONS
#endif
*/

// *** Constant Definition (from jpgpvt.h) ***
//
#define JPEGSAMPLEWIDTH                 (8)
#define JPEGSAMPLESIZE                  (64)
#define JPEGMAXHUFFMANTABLES            (4)
#define JPEGMAXQUANTIZATIONTABLES       (4)

#define JPEGMAXCOMPONENTSPERFRAME       (255)
#define JPEGMAXCOMPONENTSPERSCAN        (4)

#define JPEGMINSAMPLINGFREQUENCY        (1)
#define JPEGMAXSAMPLINGFREQUENCY        (4)

#define JPEGMAXHUFFMANCODELENGTH        (16)
#define JPEGMAXNUMBEROF8BITHUFFMANCODES (256)
#define JPEGMAXNUMBEROFHUFFMANCODES     (256)

// *** Type Definition ***
//
typedef struct tagHUFFDECODER
{
    // 20040804 tommylin: maxcode and mincode have to be "signed" (short).
    // If they're "WORD" (unsigned short), we may encounter incorrect
    // comparison while executing decode()

    // Maximum Huffman code value of length N
    
//#ifdef CT909P_IC_SYSTEM
#if defined(CT909P_IC_SYSTEM ) || defined(CT909G_IC_SYSTEM)
    //WORD maxcode [JPEGMAXHUFFMANCODELENGTH];
    LONG maxcode [JPEGMAXHUFFMANCODELENGTH];
#else
    SHORT maxcode [JPEGMAXHUFFMANCODELENGTH];
#endif
    
    // Minimum Huffman code value of length N
    SHORT mincode [JPEGMAXHUFFMANCODELENGTH];
    // Index into "values" for minimum code of length N
    WORD valptr [JPEGMAXHUFFMANCODELENGTH];             //tommy1.05: BYTE-->WORD
    // Huffman values
    WORD huff_values [JPEGMAXNUMBEROFHUFFMANCODES];     //tommy1.05: BYTE-->WORD

    //BYTE table_defined;
    WORD minimum_code_length;

#ifdef TABLE_LOOKUP_VLD
    WORD lookup0_7 [JPEGMAXNUMBEROFHUFFMANCODES];
#endif
} HUFFDECODER, * PHUFFDECODER;

typedef struct tagQUANTTAB
{
    //tommy0.81: actually, we only read 8-bit quantization value into the table
    WORD quant_values [JPEGSAMPLESIZE];     //UBYTE2    //tommy1.05: BYTE-->WORD
} QUANTTAB, * PQUANTTAB;

typedef SHORT COEFBLOCK [JPEGSAMPLESIZE];   //BYTE2

// JPEG Decoder Component
//
typedef struct tagCOMPONENT
{
    PHUFFDECODER DC_Table;
    PHUFFDECODER AC_Table;

    SHORT last_dc_value;        //int

    WORD H;
    WORD V;
    WORD Tq;    //Quantization Table Index

    WORD v_sampling;            //unsigned int
    WORD h_sampling;            //unsigned int
    //tommy0.95: End of band Run - Progressive Specific
    WORD eob_run;               //unsigned int; tommy0.95a: can't use BYTE 

    //Non-interleaved dimensions.
    WORD noninterleaved_rows;   //unsigned int
    WORD noninterleaved_cols;   //unsigned int
    WORD du_rows;               //unsigned int
    WORD du_cols;               //unsigned int

    BYTE *data_units;           //UBYTE1
    COEFBLOCK *coefficient_blocks;

    //PQUANTTAB quantization_tables;

    // same as the wMcuHeight & wMcuWidth
    //WORD image_height;
    //WORD image_width;
} COMPONENT, * PCOMPONENT;

/*typedef struct tagJPEG_BUFFER_SET
{
    volatile DWORD dwJPEGFMT_BUFF_START;
    volatile DWORD dwJPEGFMT_BUFF_END;
    volatile DWORD dwJPEGFMT_BUFF_DISP0;
    volatile DWORD dwJPEGFMT_BUFF_ORIG;
    volatile DWORD dwJPEGFMT_BUFF_TEMP;
    volatile DWORD dwJPEGFMT_BUFF_DISP1;
    volatile DWORD dwJPEG_COEFFBUFF_ADDR0;
    volatile DWORD dwJPEG_COEFFBUFF_ADDR1;
    volatile DWORD dwJPEG_COEFFBUFF_SIZE;
    volatile DWORD dwJPEG_DECBUFF_ADDR0;
    volatile DWORD dwJPEG_DECBUFF_ADDR1;
    //decoding buffer size for BASELINE
    volatile DWORD dwJPEG_DECBUFF_SIZE_BASE;
    //decoding buffer size for PROGRESSIVE: aligned to 32x
    volatile DWORD dwJPEG_DECBUFF_SIZE_PROG;
    //for 16M Thumbnail mode
    volatile DWORD dwJPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_BASE;
    volatile DWORD dwJPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_PROG;
    volatile DWORD dwJPEG_COEFFBUFF_SIZE_THUMBNAIL;
    volatile DWORD dwJPEGFMT_BUFF_TEMP1;
    
    //for 64M extra buffer
    volatile DWORD dwJPEG_DECBUFF_ADDR;
    volatile DWORD dwJPEG_DECBUFF_SIZE;
    volatile DWORD dwJPEG_EXTRABUFF_END;
    volatile DWORD dwJPEG_DISPBUF_SIZE;
    volatile DWORD dwJPEG_DISPBUF_SIZE_Y;
    volatile WORD wJPEG_DISPBUF_WIDTH;
    volatile WORD wJPEG_DISPBUF_HEIGHT;
    volatile WORD wDRAMType;
    
} JPEG_BUFFER_SET, *PJPEG_BUFFER_SET;*/

typedef struct tagJPEG_BUFFER_SET
{
    DWORD dwJPEGFMT_BUFF_START;
    DWORD dwJPEGFMT_BUFF_END;
    DWORD dwJPEGFMT_BUFF_DISP0;
    DWORD dwJPEGFMT_BUFF_ORIG;
    DWORD dwJPEGFMT_BUFF_TEMP;
    DWORD dwJPEGFMT_BUFF_DISP1;
    DWORD dwJPEG_COEFFBUFF_ADDR0;
    DWORD dwJPEG_COEFFBUFF_ADDR1;
    DWORD dwJPEG_COEFFBUFF_SIZE;
    DWORD dwJPEG_DECBUFF_ADDR0;
    DWORD dwJPEG_DECBUFF_ADDR1;
    //decoding buffer size for BASELINE
    DWORD dwJPEG_DECBUFF_SIZE_BASE;
    //decoding buffer size for PROGRESSIVE: aligned to 32x
    DWORD dwJPEG_DECBUFF_SIZE_PROG;
    //for 16M Thumbnail mode
    DWORD dwJPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_BASE;
    DWORD dwJPEGFMT_BUFF_SIZE_THUMBNAIL_DEC_PROG;
    DWORD dwJPEG_COEFFBUFF_SIZE_THUMBNAIL;
    DWORD dwJPEGFMT_BUFF_TEMP1;
    
    //for 64M extra buffer
    DWORD dwJPEG_DECBUFF_ADDR;
    DWORD dwJPEG_DECBUFF_SIZE;
    DWORD dwJPEG_EXTRABUFF_END;
    DWORD dwJPEG_DISPBUF_SIZE;
    DWORD dwJPEG_DISPBUF_SIZE_Y;
    WORD wJPEG_DISPBUF_WIDTH;
    WORD wJPEG_DISPBUF_HEIGHT;
    WORD wDefineMaxOrigWidth ;
    WORD wDefineMaxOrigHeight;
    WORD wIsDisplayFullScreen;
    volatile WORD wDRAMType;
    
} JPEG_BUFFER_SET, *PJPEG_BUFFER_SET;

typedef struct tagJPEG_SETDECODE_SC
{
    DWORD dwdecode_buffer_size;
    WORD wdecode_strip_y_nr;//record one picture need to segment how many strip to do decoding SC for Y
    //WORD wdecode_strip_uv_nr;//record one picture need to segment how many strip to do decoding SC for UV
    WORD wdecode_pixel_y_nr;//record one strip need to segment how many pixel to do decoding SC for Y 
    //WORD wdecode_pixel_uv_nr;//record one picture need to segment how many pixel to do decoding SC for UV
    WORD wdecode_Y_SCratio_v;
    //WORD wdecode_UV_SCratio_v;
    WORD wdecode_Y_SCratio_h;
    WORD wdecode_dest_width;
    WORD wdecode_dest_height;
    WORD wdecode_orig_width;
    WORD wdecode_orig_height;
    WORD wMaxOrigWidth;
    WORD wMaxOrigHeight;
    //WORD wcounter_picture_height;
    
    
}JPEG_SETDECODE_SC, *PJPEG_SETDECODE_SC;

typedef struct tagJPEG_SET_SC_ZONE
{
    DWORD dwSrcAddressY;
    DWORD dwSrcAddressUV;
    DWORD dwDstAddressY;
    DWORD dwDstAddressUV;
    WORD wSrc_width;
    WORD wSrc_height;
    WORD dest_width;
    WORD dest_height;
    //WORD WSrc_strip;
}JPEG_SET_SC_ZONE, *PJPEG_SET_SC_ZONE;

typedef struct tagJPEG_CALC_MC_SCALE_SET
{
    DWORD DecodedBufferSize;
    DWORD dwStripDecodedSize;
    WORD wMaxDecodingHeight;
    WORD wPicWidth;
    WORD wPicHeight;
}JPEG_CALC_MC_SCALE_SET, *PJPEG_CALC_MC_SCALE_SET;

//#ifdef JPEG_USE_TWO_PROCESSOR
//#ifdef CT909R_JPEG_AND_MP3
#if defined(JPEG_USE_TWO_PROCESSOR) || defined (CT909R_JPEG_AND_MP3)
//senshong:check two processor status
//processor 2 start decoder header
#define JPEG_STATUS_PARSE_HEADER_ST     4
//processor 2 start decoder vld and IDCT
#define JPEG_STATUS_DEC_ST                        5

//JPEG decoder status for VLD, IDCT and JPU scale
#define JPEG_WAIT_DEC 0
#define JPEG_WAIT_JPU 1
#define JPEG_DEC_FINISH 2

typedef struct tagJPEGFMT_DECODE_INFO
{
    WORD wTq[4];
    WORD wSampleFactor_h[4];
    WORD wSampleFactor_v[4];
    WORD wRealSampleType;
    WORD wScaleRatioH;
    WORD wScaleRatioV;
    WORD wScaleRatioExpH;
    WORD wScaleRatioExpV;
    WORD wdec_pixel_y_nr;
    WORD wAdjustScaleRatio;
    WORD wMaxHorizontalFreq;
    WORD wMaxVerticalFreq;
    WORD wMcuHeight;
    WORD wMcuWidth;
    WORD wMcuRows ;
    WORD wMcuCols ;
    WORD wJPEGFMTFrameHeight ;
    WORD wJPEGFMTFrameWidth ;
    WORD wJPEGFMTExifImageWidth ;
    WORD wJPEGFMTExifImageHeight;
    WORD wComponentCount;
    WORD wJPEGFMTIsProgressive;
    WORD wReservedCoeffNr;
    WORD wIsGetJPEGThumbSize;
    WORD wIsCTKEncodeInfo;
    CHAR  cJPEGFMTExifDate[11];
    BYTE bMcScale;
    BYTE bMcScale_UV;
    BYTE  bNeedMcScaleFormatTransform;
    
}JPEGFMT_DECODE_INFO,*PJPEGFMT_DECODE_INFO;
#endif


//#ifndef JPEG_USE_TWO_PROCESSOR

// *** Function Prototype Declaration ***
// Utility Functions
//
#ifdef USE_CT909_VLD
    // Get datastream from VLD
    //static DWORD Get_Bits(DWORD N);
    
    // Use macro substitution for utility functions
    //
    #define GETBYTE()       _JPEGFMT_GetBits(8)
    #define GETWORD()       _JPEGFMT_GetBits(16)
    //#define GETBITS(x)    (BYTE)_JPEGFMT_GetBits(x)
    //#define NEXTBIT()     (BYTE)_JPEGFMT_GetBits(1)
    //#define BYPASS(x)     _JPEGFMT_GetBits(x << 3)
    #define _JPEGFMT_MOREDATA()      1

#else
    #define JPEGSRC     (0x40500000)
    #define JPEGEOF     (0x40800000)

    //#define JPEGDST     (0x40122000)

    // Use self-maintained pointers to access datastream from specific
    // memory address
    BYTE GETBYTE();
    WORD GETWORD();
    BYTE _JPEGFMT_MOREDATA();
    //VOID BYPASS(DWORD count);
    //BYTE NEXTBIT(VOID);
    //BYTE GETBITS(BYTE count);
    //VOID EXITBITMODE(VOID);
    VOID ENTERBITMODE(BYTE initialposition);
#endif

PREFIX_INLINE WORD _JPEGFMT_NEXTBIT(VOID);
PREFIX_INLINE WORD _JPEGFMT_GETBITS(WORD count);
#ifdef TABLE_LOOKUP_VLD
    WORD _JPEGFMT_SHOWBYTE(VOID);
#endif
VOID _JPEGFMT_EXITBITMODE(VOID);
VOID _JPEGFMT_BYPASS(DWORD count);
PREFIX_INLINE SHORT _JPEGFMT_EXTEND(SHORT vv, SHORT tt);

//#endif

//#ifdef CT909P_IC_SYSTEM
#if defined(CT909P_IC_SYSTEM ) || defined(CT909G_IC_SYSTEM)
WORD JPEGFMTDecodeGo(VOID);
#ifdef SUPPORT_MOTION_JPEG
DWORD _JPEGFMT_NextStartCode(VOID);
#endif//#ifdef SUPPORT_MOTION_JPEG
#endif

// calculate IDCT/MC scaling ratio
//senshong2.16:revise BYTE _JPEGFMT_CalcMcScaleRatio(VOID) to BYTE _JPEGFMT_CalcMcScaleRatio(WORD wParseType)
//Because 16M must know current type whether is thumbnail mode.
//BYTE _JPEGFMT_CalcMcScaleRatio(WORD wParseType);
WORD _JPEGFMT_CalcMcScaleRatio(WORD wParseType);
//BYTE _JPEGFMT_CalcMcScaleRatio(VOID);

// calculate target width and height of JPU scaling
//BYTE _JPEGFMT_CalcJpuAlignedLength(WORD *dstAlignedWidth, WORD *dstAlignedHeight);

// do 1st scaling down from Forg-3/Forg-4 to Forg-1
WORD _JPEGFMT_FirstScaleToForg(WORD forg_width, WORD forg_height);


// For JPEG DECODER
//
VOID _JPEGFMT_InitJpegParser(VOID);

// Read header only (from SOI to frame header finished)
// bType: JPEG_PARSE_TYPE_NORMAL or JPEG_PARSE_TYPE_THUMBNAIL
WORD _JPEGFMT_ReadImageHeader(WORD wParseType);

// Read picture contents (from end of frame header to EOI)
WORD _JPEGFMT_ReadImageContent(VOID);

//VOID _JPEGFMT_ReadImage(VOID);
VOID _JPEGFMT_ReadStreamHeader(VOID);
VOID _JPEGFMT_ReadMarker(WORD wParseType);

//senshong2.16:revise _JPEGFMT_ReadFrameHeader(void) to _JPEGFMT_ReadFrameHeader(WORD wParseType) 
//Because 16M must know current type whether is thumbnail mode.
//VOID _JPEGFMT_ReadFrameHeader(VOID);
VOID _JPEGFMT_ReadFrameHeader(WORD wParseType);

//VOID _JPEGFMT_ReadFrameHeader(VOID);

// DQT
VOID _JPEGFMT_ReadQuantizationTable(VOID);
VOID _JPEGFMT_ReadContentsOfDQT(PQUANTTAB table, WORD precision);

// DHT
//VOID _JPEGFMT_ReadHuffmanTable(VOID);
//WORD _JPEGFMT_ReadContentsOfDHT(PHUFFDECODER table);
VOID _JPEGFMT_ReadHuffmanTable(VOID (*Huff_wait_vdFifo)(DWORD ), DWORD (*Huff_getbit)(DWORD));
WORD _JPEGFMT_ReadContentsOfDHT(PHUFFDECODER table,VOID (*Huff_wait_vdFifo)(DWORD ), DWORD (*Huff_getbit)(DWORD));

VOID _JPEGFMT_MakeTable(PHUFFDECODER table, BYTE huffbits[JPEGMAXHUFFMANCODELENGTH]);

// SOS and ECS
VOID _JPEGFMT_ReadScanHeader(VOID);
VOID _JPEGFMT_InitComponentMembers(PCOMPONENT pcomp, WORD bnum);
VOID _JPEGFMT_ReadSequentialScanData(VOID);
VOID _JPEGFMT_ReadSequentialInterleavedScan(VOID);
VOID _JPEGFMT_ReadSequentialNonInterleavedScan(VOID);
VOID _JPEGFMT_ProcessRestartMarker(VOID);
WORD _JPEGFMT_DecodeSequential(PCOMPONENT pcomp, WORD mcurow, WORD mcucol, WORD bnum);
WORD _JPEGFMT_Decode(PHUFFDECODER codec);


// DRI
VOID _JPEGFMT_ReadRestartInterval(VOID);

// Access current (protected) buffer for JPEG decoder
//senshong2.16:revise _JPEGFMT_SetCurrBuffer(WORD wCurrBuff) to _JPEGFMT_SetCurrBuffer(WORD wCurrBuff, WORD wParseType)
//Because 16M must know current type whether is thumbnail mode.
VOID _JPEGFMT_SetCurrBuffer(WORD wCurrBuff, WORD wParseType);
//VOID _JPEGFMT_SetCurrBuffer(WORD wCurrBuff);
WORD _JPEGFMT_GetCurrBuffer(VOID);

//tommy0.68
#define JPEGFMT_ENDIAN_BIG          0
#define JPEGFMT_ENDIAN_LITTLE       1

#define JPEGFMT_TIFF_DATETIME       (0x0132)
#define JPEGFMT_TIFF_EXIFIFD        (0x8769)
#define JPEGFMT_EXIF_IMAGEWIDTH     (0xa002)
#define JPEGFMT_EXIF_IMAGEHEIGHT    (0xa003)
#define JPEGFMT_JPEG_IFOFFSET       (0x0201)
#define JPEGFMT_JPEG_IFBYTECOUNT    (0x0202)
#define JPEGFMT_THUMB_COMPRESSION   (0x0103)

typedef union
{
    BYTE  byte4[4];
    DWORD dword;
} JPEGFMT_BYTE4, *PJPEGFMT_BYTE4;

typedef union
{
    BYTE byte2[2];
    WORD word;
} JPEGFMT_BYTE2, *PJPEGFMT_BYTE2;

WORD  _JPEGFMT_SwapByte2(WORD in_value);
DWORD _JPEGFMT_SwapByte4(DWORD in_value);
WORD  _JPEGFMT_ReadApp1Header(VOID);
VOID  _JPEGFMT_ReadExif(WORD wParseType);
WORD  _JPEGFMT_ReadExifTag(VOID);

//tommy0.81
VOID _JPEGFMT_ReadExifPrivateIFD(VOID);
VOID _JPEGFMT_ReadExifIFD1(WORD wParseType, DWORD dwOffsetIFD1);

//tommy0.91
VOID _JPEGFMT_ReadExifDate(VOID);

//tommy0.95: support progressive JPEG decoding
// (functions from jpgdecoder.cpp)
VOID _JPEGFMT_ReadProgressiveScanData(WORD sss, WORD sse, WORD sah, WORD sal);
VOID _JPEGFMT_ReadDcFirst (WORD ssa);
VOID _JPEGFMT_ReadDcRefine(WORD ssa);
VOID _JPEGFMT_ReadAcFirst (WORD sss, WORD sse, WORD ssa);
VOID _JPEGFMT_ReadAcRefine(WORD sss, WORD sse, WORD ssa);

VOID _JPEGFMT_UpdateImage(VOID);

// (functions from jpgdecodercomponent.cpp)
VOID _JPEGFMT_DecodeDcFirst (PCOMPONENT pcomp, WORD row, WORD col, WORD ssa);
VOID _JPEGFMT_DecodeDcRefine(PCOMPONENT pcomp, WORD row, WORD col, WORD ssa);
VOID _JPEGFMT_DecodeAcFirst (PCOMPONENT pcomp, WORD row, WORD col, WORD sss, WORD sse, WORD ssa);
VOID _JPEGFMT_DecodeAcRefine(PCOMPONENT pcomp, WORD row, WORD col, WORD sss, WORD sse, WORD ssa);

VOID _JPEGFMT_RefineAcCoefficient(WORD ssa, SHORT *value);
VOID _JPEGFMT_ExecProgressiveIDCT(PCOMPONENT pcomp, WORD mcurow, WORD mcucol, WORD bcomp, DWORD index);

// (other functions)
VOID _JPEGFMT_CalcReservedCoeffNumber(PCOMPONENT pcomp, DWORD buffsize);

//senshong2.16:revise _JPEGFMT_ClearCoeffBuffer(VOID) to _JPEGFMT_ClearCoeffBuffer(WORD wParseType)
//Because 16M must know current type whether is thumbnail mode.
VOID _JPEGFMT_ClearCoeffBuffer(WORD wParseType);
//VOID _JPEGFMT_ClearCoeffBuffer(VOID);

VOID _JPEGFMT_ClearBuffBeforeDecode(VOID);

//tommy1.05: fixed calculation
DWORD _JPEGFMT_CalcDecodingBufferSize(WORD ratioH, WORD ratioV);

//tommy1.10
WORD _JPEGFMT_HasJpegThumb(VOID);

//DWORD JPEG_FrameBufferSetting(DWORD dwFramebufferStart, DWORD dwFramebufferEnd, DWORD dwExtrabufferStart, DWORD dwExtrabufferEnd ,WORD wDRAMType);
//senshong2.17 seting frame buffer
WORD JPEG_FrameBufferSetting(PJPEG_BUFFER_SET pFrameBufferSeting);
DWORD JPEG_SetFrameBufferAddress(DWORD dwFramebufferStart, DWORD dwFramebufferEnd, DWORD dwOrigBufferStart, DWORD dwOrigBufferEnd ,WORD wDRAMType);


//senshong2.18: add new code to solve to quality
VOID _JPEGFMT_SetDecodingBuffer(PCOMPONENT pcomp, WORD bnum);
WORD _JPEGFMT_CalcDecodingSCRatio(WORD MC_ratioH, WORD MC_ratioV, PJPEG_CALC_MC_SCALE_SET pJPEG_McScaleSet);
WORD _JPEGFMT_CalcDecodingStrip(WORD SC_ratio_v, WORD MC_ratioV, WORD picWidth, WORD picHeight, DWORD DecodedBufferSize);
WORD _JPEGFMT_DecodingSC(PJPEG_SET_SC_ZONE PDec_SC_zone);
WORD _JPEGFMT_SetScalingBufferAddress(PJPEG_SET_SC_ZONE PAddr_zone);
WORD _JPEGFMT_CheckSCHorizontalStep(WORD realWidth, WORD scaledWidth, WORD wsample_type);
WORD _JPEGFMT_UseJPUMovePictureData(WORD scaledWidth, WORD scaledHeight);

//senshong: Strip scale Y component
WORD _JPEGFMT_StripScaleYComponent(PJPEG_SET_SC_ZONE PDec_SC_zone);
//senshong: Strip scale UV compoment
WORD _JPEGFMT_StripScaleUVComponent(PJPEG_SET_SC_ZONE PDec_SC_zone);
WORD _JPEGFMT_JPUScaling(WORD wSrcWidth, WORD wSrcHeight, WORD wIsCopyPicture);
//WORD _JPEGFMT_CleanUVFrameBuffer(PJPEG_SET_SC_ZONE PClean_buffer_zone, WORD wSample_type);

#ifdef SUPPORT_MOTION_JPEG
WORD _JPEGFMT_UVFormatTransform(PJPEG_SET_SC_ZONE PDec_SC_zone);
DWORD _JPEGFMT_GetHuffBit(DWORD n);
VOID _JPEGFMT_wait_dummy_vdfifo(DWORD n);
WORD _JPEGFMT_CalcMJPEGMcScaleRatio(VOID);
VOID _JPEGFMT_InitHuffmanTable(VOID);
#endif

#ifdef CT909R_JPEG_AND_MP3
//senshong: add API to support jpeg+MP3, this version only support Ct909R
WORD _JPEGFMT_ReadImageHeaderForProcessor2(WORD wParseType);
WORD _JPEGFMT_ReadImageContentForProcessor2(VOID);
VOID _JPEGFMT_ReadSequentialScanForProcressor2(VOID);
#endif
WORD _JPEGFMT_ReadThumbMarker(VOID);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__JPEG_FORMAT_H__

// END OF FILE (jpegfmt.h)
