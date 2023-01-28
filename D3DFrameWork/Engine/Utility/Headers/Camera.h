#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera(void) = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int Update_GameObject(const _double & _dTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	virtual CGameObject * Clone(void * _pArg = nullptr) override;
	virtual void Free() override;

#ifdef _DEBUG
public:
	const _vec3& Get_CameraInfo(_uint _iInfo) const;
#endif // _DEBUG

public:
	const _vec3		Get_CameraPos() { return m_vEye; }
	const _vec3		Get_CameraDir();
	_mat			Get_ReflectView(_float _fHeight);

protected:
	_vec3		m_vEye, m_vAt, m_vUp, m_vReflectEye;
	_float		m_fAspect, m_fFovY, m_fNear, m_fFar;
	_mat		m_matView, m_matProj, m_matReflectView;
};
END
#endif // !__CAMERA_H__
