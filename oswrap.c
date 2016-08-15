// oswrap.c for eCos
#include <cyg/kernel/kapi.h>
#include "winav.h"
#include "oswrap.h"
//HANDLE_T    TimerHandle;
HANDLE_T    __ClockHandle;
// !!!must declare as global, as OS will reference the object.
cyg_handle_t  AlarmHandle;
cyg_alarm      AlarmObj;
//  **************************************************************************
//  Function    :   OS_InitMutex
//  Description :   Tell OS to initialize a mutex
//  Arguments   :   Mutex,	pointer to a mutex object
//  Return      :   None
//  **************************************************************************
void	OS_InitMutex( MUTEX_T *Mutex)
{
	cyg_mutex_init (Mutex);
}
//  **************************************************************************
//  Function    :   OS_LockMutex
//  Description :   Change the state of a mutex to locked state
//  Arguments   :   Mutex,	pointer to a mutex object
//  Return      :   TRUE,	mutex has been locked
//					FALSE,	mutex has not been locked
//  **************************************************************************
BOOL	OS_LockMutex( MUTEX_T	*Mutex)
{
	return (BOOL)cyg_mutex_lock (Mutex);
}
//  **************************************************************************
//  Function    :   OS_LockMutex
//  Description :   Change the state of a mutex to unlocked state
//  Arguments   :   Mutex,	pointer to a mutex object
//  Return      :   none
//  **************************************************************************
void	OS_UnlockMutex( MUTEX_T	*Mutex)
{
	cyg_mutex_unlock (Mutex);
}
//  **************************************************************************
//  Function    :   OS_InitFlag
//  Description :   Initialize a flag variable
//  Arguments   :   Flag,	pointer to a flag object
//  Return      :   None
//  **************************************************************************
void	OS_InitFlag( FLAG_T	*Flag)
{
	cyg_flag_init (Flag);
}
//  **************************************************************************
//  Function    :   OS_DestroyFlag
//  Description :   Destroy the specified flag variable
//  Arguments   :   Flag,	pointer to a flag object
//  Return      :   None
//  **************************************************************************
void	OS_DestroyFlag( FLAG_T	*Flag)
{
	cyg_flag_destroy (Flag);
}
//  **************************************************************************
//  Function    :   OS_SetFlag
//  Description :   Set the bits in the flag value that are set to one in the dwValue parameter
//  Arguments   :   Flag,	pointer to a flag object
//					dwValue,	specified bits are set to one are to be set to the flag
//  Return      :   None
//  **************************************************************************
void	OS_SetFlag( FLAG_T	*Flag, DWORD	dwValue)
{
	cyg_flag_setbits (Flag, (cyg_flag_value_t)dwValue);
}
//  **************************************************************************
//  Function    :   OS_ClearFlag
//  Description :   Clear the bits in the flag value that are set to zero in the dwValue parameter
//  Arguments   :   Flag,	pointer to a flag object
//					dwValue,	the bits that are set to zero in this parameter are cleared in the flag
//  Return      :   None
//  **************************************************************************
void	OS_ClearFlag( FLAG_T	*Flag, DWORD	dwValue)
{
	cyg_flag_maskbits (Flag, (cyg_flag_value_t)dwValue);
}
//  **************************************************************************
//  Function    :   OS_PeekFlag
//  Description :   Return the current value of the specified flag
//  Arguments   :   Flag,	pointer to a flag object
//  Return      :   Current value of specified flag
//  **************************************************************************
DWORD	OS_PeekFlag( FLAG_T	*Flag)
{
	return (DWORD)cyg_flag_peek (Flag);
}
//  **************************************************************************
//  Function    :   OS_TimedWaitFlag
//  Description :   Wait for the conditions required by the dwPattern and
//					bMode parameters or the timeout specified by the dwTimeCount parameter
//  Arguments   :   Flag,	pointer to a flag object
//					dwPattern,	bit setting that will cause the calling thread to be woken up
//					bMode,		modifies the conditions for wake up
//					dwTimeCount,	absolute time to wait for the flag conditions to be set
//  Return      :   0,	The timeout occurs before the conditions are met
//					Flag value,	condition are met 
//  **************************************************************************
DWORD	OS_TimedWaitFlag(
	FLAG_T	*Flag,			// pointer to a flag object
	DWORD dwPattern,			// bit setting that will cause the calling thread to be woken up
	BYTE bMode,					
	DWORD dwTimeCount		// absolute time to wait
)
{
	BYTE	bCondition =0;
	if (bMode & FLAG_WAITMODE_CLR)
	{
		bCondition |= CYG_FLAG_WAITMODE_CLR;
	}
	if (bMode & FLAG_WAITMODE_AND)	
	{
		bCondition |= CYG_FLAG_WAITMODE_AND;
	}
	if (bMode & FLAG_WAITMODE_OR)	
	{
		bCondition |= CYG_FLAG_WAITMODE_OR;
	}
	return (DWORD)cyg_flag_timed_wait(Flag, (cyg_flag_value_t)dwPattern, (cyg_flag_mode_t)bCondition, cyg_current_time()+(cyg_tick_count_t)dwTimeCount);
}
//  **************************************************************************
//  Function    :   OS_YieldThread
//  Description :   Gives execution control to the thread that is ready to run at the same priority level
//  Arguments   :   None
//  Return      :   None
//  **************************************************************************
void	OS_YieldThread(void)
{
	cyg_thread_yield();
}
//  **************************************************************************
//  Function    :   OS_SuspendThread
//  Description :   Postpones the execution of a thread
//  Arguments   :   thread,	handle to the thread
//  Return      :   None
//  **************************************************************************
void	OS_SuspendThread(HANDLE_T Thread)
{
	cyg_thread_suspend(Thread);
}
//  **************************************************************************
//  Function    :   OS_killThread
//  Description :   Causes a thread to exit
//  Arguments   :   thread,	handle to the thread
//  Return      :   None
//  **************************************************************************
void	OS_KillThread(HANDLE_T Thread)
{
        // wyc2.35a,
        cyg_thread_release(Thread);
	cyg_thread_kill(Thread);
}
//  **************************************************************************
//  Function    :   OS_ResumeThread
//  Description :   Causes a thread to continue execution
//  Arguments   :   thread,	handle to the thread
//  Return      :   None
//  **************************************************************************
void	OS_ResumeThread(HANDLE_T Thread)
{
	cyg_thread_resume(Thread);
}
//  **************************************************************************
//  Function    :   OS_Thread_Create
//  Description :   Create a thread in a suspend state
//  Arguments   :   lpThreadInfo,	pointer to a memory object that store thread info
//  Return      :   None
//  **************************************************************************
void	OS_CreateThread(LPTHREAD_INFO lpThreadInfo)
{
	cyg_thread_create((cyg_addrword_t)(*lpThreadInfo).priority, (*lpThreadInfo).fpThreadEntry,
       (cyg_addrword_t) (*lpThreadInfo).dwEntryData, (*lpThreadInfo).ThreadName,
	(*lpThreadInfo).StackBase, (cyg_ucount32) (*lpThreadInfo).StackSize,
	&((*lpThreadInfo).handle), &((*lpThreadInfo).space));

}
//  ***********************************************************************
//  Function    :   OS_Identify_Version
//  Description :   Check the OS package version
//  Argument    :   None
//  Return      :   TRUE,	valid version
//					FALSE,	invalid version
//  ***********************************************************************
BOOL	OS_Identify_Version(void)
{
	DWORD	dwINITIALECOSVer;
	MACRO_ECOS_VER(dwINITIALECOSVer);
	if (LOWORD(dwINITIALECOSVer) < ECOS_PACKAGE_VERSION)
	{
		printf("\nECOS Version Error, %lx!!",dwINITIALECOSVer);
		return FALSE;
	}
	return TRUE;
}

//  **************************************************************************
//  Function    :   OS_InitialAlarm
//  Description :   Initialize an alarm and enable it.
//  Arguments   :  lpAlarmInfo, pointer to a memory object that store alarm info
//  Return      :   None
//  **************************************************************************
void	OS_InitialAlarm(LPALARM_INFO lpAlarmInfo)
{
    cyg_alarm_create(__ClockHandle, (*lpAlarmInfo).fpAlarmFunction,
             (cyg_addrword_t) (*lpAlarmInfo).wEntryData,
             &AlarmHandle, &AlarmObj);
    // let the alarm start working immediately, use cyg_current_time will return real-time clock counter value in ticks
    // so the cyg_current_time()+1 will match after one tick count.
    cyg_alarm_initialize(AlarmHandle, cyg_current_time()+1, (*lpAlarmInfo).dwInterval);
}

//  **************************************************************************
//  Function    :   OS_WaitFlag
//  Description :   Wait for the conditions required by the dwPattern and
//					bMode parameters
//  Arguments   :   Flag,	pointer to a flag object
//					dwPattern,	bit setting that will cause the calling thread to be woken up
//					bMode,		modifies the conditions for wake up
//					dwTimeCount,	absolute time to wait for the flag conditions to be set
//  Return      :   0,	The thread is forcibly woken up or forced to exit.
//					Flag value,	condition are met 
//  **************************************************************************
DWORD	OS_WaitFlag(
	FLAG_T	*Flag,			// pointer to a flag object
	DWORD dwPattern,			// bit setting that will cause the calling thread to be woken up
	BYTE bMode					
)
{
	BYTE	bCondition =0;
	if (bMode & FLAG_WAITMODE_CLR)
	{
		bCondition |= CYG_FLAG_WAITMODE_CLR;
	}
	if (bMode & FLAG_WAITMODE_AND)	
	{
		bCondition |= CYG_FLAG_WAITMODE_AND;
	}
	if (bMode & FLAG_WAITMODE_OR)	
	{
		bCondition |= CYG_FLAG_WAITMODE_OR;
	}
	return (DWORD)cyg_flag_wait(Flag, (cyg_flag_value_t)dwPattern, (cyg_flag_mode_t)bCondition);
}
//  **************************************************************************
//  Function    :   OS_TryLockMutex
//  Description :   Attempts to change the state of a mutex to the locked state
//  Arguments   :   Mutex,	pointer to a mutex object
//  Return      :   TRUE,   the mutex has been locked
//                      FALSE, the mutex has not been locked
//  **************************************************************************
BOOL	OS_TryLockMutex(MUTEX_T	*Mutex )			
{
    return (BOOL)cyg_mutex_trylock(Mutex);
}
//  **************************************************************************
//  Function    :   OS_DestroyMutex
//  Description :   Destroy a mutex
//  Arguments   :   Mutex,	pointer to a mutex object
//  Return      :   None
//  **************************************************************************
void	OS_DestroyMutex(MUTEX_T	*Mutex )			
{
    cyg_mutex_destroy(Mutex);
}

//  **************************************************************************
//  Function    :   OS_InitialTimer
//  Description :   Initialize a timer
//  Arguments   :   None
//  Return      :   None
//  **************************************************************************
void	OS_InitialTimer(void)
{
	cyg_handle_t    RTC;
	RTC = cyg_real_time_clock();
    cyg_clock_to_counter(RTC, &__ClockHandle);
}
//  **************************************************************************
//  Function    :   UTL_GetSysTimer
//  Description :   Get System timer
//  Arguments   :   None
//  Return      :   Current system timer (DWORD unit)
//  **************************************************************************
DWORD    OS_GetSysTimer(void)
{
	DWORD	dwSysTime;
	dwSysTime = (DWORD) cyg_counter_current_value(__ClockHandle);
    // Chuan1.07, System tick is a multiple of 2. We need divide by 2 to obtain increase number.
    // LLY2.21, porting Chuan's code since slice tick maybe different
    dwSysTime /= SLICE_TICK;
	return dwSysTime;
}

//  **************************************************************************
//  Function    :   OS_GetSysTimer64
//  Description :   Get System timer
//  Arguments   :   None
//  Return      :   Current system timer (ULONGLONG unit)
//  **************************************************************************
ULONGLONG OS_GetSysTimer64(void)
{
    ULONGLONG u64SysTime;
    u64SysTime = cyg_counter_current_value(__ClockHandle);
    u64SysTime /= SLICE_TICK;
    return u64SysTime;
}

//  *********************************************************************
//  Function    :   UTL_DelayTime
//  Description :   Delay until time out
//  Arguments   :   dwCount  : the delay time
//  Return      :   None
//  Side Effect :
//  *********************************************************************
// LLY0.63, expand delay time unit from WORD to DWORD
// Because, COUNT_XXX_SEC is WORD unit, and the add operation maybe larger than 65535
// LLY1.15, modify the API define: remove 2nd parameter and don't return value
//BIT   UTL_DelayTime( DWORD dwCount, BIT btBreak )
void OS_DelayTime(DWORD dwTimeCount)
{
DWORD   dwTimeStart;
    dwTimeStart = OS_GetSysTimer();
    while ( ( OS_GetSysTimer()- dwTimeStart ) < dwTimeCount )
    {
        OS_YieldThread();
    }
}

//  **************************************************************************
//  Function    :   OS_DisableAlarm
//  Description :   Disable an alarm 
//  Arguments   :  Alarm,	handle to the alarm
//  Return      :   None
//  **************************************************************************
void	OS_DisableAlarm(HANDLE_T Alarm)
{
    cyg_alarm_disable(Alarm);
}

//  **************************************************************************
//  Function    :   OS_EnableAlarm
//  Description :   Enable an alarm 
//  Arguments   :  Alarm,	handle to the alarm
//  Return      :   None
//  **************************************************************************
void	OS_EnableAlarm(HANDLE_T Alarm)
{
    cyg_alarm_enable(Alarm);
}