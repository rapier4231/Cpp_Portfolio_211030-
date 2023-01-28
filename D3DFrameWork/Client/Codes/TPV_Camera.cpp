#include "stdafx.h"
#include "..\Headers\TPV_Camera.h"

#include "Export_Function.h"

#include "Player.h"

#include "PhysX_Mgr.h"

CTPV_Camera::CTPV_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
	ZeroMemory(m_vTPVCamLook, sizeof(_vec3));
	ZeroMemory(m_vTPVCamRight, sizeof(_vec3));
}

CTPV_Camera::CTPV_Camera(const CTPV_Camera & rhs)
	: CCamera(rhs)
	, m_vTPVCamLook(rhs.m_vTPVCamLook)
	, m_vTPVCamRight(rhs.m_vTPVCamRight)
{
}

HRESULT CTPV_Camera::Ready_GameObject(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const float & fFovY, const float & fAspect, const float & fNear, const float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	if (FAILED(CCamera::Ready_GameObject()))
		return E_FAIL;


	// �÷��̾� ���� ȸ����
	m_fAngleX = 90.f;
	m_fAngleY = 20.f;

	m_fFovY = 60.f;

	return S_OK;
}

_int CTPV_Camera::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_bIsLockKeyInput)
	{
		// Ű�Է� �ޱ�
		Key_Input(_dTimeDelta);

		// ���콺 �Է°� �ޱ�
		Mouse_Move(_dTimeDelta);

		// ���콺 ����
		//Mouse_Fix();
	}

	if(m_bIsMoveFov)
		Check_TPVCamFov(_dTimeDelta);

	//_int iExit = CCamera::Update_GameObject(_dTimeDelta);

	return 0;
}

_int CTPV_Camera::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	// �÷��̾� ���� (����Ʈ ����)
	if (!m_pPlayer)
	{
		m_pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0));

		if (m_pPlayer)
		{
			CTransform* pTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
			//pTransform->Get_Info(INFO_LOOK, &m_vTPVCamLook);
			//D3DXVec3Normalize(&m_vTPVCamLook, &m_vTPVCamLook);

			pTransform->Get_Info(INFO_POS, &m_vPreEyePos);
			m_vEye = m_vPreEyePos;

			m_pPlayer->AddRef();
		}
		else
			return 0;
	}

	Looking_Player(_dTimeDelta);

	// ����Ʈ�������� �׽�Ʈ�ڵ� ////////////////////////////////////////////////////////////////////////
	if (Engine::Get_DIKeyState(DIK_Z) & 0x80)
	{
		_mat matProj, matView;

		//��������� ������������ε�, ���μ��� 1000.f�� ������(ť��?) ����
		D3DXMatrixOrthoLH(&matProj, 100.f, 100.f, 0.f, 2000.f); //ť����
		//D3DXMatrixOrthoLH(&matProj, 100.f, 100.f, 0.f, 200.f);	//��������

		//������� �÷��̾� ���� +250.f ������ �߽����� �÷��̾� ���濡 ������ �ǵ��� zȸ����Ű�� ����̹Ƿ�
		//�÷��̾� ��ġ�� �躤�� ���
		CTransform* pPlayerTransform = (CTransform*)(m_pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
		if (!pPlayerTransform)
			return E_FAIL;
		_vec3 vPlayerPos, vPlayerDir;
		pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
		pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerDir);
		vPlayerDir.y = 0.f;
		D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);
		vPlayerPos -= vPlayerDir * 25.f;
		//_vec3 vLightPos = vPlayerPos;
		//vLightPos += 250.f * vPlayerDir;
		//_vec3 vLightAt = vLightPos;
		//vLightPos.y += 10.f;
		_vec3 vLightPos = _vec3(-400.f, 100.f, 600.f);
		_vec3 vLightDir = _vec3(1.f, -10.f, -15.f);
		D3DXVec3Normalize(&vLightDir, &vLightDir);
		D3DXMatrixLookAtLH(&matView, &(vPlayerPos - vLightDir * 100.f), &(vPlayerPos) , &_vec3(0.f, 1.f, 0.f));
		//D3DXMatrixLookAtLH(&matView, &vLightPos, &vLightAt, &_vec3(0.f, 1.f, 0.f));

		m_pDevice->SetTransform(D3DTS_VIEW, &matView);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	}
	else
	{
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFovY), m_fAspect, m_fNear, m_fFar);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	// �׽�Ʈ�ڵ� ���� ////////////////////////////////////////////////////////////////////////
		D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
		m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	}

	return _int();
}

CTPV_Camera * CTPV_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const float & fFovY, const float & fAspect, const float & fNear, const float & fFar)
{
	CTPV_Camera*	pInstance = new CTPV_Camera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTPV_Camera::Free(void)
{
	Safe_Release(m_pPlayer);

	CCamera::Free();
}

void CTPV_Camera::Check_TPVCamFov(const _double& _dTimeDelta)
{
	m_dMoveFovTime += _dTimeDelta;

	m_fFovY += m_fMoveFovRatio * _float(_dTimeDelta / m_dMoveTargetFovTime);

	if (m_dMoveTargetFovTime < m_dMoveFovTime)
	{
		m_dMoveFovTime = 0.0;
		m_bIsMoveFov = false;

		m_fFovY = m_fTargetFov;
		m_fTargetFov = 0.f;
		m_fMoveFovRatio = 0.f;
	}

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFovY), m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CTPV_Camera::Set_MoveFov(_float _fTargetFov, _double _dTargetTime)
{
	m_bIsMoveFov = true;
	m_fTargetFov = _fTargetFov;
	m_dMoveTargetFovTime = _dTargetTime;
	m_fMoveFovRatio = m_fTargetFov - m_fFovY;
}

void CTPV_Camera::Set_IsPlayerRide(_bool _bIsRide, const _float& _fCamDis)
{
	//m_fMinCamDis = _fMinCamDis;
	//m_fMaxCamDis = _fMaxCamDis;
	m_bIsRidePlayer = _bIsRide;

	m_fCamDis = _fCamDis;
}

void CTPV_Camera::Set_IsPlayerShoulderView(_bool _bIsShoulderView)
{
	m_bIsShoulderView = _bIsShoulderView;

	if (_bIsShoulderView)
		m_fCamDis = 2.5f;
	else
		m_fCamDis = 3.0f;
}

void CTPV_Camera::Key_Input(const _double & _dTimeDelta)
{
	// ���콺 ����
	if(Engine::Get_DIKeyDown(DIK_G))
	{
		m_bIsFixMouse = !m_bIsFixMouse;
	}

	// ���Ż���
	if (Engine::Get_DIKeyDown(DIK_F4))
	{
		m_vEye = _vec3(0.f, 0.f, 0.f);
		m_vAt = _vec3(0.f, 0.f, 0.f);
		m_vUp = _vec3(0.f, 1.f, 0.f);
		m_fFovY = D3DXToRadian(60.f);
		m_fAspect = _float(WINCX) / WINCY;
		m_fNear = 1.f;
		m_fFar = 1000.f;

		m_bIsRidePlayer = false;

		if (FAILED(CCamera::Ready_GameObject()))
			return;
	}
}

void CTPV_Camera::Mouse_Move(const _double & _dTimeDelta)
{
	_long	dwMouseMove = 0;
	//
	// ī�޶� ���� ������
	if (dwMouseMove = Get_DIMouseMove(DIMS_Y))
	{
		m_fAngleY += dwMouseMove * (_float)_dTimeDelta * 10.f;
	
		if (-80.f >= m_fAngleY)
			m_fAngleY = -80.f;
		else if (80.f <= m_fAngleY)
			m_fAngleY = 80.f;
	}


	if (dwMouseMove = Get_DIMouseMove(DIMS_X))
	{
		m_fAngleX += dwMouseMove * (_float)_dTimeDelta * 3.f;
	}

	if (-360.f >= m_fAngleX)
		m_fAngleX += 360.f;
	else if (360.f < m_fAngleX)
		m_fAngleX -= 360.f;



	//if (dwMouseMove = Get_DIMouseMove(DIMS_Z))
	//{
	//	m_fFovY -= dwMouseMove * 0.005f;
	//
	//	if (60.f < m_fFovY)
	//		m_fFovY = 60.f;
	//	else if (30.f > m_fFovY)
	//		m_fFovY = 30.f;
	//}

	//if (dwMouseMove = Get_DIMouseMove(DIMS_Z))
	//{
	//	m_fCamDis -= dwMouseMove * 0.005f;
	//
	//	if (m_fMinCamDis > m_fCamDis)
	//		m_fCamDis = m_fMinCamDis;
	//	else if (m_fMaxCamDis < m_fCamDis)
	//		m_fCamDis = m_fMaxCamDis;
	//}
}

void CTPV_Camera::Looking_Player(const _double& _dTimeDelta)
{
	if (nullptr == m_pPlayer) 
		return;

	_vec3 vPos, vLook, vRight;

	CTransform* pTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
	CTransform* pTransformFPV = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_TransformFPV", ID_DYNAMIC));
	pTransform->Get_Info(INFO_POS, &vPos);
	pTransform->Get_Info(INFO_LOOK, &vLook);
	pTransform->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);


	/// ������ �̿��Ͽ� ī�޶��� ����Ʈ ���� ���ϱ�
	D3DXVec3Cross(&m_vTPVCamRight, &m_vTPVCamLook, &m_vUp);

	// ���콺 �Է°��� ���� ȸ����� �����
	_mat	matCamera;
	D3DXMatrixRotationYawPitchRoll(&matCamera, D3DXToRadian(m_fAngleX), D3DXToRadian(-m_fAngleY), 0.f);


	// ȸ������ camlook�� ȸ��
	D3DXVec3TransformNormal(&m_vTPVCamLook, &_vec3(0.f, 0.f, -1.f), &matCamera);
	D3DXVec3Normalize(&m_vTPVCamLook, &m_vTPVCamLook);


	// vPos���� �÷��̾� �Ӹ� ��ġ�� �α�
	m_fEyeHeightAngle = m_fAngleY * 0.005f;

	vPos.y += m_fEyeHeight;
	vPos += -(vLook * m_fEyeHeightAngle);

	m_vAt = vPos + (m_vTPVCamLook * 50.f);
	

	// eye�� ��ġ ���ϱ�
	_vec3 vEyeDis = m_vTPVCamLook * -1.f;							// at ���� Player �� ���� ���ϱ�
	
	// ī�޶� raycast�� �� �Ѿ�°� ����
	vPos += vEyeDis * 0.7f;
	
	//_float fMaxDis;
	_vec3 vPickPos;

	//Cam_BackRayCast(vPos, vEyeDis, &fMaxDis, &vPickPos);

	//fMaxDis -= 4.f;

	//if (2.f > fMaxDis)
	//	fMaxDis = 2.f;

	/// �ε巯�� ī�޶� �̵�
	//m_vFixEye = m_vAt + ((m_vTPVCamLook * -1.f) * (50.f + min(fMaxDis, m_fCamDis)));		// eye�� �÷��̾� �ڷ� �ø�ŭ at ���� ���ֱ� (���� ī�޶� ���� ��ġ ���ϱ�)

	/// ī�޶� �����
	if(m_bIsShoulderView)
		m_vFixEye += vRight * -(1.0f/*m_fCamDis * 0.3f*/);

	// ī�޶� ��ġ ��������
	if (!m_bIsLerp)
		m_vEye = m_vFixEye;
	else
		D3DXVec3Lerp(&m_vEye, &m_vPreEyePos, &m_vFixEye, (_float)_dTimeDelta * 15.f /*(_float)_dTimeDelta * 35.f*/);

	// ���� �������� ���� ���� ī�޶� ��ġ ����
	m_vPreEyePos = m_vEye;


	/// �÷��̾� ȸ��
	if (!m_bIsRidePlayer)
	{
		_vec3 vCamLook = m_vTPVCamLook;
		vCamLook.y = 0.f;
		D3DXVec3Normalize(&vCamLook, &vCamLook);
	
		_float fRatio = D3DXVec3Dot(&_vec3(0.f, 0.f, -1.f), &vCamLook);
		_float fRdian = acosf(fRatio);
	
		if (0.f < vCamLook.x)
			fRdian *= -1.f;
	
		pTransform->Set_Rotate(ROT_Y, fRdian);
		pTransformFPV->Set_Rotate(ROT_Y, fRdian);
	}
}

//void CTPV_Camera::Mouse_Fix()
//{
//	if (m_bIsFixMouse)
//	{
//		POINT		ptMouse{ WINCX >> 1, WINCY >> 1 };
//
//		ClientToScreen(g_hWnd, &ptMouse);
//		SetCursorPos(ptMouse.x, ptMouse.y);
//	}
//}

//_float CTPV_Camera::Cam_BackRayCast(const _vec3& _vPos, const _vec3& _vDir, _float* _pOutPickDis/* = nullptr*/, _vec3* _pOutPickPos/* = nullptr*/)
//{
//	PxVec3 pxPos = PxVec3(_vPos.x, _vPos.y, _vPos.z);
//	PxVec3 pxDir = PxVec3(_vDir.x, _vDir.y, _vDir.z);
//
//	return CPhysX_Mgr::GetInstance()->RayCast(pxPos, pxDir, 4.f/*m_fMaxCamDis*/, _pOutPickDis, _pOutPickPos);
//}

