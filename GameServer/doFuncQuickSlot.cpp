#include "stdhdrs.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"


///////////////////////
// Quick Slot ฐทร วิผ๖

void do_QuickSlot(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	char subtype, page;
	CNetMsg quickSlotMsg;
	
	msg >> subtype;
	
	switch (subtype)
	{
	case MSG_QUICKSLOT_ADD:
		{
			char slot, slotType;
			msg >> page
				>> slot
				>> slotType;

			if (page < 0 || page >= QUICKSLOT_PAGE_NUM || slot < 0 || slot >= QUICKSLOT_MAXSLOT)
				return ;
			
			ch->m_quickSlot[(int)page].m_slotType[(int)slot] = (int)slotType;
			
			switch (slotType)
			{
			case QUICKSLOT_TYPE_EMPTY:
				
				ch->m_quickSlot[(int)page].m_slotType[(int)slot] = -1;
				ch->m_quickSlot[(int)page].m_skillType[(int)slot] = -1;
				ch->m_quickSlot[(int)page].m_actionType[(int)slot] = -1;
				ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
				break;
				
			case QUICKSLOT_TYPE_SKILL:
				{
					int skillType;
					msg >> skillType;

					if (skillType > 0)
					{
						if (ch->m_activeSkillList.Find(skillType))
						{
							ch->m_quickSlot[(int)page].m_skillType[(int)slot] = skillType;
							ch->m_quickSlot[(int)page].m_actionType[(int)slot] = -1;
							ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
						}
#ifdef ENABLE_PET
						else if (ch->GetPet() && ch->GetPet()->FindSkill(skillType))
						{
							ch->m_quickSlot[(int)page].m_skillType[(int)slot] = skillType;
							ch->m_quickSlot[(int)page].m_actionType[(int)slot] = -1;
							ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
						}
#endif // #ifdef ENABLE_PET
#ifdef ATTACK_PET
						else if ( ch->GetAPet() && ch->GetAPet()->m_skillList.Find( skillType) )
						{
							ch->m_quickSlot[(int)page].m_skillType[(int)slot] = skillType;
							ch->m_quickSlot[(int)page].m_actionType[(int)slot] = -1;
							ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
						}
#endif//ATTACK_PET
					}
					else
						ch->m_quickSlot[(int)page].m_slotType[(int)slot] = -1;
				}
				break;
				
			case QUICKSLOT_TYPE_ACTION:
				{
					int actionType;
					msg >> actionType;

					ch->m_quickSlot[(int)page].m_skillType[(int)slot] = -1;
					ch->m_quickSlot[(int)page].m_actionType[(int)slot] = actionType;
					ch->m_quickSlot[(int)page].m_item[(int)slot] = NULL;
				}
				break;
				
			case QUICKSLOT_TYPE_ITEM:
				{
					char row, col;
					msg >> row
						>> col;
					
					CItem* item = ch->m_invenNormal.GetItem((int)row, (int)col);
					if (item)
						ch->m_quickSlot[(int)page].m_item[(int)slot] = item;
					else
					{
						ch->m_quickSlot[(int)page].m_slotType[(int)slot] = QUICKSLOT_TYPE_EMPTY;
						return ;
					}
				}

				break;

			default:
				break;
			}

			QuickSlotAddMsg(quickSlotMsg, ch, page, slot);
			SEND_Q(quickSlotMsg, ch->m_desc);
		}
		break;

	case MSG_QUICKSLOT_SWAP:
		{
			char slot1, slot2;

			msg >> page
				>> slot1
				>> slot2;

			if (page < 0 || page >= QUICKSLOT_PAGE_NUM || slot1 < 0 || slot1 >= QUICKSLOT_MAXSLOT ||  slot2 < 0 || slot2 >= QUICKSLOT_MAXSLOT)
				return;

			int tSlotType, tSkillType, tActionType;
			CItem* tItem;

			tSlotType = ch->m_quickSlot[(int)page].m_slotType[(int)slot1];
			tSkillType = ch->m_quickSlot[(int)page].m_skillType[(int)slot1];
			tActionType = ch->m_quickSlot[(int)page].m_actionType[(int)slot1];
			tItem = ch->m_quickSlot[(int)page].m_item[(int)slot1];

			ch->m_quickSlot[(int)page].m_slotType[(int)slot1] = ch->m_quickSlot[(int)page].m_slotType[(int)slot2];
			ch->m_quickSlot[(int)page].m_skillType[(int)slot1] = ch->m_quickSlot[(int)page].m_skillType[(int)slot2];
			ch->m_quickSlot[(int)page].m_actionType[(int)slot1] = ch->m_quickSlot[(int)page].m_actionType[(int)slot2];
			ch->m_quickSlot[(int)page].m_item[(int)slot1] = ch->m_quickSlot[(int)page].m_item[(int)slot2];

			ch->m_quickSlot[(int)page].m_slotType[(int)slot2] = tSlotType;
			ch->m_quickSlot[(int)page].m_skillType[(int)slot2] = tSkillType;
			ch->m_quickSlot[(int)page].m_actionType[(int)slot2] = tActionType;
			ch->m_quickSlot[(int)page].m_item[(int)slot2] = tItem;

			QuickSlotAddMsg(quickSlotMsg, ch, page, slot1);
			SEND_Q(quickSlotMsg, ch->m_desc);

			QuickSlotAddMsg(quickSlotMsg, ch, page, slot2);
			SEND_Q(quickSlotMsg, ch->m_desc);
		}
		break;
		
	default:
		break;
	}
}
