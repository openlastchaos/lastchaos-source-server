
// 신규 공격형 펫 기본 DB 정보
#include "stdhdrs.h"
#include "Log.h"
#include "APetProto.h"
#include "Server.h"
#include "DBCmd.h"

#ifdef ATTACK_PET

CAPetProto::CAPetProto():m_strName(20)
{
	InitValue();
}

CAPetProto::~CAPetProto()
{

}

void CAPetProto::InitValue()
{
	m_nIndex = m_Type = -1;
	m_nStr = m_nCon = m_nDex = m_nInt = 0;
	m_nMaxFaith = m_nMaxStm = 0;
	m_nMaxHP = m_nMaxMP = 0;
	m_nRecoverHP = m_nRecoverMP = 0;
	m_nSpeed=0;
	m_nAISlot=0;
	m_nAttack=m_nDefence=m_nMagicAttack=m_nMagicDefence=0;
	m_nHitPoint=m_nAvoidPoint=m_nMagicAvoidPoint=m_nAttackSpeed=1;
	m_nDeadly=m_nCritical=m_nAwful=m_nStrong=m_nNormal=m_nWeek=1;
}


//////////////////////////////////////////////////////////////////////////
CAPetProtoList::CAPetProtoList()
{
	m_pApetProtoList = NULL;
	m_nCount =0;
	m_virtualIndex =1;
}

CAPetProtoList::~CAPetProtoList()
{
	if( m_pApetProtoList )
		delete[] m_pApetProtoList;
	m_pApetProtoList = NULL;
	m_nCount =0;
	m_virtualIndex =1;
}

bool CAPetProtoList::Load()
{
	if (m_pApetProtoList)
		delete [] m_pApetProtoList;
	m_pApetProtoList = NULL;
	m_nCount = 0;
	m_virtualIndex = 1;

	CDBCmd cmdAPet;
	cmdAPet.Init(&gserver.m_dbdata);
	strcpy(g_buf, "SELECT * FROM t_bigpet WHERE a_enable=1 ORDER BY a_index");
	cmdAPet.SetQuery(g_buf);
	if( !cmdAPet.Open() || !cmdAPet.MoveFirst() )
		return false;

	m_nCount = cmdAPet.m_nrecords;
	m_pApetProtoList = new CAPetProto[m_nCount];

	int i = 0;
	do 
	{
		int idx=-1;
		if( i > m_nCount )
			return false;
		// 기본
		cmdAPet.GetRec("a_index",idx);
		m_pApetProtoList[i].Index(idx);
		cmdAPet.GetRec("a_type",m_pApetProtoList[i].m_Type );
		cmdAPet.GetRec("a_name",m_pApetProtoList[i].m_strName );
		cmdAPet.GetRec("a_str",m_pApetProtoList[i].m_nStr );
		cmdAPet.GetRec("a_con",m_pApetProtoList[i].m_nCon );
		cmdAPet.GetRec("a_dex",m_pApetProtoList[i].m_nDex );
		cmdAPet.GetRec("a_int",m_pApetProtoList[i].m_nInt );
		cmdAPet.GetRec("a_item_idx",m_pApetProtoList[i].m_nItemIndex );
		cmdAPet.GetRec("a_maxFaith",m_pApetProtoList[i].m_nMaxFaith );
		cmdAPet.GetRec("a_maxStm",m_pApetProtoList[i].m_nMaxStm );
		cmdAPet.GetRec("a_maxHP",m_pApetProtoList[i].m_nMaxHP );
		cmdAPet.GetRec("a_maxMP",m_pApetProtoList[i].m_nMaxMP );
		cmdAPet.GetRec("a_recoverHP",m_pApetProtoList[i].m_nRecoverHP );
		cmdAPet.GetRec("a_recoverMP",m_pApetProtoList[i].m_nRecoverMP );
		cmdAPet.GetRec("a_speed",m_pApetProtoList[i].m_nSpeed );
		cmdAPet.GetRec("a_AISlot",m_pApetProtoList[i].m_nAISlot );
		// 공격
		cmdAPet.GetRec("a_attack",m_pApetProtoList[i].m_nAttack );
		cmdAPet.GetRec("a_defence",m_pApetProtoList[i].m_nDefence );
		cmdAPet.GetRec("a_Mattack",m_pApetProtoList[i].m_nMagicAttack );
		cmdAPet.GetRec("a_Mdeffence",m_pApetProtoList[i].m_nMagicDefence );
		cmdAPet.GetRec("a_hitpoint",m_pApetProtoList[i].m_nHitPoint );
		cmdAPet.GetRec("a_avoidpoint",m_pApetProtoList[i].m_nAvoidPoint );
		cmdAPet.GetRec("a_Mavoidpoint",m_pApetProtoList[i].m_nMagicAvoidPoint );
		cmdAPet.GetRec("a_attackSpeed",m_pApetProtoList[i].m_nAttackSpeed );
		// 기본 데미지
		cmdAPet.GetRec("a_deadly",m_pApetProtoList[i].m_nDeadly );
		cmdAPet.GetRec("a_critical",m_pApetProtoList[i].m_nCritical );
		cmdAPet.GetRec("a_awful",m_pApetProtoList[i].m_nAwful );
		cmdAPet.GetRec("a_strong",m_pApetProtoList[i].m_nStrong );
		cmdAPet.GetRec("a_normal",m_pApetProtoList[i].m_nNormal );
		cmdAPet.GetRec("a_week",m_pApetProtoList[i].m_nWeek );
		i++;
	} while( cmdAPet.MoveNext() );

	return true;
}

CAPet* CAPetProtoList::Create( CPC* pOwner, int dbpet_idx, int proto_idx  )
{
	CAPet* attackPet = new CAPet;
	attackPet->m_pProto = FindProto( proto_idx );
	if( attackPet->m_pProto == NULL )
	{
		delete attackPet;
		attackPet = NULL;
	}
	else
	{
		attackPet->m_type	= MSG_CHAR_APET;
		attackPet->m_index	= dbpet_idx;
		attackPet->SetOwner(pOwner);
		attackPet->m_name	= gserver.m_itemProtoList.FindIndex( attackPet->m_pProto->m_nItemIndex )->m_name;		

		attackPet->m_str	= attackPet->m_pProto->m_nStr;
		attackPet->m_con	= attackPet->m_pProto->m_nCon;
		attackPet->m_dex	= attackPet->m_pProto->m_nDex;
		attackPet->m_int	= attackPet->m_pProto->m_nInt;

		attackPet->m_maxHP = attackPet->m_pProto->m_nMaxHP;
		attackPet->m_maxMP = attackPet->m_pProto->m_nMaxMP;
		attackPet->m_recoverHP	= attackPet->m_pProto->m_nRecoverHP;
		attackPet->m_recoverMP	= attackPet->m_pProto->m_nRecoverMP;

		attackPet->m_runSpeed	= attackPet->m_pProto->m_nSpeed;
		attackPet->m_walkSpeed	= (attackPet->m_pProto->m_nSpeed/2)+1;

		attackPet->SetFaith( attackPet->m_pProto->m_nMaxFaith );
		attackPet->SetStamina( attackPet->m_pProto->m_nMaxStm );

		attackPet->m_nSeal = 1;
		attackPet->m_nRemainStat = 0;
		attackPet->m_nSP = 0;
		attackPet->m_level = 1;

		attackPet->m_nAttack		= attackPet->m_pProto->m_nAttack;
		attackPet->m_nMagicAttack	= attackPet->m_pProto->m_nMagicAttack;
		attackPet->m_nDefence		= attackPet->m_pProto->m_nDefence;
		attackPet->m_nMagicDefence	= attackPet->m_pProto->m_nMagicDefence;
		
		attackPet->CalcStatus(false);
		attackPet->m_hp = attackPet->m_maxHP;
		attackPet->m_mp	= attackPet->m_maxMP;
#ifdef APET_AI
		attackPet->m_nAISlot = attackPet->m_pProto->m_nAISlot;
#endif
	}

	return attackPet;
}

CAPetProto* CAPetProtoList::FindProto(int apet_idx)
{
	CAPetProto proto;
	proto.Index(apet_idx);

	return (CAPetProto*)bsearch(&proto, m_pApetProtoList, m_nCount, sizeof(CAPetProto), CompIndex);
}

#endif // ATTACK_PET