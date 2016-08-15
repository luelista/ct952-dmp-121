#ifndef __VIOPR_API_H
#define __VIOPR_API_H


// Define accept video format.
typedef enum
{
	EN_LCD_VID_FORMAT_CVBS = 0,	    // the LCD video format is CVBS
	EN_LCD_VID_FORMAT_SVIDEO,	// the LCD video format is SVIDEO
	EN_LCD_VID_FORMAT_CCIR656,	// the LCD video format is CCIR656
	EN_LCD_VID_FORMAT_NUMBER
}EN_LCD_VID_FORMAT;

// Define the base band video output mode.
typedef enum
{
    EN_VID_MODE_PAL = 0,	        // Force setup the mode to PAL. (Same as DVB)
	EN_VID_MODE_NTSC,	        // Force setup the mode to NTSC.
	EN_VID_MODE_TFT_DIGITAL,
	EN_VID_MODE_NUMBER
}EN_VID_MODE;

// Define the video display type.
typedef enum
{
	EN_VID_DISPLAY_4_3 = 0,	    // the video display type 4:3
    EN_VID_DISPLAY_16_9,	// the video display type 16:9
	EN_VID_DISPLAY_UNKNOWS  // the video display type unknows
}EN_VID_DISPLAY;

// The LCD panel flip type.
typedef enum
{
    EN_PANEL_FLIP_NORMAL = 0,	// the LCD panel is no flip
	EN_PANEL_FLIP_H,		// the LCD panel is horizontal flip
	EN_PANEL_FLIP_V,		// the LCD panel is vertical flip
	EN_PANEL_FLIP_H_V,	    // the LCD panel is horizontal and vertical flip
	EN_PANEL_FLIP_NUMBER
}EN_PANEL_FLIP;


enum COLOR_SYSTEM_60
{
    COLOR_NTSC_M = 0,
    COLOR_PAL_443,
    COLOR_NTSC_443,

    COLOR_60_MAX
};

enum COLOR_SYSTEM_50
{
    COLOR_PAL_BGDHI = 0,
    COLOR_PAL_N,
    COLOR_SECAM,

    COLOR_50_MAX
};

#endif

