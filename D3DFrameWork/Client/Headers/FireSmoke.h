#pragma once
#ifndef __FIRESMOKE_H__
#define __FIRESMOKE_H__

#include "Value.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_FireSmokeParticleTex;
class CShader;
class CTexture;;
class CTransform;
END

class CFireSmoke : public CGameObject
{
private:
	explicit CFireSmoke(LPDIRECT3DDEVICE9 _pDevice);
	explicit CFireSmoke(const CFireSmoke& rhs);
	virtual ~CFireSmoke() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CFireSmoke*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void	Set_Ignition(_bool _bIgnition) { m_bIgnition = _bIgnition; }
	_bool	Get_Ignition() { return m_bIgnition; }

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	CVIBuffer_FireSmokeParticleTex*	m_pBufferCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;
	CShader*						m_pShaderCom = nullptr;

	_vec2					m_vTexUV;
	_double					m_dTime;
	_double					m_dCount;
	_int					m_iTextureX;
	_int					m_iTextureY;
	vector<_vec2>			m_vecTexUV;
	_float					m_fScale;
	_float					m_fAlpha;

	_bool					m_bIgnition;
	_bool					m_bUpdated = false;
};

#endif // !__FIRESMOKE_H__