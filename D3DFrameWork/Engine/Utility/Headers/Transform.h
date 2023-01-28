#pragma once
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pDevice);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	HRESULT				Ready_Component_Prototype() override;
	HRESULT				Ready_Component(void* _pArg = nullptr) override;
	virtual _int		Update_Component(const _double& _dTimeDelta);

public:
	static CTransform*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone(void * pArg = nullptr) override;
	virtual void		Free() override;

public:
	void				Get_Info(INFO eType, _vec3* pInfo);

	const _mat&			Get_WorldMatrix() const;
	void				Get_NRotWorldMatrix(_mat* pMatrix)const;
	_mat*				Get_MatRotAxis() { return &m_matRotAxis; }
	void				Set_ParentMatrix(const _mat* pParentMatrix);
	void				Set_WorldMatrix(const _mat* pWorld);

	_float				Get_Rotate(ROTATE eType);
	const _vec3*		Get_Scale();

	//값 고정 세팅
	void				Set_Scale(const _float& _fX, const _float& _fY, const _float& _fZ);
	void				Set_Scale(const _vec3& _vScale);

	const _vec3*		Get_Pos() { return &m_vInfo[INFO_POS]; }
	//값 고정 세팅
	void				Set_Pos(const _float& _fX, const _float& _fY, const _float& _fZ);
	void				Set_Pos(const _vec3& _vPos);
	void				Move_Pos(const _vec3& _vDir, const _float& _fSpeed, _double _dTimeDelta);
	
	// 값 고정 세팅	
	void				Set_Rotate(ROTATE eType, const _float& _fAngle);

	// 값 누적 세팅
	void				Set_AccRotate(ROTATE eType, const _float& _fAngle);
	void				Set_AxisAccRotate(const _vec3& _vAxis, const _float& _fAngle);
	void				Set_YawPitchRoll(const _float& _fYaw, const _float& _fPitch, const _float& _fRoll);
	void				Set_YawPitchRoll(const _mat& matrix);

	// Axis 축 회전 사용 여부
	void				Set_IsAxisRoate(const _bool& _bIsAxis) { m_bIsRotateAxis = _bIsAxis; }
	void				Set_IsYawPitchRoll(const _bool& _bIsYawPitchRoll) { m_bIsYawPitchRoll = _bIsYawPitchRoll; }

private:
	_vec3				m_vInfo[INFO_END];
	_vec3				m_vScale;
	_vec3				m_vAngle;
	_mat				m_matWorld;
	_mat				m_matNRotWorld;

	_bool				m_bIsRotateAxis = false;
	_mat				m_matRotAxis;

	_bool				m_bIsYawPitchRoll = false;
	_mat				m_matYawPitchRoll;
};

END

#endif // !__TRANSFORM_H__
