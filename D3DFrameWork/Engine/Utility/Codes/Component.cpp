#include "..\Headers\Component.h"

USING(Engine)

CComponent::CComponent(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
	, m_bIsClone(false)
{
	m_pDevice->AddRef();
}

CComponent::CComponent(const CComponent & _rhs)
	: m_pDevice(_rhs.m_pDevice)
	, m_bIsClone(true)
{
	m_pDevice->AddRef();
}

HRESULT CComponent::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CComponent::Ready_Component(void * _pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	Safe_Release(m_pDevice);
}
