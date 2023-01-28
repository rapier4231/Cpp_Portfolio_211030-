#pragma once
#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__

#include "Unit.h"

BEGIN(Engine)
class CStaticMesh;
class CSphereCollider;
END

class CStructure : public CUnit
{
protected:
	explicit CStructure(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStructure(const CStructure& _rhs);
	virtual ~CStructure() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double & _dTimeDelta) override;

	virtual void			Render_GameObject() override;
	virtual void			Render_GameObjectShadowMap() override;
	virtual void			Render_GameObjectShadow() override;

public:
	virtual void			Free();

public:
	void					Set_PreviewPos(const _vec3& _vPos);
	void					Set_PreviewAngle(const _float& _fAngleY);
	void					Set_TartgetPlatform(CGameObject* _pTargetPlatform);// { m_pTargetPlatform = _pTargetPlatform; }

	const STRUCTURE_TYPE&	Get_StructType() const { return m_tStructurInfo.eStructureType; }
	const _ulong&			Get_StructIndex() const { return m_tStructurInfo.dwIndex; }
	const _float&			Get_StructAngle() const { return m_tStructurInfo.fAngleY; }
	virtual const _vec3		Get_PivotDir(_uint _eCorDir) { return _vec3(); }
	CGameObject*			Get_TargetPlatform() { return m_pTargetPlatform; }

protected:
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수
	HRESULT					Set_PhysXCook();
	void					Move_PhysX();

	virtual	void			Move_Pivot();

protected:
	CStaticMesh*			m_pMeshCom = nullptr;
	CStaticMesh*			m_pMeshLodCom = nullptr;
	CSphereCollider*		m_pSphereColliderCom = nullptr;

	//_bool					m_bIsPreview = false;

	// 건축물 인덱스
	static _ulong			m_dwIndex;

	// 건축물 인포
	STRUCTURE_DESC			m_tStructurInfo;
	CGameObject*			m_pTargetPlatform = nullptr;
	_mat					m_matLocal;

	PxRigidStatic*			m_pBaseActor = nullptr;
};

#endif // !__STRUCTURE_H__
