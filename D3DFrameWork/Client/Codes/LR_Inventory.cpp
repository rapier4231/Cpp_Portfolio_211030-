#include "stdafx.h"
#include "..\Headers\LR_Inventory.h"

#include "Export_Function.h"
#include "Unit.h"
#include "InvenItem.h"
#include "Inven.h"
#include "Border.h"
#include "Stat_UI.h"
USING(Engine)

CLR_Inventory::CLR_Inventory(LPDIRECT3DDEVICE9 _pDevice)
	: CScroll(_pDevice)
{
}

CLR_Inventory::CLR_Inventory(const CLR_Inventory & rhs)
	: CScroll(rhs)
{
}

HRESULT CLR_Inventory::Ready_GameObject_Prototype()
{
	CScroll::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CLR_Inventory::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CScroll::Ready_GameObject(_pArg);

	Get_ProtoFrontItemTexName(*static_cast<wstring*>(_pArg)); //아이템 앞 부분 이름 설정

	return S_OK;
}

_int CLR_Inventory::Update_GameObject(const _double & _dTimeDelta)
{
	CScroll::Update_GameObject(_dTimeDelta);

	return 0;
}

_int CLR_Inventory::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CScroll::LateUpdate_GameObject(_dTimeDelta);

	return 0;
}

void CLR_Inventory::Get_ProtoFrontItemTexName(const wstring& _wstrProtoFrontItemTexName)
{
	m_wstrProtoFrontItemTexName = _wstrProtoFrontItemTexName;
}

void CLR_Inventory::Free()
{
	map<wstring, CTexture*>::iterator iter = m_mapItemTexture.begin();
	for (; iter != m_mapItemTexture.end();)
	{
		if (iter->second)
			Safe_Release(iter->second);
			++iter;
	}

	if (m_pDrag_DropItemSlot)
		Safe_Release(m_pDrag_DropItemSlot);

	CScroll::Free();
}

CInvenItem* CLR_Inventory::CreateDragItemSlot(POINT _pPos, _int _iIdentityNum)
{
	CInven* pInven = nullptr;
	if (nullptr == static_cast<CInven*>((m_pUnit->Get_Component(L"Com_Show_Inven", ID_DYNAMIC))))
		pInven = static_cast<CInven*>((m_pUnit->Get_Component(L"Com_Inven", ID_DYNAMIC)));
	else
		pInven = static_cast<CInven*>((m_pUnit->Get_Component(L"Com_Show_Inven", ID_DYNAMIC)));


	m_iDrag_DropSlotNumber = _iIdentityNum;
	CInvenItem* pInvenItem = nullptr;
	pInvenItem = static_cast<CInvenItem*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_InvenItem"));
	pInvenItem->Set_IdentifyNumber(_iIdentityNum);
	pInvenItem->Set_Pos_UI((_float)_pPos.x, (_float)_pPos.y);
	pInvenItem->Set_FirstPos(_pPos);
	pInvenItem->Set_DragItem();
	pInvenItem->Set_SlotNum(_iIdentityNum);
	pInvenItem->Set_ItemInfo(&pInven->Get_pInvenItem(_iIdentityNum));
	pInvenItem->Set_ItemTexture(Get_pItemTexture(pInven->Get_pInvenItem(_iIdentityNum).tchName));
	pInvenItem->Get_ORTHOUI_INFO()->bRender = true;

	POINT tMouse;
	Engine::Get_pMousePos(&tMouse);
	pInvenItem->Set_MouseClickPos(&tMouse);
	m_pDrag_DropItemSlot = pInvenItem;

	m_pDrag_DropItemSlotBorder = static_cast<CBorder*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pInvenItem->Get_ORTHOUI_INFO()));
	m_pDrag_DropItemSlotBorder->Set_BorderRenderID(RENDER_ITEMUI);

	return pInvenItem;
}

_int CLR_Inventory::DeleteDropItemSlot(POINT _pPos, _int _iIdentityNum, _bool _bLeft)
{

	return 0;
}

void CLR_Inventory::Match_ItemTex_Info()
{

}
CTexture * CLR_Inventory::Get_pItemTexture(_tchar* _wstrItemName)
{
	if (nullptr == _wstrItemName)
		return nullptr;

	auto iter = m_mapItemTexture.find(_wstrItemName);

	if (iter == m_mapItemTexture.end())
		return Create_ItemTexture(_wstrItemName);

	return iter->second;
}
CTexture * CLR_Inventory::Create_ItemTexture(_tchar* _wstrItemName)
{
	CTexture* pTex = nullptr;
	wstring wstrProtoTexName = m_wstrProtoFrontItemTexName + _wstrItemName;

	pTex = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(wstrProtoTexName));
	m_mapItemTexture.emplace(_wstrItemName, pTex);
	return pTex;
}

_int CLR_Inventory::Get_ItemSlotAmount(_bool _bAllNum)
{
	if (_bAllNum)
		return m_iLRInvenHaveItemSlotAllAmount;
	else
		return m_iLRInvenHaveItemSlotAmount;
}

_int CLR_Inventory::Plus_ItmeSlotAmount()
{
	return m_iLRInvenHaveItemSlotAmount++;
}

void CLR_Inventory::Set_InvenOwner(CUnit * _pUnit)
{
	m_pUnit = _pUnit;
}

CUnit * CLR_Inventory::Get_InvenOwner()
{
	return m_pUnit;
}

void CLR_Inventory::Set_EmplaceBack_ItemSlot(CInvenItem * _InvenItemSlot)
{
	m_vecInvenItemSlot.emplace_back(_InvenItemSlot);
}
