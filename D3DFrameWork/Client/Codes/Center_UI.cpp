#include "stdafx.h"
#include "..\Headers\Center_UI.h"

#include "Export_Function.h"
#include "Unit.h"
#include "InvenItem.h"
#include "Stat_UI.h"
#include "CenterTap.h"
#include "MonsterStat_UI.h"
#include "Left_Inventory.h"
#include "Right_Inventory.h"
#include "StructureStat_UI.h"
USING(Engine)

CCenter_UI::CCenter_UI(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CCenter_UI::CCenter_UI(const CCenter_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCenter_UI::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CCenter_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	Get_ProtoFrontEquipTexName(*static_cast<wstring*>(_pArg)); //아이템 앞 부분 이름 설정

	return S_OK;
}

_int CCenter_UI::Update_GameObject(const _double & _dTimeDelta)
{
	CGameObject::Update_GameObject(_dTimeDelta);

	return 0;
}

_int CCenter_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CGameObject::LateUpdate_GameObject(_dTimeDelta);

	return 0;
}

void CCenter_UI::Get_ProtoFrontEquipTexName(const wstring& _wstrProtoFrontEquipTexName)
{
	m_wstrProtoFrontEquipTexName = _wstrProtoFrontEquipTexName;
}

void CCenter_UI::Free()
{
	map<wstring, CTexture*>::iterator iter = m_mapEquipTexture.begin();
	for (; iter != m_mapEquipTexture.end();)
	{
		if (iter->second)
			Safe_Release(iter->second);
			++iter;
	}
	CGameObject::Free();
}

void CCenter_UI::Stat_UI_Close( )
{
	All_Off();
}

void CCenter_UI::All_Off()
{
	static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->Inventory_Render_Off();
	static_cast<CMonsterStat_UI*>(Engine::Get_GameObject(L"Center_UI", 1))->Inventory_Render_Off();
	static_cast<CStructureStat_UI*>(Engine::Get_GameObject(L"Center_UI", 2))->Inventory_Render_Off();


	//전부 종료
	static_cast<CCenterTap*>(Engine::Get_GameObject(L"Stat_UI", 2))->Reset_DaeSangTap();
	static_cast<CRight_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Right_InventoryClose();
	static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Left_InventoryClose();
}

CTexture * CCenter_UI::Get_pEquipTexture(_tchar* _wstrItemName)
{
	if (nullptr == _wstrItemName)
		return nullptr;

	auto iter = m_mapEquipTexture.find(_wstrItemName);

	if (iter == m_mapEquipTexture.end())
		return Create_EquipTexture(_wstrItemName);

	return iter->second;
}
CTexture * CCenter_UI::Create_EquipTexture(_tchar* _wstrEquipName)
{
	CTexture* pTex = nullptr;
	wstring wstrProtoTexName = m_wstrProtoFrontEquipTexName + _wstrEquipName;

	pTex = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(wstrProtoTexName));
	m_mapEquipTexture.emplace(_wstrEquipName, pTex);
	return pTex;
}

void CCenter_UI::Set_EquipOwner(CUnit * _pUnit)
{
	m_pUnit = _pUnit;
}

EQUIP_Position CCenter_UI::Get_NowMousePickingSlot()
{
	for (_int i = 0; i < EQUIP_Position_END; ++i)
		if (m_vecEquipSlot[i]->Question_Now_MouseIsUrRange())
			return (EQUIP_Position)i;
	return EQUIP_Position::EQUIP_Position_END;
}

void CCenter_UI::Set_EmplaceBack_EquipSlot(CInvenItem * _EquipSlot)
{
	m_vecEquipSlot.emplace_back(_EquipSlot);
}
