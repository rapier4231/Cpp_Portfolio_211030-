#pragma once
#ifndef __TILE_H__
#define __TILE_H__

#include "Value.h"
#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CShader;
class CCalculator;
class CTransform;
END

enum TILETEXTURE_TYPE { TILETEXTURE_COVER, TILETEXTURE_SLOPE, TILETEXTURE_TOP, TILETEXTURE_FILTER, TILETEXTURE_NOISE, TILETEXTURE_END };

class CTile : public CGameObject
{
private:
	explicit CTile(LPDIRECT3DDEVICE9 _pDevice);
	explicit CTile(const CTile& rhs);
	virtual ~CTile() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;
	//virtual void			Render_GameObjectShadowMap() override;
	virtual void			Render_GameObjectShadow() override;

public:
	static CTile*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;


private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수

public:
	HRESULT					Init_Mesh(_int iMeshIdx, _vec3 vScale, _vec3 vRot, _vec3 vPos);
	HRESULT					Init_Mesh(TCHAR* szMeshTag, _vec3 vScale, _vec3 vRot, _vec3 vPos);
	HRESULT					Setup_Tile(const _uint& iPass = 0);

private:
	CStaticMesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CShader*				m_pShaderShadowMapCom = nullptr;
	CShader*				m_pShaderShadowCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	
	LPDIRECT3DTEXTURE9*		m_ppTexture[TEXTURE_END];


	_mat					m_matWorld;
	_float					m_fDetail = 0.f;
	_float					m_fDetailCover = 0.f;
	_uint					m_iPass = 0;

	_bool					m_bClone = false;
};

#endif // !__PLAYER_H__