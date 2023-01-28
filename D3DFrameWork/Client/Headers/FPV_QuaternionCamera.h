#pragma once
#ifndef __FPV_QUATERNIONCAMERA_H__
#define __FPV_QUATERNIONCAMERA_H__

#include "Camera.h"

class CPlayer;

class CFPV_QuaternionCamera : public CCamera
{
private:
	explicit CFPV_QuaternionCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFPV_QuaternionCamera(const CFPV_QuaternionCamera& rhs);
	virtual ~CFPV_QuaternionCamera() = default;

public:
	HRESULT					Ready_GameObject(const _float& fFovY,
												const _float& fAspect,
												const _float& fNear,
												const _float& fFar);
	virtual HRESULT			Late_Ready_GameObject(void* _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double& _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;

public:
	static CFPV_QuaternionCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
												const _float& fFovY,
												const _float& fAspect,
												const _float& fNear,
												const _float& fFar);

public:
	virtual void			Free(void) override;

public:
	void					Set_CameraInfo(const _float& _fAngleX, const _float& _fAngleY);


private:
	CPlayer*				m_pPlayer = nullptr;
	const _mat*				m_matPlayerFPV = nullptr;

	D3DXQUATERNION			m_quatCamDir;
	_float					m_fSlerpSpeed = 10.f;
};

#endif // !__FPV_QUATERNIONCAMERA_H__
