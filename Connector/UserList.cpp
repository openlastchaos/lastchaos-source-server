#include "stdhdrs.h"

#include "Server.h"
#include "User.h"
#include "UserList.h"
#include "Log.h"
#include "ProcConnMsg.h"


UserList::UserList(int max_channel_count)
{
	init_(max_channel_count);
}

UserList::~UserList()
{
	std::vector<channel_info*>::iterator it = vec_channel_info.begin();
	std::vector<channel_info*>::iterator it_end = vec_channel_info.end();

	for(; it != it_end; it++)
	{
		delete (*it);
	}
}

int UserList::getUserCountInChannel( int channelNo )
{
	return vec_channel_info[channelNo - 1]->user_count;
}

bool UserList::insert_( CUser* user )
{
	if(findByUserIndex(user->m_index) != NULL)
		return false;

	login_user_info user_info;
	user_info.user = user;
	user_info.user_index = user->m_index;
	user_info.user_name = user->m_name;

	_map.insert(user_info);
	vec_channel_info[user->m_subnum - 1]->user_count++;

	GAMELOG << init("PLAYER_ADD", user->m_name)
		<< vec_channel_info[user->m_subnum - 1]->user_count
		<<end;

	return true;
}

void UserList::delete_( CUser* user )
{
	if( user == NULL )
		return;

	vec_channel_info[user->m_subnum - 1]->user_count--;

	GAMELOG << init("PLAYER_DEL", user->m_name)
		<< vec_channel_info[user->m_subnum - 1]->user_count
		<<end;

	_map.erase(user->m_index);

	delete user;

	return;
}

CUser* UserList::findByUserIndex( int user_index )
{
	map_t::index<key_index>::type& use_tag = _map.get<key_index>();
	map_t::index<key_index>::type::iterator it = use_tag.find(user_index);
	const login_user_info& user_info = (*it);

	if( it == use_tag.end() )
		return NULL;

	return user_info.user;
}


CUser* UserList::findByUserName( std::string user_name )
{
	const map_t::index<key_name>::type& use_tag = _map.get<key_name>();
	map_t::index<key_name>::type::iterator it = use_tag.find(user_name);
	const login_user_info& user_info = (*it);

	if( it == use_tag.end() )
		return NULL;

	return user_info.user;
}

int UserList::increasePlayerZoneCount( int channelNo, int zone_index )
{
	if(channelNo > gserver.m_maxSubServer)
		return -1;
	
	if(zone_index > MAX_ZONES)
		return -1;

	return (vec_channel_info[channelNo - 1]->playersPerZone[zone_index])++;
}

int UserList::decreasePlayerZoneCount( int channelNo, int zone_index )
{
	if(channelNo > gserver.m_maxSubServer)
		return -1;

	if(zone_index > MAX_ZONES)
		return -1;

	return (vec_channel_info[channelNo - 1]->playersPerZone[zone_index])--;
}

int UserList::getPlayerZoneCount( int channelNo, int zone_index )
{
	if(channelNo > gserver.m_maxSubServer)
		return -1;

	if(zone_index > MAX_ZONES)
		return -1;

	return vec_channel_info[channelNo - 1]->playersPerZone[zone_index];
}

void UserList::setPlayerZoneCount( int channelNo, int zone_index, int count )
{
	if(channelNo > gserver.m_maxSubServer)
		return ;

	if(zone_index > MAX_ZONES)
		return ;
	
	vec_channel_info[channelNo - 1]->playersPerZone[zone_index] = count;
}

void UserList::init_( int max_channel_count )
{
	for(int i = 0; i < max_channel_count; i++)
	{
		channel_info* data = new channel_info();
		this->vec_channel_info.push_back(data);
	}
}

void UserList::logout_all( CDescriptor* d )
{
	if(d == NULL)
		return;

	map_t::iterator it = _map.begin();
	map_t::iterator it_end = _map.end();

	login_user_info user_info;
	
	for( ; it != it_end ; )
	{
		user_info = (*it);

		if(user_info.user == NULL)
		{
			_map.erase(user_info.user_index);
			it = _map.begin();
			it_end = _map.end();
			continue;
		}

		if(user_info.user->m_descserver ==  d)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_CONN_REQ);
			RefMsg(rmsg) << (unsigned char)MSG_CONN_LOGOUT_REQ
				<< user_info.user->m_name;
			
			gserver.ProcessLogout(user_info.user);

			it = _map.begin();
			it_end = _map.end();
		}
		else
		{
			it++;
		}
	}

}
