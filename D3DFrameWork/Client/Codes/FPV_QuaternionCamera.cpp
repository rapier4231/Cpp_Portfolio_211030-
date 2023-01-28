#include "stdafx.h"
#include "FPV_QuaternionCamera.h"

#include "Export_Function.h"

#include "Player.h"

#include "PhysX_Mgr.h"

CFPV_QuaternionCamera::CFPV_QuaternionCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CFPV_QuaternionCamera::CFPV_QuaternionCamera(const CFPV_QuaternionCamera & rhs)
	: CCamera(rhs)
{
}

HRESULT CFPV_QuaternionCamera::Ready_GameObject(const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	// ����
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFovY), m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);


	D3DXQuaternionIdentity(&m_quatCamDir);

	return S_OK;
}

HRESULT CFPV_QuaternionCamera::Late_Ready_GameObject(void * _pArg)
{
	return S_OK;
}

_int CFPV_QuaternionCamera::Update_GameObject(const _double & _dTimeDelta)
{
	_int iExit = 0;

	// �÷��̾� ����
	if (!m_pPlayer)
	{
		m_pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0));
		if (m_pPlayer)
		{
			m_pPlayer->AddRef();

			// ī�޶� �� ��Ʈ����
			CDynamicMesh* pPlayerMeshCom = static_cast<CDynamicMesh*>(m_pPlayer->Get_Component(L"Com_MeshFPV", ID_STATIC));
			const D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("camera");
			m_matPlayerFPV = &pFrame->CombinedTransformMatrix;
		}
		else
		{
			ERR_MSG(L"Faild to Create QuaternionCamera !!!");
			return E_FAIL;
		}
	}

	return iExit;
}

_int CFPV_QuaternionCamera::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_pPlayer)
		return 0;

	_int iExit = 0;

	// �÷��̾� ������Ʈ
	CTransform*	pPlayerTransformCom = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_TransformFPV", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	_mat matTrans, matRot;

	/// ȸ��
	D3DXQUATERNION quatRot, quatRotX, quatRotY, quatRotZ, quatPlayerX, quatPlayerY;
	D3DXQuaternionRotationMatrix(&quatRot, m_matPlayerFPV);
	D3DXQuaternionInverse(&quatRot, &quatRot);

	// �Ž� ȸ������ �°� �̸� ȸ�����ֱ�
	D3DXQuaternionRotationAxis(&quatRotX, &_vec3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	//D3DXQuaternionRotationAxis(&quatRotY, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(0.f));
	D3DXQuaternionRotationAxis(&quatRotZ, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(180.f));

	// �÷��̾� ȸ���� ��������
	D3DXQuaternionRotationYawPitchRoll(&quatPlayerX, 0.f, pPlayerTransformCom->Get_Rotate(ROT_X), 0.f);
	D3DXQuaternionRotationYawPitchRoll(&quatPlayerY, -pPlayerTransformCom->Get_Rotate(ROT_Y), 0.f, 0.f);

	// ��� ȸ������ ���Ѵ�.
	quatRot = quatPlayerY * quatRot * quatRotX /** quatRotY*/ * quatRotZ * quatPlayerX;


	/// ���麸��
	D3DXQuaternionSlerp(&m_quatCamDir, &m_quatCamDir, &quatRot, m_fSlerpSpeed * (_float)_dTimeDelta);

	// ���ʹϾ��� ��Ʈ������ �ٲ�
	D3DXMatrixRotationQuaternion(&matRot, &m_quatCamDir);


	/// ��ġ
	_mat matPosition;
	matPosition = *m_matPlayerFPV * pPlayerTransformCom->Get_WorldMatrix();
	memcpy(&m_vEye, &matPosition.m[INFO_POS][0], sizeof(_vec3));

	D3DXMatrixTranslation(&matTrans, -m_vEye.x, -m_vEye.y, -m_vEye.z);


	// ����� ����� �� ����
	m_matView = matTrans * matRot;

	// ī�޶� �ڷ� �б�
	//m_matView._42 -= 0.05f;
	//m_matView._43 += 0.54f;
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	
	// ����
	//m_fFovY = 50.f;
	//m_fNear = 0.1f;
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFovY), m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return iExit;
}

CFPV_QuaternionCamera * CFPV_QuaternionCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CFPV_QuaternionCamera*	pInstance = new CFPV_QuaternionCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(fFovY, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFPV_QuaternionCamera::Free(void)
{
	Safe_Release(m_pPlayer);

	CCamera::Free();
}

void CFPV_QuaternionCamera::Set_CameraInfo(const _float & _fAngleX, const _float & _fAngleY)
{
	/// ȸ��
	// �÷��̾� ������Ʈ
	CTransform*	pPlayerTransformCom = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_TransformFPV", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom,);

	_mat matTrans, matRot;

	/// ȸ��
	D3DXQUATERNION quatRotX, /*quatRotY,*/ quatRotZ, quatPlayerX, quatPlayerY;
	D3DXQuaternionIdentity(&m_quatCamDir);
	D3DXQuaternionRotationMatrix(&m_quatCamDir, m_matPlayerFPV);
	D3DXQuaternionInverse(&m_quatCamDir, &m_quatCamDir);

	// �Ž� ȸ������ �°� �̸� ȸ�����ֱ�
	D3DXQuaternionRotationAxis(&quatRotX, &_vec3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	D3DXQuaternionRotationAxis(&quatRotZ, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(180.f));

	// �÷��̾� ȸ���� ��������
	D3DXQuaternionRotationYawPitchRoll(&quatPlayerX, 0.f, pPlayerTransformCom->Get_Rotate(ROT_X), 0.f);
	D3DXQuaternionRotationYawPitchRoll(&quatPlayerY, -pPlayerTransformCom->Get_Rotate(ROT_Y), 0.f, 0.f);

	// ��� ȸ������ ���Ѵ�.
	m_quatCamDir = quatPlayerY * m_quatCamDir * quatRotX/* * quatRotY*/ * quatRotZ * quatPlayerX;
}
