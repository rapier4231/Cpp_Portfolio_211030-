#include "stdafx.h"
#include "UI.h"
#include "Export_Function.h"

USING(Engine)

CUI::CUI(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CUI::Ready_GameObject(void * _pArg)
{
	CGameObject::Ready_GameObject(_pArg);

	return S_OK;
}

_int CUI::Update_GameObject(const _double & _dTimeDelta)
{
	CGameObject::Update_GameObject(_dTimeDelta);

	return NULL;
}

_int CUI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CGameObject::LateUpdate_GameObject(_dTimeDelta);

	return NULL;
}

void CUI::Render_GameObject()
{

}

void CUI::Free()
{
	CGameObject::Free();
}
