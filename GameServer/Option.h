#ifndef __OPTION_H__
#define __OPTION_H__

class CPC;
class APet;
class CItem;

class COptionProto
{
public:

	// DB 정보
	int		m_idNum;						// DB Index
	int		m_type;							// 옵션 번호
	int		m_levelValue[OPTION_MAX_LEVEL];	// 옵션 레벨별 값
	int		m_probValue[OPTION_MAX_LEVEL];	// 옵션 레벨별 확률
	int		m_weaponType;					// 어떤 무기에 붙는가
	int		m_wearType;						// 어떤 방어구에 붙는가
	int		m_accessoryType;				// 어떤 악세사리에 붙는가

	COptionProto();
};


class COptionProtoList
{
public:

	COptionProto*	m_proto;				// Option Proto
	int				m_nCount;				// Count

	COptionProtoList();
	~COptionProtoList();

	bool Load();							// DB Load
	COptionProto* FindProto(int type);		// Find Option Proto use Type

protected:
	static int CompIndex(const void* p1, const void* p2)
	{
		COptionProto* i1 = (COptionProto*)p1;
		COptionProto* i2 = (COptionProto*)p2;

		return i1->m_type - i2->m_type;
	}
	
};

class COption
{
public:
	COptionProto*	m_proto;		// Option Proto

	int				m_type;			// Option Type
	int				m_level;		// Option Level
	int				m_value;		// Option Value

	short			m_dbValue;		// Option DB Value

	COption();

	void MakeOptionValue(COptionProto* proto, int factLevel, int num);	// Make Option Type, Level use Proto
	void SetOptionValue(short dbValue);			// Set Option Value	use dbValue
	void ApplyOptionValue(CPC* pc, CItem* item);	// Apply Option Value

	void GetDBValue(short dbValue);				// DB value -> type : level
	void SetDBValue();							// type : level -> DB value

	static void ApplyOptionValue(CPC* pc, int nType, int nValue, CItem* pItem);
#ifdef ATTACK_PET
	void ApplyOptionValue(CAPet* apet, CItem* item);
	static void ApplyOptionValue(CAPet* apet, int nType, int nValue, CItem* pItem);
#endif //ATTACK_PET
};

#endif