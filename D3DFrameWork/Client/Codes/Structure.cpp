#include "stdafx.h"
#include "..\Headers\Structure.h"

#include "Export_Function.h"

#include "PhysX_Mgr.h"
#include "PlatformSaddle_Brachio.h"

_ulong	CStructure::m_dwIndex = 0;

CStructure::CStructure(LPDIRECT3DDEVICE9 _pDevice)
	: CUnit(_pDevice)
{
}

CStructure::CStructure(const CStructure & _rhs)
	: CUnit(_rhs)
	//, m_bIsPreview(_rhs.m_bIsPreview)
{
	ZeroMemory(&m_tStructurInfo, sizeof(STRUCTURE_DESC));
	D3DXMatrixIdentity(&m_matLocal);
}

HRESULT CStructure::Ready_GameObject_Prototype()
{
	CUnit::Ready_GameObject_Prototype();

	m_eUnitType = UNIT_STRUCTURE;

	return S_OK;
}

HRESULT CStructure::Ready_GameObject(void * _pArg)
{
	CUnit::Ready_GameObject(_pArg);

	// 프리뷰가 아닐 경우 인덱스 추가하고 증가
	if (!m_tStructurInfo.bIsPreview)
	{
		m_tStructurInfo = *(STRUCTURE_DESC*)_pArg;

		m_tStructurInfo.dwIndex = m_dwIndex;
		++m_dwIndex;
	}
	
	m_tStructurInfo.pParentMat = nullptr;

	return S_OK;
}

_int CStructure::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	CGameObject::Update_GameObject(_dTimeDelta);

	if (m_tStructurInfo.bIsPreview)
		m_pSphereColliderCom->Set_Pos(Get_Pos());

	if (m_bDissolve)
	{
		m_fDissolveAmount -= (_float)_dTimeDelta;

		if (0.f >= m_fDissolveAmount)
			m_bDissolve = false;
	}

	return NO_EVENT;
}

_int CStructure::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	// 부모가 있을 경우 피직스와 위치 이동
	Move_PhysX();

	// 렌더그룹 추가 : Interaction 컬라이더가 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	// 컬라이더 좌표는 로컬이므로 월드로 변환해준다
	_vec3 vSpherePos = m_pSphereColliderCom->Get_InteractPos();
	//D3DXVec3TransformCoord(&vSpherePos, &vSpherePos, &m_pTransformCom->Get_WorldMatrix());
	//컬라이더 반경은 공룡 스케일값으로 나눠진 상태이므로 스케일을 도로 곱해 월드로 변환해준다
	if (Engine::Check_Tree(vSpherePos, m_pSphereColliderCom->Get_InteractRadius()/* * Get_Scale().x*/))
		Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return NO_EVENT;
}

void CStructure::Render_GameObject()
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

void CStructure::Render_GameObjectShadowMap()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowMapCom->Get_EffectHandle();
	pEffect->AddRef();

	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Render_MeshesShadowMap(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);
}

void CStructure::Render_GameObjectShadow()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowCom->Get_EffectHandle();
	pEffect->AddRef();

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
	Engine::Get_RenderTargetTexture(pEffect, L"ShadowBlurY", "g_ShadowBlurTexture");
	pEffect->SetBool("g_bTerrain", false);

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Render_MeshesShadow(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);
}

void CStructure::Free()
{
	if (m_bIsClone)
	{
		if(m_pBaseActor)
			m_pBaseActor->release();
	}

	CUnit::Free();
}

void CStructure::Set_PreviewPos(const _vec3 & _vPos)
{
	Set_Pos(_vPos);
}

void CStructure::Set_PreviewAngle(const _float & _fAngleY)
{
	m_tStructurInfo.fAngleY = _fAngleY;
	m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(_fAngleY));
}

void CStructure::Set_TartgetPlatform(CGameObject * _pTargetPlatform)
{
	if (nullptr == _pTargetPlatform)
		return;

	m_pTargetPlatform = _pTargetPlatform;

	// 해당 플랫폼을 기준으로 로컬매트릭스 변경
	CTransform* pTransform = static_cast<CTransform*>(m_pTargetPlatform->Get_Component(L"Com_Transform", ID_DYNAMIC));

	m_matLocal = m_pTransformCom->Get_WorldMatrix();

	_mat matTargetWorld = pTransform->Get_WorldMatrix();
	D3DXMatrixInverse(&matTargetWorld, NULL, &matTargetWorld);

	m_matLocal *= matTargetWorld;
}

HRESULT CStructure::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CUnit::SetUp_ConstantTable(_pEffect);

	return S_OK;
}

HRESULT CStructure::Set_PhysXCook()
{
	//if (STRUCTURE_THATCH_DOORFRAME == m_tStructurInfo.eStructureType)
	//	return S_OK;

	/// 피직스 굽기
	if (m_pBaseActor)
		m_pBaseActor->release();

	_vec3 vPos = *m_pTransformCom->Get_Pos();
	_vec3 vScale = *m_pTransformCom->Get_Scale();
	_mat matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, Get_RotY(), Get_RotX(), Get_RotZ());	//각 축 회전값으로 3x3매트릭스를 생성
	PxMat33 mat33(PxVec3(matRot._11, matRot._12, matRot._13)			//이때 요피치롤이 yxz 순서임에 유의
		, PxVec3(matRot._21, matRot._22, matRot._23)
		, PxVec3(matRot._31, matRot._32, matRot._33));
	PxTransform px = PxTransform(vPos.x, vPos.y, vPos.z, PxQuat(mat33));

	////////////////////////////////////////////////////////////////////////////

	if (STRUCTURE_THATCH_DOORFRAME == m_tStructurInfo.eStructureType ||
		STRUCTURE_THATCH_ROOF == m_tStructurInfo.eStructureType ||
		STRUCTURE_STONE_GATEFRAME == m_tStructurInfo.eStructureType)
	{
		/// 정밀한 피직스 굽기
		//버텍스와 인덱스 정보 저장
		PxTriangleMeshDesc meshDesc;
		if (nullptr == m_pMeshLodCom)
		{
			meshDesc.points.count = m_pMeshCom->Get_VtxNum();
			meshDesc.points.data = m_pMeshCom->Get_VtxPos();
			meshDesc.triangles.count = m_pMeshCom->Get_IndexNum();
			meshDesc.triangles.stride = m_pMeshCom->Get_dwStride();
			meshDesc.triangles.data = m_pMeshCom->Get_Index();
		}
		else
		{
			meshDesc.points.count = m_pMeshLodCom->Get_VtxNum();
			meshDesc.points.data = m_pMeshLodCom->Get_VtxPos();
			meshDesc.triangles.count = m_pMeshLodCom->Get_IndexNum();
			meshDesc.triangles.stride = m_pMeshLodCom->Get_dwStride();
			meshDesc.triangles.data = m_pMeshLodCom->Get_Index();
		}

		meshDesc.points.stride = sizeof(PxVec3);
		meshDesc.flags = PxMeshFlag::e16_BIT_INDICES;

		// 데이터를 직렬화 시키기
		PxDefaultMemoryOutputStream writeBuffer;
		PxTriangleMeshCookingResult::Enum result;

		CPhysX_Mgr::GetInstance()->Get_Cooking()->cookTriangleMesh(meshDesc, writeBuffer, &result);
		PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		
		// 직렬화된 데이터 읽어와서 삼각형 메쉬를 만든다.
		PxTriangleMesh* m_TriangleMesh = CPhysX_Mgr::GetInstance()->Get_Physics()->createTriangleMesh(readBuffer);

		// 잘 모르겠음 스태틱 리지드바디로 생성해서 씬에 넣어주는 것 같다.
		PxTriangleMeshGeometry triGeom;
		triGeom.triangleMesh = m_TriangleMesh;
		triGeom.scale = PxMeshScale(PxVec3(vScale.x, vScale.y, vScale.z));


		m_pBaseActor = CPhysX_Mgr::GetInstance()->Get_Physics()->createRigidStatic(px);
		PxMaterial* paxMat = CPhysX_Mgr::GetInstance()->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
		PxShape* shape = PxRigidActorExt::createExclusiveShape(*m_pBaseActor, triGeom, *paxMat);

		CPhysX_Mgr::GetInstance()->Get_PxScene()->addActor(*m_pBaseActor);
	}
	else
	{
		/// 값싼 피직스 굽기
		PxConvexMeshDesc convexDesc;

		convexDesc.points.count = m_pMeshCom->Get_VtxNum();
		convexDesc.points.data = m_pMeshCom->Get_VtxPos();
		convexDesc.points.stride = sizeof(PxVec3);
		convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;


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
	}

	return S_OK;
}

void CStructure::Move_PhysX()
{
	if (nullptr == m_pTargetPlatform)
		return;

	// 플랫폼 따라 움직이기
	CTransform* pTransform = static_cast<CTransform*>(m_pTargetPlatform->Get_Component(L"Com_Transform", ID_DYNAMIC));

	_mat matParent;

	D3DXMatrixScaling(&matParent, 1.f, 1.f, 1.f);

	matParent *= pTransform->Get_WorldMatrix();

	m_pTransformCom->Set_WorldMatrix(&m_matLocal);
	m_pTransformCom->Set_ParentMatrix(&matParent);

	_vec3 vPos, vLook;
	_mat matWorld = m_pTransformCom->Get_WorldMatrix();

	memcpy(&vPos, &matWorld.m[INFO_POS][0], sizeof(_vec3));
	//memcpy(&vPos, &m_pTransformCom->Get_WorldMatrix().m[INFO_POS][0], sizeof(_vec3));
	m_pSphereColliderCom->Set_Pos(vPos);
	m_pSphereColliderCom->Update_Component(0);
	Move_Pivot();


	// 피직스 움직임
	if (!m_pBaseActor)
		return;
	pTransform = static_cast<CTransform*>(static_cast<CPlatformSaddle_Brachio*>(m_pTargetPlatform)->Get_TargetDino()->Get_Component(L"Com_Transform", ID_DYNAMIC));

	//_float fAngleX = pTransform->Get_Rotate(ROT_X);
	_float fAngleY = pTransform->Get_Rotate(ROT_Y);
	//_float fAngleZ = pTransform->Get_Rotate(ROT_Z);

	memcpy(&vLook, &matWorld.m[INFO_LOOK][0], sizeof(_vec3));
	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);

	_float fRadian = D3DXVec3Dot(&vLook, &_vec3(0.f, 0.f, 1.f));
	fRadian = acosf(fRadian);
	if (0.f > vLook.x)
		fRadian *= -1.f;

	_mat matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, fRadian, 0.f, 0.f);	//각 축 회전값으로 3x3매트릭스를 생성

	PxMat33 mat33(PxVec3(matRot._11, matRot._12, matRot._13)			//이때 요피치롤이 yxz 순서임에 유의
		, PxVec3(matRot._21, matRot._22, matRot._23)
		, PxVec3(matRot._31, matRot._32, matRot._33));

	PxTransform px = PxTransform(vPos.x, vPos.y, vPos.z, PxQuat(mat33));
	m_pBaseActor->setGlobalPose(px);
}

void CStructure::Move_Pivot()
{
	return;
}
