#include "stdafx.h"
#include "Dodo_Ball.h"
#include "Export_Function.h"
#include "PhysX_Mgr.h"
#include "Player.h"

USING(Engine)

CDodo_Ball::CDodo_Ball(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDodo_Ball::CDodo_Ball(const CDodo_Ball & rhs)
	: CDino(rhs)
	, m_iIndex(rhs.m_iIndex)
{
}

HRESULT CDodo_Ball::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDodo_Ball::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	m_wstrDinoSpecies = L"DodoBall";
	m_wstrDinoSpeciesKor = L"도도볼";

	m_pNeckBoneName = "neck";
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_Scale(0.015f);

	//// 기본 세팅
	m_iAnimation = DODO_ANIM_IDLE;
	m_eUnitState = STATE_IDLE;
	
	m_pDynamic = CPhysX_Mgr::GetInstance()->CreateSphereRigidDynamic(Get_Pos(), 0.5f);
	if (nullptr == m_pDynamic)
		return E_FAIL;

	Load_Collider(L"../Resources/Data/Collider/DodoBall_Collider.dat");

	return S_OK;
}

_int CDodo_Ball::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
	{
		// 이펙트 UI 처리?
		CGameObject* pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Goal");
		NULL_CHECK_RETURN(pGameObject, OBJECT_DEAD);
		FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), OBJECT_DEAD);
		static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(Get_Pos());

		return OBJECT_DEAD;
	}
	CUnit::Update_GameObject(_dTimeDelta);

	if (!m_bHanging)
	{
		m_iAnimation = DODO_ANIM_IDLE;
		PxTransform pxPos = m_pDynamic->getGlobalPose();
		m_pTransformCom->Set_Pos(pxPos.p.x, pxPos.p.y, pxPos.p.z);
		/*m_pTransformCom->Set_IsYawPitchRoll(true);
		m_pTransformCom->Set_YawPitchRoll(pxPos.q.y, pxPos.q.x, pxPos.q.z);*/
	}

	return NO_EVENT;
}

_int CDodo_Ball::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CUnit::LateUpdate_GameObject(_dTimeDelta);

	if (m_bHanging)
	{
		m_iAnimation = DODO_ANIM_STARLED;
		// 플레이어 손뼈 포인터 가져오기
		if (m_pBoneMatrix == nullptr)
		{
			CDynamicMesh*	pPlayerMeshCom = static_cast<CDynamicMesh*>(Engine::CManagement::GetInstance()->Get_Component(L"Layer_Player", 0, L"Com_Mesh", ID_STATIC));
			const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("c_R_weapon_jnt");
			m_pBoneMatrix = &pFrame->CombinedTransformMatrix;

			CTransform*	pPlayerTransformCom = static_cast<CTransform*>(static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0))->Get_Component(L"Com_Transform", ID_DYNAMIC));
			m_pParentWorld = &pPlayerTransformCom->Get_WorldMatrix();
		}

		_mat matWorld, matScale, matTrans;
		matWorld = *m_pBoneMatrix * *m_pParentWorld;

		m_pTransformCom->Set_IsYawPitchRoll(false);
		m_pTransformCom->Set_ParentMatrix(&matWorld);

		_vec3 vPos;
		memcpy(&vPos, &matWorld.m[INFO_POS][0], sizeof(_vec3));
		Set_Pos(vPos);

		m_pTransformCom->Update_Component(0);
	}

	m_pMeshCom->Set_Animation(m_iAnimation);				// 전체 애니메이션

	m_dAnimationTime = _dTimeDelta;

	_vec3 vSpherePos = m_pSphereColliderCom->Get_InteractPos();
	D3DXVec3TransformCoord(&vSpherePos, &vSpherePos, &m_pTransformCom->Get_WorldMatrix());
	_float fSphereRadius = m_pSphereColliderCom->Get_InteractRadius() * Get_Scale().x;

	if (m_pSphereColliderCom)
	{
		//컬라이더 반경은 공룡 스케일값으로 나눠진 상태이므로 스케일을 도로 곱해 월드로 변환해준다
		if (Engine::Check_Sphere(vSpherePos, fSphereRadius))
			Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	}

	return NO_EVENT;
}

void CDodo_Ball::Render_GameObject()
{
	//m_pMeshCom->Play_Animation(m_dAnimationTime, 1, nullptr);


	CDino::Render_GameObject();

	//IsRide_Set_SaddleMat();
}

void CDodo_Ball::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDodo_Ball::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDodo_Ball::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3 & vCollisionPos)
{
	if (dwAttackType == 1)
		return false;

	_float fColY = vCollisionPos.y;

	CSphereCollider::SPHEREINFO tColInfo = m_pSphereColliderCom->Get_listCollider(CSphereCollider::COL_HIT)->front();
	_vec3 vWorldPos;
	memcpy(&vWorldPos, &tColInfo.matColMatrix.m[INFO_POS][0], sizeof(_vec3));

	// 골인 이펙트 점수 계산
	if (vWorldPos.y < fColY)
	{
		m_bDead = true;
		return true;
	}

	return false;
}

CDodo_Ball * CDodo_Ball::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDodo_Ball*	pInstance = new CDodo_Ball(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDodo_Ball::Clone(void * _pArg)
{
	CDodo_Ball*	pClone = new CDodo_Ball(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDodo_Ball::Free()
{
	CDino::Free();
}

void CDodo_Ball::Throw_Ball()
{
	m_bHanging = false;
	m_pHangingMatrix = nullptr;

	_vec3 vDir;
	_mat matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	memcpy(&vDir, &matView.m[INFO_LOOK][0], sizeof(_vec3));
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vPos;
	memcpy(&vPos, &m_pTransformCom->Get_WorldMatrix().m[INFO_POS][0], sizeof(_vec3));
	m_pDynamic->setGlobalPose(PxTransform(vPos.x, vPos.y, vPos.z));
	m_pDynamic->addForce(vDir * 500.f);
}

void CDodo_Ball::Check_State()
{

}

HRESULT CDodo_Ball::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Dodo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

HRESULT CDodo_Ball::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}
