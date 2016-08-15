
#ifndef __OSDBMARK_H__
#define __OSDBMARK_H__

#ifdef SUPPORT_BOOKMARK 

#include "cc.h"


// #define     BOOKMARK_NO         20       // BookMark capability

////////////////          Public variables          /////////////////////////

BIT             __btBookMark;
BYTE            __bBookMark_Title[BOOKMARK_NO+ 1];  // record BookMark Title


////////////////          Public functions         /////////////////////////

void    OSDBOOKMARK_Exit(BYTE bExitMode);
BYTE    OSDBOOKMARK_ProcessKey(void);
BYTE    OSDBOOKMARK_Recover(BYTE bRecoverRegion);
void    OSDBOOKMARK_ConfigOSDRegion(void);


#endif //SUPPORT_BOOKMARK


#endif // __OSDBMARK_H__
