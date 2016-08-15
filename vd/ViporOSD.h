#ifndef  _DVD_OSD_H
#define  _DVD_OSD_H

//--------------------------------------------------------------------------
//	Function  : Vipor_OsdLoadFont
//	Purpose   : Load font code into Vipor internal OSD
//--------------------------------------------------------------------------
extern void Vipor_OsdInit(void);

extern void Vipor_OsdPositionSet(BYTE bHpos, BYTE bVpos);

//--------------------------------------------------------------------------
// Function    : Vipor_OsdDisableWindow
// Description : disable a OSD window
// Input       : bIndex -> Window index (0-3)
//--------------------------------------------------------------------------
extern void Vipor_OsdDisableWindow(BYTE bIndex);


//--------------------------------------------------------------------------
// Function    : Vipor_OsdSetAddress
// Description : Set OSD active RAM index address
// Input       : bRow -> Y position
//               bCol -> X position
//--------------------------------------------------------------------------
extern void Vipor_OsdSetAddress(BYTE bRow, BYTE bCol);

//--------------------------------------------------------------------------
// Function    : Vipor_OsdClearRow
// Description : clear row data
// Input       : bVstart -> startRow
//               bVend   -> endRow
//--------------------------------------------------------------------------
extern void Vipor_OsdClearRow(BYTE bVstart, BYTE bVend, BYTE bAttr);


//--------------------------------------------------------------------------
// Function    : PrintChar
// Description : Print a char
// Input       : bChar -> character
//               (col,row) -> (x,y) Print position
//--------------------------------------------------------------------------
//void Vipor_PrintChar(BYTE bRow, BYTE bCol, BYTE bColor, BYTE bChar);

//--------------------------------------------------------------------------
// Function    : PrintString
// Description : Print a string in OSD. The  pRomData pointer is first character
// Input       : pRomData  -> array of string head
//               (col,row) -> (x,y) Print position
//--------------------------------------------------------------------------
extern void Vipor_PrintString(BYTE bRow, BYTE bCol, BYTE bColor, BYTE code *pRomData);



//--------------------------------------------------------------------------
// Function    : PrintChar
// Description : Print a char
// Input       : bChar -> character
//               (col,row) -> (x,y) Print position
//--------------------------------------------------------------------------
extern void Vipor_PrintChar(BYTE bRow, BYTE bCol, BYTE bColor, BYTE bChar);

extern void Vipor_SetFirstCharAttr(BYTE bRow, BYTE bCol, BYTE bColor);

extern void Vipor_SetOneCharContent(BYTE bChar);


//**************************************************************************
// Function    : OsdWindow
// Description : Open a OSD window
// Input       : num              -> OSD window index(0--3)
// note:      num must between 0 and 3
//               (x0, y0, x1, y1) -> define window area
// note:	    x0 < x1(from 0 to 31), 
//               y0 < y1(from 0 to 14), 
//               color            -> define window color
//**************************************************************************
extern void OsdWindow(BYTE num, BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE color);


//--------------------------------------------------------------------------
// Function    : Vipor_OsdEnable
// Description : Enable/Disable osd display
// Input       : 1 -> Enable
//               0 -> Disable
//--------------------------------------------------------------------------
extern void Vipor_OsdEnable(BYTE flag);

#endif //_DVD_OSD_H
	
