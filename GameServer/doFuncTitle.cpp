#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DBManager.h"
#include "EventProcessWithThread.h"

#include "../ShareLib/packetType/ptype_old_do_title.h"

#include <boost/any.hpp>

void do_ExTitleSystemListReq(CPC* ch);
void do_ExTitleSystemTitleSelectReq(CPC* ch, CNetMsg::SP& msg);
void do_ExTitleSystemTitleCancelReq(CPC* ch, CNetMsg::SP& msg);
void do_ExTitleSystemTitleDeleteReq(CPC* ch, CNetMsg::SP& msg);
void do_ExTitleSystemMake(CPC* ch, CNetMsg::SP& msg);

void do_FuncTitle(CPC* ch, CNetMsg::SP& msg)
{
	if(!ch || DEAD(ch))
		return ;
	
	pTypeThirdBase* pBase = reinterpret_cast<pTypeThirdBase*>(msg->m_buf);

	switch(pBase->thirdType)
	{
	case MSG_EX_TITLE_SYSTEM_LIST_REQ:
		do_ExTitleSystemListReq(ch);
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_SELECT_REQ:
		do_ExTitleSystemTitleSelectReq(ch, msg);
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_REQ:
		do_ExTitleSystemTitleCancelReq(ch, msg);
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_DELETE_REQ:
		do_ExTitleSystemTitleDeleteReq(ch, msg);
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_MAKE:
		do_ExTitleSystemMake(ch, msg);
		break;
	default:
		break;
	}
}

void do_ExTitleSystemListReq(CPC* ch)
{
	if (ch->m_titleList.m_nCount == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSystemListMsg(rmsg, MSG_EX_TITLE_SYSTEM_LIST_REP_NO_LIST, ch->m_titleList.m_nCount);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperTitleSystemCheckTimeReq(rmsg, ch->m_index, ch->m_titleList.m_head);
		SEND_Q(rmsg, gserver->m_subHelper);
	}
}

void do_ExTitleSystemTitleSelectReq(CPC* ch, CNetMsg::SP& msg)
{
	int title_index;
	RequestClient::TitleSystemSelect* packet = reinterpret_cast<RequestClient::TitleSystemSelect*>(msg->m_buf);
	title_index = packet->title_index;
	CTitle* title  = ch->m_titleList.Find(title_index);

	if(title == NULL)
		return;

	if(!ch->m_titleList.HaveTitle(title_index))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_NOT_HAVE_TITLE, title_index, ch->m_index, title->m_custom_title_index);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	else
	{
		if(ch->m_nCurrentTitle == CUSTOM_TITLE_DUMMY_INDEX)
		{
			std::string query = boost::str(boost::format("UPDATE t_title_make set a_use = 0 where a_index = %d") % ch->m_custom_title_index);
			DBManager::instance()->pushQuery(0, query);

			CNetMsg::SP rmsg(new CNetMsg);
			TitleUserInfoMsg(rmsg, ch->m_index, -1, -1, -1, "");
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

		ch->m_nCurrentTitle = title_index;
		ch->CalcStatus(true);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_TITLE_SELECT_SUCCESS, title_index, ch->m_index, title->m_custom_title_index);
			SEND_Q(rmsg, ch->m_desc);
			ch->m_pArea->SendToCell(rmsg, ch);
		}

		if(title_index == CUSTOM_TITLE_DUMMY_INDEX)
		{
			std::map<int, MAKE_TITLE*>::iterator it = ch->_map_title.find(title->m_custom_title_index);
			if(it != ch->_map_title.end())
			{
				std::string query = boost::str(boost::format("UPDATE t_title_make set a_use = 1 where a_index = %d") % title->m_custom_title_index);
				DBManager::instance()->pushQuery(0, query);

				CNetMsg::SP rmsg(new CNetMsg);
				TitleUserInfoMsg(rmsg, ch->m_index, it->second->color, it->second->background_color, it->second->effect, it->second->name);
				ch->m_pArea->SendToCell(rmsg, ch, true);

				ch->m_custom_title_index = title->m_custom_title_index;
			}
		}

		GAMELOG << init("TITLE SELECT", ch) << delim
			<< "TITLE INDEX" << delim << title_index << end;
	}
}

void do_ExTitleSystemTitleCancelReq(CPC* ch, CNetMsg::SP& msg)
{
	int title_index;
	RequestClient::TitleSystemCancel* packet = reinterpret_cast<RequestClient::TitleSystemCancel*>(msg->m_buf);
	title_index = packet->title_index;
	CTitle* title = ch->m_titleList.Find(title_index);

	if(title == NULL)
		return;

	if(ch->m_nCurrentTitle == CUSTOM_TITLE_DUMMY_INDEX)
	{
		std::string query = boost::str(boost::format("UPDATE t_title_make set a_use = 0 where a_index = %d") % title->m_custom_title_index);
		DBManager::instance()->pushQuery(0, query);

		CNetMsg::SP rmsg(new CNetMsg);
		TitleUserInfoMsg(rmsg, ch->m_index, -1, -1, -1, "");
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	ch->m_nCurrentTitle = TITLE_SYSTEM_NO_TITLE;
	ch->m_custom_title_index = -1;
	ch->CalcStatus(true);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_SUCCESS, title_index, ch->m_index, title->m_custom_title_index);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_pArea->SendToCell(rmsg, ch);
	}

	GAMELOG << init("TITLE CANCEL", ch) << delim
		<< "TITLE INDEX" << delim << title_index << end;
}

void do_ExTitleSystemTitleDeleteReq(CPC* ch, CNetMsg::SP& msg)
{
	int title_index;
	RequestClient::TitleSystemDelete* packet = reinterpret_cast<RequestClient::TitleSystemDelete*>(msg->m_buf);
	title_index = packet->title_index;
	CTitle* title = ch->m_titleList.Find(title_index);

	if(title == NULL)
		return;

	if(!ch->m_titleList.HaveTitle(title_index))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_NOT_HAVE_TITLE, title_index, ch->m_index, title->m_custom_title_index);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		if(gserver->isRunSubHelper())
		{
			GAMELOG << init("TITLE DELETE REQ", ch) << delim
				<< "TITLE INDEX" << delim << title_index << end;
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperTitleSystemDeleteReq(rmsg, ch->m_index, title_index);
			SEND_Q(rmsg, gserver->m_subHelper);
		}
	}
}

void do_ExTitleSystemMake(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::TitleSystemMake *packet = reinterpret_cast<RequestClient::TitleSystemMake*>(msg->m_buf);

	CItem* item = ch->m_inventory.getItem(packet->tab, packet->inven_index);

	if( item == NULL )
	{
		//hacking
		LOG_INFO("HACKING? not found item. tab_index[%d], inven_index[%d], char_index[%d]", packet->tab, packet->inven_index, ch->m_index);
		ch->m_desc->Close("not found item");
		return;
	}
	
	if(item->getVIndex() != packet->vIndex)
	{
		//hacking
		LOG_INFO("HACKING? not equal vIndex. packet_vIndex[%d], item_vIndex[%d], char_index[%d]", packet->vIndex, item->getVIndex(), ch->m_index);
		ch->m_desc->Close("not equal vIndex");
		return;
	}

	if(item->getDBIndex() != 11036)
	{
		//hacking
		LOG_INFO("HACKING? invalid DBIndex. item_db_index[%d], char_index[%d]", item->getDBIndex(), ch->m_index);
		ch->m_desc->Close("invalid DBIndex");
		return;
	}
	
	if(packet->color > gserver->m_titleMakedata.nFrontCount)
	{
		//hacking
		LOG_INFO("HACKING? invalid color index. packet_color[%d], char_index[%d]", packet->color, ch->m_index);
		ch->m_desc->Close("invalid color index");
		return;
	}
	if(packet->background_color > gserver->m_titleMakedata.nBackCount)
	{
		//hacking
		LOG_INFO("HACKING? invalid background. packet_background[%d], char_index[%d]", packet->background_color, ch->m_index);
		ch->m_desc->Close("invalid background");
		return;
	}
	if(packet->effect > gserver->m_titleMakedata.nEffectCount)
	{
		//hacking
		LOG_INFO("HACKING? invalid effect. packet->effect[%d], char_index[%d]", packet->effect, ch->m_index);
		ch->m_desc->Close("invalid effect");
		return;
	}
	
	//옵션 생성
	int rand = GetRandom(0, 10000);
	int option_count = 0;

	if( rand < 500 )				//5%
		option_count = 5;
	else if( rand < 2000 )			//15%
		option_count = 4;
	else if( rand < 4000 )			//20%
		option_count = 3;
	else if( rand < 6500 )			//25%
		option_count = 2;
	else							//35%
		option_count = 1;

	int* option_index = new int[5];
	int* option_level = new int[5];

	for(int i = 0;  i < 5; i++)
	{
		option_index[i] = -1;
		option_level[i] = -1;
	}
	
	for(int i = 0; i < option_count; i++)
	{
		//옵션 인덱스, 레벨 랜덤 설정
		int rand = GetRandom(0, gserver->m_titleMakedata.nOptionCount - 1);

		option_index[i] = gserver->m_titleMakedata.getIndex(rand);
		option_level[i] = gserver->m_titleMakedata.getRandomLevel(rand);
	}

	std::string name = packet->name;

	LOG_INFO("TITLE MAKE DBT REQ. char_index[%d], item_vIndex[%d], color[%d], background[%d], effect[%d], name[%s], option_count[%d], option_index : [%d] : [%d] : [%d] : [%d] : [%d], option_level : [%d] : [%d] : [%d] : [%d] : [%d]", ch->m_index, item->getVIndex(), 
		packet->color, packet->background_color, packet->effect, packet->name, option_count, option_index[0], option_index[1] ,option_index[2], option_index[3], option_index[4], option_level[0], option_level[1], option_level[2], option_level[3], option_level[4]);

	DBManager::instance()->pushMakeTitleInfo(ch->m_desc, ch->m_index, item->getVIndex(), packet->color, packet->background_color, packet->effect, name, option_index, option_level);
}

void do_ExTitleSystemMake_return_DBThread(boost::any& any)
{
	EventProcessForDB::getMakeTitleInfo* data = boost::any_cast<EventProcessForDB::getMakeTitleInfo>(&any);

	LOG_INFO("TITLE MAKE DBT REP. char_index[%d], item_vIndex[%d], color[%d], background[%d], effect[%d], name[%s], option_index : [%d] : [%d] : [%d] : [%d] : [%d], option_level : [%d] : [%d] : [%d] : [%d] : [%d]",
		data->char_index, data->v_index, data->color, data->background_color, data->effect, data->name.c_str(), data->option_index[0], data->option_index[1], data->option_index[2], data->option_index[3], data->option_index[4], data->option_level[0], data->option_level[1], data->option_level[2], data->option_level[3], data->option_level[4]);
	
	CPC* ch = PCManager::instance()->getPlayerByCharIndex(data->char_index);
	if (ch == NULL)
	{
		delete[] data->option_index;
		delete[] data->option_level;

		return;
	}

	CItem* delete_item = ch->m_inventory.FindByVirtualIndex(data->v_index);
	CItem* item = gserver->m_itemProtoList.CreateItem(CUSTOM_TITLE_ITEM_INDEX, -1, 0, 0, 1);

	if(item == NULL)
	{
		delete[] data->option_index;
		delete[] data->option_level;

		LOG_ERROR("NOT FOUND CUSTOM TITLE ITEM. CHECK THE DATABASE. item_index[%d]", CUSTOM_TITLE_ITEM_INDEX);
		return;
	}

	if(delete_item == NULL)
	{
		delete[] data->option_index;
		delete[] data->option_level;
		
		LOG_ERROR("HACKING? not found item. char_index[%d], item_vIndex[%d], title_index[%d]", data->char_index, data->v_index, data->title_index);
		ch->m_desc->Close("not found item");
		return;
	}

	MAKE_TITLE* title = new MAKE_TITLE;
	title->color = data->color;
	title->background_color = data->background_color;
	title->effect = data->effect;
	title->title_index = data->title_index;
	strncpy(title->name, data->name.c_str(), CUSTOM_TITLE_NAME_LENGTH);
	title->option_index = data->option_index;
	title->option_level = data->option_level;

	ch->_map_title.insert(std::pair<int, MAKE_TITLE*>(data->title_index, title));

	//타이틀 정보 보내고
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSystemMakeInfoMsg(rmsg, title->title_index, title->name, (int)title->color, (int)title->background_color, (int)title->effect, title->option_index, title->option_level);
		SEND_Q(rmsg, ch->m_desc);
	}

	//재료 아이템 삭제
	ch->m_inventory.decreaseItemCount(delete_item, 1);

	//커스텀 타이틀아이템에서는 used2 값을 링크 타이틀 인덱스로 사용하도록 한다.
	item->setUsed_2(data->title_index);
	ch->m_inventory.addItem(item);
}