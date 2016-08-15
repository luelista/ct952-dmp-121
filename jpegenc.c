//JPEG encoder

// ---------------------------------
//include files
//
#include <stdio.h>
#include "winav.h"
#include "ctkav.h"
#include "jpegdec.h"
#include "../jpegenc.h"

//reference header files of libjpegv6b
#include "jpeglib.h"

// ---------------------------------
//tommylin20061114: translation function from coord (x,y) to byte offset, for CT909S-style display buffer
//because the coord (x,y) is well-aligned to 4, so this may be simplified??
//
#define _JPUUTIL_COORD_TO_BYTEOFFSET_Y(coord_x, coord_y, disp_width)    (((((coord_y) >> 4) * (((disp_width) >> 3) << 1)) + ((coord_x) >> 2)) << 6) + (((coord_y) % 16) << 2)
#define _JPUUTIL_COORD_TO_BYTEOFFSET_UV(coord_x, coord_y, disp_width)   (((((coord_y) >> 5) * (((disp_width) >> 3) << 1)) + (((coord_x) >> 4) << 2) + ((((coord_x) >> 1) % 8) >> 2)) << 6) + ((((coord_y) >> 1) % 16) << 2)

//global variables
//
volatile WORD _wJPEGENC_Abort   = FALSE;
volatile WORD _wJPEGENC_Status  = JPEGENC_COMPRESSION_EMPTY;

volatile DWORD _dwJPEGENC_SrcAddressY   = 0;
volatile DWORD _dwJPEGENC_SrcAddressUV  = 0;
volatile DWORD _dwJPEGENC_FileSizeMax   = JPEGENC_COMPRESSION_FILESIZE_MAX;
volatile WORD  _wJPEGENC_Quality        = JPEGENC_COMPRESSION_QUALITY_DEFAULT;
volatile WORD  _wJPEGENC_QualityStep    = JPEGENC_COMPRESSION_QUALITY_DEFSTEP;
volatile WORD  _wJPEGENC_SrcWidth       = JPEGENC_COMPRESSION_IMAGE_WIDTH;
volatile WORD  _wJPEGENC_SrcHeight      = JPEGENC_COMPRESSION_IMAGE_HEIGHT;
volatile WORD  _wJPEGENC_SrcStrip       = JPEGENC_COMPRESSION_IMAGE_WIDTH;
volatile WORD  _wJPEGENC_SrcCoordH      = 0;
volatile WORD  _wJPEGENC_SrcCoordV      = 0;

#ifdef JPEGENC_EVAL_PERFORMANCE
volatile WORD  _wJPEGENC_MaxRetryCount  = 0;
#endif

//memory pool
//
volatile DWORD _dwMempoolCurrAddress    = 0;
volatile DWORD _dwMempoolStartAddress   = 0;
volatile DWORD _dwMempoolTotalSize      = 0;
volatile DWORD _dwMemPoolFreeSize       = 0;

volatile DWORD _dwJPEGENC_StartOutputPtr    = 0;
volatile DWORD _dwJPEGENC_CurrentOutputPtr  = 0;

volatile DWORD _dwJPEGENC_ExceedSize    = 0;
volatile WORD  _wMempoolInitialized     = FALSE;

// Information put into APP2 marker
volatile WORD  _wJPEGENC_APP2InfoSize       = 0;
volatile DWORD _dwJPEGENC_APP2InfoStartAddr = 0;

// ---------------------------------
// ENCODER MAIN FLOW: inner function, not opened-API
//
WORD _JPEGENC_Main(VOID)
{
    // ******* ALREADY KNOWN CONDITIONS *******
    // 1. Image is in Y/UV format (used since CT909S) & its component sampling format is 420.
    // 2. JPEG maximum size: 65535 x 65535
    // 3. Output format: baseline, sampling=420 YUV, and the same color/grayscale
    // 4. corresponding options of cjpeg:
    //    (1) optimise Huffman table    ON
    //    (2) scale M/N                 (not applied) use the size as original.
    //    (3) smooth N                  OFF (not applied)
    //    (4) maxmemory N               ?? (not applied)
    //    (5) baseline                  ON (use baseline quantisation tables)
    //    (6) qtables, qslots           (not applied)
    //    (7) sample                    420=2x2,1x1,1x1

    // ******* PARAMETERS *******
    // srcbuff      use _wJPEGENC_SrcAddressY and _wJPEGENC_SrcAddressUV instead
    // dstbuff      starting address of the buffer which is prepared for compression result, the generated JPEG (JFIF) file.
    // is_grayscale TRUE for grayscale; FALSE = colour
    // width        horizontal vector of image to be compressed. use _wJPEGENC_SrcWidth instead.
    // height       vertical vector of image to be compressed. use _wJPEGENC_SrcHeight instead.
    // quality      use _wJPEGENC_Quality instead

    // ******* RETURN VALUE *******
    // { JPEGENC_COMPRESSION_SUCCESSED, JPEGENC_COMPRESSION_RUNNING, JPEGENC_COMPRESSION_SIZE_EXCEEDED,
    //   JPEGENC_COMPRESSION_LIBJPEG_ABORTED, JPEGENC_COMPRESSION_USER_ABORTED, JPEGENC_COMPRESSION_EXIT_FAILED,
    //   JPEGENC_COMPRESSION_EMPTY }

    // structure declaration and global variables
    struct jpeg_compress_struct cinfo;      /* jpeg compress information*/
    struct jpeg_error_mgr jerr;             /* error hadler*/

    //JSAMPLE *scanline_buffer;
    JSAMPROW *scanline_buffer;
    DWORD block_count;

    _wJPEGENC_Status = JPEGENC_COMPRESSION_RUNNING;

	//scanline_buffer = (JSAMPROW *)malloc(_wJPEGENC_SrcWidth * 3);
	scanline_buffer = (JSAMPROW *)JPEGENC_Malloc(_wJPEGENC_SrcWidth * 3);
	memset(scanline_buffer, 0, _wJPEGENC_SrcWidth * 3);

	/* Step 1: allocate and initialize JPEG compression object */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	/* Step 2: specify data destination (eg, a file) */
	/* Note: steps 2 and 3 can be done in either order. */
	jpeg_stdio_dest(&cinfo);

	/* Step 3: set parameters for compression */
	cinfo.image_width  = _wJPEGENC_SrcWidth;            /* image width and height, in pixels */
	cinfo.image_height = _wJPEGENC_SrcHeight;
	cinfo.input_components = 3;                         /* # of color components per pixel */
	cinfo.in_color_space = JCS_YCbCr;                   /* colorspace of input image */
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, _wJPEGENC_Quality, TRUE);  //-baseline -quality xx
	cinfo.jpeg_color_space = cinfo.in_color_space;

    //make sure all parameters were set well
    cinfo.dct_method = JDCT_IFAST;      //-dct fast
#ifdef ENTROPY_OPT_SUPPORTED
    cinfo.optimize_coding = TRUE;       //-optimize
#endif
    //set_sample_factors(cinfo, samplearg);             //-sample 2x1,1x1,1x1
    // already set to 2x2,1x1,1x1 as default values
    /*
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 2;
    cinfo.comp_info[1].h_samp_factor = 1;
    cinfo.comp_info[1].v_samp_factor = 1;
    cinfo.comp_info[2].h_samp_factor = 1;
    cinfo.comp_info[2].v_samp_factor = 1;
    */

	/* Step 4: Start compressor */
	jpeg_start_compress(&cinfo, TRUE);  //mark all tables to be written

	/* Step 5: while (scan lines remain to be written) */
    while (cinfo.next_scanline < cinfo.image_height) 
    {
        volatile DWORD dwFetchBuff;
        volatile DWORD dwFetchBuff2;
        volatile BYTE * pfill = (BYTE *)scanline_buffer;

        //tommylin20061113: compose one scanline by YUV bytes
        //Y-PART
        for (block_count = 0; block_count < _wJPEGENC_SrcWidth; block_count+=4)
        {
            dwFetchBuff  = *(DWORD *)((BYTE *)(_dwJPEGENC_SrcAddressY + _JPUUTIL_COORD_TO_BYTEOFFSET_Y((block_count + _wJPEGENC_SrcCoordH), (cinfo.next_scanline + _wJPEGENC_SrcCoordV), _wJPEGENC_SrcStrip)));
            *pfill = (BYTE)((dwFetchBuff >> 24) & 0xFF);
            pfill += 3;
            *pfill = (BYTE)((dwFetchBuff >> 16) & 0xFF);
            pfill += 3;
            *pfill = (BYTE)((dwFetchBuff >>  8) & 0xFF);
            pfill += 3;
            *pfill = (BYTE)((dwFetchBuff      ) & 0xFF);
            pfill += 3;
        }
        //UV-PART
        pfill = (BYTE *)scanline_buffer + 1;   //point to the starting address of first U byte
        for (block_count = 0; block_count < _wJPEGENC_SrcWidth; block_count+=8)
        {
            dwFetchBuff  = *(DWORD *)((BYTE *)(_dwJPEGENC_SrcAddressUV + _JPUUTIL_COORD_TO_BYTEOFFSET_UV((block_count + _wJPEGENC_SrcCoordH), (cinfo.next_scanline + _wJPEGENC_SrcCoordV), _wJPEGENC_SrcStrip)));
            dwFetchBuff2 = *(DWORD *)((BYTE *)(_dwJPEGENC_SrcAddressUV + _JPUUTIL_COORD_TO_BYTEOFFSET_UV((block_count + + _wJPEGENC_SrcCoordH), (cinfo.next_scanline + _wJPEGENC_SrcCoordV), _wJPEGENC_SrcStrip) + 128));
            *pfill = (BYTE)((dwFetchBuff  >> 24) & 0xFF);  //U0
            pfill += 1;
            *pfill = (BYTE)((dwFetchBuff2 >> 24) & 0xFF);  //V0
            pfill += 2;
            *pfill = (BYTE)((dwFetchBuff  >> 24) & 0xFF);  //U0
            pfill += 1;
            *pfill = (BYTE)((dwFetchBuff2 >> 24) & 0xFF);  //V0
            pfill += 2;
            *pfill = (BYTE)((dwFetchBuff  >> 16) & 0xFF);  //U1
            pfill += 1;
            *pfill = (BYTE)((dwFetchBuff2 >> 16) & 0xFF);  //V1
            pfill += 2;
            *pfill = (BYTE)((dwFetchBuff  >> 16) & 0xFF);  //U1
            pfill += 1;
            *pfill = (BYTE)((dwFetchBuff2 >> 16) & 0xFF);  //V1
            pfill += 2;
            *pfill = (BYTE)((dwFetchBuff  >>  8) & 0xFF);  //U2
            pfill += 1;
            *pfill = (BYTE)((dwFetchBuff2 >>  8) & 0xFF);  //V2
            pfill += 2;
            *pfill = (BYTE)((dwFetchBuff  >>  8) & 0xFF);  //U2
            pfill += 1;
            *pfill = (BYTE)((dwFetchBuff2 >>  8) & 0xFF);  //V2
            pfill += 2;
            *pfill = (BYTE)((dwFetchBuff       ) & 0xFF);  //U3
            pfill += 1;
            *pfill = (BYTE)((dwFetchBuff2      ) & 0xFF);  //V3
            pfill += 2;
            *pfill = (BYTE)((dwFetchBuff       ) & 0xFF);  //U3
            pfill += 1;
            *pfill = (BYTE)((dwFetchBuff2      ) & 0xFF);  //V3
            pfill += 2;
        }

		//row_pointer[0] = & scanline_buffer[cinfo.next_scanline * row_stride];

        //FILE GENERATION: output compressed data
        //if this flush action is done, the flushed length will make JPEG file buffer exceed
        //return JPEGENC_COMPRESSION_SIZE_EXCEEDED
		//(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
		(VOID) jpeg_write_scanlines(&cinfo, (JSAMPARRAY)&scanline_buffer, 1);
	}

	/* Step 6: Finish compression */
	jpeg_finish_compress(&cinfo);

	/* Step 7: release JPEG compression object */
	jpeg_destroy_compress(&cinfo);
	//free(scanline_buffer);
	JPEGENC_Free(scanline_buffer);

    if (JPEGENC_COMPRESSION_RUNNING == _wJPEGENC_Status)
    {
        _wJPEGENC_Status = JPEGENC_COMPRESSION_SUCCESSED;
        return (JPEGENC_COMPRESSION_SUCCESSED);
    }
    else
    {
        return _wJPEGENC_Status;
    }
}


//API export for upper-layer to adjust factors of input region for compression
//
VOID JPEGENC_SetCompressRegionSize(WORD width_value, WORD height_value)
{
    _wJPEGENC_SrcWidth  = width_value;
    _wJPEGENC_SrcHeight = height_value;
}

//API export for upper-layer to specify input region for compression
//
VOID JPEGENC_SetCompressRegionAddress(DWORD starting_address_y, DWORD starting_address_uv)
{
    _dwJPEGENC_SrcAddressY  = starting_address_y;
    _dwJPEGENC_SrcAddressUV = starting_address_uv;
}

//API export for upper-layer to adjust the maximum value of the storage slot for generated JPEG file
//
VOID JPEGENC_SetCompressFileSizeMax(DWORD max_filesize_in_byte)
{
    _dwJPEGENC_FileSizeMax = max_filesize_in_byte;
}

//API export for upper-layer to adjust the starting value for compression quality
//
VOID JPEGENC_SetCompressQuality(WORD quality_value)
{
    _wJPEGENC_Quality = quality_value;
}

//API export for upper-layer to adjust the starting value for compression quality
//
VOID JPEGENC_SetCompressQualityStep(WORD quality_step_value)
{
    _wJPEGENC_QualityStep = quality_step_value;
}

//API export for upper-layer to recordinformation of the original image, then compose them into an APP2 marker
//
VOID JPEGENC_SetApp2InfoAddressNSize(DWORD info_start_address, WORD info_total_size)
{
    _dwJPEGENC_APP2InfoStartAddr = info_start_address;
    _wJPEGENC_APP2InfoSize       = info_total_size;
}

//API export for upper-layer to reset initialisation values of the JPEG compression module
//
VOID JPEGENC_Abort(DWORD cinfo)
{
    jpeg_abort((j_common_ptr) cinfo); /* use common routine */
    _wJPEGENC_Status = JPEGENC_COMPRESSION_USER_ABORTED;
}

//API export for upper-layer to reset initialisation values of the JPEG compression module
//
VOID JPEGENC_Reset(VOID)
{
    _wJPEGENC_Abort     = FALSE;
    _wJPEGENC_Status    = JPEGENC_COMPRESSION_EMPTY;

    _dwJPEGENC_SrcAddressY  = 0;
    _dwJPEGENC_SrcAddressUV = 0;
    _dwJPEGENC_FileSizeMax  = JPEGENC_COMPRESSION_FILESIZE_MAX;
    _wJPEGENC_Quality       = JPEGENC_COMPRESSION_QUALITY_DEFAULT;
    _wJPEGENC_QualityStep   = JPEGENC_COMPRESSION_QUALITY_DEFSTEP;
    _wJPEGENC_SrcWidth      = JPEGENC_COMPRESSION_IMAGE_WIDTH;
    _wJPEGENC_SrcHeight     = JPEGENC_COMPRESSION_IMAGE_HEIGHT;

    _dwJPEGENC_ExceedSize  = 0;

    _dwJPEGENC_StartOutputPtr   = 0;
    _dwJPEGENC_CurrentOutputPtr = 0;

    _wMempoolInitialized = FALSE;
}

//API export for upper-layer to get current status of JPEG encoder
//
WORD JPEGENC_GetStatus(VOID)
{
    return (_wJPEGENC_Status);
}

//API export for upper-layer to get generated JPEG file size by JPEG encoder
//
DWORD JPEGENC_GetCompressedSize(DWORD mempool_start_address)
{
    return (_dwJPEGENC_CurrentOutputPtr - mempool_start_address);
}

//utility functions for JPEG encoder
//
WORD JPEGENC_InitMemPool(DWORD mempool_addr, DWORD mempool_size)
{
    //initialize variables related to the memory pool
    //mempool= jpeg_file + encoder_obj_members
    if (TRUE != _wMempoolInitialized)
    {
        // for JPEG file generation
        _dwJPEGENC_StartOutputPtr   = mempool_addr;
        _dwJPEGENC_CurrentOutputPtr = mempool_addr;

        // for JPEG encoder
        _dwMempoolTotalSize     = mempool_size - _dwJPEGENC_FileSizeMax;
        //check: if the mempool is large enough for generated JPEG file and encoder object members
        if (JPEGENC_REQUIRED_MEMPOOL_SIZE > _dwMempoolTotalSize)
        {
            return JPEGENC_COMPRESSION_MEM_NOT_ENOUGH;
        }
        _dwMemPoolFreeSize      = _dwMempoolTotalSize;
        _dwMempoolStartAddress  = mempool_addr + mempool_size;
        _dwMempoolCurrAddress   = mempool_addr + mempool_size;
        _wMempoolInitialized    = TRUE;

        //should we clear buffer at the beginning?
        return JPEGENC_COMPRESSION_SUCCESSED;
    }
    else
    {
        return JPEGENC_COMPRESSION_MEM_ALREADY_INIT;
    }
}

// simple malloc replacement
//
VOID *JPEGENC_Malloc(DWORD byte_count)
{
    if (0 == byte_count)
    {
        return NULL;
    }
    //CHECK: if request size > available
    if (byte_count > _dwMemPoolFreeSize)
    {
        return NULL;
    }

    if (TRUE == _wMempoolInitialized)
    {
        //NORMAL: move current ptr to next available slot (and it should be 4x-byte aligned)
        //tommy1122: ok for byte-alignment
        //byte_count = (byte_count + 2) & 0xfffffffc;
        _dwMemPoolFreeSize      -= byte_count;
        _dwMempoolCurrAddress   -= byte_count;

        return (VOID *)_dwMempoolCurrAddress;
    }
    else
    {
        return NULL;
    }
}

// simple free replacement
//
VOID JPEGENC_Free(VOID *dest)
{
    //empty: do nothing
}

// ---------------------------------
// sample of JPEG encoder processing flow:
//
WORD JPEGENC_StartCompression(DWORD mempool, DWORD len, DWORD start_y, DWORD start_uv, WORD width, WORD height, WORD PicStrip, WORD PicCoordH, WORD PicCoordV, WORD quality, WORD quality_step, DWORD max_filesize)
{
    //mempool: starting address of memory pool, for both JPEG file (from the beginning) and function JPEGENC_Malloc() (from the tail)
    //len: size of memory pool, in bytes
    //start_y: Y starting address of source image
    //start_uv: UV starting address of source image
    //width: width of source image
    //height: height of source image
    //quality: starting quality of compression. If the generated JPEG file size is exceeded, JPEG encoder will adjust this value smaller automatically.
    //quality_step: step value for quality adjustment
    //max_filesize: possible maximum file size for generated JPEG file (count in bytes)

    volatile DWORD exceed_percentage;
    volatile WORD  compress_status = JPEGENC_COMPRESSION_EMPTY;
    volatile WORD  mempool_status  = JPEGENC_COMPRESSION_EMPTY;
    //volatile WORD first_predict_quality = TRUE;

#ifdef JPEGENC_EVAL_PERFORMANCE
    volatile DWORD jpegenc_time = OS_GetSysTimer();
    volatile WORD  retry_count = 0;
#endif

    _wJPEGENC_SrcCoordH = PicCoordH;
    _wJPEGENC_SrcCoordV = PicCoordV;
    _wJPEGENC_SrcStrip = PicStrip;

    // initial for libmem module
    //DVB_MemoryCFG(mempool, len);
    // reset all configurable settings to default values
    JPEGENC_Reset();

    //_dwJPEGENC_StartOutputPtr   = buff;
    //_dwJPEGENC_CurrentOutputPtr = buff;       //PTR to result buffer: reference jdatadst.c for output

    // use these functions to setting image, buffer, and compression parameters
    JPEGENC_SetCompressRegionAddress(start_y, start_uv);
    JPEGENC_SetCompressRegionSize(width, height);
    JPEGENC_SetCompressFileSizeMax(max_filesize);
    JPEGENC_SetCompressQuality(quality);
    JPEGENC_SetCompressQualityStep(quality_step);

    // it should be called right before JPEGENC_Main() at the first time, to confirm there is enough space in memory pool
    if (JPEGENC_COMPRESSION_EMPTY == mempool_status)
    {
        mempool_status = JPEGENC_InitMemPool(mempool, len);
        if (JPEGENC_COMPRESSION_MEM_NOT_ENOUGH == mempool_status)
        {
            DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "ERR: memory pool size isn't enough\n");
            return JPEGENC_COMPRESSION_MEM_NOT_ENOUGH;
        }
    }

    // start JPEG encoder
    compress_status = _JPEGENC_Main();

#ifdef JPEGENC_EVAL_PERFORMANCE
    retry_count += 1;
#endif

    //if it's out of MAX buffer size, decrease the quality and invoke compression again:
    //heuristic adjustment by exceed size --> may improve time for trail to 3 (but not 5 or 6)
    while (JPEGENC_COMPRESSION_SIZE_EXCEEDED == compress_status)
    {
#ifdef JPEGENC_EVAL_PERFORMANCE
        DWORD exceed_size = _dwJPEGENC_ExceedSize;
#endif

        exceed_percentage = _dwJPEGENC_ExceedSize * 100 / _dwJPEGENC_FileSizeMax;   //(64 * 1024)
        JPEGENC_Reset();

        JPEGENC_SetCompressRegionAddress(start_y, start_uv);
        JPEGENC_SetCompressRegionSize(width, height);
        JPEGENC_SetCompressFileSizeMax(max_filesize);
        JPEGENC_SetCompressQualityStep(quality_step);

        //_dwJPEGENC_CurrentOutputPtr = buff;            //move current PTR to the starting of buffer

        //if (TRUE == first_predict_quality)
        //{
            // Heuristic partitions based-on:
            // (1) 64KB maximum file size constraints (and it's the most important factor of these heuristic partitions below), and
            // (2) resolutions of display buffer (640x448 pixels)
            // (3) starts from quality 90%, step is 5%.
            // This get a balance of compression performance and quality.
            //
            // TESTED DISC:
            // (1) Tommy's TestJPEG Disc#1
            // (2) Tommy's TestJPEG Disc#2
            // TUNED RESULTS:
            // (1) file size almost better than 0xB800 bytes (for max file size 64KB configuration, it reaches 71.8% usage.)
            // (2) only one of test pictures gets 0xB7A9 bytes.
            // (3) all test pictures require no more than 2 times of compression: it consumes about (or less than) 1 sec. in CT909S CheerDVD v2.38b.
            // (4) although current compression quality is good, some pictures may raise the quality value for 5 or 10 to get optimal compression of themselves.
            //     we may provide better algorithm or parameters of partitions to get the compression quality optimal in the future.
            //
            if (170 <= exceed_percentage)
            {
                quality -= 60;
            }
            else if (142 <= exceed_percentage && exceed_percentage < 170)
            {
                quality -= 55;
            }
            else if (117 <= exceed_percentage && exceed_percentage < 142)
            {
                quality -= 50;
            }
            else if (95 <= exceed_percentage && exceed_percentage < 117)
            {
                quality -= 45;
            }
            else if (90 <= exceed_percentage && exceed_percentage < 95)
            {
                quality -= 40;
            }
            else if (75 <= exceed_percentage && exceed_percentage < 90)
            {
                quality -= 35;
            }
            else if (66 <= exceed_percentage && exceed_percentage < 75)
            {
                quality -= 30;
            }
            else if (54 <= exceed_percentage && exceed_percentage < 66)
            {
                quality -= 25;
            }
            else if (48 <= exceed_percentage && exceed_percentage < 54)
            {
                quality -= 20;
            }
            else if (34 <= exceed_percentage && exceed_percentage < 48)
            {
                quality -= 15;
            }
            else if (15 <= exceed_percentage && exceed_percentage < 34)
            {
                quality -= 10;
            }
            else if (exceed_percentage < 15)
            {
                quality -= 5;
            }
            else
            {
                quality -= _wJPEGENC_QualityStep;
            }
            //first_predict_quality = FALSE;
        //}
        //else
        //{
        //    quality -= _wJPEGENC_QualityStep;       //decrease QUALITY for a step value
        //}

        //check quality value = 1~99
        if (1 > quality || 99 < quality)
        {
            DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "ERR: QUALITY=(0x%x)", quality);
            return JPEGENC_COMPRESSION_EXIT_FAILED;
        }

        JPEGENC_SetCompressQuality(quality);

        JPEGENC_InitMemPool(mempool, len);
        compress_status = _JPEGENC_Main();

#ifdef JPEGENC_EVAL_PERFORMANCE
        DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "EXCEED SIZE=0x%lx(0x%X%%); retry QUALITY=(0x%X)\n", exceed_size, exceed_percentage, quality);
        retry_count += 1;
#endif
    }

#ifdef JPEGENC_EVAL_PERFORMANCE
    //remember max retry count to evaluate possible longest compression time
    if (retry_count > _wJPEGENC_MaxRetryCount)
    {
        _wJPEGENC_MaxRetryCount = retry_count;
    }

    // JPEG total size= _pdwCurrentOutputPtr - buff
    if (1 < retry_count)
    {
        DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "TIME:0x%lx, SIZE:0x%lx", OS_GetSysTimer() - jpegenc_time, _dwJPEGENC_CurrentOutputPtr - mempool);
    }
    //required: 0xB0b4 bytes
    DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "MAX MEMPOOL USED:0x%lx", (DWORD)mempool + len - _dwMempoolCurrAddress);
#endif

    //compress successfully
    return JPEGENC_COMPRESSION_SUCCESSED;
}
