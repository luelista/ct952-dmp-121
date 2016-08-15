
#ifndef __TFTSETUP_H__
#define __TFTSETUP_H__

#include ".\VD\inc\VP_OSDFONT.H"

#define TFTSETUP_RECT0_X0   0
#define TFTSETUP_RECT0_Y0   0 
#define TFTSETUP_RECT0_X1  11 
#define TFTSETUP_RECT0_Y1   4 

#define TFTSETUP_RECT1_X0   15 
#define TFTSETUP_RECT1_Y0   0 
#define TFTSETUP_RECT1_X1   18 
#define TFTSETUP_RECT1_Y1   6 

#define ITEM_HEIGHT  1
#define ITEM_WIDTH   10

#define MENU_LEVEL   2


typedef struct tagMENU_ITEM MENU_ITEM;
typedef MENU_ITEM*          LIST_MENU; 

struct tagMENU_ITEM
{
    BYTE *pbString;   
    LIST_MENU *pSubMenu;
    BYTE bID;              // High 4 bits stores the selected Index,
                           // Low 4 bits stores the item number
};


//Alan1.08a, to define the default value 
#define TFT_DEFAULT_BRIGHTNESS ID_3
#define TFT_DEFAULT_CONTRAST   ID_3
#define TFT_DEFAULT_HUE        ID_3
#define TFT_DEFAULT_SATURATION ID_3
#define TFT_DEFAULT_SHARPNESS  ID_3

extern BYTE TFTSETUP_ProcessKey(BYTE bKey);
extern void TFTSETUP_Initial(void);
extern void TFT_SetDefaultValue(void); //Alan1.08a, to set the default value 




//////////////////   Initialize the strings ///////////////////////////////

BYTE code aValue_P3[] = {_PLUS, _3, _EOF};
BYTE code aValue_P2[] = {_PLUS, _2, _EOF};
BYTE code aValue_P1[] = {_PLUS, _1, _EOF};
BYTE code aValue_0[] =  {_SP, _0, _EOF};
BYTE code aValue_M1[] = {_MINUS, _1, _EOF};
BYTE code aValue_M2[] = {_MINUS, _2, _EOF};
BYTE code aValue_M3[] = {_MINUS, _3, _EOF};
BYTE code aBrightnessText[] = {_B, _r, _i, _g, _h, _t, _n, _e, _s, _s, _EOF};
BYTE code aContrastText[] =   {_C, _o, _n, _t, _r, _a, _s, _t, _EOF};
BYTE code aHueText[] = {_H, _u, _e, _EOF};
BYTE code aSaturationText[] = {_S, _a, _t, _u, _r, _a, _t, _i, _o, _n, _EOF};
BYTE code aSharpnessText[] = {_S, _h, _a, _r, _p, _n, _e, _s, _s, _EOF};


///////////////////   Create the level 1 menu   /////////////////////////////

enum Level1_Item {ID_0, ID_1, ID_2, ID_3, ID_4, ID_5, ID_6, ID_LEVEL1_END};

MENU_ITEM _Item_Level1_0_Value_Range = {NULL, NULL, ID_LEVEL1_END};
MENU_ITEM _Item_Level1_1_Value_Range = {NULL, NULL, ID_LEVEL1_END};
MENU_ITEM _Item_Level1_2_Value_Range = {NULL, NULL, ID_LEVEL1_END};
MENU_ITEM _Item_Level1_3_Value_Range = {NULL, NULL, ID_LEVEL1_END};
MENU_ITEM _Item_Level1_4_Value_Range = {NULL, NULL, ID_LEVEL1_END};

MENU_ITEM code _Item_Value_P3 = {aValue_P3, NULL, ID_0};
MENU_ITEM code _Item_Value_P2 = {aValue_P2, NULL, ID_1};
MENU_ITEM code _Item_Value_P1 = {aValue_P1, NULL, ID_2};
MENU_ITEM code _Item_Value_0 =  {aValue_0, NULL, ID_3};
MENU_ITEM code _Item_Value_M1 = {aValue_M1, NULL, ID_4};
MENU_ITEM code _Item_Value_M2 = {aValue_M2, NULL, ID_5};
MENU_ITEM code _Item_Value_M3 = {aValue_M3, NULL, ID_6};


LIST_MENU code _List_Menu_Level1_0[] = {
    &_Item_Level1_0_Value_Range,
    &_Item_Value_P3, &_Item_Value_P2, &_Item_Value_P1,
    &_Item_Value_0,
    &_Item_Value_M1, &_Item_Value_M2, &_Item_Value_M3
};

LIST_MENU code _List_Menu_Level1_1[] = {
    &_Item_Level1_1_Value_Range,
    &_Item_Value_P3, &_Item_Value_P2, &_Item_Value_P1,
    &_Item_Value_0,
    &_Item_Value_M1, &_Item_Value_M2, &_Item_Value_M3
};

LIST_MENU code _List_Menu_Level1_2[] = {
    &_Item_Level1_2_Value_Range,
    &_Item_Value_P3, &_Item_Value_P2, &_Item_Value_P1,
    &_Item_Value_0,
    &_Item_Value_M1, &_Item_Value_M2, &_Item_Value_M3
};

LIST_MENU code _List_Menu_Level1_3[] = {
    &_Item_Level1_3_Value_Range,
    &_Item_Value_P3, &_Item_Value_P2, &_Item_Value_P1,
    &_Item_Value_0,
    &_Item_Value_M1, &_Item_Value_M2, &_Item_Value_M3
};

LIST_MENU code _List_Menu_Level1_4[] = {
    &_Item_Level1_4_Value_Range,
    &_Item_Value_P3, &_Item_Value_P2, &_Item_Value_P1,
    &_Item_Value_0,
    &_Item_Value_M1, &_Item_Value_M2, &_Item_Value_M3
};


///////////////////   Create the level 0 menu   /////////////////////////////

enum Level0_Item {ID_BRIGHTNESS, ID_CONTRAST, ID_HUE,
                  ID_SATURATION, ID_SHARPNESS, ID_LEVEL0_END 
};

MENU_ITEM _Item_Level0_Value_Range = {NULL, NULL, ID_LEVEL0_END};

MENU_ITEM code _Item_Brightness = {aBrightnessText, _List_Menu_Level1_0, ID_BRIGHTNESS};
MENU_ITEM code _Item_Contrast = {aContrastText, _List_Menu_Level1_1, ID_CONTRAST};
MENU_ITEM code _Item_Hue = {aHueText, _List_Menu_Level1_2, ID_HUE};
MENU_ITEM code _Item_Sat = {aSaturationText, _List_Menu_Level1_3, ID_SATURATION};
MENU_ITEM code _Item_Sharp = {aSharpnessText, _List_Menu_Level1_4, ID_SHARPNESS};

LIST_MENU code _List_Menu_Level0[] = {
    &_Item_Level0_Value_Range,
    &_Item_Brightness,
    &_Item_Contrast, 
    &_Item_Hue,
    &_Item_Sat,
    &_Item_Sharp
};


typedef struct tagVIPORINFO
{
    BYTE    bBrightness;        //Brightness
    BYTE    bContrast;          //Contrast
    BYTE    bHue;               //Hue
    BYTE    bSaturation;        //Saturation
    BYTE    bSharpness;         //Sharpness
} VIPORINFO;

VIPORINFO __ViporInfo;

#endif // __TFTSETUP_H__



