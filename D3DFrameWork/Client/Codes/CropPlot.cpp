#include "stdafx.h"
#include "..\Headers\CropPlot.h"

#include "Export_Function.h"

#include "Inven.h"

USING(Engine)

CCropPlot::CCropPlot(LPDIRECT3DDEVICE9 _pDevice)
	: CStructure(_pDevice)
{
}

CCropPlot::CCropPlot(const CCropPlot & _rhs)
	: CStructure(_rhs)
{
}

HRESULT CCropPlot::Ready_GameObject_Prototype()
{
	CStructure::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CCropPlot::Ready_GameObject(void * _pArg)
{
	CStructure::Ready_GameObject(_pArg);

	m_tStructurInfo.fInteractRad = 1.5f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	InvenUserUnitTypeID eInvenID = InvenUserUnitTypeID_Brazier;
	m_pInvenCom->Set_InvenUserUnitTypeID(eInvenID);

	m_pTransformCom->Set_Scale(_vec3(0.02f, 0.02f, 0.02f));

	// _pArg�� Pos���� ���� ���(��ġ �Ϸ� ��� ��)
	if (!m_tStructurInfo.bIsPreview)
	{
		//m_tStructurInfo.eStructureType = STRUCTURE_FORGE;
		m_pTransformCom->Set_Pos(m_tStructurInfo.vPos);
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_tStructurInfo.fAngleY));
		m_pTransformCom->Update_Component(0);

		Set_PhysXCook();

		m_vColor = { 1.f, 1.f, 1.f };

		m_bDissolve = true;
		m_fDissolveAmount = 1.f;
	}

	return S_OK;
}

_int CCropPlot::Update_GameObject(const _double & _dTimeDelta)
{
	if (OBJECT_DEAD == CStructure::Update_GameObject(_dTimeDelta))
		return OBJECT_DEAD;

	return NO_EVENT;
}

_int CCropPlot::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CStructure::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CCropPlot::Render_GameObject()
{
	CStructure::Render_GameObject();
}

CCropPlot * CCropPlot::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CCropPlot*	pInstance = new CCropPlot(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CCropPlot::Clone(void * _pArg)
{
	CCropPlot*	pClone = new CCropPlot(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

HRESULT CCropPlot::Add_Component()
{
	CComponent*		pComponent = nullptr;

	/// �Ž� ������Ʈ
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_CropPlot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMeshLodCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_CropPlot_Lod"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_MeshLod", pComponent);

	/// �浹ü ������Ʈ
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	tInfo.fRadius = m_tStructurInfo.fInteractRad;
	tInfo.vSpherePos = m_tStructurInfo.vPos;

	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);


	return S_OK;
}
