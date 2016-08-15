
#ifndef __VIOPR_FUNC_H
#define __VIOPR_FUNC_H

//#define 	gbTrmBuf _REG

//set CVBS(video format)'s videomode PAL
extern void Vipor_SetVideo_CVBS_PAL(void);

//set CVBS(video format)'s videomode NTSC
extern void Vipor_SetVideo_CVBS_NTSC(void);

//set SVIDEO(video format)'s videomode PAL
extern void Vipor_SetVideo_SVIDEO_PAL(void);

//set SVIDEO(video format)'s videomode NTSC
extern void Vipor_SetVideo_SVIDEO_NTSC(void);

//Set video aspect ratio 4:3
extern void Vipor_SetAspect_4_3(void);

//Set video aspect ratio 16:9 
extern void Vipor_SetAspect_16_9(void);

//Set picture Brightness
//bBriValue: brightness value
extern void Vipor_SetPicture_Brightness(BYTE bBriValue);

//Set picture Contrast
//bConValue: contrast value
extern void Vipor_SetPicture_Contrast(BYTE bConValue);

//Set picture Saturation
//bSatValue: saturation value
extern void Vipor_SetPicture_Saturation(BYTE bSatValue);

//for NTSC(video mode) subsystem
//bVideoSubSystem:	NTSC_M, PAL_443, NTSC_443,
extern void Vipor_NTSC_VideoSubSystem(BYTE bVideoSubSystem);

//for PAL(video mode) subsystem
//bVideoSubSystem:	PAL_BGDHI, PAL_N, SECAM
extern void Vipor_PAL_VideoSubSystem(BYTE bVideoSubSystem);

//judge video if locked
//output: TRUE/FALSE
extern BYTE Vipor_GetVideoLocked(void);

//get video mode
//output: EN_VID_MODE_PAL/EN_VID_MODE_NTSC
extern BYTE Vipor_GetVideoMode(void);
extern void ViporBCAdjust(void);
extern void Vipro_LoadGammaTable(BYTE *dptr);
extern void Vipro_GammaEnable(BYTE fEnable);
#endif 
