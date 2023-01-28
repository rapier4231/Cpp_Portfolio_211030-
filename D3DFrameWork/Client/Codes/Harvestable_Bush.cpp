#include "stdafx.h"
#include "Harvestable_Bush.h"

#include "Export_Function.h"
#include "Inven.h"
#include "LR_Inventory.h"
#include "PhysX_Mgr.h"

#include "RandomFunc.h"

#include "QuickSlot_UI.h"
#include "Dino.h"
#include "GuideFont_UI.h"

USING(Engine)

CHarvestable_Bush::CHarvestable_Bush(LPDIRECT3DDEVICE9 _pDevice)
	: CHarvestable(_pDevice)
{
}

CHarvestable_Bush::CHarvestable_Bush(const CHarvestable_Bush & rhs)
	: CHarvestable(rhs)
{
}

HRESULT CHarvestable_Bush::Ready_GameObject_Prototype()
{
	CHarvestable::Ready_GameObject_Prototype();

	//m_iID = UNITID_BUSH;

	m_tStat.fHp = 15.f;

	return S_OK;
}

HRESULT CHarvestable_Bush::Ready_GameObject(void * _pArg)
{
	CHarvestable::Ready_GameObject(_pArg);
	_ulong dwOption = Check_Arg(_pArg);
	Add_Component(dwOption);

	//메시를 받은 후 경계상자(로컬) 작성
	D3DXComputeBoundingSphere(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), sizeof(_vec3), &m_vBoundingCenter, &m_fBoundingRadius);
	//스케일 곱해서 월드로 변환
	m_pTransformCom->Update_Component(0);
	D3DXVec3TransformCoord(&m_vBoundingCenter, &m_vBoundingCenter, &m_pTransformCom->Get_WorldMatrix());
	m_fBoundingRadius *= Get_Scale().x;

	_vec3 vPos = *m_pTransformCom->Get_Pos();
	_vec3 vScale = *m_pTransformCom->Get_Scale();
	PxTransform px = PxTransform(vPos.x, vPos.y, vPos.z);

	////////////////////////////////////////////////////////////////////////////
	list<D3DXMESHCONTAINER_DERIVED*>* pMeshList = m_pDynamicMeshCom->Get_MeshList();

	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = pMeshList->front()->dwVtxNum;
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = pMeshList->front()->pVtxPos;
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	PxDefaultMemoryOutputStream buf;
	PxConvexMeshCookingResult::Enum result;
	if (!CPhysX_Mgr::GetInstance()->Get_Cooking()->cookConvexMesh(convexDesc, buf, &result))
		return NULL;
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* convexMesh = CPhysX_Mgr::GetInstance()->Get_Physics()->createConvexMesh(input);

	PxConvexMeshGeometry Convex;
	Convex.convexMesh = convexMesh;
	Convex.scale = PxMeshScale(PxVec3(0.001f, 0.001f, 0.001f));

	PxMaterial* paxMat = CPhysX_Mgr::GetInstance()->Get_Physics()->createMaterial(0.f, 0.f, 0.f);
	m_pBaseActor = CPhysX_Mgr::GetInstance()->Get_Physics()->createRigidDynamic(px);
	m_pBaseActor->setAngularDamping(100.f);
	m_pBaseActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*m_pBaseActor, Convex, *paxMat);
	CPhysX_Mgr::GetInstance()->Get_PxScene()->addActor(*m_pBaseActor);
	//////////////////////////////////////////////////////////////////////////

	D3DXFRAME* pRootFrame = m_pDynamicMeshCom->Get_RootFrame();

	for (auto& data : *pMeshList)
	{
		for (_ulong i = 0; i < data->dwNumBones; ++i)
		{
			const char* pBoneName = data->pSkinInfo->GetBoneName(i);

			D3DXFRAME_DERIVED*	 pDerivedFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(pRootFrame, pBoneName);
			D3DXMESHCONTAINER_DERIVED* pDerivedContainer = (D3DXMESHCONTAINER_DERIVED*)pDerivedFrame->pMeshContainer;

			_mat BoneMatrix = pDerivedFrame->CombinedTransformMatrix;
			PxRigidDynamic* pDynamic = CPhysX_Mgr::GetInstance()->Get_Physics()->createRigidDynamic(PxTransform(px.p.x, px.p.y, px.p.z));
			PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*pDynamic, PxBoxGeometry(0.1f, 0.1f, 0.1f), *paxMat);

			pDynamic->setMass(500.f);	
			pDynamic->setAngularDamping(100.f);
			pDynamic->setMaxLinearVelocity(3.f);

			CPhysX_Mgr::GetInstance()->Get_PxScene()->addActor(*pDynamic);
			// 물리 적용 안함.
			pDynamic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);

			m_tupleBones.emplace_back(make_tuple(pDynamic, BoneMatrix));
		}
	}

	//////////////////////////////////////////////////////////////////////////

	return S_OK;
}

_int CHarvestable_Bush::Update_GameObject(const _double & _dTimeDelta)
{
	CHarvestable::Update_GameObject(_dTimeDelta);

	if (m_tStat.fHp <= 0.f)
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

	return NO_EVENT;
}

_int CHarvestable_Bush::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	//그림자 렌더그룹 추가 : 경계구체가 빛 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	//if (Engine::Check_Sphere(m_vBoundingCenter, m_fBoundingRadius, CULL_LIGHT))
	//{
	//	//Set_ShadowMap(Engine::Get_ShadowLight().vLightPos, Engine::Get_ShadowLight().vLightAt);
	//	g_bInstancing && !m_bBreak ?
	//		Engine::Add_Instance(m_wstrMeshName, RENDER_SHADOWMAP, m_pTransformCom->Get_WorldMatrix()) :
	//		Engine::Add_RenderGroup(RENDER_SHADOWMAP, this);
	//}


	//논알파 렌더그룹 추가 : 경계구체가 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	if (Engine::Check_Sphere(m_vBoundingCenter, m_fBoundingRadius))
	{
		g_bInstancing && !m_bBreak ?
			Engine::Add_Instance(m_wstrMeshName, RENDER_NONALPHA, m_pTransformCom->Get_WorldMatrix()) :
			Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	}

	return NO_EVENT;
}

void CHarvestable_Bush::Render_GameObject()
{
	if (m_bBreak)
	{
		_bool bFirst = false;
		_int i = 0;
		for (auto& data : m_tupleBones)
		{
			// 물리 적용
			get<0>(data)->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);

			if (!bFirst)
			{
				bFirst = true;
				get<0>(data)->addForce(PxVec3(0.f, -1.f, 0.f), PxForceMode::eFORCE);
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
	//이부분 뭔지모르는데 일단주석쳐봄
	//else
	//{
	//	_int i = 0;
	//	for (auto& data : m_tupleBones)
	//	{
	//		m_pDynamicMeshCom->Play_CustomAnimation(i, &get<1>(data));
	//		++i;
	//	}
	//}

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

void CHarvestable_Bush::Render_GameObjectShadowMap()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowMapCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	//m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	//pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);

	_mat matLightView, matLightProj, matWLVP;

	_vec3 vLightPos = _vec3(-400.f, 100.f, 600.f);
	_vec3 vLightAt = vLightPos + _vec3(1.f, -10.f, -15.f);
	_vec3 vLightUp = _vec3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&matLightView, &vLightPos, &vLightAt, &vLightUp);
	D3DXMatrixOrthoLH(&matLightProj, 100.f, 100.f, 0.f, 10000.f);
	matWLVP = m_pTransformCom->Get_WorldMatrix() * matLightView * matLightProj;
	pEffect->SetMatrix("g_matWorldLightViewProj", &matWLVP);

	pEffect->Begin(NULL, 0);

	// ShadowMap Pass 0 기본, 1 디졸브, 2 알파테스트
	if (m_bDissolve)
		pEffect->BeginPass(1);
	else
		pEffect->BeginPass(2);

	if (m_bBreak)
		m_pDynamicMeshCom->Render_MeshesShadowMap(pEffect);
	else
		m_pMeshCom->Render_MeshesShadowMap(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);
}

void CHarvestable_Bush::Render_GameObjectShadow()
{
	if (m_bBreak)
		return;

	LPD3DXEFFECT pEffect = m_pShaderShadowCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	m_pDevice->GetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);

	m_matWVP = m_pTransformCom->Get_WorldMatrix() * m_matView * m_matProj;
	pEffect->SetMatrix("g_matWorldViewProj", &m_matWVP);
	
	/*m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	m_matWLVB = m_matWLV * m_matScaleBias;*/
	//pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);
	//pEffect->SetMatrix("g_matWorldLightViewProjBias", &m_matWLVB);

	_mat matLightView, matLightProj, matWLVP, matScaleBias;

	_vec3 vLightPos = _vec3(-400.f, 100.f, 600.f);
	_vec3 vLightAt = vLightPos + _vec3(1.f, -10.f, -15.f);
	_vec3 vLightUp = _vec3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&matLightView, &vLightPos, &vLightAt, &vLightUp);
	D3DXMatrixOrthoLH(&matLightProj, 100.f, 100.f, 0.f, 10000.f);
	matWLVP = m_pTransformCom->Get_WorldMatrix() * matLightView * matLightProj;
	pEffect->SetMatrix("g_matWorldLightViewProj", &matWLVP);

	float fOffsetX = 0.5f + (0.5f / SHADOW_WIDTH);
	float fOffsetY = 0.5f + (0.5f / SHADOW_HEIGHT);
	matScaleBias = _mat(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		fOffsetX, fOffsetY, 0.0f, 1.0f);
	matScaleBias = matWLVP * matScaleBias;
	pEffect->SetMatrix("g_matWorldLightViewProjBias", &matScaleBias);

	_mat matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &m_pTransformCom->Get_WorldMatrix());
	_vec4 vLightDir_4;
	D3DXVec3Transform(&vLightDir_4, &_vec3(-400.f, 100.f, 600.f), &matInvWorld);
	D3DXVec4Normalize(&vLightDir_4, &vLightDir_4);
	pEffect->SetVector("g_vLightDir", &_vec4(vLightDir_4.x, vLightDir_4.y, vLightDir_4.z, 0.f));

	//D3DXMatrixInverse(&m_matWLVB, NULL, &m_pTransformCom->Get_WorldMatrix());
	//D3DXVec3Transform(&m_vDir, &m_vecLightPos, &m_matWLVB);
	//D3DXVec4Normalize(&m_vDir, &m_vDir);
	//m_vDir.w = 0;
	//pEffect->SetVector("g_vLightDir", &m_vDir);

	Engine::Get_RenderTargetTexture(pEffect, L"ShadowMap", "g_ShadowMapTexture");

	pEffect->Begin(NULL, 0);

	// Shadow Pass 0 기본, 1 디졸브, 2 알파테스트
	if (m_bDissolve)
		pEffect->BeginPass(1);
	else
		pEffect->BeginPass(2);

	if (m_bBreak)
		m_pDynamicMeshCom->Render_MeshesShadow(pEffect);
	else
		m_pMeshCom->Render_MeshesShadow(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);
}

void CHarvestable_Bush::Check_Dying(const _double & _dTimeDelta)
{
	// 죽었을 때
}

CHarvestable_Bush * CHarvestable_Bush::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CHarvestable_Bush*	pInstance = new CHarvestable_Bush(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CHarvestable_Bush::Clone(void * _pArg)
{
	CHarvestable_Bush*	pClone = new CHarvestable_Bush(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CHarvestable_Bush::Free()
{
	CHarvestable::Free();
}

_bool CHarvestable_Bush::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	if (WEAPON_HAND_HARVEST != dwAttackType && WEAPON_DINO != dwAttackType)
		return false;

	_int iAmount = 0;
	_float fAddBerry = 0.f;
	_float fAddFiber = 0.f;
	if (dwAttackType == WEAPON_HAND_HARVEST)
	{
		iAmount = (_int)CRandomFunc::Shake_Float(pAttacker->Get_Stat(STAT_ATTACK), 0.2f);
		//vCollisionPos; //<<피격이펙트출력>> 덤불 : 꽃가루 이펙트

		if (0 > iAmount)
			iAmount = 1;

		fAddBerry = 2.f * (_float)iAmount;
		fAddFiber = 0.5f * (_float)iAmount;

		// 배리 추가
		if (0 < fAddBerry)
		{
			static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_AmarBerry, (_int)fAddBerry);
			static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(fAddBerry);
		}
		// 섬유 추가
		if (0 < fAddFiber)
		{
			static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_Fiber, (_int)fAddFiber);
			static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(fAddFiber);
		}
	}
	// 일단 효율은 다 똑같이 해놓고 나중에 어떻게 할 것인지
	else if(dwAttackType == WEAPON_DINO)
	{
		iAmount = (_int)CRandomFunc::Shake_Float(pAttacker->Get_Stat(STAT_ATTACK), 0.2f);

		if (0 > iAmount)
			iAmount = 1;

		fAddBerry = 0.5f * (_float)iAmount;
		fAddFiber = 2.f *  (_float)iAmount;

		// 배리 추가
		if (0 < fAddBerry)
		{
			static_cast<CDino*>(pAttacker)->Get_Inven()->Add_Item(ITEMID_AmarBerry, (_int)fAddBerry);
			static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_LeftGuideFont(ITEMID_AmarBerry, (_int)fAddBerry, 0);
			static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(fAddBerry);
		}
		// 섬유 추가
		if (0 < fAddFiber)
		{
			static_cast<CDino*>(pAttacker)->Get_Inven()->Add_Item(ITEMID_Fiber, (_int)fAddFiber);
			static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_LeftGuideFont(ITEMID_Fiber, (_int)fAddFiber, 0);
			static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(fAddFiber);
		}
	}

	_bool bHit = CHarvestable::HitBy(pAttacker, dwAttackType, vCollisionPos);

	if (bHit)
	{
		vCollisionPos; //<<피격이펙트출력>> 덤불 : 꽃가루 이펙트
	}

	CGameObject* pGameObject = nullptr;

	m_fDustScale = 0.002f;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Dust", &m_fDustScale);
	NULL_CHECK_RETURN(pGameObject, false);
	FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), false);
	pGameObject->Set_Color(1.f, 0.3f, 0.f);

	_vec3 vDir = *static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos() - *m_pTransformCom->Get_Pos();
	D3DXVec3Normalize(&vDir, &vDir);
	vDir.y = 0.f;
	vDir *= 0.75f;

	static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vCollisionPos + _vec3(0.f, 1.f, 0.f) + vDir);

	return bHit;
}

HRESULT CHarvestable_Bush::Add_Component(_ulong dwOption)
{
	CComponent*		pComponent = nullptr;

	////옵션에 따라 다른 메시 부여 등
	//pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Bush_A"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//pComponent = m_pDynamicMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Bush_A"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_TestMesh", pComponent);

	//옵션값에 따라 크기배율을 달리 적용함.
	//(CBush는 현재 해당사항 없음)
	_int iAdj = 1;
	//if (4 < dwOption)
	//	iAdj = 4;
	//else if (2 < dwOption)
	//	iAdj = 2;

	_float fSize = Get_Scale().x * iAdj;
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_HIT;
	tInfo.fRadius = 110.f * fSize;
	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);

	tInfo.fRadius = 110.f * fSize;
	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);

	//충돌체 월드위치 세팅
	_vec3 vPos = Get_Pos();
	vPos.y += 0.5f;
	m_pSphereColliderCom->Set_Pos(vPos);

	return S_OK;
}

_int CHarvestable_Bush::Check_Arg(void * _pArg)
{
	if (_pArg)
	{
		if (dynamic_cast<OBJINFO*>((OBJINFO*)_pArg))
		{
			OBJINFO tInfo;
			memcpy(&tInfo, _pArg, sizeof(OBJINFO));

			//dwOption으로 받은 MeshID값에따라 넣을메쉬 판단
			TCHAR szText[MAX_PATH];
			Engine::Get_MeshTag_Bush(szText, tInfo.dwOption);

			if (!szText || E_FAIL == Init_Mesh(szText))
			{
				ERR_MSG(L"CHarvestable_Pebble : Failed to Find MeshTag");
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
				ERR_MSG(L"CHarvestable_Pebble : Failed to Find MeshTag_DM");
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
		//Argument 없을 때 : 기본메시 로드
	{
		Init_Mesh(L"Proto_StaticMesh_Bush_A");
	}
	return 0;
}
