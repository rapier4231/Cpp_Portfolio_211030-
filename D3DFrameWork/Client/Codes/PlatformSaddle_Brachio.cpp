#include "stdafx.h"
#include "..\Headers\PlatformSaddle_Brachio.h"

#include "Export_Function.h"

#include "Dino_Brachio.h"

#include "PhysX_Mgr.h"

USING(Engine)

CPlatformSaddle_Brachio::CPlatformSaddle_Brachio(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CPlatformSaddle_Brachio::CPlatformSaddle_Brachio(const CPlatformSaddle_Brachio & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlatformSaddle_Brachio::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CPlatformSaddle_Brachio::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Rotate(ROT_X, D3DXToRadian(88.f));
	m_pTransformCom->Set_Rotate(ROT_Z, D3DXToRadian(90.f));
	m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(0.f));
	m_pTransformCom->Set_Pos(-200.f, 0.f, 145.f);

	D3DXMatrixIdentity(&m_matParent);

	Set_PhysXCook();

	return S_OK;
}

_int CPlatformSaddle_Brachio::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	CGameObject::Update_GameObject(_dTimeDelta);

	Move_PhysX();

	return NO_EVENT;
}

_int CPlatformSaddle_Brachio::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return NO_EVENT;
}

void CPlatformSaddle_Brachio::Render_GameObject()
{
	// 쉐이더 컴객체 생성
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect, false);

	pEffect->End();

	Safe_Release(pEffect);

#ifdef _DEBUG
	m_pSphereColliderCom->Render_SphereCollider();
#endif
}

void CPlatformSaddle_Brachio::Render_GameObjectShadowMap()
{
}

void CPlatformSaddle_Brachio::Render_GameObjectShadow()
{
}

CPlatformSaddle_Brachio * CPlatformSaddle_Brachio::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CPlatformSaddle_Brachio*	pInstance = new CPlatformSaddle_Brachio(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CPlatformSaddle_Brachio::Clone(void * _pArg/* = nullptr*/)
{
	CPlatformSaddle_Brachio*	pClone = new CPlatformSaddle_Brachio(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CPlatformSaddle_Brachio::Free()
{
	CGameObject::Free();
}

void CPlatformSaddle_Brachio::Set_TargetDino(CGameObject* _pTargetDino)
{
	m_pTargetDino = static_cast<CDino_Brachio*>(_pTargetDino);
}

HRESULT CPlatformSaddle_Brachio::Add_Component()
{
	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Brachio_Platform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	/// 트랜스폼
	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	/// 쉐이더
	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Normal_Shadow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	/// 충돌체 컴포넌트
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	tInfo.fRadius = 10.f;
	tInfo.vSpherePos = _vec3(0.f, 0.f, 0.f);

	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);


	return S_OK;
}

HRESULT CPlatformSaddle_Brachio::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat matWorld, matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	// 상수테이블 세팅
	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);
	_pEffect->SetVector("g_vSetColor", &_vec4(m_vColor, 1.f));

	return S_OK;
}

HRESULT CPlatformSaddle_Brachio::Set_PhysXCook()
{
	/// 피직스 굽기
	if (m_pBaseActor)
		m_pBaseActor->release();

	_vec3 vPos = *m_pTransformCom->Get_Pos();
	_vec3 vScale = *m_pTransformCom->Get_Scale();
	_float fAngleX = m_pTransformCom->Get_Rotate(ROT_X);
	_float fAngleY = m_pTransformCom->Get_Rotate(ROT_Y);
	_float fAngleZ = m_pTransformCom->Get_Rotate(ROT_Z);

	_mat matRot;
	vScale = _vec3(0.02f, 0.02f, 0.02f);
	D3DXMatrixRotationYawPitchRoll(&matRot, fAngleY, fAngleX, fAngleZ);	//각 축 회전값으로 3x3매트릭스를 생성
	PxMat33 mat33(PxVec3(matRot._11, matRot._12, matRot._13)			//이때 요피치롤이 yxz 순서임에 유의
		, PxVec3(matRot._21, matRot._22, matRot._23)
		, PxVec3(matRot._31, matRot._32, matRot._33));
	PxTransform px = PxTransform(vPos.x, vPos.y, vPos.z, PxQuat(mat33));

	////////////////////////////////////////////////////////////////////////////
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = m_pMeshCom->Get_VtxNum();
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = m_pMeshCom->Get_VtxPos();
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

	return S_OK;
}

void CPlatformSaddle_Brachio::Move_PhysX()
{
	if (!m_pBaseActor)
		return;

	CDynamicMesh* pMesh = static_cast<CDynamicMesh*>(m_pTargetDino->Get_Component(L"Com_Mesh", ID_STATIC));
	CTransform* pTransform = static_cast<CTransform*>(m_pTargetDino->Get_Component(L"Com_Transform", ID_DYNAMIC));

	m_matParent = pMesh->Get_FrameByName("c_back4")->CombinedTransformMatrix * pTransform->Get_WorldMatrix();
	m_pTransformCom->Set_ParentMatrix(&m_matParent);

	_mat matWorld = m_pTransformCom->Get_WorldMatrix();


	_vec3 vPos = *m_pTransformCom->Get_Pos();
	_vec3 vScale = _vec3(0.02f, 0.02f, 0.02f)/**m_pTransformCom->Get_Scale()*/;
	_float fAngleX = pTransform->Get_Rotate(ROT_X);
	_float fAngleY = pTransform->Get_Rotate(ROT_Y);
	_float fAngleZ = pTransform->Get_Rotate(ROT_Z);

	memcpy(&vPos, &matWorld.m[INFO_POS][0], sizeof(_vec3));

	_mat matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, fAngleY, fAngleX, fAngleZ);	//각 축 회전값으로 3x3매트릭스를 생성
	PxMat33 mat33(PxVec3(matRot._11, matRot._12, matRot._13)			//이때 요피치롤이 yxz 순서임에 유의
		, PxVec3(matRot._21, matRot._22, matRot._23)
		, PxVec3(matRot._31, matRot._32, matRot._33));
	PxTransform px = PxTransform(vPos.x, vPos.y, vPos.z, PxQuat(mat33));

	m_pBaseActor->setGlobalPose(px);

	// 충돌체 업데이트
	m_pSphereColliderCom->Set_Pos(vPos);
}
