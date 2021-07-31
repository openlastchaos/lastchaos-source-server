#include "stdhdrs.h"
#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"

void ProcDead(CElemental* df, CCharacter* of)
{

	CPC*		opc				= NULL;
	CNPC*		onpc			= NULL;
#ifdef ENABLE_PET
	CPet*		opet			= NULL;
#endif // #ifdef ENABLE_PET
	CElemental*	oelemental		= NULL;

	switch (of->m_type)
	{
	case MSG_CHAR_PC:
		opc = TO_PC(of);
		break;

	case MSG_CHAR_NPC:
		onpc = TO_NPC(of);
		break;

	case MSG_CHAR_PET:
		opet = TO_PET(of);
		opc = opet->GetOwner();
		break;

	case MSG_CHAR_ELEMENTAL:
		oelemental = TO_ELEMENTAL(of);
		opc = oelemental->GetOwner();
		break;

	default:
		return ;
	}

	DelAttackList(df);

	CNetMsg rmsg;

	CPC* owner = df->GetOwner();
	if (owner)
		owner->UnsummonElemental(df);
}
