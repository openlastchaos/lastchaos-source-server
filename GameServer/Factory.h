#ifndef __FACTORY_H__
#define __FACTORY_H__

#ifdef FACTORY_SYSTEM

typedef struct __tagFactoryStuff
{
	int		nItemIdx;
	int		nCount;
} FACTORY_STUFF;

class CFactoryProto
{
	int				m_nIndex;
	int				m_nJob;
	int				m_nSealType;			// 제작 스킬 타입
	int				m_nItemType;			// 아이템 분류
	int				m_nItemIdx;
	LONGLONG		m_llMakeExp;
	LONGLONG		m_llNeedExp;
	LONGLONG		m_llNas;
	FACTORY_STUFF	m_stuff[MAX_FACTORY_ITEM_STUFF];

public:
	CFactoryProto();
	~CFactoryProto();

	void InitData(int nPos, 
				  int nIndex,	 int nJob, 
				  int nSealType, int nItemType, int nItemIdx, 
				  LONGLONG llMakeExp, 
				  LONGLONG llNeedExp, 
				  LONGLONG llNas,
				  FACTORY_STUFF * pStuff);

	int						GetIndex() const	{ return m_nIndex;			}
	int						GetSealType()		{ return m_nSealType;		}
	int						GetItemIdx()		{ return m_nItemIdx;		}
	const FACTORY_STUFF *	GetStuff(int nPos)	{ return &m_stuff[nPos];	}
	LONGLONG				GetNas()			{ return m_llNas;			}
	LONGLONG				GetNeedExp()		{ return m_llNeedExp;		}
	LONGLONG				GetMakeExp()		{ return m_llMakeExp;		}


	void SetIndex(int nIndex)			{ m_nIndex = nIndex; }
	
	bool IsDetail()		{ return (m_nSealType == SEAL_TYPE_DETAIL	) ? true : false; }
	bool IsColorful()	{ return (m_nSealType == SEAL_TYPE_COLORFUL	) ? true : false; }
	bool IsSharp()		{ return (m_nSealType == SEAL_TYPE_SHARP	) ? true : false; }
	bool IsHard()		{ return (m_nSealType == SEAL_TYPE_HARD		) ? true : false; }
	bool IsConsume()	{ return (m_nSealType == SEAL_TYPE_CONSUME	) ? true : false; }

//	const SETITEMOPTION* GetOption(int n) const	{ return (const SETITEMOPTION*)(m_option + n); }

};

class CFactoryProtoList
{
	CFactoryProto*		m_listFactoryItem;
	int					m_nCount;

public:
	CFactoryProtoList();
	~CFactoryProtoList();

	bool Load();
	CFactoryProto* Find(int nIndex);

protected:
	static int CompIndex(const void* p1, const void* p2)
	{
		CFactoryProto* d1 = (CFactoryProto*)p1;
		CFactoryProto* d2 = (CFactoryProto*)p2;
		return d1->GetIndex() - d2->GetIndex();
	}
};

///////////////////////////
// 캐릭터 제작 아이템 리스트
//
class CFactoryList
{
public:
	CFactoryList();
	~CFactoryList();

public:
	CLCList<CFactoryProto *> m_listDetail;				// 정교한
	CLCList<CFactoryProto *> m_listColorful;			// 화려한
	CLCList<CFactoryProto *> m_listSharp;				// 날카로운
	CLCList<CFactoryProto *> m_listHard;				// 단단한
	CLCList<CFactoryProto *> m_listConsume;				// 소모품


public:
	int GetCount()
	{
		int nCount = 0;
		nCount += m_listDetail.GetCount();
		nCount += m_listColorful.GetCount();
		nCount += m_listSharp.GetCount();
		nCount += m_listHard.GetCount();
		nCount += m_listConsume.GetCount();

		return nCount;
	}
	

	bool Add(CFactoryProto * pFactory);
	bool Find(CFactoryProto * pFactory);
	CFactoryProto * Find(int nIndex);

};

#endif // FACTORY_SYSTEM
#endif // __FACTORY_H__