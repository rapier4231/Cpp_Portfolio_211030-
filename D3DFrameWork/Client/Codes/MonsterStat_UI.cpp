#include "stdafx.h"
#include "..\Headers\MonsterStat_UI.h"

#include "Export_Function.h"
#include "InvenItem.h"
#include "Inven.h"
#include "Unit.h"
#include "CenterTap.h"
#include "Border.h"
#include "LR_Inventory.h"
#include "Right_Inventory.h"
#include "Player.h"
#include "Dino.h"
#include "StructureStat_UI.h"

USING(Engine)

CMonsterStat_UI::CMonsterStat_UI(LPDIRECT3DDEVICE9 _pDevice)
	: CCenter_UI(_pDevice)
{
}

CMonsterStat_UI::CMonsterStat_UI(const CMonsterStat_UI & rhs)
	: CCenter_UI(rhs)
{
}

HRESULT CMonsterStat_UI::Ready_GameObject_Prototype()
{
	CCenter_UI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CMonsterStat_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CCenter_UI::Ready_GameObject(_pArg);
	m_vecEquipItem.resize(EQUIP_HAT);
	for (_int i = 0; i < EQUIP_HAT; ++i)
	{
		m_vecEquipItem[i].iPosition = i;
	}
	return S_OK;
}

_int CMonsterStat_UI::Update_GameObject(const _double & _dTimeDelta)
{
	//if (!(static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Payer", 0))->Get_pPlayerLookUnit()))
	//	m_pUnit = nullptr;
	if (!m_pUnit)
		return NO_EVENT;
	CCenter_UI::Update_GameObject(_dTimeDelta);


/*	if (Engine::KeyDown(KeyCheck_I)&&!m_bUpdateEquipSlot)
	{
		Inventory_Render_On();
		m_bUpdateEquipSlot = true;
	}
	else */
	//if (Engine::KeyDown(KeyCheck_I)&& m_bUpdateEquipSlot)
	//{
	//	Inventory_Render_Off();
	//	m_bUpdateEquipSlot = false;
	//}

	//if (Engine::KeyDown(KeyCheck_I))
	//{
	//	Inventory_Render_On();
	//	m_bUpdateEquipSlot = true;
	//}
	//if (Engine::KeyDown(KeyCheck_0))
	//{
	//	Inventory_Render_Off();
	//	m_bUpdateEquipSlot = false;
	//}
	//////////////////////////////////////////////////////////////////
	if (m_pDrag_DropEquipSlot)
	{
		if (OBJECT_DEAD == m_pDrag_DropEquipSlot->Update_GameObject(_dTimeDelta))
		{
			Match_EquipTex_Info();
			Safe_Release(m_pDrag_DropEquipSlot);
			Safe_Release(m_pDrag_DropEquipBorder);
			m_pDrag_DropEquipSlot = nullptr;
			m_pDrag_DropEquipBorder = nullptr;
		}
		else
		{
			m_pDrag_DropEquipBorder->Update_GameObject(_dTimeDelta);
		}
	}
	return 0;
}

_int CMonsterStat_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CCenter_UI::LateUpdate_GameObject(_dTimeDelta);

	if (m_pDrag_DropEquipSlot)
	{
		m_pDrag_DropEquipSlot->LateUpdate_GameObject(_dTimeDelta);
		m_pDrag_DropEquipBorder->LateUpdate_GameObject(_dTimeDelta);
	}

	return 0;
}

void CMonsterStat_UI::Render_GameObject()
{

}

_bool CMonsterStat_UI::Set_Equip(ITEMID _eItemID,CUnit* _pOwnerInven)
{
	ITEM_INFO _tEquipItem = Engine::Clone_Item_Prototype(_eItemID,1);

	if (ITEMID_Equip != (_tEquipItem.iItemType))
		return false;

	if (0 < m_vecEquipItem[_tEquipItem.iPosition].iAmount) //장착하고 있었다는 뜻!
	{
		if(nullptr == _pOwnerInven)
			static_cast<CInven*>(m_pUnit->Get_Component(L"Com_Inven", ID_DYNAMIC))->Add_Item(m_vecEquipItem[_tEquipItem.iPosition]);
		else
			static_cast<CInven*>(_pOwnerInven->Get_Component(L"Com_Inven", ID_DYNAMIC))->Add_Item(m_vecEquipItem[_tEquipItem.iPosition]);
	}

	m_vecEquipItem[_tEquipItem.iPosition] = _tEquipItem;
	m_vecEquipItem[_tEquipItem.iPosition].iAmount = 1;
	Match_EquipTex_Info();
	return true;
}

void CMonsterStat_UI::Stat_UI_Open(CUnit* _pMonster)
{
	//CCenter_UI::MonsterStat_UI_Open();
	CInven*	pInven = nullptr;
	if (_pMonster->Get_Component(L"Com_Show_Inven", ID_DYNAMIC))
		pInven = static_cast<CInven*>(_pMonster->Get_Component(L"Com_Show_Inven", ID_DYNAMIC));
	else
		pInven = static_cast<CInven*>(_pMonster->Get_Component(L"Com_Inven", ID_DYNAMIC));
	
	if (pInven->Get_InvenUserUnitTypeID() == InvenUserUnitTypeID_Monster)
	{
		Inventory_Render_On(_pMonster);
		static_cast<CRight_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Right_InventoryOpen();
	}
	else if (InvenUserUnitTypeID_Campfire <= pInven->Get_InvenUserUnitTypeID() && InvenUserUnitTypeID_Harvestable > pInven->Get_InvenUserUnitTypeID())
	{
		static_cast<CStructureStat_UI*>(Engine::Get_GameObject(L"Center_UI", 2))->Inventory_Render_On(pInven->Get_InvenUserUnitTypeID(),_pMonster);
		static_cast<CRight_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Right_InventoryOpen();
	}

}

void CMonsterStat_UI::Stat_UI_Close()
{
	CCenter_UI::Stat_UI_Close();
}

CMonsterStat_UI * CMonsterStat_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CMonsterStat_UI*	pInstance = new CMonsterStat_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CMonsterStat_UI::Clone(void * _pArg/* = nullptr*/)
{
	CMonsterStat_UI*	pClone = new CMonsterStat_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CMonsterStat_UI::Free()
{
	if (m_pDrag_DropEquipSlot)
		Safe_Release(m_pDrag_DropEquipSlot);

	CCenter_UI::Free();
}

CInvenItem* CMonsterStat_UI::CreateDragEquipSlot(POINT _pPos, _int _iIdentityNum)
{
	m_iDrag_DropSlotNumber = _iIdentityNum;
	CInvenItem* pInvenItem = nullptr;
	pInvenItem = static_cast<CInvenItem*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_InvenItem"));
	pInvenItem->Set_IdentifyNumber(_iIdentityNum);
	pInvenItem->Set_Pos_UI((_float)_pPos.x, (_float)_pPos.y);
	pInvenItem->Set_FirstPos(_pPos);
	pInvenItem->Set_DragItem();
	pInvenItem->Set_pSlotOwner(this, Stat_Monster);
	pInvenItem->Set_ItemInfo(&m_vecEquipItem[m_iDrag_DropSlotNumber]);
	pInvenItem->Set_ItemTexture(Get_pEquipTexture(m_vecEquipItem[m_iDrag_DropSlotNumber].tchName));
	pInvenItem->Get_ORTHOUI_INFO()->bRender = true;

	POINT tMouse;
	Engine::Get_pMousePos(&tMouse);
	pInvenItem->Set_MouseClickPos(&tMouse);
	m_pDrag_DropEquipSlot = pInvenItem;

	m_pDrag_DropEquipBorder = static_cast<CBorder*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pInvenItem->Get_ORTHOUI_INFO()));
	m_pDrag_DropEquipBorder->Set_BorderRenderID(RENDER_ITEMUI);

	return nullptr;
}

_int CMonsterStat_UI::DeleteDropEquipSlot(POINT _pPos, _int _iIdentityNum)
{

	Match_EquipTex_Info();

	return 0;
}

void CMonsterStat_UI::Match_EquipTex_Info()
{
	_int iCheckItemNum = EQUIP_Position::EQUIP_HAT; 

	for (_int i = 0; i < iCheckItemNum; ++i)
	{
		if (m_vecEquipItem[i].iAmount)
		{
			m_vecEquipSlot[i]->Set_ItemInfo(&(m_vecEquipItem[i]));
			m_vecEquipSlot[i]->Set_ItemTexture(Get_pEquipTexture(m_vecEquipItem[i].tchName));
		}
		else
		{
			m_vecEquipSlot[i]->Set_ItemInfo(nullptr);
			m_vecEquipSlot[i]->Set_ItemTexture(nullptr);
		}
			m_vecEquipSlot[i]->Set_SlotNum(i);
	}
}

void CMonsterStat_UI::RemoveEquip(_int _iItemNum)
{
	ITEM_INFO tNewItem;
	tNewItem.iPosition = m_vecEquipItem[_iItemNum].iPosition;
	m_vecEquipItem[_iItemNum] = tNewItem;
	m_vecEquipSlot[_iItemNum]->Set_ItemTexture(nullptr);
	m_vecEquipSlot[_iItemNum]->Set_ItemInfo(&m_vecEquipItem[_iItemNum]);
}

void CMonsterStat_UI::Inventory_Render_On(CUnit* pMonster)
{
	if (!pMonster)
	{
		if (!m_pMonster)
			return;
	}
	else
		m_pMonster = pMonster;

	m_bUpdateEquipSlot = false;
	Match_EquipTex_Info();
	for (auto& iter : *(Engine::Get_Layer(L"MonsterStat_UI")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOn();
	static_cast<CCenterTap*>(Get_GameObject(L"Stat_UI" , 2))->Set_TapOwner((_int)CENTER_MONSTER_UI, m_pMonster);
	static_cast<CCenterTap*>(Get_GameObject(L"Stat_UI", 2))->Set_DaeSangTap();
	static_cast<CCenterTap*>(Get_GameObject(L"Stat_UI", 2))->UI_RenderOn();
	for (auto& iter : *(Engine::Get_Layer(L"MonsterStat_UI_ItemSlot")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOn();
}

void CMonsterStat_UI::Inventory_Render_Off(_bool _bDelete_pUnit)
{
	if(_bDelete_pUnit)
		m_pMonster = nullptr;
	for (auto& iter : *(Engine::Get_Layer(L"MonsterStat_UI")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOff();
	for (auto& iter : *(Engine::Get_Layer(L"MonsterStat_UI_ItemSlot")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOff();
	m_bUpdateEquipSlot = true;
	static_cast<CCenterTap*>(Get_GameObject(L"Stat_UI", 2))->UI_RenderOff();
}
