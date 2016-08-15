#ifndef __OSWRAP_H__
#define __OSWRAP_H__

#ifdef __cplusplus
        extern  "C"     {
#endif
#include	"os_sub.h"

typedef struct tagTHREAD_INFO
{
	DWORD		priority;	// priority value
	void		*fpThreadEntry;	// Thread Entry
	DWORD 	dwEntryData;	// data value pass to the thread entry routine
	char		*ThreadName;// thread name string	
	void		*StackBase;	// Base address of stack
	DWORD		StackSize;	// Stack bytes size
	HANDLE_T	handle;	// Store the returned handle value
	THREAD_SPACE	space;		// Thread information is stored in the thread memory object pointed by this parameter
} THREAD_INFO, *LPTHREAD_INFO;

typedef struct  tagALARM_INFO
{
	void		*fpAlarmFunction;	// Pointer to an alarm handler
	WORD	wEntryData;	// data value pass to the alarm handler
	DWORD	dwInterval;		// the interval for alarm to coour
} ALARM_INFO, *LPALARM_INFO;
extern HANDLE_T    __ClockHandle;  // must remove main flow declaration when porting OK

//---------------------------------------------------------------------------
// MACROs for Interrupt/ Code Generation/ ...

// Interrupt control macros
// Diable interrupt (raise current interrupt level to highest, i.e. 15) and keep current
//   status (PSR) at 32-bit variable _old_
#define OS_DISABLE_INTERRUPTS(_old_)    HAL_DISABLE_INTERRUPTS(_old_)

// Modify PSR to enable interrupt
#define OS_ENABLE_INTERRUPTS()          HAL_ENABLE_INTERRUPTS()

// Restore interrupt status (PSR) saved at 32-bit variable _old_
#define OS_RESTORE_INTERRUPTS(_old_)    HAL_RESTORE_INTERRUPTS(_old_)

// Get current interrupt status (PSR) and saved at 32-bit variable _old_
#define OS_QUERY_INTERRUPTS(_old_)      HAL_QUERY_INTERRUPTS(_old_)

// Execution reorder barrier.
// When optimizing the compiler can reorder code. In multithreaded systems
// where the order of actions is vital, this can sometimes cause problems.
// This macro may be inserted into places where reordering should not happen.

#define OS_REORDER_BARRIER()            HAL_REORDER_BARRIER()


//---------------------------------------------------------------------------
// Mutex APIs
//void cyg_mutex_init (cyg_mutex_t *mutex); 
void	OS_InitMutex( MUTEX_T *Mutex);

//BOOL cyg_mutex_lock (cyg_mutex_t *mutex);
BOOL	OS_LockMutex( MUTEX_T	*Mutex);

//void cyg_mutex_unlock (cyg_mutex_t *mutex);
void	OS_UnlockMutex( MUTEX_T	*mutex);

//---------------------------------------------------------------------------
// FLAGS
//void cyg_flag_init (cyg_flag_t *flag);
void	OS_InitFlag( FLAG_T	*Flag);
 
//void cyg_flag_destroy (cyg_flag_t *flag);
void	OS_DestroyFlag( FLAG_T	*Flag);

//void cyg_flag_setbits (cyg_flag_t *flag, DWORD dwValue);
void	OS_SetFlag( FLAG_T	*Flag, DWORD	dwValue);

//void cyg_flag_maskbits (cyg_flag_t *flag, DWORD dwValue);
void	OS_ClearFlag( FLAG_T	*Flag, DWORD	dwValue);

//DWORD cyg_flag_peek (cyg_flag_t *flag);
DWORD	OS_PeekFlag( FLAG_T	*Flag);

#define	FLAG_WAITMODE_AND	0	// all bits must be set
#define FLAG_WAITMODE_OR	2	// any bit must be set
#define FLAG_WAITMODE_CLR	1	// clear when satisfied

DWORD	OS_TimedWaitFlag(
	FLAG_T	*Flag,	// pointer to a flag object
	DWORD dwPattern,			// bit setting that will cause the calling thread to be woken up
	BYTE bMode,					
	DWORD dwTimeCount		// absolute time to wait
);
// return 0 if timeout first, otherwise return the flag value

//---------------------------------------------------------------------------
// THREAD operation
void	OS_YieldThread(void);                                               //ok
void	OS_SuspendThread(HANDLE_T Handle);                  //ok
void	OS_KillThread(HANDLE_T Handle);
void	OS_ResumeThread(HANDLE_T Handle);                   //ok

void	OS_CreateThread(LPTHREAD_INFO lpThreadInfo);    //ok

BOOL	OS_Identify_Version(void);
void	OS_InitialTimer(void);
DWORD   OS_GetSysTimer(void);
ULONGLONG OS_GetSysTimer64(void);
void	OS_DelayTime(DWORD dwTimeCount);
void	OS_InitialAlarm(LPALARM_INFO lpAlarmInfo);
DWORD	OS_WaitFlag(
	FLAG_T	*Flag,			// pointer to a flag object
	DWORD dwPattern,			// bit setting that will cause the calling thread to be woken up
	BYTE bMode				
);
void	OS_DestroyMutex(MUTEX_T	*Mutex )	;		
BOOL	OS_TryLockMutex(MUTEX_T	*Mutex );		
void	OS_DisableAlarm(HANDLE_T Alarm);
void	OS_EnableAlarm(HANDLE_T Alarm);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif	//#ifndef __OSWRAP_H__ 


