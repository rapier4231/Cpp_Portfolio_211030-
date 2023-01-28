#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Component.h"

BEGIN(Engine)
class CCamera;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 _pDevice);
	explicit CGameObject(const CGameObject& _rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() PURE;
	virtual HRESULT			Ready_GameObject(void* _pArg = nullptr) PURE;
	virtual HRESULT			Late_Ready_GameObject(void* _pArg = nullptr) { return S_OK; }
	virtual _int			Update_GameObject(const _double& _dTimeDelta) PURE;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) { return 0; }
	virtual void			Render_GameObject() PURE;
	virtual void			Render_GameObject(_mat _matOrtho) { return; }
	virtual void			Render_GameObject(_mat _matOrtho, _mat matOldProj) { return; }
	virtual void			Render_GameObjectShadowMap() { return; }
	virtual void			Render_GameObjectShadow() { return; }
	virtual void			Setup_Instance(LPD3DXEFFECT & _pEffect) { return; }

	void					Set_ShadowMap(_vec3 _vLightPos, _vec3 _vLightAt);
	
	void					Set_Dead() { m_bDead = true; }
	_bool					Get_Dead() {return m_bDead;}
	void					Compute_ViewZ(const _vec3* pPos);
	_float					Get_ViewZ(void) const { return m_fViewZ; }

	void					Set_Color(const _float& _fX, const _float& _fY, const _float& _fZ);
	_vec3					Get_Color();

	void					Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }

public:
	CComponent*				Get_Component(const wstring& _strComponentTag, COMPONENTID _eComponentID);
public:
	virtual CGameObject*	Clone(void* _pArg = nullptr) PURE;
	virtual void			Free() PURE;

protected:
	map<wstring, CComponent*>	m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9			m_pDevice = nullptr;

	_bool			m_bDead;
	_bool			m_bInit;
	_bool			m_bIsClone;

	_float			m_fViewZ;

	//셰이더 디퓨즈값에 변화주기 위한 용도의 컬러
	_vec3			m_vColor;

	// Shadow 관련
	_mat			m_matLightView;
	_mat			m_matLightViewRot;
	_mat			m_matLightProj;
	_mat			m_matLightProjRot;
	_mat			m_matScaleBias;
	_mat			m_matView;
	_mat			m_matProj;
	_mat			m_matWVP;
	_mat			m_matWLV;
	_mat			m_matWLVB;
	_mat			m_matWLVRot;
	_vec3			m_vecLightPos;
	_vec3			m_vecLightPosRot;
	_vec3			m_vecLightAt;
	_vec3			m_vecLightAtRot;
	_vec3			m_vecLightUp;
	_vec4			m_vDir;

	CGameObject*	m_pTarget = nullptr;

	// 테스트용
	_float			m_fPosX = 0.f, m_fPosY = 0.f, m_fPosZ = 0.f;

	_int			m_iLightIndex = 0;		// Light Index
};
END

#endif // !__GAMEOBJECT_H__
