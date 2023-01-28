#include "stdafx.h"
#include "..\Headers\Dino_Egg.h"
#include "Export_Function.h"
#include "PhysX_Mgr.h"
#include "Structure.h"
#include "Campfire.h"

CDino_Egg::CDino_Egg(LPDIRECT3DDEVICE9 _pDevice)
	: CUnit(_pDevice)
{
}

CDino_Egg::CDino_Egg(const CDino_Egg & _rhs)
	: CUnit(_rhs)
{
}

HRESULT CDino_Egg::Ready_GameObject_Prototype()
{
	CUnit::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Egg::Ready_GameObject(void * _pArg)
{
	CUnit::Ready_GameObject(_pArg);

	CComponent* pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Dino_Egg"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	Check_Arg(_pArg);


	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	tInfo.fRadius = 2.f;
	tInfo.vSpherePos = Get_Pos();
	tInfo.vSpherePos.y += 1.f;
	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);

	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(0.3f, 2.f, Get_Pos());

	Set_Scale(0.07f);

	m_vColor = { 1.f, 1.f, 1.f };

	return S_OK;
}

_int CDino_Egg::Update_GameObject(const _double & _dTimeDelta)
{
	CGameObject::Update_GameObject(_dTimeDelta);

	if (m_bDead)
	{
		m_wstrMsg = L"";

		if (!m_bDissolve)
		{
			OBJINFO tInfo;
			tInfo.vPos = Get_Pos();
			tInfo.vRot = { 0.f,0.f,0.f };
			tInfo.dwOption = 99;
			CGameObject* pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &tInfo));
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Dino_Tamed", pGameObject), E_FAIL);

			PX_RELEASE(m_pCtrl);
		}

		m_bDissolve = true;
		m_fDissolveAmount += 0.01f;

		if (1.f < m_fDissolveAmount)
			return OBJECT_DEAD;

		return NO_EVENT;
	}

	if (Engine::Get_DIKeyDown(DIK_B) && !m_bDead)
		m_bDead = true;

	list<CGameObject*>* plistStruct = Engine::Get_LayerList(L"Layer_Structure");
	if (nullptr == plistStruct)
		return NO_EVENT;
	
	_int iCampCount = 0;
	for(auto& pStructure : *plistStruct)
	{
		STRUCTURE_TYPE eType =  static_cast<CStructure*>(pStructure)->Get_StructType();
		if (eType == STRUCTURE_CAMPFIRE)
		{
			if (static_cast<CCampfire*>(pStructure)->Get_Ignition())
			{
				_vec3 vCampPos = static_cast<CCampfire*>(pStructure)->Get_Pos();
				_vec3 vPos = Get_Pos();

				_float fDis = D3DXVec3Length(&(vCampPos - vPos));

				if(fDis <= 5.f)
					++iCampCount;
			}
		}
	}

	if (iCampCount > 0)
	{
		m_fHatchingTime -= (_float)_dTimeDelta * min(iCampCount, 3);
		_int iTime = (_int)m_fHatchingTime;

		wstring wstrTime = L"0";
		if (iTime < 10)
			wstrTime += to_wstring(iTime);
		else
			wstrTime = to_wstring(iTime);

		m_wstrMsg = L"부화중 : (00:00:" + wstrTime + L")";

		if (m_fHatchingTime <= 0.f)
			m_bDead = true;
	}
	else
	{
		m_wstrMsg = L"너무 추움!";
	}

	if (m_pCtrl)
	{
		_vec3 vPos(0.f, 0.f, 0.f);
		vPos.y = (_float)(GRAVITY * _dTimeDelta);
		m_pCtrl->move(vPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());

		PxExtendedVec3 PxPos = m_pCtrl->getPosition();
		m_pTransformCom->Set_Pos(_vec3((_float)PxPos.x, (_float)PxPos.y - 2.3f, (_float)PxPos.z));
	}

	return NO_EVENT;
}

_int CDino_Egg::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	// 렌더그룹 추가 : Interaction 컬라이더가 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	if (m_pSphereColliderCom)
	{
		_vec3 vSpherePos = m_pSphereColliderCom->Get_InteractPos();

		if (Engine::Check_Tree(vSpherePos, m_pSphereColliderCom->Get_InteractRadius()))
			Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	}

	return NO_EVENT;
}

void CDino_Egg::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);

#ifdef _DEBUG
	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Render_SphereCollider();

#endif // _DEBUG
}

CDino_Egg * CDino_Egg::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Egg*	pInstance = new CDino_Egg(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Egg::Clone(void * _pArg)
{
	CDino_Egg*	pClone = new CDino_Egg(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Egg::Free()
{
	CUnit::Free();
}

void CDino_Egg::Set_ParentsName(wstring & wstrFater, wstring & wstrMother)
{
	m_wstrParents[MOTHER] = wstrMother;
	m_wstrParents[FATHER] = wstrFater;
}


HRESULT CDino_Egg::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CUnit::SetUp_ConstantTable(_pEffect);

	return S_OK;
}