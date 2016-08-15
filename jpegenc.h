#ifndef __JPEGENC_H__
#define __JPEGENC_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

//#include "jpegenc/jpeglib.h"

//default: off
#undef JPEGENC_EVAL_PERFORMANCE

// ---------------------------------
//JPEG encoder constants
// ---------------------------------
#define JPEGENC_REQUIRED_MEMPOOL_SIZE           (0xb0b4)    //bytes

//API definitions
#define JPEGENC_COMPRESSION_FILESIZE_MAX        (64*1024)   //max storage size for generated JPEG file, in bytes
#define JPEGENC_COMPRESSION_RETRY               (TRUE)
#define JPEGENC_COMPRESSION_QUALITY_DEFAULT     (90)        //regular experienced value from CJPEG simulation results; 95 isn't good because almost every picture will need to be compressed 2 times
#define JPEGENC_COMPRESSION_QUALITY_DEFSTEP     (5)         //this may be 10 -- because it doesn't affect file size so much
#define JPEGENC_COMPRESSION_IMAGE_WIDTH         (720)       //CT951 full screen: 720x448 pixels (display buffer)
#define JPEGENC_COMPRESSION_IMAGE_HEIGHT        (448)       //original buffer for JPEG decoder: 640x416 pixels

//return status of compression
#define JPEGENC_COMPRESSION_SUCCESSED           (0)
#define JPEGENC_COMPRESSION_RUNNING             (1)
#define JPEGENC_COMPRESSION_SIZE_EXCEEDED       (2)
#define JPEGENC_COMPRESSION_LIBJPEG_ABORTED     (3)
#define JPEGENC_COMPRESSION_USER_ABORTED        (4)
#define JPEGENC_COMPRESSION_EXIT_FAILED         (5)
#define JPEGENC_COMPRESSION_MEM_NOT_ENOUGH      (6)
#define JPEGENC_COMPRESSION_MEM_ALREADY_INIT    (7)
#define JPEGENC_COMPRESSION_EMPTY               (8)

// ---------------------------------
//function prototype declarations
// ---------------------------------
// simple malloc replacement
//
VOID *JPEGENC_Malloc(DWORD byte_count);

// simple free replacement
//
VOID JPEGENC_Free(VOID *dest);

// sample of JPEG encoder processing flow:
// 1. one-entry calling start point;
// 2. customized for CT951
//
WORD JPEGENC_StartCompression(DWORD mempool, DWORD len, DWORD start_y, DWORD start_uv, WORD width, WORD height, WORD PicStrip, WORD PicCoordH, WORD PicCoordV, WORD quality, WORD quality_step, DWORD max_filesize);

//mempool: starting address of memory pool, for both JPEG file (from the beginning) and function JPEGENC_Malloc() (from the tail)
//len: size of memory pool, in bytes
//start_y: Y starting address of source image
//start_uv: UV starting address of source image
//width: width of source image
//height: height of source image
//PicCoordH: Source picture horizontal started coord at frame buffer
//PicStrip: frame buffer's strip
//quality: starting quality of compression. If the generated JPEG file size is exceeded, JPEG encoder will adjust this value smaller automatically.

// ---------------------------------
//other open API for calling modules
// ---------------------------------
//API export for upper-layer to adjust factors of input region for compression
//
VOID JPEGENC_SetCompressRegionSize(WORD width_value, WORD height_value);

//API export for upper-layer to specify input region for compression
//
VOID JPEGENC_SetCompressRegionAddress(DWORD starting_address_y, DWORD starting_address_uv);

//API export for upper-layer to adjust the maximum value of the storage slot for generated JPEG file
//
VOID JPEGENC_SetCompressFileSizeMax(DWORD max_filesize_in_byte);

//API export for upper-layer to adjust the starting value for compression quality
//
VOID JPEGENC_SetCompressQuality(WORD quality_value);

//API export for upper-layer to adjust the starting value for compression quality
//
VOID JPEGENC_SetCompressQualityStep(WORD quality_step_value);

//API export for upper-layer to recordinformation of the original image, then compose them into an APP2 marker
//
VOID JPEGENC_SetApp2InfoAddressNSize(DWORD info_start_address, WORD info_total_size);

//utility functions for JPEG encoder
//
WORD JPEGENC_InitMemPool(DWORD mempool_addr, DWORD mempool_size);

//API export for upper-layer to reset initialisation values of the JPEG compression module
//
VOID JPEGENC_Abort(DWORD cinfo);
//VOID JPEGENC_Abort(j_compress_ptr cinfo);

//API export for upper-layer to reset initialisation values of the JPEG compression module
//
VOID JPEGENC_Reset(VOID);

//API export for upper-layer to get current status of JPEG encoder
//
WORD  JPEGENC_GetStatus(VOID);

//API export for upper-layer to get generated JPEG file size by JPEG encoder
//
DWORD JPEGENC_GetCompressedSize(DWORD mempool_start_address);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif  // __JPEGENC_H__
