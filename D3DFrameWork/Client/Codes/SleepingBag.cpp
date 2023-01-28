#include "stdafx.h"
#include "..\Headers\SleepingBag.h"

#include "Export_Function.h"

#include "CollisionFunc.h"
#include "Inven.h"
#include "Right_Inventory.h"
#include "Fire.h"

#include "MiniMap_UI.h"

USING(Engine)


CSleepingBag::CSleepingBag(LPDIRECT3DDEVICE9 _pDevice)
	: CStructure(_pDevice)
{
}

CSleepingBag::CSleepingBag(const CSleepingBag & _rhs)
	: CStructure(_rhs)
{
}

HRESULT CSleepingBag::Ready_GameObject_Prototype()
{
	CStructure::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CSleepingBag::Ready_GameObject(void * _pArg)
{
	CStructure::Ready_GameObject(_pArg);

	m_tStructurInfo.fInteractRad = 1.5f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.005f, 0.005f, 0.005f));

	// _pArg에 Pos값을 넣은 경우(설치 완료 라는 뜻)
	if (!m_tStructurInfo.bIsPreview)
	{
		//m_tStructurInfo.eStructureType = STRUCTURE_SLEEPINGBAG;
		m_pTransformCom->Set_Pos(m_tStructurInfo.vPos);
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_tStructurInfo.fAngleY));
		m_pTransformCom->Update_Component(0);

		m_vColor = { 1.f, 1.f, 1.f };

		m_bDissolve = true;
		m_fDissolveAmount = 1.f;
	}

	return S_OK;
}

_int CSleepingBag::Update_GameObject(const _double & _dTimeDelta)
{
	if (OBJECT_DEAD == CStructure::Update_GameObject(_dTimeDelta))
		return OBJECT_DEAD;

	return NO_EVENT;
}

_int CSleepingBag::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CStructure::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CSleepingBag::Render_GameObject()
{
	CStructure::Render_GameObject();
}

CSleepingBag * CSleepingBag::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CSleepingBag*	pInstance = new CSleepingBag(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CSleepingBag::Clone(void * _pArg/* = nullptr*/)
{
	CSleepingBag*	pClone = new CSleepingBag(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CSleepingBag::TurnOn_Structure()
{
	//침대 생성시
	m_iBedID = static_cast<CMiniMap_UI*>(Engine::Get_GameObject(L"MiniMap_UI", 0))->Bed_Create(Get_Pos());
}

void CSleepingBag::TurnOff_Structure()
{
	Set_Dead();
}

HRESULT CSleepingBag::Add_Component()
{
	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_SleepingBag"));
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
