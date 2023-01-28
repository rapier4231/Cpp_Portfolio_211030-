#include "stdafx.h"
#include "TPV_QuaternionCamera.h"

#include "Export_Function.h"

#include "Player.h"
#include "CameraMrg.h"

#include "PhysX_Mgr.h"

CTPV_QuaternionCamera::CTPV_QuaternionCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CTPV_QuaternionCamera::CTPV_QuaternionCamera(const CTPV_QuaternionCamera & rhs)
	: CCamera(rhs)
{
}

HRESULT CTPV_QuaternionCamera::Ready_GameObject(const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	/// ������� ����
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFovY), m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);


	D3DXQuaternionIdentity(&m_quatCamDir);

	return S_OK;
}

HRESULT CTPV_QuaternionCamera::Late_Ready_GameObject(void * _pArg)
{
	return S_OK;
}

_int CTPV_QuaternionCamera::Update_GameObject(const _double & _dTimeDelta)
{
	_int iExit = 0;

	// �÷��̾� ����
	if (!m_pPlayer)
	{
		m_pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0));
		if (m_pPlayer)
		{
			m_pPlayer->AddRef();
		}
		else
		{
			ERR_MSG(L"Faild to Create QuaternionCamera !!!");
			return E_FAIL;
		}
	}

	Move_CamPos(_dTimeDelta);

	return iExit;
}

_int CTPV_QuaternionCamera::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_pPlayer)
		return 0;

	_int iExit = 0;

	/// ȸ��
	D3DXQUATERNION quatCamDir, quatRotX, quatRotY;
	D3DXQuaternionIdentity(&quatCamDir);

	///// X, Y ������ ȸ����Ų��.
	// X��, ���� ����
	D3DXQuaternionRotationAxis(&quatRotX, Get_Axis(&quatCamDir, &_vec3(1.f, 0.f, 0.f)), D3DXToRadian(m_pPlayer->Get_AngleX()));
	quatCamDir *= quatRotX;

	// Y��, �¿� ����
	D3DXQuaternionRotationAxis(&quatRotY, Get_Axis(&quatCamDir, &_vec3(0.f, 1.f, 0.f)), -D3DXToRadian(m_pPlayer->Get_AngleY() + 180.f));
	quatCamDir *= quatRotY;


	/// ���麸��
	D3DXQuaternionSlerp(&m_quatCamDir, &m_quatCamDir, &quatCamDir, m_fSlerpSpeed * (_float)_dTimeDelta);


	/// ���ʹϾ��� ��Ʈ������ �ٲ��ش�.
	_mat matRot;
	D3DXMatrixRotationQuaternion(&matRot, &m_quatCamDir);


	/// ��ġ (ȸ���� ���ʹϾ��� �� ���Ϳ� �÷��̾ �д�)
	_mat matTrans;
	// �÷��̾� ������Ʈ
	CTransform*	pPlayerTransformCom = nullptr;
	if (m_bIsPlayerRide)
	{
		pPlayerTransformCom = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
	}
	else
	{
		pPlayerTransformCom = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_TransformFPV", ID_DYNAMIC));
	}
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);
	 

	// �̵� ��� �����
	pPlayerTransformCom->Get_Info(INFO_POS, &m_vEye);
	D3DXMatrixTranslation(&matTrans, -m_vEye.x, -m_vEye.y, -m_vEye.z);

	// �÷��̾� ������ ��ġ,ȸ�� ī�޶�
	m_matView = matTrans * matRot;


	/////////////////////////////////////////////////////////////////////////////////////////
	/// ī�޶� ��ġ �ڷ� �б�
	_mat matView;
	D3DXMatrixInverse(&matView, NULL, &m_matView);

	_vec3 vRayPos, vRayDir, vOutPos;

	memcpy(&vRayPos, &pPlayerTransformCom->Get_WorldMatrix().m[INFO_POS][0], sizeof(_vec3));
	memcpy(&vRayDir, &matView.m[INFO_LOOK][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRayDir, &vRayDir);

	vRayPos.y += 1.8f;				// �÷��̾� �Ӹ� ��ġ
	vRayDir *= -1.f;				// �÷��̾� �Ӹ����� ī�޶��� ����

	vRayPos += vRayDir * 1.5f;

	if (Lock_BackDistance(vRayPos, vRayDir, 10.f, &m_fTargetDis, &vOutPos))
	{
		m_fTargetDis -= 5.f;

		// ī�޶� �Ÿ� �ִ��ּҰ� üũ
		if (m_fCameraMinDis > m_fTargetDis)
			m_fTargetDis = m_fCameraMinDis;
		else if(m_fCameraMaxDis < m_fTargetDis)
			m_fTargetDis = m_fCameraMaxDis;
	}
	else
	{
		m_fTargetDis = m_fCameraMaxDis;
	}

	// ī�޶� �ڷ� �б�
	m_matView._41 -= m_fRightDis;
	m_matView._42 -= 0.5f;
	m_matView._43 += m_fCameraDis;		


	/// ����� ����
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);

	/// ������� ����
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFovY), m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);


	return iExit;
}

CTPV_QuaternionCamera * CTPV_QuaternionCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CTPV_QuaternionCamera*	pInstance = new CTPV_QuaternionCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(fFovY, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTPV_QuaternionCamera::Free(void)
{
	Safe_Release(m_pPlayer);

	CCamera::Free();
}

void CTPV_QuaternionCamera::Set_CameraInfo(const _float& _fAngleX, const _float& _fAngleY, _bool _bIsMoveCamera)
{
	if (!_bIsMoveCamera)
	{
		m_fRightDis = 0.f;
		m_fTargetRightDis = 0.f;
		m_fCameraDis = m_fCameraMinDis;
		m_fTargetDis = m_fCameraMaxDis;

		m_fTargetFov = 60.f;
	}


	/// ȸ��
	D3DXQUATERNION quatRotX, quatRotY;
	D3DXQuaternionIdentity(&m_quatCamDir);

	///// X, Y ������ ȸ����Ų��.
	// X��, ���� ����
	D3DXQuaternionRotationAxis(&quatRotX, Get_Axis(&m_quatCamDir, &_vec3(1.f, 0.f, 0.f)), D3DXToRadian(_fAngleX));
	m_quatCamDir *= quatRotX;

	// Y��, �¿� ����
	D3DXQuaternionRotationAxis(&quatRotY, Get_Axis(&m_quatCamDir, &_vec3(0.f, 1.f, 0.f)), -D3DXToRadian(_fAngleY + 180.f));
	m_quatCamDir *= quatRotY;
}

void CTPV_QuaternionCamera::Set_MoveCamera(_float _fTargetDis, _float _fTargetRightDis, _float _fTargetFov, _double _dTargetTime, _bool _bIsToTPVCamera/* = false*/, _bool _bIsSwitchCamera/* = false*/)
{
	if (_bIsToTPVCamera)
	{
		m_fCameraDis = m_fCameraMinDis;
		m_fRightDis = 0.f;
		m_fFovY = 50.f;
	}


	m_bIsToFPV = _bIsSwitchCamera;
	m_bIsCameraMove = true;
	m_dCameraMoveTime = 0.0;
	m_dTargetCameraMoveTime = _dTargetTime;

	m_fTargetDis			= _fTargetDis;
	m_fTargetRightDis		= _fTargetRightDis;
	m_fTargetFov			= _fTargetFov;

	m_fMoveDisRatio			= m_fTargetDis - m_fCameraDis;
	m_fMoveRightDisRatio	= m_fTargetRightDis - m_fRightDis;
	m_fMoveFovRatio			= m_fTargetFov - m_fFovY;
}

//void CTPV_QuaternionCamera::Set_IsPlayerShoulderView(_bool _bIsShoulderView)
//{
//	if (_bIsShoulderView)
//	{
//		m_fTargetRightDis = 1.f;
//		m_fTargetFov = 30.f;
//	}
//	else
//	{
//		m_fTargetRightDis = 0.f;
//		m_fTargetFov = 60.f;
//	}
//}

void CTPV_QuaternionCamera::Set_TPVCam_MaxDis(const _float & _fCameraDis)
{
	m_fTargetDis = m_fCameraMaxDis = _fCameraDis;
}

_vec3 * CTPV_QuaternionCamera::Get_Axis(D3DXQUATERNION * _quat, _vec3 * _pAxis)
{
	// ���ڷ� �־��� ������ ī�޶� ������ orientation ������� ���� ȸ������ ���� ���Ѵ�.
	// orientation���κ��� right, up, look ���⺤�͸� ���� �� �ִ�.
	// ���� axis�� (1,0,0)���� �ϸ� right, (0,1,0)�̸� up, (0,0,1)�̸� look���� ��ȯ�Ѵ�.

	// ī�޶� ȸ���� ��Ÿ���� orientation���κ��� ȸ������ ���� ���Ѵ�.
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, _quat);

	// ���ڷ� ���� �� ���͸� ȸ������ ���ο� ���� �����.
	D3DXVECTOR3 axisNew;
	axisNew.x = _pAxis->x * matRotation._11 + _pAxis->y * matRotation._21 + _pAxis->z * matRotation._31 + matRotation._41;
	axisNew.y = _pAxis->x * matRotation._12 + _pAxis->y * matRotation._22 + _pAxis->z * matRotation._32 + matRotation._42;
	axisNew.z = _pAxis->x * matRotation._13 + _pAxis->y * matRotation._23 + _pAxis->z * matRotation._33 + matRotation._43;

	memcpy(_pAxis, &axisNew, sizeof(axisNew));
	return _pAxis;
}

_bool CTPV_QuaternionCamera::Lock_BackDistance(const _vec3& _vRayPos, const _vec3& _vRayDir, const _float& _vRayDistance, _float* _pOutPickDis/* = nullptr*/, _vec3* _pOutPickPos/* = nullptr*/)
{
	PxVec3 pxPos = PxVec3(_vRayPos.x, _vRayPos.y, _vRayPos.z);
	PxVec3 pxDir = PxVec3(_vRayDir.x, _vRayDir.y, _vRayDir.z);

	return CPhysX_Mgr::GetInstance()->RayCast(pxPos, pxDir, _vRayDistance, _pOutPickDis, _pOutPickPos);
}

void CTPV_QuaternionCamera::Move_CamPos(const _double & _dTimeDelta)
{
	if (m_bIsCameraMove)
	{
		m_dCameraMoveTime += _dTimeDelta;

		m_fCameraDis += m_fMoveDisRatio * _float(_dTimeDelta / m_dTargetCameraMoveTime);
		m_fRightDis += m_fMoveRightDisRatio * _float(_dTimeDelta / m_dTargetCameraMoveTime);
		m_fFovY += m_fMoveFovRatio * _float(_dTimeDelta / m_dTargetCameraMoveTime);


		if ((0 > m_fMoveDisRatio && m_fCameraDis < m_fTargetDis) && (0 < m_fMoveDisRatio && m_fCameraDis > m_fTargetDis))
			m_fCameraDis = m_fTargetDis;

		if ((0 > m_fMoveRightDisRatio && m_fRightDis < m_fTargetRightDis) && (0 < m_fMoveRightDisRatio && m_fRightDis > m_fTargetRightDis))
			m_fRightDis = m_fTargetRightDis;

		if ((0 > m_fMoveFovRatio && m_fFovY < m_fTargetFov) && (0 < m_fMoveFovRatio && m_fFovY > m_fTargetFov))
			m_fFovY = m_fTargetFov;

		if (m_dCameraMoveTime >= m_dTargetCameraMoveTime)
		{
			m_bIsCameraMove = false;
			m_dCameraMoveTime = m_dTargetCameraMoveTime = 0.0;

			m_fCameraDis = m_fTargetDis;
			m_fRightDis = m_fTargetRightDis;
			m_fFovY = m_fTargetFov;

			if (m_bIsToFPV)
			{
				m_bIsToFPV = false;
				m_pPlayer->Switch_Camera(0);

				//static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_CameraType(CCameraMrg::CAMERA_FPV, m_pPlayer->Get_AngleX(), m_pPlayer->Get_AngleY());
			}
		}
	}
}
