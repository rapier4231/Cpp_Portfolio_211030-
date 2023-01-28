#pragma once
#ifndef __EFFECTOBJECT_H__
#define __EFFECTOBJECT_H__

#include "Unit.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CStaticMesh;
END

class CEffectObject : public CGameObject
{
private:
	explicit CEffectObject(LPDIRECT3DDEVICE9 _pDevice);
	explicit CEffectObject(const CEffectObject& _rhs);
	virtual ~CEffectObject() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CEffectObject*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수

private:
	CStaticMesh*			m_pMeshCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

	_bool					m_bIsRender = false;

	_vec2					m_vTexUV;
	_double					m_dTime;
	_double					m_dCount;
	_int					m_iTextureX;
	_int					m_iTextureY;
	vector<_vec2>			m_vecTexUV;
	_float					m_fScale;
	_float					m_fAlpha;

	_int					m_iCount;
};

#endif // !__EFFECTOBJECT_H__
