#ifndef __APET_PROTO_H__
#define __APET_PROTO_H__

#define HUMAN_PET_TYPE (1<<1)
#define BEAST_PET_TYPE (1<<2)

class CAPetProto
{
private:
	int		m_nIndex;
public:	
	int		m_Type;
	CLCString	m_strName;
	// Bagic
	int		m_nStr;
	int		m_nCon;
	int		m_nDex;
	int		m_nInt;
	int		m_nItemIndex;
	int		m_nMaxFaith;
	int		m_nMaxStm;
	int		m_nMaxHP;
	int		m_nMaxMP;
	int		m_nRecoverHP;
	int		m_nRecoverMP;
	int		m_nSpeed;
	int		m_nAISlot;
	// Attack
	int		m_nAttack;
	int		m_nDefence;
	int		m_nMagicAttack;
	int		m_nMagicDefence;
	int		m_nHitPoint;
	int		m_nAvoidPoint;
	int		m_nMagicAvoidPoint;
	int		m_nAttackSpeed;
	int		m_nDeadly;
	int		m_nCritical;
	int		m_nAwful;
	int		m_nStrong;
	int		m_nNormal;
	int		m_nWeek;

public:
	CAPetProto();
	~CAPetProto();

	int		Index()				{ return m_nIndex; }
	void	Index( int idx )	{ if(m_nIndex<0) m_nIndex = idx; }				// Index 가 있을때는 변경 금지

	void	InitValue();
};


class CAPet;

class CAPetProtoList  
{
public:
	CAPetProtoList();
	~CAPetProtoList();

	CAPetProto*	m_pApetProtoList;		// Apet List [Arrary]
	int			m_nCount;			// 개수
	int			m_virtualIndex;		// 가상 인덱스

	bool		Load();						// APet 기본 정보(Proto) 읽기

	CAPet*		Create(CPC* pOwner, int dbpet_idx, int proto_idx );			// 해당 idx의 PET 생성하기 ( new 가 있음으로 Delete 해줄껏 )
	CAPetProto*	FindProto(int apet_idx);		// 해당 idx의 정보 반환

protected:
	static int CompIndex(const void* p1, const void* p2)
	{
		CAPetProto* i1 = (CAPetProto*)p1;
		CAPetProto* i2 = (CAPetProto*)p2;

		return i1->Index() - i2->Index();
	}
};

#endif //__APET_PROTO_H__