#include "stdafx.h"
#include "..\Headers\StructureStat_UI.h"

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
#include "Back_StructureStat_UI.h"
USING(Engine)

CStructureStat_UI::CStructureStat_UI(LPDIRECT3DDEVICE9 _pDevice)
	: CCenter_UI(_pDevice)
{
}

CStructureStat_UI::CStructureStat_UI(const CStructureStat_UI & rhs)
	: CCenter_UI(rhs)
{
}

HRESULT CStructureStat_UI::Ready_GameObject_Prototype()
{
	CCenter_UI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CStructureStat_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CCenter_UI::Ready_GameObject(_pArg);
	return S_OK;
}

_int CStructureStat_UI::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_pUnit)
		return NO_EVENT;

	CCenter_UI::Update_GameObject(_dTimeDelta);

	return NO_EVENT;
}

_int CStructureStat_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CCenter_UI::LateUpdate_GameObject(_dTimeDelta);
	return 0;
}

void CStructureStat_UI::Render_GameObject()
{
}

void CStructureStat_UI::StructureStat_UI_Close()
{
	CCenter_UI::Stat_UI_Close();
}

CStructureStat_UI * CStructureStat_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CStructureStat_UI*	pInstance = new CStructureStat_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CStructureStat_UI::Clone(void * _pArg/* = nullptr*/)
{
	CStructureStat_UI*	pClone = new CStructureStat_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CStructureStat_UI::Free()
{
	CCenter_UI::Free();
}

void CStructureStat_UI::Inventory_Render_On(InvenUserUnitTypeID _InvenID , CUnit* _pStructureStat)
{
	if (!_pStructureStat)
	{
		if (!m_pStructure)
			return;
	}
	else
		m_pStructure = _pStructureStat;

	for (auto& iter : *(Engine::Get_Layer(L"StructureStat_UI")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOn();
	static_cast<CCenterTap*>(Get_GameObject(L"Stat_UI" , 2))->Set_TapOwner((_int)_InvenID, m_pStructure);
	static_cast<CCenterTap*>(Get_GameObject(L"Stat_UI", 2))->Set_DaeSangTap();
	static_cast<CCenterTap*>(Get_GameObject(L"Stat_UI", 2))->UI_RenderOn();
	static_cast<CBack_StructureStat_UI*>(Get_GameObject(L"StructureStat_UI", 0))->Set_pUnit(m_pStructure);
}

void CStructureStat_UI::Inventory_Render_Off(_bool _bDelete_pUnit)
{
	if(_bDelete_pUnit)
		m_pStructure = nullptr;
	for (auto& iter : *(Engine::Get_Layer(L"StructureStat_UI")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOff();
	static_cast<CCenterTap*>(Get_GameObject(L"Stat_UI", 2))->UI_RenderOff();
}
