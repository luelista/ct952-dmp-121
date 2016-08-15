#ifndef __APLOADER_H__
#define __APLOADER_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

#ifndef __ROMLD__
#include "romld.h"
#endif

typedef struct  tagAP_INFO
{
    DWORD   dwIdentify[2];          // 01 ~ 08
    DWORD   dwAP_Type;              // 09 ~ 12
    DWORD   dwAP_Size;              // 13 ~ 16
    DWORD   dwExternalFlag;         // 17 ~ 20
    DWORD   dwVersionAP;            // 21 ~ 24
    DWORD   dwChipVersion;          // 25 ~ 28, 0: CT909; 1: CT909S
    DWORD   dwDescription[4];       // 29 ~ 44, the AP description
    DWORD   dwCheckSum;             // 45 ~ 48
    DWORD   dwAP_SP;                // 49 ~ 52
	DWORD   dwAP_UNZIP_BUF;         // 53 ~ 56
    DWORD   dwReserved[114];
} AP_INFO, *PAP_INFO;

typedef struct  tagAP_INFO_ADDITIONAL
{
    PSECTION_ENTRY   pTableAddress;         // 01 ~ 04
    DWORD   dwVersionFAE;                   // 05 ~ 08
    DWORD   dwVersionServo;                 // 09 ~ 12
    DWORD   dwPCB_Number;                   // 13 ~ 16
    DWORD   dwVersionFlash;                 // 17 ~ 20
    DWORD   dwVersionAP_INFO_ADDITIONAL;    // 21 ~ 24
    DWORD   dwIRType;                       // 25 ~ 28
    DWORD   dwTVMode;                       // 29 ~ 32
    DWORD   dwAP_Type;                      // 33 ~ 36
    DWORD   dwExternalFlag;                 // 37 ~ 40
    BYTE    bIRMap[256];                    // 41 ~ 296
    WORD    wAPSource;                      // 297 ~ 298, where is the AP source, ex USB/ Servo/ DRAM/ Flash
    WORD    wAP_EEPROM_Addr;                // 299 ~ 302, the EEPROM address of keep current AP mdoe
    BYTE    bIR_Customer_Code[2];           // 303 ~ 304
    BYTE    bIRMapSize;                     // 305
    BYTE    csVersionCustomer[24];
} AP_INFO_ADDITIONAL, *PAP_INFO_ADDITIONAL;


#define     CT909AP_IDENTIFY1   (0x43543930)     // "CT90"
#define     CT909AP_IDENTIFY2   (0x392D4150)     // "9-AP"

#define     AP_TABLE_ADDRESS    (0x40000800L)

#define     SRC_FROM_FLASH      (1)
#define     SRC_FROM_DRAM       (2)
#define     SRC_FROM_SERVO      (3)
#define     SRC_FROM_USB        (4)
#define     SRC_FROM_DSU        (5)

// Define the unique ID for AP usage
// Notice: reserve 10 item for internal usage, and customer solution AP ID will start from 11
#define     AP_AUTO_UPGRADE         1
#define     AP_LOGO                 2

#define     AP_CUSTOMER_DEFINE      11
#define     AP_GPS                  (AP_CUSTOMER_DEFINE + 0)
#define     AP_GAME                 (AP_CUSTOMER_DEFINE + 1)


extern AP_INFO     __AP_Info;
DWORD       AP_Loader(DWORD dwType, DWORD dwStartPos, DWORD dwSize);
PAP_INFO    AP_Identify(BYTE bSource, DWORD dwStartPos); // LLY1.50
BYTE        AP_SwitchMode(BYTE bAPID); // LLY2.00



/////////////////////////////////////////////////////////////////////
// Global Variable Area
/////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
        }
#endif

#endif
