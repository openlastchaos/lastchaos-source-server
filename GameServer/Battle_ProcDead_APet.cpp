#include "stdhdrs.h"
#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "Log.h"
#include "doFunc.h"

#ifdef ATTACK_PET 
void ProcDead(CAPet* df, CCharacter* of)
{
	
	CPC*		opc				= NULL;
	CNPC*		onpc			= NULL;
#ifdef ENABLE_PET
	CPet*		opet			= NULL;
#endif // #ifdef ENABLE_PET
	CElemental*	oelemental		= NULL;
	CAPet*		oapet			= NULL;

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

	case MSG_CHAR_APET:
		oapet = TO_APET( of );
		opc = oapet->GetOwner();
		break;

	default:
		return ;
	}
	
	bool bPKPenalty = false;

	if (df->GetOwner())
	{
		bPKPenalty = (opc) ? IsPK(opc, df) : false;
		if (bPKPenalty)
			CalcPKPoint(opc, df->GetOwner(), true);
	}

	if( !bPKPenalty )
	{
		if( df->m_level >= 10  )
		{
			df->m_exp = (LONGLONG) ( df->m_exp - ( df->GetNeedExp() * 0.02) ) ;
			if( df->m_exp < 0 )
				df->m_exp=0;
		}

		df->AddFaith(-10);
	}
	DelAttackList(df);
	
	CNetMsg rmsg;

	CPC* owner = df->GetOwner();
	const char* ownerName = "NO OWNER";
	const char* ownerNick = "NO OWNER";
	const char* ownerID = "NO OWNER";
	if (owner)
	{
		ownerNick = (owner->IsNick()) ? owner->GetName() : ownerNick;
		ownerName = owner->m_name;
		ownerID = owner->m_desc->m_idname;
	}

	// TODO : petlog
	GAMELOG << init("PET DEAD")
			<< "APET" << delim
			<< "INDEX" << delim
			<< df->m_index << delim
			<< "LEVEL" << delim
			<< df->m_level << delim
			<< "OWNER" << delim
			<< ownerName << delim
			<< ownerNick << delim
			<< ownerID << delim
			<< "ATTACKER" << delim
			<< "TYPE" << delim;
	switch (of->m_type)
	{
	case MSG_CHAR_NPC:
		GAMELOG << "NPC" << delim
				<< onpc->m_name
				<< end;
		break;
	case MSG_CHAR_PC:
	case MSG_CHAR_PET:
	case MSG_CHAR_ELEMENTAL:
	case MSG_CHAR_APET:
	default:
		if (opc)
		{
			GAMELOG << "PC" << delim
					<< opc->m_index << delim
					<< opc->GetName()
					<< end;
		}
		else
		{
			GAMELOG << "UNKNOWN" << delim
					<< of->m_index << delim
					<< of->m_name
					<< end;
		}
		break;
	}

	if (owner)
	{
		// Item şŔŔÎ
		CItem* apet_item = owner->m_wearing[WEARING_PET];
		apet_item->m_flag |= FLAG_ITEM_SEALED;
		ItemUpdateMsg(rmsg, apet_item, 0);
		SEND_Q(rmsg, owner->m_desc);

		// Âřżë ÇŘÁ¦
		ItemWearMsg(rmsg, WEARING_PET, NULL, NULL);
		do_ItemWear(owner, rmsg);
		// Ćę »óĹÂ ş¸łż

		ExAPetStatusMsg(rmsg, df);
		SEND_Q(rmsg, owner->m_desc);
	}
}

#endif //ATTACK_PET