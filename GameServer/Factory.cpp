#include "stdhdrs.h"
#include "Server.h"
#include "Factory.h"
#include "DBCmd.h"

#ifdef FACTORY_SYSTEM

///////////////////
// CFactoryProto
CFactoryProto::CFactoryProto()
{
	m_nIndex		= 0;
	m_nJob			= -1;
	m_nSealType		= -1;
	m_nItemType		= -1;
	m_nItemIdx		= -1;
	m_llMakeExp		= 0;
	m_llNeedExp		= 0;
	m_llNas			= 0;

	memset(m_stuff, -1, sizeof(FACTORY_STUFF) * MAX_FACTORY_ITEM_STUFF);
}

CFactoryProto::~CFactoryProto()
{
}

void CFactoryProto::InitData(int nPos, 
							 int nIndex,	int nJob, 
							 int nSealType, int nItemType, int nItemIdx, 
							 LONGLONG llMakeExp, 
							 LONGLONG llNeedExp, 
							 LONGLONG llNas,
							 FACTORY_STUFF * pStuff
							 )
{
	m_nIndex = nIndex;
	m_nJob	= nJob;
	m_nSealType = nSealType;
	m_nItemType = nItemType;
	m_nItemIdx	= nItemIdx;
	m_llMakeExp	= llMakeExp;
	m_llNeedExp	= llNeedExp;
	m_llNas		= llNas;

	for (int i = 0; i < MAX_FACTORY_ITEM_STUFF; i++)
	{
		m_stuff[i].nItemIdx	= pStuff[i].nItemIdx;
		m_stuff[i].nCount	= pStuff[i].nCount;
	}
}

///////////////////////
// CFactoryProtoList
CFactoryProtoList::CFactoryProtoList()
{
	m_listFactoryItem = NULL;
	m_nCount = 0;
}

CFactoryProtoList::~CFactoryProtoList()
{
	while (m_listFactoryItem)
	{
		delete [] m_listFactoryItem;
		m_listFactoryItem = NULL;
	}
	m_nCount = 0;
}

CFactoryProto* CFactoryProtoList::Find(int nIndex)
{
	CFactoryProto key;
	key.SetIndex(nIndex);
	return (CFactoryProto*)bsearch(&key, m_listFactoryItem, m_nCount, sizeof(CFactoryProto), CompIndex);
}

bool CFactoryProtoList::Load()
{
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbdata);
	cmd.SetQuery("SELECT * FROM t_factory_item WHERE a_enable = 1 ORDER BY a_index");
	if (!cmd.Open())
		return false;

	m_listFactoryItem = new CFactoryProto[cmd.m_nrecords];

	int				a_index		= 0;
	int				a_job		= 0;
	int				a_seal_type	= 0;
	int				a_item_type = 0;
	int				a_item_idx	= 0;
	LONGLONG		a_make_exp	= 0;
	LONGLONG		a_need_exp	= 0;
	LONGLONG		a_nas		= 0;
	FACTORY_STUFF	stuff[MAX_FACTORY_ITEM_STUFF];
	memset(&stuff, 0x00, sizeof(stuff));

	m_nCount = 0;
	while (cmd.MoveNext())
	{

		if (!cmd.GetRec("a_index",		a_index		))	return false;
		if (!cmd.GetRec("a_job",		a_job		))	return false;
		if (!cmd.GetRec("a_seal_type",	a_seal_type	))	return false;
		if (!cmd.GetRec("a_item_type",	a_item_type	))	return false;
		if (!cmd.GetRec("a_item_idx",	a_item_idx	))	return false;
		if (!cmd.GetRec("a_make_exp",	a_make_exp	))	return false;
		if (!cmd.GetRec("a_need_exp",	a_need_exp	))	return false;
		if (!cmd.GetRec("a_nas",		a_nas		))	return false;

		static CLCString stuffIdx(256);
		static CLCString stuffCnt(256);
		const char* pStuffIdx = stuffIdx;
		const char* pStuffCnt = stuffCnt;

		cmd.GetRec("a_stuff",		stuffIdx);
		cmd.GetRec("a_stuff_cnt",	stuffCnt);

		char tbuf[256];
		int i = 0;
		while ((*pStuffIdx && *pStuffCnt) && i < MAX_FACTORY_ITEM_STUFF)
		{
			pStuffIdx = AnyOneArg(pStuffIdx, tbuf);
			stuff[i].nItemIdx = atoi(tbuf);

			pStuffCnt = AnyOneArg(pStuffCnt, tbuf);
			stuff[i].nCount = atoi(tbuf);
			i++;
		}

		m_listFactoryItem[m_nCount].InitData(m_nCount, a_index, a_job, 
											a_seal_type, a_item_type, a_item_idx, 
											a_make_exp, a_need_exp, a_nas, stuff);
		m_nCount++;
	}

	return true;
}


///////////////////////////
// CFactoryList
CFactoryList::CFactoryList()
:m_listDetail(NULL),
m_listColorful(NULL),
m_listSharp(NULL),
m_listHard(NULL),
m_listConsume(NULL)
{
}

CFactoryList::~CFactoryList()
{
}

bool CFactoryList::Add(CFactoryProto * pFactory)
{
	if (!pFactory)
		return false;

		 if (pFactory->IsDetail()	)	m_listDetail.AddToTail(pFactory);
	else if (pFactory->IsColorful()	)	m_listColorful.AddToTail(pFactory);
	else if (pFactory->IsSharp()	)	m_listSharp.AddToTail(pFactory);
	else if (pFactory->IsHard()		)	m_listHard.AddToTail(pFactory);
	else if (pFactory->IsConsume()	)	m_listConsume.AddToTail(pFactory);
	else	return false;

	return true;
}

bool CFactoryList::Find(CFactoryProto * pFactory)
{
	if (!pFactory)
		return false;

		 if (pFactory->IsDetail()	)	m_listDetail.FindData(pFactory);
	else if (pFactory->IsColorful()	)	m_listColorful.FindData(pFactory);
	else if (pFactory->IsSharp()	)	m_listSharp.FindData(pFactory);
	else if (pFactory->IsHard()		)	m_listHard.FindData(pFactory);
	else if (pFactory->IsConsume()	)	m_listConsume.FindData(pFactory);
	else	return false;

	return true;
}

CFactoryProto* CFactoryList::Find(int nIndex)
{
	if (nIndex < 0)
		return NULL;

	CFactoryProto * p = (CFactoryProto *)m_listDetail.GetHead();
	while (p)
	{
		if (p->GetIndex() == nIndex)
		{
			return p;
			break;
		}
		p = (CFactoryProto *)m_listDetail.GetNext(p);
	}
			 
	p = (CFactoryProto *)m_listColorful.GetHead();
	while (p)
	{
		if (p->GetIndex() == nIndex)
		{
			return p;
			break;
		}
		p = (CFactoryProto *)m_listColorful.GetNext(p);
	}

	p = (CFactoryProto *)m_listSharp.GetHead();
	while (p)
	{
		if (p->GetIndex() == nIndex)
		{
			return p;
			break;
		}
		p = (CFactoryProto *)m_listSharp.GetNext(p);
	}

	p = (CFactoryProto *)m_listHard.GetHead();
	while (p)
	{
		if (p->GetIndex() == nIndex)
		{
			return p;
			break;
		}
		p = (CFactoryProto *)m_listHard.GetNext(p);
	}

	p = (CFactoryProto *)m_listConsume.GetHead();
	while (p)
	{
		if (p->GetIndex() == nIndex)
		{
			return p;
			break;
		}
		p = (CFactoryProto *)m_listConsume.GetNext(p);
	}

	return NULL;
}
#endif // FACTORY_SYSTEM
