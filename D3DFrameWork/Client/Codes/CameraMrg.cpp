#include "stdafx.h"
#include "CameraMrg.h"

#include "Export_Function.h"

USING(Engine)

#include "FPV_QuaternionCamera.h"
#include "TPV_QuaternionCamera.h"


CCameraMrg::CCameraMrg(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CCameraMrg::CCameraMrg(const CCameraMrg & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCameraMrg::Ready_GameObject_Prototype()
{
	/// 카메라 2개 사용
	m_vecCamera.reserve(2);

	// 1인칭
	CCamera* pCamera = CFPV_QuaternionCamera::Create(m_pDevice, 50.f, _float(WINCX) / WINCY, 0.1f, 1000.f);
	m_vecCamera.emplace_back(pCamera);

	// 3인칭
	pCamera = CTPV_QuaternionCamera::Create(m_pDevice, 60.f, _float(WINCX) / WINCY, 1.f, 1000.f);
	m_vecCamera.emplace_back(pCamera);


	// 처음 시작시 1인칭으로 사용
	m_eCameraIndex = CAMERA_FPV;

	return S_OK;
}

HRESULT CCameraMrg::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	return S_OK;
}

_int CCameraMrg::Update_GameObject(const _double & _dTimeDelta)
{
	Camera_Key_Input();

	return m_vecCamera[m_eCameraIndex]->Update_GameObject(_dTimeDelta);
}

_int CCameraMrg::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	return m_vecCamera[m_eCameraIndex]->LateUpdate_GameObject(_dTimeDelta);
}

void CCameraMrg::Render_GameObject()
{
	return;
}

CCameraMrg * CCameraMrg::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraMrg*	pInstance = new CCameraMrg(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CCameraMrg::Clone(void * _pArg/* = nullptr*/)
{
	return nullptr;
}

void CCameraMrg::Free()
{
	for(_uint i =0; i < CAMERA_END; ++i)
		Safe_Release(m_vecCamera[i]);
	m_vecCamera.clear();
	m_vecCamera.shrink_to_fit();

	CGameObject::Free();
}

void CCameraMrg::Set_CameraType(CAMERA_TYPE _eCameraType, const _float& _fAngleX, const _float& _fAngleY, _bool _bIsMoveCamera)
{
	m_eCameraIndex = _eCameraType;

	switch (_eCameraType)
	{
	case CAMERA_FPV:
		static_cast<CFPV_QuaternionCamera*>(m_vecCamera[m_eCameraIndex])->Set_CameraInfo(_fAngleX, _fAngleY);
		break;
	case CAMERA_TPV:
		static_cast<CTPV_QuaternionCamera*>(m_vecCamera[m_eCameraIndex])->Set_CameraInfo(_fAngleX, _fAngleY, _bIsMoveCamera);
		break;
	default:
		break;
	}
}

void CCameraMrg::Set_TPVMoveCamera(_float _fTargetDis, _float _fTargetRightDis, _float _fTargetFov, _double _dTargetTime, _bool _bIsToTPVCamera/* = false*/, _bool _bIsSwitchCamera/* = false*/)
{
	//if (CAMERA_FPV == m_eCameraIndex)
	//	return;
	static_cast<CTPV_QuaternionCamera*>(m_vecCamera[CAMERA_TPV])->Set_MoveCamera(_fTargetDis, _fTargetRightDis, _fTargetFov, _dTargetTime, _bIsToTPVCamera, _bIsSwitchCamera);
}

void CCameraMrg::Set_TPVCam_MaxDis(const _float & _fCameraDis)
{
	if (CAMERA_FPV == m_eCameraIndex)
		return;

	return static_cast<CTPV_QuaternionCamera*>(m_vecCamera[m_eCameraIndex])->Set_TPVCam_MaxDis(_fCameraDis);
}

const _float& CCameraMrg::Get_TPVCam_Dis()
{
	if (CAMERA_FPV == m_eCameraIndex)
		return -1.f;

	return static_cast<CTPV_QuaternionCamera*>(m_vecCamera[m_eCameraIndex])->Get_TPVCam_Dis();
}

void CCameraMrg::Camera_Key_Input()
{
	/// 마우스 중앙에 고정
	if (Engine::Get_DIKeyDown(DIK_G))
	{
		m_bIsFixMouse = !m_bIsFixMouse;
	}

	if (m_bIsFixMouse)
	{
		POINT		ptMouse{ (_int)WINCX >> 1,(_int)WINCY >> 1 };

		ClientToScreen(g_hWnd, &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);
	}
}
