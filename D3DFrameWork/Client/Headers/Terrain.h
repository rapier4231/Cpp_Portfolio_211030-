//////////////////////////////////////////////////////////////////////////
//	현재 사용하지 않음(CTile로 전용함)										//
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "Value.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Terrain;
class CTexture;
class CTransform;
class CShader;
END

class CTerrain : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 _pDevice);
	explicit CTerrain(const CGameObject& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void* _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual void			Render_GameObject() override;

private:
	HRESULT					Add_Component(void);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);
	HRESULT					SetUp_Material(void);

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void* _pArg = nullptr) override;
	virtual void			Free() override;

private:
	CVIBuffer_Terrain*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pFilterCom = nullptr;
	CTexture*				m_pAuraCom = nullptr;

	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

};

#endif // !__TERRAIN_H__
