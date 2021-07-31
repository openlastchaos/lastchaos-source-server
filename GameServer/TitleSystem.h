// TitleSystem.h: interface for the CTitleProtoList class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_TITLESYSTEM_H__060344F0_5F3D_42FD_8B8C_CFC42F759C27__INCLUDED_)
#define AFX_TITLESYSTEM_H__060344F0_5F3D_42FD_8B8C_CFC42F759C27__INCLUDED_

#define MAKE_TITLE_FILENAME "/data/customtitle.bin"
#define CUSTOM_OPTION_MAX_LEVEL 5

class CTitle;
class CTitleProto;
class CTitleNode;

class CTitleProtoList
{
public:
	typedef std::map<int, CTitleProto*> map_t;
	map_t	map_;
	CTitle* Create(int index, int time);
	CTitleProto* FindProto(int index);
	bool Load();
	int m_nCount;
	CTitleProto* m_proto;
	CTitleProtoList();
	virtual ~CTitleProtoList();
};

class CTitleProto
{
public:
	int m_itemidx;
	int m_index;
	int m_time;
	int m_option[MAX_TITLE_OPTION][2];		// [0] 옵션인덱스 , [1] 레벨
	int m_nOptionCount;
	int m_castleNum;

	CTitleProto();
	virtual ~CTitleProto();
};

class CTitle
{
public:
	CTitleProto* m_proto;
	int m_endtime;
	int m_custom_title_index;
	CTitle();
	virtual ~CTitle();
};

class CTitleList
{
public:
	bool CheckTitleTime(int CurrentTitle);
	bool HaveTitle(int index);
	CTitle* Find(int index);
	CTitle* FindCustomTitle(int custom_title_index);
	CTitleNode* m_head;
	int m_nCount;
	bool Remove(CTitle* title);
	bool Add(CTitle* title);
	CTitleList();
	virtual ~CTitleList();
};

class CTitleNode
{
public:
	CTitleNode* m_prev;
	CTitleNode* m_next;
	CTitle* m_title;
	CTitleNode(CTitle* title);
	virtual ~CTitleNode();
};

//Custom Title

struct stCustomTitleOption
{
	int nIndex;
	int nLevel[CUSTOM_OPTION_MAX_LEVEL];

	stCustomTitleOption()
	{
		nIndex = -1;

		for (int i = 0; i < CUSTOM_OPTION_MAX_LEVEL; ++i)
		{
			nLevel[i] = -1;
		}
	}
};

struct stCustomTitleString
{
	char strValue[32];

	stCustomTitleString()
	{
		memset(strValue, '\0', sizeof(strValue));
	}
};

class CTitleMakeData
{
public:
	CTitleMakeData();
	~CTitleMakeData();

	stCustomTitleOption* tempLoadData;

	int	nOptionCount;
	int nFrontCount;
	int nBackCount;
	int nEffectCount;

	bool load();

	int getIndex(int index);
	int getRandomLevel(int index);
};

#endif // !defined(AFX_TITLESYSTEM_H__060344F0_5F3D_42FD_8B8C_CFC42F759C27__INCLUDED_)
//
