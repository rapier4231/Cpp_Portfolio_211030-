#include "Calculator.h"

USING(Engine)

CCalculator::CCalculator(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CCalculator::CCalculator(const CCalculator & rhs)
	: CComponent(rhs)
{
}

HRESULT CCalculator::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CCalculator::Ready_Component(void * _pArg/* = nullptr*/)
{
	return S_OK;
}

CCalculator * CCalculator::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CCalculator* pInstance = new CCalculator(_pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CCalculator::Clone(void * pArg/* = nullptr*/)
{
	return new CCalculator(*this);
}

void CCalculator::Free()
{
	CComponent::Free();
}

_float CCalculator::Get_Distance(const _vec3 * srcPos, const _vec3 * DstPos)
{
	return D3DXVec3Length(&(*DstPos - *srcPos));
}

_float CCalculator::Get_Angle(const _vec3 * srcPos, const _vec3* srcDir, const _vec3 * DstPos, _vec3* vCross)
{
	_vec3 vDir = *srcPos - *DstPos;
	D3DXVec3Normalize(&vDir, &vDir);

	_float fAngle = D3DXVec3Dot(&vDir, srcDir);

	if (fAngle >= 1.f)
		fAngle = 1.f;
	else if (fAngle <= -1.f)
		fAngle = -1.f;

	fAngle = acosf(fAngle);

	if (vCross)
		D3DXVec3Cross(vCross, &vDir, srcDir);

	return fAngle;
}

const _mat& CCalculator::VectorToRotationMatrix(_vec3* vDir, const _vec3* vUpvec)
{
	//_vec3 vDir = *pTargetPos - m_vInfo[INFO_POS];
	_vec3 vUp = { 0.f, -1.f, 0.f };

	_vec3 vAxis;
	_mat      matRot;
	D3DXMatrixIdentity(&matRot);

	//D3DXVec3Normalize(&vDir, &vDir);
	//D3DXVec3Cross(&vAxis, &vUp, &vDir);
	//_float fAngle = acosf(D3DXVec3Dot(&vDir, &vUp));
	//return *D3DXMatrixRotationAxis(&matRot, &vAxis, fAngle);

	return *D3DXMatrixRotationAxis(&matRot,
			D3DXVec3Cross(&vAxis, vUpvec, vDir),
			acosf(D3DXVec3Dot(D3DXVec3Normalize(vDir, vDir), D3DXVec3Normalize(&vUp, vUpvec))));
}