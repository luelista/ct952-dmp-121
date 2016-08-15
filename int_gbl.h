#ifndef __INT_GBL_H__
#define __INT_GBL_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

// define structure and type
typedef void (*PISRDSR_ROUTINE)( void );

typedef struct tagINT_PROC1_1ST_STATUS
{
    DWORD   dwPLAT_PROC1_1ST_INT_PENDING;
    DWORD   dwMCU_MCR6;
} INT_PROC1_1ST_STATUS, *PINT_PROC1_1ST_STATUS;

typedef struct tagINT_PROC1_2ND_STATUS
{
    DWORD   dwPLAT_PROC1_2ND_INT_PENDING;
    DWORD   dwBIU_BIUCTL;                   // biu_int (10:4)
    DWORD   dwMCU_MCR6;                     // bsrdint(10:5), abufint_overflow (10:11)
    DWORD   dwPLAT_IR_DATA;
    DWORD   dwPLAT_IR_RAW_CODE;
    
    BYTE    bSRV_DSPINTSR;                  // Andy: Servo DSP Interrupt Status
    BYTE    bSRV_DSPSLINTSR;              // Andy: Servo DSP Second Level Interrupt Status
    BYTE    bSRV_DSSPISR0;                  // Andy: Servo DSSP Interrupt Status 0
    BYTE    bSRV_DSSPISR1;                  // Andy: Servo DSSP Interrupt Status 1
} INT_PROC1_2ND_STATUS, *PINT_PROC1_2ND_STATUS;

typedef struct tagINT_PROC1_SW_STATUS
{
    DWORD   dwPLAT_PARAMETER3;
} INT_PROC1_SW_STATUS, *PINT_PROC1_SW_STATUS;


enum    tagINT_NUMBER
{
    INT_NO10 = 0,
    INT_NO13,
    MAX_INT_COUNT
};

#define MAX_2ND_INT_COUNT       (32)
#define CYGNUM_HAL_PRI_HIGH     (0)

// declare global reference functions
extern VOID    INT_Init( VOID );
extern VOID    INT_VSYNC_End(VOID);
extern VOID    ISR_BSRD_ABUFSaveClearStatus( VOID );
extern VOID    ISR_BIUSaveClearStatus( VOID );
extern VOID    ISR_IRSaveClearStatus( VOID );
extern VOID    ISR_VBUF_OF_SaveClearStatus( VOID );
extern VOID    ISR_VBUF_UF_SaveClearStatus( VOID );
extern VOID    ISR_16LSaveClearStatus( VOID );
extern VOID    ISR_DISPSaveClearStatus( VOID );
extern VOID    ISR_SRVSaveClearStatus( VOID ); // brian0.67

// declare global reference variables
extern INT_PROC1_1ST_STATUS     __INTISRintProc11stStatus;
extern INT_PROC1_2ND_STATUS     __INTISRintProc12ndStatus;

extern DWORD   __dwINTISRCounter[MAX_INT_COUNT], __dwINTDSRCounter[MAX_INT_COUNT];
extern PISRDSR_ROUTINE     __pINTISRRoutine[MAX_INT_COUNT][MAX_2ND_INT_COUNT];
extern PISRDSR_ROUTINE     __pINTDSRRoutine[MAX_INT_COUNT][MAX_2ND_INT_COUNT];

#ifdef __cplusplus
        }
#endif

#endif  // __INT_GBL_H__
