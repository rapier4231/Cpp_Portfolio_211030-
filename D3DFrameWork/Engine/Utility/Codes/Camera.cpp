#include "..\Headers\Camera.h"

USING(Engine)

CCamera::CCamera(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CCamera::CCamera(const CCamera & rhs)
	:CGameObject(rhs)
{
}

HRESULT CCamera::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CCamera::Update_GameObject(const _double & _dTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

void CCamera::Render_GameObject()
{
}

CGameObject * CCamera::Clone(void * _pArg/* = nullptr*/)
{
	return nullptr;
}

void CCamera::Free()
{
	CGameObject::Free();
}

#ifdef _DEBUG
const _vec3& CCamera::Get_CameraInfo(_uint _iInfo) const
{
	switch (_iInfo)
	{
	case 0:
		return m_vEye;
	case 1:
		return m_vAt;
	case 2:
		return m_vUp;
	default:
		return m_vEye;		// 사용안함
	}
	return m_vEye;			// 사용안함
}
#endif // _DEBUG

const _vec3 CCamera::Get_CameraDir()
{
	_vec3 vCamDir = {};

	memcpy(&vCamDir, &m_matView._31, sizeof(_vec3));

	return vCamDir;
}

_mat CCamera::Get_ReflectView(_float _fHeight)
{
	m_vReflectEye = m_vEye;
	m_vReflectEye.y = -m_vEye.y + (_fHeight * 2.f);

	D3DXMatrixLookAtLH(&m_matReflectView, &m_vReflectEye, &m_vAt, &m_vUp);

	return m_matReflectView;
}
