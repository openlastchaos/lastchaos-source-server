#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"
#include "BillingClient.h"

#if defined (USE_TENTER_BILLING)
#include "BillingClient_TLD.inc"
#else
#include "BillingClient_KOR.inc"
#endif
