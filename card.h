#ifndef __CARD_H__
#define __CARD_H__
#ifdef __cplusplus
    extern "C" {
#endif  // __cplusplus
// =================================================================================================
//#define CARD_AUTO_SWITCH_UARTDSU
// =================================================================================================
typedef enum CARD_CONTROLLER
{
    CARD_CONTROLLER_NONE,
    CARD_CONTROLLER_SD,
    CARD_CONTROLLER_MS,
    CARD_CONTROLLER_XD,
}
CARD_CONTROLLER, *PCARD_CONTROLLER;
// =================================================================================================
BOOL CARD_SetupSystem(DWORD dwDecoderSystem);
// -------------------------------------------------------------------------------------------------
BOOL CARD_InitSystem(VOID);
BOOL CARD_InitCard(VOID);
// -------------------------------------------------------------------------------------------------
CARD_CONTROLLER CARD_CardStatus_Inserted(VOID);
// -------------------------------------------------------------------------------------------------
BOOL CARD_SetBusWidth(BOOL b4BitMode);
// -------------------------------------------------------------------------------------------------
BOOL CARD_ReadSector(DWORD dwBuffer, DWORD dwSectorAddress, DWORD dwSectorCount);
BOOL CARD_WriteSector(DWORD dwBuffer, DWORD dwSectorAddress, DWORD dwSectorCount);
DWORD CARD_GetCapacity(VOID);
// =================================================================================================

#ifdef __cplusplus
    }
#endif  // __cplusplus
#endif  // __CARD_H__
