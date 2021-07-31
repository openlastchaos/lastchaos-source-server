#ifndef __RESERVED_GM_COMMAND_MANAGER_H__
#define __RESERVED_GM_COMMAND_MANAGER_H__

#include <list>
#include <map>
#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/packetType/ptype_reserved_gm_command.h"

class ReservedGmCommandManager : public rnSocketIOHandler
{
public:
	struct dataInfo
	{
		bool start_active_flag_;			// false이면 모든 작동에서 제외를 시킨다.
		bool end_active_flag_;				// false이면 모든 작동에서 제외를 시킨다.
		int start_year;
		int start_month;
		int end_year;
		int end_month;
		reservedGMCommandElement ele;

		dataInfo() : start_active_flag_(true), end_active_flag_(true) {}
	};

	struct dataInfoToGmCommand
	{
		int index;
		int subno;
		int startTime;
		char command[RESERVED_GM_COMMAND_MAX_STRING + 1];
	};

	typedef std::map<int/*a_index*/, dataInfo> map_t;
	typedef std::map<int, dataInfoToGmCommand> map_gm_t;

public:
	ReservedGmCommandManager(void) {}
	~ReservedGmCommandManager(void) {}

	static ReservedGmCommandManager* Instance();

	bool Init();
	virtual void operate(rnSocketIOService* service);
	void sendListToClient(rnSocketIOService* service, int m_Index, int year, int month);
	void sendListToClinetByGm(rnSocketIOService* service, int char_index);

	void process(CNetMsg::SP& msg);

private:
	void Delete(int a_index);
	void Add(reservedGMCommandElement& ele);
	void makeMap(reservedGMCommandElement& ele);

public:
	void Delete_gm(int a_index);
	void Add_gm(dataInfoToGmCommand& data);
	void makeMap_gm(dataInfoToGmCommand& data);

private:
	map_t			map_;			// key = a_index
	map_gm_t		map_gm_;
};

#endif
