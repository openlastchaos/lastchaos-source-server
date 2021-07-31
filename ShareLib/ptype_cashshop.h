#ifndef __PTYPE_CASHSHOP_H__
#define __PTYPE_CASHSHOP_H__

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	struct cash_purchase_bring : public pTypeThirdBase
	{
		unsigned char		count_;
		struct data_tag  
		{
			int				purchase_index_;
			int				ct_item_index_;
		} data_[0];
	};

	struct cash_gift_bring : public pTypeThirdBase
	{
		unsigned char		count_;
		struct data_tag
		{
			int				purchase_index_;
			int				ct_item_index_;
		} data_[0];
	};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
