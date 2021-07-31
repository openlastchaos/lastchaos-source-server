#ifndef __GUILD_BATTLE_MANAGER_H__
#define __GUILD_BATTLE_MANAGER_H__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#define ONE_PAGE_MAX_COUNT 8

struct st_multi_index
{
	int guild_index;
	int battle_time;
	CGuild* guild;
	GoldType_t stake_nas;
	int stake_gp;
	int reg_date;
	int ave_level;
	int zone_index;
};

static int guild_time_data[8] = {15, 30, 60, 90, 120, 180, 240, 300};

class GuildBattleManager
{
public:
	GuildBattleManager();
	~GuildBattleManager();

	struct Key_Guild_Index		{};
	struct Key_Battle_time		{};
	struct Key_Reg_date			{};

	typedef boost::multi_index_container<
		st_multi_index*,
		boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<Key_Guild_Index>, BOOST_MULTI_INDEX_MEMBER(st_multi_index, int, guild_index) >,
			boost::multi_index::ordered_non_unique<
				boost::multi_index::tag<Key_Battle_time>, BOOST_MULTI_INDEX_MEMBER(st_multi_index, int, battle_time) >,
			boost::multi_index::ordered_non_unique<
				boost::multi_index::tag<Key_Reg_date>, BOOST_MULTI_INDEX_MEMBER(st_multi_index, int, reg_date) >
		>
	> GM_MULTI_INDEX;

	static GuildBattleManager* instance();
	void regist(CGuild* guild, GoldType_t stake_nas, int guild_point, int battle_time, int ave_level, int zone_index);
	bool cancel(int guild_index);

	st_multi_index* find(int guild_index);

	void setFeeNas(int fee);
	int getFeeNas(int fee);
	void setFeeGp(int fee);
	int getFeeGp(int fee);

	void SendBattleList(CPC* ch, int page, int battle_time);

	void push_banish(int my_guild_index, int target_guild_index);
	bool check_banish(int my_guild_index, int target_guild_index);
	void delete_banish(int my_guild_index);

	void giveup(int char_index);		//로그아웃 및 포기
	

private:
	GM_MULTI_INDEX  m_gb_multi_index;

	int m_fee_nas;
	int m_fee_gp;

	//도전 거부길드 데이터 관리 컨테이너
	std::map< int, std::map<int, int> > _map_banish;
};

#endif