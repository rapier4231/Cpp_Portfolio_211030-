#include "stdafx.h"
#include "..\Headers\Smithy.h"

#include "Export_Function.h"

#include "Inven.h"

USING(Engine)

CSmithy::CSmithy(LPDIRECT3DDEVICE9 _pDevice)
	: CStructure(_pDevice)
{
}

CSmithy::CSmithy(const CSmithy & _rhs)
	: CStructure(_rhs)
{
}

HRESULT CSmithy::Ready_GameObject_Prototype()
{
	CStructure::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CSmithy::Ready_GameObject(void * _pArg)
{
	CStructure::Ready_GameObject(_pArg);

	m_tStructurInfo.fInteractRad = 1.5f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	InvenUserUnitTypeID eInvenID = InvenUserUnitTypeID_Smithy;
	m_pInvenCom->Set_InvenUserUnitTypeID(eInvenID);

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));

	// _pArg에 Pos값을 넣은 경우(설치 완료 라는 뜻)
	if (!m_tStructurInfo.bIsPreview)
	{
		//m_tStructurInfo.eStructureType = STRUCTURE_SMITHY;
		m_pTransformCom->Set_Pos(m_tStructurInfo.vPos);
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_tStructurInfo.fAngleY/* - 90.f*/));
		m_pTransformCom->Update_Component(0);

		Set_PhysXCook();

		m_vColor = { 1.f, 1.f, 1.f };

		m_bDissolve = true;
		m_fDissolveAmount = 1.f;
	}

	return S_OK;
}

_int CSmithy::Update_GameObject(const _double & _dTimeDelta)
{
	if (OBJECT_DEAD == CStructure::Update_GameObject(_dTimeDelta))
		return OBJECT_DEAD;

	return NO_EVENT;
}

_int CSmithy::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CStructure::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CSmithy::Render_GameObject()
{
	CStructure::Render_GameObject();
}

CSmithy * CSmithy::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CSmithy*	pInstance = new CSmithy(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CSmithy::Clone(void * _pArg)
{
	CSmithy*	pClone = new CSmithy(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

HRESULT CSmithy::Add_Component()
{
	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_Smithy"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

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
