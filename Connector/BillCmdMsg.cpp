#include "stdhdrs.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"

#if defined (USE_TENTER_BILLING)

#include "BillCmdMsg_TLD.inc"

#else // #if defined (USE_TENTER_BILLING)

#include "BillCmdMsg_KOR.inc"

#endif // #if defined (USE_TENTER_BILLING)
