#pragma once
#ifndef __MonsterStat_UI_H__
#define __MonsterStat_UI_H__

#include "Value.h"
#include "Center_UI.h"

class CMonsterStat_UI : public CCenter_UI
{

private:
	explicit CMonsterStat_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMonsterStat_UI(const CMonsterStat_UI& rhs);
	virtual ~CMonsterStat_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	_bool					Set_Equip(ITEMID _eItemID, CUnit* _pOwnerInven = nullptr);

public:
	void							Stat_UI_Open(CUnit* _pMonster);
	virtual void					Stat_UI_Close() override;

public:
	static CMonsterStat_UI*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;

public:
	virtual  CInvenItem*		CreateDragEquipSlot(POINT _pPos, _int _iIdentityNum);
	virtual _int				DeleteDropEquipSlot(POINT _pPos, _int _iIdentityNum);

public:
	virtual void			Match_EquipTex_Info() override;
	void					RemoveEquip(_int _iItemNum);

public:
	vector<ITEM_INFO>*		Get_vecEquipItem_Pointer() { return &m_vecEquipItem; }

public:
	void					Inventory_Render_On(CUnit* pMonster = nullptr);
	void					Inventory_Render_Off(_bool _bDelete_pUnit = true);		

public:
	CUnit*					Get_pMonster() { return m_pMonster; }


private:
	CUnit*					m_pMonster = nullptr;

	//vector<CInvenItem*>			m_vecEquipSlot; 이건 슬롯 관리 하는 친구
	vector<ITEM_INFO>		m_vecEquipItem; //얘가 플레이어 아이템 관리하는 친구

protected:
	_int					m_iDrag_DropSlotNumber = 0;
	CInvenItem*				m_pDrag_DropEquipSlot = nullptr;
	CBorder*				m_pDrag_DropEquipBorder = nullptr;
};

#endif // !__MonsterStat_UI_H__