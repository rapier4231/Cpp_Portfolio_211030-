#pragma once
#ifndef __BasketballHoop_H__
#define __BasketballHoop_H__

#include "Value.h"
#include "Unit.h"

BEGIN(Engine)
class CStaticMesh;
class CShader;
class CSphereCollider;
END

class CBasketballHoop : public CUnit
{
private:
	explicit CBasketballHoop(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBasketballHoop(const CBasketballHoop& rhs);
	virtual ~CBasketballHoop() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;
	//virtual void			Render_GameObjectShadowMap() override;
	//virtual void			Render_GameObjectShadow() override;

public:
	static CBasketballHoop*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;
	virtual void				Open_Ring_Command(CUnit* pUnit) override;
	virtual void				Set_Ring_Command(_int _iSelectCommand) override;

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수
	virtual _int			Check_Arg(void * _pArg) override;

public:
	HRESULT					Init_Mesh(wstring strMeshTag, _vec3 vScale, _vec3 vRot, _vec3 vPos);

private:
	CStaticMesh*			m_pMeshCom = nullptr;
	CSphereCollider*		m_pSphereColliderCom = nullptr;


	_vec3					m_vBallSpawnPos = { 0.f,0.f,0.f };
	_float					m_fDetail = 0.f;
	_vec3					m_vBoundingBox[8];
	_float					m_fBoundingRadius = 0.f;
	_vec3					m_vBoundingCenter;

	_bool					m_bGameStart = false;
	_double					m_dTimeSetting = 0.0;
	// 링커맨드 관련
	vector<CRingCommand_UI::STRCD> m_vRingCommand;
};

#endif // !__PLAYER_H__