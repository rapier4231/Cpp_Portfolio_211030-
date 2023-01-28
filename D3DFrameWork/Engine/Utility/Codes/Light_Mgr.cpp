#include "..\Headers\Light_Mgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLight_Mgr)

CLight_Mgr::CLight_Mgr()
{
}

CLight_Mgr::~CLight_Mgr()
{
	Free();
}

HRESULT CLight_Mgr::Ready_Light(LPDIRECT3DDEVICE9 _pDevice, const D3DLIGHT9 * _pLineInfo, const _uint & _iIndex)
{
	CLight* pLight = CLight::Create(_pDevice, _pLineInfo, _iIndex);
	
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_listLight.emplace_back(pLight);

	return S_OK;
}

CLight * CLight_Mgr::Get_Light(const _uint & _iIndex) const
{
	auto pIter = m_listLight.begin();

	for (_uint i = 0; i < _iIndex; ++i)
		++pIter;

	return *pIter;
}

void CLight_Mgr::Render_Light(LPD3DXEFFECT & pEffect, _bool _bBlur)
{
	for (auto& iter : m_listLight)
		iter->Render_Light(pEffect, _bBlur);
}

void CLight_Mgr::Free()
{
	for (auto& rIter : m_listLight)
		Safe_Release(rIter);

	m_listLight.clear();
}
