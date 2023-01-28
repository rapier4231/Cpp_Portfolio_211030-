#pragma once
#ifndef __Inven_H__
#define __Inven_H__

#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CInven : public CComponent
{
private:
	explicit CInven(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInven(const CInven& _rhs);
	virtual ~CInven() = default;

public:
	virtual HRESULT			Ready_Component_Prototype() override;
	virtual HRESULT			Ready_Component(void* _pArg = nullptr) override;
	virtual _int			Update_Component(const _double& _dTimeDelta) override;

//public:
//	void					Ready_Inven(InvenUserUnitTypeID _eInvenUserUnitTypeID);

public:
	static CInven*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*		Clone(void * pArg = nullptr) override;
	virtual void			Free() override;

public:


public:
	void							Add_Item(const ITEM_INFO& _tItemInfo);
	void							Add_Item(ITEMID _eItemID, _int _iAmount);
	_bool							Remove_Item(_int _iInvenNum);
	ITEM_INFO						Remove_Item(ITEMID _eItemID, _int _iAmount);
	_bool							UisngRemove_Item(_int _iInvenNum, _int _iAmount);
	void							ChangeItem(const ITEM_INFO& _tChangeSlot, _int _iSlotNum);
	_int							Now_Have_ItemAmount(ITEMID _eItemID);
	void							Making_Item_Remove_ingredient(const MAKING_INFO& _tCheckDeficientItem);

public:
	InvenUserUnitTypeID				Get_InvenUserUnitTypeID() { return m_eInvenUserUnitTypeID; }
	void							Set_InvenUserUnitTypeID(InvenUserUnitTypeID _eInvenUserUnitTypeID) {m_eInvenUserUnitTypeID = _eInvenUserUnitTypeID;}

public:
	void							Open_Inventory();
	vector<ITEM_INFO>*				Get_pInvenItem();
	ITEM_INFO						Get_pInvenItem(_int _iItemNumber);

private:
	void							Item_Set_Identity_Number();
private:
	InvenUserUnitTypeID					m_eInvenUserUnitTypeID = InvenUserUnitTypeID_END;
	vector<ITEM_INFO>					m_vecInvenItem;
	_int								m_iItemAllSlotNum = 45; //나중에 인벤 슬롯 확장 할 시, 0으로 하고 겟 셋 만들 것!
	_int								m_NowItemAmount = 0;
};
END
#endif // !__Inven_H__
