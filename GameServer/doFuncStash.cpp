#include "stdhdrs.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Stash.h"
#include "Log.h"

void do_Stash(CPC* ch, CNetMsg& msg)
{
	// 죽은 상태, 전투중, 스킬 시전중, 워프중, 개인상점, 교환중에서는 불가능
	if (
			DEAD(ch)
			|| ch->IsCombatMode()
			|| ch->m_currentSkill
			|| ch->IsSetPlayerState(PLAYER_STATE_WARP)
			|| ch->m_personalshop
			|| ch->m_exchange
		)
	{
		CNetMsg rmsg;
		StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	unsigned char subtype = (unsigned char)-1;
	msg.MoveFirst();
	msg >> subtype;

	switch (subtype)
	{
	case MSG_STASH_ISSETPASSWORD:
		do_StashIsSetPassword(ch, msg);
		break;

	case MSG_STASH_LIST_REQ:
		do_StashListReq(ch, msg);
		break;

	case MSG_STASH_KEEP_REQ:
		do_StashKeep(ch, msg);
		break;

	case MSG_STASH_TAKE_REQ:
		do_StashTake(ch, msg);
		break;

	case MSG_STASH_CHANGE_PASSWORD_REQ:
		do_StashChangePassword(ch, msg);
		break;

	case MSG_STASH_SEAL:
		do_StashSeal(ch, msg);
		break;
/////////////////////////////////////////////
// BANGWALL : 2005-07-01 오후 2:45:26
// Comment : 창고 암호 관련 delete password

/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 2:38:51
// Comment : 테섭적용

	case MSG_STASH_DELETE_PASSWORD_REQ:
		do_StashDeletePassword(ch, msg);
		break;
	case MSG_STASH_CHECK_PASSWORD_REQ:
		do_StashCheckPassword(ch, msg);
		break;
	case MSG_STASH_SETTING_NEWPASSWORD_REQ:
		do_StashSetNewPassword(ch, msg);
		break;

	}
}


/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 2:37:52
// Comment : 테섭적용


void do_StashSetNewPassword(CPC* ch, CNetMsg& msg)
{
	// 이전에 처리중인 창고 메시지 있는지 검사
	if (ch->m_stashMsg.m_mtype != MSG_UNKNOWN)
	{
		CNetMsg rmsg;
		StashErrorMsg(rmsg, MSG_STASH_ERROR_NOTCOMPLETE);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		CLCString stashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
		msg >> stashPassword;
		stashPassword.Trim();
		int length = stashPassword.Length();

		// 인스리아 창고 주민번호 대신에 비밀번호 푸는 암호 사용
#ifdef LC_TWN
		CLCString stashKey(14);
		msg >> stashKey;
		stashKey.Trim();
		int length2 = stashKey.Length();

		if(length > MAX_STASH_PASSWORD_LENGTH || length < 4 || length2 > 13 || length2 < 8)
#else
		if(length > MAX_STASH_PASSWORD_LENGTH || length < 4 )
#endif
		{
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_PASSWORD);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 잘못된 문자 찾기
#ifdef LC_TWN
		if (strinc(stashPassword, "'") || strinc(stashPassword, "%") || strinc(stashPassword, " ")
			|| strinc(stashKey, "'") || strinc(stashKey, "%") || strinc(stashKey, " "))
#else
		if (strinc(stashPassword, "'") || strinc(stashPassword, "%") || strinc(stashPassword, " "))
#endif
		{
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_CHAR);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			// 컨넥터를 통해 암호 변경
			CNetMsg rmsg;
#ifdef LC_TWN
			ConnStashSetNewPassword(rmsg, ch->m_desc->m_index, ch->m_index, stashPassword, stashKey);
#else
			ConnStashSetNewPassword(rmsg, ch->m_desc->m_index, ch->m_index, stashPassword);
#endif
			if (IS_RUNNING_CONN)
			{
				SEND_Q(rmsg, gserver.m_connector);
				ch->m_stashMsg.Init(msg);
			}
			else
			{
				CNetMsg rmsg;
				StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
}

void do_StashCheckPassword(CPC* ch, CNetMsg& msg)
{
	// 이전에 처리중인 창고 메시지 있는지 검사
	if (ch->m_stashMsg.m_mtype != MSG_UNKNOWN)
	{
		CNetMsg rmsg;
		StashErrorMsg(rmsg, MSG_STASH_ERROR_NOTCOMPLETE);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		CLCString stashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
		msg >> stashPassword;
		stashPassword.Trim();

		// 컨넥터를 통해 암호 검사
		CNetMsg rmsg;
		ConnStashCheckPassword(rmsg, ch->m_desc->m_index, ch->m_index, stashPassword);
		if (IS_RUNNING_CONN)
		{
			SEND_Q(rmsg, gserver.m_connector);
			ch->m_stashMsg.Init(msg);
		}
		else
		{
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	
}
void do_StashDeletePassword(CPC* ch, CNetMsg& msg)
{

	/*
	MSG_STASH_DELETE_PASSWORD_REQ,				// 암호분실시 삭제 요청		: idnum(str)
	MSG_STASH_DELETE_PASSWORD_REP,				// 암호분실 결과			: success(c:s)

	MSG_CONN_STASH_DELETE_PASSWORD, // 창고 암호 삭제	: userindex(n) charindex(n) valid(c:cs)
														: 0: 성공, 1: 민증번호틀림*/

// 이전에 처리중인 창고 메시지 있는지 검사
	if (ch->m_stashMsg.m_mtype != MSG_UNKNOWN)
	{
		CNetMsg rmsg;
		StashErrorMsg(rmsg, MSG_STASH_ERROR_NOTCOMPLETE);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		// 컨넥터를 통해 암호 삭제
		CLCString stashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
		msg >> stashPassword;
		stashPassword.Trim();

		CNetMsg rmsg;
		ConnStashDeletePassword(rmsg, ch->m_desc->m_index, ch->m_index, stashPassword);
		if (IS_RUNNING_CONN)
		{
			SEND_Q(rmsg, gserver.m_connector);
			ch->m_stashMsg.Init(msg);
		}
		else
		{
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
			SEND_Q(rmsg, ch->m_desc);
		}
	}

}

void do_StashIsSetPassword(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	ConnStashState(rmsg, ch->m_desc->m_index, ch->m_index);
	if (IS_RUNNING_CONN)
	{
		SEND_Q(rmsg, gserver.m_connector);
		ch->m_stashMsg.Init();
	}
	else
	{
		CNetMsg rmsg;
		StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_StashListReq(CPC* ch, CNetMsg& msg)
{
	// 이전에 처리중인 창고 메시지 있는지 검사
	if (ch->m_stashMsg.m_mtype != MSG_UNKNOWN)
	{
		CNetMsg rmsg;
		StashErrorMsg(rmsg, MSG_STASH_ERROR_NOTCOMPLETE);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		CLCString stashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
		msg >> stashPassword;
		stashPassword.Trim();

		// 컨넥터를 통해 암호 검사
		CNetMsg rmsg;
		ConnStashCheckPassword(rmsg, ch->m_desc->m_index, ch->m_index, stashPassword);
		if (IS_RUNNING_CONN)
		{
			SEND_Q(rmsg, gserver.m_connector);
			ch->m_stashMsg.Init(msg);
		}
		else
		{
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_StashListReq_real(CPC* ch)
{
	// 창고에 있는 아이템 구하기
	const CItem* item[MAX_STASH_ITEM];
	int count = 0;
	int i;
	for (i = 0; i < MAX_STASH_ITEM; i++)
	{
		const CItem* pitem = ch->m_stash.item(i);
		if (pitem)
		{
			item[count] = pitem;
			count++;
		}
	}

	int remain = 0;
	remain = (ch->m_stashextTime - gserver.m_gameTime);
	if (remain <= 0)
	{
		remain = 0;
	}
	else
	{
		remain = remain / OUTPUTDAY;
		if (remain == 0)
		{
			remain = 1;
		}
	}

	CNetMsg rmsg;
	if (count == 0)
	{
		// 없으면, 비어있다고 보내고
		StashListRepMsg(rmsg, NULL, true, true, true, remain);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		// 있으면 하나하나 보내기
		for (i = 0; i < count; i++)
		{
			// i 가 0이면 시작, i가 count -1이면 끝
			StashListRepMsg(rmsg, item[i], (i == 0) ? true : false, (i == count - 1) ? true : false, false, remain);
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	// 처리중인 메시지는 없어짐 -> ProcessBilling()
}

void do_StashKeep(CPC* ch, CNetMsg& msg)
{
	// 이전에 처리중인 창고 메시지 있는지 검사
	if (ch->m_stashMsg.m_mtype != MSG_UNKNOWN)
	{
		CNetMsg rmsg;
		StashErrorMsg(rmsg, MSG_STASH_ERROR_NOTCOMPLETE);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		CLCString stashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
		msg >> stashPassword;
		stashPassword.Trim();

		// 컨넥터를 통해 암호 검사
		CNetMsg rmsg;
		ConnStashCheckPassword(rmsg, ch->m_desc->m_index, ch->m_index, stashPassword);
		if (IS_RUNNING_CONN)
		{
			SEND_Q(rmsg, gserver.m_connector);
			ch->m_stashMsg.Init(msg);
		}
		else
		{
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_StashKeep_real(CPC* ch)
{
	CNetMsg rmsg;

	CNetMsg& msg = ch->m_stashMsg;
	msg.MoveFirst();
	unsigned char subtype = (unsigned char)-1;
	msg >> subtype;

	if (subtype == MSG_STASH_KEEP_REQ)
	{
		CLCString stashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
		int keepcount = 0;

		msg >> stashPassword
			>> keepcount;

		// 전체 보관할 아이템 종류 수 검사
		if (keepcount < 1 || keepcount > MAX_STASH_KEEP)
		{
			StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_PACKET);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			bool bFail = false;
			CItem* item[MAX_STASH_KEEP];
			LONGLONG count[MAX_STASH_KEEP];
			LONGLONG needmoney = 0;
			LONGLONG keepmoney = 0;

			// 보관할 아이템 검사
			int i;
			for (i = 0; i < keepcount; i++)
			{
				char row = -1;
				char col = -1;
				int itemindex = -1;
				LONGLONG itemcount = -1;

				msg >> row
					>> col
					>> itemindex
					>> itemcount;

				if (row < 0 || col < 0 || itemindex < 1 || itemcount < 1)
				{
					bFail = true;
					break;
				}
				else
				{
					CItem* pitem = ch->m_invenNormal.GetItem(row, col);
					if (pitem == NULL || pitem->m_index != itemindex || pitem->Count() < itemcount || !pitem->CanKeepStash())
					{
						bFail = true;
						break;
					}
					else
					{
                        int j;  
                        for (j = 0; j < i; j++) 
                        {       
                            if (item[j]->m_index == itemindex)
                            {       
                                bFail = true; 
                                break;  
                            }       
                        }       
                        if (bFail) 
                            break;  

						item[i] = pitem;
						count[i] = itemcount;
						if (pitem->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index)
						{
							keepmoney += itemcount;
						}
						else if (pitem->m_itemProto->m_weight > 0)
						{
							needmoney += pitem->m_itemProto->m_weight * itemcount;
						}
					}
				}
			}

			if (bFail)
			{
				StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_PACKET);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				// 보관료와 보관하려는 돈의 액수이상 소유하고 있는지 검사
				if ((needmoney + keepmoney > 0 && ch->m_moneyItem && ch->m_moneyItem->Count() >= needmoney + keepmoney) || (needmoney + keepmoney == 0))
				{
					// 완전히 빠지고, 창고에 새로 등록될 아이템 검사
					bool bDel[MAX_STASH_KEEP];
					bool bAdd[MAX_STASH_KEEP];
					int addcount = 0;
					int itemdbindex[MAX_STASH_KEEP];

					for (i = 0; i < keepcount; i++)
					{
						if (item[i]->Count() == count[i])
						{
							bDel[i] = true;
						}
						else
						{
							bDel[i] = false;
						}

						if ((item[i]->m_itemProto->m_flag & ITEM_FLAG_COUNT) && ch->m_stash.find(item[i]->m_idNum, item[i]->m_plus, item[i]->m_flag) != -1)
						{
							bAdd[i] = false;
						}
						else
						{
							bAdd[i] = true;
							addcount++;
						}
						itemdbindex[i] = item[i]->m_idNum;
					}

					// 창고 공간 검사
					int remain = 0;
					
					remain = ch->m_stashextTime - gserver.m_gameTime;

					if(remain < 0)
						remain = 0;
					
					if (ch->m_stash.space(remain == 0 ? false : true) < addcount)
					{
						StashErrorMsg(rmsg, MSG_STASH_ERROR_STASH_FULL);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						int nStashIdx = -1;

						// 넣고 빼고
						GAMELOG << init("STASH KEEP", ch)
								<< "NEED" << delim << needmoney << delim;
						for (i = 0; i < keepcount; i++)
						{
							GAMELOG << "KEEP COUNT" << delim
									<< count[i] << delim
									<< "ITEM" << delim
									<< itemlog(item[i]) << delim;

							if (bDel[i])
							{
								// 완전히 빼기
								ItemDeleteMsg(rmsg, item[i]);
								SEND_Q(rmsg, ch->m_desc);
								RemoveFromInventory(ch, item[i], false, true);

								// 넣기
								if (bAdd[i])
								{
									// 새로 추가
									nStashIdx = ch->m_stash.add(item[i]);
								}
								else
								{
									// 겹쳐 넣기
									nStashIdx = ch->m_stash.add(item[i]->m_idNum, item[i]->m_plus, item[i]->m_flag, count[i]);
									delete item[i];
								}
							}
							else
							{
								// 일부분 빼기
								DecreaseFromInventory(ch, item[i], count[i]);
								ItemUpdateMsg(rmsg, item[i], -count[i]);
								SEND_Q(rmsg, ch->m_desc);

								// 일부분 빠지는 것은 겹쳐지는 아이템이다
								// 겹쳐 넣고, 새로 추가하는 것은 CStash::add()에서 알아서 함!
								nStashIdx = ch->m_stash.add(item[i]->m_idNum, item[i]->m_plus, item[i]->m_flag, count[i]);
							}

							CItem * pItem = ch->m_stash.item(nStashIdx);
							if (pItem)
							{
								GAMELOG << "REST COUNT" << delim << pItem->Count() << delim;
							}
						}

						GAMELOG << end;

						// 돈 줄이고
						if (needmoney > 0)
						{
							if (ch->m_moneyItem->Count() == needmoney)
							{
								ItemDeleteMsg(rmsg, ch->m_moneyItem);
								SEND_Q(rmsg, ch->m_desc);
								ch->AddMoney(-needmoney);
							}
							else
							{
								ch->AddMoney(-needmoney);
								ItemUpdateMsg(rmsg, ch->m_moneyItem, -needmoney);
								SEND_Q(rmsg, ch->m_desc);
							}
						}

						// 결과 알리기
						StashKeepRepMsg(rmsg, keepcount, itemdbindex, count);
						SEND_Q(rmsg, ch->m_desc);

						// 바로 저장
						ch->SaveNow(true);
					}
				}
				else
				{
					StashErrorMsg(rmsg, MSG_STASH_ERROR_MONEY);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}
	}

	// 처리중인 메시지는 없어짐 -> ProcessBilling()
}

void do_StashTake(CPC* ch, CNetMsg& msg)
{
	// 이전에 처리중인 창고 메시지 있는지 검사
	if (ch->m_stashMsg.m_mtype != MSG_UNKNOWN)
	{
		CNetMsg rmsg;
		StashErrorMsg(rmsg, MSG_STASH_ERROR_NOTCOMPLETE);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		CLCString stashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
		msg >> stashPassword;
		stashPassword.Trim();

		// 컨넥터를 통해 암호 검사
		CNetMsg rmsg;
		ConnStashCheckPassword(rmsg, ch->m_desc->m_index, ch->m_index, stashPassword);
		if (IS_RUNNING_CONN)
		{
			SEND_Q(rmsg, gserver.m_connector);
			ch->m_stashMsg.Init(msg);
		}
		else
		{
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_StashTake_real(CPC* ch)
{
	CNetMsg rmsg;

	CNetMsg& msg = ch->m_stashMsg;
	msg.MoveFirst();
	unsigned char subtype = (unsigned char)-1;
	msg >> subtype;

	if (subtype == MSG_STASH_TAKE_REQ)
	{
		CLCString stashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
		int takecount = 0;

		msg >> stashPassword
			>> takecount;

		// 찾을 수 검사
		if (takecount < 1 || takecount > MAX_STASH_TAKE)
		{
			StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_PACKET);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			bool bfail = false;
			CItem* item[MAX_STASH_TAKE];
			LONGLONG count[MAX_STASH_TAKE];
			int weight = 0;

			// 찾을 아이템 검사
			int i;
			for (i = 0; i < takecount; i++)
			{
				int itemindex = -1;
				LONGLONG itemcount = -1;

				msg >> itemindex
					>> itemcount;

				if (itemindex < 1 || itemcount < 1)
				{
					bfail = true;
					break;
				}
				else
				{
					CItem* pitem = ch->m_stash.item(ch->m_stash.find(itemindex));
					if (pitem == NULL || pitem->Count() < itemcount)
					{
						bfail = true;
						break;
					}
					else
					{
                        int j;  
                        for (j = 0; j < i; j++) 
                        {       
                            if (item[j]->m_index == itemindex)
                            {       
                                bfail = true; 
                                GAMELOG << init("HACK STASH", ch) << end;
                                break;  
                            }       
                        }       
                        if (bfail)
                            break;
						
						item[i] = pitem;
						count[i] = itemcount;
						weight += pitem->m_itemProto->m_weight * itemcount;
					}
				}
			}

			if (bfail)
			{
				StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_PACKET);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				// 무게 검사
				if (ch->m_weight + weight >= ch->m_maxWeight * 15 / 10)
				{
					StashErrorMsg(rmsg, MSG_STASH_ERROR_WEIGHT_OVER);
					SEND_Q(rmsg, ch->m_desc);
				}
				else
				{
					// 창고에서 완전히 빠지고, 인벤에 새로 추가될 아이템 검사
					bool bdel[MAX_STASH_TAKE];
					bool badd[MAX_STASH_TAKE];
					int addcount = 0;
					int itemdbindex[MAX_STASH_TAKE];
					int row, col;

					for (i = 0; i < takecount; i++)
					{
						if (item[i]->Count() == count[i])
						{
							bdel[i] = true;
						}
						else
						{
							bdel[i] = false;
						}

						if ((item[i]->m_itemProto->m_flag & ITEM_FLAG_COUNT) && ch->m_invenNormal.FindItem(&row, &col, item[i]->m_idNum, item[i]->m_plus, item[i]->m_flag))
						{
							badd[i] = false;
						}
						else
						{
							badd[i] = true;
							addcount++;
						}
						itemdbindex[i] = item[i]->m_idNum;
					}

					// 인벤 공간 검사
					if (ch->m_invenNormal.GetSpace() < addcount)
					{
						StashErrorMsg(rmsg, MSG_STASH_ERROR_INVENTORY_FULL);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						// 넣고 빼고
						GAMELOG << init("STASH TAKE", ch);
						for (i = 0; i < takecount; i++)
						{
							GAMELOG << "TAKE COUNT" << delim
									<< count[i] << delim
									<< "ITEM" << delim
									<< itemlog(item[i]) << delim;

							if (bdel[i])
							{
								// 완전히 빼기
								ch->m_stash.remove(item[i]->m_index);

								// 넣기
								if (badd[i])
								{
									// 새로 추가되는 것은 아이템 통째로 넣고
									AddToInventory(ch, item[i], false, true);
									ItemAddMsg(rmsg, item[i]);
									SEND_Q(rmsg, ch->m_desc);
								}
								else
								{
									// 완전히 빼서 겹쳐서 넣기이므로 item[i]를 넣고 지운다
									AddToInventory(ch, item[i], false, true);
									ch->m_invenNormal.FindItem(&row, &col, item[i]->m_idNum, item[i]->m_plus, item[i]->m_flag);
									CItem* pitem = ch->m_invenNormal.GetItem(row, col);
									ItemUpdateMsg(rmsg, pitem, count[i]);
									SEND_Q(rmsg, ch->m_desc);
									delete item[i];
								}

								GAMELOG << "REST COUNT" << delim << "take all" << delim;
							}
							else
							{
								int nCount = 0;
								// 일부분 빼기
								nCount = ch->m_stash.remove(item[i]->m_index, count[i]);

								// 일부분 빼서 넣기이므로 일단 떼어낸 것을 만든다 : 당현히 겹치는 아이템이다
								CItem* pitem = gserver.m_itemProtoList.CreateItem(item[i]->m_idNum, -1, item[i]->m_plus, item[i]->m_flag, count[i]);

								// 넣기
								if (badd[i])
								{
									// 새로 추가하는 것은 인벤에 넣고 끝
									AddToInventory(ch, pitem, false, true);
									ItemAddMsg(rmsg, pitem);
									SEND_Q(rmsg, ch->m_desc);
								}
								else
								{
									// 일부 빼서 일부 넣기는 생성된 아이템을 넣고 지운다
									AddToInventory(ch, pitem, false, true);
									ch->m_invenNormal.FindItem(&row, &col, pitem->m_idNum, pitem->m_plus, pitem->m_flag);
									CItem* pitem2 = ch->m_invenNormal.GetItem(row, col);
									ItemUpdateMsg(rmsg, pitem2, count[i]);
									SEND_Q(rmsg, ch->m_desc);
									delete pitem;
								}

								GAMELOG << "REST COUNT" << delim << nCount << delim;
							}
						}
						GAMELOG << end;

						StashTakeRepMsg(rmsg, takecount, itemdbindex, count);
						SEND_Q(rmsg, ch->m_desc);

						// 바로 저장
						ch->SaveNow(true);
					}
				}
			}
		}
	}

	// 처리중인 메시지는 없어짐 -> ProcessBilling()
}

void do_StashChangePassword(CPC* ch, CNetMsg& msg)
{
	// 이전에 처리중인 창고 메시지 있는지 검사
	if (ch->m_stashMsg.m_mtype != MSG_UNKNOWN)
	{
		CNetMsg rmsg;
		StashErrorMsg(rmsg, MSG_STASH_ERROR_NOTCOMPLETE);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		CLCString oldStashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
		CLCString newStashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
		msg >> oldStashPassword
			>> newStashPassword;
		oldStashPassword.Trim();
		newStashPassword.Trim();

		// 잘못된 문자 찾기
		if (strinc(newStashPassword, "'") || strinc(newStashPassword, "%"))
		{
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_CHAR);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			// 컨넥터를 통해 암호 변경
			CNetMsg rmsg;
			ConnStashChangePassword(rmsg, ch->m_desc->m_index, ch->m_index, oldStashPassword, newStashPassword);
			if (IS_RUNNING_CONN)
			{
				SEND_Q(rmsg, gserver.m_connector);
				ch->m_stashMsg.Init(msg);
			}
			else
			{
				CNetMsg rmsg;
				StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
}

void do_StashSeal(CPC* ch, CNetMsg& msg)
{
	// 컨넥터를 통해 암호 변경
	CNetMsg rmsg;
	ConnStashSeal(rmsg, ch->m_desc->m_index, ch->m_index);
	if (IS_RUNNING_CONN)
	{
		SEND_Q(rmsg, gserver.m_connector);
		ch->m_stashMsg.Init(msg);
	}
	else
	{
		CNetMsg rmsg;
		StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_SEAL);
		SEND_Q(rmsg, ch->m_desc);
	}
}
