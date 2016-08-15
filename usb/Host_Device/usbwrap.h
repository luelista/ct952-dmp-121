

#define USB_STATUS_UNKNOWN              0
#define USB_STATUS_NO_DEVICE            1
#define USB_STATUS_NOT_SUPPORTED_DEVICE     2
#define USB_STATUS_MASS_STORAGE_DEVICE  3
#define USB_STATUS_NEW_DEVICE_ATTACH  4
#define USB_STATUS_NO_DISC  5
// wyc2.38-909s, report the card status
#define USB_STATUS_CARD_ABNORMAL    6


// USB_CheckCard value
// wyc2.38-909s, report the card status
#define USB_CHECK_NO_CARD           0
#define USB_CHECK_HAS_CARD          1
#define USB_CHECK_CARD_ABNORMAL     2
#define USB_CHECK_CARD_NOT_READY    3
// wyc2.51-909P,
#define USB_CHECK_TIME_OUT          4

#define SMAPLE_STACK_SIZE 65536 //32768 //16384

#define REG_PLAT_GPG_MUX_SELECT					 (*((volatile DWORD *) (0x80004074)))

// Can't change the value because it must sync with USB_OP_MODE_HOST/USB_OP_MODE_DEVICE/USB_OP_MODE_HOST_DEVICE.
#define USB_MODE_HOST           0
#define USB_MODE_DEVICE         1
#define USB_MODE_HOST_DEVICE    2 // the idle mode: for OTG case. For now, we should NOT set to this mode.
#define USB_MODE_EXIT_MODE      3
#define USB_MODE_NONE           0xFF


void USB_HCInit(BYTE bUSBMode, BYTE bPort, DWORD dwUSBBufferAddr, DWORD dwSize);
void USB_HCExit(void);

BYTE USB_CheckConnect(void);
// wyc.070515
BYTE USB_GetOTGID(void);

BYTE USB_FindDevice(void);

DWORD USB_CheckStatus(void);

void USB_ClearStatus(void);

BYTE USB_CheckCard(BYTE bLUN);

BYTE USB_ReadData(DWORD dwStartSector, DWORD dwNumOfSector, DWORD dwBufferAddr);
BYTE USB_WriteData(DWORD dwStartSector, DWORD dwNumOfSector, DWORD dwBufferAddr);
DWORD USB_GetUSBStatus(void);
// wyc.070517
void USB_ConfigDeviceMemPool(DWORD BufferAddr, DWORD BufferSize);
BYTE USB_ConnectToPC(void);


