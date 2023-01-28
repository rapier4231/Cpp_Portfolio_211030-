#pragma once
#ifndef __WATER_H__
#define __WATER_H__

#include "Value.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_WaterTerrain;
class CVIBuffer_RectTex;
class CVIBuffer_CubeTex;
class CShader;
class CTexture;;
class CTransform;
END

class CWater : public CGameObject
{
private:
	explicit CWater(LPDIRECT3DDEVICE9 _pDevice);
	explicit CWater(const CWater& rhs);
	virtual ~CWater() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CWater*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	CVIBuffer_WaterTerrain*		m_pBufferCom = nullptr;
	//CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	//CVIBuffer_CubeTex*		m_pBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CTexture*					m_pNoiseTextureCom = nullptr;
	CTexture*					m_pNormalTextureCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CShader*					m_pShaderCom = nullptr;

	_double						m_dTime;
	_double						m_dCount;
	_float						m_fScale;
	_float						m_fAlpha;
	_float						m_fTime;
};

#endif // !__WATER_H__