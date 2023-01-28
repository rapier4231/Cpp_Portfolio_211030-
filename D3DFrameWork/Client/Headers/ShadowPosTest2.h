#pragma once
#ifndef __SHADOWPOSTEST2_H__
#define __SHADOWPOSTEST2_H__

#include "Unit.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CStaticMesh;
END

class CShadowPosTest2 : public CGameObject
{
private:
	explicit CShadowPosTest2(LPDIRECT3DDEVICE9 _pDevice);
	explicit CShadowPosTest2(const CShadowPosTest2& _rhs);
	virtual ~CShadowPosTest2() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CShadowPosTest2*	Create(LPDIRECT3DDEVICE9 _pDevice);
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

#endif // !__SHADOWPOSTEST2_H__
