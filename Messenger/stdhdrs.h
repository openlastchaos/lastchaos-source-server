#ifndef __STDHDRS_H__
#define __STDHDRS_H__

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
