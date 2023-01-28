#include "stdafx.h"
#include "..\Headers\Scroll.h"

#include "Export_Function.h"

USING(Engine)

CScroll::CScroll(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CScroll::CScroll(const CScroll & rhs)
	: CGameObject(rhs)
{
}

HRESULT CScroll::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CScroll::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	return S_OK;
}
_int CScroll::Update_GameObject(const _double & _dTimeDelta)
{
	CGameObject::Update_GameObject(_dTimeDelta);
	if (!m_bSet)
	{
		Scroll_Set();
		m_bSet = true;
	}
	Scroll_Func();
	return 0;
}

_int CScroll::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CGameObject::LateUpdate_GameObject(_dTimeDelta);

	return 0;
}

void CScroll::Free()
{
	CGameObject::Free();
}

void CScroll::Scroll_RatioY_Swap(_int _iSwapNum)
{
	_float fTemp = m_fShow_RatioY;
	m_fShow_RatioY = m_fSave_RatioY[_iSwapNum];
	m_fSave_RatioY[_iSwapNum] = fTemp;
}
