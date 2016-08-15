#ifndef __HIO_H__
#define __HIO_H__

#ifdef __cplusplus
   extern "C"{
#endif
#define SUPPORT_I2C
typedef struct tagTRANSFER_CONFIG
{
    BYTE bAddr;				// the device address for data transfer
    BYTE bCount;				// Bytes count for data transfer
    BYTE *pbBuffer;			// Buffer pointer for data transfer
}TRANSFER_CONFIG, *PTRANSFER_CONFIG;

#ifdef  SUPPORT_I2C

// LLY2.01, define I2C R/W prototype configure structure
typedef struct tagI2C_CONFIG {
    BYTE bIOGroup;
    BYTE bSDAT_Pin;
    BYTE bSCLK_Pin;
} I2C_CONFIG, * PI2C_CONFIG;

BOOL 	HAL_I2C_Read (BYTE DevSel, PI2C_CONFIG  pConfig, PTRANSFER_CONFIG pTransferConfig);
BOOL 	HAL_I2C_Write (BYTE DevSel, PI2C_CONFIG  pConfig, PTRANSFER_CONFIG pTransferConfig );
#endif  // #ifdef SUPPORT_I2C
//# [21]GPIO relative function
BYTE    HAL_ReadGPIO(BYTE bGroup, DWORD dwPort); //
void    HAL_WriteGPIO(BYTE bGroup, DWORD dwPort, BYTE bValue); //
BYTE    HAL_ReadGPO_Value(BYTE bGroup, DWORD dwPort); // LLY2.51, report desired GPO value.
void    HAL_Init_ExpGPIO(void); // LLY2.13, add for expand GPIO initial
BOOL    HAL_I2CSrc_Ctrl(BYTE bAction);  // LLY2.18
DWORD   HAL_Read_ExpGPI(void); // LLY2.20



// LLY2.01, define the GPIO group for HAL_ReadGPIO()/ HAL_WriteGPIO()
// LLY2.13, remove the GPIO group define to "winav.h"
// Otherwise, compiler will fail for pio.h
/*
#define GPIO_A      0
#define GPIO_B      1
#define GPIO_C      2
#define GPIO_D      3
#define GPIO_E      4
#define GPIO_F      5
#define GPIO_EXP    6   // LLY2.13, add for expand GPIO
*/

void    HAL_CopyDRAM(DWORD dwSrc, DWORD dwDst, DWORD dwLen);
void    HAL_DumpUserData( BYTE* pbBuf, DWORD dwLen, DWORD dwPosition );

// SYuan2.77 begin,
// Modify following code to fix DSU/ UART new design
// expand BYTE to DWORD unit 
//#define HAL_UART1            (0L)
//#define HAL_UART2            (1L)

// LLY2.10, define UART Baud Rate
#define BAUDRATE_115200     115200
VOID    HAL_UART_BaudRateSet(DWORD dwSysCLK, DWORD dwBaudRate, DWORD dwPort);
VOID    HAL_UART_Disable(DWORD dwPort);
VOID    HAL_UART_Enable(DWORD dwPort, BYTE bPath);
BYTE    HAL_UART_ErrorStatus(DWORD dwPort);
BOOL    HAL_UART_ReceiveChar(PBYTE pData, DWORD dwPort);
BOOL    HAL_UART_SendChar(BYTE bData, DWORD dwPort);

// LLY2.15, new API for DSU pin control
VOID    HAL_DSU_Pin_Enable(DWORD dwPort, BYTE bPath);
VOID    HAL_DSU_Pin_Disable(DWORD dwPort);
VOID    HAL_DSU_BaudRateSet(DWORD dwSysCLK, DWORD dwBaudRate, DWORD dwPort);

//- Expand BYTE to DWORD unit
//- Bit[31:24]: used to specify the DSU group, ex DSU1/ DSU2/ ¡K
//- Bit[23:8]: used to specify the UART group, ex UART1/ UART2/ ...
//- Bit[7:0]: used to specify the TX or RX 
//So, we can get following new define ID.
// [DSU group]
#define		HAL_DSU1		(0x1L <<  24)
#define		HAL_DSU2		(0x2L <<  24)

// [UART group]
#define		HAL_UART1		(0x1L << 8)
#define		HAL_UART2		(0x2L << 8)

// [TX/ RX choice]
#define		HAL_TX			(0x1L)
#define		HAL_RX			(0x2L)

// [Total Choice]
#define		HAL_DSU1_TX		(HAL_DSU1 | HAL_TX)
#define		HAL_DSU1_RX		(HAL_DSU1 | HAL_RX)
#define		HAL_DSU2_TX		(HAL_DSU2 | HAL_TX)
#define		HAL_DSU2_RX		(HAL_DSU2 | HAL_RX)
#define 	HAL_UART1_TX	(HAL_UART1 | HAL_TX)
#define		HAL_UART1_RX	(HAL_UART1 | HAL_RX)
#define		HAL_UART2_TX	(HAL_UART2 | HAL_TX)
#define		HAL_UART2_RX	(HAL_UART2 | HAL_RX)

// [Get UART port ; ex: UART1 or UART2]
#define		GET_UART_PORT(x)	   ((x) & 0x0000ff00) >> 8

// [Get DSU port : ex: DSU1 or DSU2]
#define      GET_DSU_PORT(x)      ((x) & 0xff000000)
// SYuan2.77 end ...

// LLY2.16, porting Aron's code to get NIM module I2C configure pin
#ifdef SUPPORT_STB
VOID    HAL_NIM_GetI2CConfig (PI2C_CONFIG pI2C_Config);
#endif // #ifdef SUPPORT_STB



// LLY2.18, define the ID for HAL_I2CSrc_Ctr() 1st parameter
#define MUTEX_ACTION_INIT       0
#define MUTEX_ACTION_LOCK       1
#define MUTEX_ACTION_UNLOCK     2
#define MUTEX_ACTION_TRYLOCK    3

#ifdef  SW_EXPAND_GPIO
// LLY2.16, define the macro for S/W expand GPIO usage
// Clock Signal High/ Low control
#define _KS_CLK_HI        {HAL_WriteGPIO(KS_CLK_GRP, GPIO_KS_CLK, 1);}
#define _KS_CLK_LO        {HAL_WriteGPIO(KS_CLK_GRP, GPIO_KS_CLK, 0);}

// Data Signal High/ Low control
#define _KS_DAT_HI        {HAL_WriteGPIO(KS_DATA_GRP, GPIO_KS_DATA, 1);}
#define _KS_DAT_LO        {HAL_WriteGPIO(KS_DATA_GRP, GPIO_KS_DATA, 0);}

// Read-back Key Detect 0/ 1 value
#define _KS_DET0_READ     (HAL_ReadGPIO(KS_DET0_GRP, GPIO_KS_DET0)?1:0)
#define _KS_DET1_READ     (HAL_ReadGPIO(KS_DET1_GRP, GPIO_KS_DET1)?1:0)

// Latch 0/1 Signal High/ Low control
#define _KS_LATCH0_HI     {HAL_WriteGPIO(KS_LATCH0_GRP, GPIO_KS_LATCH0, 1);}
#define _KS_LATCH0_LO     {HAL_WriteGPIO(KS_LATCH0_GRP, GPIO_KS_LATCH0, 0);}
#define _KS_LATCH1_HI     {HAL_WriteGPIO(KS_LATCH1_GRP, GPIO_KS_LATCH1, 1);}
#define _KS_LATCH1_LO     {HAL_WriteGPIO(KS_LATCH1_GRP, GPIO_KS_LATCH1, 0);}
#endif  // #ifdef  SW_EXPAND_GPIO

#ifdef __cplusplus
   }
#endif

#endif  // __HIO_H__



