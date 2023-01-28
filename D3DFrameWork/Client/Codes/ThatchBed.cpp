#include "stdafx.h"
#include "..\Headers\ThatchBed.h"

#include "Export_Function.h"

#include "CollisionFunc.h"
#include "Inven.h"
#include "Right_Inventory.h"
#include "Fire.h"

#include "MiniMap_UI.h"

USING(Engine)


CThatchBed::CThatchBed(LPDIRECT3DDEVICE9 _pDevice)
	: CStructure(_pDevice)
{
}

CThatchBed::CThatchBed(const CThatchBed & _rhs)
	: CStructure(_rhs)
{
}

HRESULT CThatchBed::Ready_GameObject_Prototype()
{
	CStructure::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CThatchBed::Ready_GameObject(void * _pArg)
{
	CStructure::Ready_GameObject(_pArg);

	m_tStructurInfo.fInteractRad = 1.5f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.007f, 0.007f, 0.007f));

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

_int CThatchBed::Update_GameObject(const _double & _dTimeDelta)
{
	if (OBJECT_DEAD == CStructure::Update_GameObject(_dTimeDelta))
		return OBJECT_DEAD;

	return NO_EVENT;
}

_int CThatchBed::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CStructure::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CThatchBed::Render_GameObject()
{
	CStructure::Render_GameObject();
}

CThatchBed * CThatchBed::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CThatchBed*	pInstance = new CThatchBed(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CThatchBed::Clone(void * _pArg/* = nullptr*/)
{
	CThatchBed*	pClone = new CThatchBed(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CThatchBed::TurnOn_Structure()
{
	//침대 생성시
	m_iBedID = static_cast<CMiniMap_UI*>(Engine::Get_GameObject(L"MiniMap_UI", 0))->Bed_Create(Get_Pos());
}

void CThatchBed::TurnOff_Structure()
{
	//Set_Dead();
}

HRESULT CThatchBed::Add_Component()
{
	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_ThatchBed"));
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
