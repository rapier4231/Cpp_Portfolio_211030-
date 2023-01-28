#pragma once
#ifndef __TPV_QUATERNIONCAMERA_H__
#define __TPV_QUATERNIONCAMERA_H__

#include "Value.h"
#include "Camera.h"

class CPlayer;

class CTPV_QuaternionCamera : public CCamera
{
private:
	explicit CTPV_QuaternionCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTPV_QuaternionCamera(const CTPV_QuaternionCamera& rhs);
	virtual ~CTPV_QuaternionCamera() = default;

public:
	HRESULT					Ready_GameObject(const _float& fFovY,
												const _float& fAspect,
												const _float& fNear,
												const _float& fFar);
	virtual HRESULT			Late_Ready_GameObject(void* _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double& _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;

public:
	static CTPV_QuaternionCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
												const _float& fFovY,
												const _float& fAspect,
												const _float& fNear,
												const _float& fFar);
public:
	virtual void			Free(void) override;

public:
	void					Set_CameraInfo(const _float& _fAngleX, const _float& _fAngleY, _bool _bIsMoveCamera);		// �÷��̾ ó������ ī�޶� ���� ���� �� ī�޶� ������ �ʱ�ȭ ���ִ� �Լ�
	void					Set_MoveCamera(_float _fTargetDis, _float _fTargetRightDis, _float _fTargetFov, _double _dTargetTime, _bool _bIsToTPVCamera = false, _bool _bIsSwitchCamera = false);
	//void					Set_IsPlayerShoulderView(_bool _bIsShoulderView);
	void					Set_TPVCam_MaxDis(const _float& _fCameraDis);

	const _float&			Get_TPVCam_Dis() { return m_fCameraDis; }

private:
	_vec3*					Get_Axis(D3DXQUATERNION* _quat, _vec3* _pAxis);		// ���ʹϾ� ����� ������ ���� �������� �Լ�
	_bool					Lock_BackDistance(const _vec3& _vRayPos, const _vec3& _vRayDir, const _float& _vRayDistance, _float* _pOutPickDis = nullptr, _vec3* _pOutPickPos = nullptr);	// ī�޶� ������ ���� ���ϰ� ����ĳ�������� ���������� �Ÿ��� ��ȯ�ϴ� �Լ�

	void					Move_CamPos(const _double & _dTimeDelta);

private:
	CPlayer*				m_pPlayer = nullptr;

	D3DXQUATERNION			m_quatCamDir;
	_float					m_fSlerpSpeed = 10.f;

	// �յ�
	_float					m_fCameraDis = 0.f;
	_float					m_fCameraMinDis = MINCAMDIS;
	_float					m_fCameraMaxDis = MAXCAMDIS;
	_float					m_fTargetDis = 0.f;
	_float					m_fMoveDisRatio = 0.f;

	// �¿�
	_float					m_fRightDis = 0.f;
	_float					m_fTargetRightDis = 0.f;
	_float					m_fMoveRightDisRatio = 0.f;

	// FOV
	_float					m_fTargetFov = 0.f;
	_float					m_fMoveFovRatio = 0.f;

	// ī�޶� ����
	_bool					m_bIsCameraMove = false;
	_double					m_dCameraMoveTime = 0.0;
	_double					m_dTargetCameraMoveTime = 0.0;

	_bool					m_bIsToFPV = false;

	_bool					m_bIsShoulderView = false;		// �����
	_bool					m_bIsPlayerRide = false;		// ����ž�¿���
};

#endif // !__TPV_QUATERNIONCAMERA_H__
