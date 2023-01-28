#pragma once
#ifndef __DUST_H__
#define __DUST_H__

#include "Value.h"
#include "GameObject.h"

BEGIN(Engine)
//class CVIBuffer_RectTex;
class CVIBuffer_ParticleTex;
class CShader;
class CTexture;;
class CTransform;
END

class CDust : public CGameObject
{
private:
	explicit CDust(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDust(const CDust& rhs);
	virtual ~CDust() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CDust*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	CVIBuffer_ParticleTex*	m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

	_vec2					m_vTexUV;
	_double					m_dTime;
	_double					m_dCount;
	_int					m_iTextureX;
	_int					m_iTextureY;
	vector<_vec2>			m_vecTexUV;
	_float					m_fScale;
	_float					m_fAlpha;

	_bool					m_bUpdated = false;
};

#endif // !__DUST_H__