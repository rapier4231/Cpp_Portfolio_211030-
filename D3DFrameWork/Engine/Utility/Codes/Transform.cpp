#include "..\Headers\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_vScale(1.f, 1.f, 1.f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform & rhs)
	:CComponent(rhs)
	, m_vScale(rhs.m_vScale)
	, m_vAngle(rhs.m_vAngle)
	, m_matWorld(rhs.m_matWorld)
	, m_bIsRotateAxis(rhs.m_bIsRotateAxis)
	, m_matRotAxis(rhs.m_matRotAxis)
{
	memcpy(&m_vInfo, &rhs.m_vInfo, sizeof(_vec3) * INFO_END);
}

HRESULT CTransform::Ready_Component_Prototype()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRotAxis);

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

HRESULT CTransform::Ready_Component(void* _pArg/* = nullptr*/)
{
	return S_OK;
}

_int CTransform::Update_Component(const _double & _dTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	
	// 크기
	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matNRotWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	// 회전
	_mat	matRot[ROT_END];
	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	if (m_bIsYawPitchRoll)
	{
		// 스케일, zyx 회전만 적용
		for (_uint i = 0; i < INFO_POS; ++i)
			memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

		m_matWorld *= m_matYawPitchRoll;

		// 이동
		memcpy(&m_matWorld.m[INFO_POS][0], &m_vInfo[INFO_POS], sizeof(_vec3));
	}
	else if (m_bIsRotateAxis)
	{
		/// Axis 회전
		// 스케일, zyx 회전만 적용
		for (_uint i = 0; i < INFO_POS; ++i)
			memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

		// Axis 축회전
		m_matWorld *= m_matRotAxis;

		// 이동
		memcpy(&m_matWorld.m[INFO_POS][0], &m_vInfo[INFO_POS], sizeof(_vec3));
	}
	else
	{
		for (_uint i = 0; i < INFO_END; ++i)
			memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
	}

	return _int();
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTransform*	pInstance = new CTransform(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg/* = nullptr*/)
{
	CTransform*	pClone = new CTransform(*this);

	if (FAILED(pClone->Ready_Component(pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CTransform::Free()
{
	CComponent::Free();
}

const _mat & CTransform::Get_WorldMatrix() const
{
	return m_matWorld;
}

void CTransform::Get_NRotWorldMatrix(_mat * pMatrix) const
{
	*pMatrix = m_matNRotWorld;
}

void CTransform::Set_Scale(const _float & _fX, const _float & _fY, const _float & _fZ)
{
	m_vScale.x = _fX;
	m_vScale.y = _fY;
	m_vScale.z = _fZ;
}

void CTransform::Set_Scale(const _vec3 & _vScale)
{
	m_vScale = _vScale;
}

void CTransform::Set_Pos(const _float & _fX, const _float & _fY, const _float & _fZ)
{
	m_vInfo[INFO_POS].x = _fX;
	m_vInfo[INFO_POS].y = _fY;
	m_vInfo[INFO_POS].z = _fZ;
}

void CTransform::Set_Pos(const _vec3 & _vPos)
{
	m_vInfo[INFO_POS] = _vPos;
}

void CTransform::Set_Rotate(ROTATE eType, const _float & _fAngle)
{
	// vec3 자료형의 m_vAngle 을 float으로 바꾼 뒤 eType으로 포인터연산후 해당 Angle을 찾아가서 대입한다.
	*(((_float*)&m_vAngle) + eType) = _fAngle;
}

void CTransform::Set_AccRotate(ROTATE eType, const _float & _fAngle)
{
	*(((_float*)&m_vAngle) + eType) += _fAngle;
}

void CTransform::Set_AxisAccRotate(const _vec3 & _vAxis, const _float & _fAngle)
{
	_mat matAxis;

	D3DXMatrixRotationAxis(&matAxis, &_vAxis, _fAngle);

	m_matRotAxis = m_matRotAxis * matAxis;
}

void CTransform::Set_YawPitchRoll(const _float & _fYaw, const _float & _fPitch, const _float & _fRoll)
{
	D3DXMatrixRotationYawPitchRoll(&m_matYawPitchRoll, _fYaw, _fPitch, _fRoll);
}

void CTransform::Set_YawPitchRoll(const _mat & matrix)
{
	m_matYawPitchRoll = matrix;
}

void CTransform::Set_ParentMatrix(const _mat * pParentMatrix)
{
	m_matWorld *= *pParentMatrix;
}

void CTransform::Set_WorldMatrix(const _mat * pWorld)
{
	m_matWorld = *pWorld;
}


_float CTransform::Get_Rotate(ROTATE eType)
{
	_float fAngle;
	memcpy(&fAngle, (_float*)&m_vAngle + eType, sizeof(_float));
	return fAngle;
}
const _vec3 * CTransform::Get_Scale()
{
	return &m_vScale;
}

void CTransform::Move_Pos(const _vec3 & _vDir, const _float & _fSpeed, _double _dTimeDelta)
{
	m_vInfo[INFO_POS] += _vDir * _fSpeed * (_float)_dTimeDelta;
}

void CTransform::Get_Info(INFO eType, _vec3 * pInfo)
{
	// 월드 좌표의 값을 가져와야 하므로 m_matWorld 의 값을 가져온다.
	memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
}