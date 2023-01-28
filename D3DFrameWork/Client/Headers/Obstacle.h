#pragma once
#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "Value.h"
#include "Unit.h"

BEGIN(Engine)
class CStaticMesh;
class CShader;
class CCalculator;
class CTransform;
END

class CObstacle : public CUnit
{
private:
	explicit CObstacle(LPDIRECT3DDEVICE9 _pDevice);
	explicit CObstacle(const CObstacle& rhs);
	virtual ~CObstacle() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;
	//virtual void			Render_GameObjectShadowMap() override;
	//virtual void			Render_GameObjectShadow() override;

public:
	static CObstacle*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;


private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수
	virtual _int			Check_Arg(void * _pArg) override;

public:
	HRESULT					Init_Mesh(wstring strMeshTag, _vec3 vScale, _vec3 vRot, _vec3 vPos);

private:
	CStaticMesh*			m_pMeshCom = nullptr;
	/*CShader*				m_pShaderCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;	
	CTransform*				m_pTransformCom = nullptr;*/

	//_vec3					m_vColor;
	_float					m_fDetail = 0.f;
	_vec3					m_vBoundingBox[8];
	_float					m_fBoundingRadius = 0.f;
	_vec3					m_vBoundingCenter;
};

#endif // !__PLAYER_H__