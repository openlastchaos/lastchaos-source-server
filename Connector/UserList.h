#ifndef __USERLIST_H__
#define __USERLIST_H__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <vector>

struct login_user_info
{
	int			user_index;
	std::string user_name;
	CUser*		user;
};

struct channel_info
{
	int	playersPerZone[MAX_ZONES];
	int user_count;
};


class UserList
{
public:
	UserList(int max_channel_count);
	~UserList();
	
private:
	
	struct key_index {};
	struct key_name	{};

	typedef boost::multi_index_container
		<
			login_user_info,
			boost::multi_index::indexed_by
			<
			boost::multi_index::hashed_unique
			<
			boost::multi_index::tag<key_index>, BOOST_MULTI_INDEX_MEMBER( login_user_info, int, user_index ) 
			>,
			boost::multi_index::hashed_unique
			<
			boost::multi_index::tag<key_name>, BOOST_MULTI_INDEX_MEMBER( login_user_info, std::string, user_name)
			>
			>
		> map_t;

	std::vector<channel_info*> vec_channel_info;
	map_t _map;

public:
	void init_(int max_channel_count);
	bool insert_(CUser* user);
	void delete_(CUser* user);
	
	CUser* findByUserIndex(int user_index);
	CUser* findByUserName(std::string user_name);
	
	int getUserCountInChannel(int channelNo);

	int increasePlayerZoneCount(int channelNo, int zone_index);
	int decreasePlayerZoneCount(int channelNo, int zone_index);

	int getPlayerZoneCount(int channelNo, int zone_index);
	void setPlayerZoneCount(int channelNo, int zone_index, int count);

	void logout_all(CDescriptor* d);
};

#endif