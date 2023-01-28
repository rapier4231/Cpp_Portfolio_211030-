#pragma once
#ifndef __BULLET_H__
#define __BULLET_H__

#include "Unit.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CStaticMesh;
class CSphereCollider;
END

class CBullet : public CUnit
{
private:
	explicit CBullet(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBullet(const CBullet& _rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double & _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CBullet*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void					TobeBolaWrapped(const _mat& _matTargetWorld, const _float& _fScale);
	void					Set_Attacker(CUnit* _pAttacker) { m_pAttcker = _pAttacker; m_pAttcker->AddRef(); }
	void					Set_ParentMat(_mat* _pParentMat);

	const BULLET_DESC&		Get_BulletInfo() const { return m_tBulletInfo; }

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);
	HRESULT					Add_MeshComponent();

private:
	void					Check_DinoHit();

private:
	CTransform*				m_pTransformCom = nullptr;		// 트랜스폼
	CShader*				m_pShaderCom = nullptr;			// 쉐이더
	CStaticMesh*			m_pMeshCom = nullptr;			// 매쉬
	CStaticMesh*			m_pMeshBola = nullptr;			// 볼라 잔여물
	CSphereCollider*		m_pSphereColliderCom = nullptr;	// 콜리더

	CUnit*					m_pAttcker = nullptr;
	CGameObject*			m_pTargetObj = nullptr;

	PxCapsuleController*	m_pCtrl = nullptr;
	PxRigidDynamic*			pDynamic = nullptr;

	BULLET_DESC				m_tBulletInfo;

	///
	_double					m_dTime = 0.0;
	_float					m_fYaw = 0.f;
	_float					m_fPitch = 0.f;
	_float					m_fRoll = 0.f;

	_bool					m_bIsHit = false;

	_mat*					m_pParentMat = nullptr;
	_mat					m_matLoaclMat;
};

#endif // !__BULLET_H__
