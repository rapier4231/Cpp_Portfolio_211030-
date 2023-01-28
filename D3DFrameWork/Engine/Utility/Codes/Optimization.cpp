#include "..\Headers\Optimization.h"

USING(Engine)

COptimization::COptimization(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

COptimization::COptimization(const COptimization & rhs)
	: CComponent(rhs)
{
}

HRESULT COptimization::Ready_Component_Prototype()
{
	//// 투영 절두체
	//m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	//m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	//m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	//m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);
	//
	//m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	//m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	//m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	//m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

HRESULT COptimization::Ready_Component(void * _pArg/* = nullptr*/)
{
	return S_OK;
}

_bool COptimization::Isin_Frustum_ForObject(const _vec3& _vMin, const _vec3& _vMax, const _vec3* _pPos)
{
	// 투영 절두체
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	_mat		matProj, matView;
	_float		fRadius = D3DXVec3Length(&(*_pPos - _vMax));

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}

	// 월드 좌표까지 변환된 정점을 이용해서 6개의 평면을 만들어주자
	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);
	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);
	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);
	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);
	// z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);
	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);



	_float	fResult = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		fResult = D3DXPlaneDotCoord(&m_Plane[i], _pPos);

		if (fResult > fRadius)
			return false;
	}

	return true;
}

COptimization * COptimization::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	COptimization*	pInstance = new COptimization(_pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * COptimization::Clone(void * pArg/* = nullptr*/)
{
	COptimization*	pClone = new COptimization(*this);

	if (FAILED(pClone->Ready_Component()))
		Safe_Release(pClone);

	return pClone;
}

void COptimization::Free()
{
	CComponent::Free();
}
