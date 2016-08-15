//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2006,
//          J500, all right reserved.
//
//      Product : CT909
//
//      Author  : Cheertek (J500 Tommy)
//      Purpose : JPEG Decoder
//      Sources : 
//  **************************************************************************
//
#ifndef __JPEG_DECODER_H__
#define __JPEG_DECODER_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include "haljpeg.h"
//#include "jpegfmt.h"
// *** PLATFORM dependent ***
//
//tommy0.80: separate error msgs from debug msgs
#define JPEG_DECODER_ERRMSG

// Compile for eCos909/ CheerDVD, with threads and using synchronization mechanisms
#define JPEG_DECODER_ECOS

//tommy1.21: define JPEG thread timed-out seconds for wait VDRemainder
//tommy1.50: 2 is enough, but it's better to use 3 for worst loader/servo case.
#define JPEG_WAIT_VDREM_TIMEDOUT_COUNT          (COUNT_3_SEC)

//tommy2.05: define JPU scale sampling mechanism: INTERPOLATION or NEAREST
#define JPEG_SCALE_SAMPLING_INTERPOLATION
//#define JPEG_SCALE_SAMPLING_NEAREST

//tommy2.11: fix CT909S JPU horizontal scale down defect
#define PATCH_CT909S_JPU_SCALE_DOWN

//tommy2.11: [test code] patch src width to eliminate possible fractal part
#undef PATCH_CT909S_JPU_SCALE_DOWN_2

//tommy2.16: switch to apply CT909S' JPU SC+FR combo-operation feature
//#define USE_JPU_SCFR_COMBO_OPERATION_PATCH

//tommy2.17: switch to check and print for HVSC_FACTOR horizontal scale-down defect
#ifdef PATCH_CT909S_JPU_SCALE_DOWN
    #undef CHECK_AND_PRINT_FOR_CT909S_JPU_SCALE_DOWN
#endif

#undef NO_USE_JPU_SCFR_COMBO_OPERATION_PATCH

//senshong2.17:switch to use JPU SC+FR combo or use JPU SC and JPU FR
//#define NOT_USE_JPU_SCFR_COMBO

//#define JPEG_DEC_DEBUG

#define JPEG_DRAM_16M 1
#define JPEG_DRAM_32M 2
#define JPEG_DRAM_64M 3

#define JPEG_EFFECT_NONE    0
#define JPEG_EFFECT_TRUE    1
#define JPEG_EFFECT_DISP_BUFFER_2_TO_0  1
#define JPEG_EFFECT_DISP_BUFFER_1_TO_0  2

#define JPEG_DISP_SIZE_FOR_16M  0x69000

//tommy2.16: macroes for VPU lock and unlock
//
//#if (PLATFORM == ECOS_OS) && !defined(ECOS909)
    #define LOCK_MUTEX_VPU      OS_LockMutex(&__mutexGDIIdle);
    #define UNLOCK_MUTEX_VPU    OS_UnlockMutex(&__mutexGDIIdle);
//#else   //(PLATFORM == ECOS_OS) && !defined(ECOS909)
    //#define LOCK_MUTEX_VPU      
    //#define UNLOCK_MUTEX_VPU    
//#endif  //(PLATFORM == ECOS_OS) && !defined(ECOS909)

//tommy2.16: fix the extra pixels by first failed writing ("writo") of CT909S' JPU FR operation
#define PATCH_CT909S_JPU_FLIPROTATE_FIRST_WRITO

//tommy2.16b: fix the interpolation defect of CT909S' JPU SC+FR operation (within vertical scaling)
#define PATCH_CT909S_JPU_SCFR_VERTICAL_SCALE


//#define CT909P_SUPPORT_MJPEG


// --------------------
// public declarations
// --------------------
//Parse the header of normal JPEG or thumbnail
//
VOID JPEG_ParseHeader(BYTE bType, BYTE bDecodeBuff);
//bType:
//JPEG_PARSE_TYPE_NORMAL        Parse the normal JPEG.
//JPEG_PARSE_TYPE_THUMBNAIL     Parse the thumbnail. If there is no thumbnail in the file, parse the normal JPEG automatically.
//tommy0.67:
//bDecodeBuff: specify the protected buffer; 0 or 1.

//Decode JPEG
//
VOID JPEG_Decode(PJPEGDECODE pDecode);

#ifdef SUPPORT_MOTION_JPEG
VOID JPEG_MotionDecode(VOID);
#endif
//pDecode: The decoding relative attributes.

//Do zoom/rotate/flip operations
//
VOID JPEG_Operate(PJPEGSCALING pScaling);
//pScaling: The scaling relative attributes.

//Do "reset" before decoding each JPEG picture
//
VOID JPEG_Reset(VOID);

//Preset the frame buffer
//
VOID JPEG_PresetFrameBuf(BYTE bBuffer, URECT *pRect, DWORD dwColor);
//bBuffer:  Frame buffer 0 or 1. 
//wWidth:   The width of the block we want to fill in the frame buffer. (pixels)
//wHeight:  The height  of the block we want to fill in the frame buffer. (pixels)
//dwColor:  The color we want to fill into the frame buffer.

//Call this function to get the status of JPEG Decoder.
//
BYTE JPEG_Status(BYTE bStatusType);
//bStatusType:
//JPEG_PARSE_HEADER     Get "parse header" status.
//JPEG_DECODE           Get "Decode" status.
//JPEG_OPERATION        Get "Operate" status.
//JPEG_PSFB             Get "Preset frame buffer" status.
//JPEG_RESET            Get "reset" status.
//return:
//JPEG_STATUS_UNFINISH  The action is unfinished.
//JPEG_STATUS_UNSUPPORT This return value is for "parse header" to represent that the JPEG file can't be decoded in our solution.
//JPEG_STATUS_OK        The action is OK.
//JPEG_STATUS_FAIL      The action is failed.

//Abort JPEG decoding.
//
VOID JPEG_Abort(VOID);

VOID JPEG_SetSharpRatio(WORD wFristNoInterpolnterpolation,WORD wSecondNoInterpolnterpolation);
// --------------------
// local macro definitions
// --------------------

// MACRO definitions
// IS THERE ANY TESTBENCH for these MACROES?

#define MBINT_CTL_JPEG_ST           (0x800)

// align to 32x.
#define ALIGN32(src)                (((src) + 31) & 0xffe0)
#define STRIP32(src)                ((src) & 0xffe0)

// align to 16x.
#define ALIGN16(src)                (((src) + 15) & 0xfff0)
#define STRIP16(src)                ((src) & 0xfff0)

// align to 4x.
#define ALIGN04(src)                (((src) + 3) & 0xfffc)
#define STRIP04(src)                ((src) & 0xfffc)

// align to 8x.
#define ALIGN08(src)                (((src) + 7) & 0xfff8)
#define STRIP08(src)                ((src) & 0xfff8)

// align to 2x.
#define ALIGN02(src)                (((src) + 1) & 0xfffe)
#define STRIP02(src)                ((src) & 0xfffe)

// set value for the REG_JPU_CTL
// required parameters and their mapping field:
//
//make sure GPU_OP = 0, for JPU operation
#ifdef JPEG_SCALE_SAMPLING_INTERPOLATION
    #define TRANS_JPU_CTL(type, op, uv)             ((0 << 28) | ((type) << 12) | ((op)) | ((uv) << 3))
    #define TRANS_JPU_CTL_FR(type, op, frtype, uv)  (((type) << 12) | ((op)) | ((frtype) << 8) | ((uv) << 3))
    #define TRANS_JPU_CTL_NO_INTERPOLATION(type, op, uv)             ((0 << 28) | JPU_V_NEAREST_EN | JPU_H_NEAREST_EN | ((type) << 12) | ((op)) | ((uv) << 3))
#else   //NEAREST
    #define TRANS_JPU_CTL(type, op, uv)             ((0 << 28) | JPU_V_NEAREST_EN | JPU_H_NEAREST_EN | ((type) << 12) | ((op)) | ((uv) << 3))
    #define TRANS_JPU_CTL_FR(type, op, frtype, uv)  (JPU_V_NEAREST_EN | JPU_H_NEAREST_EN | ((type) << 12) | ((op)) | ((frtype) << 8) | ((uv) << 3))
#endif

// set register REG_MC_CTL for MC scaling H/V ratios
// mcctl[2:1], horizontal scale down setting
// mcctl[4:3], vertical scale down setting
// clear the bits of vertical and horizontal scaling ratios first.
// then fill mask-ed ratio values into these bit fields.
//
//tommy1.50: fix CLEAR_MCCTL_SCALE
//#define CLEAR_MCCTL_SCALE()                 (REG_MC_CTL &= 0xe1)
#define CLEAR_MCCTL_SCALE()                 (REG_MC_CTL &= 0xFFFFFFE1)
#define TRANS_MCCTL_SCALE(v_ratio,h_ratio)  ((((v_ratio) & 0x3) << 3) | (((h_ratio) & 0x3) << 1))
#ifdef CT909G_IC_SYSTEM
#define TRANS_JPGHV_SCALE(y_v_ratio,y_h_ratio,uv_v_ratio,uv_h_ratio)  ((((y_h_ratio) & 0x3) ) | (((uv_h_ratio) & 0x3) << 2) |(((uv_h_ratio) & 0x3) << 4) |(((y_v_ratio) & 0x3) << 6) |(((uv_v_ratio) & 0x3) << 8)|(((uv_v_ratio) & 0x3) << 10))

#endif

// calculate value for JPU's heiwid_src and  heiwid_dst registers
//tommy1.50: adjust for modifications on CT909S' JPU HEIWID_SRC and HEIWID_DST registers
#define TRANS_HEIWID_REG(width,height)      (((width) << 16) | (height))

// calculate value for JPU's stripe register
//tommy1.50: adjust for modifications on CT909S' JPU STRIPE_RW register
#define GET_STRIPE(src)                     (((src) + 15) >> 4)
#define GET_STRIPE_UV(src)                  (((src) +  7) >> 3)
#define TRANS_STRIPE_REG(src,dst)           ((GET_STRIPE(src)    << 16) | (GET_STRIPE(dst)))
#define TRANS_STRIPE_REG_UV(src,dst)        ((GET_STRIPE_UV(src) << 16) | (GET_STRIPE_UV(dst)))

// calculate starting address of write data for flip & rotate operation (of YUV420)
// translation expression is from D400/Raiden.
// block address * 8 = byte address
//
//tommy1.50: adjust for modifications on CT909S' JPU STRIPE definition
//tommy2.03: update FR_OFFSET expressions (for SAMPLE420 format only)
#define TRANS_FR_OFFSET_Y(width,x,y)        (((((y) >> 4) * (GET_STRIPE(width) << 6)) + ((y) % 16) + (((x) >> 2) << 4)) << 2)
//version1: use Y's width & height (use real width)
#define TRANS_FR_OFFSET_UV(width,x,y)       ((((((y) >> 1) >> 4) * (GET_STRIPE_UV(width >> 1) << 6)) + (((y) >> 1) % 16) + ((((x) >> 1) >> 3) << 6) + (((((x) >> 1) % 8) >> 2) << 4)) << 2)
//version2: use UV's actual width & height (but use real width)
#define TRANS_FR_OFFSET_UV2(width,x,y)      (((((y) >> 4) * (GET_STRIPE_UV(width >> 1) << 6)) + ((y) % 16) + (((x) >> 3) << 6) + ((((x) % 8) >> 2) << 4)) << 2) 




//#ifdef CT909P_IC_SYSTEM
#if defined(CT909P_IC_SYSTEM ) || defined(CT909G_IC_SYSTEM)

#define GET_SCALE_FACTOR(src,dst,uv)            (((((src)) - ((src) < (dst))) << 13) / (((dst)) - ((src) < (dst))))
//senshong:Because 909p jpu has one bug that can cause jpu infinite operate
//So we calculate horizontal step that must use ((src -1)<<13)/(dst -1) formula
#define GET_SCALE_FACTOR2(src,dst)              ((((src) - 1) << 13) / ((dst) - 1))
#define TRANS_HVSC_REG(srcW,srcH,dstW,dstH,uv)  ((GET_SCALE_FACTOR(srcH,dstH,uv) << 16) | (GET_SCALE_FACTOR2(srcW,dstW)))

#else
// calculate a 32-bit word of veritical & horizontal steps for JPU's HVSC factor register.
//
//#define GET_SCALE_FACTOR(src,dst,uv)            (((((src) / ((uv) + 1)) - ((src) < (dst))) << 13) / (((dst) / ((uv) + 1)) - ((src) < (dst))))
#define GET_SCALE_FACTOR(src,dst,uv)            (((((src)) - ((src) < (dst))) << 13) / (((dst)) - ((src) < (dst))))
#define TRANS_HVSC_REG(srcW,srcH,dstW,dstH,uv)  ((GET_SCALE_FACTOR(srcH,dstH,uv) << 16) | (GET_SCALE_FACTOR(srcW,dstW,uv)))

//senshong2.51:909P has be fixed for jpu scale down defect
//#ifndef CT909P_IC_SYSTEM
//tommy2.11: fix for JPU scale down defect
#ifdef PATCH_CT909S_JPU_SCALE_DOWN
    #define GET_SCALE_FACTOR2(src,dst)              ((((src) - 1) << 13) / ((dst) - 1))
    //#define GET_SCALE_FACTOR2(src,dst,uv)              (((((src) /((uv)+1)) - 1) << 13) / (((dst) / ((uv) + 1)) - 1))
    //tommy2.11: patch for horizontal only
    #define TRANS_HVSC_REG2(srcW,srcH,dstW,dstH,uv) ((GET_SCALE_FACTOR(srcH,dstH,uv) << 16) | (GET_SCALE_FACTOR2(srcW,dstW)))
    #define FIX_JPU_SCALE_DEFECT(srcW,srcH,dstW,dstH,uv)    if ((srcW) > (dstW))                                                                    \
                                                            {                                                                                       \
                                                                DWORD check_value = (((srcW) << 13) / (dstW)) * ((dstW) - 1);                       \
                                                                if ((0x6000 == (check_value & 0x6000)) && (0 != (check_value & 0x1fff)))            \
                                                                {                                                                                   \
                                                                    REG_JPU_HVSC_FACTOR = TRANS_HVSC_REG2(srcW, srcH, dstW, dstH, uv);              \
                                                                }                                                                                   \
                                                            }

    #define FIX_JPU_SCALE_DEFFECT_H_STEP(srcW,dstW,HStep)			if ((srcW) > (dstW))                                                            \
                                                            {                                                                                       \
                                                                DWORD check_value = (((srcW) << 13) / (dstW)) * ((dstW) - 1);                       \
                                                                if ((0x6000 == (check_value & 0x6000)) && (0 != (check_value & 0x1fff)))            \
                                                                {                                                                                   \
                                                                    HStep = GET_SCALE_FACTOR2(srcW,dstW);											\
                                                                }                                                                                   \
                                                            }
//tommy2.16: error message removed. DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "PATCH: H part of HVSC_FACTOR.");
#else   //PATCH_CT909S_JPU_SCALE_DOWN
    //#define FIX_JPU_SCALE_DEFECT(srcW,srcH,dstW,dstH,uv)    
    #define FIX_JPU_SCALE_DEFECT(srcW,srcH,dstW,dstH)    
#endif  //PATCH_CT909S_JPU_SCALE_DOWN

#endif//#ifdef CT909P_IC_SYSTEM




//tommy1.50: support CT909S
#define MACRO_RESET_JPU()       REG_PLAT_RESET_CONTROL_ENABLE  = PLAT_RESET_VPU_ENABLE;     \
                                REG_PLAT_RESET_CONTROL_DISABLE = PLAT_RESET_VPU_DISABLE;

// --------------------
// local declarations
// --------------------
// sampling types
//
#define SAMPLE420			0
#define SAMPLE440			1
#define SAMPLE422			2
#define SAMPLE444			3
#define SAMPLE400			4

// flip & rotate types
//
#define FR_ORIGINAL			0
#define FR_R90				1
#define FR_R180				2
#define FR_R270				3
#define FR_FH				4
#define FR_FH_R90			5
#define FR_FH_R180			6
#define FR_FH_R270			7

// block parts
//
#define BLOCK_Y				0
#define BLOCK_UV			1
#define BLOCK_ALL			2

// parse type
#define  JPEG_PARSE_TYPE_NORMAL		0
#define  JPEG_PARSE_TYPE_THUMBNAIL	1
#define  JPEG_PARSE_TYPE_THUMB_ENCODE      2
#define  JPEG_PARSE_TYPE_THUMB_BACKGROUND 3

#define	 JPEG_SLIDESHOW_ENCODE_NONE	0
#define	 JPEG_SLIDESHOW_ENCODE	1
// local function prototypes
//

// JPEG Decoder entrance for eCos thread
VOID JPEG_ThreadMain(DWORD data);
VOID JPEG_ThreadExit(WORD data);
VOID JPEG_ThreadInit(VOID);

// JPU PART
VOID _JPEGDEC_InitJPU(VOID);
WORD _JPEGDEC_RunJPU(VOID);
DWORD _JPEGDEC_GetBlockSize(WORD width, WORD height, WORD uv);
DWORD _JPEGDEC_GetDispFrameOffset(WORD x, WORD y, WORD uv);
DWORD _JPEGDEC_GetNormFrameOffset(WORD width, WORD x, WORD y, WORD uv);
VOID _JPEGDEC_ClearBuffBeforeOperate(WORD wMatchingRule, WORD bFrameBuffer);

// JPEG Decoder wrapper
WORD _JPEGDEC_Decode(PJPEGDECODE pDecode);
WORD _JPEGDEC_Operate(PJPEGSCALING pScaling);
WORD _JPEGDEC_PresetFrameBuf(WORD wBuffer, URECT *pRect, DWORD dwColor);

//tommy2.15: dedicated structure for JPUUTIL API and 16M solution
typedef struct tagJPU_OPZONE
{
    WORD    wPosCoordH;
    WORD    wPosCoordV;
    WORD    wWidth;
    WORD    wHeight;
    WORD    wFrameStripe;
} JPU_OPZONE, *PJPU_OPZONE;

typedef struct tagJPEG_ENCODE_INFO
{
    DWORD dwPicAddrY;
    DWORD dwPicAddrUV;
    DWORD dwPoolAddrSt;
    DWORD dwPoolLength;
    WORD wPicWidth;
    WORD wPicHeight;
    WORD wPicCoordH;
    WORD wPicCoordV;
    WORD wPicStrip;
    WORD wExifWidth;
    WORD wExifHeight;
    CHAR cExifDate[11];
}JPEG_ENCODE_INFO, *PJPEG_ENCODE_INFO;

WORD _JPUUTIL_FillColor(DWORD dwDestAddress, PJPU_OPZONE pDestination, DWORD wFillColor, WORD wFillOp, WORD wYuvType);
//WORD _JPUUTIL_ScaleAndFlipRotate(DWORD dwSrcAddress, DWORD dwDestAddress, WORD wFrType, PJPU_OPZONE pSource, PJPU_OPZONE pDestination, WORD wYuvType);
DWORD _JPUUTIL_GetDispFrameOffset_SCFR(PJPU_OPZONE pzone, WORD type_yuv, WORD type_fr);
WORD _JPUUTIL_Scale(DWORD dwSrcAddress, DWORD dwDestAddress, PJPU_OPZONE pSource, PJPU_OPZONE pDestination, WORD wYuvType);
WORD _JPUUTIL_FlipRotate(DWORD dwSrcAddress, DWORD dwDestAddress, DWORD dwtagaddress, WORD wFrType, PJPU_OPZONE pSource, PJPU_OPZONE pDestination, WORD wYuvType);

VOID JPEGDEC_EffectConfigure(BYTE wEffectConfigure);
VOID JPEGDEC_CopyPictureToDisplayBuffer(BYTE bBufferSet);
BYTE JPEGDEC_TransformOperation(BYTE bPreviousResult, WORD bCurrentOperation);
WORD _JPUUTIL_FlipRotateAndCopyPic(DWORD dwSrcAddress, DWORD dwDestAddress, WORD wFrType, PJPU_OPZONE pSource, PJPU_OPZONE pDestination);
VOID JPEG_DisplayFullScreen(WORD wIsFullScreen);
//VOID JPEGDEC_ReportYUVAddr(PDWORD dwYAddr, PDWORD dwUVAddr,PWORD wPicWidth, PWORD wPicHeight, PDWORD dwPool, PDWORD dwPoolLength);
VOID JPEGDEC_ReportEncodedInfo(PJPEG_ENCODE_INFO pEncodedInfo);
VOID JPEGDEC_ConfigProcessor(WORD wConfigProcessor);
DWORD JPEGDEC_QueryEmptyBuffer(VOID);
VOID JPEGDEC_SlideShowEncodeMode(WORD wSlideShowEncode);
VOID JPEGDEC_SrcPicOffset(WORD wOffsetH, WORD wOffsetV);
VOID JPEGDEC_QuerySrcPicSize(PWORD pwSrcPicWidth,PWORD pwSrcPicHeight);
#ifdef SUPPORT_MOTION_JPEG
VOID JPEGDEC_MjpegFrameBufferConfigure(DWORD dwFramebufferStart, DWORD dwFramebufferEnd,WORD wDecoderMode);
DWORD _JPEGDEC_CheckPlayCommand(DWORD dwType);
VOID JPEGDEC_UpdatePictureInfo(VOID);
#endif
VOID JPEGDEC_ColorTransform(DWORD dwFrameAddrY,DWORD dwFrameAddrUV,PJPU_OPZONE pDest_zone);
VOID JPEGDEC_EnableColorTransform(WORD wEnableTransform);
DWORD JPUUTIL_FillBlock(WORD wDispIndex,WORD wCoordH,WORD wCoordV,WORD wPicWidth, WORD wPicHeight ,WORD wBlockType);
DWORD JPUUTIL_FillBlockColor(WORD wDispIndex,WORD wCoordH,WORD wCoordV,WORD wPicWidth, WORD wPicHeight,DWORD dwFillColor);
//#define _JPUUTIL_COORD_TO_BYTEOFFSET_Y(coord_x, coord_y)    (((((coord_y) >> 4) * 180) + ((coord_x) >> 2)) << 6) + (((coord_y) % 16) << 2)
//#define _JPUUTIL_COORD_TO_BYTEOFFSET_UV(coord_x, coord_y)   (((((coord_y) >> 5) * 180) + (((coord_x) >> 4) << 2) + ((((coord_x) >> 1) % 8) >> 2)) << 6) + ((((coord_y) >> 1) % 16) << 2)
#define _JPUUTIL_COORD_TO_BYTEOFFSET_Y(coord_x, coord_y, disp_width)    (((((coord_y) >> 4) * (((disp_width) >> 3) << 1)) + ((coord_x) >> 2)) << 6) + (((coord_y) % 16) << 2)
#define _JPUUTIL_COORD_TO_BYTEOFFSET_UV(coord_x, coord_y, disp_width)   (((((coord_y) >> 5) * (((disp_width) >> 3) << 1)) + (((coord_x) >> 4) << 2) + ((((coord_x) >> 1) % 8) >> 2)) << 6) + ((((coord_y) >> 1) % 16) << 2)

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__JPEG_DECODER_H__

// END OF FILE (jpegdec.h)
