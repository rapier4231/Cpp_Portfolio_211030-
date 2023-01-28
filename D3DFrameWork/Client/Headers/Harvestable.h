#pragma once
#ifndef __HARVESTABLE_H__
#define __HARVESTABLE_H__

#include "Unit.h"

BEGIN(Engine)
class CStaticMesh;
class CDynamicMesh;
class CSphereCollider;
END

class CHarvestable abstract : public CUnit
{
protected:
	explicit CHarvestable(LPDIRECT3DDEVICE9 _pDevice);
	explicit CHarvestable(const CHarvestable& rhs);
	virtual ~CHarvestable() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	//충돌체 장착
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	virtual CGameObject *	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

	virtual _bool			HitBy(CUnit* pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos);

private:
	HRESULT					Add_Component_Prototype();
	HRESULT					Add_Component();

protected:
	HRESULT					Init_Mesh(wstring strMeshTag);
	HRESULT					Init_DynamicMesh(wstring strMeshTag, _vec3 vScale, _vec3 vRot, _vec3 vPos);
	virtual _bool			Add_RenderGroup_Culling(RENDERID eID, CGameObject * pGameObject) override;

protected:
	CStaticMesh*			m_pMeshCom = nullptr;
	CDynamicMesh*			m_pDynamicMeshCom = nullptr;
	CSphereCollider*		m_pSphereColliderCom = nullptr;

	//경계구체
	_float					m_fBoundingRadius = 0.f;
	_vec3					m_vBoundingCenter;

	//경계상자
	_vec3					m_vBoundingBox[8];
};

#endif // !__HARVESTABLE_H__
