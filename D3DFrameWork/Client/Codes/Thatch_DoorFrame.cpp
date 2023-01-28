#include "stdafx.h"
#include "..\Headers\Thatch_DoorFrame.h"

#include "Export_Function.h"

#include "SphereCollider.h"

USING(Engine)

CThatch_DoorFrame::CThatch_DoorFrame(LPDIRECT3DDEVICE9 _pDevice)
	: CStructure(_pDevice)
{
}

CThatch_DoorFrame::CThatch_DoorFrame(const CThatch_DoorFrame & _rhs)
	: CStructure(_rhs)
{
}

HRESULT CThatch_DoorFrame::Ready_GameObject_Prototype()
{
	CStructure::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CThatch_DoorFrame::Ready_GameObject(void * _pArg)
{
	CStructure::Ready_GameObject(_pArg);

	m_tStructurInfo.fInteractRad = 2.0f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));

	// _pArg에 Pos값을 넣은 경우(설치 완료 라는 뜻)
	if (!m_tStructurInfo.bIsPreview)
	{
		m_tStructurInfo.eStructureType = STRUCTURE_THATCH_DOORFRAME;
		m_pTransformCom->Set_Pos(m_tStructurInfo.vPos);
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_tStructurInfo.fAngleY));
		m_pTransformCom->Update_Component(0);

		/// 피벗 추가
		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);

		CSphereCollider::SPHEREINFO tInfo;
		tInfo.eColType = CSphereCollider::COL_BUILD_PIVOT;
		tInfo.fRadius = 1.f;
		
		tInfo.vSpherePos = _vec3(0.f, 1.0f, 0.f);
		tInfo.eColDir = CSphereCollider::COL_UP;
		m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);

		// 문 피벗
		tInfo.vSpherePos = _vec3(0.f, 0.f, 0.f);
		tInfo.eColDir = CSphereCollider::COL_DOWN;
		m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);

		Set_PhysXCook();

		m_vColor = { 1.f, 1.f, 1.f };

		m_bDissolve = true;
		m_fDissolveAmount = 1.f;
	}

	return S_OK;
}

_int CThatch_DoorFrame::Update_GameObject(const _double & _dTimeDelta)
{
	if (OBJECT_DEAD == CStructure::Update_GameObject(_dTimeDelta))
		return OBJECT_DEAD;
	return NO_EVENT;
}

_int CThatch_DoorFrame::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CStructure::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CThatch_DoorFrame::Render_GameObject()
{
	CStructure::Render_GameObject();
}

CThatch_DoorFrame * CThatch_DoorFrame::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CThatch_DoorFrame*	pInstance = new CThatch_DoorFrame(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CThatch_DoorFrame::Clone(void * _pArg)
{
	CThatch_DoorFrame*	pClone = new CThatch_DoorFrame(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

const _vec3 CThatch_DoorFrame::Get_PivotDir(_uint _eCorDir)
{
	//_vec3 vLook, vRight;
	//m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	//m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	//D3DXVec3Normalize(&vLook, &vLook);
	//D3DXVec3Normalize(&vRight, &vRight);

	//return _vec3(0.f, 0.f, 0.f);

	switch (_eCorDir)
	{
	case CSphereCollider::COL_FWD:
		return _vec3(0.f, 0.f, 0.f);
	case CSphereCollider::COL_BWD:
		return _vec3(0.f, 0.f, 0.f);
	case CSphereCollider::COL_LEFT:
		return _vec3(0.f, 0.f, 0.f);
	case CSphereCollider::COL_RIGHT:
		return _vec3(0.f, 0.f, 0.f);
	case CSphereCollider::COL_UP:
		return _vec3(0.f, 0.85f, 0.f);
	case CSphereCollider::COL_DOWN:
		return _vec3();
	default:
		return _vec3();
	}
	return _vec3();
}

HRESULT CThatch_DoorFrame::Add_Component()
{
	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	if (STRUCTURE_THATCH_DOORFRAME == m_tStructurInfo.eStructureType)
	{
		pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_ThatchDoorFrame"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

		pComponent = m_pMeshLodCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_ThatchDoorFrame_Lod"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshLod", pComponent);
	}
	else if (STRUCTURE_STONE_DOORFRAME == m_tStructurInfo.eStructureType)
	{
		pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_StoneDoorFrame"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

		//pComponent = m_pMeshLodCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_ThatchDoorFrame_Lod"));
		//NULL_CHECK_RETURN(pComponent, E_FAIL);
		//m_mapComponent[ID_STATIC].emplace(L"Com_MeshLod", pComponent);
	}

	/// 충돌체 컴포넌트
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	tInfo.fRadius = m_tStructurInfo.fInteractRad;
	tInfo.vSpherePos = m_tStructurInfo.vPos;

	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);

	return S_OK;
}
