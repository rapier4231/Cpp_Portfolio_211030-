#include "stdafx.h"
#include "..\Headers\Stat_UI.h"

#include "Export_Function.h"
#include "InvenItem.h"
#include "Inven.h"
#include "Unit.h"
#include "Border.h"
#include "LR_Inventory.h"
#include "Left_Inventory.h"
#include "CenterTap.h"
USING(Engine)

CStat_UI::CStat_UI(LPDIRECT3DDEVICE9 _pDevice)
	: CCenter_UI(_pDevice)
{
}

CStat_UI::CStat_UI(const CStat_UI & rhs)
	: CCenter_UI(rhs)
{
}

HRESULT CStat_UI::Ready_GameObject_Prototype()
{
	CCenter_UI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CStat_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CCenter_UI::Ready_GameObject(_pArg);
	m_vecEquipItem.resize(EQUIP_Position::EQUIP_Position_END);
	for (_int i = 0; i < EQUIP_Position_END; ++i)
	{
		m_vecEquipItem[i].iPosition = i;
	}
	return S_OK;
}

_int CStat_UI::Update_GameObject(const _double & _dTimeDelta)
{
	CCenter_UI::Update_GameObject(_dTimeDelta);
	if (!m_pUnit)
	{
		m_pUnit = static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0));

		//Test 용
		//Set_Equip(ITEMID::ITEMID_Cloth_Hat);
		//Set_Equip(ITEMID::ITEMID_Cloth_Gloves);
		//Set_Equip(ITEMID::ITEMID_Cloth_TOP);
		//Set_Equip(ITEMID::ITEMID_Leather_TOP);
		//Set_Equip(ITEMID::ITEMID_Leather_Legs);
		//Set_Equip(ITEMID::ITEMID_Cloth_Legs);
		//Set_Equip(ITEMID::ITEMID_Leather_Gloves);
		//Set_Equip(ITEMID::ITEMID_Leather_Boots);
		//Set_Equip(ITEMID::ITEMID_Wood_Sheild);
	}

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

_int CStat_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CCenter_UI::LateUpdate_GameObject(_dTimeDelta);

	if (m_pDrag_DropEquipSlot)
	{
		m_pDrag_DropEquipSlot->LateUpdate_GameObject(_dTimeDelta);
		m_pDrag_DropEquipBorder->LateUpdate_GameObject(_dTimeDelta);
	}

	return 0;
}

void CStat_UI::Render_GameObject()
{

}

_bool CStat_UI::Set_Equip(ITEMID _eItemID,CUnit* _pOwnerInven)
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

void CStat_UI::Stat_UI_Open()
{
	//CCenter_UI::Stat_UI_Open();
	Inventory_Render_On();
	static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Left_InventoryOpen();
}

void CStat_UI::Stat_UI_Close()
{
	CCenter_UI::Stat_UI_Close();
}

CStat_UI * CStat_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CStat_UI*	pInstance = new CStat_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CStat_UI::Clone(void * _pArg/* = nullptr*/)
{
	CStat_UI*	pClone = new CStat_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CStat_UI::Free()
{
	if (m_pDrag_DropEquipSlot)
		Safe_Release(m_pDrag_DropEquipSlot);

	CCenter_UI::Free();
}

CInvenItem* CStat_UI::CreateDragEquipSlot(POINT _pPos, _int _iIdentityNum)
{
	m_iDrag_DropSlotNumber = _iIdentityNum;
	CInvenItem* pInvenItem = nullptr;
	pInvenItem = static_cast<CInvenItem*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_InvenItem"));
	pInvenItem->Set_IdentifyNumber(_iIdentityNum);
	pInvenItem->Set_Pos_UI((_float)_pPos.x, (_float)_pPos.y);
	pInvenItem->Set_FirstPos(_pPos);
	pInvenItem->Set_DragItem();
	pInvenItem->Set_pSlotOwner(this, Stat_Player);
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

_int CStat_UI::DeleteDropEquipSlot(POINT _pPos, _int _iIdentityNum)
{
	//if (_pPos.x >= 517
	//	&& _pPos.x <= 717
	//	&& _pPos.y >= 137
	//	&& _pPos.y <= 493)
	//{
	//	static_cast<CInven*>(static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Get_InvenOwner()->Get_Component(L"Com_Inven", ID_DYNAMIC))->Add_Item(m_vecEquipItem[_iIdentityNum]);

	//	ITEM_INFO tNewItem;
	//	m_vecEquipItem[_iIdentityNum] = tNewItem;
	//}
	//else
	//{
	//	if (_pPos.x >= 77
	//		&& _pPos.x <= 277
	//		&& _pPos.y >= 137
	//		&& _pPos.y <= 493)
	//	{
	//		static_cast<CInven*>(static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_InvenOwner()->Get_Component(L"Com_Inven", ID_DYNAMIC))->Add_Item(m_vecEquipItem[_iIdentityNum]);

	//		ITEM_INFO tNewItem;
	//		m_vecEquipItem[_iIdentityNum] = tNewItem;
	//	}
	//}
	Match_EquipTex_Info();

	return 0;
}

void CStat_UI::Match_EquipTex_Info()
{
	_int iCheckItemNum = EQUIP_Position::EQUIP_Position_END; //플레이어 꺼니까 6개 고정

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

void CStat_UI::RemoveEquip(_int _iItemNum)
{
	ITEM_INFO tNewItem;
	tNewItem.iPosition = m_vecEquipItem[_iItemNum].iPosition;
	m_vecEquipItem[_iItemNum] = tNewItem;
	m_vecEquipSlot[_iItemNum]->Set_ItemTexture(nullptr);
	m_vecEquipSlot[_iItemNum]->Set_ItemInfo(&m_vecEquipItem[_iItemNum]);
}

void CStat_UI::Inventory_Render_On()
{
	m_bUpdateEquipSlot = false;
	Match_EquipTex_Info();
	for (auto& iter : *(Engine::Get_Layer(L"Stat_UI")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOn();
	for (auto& iter : *(Engine::Get_Layer(L"Stat_UI_ItemSlot")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOn();
	static_cast<CCenterTap*>(Get_GameObject(L"Stat_UI", 2))->Set_TapOwner((_int)CENTER_STAT_UI);
}

void CStat_UI::Inventory_Render_Off()
{
	for (auto& iter : *(Engine::Get_Layer(L"Stat_UI")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOff();
	for (auto& iter : *(Engine::Get_Layer(L"Stat_UI_ItemSlot")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOff();
	m_bUpdateEquipSlot = true;
}
