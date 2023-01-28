#include "stdafx.h"
#include "..\Headers\CookingCampfire.h"

#include "Export_Function.h"

#include "CollisionFunc.h"
#include "Inven.h"
#include "Right_Inventory.h"
#include "Fire.h"
#include "FireTest.h"
#include "FireSmoke.h"

USING(Engine)


CCookingCampfire::CCookingCampfire(LPDIRECT3DDEVICE9 _pDevice)
	: CStructure(_pDevice)
{
}

CCookingCampfire::CCookingCampfire(const CCookingCampfire & _rhs)
	: CStructure(_rhs)
{
}

HRESULT CCookingCampfire::Ready_GameObject_Prototype()
{
	CStructure::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CCookingCampfire::Ready_GameObject(void * _pArg)
{
	CStructure::Ready_GameObject(_pArg);

	m_tStructurInfo.fInteractRad = 1.5f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	InvenUserUnitTypeID eInvenID = InvenUserUnitTypeID_Campfire;
	m_pInvenCom->Set_InvenUserUnitTypeID(eInvenID);

	m_pTransformCom->Set_Scale(_vec3(0.015f, 0.015f, 0.015f));

	// _pArg에 StructureInfo값을 넣은 경우(설치 완료 라는 뜻)
	if (!m_tStructurInfo.bIsPreview)
	{
		//m_tStructurInfo.eStructureType = STRUCTURE_CAMPFIRE;
		m_pTransformCom->Set_Pos(m_tStructurInfo.vPos);
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_tStructurInfo.fAngleY));
		m_pTransformCom->Update_Component(0);

		m_vColor = { 1.f, 1.f, 1.f };

		m_bDissolve = true;
		m_fDissolveAmount = 1.f;

		// 모닥불 불 이펙트
		pFire = static_cast<CFireTest*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_FireTest"));
		NULL_CHECK_RETURN(pFire, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pFire), E_FAIL);

		// 모닥불 연기 이펙트
		pFireSmoke = static_cast<CFireSmoke*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_FireSmoke"));
		NULL_CHECK_RETURN(pFireSmoke, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pFireSmoke), E_FAIL);

		// 점조명
		D3DLIGHT9 tLightInfo;
		ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

		tLightInfo.Type = D3DLIGHT_POINT;
		tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.2f, 0.f, 1.f);
		tLightInfo.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		tLightInfo.Position = m_tStructurInfo.vPos + _vec3(0.f, 0.3f, 0.f);
		tLightInfo.Range = 0.f;

		m_iLightIndex = Engine::Get_LightListSize();

		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pDevice, &tLightInfo, m_iLightIndex), S_OK);

		m_fLightRangeMin = 4.f;
		m_fLightRangeMax = 6.f;
	}

	return S_OK;
}

_int CCookingCampfire::Update_GameObject(const _double & _dTimeDelta)
{
	if (OBJECT_DEAD == CStructure::Update_GameObject(_dTimeDelta))
		return OBJECT_DEAD;
	
	/// 테스트용 무한 점화
	//if (!m_bIsIgnition)
	//{
	//	Set_Ignition(true);
	//}

	if (pFire)
	{
		_vec3	vPlayerPos = *static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos();
		_vec3	vDir = vPlayerPos - Get_Pos();
		D3DXVec3Normalize(&vDir, &vDir);
		vDir *= 0.1f;
		vDir.y = 0.f;

		static_cast<CTransform*>(pFire->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(Get_Pos() + vDir + _vec3(0.f, 0.4f, 0.f));
		static_cast<CTransform*>(pFireSmoke->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(Get_Pos() + vDir + _vec3(0.f, 0.4f, 0.f));

		// 모닥불 불 이펙트
		if (m_bIsIgnition)
		{
			pFire->Set_Ignition(true);
			pFireSmoke->Set_Ignition(true);
			Engine::Get_Light(m_iLightIndex)->Set_LightStat(LIGHTRANGEUP, m_fLightRangeMax);
		}

		else
		{
			pFire->Set_Ignition(false);
			pFireSmoke->Set_Ignition(false);
			Engine::Get_Light(m_iLightIndex)->Set_LightStat(LIGHTRANGEDOWN, 0.1f);
		}
	}

	Check_Roast(_dTimeDelta);

	//if(GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
	//	Engine::Get_Light(m_iLightIndex)->Set_LightStat(LIGHTPOWERDOWN, 0.f);
	//if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	//	Engine::Get_Light(m_iLightIndex)->Set_LightStat(LIGHTPOWERUP, 1.f);

	return NO_EVENT;
}

_int CCookingCampfire::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CStructure::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CCookingCampfire::Render_GameObject()
{
	CStructure::Render_GameObject();
}

CCookingCampfire * CCookingCampfire::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CCookingCampfire*	pInstance = new CCookingCampfire(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CCookingCampfire::Clone(void * _pArg/* = nullptr*/)
{
	CCookingCampfire*	pClone = new CCookingCampfire(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

_bool CCookingCampfire::Set_Ignition(_bool _bIsIgnition)
{
	if (_bIsIgnition)
	{
		if (!m_bIsIgnition)
		{
			/// 점화
			if (0 != m_pInvenCom->Now_Have_ItemAmount(ITEMID_Thatch))
			{
				// 짚이 있을 경우
				m_pInvenCom->Remove_Item(ITEMID_Thatch, 1);
				m_dIgnitionTime += 5.f;
				m_bIsIgnition = true;

				return true;
			}
			else if (0 != m_pInvenCom->Now_Have_ItemAmount(ITEMID_Wood))
			{
				// 나무가 있을 경우
				m_pInvenCom->Remove_Item(ITEMID_Wood, 1);
				m_dIgnitionTime += 30.f;
				m_bIsIgnition = true;

				return true;
			}
		}
	}
	else
	{
		m_bIsIgnition = false;
		m_dIgnitionTime = 0.0;
	}

	return false;
}

HRESULT CCookingCampfire::Add_Component()
{
	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_CookingCampFire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMeshLodCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_CookingCampFire_Lod"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_MeshLod", pComponent);

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

void CCookingCampfire::Check_Roast(const _double & _dTimeDelta)
{
	if (m_bIsIgnition)
	{
		m_dIgnitionTime -= _dTimeDelta;

		/// 고기가 하나 이상 있을 경우 고기를 굽는다
		if (0 < m_pInvenCom->Now_Have_ItemAmount(ITEMID_Meat))
		{
			m_dRoastTime += _dTimeDelta;
		
			// 고기가 5초 이상됐을 경우 고기를 굽는다.
			if (5 < m_dRoastTime)
			{
				m_dRoastTime = 0.0;

				m_pInvenCom->Remove_Item(ITEMID_Meat, 1);
				m_pInvenCom->Add_Item(ITEMID_CookedMeat, 1);

				static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Match_ItemTex_Info();
			}
		}

		/// 점화 시간을 다 사용했을 경우
		if (0 > m_dIgnitionTime)
		{
			if (0 != m_pInvenCom->Now_Have_ItemAmount(ITEMID_Thatch))
			{
				// 짚이 있을 경우
				m_pInvenCom->Remove_Item(ITEMID_Thatch, 1);
				m_dIgnitionTime += 5.f;

				static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Match_ItemTex_Info();
			}
			else if (0 != m_pInvenCom->Now_Have_ItemAmount(ITEMID_Wood))
			{
				// 나무가 있을 경우
				m_pInvenCom->Remove_Item(ITEMID_Wood, 1);
				m_dIgnitionTime += 30.f;

				static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Match_ItemTex_Info();
			}
			else
			{
				m_bIsIgnition = false;
				m_dIgnitionTime = 0.0;
			}
		}
	}
}
