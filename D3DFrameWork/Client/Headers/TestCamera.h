#pragma once
#ifndef __TESTCAMERA_H__
#define __TESTCAMERA_H__

//#include "Value.h"
#include "Camera.h"

class CPlayer;

class CTPV_Camera : public CCamera
{
private:
	explicit CTPV_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTPV_Camera(const CTPV_Camera& rhs);
	virtual ~CTPV_Camera() = default;

public:
	HRESULT					Ready_GameObject(const _vec3* pEye,
												const _vec3* pAt,
												const _vec3* pUp,
												const float& fFovY,
												const float& fAspect,
												const float& fNear,
												const float& fFar);
	virtual _int			Update_GameObject(const _double& _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;

public:
	static CTPV_Camera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _vec3* pEye,
									const _vec3* pAt,
									const _vec3* pUp,
									const float& fFovY,
									const float& fAspect,
									const float& fNear,
									const float& fFar);

public:
	virtual void			Free(void) override;

public:
	const _float&			Get_TPVCam_AngleY() const { return m_fAngleY; }
	const _float&			Get_TPVCam_AngleX() const { return m_fAngleX; }
	const _vec3&			Get_TPVCam_Look() const { return m_vTPVCamLook; }
	const _float&			Get_TPVCam_Dis() const { return m_fCamDis; }

	void					Set_TPVCamAccRotatX(const _float& _fAngleX) { m_fAngleX += _fAngleX; }
	void					Set_EyeHeight(const _float& _fEyeHeight) { m_fEyeHeight = _fEyeHeight; }
	void					Set_IsLerp(_bool _bIsLerp) { m_bIsLerp = _bIsLerp; }
	void					Set_MoveFov(_float _fTargetFov, _double _dTargetTime);

public:
	void					Set_IsPlayerRide(_bool _bIsRide, const _float& _fCamDis);
	void					Set_IsLockKeyInput(_bool _bIsLockKeyInput) { m_bIsLockKeyInput = _bIsLockKeyInput; }
	void					Set_IsPlayerShoulderView(_bool _bIsShoulderView);

private:
	void					Key_Input(const _double& _dTimeDelta);
	void					Mouse_Move(const _double& _dTimeDelta);
	void					Looking_Player(const _double& _dTimeDelta);

	void					Mouse_Fix();

	_float					Cam_BackRayCast(const _vec3& _vPos, const _vec3& _vDir, _float* _pOutPickDis = nullptr, _vec3* _pOutPickPos = nullptr);
	void					Check_TPVCamFov(const _double& _dTimeDelta);

private:
	CPlayer*				m_pPlayer = nullptr;

	_float					m_fAngleY = 0.f;
	_float					m_fAngleX = 0.f;
	_float					m_fCamDis = 3.0f;
	//_float					m_fMinCamDis = 3.f;
	//_float					m_fMaxCamDis = 15.f;

	_vec3					m_vTPVCamLook;
	_vec3					m_vTPVCamRight;

	_vec3					m_vFixEye;
	_vec3					m_vPreEyePos;

	_float					m_fEyeHeight = 1.8f;
	_float					m_fEyeHeightAngle = 0.f;

	/// 카메라 특성
	_bool					m_bIsLockKeyInput = false;			// 인벤토리 볼 때
	_bool					m_bIsShoulderView = false;			// 숄더뷰


	// 공룡 탑승중일 때 카메라
	_bool					m_bIsRidePlayer = false;
	_bool					m_bIsFixMouse = true;


	/// 임시로 카메라 잡기
	_bool					m_bIsLerp = false;

	// Fov 값 변경
	_bool					m_bIsMoveFov = false;
	_double					m_dMoveFovTime = 0.0;
	_double					m_dMoveTargetFovTime = 0.0;
	_float					m_fTargetFov = 0.f;
	_float					m_fMoveFovRatio = 0.f;
};

#endif // !__TPV_CAMERA_H__
