#include "stdhdrs.h"
#include "Server.h"
#include "Cell.h"

CCell::CCell()
{
	m_listChar = NULL;
	m_listItem = NULL;
	m_nCountNPC = 0;
	m_nCountPC = 0;
#ifdef NPC_REGEN_ADJUST
	m_nTotalRegen = 0;
#endif // NPC_REGEN_ADJUST
}

CCell::~CCell()
{
}

void CCell::CharToCell(CCharacter* ch)
{
	if (IS_IN_CELL(ch))
		return;

	ADD_TO_BILIST(ch, m_listChar, m_pCellPrev, m_pCellNext);
	
	if (IS_NPC(ch))
		m_nCountNPC++;
	else if (IS_PC(ch))
		m_nCountPC++;
}

void CCell::CharFromCell(CCharacter* ch)
{
	if (!IS_IN_CELL(ch))
		return ;
	
	REMOVE_FROM_BILIST(ch, m_listChar, m_pCellPrev, m_pCellNext);
	
	if (IS_NPC(ch))
		m_nCountNPC--;
	else if (IS_PC(ch))
		m_nCountPC--;
}

bool CCell::Send(CNetMsg &msg, CPC* ch)
{
	CCharacter* pChar;
	CCharacter* pNext = m_listChar;
	while ((pChar = pNext))
	{
		pNext = pChar->m_pCellNext;
		if (pChar == ch)
			continue ;
		
		if (IS_PC(pChar))
			SEND_Q(msg, TO_PC(pChar)->m_desc);
	} // end while

	return true;
}

void CCell::ItemToCell(CItem* item)
{
	if (IS_IN_CELL(item))
		return;

	ADD_TO_BILIST(item, m_listItem, m_pCellPrev, m_pCellNext);
}

void CCell::ItemFromCell(CItem* item)
{
	if (!IS_IN_CELL(item))
		return ;
	
	REMOVE_FROM_BILIST(item, m_listItem, m_pCellPrev, m_pCellNext);
}

