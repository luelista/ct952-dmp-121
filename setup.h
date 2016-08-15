#include "winav.h"

#ifndef __OSD_H__
#include "osd.h"
#endif

#ifdef SUPPORT_STB
#ifdef IMAGE_FRAME_SETUP
#include "dvdsetup.h"
#else
#include "stb/usrif/dvdsetup.h"
#endif //IMAGE_FRAME_SETUP
#else
#ifdef IMAGE_FRAME_SETUP
#include "dvdsetup.h"
#else
#include "oldsetup.h"
#endif //IMAGE_FRAME_SETUP
#endif //#ifdef SUPPORT_STB
