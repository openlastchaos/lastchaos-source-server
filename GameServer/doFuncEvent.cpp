#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "hardcoding.h"

#ifdef EVENT_XMAS_2007
void do_EventXmas2007( CPC* ch, CNetMsg& msg );
#endif //EVENT_XMAS_2007

#ifdef EVENT_SSHOT_2007
void do_EventSshot2007( CPC* ch, CNetMsg& msg );
#endif //EVENT_SSHOT_2007

#ifdef EVENT_BJMONO_2007
void do_EventBjMono2007( CPC* ch, CNetMsg& msg );
#endif //EVENT_BJMONO_2007

#if defined( EVENT_SUMMER_VACATION_2007_RESULT ) || defined (EVENT_SUMMER_VACATION_2007 )
void do_EventSummerVacation2007( CPC* ch, CNetMsg& msg );
#endif// defined( EVENT_SUMMER_VACATION_2007_RESULT ) || defined (EVENT_SUMMER_VACATION_2007 )

#ifdef EVENT_FLOWERTREE_2007
void do_EventFlowerTree2007( CPC* ch, CNetMsg& msg);
#endif // EVENT_FLOWERTREE_2007 

#ifdef EVENT_CHILDRENSDAY_2007 
void do_EventChildrensDay_2007(CPC* ch, CNetMsg& msg);
#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_GOMDORI_2007
void do_EventGomdori2007(CPC* ch, CNetMsg& msg);
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_2007_PARENTSDAY
void do_EventParentsday2007( CPC* ch, CNetMsg& msg );
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_WHITEDAY_2007 
void do_EventWhiteday2007(CPC* ch, CNetMsg& msg);
#endif // EVENT_WHITEDAY_2007 [3/6/2007 KwonYongDae] 

#if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
void do_EventValentine2007(CPC* ch, CNetMsg& msg);
bool BINGO(int plus, int* score, int* bingocount);
#endif  // #if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 

#ifdef EVENT_XMAS_2006
void do_EventXmas2006(CPC* ch, CNetMsg& msg);
#endif // EVENT_XMAS_2006

#ifdef EVENT_CHUSEOK_2006_GIFT
void do_EventChuseok2006(CPC* ch, CNetMsg& msg);
#endif // EVENT_CHUSEOK_2006_GIFT
void do_EventSuperstone(CPC* ch, CNetMsg &msg);
#ifdef EVENT_TREASUREBOX
void do_EventTreasurebox(CPC* ch, CNetMsg &msg);
#endif // #ifdef EVENT_TREASUREBOX

#ifdef EVENT_TREASUREBOX_RED
void do_EventTreasurebox_red(CPC* ch, CNetMsg &msg);
#endif // EVENT_TREASUREBOX_RED

#ifdef EVENT_MOONSTONE
void do_EventMoonStone(CPC* ch, CNetMsg& msg);
#endif // #ifdef EVENT_MOONSTONE
void do_EventLatto(CPC* ch, CNetMsg& msg);
void do_EventChangeWeapon(CPC* ch, CNetMsg& msg);
void do_EventLetter(CPC* ch, CNetMsg& msg);
void do_EventChuseokUpgrade(CPC* ch, CNetMsg& msg);
void do_EventChuseokExchange(CPC* ch, CNetMsg& msg);
#ifdef EVENT_CHANGE_WITHOUT_OPTION
void do_EventChangeWithoutOption(CPC* ch, CNetMsg& msg);
#endif // #ifdef EVENT_CHANGE_WITHOUT_OPTION
#ifdef EVENT_2PAN4PAN_LETTER
#ifndef TEST_SERVER
void do_Event2pan4panLetter(CPC* ch, CNetMsg& msg);
#endif // #ifndef TEST_SERVER
#endif // #ifdef EVENT_2PAN4PAN_LETTER
#ifdef EVENT_2PAN4PAN_BOX
void do_Event2pan4panBoxCheck(CPC* ch, CNetMsg& msg);
void do_Event2pan4panBoxReq(CPC* ch, CNetMsg& msg);
#endif // #ifdef EVENT_2PAN4PAN_BOX
#ifdef EVENT_2PAN4PAN_GOODS
void do_Event2pan4panGoodsCheck(CPC* ch, CNetMsg& msg);
void do_Event2pan4panGoodsReq(CPC* ch, CNetMsg& msg);
#endif // #ifdef EVENT_2PAN4PAN_GOODS
#ifdef EVENT_2PAN4PAN_MONEY
void do_Event2pan4panMoneyReq(CPC* ch, CNetMsg& msg);
#endif // #ifdef EVENT_2PAN4PAN_MONEY
#ifdef EVENT_XMAS_2005
void do_EventXMas2005(CPC* ch, CNetMsg& msg);
#endif // #ifdef EVENT_XMAS_2005
#ifdef EVENT_NEWYEAR_2006
void do_EventNewYear2006Check(CPC* ch, CNetMsg& msg);
void do_EventNewYear2006Goods(CPC* ch, CNetMsg& msg);
#endif // #endif // #ifdef EVENT_XMAS_2005

#ifdef EVENT_SEARCHFRIEND
//휴면케릭 
void do_EventSearchFriendAdd(CPC* ch, CNetMsg& msg);       //케릭 등록
void do_EventSearchFriendAddSelect(CPC* ch, CNetMsg& msg);     //등록한 리스트중 선택하는 부분
void do_EventSearchFriendTimeCheck(CPC* ch, CNetMsg& msg);     // 사냥시간 체크
void do_EventSearchFriendListGoods(CPC* ch, CNetMsg& msg);     // 등록된 친구 보상가능여부
void do_EventSearchFriendGoods(CPC* ch, CNetMsg& msg);     // 아이템 지급 개인
#endif //#ifdef EVENT_SEARCHFRIEND  
#ifdef EVENT_PACKAGE_ITEM
void do_EventPackageItemGive(CPC* ch, CNetMsg& msg);
#endif

#ifdef NEW_SERIAL_PACKAGE_EVENT
void do_EventPackageItemGive_new( CPC* ch, CNetMsg& msg );
#endif // NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_SAKURA
void do_EventSakrabudGive(CPC* ch, CNetMsg& msg);
#endif
#ifdef EVENT_PCBANG
void do_EventPCbang(CPC* ch, CNetMsg& msg);
#endif

void do_EventWorldcup(CPC* ch, CNetMsg& msg);
void do_EventGoldenball(CPC* ch, CNetMsg& msg);

#ifdef EVENT_RAIN_2006
void do_EventRain2006(CPC* ch, CNetMsg& msg);
#endif // EVENT_RAIN_2006

#ifdef EVENT_TLD_BUDDHIST
void do_EventTldBuddhist(CPC* ch, CNetMsg& msg);
#endif // EVENT_TLD_BUDDHIST

#ifdef EVENT_COLLECT_BUG
void do_EventCollectBug(CPC* ch, CNetMsg& msg);
#endif // EVENT_COLLECT_BUG

#ifdef EVENT_TEACH_2007
void do_ExEventTeach2007(CPC* ch, CNetMsg& msg);
#endif // EVENT_TEACH_2007

#ifdef EVENT_TLD_MOTHERDAY_2007
void do_EventTldMotherday2007( CPC* ch, CNetMsg& msg );
#endif // EVENT_TLD_MOTHERDAY_2007

#if defined(EVENT_OPEN_ADULT_SERVER) || defined (EVENT_MAGIC_CARD)
void do_EventOpenAdultServer(CPC* ch, CNetMsg& msg);
#endif // EVENT_OPEN_ADULT_SERVER || defined (EVENT_MAGIC_CARD)

#ifdef GIFT_EVENT_2007
void do_EventGift2007(CPC* ch, CNetMsg& msg);
#endif // GIFT_EVENT_2007

#ifdef EVENT_LC_1000DAY
void do_EventLC1000Day( CPC* ch, CNetMsg& msg );
#endif // EVENT_LC_1000DAY

#ifdef EVENT_RICHYEAR_2007
void do_EventRichYear(CPC* ch, CNetMsg& msg);
#endif // EVENT_RICHYEAR_2007

#ifdef EVENT_HALLOWEEN_2007
void do_EventHalloween2007(CPC* ch, CNetMsg& msg);
#endif // EVENT_HALLOWEEN_2007

#ifdef EVENT_NEW_MOONSTONE
void do_EventNewMoonstone(CPC* ch, CNetMsg& msg);
#endif // EVENT_NEW_MOONSTONE

#ifdef NEWYEAR_EVENT_2008
void do_EventNewYear2008(CPC * ch, CNetMsg & msg);
#endif // NEWYEAR_EVENT_2008

#if defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)
void do_EventSakura2008(CPC* ch, CNetMsg& msg );
#endif //defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)

#ifdef EVENT_PHOENIX  // yhj   081105  피닉스
void do_EventPhoenix( CPC* ch, CNetMsg& msg );
void do_EventPhoenix_Success( CPC* ch, CNetMsg& msg );
#endif

#ifdef REQUITAL_EVENT	// 보상 이벤트 
void do_EventRequital( CPC* ch, CNetMsg& msg );
#endif // REQUITAL_EVENT

void do_Event(CPC* ch, CNetMsg& msg)
{
    msg.MoveFirst();
    unsigned char subtype;
    msg >> subtype;

    switch (subtype)
    {
#ifdef EVENT_XMAS_2007
    case MSG_EVENT_XMAS_2007 :
        do_EventXmas2007( ch, msg );
        break;
#endif // EVENT_XMAS_2007

#ifdef EVENT_HALLOWEEN_2007
    case MSG_EVENT_HALLOWEEN_2007:
        do_EventHalloween2007( ch, msg );
        break;
#endif //EVENT_HALLOWEEN_2007

#ifdef EVENT_SSHOT_2007
    case MSG_EVENT_SSHOT_2007:
        do_EventSshot2007( ch, msg );
        break;
#endif //EVENT_SSHOT_2007

#ifdef EVENT_LC_1000DAY
    case MSG_EVENT_LC_1000DAY:
        do_EventLC1000Day( ch, msg );
        break;
#endif // EVENT_LC_1000DAY

#ifdef EVENT_BJMONO_2007
    case MSG_EVENT_BJMONO_2007:
        do_EventBjMono2007( ch, msg );
        break;
#endif // EVENT_BJMONO_2007

#ifdef EVENT_TLD_MOTHERDAY_2007
    case MSG_EVENT_TLD_MOTHERDAY_2007:
        do_EventTldMotherday2007( ch, msg );
#endif // EVENT_TLD_MOTHERDAY_2007

#if defined( EVENT_SUMMER_VACATION_2007_RESULT ) || defined (EVENT_SUMMER_VACATION_2007 )
    case MSG_EVENT_SUMMER_VACATION_2007:
        do_EventSummerVacation2007( ch, msg );
        break;
#endif// defined( EVENT_SUMMER_VACATION_2007_RESULT ) || defined (EVENT_SUMMER_VACATION_2007 )

#ifdef EVENT_FLOWERTREE_2007
    case MSG_EVENT_FLOWERTREE_2007:
        do_EventFlowerTree2007( ch, msg );
        break;
#endif
#ifdef EVENT_GOMDORI_2007
    case MSG_EVENT_GOMDORI_2007:
        do_EventGomdori2007(ch, msg);
    break;
#endif // EVENT_GOMDORI_2007 

#ifdef EVENT_CHILDRENSDAY_2007
    case MSG_EVENT_CHILDRENSDAY_2007 :
        do_EventChildrensDay_2007(ch, msg);
        break;
#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_2007_PARENTSDAY
    case MSG_EVENT_PARENTSDAY_2007:
        do_EventParentsday2007( ch, msg );
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_WHITEDAY_2007 
    case MSG_EVENT_WHITEDAY_2007:
        do_EventWhiteday2007(ch, msg);
        break;
#endif //EVENT_WHITEDAY_2007 

#if defined (EVENT_VALENTINE_2007) || defined ( EVENT_VALENTINE_2007_GIFT )
    case MSG_EVENT_VALENTINE_2007:
        do_EventValentine2007(ch, msg);
#endif  // defined (EVENT_VALENTINE_2007) || defined ( EVENT_VALENTINE_2007_GIFT )

#ifdef EVENT_XMAS_2006
    case MSG_EVENT_XMAS_2006:
        do_EventXmas2006(ch, msg);
        break;
#endif // EVENT_XMAS_2006

#ifdef EVENT_CHUSEOK_2006_GIFT
    case MSG_EVENT_CHUSEOK_2006:
        do_EventChuseok2006(ch, msg);
        break;
#endif // EVENT_CHUSEOK_2006_GIFT

#ifdef EVENT_COLLECT_BUG
    case MSG_EVENT_COLLECT_BUG:
        do_EventCollectBug(ch, msg);
        break;
#endif // EVENT_COLLECT_BUG

#ifdef EVENT_TLD_BUDDHIST
    case MSG_EVENT_TLD_BUDDHIST:
        do_EventTldBuddhist(ch, msg);
        break;
#endif // EVENT_TLD_BUDDHIST

#ifdef EVENT_RAIN_2006
    case MSG_EVENT_RAIN_2006:
        do_EventRain2006(ch, msg);
        break;
#endif // EVENT_RAIN_2006

    case MSG_EVENT_GOLDENBALL:
        do_EventGoldenball(ch, msg);
        break;

    case MSG_EVENT_WORLDCUP:
        do_EventWorldcup(ch, msg);
        break;

#ifdef EVENT_PCBANG
    case MSG_EVENT_PCBANG:
        do_EventPCbang(ch, msg);
        break;
#endif
#ifdef EVENT_SAKURA
    case MSG_EVENT_SAKURABUD:
        do_EventSakrabudGive(ch, msg);
        break;
#endif

#ifdef NEW_SERIAL_PACKAGE_EVENT
    case MSG_NEW_EVENT_PACKAGE_ITEM_GIVE:
        do_EventPackageItemGive_new(ch, msg );
        break;
#endif // NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_PACKAGE_ITEM
//#ifdef NEW_SERIAL_PACKAGE_EVENT
//  case MSG_EVENT_PACKAGE_ITEM_GIVE:
//      do_EventPackageItemGive_new( ch, msg );
//      break;
//#else
    case MSG_EVENT_PACKAGE_ITEM_GIVE:
        do_EventPackageItemGive(ch, msg);
        break;
//#endif // NEW_SERIAL_PACKAGE_EVENT
#endif // EVENT_PACKAGE_ITEM

#ifdef EVENT_SEARCHFRIEND
    case MSG_EVENT_SEARCHFRIEND_ADD_CHECK: 
        do_EventSearchFriendAdd(ch, msg); // 휴면 케릭을 등록부분 
        break;
    case MSG_EVENT_SEARCHFRIEND_ADDSELECT_CHECK: 
        do_EventSearchFriendAddSelect(ch, msg); // 등록한 리스트중 선택하는 부분 
        break;
    case MSG_EVENT_SEARCHFRIEND_TIME_CHECK:
        do_EventSearchFriendTimeCheck(ch, msg); // 휴면 케릭 사냥시간 체크
        break;
    case MSG_EVENT_SEARCHFRIEND_LIST_GOODS:     // 등록된 친구 보상가능여부
        do_EventSearchFriendListGoods(ch, msg);
        break;
    case MSG_EVENT_SEARCHFRIEND_GOODS_CHECK:        // 아이템 지급 보상.
        do_EventSearchFriendGoods(ch, msg);
        break;
#endif //#ifdef EVENT_SEARCHFRIEND
#ifdef EVENT_NEWYEAR_2006
    case MSG_EVENT_NEWYEAR_2006_CHECK:
        do_EventNewYear2006Check(ch, msg);
        break;
    case MSG_EVENT_NEWYEAR_2006_GOODS:
        do_EventNewYear2006Goods(ch, msg);
        break;
#endif // #ifdef EVENT_NEWYEAR_2006

#ifdef EVENT_XMAS_2005
    case MSG_EVENT_XMAS_2005:
        do_EventXMas2005(ch, msg);
        break;
#endif // #ifdef EVENT_XMAS_2005

#ifdef EVENT_2PAN4PAN_MONEY
    case MSG_EVENT_2PAN4PAN_MONEY_REQ:
        do_Event2pan4panMoneyReq(ch, msg);
        break;
#endif // #ifdef EVENT_2PAN4PAN_MONEY

#ifdef EVENT_2PAN4PAN_BOX
    case MSG_EVENT_2PAN4PAN_BOX_CHECK:
        do_Event2pan4panBoxCheck(ch, msg);
        break;
    case MSG_EVENT_2PAN4PAN_BOX_REQ:
        do_Event2pan4panBoxReq(ch, msg);
        break;
#endif // #ifdef EVENT_2PAN4PAN_BOX

#ifdef EVENT_2PAN4PAN_GOODS
    case MSG_EVENT_2PAN4PAN_GOODS_CHECK:
        do_Event2pan4panGoodsCheck(ch, msg);
        break;
    case MSG_EVENT_2PAN4PAN_GOODS_REQ:
        do_Event2pan4panGoodsReq(ch, msg);
        break;
#endif // #ifdef EVENT_2PAN4PAN_GOODS

#ifdef EVENT_2PAN4PAN_LETTER
#ifndef TEST_SERVER
    case MSG_EVENT_2PAN4PAN_LETTER:
        do_Event2pan4panLetter(ch, msg);
        break;
#endif // #ifndef TEST_SERVER
#endif // #ifdef EVENT_2PAN4PAN_LETTER

#ifdef EVENT_CHANGE_WITHOUT_OPTION
    case MSG_EVENT_CHANGE_WITHOUT_OPTION:
        do_EventChangeWithoutOption(ch, msg);
        break;
#endif // #ifdef EVENT_CHANGE_WITHOUT_OPTION

    case MSG_EVENT_CHANGEWEAPON:
        do_EventChangeWeapon(ch, msg);
        break;

    case MSG_EVENT_LETTER:
        do_EventLetter(ch, msg);
        break;

    case MSG_EVENT_PREOPEN_GIFT:
        if (IS_RUNNING_CONN)
        {
#ifdef EVENT_PREOPEN_GIFT
            CNetMsg rmsg;
            ConnPreopenGiftMsg(rmsg, ch->m_desc->m_index, ch->m_index, false);
            SEND_Q(rmsg, gserver.m_connector);
#endif // EVENT_PREOPEN_GIFT
        }
        break;

        // 하드 코딩
    case MSG_EVENT_LATTO:
        do_EventLatto(ch, msg);
        break;
//0502 kwon
    case MSG_EVENT_MOONSTONE:
#ifdef EVENT_MOONSTONE
        do_EventMoonStone(ch, msg);
#endif // EVENT_MOONSTONE
        break;

    case MSG_EVENT_TREASUREBOX:
#ifdef EVENT_TREASUREBOX        
        do_EventTreasurebox(ch, msg);
#endif // EVENT_TREASUREBOX
        break;
        
#ifdef EVENT_TREASUREBOX_RED
    case MSG_EVENT_TREASUREBOX_RED:
        do_EventTreasurebox_red(ch, msg);
        break;
#endif // EVENT_TREASUREBOX_RED

    //0627
    case MSG_EVENT_SUPERSTONE:
        do_EventSuperstone(ch, msg);
        break;

    case MSG_EVENT_CHUSEOK_UPGRADE:
        do_EventChuseokUpgrade(ch, msg);
        break;

    case MSG_EVENT_CHUSEOK_EXCHANGE:
        do_EventChuseokExchange(ch, msg);
        break;

    case MSG_EVENT_OPEN_BETA_GIFT:
        if (IS_RUNNING_CONN)
        {
#ifdef EVENT_OPEN_BETA_GIFT
            CNetMsg rmsg;
//          ConnOpenBetaGiftMsg(rmsg, ch->m_desc->m_index, ch->m_index, false);
            ConnOpenBetaGiftMsg(rmsg, ch->m_desc->m_index, ch->m_index );
            SEND_Q(rmsg, gserver.m_connector);
#endif // EVENT_OPEN_BETA_GIFT
        }
        break;


#ifdef EVENT_TEACH_2007
    case MSG_EVENT_TEACH_2007:
        do_ExEventTeach2007(ch, msg);
        break;
#endif // EVENT_TEACH_2007

#if defined(EVENT_OPEN_ADULT_SERVER) || defined (EVENT_MAGIC_CARD)
    case MSG_EVENT_OPEN_ADULT_SERVER:
        do_EventOpenAdultServer(ch, msg);
        break;
#endif // EVENT_OPEN_ADULT_SERVER || defined (EVENT_MAGIC_CARD)
        
#ifdef GIFT_EVENT_2007
    case MSG_EVENT_GIFT_2007:
        do_EventGift2007(ch ,msg);
        break;
#endif // GIFT_EVENT_2007

#ifdef EVENT_RICHYEAR_2007
    case MSG_EVENT_RICHYEAR:
        do_EventRichYear(ch, msg);
        break;
#endif // EVENT_RICHYEAR_2007

#ifdef EVENT_NEW_MOONSTONE
    case MSG_EVENT_NEW_MOONSTONE:
        do_EventNewMoonstone(ch, msg);
        break;
#endif // EVENT_NEW_MOONSTONE

#ifdef NEWYEAR_EVENT_2008
    case MSG_EVENT_NEWYEAR_2008:
        do_EventNewYear2008(ch, msg);
        break;
#endif // NEWYEAR_EVENT_2008

#if defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)
    case MSG_EVENT_SAKURA_2008:
        do_EventSakura2008(ch, msg);
        break;
#endif // defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)

#ifdef EVENT_PHOENIX  // yhj   081105  피닉스
    case MSG_EVENT_PHOENIX:
        do_EventPhoenix(ch, msg);
        break;

    case MSG_EVENT_PHOENIX_SUCCESS:
        do_EventPhoenix_Success(ch, msg);
        break;
#endif

#ifdef REQUITAL_EVENT	// 보상 이벤트 
	case MSG_EVENT_REQUITAL:
        do_EventRequital(ch, msg);
        break;
#endif // REQUITAL_EVENT

    default:
        break;
    }
}

void do_EventSuperstone(CPC* ch, CNetMsg &msg)
{           
    CNetMsg errMsg;
    CNetMsg rmsg;

    unsigned char type;
    int chaindex;


    msg >> type;
    msg >> chaindex;    

    if(type == MSG_EVENT_SUPERSTONE_REQ)
    {
        static const int superStoneTable[7][2] = {
            {0, 200},
            {1, 300},
            {2, 500},
            {3, 800},
            {4, 1200},
            {5, 1700},
            {6, 2300},
        };
        int i;
        for (i = 0; i < 7; i++)
        {
            if (ch->m_superstone == superStoneTable[i][0])
            {
                if (ch->m_fame < superStoneTable[i][1])
                {
                    SysMsg(errMsg, MSG_SYS_FAME_NOT_ENOUGH);
                    SEND_Q(errMsg, ch->m_desc);
                    return;
                }
                break;
            }
        }
        if (i == 7)
        {
            SysMsg(errMsg, MSG_SYS_NO_MORE_SUPERSTONE);
            SEND_Q(errMsg, ch->m_desc);
            return;
        }

        CNetMsg itemmsg;
        CItem* item = NULL;
        item = gserver.m_itemProtoList.CreateItem(779, -1, 0, 0, 1);//우선은 고급제련석.
        if (!item)
            return;
                            
        CInventory* inven = GET_INVENTORY(ch, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
        if (!inven)
            return ;
        
        bool bCountable = false;
        // 인벤에 넣기
        if (AddToInventory(ch, item, true, true))
        {
            // 겹쳐졌는지 검사
            if (item->tab() == -1)
            {
                bCountable = true;
                
                // 수량 변경 알림
                int r, c;
                if (inven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
                {
                    CItem* p = inven->GetItem(r, c);
                    if (p)
                        ItemUpdateMsg(itemmsg, p, item->Count());
                }
            }
            else
            {
                ItemAddMsg(itemmsg, item);
                // 돈 검사
                if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
                    ch->m_moneyItem = item;
            }
        }
        else
        {               
            // 인벤토리 가득참                                      
            SysMsg(errMsg, MSG_SYS_INVEN_NOT_ENOUGH);
            SEND_Q(errMsg, ch->m_desc);

            delete item;
            item = NULL;

            return;
        }
        
        // 메시지 보내고
        SEND_Q(itemmsg, ch->m_desc);                
        
        EventSuperStoneMsg(rmsg, MSG_EVENT_SUPERSTONE_REP);
        SEND_Q(rmsg, ch->m_desc);               

        ch->m_superstone++;

        if(IS_RUNNING_HELPER)
        {
            HelperTeacherSuperstoneRecieve(rmsg, ch->m_index);
            SEND_Q(rmsg, gserver.m_helper);
        }

        // Item LOG
        GAMELOG << init("CHANGE_SUPERSTONE", ch)
                << itemlog(item)
                << end;
        
        if (bCountable)
            delete item;

    }
}

#ifdef EVENT_TREASUREBOX
void do_EventTreasurebox(CPC* ch, CNetMsg &msg)
{
    CNetMsg rMsg;
                
    unsigned char type;
    int chaindex;
    int level;
    
    msg >> type;
    msg >> chaindex;    

    if(type == MSG_EVENT_TREASUREBOX_TRY_REQ)
    {                                           
        int r, c;
        if (ch->m_invenEvent.FindItem(&r, &c,  760/*보물상자 */, 0, 0))//10레벨
        {       
            level = 10;     
            if(ch->m_level >= level)                    
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_10LEVEL);                   
            else
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
        }
        else if(ch->m_invenEvent.FindItem(&r, &c,  761/*보물상자 */, 0, 0))//14레벨
        {
            level = 14;
            if(ch->m_level >= level)                    
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_14LEVEL);                   
            else
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
        }
        else if(ch->m_invenEvent.FindItem(&r, &c,  762/*보물상자 */, 0, 0))//18레벨
        {
            level = 18;     
            if(ch->m_level >= level)                    
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_18LEVEL);                   
            else
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
        }
        else if(ch->m_invenEvent.FindItem(&r, &c,  763/*보물상자 */, 0, 0))//22레벨
        {
            level = 22;
            if(ch->m_level >= level)                    
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_22LEVEL);                   
            else
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
        }
        else if(ch->m_invenEvent.FindItem(&r, &c,  764/*보물상자 */, 0, 0))//26레벨
        {
            level = 26;
            if(ch->m_level >= level)                    
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_26LEVEL);                   
            else
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
        }
        else if(ch->m_invenEvent.FindItem(&r, &c,  765/*보물상자 */, 0, 0))//30레벨
        {
            level = 30;
            if(ch->m_level >= level)                    
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_30LEVEL);                   
            else
                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
        }
        else//보물상자가 없다.
        {
            level = 0;          
            EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NO_BOX);
        }
        
        SEND_Q(rMsg, ch->m_desc);
    
        return;
    }
    else if(type == MSG_EVENT_TREASUREBOX_OPEN_REQ)
    {               
        int r, c;
        int itemidx;
        int treasurelevel;

        int prob = GetRandom(1, 10000);
        CItem* gift = NULL;

        
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
        if( ch->m_invenNormal.GetSpace() < 5)
        {
            CNetMsg msg;
//          EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_FULL_INVEN);
            SysFullInventoryMsg(msg, INVENTORY_NORMAL);
            SEND_Q(msg, ch->m_desc);
            return;
        }
#endif // #ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND


#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
        CItem* gift1 = NULL;
#endif // #ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
            int lep = -1;
            int items[6][4][2] = 
            {
                {
                    { 2492, 20 },   // 빗물병 20개
                    { 2493, 40 },   // 빗방울 40개
                    { 2494, 3 },    // 잘익은 수박 3개
                    { 2500, 1 }     // 광속(5일) 1개
                },
                {
                    { 2492, 25 },   // 빗물병 25개
                    { 2493, 45 },   // 빗방울 45개
                    { 2494, 6 },    // 잘익은 수박 6개
                    { 2500, 1 }     // 광속(5일) 1개
                },
                {
                    { 2492, 30 },   // 빗물병 30개
                    { 2493, 50 },   // 빗방울 50개
                    { 2495, 3 },    // 케이크 3개
                    { 2500, 1 }     // 광속(5일) 1개
                },  
                {
                    { 2496, 20 },   // 사탕 20개
                    { 2495, 6 },    // 케이크 6개
                    { 2500, 1 },    // 광속(5일) 1개
                    { 0 ,0 }
                },
                {
                    { 2496, 25 },   // 사탕 25개
                    { 2499, 3 },    // 경험의 스크롤 3개
                    { 2500, 1 },    // 광속(5일) 2개
                    { 2500, 1 }
                },
                {
                    { 2497, 30 },   // 초콜릿 30개
                    { 2498, 4 },    // 눈사람 인형 4개
                    { 2500, 1 },    // 광속(5일) 2개
                    { 2500, 1 }
                }
            };
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND

        if (ch->m_invenEvent.FindItem(&r, &c,  760/*보물상자 */, 0, 0))//10레벨
        {       
            treasurelevel = 10;
            if (prob <= 2000)
            {
                // +4 13레벨 무기.

                switch (ch->m_job)
                {
                case JOB_TITAN:
                    if(prob <= 1000)                            
                        itemidx = 105;// 기간틱블레이드
                    else
                        itemidx = 603;// 러커버 액스
                    break;
                case JOB_KNIGHT:
                    if(prob <= 1000)        
                        itemidx = 107;// 세퍼소드
                    else
                        itemidx = 612;// 세퍼 듀얼소드
                    break;
                
                case JOB_HEALER:
                    if(prob <= 1000)        
                        itemidx = 106;// 테일보우
                    else
                        itemidx = 621;// 그웰리온 완드
                    break;
                
                case JOB_MAGE:
                    if(prob <= 1000)        
                        itemidx = 359;// 씨덕션완드
                    else
                        itemidx = 630;// 일레스 스테프
                    break;
                
                case JOB_ROGUE:
                    if(prob <= 1000)        
                        itemidx = 533;// 푸기오
                    else
                        itemidx = 638;// 시즈보우 석궁
                    break;

                case JOB_SORCERER:
                    if(prob <= 1000)        
                        itemidx = 979;// 시온포일 사이드
                    else
                        itemidx = 991;// 파스토이런 폴암
                    break;
                
                }
                gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 4, 0, 1);
                if (!gift)
                    return;

                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_13LEVEL_WEAPON);
            }
            else if(prob <= 5000)//나스
            {
                itemidx = 19; //나스
                gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 30000);
                if (!gift)
                    return;

                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
            }
            else
            {
                itemidx = 556; //사탕
                gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 5);
                if (!gift)
                    return;

                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
            }
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
            itemidx = 1289;
            gift1 = gserver.m_itemProtoList.CreateItem( itemidx, -1, 0, 0, 1 );
#endif //EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
            lep = 0;            
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND   
        }
        else if(ch->m_invenEvent.FindItem(&r, &c,  761/*보물상자 */, 0, 0))//14레벨
        {
            treasurelevel = 14;

            if (prob <= 2000)
            {
                itemidx = 510; //공격력향상제
                gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 10);//10개
                if (!gift)
                    return;

                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_ATTACK_POTION);
            }
            else if(prob <= 5000)//나스
            {
                itemidx = 19; //나스
                gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 50000);
                if (!gift)
                    return;

                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
            }
            else
            {
                itemidx = 556; //사탕
                gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 10);
                if (!gift)
                    return;

                EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
            }

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
            itemidx = 1292;
            gift1 = gserver.m_itemProtoList.CreateItem( itemidx, -1, 0, 0, 1 );
#endif //EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
            lep = 1;            
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND

        }
        else if(ch->m_invenEvent.FindItem(&r, &c,  762/*보물상자 */, 0, 0))//18레벨
        {
            treasurelevel = 18;

            if (prob <= 2000)
            {
                // +4 21레벨 무기.

                switch (ch->m_job)
                {
				case JOB_TITAN:
					if(prob <= 1000)							
						itemidx = 307;// 메테오블레이드 대검
					else
						itemidx = 605;// 브로바 액스
					break;
				case JOB_KNIGHT:
					if(prob <= 1000)		
						itemidx = 322;// 노바소드
					else
						itemidx = 614;// 노바 듀얼소드
					break;
				
				case JOB_HEALER:
					if(prob <= 1000)		
						itemidx = 341;// 크로스보우
					else
						itemidx = 623;// 사일런트 완드
					break;
				
				case JOB_MAGE:
					if(prob <= 1000)		
						itemidx = 361;// 아크완드
					else
						itemidx = 632;// 세이라 스테프
					break;
				
				case JOB_ROGUE:
					if(prob <= 1000)		
						itemidx = 535;// 보위나이프 단검
					else
						itemidx = 640;// 임퍼테스 보우
					break;

				case JOB_SORCERER:
					if(prob <= 1000)		
						itemidx = 981;// 테너일러 사이드
					else
						itemidx = 993;// 블루마그마 폴암
					break;

				}
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 4, 0, 1);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_21LEVEL_WEAPON);
			}
			else if(prob <= 5000)//나스
			{
				itemidx = 19; //나스
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 80000);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
			}
			else
			{
				itemidx = 556; //사탕
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 15);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
			}
		
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
			itemidx = 840;
			gift1 = gserver.m_itemProtoList.CreateItem( itemidx, -1, 0, 0, 1 );
#endif //EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
			lep = 2;			
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
		}
		else if(ch->m_invenEvent.FindItem(&r, &c,  763/*보물상자 */, 0, 0))//22레벨
		{
			treasurelevel = 22;

			if (prob <= 2000)
			{
				itemidx = 511; //방어력향상제
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 15);//15개
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_DEFENSE_POTION);
			}
			else if(prob <= 5000)//나스
			{
				itemidx = 19; //나스
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 100000);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
			}
			else
			{
				itemidx = 556; //사탕
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 20);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
			}
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
			itemidx = 1295;
			gift1 = gserver.m_itemProtoList.CreateItem( itemidx, -1, 0, 0, 1 );
#endif //EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
			lep = 3;			
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
		}
		else if(ch->m_invenEvent.FindItem(&r, &c,  764/*보물상자 */, 0, 0))//26레벨
		{
			treasurelevel = 26;

			if (prob <= 2000)
			{
				// +4 29레벨 무기.

				switch (ch->m_job)
				{
				case JOB_TITAN:
					if(prob <= 1000)							
						itemidx = 309;// 폴브레이크 대검
					else
						itemidx = 607;// 크레센트 액스
					break;
				case JOB_KNIGHT:
					if(prob <= 1000)		
						itemidx = 324;// 소울크래셔 
					else
						itemidx = 616;// 더블 소울크래셔
					break;
				
				case JOB_HEALER:
					if(prob <= 1000)		
						itemidx = 343;// 글로우보우
					else
						itemidx = 625;// 페이스 완드
					break;
				
				case JOB_MAGE:
					if(prob <= 1000)		
						itemidx = 363;// 커미셔완드 
					else
						itemidx = 634;// 이터널 스테프
					break;
				
				case JOB_ROGUE:
					if(prob <= 1000)		
						itemidx = 537;// 푸아냐르 단검
					else
						itemidx = 642;// 드미트리 보우
					break;

				case JOB_SORCERER:
					if(prob <= 1000)		
						itemidx = 983;// 파이언스톰 사이드
					else
						itemidx = 995;// 포세이던트 폴암
					break;
				
				}
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 4, 0, 1);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_29LEVEL_WEAPON);
			}
			else if(prob <= 5000)//나스
			{
				itemidx = 19; //나스
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 150000);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
			}
			else
			{
				itemidx = 556; //사탕
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 30);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
			}

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
			itemidx = 841;
			gift1 = gserver.m_itemProtoList.CreateItem( itemidx, -1, 0, 0, 1 );
#endif //EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
			lep = 4;			
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
		}
		else if(ch->m_invenEvent.FindItem(&r, &c,  765/*보물상자 */, 0, 0))//30레벨
		{
			treasurelevel = 30;
			if (prob <= 2000)
			{
#ifdef LC_TLD
				itemidx = PET_BLUE_HORSE_EGG_INDEX; //믈루 판의 피리
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);//1개
				if (!gift)
					return;
				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_BLUE_HORSE);
#else
				itemidx = 510; //공격력향상제
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 20);//20개
				if (!gift)
					return;
				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_ATTACK_POTION);
#endif // LC_TLD				
			}
			else if(prob <= 5000)//나스
			{
				itemidx = 19; //나스
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 200000);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
			}
			else
			{
#ifdef LC_TLD
				itemidx = PET_PINK_DRAGON_EGG_INDEX; //핑크 드레이크알 
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);//1개
				if (!gift)
					return;
				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_PINK_DRAGON);
#else
				itemidx = 556; //사탕
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 40);
				if (!gift)
					return;
				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
#endif // LC_TLD			
			}

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
			itemidx = 1295;
			gift1 = gserver.m_itemProtoList.CreateItem( itemidx, -1, 0, 0, 1 );
#endif //EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
			lep = 5;			
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND
		}
		else//보물상자가 없다.
		{
			EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NO_TREASUREBOX);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}

		//보물상자를 지운다.
		if(treasurelevel == 0)
			return;
		

		// 찾았으면 수량 확인 해서 줄이기
		CItem* item = ch->m_invenEvent.GetItem(r, c);
		
		if (!item || item->Count() < 1)
		{				
			EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NO_TREASUREBOX);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
		
		// 보물상자를 삭제.
		CNetMsg itemmsg;
		DecreaseFromInventory(ch, item, 1);
		
		if (item->Count() > 0)
		{
			ItemUpdateMsg(itemmsg, item, -1);
			SEND_Q(itemmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(itemmsg, item);
			SEND_Q(itemmsg, ch->m_desc);
			ch->RemoveItemFromQuickSlot(item);
			RemoveFromInventory(ch, item, true, true);
		}

		// 보상품이 들어갈 인벤토리 결정
		CInventory* inven = GET_INVENTORY(ch, GET_TAB(gift->m_itemProto->m_typeIdx, gift->m_itemProto->m_subtypeIdx));
		if (!inven)
			return ;
		
		bool bCountable = false;
		// 인벤에 넣기
		if (AddToInventory(ch, gift, true, true))
		{
			// 겹쳐졌는지 검사
			if (gift->tab() == -1)
			{
				bCountable = true;
				
				// 수량 변경 알림
				int r, c;
				if (inven->FindItem(&r, &c, gift->m_idNum, gift->m_plus, gift->m_flag))
				{
					CItem* p = inven->GetItem(r, c);
					if (p)
						ItemUpdateMsg(itemmsg, p, gift->Count());
				}
			}
			else
			{
				ItemAddMsg(itemmsg, gift);
				// 돈 검사
				if (gift->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
					ch->m_moneyItem = gift;
			}
		}
		else
		{				
			// 인벤토리 가득참
			gift = ch->m_pArea->DropItem(gift, ch);
			if (!gift)
				return;
			
			CNetMsg dropMsg;
			gift->m_preferenceIndex = ch->m_index;
			ItemDropMsg(dropMsg, ch, gift);
			ch->m_pArea->SendToCell(dropMsg, ch, true);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
		
		// 메시지 보내고
		SEND_Q(itemmsg, ch->m_desc);
		SEND_Q(rMsg, ch->m_desc);
	
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
		if( ch->GiveItem( gift1->m_idNum, 0, 0, 1, true ) )
		{
			GAMELOG << init("EVENT_TREASUREBOX_ADD_ITEM ")
			<< gift1->m_idNum
			<< 1
			<< end;
		}
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND	
	if (lep >= 0 && lep <= 5)
	{
		for(int j=0; j < 4; j++)
		{
			if(items[lep][j][0] != 0 && items[lep][j][1] != 0)
			{
				if( ch->GiveItem( items[lep][j][0], 0, 0, items[lep][j][1], true ) )
				{
					GAMELOG << init("EVENT_TREASUREBOX_ADD_ITEM_2ND ")
					<< items[lep][j][0] << delim
					<< items[lep][j][1]
					<< end;
				}
			}
		}
	}
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM_2ND

		// 메시지 보내고
		
		// Item LOG
		GAMELOG << init("CHANGE_TREASUREBOX", ch)
				<< itemlog(gift)
				<< end;
		
		if (bCountable)
			delete gift;


// 다음 레벨 보물상자를 준다.
		switch(treasurelevel)
		{
		case 10:
			itemidx =761;
			break;
		case 14:
			itemidx =762;
			break;
		case 18:
			itemidx =763;
			break;
		case 22:
			itemidx =764;
			break;
		case 26:
			itemidx =765;
			break;
		case 30:					
			return;
		}

		gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);
		if (!gift)
			return;
		
		inven = GET_INVENTORY(ch, GET_TAB(gift->m_itemProto->m_typeIdx, gift->m_itemProto->m_subtypeIdx));
		if (!inven)
			return ;
		
		bCountable = false;
		// 인벤에 넣기
		if (AddToInventory(ch, gift, true, true))
		{
			// 겹쳐졌는지 검사
			if (gift->tab() == -1)
			{
				bCountable = true;
				
				// 수량 변경 알림
				int r, c;
				if (inven->FindItem(&r, &c, gift->m_idNum, gift->m_plus, gift->m_flag))
				{
					CItem* p = inven->GetItem(r, c);
					if (p)
						ItemUpdateMsg(itemmsg, p, gift->Count());
				}
			}
			else
			{
				ItemAddMsg(itemmsg, gift);
				// 돈 검사
				if (gift->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
					ch->m_moneyItem = gift;
			}
		}
		else
		{				
			// 인벤토리 가득참
			gift = ch->m_pArea->DropItem(gift, ch);
			if (!gift)
				return;
			
			CNetMsg dropMsg;
			gift->m_preferenceIndex = ch->m_index;
			ItemDropMsg(dropMsg, ch, gift);
			ch->m_pArea->SendToCell(dropMsg, ch, true);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
		
		// 메시지 보내고
		SEND_Q(itemmsg, ch->m_desc);
		SEND_Q(rMsg, ch->m_desc);
		
		// Item LOG
		GAMELOG << init("GIVE_TREASUREBOX", ch)
				<< itemlog(gift)
				<< end;
		
		if (bCountable)
			delete gift;

	}
}
#endif // EVENT_TREASUREBOX

#ifdef EVENT_TREASUREBOX_RED
void do_EventTreasurebox_red(CPC* ch, CNetMsg &msg)
{
	CNetMsg rMsg;
				
	unsigned char type;
	int chaindex;
	int level;
	
	msg >> type;
	msg >> chaindex;	

	if(type == MSG_EVENT_TREASUREBOX_TRY_REQ)
	{											
		int r = -1, c = -1;
		if (ch->m_invenEvent.FindItem(&r, &c,  2660/*보물상자 */, 0, 0))	// 5레벨
		{		
			level = 5;		
			if(ch->m_level >= level)					
				EventTreasureBoxRedMsg(rMsg, MSG_EVENT_TREASUREBOX_RED_TRY_REP, (int)MSG_EVENT_TREASUREBOX_RED_5LEVEL);					
			else
				EventTreasureBoxRedMsg(rMsg, MSG_EVENT_TREASUREBOX_RED_TRY_REP, (int)MSG_EVENT_TREASUREBOX_RED_ERROR_NOT_LEVEL);
		}
		else if(ch->m_invenEvent.FindItem(&r, &c,  2661/*보물상자 */, 0, 0))	// 12레벨
		{
			level = 12;
			if(ch->m_level >= level)					
				EventTreasureBoxRedMsg(rMsg, MSG_EVENT_TREASUREBOX_RED_TRY_REP, (int)MSG_EVENT_TREASUREBOX_RED_12LEVEL);					
			else
				EventTreasureBoxRedMsg(rMsg, MSG_EVENT_TREASUREBOX_RED_TRY_REP, (int)MSG_EVENT_TREASUREBOX_RED_ERROR_NOT_LEVEL);
		}
		else if(ch->m_invenEvent.FindItem(&r, &c,  2662/*보물상자 */, 0, 0))	// 16레벨
		{
			level = 16;		
			if(ch->m_level >= level)					
				EventTreasureBoxRedMsg(rMsg, MSG_EVENT_TREASUREBOX_RED_TRY_REP, (int)MSG_EVENT_TREASUREBOX_RED_16LEVEL);					
			else
				EventTreasureBoxRedMsg(rMsg, MSG_EVENT_TREASUREBOX_RED_TRY_REP, (int)MSG_EVENT_TREASUREBOX_RED_ERROR_NOT_LEVEL);
		}
		else // 보물상자가 없다.
		{
			level = 0;			
			EventTreasureBoxRedMsg(rMsg, MSG_EVENT_TREASUREBOX_RED_TRY_REP, (int)MSG_EVENT_TREASUREBOX_RED_ERROR_NO_BOX);
		}
		
		SEND_Q(rMsg, ch->m_desc);	
		return;
	}
	else if(type == MSG_EVENT_TREASUREBOX_RED_OPEN_REQ)
	{				
		int r = -1, c = -1;
		int itemcnt = 0;
		int treasurelevel = 0;
		int item[7][2];

		if (ch->m_invenEvent.FindItem(&r, &c,  2660/*보물상자 */, 0, 0)) // 5레벨
		{		
			treasurelevel = 5;
			itemcnt = 2;
			item[0][0] = 2658;			item[0][1] = 5;
			item[1][0] = 2659;			item[1][1] = 5;
		}
		else if(ch->m_invenEvent.FindItem(&r, &c,  2661/*보물상자 */, 0, 0)) // 12레벨
		{
			treasurelevel = 12;
#ifdef	EVENT_SUMMER_2008
			itemcnt = 3;
			item[0][0] = 2860;			item[0][1] = 10;
			item[1][0] = 2658;			item[1][1] = 5;
			item[2][0] = 2659;			item[2][1] = 5;
#else	//EVENT_SUMMER_2008
			treasurelevel = 12;
			itemcnt = 7;
			item[0][0] = 2658;			item[0][1] = 5;
			item[1][0] = 2659;			item[1][1] = 5;
			item[2][0] = 2664;			item[2][1] = 4;

			switch (ch->m_job)
			{
			case JOB_TITAN:
				item[3][0] = 2151;		item[3][1] = 1;
				item[4][0] = 2152;		item[4][1] = 1;
				item[5][0] = 2153;		item[5][1] = 1;
				item[6][0] = 2154;		item[6][1] = 1;
				break;
			case JOB_KNIGHT:
				item[3][0] = 2155;		item[3][1] = 1;
				item[4][0] = 2156;		item[4][1] = 1;
				item[5][0] = 2157;		item[5][1] = 1;
				item[6][0] = 2158;		item[6][1] = 1;
				break;
			
			case JOB_HEALER:
				item[3][0] = 2159;		item[3][1] = 1;
				item[4][0] = 2160;		item[4][1] = 1;
				item[5][0] = 2161;		item[5][1] = 1;
				item[6][0] = 2162;		item[6][1] = 1;
				break;
			
			case JOB_MAGE:
				item[3][0] = 2163;		item[3][1] = 1;
				item[4][0] = 2164;		item[4][1] = 1;
				item[5][0] = 2165;		item[5][1] = 1;
				item[6][0] = 2166;		item[6][1] = 1;
				break;
			
			case JOB_ROGUE:
				item[3][0] = 2167;		item[3][1] = 1;
				item[4][0] = 2168;		item[4][1] = 1;
				item[5][0] = 2169;		item[5][1] = 1;
				item[6][0] = 2170;		item[6][1] = 1;
				break;

			case JOB_SORCERER:
				item[3][0] = 2149;		item[3][1] = 1;
				item[4][0] = 2150;		item[4][1] = 1;
				item[5][0] = 2171;		item[5][1] = 1;
				item[6][0] = 2172;		item[6][1] = 1;
				break;
			
			}
#endif //EVENT_SUMMER_2008
		}
		else if(ch->m_invenEvent.FindItem(&r, &c,  2662/*보물상자 */, 0, 0)) // 16레벨
		{
			treasurelevel = 16;
			itemcnt =2;
			item[0][0] = 2658;		item[0][1] = 5;
			item[1][0] = 2659;		item[1][1] = 5;
		}
		else//보물상자가 없다.
		{
			EventTreasureBoxRedMsg(rMsg, MSG_EVENT_TREASUREBOX_RED_OPEN_REP, (int)MSG_EVENT_TREASUREBOX_RED_ERROR_NO_BOX);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}

		// 지급
		CItem * pGift = NULL;
		bool bCountable;
		for(int i=0; i < itemcnt; i++)
		{
			bCountable = false;
			pGift = gserver.m_itemProtoList.CreateItem(item[i][0], -1, 0, 0, item[i][1]);
			if(pGift == NULL)
			{
				continue;
			}

			CInventory * inven = GET_INVENTORY(ch, GET_TAB(pGift->m_itemProto->m_typeIdx, pGift->m_itemProto->m_subtypeIdx));
			if(inven == NULL)
			{
				continue;
			}

			CNetMsg itemmsg;

			// 인벤에 넣기
			if(AddToInventory(ch, pGift, true, true))
			{	// 겹쳐졌는지 검사
				if (pGift->tab() == -1)
				{	
					bCountable = true;

					// 수량 변경 알림
					int r, c;
					if (inven->FindItem(&r, &c, pGift->m_idNum, pGift->m_plus, pGift->m_flag))
					{
						CItem * p = inven->GetItem(r, c);
						if(p != NULL)
						{
							ItemUpdateMsg(itemmsg, p, pGift->Count());
						}
					}
				}
				else
				{
					ItemAddMsg(itemmsg, pGift);
				}
			}
			else
			{				
				// 인벤토리 가득참
				pGift = ch->m_pArea->DropItem(pGift, ch);
				if(pGift == NULL)
				{
					return;
				}
				
				CNetMsg dropMsg;
				pGift->m_preferenceIndex = ch->m_index;
				ItemDropMsg(dropMsg, ch, pGift);
				ch->m_pArea->SendToCell(dropMsg, ch, true);
				SEND_Q(rMsg, ch->m_desc);
				continue;
			}

			// 메시지 보내고
			SEND_Q(itemmsg, ch->m_desc);

			// Item LOG
			GAMELOG << init("GIVE_TREASUREBOX", ch)
				<< itemlog(pGift)
				<< end;
		
			if(bCountable)
			{
				delete pGift;
			}
		}	

		// 보물상자를 지운다.
		if( r == -1 || c == -1)
		{
			return;
		}	
		// 찾았으면 수량 확인 해서 줄이기
		CItem * pItem = ch->m_invenEvent.GetItem(r, c);
		
		if (!pItem || pItem->Count() < 1)
		{				
			EventTreasureBoxRedMsg(rMsg, MSG_EVENT_TREASUREBOX_RED_OPEN_REP, (int)MSG_EVENT_CHANGE_NO_TREASUREBOX);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
		
		// 보물상자를 삭제.
		CNetMsg itemmsg;
		DecreaseFromInventory(ch, pItem, 1);
		
		if (pItem->Count() > 0)
		{
			ItemUpdateMsg(itemmsg, pItem, -1);
			SEND_Q(itemmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(itemmsg, pItem);
			SEND_Q(itemmsg, ch->m_desc);
			ch->RemoveItemFromQuickSlot(pItem);
			RemoveFromInventory(ch, pItem, true, true);
		}
					
		// 다음 레벨 보물상자를 준다.
		int itemidx = -1;
		switch(treasurelevel)
		{
		case 5:
			itemidx = 2661;
			break;
		case 12:
			itemidx = 2662;
			break;
		case 16:					
			return;
		}

		pGift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);
		if(!pGift)
		{
			return;
		}
		
		CInventory * inven = GET_INVENTORY(ch, GET_TAB(pGift->m_itemProto->m_typeIdx, pGift->m_itemProto->m_subtypeIdx));
		if(!inven)
		{
			return;
		}
		
		bCountable = false;
		// 인벤에 넣기
		if (AddToInventory(ch, pGift, true, true))
		{
			// 겹쳐졌는지 검사
			if (pGift->tab() == -1)
			{
				bCountable = true;
				
				// 수량 변경 알림
				int r, c;
				if (inven->FindItem(&r, &c, pGift->m_idNum, pGift->m_plus, pGift->m_flag))
				{
					CItem* p = inven->GetItem(r, c);
					if (p)
						ItemUpdateMsg(itemmsg, p, pGift->Count());
				}
			}
			else
			{
				ItemAddMsg(itemmsg, pGift);
			}
		}
		else
		{				
			// 인벤토리 가득참
			pGift = ch->m_pArea->DropItem(pGift, ch);
			if (!pGift)
			{
				return;
			}
			
			CNetMsg dropMsg;
			pGift->m_preferenceIndex = ch->m_index;
			ItemDropMsg(dropMsg, ch, pGift);
			ch->m_pArea->SendToCell(dropMsg, ch, true);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
		
		// 메시지 보내고
		SEND_Q(itemmsg, ch->m_desc);
		
		// Item LOG
		GAMELOG << init("GIVE_TREASUREBOX", ch)
				<< itemlog(pGift)
				<< end;
		
		if (bCountable)
		{
			delete pGift;
		}
	}
}
#endif // EVENT_TREASUREBOX_RED

#ifdef EVENT_MOONSTONE
void do_EventMoonStone(CPC* ch, CNetMsg& msg)
{
	CNetMsg rMsg;
				
	unsigned char type;
	int chaindex;
	msg >> type;
	
	if(type == MSG_EVENT_MOONSTONE_START_REQ)
	{
		msg >> chaindex;				

		EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_START_REP, MSG_CHANGE_MOONSTONE_ACC_NAS, gserver.m_nMoonStoneNas);				
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
	else if(type == MSG_EVENT_MOONSTONE_TRY_REQ)//우선 문스톤이 있는지 체크해서 클라이언트로 5개의 숫자를 보낸다.
	{
		msg >> chaindex;

		// 이전 문스톤 결과를 받았는가?
		if (ch->m_nMoonStoneSum != 0 || ch->m_nMoonStoneDigit != -1)
		{
			EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_TRY_REP, MSG_CHANGE_NO_MOONSTONE);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}

		//문스톤이 있는가
		int r, c;
		if (!ch->m_invenNormal.FindItem(&r, &c, 723 /*문스톤 */, 0, 0))
		{
			ch->m_nMoonStoneSum = 0;
			ch->m_nMoonStoneDigit = -1;
			
			EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_TRY_REP, MSG_CHANGE_NO_MOONSTONE);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}				
	
#ifdef EVENT_MOONSTONE_BRZTEST
		int prob = 7;
		int prob2 = GetRandom(1, 10000);
#else
		int prob = GetRandom(1, 16807);
		int prob2 = GetRandom(1, 10000);
#endif // EVENT_MOONSTONE_BRZTEST
		typedef struct __tagMoonstoneProbTable
		{
			int		probSameIcon;			// n개의 그림이 같을 확률
			int		probDigit[7];			// 인덱스 번호가 같을 확률
		} MOONSTONEPROBTABLE;

		// 당첨 테이블 : (인덱스 + 1)개의 그림이 같을 확률
		const MOONSTONEPROBTABLE probTable[5] = {
			{16807, {10000, 8000, 6000, 5000, 3000, 2000, 1000}},	// 1개 같을 때 그림별 확률
			{ 8772, {10000, 8000, 6000, 5000, 3000, 2000, 1000}},	// 2개 같을 때 그림별 확률
			{ 2737, {10000, 8000, 6000, 5000, 3000, 2000, 1000}},	// 3개 같을 때 그림별 확률
			{  217, {10000, 8000, 6000, 5000, 3000, 2000, 1000}},	// 4개 같을 때 그림별 확률
			{    7, {10000, 5600, 3600, 2600, 2000, 1500, 1300}}	// 5개 같을 때 그림별 확률
		};

		int i = 1, j = 0;
		for (i = 5; i >= 1; i--)			// i개의 그림이 같을 확률 루프 : 1개는 제외
		{
			if (prob <= probTable[i - 1].probSameIcon)
			{
				for (j = 6; j >= 1; j--)	// i개의 그림이 같을 때, j번 그림을 선택하는 루프 : 0번은 제외
				{
					if (prob2 <= probTable[i - 1].probDigit[j])
						break;
				}
				break;
			}
		}

		// 여기로 오면, j번 그림이 i개 같은 상태
		ch->m_nMoonStoneSum = i;
		ch->m_nMoonStoneDigit = j;

		const static int maxNas = 10000000;
		int sendnas = 0;
		if(gserver.m_nMoonStoneNas + 200 > maxNas)
		{
			sendnas = maxNas;
		}
		else
		{
			sendnas = gserver.m_nMoonStoneNas + 200;
		}
	
		EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_TRY_REP, MSG_CHANGE_MOONSTONE_RULLET, ch->m_nMoonStoneDigit, ch->m_nMoonStoneSum, sendnas);				
		SEND_Q(rMsg, ch->m_desc);
		return;
		
	}
	// 클라이언트에서 룰렛이 모두 멈추었으니 결과를 보내달라는 메시지.
	else if(type == MSG_EVENT_MOONSTONE_RESULT_REQ) 
	{
		msg >> chaindex;
		if(ch->m_nMoonStoneDigit == -1 || ch->m_nMoonStoneSum == 0)
		{
			ch->m_nMoonStoneSum = 0;
			ch->m_nMoonStoneDigit = -1;
			
			EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_NO_MOONSTONE);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
		
		//문스톤이 있는가
		int r, c;
		if (!ch->m_invenNormal.FindItem(&r, &c, 723 /*문스톤 */, 0, 0))
		{
			ch->m_nMoonStoneSum = 0;
			ch->m_nMoonStoneDigit = -1;
			
			EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_NO_MOONSTONE);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}			
		
		// 찾았으면 수량 확인 해서 줄이기
		CItem* item = ch->m_invenNormal.GetItem(r, c);
		
		if (!item || item->Count() < 1)
		{
			ch->m_nMoonStoneSum = 0;
			ch->m_nMoonStoneDigit = -1;
			
			EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_NO_MOONSTONE);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
		
		// 문스톤 item 수량 변경
		CNetMsg itemmsg;
		DecreaseFromInventory(ch, item, 1);
		
		if (item->Count() > 0)
		{
			ItemUpdateMsg(itemmsg, item, -1);
			SEND_Q(itemmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(itemmsg, item);
			SEND_Q(itemmsg, ch->m_desc);
			ch->RemoveItemFromQuickSlot(item);
			RemoveFromInventory(ch, item, true, true);
		}
		
		CNetMsg hmsg;
		HelperEventMoonStoneUpdateReqMsg(hmsg);
		SEND_Q(hmsg, gserver.m_helper);

		CItem* gift = NULL;				
		
		if(ch->m_nMoonStoneSum == 1)
		{
			switch(ch->m_nMoonStoneDigit)
			{
//0517
			case 0: // 중형MP물약 1개
				{																
					gift = gserver.m_itemProtoList.CreateItem(485, -1, 0, 0, 1);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_STONELISE);
				}
				break;
			case 1: // 대형HP물약 1개
				{
					gift = gserver.m_itemProtoList.CreateItem(45, -1, 0, 0, 1);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_CRAK_BRANCH);
				}	
				break;
			case 2: // 푸른넨3 3개
				{
					gift = gserver.m_itemProtoList.CreateItem(501, -1, 0, 3, 3);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_MIDIUM_HEAL_POTION);
				}		
				break;
			case 3: // 중형HP물약 3개
				{
					gift = gserver.m_itemProtoList.CreateItem(44, -1, 0, 0, 3);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_D_GRADE_ELEMENTAL);
				}		
				break;
			case 4: // 푸른케르3 3개
				{
					gift = gserver.m_itemProtoList.CreateItem(500, -1, 0, 3, 3);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_STONE_WEAVER);
				}			
				break;
			case 5: // 붉은넨3 3개
				{
					gift = gserver.m_itemProtoList.CreateItem(499, -1, 0, 3, 3);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_CRAK_BLUE_LEAF);
				}			
				break;
			case 6: // 붉은케르3 3개
				{
				gift = gserver.m_itemProtoList.CreateItem(498, -1, 0, 3, 3);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_C_GRADE_ELEMENTAL);
				}			
				break;
			}
		}
		else if(ch->m_nMoonStoneSum == 2)
		{
			switch(ch->m_nMoonStoneDigit)
			{
			case 0: //메모리 스크롤 3개
				{
					gift = gserver.m_itemProtoList.CreateItem(47, -1, 0, 0, 3);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_MEMORY_SCROLL);
				}	
				break;
			case 1: //중형 MP물약 3개
				{
					gift = gserver.m_itemProtoList.CreateItem(485, -1, 0, 0, 3);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_MIDDLE_MP_POTION);
				}		
				break;
			case 2: //푸른넨 3개
				{
					gift = gserver.m_itemProtoList.CreateItem(501, -1, 0, 6, 3);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_6GRADE_BLUE_NEN);
				}			
				break;
			case 3: //대형 HP물약 3개
				{
					gift = gserver.m_itemProtoList.CreateItem(45, -1, 0, 0, 3);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_BIG_HEAL_POTION);
				}				
				break;
			case 4: //푸른 케르 3개
				{
					gift = gserver.m_itemProtoList.CreateItem(500, -1, 0, 6, 3);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_6GRADE_BLUE_KER);
				}		
				break;
			case 5: //붉은 넨 3개
				{
					gift = gserver.m_itemProtoList.CreateItem(499, -1, 0, 6, 3);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_6GRADE_RED_NEN);
				}	
				break;
			case 6: //붉은 케르 3개 
				{
					gift = gserver.m_itemProtoList.CreateItem(498, -1, 0, 6, 3);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_6GRADE_RED_KER);
				}
				break;
			}
		}
		else if(ch->m_nMoonStoneSum == 3)
		{
			switch(ch->m_nMoonStoneDigit)
			{
			case 0: // 부스터 2개
				{
#if defined (LC_KOR) || defined(LC_BRZ)
					gift = gserver.m_itemProtoList.CreateItem(786, -1, 0, 0, 2);

					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_25000_EXP);
#elif defined(LC_MAL) || defined(LC_USA)
					gift = gserver.m_itemProtoList.CreateItem(724, -1, 0, 0, 4);

					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_25000_EXP);
#else
					if( GetRandom(1, 100) % 2 )
					{
						gift = gserver.m_itemProtoList.CreateItem(786, -1, 0, 0, 2);
						if (!gift)
						return;
					
						EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_25000_EXP);
					}
					else
					{
						gift = gserver.m_itemProtoList.CreateItem(723, -1, 0, 0, 5);
						if (!gift)
						return;
					
						EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_5_MOONSTONE);
					}
#endif
				}
				break;
			case 1: //문스톤 5개
				{
					gift = gserver.m_itemProtoList.CreateItem(723, -1, 0, 0, 5);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_5_MOONSTONE);
				}		
				break;
			case 2: //푸른 넨 5개
				{
					gift = gserver.m_itemProtoList.CreateItem(501, -1, 0, 12, 5);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_5_12GRADE_BLUE_NEN);
				}			
				break;
			case 3: //대형 MP물약 5개
				{
					gift = gserver.m_itemProtoList.CreateItem(724, -1, 0, 0, 5);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_5_BIG_MP_POTION);
				}	
				break;
			case 4: //푸른 케르 5개
				{
					gift = gserver.m_itemProtoList.CreateItem(500, -1, 0, 12, 5);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_5_12GRADE_BLUE_KER);
				}
				break;
			case 5: //붉은 넨 5개
				{
					gift = gserver.m_itemProtoList.CreateItem(499, -1, 0, 12, 5);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_5_12GRADE_RED_NEN);
				}	
				break;
			case 6: //붉은 케르 5개
				{
					gift = gserver.m_itemProtoList.CreateItem(498, -1, 0, 12, 5);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_5_12GRADE_RED_KER);
				}	
				break;
			}
		}
		else if(ch->m_nMoonStoneSum == 4)
		{
			switch(ch->m_nMoonStoneDigit)
			{
			case 0: //하급 경험의 결정.
				{
					gift = gserver.m_itemProtoList.CreateItem(671, -1, 0, 0, 3);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_MIDDLE_CRYSTAL_OF_EXPERIMENCE);
				}	
				break;
			case 1: // 노력의스크롤 1개
				{
#if defined (LC_MAL) || defined (LC_USA)
					gift = gserver.m_itemProtoList.CreateItem(723, -1, 0, 0, 10);
#else
					gift = gserver.m_itemProtoList.CreateItem(973, -1, 0, 0, 1);
#endif
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_70000_EXP);
				}	
				break;
			case 2: // 경험의스크롤 1개
				{
#if defined (LC_MAL) || defined (LC_USA)
					gift = gserver.m_itemProtoList.CreateItem(673, -1, 0, 0, 3);
#else
					gift = gserver.m_itemProtoList.CreateItem(971, -1, 0, 0, 1);
#endif
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_10_PURE_STONE);
				}		
				break;
			case 3: //중급경험의 결정
				{							
					gift = gserver.m_itemProtoList.CreateItem(672, -1, 0, 0, 1);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_MIDDLE_CRYSTAL_OF_EXPERIMENCE);
				}	
				break; 
			case 4: // 행운의스크롤 1개
				{
#if defined (LC_MAL) || defined (LC_USA)
					gift = gserver.m_itemProtoList.CreateItem(673, -1, 0, 0, 3);
#else
					gift = gserver.m_itemProtoList.CreateItem(972, -1, 0, 0, 1);
#endif
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_150000_EXP);
				}	
				break;
			case 5: //상급 경험의 결정
				{
					gift = gserver.m_itemProtoList.CreateItem(673, -1, 0, 0, 1);
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_MIDDLE_CRYSTAL_OF_EXPERIMENCE);
				}		
				break;
			case 6: // 경험의스크롤 3개
				{
#if defined (LC_MAL) || defined (LC_USA)
					gift = gserver.m_itemProtoList.CreateItem(673, -1, 0, 0, 3);
#else
					gift = gserver.m_itemProtoList.CreateItem(971, -1, 0, 0, 3);
#endif
					
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_CRYSTAL_OF_EFFORT);
				}			
				break;
			}
		}
		else if(ch->m_nMoonStoneSum == 5)
		{
			switch(ch->m_nMoonStoneDigit)
			{
			case 0: //고급제련석 1
				{
					gift = gserver.m_itemProtoList.CreateItem(85, -1, 0, 0, 1);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_1_SPECIAL_STONE);
				}
				break;
			case 1: //고급제련석 2
				{
					gift = gserver.m_itemProtoList.CreateItem(85, -1, 0, 0, 2);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_2_SPECIAL_STONE);
				}
				break;
			case 2: //고급제련석 3
				{
					gift = gserver.m_itemProtoList.CreateItem(85, -1, 0, 0, 3);
					if (!gift)
						return;
					
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3_SPECIAL_STONE);
				}	
				break;
			case 3: // 45레벨무기
			case 4: // +3 45레벨 무기
			case 5: // +5 45레벨 무기
				{
					int itemidx = -1;
					int plus = 0;
					switch (ch->m_job)
					{
					case JOB_TITAN:
						itemidx = 793;//대검
						break;
					case JOB_KNIGHT:
						itemidx = 800;//기사도
						break;
					case JOB_HEALER:
						itemidx = 808;//활
						break;
					case JOB_MAGE:
						itemidx = 815;//숏스테프
						break;
					case JOB_ROGUE:
						itemidx = 822;//단검
						break;
					case JOB_SORCERER:
						itemidx = 987;// 프로코나 사이드
						break;
					}
					
					if(ch->m_nMoonStoneDigit == 5)
					{
						plus = 5;
					}
					else if(ch->m_nMoonStoneDigit == 4)
					{
						plus = 3;
					}
					
					gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, plus, 0, 1);
					if (!gift)
						return;
					
					if(ch->m_nMoonStoneDigit == 5)
					{
						EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_5PLUS_41LEVEL_WEAPON);
					}
					else if(ch->m_nMoonStoneDigit == 4)
					{
						EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_3PLUS_41LEVEL_WEAPON);
					}
					else
					{
						EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_41LEVEL_WEAPON);
					}
				}
				break;
				
			case 6: // 잭팟.
				{						
					//헬퍼로 보낸다.
					CNetMsg hmsg;
					HelperEventMoonStoneJackPotReqMsg(hmsg, chaindex);
					SEND_Q(hmsg, gserver.m_helper);

					ch->m_nMoonStoneSum = 0;
					ch->m_nMoonStoneDigit = -1;
					EventMoonStoneMsg(rMsg, MSG_EVENT_MOONSTONE_RESULT_REP, MSG_CHANGE_JACK_POT);
					SEND_Q(rMsg, ch->m_desc);
					return;							
				}
				
				break;
			}
		}
		
		ch->m_nMoonStoneSum = 0;
		ch->m_nMoonStoneDigit = -1;
		if (!gift)//경험치나 명성치,숙련도가 올랐을때는 여기서 리턴을...
			return;
		
		// 들어갈 인벤토리 결정
		CInventory* inven = GET_INVENTORY(ch, GET_TAB(gift->m_itemProto->m_typeIdx, gift->m_itemProto->m_subtypeIdx));
		if (!inven)
			return ;
		
		bool bCountable = false;
		// 인벤에 넣기
		if (AddToInventory(ch, gift, true, true))
		{
			// 겹쳐졌는지 검사
			if (gift->tab() == -1)
			{
				bCountable = true;
				
				// 수량 변경 알림
				int r, c;
				if (inven->FindItem(&r, &c, gift->m_idNum, gift->m_plus, gift->m_flag))
				{
					CItem* p = inven->GetItem(r, c);
					if (p)
						ItemUpdateMsg(itemmsg, p, gift->Count());
				}
			}
			else
			{
				ItemAddMsg(itemmsg, gift);
				// 돈 검사
				if (gift->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
					ch->m_moneyItem = gift;
			}
		}
		else
		{				
			// 인벤토리 가득참
			gift = ch->m_pArea->DropItem(gift, ch);
			if (!gift)
				return;
			
			CNetMsg dropMsg;
			gift->m_preferenceIndex = ch->m_index;
			ItemDropMsg(dropMsg, ch, gift);
			ch->m_pArea->SendToCell(dropMsg, ch, true);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
		
		// 메시지 보내고
		SEND_Q(itemmsg, ch->m_desc);
		SEND_Q(rMsg, ch->m_desc);
		
		// Item LOG
		GAMELOG << init("CHANGE_MOONSTONE", ch)
			<< itemlog(gift)
			<< end;
		
		if (bCountable)
			delete gift;
		
	}
}
#endif//문스톤 끝.		

void do_EventLatto(CPC* ch, CNetMsg& msg)
{
	return ;

	unsigned char type;
	msg >> type;

	if (type != (unsigned char) MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REQ)
		return;

	CNetMsg rMsg;

	// 복주머니가 있는가
	int r, c;
	if (!ch->m_invenEvent.FindItem(&r, &c, 507, 0, 0))
	{
		EventLattoMsg(rMsg, MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP, (int) MSG_CHANGE_NO_LUCKYBAG);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
	
	// 찾았으면 수량 확인 해서 줄이기
	CItem* item = ch->m_invenEvent.GetItem(r, c);
	
	if (!item)
	{
		EventLattoMsg(rMsg, MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP, (int) MSG_CHANGE_NO_LUCKYBAG);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
	
	// 필요 숫자 만큼 없음
	if (item->Count() < 1)
	{
		EventLattoMsg(rMsg, MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP, (int) MSG_CHANGE_NO_LUCKYBAG);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	// 복주머니 item 수량 변경
	CNetMsg itemmsg;
	DecreaseFromInventory(ch, item, 1);
	
	if (item->Count() > 0)
	{
		ItemUpdateMsg(itemmsg, item, -1);
		SEND_Q(itemmsg, ch->m_desc);
	}
	else
	{
		ItemDeleteMsg(itemmsg, item);
		SEND_Q(itemmsg, ch->m_desc);
		ch->RemoveItemFromQuickSlot(item);
		RemoveFromInventory(ch, item, true, true);
	}

	int prob = GetRandom(1, 10000);
	CItem* gift = NULL;

	if (prob <= CHANGE_26LEVEL_GENERAL_STONE_PROB)
	{
		// 26 레벨 일반 제련석
		gift = gserver.m_itemProtoList.CreateItem(84, -1, 0, 26, 1);
		if (!gift)
			return;

		EventLattoMsg(rMsg, MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP, (int) MSG_CHANGE_26LEVEL_GENERAL_STONE);
	}
	else if (prob <= CHANGE_SPECIAL_STONE_PROB)
	{
		// 고급 제련석
		gift = gserver.m_itemProtoList.CreateItem(85, -1, 0, 0, 1);
		if (!gift)
			return;

		EventLattoMsg(rMsg, MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP, (int) MSG_CHANGE_SPECIAL_STONE);
	}
	else if (prob <= CHANGE_22LEVEL_EVENT_WEAPON_PROB)
	{
		// 22 레벨 무기 (종족별로)
		int itemidx = -1;
		switch (ch->m_job)
		{
		case JOB_TITAN:
			itemidx = 460;
			break;
		case JOB_KNIGHT:
			itemidx = 427;
			break;
		case JOB_HEALER:
			itemidx = 462;
			break;
		case JOB_MAGE:
			itemidx = 461;
			break;
		case JOB_ROGUE:
			itemidx = 951;
			break;
		case JOB_SORCERER:
			itemidx = 1069;
			break;
		}
		gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);
		if (!gift)
			return;

		EventLattoMsg(rMsg, MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP, (int) MSG_CHANGE_22LEVEL_EVENT_WEAPON);
	}
	else if (prob <= CHANGE_31LEVEL_ARMOR_PROB)
	{
		// 31 레벨 방어구 상의 (종족별로)
		int itemidx = -1;
		switch (ch->m_job)
		{
		case JOB_TITAN:
			itemidx = 313;
			break;
		case JOB_KNIGHT:
			itemidx = 328;
			break;
		case JOB_HEALER:
			itemidx = 347;
			break;
		case JOB_MAGE:
			itemidx = 370;
			break;
		case JOB_ROGUE:
			itemidx = 571;
			break;
		case JOB_SORCERER:
			itemidx = 1006;
			break;
		}
		gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);
		if (!gift)
			return;

		EventLattoMsg(rMsg, MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP, (int) MSG_CHANGE_31LEVEL_ARMOR);
	}
	else if (prob <= CHANGE_ATTACK_UP_PROB)
	{
		// 공격력 향상제 (메이지는 마법, 나머지는 물리) 레벨 3짜리
		gift = gserver.m_itemProtoList.CreateItem(510, -1, 0, 3, 1);
		if (!gift)
			return;

		EventLattoMsg(rMsg, MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP, (int) MSG_CHANGE_ATTACK_UP);
	}
	else if (prob <= CHANGE_DEFENSE_UP_PROB)
	{
		// 방어력 향상제
		gift = gserver.m_itemProtoList.CreateItem(511, -1, 0, 1, 1);
		if (!gift)
			return;

		EventLattoMsg(rMsg, MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP, (int) MSG_CHANGE_DEFENSE_UP);
	}
	else if (prob <= CAHNGE_50000NAS_PROB)
	{
		// 50000 나스
		gift = gserver.m_itemProtoList.CreateItem(19, -1, 0, 0, 50000);
		if (!gift)
			return;

		EventLattoMsg(rMsg, MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP, (int) MSG_CHANGE_50000NAS);
	}
	
	// 들어갈 인벤토리 결정
	CInventory* inven = GET_INVENTORY(ch, GET_TAB(gift->m_itemProto->m_typeIdx, gift->m_itemProto->m_subtypeIdx));
	if (!inven)
		return ;
	
	bool bCountable = false;
	// 인벤에 넣기
	if (AddToInventory(ch, gift, true, true))
	{
		// 겹쳐졌는지 검사
		if (gift->tab() == -1)
		{
			bCountable = true;
			
			// 수량 변경 알림
			int r, c;
			if (inven->FindItem(&r, &c, gift->m_idNum, gift->m_plus, gift->m_flag))
			{
				CItem* p = inven->GetItem(r, c);
				if (p)
					ItemUpdateMsg(itemmsg, p, gift->Count());
			}
		}
		else
		{
			ItemAddMsg(itemmsg, gift);
			// 돈 검사
			if (gift->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
				ch->m_moneyItem = gift;
		}
	}
	else
	{				
		// 인벤토리 가득참
		gift = ch->m_pArea->DropItem(gift, ch);
		if (!gift)
			return;
		
		CNetMsg dropMsg;
		gift->m_preferenceIndex = ch->m_index;
		ItemDropMsg(dropMsg, ch, gift);
		ch->m_pArea->SendToCell(dropMsg, ch, true);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
	
	// 메시지 보내고
	SEND_Q(itemmsg, ch->m_desc);
	SEND_Q(rMsg, ch->m_desc);
	
	// Item LOG
	GAMELOG << init("CHANGE_LUCKYBAG", ch)
			<< itemlog(gift)
			<< end;
	
	if (bCountable)
		delete gift;
}

void do_EventLetter(CPC* ch, CNetMsg& msg)
{
#ifdef EVENT_LETTER_CHANGE
	CNetMsg rmsg;

	unsigned char subtype = (unsigned char)-1;

	msg >> subtype;

	if (subtype != MSG_EVENT_LETTER_REQ)
		return ;

	int count;
	CItem* items[10];
	memset(items, 0, sizeof(CItem*) * 10);

	msg >> count;
	if (count < 1 || count > 10)
	{
		EventLetterMsg(rmsg, MSG_EVENT_LETTER_WRONGPACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 낱말 비트 필드
	int wordmatch = 0;

	int i;
	for (i = 0; i < count; i++)
	{
		int tmp = -1;
		msg >> tmp;

		int r, c;

		// 아이템 찾고
		if (!ch->m_invenNormal.FindItem(&r, &c, tmp, 0, 0))
		{
			EventLetterMsg(rmsg, MSG_EVENT_LETTER_WRONGPACKET);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 아이템 포인터 얻기
		items[i] = ch->m_invenNormal.GetItem(r, c);
		if (items[i] == NULL)
		{
			EventLetterMsg(rmsg, MSG_EVENT_LETTER_WRONGPACKET);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		wordmatch |= GetLetterBit(tmp);
	}

	MSG_EVENT_LETTER_TYPE ltype = CheckLetterBit(wordmatch);

	CItem* gift = NULL;
	int nCount = 0;

	switch (ltype)
	{
	case MSG_EVENT_LETTER_LASTCHAOS:
		nCount = 1;
		gift = gserver.m_itemProtoList.CreateItem(85, -1, 0, 0, 1);
		break;

	case MSG_EVENT_LETTER_NAKO:
		nCount = 1;
		gift = gserver.m_itemProtoList.CreateItem(84, -1, 0, 33, 1);
		break;

	case MSG_EVENT_LETTER_KNIGHT:
		nCount = 100000;
		gift = gserver.m_itemProtoList.CreateItem(0, -1, 0, 0, 100000);
		break;

	case MSG_EVENT_LETTER_MERAC:
		nCount = 10;
		gift = gserver.m_itemProtoList.CreateItem(498, -1, 0, 4, 10);
		break;

	case MSG_EVENT_LETTER_CRIS:
		nCount = 10;
		gift = gserver.m_itemProtoList.CreateItem(500, -1, 0, 4, 10);
		break;

	case MSG_EVENT_LETTER_LAMIAN:
		nCount = 10;
		gift = gserver.m_itemProtoList.CreateItem(44, -1, 0, 0, 10);
		break;

	case MSG_EVENT_LETTER_ARCHWAND:
		nCount = 10;
		gift = gserver.m_itemProtoList.CreateItem(43, -1, 0, 0, 10);
		break;

	default:
		EventLetterMsg(rmsg, MSG_EVENT_LETTER_NOMATCH);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("EVENT LETTER", ch)
			<< "WORD" << delim;

	switch (ltype)
	{
	case MSG_EVENT_LETTER_LASTCHAOS:	GAMELOG << "LASTCHAOS" << end;	break;
	case MSG_EVENT_LETTER_NAKO:			GAMELOG << "NAKO" << end;		break;
	case MSG_EVENT_LETTER_KNIGHT:		GAMELOG << "KNIGHT" << end;		break;
	case MSG_EVENT_LETTER_MERAC:		GAMELOG << "MERAC" << end;		break;
	case MSG_EVENT_LETTER_CRIS:			GAMELOG << "CRIS" << end;		break;
	case MSG_EVENT_LETTER_LAMIAN:		GAMELOG << "LAMIAN" << end;		break;
	case MSG_EVENT_LETTER_ARCHWAND:		GAMELOG << "ARCHWAND" << end;	break;
	default:															break;
	}

	for (i = 0; i < count; i++)
	{
		if (items[i]->Count() > 1)
		{
			DecreaseFromInventory(ch, items[i], 1);
			ItemUpdateMsg(rmsg, items[i], -1);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, items[i]);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, items[i], true, true);
		}
	}

	EventLetterMsg(rmsg, ltype);
	SEND_Q(rmsg, ch->m_desc);

	int r, c;
	if (ch->m_invenNormal.FindItem(&r, &c, gift->m_idNum, gift->m_plus, gift->m_flag))
	{
		AddToInventory(ch, gift, false, false);
		delete gift;
		gift = ch->m_invenNormal.GetItem(r, c);
		ItemUpdateMsg(rmsg, gift, nCount);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		AddToInventory(ch, gift, false, false);
		ItemAddMsg(rmsg, gift);
		SEND_Q(rmsg, ch->m_desc);
	}
#endif
}

void do_EventChangeWeapon(CPC* ch, CNetMsg& msg)
{
#ifdef EVENT_CHANGEWEAPON
	CNetMsg rmsg;

	CItem* olditem = ch->m_wearing[WEARING_WEAPON];

	if (olditem == NULL)
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_WRONG_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (!olditem->IsWeaponType())
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_WRONG_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

#ifdef MONSTER_RAID_SYSTEM
	if (olditem->IsRareItem())
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_WRONG_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif // MONSTER_RAID_SYSTEM

#ifdef SET_ITEM
	if (olditem->IsOriginItem())
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_WRONG_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif

	int newtype = -1;
	switch (olditem->m_itemProto->m_subtypeIdx)
	{
	case IWEAPON_NIGHT:			newtype = IWEAPON_TWOSWORD;		break;
	case IWEAPON_CROSSBOW:		newtype = IWEAPON_SHORTGUM;		break;
	case IWEAPON_STAFF:			newtype = IWEAPON_SHORTSTAFF;	break;
	case IWEAPON_BIGSWORD:		newtype = IWEAPON_AXE;			break;
	case IWEAPON_AXE:			newtype = IWEAPON_BIGSWORD;		break;
	case IWEAPON_SHORTSTAFF:	newtype = IWEAPON_STAFF;		break;
	case IWEAPON_BOW:			newtype = IWEAPON_WAND;			break;
	case IWEAPON_SHORTGUM:		newtype = IWEAPON_CROSSBOW;		break;
	case IWEAPON_TWOSWORD:		newtype = IWEAPON_NIGHT;		break;
	case IWEAPON_WAND:			newtype = IWEAPON_BOW;			break;
	case IWEAPON_SCYTHE:		newtype = IWEAPON_POLEARM;		break;
	case IWEAPON_POLEARM:		newtype = IWEAPON_SCYTHE;		break;
	case IWEAPON_SOUL:			newtype = IWEAPON_SOUL;			break;
	default:
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_WRONG_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int newindex = -1;
	int i;
	for (i = 0; newindex == -1 && i < gserver.m_itemProtoList.m_nCount; i++)
	{
		if (gserver.m_itemProtoList.m_protoItems[i].m_typeIdx == ITYPE_WEAPON)
		{
			if (gserver.m_itemProtoList.m_protoItems[i].m_subtypeIdx == newtype)
			{
				if (gserver.m_itemProtoList.m_protoItems[i].m_level == olditem->m_itemProto->m_level)
				{
					newindex = gserver.m_itemProtoList.m_protoItems[i].m_index;
					break;
				}
			}
		}
	}

	if (newindex == -1)
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_NO_MATCH);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* newitem = gserver.m_itemProtoList.CreateItem(newindex, -1, olditem->m_plus, olditem->m_flag, 1);
	if (newitem == NULL)
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_NO_MATCH);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	newitem->m_used = olditem->m_used;

	// 옵션 옮기기
	switch (olditem->m_itemProto->m_subtypeIdx)
	{
	case IWEAPON_NIGHT:
	case IWEAPON_STAFF:
	case IWEAPON_BIGSWORD:
	case IWEAPON_AXE:
	case IWEAPON_SHORTSTAFF:
	case IWEAPON_MINING:
	case IWEAPON_GATHERING:
	case IWEAPON_CHARGE:
	case IWEAPON_TWOSWORD:
	case IWEAPON_SCYTHE: // 할일 : 소서러 : 사이드 폴암의 옵션 변경
	case IWEAPON_POLEARM:
		newitem->m_nOption = 0;
		for (i = 0; i < olditem->m_nOption; i++)
		{
			if (!olditem->m_option[i].m_proto)
				continue ;
			newitem->m_option[i].m_proto	= olditem->m_option[i].m_proto;
			newitem->m_option[i].m_type		= olditem->m_option[i].m_type;
			newitem->m_option[i].m_level	= olditem->m_option[i].m_level;
			newitem->m_nOption++;
		}
		break;

	case IWEAPON_BOW:
		newitem->m_nOption = 0;
		for (i = 0; i < olditem->m_nOption; i++)
		{
			if (!olditem->m_option[i].m_proto)
				continue ;
			switch (olditem->m_option[i].m_proto->m_type)
			{
			case 6:		// 물리 공격 -> 지혜
				newitem->m_option[i].m_proto	= gserver.m_optionProtoList.FindProto(2);
				break;

			case 8:		// 원거리 공격 -> 마법 공격
				newitem->m_option[i].m_proto	= gserver.m_optionProtoList.FindProto(16);
				break;

			case 10:	// 원거리 명중 -> 마법 명중
				newitem->m_option[i].m_proto	= gserver.m_optionProtoList.FindProto(17);
				break;

			default:
				newitem->m_option[i].m_proto	= olditem->m_option[i].m_proto;
				break;
			}

			newitem->m_option[i].m_type		= newitem->m_option[i].m_proto->m_type;
			newitem->m_option[i].m_level	= olditem->m_option[i].m_level;
			newitem->m_nOption++;
		}
		break;

	case IWEAPON_WAND:
		newitem->m_nOption = 0;
		for (i = 0; i < olditem->m_nOption; i++)
		{
			if (!olditem->m_option[i].m_proto)
				continue ;
			switch (olditem->m_option[i].m_proto->m_type)
			{
			case 2:		// 지혜 -> 물리 공격
				newitem->m_option[i].m_proto	= gserver.m_optionProtoList.FindProto(6);
				break;

			case 16:	// 마법 공격 -> 원거리 공격
				newitem->m_option[i].m_proto	= gserver.m_optionProtoList.FindProto(8);
				break;

			case 17:	// 마법 명중 -> 원거리 명중
				newitem->m_option[i].m_proto	= gserver.m_optionProtoList.FindProto(10);
				break;

			default:
				newitem->m_option[i].m_proto	= olditem->m_option[i].m_proto;
				break;
			}

			newitem->m_option[i].m_type		= newitem->m_option[i].m_proto->m_type;
			newitem->m_option[i].m_level	= olditem->m_option[i].m_level;
			newitem->m_nOption++;
		}
		break;

	case IWEAPON_CROSSBOW:
		newitem->m_nOption = 0;
		for (i = 0; i < olditem->m_nOption; i++)
		{
			if (!olditem->m_option[i].m_proto)
				continue ;
			switch (olditem->m_option[i].m_proto->m_type)
			{
			case 8:		// 원거리 공격 -> 근접 공격
				newitem->m_option[i].m_proto	= gserver.m_optionProtoList.FindProto(7);
				break;

			case 10:	// 원거리 명중 -> 근접 명중
				newitem->m_option[i].m_proto	= gserver.m_optionProtoList.FindProto(9);
				break;

			default:
				newitem->m_option[i].m_proto	= olditem->m_option[i].m_proto;
				break;
			}

			newitem->m_option[i].m_type		= newitem->m_option[i].m_proto->m_type;
			newitem->m_option[i].m_level	= olditem->m_option[i].m_level;
			newitem->m_nOption++;
		}
		break;

	case IWEAPON_SHORTGUM:
		newitem->m_nOption = 0;
		for (i = 0; i < olditem->m_nOption; i++)
		{
			if (!olditem->m_option[i].m_proto)
				continue ;
			switch (olditem->m_option[i].m_proto->m_type)
			{
			case 7:		// 근접 공격 -> 원거리 공격
				newitem->m_option[i].m_proto	= gserver.m_optionProtoList.FindProto(8);
				break;

			case 9:		// 근접 명중 -> 원거리 명중
				newitem->m_option[i].m_proto	= gserver.m_optionProtoList.FindProto(10);
				break;

			default:
				newitem->m_option[i].m_proto	= olditem->m_option[i].m_proto;
				break;
			}

			newitem->m_option[i].m_type		= newitem->m_option[i].m_proto->m_type;
			newitem->m_option[i].m_level	= olditem->m_option[i].m_level;
			newitem->m_nOption++;
		}
		break;
	}

	if (olditem->m_nOption != newitem->m_nOption)
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CANT_OPTION);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	for (i = 0; i < newitem->m_nOption; i++)
	{
		if (!newitem->m_option[i].m_proto)
			continue ;
		newitem->m_option[i].m_value = newitem->m_option[i].m_proto->m_levelValue[newitem->m_option[i].m_level - 1];
		newitem->m_option[i].SetDBValue();
	}

	GAMELOG << init("EVENT CHANGE WEAPON", ch)
			<< "OLD ITEM" << delim
			<< itemlog(olditem) << delim;
	}
	GAMELOG << "NEW ITEM" << delim
			<< itemlog(newitem)
			<< end;

	int olddb = olditem->m_itemProto->m_index;
	int newdb = newitem->m_itemProto->m_index;

	ItemWearMsg(rmsg, WEARING_WEAPON, NULL, olditem);
	SEND_Q(rmsg, ch->m_desc);

	WearingMsg(rmsg, ch, WEARING_WEAPON, -1, 0);
	ch->m_pArea->SendToCell(rmsg, ch, true);

	ItemDeleteMsg(rmsg, olditem);
	SEND_Q(rmsg, ch->m_desc);
	ch->m_invenNormal.Remove(olditem, true, true);

	ch->m_invenNormal.AddItem(newitem, ch, false, true);
	ch->m_wearing[WEARING_WEAPON] = NULL;
	ItemAddMsg(rmsg, newitem);
	SEND_Q(rmsg, ch->m_desc);

	EventChangeWeaponMsg(rmsg, olddb, newdb);
	SEND_Q(rmsg, ch->m_desc);

	ch->m_assist.CheckApplyConditions();
	ch->CalcStatus(true);
#endif
}

void do_EventChuseokUpgrade(CPC* ch, CNetMsg& msg)
{
#ifdef LC_TLD
	#ifdef EVENT_OPEN_BETA_TLD
	if(!gserver.m_bOpenEvent)
		return;

	// 이벤트 인벤에 작물 있나? : 열매 제외
	int rowCrop, colCrop;
	int grade = -1;
	if (ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 876, 0, 0))			grade = 0;	// 씨앗
	else if (ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 877, 0, 0))		grade = 1;	// 묘목
	else if (ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 878, 0, 0))		grade = 2;	// 분재
	else if (ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 879, 0, 0))		grade = 3;	// 나무
	else if (ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 880, 0, 0))		grade = 4;	// 열매

	CNetMsg rmsg;

	int prob = 0;
	int newCropIndex = 0;

	// 배양토 검색
	int rowSoil, colSoil;

	switch (grade)
	{
	case 0:
		prob = 100;
		newCropIndex = 877;
		if (!ch->m_invenNormal.FindItem(&rowSoil, &colSoil, 881, 0, 0))
		{
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_SOIL);
			rmsg << (int) 881;
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		break;
	case 1:
		prob = 70;
		newCropIndex = 878;
		if (!ch->m_invenNormal.FindItem(&rowSoil, &colSoil, 1058, 0, 0))
		{
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_SOIL);
			rmsg << (int) 1058;
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		break;
	case 2:
		prob = 50;
		newCropIndex = 879;
		if (!ch->m_invenNormal.FindItem(&rowSoil, &colSoil, 1059, 0, 0))
		{
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_SOIL);
			rmsg << (int) 1059;
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		break;
	case 3:
		prob = 30;
		newCropIndex = 880;
		if (!ch->m_invenNormal.FindItem(&rowSoil, &colSoil, 1060, 0, 0))
		{
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_SOIL);
			rmsg << (int) 1060;
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		break;
	case 4:
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_ALREADY_UPGRADE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	default:
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_CROP);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* crop = ch->m_invenEvent.GetItem(rowCrop, colCrop);
	CItem* soil = ch->m_invenNormal.GetItem(rowSoil, colSoil);

	if (!crop || !soil)
		return ;

	CItem* newCrop = gserver.m_itemProtoList.CreateItem(newCropIndex, -1, 0, 0, 1);
	if (!newCrop)
		return ;

	if( soil->Count() < 10 )
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_SOIL);
		rmsg << (int) soil->m_itemProto->m_index;
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 배양토 감소
	if (soil->Count() > 10)
	{
		DecreaseFromInventory(ch, soil, 10);
		ItemUpdateMsg(rmsg, soil, -1);
	}
	else
	{
		ItemDeleteMsg(rmsg, soil);
		RemoveFromInventory(ch, soil, true, true);
	}
	SEND_Q(rmsg, ch->m_desc);

	// 확률 적용
	if (GetRandom(1, 100) <= prob)
	{
		// 작물 제거
		ItemDeleteMsg(rmsg, crop);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, crop, true, true);

		// 새 작물 추가
		bool bDrop = false;
		if (AddToInventory(ch, newCrop, true, true))
		{
			ItemAddMsg(rmsg, newCrop);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ch->m_pArea->DropItem(newCrop, ch);
			newCrop->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, newCrop);
			SEND_Q(rmsg, ch->m_desc);
			bDrop = true;
		}

		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_UPGRADE_OK);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("EVENT OPEN BETA UPGRADE SUCCESS", ch->m_name, ch->m_desc->m_idname)
				<< grade << delim
				<< (grade + 1)
				<< ((bDrop) ? "DROP" : "GET")
				<< end;
	}
	else
	{
		delete newCrop;

		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_UPGRADE_FAIL);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("EVENT OPEN BETA UPGRADE FAIL", ch->m_name, ch->m_desc->m_idname)
				<< grade
				<< end;
	}
#endif
#else
	// 이벤트 인벤에 작물 있나? : 열매 제외
	int rowCrop, colCrop;
	int grade = -1;
	if (ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 876, 0, 0))			grade = 0;	// 씨앗
	else if (ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 877, 0, 0))		grade = 1;	// 묘목
	else if (ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 878, 0, 0))		grade = 2;	// 분재
	else if (ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 879, 0, 0))		grade = 3;	// 나무
	else if (ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 880, 0, 0))		grade = 4;	// 열매

	CNetMsg rmsg;

	int prob = 0;
	int newCropIndex = 0;
	switch (grade)
	{
	case 0:
		prob = 100;
		newCropIndex = 877;
		break;
	case 1:
		prob = 70;
		newCropIndex = 878;
		break;
	case 2:
		prob = 50;
		newCropIndex = 879;
		break;
	case 3:
		prob = 30;
		newCropIndex = 880;
		break;
	case 4:
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_ALREADY_UPGRADE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	default:
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_CROP);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 배양토 검색
	int rowSoil, colSoil;
	if (!ch->m_invenNormal.FindItem(&rowSoil, &colSoil, 881, 0, 0))
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_SOIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* crop = ch->m_invenEvent.GetItem(rowCrop, colCrop);
	CItem* soil = ch->m_invenNormal.GetItem(rowSoil, colSoil);

	if (!crop || !soil)
		return ;

	CItem* newCrop = gserver.m_itemProtoList.CreateItem(newCropIndex, -1, 0, 0, 1);
	if (!newCrop)
		return ;

	// 배양토 감소
	if (soil->Count() > 1)
	{
		DecreaseFromInventory(ch, soil, 1);
		ItemUpdateMsg(rmsg, soil, -1);
	}
	else
	{
		ItemDeleteMsg(rmsg, soil);
		RemoveFromInventory(ch, soil, true, true);
	}
	SEND_Q(rmsg, ch->m_desc);

	// 확률 적용
	if (GetRandom(1, 100) <= prob)
	{
		// 작물 제거
		ItemDeleteMsg(rmsg, crop);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, crop, true, true);

		// 새 작물 추가
		bool bDrop = false;
		if (AddToInventory(ch, newCrop, true, true))
		{
			ItemAddMsg(rmsg, newCrop);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ch->m_pArea->DropItem(newCrop, ch);
			newCrop->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, newCrop);
			SEND_Q(rmsg, ch->m_desc);
			bDrop = true;
		}

		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_UPGRADE_OK);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("EVENT CHUSEOK UPGRADE SUCCESS", ch)
				<< grade << delim
				<< (grade + 1)
				<< ((bDrop) ? "DROP" : "GET")
				<< end;
	}
	else
	{
		delete newCrop;

		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_UPGRADE_FAIL);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("EVENT CHUSEOK UPGRADE FAIL", ch)
				<< grade
				<< end;
	}
#endif
}

void do_EventChuseokExchange(CPC* ch, CNetMsg& msg)
{
#ifdef LC_TLD
#ifdef EVENT_OPEN_BETA_TLD
	if(!gserver.m_bOpenEvent)
		return;

	CNetMsg rmsg;

	// 작물 찾기
	int r, c;
	int grade = -1;
	if (ch->m_invenEvent.FindItem(&r, &c, 876, 0, 0))			grade = 0;	// 씨앗
	else if (ch->m_invenEvent.FindItem(&r, &c, 877, 0, 0))		grade = 1;	// 묘목
	else if (ch->m_invenEvent.FindItem(&r, &c, 878, 0, 0))		grade = 2;	// 분재
	else if (ch->m_invenEvent.FindItem(&r, &c, 879, 0, 0))		grade = 3;	// 나무
	else if (ch->m_invenEvent.FindItem(&r, &c, 880, 0, 0))		grade = 4;	// 열매
	else
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_CROP);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	CItem* crop = ch->m_invenEvent.GetItem(r, c);
	if (!crop)
		return ;

	CItem* item = NULL;

	switch (grade)
	{
	case 0:		//  기사의 문장
		item = gserver.m_itemProtoList.CreateItem(1061, -1, 0, 0, 1);
		break;
	case 1:		// 남작의 문장
		item = gserver.m_itemProtoList.CreateItem(1062, -1, 0, 0, 1);
		break;
	case 2:		// 백작의 문장
		item = gserver.m_itemProtoList.CreateItem(1063, -1, 0, 0, 1);
		break;
	case 3:		// 후작의 문장
		item = gserver.m_itemProtoList.CreateItem(1064, -1, 0, 0, 1);
		break;
	case 4:		// 공작의 문장
		item = gserver.m_itemProtoList.CreateItem(1065, -1, 0, 0, 1);
		break;
	default:
		return ;
	}

	if (item == NULL)
		return ;

#ifdef MONSTER_RAID_SYSTEM
		if (item->IsRareItem())
		{
			delete item;
			return ;
		}
#endif // MONSTER_RAID_SYSTEM

#ifdef SET_ITEM
		if (item->IsOriginItem())
		{
			delete item;
			return ;
		}
#endif // SET_ITEM

	int i;

	// 악세사리 일때 Option Setting
	if (item->IsAccessary() && item->m_nOption < 1)
	{
		// 순서대로 5,4,3,1개 붙을 확률
		static const int factor[] = {10, 40, 80, 100};
		static const int limit[] = {500, 2000, 4000, 5000};

		int prob[MAX_ACCESSORY_OPTION-1];

		for (i=0; i < MAX_ACCESSORY_OPTION - 1; i++)
		{
			if ((prob[i] = ch->m_level * factor[i]) > limit[0])
				prob[i] = limit[i];
		}

		// 옵션 수 결정
		int result = GetRandom(1, 10000);

		// 최소 3개
		int num = 3;	
		int* type;
		
		if (result <= prob[0])
			num = 5;
		else if (result <= prob[1])
			num = 4;
		else if (result <= prob[2])
			num = 3;
		else if (result <= prob[3])
			num = 3;
		else 
			num = 3;

		switch(item->m_itemProto->m_index)
		{
		case 1062:
		case 1063:
			if( num < 4 ) num = 4;
			break;
		case 1064:
		case 1065:
			num = 5;
			break;
		}

		type = new int[num];

		// 옵션 타입 결정
		i = 0;
		char checkRep = 0;
		while (i < num)
		{
			// 근거리 무기 : 0-힘,3-체질, 4-생명력, 7-물근공, 20-전공, 11-물방  : 6종
			// 원거리 무기 : 민첩-1,전회-23,전명-21,물원공-8,전공-20,물방-11	: 6종
			// 마법 무기 : 지혜-2, 마공-16, 마나-5, 생명-4,전공-20,물방-11		: 6종
			
			// type[i] 결정
			// 중복 검사를 위해 checkRep를 비트를 사용하고 이미 옵션이 붙었으면 다음것 또 다음 것 또 다음 것. 마지막 일것일 경우
			// goto start 포지션으로 옴
			if(ch->m_wearing[WEARING_WEAPON] && ch->m_wearing[WEARING_WEAPON]->m_wearPos != -1 )
			{
				switch( rand() % 6 )
				{
start:
				checkRep = checkRep | ( 1 << 6 );
				case 0:
					if(ch->m_wearing[WEARING_WEAPON]->IsMagicWeapon() )
						type[i] = 2;
					else if ( ch->m_wearing[WEARING_WEAPON]->IsRangeWeapon() )
						type[i] = 1;
					else
						type[i] = 0;

					if( !(checkRep & (1 << 0)) ) 
					{
						checkRep = checkRep | ( 1 << 0 );
						break;
					}
				case 1:
					if(ch->m_wearing[WEARING_WEAPON]->IsMagicWeapon() )
						type[i] = 16;
					else if ( ch->m_wearing[WEARING_WEAPON]->IsRangeWeapon() )
						type[i] = 23;
					else
						type[i] = 3;

					if( !(checkRep & (1 << 1)) ) 
					{
						checkRep = checkRep | ( 1 << 1 );
						break;
					}
				case 2:
					if(ch->m_wearing[WEARING_WEAPON]->IsMagicWeapon() )
						type[i] = 5;
					else if ( ch->m_wearing[WEARING_WEAPON]->IsRangeWeapon() )
						type[i] = 21;
					else
						type[i] = 3;

					if( !(checkRep & (1 << 2)) ) 
					{
						checkRep = checkRep | ( 1 << 2 );
						break;
					}
				case 3:
					if(ch->m_wearing[WEARING_WEAPON]->IsMagicWeapon() )
						type[i] = 4;
					else if ( ch->m_wearing[WEARING_WEAPON]->IsRangeWeapon() )
						type[i] = 8;
					else
						type[i] = 7;

					if( !(checkRep & (1 << 3)) ) 
					{
						checkRep = checkRep | ( 1 << 3 );
						break;
					}
				case 4:
					type[i] = 20;

					if( !(checkRep & (1 << 4))) 
					{
						checkRep = checkRep | ( 1 << 4 );
						break;
					}
				case 5:
					type[i] = 11;

					if( !(checkRep & (1 << 5)) ) 
					{
						checkRep = checkRep | ( 1 << 5 );
						break;
					}
					else
					{
						// 한번이라도 스타트 레이블로 간적이 있으면 종료한다.
						if( checkRep & ( 1 << 6 ) )
							break;
						goto start;
					}
				default:
					break;
				}
			}
			else
			{
				switch( rand() % 6 )
				{
start2:
				checkRep = checkRep | ( 1 << 6 );
				case 0:
					switch( rand() % 3)
					{
					case 0:
						type[i] = 2;
						break;
					case 1:
						type[i] = 1;
						break;
					case 2:
						type[i] = 0;
						break;
					default :
						break;
					}

					if( !(checkRep & (1 << 0)) ) 
					{
						checkRep = checkRep | ( 1 << 0 );
						break;
					}
				case 1:
					switch( rand() % 3)
					{
					case 0:
						type[i] = 16;
						break;
					case 1:
						type[i] = 23;
						break;
					case 2:
						type[i] = 3;
						break;
					default :
						break;
					}

					if( !(checkRep & (1 << 1)) ) 
					{
						checkRep = checkRep | ( 1 << 1 );
						break;
					}
				case 2:
					switch( rand() % 3)
					{
					case 0:
						type[i] = 5;
						break;
					case 1:
						type[i] = 21;
						break;
					case 2:
						type[i] = 3;
						break;
					default :
						break;
					}

					if( !(checkRep & (1 << 2)) ) 
					{
						checkRep = checkRep | ( 1 << 2 );
						break;
					}
				case 3:
					switch( rand() % 3)
					{
					case 0:
						type[i] = 4;
						break;
					case 1:
						type[i] = 8;
						break;
					case 2:
						type[i] = 7;
						break;
					default :
						break;
					}

					if( !(checkRep & (1 << 3)) ) 
					{
						checkRep = checkRep | ( 1 << 3 );
						break;
					}
				case 4:
					type[i] = 20;

					if( !(checkRep & (1 << 4))) 
					{
						checkRep = checkRep | ( 1 << 4 );
						break;
					}
				case 5:
					type[i] = 11;

					if( !(checkRep & (1 << 5)) ) 
					{
						checkRep = checkRep | ( 1 << 5 );
						break;
					}
					else
					{
						// 한번이라도 스타트 레이블로 간적이 있으면 종료한다.
						if( checkRep & ( 1 << 6 ) )
							break;
						goto start2;
					}
				default:
					break;
				}
			}

			i++;
		}

		// 옵션 부여
		for (i=0; i < num; i++)
		{
			COptionProto* proto = gserver.m_optionProtoList.FindProto(type[i]);
			
			if (!proto)
				continue;
			
			item->m_nOption++;
			// Level, Value 셋팅
			item->m_option[i].MakeOptionValue(proto, ch->m_level, num);
		}

		int bestOptionCount = 0;
		switch(item->m_itemProto->m_index)
		{
			// 최고등급 2개이상
		case 1063:
			for(i = 0; i < num; i++)
			{
				if(item->m_option[i].m_level == 3)
					bestOptionCount++;
			}
			if( bestOptionCount < 2 )
			{
				for(i = 0; i < num && bestOptionCount < 2; i++)
				{
					if( item->m_option[i].m_level != 3 )
					{
						item->m_option[i].m_level = 3;
						item->m_option[i].m_value = item->m_option[i].m_proto->m_levelValue[2];
						bestOptionCount++;
					}
				}
			}
			break;
			// 최고등급 3개이상
		case 1064:
			for(i = 0; i < num; i++)
			{
				if(item->m_option[i].m_level == 3)
					bestOptionCount++;
			}
			if( bestOptionCount < 3 )
			{
				for(i = 0; i < num && bestOptionCount < 3; i++)
				{
					if( item->m_option[i].m_level != 3 )
					{
						item->m_option[i].m_level = 3;
						item->m_option[i].m_value = item->m_option[i].m_proto->m_levelValue[2];
						bestOptionCount++;
					}
				}
			}
			break;
			// 최고등급 4개이상
		case 1065:
			for(i = 0; i < num; i++)
			{
				if(item->m_option[i].m_level == 3)
					bestOptionCount++;
			}
			if( bestOptionCount < 4 )
			{
				for(i = 0; i < num && bestOptionCount < 4; i++)
				{
					if( item->m_option[i].m_level != 3 )
					{
						item->m_option[i].m_level = 3;
						item->m_option[i].m_value = item->m_option[i].m_proto->m_levelValue[2];
						bestOptionCount++;
					}
				}
			}
			break;
		}

		delete [] type;
	}


	// 작물 수거
	ItemDeleteMsg(rmsg, crop);
	SEND_Q(rmsg, ch->m_desc);
	RemoveFromInventory(ch, crop, true, true);

	// 아이템 지급
	bool bDrop = false;
	if (AddToInventory(ch, item, true, true))
	{
		if (item->tab() < 0)
		{
			// 겹치면 수량 변경 알림
			ch->m_invenNormal.FindItem(&r, &c, item->m_idNum, 0, 0);
			delete item;
			item = ch->m_invenNormal.GetItem(r, c);
			ItemUpdateMsg(rmsg, item, 1);
		}
		else
		{
			// 추가 알림
			ItemAddMsg(rmsg, item);
		}
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		ch->m_pArea->DropItem(item, ch);
		item->m_preferenceIndex = ch->m_index;
		ItemDropMsg(rmsg, ch, item);
		SEND_Q(rmsg, ch->m_desc);
		bDrop = true;
	}

	EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_EXCHANGE_OK);
	SEND_Q(rmsg, ch->m_desc);

	GAMELOG << init("EVENT OPEN BETA EXCHANGE", ch->m_name, ch->m_desc->m_idname)
			<< "GRADE" << delim
			<< grade << delim
			<< "ITEM" << delim
			<< itemlog(item) << delim
			<< ((bDrop) ? "DROP" : "GET")
			<< end;

	// 이벤트 변수 세팅
	ch->m_etcEvent = ch->m_etcEvent | OPEN_EVENT_REWARDED;

#endif
#else
	// 확률 테이블 : 누적확률%
	const int probTable[5][5] = {
		{ 70,  85,  95, 100, 100},		// 씨앗 : 70 15 10 5 0
		{ 35,  65,  85,  95, 100},		// 묘목 : 35 30 20 10 5
		{ 20,  45,  75,  90, 100},		// 분재 : 20 25 30 15 10
		{ 10,  25,  50,  80, 100},		// 나무 : 10 15 25 30 20
		{  0,  10,  40,  70, 100}		// 열매 : 0 10 30 30 30
	};

	CNetMsg rmsg;

	// 작물 찾기
	int r, c;
	int grade = -1;
	if (ch->m_invenEvent.FindItem(&r, &c, 876, 0, 0))			grade = 0;	// 씨앗
	else if (ch->m_invenEvent.FindItem(&r, &c, 877, 0, 0))		grade = 1;	// 묘목
	else if (ch->m_invenEvent.FindItem(&r, &c, 878, 0, 0))		grade = 2;	// 분재
	else if (ch->m_invenEvent.FindItem(&r, &c, 879, 0, 0))		grade = 3;	// 나무
	else if (ch->m_invenEvent.FindItem(&r, &c, 880, 0, 0))		grade = 4;	// 열매
	else
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_CROP);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	CItem* crop = ch->m_invenEvent.GetItem(r, c);
	if (!crop)
		return ;

	// 확률 적용
	int prob = GetRandom(1, 100);
	int i;
	for (i = 0; i < 5; i++)
	{
		if (prob <= probTable[grade][i])
			break;
	}

	CItem* item = NULL;
	int itemindex = 0;
	LONGLONG itemcount = 0;

	switch (i)
	{
	case 0:		// 10000 나스
		itemindex = gserver.m_itemProtoList.m_moneyItem->m_index;
		itemcount = 10000;
		break;
	case 1:		// 50000 나스
		itemindex = gserver.m_itemProtoList.m_moneyItem->m_index;
		itemcount = 50000;
		break;
	case 2:		// 200000 나스
		itemindex = gserver.m_itemProtoList.m_moneyItem->m_index;
		itemcount = 200000;
		break;
	case 3:		// 500000 나스
		itemindex = gserver.m_itemProtoList.m_moneyItem->m_index;
		itemcount = 500000;
		break;
	case 4:		// 고제 2 개
		itemindex = gserver.m_itemProtoList.m_specialRefineItem->m_index;
		itemcount = 2;
		break;
	default:
		return ;
	}

	item = gserver.m_itemProtoList.CreateItem(itemindex, -1, 0, 0, itemcount);

	if (item == NULL)
		return ;

	// 작물 수거
	ItemDeleteMsg(rmsg, crop);
	SEND_Q(rmsg, ch->m_desc);
	RemoveFromInventory(ch, crop, true, true);

	// 아이템 지급
	bool bDrop = false;
	if (AddToInventory(ch, item, true, true))
	{
		if (item->tab() < 0)
		{
			// 겹치면 수량 변경 알림
			ch->m_invenNormal.FindItem(&r, &c, item->m_idNum, 0, 0);
			delete item;
			item = ch->m_invenNormal.GetItem(r, c);
			ItemUpdateMsg(rmsg, item, itemcount);
		}
		else
		{
			// 추가 알림
			ItemAddMsg(rmsg, item);
		}
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		ch->m_pArea->DropItem(item, ch);
		item->m_preferenceIndex = ch->m_index;
		ItemDropMsg(rmsg, ch, item);
		SEND_Q(rmsg, ch->m_desc);
		bDrop = true;
	}

	EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_EXCHANGE_OK);
	SEND_Q(rmsg, ch->m_desc);

	GAMELOG << init("EVENT CHUSEOK EXCHANGE", ch)
			<< "GRADE" << delim
			<< grade << delim
			<< "ITEM" << delim
			<< itemindex << delim
			<< itemcount << delim
			<< ((bDrop) ? "DROP" : "GET")
			<< end;
#endif
}

#ifdef EVENT_CHANGE_WITHOUT_OPTION
void do_EventChangeWithoutOption(CPC* ch, CNetMsg& msg)
{
	int olditem = 0;
	int newsubtype = 0;
	msg >> olditem
		>> newsubtype;

	CItem* item = ch->m_invenNormal.GetItem(olditem);
	if (!item)
		return ;

	if (!item->IsWeaponType())
		return ;

	if (item->m_itemProto->m_subtypeIdx == newsubtype)
		return ;

	if (newsubtype < 0 || newsubtype >= 16)
		return ;

	int itemlevel = item->m_itemProto->m_level;

	// 레벨, 한손검, 석궁, 스태프, 대검, 도끼, 숏스테프, 활, 단검, -1, -1, -1, 이도류, 완드, 사이드, 폴암 : 할일 : 소서러
	const static int matchTable[12][16] = {
		{1,48,530,600,12,558,356,50,528,-1,-1,-1,459,599,-1,-1},
		{5,53,666,628,51,601,357,56,529,-1,-1,-1,610,619,-1,-1},
		{9,55,637,629,52,602,358,57,532,-1,-1,-1,611,620,-1,-1},
		{13,107,638,630,105,603,359,106,533,-1,-1,-1,612,621,-1,-1},
		{17,180,639,631,306,604,360,185,534,-1,-1,-1,613,622,-1,-1},
		{21,322,640,632,307,605,361,341,535,-1,-1,-1,614,623,-1,-1},
		{25,323,641,633,308,606,362,342,536,-1,-1,-1,615,624,-1,-1},
		{29,324,642,634,309,607,363,343,537,-1,-1,-1,616,625,-1,-1},
		{33,325,643,635,310,608,364,344,538,-1,-1,-1,617,626,-1,-1},
		{37,326,644,636,311,609,365,345,539,-1,-1,-1,618,627,-1,-1},
		{41,681,721,719,715,645,720,718,722,-1,-1,-1,716,717,-1,-1},
		{45,800,823,816,793,794,815,808,822,-1,-1,-1,801,809,-1,-1}
	};

	CItemProto* proto = NULL;
	int i;
	for (i = 0; i < 12; i++)
	{
		if (matchTable[i][0] == itemlevel)
		{
			proto = gserver.m_itemProtoList.FindIndex(matchTable[i][newsubtype + 1]);
			break;
		}
	}

	if (proto == NULL)
		return ;

	CItem* newitem = gserver.m_itemProtoList.CreateItem(proto->m_index, -1, item->m_plus, item->m_flag, 1);
	if (!newitem)
		return ;

	CNetMsg rmsg;

	GAMELOG << init("EVENT CHANGE WITHOUT OPTION", ch)
			<< "OLD" << delim
			<< itemlog(item) << delim;

	GAMELOG << "NEW" << delim
			<< itemlog(newitem) << delim;

	ItemDeleteMsg(rmsg, item);
	SEND_Q(rmsg, ch->m_desc);
	RemoveFromInventory(ch, item, true, true);

	if (AddToInventory(ch, newitem, true, true))
	{
		ItemAddMsg(rmsg, newitem);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << "ADD INVEN"
				<< end;
	}
	else
	{
		ch->m_pArea->DropItem(newitem, ch);
		newitem->m_preferenceIndex = ch->m_index;
		ItemDropMsg(rmsg, ch, newitem);
		ch->m_pArea->SendToCell(rmsg, GET_YLAYER(newitem), newitem->m_cellX, newitem->m_cellZ);
		GAMELOG << "DROP"
				<< end;
	}

	EventChangeWithoutOptionMsg(rmsg, olditem, newsubtype);
	SEND_Q(rmsg, ch->m_desc);
}
#endif // #ifdef EVENT_CHANGE_WITHOUT_OPTION

#ifdef EVENT_2PAN4PAN_LETTER
int event2pan4panLetters[19] = 
{
	952,	// 이벤트[이]
	953,	// 이벤트[판]
	954,	// 이벤트[사]
	955,	// 이벤트[노]
	956,	// 이벤트[라]
	957,	// 이벤트[바]
	958,	// 이벤트[스]
	959,	// 이벤트[트]
	960,	// 이벤트[카]
	961,	// 이벤트[오]
	962,	// 이벤트[코]
	963,	// 이벤트[믹]
	964,	// 이벤트[액]
	965,	// 이벤트[션]
	966,	// 이벤트[싸]
	967,	// 이벤트[워]
	968,	// 이벤트[그]
	969,	// 이벤트[하]
	970,	// 이벤트[임]
};
int event2pan4panWords[5][7][2] = {
	{{962, 1}, {963, 1}, {964, 1}, {965, 1}, {966, 1}, {967, 1}, {-1, 0}},		// MSG_EVENT_2PAN4PAN_WORD_SSAWAR   
	{{952, 1}, {953, 2}, {954, 1}, { -1, 0}, { -1, 0}, { -1, 0}, {-1, 0}},		// MSG_EVENT_2PAN4PAN_WORD_2PAN4PAN 
	{{956, 1}, {958, 2}, {959, 1}, {960, 1}, {961, 1}, { -1, 0}, {-1, 0}},		// MSG_EVENT_2PAN4PAN_WORD_LASTCHAOS
	{{956, 1}, {968, 1}, {969, 1}, {970, 1}, { -1, 0}, { -1, 0}, {-1, 0}},		// MSG_EVENT_2PAN4PAN_WORD_LAGHAIM  
	{{955, 1}, {956, 1}, {957, 1}, { -1, 0}, { -1, 0}, { -1, 0}, {-1, 0}}		// MSG_EVENT_2PAN4PAN_WORD_NORABA   
};

#ifndef TEST_SERVER
void do_Event2pan4panLetter(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int wordtype = -1;
	msg >> wordtype;
	if (wordtype < 0 || wordtype >= 5)
		return ;

	// 해당 아이템 소유 여부
	int i;
	for (i = 0; i < 7 && event2pan4panWords[wordtype][i][1] > 0; i++)
	{
		if (event2pan4panWords[wordtype][i][0] != -1)
		{
			int r, c;
			if (!ch->m_invenNormal.FindItem(&r, &c, event2pan4panWords[wordtype][i][0], 0, 0))
			{
				Event2pan4panLetterMsg(rmsg, MSG_EVENT_2PAN4PAN_WORD_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			CItem* item = ch->m_invenNormal.GetItem(r, c);
			if (!item || item->Count() < event2pan4panWords[wordtype][i][1])
			{
				Event2pan4panLetterMsg(rmsg, MSG_EVENT_2PAN4PAN_WORD_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
	}

	// 컨넥터에 요청
	ConnEvent2pan4panLetterReqMsg(rmsg, ch->m_desc->m_index, ch->m_index, (MSG_EVENT_2PAN4PAN_WORD_TYPE)wordtype);
	if (IS_RUNNING_CONN)
	{
		GAMELOG << init("EVENT 2PAN4PAN LETTER REQUEST", ch)
				<< "WORDTYPE" << delim
				<< wordtype
				<< end;
		SEND_Q(rmsg, gserver.m_connector);
	}
}
#endif // #ifndef TEST_SERVER
#endif // #ifdef EVENT_2PAN4PAN_LETTER

#ifdef EVENT_2PAN4PAN_GOODS
void do_Event2pan4panGoodsCheck(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	if (ch->m_desc->m_nEvent2pan4panGoods > 0)
	{
		Event2pan4panGoodsCheckMsg(rmsg, ch->m_desc->m_nEvent2pan4panGoods);
		SEND_Q(rmsg, ch->m_desc);
	}
	if (IS_RUNNING_CONN)
	{
		ConnEvent2pan4panGoodsReqMsg(rmsg, ch->m_desc->m_index, ch->m_index);
		SEND_Q(rmsg, gserver.m_connector);
	}
}

void do_Event2pan4panGoodsReq(CPC* ch, CNetMsg& msg)
{
	if (IS_RUNNING_CONN)
	{
		CNetMsg rmsg;

		if (ch->m_desc->m_nEvent2pan4panGoods > 0)
		{
			GAMELOG << init("EVENT 2PAN4PAN GOODS", ch)
					<< "GIVE" << delim;

			// 아이템 지급 후
			int givegoodsindex[20];
			int giveitemindex[20];
			LONGLONG giveitemcount[20];
			int nGive = 0;

			int i;
			for (i = 0; i < 20; i++)
			{
				if (i < ch->m_desc->m_nEvent2pan4panGoods)
				{
					CItem* item = NULL;

					int itemindex = ch->m_desc->m_event2pan4panGoods[i].itemindex;
					int itemflag = 0;
					LONGLONG itemcount = ch->m_desc->m_event2pan4panGoods[i].itemcount;

					// 아이템 생성
					switch (itemindex)
					{
					case 498:	itemflag = 12;		break;	// 붉은 케르 12 레벨
					case 500:	itemflag = 12;		break;	// 푸른 케르 12 레벨
					case 85:	itemflag = 0;		break;	// 고급제련석
					default:	itemindex = -1;		break;
					}

					item = gserver.m_itemProtoList.CreateItem(itemindex, -1, 0, itemflag, itemcount);
					if (item)
					{
						// 지급
						if (AddToInventory(ch, item, true, true))
						{
							GAMELOG << "ITEM" << delim
									<< itemlog(item) << delim;
							if (item->tab() < 0)
							{
								// 겹침
								delete item;
								int r, c;
								if (ch->m_invenNormal.FindItem(&r, &c, itemindex, 0, itemflag))
								{
									item = ch->m_invenNormal.GetItem(r, c);
									if (item)
									{
										ItemUpdateMsg(rmsg, item, itemcount);
										SEND_Q(rmsg, ch->m_desc);
									}
								}
							}
							else
							{
								// 추가
								ItemAddMsg(rmsg, item);
								SEND_Q(rmsg, ch->m_desc);
							}
							givegoodsindex[nGive] = ch->m_desc->m_event2pan4panGoods[i].goodsindex;
							giveitemindex[nGive] = ch->m_desc->m_event2pan4panGoods[i].itemindex;
							giveitemcount[nGive] = ch->m_desc->m_event2pan4panGoods[i].itemcount;
							nGive++;
						}
						else
						{
							delete item;
						}
					}
				}
				else
					break;
			}

			GAMELOG << "TOTAL" << delim
					<< nGive
					<< end;

			// 결과를 컨넥터로
			if (nGive > 0)
			{
				if (nGive == ch->m_desc->m_nEvent2pan4panGoods)
					Event2pan4panGoodsRepMsg(rmsg, MSG_EVENT_2PAN4PAN_GOODS_ERROR_OK, nGive, giveitemindex, giveitemcount);
				else
					Event2pan4panGoodsRepMsg(rmsg, MSG_EVENT_2PAN4PAN_GOODS_ERROR_SOMEREMAIN, nGive, giveitemindex, giveitemcount);
				SEND_Q(rmsg, ch->m_desc);
				ConnEvent2pan4panGoodsCommitMsg(rmsg, ch->m_desc->m_index, ch->m_index, nGive, givegoodsindex);
				SEND_Q(rmsg, gserver.m_connector);

				// 초기화
				ch->m_desc->m_nEvent2pan4panGoods = 0;
				delete [] ch->m_desc->m_event2pan4panGoods;
				ch->m_desc->m_event2pan4panGoods = NULL;
			}
			else
			{
				Event2pan4panGoodsRepMsg(rmsg, MSG_EVENT_2PAN4PAN_GOODS_ERROR_NOSPACE, 0, NULL, NULL);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		else
		{
			GAMELOG << init("EVENT 2PAN4PAN GOODS", ch)
					<< "NO ITEMS"
					<< end;
			Event2pan4panGoodsRepMsg(rmsg, MSG_EVENT_2PAN4PAN_GOODS_ERROR_NOGOODS, 0, NULL, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}
#endif // #ifdef EVENT_2PAN4PAN_GOODS

#ifdef EVENT_2PAN4PAN_BOX
void do_Event2pan4panBoxCheck(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	if (ch->m_desc->m_nEvent2pan4panBox > 0)
	{
		Event2pan4panBoxCheckMsg(rmsg, ch->m_desc->m_nEvent2pan4panBox);
		SEND_Q(rmsg, ch->m_desc);
	}
	if (IS_RUNNING_CONN)
	{
		ConnEvent2pan4panBoxReqMsg(rmsg, ch->m_desc->m_index, ch->m_index);
		SEND_Q(rmsg, gserver.m_connector);
	}
}

void do_Event2pan4panBoxReq(CPC* ch, CNetMsg& msg)
{
	if (IS_RUNNING_CONN)
	{
		CNetMsg rmsg;

		if (ch->m_desc->m_nEvent2pan4panBox > 0)
		{
			GAMELOG << init("EVENT 2PAN4PAN BOX", ch)
					<< "GIVE" << delim;

			// 아이템 지급 후
			int givegoodsindex[20];
			int giveitemindex[20];
			LONGLONG giveitemcount[20];
			int nGive = 0;

			int i;
			for (i = 0; i < 20; i++)
			{
				if (i < ch->m_desc->m_nEvent2pan4panBox)
				{
					CItem* item = NULL;

					int itemindex = ch->m_desc->m_event2pan4panBox[i].itemindex;
					LONGLONG itemcount = ch->m_desc->m_event2pan4panBox[i].itemcount;

					// 아이템 생성
					switch (itemindex)
					{
					case 1053:
						switch (GetRandom(1, 3))
						{
						case 1:		itemindex = 786;	itemcount = 5;		break;		// 부스터 5개
						case 2:		itemindex = 723;	itemcount = 5;		break;		// 문스톤 5개
						case 3:		itemindex = 1050;	itemcount = 10;		break;		// 물약 10개
						default:	itemindex = -1;		itemcount = 0;		break;
						}
						break;
					default:		itemindex = -1;		itemcount = 0;		break;
					}

					item = gserver.m_itemProtoList.CreateItem(itemindex, -1, 0, 0, itemcount);
					if (item)
					{
						// 지급
						if (AddToInventory(ch, item, true, true))
						{
							GAMELOG << "ITEM" << delim
									<< itemlog(item) << delim;
							if (item->tab() < 0)
							{
								// 겹침
								delete item;
								int r, c;
								if (ch->m_invenNormal.FindItem(&r, &c, itemindex, 0, 0))
								{
									item = ch->m_invenNormal.GetItem(r, c);
									if (item)
									{
										ItemUpdateMsg(rmsg, item, itemcount);
										SEND_Q(rmsg, ch->m_desc);
									}
								}
							}
							else
							{
								// 추가
								ItemAddMsg(rmsg, item);
								SEND_Q(rmsg, ch->m_desc);
							}
							givegoodsindex[nGive] = ch->m_desc->m_event2pan4panBox[i].goodsindex;
							giveitemindex[nGive] = ch->m_desc->m_event2pan4panBox[i].itemindex;
							giveitemcount[nGive] = ch->m_desc->m_event2pan4panBox[i].itemcount;
							nGive++;
						}
						else
						{
							delete item;
						}
					}
				}
				else
					break;
			}

			GAMELOG << "TOTAL" << delim
					<< nGive
					<< end;

			// 결과를 컨넥터로
			if (nGive > 0)
			{
				if (nGive == ch->m_desc->m_nEvent2pan4panBox)
					Event2pan4panBoxRepMsg(rmsg, MSG_EVENT_2PAN4PAN_BOX_ERROR_OK, nGive, giveitemindex, giveitemcount);
				else
					Event2pan4panBoxRepMsg(rmsg, MSG_EVENT_2PAN4PAN_BOX_ERROR_SOMEREMAIN, nGive, giveitemindex, giveitemcount);
				SEND_Q(rmsg, ch->m_desc);
				ConnEvent2pan4panBoxCommitMsg(rmsg, ch->m_desc->m_index, ch->m_index, nGive, givegoodsindex);
				SEND_Q(rmsg, gserver.m_connector);

				// 초기화
				ch->m_desc->m_nEvent2pan4panBox = 0;
				delete [] ch->m_desc->m_event2pan4panBox;
				ch->m_desc->m_event2pan4panBox = NULL;
			}
			else
			{
				Event2pan4panBoxRepMsg(rmsg, MSG_EVENT_2PAN4PAN_BOX_ERROR_NOSPACE, 0, NULL, NULL);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		else
		{
			GAMELOG << init("EVENT 2PAN4PAN BOX", ch)
					<< "NO ITEMS"
					<< end;
			Event2pan4panBoxRepMsg(rmsg, MSG_EVENT_2PAN4PAN_BOX_ERROR_NOBOX, 0, NULL, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}
#endif // #ifdef EVENT_2PAN4PAN_BOX

#ifdef EVENT_2PAN4PAN_MONEY
void do_Event2pan4panMoneyReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int type = 0;
	msg >> type;

	// 아이템 소지 검사
	int nItemIndex = -1;
	switch (type)
	{
	case 1:		nItemIndex = 1051;		break;		// 맞고
	case 2:		nItemIndex = 1052;		break;		// 포커
	default:
		Event2pan4panMoneyRepMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 프로토를 찾아서
	CItemProto* proto = gserver.m_itemProtoList.FindIndex(nItemIndex);
	if (!proto)
	{
		GAMELOG << init("EVENT 2PAN4PAN MONEY REQ", ch)
				<< "NOT FOUND ITEM INDEX" << delim
				<< nItemIndex
				<< end;
		Event2pan4panMoneyRepMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 프로토에 따라 인벤을 결정
	CInventory* inven = GET_INVENTORY(ch, GET_TAB(proto->m_typeIdx, proto->m_subtypeIdx));
	if (!inven)
	{
		GAMELOG << init("EVENT 2PAN4PAN MONEY REQ", ch)
				<< "NOT FOUND ITEM INDEX" << delim
				<< nItemIndex
				<< end;
		Event2pan4panMoneyRepMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 인벤에서 아이템 찾기
	int r, c;
	CItem* item = NULL;
	if (inven->FindItem(&r, &c, nItemIndex, 0, 0))
		item = inven->GetItem(r, c);
	if (!item)
	{
		switch (type)
		{
		case 1:		nItemIndex = 1055;		break;		// 맞고
		case 2:		nItemIndex = 1054;		break;		// 포커
		default:
			Event2pan4panMoneyRepMsg(rmsg, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 프로토를 찾아서
		proto = gserver.m_itemProtoList.FindIndex(nItemIndex);
		if (!proto)
		{
			GAMELOG << init("EVENT 2PAN4PAN MONEY REQ", ch)
					<< "NOT FOUND ITEM INDEX" << delim
					<< nItemIndex
					<< end;
			Event2pan4panMoneyRepMsg(rmsg, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 프로토에 따라 인벤을 결정
		inven = GET_INVENTORY(ch, GET_TAB(proto->m_typeIdx, proto->m_subtypeIdx));
		if (!inven)
		{
			GAMELOG << init("EVENT 2PAN4PAN MONEY REQ", ch)
					<< "NOT FOUND ITEM INDEX" << delim
					<< nItemIndex
					<< end;
			Event2pan4panMoneyRepMsg(rmsg, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 인벤에서 아이템 찾기
		if (!inven->FindItem(&r, &c, nItemIndex, 0, 0))
		{
			Event2pan4panMoneyRepMsg(rmsg, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		item = inven->GetItem(r, c);
		if (!item)
		{
			Event2pan4panMoneyRepMsg(rmsg, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	if (IS_RUNNING_CONN)
	{
		if (item->Count() > 1)
		{
			DecreaseFromInventory(ch, item, 1);
			ItemUpdateMsg(rmsg, item, -1);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, item);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, item, true, true);
		}

		ConnEvent2pan4panMoneyReqMsg(rmsg, ch->m_desc->m_index, ch->m_index, nItemIndex);
		SEND_Q(rmsg, gserver.m_connector);

		GAMELOG << init("EVENT 2PAN4PAN MONEY REQ", ch)
				<< "TYPE" << delim
				<< type << delim
				<< "ITEM" << delim
				<< nItemIndex
				<< end;
	}
}
#endif // #ifdef EVENT_2PAN4PAN_MONEY

#ifdef EVENT_XMAS_2005
void do_EventXMas2005(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	// 교환 테이블
	const int needItemTableSize = 7;
	int needItemCount[needItemTableSize][2] = {
		{ 4,  1},			// 꿈4 희망1
		{ 8,  2},			// 꿈8 희망2
		{16,  4},			// 꿈16 희망4
		{24,  6},			// 꿈24 희망6
		{40, 10},			// 꿈40 희망10
		{56, 14},			// 꿈56 희망14
		{80, 20}			// 꿈80 희망20
	};

	const int goodsTableSize = 11;

#ifdef LC_TLD		// 태국 보상 반값
		int goodsIndex[goodsTableSize][4]		= {
		{0,  44,  0, 5},		// 보상 0,	중형HP물약,		 0레벨, 5 개
		{0, 485,  0, 5},		// 보상 0,	중형MP물약,		 0레벨, 5 개
		{1,  45,  0, 5},		// 보상 1,	대형HP물약,		 0레벨, 5 개
		{1, 724,  0, 5},		// 보상 1,	대형MP물약,		 0레벨, 5 개
		{2, 499, 12, 5},		// 보상 2,	붉은넨,			12레벨, 5 개
		{2, 501, 12, 5},		// 보상 2,	푸른넨,			12레벨, 5 개
		{3, 498, 12, 5},		// 보상 3,	붉은케르,		12레벨, 5 개
		{3, 500, 12, 5},		// 보상 3,	푸른케르,		12레벨, 5 개
		{4, 972,  0, 2},		// 보상 4,	행운의스크롤,	 0레벨, 2 개
		{5, 882,  0, 2},		// 보상 5,	경험치증폭제,	 0레벨, 2 개
		{6,  85,  0, 1}			// 보상 6,	고급제련석,		 0레벨, 1 개
	};
#else
	int goodsIndex[goodsTableSize][4]		= {
		{0,  44,  0, 10},		// 보상 0,	중형HP물약,		 0레벨, 10 개
		{0, 485,  0, 10},		// 보상 0,	중형MP물약,		 0레벨, 10 개
		{1,  45,  0, 10},		// 보상 1,	대형HP물약,		 0레벨, 10 개
		{1, 724,  0, 10},		// 보상 1,	대형MP물약,		 0레벨, 10 개
		{2, 499, 12, 10},		// 보상 2,	붉은넨,			12레벨, 10 개
		{2, 501, 12, 10},		// 보상 2,	푸른넨,			12레벨, 10 개
		{3, 498, 12, 10},		// 보상 3,	붉은케르,		12레벨, 10 개
		{3, 500, 12, 10},		// 보상 3,	푸른케르,		12레벨, 10 개
		{4, 972,  0,  5},		// 보상 4,	행운의스크롤,	 0레벨,  5 개
		{5, 882,  0,  5},		// 보상 5,	경험치증폭제,	 0레벨,  5 개
		{6,  85,  0,  2}		// 보상 6,	고급제련석,		 0레벨,  2 개
	};
#endif

	try
	{
		// 꿈/희망 아이템 프로토 얻음
		CItemProto* pItemProto1 = gserver.m_itemProtoList.FindIndex(1134);		// 꿈
		CItemProto* pItemProto2 = gserver.m_itemProtoList.FindIndex(1135);		// 희망
		if (pItemProto1 == NULL || pItemProto2 == NULL)
			throw 1;

		// 인벤토리 결정
		CInventory* inven1 = GET_INVENTORY(ch, GET_TAB(pItemProto1->m_typeIdx, pItemProto1->m_subtypeIdx));
		CInventory* inven2 = GET_INVENTORY(ch, GET_TAB(pItemProto2->m_typeIdx, pItemProto2->m_subtypeIdx));
		if (inven1 == NULL || inven2 == NULL)
			throw 1;

		// 꿈/희망 아이템 포인트 얻기
		int r, c;
		if (!inven1->FindItem(&r, &c, pItemProto1->m_index, 0, 0))
			throw 1;
		CItem* pItem1 = inven2->GetItem(r, c);
		if (!inven2->FindItem(&r, &c, pItemProto2->m_index, 0, 0))
			throw 1;
		CItem* pItem2 = inven2->GetItem(r, c);
		if (pItem1 == NULL || pItem2 == NULL)
			throw 1;

		int goodsType;
		for (goodsType = needItemTableSize - 1; goodsType >= 0; goodsType--)
		{
			if (pItem1->Count() >= needItemCount[goodsType][0]
				&& pItem2->Count() >= needItemCount[goodsType][1])
				break;
		}
		if (goodsType < 0)
			throw 1;

		// 아이템 소비
		if (pItem1->Count() > needItemCount[goodsType][0])
		{
			DecreaseFromInventory(ch, pItem1, needItemCount[goodsType][0]);
			ItemUpdateMsg(rmsg, pItem1, -needItemCount[goodsType][0]);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pItem1);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItem1, true, true);
		}
		if (pItem2->Count() > needItemCount[goodsType][1])
		{
			DecreaseFromInventory(ch, pItem2, needItemCount[goodsType][1]);
			ItemUpdateMsg(rmsg, pItem2, -needItemCount[goodsType][1]);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pItem2);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItem2, true, true);
		}

		// 아이템 지급
		int i;
		for (i = 0; i < goodsTableSize; i++)
		{
			if (goodsIndex[i][0] == goodsType)
			{
				CItem* pItemGood = gserver.m_itemProtoList.CreateItem(goodsIndex[i][1], -1, 0, goodsIndex[i][2], goodsIndex[i][3]);
				if (pItemGood == NULL)
				{
					GAMELOG << init("EVENT XMAS 2005 ERROR", ch)
							<< "ITEM NOT FOUND" << delim
							<< goodsIndex[i][1]
							<< end;
					continue ;
				}

				GAMELOG << init("EVENT XMAS 2005", ch)
						<< "TYPE" << delim
						<< goodsType << delim
						<< "ITEM" << delim
						<< itemlog(pItemGood) << delim;

				if (AddToInventory(ch, pItemGood, true, true))
				{
					if (pItemGood->tab() < 0)
					{
						// 겹침
						CInventory* pInvenGoods = GET_INVENTORY(ch, GET_TAB(pItemGood->m_itemProto->m_typeIdx, pItemGood->m_itemProto->m_subtypeIdx));
						pInvenGoods->FindItem(&r, &c, pItemGood->m_idNum, pItemGood->m_plus, pItemGood->m_flag);
						CItem* pItemCurGood = pInvenGoods->GetItem(r, c);
						ItemUpdateMsg(rmsg, pItemCurGood , pItemGood->Count());
						SEND_Q(rmsg, ch->m_desc);

						delete pItemGood;
						pItemGood = NULL;
					}
					else
					{
						// 추가
						ItemAddMsg(rmsg, pItemGood);
						SEND_Q(rmsg, ch->m_desc);

						pItemGood = NULL;
					}

					GAMELOG << "ADD TO INVEN" << end;
				}
				else
				{
					// 아이템 드롭
					ch->m_pArea->DropItem(pItemGood, ch);
					pItemGood->m_preferenceIndex = ch->m_index;
					ItemDropMsg(rmsg, ch, pItemGood);
					pItemGood->m_pArea->SendToCell(rmsg, GET_YLAYER(pItemGood), pItemGood->m_cellX, pItemGood->m_cellZ);

					pItemGood = NULL;

					GAMELOG << "DROP" << end;
				}
			}
		}

		EventXMas2005Msg(rmsg, MSG_EVENT_XMAS_2005_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
	catch (...)
	{
		EventXMas2005Msg(rmsg, MSG_EVENT_XMAS_2005_ERROR_NO_ITEM);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}
#endif // #ifdef EVENT_XMAS_2005

#ifdef EVENT_NEWYEAR_2006
void do_EventNewYear2006Check(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	EventNewYear2006CheckMsg(rmsg, ch->m_nTimeEventNewYear2006);
	SEND_Q(rmsg, ch->m_desc);
}

void do_EventNewYear2006Goods(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int nHour = ch->m_nTimeEventNewYear2006 / 60 / 60;

#ifdef EVENT_NEWYEAR_2006_GOODS

	// 시간 누적 중이면 안됨
	if (gserver.m_bNewYear2006Event)
	{
		EventNewYear2006GoodsMsg(rmsg, MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_END);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 이미 받았으면 안됨
	if (ch->m_etcEvent & ETC_EVENT_NEWYEAR_2006)
	{
		EventNewYear2006GoodsMsg(rmsg, MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 시간이 최소 10시간 이상이어야 함
	if (nHour < 10)
	{
		EventNewYear2006GoodsMsg(rmsg, MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_TIME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 시간에 따른 보상
	int nGoodsIndex = -1;
	LONGLONG nGoodsCount = 0;

#ifndef LC_TLD
	if (nHour >= 30)
	{
		// 30시간 이상 고제 1개
		nGoodsIndex = 85;
		nGoodsCount = 1;
	}
	else if (nHour >= 20)
	{
		// 20시간 이상 200,000 나스
		nGoodsIndex = 19;
		nGoodsCount = 200000;
	}
	else
	{
		// 그외 100,000 나스
		nGoodsIndex = 19;
		nGoodsCount = 100000;
	}
#else
	// 태국은 20만, 고제1, 20만+고제1
	if (nHour >= 20)
	{
		nGoodsIndex = 85;
		nGoodsCount = 1;
		
	}
	else
	{
		nGoodsIndex = 19;
		nGoodsCount = 200000;
	}
#endif

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nGoodsIndex, -1, 0, 0, nGoodsCount);
	if (!pItem)
	{
		GAMELOG << init("EVENT NEW YEAR 2006 GOODS ERROR NOTFOUND ITEM", ch->m_name, ch->m_desc->m_idname)
				<< "TIME" << delim
				<< ch->m_nTimeEventNewYear2006
				<< "(" << nHour << ")"
				<< end;
		return ;
	}

	GAMELOG << init("EVENT NEW YEAR 2006 GOODS", ch->m_name, ch->m_desc->m_idname)
			<< "TIME" << delim
			<< ch->m_nTimeEventNewYear2006
			<< "(" << nHour << ")" << delim
			<< "ITEM" << delim
			<< itemlog(pItem) << delim;


	if (AddToInventory(ch, pItem, true, true))
	{
		if (pItem->tab() < 0)
		{
			// 겹침
			CInventory* pInvenGoods = GET_INVENTORY(ch, GET_TAB(pItem->m_itemProto->m_typeIdx, pItem->m_itemProto->m_subtypeIdx));
			int r, c;
			pInvenGoods->FindItem(&r, &c, pItem->m_idNum, pItem->m_plus, pItem->m_flag);
			CItem* pItemCurGood = pInvenGoods->GetItem(r, c);
			ItemUpdateMsg(rmsg, pItemCurGood , pItem->Count());
			SEND_Q(rmsg, ch->m_desc);

			delete pItem;
			pItem = NULL;
		}
		else
		{
			// 추가
			ItemAddMsg(rmsg, pItem);
			SEND_Q(rmsg, ch->m_desc);

			pItem = NULL;
		}

		GAMELOG << "ADD TO INVEN" << end;
	}
	else
	{
		// 아이템 드롭
		ch->m_pArea->DropItem(pItem, ch);
		pItem->m_preferenceIndex = ch->m_index;
		ItemDropMsg(rmsg, ch, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

		pItem = NULL;

		GAMELOG << "DROP" << end;
	}

#ifdef LC_TLD
	if( nHour >= 30 )
	{
		nGoodsIndex = 19;
		nGoodsCount = 200000;

		CItem* pItem = gserver.m_itemProtoList.CreateItem(nGoodsIndex, -1, 0, 0, nGoodsCount);
		if (!pItem)
		{
			GAMELOG << init("EVENT NEW YEAR 2006 GOODS ERROR NOTFOUND ITEM", ch->m_name, ch->m_desc->m_idname)
					<< "TIME" << delim
					<< ch->m_nTimeEventNewYear2006
					<< "(" << nHour << ")"
					<< end;
			return ;
		}

		GAMELOG << init("EVENT NEW YEAR 2006 GOODS", ch->m_name, ch->m_desc->m_idname)
				<< "TIME" << delim
				<< ch->m_nTimeEventNewYear2006
				<< "(" << nHour << ")" << delim
				<< "ITEM" << delim
				<< itemlog(pItem) << delim;


		if (AddToInventory(ch, pItem, true, true))
		{
			if (pItem->tab() < 0)
			{
				// 겹침
				CInventory* pInvenGoods = GET_INVENTORY(ch, GET_TAB(pItem->m_itemProto->m_typeIdx, pItem->m_itemProto->m_subtypeIdx));
				int r, c;
				pInvenGoods->FindItem(&r, &c, pItem->m_idNum, pItem->m_plus, pItem->m_flag);
				CItem* pItemCurGood = pInvenGoods->GetItem(r, c);
				ItemUpdateMsg(rmsg, pItemCurGood , pItem->Count());
				SEND_Q(rmsg, ch->m_desc);

				delete pItem;
				pItem = NULL;
			}
			else
			{
				// 추가
				ItemAddMsg(rmsg, pItem);
				SEND_Q(rmsg, ch->m_desc);

				pItem = NULL;
			}

			GAMELOG << "ADD TO INVEN" << end;
		}
		else
		{
			// 아이템 드롭
			ch->m_pArea->DropItem(pItem, ch);
			pItem->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, pItem);
			pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

			pItem = NULL;

			GAMELOG << "DROP" << end;
		}

	}
#endif

#ifdef LC_TLD
	if( nHour >= 30 )
		ch->m_nTimeEventNewYear2006  -=  30 * 60 * 60;
	else if( nHour >= 20 )
		ch->m_nTimeEventNewYear2006  -=  20 * 60 * 60;
	else if( nHour >= 10 )
		ch->m_nTimeEventNewYear2006  -=  10 * 60 * 60;
#else
	ch->m_etcEvent |= ETC_EVENT_NEWYEAR_2006;
#endif

	EventNewYear2006GoodsMsg(rmsg, MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_OK);
	SEND_Q(rmsg, ch->m_desc);
#else // #ifdef EVENT_NEWYEAR_2006_GOODS

	EventNewYear2006GoodsMsg(rmsg, MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_END);
	SEND_Q(rmsg, ch->m_desc);
	return ;

#endif // #ifdef EVENT_NEWYEAR_2006_GOODS

}
#endif // #ifdef EVENT_NEWYEAR_2006

#ifdef EVENT_SEARCHFRIEND
void do_EventSearchFriendAdd(CPC* ch, CNetMsg& msg)
{
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	msg >> name;

	CNetMsg rmsg;

	if( strcmp(ch->m_name, name) == 0 || strcmp(ch->m_nick, name) == 0)
	{
		GAMELOG << init("EVENT SEARCH FRIEND ERROR USER NAME", ch)
				<< name<< delim
				<< end;
		EventSearchFriendErrorMsg(rmsg, MSG_EVENT_SEARCHFRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 이름 검사
	int lenname = strlen(name);
	if (lenname < 2 || lenname > 16)
	{
		GAMELOG << init("EVENT SEARCH FRIEND ERROR NAME", ch)
				<< name<< delim
				<< end;
		EventSearchFriendErrorMsg(rmsg, MSG_EVENT_SEARCHFRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (strinc(name, "'") || strinc(name, " ") )
	{
		GAMELOG << init("EVENT SEARCH FRIEND ERROR NAME", ch)
				<< name<< delim
				<< end;
		EventSearchFriendErrorMsg(rmsg, MSG_EVENT_SEARCHFRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(IS_RUNNING_HELPER)
	{
		//Helper 커맨드로 보내는 부분
		GAMELOG << init("SEARCHFRIEND_ADDREQ_OK helper", ch)
				<< name << end;
		EventHelperSearchFriendAddReqMsg(rmsg
						, ch->m_index, ch->m_nick, ch->m_job
						, ch->m_desc->m_index, ch->m_serverOld, g_buf);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		GAMELOG << init("SEARCHFRIEND_ADDREQ_FAIL-not helper", ch)
				<< name << end;
		return;
	}
}

//등록한 리스트중 선택하는 부분
void do_EventSearchFriendAddSelect(CPC* ch, CNetMsg& msg)
{
	int reqindex;
	msg >> reqindex;

	if (reqindex < 0)
	{
		GAMELOG << init("SEARCHFRIEND_SelectAddReq_FAIL-not helper", ch)
				<< reqindex	<< end;
		return;
	}

	CNetMsg rmsg;
	if(IS_RUNNING_HELPER)
	{
		//Helper 커맨드로 보내는 부분
		EventHelperSearchFriendSelectAddReqMsg(rmsg, ch->m_index, reqindex);
		SEND_Q(rmsg, gserver.m_helper);

		GAMELOG << init("SEARCHFRIEND_SelectAddReq_OK helper", ch)
				<< reqindex	<< end;
	}
	else
	{
		GAMELOG << init("SEARCHFRIEND_SelectAddReq_FAIL-not helper", ch)
				<< reqindex	<< end;
		return;
	}
}

//휴면 케릭 사냥시간 체크
void do_EventSearchFriendTimeCheck(CPC* ch, CNetMsg& msg) 
{
	CNetMsg rmsg;
	EventSearchFriendTimeCheckMsg(rmsg, ch->m_nTimeEventSearchFriend);
	SEND_Q(rmsg, ch->m_desc);
}

// 등록된 친구 보상가능여부
void do_EventSearchFriendListGoods(CPC* ch, CNetMsg& msg)
{	
	CNetMsg rmsg;
	if(ch->m_bGoodEventSearchFriendListImprove ==  true)
	{
		//Helper 커맨드로 보내는 부분
		EventHelperSearchFriendListGoodReqMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else if(ch->m_bGoodEventSearchFriendListImprove ==  false)
	{
		if (ch->m_nGoodEventSearchFriendListGoodCount <= 20)
		{
			EventSearchFriendListGoodsMsg(rmsg, ch->m_nGoodEventSearchFriendListGoodCount, ch->m_nGoodEventSearchFriendIndex
				, ch->m_nGoodEventSearchFriendNick, ch->m_nGoodEventSearchFriendGood, 0, ch->m_nGoodEventSearchFriendListGoodCount);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			EventSearchFriendListGoodsMsg(rmsg, 20, ch->m_nGoodEventSearchFriendIndex
				, ch->m_nGoodEventSearchFriendNick, ch->m_nGoodEventSearchFriendGood, 0, ch->m_nGoodEventSearchFriendListGoodCount);
			SEND_Q(rmsg, ch->m_desc);
			if (ch->m_nGoodEventSearchFriendListGoodCount <= 40)
			{
				EventSearchFriendListGoodsMsg(rmsg, ch->m_nGoodEventSearchFriendListGoodCount - 20, ch->m_nGoodEventSearchFriendIndex + 20
					, ch->m_nGoodEventSearchFriendNick + 20, ch->m_nGoodEventSearchFriendGood + 20, 20, ch->m_nGoodEventSearchFriendListGoodCount);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				EventSearchFriendListGoodsMsg(rmsg, 20, ch->m_nGoodEventSearchFriendIndex + 20
					, ch->m_nGoodEventSearchFriendNick + 20, ch->m_nGoodEventSearchFriendGood + 20, 20, ch->m_nGoodEventSearchFriendListGoodCount);
				SEND_Q(rmsg, ch->m_desc);
				EventSearchFriendListGoodsMsg(rmsg, ch->m_nGoodEventSearchFriendListGoodCount - 40, ch->m_nGoodEventSearchFriendIndex + 40
					, ch->m_nGoodEventSearchFriendNick + 40, ch->m_nGoodEventSearchFriendGood + 40, 40, ch->m_nGoodEventSearchFriendListGoodCount);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
}

//자신 보상 부분.
void do_EventSearchFriendGoods(CPC* ch, CNetMsg& msg)
{
	int nlistNumber;
	msg >> nlistNumber;

	CNetMsg rmsg;
	int nCharGood = 0; //현재 보상받을수 있는 개수 

#ifdef EVENT_SEARCHFRIEND_GOODS

	if(( nlistNumber >= 50)||( nlistNumber < 0))
	{
		GAMELOG << init("EVENT SEARCH FRIEND GOODS ERROR ", ch)
				<< "LISTNUMBER " << delim
				<< nlistNumber
				<< end;
		return;
	}	
	
	nCharGood = ch->m_nGoodEventSearchFriendGood[nlistNumber];		
    ch->m_nGoodEventSearchFriendGoodNumber = nlistNumber;

	int nGoodsIndex = -1;
	LONGLONG nGoodsCount = 0;

	// 이미 받았으면 안됨
	if (nCharGood == 3)
	{
		EventSearchFriendGoodsMsg(rmsg, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_NOGOODS);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("EVENT SEARCH FRIEND GOODS ERROR NOGOODS", ch)<< end;
		return ;
	}
	else if (nCharGood >= 0)
	{
		// 30시간 이상 고제 1개
		nGoodsIndex = 85;
		nGoodsCount = nCharGood;
	}
	else if(nCharGood == -1)
		nGoodsIndex = 85;		

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nGoodsIndex, -1, 0, 0, nGoodsCount);
	if (!pItem)
	{
		EventSearchFriendGoodsMsg(rmsg, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TIME);
		SEND_Q(rmsg, ch->m_desc);
		
		GAMELOG << init("EVENT SEARCH FRIEND GOODS ERROR NOTFOUND ITEM", ch)
				<< "ITME " << delim
				<< nGoodsCount
				<< end;
		return ;
	}

	GAMELOG << init("EVENT SEARCH FRIEND GOODS", ch)
			<< nGoodsCount << delim
			<< "ITEM " << delim
			<< itemlog(pItem) << delim;

	if (AddToInventory(ch, pItem, true, true))
	{
		if (pItem->tab() < 0)
		{
			// 겹침
			CInventory* pInvenGoods = GET_INVENTORY(ch, GET_TAB(pItem->m_itemProto->m_typeIdx, pItem->m_itemProto->m_subtypeIdx));
			int r, c;
			pInvenGoods->FindItem(&r, &c, pItem->m_idNum, pItem->m_plus, pItem->m_flag);
			CItem* pItemCurGood = pInvenGoods->GetItem(r, c);
			ItemUpdateMsg(rmsg, pItemCurGood , pItem->Count());
			SEND_Q(rmsg, ch->m_desc);

			delete pItem;
			pItem = NULL;
		}
		else
		{
			// 추가
			ItemAddMsg(rmsg, pItem);	
			SEND_Q(rmsg, ch->m_desc);

			pItem = NULL;
		}
		GAMELOG << "ADD TO INVEN" << end;
	}
	else
	{
		// 아이템 드롭
		ch->m_pArea->DropItem(pItem, ch);
		pItem->m_preferenceIndex = ch->m_index;
		ItemDropMsg(rmsg, ch, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

		pItem = NULL;

		GAMELOG << "DROP" << end;
	}
	//Helper 커맨드로 보내는 부분
	EventHelperSearchFriendGoodReqMsg(rmsg, ch->m_index, ch->m_nGoodEventSearchFriendIndex[nlistNumber], nGoodsCount);
	SEND_Q(rmsg, gserver.m_helper);
#else // #ifdef EVENT_SEARCHFRIEND_GOODS
	EventSearchFriendGoodsMsg(rmsg, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_END);
	SEND_Q(rmsg, ch->m_desc);
	return ;
#endif // #ifdef EVENT_SEARCHFRIEND_GOODS
}
#endif // #ifdef EVENT_SEARCHFRIEND

#ifdef NEW_SERIAL_PACKAGE_EVENT
void do_EventPackageItemGive_new( CPC* ch, CNetMsg& msg )
{
	CLCString couponNo(20);
	couponNo = "";
	msg >> couponNo;

	if( couponNo.Length() > 0 )
	{
		if( IS_RUNNING_CONN )
		{
			CNetMsg msg;
			ConnCouponConfirm_new( msg, ch->m_index, couponNo );
			//ConnCouponConfirm(msg, ch->m_index, couponNo);
			SEND_Q( msg, gserver.m_connector );
		}
	}
}
#endif // NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_PACKAGE_ITEM
void do_EventPackageItemGive(CPC* ch, CNetMsg& msg)
{
	CLCString couponNo(21);

	couponNo = "";

	msg >> couponNo;

	if( couponNo.Length() > 0 )
	{
		if( IS_RUNNING_CONN)
		{
			CNetMsg msg;
			ConnCouponConfirm(msg, ch->m_index, couponNo);

			SEND_Q(msg, gserver.m_connector);
		}
	}
}
#endif

#ifdef EVENT_SAKURA
void do_EventSakrabudGive(CPC* ch, CNetMsg& msg)
{
	char type;
	int budNum = 0, i;

	static int costume[5][3][2] = {
		{ {1229, 1230}, { 1231, 1232 } , { 1228, -1 } },
		{ {1234, 1235}, { 1236, 1237 } , { 1233, -1 } },
		{ {1240, 1241}, { 1242, 1243 } , { 1239, -1 } },
		{ {1245, 1246}, { 1247, 1248 } , { 1244, -1 } },
		{ {1250, 1251}, { 1252, 1253 } , { 1249, -1 } }
	};

	msg >> type;

	if( type < 0 || type > 3 || ch->m_job >= JOB_SORCERER)
		return;

	CItem* bud = NULL;
	int r, c;
	CNetMsg rmsg;
	if( !ch->m_invenEvent.FindItem(&r, &c, 1312, 0, 0) || !(bud = ch->m_invenEvent.GetItem(r, c) ))
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_SAKURABUD_LEAKITEM );
		rmsg << type;

		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	switch( type )
	{
	case 0:
		budNum = 18;
		break;
	case 1:
		budNum = 12;
		break;
	case 2:
		budNum = 7;
		break;
	default:
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_SAKURABUD_LEAKITEM );
		rmsg << type;

		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( bud->Count() < budNum )
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_SAKURABUD_LEAKITEM );
		rmsg << type;

		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	DecreaseFromInventory(ch, bud, budNum);
	if (bud->Count() > 0)
	{
		ItemUpdateMsg(rmsg, bud, -budNum);
	}
	else
	{
		// 없어질 때
		ItemDeleteMsg(rmsg, bud);
		
		// 인벤에서 제거
		RemoveFromInventory(ch, INVENTORY_EVENT, r, c, true, true);
	}
	SEND_Q(rmsg, ch->m_desc);

	for( i = 0; i < 2; i++ )
	{
		if( costume[(int)ch->m_job][(int)type][i] == -1 )
			continue;

		if(!ch->GiveItem(costume[(int)ch->m_job][(int)type][i], 0, 0, 1, false ) )
		{
			//롤백
			int r, c;
			CItem* cost = NULL;
			for(i--;i >= 0; i--)
			{
				ch->m_invenNormal.FindItem(&r, &c, costume[(int)ch->m_job][(int)type][i], 0, 0);
				cost = ch->m_invenNormal.GetItem(r, c);

				if( cost == NULL )
					continue;

				DecreaseFromInventory(ch, cost, 1);
				if (cost->Count() > 0)
				{
					ItemUpdateMsg(rmsg, cost, -1);
				}
				else
				{
					// 없어질 때
					ItemDeleteMsg(rmsg, cost);
					// 인벤에서 제거
					RemoveFromInventory(ch, INVENTORY_NORMAL, r, c, true, true);
				}
				SEND_Q(rmsg, ch->m_desc);
			}

			ch->GiveItem(1312, 0, 0, budNum);

			EventErrorMsg(rmsg, MSG_EVENT_ERROR_FULLINVENTORY);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("SAKURABUD_EXCHANGE", ch->m_name, ch->m_desc->m_idname)
					<< type << delim
					<< "ROLLBACK" << end ;
			return;
		}
	}

	// 지급 성공
	EventErrorMsg(rmsg, MSG_EVENT_ERROR_SAKURABUD_SUCCESS);
	rmsg << type;

	SEND_Q(rmsg, ch->m_desc);

	GAMELOG << init("SAKURABUD_EXCHANGE", ch->m_name, ch->m_desc->m_idname)
			<< type << delim
			<< "SUCCESS" << end;
}
#endif
#ifdef EVENT_PCBANG
void do_EventPCbang(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	int r, c;
	if( !ch->m_invenEvent.FindItem(&r, &c, 1394, 0, 0 ) )
	{
		if( !ch->m_invenEvent.FindItem(&r, &c, 1395, 0, 0 ) )
		{
			if( !ch->m_invenEvent.FindItem(&r, &c, 1396, 0, 0 ) )
			{
				if( !ch->m_invenEvent.FindItem(&r, &c, 1397, 0, 0 ) )
				{
					EventPCbangRepMsg(rmsg, -1);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}
			}
		}
	}

	CItem* item = ch->m_invenEvent.GetItem(r, c);

	if( !item )
	{
		EventPCbangRepMsg(rmsg, -1);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	static const int giveItemTable[4][2][3] = {
		{ {391,	  43,	46} , {10, 10, 10 } },
		{ {485,   47,  284} , { 7,  7,  7 } },
		{ {724,   45,  723} , { 7,  5,  5 } },
		{ {556, 1307, 1313} , { 5,  5, 3  } } };

	bool bGive = false;
	int rand = GetRandom(0, 2);
	switch(item->m_itemProto->m_index)
	{
	case 1394:
		if( ch->GiveItem(giveItemTable[0][0][rand], 0, 0,giveItemTable[0][1][rand]) )
			bGive = true;
		break;
	case 1395:
		if( ch->GiveItem(giveItemTable[1][0][rand], 0, 0,giveItemTable[1][1][rand]) )
			bGive = true;
		break;
	case 1396:
		if( ch->GiveItem(giveItemTable[2][0][rand], 0, 0,giveItemTable[2][1][rand]) )
			bGive = true;
		break;
	case 1397:
		if( ch->GiveItem(giveItemTable[3][0][rand], 0, 0,giveItemTable[3][1][rand]) )
			bGive = true;
		break;
	default:
		return;
	}

	if( !bGive ) 
	{
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_FULLINVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	EventPCbangRepMsg(rmsg, giveItemTable[0][0][rand]);
	SEND_Q(rmsg, ch->m_desc);
	GAMELOG << init("PCBANG_ITEM_EXCHANGE_SUCCESS", ch)
			<< itemlog(item) << delim
			<< giveItemTable[0][0][rand] << delim
			<< giveItemTable[0][1][rand] << end;

	DecreaseFromInventory(ch, item, 1);
	if (item->Count() == 0)
	{
		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, item, true, true);
	}
	else
	{
		ItemUpdateMsg(rmsg, item, -1);
		SEND_Q(rmsg, ch->m_desc);
	}
}
#endif // EVENT_PCBANG

void do_EventWorldcup(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	// FIFA 컵 아이템 인덱스
	static const int nFIFACupIndex = 1484;

	// 국가 테이블 : 국가 번호, FIFA컵 개수, 국기 Index, 순위(1, 2, 3, 4)
#define TABLE_REF_COUNTRY		0
#define TABLE_REF_FIFACUP		1
#define TABLE_REF_FLAG			2
#define TABLE_REF_RANK			3
	static const int nCountOfCountry = 32;
	static const int tableCountry[nCountOfCountry][4] = {
		{ 0,	100,	1485,		4},		// 브라질
		{ 1,	100,	1486,		3},		// 독일
		{ 2,	100,	1487,		1},		// 이탈리아
		{ 3,	100,	1488,		4},		// 영국
		{ 4,	100,	1489,		4},		// 아르헨티나
		{ 5,	 75,	1490,		2},		// 프랑스
		{ 6,	 75,	1491,		4},		// 네덜란드
		{ 7,	 75,	1492,		4},		// 스페인
		{ 8,	 75,	1493,		4},		// 포르투갈
		{ 9,	 75,	1494,		4},		// 체코
		{10,	 75,	1495,		4},		// 스웨덴
		{11,	 75,	1496,		4},		// 멕시코
		{12,	 75,	1497,		4},		// 우크라이나
		{13,	 75,	1498,		4},		// 크로아티아
		{14,	 50,	1499,		4},		// 미국
		{15,	 50,	1500,		4},		// 스위스
		{16,	 50,	1501,		4},		// 코트디부아르
		{17,	 50,	1502,		4},		// 세르비아-몬테네그로
		{18,	 50,	1503,		4},		// 폴란드
		{19,	 50,	1504,		4},		// 파라과이
		{20,	 50,	1505,		4},		// 대한민국
		{21,	 50,	1506,		4},		// 호주
		{22,	 50,	1507,		4},		// 일본
		{23,	 30,	1508,		4},		// 가나
		{24,	 30,	1509,		4},		// 튀니지
		{25,	 30,	1510,		4},		// 에콰도르
		{26,	 30,	1511,		4},		// 토고
		{27,	 30,	1512,		4},		// 앙골라
		{28,	 30,	1513,		4},		// 이란
		{29,	 30,	1514,		4},		// 사우디아라비아
		{30,	 30,	1515,		4},		// 코스타리카
		{31,	 30,	1516,		4},		// 트리니다드 토바고
	};

	// 보상 테이블 : 보상 아이템 번호, 보상 아이템 수량
	static const int tableGift[4][4] = {
		{ 85,	5},			// 1 위		고제 5개
		{ 85,	2},			// 2 위		고제 2개
		{ 19,	300000},	// 3 위		나스 300,000
		{723,	5},			// 그외		문스톤 5개
	};

	int nSubType = -1;
	msg >> nSubType;

	// 공용 변수
	int nCountryIndex = -1;

	// VOTE용
	int nCountry = 0;
	CItem* pItemCountry = NULL;
	CItem* pItemFIFACup = NULL;
	int nRowCountryItem, nColCountryItem;

	// GIFT용
	int nRank = 0;
	int nRowFlagItem, nColFlagItem;
	CLCList<int> listFlagItemIndex(NULL);
	int nGiftItemIndex = -1;
	LONGLONG nGiftItemCount = 0;
	CItem* pItemFlag = NULL;
	CItem* pItemGift = NULL;

	switch (nSubType)
	{

	// 응모 처리
	case MSG_EVENT_WORLDCUP_VOTE:

		msg >> nCountry;

		// 1. 응모 기간 검사
		// 2. 테이블에서 해당 국가 찾기
		// 3. 해당 국가 응모 검사 : 절대 같은 국기를 소유해선 안됨
		// 4. FIFA 컵 수량 검사
		// 5. 국기 생성
		// 6. 국기 지급
		// 7. FIFA 컵 회수
		// 8. 결과 알림
		// 9. 게임 로그

		// 1. 응모 기간 검사
#ifdef EVENT_WORLDCUP_2006_VOTE
#else
		EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_NOTCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
#endif // EVENT_WORLDCUP_2006_VOTE

		// 2. 테이블에서 해당 국가 찾기
		for (nCountryIndex = 0; nCountryIndex < nCountOfCountry; nCountryIndex++)
		{
			if (tableCountry[nCountryIndex][TABLE_REF_COUNTRY] == nCountry)
				break;
		}
		if (nCountryIndex == nCountOfCountry)
			return ;

		// 3. 해당 국가 응모 검사 : 절대 같은 국기를 소유해선 안됨
		if (ch->m_invenEvent.FindItem(&nRowCountryItem, &nColCountryItem, tableCountry[nCountryIndex][TABLE_REF_FLAG], 0, 0))
		{
			EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_ALREADY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 4. FIFA 컵 수량 검사
		if (!ch->m_invenNormal.FindItem(&nRowCountryItem, &nColCountryItem, nFIFACupIndex, 0, 0))
		{
			EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItemFIFACup = ch->m_invenNormal.GetItem(nRowCountryItem, nColCountryItem);
		if (!pItemFIFACup || pItemFIFACup->Count() < tableCountry[nCountryIndex][TABLE_REF_FIFACUP])
		{
			EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 5. 국기 생성
		pItemCountry = gserver.m_itemProtoList.CreateItem(tableCountry[nCountryIndex][TABLE_REF_FLAG], -1, 0, 0, 1);
		if (!pItemCountry)
		{
			EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 6. 국기 지급
		if (AddToInventory(ch, pItemCountry, true, true))
		{
			// 절대 같은 국기가 있을 수 없다.
			ItemAddMsg(rmsg, pItemCountry);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			delete pItemCountry;
			pItemCountry = NULL;
			EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_FULLINVEN);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 7. FIFA 컵 회수
		if (pItemFIFACup->Count() > tableCountry[nCountryIndex][TABLE_REF_FIFACUP])
		{
			DecreaseFromInventory(ch, pItemFIFACup, tableCountry[nCountryIndex][TABLE_REF_FIFACUP]);
			ItemUpdateMsg(rmsg, pItemFIFACup, -tableCountry[nCountryIndex][TABLE_REF_FIFACUP]);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pItemFIFACup);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItemFIFACup, true, true);
			pItemFIFACup = NULL;
		}

		// 8. 결과 알림
		EventWorldcupVoteMsg(rmsg, tableCountry[nCountryIndex][TABLE_REF_COUNTRY], MSG_EVENT_WORLDCUP_ERROR_VOTE_OK);
		SEND_Q(rmsg, ch->m_desc);

		// 9. 게임 로그
		GAMELOG << init("EVENT WORLDCUP 2006 VOTE", ch)
				<< "COUNTRY" << delim
				<< tableCountry[nCountryIndex][TABLE_REF_COUNTRY] << delim
				<< "FIFA CUP" << delim
				<< tableCountry[nCountryIndex][TABLE_REF_FIFACUP] << delim
				<< "REMAIN" << delim;
		if (pItemFIFACup && pItemFIFACup->Count() > 0)
			GAMELOG << pItemFIFACup->Count();
		else
			GAMELOG << 0;
		GAMELOG << end;
		break;


	// 보상 처리
	case MSG_EVENT_WORLDCUP_GIFT:

		msg >> nRank;

		// 1. 보상 기간 검사
		// 2. 순위권 국가 검사
		// 2.1.1. 순위권 국기 소유 검사
		// 2.1.2. 보상품 결정
		// 2.2.1. 비 순위권 국기 소유 리스트 작성
		// 2.2.2. 보상품 및 수량 결정
		// 3. 보상품 생성
		// 4. 보상품 지급
		// 5. 게임 로그 1
		// 6. 국기 회수
		// 7. 결과 알림
		// 8. 게임 로그 2

		// 1. 보상 기간 검사
#ifdef EVENT_WORLDCUP_2006_GIFT
#else
		EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_NOTCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
#endif // EVENT_WORLDCUP_2006_GIFT

		// 2. 순위권 국가 검사
		if (1 <= nRank && nRank <= 3)
		{
			// 2.1.1. 순위권 국기 소유 검사
			for (nCountryIndex = 0; nCountryIndex < nCountOfCountry; nCountryIndex++)
			{
				if (tableCountry[nCountryIndex][TABLE_REF_RANK] == nRank)
					break;
			}
			if (nCountryIndex == nCountOfCountry)
				return ;
			if (!ch->m_invenEvent.FindItem(&nRowFlagItem, &nColFlagItem, tableCountry[nCountryIndex][TABLE_REF_FLAG], 0, 0))
			{
				EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			pItemFlag = ch->m_invenEvent.GetItem(nRowFlagItem, nColFlagItem);
			if (!pItemFlag)
			{
				EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			listFlagItemIndex.AddToHead(pItemFlag->m_index);
			pItemFlag = NULL;

			// 2.1.2. 보상품 결정
			nGiftItemIndex = tableGift[nRank - 1][0];
			nGiftItemCount = tableGift[nRank - 1][1];
		}
		else
		{
			nRank = 4;

			// 2.2.1. 비 순위권 국기 소유 리스트 작성
			// event 인벤토리 전체에서
			for (nRowFlagItem = 0; nRowFlagItem < MAX_INVENTORY_ROWS; nRowFlagItem++)
			{
				for (nColFlagItem = 0; nColFlagItem < ITEMS_PER_ROW; nColFlagItem++)
				{
					pItemFlag = ch->m_invenEvent.GetItem(nRowFlagItem, nColFlagItem);
					if (pItemFlag)
					{
						// 32개국 국기 아이템 인덱스와 비교하여
						for (nCountryIndex = 0; nCountryIndex < nCountOfCountry; nCountryIndex++)
						{
							if (tableCountry[nCountryIndex][TABLE_REF_FLAG] == pItemFlag->m_idNum)
								break;
						}
						if (nCountryIndex < nCountOfCountry)
						{
							// 32개국 국기 중 하나면
							// 순위권 국가가 아닌지 검사
							if (tableCountry[nCountryIndex][TABLE_REF_RANK] > 3)
							{
								listFlagItemIndex.AddToTail(pItemFlag->m_index);
							}
						}
						pItemFlag = NULL;
					}
				}
			}
			if (listFlagItemIndex.GetCount() < 1)
			{
				EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 2.2.2. 보상품 및 수량 결정
			nGiftItemIndex = tableGift[nRank - 1][0];
			nGiftItemCount = tableGift[nRank - 1][1] * listFlagItemIndex.GetCount();
		}

		// 3. 보상품 생성
		pItemGift = gserver.m_itemProtoList.CreateItem(nGiftItemIndex, -1, 0, 0, nGiftItemCount);
		if (!pItemGift)
		{
			EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 4. 보상품 지급
		if (AddToInventory(ch, pItemGift, true, true))
		{
			if (pItemGift->tab() < 0)
			{
				ch->m_invenNormal.FindItem(&nRowFlagItem, &nColFlagItem, pItemGift->m_idNum, 0, 0);
				delete pItemGift;
				pItemGift = ch->m_invenNormal.GetItem(nRowFlagItem, nColFlagItem);
				ItemUpdateMsg(rmsg, pItemGift, nGiftItemCount);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemAddMsg(rmsg, pItemGift);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		else
		{
			EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_FULLINVEN);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 5. 게임 로그 1
		GAMELOG << init("EVENT WORLDCUP 2006 GIFT", ch)
				<< "RANK" << delim
				<< nRank << delim
				<< "FLAG" << delim;

		// 6. 국기 회수
		while (listFlagItemIndex.GetCount() > 0)
		{
			nRowFlagItem = listFlagItemIndex.GetData(listFlagItemIndex.GetHead());
			listFlagItemIndex.Remove(listFlagItemIndex.GetHead());
			pItemFlag = ch->m_invenEvent.GetItem(nRowFlagItem);
			if (pItemFlag)
			{
				GAMELOG << itemlog(pItemFlag) << delim;
				ItemDeleteMsg(rmsg, pItemFlag);
				SEND_Q(rmsg, ch->m_desc);
				RemoveFromInventory(ch, pItemFlag, true, true);
			}
		}

		// 7. 결과 알림
		EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_OK);
		SEND_Q(rmsg, ch->m_desc);

		// 8. 게임 로그 2
		GAMELOG << "GIFT" << delim
				<< nGiftItemIndex << delim
				<< "COUNT" << delim
				<< nGiftItemCount << delim
				<< "TOTAL" << delim
				<< itemlog(pItemGift)
				<< end;
		break;

	default:
		return ;
	}

#undef TABLE_REF_COUNTRY
#undef TABLE_REF_FIFACUP
#undef TABLE_REF_FLAG
#undef TABLE_REF_RANK
}

void do_EventGoldenball(CPC* ch, CNetMsg& msg)
{
	// 필요 공 수
	static const int nNeedBallCount = 10;

	// 상품 테이블 : index, flag, count
	static const int tableGift[3][4] = {
		{ 85,   0,  5},			// 승리국/점수 적중			고제 5개
		{ 85,   0,  1},			// 승리국 적중				고제 1개
		{723,   0,  5},			// 꽝						문스톤 5개
	};

	CNetMsg			rmsg;

	int				nSubType;
	int				nTeam1Score;
	int				nTeam2Score;

	int				nRow;
	int				nCol;
	CItem*			pItemBall;
	CItem*			pItemCard;
	CItem*			pItemGift;
	int				nCardScore;
	int				nGiftType;		// 0 - 모두 적중, 1 - 승리국 적중, 2 - 꽝

	msg >> nSubType;
	
	switch (nSubType)
	{

	case MSG_EVENT_GOLDENBALL_VOTE:

		msg >> nTeam1Score
			>> nTeam2Score;

		// 1. 스코어 검사
		// 2. 응모 기간 검사
		// 3. 응모 여부 검사
		// 4. 공 아이템 검사
		// 5. 카드 지급
		// 6. 결과 알림
		// 7. 게임로그

		// 1. 스코어 검사
		if (nTeam1Score < 0 || nTeam1Score > 10000 || nTeam2Score < 0 || nTeam2Score > 10000)
			return ;

		// 2. 응모 기간 검사
		if (gserver.m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_VOTE)
		{
			EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_TIME);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 3. 응모 여부 검사
		if (ch->m_invenEvent.FindItem(&nRow, &nCol, GOLDENBALL_CARD_INDEX, -1, -1))
		{
			EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_ALREADY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 4. 공 아이템 검사
		if (!ch->m_invenNormal.FindItem(&nRow, &nCol, GOLDENBALL_BALL_INDEX, 0, 0))
		{
			EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItemBall = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pItemBall == NULL || pItemBall->Count() < nNeedBallCount)
		{
			EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 5. 카드 지급 및 공 회수 : plus에 점수 설정
		nCardScore = ((nTeam1Score << 16) & 0xffff0000) | (nTeam2Score & 0x0000ffff);
		pItemCard = gserver.m_itemProtoList.CreateItem(GOLDENBALL_CARD_INDEX, -1, nCardScore, 0, 1);
		if (!pItemCard)
		{
			EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (!AddToInventory(ch, pItemCard, true, true))
		{
			delete pItemCard;
			pItemCard = NULL;
			EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_FULLINVEN);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (pItemCard->tab() < 0)
		{
			ch->m_invenEvent.FindItem(&nRow, &nCol, pItemCard->m_idNum, pItemCard->m_plus, pItemCard->m_flag);
			delete pItemCard;
			pItemCard = ch->m_invenEvent.GetItem(nRow, nCol);
			ItemUpdateMsg(rmsg, pItemCard, 1);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemAddMsg(rmsg, pItemCard);
			SEND_Q(rmsg, ch->m_desc);
		}
		if (pItemBall->Count() > nNeedBallCount)
		{
			DecreaseFromInventory(ch, pItemBall, nNeedBallCount);
			ItemUpdateMsg(rmsg, pItemBall, -nNeedBallCount);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pItemBall);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItemBall, true, true); 
			pItemBall = NULL;
		}

		// 6. 결과 알림
		EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_OK);
		SEND_Q(rmsg, ch->m_desc);

		// 7. 게임로그
		GAMELOG << init("EVENT GOLDEN BALL VOTE", ch)
				<< "SCORE" << delim
				<< gserver.m_clGoldenBall.GetTeam1Name() << delim
				<< nTeam1Score << delim
				<< gserver.m_clGoldenBall.GetTeam2Name() << delim
				<< nTeam2Score << delim
				<< "CARD" << delim
				<< itemlog(pItemCard, true) << delim
				<< "BALL REMAIN" << delim;
		if (pItemBall)
			GAMELOG << pItemBall->Count();
		else
			GAMELOG << 0;
		GAMELOG << end;
		break;



	case MSG_EVENT_GOLDENBALL_GIFT:

		// 1. 보상 기간 검사
		// 2. 카드 찾기
		// 3. 점수 뽑기
		// 4. 적중 판단
		// 4.1. 점수가 정확히 맞으면 0
		// 4.2. 무승부가 아니면 승리국만 적중하는 경우 검사
		// 5. 아이템 생성
		// 6. 아이템 지급
		// 7. 게임로그
		// 8. 카드 회수
		// 9. 결과 알림

		// 1. 보상 기간 검사
		if (gserver.m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_GIFT)
		{
			EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_TIME);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 2. 카드 찾기
		if (!ch->m_invenEvent.FindItem(&nRow, &nCol, GOLDENBALL_CARD_INDEX, -1, -1))
		{
			EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItemCard = ch->m_invenEvent.GetItem(nRow, nCol);
		if (!pItemCard)
		{
			EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 3. 점수 뽑기
		nCardScore = pItemCard->m_plus;
		nTeam1Score = (nCardScore >> 16) & 0x0000ffff;
		nTeam2Score =  nCardScore        & 0x0000ffff;

		// 4. 적중 판단
		nGiftType = 2;
		// 4.1. 점수가 정확히 맞으면 0
		if (nTeam1Score == gserver.m_clGoldenBall.GetTeam1Score() && nTeam2Score == gserver.m_clGoldenBall.GetTeam2Score())
			nGiftType = 0;
		// 4.2. 무승부가 아니면 승리국만 적중하는 경우 검사
		else if ( (nTeam1Score != nTeam2Score) && (gserver.m_clGoldenBall.GetTeam1Score() != gserver.m_clGoldenBall.GetTeam2Score()) )
		{
			if ((nTeam1Score > nTeam2Score) == (gserver.m_clGoldenBall.GetTeam1Score() > gserver.m_clGoldenBall.GetTeam2Score()))
				nGiftType = 1;
		}

		// 5. 아이템 생성
		pItemGift = gserver.m_itemProtoList.CreateItem(tableGift[nGiftType][0], -1, 0, tableGift[nGiftType][1], tableGift[nGiftType][2]);
		if (!pItemGift)
		{
			EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 6. 아이템 지급
		if (!AddToInventory(ch, pItemGift, true, true))
		{
			delete pItemGift;
			pItemGift = NULL;
			EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_FULLINVEN);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (pItemGift->tab() < 0)
		{
			ch->m_invenNormal.FindItem(&nRow, &nCol, pItemGift->m_idNum, pItemGift->m_plus, pItemGift->m_flag);
			delete pItemGift;
			pItemGift = ch->m_invenNormal.GetItem(nRow, nCol);
			ItemUpdateMsg(rmsg, pItemGift, tableGift[nGiftType][2]);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemAddMsg(rmsg, pItemGift);
			SEND_Q(rmsg, ch->m_desc);
		}

		// 7. 게임로그
		GAMELOG << init("EVENT GOLDEN BALL GIFT", ch)
				<< "GIFT TYPE" << delim
				<< nGiftType << delim
				<< "GIFT" << delim
				<< tableGift[nGiftType][0] << delim
				<< pItemGift->m_itemProto->m_name << delim
				<< pItemGift->m_plus << delim
				<< tableGift[nGiftType][1] << delim
				<< tableGift[nGiftType][2] << delim
				<< "CARD" << delim
				<< itemlog(pItemCard)
				<< end;

		// 8. 카드 회수
		ItemDeleteMsg(rmsg, pItemCard);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, pItemCard, true, true);

		// 9. 결과 알림
		switch (nGiftType)
		{
		case 0:		EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_EXACT);		break;
		case 1:		EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_WINNER);	break;
		case 2:		EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_NONE);		break;
		}
		SEND_Q(rmsg, ch->m_desc);
		break;

	default:
		return ;
	}
}

#ifdef EVENT_RAIN_2006
void do_EventRain2006(CPC* ch, CNetMsg& msg)
{
	// 아이템 인덱스
#ifdef LC_TLD
	// 태국 어머니날 이벤트로 변경
	static const int nRaindrop = 1705;
	static const int nRainBottle = 1704;
#else
	static const int nRaindrop = 1518;
	static const int nRainBottle = 1517;
#endif

	// 교환 필요 방울 수
	static const int nNeedDrops = 10;

	// 물병당 방울 수 : nNeedDrops % nDropsPerBottle == 0 이어야 한다
	static const int nDropsPerBottle = 5;

	// 보상 테이블 : 확률[1/10000], index1, flag1, count1, index2, flag2, count2
	static const int nTableRefProb		= 0;	// 테이블 참조 인덱스
	static const int nTableRefIndex1	= 1;
	static const int nTableRefFlag1		= 2;
	static const int nTableRefCount1	= 3;
	static const int nTableRefIndex2	= 4;
	static const int nTableRefFlag2		= 5;
	static const int nTableRefCount2	= 6;
	static const int nTableSize			= 7;	// 보상 테이블 크기
	static const int tableGift[nTableSize][7] = {
		{2500, 556, 0, 5,  -1, 0, 0},
		{1700,  45, 0, 5,  -1, 0, 0},
		{1400, 500, 5, 3, 501, 5, 3},
		{1400, 498, 5, 3, 499, 5, 3},
		{1100, 973, 0, 1,  -1, 0, 0},
		{1100, 972, 0, 1,  -1, 0, 0},
#ifdef LC_USA
		{ 800,  779, 0, 1,  -1, 0, 0},
#else
		{ 800,  85, 0, 1,  -1, 0, 0},
#endif // LC_USA
	};

	CNetMsg			rmsg;
	int				nRow;
	int				nCol;
	CItem*			pItemRaindrop = NULL;
	CItem*			pItemRainBottle = NULL;
	int				nRainCount = 0;		// 빗방울 1, 빗물병 5 씩 증가
	int				nGiftType;
	int				nProb;				// 확률 변수 : 루프에서 감소되며 비교함
	CItem*			pItemGift1 = NULL;
	CItem*			pItemGift2 = NULL;

	bool			bItemDrop1 = false;
	bool			bItemDrop2 = false;

	// 1. 각각의 아이템 찾기
	// 2. 물병 수량 검사
	// 3. 보상품 결정
	// 4. 보상품 지급
	// 5. 게임 로그 1
	// 6. 물병 회수
	// 7. 방울 회수
	// 8. 게임 로그 2
	// 9. 결과 알림

	// 1. 각각의 아이템 찾기
	if (ch->m_invenNormal.FindItem(&nRow, &nCol, nRaindrop, 0, 0))
		pItemRaindrop = ch->m_invenNormal.GetItem(nRow, nCol);
	if (ch->m_invenNormal.FindItem(&nRow, &nCol, nRainBottle, 0, 0))
		pItemRainBottle = ch->m_invenNormal.GetItem(nRow, nCol);

	// 2. 방울 수량 검사
	if (pItemRaindrop)
		nRainCount += pItemRaindrop->Count();
	if (pItemRainBottle)
		nRainCount += pItemRainBottle->Count() * nDropsPerBottle;
	if (nRainCount < nNeedDrops)
	{
		EventRain2006Msg(rmsg, MSG_EVENT_RAIN_2006_ERROR_NOITEM);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	nRainCount = nNeedDrops;

	// 3. 보상품 결정
	nProb = GetRandom(1, 10000);
	for (nGiftType = 0; nGiftType < nTableSize; nGiftType++)
	{
		if (nProb <= tableGift[nGiftType][nTableRefProb])
			break;
		else
			nProb -= tableGift[nGiftType][nTableRefProb];
	}
	if (nGiftType == nTableSize)
		nGiftType = nTableSize - 1;

	// 4. 보상품 지급
	pItemGift1 = gserver.m_itemProtoList.CreateItem(tableGift[nGiftType][nTableRefIndex1], -1, 0, tableGift[nGiftType][nTableRefFlag1], tableGift[nGiftType][nTableRefCount1]);
	if (!pItemGift1)
	{
		EventRain2006Msg(rmsg, MSG_EVENT_RAIN_2006_ERROR_NOITEM);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (tableGift[nGiftType][nTableRefIndex2] > 0)
	{
		pItemGift2 = gserver.m_itemProtoList.CreateItem(tableGift[nGiftType][nTableRefIndex2], -1, 0, tableGift[nGiftType][nTableRefFlag2], tableGift[nGiftType][nTableRefCount2]);
		if (!pItemGift2)
		{
			delete pItemGift1;
			pItemGift1 = NULL;
			EventRain2006Msg(rmsg, MSG_EVENT_RAIN_2006_ERROR_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}
	if (!AddToInventory(ch, pItemGift1, true, true))
	{
		/*
		delete pItemGift1;
		pItemGift1 = NULL;
		EventRain2006Msg(rmsg, MSG_EVENT_RAIN_2006_ERROR_FULLINVEN);
		SEND_Q(rmsg, ch->m_desc);
		return ;
		*/
		ch->m_pArea->DropItem(pItemGift1, ch);
		pItemGift1->m_preferenceIndex = ch->m_index;
		ItemDropMsg(rmsg, ch, pItemGift1);
		SEND_Q(rmsg, ch->m_desc);
		bItemDrop1 = true;
	}
	if (pItemGift2)
	{
		if (!AddToInventory(ch, pItemGift2, true, true))
		{
			/*
			// 2번째꺼 못주면 1번째꺼 회수
			if (pItemGift1->tab() < 0)
			{
				// 겹쳐서 지급한거면 수량 감소
				ch->m_invenNormal.FindItem(&nRow, &nCol, pItemGift1->m_idNum, pItemGift1->m_plus, pItemGift1->m_flag);
				delete pItemGift1;
				pItemGift1 = ch->m_invenNormal.GetItem(nRow, nCol);
				DecreaseFromInventory(ch, pItemGift1, tableGift[nGiftType][nTableRefCount1]);
				pItemGift1 = NULL;
			}
			else
			{
				// 추가된거면 제거
				RemoveFromInventory(ch, pItemGift1, true, true);
				pItemGift1 = NULL;
			}

			delete pItemGift2;
			pItemGift2 = NULL;

			EventRain2006Msg(rmsg, MSG_EVENT_RAIN_2006_ERROR_FULLINVEN);
			SEND_Q(rmsg, ch->m_desc);
			return ;
			*/
			ch->m_pArea->DropItem(pItemGift2, ch);
			pItemGift2->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, pItemGift2);
			SEND_Q(rmsg, ch->m_desc);	
			bItemDrop2 = true;
		}
	}

	// 5. 게임 로그 1
	GAMELOG << init("EVENT RAIN 2006 GIFT", ch);

	// 6. 물병 회수
	if (pItemRainBottle)
	{
		if (pItemRainBottle->Count() * nDropsPerBottle > nNeedDrops)
		{
			nRainCount -= nNeedDrops;
			DecreaseFromInventory(ch, pItemRainBottle, nNeedDrops / nDropsPerBottle);
			ItemUpdateMsg(rmsg, pItemRainBottle, -nNeedDrops / nDropsPerBottle);
			SEND_Q(rmsg, ch->m_desc);

			GAMELOG << "RAINBOTTLE" << delim
					<< (nNeedDrops / nDropsPerBottle) << delim
					<< pItemRainBottle->Count() << delim;
		}
		else
		{
			GAMELOG << "RAINBOTTLE" << delim
					<< (pItemRainBottle->Count() * nDropsPerBottle) << delim
					<< 0 << delim;

			nRainCount -= pItemRainBottle->Count() * nDropsPerBottle;
			ItemDeleteMsg(rmsg, pItemRainBottle);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItemRainBottle, true, true);
			pItemRainBottle = NULL;
		}
	}

	// 7. 방울 회수
	if (pItemRaindrop && nRainCount > 0)
	{
		if (pItemRaindrop->Count() > nRainCount)
		{
			DecreaseFromInventory(ch, pItemRaindrop, nRainCount);
			ItemUpdateMsg(rmsg, pItemRaindrop, -nRainCount);
			SEND_Q(rmsg, ch->m_desc);

			GAMELOG << "RAINDROP" << delim
					<< nRainCount << delim
					<< pItemRaindrop->Count() << delim;
		}
		else
		{
			GAMELOG << "RAINDROP" << delim
					<< pItemRaindrop->Count() << delim
					<< 0 << delim;

			ItemDeleteMsg(rmsg, pItemRaindrop);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch,  pItemRaindrop, true, true);
			pItemRaindrop = NULL;
		}
	}

	// 8. 게임 로그 2
	GAMELOG << "GIFT 1" << delim
			<< itemlog(pItemGift1) << delim
			<< "GIFT 2" << delim;
	if (pItemGift2)
		GAMELOG << itemlog(pItemGift2);
	else
		GAMELOG << "(NONE)";
	GAMELOG << end;

	// 9. 결과 알림
	if(bItemDrop1 == false)
	{
		if (pItemGift1->tab() < 0)
		{
			ch->m_invenNormal.FindItem(&nRow, &nCol, pItemGift1->m_idNum, pItemGift1->m_plus, pItemGift1->m_flag);
			delete pItemGift1;
			pItemGift1 = ch->m_invenNormal.GetItem(nRow, nCol);
			ItemUpdateMsg(rmsg, pItemGift1, tableGift[nGiftType][nTableRefCount1]);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemAddMsg(rmsg, pItemGift1);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	if(bItemDrop2 == false)
	{
		if (pItemGift2)
		{
			if (pItemGift2->tab() < 0)
			{
				ch->m_invenNormal.FindItem(&nRow, &nCol, pItemGift2->m_idNum, pItemGift2->m_plus, pItemGift2->m_flag);
				delete pItemGift2;
				pItemGift2 = ch->m_invenNormal.GetItem(nRow, nCol);
				ItemUpdateMsg(rmsg, pItemGift2, tableGift[nGiftType][nTableRefCount2]);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemAddMsg(rmsg, pItemGift2);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}

	EventRain2006Msg(rmsg, MSG_EVENT_RAIN_2006_ERROR_OK);
	SEND_Q(rmsg, ch->m_desc);
}
#endif // EVENT_RAIN_2006

#ifdef EVENT_TLD_BUDDHIST
void do_EventTldBuddhist(CPC* ch, CNetMsg& msg)
{
	// 모래 봉투 인덱스
	static const int nSandItemIndex[4] = {
		1571,		// 빨강 (R)
		1572,		// 노랑 (Y)
		1573,		// 녹색 (G)
		1574,		// 남색 (B)		
	};

	// 보상 테이블 : 인덱스, 적,황 필요모래 수, 녹남 필요모래 수
	static const int tableGift[7][3] = {
		{974, 10, 10 },		// 행운의 고급 제련석
		{85,  7, 7},		// 고급 제련석
		{1576,  5, 5},		// 성수병
		{884, 3, 3},		// 드롭률 증폭제
		{1575,  2, 2},		// 양초
		{675, 2, 0},		// 구원의 눈물
		{676,  0, 2},		// 용서의 눈물
	};

	CNetMsg		rmsg;
	char		cRequestType = 0;
	int			nGiftItemIndex = 0;
	int			nNeedRYSand = 0;
	int			nNeedGBSand = 0;

	CItem*		pItemSand[4] = {NULL, NULL, NULL, NULL};		// 모래 아이템
	CItem*		pItemGift = NULL;
	int			i;
	int			nRow, nCol;

	// 1. 요청한 타입에 따라 인덱스 및 수량 결정
	// 2. 모래 아이템 검사
	// 3. 아이템 생성
	// 4. 아이템 지급
	// 5. 모래 회수
	// 6. 게임 로그
	// 7. 결과 알림

	// 1. 요청한 타입에 따라 인덱스 및 수량 결정
	msg >> cRequestType;
	if( cRequestType > 6 )
	{
		return;
	}	

	nGiftItemIndex	= tableGift[cRequestType][0];
	nNeedRYSand		= tableGift[cRequestType][1];
	nNeedGBSand		= tableGift[cRequestType][2];

	// 2. 모래 아이템 검사
	for (i = 0; i < 4; i++)
	{
		int nNeedSand = i<2 ? nNeedRYSand: nNeedGBSand;

		if ( nNeedSand == 0 )
			continue;

		if (!ch->m_invenNormal.FindItem(&nRow, &nCol, nSandItemIndex[i], 0, 0))
		{
			EventTldBuddhistMsg(rmsg, cRequestType, MSG_EVENT_TLD_BUDDHIST_ERROR_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItemSand[i] = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pItemSand[i] == NULL)
		{
			EventTldBuddhistMsg(rmsg, cRequestType, MSG_EVENT_TLD_BUDDHIST_ERROR_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (pItemSand[i]->Count() < nNeedSand)
		{
			EventTldBuddhistMsg(rmsg, cRequestType, MSG_EVENT_TLD_BUDDHIST_ERROR_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	// 3. 아이템 생성
	pItemGift = gserver.m_itemProtoList.CreateItem(nGiftItemIndex, -1, 0, 0, 1 );
	if (!pItemGift)
	{
		EventTldBuddhistMsg(rmsg, cRequestType, MSG_EVENT_TLD_BUDDHIST_ERROR_NOITEM);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 4. 아이템 지급
	if (AddToInventory(ch, pItemGift, true, true))
	{
		if (pItemGift->tab() < 0)
		{
			delete pItemGift;
			ch->m_invenNormal.FindItem(&nRow, &nCol, nGiftItemIndex, 0, 0);
			pItemGift = ch->m_invenNormal.GetItem(nRow, nCol);
			ItemUpdateMsg(rmsg, pItemGift, 1 );
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemAddMsg(rmsg, pItemGift);
			SEND_Q(rmsg, ch->m_desc);
		}

		// 5. 모래 회수
		for (i = 0; i < 4; i++)
		{
			int nNeedSand = i<2 ? nNeedRYSand: nNeedGBSand;
			if(nNeedSand == 0)
				continue;

			if (pItemSand[i]->Count() > nNeedSand)
			{
				DecreaseFromInventory(ch, pItemSand[i], nNeedSand);
				ItemUpdateMsg(rmsg, pItemSand[i], -nNeedSand);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, pItemSand[i]);
				SEND_Q(rmsg, ch->m_desc);
				RemoveFromInventory(ch, pItemSand[i], true, true);
				pItemSand[i] = NULL;
			}
		}

		// 6. 게임 로그
		GAMELOG << init("EVENT TLD BUDDHIST CHANGE", ch)
				<< "REQUEST" << delim
				<< cRequestType << delim
				<< "GIFT" << delim
				<< itemlog(pItemGift, true) << delim
				<< "SAND REMAIN (RED/YELLOW/BLUE/GREEN)";
		for (i = 0; i < 4; i++)
		{
			if (pItemSand[i])
				GAMELOG << delim << pItemSand[i]->Count();
			else
				GAMELOG << delim << 0;
		}
		GAMELOG << end;

		// 7. 결과 알림
		EventTldBuddhistMsg(rmsg, cRequestType, MSG_EVENT_TLD_BUDDHIST_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		EventTldBuddhistMsg(rmsg, cRequestType, MSG_EVENT_TLD_BUDDHIST_ERROR_FULLINVEN);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}
#endif // EVENT_TLD_BUDDHIST

#ifdef EVENT_COLLECT_BUG
void TO_BUGDATA(CItem* pItem, unsigned char ucBug[16])
{
	ucBug[ 0] = (unsigned char)((pItem->m_plus >> 28) & 0x0f);
	ucBug[ 1] = (unsigned char)((pItem->m_plus >> 24) & 0x0f);
	ucBug[ 2] = (unsigned char)((pItem->m_plus >> 20) & 0x0f);
	ucBug[ 3] = (unsigned char)((pItem->m_plus >> 16) & 0x0f);
	ucBug[ 4] = (unsigned char)((pItem->m_plus >> 12) & 0x0f);
	ucBug[ 5] = (unsigned char)((pItem->m_plus >>  8) & 0x0f);
	ucBug[ 6] = (unsigned char)((pItem->m_plus >>  4) & 0x0f);
	ucBug[ 7] = (unsigned char)((pItem->m_plus      ) & 0x0f);
	ucBug[ 8] = (unsigned char)((pItem->m_used >> 28) & 0x0f);
	ucBug[ 9] = (unsigned char)((pItem->m_used >> 24) & 0x0f);
	ucBug[10] = (unsigned char)((pItem->m_used >> 20) & 0x0f);
	ucBug[11] = (unsigned char)((pItem->m_used >> 16) & 0x0f);
	ucBug[12] = (unsigned char)((pItem->m_used >> 12) & 0x0f);
	ucBug[13] = (unsigned char)((pItem->m_used >>  8) & 0x0f);
	ucBug[14] = (unsigned char)((pItem->m_used >>  4) & 0x0f);
	ucBug[15] = (unsigned char)((pItem->m_used      ) & 0x0f);

	if( ucBug[0] == 11 )
		ucBug[0] = 8;

	if( ucBug[8] == 11 )
		ucBug[8] = 8;
}

void FROM_BUGDATA(unsigned char ucBug[16], CItem* pItem)
{
	if( ucBug[8] == 8 )
	{
		ucBug[8] = 11;
	}

	if( ucBug[0] == 8 )
	{
		ucBug[0] = 11;
	}

	pItem->m_plus	= ((unsigned int)(ucBug[ 0]) << 28) & 0xf0000000
					| ((unsigned int)(ucBug[ 1]) << 24) & 0x0f000000
					| ((unsigned int)(ucBug[ 2]) << 20) & 0x00f00000
					| ((unsigned int)(ucBug[ 3]) << 16) & 0x000f0000
					| ((unsigned int)(ucBug[ 4]) << 12) & 0x0000f000
					| ((unsigned int)(ucBug[ 5]) <<  8) & 0x00000f00
					| ((unsigned int)(ucBug[ 6]) <<  4) & 0x000000f0
					| ((unsigned int)(ucBug[ 7])      ) & 0x0000000f;
	pItem->m_used	= ((unsigned int)(ucBug[ 8]) << 28) & 0xf0000000
					| ((unsigned int)(ucBug[ 9]) << 24) & 0x0f000000
					| ((unsigned int)(ucBug[10]) << 20) & 0x00f00000
					| ((unsigned int)(ucBug[11]) << 16) & 0x000f0000
					| ((unsigned int)(ucBug[12]) << 12) & 0x0000f000
					| ((unsigned int)(ucBug[13]) <<  8) & 0x00000f00
					| ((unsigned int)(ucBug[14]) <<  4) & 0x000000f0
					| ((unsigned int)(ucBug[15])      ) & 0x0000000f;
}

void do_EventCollectBug(CPC* ch, CNetMsg& msg)
{
	static const int	nNormalBoxIndex = 1577;		// 일반 상자 인덱스
	static const int	nGoldenBoxIndex = 1578;		// 골든 상자 인덱스
	static const int	nGoldenPoint = 30;			// 골든 상자 보너스 포인트

#ifdef LC_JPN
	static const int	tableGift[12][2] = {
		{  19,1000},		// 나스				1000	//	1-25
		{  47,   3},		// 메모리 스크롤	3		//	26-40		
		{ 723,   2},		// 문스톤			2		//	41-55
		{ 723,   5},		// 문스톤			5		//	56-70
		{ 500,   3},		// 푸른 케르(Lv5)   3		//	71-85
		{ 498,   3},		// 붉은 케르(Lv5)	3		//	86-100
		{ 724,   5},		// 대형마나회복제	5		//	101-115
		{  45,   5},		// 대형체력회복제	5		//	116-120
		{ 493,   5},		// 소형마나회복증속약 5		//	121-125
		{ 490,   5},		// 소형체력회복증속약 5		//	126-130
		{ 786,   5},		// 부스터			5		//	131-159
		{  85,   1},		// 고급 제련석		1		//	160-
	};
#else
	static const int	tableGift[12][2] = {
		{  19, 500},		// 나스				500
		{  47,   3},		// 메모리 스크롤	3
		{ 724,   5},		// 대형 마나 회복제	5
		{  45,   5},		// 대형 체력 회복제	5
		{ 723,   2},		// 문스톤			2
		{ 723,   5},		// 문스톤			5
		{1628,   5},		// 참외				5
		{1629,   5},		// 자두				5
		{1627,   5},		// 수박				5
		{  85,   1},		// 고급제련석		1
		{  85,   2},		// 고급제련석		2
		{  85,   3},		// 고급제련석		3
	};
#endif // LC_JPN

#ifdef EVENT_COLLECT_BUG_DROP
	static const int	nNeedMoney = 100;			// 상자 구매 금액
	static const int	nProbGoldenBox = 1;			// 골든 상자 확률
#endif // EVENT_COLLECT_BUG_DROP

	CNetMsg			rmsg;
	int				nSubType = 0;
	CItem*			pItemBox = NULL;
	int				nRow, nCol;
	int				nSelectedBoxIndex = 0;
	int				nSelectedBoxPoint = 0;
	bool			bIsFull = true;
	int				nGift = 0;
	int				nGiftIndex = 0;
	int				nGiftCount = 0;
	CItem*			pItemGift = NULL;
	unsigned char	ucBug[16];
	int				i;

#ifdef EVENT_COLLECT_BUG_DROP
	int				nProb = 0;
#endif // EVENT_COLLECT_BUG_DROP

	msg >> nSubType;

	switch (nSubType)
	{
	case MSG_EVENT_COLLECT_BUG_DROP:
		msg >> nSelectedBoxIndex;
		if (nSelectedBoxIndex < 0 || nSelectedBoxIndex >= 16)
			return ;

		// 1. 일반 상자 검색
		// 2. 골든 상자 검색
		// 3. 게임 로그 1
		// 4. 상자 내용 변경
		// 5. 게임 로그 2
		// 6. 결과 알림

		// 1. 일반 상자 검색
		if (ch->m_invenNormal.FindItem(&nRow, &nCol, nNormalBoxIndex, -1, -1))
		{
			pItemBox = ch->m_invenNormal.GetItem(nRow, nCol);
			TO_BUGDATA(pItemBox, ucBug);
		}

		// 2. 골든 상자 검색
		if (!pItemBox && ch->m_invenNormal.FindItem(&nRow, &nCol, nGoldenBoxIndex, -1, -1))
		{
			pItemBox = ch->m_invenNormal.GetItem(nRow, nCol);
			TO_BUGDATA(pItemBox, ucBug);
		}

		if (pItemBox)
		{
			// 3. 게임 로그 1
			GAMELOG << init("EVENT COLLECT BUG DEL", ch)
					<< "POSITION" << delim
					<< nSelectedBoxIndex << delim
					<< "PREV" << delim
					<< itemlog(pItemBox) << delim
					<< "NEXT" << delim;

			// 4. 상자 내용 변경
			pItemBox->m_flag -= ucBug[nSelectedBoxIndex];
			ucBug[nSelectedBoxIndex] = 0;
			FROM_BUGDATA(ucBug, pItemBox);
			ItemUpdateMsg(rmsg, pItemBox, 0);
			SEND_Q(rmsg, ch->m_desc);

			// 5. 게임 로그 2
			GAMELOG << itemlog(pItemBox)
					<< end;

			// 6. 결과 알림
			EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_DROP, nSelectedBoxIndex);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;

	case MSG_EVENT_COLLECT_BUG_GIFT_REQ:
		// 1. 일반 상자 검색
		// 2. 골든 상자 검색
		// 3. 포인트에 따른 보상 선택
		// 4. 보상 지급
		// 5. 게임 로그 1
		// 6. 게임 로그 2
		// 7. 상자 회수
		// 8. 결과 알림

		// 1. 소유 상자 검색
		if (ch->m_invenNormal.FindItem(&nRow, &nCol, nNormalBoxIndex, -1, -1))
		{
			pItemBox = ch->m_invenNormal.GetItem(nRow, nCol);
			nSelectedBoxPoint = 0;
			TO_BUGDATA(pItemBox, ucBug);
			for (i = 0; i < 16; i++)
			{
				if (ucBug[i] == 0)
					break;
				nSelectedBoxPoint += ucBug[i];
			}
			if (i != 16)
			{
				bIsFull = false;
				pItemBox = NULL;
			}
		}

		// 2. 골든 상자 검색
		if (!pItemBox && ch->m_invenNormal.FindItem(&nRow, &nCol, nGoldenBoxIndex, -1, -1))
		{
			bIsFull = true;
			pItemBox = ch->m_invenNormal.GetItem(nRow, nCol);
			nSelectedBoxPoint = 0;
			TO_BUGDATA(pItemBox, ucBug);
			for (i = 0; i < 16; i++)
			{
				if (ucBug[i] == 0)
					break;
				nSelectedBoxPoint += ucBug[i];
			}
			if (i != 16)
			{
				bIsFull = false;
				pItemBox = NULL;
			}

			// 골든 상자 추가 포인트
			nSelectedBoxPoint += nGoldenPoint;
		}
		if (pItemBox == NULL)
		{
			if (bIsFull)
				EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_NOITEM, 0);
			else
				EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_NOTFULL, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 3. 포인트에 따른 보상 선택
		if      (nSelectedBoxPoint <  26)			nGift = 0;
		else if (nSelectedBoxPoint <  41)			nGift = 1;
		else if (nSelectedBoxPoint <  56)			nGift = 2;
		else if (nSelectedBoxPoint <  71)			nGift = 3;
		else if (nSelectedBoxPoint <  86)			nGift = 4;
		else if (nSelectedBoxPoint < 101)			nGift = 5;
		else if (nSelectedBoxPoint < 116)			nGift = 6;
		else if (nSelectedBoxPoint < 121)			nGift = 7;
		else if (nSelectedBoxPoint < 126)			nGift = 8;
		else if (nSelectedBoxPoint < 131)			nGift = 9;
		else if (nSelectedBoxPoint < 160)			nGift = 10;
		else                             			nGift = 11;
		nGiftIndex = tableGift[nGift][0];
		nGiftCount = tableGift[nGift][1];

		// 4. 보상 지급
#ifdef LC_JPN
		if (nGiftIndex == 500 || nGiftIndex == 498)
		{	// 푸른 케르, 붉은 케르 5레벨 
			pItemGift = gserver.m_itemProtoList.CreateItem(nGiftIndex, -1, 0, 5, nGiftCount);
		}
		else
		{
			pItemGift = gserver.m_itemProtoList.CreateItem(nGiftIndex, -1, 0, 0, nGiftCount);
		}		
#else
		pItemGift = gserver.m_itemProtoList.CreateItem(nGiftIndex, -1, 0, 0, nGiftCount);
#endif // LC_JPN
		if (!pItemGift)
		{
			EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_FULLINVEN, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (AddToInventory(ch, pItemGift, true, true))
		{
			// 5. 게임 로그 1
			GAMELOG << init("EVENT COLLECT BUG GIFT", ch)
					<< "POINT" << delim
					<< nSelectedBoxPoint << delim
					<< "BOX" << delim
					<< itemlog(pItemBox) << delim
					<< "GIFT" << delim
					<< itemlog(pItemGift) << delim
					<< "TOTAL" << delim;

			if (pItemGift->tab() < 0)
			{
				delete pItemGift;
				pItemGift = NULL;
#ifdef LC_JPN
				if (nGiftIndex == 500 || nGiftIndex == 498)
				{	// 푸른 케르, 붉은 케르 5레벨 
					ch->m_invenNormal.FindItem(&nRow, &nCol, nGiftIndex, 0, 5);
				}
				else
				{
					ch->m_invenNormal.FindItem(&nRow, &nCol, nGiftIndex, 0, 0);
				}		
#else
				ch->m_invenNormal.FindItem(&nRow, &nCol, nGiftIndex, 0, 0);
#endif // LC_JPN
				pItemGift = ch->m_invenNormal.GetItem(nRow, nCol);
				if(pItemGift == NULL)
				{
					EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_FULLINVEN, 0);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}
				ItemUpdateMsg(rmsg, pItemGift, nGiftCount);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemAddMsg(rmsg, pItemGift);
				SEND_Q(rmsg, ch->m_desc);
			}

			// 6. 게임 로그 2
			GAMELOG << itemlog(pItemGift)
					<< end;

			// 7. 상자 회수
			ItemDeleteMsg(rmsg, pItemBox);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItemBox, true, true);
			pItemBox = NULL;

			// 8. 결과 알림
			EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_GIFT_REP_OK, nGift);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_FULLINVEN, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		break;

	case MSG_EVENT_COLLECT_BUG_BUY_BOX_REQ:
#ifdef EVENT_COLLECT_BUG_DROP
		// 1. 소지 금액 검사
		// 2. 소유 아이템 검사
		// 3. 확률 적용 : 1% 골든 상자
		// 4. 상자 지급
		// 5. 돈 회수
		// 6. 결과 알림
		// 7. 게임 로그

		// 1. 소지 금액 검사
		if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < nNeedMoney)
		{
			EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_MONEY, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 2. 소유 아이템 검사
		if (	   ch->m_invenNormal.FindItem(&nRow, &nCol, nNormalBoxIndex, -1, -1)
				|| ch->m_invenNormal.FindItem(&nRow, &nCol, nGoldenBoxIndex, -1, -1)
			)
		{
			EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_DUPLICATE, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 3. 확률 적용 : 1% 골든 상자
		nProb = GetRandom(1, 100);
		if (nProb == nProbGoldenBox)
		{
			nSelectedBoxIndex = nGoldenBoxIndex;
			nSelectedBoxPoint = nGoldenPoint;
		}
		else
		{
			nSelectedBoxIndex = nNormalBoxIndex;
			nSelectedBoxPoint = 0;
		}

		// 4. 상자 지급
		pItemBox = gserver.m_itemProtoList.CreateItem(nSelectedBoxIndex, -1, 0, nSelectedBoxPoint, 1);
		if (!pItemBox)
		{
			EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_FULLINVEN, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItemBox->m_used = 0;
		if (AddToInventory(ch, pItemBox, true, true))
		{
			ItemAddMsg(rmsg, pItemBox);
			SEND_Q(rmsg, ch->m_desc);

			// 5. 돈 회수
			if (ch->m_moneyItem->Count() > nNeedMoney)
			{
				DecreaseFromInventory(ch, ch->m_moneyItem, nNeedMoney);
				ItemUpdateMsg(rmsg, ch->m_moneyItem, -nNeedMoney);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, ch->m_moneyItem);
				SEND_Q(rmsg, ch->m_desc);
				RemoveFromInventory(ch, ch->m_moneyItem, true, true);
			}

			// 6. 결과 알림
			EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_OK, (nSelectedBoxIndex == nGoldenBoxIndex) ? 1 : 0);
			SEND_Q(rmsg, ch->m_desc);

			// 7. 게임 로그
			GAMELOG << init("EVENT COLLECT BUG BUY", ch)
					<< "BOX" << delim
					<< itemlog(pItemBox)
					<< end;
		}
		else
		{
			EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_FULLINVEN, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
#endif // EVENT_COLLECT_BUG_DROP
		break;
	}
}
#endif // EVENT_COLLECT_BUG

#ifdef EVENT_CHUSEOK_2006_GIFT
void do_EventChuseok2006(CPC* ch, CNetMsg& msg)
{
	static const int		nIndexRice		= 1814;	// 쌀가루
	static const int		nIndexHoney		= 1815;	// 꿀
	static const int		nIndexPine		= 1816;	// 솔잎
	static const int		nIndexCake		= 1817;	// 송편
	static const int		nIndexRainbow	= 1818;	// 오색 송편
	static const int		tableGift[5][3] = {
		{35, 19,  10000},							// 35% 나스  10,000
		{30, 19,  50000},							// 30% 나스  50,000
		{20, 19, 200000},							// 20% 나스 200,000
		{10, 19, 500000},							// 10% 나스 500,000
		{ 5, 85,      2},							//  5% 고제 2
	};

	static const int		nProbCake		= 70;	// 송편 확률 : 70%
	static const int		nProbRainbow	= 50;	// 오색 송편 확률 : 50%

	CNetMsg			rmsg;
	int				nSubtype;
	int				nRow, nCol;
	CItem*			pItemNeed1		= NULL;
	CItem*			pItemNeed2		= NULL;
	CItem*			pItemNeed3		= NULL;
	CItem*			pItemMade		= NULL;
	bool			bSuccess		= false;
	int				nNeedCount		= 10;
	int				nProbGift;
	int				i;

	msg >> nSubtype;

	switch (nSubtype)
	{
	case MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE:			// 송편 제작
		// 재료 1 : 쌀가루
		if (!ch->m_invenNormal.FindItem(&nRow, &nCol, nIndexRice, 0, 0))
		{
			EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItemNeed1 = ch->m_invenNormal.GetItem(nRow, nCol);

		// 재료 2 : 꿀
		if (!ch->m_invenNormal.FindItem(&nRow, &nCol, nIndexHoney, 0, 0))
		{
			EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItemNeed2 = ch->m_invenNormal.GetItem(nRow, nCol);

		// 재료 3 : 솔잎
		if (!ch->m_invenNormal.FindItem(&nRow, &nCol, nIndexPine, 0, 0))
		{
			EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItemNeed3 = ch->m_invenNormal.GetItem(nRow, nCol);

		// 성공 결정
		if (GetRandom(1, 100) <= nProbCake)
		{
			bSuccess = true;

			// 생성 및 지급
			pItemMade = gserver.m_itemProtoList.CreateItem(nIndexCake, -1, 0, 0, 1);
			if (!pItemMade)
			{
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			if (!AddToInventory(ch, pItemMade, true, true))
			{
				delete pItemMade;
				pItemMade = NULL;
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_FULLINVEN);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}

		GAMELOG << init("EVENT CHUSEOK 2006 MAKE CAKE", ch)
				<< "NEED 1"						 << delim
				<< itemlog(pItemNeed1, true)	 << delim
				<< "NEED 2"						 << delim
				<< itemlog(pItemNeed2, true)	 << delim
				<< "NEED 3"						 << delim
				<< itemlog(pItemNeed3, true)	 << delim
				<< "CAKE"						 << delim;
		if (bSuccess)
			GAMELOG << itemlog(pItemMade, true);
		else
			GAMELOG << "FAIL";
		GAMELOG << end;

		// 재료 1 소모
		if (pItemNeed1->Count() == 1)
		{
			ItemDeleteMsg(rmsg, pItemNeed1);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItemNeed1, true, true);
			pItemNeed1 = NULL;
		}
		else
		{
			DecreaseFromInventory(ch, pItemNeed1, 1);
			ItemUpdateMsg(rmsg, pItemNeed1, -1);
			SEND_Q(rmsg, ch->m_desc);
			pItemNeed1 = NULL;
		}

		// 재료 2 소모
		if (pItemNeed2->Count() == 1)
		{
			ItemDeleteMsg(rmsg, pItemNeed2);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItemNeed2, true, true);
			pItemNeed2 = NULL;
		}
		else
		{
			DecreaseFromInventory(ch, pItemNeed2, 1);
			ItemUpdateMsg(rmsg, pItemNeed2, -1);
			SEND_Q(rmsg, ch->m_desc);
			pItemNeed2 = NULL;
		}

		// 재료 3 소모
		if (pItemNeed3->Count() == 1)
		{
			ItemDeleteMsg(rmsg, pItemNeed3);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItemNeed3, true, true);
			pItemNeed3 = NULL;
		}
		else
		{
			DecreaseFromInventory(ch, pItemNeed3, 1);
			ItemUpdateMsg(rmsg, pItemNeed3, -1);
			SEND_Q(rmsg, ch->m_desc);
			pItemNeed3 = NULL;
		}

		// 결과 전송
		if (bSuccess)
		{
			if (pItemMade->tab() < 0)
			{
				CItem*			pItemPrev = NULL;
				CInventory*		pInven = GET_INVENTORY(ch, GET_TAB(pItemMade->m_itemProto->m_typeIdx, pItemMade->m_itemProto->m_subtypeIdx));
				pInven->FindItem(&nRow, &nCol, nIndexCake, 0, 0);
				pItemPrev = pInven->GetItem(nRow, nCol);
				delete pItemMade;
				pItemMade = NULL;
				ItemUpdateMsg(rmsg, pItemPrev, 1);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemAddMsg(rmsg, pItemMade);
				SEND_Q(rmsg, ch->m_desc);
				pItemMade = NULL;
			}
			EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_OK);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_FAIL);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;

	case MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE:		// 오색 송편 제작
		// 재료 : 송편
		if (!ch->m_invenNormal.FindItem(&nRow, &nCol, nIndexCake, 0, 0))
		{
			EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItemNeed1 = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pItemNeed1->Count() < nNeedCount)
		{
			EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 성공 결정
		if (GetRandom(1, 100) <= nProbRainbow)
		{
			bSuccess = true;

			// 생성 및 지급
			pItemMade = gserver.m_itemProtoList.CreateItem(nIndexRainbow, -1, 0, 0, 1);
			if (!pItemMade)
			{
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			if (!AddToInventory(ch, pItemMade, true, true))
			{
				delete pItemMade;
				pItemMade = NULL;
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_FULLINVEN);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		else
			nNeedCount = 8;

		GAMELOG << init("EVENT CHUSEOK 2006 MAKE RAINBOW CAKE", ch)
				<< "NEED"						 << delim
				<< itemlog(pItemNeed1, true)	 << delim
				<< "CAKE"						 << delim;
		if (bSuccess)
			GAMELOG << itemlog(pItemMade, true);
		else
			GAMELOG << "FAIL";
		GAMELOG << end;

		// 재료 소모
		if (pItemNeed1->Count() == nNeedCount)
		{
			ItemDeleteMsg(rmsg, pItemNeed1);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItemNeed1, true, true);
			pItemNeed1 = NULL;
		}
		else
		{
			DecreaseFromInventory(ch, pItemNeed1, nNeedCount);
			ItemUpdateMsg(rmsg, pItemNeed1, -nNeedCount);
			SEND_Q(rmsg, ch->m_desc);
			pItemNeed1 = NULL;
		}

		// 결과 전송
		if (bSuccess)
		{
			if (pItemMade->tab() < 0)
			{
				CItem*			pItemPrev = NULL;
				CInventory*		pInven = GET_INVENTORY(ch, GET_TAB(pItemMade->m_itemProto->m_typeIdx, pItemMade->m_itemProto->m_subtypeIdx));
				pInven->FindItem(&nRow, &nCol, nIndexRainbow, 0, 0);
				pItemPrev = pInven->GetItem(nRow, nCol);
				delete pItemMade;
				pItemMade = NULL;
				ItemUpdateMsg(rmsg, pItemPrev, 1);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemAddMsg(rmsg, pItemMade);
				SEND_Q(rmsg, ch->m_desc);
				pItemMade = NULL;
			}
			EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_OK);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_FAIL);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;

	case MSG_EVENT_CHUSEOK_2006_GIFT:	// 오색 송편 교환 요청
		// 재료 : 오색 송편
		if (!ch->m_invenNormal.FindItem(&nRow, &nCol, nIndexRainbow, 0, 0))
		{
			EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_GIFT_OK_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItemNeed1 = ch->m_invenNormal.GetItem(nRow, nCol);

		// 보상 결정
		nProbGift = GetRandom(1, 100);
		for (i = 0; i < 5; i++)
		{
			if (nProbGift <= tableGift[i][0])
			{
				// 생성 및 지급
				pItemMade = gserver.m_itemProtoList.CreateItem(tableGift[i][1], -1, 0, 0, tableGift[i][2]);
				if (!pItemMade)
				{
					EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_GIFT_OK_NOITEM);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
				if (!AddToInventory(ch, pItemMade, true, true))
				{
					delete pItemMade;
					pItemMade = NULL;
					EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_GIFT_OK_FULLINVEN);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
				break;
			}
			else
			{
				nProbGift -= tableGift[i][0];
			}
		}

		GAMELOG << init("EVENT CHUSEOK 2006 GIFT", ch)
				<< "NEED"						 << delim
				<< itemlog(pItemNeed1, true)	 << delim
				<< "GIFT"						 << delim
				<< itemlog(pItemMade, true)
				<< end;

		// 재료 소모
		if (pItemNeed1->Count() == 1)
		{
			ItemDeleteMsg(rmsg, pItemNeed1);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItemNeed1, true, true);
			pItemNeed1 = NULL;
		}
		else
		{
			DecreaseFromInventory(ch, pItemNeed1, 1);
			ItemUpdateMsg(rmsg, pItemNeed1, -1);
			SEND_Q(rmsg, ch->m_desc);
			pItemNeed1 = NULL;
		}

		// 결과 전송
		if (pItemMade->tab() < 0)
		{
			CItem*			pItemPrev = NULL;
			CInventory*		pInven = GET_INVENTORY(ch, GET_TAB(pItemMade->m_itemProto->m_typeIdx, pItemMade->m_itemProto->m_subtypeIdx));
			pInven->FindItem(&nRow, &nCol, pItemMade->m_itemProto->m_index, 0, 0);
			pItemPrev = pInven->GetItem(nRow, nCol);
			ItemUpdateMsg(rmsg, pItemPrev, pItemMade->Count());
			SEND_Q(rmsg, ch->m_desc);
			delete pItemMade;
			pItemMade = NULL;
		}
		else
		{
			ItemAddMsg(rmsg, pItemMade);
			SEND_Q(rmsg, ch->m_desc);
			pItemMade = NULL;
		}
		EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_GIFT_OK);
		SEND_Q(rmsg, ch->m_desc);
		break;

	default:
		break;
	}
}
#endif // EVENT_CHUSEOK_2006_GIFT

#ifdef EVENT_XMAS_2006
void do_EventXmas2006(CPC* ch, CNetMsg& msg)
{
	int nCakeCount = 0;
	int nDollCount = 0;
	msg >> nCakeCount;
	nCakeCount = nCakeCount - (nCakeCount % 3);

	if (nCakeCount < 3)
		return ;

	nDollCount = nCakeCount / 3;

	CNetMsg rmsg;

	int nRow;
	int nCol;
	if (!ch->m_invenNormal.FindItem(&nRow, &nCol, 1975, 0, 0))
	{
		EventXmas2006Msg(rmsg, 0, MSG_EVENT_XMAS_2006_ERROR_NOITEM);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	CItem* pItemCake = ch->m_invenNormal.GetItem(nRow, nCol);
	if (!pItemCake || pItemCake->Count() < nCakeCount)
	{
		EventXmas2006Msg(rmsg, 0, MSG_EVENT_XMAS_2006_ERROR_NOITEM);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* pItemDoll = gserver.m_itemProtoList.CreateItem(1976, -1, 0, 0, nDollCount);
	if (!pItemDoll)
	{
		EventXmas2006Msg(rmsg, 0, MSG_EVENT_XMAS_2006_ERROR_FULLINVEN);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (!AddToInventory(ch, pItemDoll, true, true))
	{
		delete pItemDoll;
		EventXmas2006Msg(rmsg, 0, MSG_EVENT_XMAS_2006_ERROR_FULLINVEN);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("EVENT XMAS 2006 GIFT", ch);

	if (pItemDoll->tab() < 0)
	{
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItemDoll->m_itemProto->m_index, 0, 0);
		CItem* pItemPrev = ch->m_invenNormal.GetItem(nRow, nCol);
		ItemUpdateMsg(rmsg, pItemPrev, nDollCount);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << "DOLL" << delim
				<< itemlog(pItemPrev);
		delete pItemDoll;
		pItemDoll = NULL;
	}
	else
	{
		GAMELOG << "DOLL" << delim
				<< itemlog(pItemDoll);
		ItemAddMsg(rmsg, pItemDoll);
		SEND_Q(rmsg, ch->m_desc);
		pItemDoll = NULL;
	}

	if (pItemCake->Count() > nCakeCount)
	{
		DecreaseFromInventory(ch, pItemCake, nCakeCount);
		GAMELOG << "CAKE" << delim
				<< pItemCake->Count();
		ItemUpdateMsg(rmsg, pItemCake, -nCakeCount);
		SEND_Q(rmsg, ch->m_desc);
		pItemCake = NULL;
	}
	else
	{
		GAMELOG << "CAKE" << delim
				<< 0;
		ItemDeleteMsg(rmsg, pItemCake);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, pItemCake, true, true);
		pItemCake = NULL;
	}
	GAMELOG << end;

	EventXmas2006Msg(rmsg, nCakeCount, MSG_EVENT_XMAS_2006_ERROR_OK);
	SEND_Q(rmsg, ch->m_desc);
}
#endif // EVENT_XMAS_2006

#if defined ( EVENT_VALENTINE_2007 ) || defined ( EVENT_VALENTINE_2007_GIFT )
void do_EventValentine2007(CPC* ch, CNetMsg& msg)
{
	int nSubType;
	CNetMsg rmsg;
	msg >> nSubType;
	
	switch(nSubType)
	{
	case MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE:		// 빙고아이템 저장 요청
		{
			unsigned char itempos; 
			int itemindex, bingoitemindex, row, col;
			int flag = 7;

			msg >> itempos
				>> itemindex
				>> bingoitemindex;

			switch(itempos)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				break;
			default:
				return;
			}

			int const itemcount = 3;
#if defined ( EVENT_VALENTINE_2008_ITEM ) 
			static int const itemtable[itemcount][2] = {		// {포장된 아이템, 아이템 비트값}
				{ 2634, 1 },		// 묶인 파란 복주머니
				{ 2635, 2 },		// 묶인 노란 복주머니 
				{ 2636, 4 },		// 묶인 빨강 복주머니
			};
#else 
			static int const itemtable[itemcount][2] = {		// {포장된 아이템, 아이템 비트값}
				{ 2048, 1 },		// 카카오 30
				{ 2049, 2 },		// 카카오 60
				{ 2050, 4 },		// 카카오 90
			};			
#endif

			CItem* pBingoBox;
			CItem* pPackagedItem;

			if(ch->m_invenNormal.FindItem(&row, &col, bingoitemindex, -1, -1))
			{
				pBingoBox = ch->m_invenNormal.GetItem(row, col);
			}
			else
			{
				// 빙고상자 아이템이 없다.
				EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_NOTFOUND);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			if(ch->m_invenNormal.FindItem(&row, &col, itemindex, -1, -1))
			{
				pPackagedItem = ch->m_invenNormal.GetItem(row, col);
			}
			else
			{
				// 빙고상자에 저장할 아이템이 없다.
				//EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_PACKAGE_NOTFOUNT_ITEM);
			//	SEND_Q(rmsg, ch->m_desc);
				return;
			}

			int i;
			for(i = 0 ; i < itemcount; ++i)
			{
				if(itemindex == itemtable[i][0])
				{
					int movebit = 3 * itempos;
					int nArrangeItem = itemtable[i][1] << movebit;

					if(( flag << movebit ) & pBingoBox->m_plus)
					{
						// 이미 배열 되어있는 곳이다.
						EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_ALREADY);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					pBingoBox->m_plus |= nArrangeItem;
					
					int score = 0;
					int bingocount = 0;
					int addscore = 0;

					if(!BINGO(pBingoBox->m_plus, &score, &bingocount))	// 빙고 실패하면 원래대로 되돌린다.
					{
						pBingoBox->m_plus &= ~nArrangeItem;
						return ;
					}

					GAMELOG << init("EVENT VALENTINE 2007 ARRANGE", ch)
							<< "POSITION" << delim
							<< itempos << delim
							<< "ITEM" << delim
							<< itemlog(pPackagedItem) << delim
							<< end;

					addscore = score - pBingoBox->m_used;
					pBingoBox->m_used = score;

					// 저장된 아이템을 인벤에서 지운다.
					DecreaseFromInventory(ch, pPackagedItem, 1);
					if (pPackagedItem->Count() > 0)
					{
						ItemUpdateMsg(rmsg, pPackagedItem, -1);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						ItemDeleteMsg(rmsg, pPackagedItem);
						SEND_Q(rmsg, ch->m_desc);
						RemoveFromInventory(ch, pPackagedItem, true, true);
					}

					// 빙고박스 아이탬 갱신
					ItemUpdateMsg(rmsg, pBingoBox, 0);
					SEND_Q(rmsg, ch->m_desc);

					EventValentine2007Msg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_OK, addscore);
					SEND_Q(rmsg, ch->m_desc);
					break;
				}
			}
			if(i == itemcount)
			{
				// 포장된 초코렛이 아닌 것은 배열할 수 없다. 
				EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_NOTPACKAGE);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_GIFT:				// 보상요청
		{
			int bingoitemindex, row, col;
            CNetMsg rmsg;

            const int giftitemcount = 7;

            // 선물지급되는 확률
#if defined (EVENT_VALENTINE_2009_ITEM)
            static const int giftprobtable[5][giftitemcount] = {
                { 200, 1000, 1000, 1000,   1,  1,  1},
                { 400, 1000, 1000, 1000,  40, 20,  5},
                { 600, 1000, 1000, 1000,  60, 30, 10},
                { 800, 1000, 1000, 1000, 100, 50, 20},
                {1000, 1000, 1000, 1000, 200, 80, 30},
            };
#elif defined (LC_GER) || defined (LC_EUR)
            static const int giftprobtable[5][giftitemcount] = {
                { 200,  200, 100, 100,  200,  200,   1},
                { 400,  400, 200, 200,  400,  400,  10},
                { 600,  600, 400, 400,  600,  600,  30},
                { 800,  800, 600, 600,  800,  800,  60},
                {1000, 1000, 800, 800, 1000, 1000, 100},
            };
#else
            static const int giftprobtable[5][giftitemcount] = {
                {1000, 1000, 1, 1, 200, 200, 1},
                {1000, 1000, 200, 200, 400, 400, 10},
                {1000, 1000, 400, 400, 600, 600, 30},
                {1000, 1000, 600, 600, 800, 800, 60},
                {1000, 1000, 800, 800, 1000, 1000, 100},
            };
#endif


#if defined(LC_JPN)
            static const int giftitemtable[giftitemcount][2] = {        // {선물아이템인덱스, 선물아이템개수}
                {153, 1},
                {158, 1},
				{498, 1},
				{500, 1},
				{838, 1},
				{85, 1},
				{2395, 1},
			};
#elif defined(LC_EUR)
            static const int giftitemtable[giftitemcount][2] = {        // {선물아이템인덱스, 선물아이템개수}
                {971,  2},                                              // 경험의 스크롤
                {973,  2},                                              // 노력의 스크롤
                {1820, 2},                                              // 공격 향상제(대)
                {1819, 2},                                              // 방어 향상제(대)
                {1821, 2},                                              // 공격 향상제(소)
                {1822, 2},                                              // 방어 향상제(소)
                {  85, 1},                                              // 고급 제련석
            };
#elif defined ( EVENT_VALENTINE_2008_ITEM )
			static const int giftitemtable[giftitemcount][2] = {		// {선물아이템인덱스, 선물아이템개수}
				{508, 5},
				{509, 5},
				{2033, 1},
				{2034, 1},
				{2382, 5},
				{1819, 5},
				{779, 1},
            };
#elif defined (EVENT_VALENTINE_2009_ITEM)
            static const int giftitemtable[giftitemcount][2] = {        // {선물아이템인덱스, 선물아이템개수}
                { 971, 5},                                              // 경험의 스크롤
                { 973, 5},                                              // 노력의 스크롤
                {1820, 5},                                              // 공격 향상제(대)
                {1819, 5},                                              // 방어 향상제(대)
                { 884, 3},                                              // 드롭율 증폭제
                {  85, 1},                                              // 고급 제련석
                {2844, 1},                                              // 카오스 제련석
            };
#else
            static const int giftitemtable[giftitemcount][2] = {        // {선물아이템인덱스, 선물아이템개수}
                {971, 5},                                               // 경험의 스크롤
                {973, 5},                                               // 노력의 스크롤
                {1820, 5},                                              // 공격 향상제(대)
                {1819, 5},                                              // 방어 향상제(대)
                {1821, 5},                                              // 공격 향상제(소)
                {1822, 5},                                              // 방어 향상제(소)
                {779, 1},                                               // 초고급 제련석
            };
#endif 

			msg >> bingoitemindex;
			
			CItem *giftitem[giftitemcount];
			memset(giftitem, 0, sizeof(CItem*) * giftitemcount);

			if(ch->m_invenNormal.FindItem(&row, &col, bingoitemindex, -1, -1))
			{
				CItem* pBingoBox;
				pBingoBox = ch->m_invenNormal.GetItem(row, col);
				if(pBingoBox)
				{
					int score = 0;
					int bingocount = 0;
					int i;

					if(!BINGO(pBingoBox->m_plus, &score, &bingocount))
					{
						return;
					}

					// 빙고가 없으면
					if(score == 0)
					{
						// 상품을 줄 수 없다고 한다.
						EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_GIFT_NOITEM);
						SEND_Q(rmsg, ch->m_desc);
						return ;
					}
					
					try
					{
						// 점수에 따라서 상품을 준다.
						// 빙고판 아이템 제거
						int gifttableindex;
						if(score <= 23) gifttableindex = 0;
						else if(score <= 39) gifttableindex = 1;
						else if(score <= 55) gifttableindex = 2;
						else if(score <= 71) gifttableindex = 3;
						else gifttableindex = 4;
						
						int nProb;
						int k;

						// 인벤에 아이템 추가
						for(k = 0; k < giftitemcount ; ++k)
						{
							nProb = GetRandom(1, 1000);
							if(nProb <= giftprobtable[gifttableindex][k])
							{
								int flag = 0;
#ifdef LC_JPN
								if(giftitemtable[k][0] == 498		// 붉은 케르 14Lv
									|| giftitemtable[k][0] == 500)	// 푸른 케르 14Lv								
								{
									flag = 14;
								}
#endif // LC_JPB
								giftitem[k] = gserver.m_itemProtoList.CreateItem(giftitemtable[k][0], -1, 0, flag, giftitemtable[k][1]);
								if(!giftitem[k])
								{
									throw 0;		// 디비에 아이템이 없음
								}
								
								if(!AddToInventory(ch, giftitem[k], false, true))
								{
									delete giftitem[k];
									giftitem[k] = NULL;
									throw 1;		// 인벤에 가득 찼음.
								}
							}
						}

						// 아이템 추가 전송
						for(k = 0 ; k < giftitemcount; ++k)
						{
							if(giftitem[k])
							{
								CNetMsg itemmsg;
								
								if(giftitem[k]->tab() != -1)		// 인벤에 아이템 새롭게 추가
								{
									ItemAddMsg(itemmsg, giftitem[k]);
									GAMELOG << init("EVENT VALENTINE 2007 GIFT ITEM", ch)
											<< itemlog(giftitem[k])
											<< end;

									giftitem[k] = NULL;
								}
								else								// 기존에 있는 아이템에 추가
								{
									int row, col;
									if(ch->m_invenNormal.FindItem(&row, &col, giftitem[k]->m_idNum, giftitem[k]->m_plus, giftitem[k]->m_flag))
									{
										CItem* pItem = ch->m_invenNormal.GetItem(row, col);
										if(pItem)
											ItemUpdateMsg(itemmsg, pItem, giftitemtable[k][1]);
										delete giftitem[k];
										giftitem[k] = NULL;

										GAMELOG << init("EVENT VALENTINE 2007 GIFT ITEM", ch)
												<< "ITEM INDEX" << delim << giftitemtable[k][0] << delim
												<< "ADD COUNT" << delim << giftitemtable[k][1] << delim
												<< itemlog(pItem)
												<< end;
									}
								}

								SEND_Q(itemmsg, ch->m_desc);
							}
						}

						// 빙고박스 아이템을 제거한다.
						ItemDeleteMsg(rmsg, pBingoBox);
						SEND_Q(rmsg, ch->m_desc);
						RemoveFromInventory(ch, pBingoBox, true, true);
						
						// 빙고개수도 같이 전해준다.
						EventValentine2007Msg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_GIFT_OK, bingocount);
						SEND_Q(rmsg, ch->m_desc);
					}
					catch (const int errCode)
					{
						for(i = 0 ; i < giftitemcount; ++i)
						{
							if(giftitem[i])
							{
								if(giftitem[i]->tab() != -1)		// 새롭게 추가한 것
								{
									RemoveFromInventory(ch, giftitem[i], true, false);
								}
								else								// 기존에 있는 거에 추가한 것
								{
									if(ch->m_invenNormal.FindItem(&row, &col, giftitem[i]->m_itemProto->m_index, giftitem[i]->m_plus, giftitem[i]->m_flag))
									{
										CItem* pItem = ch->m_invenNormal.GetItem(row, col);
										if(pItem)
										{
											DecreaseFromInventory(ch, pItem, giftitemtable[i][1]);
										}
									}
								}
							}
						}
						switch(errCode)
						{
						case 0:			// DB에 없는 아이템
							{
								
							}
							break;
						case 1:			// 선물 주는데 인벤이 부족하다.
							{
								EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_PACKAGE_FULLINVEN);
								SEND_Q(rmsg, ch->m_desc);
							}
							break;
						}
					}
				}
			}
			else
			{
				// 빙고판 아이템 없음
				EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_NOTFOUND);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	}
}

bool BINGO(int plus, int* score, int* bingocount)
{
	if (!score || !bingocount)
		return false;

	*score = 0;
	*bingocount = 0;

	int flag = 7;
	static const int scoretable[3] = {1, 2, 10};
	int BingoState[9];
	int i;
	for(i = 0; i < 9; ++i)
	{
		BingoState[i] = ( plus >> (i * 3) ) & flag;

		switch(BingoState[i])
		{
		case 0:
		case 1:
		case 2:
		case 4:
			break;
		default:
			return false;
		}
	}
	
	if(BingoState[0] > 0 && BingoState[0] == BingoState[1] && BingoState[1] == BingoState[2])
	{
		*score += scoretable[BingoState[0]/2];
		(*bingocount)++;
	}
	if(BingoState[3] > 0 && BingoState[3] == BingoState[4] && BingoState[4] == BingoState[5])
	{
		*score += scoretable[BingoState[3]/2];
		(*bingocount)++;
	}
	if(BingoState[6] > 0 && BingoState[6] == BingoState[7] && BingoState[7] == BingoState[8])
	{
		*score += scoretable[BingoState[6]/2];
		(*bingocount)++;
	}
	if(BingoState[0] > 0 && BingoState[0] == BingoState[3] && BingoState[3] == BingoState[6])
	{
		*score += scoretable[BingoState[0]/2];
		(*bingocount)++;
	}
	if(BingoState[1] > 0 && BingoState[1] == BingoState[4] && BingoState[4] == BingoState[7])
	{
		*score += scoretable[BingoState[1]/2];
		(*bingocount)++;
	}
	if(BingoState[2] > 0 && BingoState[2] == BingoState[5] && BingoState[5] == BingoState[8])
	{
		*score += scoretable[BingoState[2]/2];
		(*bingocount)++;
	}
	if(BingoState[0] > 0 && BingoState[0] == BingoState[4] && BingoState[4] == BingoState[8])
	{
		*score += scoretable[BingoState[0]/2];
		(*bingocount)++;
	}
	if(BingoState[2] > 0 && BingoState[2] == BingoState[4] && BingoState[4] == BingoState[6])
	{
		*score += scoretable[BingoState[2]/2];
		(*bingocount)++;
	}

	return true;
}
#endif	// #if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 

#ifdef EVENT_WHITEDAY_2007 
void do_EventWhiteday2007(CPC* ch, CNetMsg& msg)
{
	const int nRepaySkill[6][2] = { 
						{423,250},		// 바람의 러브매직 25%
						{424,150},		// 힘의 러브매직 15%
						{425,150},		// 민첩의 러브매직 15%
						{426,150},		// 지혜의 러브매직 15%
						{427,150},		// 체질의 러브매직 15%
						{428,150},		// 로레인의 러브매직 15%
					};
	int nNeedItemCountSky =0;			// 교환에 필요한 하늘색 캔디 개수 
	int nNeedItemCountPink=0;			// 교환에 필요한 분홍색 캔디 개수 
	int nRepaySkillIndex = 0;
	int nSubType;

	CNetMsg rmsg;

	msg >> nSubType;
	switch(nSubType)
	{
	case MSG_EVENT_WHITEDAY_2007_EXCHANGE_REQ : //
		{
			unsigned char exchangeType;
			msg >> exchangeType;
			//////////////////////////////////////////////////////////////////////////
			switch( exchangeType )
			{
			case 0 :	// 하늘 색 5개  분홍색 5개 (보상 : 스킬 )
				nNeedItemCountSky	= 5;
				nNeedItemCountPink	= 5;
				break;
			case 1:		// 하늘색 20개 (보상 : 하늘색 편지지 )
				nNeedItemCountSky	= 20;
				nNeedItemCountPink	= 0;
				break;
			case 2:		// 분홍색 20개 (보상 : 분홍색 편지지  )
				nNeedItemCountSky	= 0;
				nNeedItemCountPink	= 20;
				break;
			}
			int		nRow, nCol;
			CItem*	pItemSkyCandy	= NULL;
			CItem*	pItemPinkCandy	= NULL;
			
			try
			{
				//////////////////////////////////////////////////////////////////////////
				// 필요 아이템 검색 
#ifdef EVENT_EGGS_HUNT_2008_ITEM
				if (nNeedItemCountSky > 0 )		// 하늘색 달걀 갯수 검사
				{
					if (  !ch->m_invenNormal.FindItem(&nRow, &nCol, 2745 , 0, 0)) // 하늘색 달걀
					{
						throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM;	// 아이템 없음
					}
					pItemSkyCandy = ch->m_invenNormal.GetItem(nRow, nCol);
					if( pItemSkyCandy != NULL && pItemSkyCandy->Count() < nNeedItemCountSky )
					{							
						throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM;
					}
				}

				if ( nNeedItemCountPink > 0 )	// 분홍색 달걀 개수 검사
				{
					if ( !ch->m_invenNormal.FindItem(&nRow, &nCol, 2746 , 0, 0)) // 분홍색 달걀
					{
						throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM ;
					}
					pItemPinkCandy = ch->m_invenNormal.GetItem(nRow, nCol);
					if( pItemPinkCandy != NULL && pItemPinkCandy->Count() < nNeedItemCountPink )
					{
						throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM;
					}
				}
#else
				if (nNeedItemCountSky > 0 )		// 하늘색 사탕 갯수 검사
				{
					if (  !ch->m_invenNormal.FindItem(&nRow, &nCol, 2133 , 0, 0)) // 하늘색 사탕 찾기
					{
						throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM;	// 아이템 없음
					}
					pItemSkyCandy = ch->m_invenNormal.GetItem(nRow, nCol);
					if( pItemSkyCandy != NULL && pItemSkyCandy->Count() < nNeedItemCountSky )
					{							
						throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM;
					}
				}

				if ( nNeedItemCountPink > 0 )	// 분홍색 사탕 개수 검사
				{
					if ( !ch->m_invenNormal.FindItem(&nRow, &nCol, 2134 , 0, 0)) // 분홍색 사탕 찾기
					{
						throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM ;
					}
					pItemPinkCandy = ch->m_invenNormal.GetItem(nRow, nCol);
					if( pItemPinkCandy != NULL && pItemPinkCandy->Count() < nNeedItemCountPink )
					{
						throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM;
					}
				}
#endif // EVENT_EGGS_HUNT_2008_ITEM

				//////////////////////////////////////////////////////////////////////////
				// 보상
				switch( exchangeType )
				{
				case 0 : // 스킬 보상
					{
						int nRand = GetRandom(0, 1000);
						for( nRepaySkillIndex=0; nRepaySkillIndex<6 ; nRepaySkillIndex++ )
						{
							if( nRand <= nRepaySkill[nRepaySkillIndex][1] )
							{			
								CSkill* skill = new CSkill( gserver.m_skillProtoList.Find(nRepaySkill[nRepaySkillIndex][0]), 1 );		//test 423
								bool bApply;
								ApplySkill(ch, ch, skill, -1, bApply);
								delete skill;

								if( bApply == false )
								{
									throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_FAILED;
								}
								break;
							}
							nRand -= nRepaySkill[nRepaySkillIndex][1];
						}
					}
					break;
				case 1 :		// 아이템 보상 
				case 2 :
					{	
						CItem *pRepayItem = NULL;
						int		nItemIdx = 0;
						int		r, c;

						if( exchangeType == 1 )
						{ nItemIdx = 2135;	}				// 하늘색 편지지  2135
						else if( exchangeType == 2 )
						{ nItemIdx = 2136;	}				// 분홍색 편지지  2136

						if(ch->m_invenNormal.FindItem(&r, &c, nItemIdx, 0, 0))
						{
							CItem*  item = ch->m_invenNormal.GetItem(r, c);
							if(item && item->Count() >= 5)
							{
								throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_MAXITEM;
							}
						}

						pRepayItem = gserver.m_itemProtoList.CreateItem( nItemIdx , -1, 0, 0, 1 );		
						if (!pRepayItem)
						{
							throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_FAILED;		// 보상이이템 생성 실패
						}

						if (!AddToInventory(ch, pRepayItem, true, true))
						{
							delete pRepayItem;
							pRepayItem = NULL;
							throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_FULLINVEN;		//인벤 가득 참
						}

						GAMELOG << init("WHITE DAY EVENT ITEM [LETTER]", ch)
							<< "EXCHANGE"					 << delim
							<< itemlog(pRepayItem, true);

						// 결과 전송
						if (pRepayItem->tab() < 0)
						{
							CItem*			pItemPrev = NULL;
							CInventory*		pInven = GET_INVENTORY(ch, GET_TAB(pRepayItem->m_itemProto->m_typeIdx, pRepayItem->m_itemProto->m_subtypeIdx));
							pInven->FindItem(&nRow, &nCol, pRepayItem->m_itemProto->m_index, 0, 0);
							pItemPrev = pInven->GetItem(nRow, nCol);
							ItemUpdateMsg(rmsg, pItemPrev, pRepayItem->Count());
							SEND_Q(rmsg, ch->m_desc);
							delete pRepayItem;
							pRepayItem = NULL;
						}
						else
						{
							ItemAddMsg(rmsg, pRepayItem);
							SEND_Q(rmsg, ch->m_desc);
							pRepayItem = NULL;
						}
					}
					break;
				}
		
				//////////////////////////////////////////////////////////////////////////
				// 성공 메세지ㄹ
				EventWhiteday2007Msg(rmsg, MSG_EVENT_WHITEDAY_2007_EXCHANGE_OK );
				rmsg << exchangeType;		// 0-스킬 보상, 1-하늘색 편지지 보상 , 2- 분홍색 편지지 보상
				if( exchangeType == 0 )
				{ rmsg << nRepaySkill[nRepaySkillIndex][0];	}	// 스킬 보상일경우 스킬 인덱스
				SEND_Q(rmsg, ch->m_desc);

				//////////////////////////////////////////////////////////////////////////
				// 인벤에서 아이템 제거
				if( pItemSkyCandy!=NULL)		// 하늘 색 사탕
				{
					if(  nNeedItemCountSky<pItemSkyCandy->Count() )
					{
						DecreaseFromInventory(ch, pItemSkyCandy, nNeedItemCountSky);
						ItemUpdateMsg(rmsg, pItemSkyCandy, -nNeedItemCountSky);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						ItemDeleteMsg(rmsg, pItemSkyCandy);
						SEND_Q(rmsg, ch->m_desc);
						ch->RemoveItemFromQuickSlot(pItemSkyCandy);
						RemoveFromInventory(ch, pItemSkyCandy, true, true);
					}
				}
				
				if( pItemPinkCandy!=NULL )		// 분홍색 사탕
				{
					if( nNeedItemCountPink<pItemPinkCandy->Count() )
					{
						DecreaseFromInventory(ch, pItemPinkCandy, nNeedItemCountPink);
						ItemUpdateMsg(rmsg, pItemPinkCandy, -nNeedItemCountPink);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						ItemDeleteMsg(rmsg, pItemPinkCandy);
						SEND_Q(rmsg, ch->m_desc);
						ch->RemoveItemFromQuickSlot(pItemPinkCandy);
						RemoveFromInventory(ch, pItemPinkCandy, true, true);
					}
				}			
			}
			catch (MSG_EVENT_WHITEDAY_2007_TYPE error) 
			{
				EventWhiteday2007Msg(rmsg, error );
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}
		break; // MSG_EVENT_WHITEDAY_2007_EXCHANGE_REQ

	case MSG_EVENT_WHITEDAY_2007_LETTER_REQ :	// Spred Message
		{
			int color;
			int nRow, nCol;
			int nItemIndex =0 ;
			CItem* pNeedItem = NULL;


			CLCString	msgStr(256);		//클라이언트 50자
			CLCString	rmsgStr(256);
			msg >> nItemIndex;
			msg >> color;
			msg >> msgStr;

			if (  !ch->m_invenNormal.FindItem(&nRow, &nCol, nItemIndex , 0, 0)) // 하늘색 사탕 찾기
			{
				EventWhiteday2007Msg(rmsg, MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM );
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}	
			pNeedItem = ch->m_invenNormal.GetItem(nRow, nCol);	

			if( pNeedItem != NULL )
			{
				rmsgStr.Format( "%s> %s", ch->GetName(), (const char*)msgStr );
				EventWhiteday2007Msg(rmsg, MSG_EVENT_WHITEDAY_2007_LETTER_REP );
				rmsg << color
					 << (const char*)rmsgStr;
				gserver.m_playerList.SendToAll( rmsg );	

				GAMELOG << init("WHITE DAY EVENT ITEM [LETTER]", ch)
						<< "USE"			 << delim
						<< itemlog(pNeedItem, true);

				if ( pNeedItem->Count() > 1)
				{
					DecreaseFromInventory(ch, pNeedItem, 1);
					ItemUpdateMsg(rmsg, pNeedItem, -1);
					SEND_Q(rmsg, ch->m_desc);
				}
				else
				{
					ItemDeleteMsg(rmsg, pNeedItem);
					SEND_Q(rmsg, ch->m_desc);
					ch->RemoveItemFromQuickSlot(pNeedItem);
					RemoveFromInventory(ch, pNeedItem, true, true);
				}
			}
		
		}
		break; // MSG_EVENT_WHITEDAY_2007_LETTER_REQ
	}

}
#endif // EVENT_WHITEDAY_2007 [3/6/2007 KwonYongDae] 

#ifdef EVENT_2007_PARENTSDAY
void do_EventParentsday2007( CPC* ch, CNetMsg& msg )
{
	int nSubType;

	CNetMsg rmsg;

	msg >> nSubType;

	if( nSubType == MSG_EVENT_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST )
	{
		HelperEventParentsdayAccumulateList( rmsg, ch->m_index );
		SEND_Q( rmsg, gserver.m_helper );
		return;
	}

	CGuildMember * guildInfo = ch->m_guildInfo;
	if( !guildInfo )
	{
		CNetMsg errMsg;
		GuildErrorMsg(errMsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(errMsg, ch->m_desc);
		return;
	}
//	CGuild*		guild = ch->m_guildInfo->guild();
//	if( !guild )
//	{
//		CNetMsg errMsg;
//		GuildErrorMsg(errMsg, MSG_GUILD_ERROR_NOGUILD);
//		SEND_Q(errMsg, ch->m_desc);
//		return;
//	}

	CItem*		pItems;

	switch( nSubType )
	{
//		case	MSG_EVENT_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST:
//		// 헬퍼 서버로 누적 포인트 RankingList 요청
//				HelperEventParentsdayAccumulateList( rmsg, ch->m_index );
//				SEND_Q( rmsg, gserver.m_helper );
//				break;
		case	MSG_EVENT_PARENTSDAY_2007_ADD_POINT:			
			{
				// 헬퍼 서버로 포인트 누적 요청
				int nRow, nCol;
				int RedCarnation = 2331;

				if ( ( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )  && ( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_OFFICER ) )
				{
					EventParentsdayAddPoint( rmsg, MSG_ERROR_PARENTSDAY_2007_NOT_GUILD_OWNER, 0, 0 );
					SEND_Q( rmsg, ch->m_desc )
					return;
				}

				if (!ch->m_invenNormal.FindItem(&nRow, &nCol, RedCarnation, 0, 0) )
				{
					EventParentsdayAddPoint(rmsg, MSG_ERROR_PARENTSDAY_2007_ERROR_NOITEM, 0, 0 );
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
				pItems = ch->m_invenNormal.GetItem(nRow, nCol);

				HelperEventParentsdayAddPoint( rmsg, ch->m_index, guildInfo->guild()->index(), pItems->Count() );
				SEND_Q( rmsg, gserver.m_helper );
			}
			break;

		case	MSG_EVENT_PARENTSDAY_2007_EXCHANGE_TICKET:
			// 누적된 포인트로 교환권 요청
			if ( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS  )
			{
				EventParentsdayExchangeTicket( rmsg, MSG_ERROR_PARENTSDAY_2007_NOT_GUILD_OWNER, 0 );
				SEND_Q( rmsg, ch->m_desc )
				return;
			}

			HelperEventParentsdayExchangeTicket( rmsg, ch->m_index, guildInfo->guild()->index() );
			SEND_Q( rmsg, gserver.m_helper );
			break;
		case	MSG_EVENT_PARENTSDAY_2007_EXCHANGE_ITEM:
			{
				// 아이템 교환 요청
				int ExchagneTicket = 2349;
				int nRow, nCol;
				if ( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS  )
				{
					EventParentsdayExchangeItem( rmsg, MSG_ERROR_PARENTSDAY_2007_NOT_GUILD_OWNER );
					SEND_Q( rmsg, ch->m_desc )
					return;
				}

				if (!ch->m_invenNormal.FindItem(&nRow, &nCol, ExchagneTicket, 0, 0) )
				{
					EventParentsdayExchangeItem(rmsg, MSG_ERROR_PARENTSDAY_2007_ERROR_NOITEM);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}

				pItems = ch->m_invenNormal.GetItem(nRow, nCol);

				HelperEventParentsdayExchangeItem( rmsg, ch->m_index, guildInfo->guild()->index() );
				SEND_Q( rmsg, gserver.m_helper );
			}
			break;
	}
}
#endif // EVENT_2007_PARENTSDAY


#ifdef EVENT_TEACH_2007
void do_ExEventTeach2007(CPC* ch, CNetMsg& msg)
{	
	CNetMsg rmsg;
	int value = 0;
	
	int row = 0, col = 0;
	if (ch->m_invenEvent.FindItem(&row, &col, 2329, 0, 0) == true)
	{
		value = 1;
		// 견장 삭제
		CItem * pItem = ch->m_invenEvent.GetItem(row, col);
		if (pItem == NULL)
		{
			return;
		}

		ItemDeleteMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);

		if (RemoveFromInventory(ch, pItem, true, true) == false)
		{
			return;
		}				

		// 25레벨 제련석 10개 추가
		if (ch->GiveItem(84, 0, 25, 10, true) == true)
		{
			GAMELOG << init("EVENT_TEACH_2007", ch)
				<< " GIVE STONE AND DELETE STRAP";
		}

		ch->SaveNow(false);
	}
	else if (ch->m_invenEvent.FindItem(&row, &col, 2330, 0, 0) == true)
	{
		// 아이템 확인
		CItem * pItem = ch->m_invenEvent.GetItem(row, col);
		if (pItem == NULL)
		{
			return;
		}

		// 명성치 증가
		ch->m_fame += pItem->Count() * 10;


		// 헬퍼 명성치 증가 전송 
		CNetMsg FameMsg;
		HelperTeachFameUp2007ReqMsg(FameMsg, ch->m_index, pItem->Count() * 10);
		SEND_Q(FameMsg, gserver.m_helper);

		value = ch->m_fame;
		GAMELOG << init("EVENT_TEACH_2007", ch)
				<< " GIVE FAME POINT " << delim << ch->m_fame;
		
		// 노란 카네이션 삭제
		ItemDeleteMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);

		if (RemoveFromInventory(ch, pItem, true, true) == false)
		{
			return;
		}		
	}
	
	if (value != 0)
	{
		EventTeach2007Msg(rmsg, value);
		SEND_Q(rmsg, ch->m_desc);
	}	
}
#endif // EVENT_TEACH_2007

#ifdef EVENT_GOMDORI_2007
void do_EventGomdori2007(CPC* ch, CNetMsg& msg)
{
	// 승패 확률 그외는 비김, 승리-0, 패배-1
	const static int nTableWinProb[8][2] = {
		{34, 67},		// 0승 -> 1승
		{34, 67},		// 1승 -> 2승
		{34, 67},		// 2승 -> 3승
		{30, 70},		// 3승 -> 4승
		{30, 70},		// 4승 -> 5승
		{30, 70},		// 5승 -> 6승
		{30, 65},		// 6승 -> 7승
		{30, 65},		// 7승 -> 8승
	};

	const static int nTableGomdoriItem[9] = {
		2335,
		2336,
		2337,
		2338,
		2339,
		2340,
		2341,
		2342,
		2343
	};
#if defined (LC_USA)
	const static int nTableWearSize = 4;
	const static int nTableWear[JOBCOUNT][nTableWearSize] = {
		{1422, 1423, 1424, 1425},		// 타이탄
		{1426, 1427, 1428, 1429},		// 나이트
		{1430, 1431, 1432, 1433},		// 힐러
		{1434, 1435, 1436, 1437},		// 메이지
		{1438, 1439, 1440, 1441},		// 로그
		{1442, 1443, 1444, 1445},		// 소서러
#ifdef NIGHT_SHADOW
		{1426, 1427, 1428, 1429},		// 나이트
#endif //NIGHT_SHADOW
	};

	const static int nTableWeaponSize = 2;
	const static int nTableWeapon[JOBCOUNT][nTableWeaponSize] = {
		{1470, 1471},		// 타이탄
		{1473, 1472},		// 나이트
		{1474, 1475},		// 힐러
		{1476, 1477},		// 메이지
		{1478, 1479},		// 로그
		{1480, 1481},		// 소서러
#ifdef NIGHT_SHADOW
		{1473, 1472},		// 나이트
#endif //NIGHT_SHADOW
	};
#elif defined (LC_HBK)
	const static int nTableWearSize = 4;
	const static int nTableWear[JOBCOUNT][nTableWearSize] = {
		{1349, 1350, 1351, 1352},		// 타이탄
		{1343, 1344, 1345, 1346},		// 나이트
		{1354, 1355, 1356, 1357},		// 힐러
		{1359, 1360, 1361, 1362},		// 메이지
		{1364, 1365, 1366, 1367},		// 로그
		{1374, 1375, 1376, 1377}		// 소서러
#ifdef NIGHT_SHADOW
		{1343, 1344, 1345, 1346},		// 나이트
#endif //NIGHT_SHADOW
	};

	const static int nTableWeaponSize = 2;
	const static int nTableWeapon[JOBCOUNT][nTableWeaponSize] = {
		{1333, 1334},		// 타이탄
		{1331, 1332},		// 나이트
		{1335, 1336},		// 힐러
		{1338, 1337},		// 메이지
		{1339, 1340},		// 로그
		{1341, 1342},		// 소서러
#ifdef NIGHT_SHADOW
		{1331, 1332},		// 나이트
#endif //NIGHT_SHADOW
	};
#elif defined (LC_JPN)
#elif defined (LC_BRZ)
#else
	const static int nTableWearSize = 4;
	const static int nTableWear[JOBCOUNT][nTableWearSize] = {
		{2151, 2152, 2153, 2154},		// 타이탄
		{2155, 2156, 2157, 2158},		// 나이트
		{2159, 2160, 2161, 2162},		// 힐러
		{2163, 2164, 2165, 2166},		// 메이지
		{2167, 2168, 2169, 2170},		// 로그
		{2171, 2172, 2149, 2150}		// 소서러
#ifdef NIGHT_SHADOW
		{2155, 2156, 2157, 2158},		// 나이트
#endif //NIGHT_SHADOW
	};

	const static int nTableWeaponSize = 2;
	const static int nTableWeapon[JOBCOUNT][nTableWeaponSize] = {
		{2173, 2174},		// 타이탄
		{2175, 2176},		// 나이트
		{2177, 2178},		// 힐러
		{2179, 2180},		// 메이지
		{2181, 2182},		// 로그
		{2183, 2184},		// 소서러
#ifdef NIGHT_SHADOW
		{2175, 2176},		// 나이트
#endif //NIGHT_SHADOW
	};
#endif // LC_USA
	
	
#ifdef LC_HBK
	const static int nTableGiftSize = 9;
	const static int nTableGift[nTableGiftSize][2] = {
		{ 46, 10},
		{ 45, 10},
		{ 724, 20},
		{ 492, 10},
		{ 495, 20},
		{ 723, 10},
		{ 786, 10},
		{  85, 5},
		{  85, 10}
	};
#elif defined(LC_JPN)
	const static int nTableGiftSize = 9;
	const static int nTableGift[nTableGiftSize][2] = {
		{ 457, 10},
		{ 284, 10},
		{ 1298, 1},
		{ 854, 10},
		{ 853, 10},
		{ 852, 10},
		{ 1299, 1},
		{ 85, 1},
		{ 85, 2}
	};
#elif defined (LC_BRZ)
	const static int nTableGiftSize = 8;
	const static int nTableGift[nTableGiftSize][2] = {
		{ 723, 5},
		{ 19, 200000},
		{ 19, 500000},
		{ 19, 1000000},
		{ 85, 1},
		{ 85, 2},
		{ 85, 3},
		{ 85, 4}
	};
#else
	const static int nTableGiftSize = 9;
	const static int nTableGift[nTableGiftSize][2] = {
		{ 855, 1},
		{ 856, 5},
		{ 855, 5},
		{ 852, 5},
		{1420, 1},
		{1420, 2},
		{  85, 1},
		{  85, 2},
		{  85, 3}
	};
#endif //

	CNetMsg					rmsg;
	unsigned char			cSubType;
	unsigned char			cUserSel;
	unsigned char			cNPCSelWin;
	unsigned char			cNPCSelLose;
	int						i;
	int						nWinProb;
#if !defined (LC_JPN) && !defined (LC_BRZ)
	int						nSelItem;
#endif
	CItem*					pItems[9];

	msg >> cSubType;

	switch (cSubType)
	{
	case MSG_EVENT_GOMDORI_2007_CHECKITEM:
		for (i = 0; i < 9; i++)
		{
			int nRow, nCol;
			if (!ch->m_invenNormal.FindItem(&nRow, &nCol, nTableGomdoriItem[i], 0, 0))
			{
				EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_CHECKITEM);
		SEND_Q(rmsg, ch->m_desc);
		break;

	case MSG_EVENT_GOMDORI_2007_START:
		if (ch->m_bEventGomdori2007Start || ch->m_nEventGomdori2007Win > 0)
			return ;
		for (i = 0; i < 9; i++)
		{
			int nRow, nCol;
			if (!ch->m_invenNormal.FindItem(&nRow, &nCol, nTableGomdoriItem[i], 0, 0))
			{
				EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			pItems[i] = ch->m_invenNormal.GetItem(nRow, nCol);
		}
		for (i = 0; i < 9; i++)
		{
			if (pItems[i]->Count() > 1)
			{
				DecreaseFromInventory(ch, pItems[i], 1);
				ItemUpdateMsg(rmsg, pItems[i], -1);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, pItems[i]);
				SEND_Q(rmsg, ch->m_desc);
				RemoveFromInventory(ch, pItems[i], true, true);
			}
		}
		ch->m_bEventGomdori2007Start = true;
		ch->m_nEventGomdori2007Win = 0;
		ch->m_nEventGomdori2007Game = 1;
		ch->m_nEventGomdori2007Total++;
		ch->m_bEventGomdori2007CanSelect = true;
		EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_START);
		SEND_Q(rmsg, ch->m_desc);
		if (IS_RUNNING_CONN)
		{
			ConnEventGomdori2007CountMsg(rmsg);
			SEND_Q(rmsg, gserver.m_connector);
		}
		GAMELOG << init("EVENT GOMDORI GAME START", ch) << end;
		break;

	case MSG_EVENT_GOMDORI_2007_SELECT:
#ifdef LC_BRZ
		if (!ch->m_bEventGomdori2007Start || ch->m_nEventGomdori2007Win >= nTableGiftSize || !ch->m_bEventGomdori2007CanSelect)
#else
		if (!ch->m_bEventGomdori2007Start || ch->m_nEventGomdori2007Win >= nTableGiftSize - 1 || !ch->m_bEventGomdori2007CanSelect)
#endif // 
			return ;
		msg >> cUserSel;
		switch (cUserSel)
		{
		case 0:	cNPCSelWin  = 1;	cNPCSelLose = 2;	break;		// 가위
		case 1:	cNPCSelWin  = 2;	cNPCSelLose = 0;	break;		// 바위
		case 2:	cNPCSelWin  = 0;	cNPCSelLose = 1;	break;		// 보
		default:										return ;
		}
		ch->m_bEventGomdori2007CanSelect = false;
		nWinProb = GetRandom(1, 100);
#ifdef _DEBUG
		switch (cUserSel)
		{
		case 0:	nWinProb = 20;	break;		// 가위
		case 1:	nWinProb = 60;	break;		// 바위
		case 2:	nWinProb = 80;	break;		// 보
		}
#endif // _DEBUG
		if (nTableWinProb[(int)ch->m_nEventGomdori2007Win][0] >= nWinProb)
		{
			// 유저 승리
			ch->m_nEventGomdori2007Win++;
			GAMELOG << init("EVENT GOMDORI GAME RESULT WIN", ch)
					<< "USER SEL" << delim
					<< cUserSel << delim
					<< "WIN TOTAL" << delim
					<< ch->m_nEventGomdori2007Win
					<< end;
			EventGomdori2007ResultMsg(rmsg, MSG_EVENT_GOMDORI_2007_RESULT_WIN, ch->m_nEventGomdori2007Win, cUserSel, cNPCSelLose);
			SEND_Q(rmsg, ch->m_desc);
		}
		else if (nTableWinProb[(int)ch->m_nEventGomdori2007Win][1] >= nWinProb)
		{
			// NPC 승리
			ch->m_bEventGomdori2007Start = false;
			GAMELOG << init("EVENT GOMDORI GAME RESULT LOSE", ch)
					<< "USER SEL" << delim
					<< cUserSel << delim
					<< "WIN TOTAL" << delim
					<< ch->m_nEventGomdori2007Win
					<< end;
			EventGomdori2007ResultMsg(rmsg, MSG_EVENT_GOMDORI_2007_RESULT_LOSE, ch->m_nEventGomdori2007Win, cUserSel, cNPCSelWin);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			// 비김
			GAMELOG << init("EVENT GOMDORI GAME RESULT DRAW", ch)
					<< "USER SEL" << delim
					<< cUserSel << delim
					<< "WIN TOTAL" << delim
					<< ch->m_nEventGomdori2007Win
					<< end;
			EventGomdori2007ResultMsg(rmsg, MSG_EVENT_GOMDORI_2007_RESULT_DRAW, ch->m_nEventGomdori2007Win, cUserSel, cUserSel);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;

	case MSG_EVENT_GOMDORI_2007_CONTINUE:
		if (!ch->m_bEventGomdori2007Start)
			return ;
		EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_CONTINUE);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("EVENT GOMDORI GAME CONTINUE", ch) << end;
		ch->m_nEventGomdori2007Game++;
		ch->m_bEventGomdori2007CanSelect = true;
		break;

	case MSG_EVENT_GOMDORI_2007_END:
		{
		if (ch->m_nEventGomdori2007Game < 1)
			return ;
		
#ifndef LC_BRZ
		if (ch->m_nEventGomdori2007Win == 0 && ch->m_nEventGomdori2007FirstLose == 1)
		{
#ifdef LC_JPN
			// 지력의 물약 (2143) 1개
			CItem * pItem = gserver.m_itemProtoList.CreateItem(2143, -1, 0, 0, 1);
			if (pItem != NULL)
			{
				// 무게 검사, 인벤 공간 검사
				if (ch->m_weight + pItem->m_itemProto->m_weight >= ch->m_maxWeight * 15 /10 
					|| ch->m_invenNormal.GetSpace() < 1)
				{
					EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
					SEND_Q(rmsg, ch->m_desc);

					delete pItem;
					return;
				}

				// 아이템 지급
				if (AddToInventory(ch, pItem, true, true))
				{
					// 로그 남기고
					GAMELOG << init("EVENT GOMDORI 2007 GIVE LOSE", ch);
					GAMELOG << itemlog(pItem) << delim;
					
					if (pItem->tab() < 0)
					{
						int nRow, nCol;
						ch->m_invenNormal.FindItem(&nRow, &nCol, 2143, 0, 0);
						CItem * pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
						ItemUpdateMsg(rmsg, pPrevItem, 1);
						SEND_Q(rmsg, ch->m_desc);
						GAMELOG << "ITEM" << delim
								<< itemlog(pPrevItem)
								<< end;
						delete pItem;
						pItem = NULL;
					}
					else
					{
						ItemAddMsg(rmsg, pItem);
						SEND_Q(rmsg, ch->m_desc);
						GAMELOG << "ITEM" << delim
								<< itemlog(pItem)
								<< end;
						pItem = NULL;
					}
				}
				else
				{
					GAMELOG << init("EVENT GOMDORI 2007 GIVE WEAR ERROR", ch)
							<< itemlog(pItem)
							<< end;
					return;
				}			
			
				// 알림
				EventGomdori2007EndMsg(rmsg, ch->m_nEventGomdori2007Win, 1, 0, 0);
				SEND_Q(rmsg, ch->m_desc);

				ch->m_nEventGomdori2007FirstLose = 0;
			}			
#else
			// 체험 교복 세트 보상

			// 만들고
			int nNeedWeight = 0;
			CItem* pItems[nTableWearSize] = {NULL, NULL, NULL, NULL};
			for (i = 0; i < nTableWearSize; i++)
			{
				pItems[i] = gserver.m_itemProtoList.CreateItem(nTableWear[(int)ch->m_job][i], -1, 0, 0, 1);
				if (pItems[i] == NULL)
					break;
				nNeedWeight = pItems[i]->m_itemProto->m_weight;
			}
			if (i < nTableWearSize || ch->m_weight + nNeedWeight >= ch->m_maxWeight * 15 / 10 || ch->m_invenNormal.GetSpace() < nTableWearSize)
			{
				EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
				SEND_Q(rmsg, ch->m_desc);
				for (i = 0; i < nTableWearSize; i++)
				{
					if (pItems[i])
					{
						delete pItems[i];
						pItems[i] = NULL;
					}
				}
				return ;
			}

			// 주고
			for (i = 0; i < nTableWearSize; i++)
			{
				if (AddToInventory(ch, pItems[i], true, true))
				{
					ItemAddMsg(rmsg, pItems[i]);
					SEND_Q(rmsg, ch->m_desc);
				}
				else
				{
					GAMELOG << init("EVENT GOMDORI 2007 GIVE WEAR ERROR", ch)
							<< itemlog(pItems[i])
							<< end;
				}
			}

			// 로그 남기고
			GAMELOG << init("EVENT GOMDORI 2007 GIVE WEAR", ch);
			for (i = 0; i < nTableWearSize; i++)
			{
				GAMELOG << itemlog(pItems[i]) << delim;
				if (pItems[i]->tab() < 0)
					delete pItems[i];
			}

			// 알림
			EventGomdori2007EndMsg(rmsg, ch->m_nEventGomdori2007Win, 1, 0, 0);
			SEND_Q(rmsg, ch->m_desc);

			ch->m_nEventGomdori2007FirstLose = 0;
#endif // LC_JPN
		}
		else if (ch->m_nEventGomdori2007Win == 1 && ch->m_nEventGomdori2007FirstWin == 1)
		{
#ifdef LC_JPN
			// 건강의 물약 (2142) 1개
			CItem * pItem = gserver.m_itemProtoList.CreateItem(2142, -1, 0, 0, 1);
			if (pItem != NULL)
			{
				// 무게 검사, 인벤 공간 검사
				if (ch->m_weight + pItem->m_itemProto->m_weight >= ch->m_maxWeight * 15 /10 
					|| ch->m_invenNormal.GetSpace() < 1)
				{
					EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}

				// 아이템 지급
				if (AddToInventory(ch, pItem, true, true))
				{
					// 로그 남기고
					GAMELOG << init("EVENT GOMDORI 2007 GIVE FIRST WIN", ch);
					GAMELOG << itemlog(pItem) << delim;

					if (pItem->tab() < 0)
					{
						int nRow, nCol;
						ch->m_invenNormal.FindItem(&nRow, &nCol, 2142, 0, 0);
						CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
						ItemUpdateMsg(rmsg, pPrevItem, 1);
						SEND_Q(rmsg, ch->m_desc);
						GAMELOG << "ITEM" << delim
								<< itemlog(pPrevItem)
								<< end;
						delete pItem;
						pItem = NULL;
					}
					else
					{
						ItemAddMsg(rmsg, pItem);
						SEND_Q(rmsg, ch->m_desc);
						GAMELOG << "ITEM" << delim
								<< itemlog(pItem)
								<< end;
						pItem = NULL;
					}
				}
				else
				{
					delete pItem;
					pItem = NULL;
					EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}

				// 알림
				EventGomdori2007EndMsg(rmsg, ch->m_nEventGomdori2007Win, 1, 0, 0);
				SEND_Q(rmsg, ch->m_desc);

				// 초기화
				ch->m_nEventGomdori2007Win = 0;
				ch->m_nEventGomdori2007Game = 0;
				ch->m_nEventGomdori2007FirstWin = 0;
				ch->m_bEventGomdori2007Start = false;

				return;
			}	
			else
			{
				EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
#else
			// 엽기 무기 보상 묻기
			// 암것도 안하고 물어보기
			EventGomdori2007EndMsg(rmsg, ch->m_nEventGomdori2007Win, 2, nTableWeapon[(int)ch->m_job][0], nTableWeapon[(int)ch->m_job][1]);
			SEND_Q(rmsg, ch->m_desc);
			return ;
#endif // LC_JPN
		}
		else
		{
#endif // #ifdef (LC_BRZ)
		

#if defined (LC_BRZ)
			if(ch->m_nEventGomdori2007Win < 1)
			{
				EventGomdori2007EndMsg(rmsg, ch->m_nEventGomdori2007Win, 3, 0, 0);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			int nGiftIndex = nTableGift[(int)(ch->m_nEventGomdori2007Win - 1)][0];
			int nGiftCount = nTableGift[(int)(ch->m_nEventGomdori2007Win - 1)][1];
#else
			// 일반 보상
			int nGiftIndex = nTableGift[(int)ch->m_nEventGomdori2007Win][0];
			int nGiftCount = nTableGift[(int)ch->m_nEventGomdori2007Win][1];
#endif // defined (LC_BRZ)
			
			CItem* pItem = gserver.m_itemProtoList.CreateItem(nGiftIndex, -1, 0, 0, nGiftCount);
			if (pItem)
			{
				if (AddToInventory(ch, pItem, true, true))
				{
					GAMELOG << init("EVENT GOMDORI 2007 GIVE ITEM", ch)
							<< "WIN" << delim
							<< ch->m_nEventGomdori2007Win << delim
							<< "GIFT" << delim
							<< itemlog(pItem) << delim;
					if (pItem->tab() < 0)
					{
						int nRow, nCol;
						ch->m_invenNormal.FindItem(&nRow, &nCol, nGiftIndex, 0, 0);
						CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
						ItemUpdateMsg(rmsg, pPrevItem, nGiftCount);
						SEND_Q(rmsg, ch->m_desc);
						GAMELOG << "ITEM" << delim
								<< itemlog(pPrevItem)
								<< end;
						delete pItem;
						pItem = NULL;
					}
					else
					{
						ItemAddMsg(rmsg, pItem);
						SEND_Q(rmsg, ch->m_desc);
						GAMELOG << "ITEM" << delim
								<< itemlog(pItem)
								<< end;
						pItem = NULL;
					}

					EventGomdori2007EndMsg(rmsg, ch->m_nEventGomdori2007Win, 0, nGiftIndex, nGiftCount);
					SEND_Q(rmsg, ch->m_desc);
				}
				else
				{
					delete pItem;
					pItem = NULL;
					EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
			else
			{
				EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			
			
#ifndef LC_BRZ
		}
#endif // #ifndef LC_BRZ

		// 초기화
		ch->m_nEventGomdori2007Win = 0;
		ch->m_nEventGomdori2007Game = 0;
		ch->m_bEventGomdori2007Start = false;
		}
		break;

#if !defined (LC_JPN) && !defined (LC_BRZ)
	case MSG_EVENT_GOMDORI_2007_SELECT_GIFT:
		msg >> nSelItem;
		if (nTableWeapon[(int)ch->m_job][0] == nSelItem || nTableWeapon[(int)ch->m_job][1] == nSelItem)
		{
			CItem* pItem = gserver.m_itemProtoList.CreateItem(nSelItem, -1, 0, 0, 1);
			if (pItem)
			{
				if (AddToInventory(ch, pItem, true, true))
				{
					GAMELOG << init("EVENT GOMDORI 2007 GIVE ITEM", ch)
							<< "WIN" << delim
							<< ch->m_nEventGomdori2007Win << delim
							<< "GIFT" << delim
							<< itemlog(pItem)
							<< end;
					ItemAddMsg(rmsg, pItem);
					SEND_Q(rmsg, ch->m_desc);

					EventGomdori2007SelectGiftMsg(rmsg, nSelItem);
					SEND_Q(rmsg, ch->m_desc);
				}
				else
				{
					delete pItem;
					pItem = NULL;
					EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
			else
			{
				EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 초기화
			ch->m_nEventGomdori2007Win = 0;
			ch->m_nEventGomdori2007Game = 0;
			ch->m_nEventGomdori2007FirstWin = 0;
			ch->m_bEventGomdori2007Start = false;
		}
		else
			return ;
		break;
#endif // !defined (LC_JPN) && !defined (LC_BRZ)
		
	case MSG_EVENT_GOMDORI_2007_VIEW_STATUS:
		if (IS_RUNNING_CONN)
		{
			ConnEventGomdori2007StatusMsg(rmsg, ch->m_index);
			SEND_Q(rmsg, gserver.m_connector);
		}
		break;

	default:
		return ;
	}
}
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_CHILDRENSDAY_2007
void do_EventChildrensDay_2007(CPC* ch, CNetMsg& msg)
{
	int nSubType;
	CNetMsg rmsg;
	msg >> nSubType;
	switch( nSubType )
	{
	case MSG_EVENT_CHILDRENSDAY_2007_REQ :
		int nItemIndex;			//보상 아이템
		int nFlowerCount;		//필요 개나리

		msg >> nItemIndex;
		msg >> nFlowerCount;

		CItemProto* itemtype = gserver.m_itemProtoList.FindIndex( nItemIndex );
		if(itemtype == NULL)
			return ;

		int nNeedFlowerCount = (int)((float)itemtype->GetItemProtoLevel() /2 +0.5f);

		if( nNeedFlowerCount != nFlowerCount ) // 요청 개나리 갯수 다름
		{
			EventChildrensDay2007Msg( rmsg, 1 );		// 
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		if( !(itemtype->m_jobFlag & (1<<ch->m_job)) )	//직업 다름 
		{
			EventChildrensDay2007Msg( rmsg, 2 );	
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		// 개나리 일단 갯수 만큼 있나 검사 !
		int nRow =0, nCol=0;
		CItem	*pFlower = NULL;
		if (  ch->m_invenNormal.FindItem(&nRow, &nCol, 2344, 0, 0) )		//2344 노란 개나리
		{
			pFlower = ch->m_invenNormal.GetItem(nRow, nCol);

			if(  pFlower && nNeedFlowerCount > pFlower->Count() )
			{
				EventChildrensDay2007Msg( rmsg, 1 );	// 개나리 부족 
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}
		else
		{
			EventChildrensDay2007Msg( rmsg, 1 );		// 개나리 없음 
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		
		// 이전에 지급한 적이 있었는지 검사 (헬퍼에 요청 (다음은 헬퍼에 맞긴다) ProcHelperChildrenEvent() )
		HelperChildrensDay2007Msg( rmsg , MSG_HELPER_CHILDRENSDAY_2007_CHECK, ch->m_index, nItemIndex );
		rmsg << nNeedFlowerCount;		
		SEND_Q( rmsg, gserver.m_helper );
		break;
	}

}
#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
void do_EventFlowerTree2007(CPC* ch, CNetMsg& msg)
{
	const int nEventItem[2] = { 2345, 2346 }; // 퇴비 , 정화수
	unsigned int uPoint = 0;
	unsigned char nSubType;
	int nRow, nCol, i;
	CNetMsg rmsg;
	msg >> nSubType;
	switch( nSubType )
	{		
	case MSG_EVENT_FLOWERTREE_2007_MY_POINT :
		HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_MYPOINT, ch->m_index );	//헬퍼에 현재 자신의 포인트 요청
		SEND_Q( rmsg, gserver.m_helper );
		break;

	case MSG_EVENT_FLOWERTREE_2007_GROWUP :
		{
			for( i=0; i<2; i++ )		// 아이템 찾고, 포인트 계산 후 제거 
			{
				CItem	*pItem = NULL;
				if (  ch->m_invenNormal.FindItem(&nRow, &nCol, nEventItem[i], 0, 0) )		//2345 퇴비, 2346 정화수
				{
					pItem = ch->m_invenNormal.GetItem(nRow, nCol);
					if( pItem ) 
					{
						uPoint += pItem->Count();

						ItemDeleteMsg(rmsg, pItem);									//아이템 제거
						SEND_Q(rmsg, ch->m_desc);
						ch->RemoveItemFromQuickSlot(pItem);
						RemoveFromInventory(ch, pItem, true, true);
					}
				}
			}

			HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_ADDMYPOINT, ch->m_index );
			rmsg << uPoint;
			SEND_Q( rmsg, gserver.m_helper );

			ConnEventFlowerTree2007Msg( rmsg, MSG_CONN_EVENT_FLOWERTREE_2007_ADDPOINT, ch->m_index );
			rmsg << uPoint;
			SEND_Q( rmsg, gserver.m_connector );
		} break;

	case MSG_EVENT_FLOWERTREE_2007_RECV_TICKET :
		HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_RECV_TICKET, ch->m_index );
		SEND_Q( rmsg, gserver.m_helper );
		break;

	case MSG_EVENT_FLOWERTREE_2007_OPEN_TICKET :			// 현재 티켓 확인하고 티켓 하나 줄이고, 보상 받기
		{
			int nRow, nCol; 
			CItem	*pTicket = NULL;
			if (  ch->m_invenNormal.FindItem(&nRow, &nCol, 2351, 0 , 0) )		//2351 교환권
			{
				pTicket = ch->m_invenNormal.GetItem(nRow, nCol);
				if( pTicket != NULL )
				{
					//헬퍼에 아이템 선택을 요청
					HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_PRICEITEM, ch->m_index );
					SEND_Q( rmsg, gserver.m_helper );
				}
			}
			else	//교환권 없음
			{
				EventFlowerTree2007Msg( rmsg, MSG_EVENT_FLOWERTREE_2007_OPEN_TICKET );
				rmsg << (unsigned char) 0;
				SEND_Q( rmsg, ch->m_desc );
			}
		}break;

	case MSG_EVENT_FLOWERTREE_2007_POINT :			// 자신서버의 나무포인트 요청
		{
			ConnEventFlowerTree2007Msg( rmsg, MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT, ch->m_index );
			SEND_Q( rmsg, gserver.m_connector );
		}break;

	case MSG_EVENT_FLOWERTREE_2007_POINT_RANK :		// 커넥터에 나무 포인트 랭크 요청
		{
			ConnEventFlowerTree2007Msg( rmsg, MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT_RANK, ch->m_index );
			SEND_Q( rmsg, gserver.m_connector );
		} break;
	}
}
#endif


#if defined( EVENT_SUMMER_VACATION_2007_RESULT ) || defined (EVENT_SUMMER_VACATION_2007 )
void do_EventSummerVacation2007( CPC* ch, CNetMsg& msg )
{
	int i=0;
	const static int nTableGiftSize = 10;
	const static int nTableGift[nTableGiftSize][3] =
	{
		//figuretype, item_index, count
		{1, 19, 500}, 
		{2, 47, 3 }, 
		{3, 724, 10}, 
		{4, 45, 10}, 
		{5, 723, 2},
		{6, 723, 5}, 
		{7, 723,10},
#ifdef LC_JPN
		{8, 2143, 1}, 
		{9, 2142, 1}, 
		{10,2138, 1}	
#else
		{8, 85, 1}, 
		{9, 85, 2}, 
		{10, 85,3}
#endif // LC_JPN
	};

	CNetMsg	rmsg;
	unsigned char cSubType;

//	int paperindex[4];
//	int papercount[4];
	
	CItem*	pItem;
	msg >> cSubType;

	switch( cSubType )
	{
	case MSG_EVENT_SUMMER_VACATION_2007_PAPER_FIGURES:
		{
			int itemindex[3];		
			int redpaper = 0;
			int greenpaper = 0;
			int silverpaper = 0;
			int goldpaper = 0; 
			for( i = 0; i < 3; i++ )
			{
				msg >> itemindex[i];
				if( itemindex[i] == GREEN_PAPER_ITEM )
					greenpaper++;
				else if( itemindex[i] == RED_PAPER_ITEM )
					redpaper++;
				else if( itemindex[i] == SILVER_PAPER_ITEM )
					silverpaper++;
				else if( itemindex[i] == GOLD_PAPER_ITEM )
					goldpaper++;
			} 

			int MixSheet[10][5] = {
				// 골드, 실버, 레드, 그린, 피규어
				{0, 0, 0, 3, 1 },
				{0, 0, 1, 2, 2 },
				{0, 0, 2, 1, 3 },
				{0, 0, 3, 0, 4 },
				{0, 1, 2, 0, 5 },
				{0, 2, 1, 0, 6 },
				{0, 3, 0, 0, 7 },
				{1, 2, 0, 0, 8 },
				{2, 1, 0, 0, 9 },
				{3, 0, 0, 0, 10 },			
			};
			int figuretype =0;

			for( i = 0; i < 10; i++ )
			{
					if( MixSheet[i][0] == goldpaper && MixSheet[i][1] == silverpaper && 
						MixSheet[i][2] == redpaper && MixSheet[i][3] == greenpaper )
					{
						figuretype = MixSheet[i][4];
						break;
					}
			}
			if( figuretype == 0 )
			{
				EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_FAIL, 0, 0, 0 );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
			
				
			int nRow, nCol; 
			CItem	*pNas = NULL;
			CItem	*paperItem1 = NULL;
			CItem	*paperItem2 = NULL;
			CItem	*paperItem3 = NULL;
			CItem	*paperItem4 = NULL;
			if( redpaper != 0 )
			{
				if( !ch->m_invenNormal.FindItem( &nRow, &nCol, RED_PAPER_ITEM, 0, 0 ) )
				{
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
					SEND_Q( rmsg, ch->m_desc );
					return;					
				}
				else
				{
					paperItem1 = ch->m_invenNormal.GetItem( nRow, nCol );
					if( !paperItem1 || paperItem1->Count() < redpaper  )
					{
						EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
						SEND_Q( rmsg, ch->m_desc );
						return;
					}
				}
			}
			if( greenpaper != 0)
			{
				if( !ch->m_invenNormal.FindItem( &nRow, &nCol, GREEN_PAPER_ITEM, 0, 0 ) )
				{
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
					SEND_Q( rmsg, ch->m_desc );
					return;					
				}
				else
				{
					paperItem1 = ch->m_invenNormal.GetItem( nRow, nCol );
					if( !paperItem1 || paperItem1->Count() < greenpaper  )
					{
						EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
						SEND_Q( rmsg, ch->m_desc );
						return;
					}
				}
			}
			if( silverpaper != 0 )
			{
				if( !ch->m_invenNormal.FindItem( &nRow, &nCol, SILVER_PAPER_ITEM, 0, 0 ) )
				{
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
					SEND_Q( rmsg, ch->m_desc );
					return;					
				}
				else
				{
					paperItem1 = ch->m_invenNormal.GetItem( nRow, nCol );
					if( !paperItem1 || paperItem1->Count() < silverpaper  )
					{
						EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
						SEND_Q( rmsg, ch->m_desc );
						return;
					}
				}
			}
			if( goldpaper != 0 )
			{
				if(  !ch->m_invenNormal.FindItem( &nRow, &nCol, GOLD_PAPER_ITEM, 0, 0 ) )
				{					
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
					SEND_Q( rmsg, ch->m_desc );
					return;					
				}
				else
				{
					paperItem1 = ch->m_invenNormal.GetItem( nRow, nCol );
					if( !paperItem1 || paperItem1->Count() < goldpaper  )
					{
						EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
						SEND_Q( rmsg, ch->m_desc );
						return;
					}
				}
			}

			if (  ch->m_invenNormal.FindItem(&nRow, &nCol, 19, 0 , 0) )		//19 나스
			{
				pNas = ch->m_invenNormal.GetItem(nRow, nCol);
				if( pNas == NULL )
				{					
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOMONEY, 0, 0, 0 );
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}
			else
			{
				EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOMONEY, 0, 0, 0 );
					SEND_Q( rmsg, ch->m_desc );
					return;

			}
			if( pNas->Count() < 100 )
			{
				EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOMONEY, 0, 0, 0 );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
			

			CItem* item;
			int giftitemindx = 0;
			int itemcount = 0;
			for( i = 0; i < 10 ; i++ )
			{
				if( nTableGift[i][0] == figuretype )
				{
					item = gserver.m_itemProtoList.CreateItem( nTableGift[i][1], -1, 0, 0, nTableGift[i][2] );
					giftitemindx  = nTableGift[i][1];
					itemcount = nTableGift[i][2];
					break;
				}
			}

			if( giftitemindx == 0 && figuretype == 0 )
			{
				EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_FAIL, figuretype, giftitemindx, itemcount );
				SEND_Q(rmsg, ch->m_desc );
				return;
			}

			if( item )
			{
				if( AddToInventory( ch, item, true, true ) )
				{
					GAMELOG << init("EVENT SUMMER VACATION 2007 GIVE ITEM", ch )
						<< "GIFT" << delim
						<< itemlog( item )
						<< end;

					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_OK, figuretype, giftitemindx, itemcount );
					SEND_Q(rmsg, ch->m_desc );
					if( item->tab() < 0 )
					{
						// 겹침
						CInventory* pInven = GET_INVENTORY(ch, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
						int r, c;
						pInven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag);
						CItem* pItemCurGood = pInven->GetItem(r, c);
						ItemUpdateMsg(rmsg, pItemCurGood , item->Count());
						SEND_Q(rmsg, ch->m_desc);

						delete item;
						item = NULL;

					}
					else
					{
						ItemAddMsg( rmsg, item );
						SEND_Q( rmsg, ch->m_desc );
						item = NULL;
					}
				}
				else
				{
					delete item;
					item = NULL;
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY, figuretype, giftitemindx, itemcount );
					SEND_Q( rmsg, ch->m_desc );
					return;
				}

				// 나스 제거 
				DecreaseFromInventory( ch, pNas, 100 );
				if( pNas->Count() > 100 )
				{
					ItemUpdateMsg( rmsg, pNas, -100 );
					SEND_Q( rmsg, ch->m_desc );
				}
				else
				{
					ItemDeleteMsg( rmsg, pNas );
					SEND_Q( rmsg, ch->m_desc );
					RemoveFromInventory( ch, pNas, true, true );
				}

				if( redpaper != 0 )
				{
					if( ch->m_invenNormal.FindItem( &nRow, &nCol, RED_PAPER_ITEM, 0, 0 ) )
					{
						paperItem1 = ch->m_invenNormal.GetItem( nRow, nCol );
						DecreaseFromInventory( ch, paperItem1, redpaper );
						if( paperItem1->Count() > redpaper )
						{
							ItemUpdateMsg( rmsg, paperItem1, -redpaper );
							SEND_Q( rmsg, ch->m_desc );
						}
						else
						{
							ItemDeleteMsg( rmsg, paperItem1 );
							SEND_Q( rmsg, ch->m_desc );
							RemoveFromInventory( ch, paperItem1, true, true );
						}
					}
				}	
				if( greenpaper != 0)
				{
					if( ch->m_invenNormal.FindItem( &nRow, &nCol, GREEN_PAPER_ITEM, 0, 0 ) )
					{
						paperItem2 = ch->m_invenNormal.GetItem( nRow, nCol );
						if( paperItem2->Count() > greenpaper )
						{
							DecreaseFromInventory( ch, paperItem2, greenpaper );
							ItemUpdateMsg( rmsg, paperItem2, -greenpaper );
							SEND_Q( rmsg, ch->m_desc );
						}
						else
						{
							ItemDeleteMsg( rmsg, paperItem2 );
							SEND_Q( rmsg, ch->m_desc );
							RemoveFromInventory( ch, paperItem2, true, true );
						}
					}
				}
				if( silverpaper != 0 )
				{
					if( ch->m_invenNormal.FindItem( &nRow, &nCol, SILVER_PAPER_ITEM, 0, 0 ) )
					{
						paperItem3 = ch->m_invenNormal.GetItem( nRow, nCol );
						if( paperItem3->Count() > silverpaper )
						{
							DecreaseFromInventory( ch, paperItem3, silverpaper );
							ItemUpdateMsg( rmsg, paperItem3, -silverpaper );
							SEND_Q( rmsg, ch->m_desc );
						}
						else
						{
							ItemDeleteMsg( rmsg, paperItem3 );
							SEND_Q( rmsg, ch->m_desc );
							RemoveFromInventory( ch, paperItem3, true, true );
						}
					}
				}
				if( goldpaper != 0 )
				{
					if( ch->m_invenNormal.FindItem( &nRow, &nCol, GOLD_PAPER_ITEM, 0, 0 ) )
					{
						paperItem4 = ch->m_invenNormal.GetItem( nRow, nCol );
						
						if( paperItem4->Count() > goldpaper )
						{
							DecreaseFromInventory( ch, paperItem4, goldpaper );
							ItemUpdateMsg( rmsg, paperItem4, -goldpaper );
							SEND_Q( rmsg, ch->m_desc );
						}
						else
						{
							ItemDeleteMsg( rmsg, paperItem4 );
							SEND_Q( rmsg, ch->m_desc );
							RemoveFromInventory( ch, paperItem4, true, true );
						}
					}
				}
			}
			else
			{
				EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY, figuretype, giftitemindx, itemcount );
				SEND_Q( rmsg, ch->m_desc );			
			}
		}
		break;

	case MSG_EVENT_SUMMER_VACATION_2007_INCHEN:
	{		
		int paperindex;

		msg >> paperindex;

		int nRow, nCol;
			if (!ch->m_invenNormal.FindItem(&nRow, &nCol, paperindex, 0, 0))
			{
				EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, paperindex );
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		pItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if( pItem->Count() < 5 )
		{
			EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, paperindex );
			SEND_Q(rmsg, ch->m_desc);
			return ;

		}

		const int GoldPaperInchenProb = 100;
		const int SilverPaperIncheProb = 400;
		const int RedPaperInchenProb = 900;
		int nProb = GetRandom( 1, 1000 );
		if( paperindex == GREEN_PAPER_ITEM )
		{
			if( nProb < RedPaperInchenProb )
			{
				// 성공 메세지 전송
				CItem* giveitem = gserver.m_itemProtoList.CreateItem( RED_PAPER_ITEM, -1, 0, 0, 1 );

				if( giveitem )
				{
					if( AddToInventory( ch, giveitem, true, true ) )
					{
						if( giveitem->tab() < 0 )
						{
							// 겹침
							CInventory* pInven = GET_INVENTORY(ch, GET_TAB(giveitem->m_itemProto->m_typeIdx, giveitem->m_itemProto->m_subtypeIdx));
							int r, c;
							pInven->FindItem(&r, &c, giveitem->m_idNum, giveitem->m_plus, giveitem->m_flag);
							CItem* pItemCurGood = pInven->GetItem(r, c);
							ItemUpdateMsg(rmsg, pItemCurGood , giveitem->Count());
							SEND_Q(rmsg, ch->m_desc);

							delete giveitem;
							giveitem = NULL;

						}
						else
						{
							ItemAddMsg( rmsg, giveitem );
							SEND_Q( rmsg, ch->m_desc );
							giveitem = NULL;
						}
					}
					EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_OK, RED_PAPER_ITEM );
					SEND_Q(rmsg, ch->m_desc);
					
					// 인첸 된 아이템 삭제 - 5개
					if( pItem->Count() > 5 )
					{
						DecreaseFromInventory( ch, pItem, 5 );
						ItemUpdateMsg( rmsg, pItem, -5 );
						SEND_Q( rmsg, ch->m_desc );
					}
					else
					{
						ItemDeleteMsg( rmsg, pItem );
						SEND_Q( rmsg, ch->m_desc );
						RemoveFromInventory( ch, pItem, true, true );
					}
					return;

				}
				else
				{
					EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY, paperindex );
					SEND_Q( rmsg, ch->m_desc );	
					return;
				}				

			}
			EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL, paperindex );
			SEND_Q(rmsg, ch->m_desc);
			
			// 실패전송
			// 인첸 된 아이템 삭제 - 4개			
			if( pItem->Count() > 5 )
			{
				DecreaseFromInventory( ch, pItem, 4 );
				ItemUpdateMsg( rmsg, pItem, -4 );
				SEND_Q( rmsg, ch->m_desc );
			}
			else
			{
				ItemDeleteMsg( rmsg, pItem );
				SEND_Q( rmsg, ch->m_desc );
				RemoveFromInventory( ch, pItem, true, true );
			}
			return;
		}
		else if ( paperindex == RED_PAPER_ITEM )
		{
			if( nProb < SilverPaperIncheProb )
			{
					// 성공 메세지 전송
				CItem* giveitem = gserver.m_itemProtoList.CreateItem( SILVER_PAPER_ITEM, -1, 0, 0, 1 );
					
				if( giveitem )
				{
					if( AddToInventory( ch, giveitem, true, true ) )
					{
						if( giveitem->tab() < 0 )
						{
							// 겹침
							CInventory* pInven = GET_INVENTORY(ch, GET_TAB(giveitem->m_itemProto->m_typeIdx, giveitem->m_itemProto->m_subtypeIdx));
							int r, c;
							pInven->FindItem(&r, &c, giveitem->m_idNum, giveitem->m_plus, giveitem->m_flag);
							CItem* pItemCurGood = pInven->GetItem(r, c);
							ItemUpdateMsg(rmsg, pItemCurGood , giveitem->Count());
							SEND_Q(rmsg, ch->m_desc);

							delete giveitem;
							giveitem = NULL;

						}
						else
						{
							ItemAddMsg( rmsg, giveitem );
							SEND_Q( rmsg, ch->m_desc );
							giveitem = NULL;
						}
					}

					EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_OK, SILVER_PAPER_ITEM );
					SEND_Q(rmsg, ch->m_desc);
					// 인첸 된 아이템 삭제 - 5개
					if( pItem->Count() > 5 )
					{
						DecreaseFromInventory( ch, pItem, 5 );
						ItemUpdateMsg( rmsg, pItem, -5 );
						SEND_Q( rmsg, ch->m_desc );
					}
					else
					{
						ItemDeleteMsg( rmsg, pItem );
						SEND_Q( rmsg, ch->m_desc );
						RemoveFromInventory( ch, pItem, true, true );
					}
					return;

				}
				else
				{
					EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY, paperindex );
					SEND_Q( rmsg, ch->m_desc );	
					return;
				}				

			}
			EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL, paperindex );
			SEND_Q(rmsg, ch->m_desc);
			
			// 실패전송
			// 인첸 된 아이템 삭제 - 4개			
			if( pItem->Count() > 5 )
			{
				DecreaseFromInventory( ch, pItem, 4 );
				ItemUpdateMsg( rmsg, pItem, -4 );
				SEND_Q( rmsg, ch->m_desc );
			}
			else
			{
				ItemDeleteMsg( rmsg, pItem );
				SEND_Q( rmsg, ch->m_desc );
				RemoveFromInventory( ch, pItem, true, true );
			}
			return;

		}
		else if( paperindex == SILVER_PAPER_ITEM )
		{
			if( nProb < GoldPaperInchenProb )
			{
				// 성공 메세지 전송
				CItem* giveitem = gserver.m_itemProtoList.CreateItem( GOLD_PAPER_ITEM, -1, 0, 0, 1 );

				if( giveitem )
				{
					if( AddToInventory( ch, giveitem, true, true ) )
					{
						if( giveitem->tab() < 0 )
						{
							// 겹침
							CInventory* pInven = GET_INVENTORY(ch, GET_TAB(giveitem->m_itemProto->m_typeIdx, giveitem->m_itemProto->m_subtypeIdx));
							int r, c;
							pInven->FindItem(&r, &c, giveitem->m_idNum, giveitem->m_plus, giveitem->m_flag);
							CItem* pItemCurGood = pInven->GetItem(r, c);
							ItemUpdateMsg(rmsg, pItemCurGood , giveitem->Count());
							SEND_Q(rmsg, ch->m_desc);

							delete giveitem;
							giveitem = NULL;

						}
						else
						{
							ItemAddMsg( rmsg, giveitem );
							SEND_Q( rmsg, ch->m_desc );
							giveitem = NULL;
						}
					}

					EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_OK, GOLD_PAPER_ITEM );
					SEND_Q(rmsg, ch->m_desc);
					// 인첸 된 아이템 삭제 - 5개
					if( pItem->Count() > 5 )
					{
						DecreaseFromInventory( ch, pItem, 5 );
						ItemUpdateMsg( rmsg, pItem, -5 );
						SEND_Q( rmsg, ch->m_desc );
					}
					else
					{
						ItemDeleteMsg( rmsg, pItem );
						SEND_Q( rmsg, ch->m_desc );
						RemoveFromInventory( ch, pItem, true, true );
					}
					return;

				}
				else
				{
					EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY, paperindex );
					SEND_Q( rmsg, ch->m_desc );	
					return;
				}				

			}
			EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL, paperindex );
			SEND_Q(rmsg, ch->m_desc);
			
			// 실패전송
			// 인첸 된 아이템 삭제 - 4개			
			if( pItem->Count() >= 5 )
			{
				DecreaseFromInventory( ch, pItem, 4 );
				ItemUpdateMsg( rmsg, pItem, -4 );
				SEND_Q( rmsg, ch->m_desc );
			}
			else
			{
				ItemDeleteMsg( rmsg, pItem );
				SEND_Q( rmsg, ch->m_desc );
				RemoveFromInventory( ch, pItem, true, true );
			}
			return;
		}
		else if( paperindex == GOLD_PAPER_ITEM )
		{
			EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL, paperindex );
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

	}
		break;
	default:
		return;
	}

}

#endif// defined( EVENT_SUMMER_VACATION_2007_RESULT ) || defined (EVENT_SUMMER_VACATION_2007 )


#ifdef EVENT_TLD_MOTHERDAY_2007
void do_EventTldMotherday2007(CPC* ch, CNetMsg& msg)
{
	// 모래 봉투 인덱스
	//TODO: ITEM 인덱스 결정 되면 수정할것
	static const int eventItem[3] = {
		2385,		// Mother's Card
		2386,		// jasmine
		2387,		// Garland
	};

	// 보상 테이블 : change_type, need_item_count1, need_item_count2, need_item_count2, give_item_index
	static const int tableGift[5][5] = {
		{1, 20, 20, 10, 85 },		// 고급 제련석
		{2, 10, 10, 10, 2388 },		// Poem book
		{3, 10, 0, 0, 2391 },		// Blessing of Mother(red)
		{4, 0, 10, 0, 2390},		// Blessing of Mother(yellow)
		{5, 0, 0, 5, 2389},		// Blessing of Mother(blue)
	};

	CNetMsg		rmsg;
	char		cRequestType = 0;

	
	// 1. 요청한 타입에 따라 인덱스 및 수량 결정
	// 2. 아이템 검사
	// 4. 아이템 지급
	// 5. 사용된 아이템 삭제
	// 6. 게임 로그
	// 7. 결과 알림

	// 1. 요청한 타입에 따라 인덱스 및 수량 결정
	msg >> cRequestType;
	if( cRequestType > 5 )
	{
		return;
	}

	int giftItemIndex		= tableGift[cRequestType][4];


	// 2. 아이템 검사
	int			i;
	int			nRow, nCol;

	CItem*	pMotherInvenItem[3] = { NULL, NULL, NULL };		// 사용 되는 아이템
	int needitemcount;

	// 아이템 검사
	for( i = 0; i < 3; i++ )
	{
		needitemcount = tableGift[cRequestType][i+1];
		if( needitemcount == 0 )
			continue;

		if( !ch->m_invenNormal.FindItem( &nRow, &nCol, eventItem[i], 0, 0 ) )
		{
			EventTldMotherday2007tMsg( rmsg, MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_NOITEM );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}

		pMotherInvenItem[i] = ch->m_invenNormal.GetItem( nRow, nCol );
		if( pMotherInvenItem[i] == NULL )
		{
			EventTldMotherday2007tMsg( rmsg, MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_NOITEM );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}

		if( pMotherInvenItem[i]->Count() < needitemcount )
		{
			EventTldMotherday2007tMsg(rmsg, MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_NOITEM );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
	}

	// 아이템 생성
	CItem* pGiftItem = gserver.m_itemProtoList.CreateItem( giftItemIndex, -1, 0, 0, 1 );
	if( !pGiftItem )
	{
		EventTldMotherday2007tMsg(rmsg, MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_NOITEM );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	// 아이템 지급
	if( AddToInventory( ch, pGiftItem, true, true ) )
	{
		if( pGiftItem->tab() < 0 )
		{
			delete pGiftItem;
			ch->m_invenNormal.FindItem( &nRow, &nCol, giftItemIndex, 0, 0 );
			pGiftItem =  ch->m_invenNormal.GetItem( nRow, nCol );
			ItemUpdateMsg( rmsg, pGiftItem, 1 );
			SEND_Q( rmsg, ch->m_desc );
		}
		else
		{
			ItemAddMsg( rmsg, pGiftItem );
			SEND_Q( rmsg, ch->m_desc );
		}

		// 아이템 회수
		for( i = 0; i < 3; i++ )
		{
			needitemcount = tableGift[cRequestType][i+1];
			if( needitemcount == 0 )
				continue;

			if( pMotherInvenItem[i]->Count() > needitemcount )
			{
				DecreaseFromInventory( ch, pMotherInvenItem[i], needitemcount );
				ItemUpdateMsg( rmsg, pMotherInvenItem[i], -needitemcount );
				SEND_Q( rmsg, ch->m_desc );
			}
			else 
			{
				ItemDeleteMsg( rmsg, pMotherInvenItem[i] );
				SEND_Q( rmsg, ch->m_desc );
				RemoveFromInventory( ch, pMotherInvenItem[i], true, true );
				pMotherInvenItem[i] = NULL;
			}
		}

		//  게임로그
		GAMELOG << init("EVENT TLD MOTHER DAY CHANGE", ch )
				<< "REQUEST" << delim
				<< cRequestType << delim
				<< "GIFT" << delim
				<< itemlog( pGiftItem, true )<< delim
				<< end;
		// 결과 알림
		EventTldMotherday2007tMsg( rmsg, MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_OK );
		SEND_Q( rmsg, ch->m_desc );
	}
	else
	{
		EventTldMotherday2007tMsg( rmsg, MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_FULLINVEN );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}
}
#endif // EVENT_TLD_MOTHERDAY_2007



#if defined(EVENT_OPEN_ADULT_SERVER) || defined (EVENT_MAGIC_CARD)
void do_EventOpenAdultServer(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	unsigned char nSubType;
	msg >> nSubType;

	try
	{
		switch(nSubType)
		{
		case MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_REQ:
			{
				const int nRepaySkill[6][2] = { 
					{423,250},		// 바람의 러브매직 25%
					{424,150},		// 힘의 러브매직 15%
					{425,150},		// 민첩의 러브매직 15%
					{426,150},		// 지혜의 러브매직 15%
					{427,150},		// 체질의 러브매직 15%
					{428,150},		// 로레인의 러브매직 15%
				};
				int nNeedItemCountSpade = 1;			// 교환에 필요한 스페이드 개수 
				int nNeedItemCountHeart = 1;			// 교환에 필요한 하트 개수 
				unsigned char nRepaySkillIndex = 0;
				
				
				int nRow, nCol;
				CItem* pItemSpade = NULL;
				CItem* pItemHeart = NULL;

				if(nNeedItemCountSpade > 0)
				{
					if (  !ch->m_invenNormal.FindItem(&nRow, &nCol, 2383 , 0, 0)) // 스페이드 A카드
					{
						 throw MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_NOITEM;	// 아이템 없음
					}
					pItemSpade = ch->m_invenNormal.GetItem(nRow, nCol);
					if( pItemSpade != NULL && pItemSpade->Count() < nNeedItemCountSpade )
					{							
						throw MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_NOITEM;
					}
				}
				if(nNeedItemCountHeart > 0)
				{
					if (  !ch->m_invenNormal.FindItem(&nRow, &nCol, 2384 , 0, 0)) // 스페이드 A카드
					{
						throw MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_NOITEM;	// 아이템 없음
					}
					pItemHeart = ch->m_invenNormal.GetItem(nRow, nCol);
					if( pItemHeart != NULL && pItemHeart->Count() < nNeedItemCountHeart )
					{							
						throw MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_NOITEM;
					}
				}
				
				int nRand = GetRandom(0, 1000);
				for( nRepaySkillIndex=0; nRepaySkillIndex < 6 ; nRepaySkillIndex++ )
				{
					if( nRand <= nRepaySkill[nRepaySkillIndex][1] )
					{			
						CSkill* skill = new CSkill( gserver.m_skillProtoList.Find(nRepaySkill[nRepaySkillIndex][0]), 1 );		//test 423
						bool bApply;
						ApplySkill(ch, ch, skill, -1, bApply);
						delete skill;
						
						if( bApply == false )
						{
							throw MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_FAILED;
						}
						break;
					}
					nRand -= nRepaySkill[nRepaySkillIndex][1];
				}
				if(nRepaySkillIndex != 6)
				{
					// 재료 지우기
					if(pItemSpade->Count() > nNeedItemCountSpade)
					{
						DecreaseFromInventory(ch, pItemSpade, nNeedItemCountSpade);
						ItemUpdateMsg(rmsg, pItemSpade, -nNeedItemCountSpade);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						ItemDeleteMsg(rmsg, pItemSpade);
						SEND_Q(rmsg, ch->m_desc);
						RemoveFromInventory(ch, pItemSpade, true, true);
					}

					if(pItemHeart->Count() > nNeedItemCountHeart)
					{
						DecreaseFromInventory(ch, pItemHeart, nNeedItemCountHeart);
						ItemUpdateMsg(rmsg, pItemHeart, -nNeedItemCountHeart);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						ItemDeleteMsg(rmsg, pItemHeart);
						SEND_Q(rmsg, ch->m_desc);
						RemoveFromInventory(ch, pItemHeart, true, true);
					}

					EventOpenAdultServerMsg(rmsg, MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_OK);
					rmsg << nRepaySkillIndex;
					SEND_Q(rmsg, ch->m_desc);

					GAMELOG << init("EVENT OPEN ADULT SERVER", ch)
							<< "MAGIC CARD REWARD" << delim
							<< "SKILL INDEX" << delim
							<< nRepaySkill[nRepaySkillIndex][0]
							<< end;
				}
				else
				{
					throw MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_FAILED;
				}
			}
			break;
#ifdef EVENT_OPEN_ADULT_SERVER
		case MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_EXCHANGE:
			{
				if(ch->m_job2 == 0)
				{
					throw MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_FAILED;
				}
				
				CNetMsg rmsg;
				HelerOpenAdultServerMsg( rmsg, ch->m_index, MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB );
				SEND_Q( rmsg, gserver.m_helper );
			}
			break;
		case MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_REQ:
			{

				static const unsigned short UpgradeNeedItem[7][4] = {
					{113, 2, 114, 2},
					{119, 3, 110, 3},
					{127, 5, 123, 3},
					{116, 5, 133, 5},
					{124, 7, 140, 5},
					{129, 7, 134, 7},
					{118, 9, 128, 9},
				};

				int tab, row, col;
				int itemlevel;
				int needindex;
				CInventory* inven;

				msg >> tab >> row >> col;

				inven = GET_INVENTORY(ch, tab);
				if(!inven)
					return ;

				CItem* olditem = inven->GetItem(row, col);
				if(olditem == NULL || olditem->m_wearPos != -1)
					return ;

				if(!(olditem->IsArmorType() || olditem->IsWeaponType()))
					return ;

				if(olditem->m_flag & FLAG_ITEM_COMPOSITION)
					return ;

				if((olditem->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE) || (olditem->m_itemProto->m_flag & ITEM_FLAG_CASH))
					return ;

				if(olditem->IsRareItem())
					return ;

#ifdef SET_ITEM
				if(olditem->IsOriginItem())
					return ;
#endif

				if((olditem->m_itemProto->m_index >= 1228 && olditem->m_itemProto->m_index <= 1278)
				|| (olditem->m_itemProto->m_index >= 1978 && olditem->m_itemProto->m_index <= 2031))
					return ;

				itemlevel = olditem->GetItemLevel(true);
				if(itemlevel < 1 || itemlevel > 33) return ;
				else if(itemlevel < 6) needindex = 0;
				else if(itemlevel < 11) needindex = 1;
				else if(itemlevel < 16) needindex = 2;
				else if(itemlevel < 21) needindex = 3;
				else if(itemlevel < 26) needindex = 4;
				else if(itemlevel < 31) needindex = 5;
				else if(itemlevel < 36) needindex = 6;
				else return ;

				CItem* needItem1 = NULL;
				CItem* needItem2 = NULL;
				if(inven->FindItem(&row, &col, UpgradeNeedItem[needindex][0], -1, -1))
				{
					needItem1 = inven->GetItem(row, col);
				}

				if(inven->FindItem(&row, &col, UpgradeNeedItem[needindex][2], -1, -1))
				{
					needItem2 = inven->GetItem(row, col);
				}

				if(!needItem1 || !needItem2)
					throw MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_NOT_FOUND;

				if(needItem1->Count() < UpgradeNeedItem[needindex][1] || needItem2->Count() < UpgradeNeedItem[needindex][3])
					throw MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_NOT_FOUND;

				CItemUpgradeData* UpgradeItem = gserver.m_ItemUpgradeList.FindIndex((unsigned short)olditem->m_itemProto->m_index);
				if(!UpgradeItem)
					throw MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FAILED;


				int plus = 0;
				int rand = 100 - itemlevel;
				if(GetRandom(1, 100) < rand)
				{
					// 조합성공
					int rand = GetRandom(1, 100);
					if(rand <= 20)	plus = 0;
					else if(rand <= 40) plus = 1;
					else if(rand <= 60) plus = 2;
					else if(rand <= 80) plus = 3;
					else	plus = 4;

					CItem* newitem = gserver.m_itemProtoList.CreateItem((int)UpgradeItem->m_exchange_item_idx, -1, plus, 0, 1);
					if(!newitem)
						throw MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FAILED;

					if(newitem->GetItemLevel(true) > 39 
					|| newitem->m_itemProto->m_typeIdx != olditem->m_itemProto->m_typeIdx
					|| newitem->m_itemProto->m_subtypeIdx != olditem->m_itemProto->m_subtypeIdx)
					{
						delete newitem;
						newitem = NULL;
						throw MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FAILED;
					}

					if(AddToInventory(ch, newitem, true, true))
					{
						ItemAddMsg(rmsg, newitem);
						SEND_Q(rmsg, ch->m_desc);

						GAMELOG << init("EVENT OPEN ADULT SERVER", ch)
								<< "ITEM UPGRADE REWARD" << delim
								<< "PREV ITEMINDEX" << delim
								<< olditem->m_itemProto->m_index << delim
								<< olditem->m_itemProto->m_name << delim
								<< "AFTER ITEMINDEX" << delim
								<< newitem->m_itemProto->m_index << delim
								<< newitem->m_itemProto->m_name
								<< end;

						EventOpenAdultServerMsg(rmsg, MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_OK);
						SEND_Q(rmsg, ch->m_desc);
						
						// 재료지우기
						ItemDeleteMsg(rmsg, olditem);
						SEND_Q(rmsg, ch->m_desc);
						RemoveFromInventory(ch, olditem, true, true);

						if(needItem1->Count() > UpgradeNeedItem[needindex][1])
						{
							DecreaseFromInventory(ch, needItem1, UpgradeNeedItem[needindex][1]);
							ItemUpdateMsg(rmsg, needItem1, -UpgradeNeedItem[needindex][1]);
							SEND_Q(rmsg, ch->m_desc);
						}
						else
						{
							ItemDeleteMsg(rmsg, needItem1);
							SEND_Q(rmsg, ch->m_desc);
							RemoveFromInventory(ch, needItem1, true, true);
						}
						
						if(needItem2->Count() > UpgradeNeedItem[needindex][3])
						{
							DecreaseFromInventory(ch, needItem2, UpgradeNeedItem[needindex][3]);
							ItemUpdateMsg(rmsg, needItem2, -UpgradeNeedItem[needindex][3]);
							SEND_Q(rmsg, ch->m_desc);
						}
						else
						{
							ItemDeleteMsg(rmsg, needItem2);
							SEND_Q(rmsg, ch->m_desc);
							RemoveFromInventory(ch, needItem2, true, true);
						}
					}
					else
					{
						delete newitem;
						newitem = NULL;
						throw MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FULLINVEN;
					}
				}
				else
				{
					// 조합실패
					// 재료를 지운다.
					if(needItem1->Count() > UpgradeNeedItem[needindex][1])
					{
						DecreaseFromInventory(ch, needItem1, UpgradeNeedItem[needindex][1]);
						ItemUpdateMsg(rmsg, needItem1, -UpgradeNeedItem[needindex][1]);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						ItemDeleteMsg(rmsg, needItem1);
						SEND_Q(rmsg, ch->m_desc);
						RemoveFromInventory(ch, needItem1, true, true);
					}
					
					if(needItem2->Count() > UpgradeNeedItem[needindex][3])
					{
						DecreaseFromInventory(ch, needItem2, UpgradeNeedItem[needindex][3]);
						ItemUpdateMsg(rmsg, needItem2, -UpgradeNeedItem[needindex][3]);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						ItemDeleteMsg(rmsg, needItem2);
						SEND_Q(rmsg, ch->m_desc);
						RemoveFromInventory(ch, needItem2, true, true);
					}
					throw MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FAILED;
				}
			}
			break;
#endif //EVENT_OPEN_ADULT_SERVER
		}
	}
	catch(MSG_EVENT_OPEN_ADULT_SERVER_TYPE err)
	{
		EventOpenAdultServerMsg(rmsg, err);
		SEND_Q(rmsg, ch->m_desc);
	}
}
#endif // EVENT_OPEN_ADULT_SERVER


#ifdef GIFT_EVENT_2007
void do_EventGift2007(CPC* ch, CNetMsg& msg)
{
	if (IS_RUNNING_CONN)
	{
		CNetMsg	rmsg;
		ConnEventGift2007Msg(rmsg, MSG_CONN_EVENT_GIFT_2007_CHECK, ch);
		SEND_Q(rmsg, gserver.m_connector);
	}
}


void do_EventGift2007(CPC* ch, CNetMsg& msg)
{
	if (IS_RUNNING_CONN)
	{
		CNetMsg	rmsg;
		ConnEventGift2007Msg(rmsg, MSG_CONN_EVENT_GIFT_2007_CHECK, ch);
		SEND_Q(rmsg, gserver.m_connector);
	}
}
#endif // GIFT_EVENT_2007 

#ifdef EVENT_BJMONO_2007
void do_EventBjMono2007 (CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	unsigned char nSubType;
	int nCount;
	int i=0;
	const int PriceItem[6][2] = {	{ 723, 10 } , { 838, 10 }, 
									{ 836 , 15 } , { 837 , 20 },
									{ 0, 0 } , { 1288 , 5 } };		// 0,0 은 다른 셋트

	const static int nTableWearSize = 4;
	const static int nTableWear[JOBCOUNT][nTableWearSize] = {
		{2054, 2055, 2056, 2057},		// 타이탄
		{2058, 2059, 2060, 2061},		// 나이트
		{2062, 2063, 2064, 2065},		// 힐러
		{2066, 2067, 2068, 2069},		// 메이지
		{2070, 2071, 2072, 2073},		// 로그
		{2074, 2075, 2076, 2077},		// 소서러
#ifdef NIGHT_SHADOW
		{2058, 2059, 2060, 2061},		// 나이트
#endif //NIGHT_SHADOW		
	};

	msg >> nSubType;

	switch( nSubType )
	{
	case MSG_EVENT_BJMONO_2007_TICKET_REQ :
		{
			CNetMsg rmsg;
			ConnEventBjMono2007Msg( rmsg, MSG_CONN_EVENT_BJMONO_TICKET_CHECK ,  ch );
			SEND_Q( rmsg, gserver.m_connector );
		}break;
	case MSG_EVENT_BJMONO_2007_CHANGE_TICKET_REQ :
		{
			// 응모권 확인 후 아이템 주기
			int nRow, nCol;
			CItem* pEventTicket; 
			msg >> nCount;

			if( nCount < 1 || nCount > 6 )
			{
				// 교환 응모권 개수 클라이언트에서 예외 처리
				return;
			}

			// 응모권 확인
			if (  ch->m_invenNormal.FindItem(&nRow, &nCol, 2417 , 0, 0)) // <<-- 응모권 idx나오면 입력 
			{
				pEventTicket = ch->m_invenNormal.GetItem(nRow, nCol);				
				if( pEventTicket == NULL || pEventTicket->Count() < nCount )
				{
					EventBjMono2007Msg( rmsg, MSG_EVENT_BJMONO_2007_DONT_CHANGE_TICKET);	// 응모권 아이템 없음
					SEND_Q(rmsg, ch->m_desc);
					return;
				}
			}
			else
			{
				EventBjMono2007Msg( rmsg, MSG_EVENT_BJMONO_2007_DONT_CHANGE_TICKET);	// 응모권 아이템 없음
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 아이템 지급
			if( nCount == 5 ) // 교복 셋트  
			{
				//////////////////////////////////////////////////////////////////////////
				// 체험 교복 세트 보상 과 같은 소스 // 만들고
				int nNeedWeight = 0;
				CItem* pItems[nTableWearSize] = {NULL, NULL, NULL, NULL};
				for (i = 0; i < nTableWearSize; i++)
				{
					pItems[i] = gserver.m_itemProtoList.CreateItem(nTableWear[(int)ch->m_job][i], -1, 0, 0, 1);
					if (pItems[i] == NULL)
						break;
					nNeedWeight = pItems[i]->m_itemProto->m_weight;
				}
				if (i < nTableWearSize || ch->m_weight + nNeedWeight >= ch->m_maxWeight * 15 / 10 || ch->m_invenNormal.GetSpace() < nTableWearSize)
				{
					EventBjMono2007Msg( rmsg, MSG_EVENT_BJMONO_2007_PRICE_FULLINVEN);	// 인벤 가득 참으로 수정 필요
					SEND_Q(rmsg, ch->m_desc);
					for (i = 0; i < nTableWearSize; i++)
					{
						if (pItems[i])
						{
							delete pItems[i];
							pItems[i] = NULL;
						}
					}
					return ;
				}

				// 주고
				for (i = 0; i < nTableWearSize; i++)
				{
					if (AddToInventory(ch, pItems[i], true, true))
					{
						ItemAddMsg(rmsg, pItems[i]);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						GAMELOG << init("EVENT SK BJ MONO WEAR ERROR", ch)
								<< itemlog(pItems[i])
								<< end;
					}
				}

				// 로그 남기고
				GAMELOG << init("EVENT SK BJ MONO GIVE WEAR", ch);
				for (i = 0; i < nTableWearSize; i++)
				{
					GAMELOG << itemlog(pItems[i]) << delim;
					if (pItems[i]->tab() < 0)
						delete pItems[i];
				}						
				//////////////////////////////////////////////////////////////////////////
			}
			else
			{
				// 아이템 생성
				CItem* pPriceItem = gserver.m_itemProtoList.CreateItem( PriceItem[nCount-1][0] , -1, 0, 0, PriceItem[nCount-1][1] );
				if( !pPriceItem )
				{
					// 아이템 생성 실패
					return;
				}

				// 아이템 지급
				if( AddToInventory( ch, pPriceItem, true, true ) )
				{
					if( pPriceItem->tab() < 0 )
					{
						delete pPriceItem;
						ch->m_invenNormal.FindItem( &nRow, &nCol, PriceItem[nCount-1][0] , 0, 0 );
						pPriceItem =  ch->m_invenNormal.GetItem( nRow, nCol );
						ItemUpdateMsg( rmsg, pPriceItem, PriceItem[nCount-1][1] );
						SEND_Q( rmsg, ch->m_desc );
					}
					else
					{
						ItemAddMsg( rmsg, pPriceItem );
						SEND_Q( rmsg, ch->m_desc );
					}
				}
				else
				{
					EventBjMono2007Msg( rmsg, MSG_EVENT_BJMONO_2007_PRICE_FULLINVEN);
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}
			
			// 응모권 제거
			if( pEventTicket->Count() > nCount )
			{
				DecreaseFromInventory( ch, pEventTicket, nCount );
				ItemUpdateMsg( rmsg, pEventTicket, -nCount );
				SEND_Q( rmsg, ch->m_desc );
			}
			else 
			{
				ItemDeleteMsg( rmsg, pEventTicket );
				SEND_Q( rmsg, ch->m_desc );
				RemoveFromInventory( ch, pEventTicket, true, true );
				pEventTicket = NULL;
			}			

			//  게임로그
			GAMELOG << init("EVENT SK BJ MONO", ch )
					<< nCount << end;
			// 결과 알림
			EventBjMono2007Msg( rmsg, MSG_EVENT_BJMONO_2007_CHANGE_TICKET_REQ );
			SEND_Q( rmsg, ch->m_desc );
		}
		break;
	}
}
#endif // EVENT_BJMONO_2007

#ifdef EVENT_SSHOT_2007
void do_EventSshot2007( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	unsigned char nSubType;
	msg >> nSubType;

	switch( nSubType )
	{
	case MSG_EVENT_SSHOT_2007_RECIVE :
		{
			ConnEventSshot2007Msg( rmsg, MSG_CONN_EVENT_SSHOT_CHECK ,ch );
			SEND_Q( rmsg, gserver.m_connector );
		}		
		break;
	}

}
#endif //EVENT_SSHOT_2007

#ifdef EVENT_RICHYEAR_2007
void do_EventRichYear(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	unsigned char nSubType;

	msg >> nSubType;
	switch(nSubType)
	{
	case MSG_EVENT_RICHYEAR_UPGRADE:		// 아이템 업그레이드
		{
			int grade = -1;
			int rowCrop, colCrop;
			int rowNeedItem, colNeedItem;
			int newitemindex = 0;
			int nUpgradeProb = 0;
			if(ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 876, 0, 0)) grade = 0;
			else if(ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 877, 0, 0)) grade = 1;
			else if(ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 878, 0, 0)) grade = 2;
			else if(ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 879, 0, 0)) grade = 3;
			else if(ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 2422, 0, 0)) grade = 4;

			switch(grade)
			{
			case 0:							// 씨앗 -> 묘목
				{
					newitemindex = 877;
					nUpgradeProb = 100;
#ifdef EVENT_RICHYEAR_CHANGE_2008
					if(!ch->m_invenNormal.FindItem(&rowNeedItem, &colNeedItem, 881, 0, 0))
#else
					if(!ch->m_invenNormal.FindItem(&rowNeedItem, &colNeedItem, 2418, 0, 0))
#endif // LC_HBK
					{
						EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_ERROR_UPGRADE_NOITEM);
						SEND_Q(rmsg, ch->m_desc);
						return ;
					}
				}
				break;
			case 1:							// 묘목 -> 분재
				{
					newitemindex = 878;
					nUpgradeProb = 70;
#ifdef EVENT_RICHYEAR_CHANGE_2008
					if(!ch->m_invenNormal.FindItem(&rowNeedItem, &colNeedItem, 1058, 0, 0))
#else
					if(!ch->m_invenNormal.FindItem(&rowNeedItem, &colNeedItem, 2419, 0, 0))
#endif // LC_HBK
					{
						EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_ERROR_UPGRADE_NOITEM);
						SEND_Q(rmsg, ch->m_desc);
						return ;
					}
				}
				break;
			case 2:							// 분재 -> 나무
				{
					newitemindex = 879;
					nUpgradeProb = 50;
#ifdef EVENT_RICHYEAR_CHANGE_2008
					if(!ch->m_invenNormal.FindItem(&rowNeedItem, &colNeedItem, 1059, 0, 0))
#else
					if(!ch->m_invenNormal.FindItem(&rowNeedItem, &colNeedItem, 2420, 0, 0))
#endif // EVENT_RICHYEAR_CHANGE_2008
					{
						EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_ERROR_UPGRADE_NOITEM);
						SEND_Q(rmsg, ch->m_desc);
						return ;
					}
				}
				break;
			case 3:							// 나무 -> 로고
				{
					newitemindex = 2422;
					nUpgradeProb = 30;
#ifdef EVENT_RICHYEAR_CHANGE_2008
					if(!ch->m_invenNormal.FindItem(&rowNeedItem, &colNeedItem, 1060, 0, 0))
#else
					if(!ch->m_invenNormal.FindItem(&rowNeedItem, &colNeedItem, 2421, 0, 0))
#endif // EVENT_RICHYEAR_CHANGE_2008
					{
						EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_ERROR_UPGRADE_NOITEM);
						SEND_Q(rmsg, ch->m_desc);
						return ;
					}
				}
				break;
			case 4:							// 더이상 업그레이드 불가
				{
					EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_ERROR_ALREADY_UPGRADE);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			default:						// 작물 없음
				{
					EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_ERROR_NOCROP);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}

			CItem* crop = ch->m_invenEvent.GetItem(rowCrop, colCrop);
			CItem* needitem = ch->m_invenNormal.GetItem(rowNeedItem, colNeedItem);

			if(!crop || !needitem)
				return ;

			CItem* newitem = gserver.m_itemProtoList.CreateItem(newitemindex, -1, 0, 0, 1);
			if(!newitem)
				return ;
			
			DecreaseFromInventory(ch, needitem, 1);
			if (needitem->Count() > 0)
			{
				ItemUpdateMsg(rmsg, needitem, -1);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, needitem);
				SEND_Q(rmsg, ch->m_desc);
				RemoveFromInventory(ch, needitem, true, true);
			}

			if(GetRandom(1, 100) <= nUpgradeProb)
			{
				ItemDeleteMsg(rmsg, crop);
				SEND_Q(rmsg, ch->m_desc);
				RemoveFromInventory(ch, crop, true, true);

				bool bDrop = false;
				if (AddToInventory(ch, newitem, true, true))
				{
					ItemAddMsg(rmsg, newitem);
					SEND_Q(rmsg, ch->m_desc);
				}
				else
				{
					ch->m_pArea->DropItem(newitem, ch);
					newitem->m_preferenceIndex = ch->m_index;
					ItemDropMsg(rmsg, ch, newitem);
					SEND_Q(rmsg, ch->m_desc);
					bDrop = true;
				}
				
				EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_UPGRADE_OK);
				SEND_Q(rmsg, ch->m_desc);


				GAMELOG << init("EVENT RICHYEAR UPGRADE SUCCESS", ch)
						<< grade << delim
						<< (grade + 1)
						<< ((bDrop) ? "DROP" : "GET")
						<< end;
			}
			else
			{
				delete newitem;
				newitem = NULL;

				EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_ERROR_UPGRADE_FAILED);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("EVENT RICHYEAR UPGRADE FAIL", ch)
						<< grade
						<< end;
			}
		}
		break;
	case MSG_EVENT_RICHYEAR_EXCHANGE:		// 아이템 보상
		{
			int grade = -1;
			int rowCrop, colCrop;
			const int giftitemcount = 5;

#ifdef EVENT_RICHYEAR_CHANGE_2008
#ifdef LC_TLD
				static const int giftitem[giftitemcount][2] = {
				{723, 5},					// 문스톤 5개
				{1629, 3},					// 새콤한 자두 3개
				{1628, 3},					// 달콤한 참외 3개
				{1627, 3},					// 잘익은 수박3개
				{85, 1}};					// 고급 재련석 1개
#elif defined(LC_JPN)
			static const int giftitem[giftitemcount][2] = {
				{ 723, 1},					// 문스톤
				{2142, 1},					// 건강의 물약
				{2143, 1},					// 지력의 물약
				{2347, 1},					// 창고 7일 확장 카드
				{ 884, 3}};					// 드랍증폭제
#else 
			static const int giftitem[giftitemcount][2] = {
				{723,  1},					// 문스톤 5개
				{884,  3},					// 드롭증폭제 3개
				{2347, 1},					// 창고 7일 확장카드
				{2143, 1},					// 지력의 물약
				{2142, 1}};					// 건강의 물약
#endif

#else // EVENT_RICHYEAR_CHANGE_2008
#ifdef LC_HBK
			static const int giftitem[giftitemcount][2] = {
				{723, 5},					// 문스톤 1개
				{884, 3},					// 드롭증폭제 3개
				{786, 1},					// 창고 7일 확장카드
				{492, 1},					// 지력의 물약
				{495, 1}};
#elif LC_TLD
				static const int giftitem[giftitemcount][2] = {
				{723, 5},					// 문스톤 5개
				{838, 3},					// 자두 3개
				{837, 3},					// 참외 3개
				{85, 1},					// 고급제련석
				{836, 3}};					// 수박 3개

#else
			static const int giftitem[giftitemcount][2] = {
				{723, 1},					// 문스톤 1개
				{884, 3},					// 드롭증폭제 3개
				{2347, 1},					// 창고 7일 확장카드
				{2143, 1},					// 지력의 물약
				{2142, 1}};					// 건강의 물약
#endif // LC_HBK
#endif // EVENT_RICHYEAR_CHANGE_2008
			if( ch->m_invenNormal.GetSpace() < giftitemcount )
			{
				EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_ERROR_FULLINVEN);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			int giftprob[giftitemcount];
			memset(giftprob, 0, sizeof(giftprob));

			if(ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 876, 0, 0)) grade = 0;			// 단계1(씨앗)
			else if(ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 877, 0, 0)) grade = 1;		// 단계2(묘목)
			else if(ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 878, 0, 0)) grade = 2;		// 단계3(분재)
			else if(ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 879, 0, 0)) grade = 3;		// 단계4(나무)
			else if(ch->m_invenEvent.FindItem(&rowCrop, &colCrop, 2422, 0, 0)) grade = 4;		// 단계5(열매)
			else
			{
				EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_ERROR_NOCROP);
				SEND_Q(rmsg, ch->m_desc);
			}

			CItem* cropitem = ch->m_invenEvent.GetItem(rowCrop, colCrop);			// 작물
			if(!cropitem)
				return ;

			switch(grade)
			{
			case 0:
				{
					giftprob[0] = 70;				// 문스톤 1개
					giftprob[1] = 30;				// 드롭증폭제 3개
					giftprob[2] = 0;				// 창고 7일 확장카드
					giftprob[3] = 0;				// 지력의 물약
					giftprob[4] = 0;				// 건강의 물약
				}
				break;
			case 1:
				{
					giftprob[0] = 40;
					giftprob[1] = 30;
					giftprob[2] = 20;
					giftprob[3] = 5;
					giftprob[4] = 5;
				}
				break;
			case 2:
				{
					giftprob[0] = 20;
					giftprob[1] = 30;
					giftprob[2] = 30;
					giftprob[3] = 10;
					giftprob[4] = 10;
				}
				break;
			case 3:
				{
					giftprob[0] = 20;
					giftprob[1] = 15;
					giftprob[2] = 25;
					giftprob[3] = 20;
					giftprob[4] = 20;
				}
				break;
			case 4:
				{
					giftprob[0] = 10;
					giftprob[1] = 10;
					giftprob[2] = 30;
					giftprob[3] = 25;
					giftprob[4] = 25;
				}
				break;
			default:
				return;
			}

			int rand = GetRandom(1, 100);
			
			int i;
			int prob = 0;
			for(i = 0; i < giftitemcount; ++i)
			{
				prob += giftprob[i];
				if(rand <= prob)
				{
					CItem* item = gserver.m_itemProtoList.CreateItem(giftitem[i][0], -1, 0, 0, giftitem[i][1]);
					if(item)
					{
						if(AddToInventory(ch, item, true, true))
						{
							GAMELOG << init("EVENT RICHYEAR REWARD SUCCESS", ch);
							if(item->tab() < 0)
							{
								int row, col;
								if(ch->m_invenNormal.FindItem(&row, &col, item->m_itemProto->m_index, 0, 0))
								{
									CItem* prevItem;
									prevItem = ch->m_invenNormal.GetItem(row, col);
									ItemUpdateMsg(rmsg, prevItem, giftitem[i][1]);
									SEND_Q(rmsg, ch->m_desc);

									delete item;
									item = NULL;

									GAMELOG << "ITEM INDEX" << delim
											<< giftitem[i][0]
											<< "ADD COUNT" << delim
											<< giftitem[i][1] << delim
											<< itemlog(prevItem)
											<< end;
								}
							}
							else
							{
								ItemAddMsg( rmsg, item );
								SEND_Q( rmsg, ch->m_desc );
								GAMELOG << itemlog(item)
										<< end;
							}


						}
						else
						{
							EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_ERROR_FULLINVEN);
							SEND_Q(rmsg, ch->m_desc);
							return ;
						}
						break;
					}
				}
			}

			if(i == giftitemcount)
				return ;

			ItemDeleteMsg(rmsg, cropitem);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, cropitem, true, true);

			EventRichYearMsg(rmsg, MSG_EVENT_RICHYEAR_EXCHAGE_OK);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	}
}
#endif // EVENT_RICHYEAR_2007



#ifdef EVENT_LC_1000DAY
void do_EventLC1000Day( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	unsigned char nSubType;
	
	msg >> nSubType;

	int option[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 17, 18, 19, 21, 22, 65 };
	/// 순서대로 옵션 붙을 확률 5, 4, 3, 2, 1 붙을 확률 
	int OptionProb[] = { 10, 90, 200, 300, 400 };

	int letterItem[11] = { 2435, 2436, 2437, 2438, 2439, 2440, 2441, 2442, 2443, 2444, 2445 };  // 문자 아이템 인덱스 미정
#ifdef LC_TLD
	int HatItem[12] = { 2423,2424, 2425, 2426, 2427, 2428, 3012, 3013, 3014, 3015, 3016, 3017 };						// 모자 아이템 인덱스 미정
#else
	int HatItem[12] = { 2423,2424, 2425, 2426, 2427, 2428, 2429, 2430, 2431, 2432, 2433, 2434 };						// 모자 아이템 인덱스 미정	
#endif // LC_TLD

	LONGLONG bitfield = 0;
	int maxnumoption = 18;

	switch( nSubType )
	{
		case MSG_EVENT_LC_1000DAY_HAT: // 모자 교환 요청
			{	
				// 인벤에서 아이템 검사 ( 글자 아이템 11개 )
				int i;

				for( i = 0; i < 11; i++ )
				{
					int nRow, nCol;					
					if( !ch->m_invenNormal.FindItem( &nRow, &nCol, letterItem[i], 0, 0 ) )
					{
						EventLC1000DayMsg( rmsg, MSG_EVENT_LC_1000DAY_NOITEM );
						SEND_Q( rmsg, ch->m_desc );
						return;
					}
				}
				int HatProb = GetRandom( 0, 11 );

				CItem* giveHatItem = gserver.m_itemProtoList.CreateItem( HatItem[HatProb], -1, 0, 0, 1 );

				// 옵션수 결정 
				int optionProb = GetRandom( 0, 1000 );
				int optionCount = 1;
				if( optionProb <= OptionProb[0] )
					optionCount = 5;
				else if( optionProb <= OptionProb[1] )
					optionCount = 4;
				else if( optionProb <= OptionProb[2] )
					optionCount = 3;
				else if( optionProb <= OptionProb[3] )
					optionCount = 2;
				else 
					optionCount = 1;				

				if( !giveHatItem )
					return;

#ifdef HSTEST
				optionCount = 3;
#endif // HSTEST

				// 모자 지급 - 옵션 설정 ( 1 ~ 5개의 옵션 ) 모자 6개중 랜덤 ( 하나 지급 )
				int* type = new int[optionCount];
				i = 0;
				int j = 0;
				while( i < optionCount )
				{
					// type[i] 결정 
					int optionArr = GetRandom( 0, maxnumoption - 1 );
					type[i] = option[optionArr];
					for( j = 0; j < maxnumoption; j++ )
					{
						if( (bitfield & ((LONGLONG)1 << ((type[i] +j ) % maxnumoption ))) == 0 )
							break;
					}

					if( j == maxnumoption )
						optionCount = 1;
					else
						type[i] = ( type[i] + j ) % maxnumoption;

					bitfield |= ((LONGLONG)1 << type[i] );

					// 옵션 번호로 붙을 수 있는 옵션인지 검사 
					COptionProto* proto = gserver.m_optionProtoList.FindProto( type[i] );					
					if( !proto )
						continue;

//					proto->m_levelValue[i] = 1;
					i++;
				}

				// 옵션 부여
				for( i = 0; i < optionCount; i++ )
				{
					COptionProto* proto = gserver.m_optionProtoList.FindProto( type[i] );

					if( !proto )
						continue;

					giveHatItem->m_nOption++;

					//Level, Value 셋팅
					giveHatItem->m_option[i].MakeOptionValue(proto, ch->m_level, optionCount );
					giveHatItem->m_option[i].m_level = 1;
				}

				delete [] type;

								// 아이템 삭제
				for( i = 0; i < 11; i++ )
				{
					int nRow, nCol;
					CItem* pLetter;
					if( !ch->m_invenNormal.FindItem( &nRow, &nCol, letterItem[i], 0, 0 ) )
					{
						EventLC1000DayMsg( rmsg, MSG_EVENT_LC_1000DAY_NOITEM );
						SEND_Q( rmsg, ch->m_desc );
						return;
					}
					else
					{
						pLetter = ch->m_invenNormal.GetItem( nRow, nCol );

						if( pLetter->Count() > 1 )
						{
							DecreaseFromInventory( ch, pLetter, 1 );
							ItemUpdateMsg( rmsg, pLetter, -1  );
							SEND_Q( rmsg, ch->m_desc );
						}
						else
						{
							ItemDeleteMsg( rmsg, pLetter );
							SEND_Q( rmsg, ch->m_desc );
							RemoveFromInventory( ch, pLetter, true, true );
							pLetter = NULL;
						}						
					}
					//	GAMELOG << itemlog(pLetter) << delim;
				}


				if( AddToInventory( ch, giveHatItem, true, true ) )
				{
					ItemAddMsg( rmsg, giveHatItem );
					SEND_Q( rmsg, ch->m_desc );

					GAMELOG << init("EVENT HAT EXCHAGE OK", ch) << itemlog(giveHatItem) << end;

					//GAMELOG << itemlog(giveHatItem) << delim;
				}
			}
			break;
		case MSG_EVENT_LC_1000DAY_HAT_UPDATE: // 모자 옵션 업데이트
			{

				char nRow, nCol;
				int nHatIndex;
				msg >> nRow
					>> nCol
					>> nHatIndex;

				
				// 인벤에 문자 아이템이 있는지 검사
				int i;

				for( i = 0; i < 11; i++ )
				{
					int nRow, nCol;					
					if( !ch->m_invenNormal.FindItem( &nRow, &nCol, letterItem[i], 0, 0 ) )
					{
						EventLC1000DayMsg( rmsg, MSG_EVENT_LC_1000DAY_NOITEM );
						SEND_Q( rmsg, ch->m_desc );
						return;
					}
				}

				// 인벤에 모자 아이템이 있는지 검사

				CItem* hatItem = ch->m_invenNormal.GetItem( nRow, nCol );
				if( !hatItem )
				{
					EventLC1000DayMsg( rmsg, MSG_EVENT_LC_1000DAY_HAT_UPDATE_FAIL );
					rmsg << char(1);   // 1이면 실패
					return;
				}

				int updateProb = GetRandom( 1, 1000 );
				bool bUpdate = false;
#ifdef HSTEST
				updateProb = 1;
#endif // HSTEST

				// 모자가 있다면 옵션 레벨 검사
				if( hatItem->m_nOption != 0 )
				{
					for( i = 0; i < MAX_ITEM_OPTION; i++ )
					{					

						hatItem->m_option[i].m_proto = gserver.m_optionProtoList.FindProto( hatItem->m_option[i].m_type );

						if(!hatItem->m_option[i].m_proto )
							continue;
						

						if( hatItem->m_option[i].m_level == 1 && updateProb <= 470 )
						{
							bUpdate = true;
						}
						else if ( hatItem->m_option[i].m_level == 2 && updateProb <= 300 )
						{
							bUpdate = true;
						}
						else if( hatItem->m_option[i].m_level == 3 && updateProb <= 200 )
						{
							bUpdate = true;

						}
						else if( hatItem->m_option[i].m_level == 4 && updateProb <= 10 )
						{
							bUpdate = true;
						}
						else if( hatItem->m_option[i].m_level == 5 && updateProb <= 10 )
						{
							bUpdate = true;
						}
						else
						{
							bUpdate = false;
							break;
						}
					}
				}
				else
				{
					// 아이템 파괴
					bUpdate = false;
				}

				if( !bUpdate )
				{
					// 아이템 파괴
					if( ch->m_wearing[WEARING_HELMET] &&  ( ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2423 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2424 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2425
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2426 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2427 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2428 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2429 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2430 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2431 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2432 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2433 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 2434 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 3012 
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 3013  
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 3014  
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 3015  
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 3016  
						|| ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index == 3017  
						) )
					{
						ItemDeleteMsg( rmsg, hatItem );
						SEND_Q( rmsg, ch->m_desc );

						EventLC1000DayMsg( rmsg, MSG_EVENT_LC_1000DAY_HAT_UPDATE_FAIL );
						rmsg << char(0);   // 1이면 실패
						SEND_Q( rmsg, ch->m_desc );
						RemoveFromInventory( ch, hatItem, true, true );								
						break;						
					}
					else if( hatItem->m_itemProto->m_index == 2423 || hatItem->m_itemProto->m_index == 2424 
						|| hatItem->m_itemProto->m_index == 2425  || hatItem->m_itemProto->m_index == 2426 
						|| hatItem->m_itemProto->m_index == 2427  || hatItem->m_itemProto->m_index == 2428
						|| hatItem->m_itemProto->m_index == 2429  || hatItem->m_itemProto->m_index == 2430 
						|| hatItem->m_itemProto->m_index == 2431  || hatItem->m_itemProto->m_index == 2432
						|| hatItem->m_itemProto->m_index == 2433 || hatItem->m_itemProto->m_index == 2434
						|| hatItem->m_itemProto->m_index == 3012  || hatItem->m_itemProto->m_index == 3013 
						|| hatItem->m_itemProto->m_index == 3014  || hatItem->m_itemProto->m_index == 3015
						|| hatItem->m_itemProto->m_index == 3016 || hatItem->m_itemProto->m_index == 3017
						)
					{

						ItemDeleteMsg( rmsg, hatItem );
						SEND_Q( rmsg, ch->m_desc );
						EventLC1000DayMsg( rmsg, MSG_EVENT_LC_1000DAY_HAT_UPDATE_FAIL );
						rmsg << char(0);   // 1이면 실패
						SEND_Q( rmsg, ch->m_desc );
						RemoveFromInventory( ch, hatItem, true, true );
					}
					
				}
				else
				{

					for( i = 0; i < MAX_ITEM_OPTION; i++ )
					{
						hatItem->m_option[i].m_proto = gserver.m_optionProtoList.FindProto( hatItem->m_option[i].m_type );

						if(!hatItem->m_option[i].m_proto )
							continue;


						hatItem->m_option[i].m_level++;
					}

					ItemUpdateMsg( rmsg, hatItem, 0 );
					SEND_Q( rmsg, ch->m_desc );

					EventLC1000DayMsg( rmsg, MSG_EVENT_LC_1000DAY_HAT_UPDATE_SUCCESS );
					SEND_Q( rmsg, ch->m_desc );
				}
				// 아이템 삭제
				for( i = 0; i < 11; i++ )
				{
					int nRow, nCol;
					CItem* pLetter;
					if( !ch->m_invenNormal.FindItem( &nRow, &nCol, letterItem[i], 0, 0 ) )
					{
						EventLC1000DayMsg( rmsg, MSG_EVENT_LC_1000DAY_NOITEM );
						SEND_Q( rmsg, ch->m_desc );
						return;
					}
					else
					{
						pLetter = ch->m_invenNormal.GetItem( nRow, nCol );

						if( pLetter->Count() > 1 )
						{
							DecreaseFromInventory( ch, pLetter, 1 );
							ItemUpdateMsg( rmsg, pLetter, -1  );
							SEND_Q( rmsg, ch->m_desc );
						}
						else
						{
							ItemDeleteMsg( rmsg, pLetter );
							SEND_Q( rmsg, ch->m_desc );
							RemoveFromInventory( ch, pLetter, true, true );
							pLetter = NULL;
						}						
					}
						GAMELOG << itemlog(pLetter) << delim;
				}

			}
			break;
		case MSG_EVENT_LC_1000DAY_FIRECRACKER: // 폭죽 아이템 요청
			{
				// inven 에 폭죽 아이템이 있는지 검사
				int nRow, nCol;				
				if( ch->m_invenNormal.FindItem( &nRow, &nCol, 2403, 0, 0 ) )
				{
					EventLC1000DayMsg( rmsg, MSG_EVENT_LC_1000DAY_FIRECRACKER_DUPLICATION );
					SEND_Q( rmsg, ch->m_desc );
				}
				else
				{
					// 폭죽 아이템 지급
					CItem* giveFierCrackerItem = gserver.m_itemProtoList.CreateItem( 2403, -1, 0, 0, 5 ); // 폭죽 아이템 4개 지급
					if( AddToInventory( ch, giveFierCrackerItem, true, true ) )
					{
						ItemAddMsg( rmsg, giveFierCrackerItem );
						SEND_Q( rmsg, ch->m_desc );
					}
				}
			}
			break;
	}

}
#endif // EVENT_LC_1000DAY


#ifdef EVENT_HALLOWEEN_2007
void do_EventHalloween2007( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	unsigned char ucSubType;
	msg >> ucSubType;

	int nTasteSealIndex[2] = { 2488, 2489 };
	int nCandyIndex[6] = { 2482, 2483, 2484, 2485, 2486, 2487 };

	int nPumpkinHeadIndex[6] = { 2470, 2471, 2472, 2473, 2474, 2475};
	int nWitchHatIndex[6] = { 2476, 2477, 2478, 2479, 2480, 2481 };
	// JOB_TITAN 0, JOB_KNIGHT 1, JOB_HEALER 2, JOB_MAGE 3, JOB_ROGUE 4, JOB_SORCERER 5

#ifdef LC_HBK
	int nPumpkinPriceItem[6][3] = { 
		{ 85,  1,  50 },		// 고급재련석 1개 5%
		{ 492, 1, 200 },		// 대형 체력회복증속 물약 1개 20%
		{ 495, 1, 200 },		// 대형 마나회복증속 물약 1개 20%
		{ 1057, 1, 200 },		// 아몬드 빼빼로 1개 20%
		{ 723, 5, 100 },		// 문스톤 5개 10%
		{ 2403 , 10, 250 }		// 폭죽 10개 25%
	};	

#elif defined (LC_BRZ)
	int nPumpkinPriceItem[6][3] = { 
		{ 85,  1,  30 },		// 고급재련석 1개 3%
		{ 972, 1, 150 },		// 행운의 스크롤 1개 15%
		{ 973, 1, 200 },		// 노력의 스크롤 1개 20%
		{ 971, 1, 100 },		// 경험의 스크롤 1개 10%
		{ 723, 5, 250 },		// 문스톤 5개 25%
		{ 723,10, 220 }			// 문스톤 10개 22%
	};	
#elif defined (LC_GER)	
	int nPumpkinPriceItem[6][3] = { 
		{   85, 1,  25 },		// 고급재련석 1개 2.5%
		{ 2139, 1, 100 },		// 훈련주문서 1개 10%
		{   45, 5, 150 },		// 대형 HP 5개 15%
		{  724, 5, 200 },		// 대형 MP 5개 20%
		{  723, 5, 100 },		// 문스톤  5개 10%
		{ 2403,10, 425 }		// 폭죽   10개 42.5%
	};	
#elif defined (LC_JPN)
	int nPumpkinPriceItem[6][3] = { 
		{  972, 1,  30 },		// 행운의 스크롤 1개 3%
		{  973, 1, 100 },		// 노력의 스크롤 1개 10%
		{   47, 3, 150 },		// 메모리 스크롤 3개 15%
		{  391, 3, 200 },		// 속도향상 물약 3개 20%
		{  484,10, 220 },		// 소형 마나회복 물약  10개 22%
		{   43,10, 300 }		// 소형 체력회복 물약  10개 30%
	};	
#else
	int nPumpkinPriceItem[6][3] = { { 85,  1,  30 },					// 고급재련석 1개 3%
									{ 971,  1, 100 },					// 경험의 스크롤 1개 10%
									{ 972,  5, 150 },					// 행운의 스크롤 1개 15%
									{ 973, 5, 200 },					// 노력의 스크롤 1개 20%
									{ 723, 5, 220 },					// 문스톤 5개 22%
									{ 2403, 10 , 300 } };				// 폭죽 10개 30%
#endif // LC_HBK

	switch( ucSubType )
	{
	case MSG_EVENT_TAKE_DEVILHEAIR :		// Helper 에 확인 요청
		{
			CNetMsg rmsg;
			HelperHalloween2007Msg( rmsg, ch->m_index );
			SEND_Q( rmsg, gserver.m_helper );
		}break;
	case MSG_EVENT_CHANGE_CANDYBASKET :		// 사탕바구니 확인하고 렌덤으로 사탕주기
		{
			int nRow, nCol;	
			CItem* pCandyBasket = NULL;

			if( !ch->m_invenNormal.FindItem( &nRow, &nCol, 2490/*사탕바구니*/, 0, 0 ) )
			{
				EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_CANDYBASKET, 1 );		// 사탕 바구니 없음
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
			pCandyBasket = ch->m_invenNormal.GetItem(nRow, nCol);
			if ( pCandyBasket != NULL )
			{
				// 사탕지급
				// 아이템 생성
				int nItemIndex = GetRandom(0,100) % 6;
				CItem* pCandy = gserver.m_itemProtoList.CreateItem( nCandyIndex[nItemIndex] , -1, 0, 0, 1 );
				if( !pCandy )
				{
					EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
					SEND_Q( rmsg, ch->m_desc );
					return;
				}

				// 아이템 지급
				if( AddToInventory( ch, pCandy, true, true ) )
				{
					if( pCandy->tab() < 0 )
					{
						delete pCandy;
						ch->m_invenNormal.FindItem( &nRow, &nCol, nCandyIndex[nItemIndex], 0, 0 );
						pCandy =  ch->m_invenNormal.GetItem( nRow, nCol );
						ItemUpdateMsg( rmsg, pCandy, 1 );
						SEND_Q( rmsg, ch->m_desc );
					}
					else
					{
						ItemAddMsg( rmsg, pCandy );
						SEND_Q( rmsg, ch->m_desc );
					}

					// 아이템 제거
					if( pCandyBasket->Count() > 1 )
					{
						DecreaseFromInventory( ch, pCandyBasket, 1 );
						ItemUpdateMsg( rmsg, pCandyBasket, -1 );
						SEND_Q( rmsg, ch->m_desc );
					}
					else 
					{
						ItemDeleteMsg( rmsg, pCandyBasket );
						SEND_Q( rmsg, ch->m_desc );
						RemoveFromInventory( ch, pCandyBasket, true, true );
						pCandyBasket = NULL;
					}			
				}
				else
				{
					EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN ); // FULL INVEN
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}

			EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_CANDYBASKET, 0 ); //성공
			SEND_Q( rmsg, ch->m_desc );
		}break;

	case MSG_EVENT_CHANGE_PUMKINHEAD :		// 사탕 6개로 교환 받기
		{
			int nRow, nCol;	
			CItem* pCandy = NULL;
			int i=0;

			for(i=0; i<6 ; i++ )
			{
				if( !ch->m_invenNormal.FindItem( &nRow, &nCol, nCandyIndex[i]/*사탕*/, 0, 0 ) )
				{
					EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_PUMKINHEAD, 1 );		// 사탕 없음
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}
			
			// 호박머리 지급
			CItem* pPumpkinHead =  gserver.m_itemProtoList.CreateItem( nPumpkinHeadIndex[ch->m_job], -1, 0, 0, 1 );
			if( !pPumpkinHead )
			{
				EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

				// 아이템 지급
			if( AddToInventory( ch, pPumpkinHead, true, true ) )
			{
				if( pPumpkinHead->tab() < 0 )
				{
					delete pPumpkinHead;
					ch->m_invenNormal.FindItem( &nRow, &nCol, nPumpkinHeadIndex[ch->m_job], 0, 0 );
					pPumpkinHead =  ch->m_invenNormal.GetItem( nRow, nCol );
					ItemUpdateMsg( rmsg, pPumpkinHead, 1 );
					SEND_Q( rmsg, ch->m_desc );
				}
				else
				{
					ItemAddMsg( rmsg, pPumpkinHead );
					SEND_Q( rmsg, ch->m_desc );
				}

				// 아이템 제거
				for(i=0; i<6 ; i++ )
				{
					if( ch->m_invenNormal.FindItem( &nRow, &nCol, nCandyIndex[i]/*사탕*/, 0, 0 ) )
					{
						pCandy = ch->m_invenNormal.GetItem( nRow, nCol );
						if ( pCandy != NULL )
						{
							if( pCandy->Count() > 1 )
							{
								DecreaseFromInventory( ch, pCandy, 1 );
								ItemUpdateMsg( rmsg, pCandy, -1 );
								SEND_Q( rmsg, ch->m_desc );
							}
							else 
							{
								ItemDeleteMsg( rmsg, pCandy );
								SEND_Q( rmsg, ch->m_desc );
								RemoveFromInventory( ch, pCandy, true, true );
								pCandy = NULL;
							}
						}
					}
				}
			}
			else //아이템 지급 실패
			{
				EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_PUMKINHEAD, 0 ); //성공
			SEND_Q( rmsg, ch->m_desc );
		}break;

	case MSG_EVENT_CHANGE_WITCHHAT :		// 증표 2개(이벤트인벤) 확인 후 교환 
		{
			int nRow, nCol;	
			CItem* pTasteSeal = NULL;
			int i=0;

			for(i=0; i<2 ; i++ )
			{
				if( !ch->m_invenEvent.FindItem( &nRow, &nCol, nTasteSealIndex[i]/*맛증표*/, 0, 0 ) )
				{
					EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_WITCHHAT, 1 );		// 사탕 바구니 없음
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}
			
			// 마녀 모자 지급
			CItem* pWitchHat =  gserver.m_itemProtoList.CreateItem( nWitchHatIndex[ch->m_job], -1, 0, 0, 1 );
			if( !pWitchHat )
			{
				EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

				// 아이템 지급
			if( AddToInventory( ch, pWitchHat, true, true ) )
			{
				if( pWitchHat->tab() < 0 )
				{
					delete pWitchHat;
					ch->m_invenNormal.FindItem( &nRow, &nCol, nWitchHatIndex[ch->m_job], 0, 0 );
					pWitchHat =  ch->m_invenNormal.GetItem( nRow, nCol );
					ItemUpdateMsg( rmsg, pWitchHat, 1 );
					SEND_Q( rmsg, ch->m_desc );
				}
				else
				{
					ItemAddMsg( rmsg, pWitchHat );
					SEND_Q( rmsg, ch->m_desc );
				}

				// 아이템 제거
				for(i=0; i<2 ; i++ )
				{
					if( ch->m_invenEvent.FindItem( &nRow, &nCol, nTasteSealIndex[i]/*맛증표*/, 0, 0 ) )
					{
						pTasteSeal = ch->m_invenEvent.GetItem( nRow, nCol );
						if ( pTasteSeal != NULL )
						{
							if( pTasteSeal->Count() > 1 )
							{
								DecreaseFromInventory( ch, pTasteSeal, 1 );
								ItemUpdateMsg( rmsg, pTasteSeal, -1 );
								SEND_Q( rmsg, ch->m_desc );
							}
							else 
							{
								ItemDeleteMsg( rmsg, pTasteSeal );
								SEND_Q( rmsg, ch->m_desc );
								RemoveFromInventory( ch, pTasteSeal, true, true );
								pTasteSeal = NULL;
							}
						}
					}
				}
			}
			else //아이템 지급 실패
			{
				EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_WITCHHAT, 0 ); //성공
			SEND_Q( rmsg, ch->m_desc );

		} break;

	case MSG_EVENT_CHANGE_PUMKIN :			// 호박을 아이템으로 
		{
			int nRow, nCol;	
			CItem* pPumkin = NULL;
			int nPriceItemIndex = 0 , i =0 ;

			if( !ch->m_invenNormal.FindItem( &nRow, &nCol, 2491/*호박*/, 0, 0 ) )
			{
				EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_PUMKIN, 1 );		// 사탕 바구니 없음
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
			pPumkin = ch->m_invenNormal.GetItem(nRow, nCol);

			if ( pPumkin != NULL )
			{
				// 아이템 지급
				// 아이템 생성
				int prob = GetRandom(0,1000) ;
				for(i=0; i<6; i++)
				{
					prob -= nPumpkinPriceItem[i][2];
					if( prob <= 0 )
					{
						nPriceItemIndex = i;
						break;
					}					
				}
				
				if( ch->m_invenNormal.GetSpace() == 0 )
				{
					EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
					SEND_Q( rmsg, ch->m_desc );
					return;
				}

				CItem* pPriceItem = gserver.m_itemProtoList.CreateItem( nPumpkinPriceItem[nPriceItemIndex][0] , -1, 0, 0, nPumpkinPriceItem[nPriceItemIndex][1] );
				if( !pPriceItem )
				{
					EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
					SEND_Q( rmsg, ch->m_desc );
					return;
				}

				// 아이템 지급
				if( AddToInventory( ch, pPriceItem, true, true ) )
				{
					if( pPriceItem->tab() < 0 )
					{
						delete pPriceItem;
						ch->m_invenNormal.FindItem( &nRow, &nCol, nPumpkinPriceItem[nPriceItemIndex][0], 0, 0 );
						pPriceItem =  ch->m_invenNormal.GetItem( nRow, nCol );
						ItemUpdateMsg( rmsg, pPriceItem, nPumpkinPriceItem[nPriceItemIndex][1] );
						SEND_Q( rmsg, ch->m_desc );
					}
					else
					{
						ItemAddMsg( rmsg, pPriceItem );
						SEND_Q( rmsg, ch->m_desc );
					}

					// 아이템 제거
					if( pPumkin->Count() > 1 )
					{
						DecreaseFromInventory( ch, pPumkin, 1 );
						ItemUpdateMsg( rmsg, pPumkin, -1 );
						SEND_Q( rmsg, ch->m_desc );
					}
					else 
					{
						ItemDeleteMsg( rmsg, pPumkin );
						SEND_Q( rmsg, ch->m_desc );
						RemoveFromInventory( ch, pPumkin, true, true );
						pPumkin = NULL;
					}			
				}
				else
				{
					EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN ); // FULL INVEN
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}

			//성공
			EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_PUMKIN, 0 );
			rmsg << nPumpkinPriceItem[nPriceItemIndex][0];
			SEND_Q( rmsg, ch->m_desc );

		}break;
	}

}

#endif //EVENT_HALLOWEEN_2007


#ifdef EVENT_NEW_MOONSTONE
void do_EventNewMoonstone(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	unsigned char subtype;
	msg >> subtype;


	switch(subtype)
	{
	case MSG_EVENT_NEW_MOONSTONE_START_REQ:
		{
			EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_START_REP);
			rmsg << gserver.m_nMoonStoneNas;
			SEND_Q( rmsg, ch->m_desc );
		}
		break;
	case MSG_EVENT_NEW_MOONSTONE_TRY_REQ:
		{
			int nMoonStoneType;
			msg >> nMoonStoneType;

			if(ch->m_nMSGiftIndex != 0 || ch->m_nMSIndex != 0)
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;
				return ;
			}

			int r, c;
			if(!ch->m_invenNormal.FindItem(&r, &c, nMoonStoneType, -1, -1))
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;

				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE);
				rmsg << nMoonStoneType;
				SEND_Q( rmsg, ch->m_desc );
				return ;
			}

			CMoonStoneRewardData* reward = gserver.m_moonstoneReward.Find(nMoonStoneType);
			if(!reward || reward->m_nTotalProb < 2 || reward->m_listReward.GetCount() == 0)
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;
				return ;
			}

			int rand;
			int totalprob = 0;
			rand = GetRandom(1, reward->m_nTotalProb);
			
			void* pos;
			CMoonStoneRewardData::REWARD* rdata;
			pos = reward->m_listReward.GetHead();
			while(pos)
			{
				rdata = reward->m_listReward.GetData(pos);
				pos = reward->m_listReward.GetNext(pos);

				totalprob += (int)(rdata->prob * 10000.0);
				if(rand <= totalprob)
				{
					ch->m_nMSGiftIndex = rdata->rewardindex;
					break;
				}
			}

			if(ch->m_nMSGiftIndex == 0)
			{
				ch->m_nMSGiftIndex = 0;
				ch->m_nMSIndex = 0;
				return ;
			}

			ch->m_nMSIndex = nMoonStoneType;

#ifdef WJKTEST
		//	ch->m_nMSGiftIndex = 19;
#endif // WJKTEST
			const static int maxNas = 10000000;
			int sendnas = 0;
			if(gserver.m_nMoonStoneNas + 200 > maxNas)
			{
				sendnas = maxNas;
			}
			else
			{
				sendnas = gserver.m_nMoonStoneNas + 200;
			}

			EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_TRY_REP);
			rmsg << ch->m_nMSGiftIndex
				 << sendnas;

			SEND_Q( rmsg, ch->m_desc );
		}
		break;
	case MSG_EVENT_NEW_MOONSTONE_RESULT_REQ:
		{
			CItem* moonstone;
			if(ch->m_nMSGiftIndex == 0 || ch->m_nMSIndex == 0)
			{
				ch->m_nMSGiftIndex = 0;
				ch->m_nMSIndex = 0;
				return ;
			}

			// 문스톤 확인
			int r, c;
			if(!ch->m_invenNormal.FindItem(&r, &c, ch->m_nMSIndex, -1, -1))
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;

				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE);
				rmsg << ch->m_nMSIndex;
				SEND_Q( rmsg, ch->m_desc );
				return ;
			}

			moonstone = ch->m_invenNormal.GetItem(r, c);
			if(!moonstone || moonstone->Count() < 1)
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;

				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE);
				rmsg << ch->m_nMSIndex;
				SEND_Q( rmsg, ch->m_desc );
				return ;
			}

			// 보상품 확인
			CMoonStoneRewardData* reward = gserver.m_moonstoneReward.Find(ch->m_nMSIndex);
			if(!reward)
			{
				ch->m_nMSGiftIndex = 0;
				ch->m_nMSIndex = 0;
				return ;
			}
			
			void* pos;
			
			CMoonStoneRewardData::REWARD* rdata;
			rdata = new CMoonStoneRewardData::REWARD;
			rdata->rewardindex = ch->m_nMSGiftIndex;
			pos = reward->m_listReward.FindData(rdata);
			delete rdata;

			if(!pos) 
			{
				ch->m_nMSGiftIndex = 0;
				ch->m_nMSIndex = 0;
				return ;
			}
			rdata = reward->m_listReward.GetData(pos);

			// 문스톤 아이템 감소
			CNetMsg itemmsg;
			DecreaseFromInventory(ch, moonstone, 1);
			if (moonstone->Count() > 0)
			{
				ItemUpdateMsg(itemmsg, moonstone, -1);
				SEND_Q(itemmsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(itemmsg, moonstone);
				SEND_Q(itemmsg, ch->m_desc);
				ch->RemoveItemFromQuickSlot(moonstone);
				RemoveFromInventory(ch, moonstone, true, true);
			}
			
			CNetMsg hmsg;
			HelperEventMoonStoneUpdateReqMsg(hmsg);
			SEND_Q(hmsg, gserver.m_helper);
			
			if(rdata->rewardindex == 19)		// 잭팟
			{
				CNetMsg hmsg;
				HelperEventMoonStoneJackPotReqMsg(hmsg, ch->m_index);
				SEND_Q(hmsg, gserver.m_helper);

				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;

				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_JACK_POT);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			int plus = 0;
			CItemProto* itemproto = gserver.m_itemProtoList.FindIndex(rdata->rewardindex);
			if(!itemproto)
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;
				return ;
			}
			if(itemproto->m_typeIdx == ITYPE_WEAPON) // 무기는 plus 0 ~ 4;
			{
				plus = GetRandom(0, 4);
			}

			CItem* gift;
			gift = gserver.m_itemProtoList.CreateItem(rdata->rewardindex, -1, plus, rdata->rewardflag, rdata->rewardcount);
			if(!gift)
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;
				return ;
			}
			if( AddToInventory(ch, gift, true, true) )
			{
				GAMELOG << init("CHANGE_MOONSTONE", ch)
					<< itemlog(gift)
					<< end;

				if(gift->tab() != -1)
				{
					ItemAddMsg(itemmsg, gift);
				}
				else
				{
					int r,c;
					if(ch->m_invenNormal.FindItem(&r, &c, gift->m_idNum, plus, rdata->rewardflag))
					{
						CItem* item = ch->m_invenNormal.GetItem(r, c);
						ItemUpdateMsg(itemmsg, item, rdata->rewardcount);
						delete gift;
						gift = NULL;
					}
				}
				SEND_Q(itemmsg, ch->m_desc);
			}
			else
			{
				gift = ch->m_pArea->DropItem(gift, ch);
				if (!gift)
				{
					ch->m_nMSGiftIndex = 0;
					ch->m_nMSIndex = 0;
					return;
				}
				CNetMsg dropMsg;
				gift->m_preferenceIndex = ch->m_index;
				ItemDropMsg(dropMsg, ch, gift);
				ch->m_pArea->SendToCell(dropMsg, ch, true);
			}

			ch->m_nMSGiftIndex = 0;
			ch->m_nMSIndex = 0;

			EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_RESULT_REP);
			rmsg << rdata->rewardindex
				 << rdata->rewardcount;

			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_EVENT_NEW_MOONSTONE_MIX_REQ:
		{
			const int MixGradeCount = 4;
			static const int MoonStoneMixGrade[MixGradeCount][3] = { 
				{2545, 2546, 90},   // 문스톤 종류, 조합되는 문스톤, 확률
				{2546, 723, 60},
				{723, 2547, 30},
				{2547, 2548, 10},
			};

			const int MixBoxCount = 5;
			const int MixBoxes[MixBoxCount][4] = {
				{2552, 100, 100,	1},			// 조합상자종류, 증가확률, 소비되는 나스, 실패시 문스톤사라짐
				{2553, 200, 1000,	1},
				{2554, 300, 10000,	1},
				{2555, 400, 0,		1},
				{2556, 500, 0,		0},
			};

			CNetMsg moneyMsg, itemmsg;
			int nMoonStoneIndex;
			int nMoonStoneMixBox;
			msg >> nMoonStoneIndex
				>> nMoonStoneMixBox;

			char cSelMoonStone;
			char cSelMoonStoneBox;
			switch(nMoonStoneIndex)
			{
			case 2545: cSelMoonStone = 0; break;
			case 2546: cSelMoonStone = 1; break;
			case 723:  cSelMoonStone = 2; break;
			case 2547: cSelMoonStone = 3; break;
			default:
				{
					EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_MIX_MISS);
					SEND_Q( rmsg, ch->m_desc );
					return ;
				}
			}

			switch(nMoonStoneMixBox)
			{
			case 2552:	cSelMoonStoneBox = 0; break;
			case 2553:	cSelMoonStoneBox = 1; break;
			case 2554:	cSelMoonStoneBox = 2; break;
			case 2555:	cSelMoonStoneBox = 3; break;
			case 2556:	cSelMoonStoneBox = 4; break;
			default:
				return ;
			}

			CItem* moonstone = NULL;
			CItem* moonstonebox = NULL;
			// 문스톤 있는지 확인
			int row, col;
			if(ch->m_invenNormal.FindItem(&row, &col, nMoonStoneIndex, -1, -1))
			{
				moonstone = ch->m_invenNormal.GetItem(row, col);
				if(!moonstone || moonstone->Count() < 3)
				{
					EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE);
					rmsg << nMoonStoneIndex;
					SEND_Q( rmsg, ch->m_desc );
					return ;
				}
			}
			else
			{
				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE);
				rmsg << nMoonStoneIndex;
				SEND_Q( rmsg, ch->m_desc );
				return ;
			}

			// 문스톤 상자 있는지 확인
			if(ch->m_invenNormal.FindItem(&row, &col, nMoonStoneMixBox, -1, -1))
			{
				moonstonebox = ch->m_invenNormal.GetItem(row, col);
				if(!moonstonebox) return ;
			}
			else
				return ;

			// 나스검사
			if(MixBoxes[cSelMoonStoneBox][2] > 0)
			{
				if(!ch->m_moneyItem || ch->m_moneyItem->Count() < MixBoxes[cSelMoonStoneBox][2])
				{
					SysMsg( rmsg, MSG_SYS_SHORT_MONEY);
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}

			int rand = GetRandom(1, 100);
#ifdef WJKTEST
			rand  = GetRandom(1, MoonStoneMixGrade[cSelMoonStone][2]);
#endif // WJKTEST
			if(rand <= MoonStoneMixGrade[cSelMoonStone][2] * MixBoxes[cSelMoonStoneBox][1]/100)
			{
				// 조합성공
				CItem* upgradeitem = gserver.m_itemProtoList.CreateItem(MoonStoneMixGrade[cSelMoonStone][1], -1, 0, 0, 1);
				if(!upgradeitem) return ;

				if(AddToInventory(ch, upgradeitem, false, true))
				{
					if(upgradeitem->tab() != -1)
					{
						ItemAddMsg(itemmsg, upgradeitem);
					}
					else
					{
						int r,c;
						if(ch->m_invenNormal.FindItem(&r, &c, upgradeitem->m_idNum, 0, 0))
						{
							CItem* item = ch->m_invenNormal.GetItem(r, c);
							ItemUpdateMsg(itemmsg, item, 1);
							delete upgradeitem;
							upgradeitem = item;
						}
					}
					SEND_Q(itemmsg, ch->m_desc);					
				}
				else
				{
					upgradeitem = ch->m_pArea->DropItem(upgradeitem, ch);
					if (!upgradeitem)
						return;

					CNetMsg dropMsg;
					upgradeitem->m_preferenceIndex = ch->m_index;
					ItemDropMsg(dropMsg, ch, upgradeitem);
					ch->m_pArea->SendToCell(dropMsg, ch, true);
				}

				// 문스톤, 문스톤 박스 삭제
				if(moonstone->Count() > 3)
				{
					DecreaseFromInventory(ch, moonstone, 3);
					ItemUpdateMsg(itemmsg, moonstone, -3);
				}
				else
				{
					ItemDeleteMsg(itemmsg, moonstone);
					RemoveFromInventory(ch, moonstone, true , true);
				}
				SEND_Q(itemmsg, ch->m_desc);
				
				if(moonstonebox->Count() > 1)
				{
					DecreaseFromInventory(ch, moonstonebox, 1);
					ItemUpdateMsg(itemmsg, moonstonebox, -1);
				}
				else
				{
					ItemDeleteMsg(itemmsg, moonstonebox);
					RemoveFromInventory(ch, moonstonebox, true, true);
				}
				SEND_Q(itemmsg, ch->m_desc);
				
				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_MIX_SUCCESS);
				rmsg << upgradeitem->m_idNum;
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("MOONSTONE MIX SUCCESS", ch)
						<< "Char Index" << delim << ch->m_index << delim
						<< "NewMoonStone" << delim << upgradeitem->m_idNum
						<< end ;

#ifdef SAVE_MOONSTONE_MIX
				if(upgradeitem->m_idNum == 2548)  // 최상급문스톤
				{
					if(IS_RUNNING_HELPER)
					{
						HelperSaveMoonStoneMix(rmsg, ch->m_index);
						SEND_Q(rmsg, gserver.m_helper);
					}
				}
#endif // SAVE_MOONSTONE_MIX
			}
			else
			{
				// 조합실패
				if(MixBoxes[cSelMoonStoneBox][3])
				{
					// 문스톤 감소
					if(moonstone->Count() > 2)
					{
						DecreaseFromInventory(ch, moonstone, 2);
						ItemUpdateMsg(itemmsg, moonstone, -2);
					}
					else
					{
						ItemDeleteMsg(itemmsg, moonstone);
						RemoveFromInventory(ch, moonstone, true , true);
					}
					SEND_Q(itemmsg, ch->m_desc);
				}

				if(moonstonebox->Count() > 1)
				{
					DecreaseFromInventory(ch, moonstonebox, 1);
					ItemUpdateMsg(itemmsg, moonstonebox, -1);
				}
				else
				{
					ItemDeleteMsg(itemmsg, moonstonebox);
					RemoveFromInventory(ch, moonstonebox, true, true);
				}
				SEND_Q(itemmsg, ch->m_desc);
				
				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_MIX_FAILED);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("MOONSTONE MIX FAILED", ch)
					    << end;
			}

			// 차감
			if(MixBoxes[cSelMoonStoneBox][2] > 0)
			{
				if(ch->m_moneyItem)
				{
					if(ch->m_moneyItem->Count() <= MixBoxes[cSelMoonStoneBox][2])
					{
						ItemDeleteMsg(moneyMsg, ch->m_moneyItem);
						ch->AddMoney(-ch->m_moneyItem->Count());
						
					}
					else if(ch->m_moneyItem->Count() > MixBoxes[cSelMoonStoneBox][2])
					{
						ch->AddMoney(-MixBoxes[cSelMoonStoneBox][2]);
						ItemUpdateMsg(moneyMsg, ch->m_moneyItem, -MixBoxes[cSelMoonStoneBox][2]);
					}
					SEND_Q(moneyMsg, ch->m_desc);
				}
			}
		}
		break;
	default:
		break;
	}
}
#endif // EVENT_NEW_MOONSTONE

#ifdef EVENT_XMAS_2007
void do_EventXmas2007(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	unsigned char subtype;
	msg >> subtype;

	// KYD TO DO
	switch(subtype)
	{
	case MSG_EVENT_XMASTREE_ADD_POINT :		// 포인트 적립 커넥터로
		{
			// 장식 아이템
			const int ItemCount = 6;
			int nXmasItem[ItemCount] = { 2584, 2585, 2586, 2587, 2588, 2589 }; //양말,막대사탕 , 종, 리본, 구슬, 별

			unsigned int unPoint = 0;
			// 아이템 확인
			for( int nItem=0; nItem<ItemCount; nItem++ )
			{
				int row, col;
				if(ch->m_invenNormal.FindItem(&row, &col, nXmasItem[nItem], -1, -1))
				{
					CItem* pTreeDeco = NULL;
					CNetMsg itemMsg;
					pTreeDeco = ch->m_invenNormal.GetItem(row, col);
					if( pTreeDeco && pTreeDeco->Count() >0 )
					{
						unPoint += pTreeDeco->Count();
						ItemDeleteMsg(itemMsg, pTreeDeco);
						RemoveFromInventory(ch, pTreeDeco, true, true);
						SEND_Q( itemMsg, ch->m_desc );
						// 로그 남기기
					}
				}
			}

			// unPoint가 0보다 크면 성공, 작으면 아이템 없음
			EventXmas2007Msg( rmsg, MSG_EVENT_XMASTREE_ADD_POINT );
			if( unPoint > 0 )
				rmsg << (char) 0;
			else
				rmsg << (char) 1;
			SEND_Q( rmsg, ch->m_desc );
			
			if( unPoint > 0 )
			{
				ConnEventXmas2007Msg( rmsg, MSG_CONN_EVENT_XMASTREE_ADDPOINT );
				rmsg << unPoint;
				SEND_Q( rmsg, gserver.m_connector );
			}
		}break;

	case MSG_EVENT_XMASTREE_GET_BLESS :		// 축복 받기
		{
			if( ch->m_assist.FindBySkillIndex( 489 ) == 0 )	// if( 현재 크리스 마스 디버프가 없다. )
			{
				unsigned int unLevel = gserver.m_unEventXmasTreePoint/1000;
				int	nSkillIndex=0;
	
				// nSkillIndex 스킬 주기
				CSkill* pSkill = NULL;
				bool bApply;

				if( unLevel < 1 )	{ nSkillIndex = 0; }
				else if ( unLevel >=1 && unLevel < 3 )		{ nSkillIndex = 485; }
				else if ( unLevel >=3 && unLevel < 15 )		{ nSkillIndex = 486; }
				else if ( unLevel >=15 && unLevel < 100 )	{ nSkillIndex = 487; }
				else if ( unLevel >= 100 )					{ nSkillIndex = 488; }

				for( int i=0; i<2 ; i++ )
				{
					bApply = false;

					pSkill = gserver.m_skillProtoList.Create( nSkillIndex , 1 );
					if( pSkill )
					{						
						ApplySkill( ch, ch, pSkill, -1 , bApply );
						if( bApply ) // 스킬 성공
						{
							delete pSkill;
							pSkill = NULL;
							nSkillIndex = 489;	// 크리스 마스 디버프 스킬로 변경;
						}
						else // 스킬 실패
							break;
					}
					else	// 스킬 실패
						break;	
				}
				
				EventXmas2007Msg( rmsg, MSG_EVENT_XMASTREE_GET_BLESS );
				if(bApply)		// 스킬 2개 성공
				{ rmsg << (char) 0; }
				else			// 스킬 2개 모두는 성공 못했다.
				{ rmsg << (char) 1; }
				SEND_Q( rmsg, ch->m_desc );
			}
			else
			{
				// 현재 이미 같은 스킬이 있음
				EventXmas2007Msg( rmsg, MSG_EVENT_XMASTREE_GET_BLESS );
				rmsg << (char) 1;
				SEND_Q( rmsg, ch->m_desc );
			}
		}break;

	}

	return;
}
#endif //EVENT_XMAS_2007


#ifdef NEWYEAR_EVENT_2008
void do_EventNewYear2008(CPC * ch, CNetMsg & msg)
{
	CSkill * pSkill1 = gserver.m_skillProtoList.Create(492);
	CSkill * pSkill2 = gserver.m_skillProtoList.Create(491);
	if( pSkill1 == NULL || pSkill2 == NULL)
	{
		return;
	}

	bool bApply;
	ApplySkill(ch, ch, pSkill1, -1, bApply);	// 해맞이 버프
	if(bApply)
	{
		ApplySkill(ch, ch, pSkill2, -1, bApply); // 해맞이 피시전자 쿨타임 버프
		if(!bApply)
		{
			delete pSkill2;
			pSkill2 = NULL;
		}
	}
	else
	{
		if(pSkill1)
		{
			delete pSkill1;
			pSkill1 = NULL;
		}
	}
}
#endif // NEWYEAR_EVENT_2008

#if defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)
void do_EventSakura2008( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	unsigned char subtype;
	msg >> subtype;

	// KYD TO DO
	switch(subtype)
	{
	case MSG_EVNET_SAKURAEVENT_2008 :			// 퇴비(2345)를 회수하고 상자(2747)를 제공한다.
		{
			//////////////////////////////////////////////////////////////////////////
			int nRow, nCol;	
			CItem* pSeedItem = NULL;
			int nSeedCount = 0;

			if( !ch->m_invenNormal.FindItem( &nRow, &nCol, 2345/*퇴비*/, 0, 0 ) )			// 퇴비
			{
				EventSakuraEvent2008Msg( rmsg, MSG_EVENT_SAKURAEVENT_2008_ERROR );
				rmsg << (int) 1; // FAILED
				SEND_Q( rmsg, ch->m_desc );
				return;
			}


			pSeedItem = ch->m_invenNormal.GetItem( nRow, nCol );
			if( pSeedItem != NULL )
				nSeedCount = pSeedItem->Count();

			// 벚꽃선물상자 지급
			CItem* pSakuraBox =  gserver.m_itemProtoList.CreateItem( 2747/*벚꽃선물상자*/ , -1, 0, 0, nSeedCount );
			if( !pSakuraBox )
			{
				EventSakuraEvent2008Msg( rmsg, MSG_EVENT_SAKURAEVENT_2008_ERROR );
				rmsg << (int) 1; // FAILED
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

				// 아이템 지급
			if( AddToInventory( ch, pSakuraBox, true, true ) )
			{
				if( pSakuraBox->tab() < 0 )
				{
					delete pSakuraBox;
					ch->m_invenNormal.FindItem( &nRow, &nCol,  2747/*벚꽃선물상자*/, 0, 0 );
					pSakuraBox =  ch->m_invenNormal.GetItem( nRow, nCol );
					ItemUpdateMsg( rmsg, pSakuraBox, nSeedCount );
					SEND_Q( rmsg, ch->m_desc );
				}
				else
				{
					ItemAddMsg( rmsg, pSakuraBox );
					SEND_Q( rmsg, ch->m_desc );
				}

				// 아이템 제거
				if( ch->m_invenNormal.FindItem( &nRow, &nCol, 2345/*퇴비*/, 0, 0 ) )
				{
					pSeedItem = ch->m_invenNormal.GetItem( nRow, nCol );
					if ( pSeedItem != NULL )
					{
						ItemDeleteMsg( rmsg, pSeedItem );
						SEND_Q( rmsg, ch->m_desc );
						RemoveFromInventory( ch, pSeedItem, true, true );
					}
				}
			}
			else //아이템 지급 실패
			{
				EventSakuraEvent2008Msg( rmsg, MSG_EVENT_SAKURAEVENT_2008_ERROR );
				rmsg << (int) 1; // FAILED
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			EventSakuraEvent2008Msg( rmsg, MSG_EVENT_SAKURAEVENT_2008_ERROR );
			rmsg << (int) 0; // SUCESS
			GAMELOG << init("SAKURA EVENT CAHNGE SEED ", ch ) 
					<< "COUNT: " << nSeedCount << end;
			SEND_Q( rmsg, ch->m_desc );
			//////////////////////////////////////////////////////////////////////////

		}break;
	}
}
#endif // defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)

#ifdef CHAOSBALL
void do_ChaosBall(CPC * ch, CNetMsg & msg )
{
	if(ch == NULL)
	{
		return;
	}

	// 전송을 위한 메세지 버퍼
	CNetMsg rmsg;

	// 0. 인벤토리에 빈 공간이 3개 남아야 한다.
	if(ch->m_invenNormal.GetSpace() < 3)
	{
		ChaosBallMsg(rmsg, ERROR_NOSPACE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 1. 아이템 위치 받기
	int row = 0, col = 0;
	msg >> row >> col;

	// 2. 아이템 가져오기
	CItem * pItem = ch->m_invenNormal.GetItem(row, col);
	if(!pItem || pItem->m_idNum != 2843)
	{
		ChaosBallMsg(rmsg, ERROR_ITEM_FAULT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(pItem->Count() < 1)
	{
		ChaosBallMsg(rmsg, ERROR_ITEM_COUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 보상 테이블
	int reqord_item[13][7] = {
		{ 2844,  1,   0, 0,   0, 0,  50,  },	// 특급 제련석
		{ 1416,  1,   0, 0,   0, 0,  300, },	// 플래티늄 아이리스의 축복
		{ 1288,  1,   0, 0,   0, 0,  300, },	// 아이리스의 축복
		{   85,  1,   0, 0,   0, 0,  500, },	// 고급 제련석
		{  971,  1, 973, 1, 972, 1,  700, },	// 경험의 스크롤, 노력의 스크롤, 행운의 스크롤
		{  882,  1, 883, 1, 884, 1,  800, },	// 경험치 증폭제, 숙련도 증폭제, 드롭율 증폭제
		{   45, 10,   0, 0,   0, 0, 1150, },	// 대형 체력회복 물약
		{  724, 10,   0, 0,   0, 0, 1000, },	// 대형 마나회복 물약
		{  498,  5, 500, 5,   0, 0,  900, },	// 12레벨 붉은케르, 12레벨 푸른케르
		{  499,  5, 501, 5,   0, 0,  900, },	// 12레벨 붉은 넨, 12레벨 푸른 넨
		{  786,  3,   0, 0,   0, 0,  900, },	// 부스터
		{  556, 10,   0, 0,   0, 0,  900, },	// 사탕
		{   19, 10000,0, 0,   0, 0, 1600, },	// 10,000나스
	};

	int rate = GetRandom(0, 10000);
	int temp = 0, itemIdx = -1;
	int i;
	for(i = 0; i < 13; i++)
	{
		temp += reqord_item[i][6];
		if(rate < temp)
		{
			itemIdx = i;
			break;
		}
	}
	
	if(itemIdx < 0)
	{
		ChaosBallMsg(rmsg, ERROR_ITEM_COUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int flag = 0;	// 
	if(i == 8 || i == 9)
	{	// 붉은 케르와 푸른 넨 인 경우
		flag = 12;
	}

	// 아이템 지급
	for(int j = 0; j < 6; j+=2)
	{
		if(reqord_item[itemIdx][j+1] != 0)
		{
			if(ch->GiveItem(reqord_item[i][j], 0, flag, reqord_item[i][j+1], false, false) == false)
			{
				ChaosBallMsg(rmsg, ERROR_ITEM_FAULT);
				SEND_Q(rmsg, ch->m_desc);
				
				// 롤백 
				int r, c;
				for( ; j > 0; j-=2)
				{
					if( ch->m_invenNormal.FindItem(&r, &c, reqord_item[i][j]) )
					{
						CItem* item = ch->m_invenNormal.GetItem(r, c);
						if(item)
						{
							CNetMsg itemMsg;
							
							// Item 수량 변경
							DecreaseFromInventory(ch, item, reqord_item[i][j+1]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -reqord_item[i][j+1]);
								SEND_Q(itemMsg, ch->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, ch->m_desc);
								ch->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(ch, item, true, true);
							}
						}
					}
				} // 롤백 end
				
				return;
			}

			GAMELOG << init("CHAOSBALL GIVE", ch) << reqord_item[i][j] << delim << reqord_item[i][j+1] << end;
		}
	}

	// 카오스 볼 제거
	if(pItem->Count() > 1)
	{	// 하나 만 제거
		DecreaseFromInventory(ch, pItem, 1);

		ItemUpdateMsg(rmsg, pItem, -1);
		SEND_Q(rmsg, ch->m_desc);
	}
	else if(pItem->Count() == 1)
	{
		// 완전히 빼기
		ItemDeleteMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);

		RemoveFromInventory(ch, pItem, false, true);
	}

	// 카오스 볼 교환 성공 전송
	ChaosBallMsg(rmsg, ERROR_CHAOSBALL_SUCCESS);
	SEND_Q(rmsg, ch->m_desc);
}
#endif

#ifdef IRIS_POINT
void do_UpdatePlayTime(CPC* ch, CNetMsg& msg )
{
	return;	// 과부하로 인한 사용 금지 [3/18/2009 KwonYongDae]
	/*
	if( ch == NULL ) 
		return;
	
	int connect_min = 0;
	msg >> connect_min;

	if(  (connect_min - ch->m_connect_min) < 0 || (connect_min - ch->m_connect_min) % 60 )
		return;
	

	// 시간당  한번 클라이언트에서 보내준다.
	if( IS_RUNNING_CONN )
	{
		CNetMsg rmsg;
		ConnIPointAddMsg(rmsg, ch->m_desc->m_index, ch->m_index, MSG_IPOINT_TYPE_PLAYTIME, 0 );
		SEND_Q( rmsg, gserver.m_connector );
		ch->m_connect_min = connect_min;
	}
	*/
}
#endif //IRIS_POINT


#ifdef EVENT_PHOENIX// yhj   081105  피닉스 
void do_EventPhoenix( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	int r, c;

	if (ch == NULL)
		return;	

	CPC * tch = gserver.m_playerList.Find(ch->m_index);
	if ( tch == NULL )
		return;

	// 이미 피닉스로 등록이 되어 있다.
	if ( ch->m_bPhoenix_Char == 1 )
	{
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_ALREADY_APPLY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 현재 캐릭터의 레벨이 31을 초과하여 피닉스 이벤트 자격이 안된다.
	else if ( ch->m_level >= 31 )
	{
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_REQUIREMENT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인벤토리 가능 검사
	else if ( ch->m_invenNormal.GetSpace() < 12 )
	{
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_FULL_INVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인벤토리 무게 검사
	else if (ch->m_weight + 3051 >= ch->m_maxWeight * 15 / 10)
	{
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_FULL_INVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	// 인벤토리에 피닉스 클럽 가입권이 존재하는 지 확인
	else if ( !ch->m_invenNormal.FindItem(&r, &c, 3217, 0, 0) )
	{
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_REQUIREMENT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 모든 상황을 통과하였다면 캐릭터 DB로 검색해 보기 위해 HELPER SERVER로 패킷을 보낸다.
	else
	{
		HelperEventPhoenixMsg( rmsg, ch->m_desc->m_index, ch->m_index );
		SEND_Q(rmsg, gserver.m_helper);
		return;
	}
	
}

void do_EventPhoenix_Success( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	int job_index = 0;
	int ra, ca;

	const static int phoenixItem_table[6][10] = {
				{ 3218, 3262, 3006, 3221, 3222, 3231, 3232, 3233, 3234, 3235 },		// 타이탄
				{ 3218, 3262, 3007, 3219, 3220, 3236, 3237, 3238, 3239, 3240 },		// 나이트 
				{ 3218, 3262, 3008, 3223, 3224, 3242, 3243, 3244, 3245, 3246 },		// 힐러
				{ 3218, 3262, 3009, 3225, 3226, 3247, 3248, 3249, 3250, 3251 },		// 메이지
				{ 3218, 3262, 3010, 3227, 3228, 3252, 3253, 3254, 3255, 3256 },		// 로그
				{ 3218, 3262, 3011, 3229, 3230, 3257, 3258, 3259, 3260, 3261 } };	// 소서러 

	if ( ch == NULL )
		return;

	CPC * tch = gserver.m_playerList.Find(ch->m_index);
	if ( tch == NULL )
		return;

	// 인벤토리 가능 검사
	if ( ch->m_invenNormal.GetSpace() < 12 )
	{
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_FULL_INVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인벤토리 무게 검사
	else if (ch->m_weight + 3051 >= ch->m_maxWeight * 15 / 10)
	{
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_FULL_INVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	// 인벤토리에 피닉스 클럽 가입권이 존재하는 지 확인
	else if ( !ch->m_invenNormal.FindItem(&ra, &ca, 3217, 0, 0) )
	{
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_REQUIREMENT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//유저의 직업을 찾음, 피닉스 날개를 주고 피닉스 무기와 착용아이템을 주기 위해서 
	switch( ch->m_job )
	{
	case JOB_TITAN:
		job_index = 0;
		break;

	case JOB_KNIGHT:
		job_index = 1;
		break;

	case JOB_HEALER:
		job_index = 2;
		break;

	case JOB_MAGE:
		job_index = 3;
		break;

	case JOB_ROGUE:
		job_index = 4;
		break;

	case JOB_SORCERER:
		job_index = 5;
		break;

	default:
		return;
		break;
	}

	// 피닉스 아이템 지급 
	for ( int i = 0; i < 10; i++ )
	{
		CItem* pItem;

		if ( i < 3 )
		{
			pItem = gserver.m_itemProtoList.CreateItem(phoenixItem_table[job_index][i], -1, 0, 0, 1);
		}
		else
		{
			pItem = gserver.m_itemProtoList.CreateItem(phoenixItem_table[job_index][i], -1, 4, 0, 1);
		}

		if ( pItem == NULL )
			goto ITEM_FAIL;

		if( !AddToInventory(ch, pItem, true, false) )
		{
			delete pItem;
			pItem = NULL;
			goto ITEM_FAIL;
		}
		else
		{
			ItemAddMsg(rmsg, pItem);
			SEND_Q(rmsg, ch->m_desc);
			pItem = NULL;
		}
	}
	
	// 직업이 기사이면 방패도 지급
	if ( ch->m_job == JOB_KNIGHT )
	{
		// 피닉스 방패
		CItem* pItem_shield = gserver.m_itemProtoList.CreateItem(3241, -1, 4, 0, 1);

		if ( pItem_shield == NULL )
			goto ITEM_FAIL;

		if( !AddToInventory(ch, pItem_shield, true, false) )
		{
			delete pItem_shield;
			pItem_shield = NULL;
			goto ITEM_FAIL;
		}
		else
		{
			ItemAddMsg(rmsg, pItem_shield);
			SEND_Q(rmsg, ch->m_desc);
			pItem_shield = NULL;
		}
	}

	// 버프 효과 보이기
	CSkill* pSkill1;
	pSkill1 = gserver.m_skillProtoList.Create( 516, 1 );
	if(pSkill1 != NULL)
	{
		bool bApply = false;
		ApplySkill(ch, ch, pSkill1, -1 , bApply);
		if(bApply == false)
		{
			GAMELOG << init("EVENT_PHOENIX SKILL FAILED ") << ch->m_name << end;// 스킬 적용 실패
		}
	}

	//레벨 업
	int remain_level;
	remain_level = 35 - ch->m_level;

#ifdef EVENT_NOM_MOVIE
	int nOldLevel;
	nOldLevel = ch->m_level;
#endif

	// 35까지 남은 레벨 지급,  경험치는 0으로 조정
	ch->m_exp = 0;
	ch->m_level += remain_level;
	ch->LevelUp(true);

	// statpoint +30 지급에 35 까지 남은 레벨에 대한 statpoint 지급
	ch->m_statpt_remain = ch->m_statpt_remain + 30 + remain_level;
	StatPointRemainMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);

	// 숙련도는 4000에서 레벨에 따라 차등 지급
	ch->m_skillPoint += 6000000 + (1000000 * remain_level);

	// 케릭터 스텟 리프레쉬 
	ch->InitStat();
	ch->CalcStatus(true);
	ch->Restore();

	// 다음 영화 이벤트를 위해 놈놈놈 이벤트 추가
#ifdef EVENT_NOM_MOVIE
	if (nOldLevel < 20 && ch->m_level >= 20)
	{
		if (IS_RUNNING_CONN)
		{
			CNetMsg rmsg;
			ConnEventNomMsg(rmsg, MSG_CONN_EVENT_NOM_REQ, ch->m_desc->m_index, ch->m_index);
			SEND_Q(rmsg, gserver.m_connector);
		}
	}
#endif // EVENT_NOM_MOVIE

// 같은 파티 원에게도 레벨업 보여주기
#ifdef PARTY_MATCHING
	if (IS_RUNNING_HELPER)
	{
		HelperPartyMatchMemberChangeInfoMsg(msg, ch->m_index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL, "", ch->m_level, 0);
		SEND_Q(msg, gserver.m_helper);
#ifdef LC_HBK
		HelperLevelUpLogMsg(msg, ch);
		SEND_Q(msg, gserver.m_helper);
#endif // LC_HBK
	}
#endif // PARTY_MATCHING

	// 피닉스 클럽 가입권을 삭제한다.
	int r, c;
	if ( ch->m_invenNormal.FindItem(&r, &c, 3217, 0, 0) )
	{
		CItem* item = ch->m_invenNormal.GetItem(r, c);
		if( item )
		{
			CNetMsg rmsg;
			DecreaseFromInventory(ch, item, 1);
		
			if (item->Count() > 0)
			{
				ItemUpdateMsg(rmsg, item, -1);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, item);
				SEND_Q(rmsg, ch->m_desc);
				ch->RemoveItemFromQuickSlot(item);
				RemoveFromInventory(ch, item, true, true);
			}
		}
	}
	
	// 캐릭터를 피닉스로 업데이트
	ch->m_bPhoenix_Char = 1;
	
	EventPhoenix_SuccessMsg(rmsg);
	SEND_Q(rmsg, ch->m_desc);
	return;

ITEM_FAIL:

	for ( int j = 0; j < 10; j++ )
	{
		if ( j < 3 )
		{
			if( ch->m_invenNormal.FindItem(&r, &c, phoenixItem_table[job_index][j], 0, 0) )
			{
				CItem* item = ch->m_invenNormal.GetItem(r, c);
				if( item )
				{
					CNetMsg rmsg;		

					// Item 수량 변경
					DecreaseFromInventory(ch, item, 1);

					ItemDeleteMsg(rmsg, item);
					SEND_Q(rmsg, ch->m_desc);
					ch->RemoveItemFromQuickSlot(item);
					RemoveFromInventory(ch, item, true, true);
				}
			}
		}
		else
		{
			if( ch->m_invenNormal.FindItem(&r, &c, phoenixItem_table[job_index][j], 4, 0) )
			{
				CItem* item = ch->m_invenNormal.GetItem(r, c);
				if( item )
				{
					CNetMsg rmsg;		

					// Item 수량 변경
					DecreaseFromInventory(ch, item, 1);

					ItemDeleteMsg(rmsg, item);
					SEND_Q(rmsg, ch->m_desc);
					ch->RemoveItemFromQuickSlot(item);
					RemoveFromInventory(ch, item, true, true);
				}
			}

		}
		
	} // for
}

#endif //EVENT_PHOENIX

#ifdef REQUITAL_EVENT	// 보상 이벤트 
void do_EventRequital( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	int	eventType = -1;

	if (ch == NULL)
		return;	

	CPC * tch = gserver.m_playerList.Find(ch->m_index);
	if ( tch == NULL )
		return;

	msg >> eventType;

	// 인벤토리 가능 검사
	if ( ch->m_invenNormal.GetSpace() < 4 )
	{
		EventRequitalMsg(rmsg, MSG_EVENT_REQUITAL_ERROR_FULL_INVEN);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인벤토리 무게 검사
	else if (ch->m_weight + 10 >= ch->m_maxWeight * 15 / 10)
	{
		EventRequitalMsg(rmsg, MSG_EVENT_REQUITAL_ERROR_TOO_HEAVY_INVEN);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

#ifdef ADULT_SERVER
	else if ( eventType == 0 )
	{
		EventRequitalMsg(rmsg, MSG_EVENT_REQUITAL_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
#endif // ADULT_SERVER

	// 모든 상황을 통과하였다면 유저 DB로 검색해 보기 위해 Connector SERVER로 패킷을 보낸다.
	else
	{
		if (IS_RUNNING_CONN)
		{			
			ConnEventRequitalMsg( rmsg, ch->m_desc->m_index, ch->m_index, eventType, ch->m_desc->m_idname, ch->m_name );
			SEND_Q(rmsg, gserver.m_connector);
		}
	}
}
#endif // REQUITAL_EVENT