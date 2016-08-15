#define CONVCHAR_MAIN_FILE

#include "winav.h"
#include "osdfont.h"
#include "fontindex.h" //++CoCo2.37P

/*
#ifndef CUSTOMER_ASCII
#include "OSDString/Ascii_Remapping.txt"
#endif

WORD _aASCIIMapTab[][2] = 
{
    #include "OSDString/Ascii_Mapping.txt"
};
*/

WORD* _pwMapTab = NULL;

WORD CONVCHAR_ASCII_To_CTK(BYTE bChar)
{   
    BYTE bTemp;
	
    _pwMapTab = (WORD*) (__aAsciiMappingTable[0]);
			
    for (bTemp = 1; bTemp < _pwMapTab[0]; bTemp++)
    {
        if (bChar == _pwMapTab[bTemp*2])
        {
            break;
        }
    }
	           
    if (bTemp == _pwMapTab[0]) // not Found
    {
        bChar = (bChar % 26) + 0x41;  // Random remap to character A~Z.
        for (bTemp = 1; bTemp < _pwMapTab[0]; bTemp++)
        {
            if (bChar == _pwMapTab[bTemp*2])
            {
                break;
            }
        }   
    }
   
    return _pwMapTab[bTemp*2+1];    
}

WORD CONVCHAR_Uni_To_CTK(WORD wChar)
{
    BYTE bTemp;
    //Step1: search ASCII mapping table.	
    _pwMapTab = (WORD*) (__aAsciiMappingTable[0]);

    for (bTemp = 1; bTemp < _pwMapTab[0]; bTemp++)
    {
        if (wChar == _pwMapTab[bTemp*2])
        {
            break;
        }
    }
    //Step2: if not found in ASCII mapping table, search Unicode mapping table.
    if (bTemp == _pwMapTab[0]) // not Found
    {
        _pwMapTab = __wCurUniMapTab;
        if (_pwMapTab != (WORD*) (__aAsciiMappingTable[0]))
        {
             for (bTemp = 1; bTemp < _pwMapTab[0]; bTemp++)
             {
                 if (wChar == _pwMapTab[bTemp*2])
                 {
                     break;
                 }
             }
        }
    }
    //Step3: if not found, return a arbitrary character between A~Z. Otherwise, return the found character.           
    if (bTemp == _pwMapTab[0]) // not Found
    {
        wChar = (wChar % 26) + 0x41;        
        return CONVCHAR_ASCII_To_CTK((BYTE)wChar); 
    }
    else
    {
        return _pwMapTab[bTemp*2+1]; 
    }
}

WORD CONVCHAR_ISO_CP_To_CTK(BYTE bChar)
{
    BYTE bTemp;
    //Step1: search ASCII mapping table.	
    _pwMapTab = (WORD*) (__aAsciiMappingTable[0]);

    for (bTemp = 1; bTemp < _pwMapTab[0]; bTemp++)
    {
        if (bChar == _pwMapTab[bTemp*2])
        {
            break;
        }
    }
    //Step2: if not found in ASCII mapping table, search ISO/CP mapping table.
    if (bTemp == _pwMapTab[0]) // not Found
    {
        _pwMapTab = __wCurLocMapTab;
        if (_pwMapTab != (WORD*) (__aAsciiMappingTable[0]))
        {
             for (bTemp = 1; bTemp < _pwMapTab[0]; bTemp++)
             {
                 if (bChar == _pwMapTab[bTemp*2])
                 {
                     break;
                 }
             }
        }
    }
    //Step3: if not found, return a arbitrary character between A~Z. Otherwise, return the found character.           
    if (bTemp == _pwMapTab[0]) // not Found
    {
        bChar = (bChar % 26) + 0x41;        
        return CONVCHAR_ASCII_To_CTK(bChar); 
    }
    else
    {
        return _pwMapTab[bTemp*2+1]; 
    }
}

#ifdef SUPPORT_UNICODE_CODING
WORD CONVCHAR_Uni_To_FontSize(WORD wChar)
{
    BYTE	bCutNum = 30;	// Total number of separated mapping table
    WORD wTabSection[bCutNum+1];
    WORD wCharIndex = 0;
    BYTE bSectionIndex = 0;

    _pwMapTab = __wFontSizeTable;

    // Separate table into several section, and just do search in one section.
    // It will make search more efficiently.
    for (bSectionIndex = 0; bSectionIndex < bCutNum; bSectionIndex++)
    {
        wTabSection[bSectionIndex] = (_pwMapTab[0] / bCutNum) * bSectionIndex;
    }
  
    wTabSection[bCutNum] = _pwMapTab[0] - 1;    
   
    // Get the mapping Unicode font size.    
         
    bSectionIndex = 0;	            

    while (bSectionIndex < bCutNum)
    {
        bSectionIndex++;
		 
        if (_pwMapTab[wTabSection[bSectionIndex]*2] >= wChar)
        {			 
            for (wCharIndex = wTabSection[bSectionIndex-1]+1; wCharIndex <= wTabSection[bSectionIndex]; wCharIndex++)
            {
                if (_pwMapTab[wCharIndex*2] == wChar)
                    break;
            }
            if (wCharIndex == wTabSection[bSectionIndex]+1)
                wCharIndex  = 0xFFFF;    // not found                        

            bSectionIndex = 0xFF;	// The character has been searched
        }
    }	

    if ((bSectionIndex != 0xFF) || (wCharIndex == 0xFFFF)) // not found      
        return 0x9A00; // Default font size: 26*26 pixels.
    else
        return _pwMapTab[wCharIndex*2+1];    
}

WORD CONVCHAR_Uni_To_IDX(WORD wChar)
{
    BYTE	bCutNum = 30;	// Total number of separated mapping table
    WORD wTabSection[bCutNum+1];
    WORD wCharIndex = 0;
    BYTE bSectionIndex = 0;

    _pwMapTab = __wUnicodeTable;

    // Separate table into several section, and just do search in one section.
    // It will make search more efficiently.
    for (bSectionIndex = 0; bSectionIndex < bCutNum; bSectionIndex++)
    {
        wTabSection[bSectionIndex] = (_pwMapTab[0] / bCutNum) * bSectionIndex;
    }
      
    wTabSection[bCutNum] = _pwMapTab[0] - 1;    
       
    // Map Unicode to the Index of FLASH address.    
             
    bSectionIndex = 0;	            
    
    while (bSectionIndex < bCutNum)
    {
        bSectionIndex++;
    		 
        if (_pwMapTab[wTabSection[bSectionIndex]] >= wChar)
        {			 
            for (wCharIndex = wTabSection[bSectionIndex-1]+1; wCharIndex <= wTabSection[bSectionIndex]; wCharIndex++)
            {
                if (_pwMapTab[wCharIndex] == wChar)
                    break;
            }
				
            if (wCharIndex == wTabSection[bSectionIndex]+1)
                wCharIndex  = 0xFFFF;    // not found
            else
                wCharIndex--;
    
            bSectionIndex = 0xFF;	// The character has been searched
        }
    }	
    
    if ((bSectionIndex != 0xFF) || (wCharIndex  == 0xFFFF)) // not found
    {
        wChar = (wChar % 26) + 0x41;  // Random map to A~Z
        for (wCharIndex = 1; wCharIndex <= _pwMapTab[0]; wCharIndex++)
        {
           if (_pwMapTab[wCharIndex] == wChar)
               break;
        }	
        wCharIndex--;		
    }
       
    return wCharIndex;             
}

WORD CONVCHAR_To_Uni(WORD wChar)
{
    BYTE	bCutNum = 30;	// Total number of separated mapping table
    WORD wTabSection[bCutNum+1];
    WORD wCharIndex = 0;
    BYTE bSectionIndex = 0;

    _pwMapTab = __wCurUniMapTab;

    // Separate table into several section, and just do search in one section.
    // It will make search more efficiently.
    for (bSectionIndex = 0; bSectionIndex < bCutNum; bSectionIndex++)
    {
        wTabSection[bSectionIndex] = (_pwMapTab[0] / bCutNum) * bSectionIndex;
    }
  
    wTabSection[bCutNum] = _pwMapTab[0] - 1;    
   
    // Translate A coding -> B coding    
         
    bSectionIndex = 0;	            

    while (bSectionIndex < bCutNum)
    {
        bSectionIndex++;
		 
        if (_pwMapTab[wTabSection[bSectionIndex]*2] >= wChar)
        {			 
            for (wCharIndex = wTabSection[bSectionIndex-1]+1; wCharIndex <= wTabSection[bSectionIndex]; wCharIndex++)
            {
                if (_pwMapTab[wCharIndex*2] == wChar)
                    break;
            }
            if (wCharIndex == wTabSection[bSectionIndex]+1)
                wCharIndex  = 0xFFFF;    // not found                        

            bSectionIndex = 0xFF;	// The character has been searched
        }
    }	

    if ((bSectionIndex != 0xFF) || (wCharIndex == 0xFFFF)) // not found      
        return ((wChar % 26) + 0x41);
    else
        return _pwMapTab[wCharIndex*2+1];     
}
#endif

