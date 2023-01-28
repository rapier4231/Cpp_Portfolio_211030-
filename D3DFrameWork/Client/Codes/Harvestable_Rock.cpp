#include "stdafx.h"
#include "Harvestable_Rock.h"
#include "Export_Function.h"
#include "Inven.h"
#include "LR_Inventory.h"
#include "PhysX_Mgr.h"
#include "RandomFunc.h"
#include "QuickSlot_UI.h"
#include "Dino.h"
#include "GuideFont_UI.h"

USING(Engine)

CHarvestable_Rock::CHarvestable_Rock(LPDIRECT3DDEVICE9 _pDevice)
	: CHarvestable(_pDevice)
{
}

CHarvestable_Rock::CHarvestable_Rock(const CHarvestable_Rock & rhs)
	: CHarvestable(rhs)
{
}

HRESULT CHarvestable_Rock::Ready_GameObject_Prototype()
{
	CHarvestable::Ready_GameObject_Prototype();

	//m_iID = UNITID_ROCK;

	m_tStat.fHp = 100.f;	

	return S_OK;
}

HRESULT CHarvestable_Rock::Ready_GameObject(void * _pArg)
{
	CHarvestable::Ready_GameObject(_pArg);
	_ulong dwOption = Check_Arg(_pArg);		//매개변수를 해석해 적용하고 옵션값에 따른 메시 획득
	Add_Component(dwOption);	//충돌체 획득

	//경계상자(로컬) 작성
	D3DXComputeBoundingSphere(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), sizeof(_vec3), &m_vBoundingCenter, &m_fBoundingRadius);
	//스케일 곱해서 월드로 변환
	m_pTransformCom->Update_Component(0);
	D3DXVec3TransformCoord(&m_vBoundingCenter, &m_vBoundingCenter, &m_pTransformCom->Get_WorldMatrix());
	m_fBoundingRadius *= Get_Scale().x;

	_vec3 vPos = *m_pTransformCom->Get_Pos();
	_vec3 vScale = *m_pTransformCom->Get_Scale();
	_mat matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, Get_RotY(), Get_RotX(), Get_RotZ());	//각 축 회전값으로 3x3매트릭스를 생성
	PxMat33 mat33(PxVec3(matRot._11, matRot._12, matRot._13)			//이때 요피치롤이 yxz 순서임에 유의
		, PxVec3(matRot._21, matRot._22, matRot._23)
		, PxVec3(matRot._31, matRot._32, matRot._33));
	PxTransform px = PxTransform(vPos.x, vPos.y, vPos.z, PxQuat(mat33));

	////////////////////////////////////////////////////////////////////////////
	list<D3DXMESHCONTAINER_DERIVED*>* pMeshList = m_pDynamicMeshCom->Get_MeshList();

	PxConvexMeshDesc convexDesc;
	convexDesc.points.count		= pMeshList->front()->dwVtxNum;
	convexDesc.points.stride	= sizeof(PxVec3);
	convexDesc.points.data		= pMeshList->front()->pVtxPos;
	convexDesc.flags			= PxConvexFlag::eCOMPUTE_CONVEX;
	
	PxDefaultMemoryOutputStream buf;
	PxConvexMeshCookingResult::Enum result;
	if (!CPhysX_Mgr::GetInstance()->Get_Cooking()->cookConvexMesh(convexDesc, buf, &result))
		return NULL;
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* convexMesh = CPhysX_Mgr::GetInstance()->Get_Physics()->createConvexMesh(input);
	
	PxConvexMeshGeometry Convex;
	Convex.convexMesh = convexMesh;
	// 임시로 테스트
	Convex.scale = PxMeshScale(PxVec3(vScale.x, vScale.y, vScale.z));
	
	PxMaterial* paxMat = CPhysX_Mgr::GetInstance()->Get_Physics()->createMaterial(0.f, 0.f, 0.f);

	m_pBaseActor = CPhysX_Mgr::GetInstance()->Get_Physics()->createRigidStatic(px);
	PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*m_pBaseActor, Convex, *paxMat);

	CPhysX_Mgr::GetInstance()->Get_PxScene()->addActor(*m_pBaseActor);
	m_pBaseActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	m_pBaseActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);

	//////////////////////////////////////////////////////////////////////////

	D3DXFRAME* pRootFrame = m_pDynamicMeshCom->Get_RootFrame();

	for (auto& data : *pMeshList)
	{
		for (_ulong i = 0; i < data->dwNumBones; ++i)
		{
			const char* pBoneName = data->pSkinInfo->GetBoneName(i);	

			D3DXFRAME_DERIVED*	 pDerivedFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(pRootFrame, pBoneName); 

			_mat BoneMatrix = pDerivedFrame->CombinedTransformMatrix;
			PxRigidDynamic* pDynamic =  CPhysX_Mgr::GetInstance()->Get_Physics()->createRigidDynamic(px/*PxTransform(px.p.x, px.p.y, px.p.z)*/);
			PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*pDynamic, PxBoxGeometry(0.2f, 0.2f, 0.2f), *paxMat);

			pDynamic->setMass(500.f);
			pDynamic->setAngularDamping(500.f);
			pDynamic->setMaxLinearVelocity(3.f);
			PxRigidBodyExt::updateMassAndInertia(*pDynamic, 300.f);

			CPhysX_Mgr::GetInstance()->Get_PxScene()->addActor(*pDynamic);
			
			// 물리 적용 안함.
			pDynamic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);

			m_tupleBones.emplace_back(make_tuple(pDynamic, BoneMatrix));
		}
	}

	//////////////////////////////////////////////////////////////////////////

	return S_OK;
}

_int CHarvestable_Rock::Update_GameObject(const _double & _dTimeDelta)
{
	CHarvestable::Update_GameObject(_dTimeDelta);

	if (m_tStat.fHp <= 0)
		m_bBreak = true;
	
	if (m_bBreak)
	{
		if (m_pBaseActor)
		{
			m_pBaseActor->release();
			m_pBaseActor = nullptr;
		}

		m_fDeleteTime += (_float)_dTimeDelta;

		if (m_fDeleteTime >= 1.5f)
		{
			// 죽으면 디졸브 효과 발생, 
			m_fDissolveAmount += 0.02f;
			m_bDissolve = true;

			if (1.f < m_fDissolveAmount)
				m_fDissolveAmount = 1.f;
		}

		if (m_fDeleteTime >= 3.f)
		{
			for (auto& data : m_tupleBones)
				get<0>(data)->release();

			return OBJECT_DEAD;
		}
	}
	else
	{
		PxTransform pxPos = m_pBaseActor->getGlobalPose();
		m_pTransformCom->Set_Pos(pxPos.p.x, pxPos.p.y, pxPos.p.z);
	}

	return NO_EVENT;
}

_int CHarvestable_Rock::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	//그림자 렌더그룹 추가 : 경계구체가 빛 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	//if (Engine::Check_Sphere(m_vBoundingCenter, m_fBoundingRadius, CULL_LIGHT))
	//{
	//	g_bInstancing && !m_bBreak ? 
	//		Engine::Add_Instance(m_wstrMeshName, RENDER_SHADOWMAP, m_pTransformCom->Get_WorldMatrix()) : 
	//		Engine::Add_RenderGroup(RENDER_SHADOWMAP, this);
	//}

	//렌더그룹 추가 : 경계구체가 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	if (Engine::Check_Sphere(m_vBoundingCenter, m_fBoundingRadius))
	{
		g_bInstancing && !m_bBreak ?
			Engine::Add_Instance(m_wstrMeshName, RENDER_NONALPHA, m_pTransformCom->Get_WorldMatrix()) :
			Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	}

	return NO_EVENT;
}

void CHarvestable_Rock::Render_GameObject()
{
	if (m_bBreak)
	{
		_bool bFirst = false;
		_int i = 0;
		for (auto& data : m_tupleBones)
		{
			// 물리 적용
			get<0>(data)->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
			if (m_bPowerBreak)
			{
				get<0>(data)->setMaxLinearVelocity(30.f);
				get<0>(data)->setMass(50.f);
				get<0>(data)->setAngularDamping(50.f);
			}

			if (!bFirst)
			{
				bFirst = true;
				get<0>(data)->addForce(PxVec3(0.f, m_bPowerBreak ? 1.f : -1.f, 0.f), PxForceMode::eFORCE);
			}

			PxTransform PxPos = get<0>(data)->getGlobalPose();

			_vec3 vPos = { PxPos.p.x,PxPos.p.y,PxPos.p.z };
			_vec3 vRot = { PxPos.q.x,PxPos.q.y,PxPos.q.z };

			// 월드를 로컬로?
			_mat		matWorld;
			matWorld = m_pTransformCom->Get_WorldMatrix();
			D3DXMatrixInverse(&matWorld, NULL, &matWorld);
			D3DXVec3TransformCoord(&vPos, &vPos, &matWorld);

			D3DXMatrixIdentity(&get<1>(data));
			memcpy(&get<1>(data).m[3][0], &vPos, sizeof(_vec3));

			// 회전
			_mat	matRot[ROT_END];
			_vec3	m_vinfo[INFO_END];
			ZeroMemory(&m_vinfo, sizeof(_vec3) * INFO_END);

			for (_int i = 0; i < INFO_END; ++i)
				memcpy(m_vinfo[i], &get<1>(data).m[i][0], sizeof(_vec3));

			D3DXMatrixRotationX(&matRot[ROT_X], vRot.x);
			D3DXMatrixRotationY(&matRot[ROT_Y], vRot.y);
			D3DXMatrixRotationZ(&matRot[ROT_Z], vRot.z);

			for (_uint i = 0; i < INFO_POS; ++i)
				for (_uint j = 0; j < ROT_END; ++j)
				{
					D3DXVec3TransformNormal(&m_vinfo[i], &m_vinfo[i], &matRot[j]);
				}

			for (_uint i = 0; i < INFO_END; ++i)
				memcpy(&get<1>(data).m[i][0], &m_vinfo[i], sizeof(_vec3));

			m_pDynamicMeshCom->Play_CustomAnimation(i, &get<1>(data));
			++i;
		}
	}

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	if (m_bBreak)
		m_pDynamicMeshCom->Render_Meshes(pEffect, m_bDissolve);
	else
		m_pMeshCom->Render_Meshes(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);

#ifdef _DEBUG
	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Render_SphereCollider();

#endif // !_DEBUG
	//////////////////////////////////////////////////////////////////////////
}

void CHarvestable_Rock::Render_GameObjectShadowMap()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowMapCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);

	pEffect->Begin(NULL, 0);

	// ShadowMap Pass 0 기본, 1 디졸브, 2 알파테스트
	if (m_bDissolve)
		pEffect->BeginPass(1);
	else
		pEffect->BeginPass(0);

	if (m_bBreak)
		m_pDynamicMeshCom->Render_MeshesShadowMap(pEffect);
	else
		m_pMeshCom->Render_MeshesShadowMap(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);
}

void CHarvestable_Rock::Render_GameObjectShadow()
{
	if (m_bBreak)
		return;

	LPD3DXEFFECT pEffect = m_pShaderShadowCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	m_pDevice->GetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);

	m_matWVP = m_pTransformCom->Get_WorldMatrix() * m_matView * m_matProj;
	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	m_matWLVB = m_matWLV * m_matScaleBias;

	pEffect->SetMatrix("g_matWorldViewProj", &m_matWVP);
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);
	pEffect->SetMatrix("g_matWorldLightViewProjBias", &m_matWLVB);

	D3DXMatrixInverse(&m_matWLVB, NULL, &m_pTransformCom->Get_WorldMatrix());
	D3DXVec3Transform(&m_vDir, &m_vecLightPos, &m_matWLVB);
	D3DXVec4Normalize(&m_vDir, &m_vDir);
	m_vDir.w = 0;

	pEffect->SetVector("g_vLightDir", &m_vDir);

	Engine::Get_RenderTargetTexture(pEffect, L"ShadowMap", "g_ShadowMapTexture");

	pEffect->Begin(NULL, 0);

	// Shadow Pass 0 기본, 1 디졸브, 2 알파테스트
	if (m_bDissolve)
		pEffect->BeginPass(1);
	else
		pEffect->BeginPass(0);

	if (m_bBreak)
		m_pDynamicMeshCom->Render_MeshesShadow(pEffect);
	else
		m_pMeshCom->Render_MeshesShadow(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);
}

void CHarvestable_Rock::Check_Dying(const _double & _dTimeDelta)
{
	// 죽었을 때
}

CHarvestable_Rock * CHarvestable_Rock::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CHarvestable_Rock*	pInstance = new CHarvestable_Rock(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CHarvestable_Rock::Clone(void * _pArg)
{
	CHarvestable_Rock*	pClone = new CHarvestable_Rock(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CHarvestable_Rock::Free()
{
	CHarvestable::Free();
}

_int CHarvestable_Rock::Check_Arg(void * _pArg)
{

	if (_pArg)
	{
		if (dynamic_cast<OBJINFO*>((OBJINFO*)_pArg))
		{
			OBJINFO tInfo;
			memcpy(&tInfo, _pArg, sizeof(OBJINFO));

			//dwOption으로 받은 MeshID값에따라 넣을메쉬 판단
			TCHAR szText[MAX_PATH];
			Engine::Get_MeshTag_Rock(szText, tInfo.dwOption);

			if (!szText || E_FAIL == Init_Mesh(szText))
			{
				ERR_MSG(L"CHarvestable_Rock : Failed to Find MeshTag");
				return -1;
			}

			_float fShaker = 0.1f;

			Set_Scale(CRandomFunc::Shake_Float(tInfo.vScale.x, fShaker));
			Set_Rot(CRandomFunc::Shake_Float(tInfo.vRot.x, fShaker)
				, CRandomFunc::Shake_Float(tInfo.vRot.y, fShaker)
				, CRandomFunc::Shake_Float(tInfo.vRot.z, fShaker));
			Set_Pos(tInfo.vPos.x, tInfo.vPos.y, tInfo.vPos.z);
			m_vColor = tInfo.vColor;

			wstring strTagDM = szText + wstring(L"_DM");
			if (E_FAIL == Init_DynamicMesh(strTagDM, tInfo.vScale, tInfo.vRot, tInfo.vPos))
			{
				ERR_MSG(L"CHarvestable_Rock : Failed to Find MeshTag_DM");
				return -1;
			}

			//경험치 더해줘야함
			Add_ExpToLevel(tInfo.dwLevel);

			return tInfo.dwOption;
		}
		else
		{
			ERR_MSG(L"Wrong Format Argument");
			return 0;
		}
	}
	else
		//Argument 없으면 기본메시 로드
	{
		Init_Mesh(L"Proto_StaticMesh_Rock_Small_A");
	}
	return 0;
}

_bool CHarvestable_Rock::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	if (WEAPON_HATCHET != dwAttackType && WEAPON_STONEPICK != dwAttackType  && WEAPON_DINO != dwAttackType)
		return false;

	_int iStoneAmount, iFlintAmount;
	_int iAmount = (_int)CRandomFunc::Shake_Float(pAttacker->Get_Stat(STAT_ATTACK), 0.2f);		// 맨손 4~6			// 도끼,곡괭이 16~24
																								//if (0 > iAmount)
	if (0 > iAmount)
		iAmount = 1;

	if (WEAPON_HATCHET == dwAttackType)
	{
		// 도끼로 타격시
		iStoneAmount = _int(iAmount * 0.5f);
		iFlintAmount = _int(iAmount * 0.2f);
	}
	else if (WEAPON_STONEPICK == dwAttackType)
	{
		// 곡괭이로 타격시
		iStoneAmount = _int(iAmount * 0.2f);
		iFlintAmount = _int(iAmount * 0.5f);
	}
	else if (WEAPON_DINO == dwAttackType)
	{
		// 전부 다 캠
		iStoneAmount = _int(iAmount * 2.f);
		iFlintAmount = _int(iAmount * 2.f);
		m_tStat.fHp = 0.f;

		m_bPowerBreak = true;
	}

	_bool bHit = CHarvestable::HitBy(pAttacker, dwAttackType, vCollisionPos);

	if (!bHit)
		return bHit;

	if (bHit)
	{
		vCollisionPos;//<<피격이펙트출력>> 바위 : 바위파편이나 먼지
	}

	if (WEAPON_DINO == dwAttackType)
	{
		if (0 < iStoneAmount)
		{
			static_cast<CDino*>(pAttacker)->Get_Inven()->Add_Item(ITEMID_Stone, iStoneAmount);
			static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_LeftGuideFont(ITEMID_Stone, (_int)iStoneAmount, 0);
			static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(_float(iStoneAmount));
		}
		if (0 < iFlintAmount)
		{
			static_cast<CDino*>(pAttacker)->Get_Inven()->Add_Item(ITEMID_Flint, iStoneAmount);
			static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_LeftGuideFont(ITEMID_Flint, (_int)iFlintAmount, 0);
			static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(_float(iFlintAmount));
		}
	}
	else
	{
		if (0 < iStoneAmount)
		{
			static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_Stone, iStoneAmount);
			static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(_float(iStoneAmount));
		}
		if (0 < iFlintAmount)
		{
			static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_Flint, iFlintAmount);
			static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(_float(iFlintAmount));
		}
	}

	CGameObject* pGameObject = nullptr;

	m_fDustScale = 0.001f;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Dust", &m_fDustScale);
	NULL_CHECK_RETURN(pGameObject, false);
	FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), false);

	//_vec3 vDir = *static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos() - *m_pTransformCom->Get_Pos();
	//D3DXVec3Normalize(&vDir, &vDir);
	//vDir.y = 0.f;
	//vDir *= 0.75f;

	static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vCollisionPos /*+ _vec3(0.f, 1.f, 0.f) + vDir*/);

	return bHit;
}

HRESULT CHarvestable_Rock::Add_Component(_ulong dwOption)
{
	CComponent*		pComponent = nullptr;

	////옵션에 따라 다른 메시 부여 등
	//pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Rock_Small_A"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//pComponent = m_pDynamicMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Rock_Small_A"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_TestMesh", pComponent);

	//////////////////////////////////////////////////////////////////////////
	// 충돌테스트
	//////////////////////////////////////////////////////////////////////////
	/// 충돌체

	//옵션값에 따라 크기배율을 달리 적용함.
	//	0 ~ 1	Rock_Small		x1
	//	2 ~ 3	Rock_Medium		x2
	//	4 ~ 6	Rock_Large		x4
	_float fAdj = 1.f;
	if (3 < dwOption)
		fAdj = 4.8f;
	else if (1 < dwOption)
		fAdj = 2.4f;

	_float fSize =  Get_Scale().x * fAdj;
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_HIT;
	tInfo.fRadius = 80.f * fSize;
	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);

	tInfo.fRadius = 80.f * fSize;
	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);

	//충돌체 월드위치 세팅
	m_pSphereColliderCom->Set_Pos(Get_Pos());

	return S_OK;
}
