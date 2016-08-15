#ifndef __OS_SUB_H__
#define __OS_SUB_H__

#ifdef __cplusplus
        extern  "C"     {
#endif
// Must modify for different OS
// os_sub.h for eCos
#define	HANDLE_T	cyg_handle_t
#define	FLAG_T         cyg_flag_t
#define	MUTEX_T		cyg_mutex_t
#define	THREAD_SPACE	cyg_thread
#define MACRO_ECOS_VER( dwVer )     (dwVer = ecos_lib_ver);

#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/hal_arch.h>          // Architecture specific definitions
#define PACK_FORMAT __attribute__ ((packed))
#define PREFIX_INLINE   __inline
#ifdef __cplusplus
}
#endif //__cplusplus

#endif	//#ifndef __OS_SUB_H__ 
