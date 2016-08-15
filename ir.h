
#ifndef __IR_H__
#define __IR_H__

#include    "input.h"

/*********************************************/
/* Should select a correct remote controllor */
/*********************************************/
#define LARGE_VCD30 	    1
#define LARGE_VCD30_EX 	    2
#define KF_8000A            3  
#define SMALL_VCD30         4
#define RAITE_713K          5 
#define SINIC_2100          6
#define MUSTEK_V300         7
#define XINGQIU_S2100       8
#define XINGQIU_S2100_IR2   9
#define WB_PANASONIC        10
#define WB_WINBOND          11 
#define LARGE_VCD20         12
#define MUSTEK_600          13  
#define MUSTEK_BLACK_IR     14
#define MUSTEK_SILVER_IR    15
#define MUSTEK_KARAOKE      16
#define FOXCONN_MDP3000     17
#define FOXCONN_MDP3000_S   18
#define MUSTEK_P_COMBO      19
#define MUSTEK_P_28KEY      20  
#define KONKA_IR            21
#define YHI_IR              22
#define PHILIPS_IR          23
#define WS_258_WHITE	    24
#define CHEERTEK_IR         25

#define KXDDIGITAL_IR    30
#define CONNTEL_IR         31
#define CONNTEL_IR2         32
/*-------------------------------------------*/
#define IR_TYPE CONNTEL_IR2
/*-------------------------------------------*/


/*********************************************/
/* IR_CODE_TYPE                              */
/*********************************************/
#define HW_NEC      0   // Hardware Detect NEC Coding
#define SW_NEC      1   // Software Detect NEC Coding
#define SW_KONKA    2   // Software Detect KONKA Coding
#define SW_RC5      3   // Software Detect RC5 Coding

/*-------------------------------------------*/
#if (IR_TYPE == LARGE_VCD30)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFF
const BYTE aIRMap[] =
{
KEY_REMAIN       ,KEY_DIGEST       ,KEY_VOL_DOWN     ,KEY_VOL_UP       ,//00-03
KEY_NO_KEY       ,KEY_TVMODE       ,KEY_UP           ,KEY_ZOOM         ,//04-07
KEY_GOTOTIME     ,KEY_N1           ,KEY_SCB          ,KEY_NO_KEY       ,//08-0B
KEY_NO_KEY       ,KEY_N4           ,KEY_PREV         ,KEY_OSDGAME      ,//0C-0F
KEY_OPEN_CLOSE   ,KEY_N7           ,KEY_N0           ,KEY_PAUSE        ,//10-13
KEY_POWER        ,KEY_N8           ,KEY_NO_KEY       ,KEY_N9           ,//14-17
KEY_SF           ,KEY_N5           ,KEY_NEXT         ,KEY_N6           ,//18-1B
KEY_NO_KEY       ,KEY_N2           ,KEY_SCF          ,KEY_N3           ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_RIGHT        ,KEY_KEYNORMAL    ,KEY_ANGLE_CTL    ,KEY_AST_CTL      ,//40-43
KEY_DOWN         ,KEY_RANDOM       ,KEY_KEYUP        ,KEY_LEFT         ,//44-47
KEY_PBC          ,KEY_PROGRAM      ,KEY_MUTE         ,KEY_STEP         ,//48-4B
KEY_STOP         ,KEY_SETUP        ,KEY_MENU         ,KEY_NO_KEY       ,//4C-4F
KEY_DISPLAY      ,KEY_PLAY         ,KEY_RESUME       ,KEY_RETURN       ,//50-53
KEY_NO_KEY       ,KEY_REPEAT       ,KEY_FREEZE       ,KEY_KEYDOWN      ,//54-57
KEY_NO_KEY       ,KEY_N10PLUS      ,KEY_REPEAT_AB    ,KEY_NO_KEY       ,//58-5B
KEY_RESUME       ,KEY_NO_KEY       ,KEY_AUDIO_LR     ,KEY_SPST_CTL     ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == LARGE_VCD30_EX)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFF
const BYTE aIRMap[] =
{
KEY_AMP          ,KEY_DIGEST       ,KEY_VOL_DOWN     ,KEY_VOL_UP       ,//00-03
KEY_NO_KEY       ,KEY_TVMODE       ,KEY_UP           ,KEY_ZOOM         ,//04-07
KEY_GOTOTIME     ,KEY_N1           ,KEY_SCB          ,KEY_NO_KEY       ,//08-0B
KEY_NO_KEY       ,KEY_N4           ,KEY_PREV         ,KEY_FUNCTION      ,//0C-0F
KEY_OPEN_CLOSE   ,KEY_N7           ,KEY_N0           ,KEY_PAUSE        ,//10-13
KEY_POWER        ,KEY_N8           ,KEY_NO_KEY       ,KEY_N9           ,//14-17
KEY_SF           ,KEY_N5           ,KEY_NEXT         ,KEY_N6           ,//18-1B
KEY_NO_KEY       ,KEY_N2           ,KEY_SCF          ,KEY_N3           ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_RIGHT        ,KEY_KEYNORMAL    ,KEY_ANGLE_CTL    ,KEY_AST_CTL      ,//40-43
KEY_DOWN         ,KEY_RANDOM       ,KEY_KEYUP        ,KEY_LEFT         ,//44-47
KEY_PBC          ,KEY_PROGRAM      ,KEY_MUTE         ,KEY_STEP         ,//48-4B
KEY_STOP         ,KEY_SETUP        ,KEY_MENU         ,KEY_NO_KEY       ,//4C-4F
KEY_DISPLAY      ,KEY_PLAY         ,KEY_RESUME       ,KEY_RETURN       ,//50-53
KEY_NO_KEY       ,KEY_REPEAT       ,KEY_EQUALIZER    ,KEY_KEYDOWN      ,//54-57
KEY_NO_KEY       ,KEY_N10PLUS      ,KEY_REPEAT_AB    ,KEY_NO_KEY       ,//58-5B
KEY_RESUME       ,KEY_NO_KEY       ,KEY_AUDIO_LR     ,KEY_SPST_CTL     ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == SMALL_VCD30)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFF
const BYTE aIRMap[] =
{
KEY_REMAIN       ,KEY_DIGEST       ,KEY_VOL_DOWN     ,KEY_VOL_UP       ,//00-03
KEY_NO_KEY       ,KEY_TVMODE       ,KEY_UP           ,KEY_ZOOM         ,//04-07
KEY_GOTOTIME     ,KEY_N1           ,KEY_SCB          ,KEY_NO_KEY       ,//08-0B
KEY_NO_KEY       ,KEY_N4           ,KEY_PREV         ,KEY_OSDGAME      ,//0C-0F
KEY_OPEN_CLOSE   ,KEY_N7           ,KEY_N0           ,KEY_PAUSE        ,//10-13
KEY_POWER        ,KEY_N8           ,KEY_NO_KEY       ,KEY_N9           ,//14-17
KEY_SF           ,KEY_N5           ,KEY_NEXT         ,KEY_N6           ,//18-1B
KEY_NO_KEY       ,KEY_N2           ,KEY_SCF          ,KEY_N3           ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_RIGHT        ,KEY_KEYNORMAL    ,KEY_ANGLE_CTL    ,KEY_AST_CTL      ,//40-43
KEY_DOWN         ,KEY_MENU         ,KEY_KEYUP        ,KEY_LEFT         ,//44-47
KEY_PBC          ,KEY_PROGRAM      ,KEY_MUTE         ,KEY_STEP         ,//48-4B
KEY_STOP         ,KEY_SETUP        ,KEY_MENU         ,KEY_NO_KEY       ,//4C-4F
KEY_DISPLAY      ,KEY_PLAY         ,KEY_REPEAT_AB    ,KEY_RETURN       ,//50-53
KEY_NO_KEY       ,KEY_REPEAT       ,KEY_STOP         ,KEY_KEYDOWN      ,//54-57
KEY_NO_KEY       ,KEY_N10PLUS      ,KEY_REPEAT_AB    ,KEY_NO_KEY       ,//58-5B
KEY_RESUME       ,KEY_NO_KEY       ,KEY_SETUP        ,KEY_SPST_CTL     ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == RAITE_713K)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x20
#define     CUSTOMER_CODE1      0xDF 
const BYTE aIRMap[] =
{
KEY_VOCAL        ,KEY_RETURN       ,KEY_SETUP        ,KEY_MUTE         ,//00-03
KEY_KEYUP        ,KEY_OSDGAME      ,KEY_MENU         ,KEY_KEYDOWN      ,//04-07
KEY_N8           ,KEY_N9           ,KEY_RESUME       ,KEY_N7           ,//08-0B
KEY_N5           ,KEY_N6           ,KEY_POWER        ,KEY_N4           ,//0C-0F
KEY_N2           ,KEY_N3           ,KEY_SPST_CTL     ,KEY_N1           ,//10-13
KEY_REPEAT       ,KEY_VOL_DOWN     ,KEY_AST_CTL      ,KEY_REPEAT_AB    ,//14-17
KEY_SCF          ,KEY_PREV         ,KEY_OPEN_CLOSE   ,KEY_SCB          ,//18-1B
KEY_LEFT         ,KEY_DOWN         ,KEY_ZOOM         ,KEY_SF           ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_DIGEST       ,KEY_TVMODE       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//40-43
KEY_UP           ,KEY_PROGRAM      ,KEY_NO_KEY       ,KEY_NO_KEY       ,//44-47
KEY_RIGHT        ,KEY_N0           ,KEY_NO_KEY       ,KEY_NO_KEY       ,//48-4B
KEY_PBC          ,KEY_N10PLUS      ,KEY_NO_KEY       ,KEY_NO_KEY       ,//4C-4F
KEY_GOTOTIME     ,KEY_STOP         ,KEY_NO_KEY       ,KEY_NO_KEY       ,//50-53
KEY_ANGLE_CTL    ,KEY_VOL_UP       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//54-57
KEY_DISPLAY      ,KEY_NEXT         ,KEY_NO_KEY       ,KEY_NO_KEY       ,//58-5B
KEY_PLAY         ,KEY_PAUSE        ,KEY_NO_KEY       ,KEY_NO_KEY       ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == SINIC_2100)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x08
#define     CUSTOMER_CODE1      0xF7    
const BYTE aIRMap[] =
{
KEY_VOL_UP       ,KEY_VOL_DOWN     ,KEY_NO_KEY       ,KEY_NO_KEY       ,//00-03
KEY_SURROUND     ,KEY_AUDIO_LR     ,KEY_NO_KEY       ,KEY_NO_KEY       ,//04-07
KEY_RETURN       ,KEY_SF           ,KEY_N4           ,KEY_SPST_CTL     ,//08-0B
KEY_KEYUP        ,KEY_KEYDOWN      ,KEY_NO_KEY       ,KEY_NO_KEY       ,//0C-0F
KEY_ZOOM         ,KEY_REPEAT_AB    ,KEY_N1           ,KEY_TVMODE       ,//10-13
KEY_DOWN         ,KEY_MENU         ,KEY_N7           ,KEY_ANGLE_CTL    ,//14-17
KEY_PLAY         ,KEY_LEFT         ,KEY_DISPLAY      ,KEY_POWER        ,//18-1B
KEY_UP           ,KEY_DIGEST       ,KEY_N0           ,KEY_AST_CTL      ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_MUTE         ,KEY_PAUSE        ,//40-43
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_STOP         ,//44-47
KEY_N5           ,KEY_N6           ,KEY_PREV         ,KEY_NEXT         ,//48-4B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_SCB          ,KEY_SCF          ,//4C-4F
KEY_N2           ,KEY_N3           ,KEY_NO_KEY       ,KEY_REPEAT       ,//50-53
KEY_N8           ,KEY_N9           ,KEY_NO_KEY       ,KEY_SETUP        ,//54-57
KEY_PBC          ,KEY_OPEN_CLOSE   ,KEY_NO_KEY       ,KEY_RIGHT        ,//58-5B
KEY_N10PLUS      ,KEY_GOTOTIME     ,KEY_NO_KEY       ,KEY_PROGRAM      ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == MUSTEK_V300)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x10
#define     CUSTOMER_CODE1      0xED 
const BYTE aIRMap[] =
{
KEY_ECHO_UP      ,KEY_KEYDOWN      ,KEY_SCB          ,KEY_ECHO_DOWN    ,//00-03
KEY_VOCAL        ,KEY_SURROUND     ,KEY_PREV         ,KEY_TVMODE       ,//04-07
KEY_N8           ,KEY_N9           ,KEY_SCF          ,KEY_N7           ,//08-0B
KEY_N5           ,KEY_N6           ,KEY_PBC          ,KEY_N4           ,//0C-0F
KEY_N2           ,KEY_N3           ,KEY_SF           ,KEY_N1           ,//10-13
KEY_REPEAT       ,KEY_PROGRAM      ,KEY_OPEN_CLOSE   ,KEY_REPEAT_AB    ,//14-17
KEY_AST_CTL      ,KEY_ANGLE_CTL    ,KEY_POWER        ,KEY_SPST_CTL     ,//18-1B
KEY_LEFT         ,KEY_DOWN         ,KEY_NEXT         ,KEY_PAUSE        ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_GOTOTIME     ,KEY_KEYUP        ,KEY_NO_KEY       ,KEY_NO_KEY       ,//40-43
KEY_UP           ,KEY_MUTE         ,KEY_NO_KEY       ,KEY_NO_KEY       ,//44-47
KEY_RIGHT        ,KEY_N0           ,KEY_NO_KEY       ,KEY_NO_KEY       ,//48-4B
KEY_MENU         ,KEY_VOL_DOWN     ,KEY_NO_KEY       ,KEY_NO_KEY       ,//4C-4F
KEY_ZOOM         ,KEY_VOL_UP       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//50-53
KEY_SETUP        ,KEY_CLEAR        ,KEY_NO_KEY       ,KEY_NO_KEY       ,//54-57
KEY_DIGEST       ,KEY_DISPLAY      ,KEY_NO_KEY       ,KEY_NO_KEY       ,//58-5B
KEY_PLAY         ,KEY_STOP         ,KEY_NO_KEY       ,KEY_NO_KEY       ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == XINGQIU_S2100)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0x99
const BYTE aIRMap[] =
{
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_DIGEST       ,//00-03
KEY_BOOKMARK     ,KEY_GOTOTIME     ,KEY_TVMODE       ,KEY_SETUP        ,//04-07
KEY_N9           ,KEY_N6           ,KEY_N3           ,KEY_NO_KEY       ,//08-0B
KEY_N8           ,KEY_N5           ,KEY_N2           ,KEY_CLEAR        ,//0C-0F
KEY_N7           ,KEY_N4           ,KEY_N1           ,KEY_MENU         ,//10-13
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_N0           ,//14-17
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//18-1B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_REPEAT_AB    ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_ANGLE_CTL    ,KEY_SPST_CTL     ,KEY_AST_CTL      ,KEY_NO_KEY       ,//40-43
KEY_SCF          ,KEY_PLAY         ,KEY_RIGHT        ,KEY_OPEN_CLOSE   ,//44-47
KEY_SCB          ,KEY_DOWN         ,KEY_PLAY         ,KEY_UP           ,//48-4B
KEY_PAUSE        ,KEY_SF           ,KEY_DISPLAY      ,KEY_NEXT         ,//4C-4F
KEY_PREV         ,KEY_STOP         ,KEY_LEFT         ,KEY_POWER        ,//50-53
KEY_RETURN       ,KEY_PBC          ,KEY_VOL_DOWN     ,KEY_NO_KEY       ,//54-57
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_VOL_UP       ,KEY_NO_KEY       ,//58-5B
KEY_OSDGAME      ,KEY_ZOOM         ,KEY_NO_KEY       ,KEY_NO_KEY       ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == XINGQIU_S2100_IR2)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0x99
const BYTE aIRMap[] =
{
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_TITLE,       ,//00-03
KEY_SF           ,KEY_GOTOTIME     ,KEY_TVMODE       ,KEY_SETUP        ,//04-07
KEY_N9           ,KEY_N6           ,KEY_N3           ,KEY_NO_KEY       ,//08-0B
KEY_N8           ,KEY_N5           ,KEY_N2           ,KEY_N10PLUS      ,//0C-0F
KEY_N7           ,KEY_N4           ,KEY_N1           ,KEY_MENU         ,//10-13
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_N0           ,//14-17
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//18-1B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_CLEAR        ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_ANGLE_CTL    ,KEY_SPST_CTL     ,KEY_AUDIO_LR     ,KEY_NO_KEY       ,//40-43
KEY_SCF          ,KEY_PAUSE_STEP   ,KEY_RIGHT        ,KEY_OPEN_CLOSE   ,//44-47
KEY_SCB          ,KEY_DOWN         ,KEY_PLAY         ,KEY_UP           ,//48-4B
KEY_REPEAT_AB    ,KEY_RANDOM       ,KEY_NO_KEY       ,KEY_NEXT         ,//4C-4F
KEY_PREV         ,KEY_STOP         ,KEY_LEFT         ,KEY_POWER        ,//50-53
KEY_RETURN       ,KEY_PROGRAM      ,KEY_VOL_DOWN     ,KEY_DISPLAY      ,//54-57
KEY_DISPLAY      ,KEY_DISPLAY      ,KEY_VOL_UP       ,KEY_DISPLAY      ,//58-5B
KEY_REPEAT       ,KEY_ZOOM         ,KEY_DISPLAY       ,KEY_DISPLAY     ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == WB_PANASONIC)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x10
#define     CUSTOMER_CODE1      0xED 
const BYTE aIRMap[] =
{
KEY_RETURN       ,KEY_TVMODE       ,KEY_SETUP        ,KEY_MUTE         ,//00-03
KEY_KEYUP    ,KEY_VIDEO_OUTPUT_CTL ,KEY_MENU         ,KEY_KEYDOWN      ,//04-07
KEY_N8           ,KEY_N9           ,KEY_RESUME       ,KEY_N7           ,//08-0B
KEY_N5           ,KEY_N6           ,KEY_POWER        ,KEY_N4           ,//0C-0F
KEY_N2           ,KEY_N3           ,KEY_SPST_CTL     ,KEY_N1           ,//10-13
KEY_REPEAT       ,KEY_VOL_DOWN     ,KEY_AST_CTL      ,KEY_REPEAT_AB    ,//14-17
KEY_SCF          ,KEY_PREV         ,KEY_OPEN_CLOSE   ,KEY_SCB          ,//18-1B
KEY_LEFT         ,KEY_DOWN         ,KEY_ZOOM         ,KEY_SF           ,// 1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,// 2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,// 3C-3F
KEY_DIGEST       ,KEY_PROGRAM      ,KEY_NO_KEY       ,KEY_NO_KEY       ,//40-43
KEY_UP           ,KEY_FUNCTION     ,KEY_NO_KEY       ,KEY_NO_KEY       ,//44-47
KEY_RIGHT        ,KEY_N0           ,KEY_NO_KEY       ,KEY_NO_KEY       ,//48-4B
KEY_PBC          ,KEY_N10PLUS      ,KEY_NO_KEY       ,KEY_NO_KEY       ,// 4C-4F
KEY_GOTOTIME     ,KEY_STOP         ,KEY_NO_KEY       ,KEY_NO_KEY       ,//50-53
KEY_ANGLE_CTL    ,KEY_VOL_UP       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//54-57
KEY_DISPLAY      ,KEY_NEXT         ,KEY_NO_KEY       ,KEY_NO_KEY       ,//58-5B
KEY_PLAY         ,KEY_PAUSE        ,KEY_NO_KEY       ,KEY_NO_KEY       ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == WB_WINBOND)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x10
#define     CUSTOMER_CODE1      0xED 
const BYTE aIRMap[] =
{
KEY_VOL_UP     ,KEY_MUTE       ,KEY_REPEAT     ,KEY_REPEAT_AB  , // 00~03
KEY_VOL_DOWN   ,KEY_SCF        ,KEY_AST_CTL    ,KEY_PREV       , // 04~07
KEY_RIGHT      ,KEY_STOP       ,KEY_PAUSE_STEP ,KEY_LEFT       , // 08~0B
KEY_RETURN     ,KEY_RETURN     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 0C~0F
KEY_SURROUND   ,KEY_ZOOM       ,KEY_KEYDOWN    ,KEY_ZOOM       , // 10~13
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 14~17
KEY_N0         ,KEY_CLEAR      ,KEY_N5         ,KEY_N7         , // 18~1B
KEY_OPEN_CLOSE ,KEY_PROGRAM    ,KEY_N2         ,KEY_POWER      , // 1C~1F
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 20~23
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 24~27
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 28~2B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 2C~2F
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 30~33
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 34~37
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 38~3B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 3C~3F
KEY_PBC        ,KEY_MENU       ,KEY_TITLE      ,KEY_GOTOTIME   , // 40~43
KEY_SPST_CTL   ,KEY_NEXT       ,KEY_ANGLE_CTL  ,KEY_SCB        , // 44~47
KEY_SF         ,KEY_PLAY       ,KEY_UP         ,KEY_DOWN       , // 48~4B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 4C~4F
KEY_DISPLAY    ,KEY_KEYUP      ,KEY_NO_KEY     ,KEY_NO_KEY     , // 50~53
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 54~57
KEY_N4         ,KEY_N8         ,KEY_N9         ,KEY_N6         , // 58~5B
KEY_N1         ,KEY_OSDGAME    ,KEY_N3         ,KEY_SETUP      , // 5C~5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == KF_8000A)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x40
#define     CUSTOMER_CODE1      0x3F    
const BYTE aIRMap[] =
{
#ifdef LUCKY_KARAOKE
KEY_PROGRAM    ,KEY_UP         ,KEY_REPEAT_AB  ,KEY_REPEAT     , // 00~03
KEY_STOP       ,KEY_NO_KEY     ,KEY_RETURN     ,KEY_PAUSE_STEP , // 04~07
KEY_CLEAR      ,KEY_DOWN       ,KEY_SF         ,KEY_DIGEST     , // 08~0B
KEY_DISPLAY    ,KEY_VOL_UP     ,KEY_NEXT       ,KEY_PREV       , // 0C~0F
KEY_GOTOTIME   ,KEY_RIGHT      ,KEY_POWER      ,KEY_VOL_DOWN   , // 10~13
KEY_SCF        ,KEY_RANDOM     ,KEY_OPEN_CLOSE ,KEY_PLAY       , // 14~17
KEY_N0         ,KEY_LEFT       ,KEY_ZOOM       ,KEY_MENU       , // 18~1B
KEY_SCB        ,KEY_MUTE       ,KEY_KEYUP      ,KEY_KEYDOWN    , // 1C~1F
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 20~23
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 24~27
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 28~2B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 2C~2F
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 30~33
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 34~37
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 38~3B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 3C~3F
KEY_ENTER      ,KEY_N1         ,KEY_N4         ,KEY_N7         , // 40~43
KEY_NO_KEY     ,KEY_N3         ,KEY_N6         ,KEY_N9         , // 44~47
KEY_SETUP      ,KEY_AST_CTL    ,KEY_ANGLE_CTL  ,KEY_SPST_CTL   , // 48~4B
KEY_NO_KEY     ,KEY_N2         ,KEY_N5         ,KEY_N8         , // 4C~4F
KEY_NO_KEY     ,KEY_AUDIO_LR   ,KEY_NO_KEY     ,KEY_KARAOKE    , // 50~53
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 54~57
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 58~5B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 5C~5F
#else
KEY_PROGRAM    ,KEY_UP         ,KEY_REPEAT_AB  ,KEY_REPEAT     , // 00~03
KEY_STOP       ,KEY_NO_KEY     ,KEY_RETURN     ,KEY_PAUSE_STEP , // 04~07
KEY_CLEAR      ,KEY_DOWN       ,KEY_SF         ,KEY_DIGEST     , // 08~0B
KEY_DISPLAY    ,KEY_VOL_UP     ,KEY_NEXT       ,KEY_PREV       , // 0C~0F
KEY_GOTOTIME   ,KEY_RIGHT      ,KEY_POWER      ,KEY_VOL_DOWN   , // 10~13
KEY_SCF        ,KEY_RANDOM     ,KEY_OPEN_CLOSE ,KEY_PLAY       , // 14~17
KEY_N0         ,KEY_LEFT       ,KEY_ZOOM       ,KEY_MENU       , // 18~1B
KEY_SCB        ,KEY_MUTE       ,KEY_NO_KEY     ,KEY_NO_KEY     , // 1C~1F
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 20~23
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 24~27
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 28~2B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 2C~2F
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 30~33
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 34~37
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 38~3B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 3C~3F
KEY_ENTER      ,KEY_N1         ,KEY_N4         ,KEY_N7         , // 40~43
KEY_NO_KEY     ,KEY_N3         ,KEY_N6         ,KEY_N9         , // 44~47
KEY_SETUP      ,KEY_AST_CTL    ,KEY_ANGLE_CTL  ,KEY_SPST_CTL   , // 48~4B
KEY_NO_KEY     ,KEY_N2         ,KEY_N5         ,KEY_N8         , // 4C~4F
KEY_NO_KEY     ,KEY_AUDIO_LR     ,KEY_NO_KEY   ,KEY_NO_KEY     , // 50~53
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 54~57
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 58~5B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 5C~5F
#endif
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == LARGE_VCD20)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFF
const BYTE aIRMap[] =
{
KEY_REMAIN       ,KEY_DIGEST       ,KEY_VOL_DOWN     ,KEY_VOL_UP       ,//00-03
KEY_VIDEO_OUTPUT_CTL ,KEY_TVMODE   ,KEY_UP           ,KEY_ZOOM         ,//04-07
KEY_GOTOTIME     ,KEY_N1           ,KEY_SCB          ,KEY_SURROUND     ,//08-0B
KEY_FUNCTION     ,KEY_N4           ,KEY_PREV         ,KEY_PSCAN        ,//0C-0F
KEY_OPEN_CLOSE   ,KEY_N7           ,KEY_N0           ,KEY_PAUSE        ,//10-13
KEY_POWER        ,KEY_N8           ,KEY_NO_KEY       ,KEY_N9           ,//14-17
KEY_SF           ,KEY_N5           ,KEY_NEXT         ,KEY_N6           ,//18-1B
KEY_AMP          ,KEY_N2           ,KEY_SCF          ,KEY_N3           ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_RIGHT        ,KEY_KEYNORMAL    ,KEY_ANGLE_CTL    ,KEY_AST_CTL      ,//40-43
KEY_DOWN         ,KEY_RANDOM       ,KEY_KEYUP        ,KEY_LEFT         ,//44-47
KEY_PBC          ,KEY_PROGRAM      ,KEY_MUTE         ,KEY_STEP         ,//48-4B
KEY_STOP         ,KEY_SETUP        ,KEY_MENU         ,KEY_NO_KEY       ,//4C-4F
KEY_DISPLAY      ,KEY_PLAY         ,KEY_RESUME       ,KEY_RETURN       ,//50-53
KEY_NO_KEY       ,KEY_REPEAT       ,KEY_FREEZE       ,KEY_KEYDOWN      ,//54-57
KEY_NO_KEY       ,KEY_N10PLUS      ,KEY_REPEAT_AB    ,KEY_NO_KEY       ,//58-5B
KEY_BAND         ,KEY_NO_KEY       ,KEY_AUDIO_LR     ,KEY_SPST_CTL     ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == MUSTEK_600)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x10 
#define     CUSTOMER_CODE1      0xED  
const BYTE aIRMap[] =
{
KEY_SCF          ,KEY_VOL_DOWN     ,KEY_MEMORY       ,KEY_CLEAR        ,//00-03
KEY_VOL_UP       ,KEY_STOP         ,KEY_AMP          ,KEY_NO_KEY       ,//04-07
KEY_RIGHT        ,KEY_PLAY         ,KEY_RETURN       ,KEY_LEFT         ,//08-0B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//0C-0F
KEY_MUTE         ,KEY_SPST_CTL     ,KEY_ANGLE_CTL    ,KEY_PBC          ,//10-13
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//14-17
KEY_N0           ,KEY_REPEAT       ,KEY_N5           ,KEY_N7           ,//18-1B
KEY_OPEN_CLOSE   ,KEY_REPEAT_AB    ,KEY_N2           ,KEY_POWER        ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_PREV         ,KEY_NEXT         ,KEY_SCB          ,KEY_BAND         ,//40-43
KEY_NO_KEY       ,KEY_FF           ,KEY_EQUALIZER    ,KEY_PAUSE_STEP   ,//44-47
KEY_FUNCTION     ,KEY_PLAY         ,KEY_UP           ,KEY_DOWN         ,//48-4B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//4C-4F
KEY_GOTOTIME     ,KEY_DIGEST       ,KEY_AST_CTL      ,KEY_ZOOM         ,//50-53
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//54-57
KEY_N4           ,KEY_N8           ,KEY_N9           ,KEY_N6           ,//58-5B
KEY_N1           ,KEY_DISPLAY      ,KEY_N3           ,KEY_SETUP        ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == MUSTEK_BLACK_IR)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x10
#define     CUSTOMER_CODE1      0xED
const BYTE aIRMap[] =
{
KEY_VOL_UP     ,KEY_MUTE       ,KEY_REPEAT     ,KEY_REPEAT_AB  , // 00~03
KEY_VOL_DOWN   ,KEY_SCF        ,KEY_AST_CTL    ,KEY_PREV       , // 04~07
KEY_RIGHT      ,KEY_STOP       ,KEY_PAUSE_STEP ,KEY_LEFT       , // 08~0B
KEY_RETURN     ,KEY_RETURN     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 0C~0F
KEY_SURROUND   ,KEY_ZOOM       ,KEY_KEYDOWN    ,KEY_ZOOM       , // 10~13
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 14~17
KEY_N0         ,KEY_CLEAR      ,KEY_N5         ,KEY_N7         , // 18~1B
KEY_OPEN_CLOSE ,KEY_PROGRAM    ,KEY_N2         ,KEY_POWER      , // 1C~1F
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 20~23
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 24~27
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 28~2B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 2C~2F
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 30~33
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 34~37
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 38~3B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 3C~3F
KEY_PBC        ,KEY_MENU       ,KEY_TITLE      ,KEY_GOTOTIME   , // 40~43
KEY_SPST_CTL   ,KEY_NEXT       ,KEY_ANGLE_CTL  ,KEY_SCB        , // 44~47
KEY_SF         ,KEY_PLAY       ,KEY_UP         ,KEY_DOWN       , // 48~4B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 4C~4F
KEY_DISPLAY    ,KEY_KEYUP      ,KEY_NO_KEY     ,KEY_NO_KEY     , // 50~53
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     , // 54~57
KEY_N4         ,KEY_N8         ,KEY_N9         ,KEY_N6         , // 58~5B
KEY_N1         ,KEY_TVMODE     ,KEY_N3         ,KEY_SETUP      , // 5C~5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == MUSTEK_SILVER_IR )
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x10
#define     CUSTOMER_CODE1      0xED
const BYTE aIRMap[] =
{
KEY_SCF          ,KEY_VOL_DOWN     ,KEY_PROGRAM      ,KEY_CLEAR        ,//00-03
KEY_VOL_UP       ,KEY_PAUSE_STEP   ,KEY_TVMODE       ,KEY_EQUALIZER    ,//04-07
KEY_RIGHT        ,KEY_STOP         ,KEY_BAND         ,KEY_LEFT         ,//08-0B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//0C-0F
KEY_MUTE         ,KEY_SPST_CTL     ,KEY_ANGLE_CTL    ,KEY_PBC          ,//10-13
KEY_PAUSE_STEP   ,KEY_STOP         ,KEY_NO_KEY       ,KEY_NO_KEY       ,//14-17
KEY_N0           ,KEY_REPEAT       ,KEY_N5           ,KEY_N7           ,//18-1B
KEY_OPEN_CLOSE   ,KEY_REPEAT_AB    ,KEY_N2           ,KEY_POWER        ,//1C-1F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//20-23
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//24-27
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//28-2B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//2C-2F
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//30-33
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//34-37
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//38-3B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//3C-3F
KEY_PREV         ,KEY_NEXT         ,KEY_SCB          ,KEY_GOTOTIME     ,//40-43
KEY_ZOOM         ,KEY_AMP          ,KEY_DISPLAY      ,KEY_SF           ,//44-47
KEY_FUNCTION     ,KEY_PLAY         ,KEY_UP           ,KEY_DOWN         ,//48-4B
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//4C-4F
KEY_SETUP        ,KEY_MENU         ,KEY_AUDIO_LR     ,KEY_DIGEST       ,//50-53
KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,KEY_NO_KEY       ,//54-57
KEY_N4           ,KEY_N8           ,KEY_N9           ,KEY_N6           ,//58-5B
KEY_N1           ,KEY_NO_KEY       ,KEY_N3           ,KEY_FULLSCREEN   ,//5C-5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == MUSTEK_KARAOKE )
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x10
#define     CUSTOMER_CODE1      0xED
const BYTE aIRMap[] =
{
KEY_VOL_UP          ,KEY_MUTE            ,KEY_REPEAT          ,KEY_REPEAT_AB       ,  // 00 - 03
KEY_VOL_DOWN        ,KEY_SCF             ,KEY_AUDIO_LR        ,KEY_PREV            ,  // 04 - 07
KEY_RIGHT           ,KEY_STOP            ,KEY_PAUSE_STEP      ,KEY_LEFT            ,  // 08 - 0B
KEY_NEXT            ,KEY_PREV            ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 0C - 0F
KEY_SURROUND        ,KEY_VOCAL           ,KEY_KEYDOWN         ,KEY_ZOOM            ,  // 10 - 13
KEY_PLAY            ,KEY_STOP            ,KEY_PAUSE           ,KEY_OPEN_CLOSE      ,  // 14 - 17
KEY_N0              ,KEY_CLEAR           ,KEY_N5              ,KEY_N7              ,  // 18 - 1B
KEY_OPEN_CLOSE      ,KEY_PROGRAM         ,KEY_N2              ,KEY_POWER           ,  // 1C - 1F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 20 - 23
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 24 - 27
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 28 - 2B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 2C - 2F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 30 - 33
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 34 - 37
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 38 - 3B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 3C - 3F
KEY_PBC             ,KEY_MENU            ,KEY_DIGEST          ,KEY_GOTOTIME        ,  // 40 - 43
KEY_SPST_CTL        ,KEY_NEXT            ,KEY_ANGLE_CTL       ,KEY_SCB             ,  // 44 - 47
KEY_SF              ,KEY_PLAY            ,KEY_UP              ,KEY_DOWN            ,  // 48 - 4B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 4C - 4F
KEY_DISPLAY         ,KEY_KEYUP           ,KEY_ECHO_DOWN       ,KEY_ECHO_UP         ,  // 50 - 53
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 54 - 57
KEY_N4              ,KEY_N8              ,KEY_N9              ,KEY_N6              ,  // 58 - 5B
KEY_N1              ,KEY_TVMODE          ,KEY_N3              ,KEY_SETUP           ,  // 5C - 5F
};
#endif

/*-------------------------------------------*/
#if  (IR_TYPE == FOXCONN_MDP3000)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFF
const BYTE aIRMap[] =
{
KEY_OPEN_CLOSE ,KEY_N3        ,KEY_SPST_CTL   ,KEY_RETURN   , // 00~03
KEY_AUDIO_LR   ,KEY_N2        ,KEY_PLAY       ,KEY_PBC      , // 04~07
KEY_TVMODE     ,KEY_N1        ,KEY_ANGLE_CTL  ,KEY_TITLE    , // 08~0B
KEY_POWER      ,KEY_DISPLAY   ,KEY_AST_CTL    ,KEY_SETUP    , // 0C~0F
KEY_NEXT       ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 10~13
KEY_PREV       ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 14~17
KEY_SCF        ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 18~1B
KEY_SCB        ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 1C~1F
KEY_NO_KEY     ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 20~23
KEY_NO_KEY     ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 24~27
KEY_NO_KEY     ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 28~2B
KEY_NO_KEY     ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 2C~2F
KEY_NO_KEY     ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 30~33
KEY_NO_KEY     ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 34~37
KEY_NO_KEY     ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 38~3B
KEY_NO_KEY     ,KEY_NO_KEY    ,KEY_NO_KEY     ,KEY_NO_KEY   , // 3C~3F
KEY_RIGHT      ,KEY_GOTOTIME  ,KEY_N9         ,KEY_N6       , // 40~43
KEY_UP         ,KEY_N10PLUS   ,KEY_N8         ,KEY_N5       , // 44~47
KEY_DOWN       ,KEY_N0        ,KEY_N7         ,KEY_N4       , // 48~4B
KEY_LEFT       ,KEY_CLEAR     ,KEY_PROGRAM    ,KEY_RANDOM   , // 4C~4F
KEY_REMAIN     ,KEY_SF        ,KEY_STOP       ,KEY_NO_KEY   , // 50~53
KEY_BOOKMARK   ,KEY_REPEAT_AB ,KEY_PAUSE      ,KEY_PLAY     , // 54~57
KEY_KARAOKE    ,KEY_TRANSFER  ,KEY_REPEAT     ,KEY_STEP     , // 58~5B
KEY_ZOOM       ,KEY_VOL_UP    ,KEY_VOL_DOWN   ,KEY_MUTE     , // 5C~5F
};
#endif

/*-------------------------------------------*/
#if  (IR_TYPE == FOXCONN_MDP3000_S)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFF
const BYTE aIRMap[] =
{
KEY_NEXT       ,KEY_STOP       ,KEY_SF         ,KEY_MUTE 	    , // 00~03
KEY_BOOKMARK   ,KEY_TVMODE     ,KEY_PAUSE      ,KEY_N9          , // 04~07
KEY_RANDOM     ,KEY_AUDIO_LR   ,KEY_STEP       ,KEY_N7   	    , // 08~0B
KEY_PROGRAM    ,KEY_MENU       ,KEY_SCF        ,KEY_N10PLUS 	, // 0C~0F
KEY_REPEAT_AB  ,KEY_TITLE      ,KEY_PLAY       ,KEY_N8          , // 10~13
KEY_CLEAR      ,KEY_PREV       ,KEY_SCB        ,KEY_GOTOTIME    , // 14~17
KEY_REPEAT     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     	, // 18~1B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY      , // 1C~1F
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY      , // 20~23
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY      , // 24~27
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY      , // 28~2B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY      , // 2C~2F
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY      , // 30~33
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY      , // 34~37
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY      , // 38~3B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY      , // 3C~3F
KEY_N0         ,KEY_N1         ,KEY_ANGLE_CTL  ,KEY_POWER       , // 40~43
KEY_N4         ,KEY_SETUP      ,KEY_PLAY       ,KEY_UP          , // 44~47
KEY_N5         ,KEY_AST_CTL    ,KEY_DISPLAY    ,KEY_LEFT        , // 48~4B
KEY_N2         ,KEY_VOL_UP     ,KEY_ZOOM       ,KEY_DOWN 	    , // 4C~4F
KEY_N6         ,KEY_VOL_DOWN   ,KEY_VOL_DOWN   ,KEY_RIGHT       , // 50~53
KEY_N3         ,KEY_SPST_CTL   ,KEY_VOL_UP     ,KEY_OPEN_CLOSE 	, // 54~57
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY 	    , // 58~5B
KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY     ,KEY_NO_KEY 	    , // 5C~5F	
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == MUSTEK_P_COMBO )
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x10
#define     CUSTOMER_CODE1      0xED
const BYTE aIRMap[] =
{
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 00 - 03
KEY_NO_KEY          ,KEY_RIGHT           ,KEY_AUDIO_LR        ,KEY_PREV     ,  // 04 - 07
KEY_NO_KEY          ,KEY_DOWN            ,KEY_UP              ,KEY_NO_KEY   ,  // 08 - 0B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 0C - 0F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_FUNCTION        ,KEY_NO_KEY   ,  // 10 - 13
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NAVIGATOR,  // 14 - 17
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 18 - 1B
KEY_OPEN_CLOSE      ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_POWER    ,  // 1C - 1F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 20 - 23
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 24 - 27
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 28 - 2B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 2C - 2F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 30 - 33
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 34 - 37
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 38 - 3B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 3C - 3F
KEY_NO_KEY          ,KEY_MENU            ,KEY_TITLE           ,KEY_NO_KEY   ,  // 40 - 43
KEY_SPST_CTL        ,KEY_NEXT            ,KEY_ANGLE_CTL       ,KEY_LEFT     ,  // 44 - 47
KEY_NO_KEY          ,KEY_PLAY            ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 48 - 4B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 4C - 4F
KEY_DISPLAY         ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 50 - 53
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 54 - 57
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 58 - 5B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_SETUP    ,  // 5C - 5F
};
/*
const BYTE aIRMap1[] =
{
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 00 - 03
KEY_NO_KEY          ,KEY_NEXT            ,KEY_AUDIO_LR        ,KEY_PREV     ,  // 04 - 07
KEY_NO_KEY          ,KEY_DOWN            ,KEY_UP              ,KEY_NO_KEY   ,  // 08 - 0B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 0C - 0F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_FUNCTION        ,KEY_NO_KEY   ,  // 10 - 13
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NAVIGATOR,  // 14 - 17
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 18 - 1B
KEY_OPEN_CLOSE      ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_POWER    ,  // 1C - 1F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 20 - 23
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 24 - 27
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 28 - 2B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 2C - 2F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 30 - 33
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 34 - 37
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 38 - 3B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 3C - 3F
KEY_NO_KEY          ,KEY_MENU            ,KEY_TITLE           ,KEY_NO_KEY   ,  // 40 - 43
KEY_SPST_CTL        ,KEY_NEXT            ,KEY_ANGLE_CTL       ,KEY_PREV     ,  // 44 - 47
KEY_NO_KEY          ,KEY_PLAY            ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 48 - 4B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 4C - 4F
KEY_DISPLAY         ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 50 - 53
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 54 - 57
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 58 - 5B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_SETUP    ,  // 5C - 5F
};
*/
#endif

/*-------------------------------------------*/
#if (IR_TYPE == MUSTEK_P_28KEY )
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x10
#define     CUSTOMER_CODE1      0xED
const BYTE aIRMap[] =
{
KEY_VOL_UP          ,KEY_MUTE            ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 00 - 03
KEY_VOL_DOWN        ,KEY_RIGHT           ,KEY_AUDIO_LR        ,KEY_PREV     ,  // 04 - 07
KEY_NO_KEY          ,KEY_DOWN            ,KEY_UP              ,KEY_NO_KEY   ,  // 08 - 0B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 0C - 0F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_FUNCTION        ,KEY_NO_KEY   ,  // 10 - 13
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NAVIGATOR,  // 14 - 17
KEY_N0              ,KEY_NO_KEY          ,KEY_N5              ,KEY_N7       ,  // 18 - 1B
KEY_OPEN_CLOSE      ,KEY_NO_KEY          ,KEY_N2              ,KEY_POWER    ,  // 1C - 1F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 20 - 23
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 24 - 27
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 28 - 2B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 2C - 2F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 30 - 33
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 34 - 37
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 38 - 3B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 3C - 3F
KEY_NO_KEY          ,KEY_MENU            ,KEY_TITLE           ,KEY_NO_KEY   ,  // 40 - 43
KEY_SPST_CTL        ,KEY_NEXT            ,KEY_ANGLE_CTL       ,KEY_LEFT     ,  // 44 - 47
KEY_NO_KEY          ,KEY_PLAY            ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 48 - 4B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 4C - 4F
KEY_DISPLAY         ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 50 - 53
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY   ,  // 54 - 57
KEY_N4              ,KEY_N9              ,KEY_NO_KEY          ,KEY_N6       ,  // 58 - 5B
KEY_N1              ,KEY_NO_KEY          ,KEY_N3              ,KEY_SETUP    ,  // 5C - 5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == KONKA_IR)
#define     IR_CODE_TYPE        SW_KONKA
#define     CUSTOMER_CODE       0x06
#define     CUSTOMER_CODE1      0x00
const BYTE aIRMap[] =
{
KEY_N0              ,KEY_N1             ,KEY_N2             ,KEY_N3         ,//00-03
KEY_N4              ,KEY_N5             ,KEY_N6             ,KEY_N7         ,//04-07
KEY_N8              ,KEY_N9             ,KEY_DISPLAY        ,KEY_POWER      ,//08-0B
KEY_TITLE           ,KEY_REPEAT         ,KEY_GOTOTIME       ,KEY_OPEN_CLOSE ,//0C-0F
KEY_PREV            ,KEY_NEXT           ,KEY_NO_KEY         ,KEY_NO_KEY     ,//10-13
KEY_NO_KEY          ,KEY_MENU           ,KEY_UP             ,KEY_PLAY_PAUSE ,//14-17
KEY_NO_KEY          ,KEY_AST_CTL        ,KEY_SPST_CTL       ,KEY_NO_KEY     ,//18-1B
KEY_NO_KEY          ,KEY_NO_KEY         ,KEY_STOP           ,KEY_ZOOM       ,//1C-1F
KEY_LEFT            ,KEY_PLAY           ,KEY_RIGHT          ,KEY_ANGLE_CTL  ,//20-23
KEY_NO_KEY          ,KEY_DOWN           ,KEY_NO_KEY         ,KEY_SCF        ,//24-27
KEY_SETUP           ,KEY_SCB            ,KEY_PBC            ,KEY_NO_KEY     ,//28-2B
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == YHI_IR)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x01
#define     CUSTOMER_CODE1      0xFE
const BYTE aIRMap[] =
{
KEY_SETUP           ,KEY_AST_CTL         ,KEY_N1              ,KEY_N2              ,  // 00 - 03
KEY_OPEN_CLOSE      ,KEY_DISPLAY         ,KEY_N4              ,KEY_N5              ,  // 04 - 07
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_N7              ,KEY_N8              ,  // 08 - 0B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_N0              ,KEY_UP              ,  // 0C - 0F
KEY_VOL_DOWN        ,KEY_NO_KEY          ,KEY_LEFT            ,KEY_ENTER           ,  // 10 - 13
KEY_NO_KEY          ,KEY_MUTE            ,KEY_ZOOM            ,KEY_DOWN            ,  // 14 - 17
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_REPEAT          ,KEY_TITLE           ,  // 18 - 1B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_VOL_UP          ,KEY_MENU            ,  // 1C - 1F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 20 - 23
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 24 - 27
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 28 - 2B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 2C - 2F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 30 - 33
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 34 - 37
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 38 - 3B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 3C - 3F
KEY_GOTOTIME        ,KEY_N3              ,KEY_PROGRAM         ,KEY_PBC             ,  // 40 - 43
KEY_SPST_CTL        ,KEY_N6              ,KEY_FUNCTION        ,KEY_NO_KEY          ,  // 44 - 47
KEY_PLAY            ,KEY_N9              ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 48 - 4B
KEY_STOP            ,KEY_RETURN          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 4C - 4F
KEY_PAUSE_STEP      ,KEY_RIGHT           ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 50 - 53
KEY_SF              ,KEY_DIGEST          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 54 - 57
KEY_SCF             ,KEY_SCB             ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 58 - 5B
KEY_NEXT            ,KEY_PREV            ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 5C - 5F
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == PHILIPS_IR)
#define     IR_CODE_TYPE        SW_RC5
#define     CUSTOMER_CODE       0x17
#define     CUSTOMER_CODE1      0
const BYTE aIRMap[] =
{
KEY_N7              ,KEY_DOWN           ,KEY_LEFT           ,KEY_REPEAT_AB      ,   // 00 - 03
KEY_N9              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_ZOOM           ,   // 04 - 07
KEY_N10PLUS         ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_PROGRAM        ,   // 08 - 0b
KEY_OPEN_CLOSE      ,KEY_PREV           ,KEY_NO_KEY         ,KEY_TITLE          ,   // 0c - 0f
KEY_N3              ,KEY_SELECT         ,KEY_N1             ,KEY_SPST_CTL       ,   // 10 - 13
KEY_N4              ,KEY_RETURN         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 14 - 17
KEY_N5              ,KEY_NO_KEY         ,KEY_SCB            ,KEY_AST_CTL        ,   // 18 - 1b // wyc2.21-909S, modify key assignment to correct key.
KEY_MENU            ,KEY_PLAY_PAUSE     ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 1c - 1f
KEY_CLEAR           ,KEY_UP             ,KEY_RIGHT          ,KEY_NO_KEY         ,   // 20 - 23
KEY_N8              ,KEY_ANGLE_CTL      ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 24 - 27
KEY_N6              ,KEY_NO_KEY         ,KEY_SCF            ,KEY_NO_KEY         ,   // 28 - 2b
KEY_POWER           ,KEY_NEXT           ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 2c - 2f
KEY_N2              ,KEY_SEARCH         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 30 - 33
KEY_NO_KEY          ,KEY_REPEAT         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 34 - 37
KEY_N0              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_INFO           ,   // 38 - 3b
KEY_NO_KEY          ,KEY_STOP           ,KEY_NO_KEY         ,KEY_DISPLAY        ,   // 3c - 3f
KEY_NO_KEY          ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 40 - 43
KEY_NO_KEY          ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 44 - 47
KEY_NO_KEY          ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 48 - 4b
KEY_NO_KEY          ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 4c - 4f
KEY_NO_KEY          ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 50 - 53
KEY_NO_KEY          ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 54 - 57
KEY_NO_KEY          ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 58 - 5b
KEY_NO_KEY          ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 5c - 5f
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == WS_258_WHITE)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFF

BYTE aIRMap[]=
{
KEY_ANGLE_CTL       ,KEY_SCF             ,KEY_STOP            ,KEY_PLAY_PAUSE      ,  // 00 - 03
KEY_SPST_CTL        ,KEY_RETURN          ,KEY_PROGRAM         ,KEY_AST_CTL         ,  // 04 - 07
KEY_REMAIN          ,KEY_ZOOM            ,KEY_N10PLUS         ,KEY_N7              ,  // 08 - 0B
KEY_PBC             ,KEY_DISPLAY         ,KEY_N2              ,KEY_N4              ,  // 0C - 0F
KEY_VOL_UP          ,KEY_NEXT            ,KEY_FUNCTION        ,KEY_CLEAR/*KEY_VIDEO_OUTPUT_CTL*/,  // 10 - 13
KEY_VOL_DOWN        ,KEY_RIGHT           ,KEY_DOWN            ,KEY_OPEN_CLOSE      ,  // 14 - 17
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 18 - 1B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 1C - 1F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 20 - 23
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 24 - 27
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 28 - 2B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 2C - 2F
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 30 - 33
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 34 - 37
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 38 - 3B
KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 3C - 3F
KEY_SETUP           ,KEY_SCB             ,KEY_NO_KEY          ,KEY_NO_KEY          ,  // 40 - 43
KEY_AUDIO_LR        ,KEY_TVMODE          ,KEY_UP              ,KEY_LEFT            ,  // 44 - 47
KEY_N0              ,KEY_GOTOTIME        ,KEY_N9              ,KEY_N8              ,  // 48 - 4B
KEY_N1              ,KEY_N3              ,KEY_N6              ,KEY_N5              ,  // 4C - 4F
KEY_KARAOKE         ,KEY_PREV            ,KEY_REPEAT_AB       ,KEY_REPEAT          ,  // 50 - 53
KEY_SF              ,KEY_PLAY            ,KEY_MUTE            ,KEY_STEP            ,  // 54 - 57
};
#endif

/*-------------------------------------------*/
#if (IR_TYPE == CHEERTEK_IR)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFF
BYTE aIRMap[]=
{
KEY_VIDEO_OUTPUT_CTL    ,KEY_RESUME         ,KEY_N6             ,KEY_N5             ,  // 00 - 03
KEY_KEYUP               ,KEY_KEYDOWN        ,KEY_N9             ,KEY_N8             ,  // 04 - 07
KEY_FUNCTION            ,KEY_NAVIGATOR      ,KEY_N3             ,KEY_N2             ,  // 08 - 0B
KEY_EXIT                ,KEY_SERVICE_LIST   ,KEY_DISPLAY        ,KEY_ANGLE_CTL      ,  // 0C - 0F
KEY_NEXT                ,KEY_PREV           ,KEY_RETURN         ,KEY_ENTER          ,  // 10 - 13
KEY_PBC                 ,KEY_STEP           ,KEY_MENU           ,KEY_UP             ,  // 14 - 17
KEY_VOL_UP              ,KEY_VOL_DOWN       ,KEY_GOTOTIME       ,KEY_N10PLUS        ,  // 18 - 1B
KEY_TV_RADIO            ,KEY_YELLOW         ,KEY_OPEN_CLOSE     ,KEY_AST_CTL        ,  // 1C - 1F
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 20 - 23
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 24 - 27
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 28 - 2B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 2C - 2F
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 30 - 33
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 34 - 37
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 38 - 3B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 3C - 3F
KEY_CLEAR               ,KEY_N4             ,KEY_PLAY           ,KEY_PROGRAM        ,  // 40 - 43
KEY_RANDOM              ,KEY_N7             ,KEY_SCF            ,KEY_SF             ,  // 44 - 47
KEY_DIGEST              ,KEY_N1             ,KEY_SCB            ,KEY_TVMODE         ,  // 48 - 4B
KEY_TTX                 ,KEY_SPST_CTL       ,KEY_RIGHT          ,KEY_EPG            ,  // 4C - 4F
KEY_FAV_UP              ,KEY_SETUP          ,KEY_DOWN           ,KEY_FAV_DOWN       ,  // 50 - 53
KEY_REPEAT              ,KEY_TITLE          ,KEY_LEFT           ,KEY_REPEAT_AB      ,  // 54 - 57
KEY_MUTE                ,KEY_N0             ,KEY_STOP           ,KEY_ZOOM           ,  // 58 - 5B
KEY_GREEN               ,KEY_POWER          ,KEY_PAUSE          ,KEY_RED            ,  // 5C - 5F
};
#endif //(IR_TYPE == CHEERTEK_IR)

#if (IR_TYPE == KXDDIGITAL_IR)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFB
BYTE aIRMap[]=
{
KEY_MENU,			KEY_NO_KEY         ,KEY_FUNCTION,		KEY_NO_KEY         ,  // 00 - 03
KEY_PLAY_PAUSE,		KEY_UP,			KEY_ZOOM,		KEY_NO_KEY         ,   // 04 - 07
KEY_LEFT,			KEY_ENTER,		KEY_RIGHT,		KEY_NO_KEY         ,   // 08 - 0B
KEY_STOP,			KEY_DOWN,		KEY_VOL_UP,KEY_NO_KEY         ,   // 0C - 0F
	
KEY_PREV,			KEY_NEXT,		KEY_VOL_DOWN,	KEY_NO_KEY         ,   // 10 - 13
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 14 - 17
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 18 - 1B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 1C - 1F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 20 - 23
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 24 - 27
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 28 - 2B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 2C - 2F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 30 - 33
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 34 - 37
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 38 - 3B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 3C - 3F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 40 - 43
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 44 - 47
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 48 - 4B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 4C - 4F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 50 - 53
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 54 - 57
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 58 - 5B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 5C - 5F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 60 - 63
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 64 - 567
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 68 - 6B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 6C - 6F

};
#endif //(IR_TYPE == CHEERTEK_IR)

#if (IR_TYPE == CONNTEL_IR)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFF
BYTE aIRMap[]=
{
KEY_FUNCTION,		KEY_NO_KEY         ,KEY_PREV,		KEY_NO_KEY         ,  // 00 - 03
KEY_DOWN,			KEY_NEXT,		KEY_PLAY_PAUSE,	KEY_NO_KEY         ,   // 04 - 07
KEY_NO_KEY              ,KEY_PHOTO,		KEY_MENU,		KEY_NO_KEY         ,   // 08 - 0B
KEY_NO_KEY              ,KEY_STOP,		KEY_ZOOM,	KEY_NO_KEY        ,   // 0C - 0F
	
KEY_POWER              ,KEY_UP,			KEY_LEFT,		KEY_NO_KEY         ,   // 10 - 13
KEY_NO_KEY              ,KEY_INFO,		KEY_SELECT,		KEY_VOL_UP,  // 14 - 17
KEY_NO_KEY              ,KEY_PHOTO_MUSIC,KEY_COPY_DEL,    KEY_SETUP,   // 18 - 1B
KEY_NO_KEY              ,KEY_MUSIC,		KEY_TIME         ,KEY_MOVIE,  // 1C - 1F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 20 - 23
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 24 - 27
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 28 - 2B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 2C - 2F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 30 - 33
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 34 - 37
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 38 - 3B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 3C - 3F

KEY_VOL_DOWN,		KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 40 - 43
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 44 - 47
KEY_MUTE,		    KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 48 - 4B
KEY_RIGHT,		    KEY_SCF,			KEY_NO_KEY         ,KEY_NO_KEY         , // 4C - 4F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 50 - 53
KEY_SCB,		     KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 54 - 57
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 58 - 5B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 5C - 5F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 60 - 63
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 64 - 567
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 68 - 6B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 6C - 6F

};
#endif //(IR_TYPE == CHEERTEK_IR)


#if (IR_TYPE == CONNTEL_IR2)
#define     IR_CODE_TYPE        HW_NEC
#define     CUSTOMER_CODE       0x00
#define     CUSTOMER_CODE1      0xFF
BYTE aIRMap[]=
{
KEY_FUNCTION,         KEY_COPY_DEL,		KEY_NO_KEY         ,KEY_ZOOM,  // 00 - 03
KEY_DOWN,			KEY_NO_KEY         ,KEY_MUTE,		KEY_NO_KEY          ,   // 04 - 07
KEY_NO_KEY              ,KEY_PHOTO,		KEY_NO_KEY         ,KEY_NO_KEY          ,   // 08 - 0B
KEY_NO_KEY              ,KEY_PHOTO_MUSIC         ,KEY_NO_KEY,KEY_NO_KEY         ,   // 0C - 0F
	
KEY_POWER,			KEY_UP,			KEY_LEFT,		KEY_NO_KEY         ,   // 10 - 13
KEY_NO_KEY              ,KEY_NEXT,		KEY_PLAY_PAUSE,  KEY_VOL_UP,  // 14 - 17
KEY_NO_KEY              ,KEY_TIME,		KEY_NO_KEY         ,KEY_PREV,   // 18 - 1B
KEY_NO_KEY              ,KEY_MUSIC,		KEY_NO_KEY         ,KEY_MOVIE,  // 1C - 1F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 20 - 23
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 24 - 27
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 28 - 2B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 2C - 2F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 30 - 33
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 34 - 37
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 38 - 3B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 3C - 3F

KEY_VOL_DOWN,		KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY           ,  // 40 - 43
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 44 - 47
KEY_STOP,			KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,   // 48 - 4B
KEY_RIGHT,		     KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY           , // 4C - 4F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 50 - 53
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY           , // 54 - 57
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 58 - 5B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 5C - 5F

KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 60 - 63
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 64 - 567
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         ,  // 68 - 6B
KEY_NO_KEY              ,KEY_NO_KEY         ,KEY_NO_KEY         ,KEY_NO_KEY         , // 6C - 6F

};
#endif //(IR_TYPE == CHEERTEK_IR)



#endif //__IR_H__
