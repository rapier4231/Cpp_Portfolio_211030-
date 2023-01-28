#include "stdafx.h"
#include "..\Headers\Thatch_Floor.h"

#include "Export_Function.h"

#include "SphereCollider.h"

USING(Engine)

CThatch_Floor::CThatch_Floor(LPDIRECT3DDEVICE9 _pDevice)
	: CStructure(_pDevice)
{
}

CThatch_Floor::CThatch_Floor(const CThatch_Floor & _rhs)
	: CStructure(_rhs)
{
}

HRESULT CThatch_Floor::Ready_GameObject_Prototype()
{
	CStructure::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CThatch_Floor::Ready_GameObject(void * _pArg)
{
	CStructure::Ready_GameObject(_pArg);

	m_tStructurInfo.fInteractRad = 2.0f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));

	// _pArg에 Pos값을 넣은 경우(설치 완료 라는 뜻)
	if (!m_tStructurInfo.bIsPreview)
	{
		//m_tStructurInfo.eStructureType = STRUCTURE_THATCH_FLOOR;
		m_pTransformCom->Set_Pos(m_tStructurInfo.vPos);
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_tStructurInfo.fAngleY));
		m_pTransformCom->Update_Component(0);

		/// 피벗 추가
		_vec3 vLook, vRight;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);

		CSphereCollider::SPHEREINFO tInfo;
		tInfo.eColType = CSphereCollider::COL_BUILD_PIVOT;
		tInfo.fRadius = 1.f;

		tInfo.vSpherePos = vLook * 1.5f;
		tInfo.eColDir = CSphereCollider::COL_FWD;
		m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);

		tInfo.vSpherePos = vLook * -1.5f;
		tInfo.eColDir = CSphereCollider::COL_BWD;
		m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);
		
		tInfo.vSpherePos = vRight * 1.5f;
		tInfo.eColDir = CSphereCollider::COL_RIGHT;
		m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);

		tInfo.vSpherePos = vRight * -1.5f;
		tInfo.eColDir = CSphereCollider::COL_LEFT;
		m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);



		Set_PhysXCook();

		m_vColor = { 1.f, 1.f, 1.f };

		m_bDissolve = true;
		m_fDissolveAmount = 1.f;
	}

	return S_OK;
}

_int CThatch_Floor::Update_GameObject(const _double & _dTimeDelta)
{
	if (OBJECT_DEAD == CStructure::Update_GameObject(_dTimeDelta))
		return OBJECT_DEAD;

	return NO_EVENT;
}

_int CThatch_Floor::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CStructure::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CThatch_Floor::Render_GameObject()
{
	CStructure::Render_GameObject();
}

CThatch_Floor * CThatch_Floor::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CThatch_Floor*	pInstance = new CThatch_Floor(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CThatch_Floor::Clone(void * _pArg)
{
	CThatch_Floor*	pClone = new CThatch_Floor(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CThatch_Floor::Move_Pivot()
{
	/// 피벗 추가
	_vec3 vLook, vRight;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	m_pSphereColliderCom->Get_listCollider(CSphereCollider::COL_BUILD_PIVOT);
	
	for (auto& iter : *m_pSphereColliderCom->Get_listCollider(CSphereCollider::COL_BUILD_PIVOT))
	{
		if (CSphereCollider::COL_FWD == iter.eColDir)
		{
			iter.vSpherePos = vLook * 1.5f;
		}
		else if (CSphereCollider::COL_BWD == iter.eColDir)
		{
			iter.vSpherePos = vLook * -1.5f;
		}
		else if (CSphereCollider::COL_RIGHT == iter.eColDir)
		{
			iter.vSpherePos = vRight * 1.5f;
		}
		else if (CSphereCollider::COL_LEFT == iter.eColDir)
		{
			iter.vSpherePos = vRight * -1.5f;
		}
	}
}

const _vec3 CThatch_Floor::Get_PivotDir(_uint _eCorDir)
{
	_vec3 vLook, vRight;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	switch (_eCorDir)
	{
	case CSphereCollider::COL_FWD:
		return vLook * 1.5f;
	case CSphereCollider::COL_BWD:
		return vLook * -1.5f;
	case CSphereCollider::COL_LEFT:
		return vRight * -1.5f;
	case CSphereCollider::COL_RIGHT:
		return vRight * 1.5f;
	case CSphereCollider::COL_UP:
		return _vec3();
	case CSphereCollider::COL_DOWN:
		return _vec3();
	default:
		return _vec3();
	}
	return _vec3();
}

HRESULT CThatch_Floor::Add_Component()
{
	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	if (STRUCTURE_THATCH_FLOOR == m_tStructurInfo.eStructureType)
		pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_ThatchFloor"));
	else if (STRUCTURE_STONE_FLOOR == m_tStructurInfo.eStructureType)
		pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_StoneFloor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	/// 충돌체 컴포넌트
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	tInfo.fRadius = m_tStructurInfo.fInteractRad;
	//tInfo.vSpherePos = _vec3(0.f, 0.f, 0.f);
	tInfo.vSpherePos = m_tStructurInfo.vPos;

	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);

	return S_OK;
}
