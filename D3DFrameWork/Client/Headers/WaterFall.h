#pragma once
#ifndef __WATERFALL_H__
#define __WATERFALL_H__

#include "Value.h"
#include "Unit.h"

BEGIN(Engine)

class CStaticMesh;

END

class CWaterFall : public CUnit
{
private:
	explicit CWaterFall(LPDIRECT3DDEVICE9 _pDevice);
	explicit CWaterFall(const CWaterFall& rhs);
	virtual ~CWaterFall() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;
	virtual void			Render_GameObjectShadowMap() override;
	virtual void			Render_GameObjectShadow() override;

public:
	static CWaterFall*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	CStaticMesh*			m_pMeshCom = nullptr;
	CTexture*				m_pNoiseTextureCom = nullptr;
	CTexture*				m_pGradientTextureCom = nullptr;

	_vec3					m_vBoundingBox[8];
	_float					m_fBoundingRadius = 0.f;
	_vec3					m_vBoundingCenter;

	_float					m_fNoiseAcc = 0.f;
};

#endif // !__PLAYER_H__