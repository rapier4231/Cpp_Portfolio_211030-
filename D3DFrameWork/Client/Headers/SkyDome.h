#pragma once
#ifndef __SKYDOME_H__
#define __SKYDOME_H__

#include "Value.h"
#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CShader;
class CCalculator;
class CTransform;
END

class CSkyDome : public CGameObject
{
private:
	explicit CSkyDome(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSkyDome(const CSkyDome& rhs);
	virtual ~CSkyDome() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CSkyDome*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수

private:
	CStaticMesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	_mat					m_matWorld;
	_float					m_fPosYAdj = 0.f;
};

#endif // !__SKYDOME_H__