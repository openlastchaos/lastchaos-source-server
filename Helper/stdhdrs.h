#ifndef __STDHDRS_H__
#define __STDHDRS_H__

// 헬퍼 서버만 설정하는 거
#define __HELPER_SERVER__

#include "Conf.h"
#include "Config.h"
#include "Sysdep.h"

#ifdef  PROC_BILLING
#	undef PROC_BILLING
#else
#	define PROC_BILLING	0
#endif

#include "Utils.h"

#endif
