#pragma once
#ifndef __CAMERAMRG_H__
#define __CAMERAMRG_H__

#include "Value.h"
#include "GameObject.h"

class CCameraMrg : public CGameObject
{
public:
	enum CAMERA_TYPE
	{
		CAMERA_FPV,
		CAMERA_TPV,
		CAMERA_END
	};
private:
	explicit CCameraMrg(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCameraMrg(const CCameraMrg& rhs);
	virtual ~CCameraMrg() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double & _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CCameraMrg*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CGameObject *	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void					Set_CameraType(CAMERA_TYPE _eCameraType, const _float& _fAngleX, const _float& _fAngleY, _bool _bIsMoveCamera = false);
	void					Set_TPVMoveCamera(_float _fTargetDis, _float _fTargetRightDis, _float _fTargetFov, _double _dTargetTime, _bool _bIsToTPVCamera = false, _bool _bIsSwitchCamera = false);
	//void					Set_TPVIsPlayerShoulderView(_bool _bIsShoulderView);
	void					Set_FixMouseCursor(_bool _bIsFixMouse) { m_bIsFixMouse = _bIsFixMouse; }

	void					Set_TPVCam_MaxDis(const _float& _fCameraDis);

	const _float&			Get_TPVCam_Dis();

private:
	void					Camera_Key_Input();

private:
	vector<CCamera*>		m_vecCamera;

	CAMERA_TYPE				m_eCameraIndex = CAMERA_END;

	_bool					m_bIsFixMouse = true;
};

#endif // __CAMERA_H__
