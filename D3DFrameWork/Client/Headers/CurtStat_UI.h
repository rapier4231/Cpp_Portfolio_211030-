#pragma once
#ifndef __CurtStat_UI_H__
#define __CurtStat_UI_H__

#include "Value.h"
#include "OrthoUI.h"

class CCurtStat_UI : public COrthoUI
{

private:
	explicit CCurtStat_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCurtStat_UI(const CCurtStat_UI& rhs);
	virtual ~CCurtStat_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	_bool					Set_Equip(ITEMID _eItemID, CUnit* _pOwnerInven = nullptr);



public:
	static CCurtStat_UI*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;

public:
	virtual  CInvenItem*		CreateDragEquipSlot(POINT _pPos, _int _iIdentityNum);
	virtual _int				DeleteDropEquipSlot(POINT _pPos, _int _iIdentityNum);

public:
	virtual void			Match_EquipTex_Info() override;
	void					RemoveEquip(_int _iItemNum);

private:
	void					Inventory_Render_On();
	void					Inventory_Render_Off();		

	//vector<CInvenItem*>			m_vecEquipSlot; 이건 슬롯 관리 하는 친구
	vector<ITEM_INFO>		m_vecEquipItem; //얘가 플레이어 아이템 관리하는 친구

protected:
	_int					m_iDrag_DropSlotNumber = 0;
	CInvenItem*				m_pDrag_DropEquipSlot = nullptr;
	CBorder*				m_pDrag_DropEquipBorder = nullptr;
};

#endif // !__CurtStat_UI_H__