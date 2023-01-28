#include "stdafx.h"
#include "..\Headers\Bullet.h"

#include "Export_Function.h"

#include "SphereCollider.h"
#include "CollisionFunc.h"

#include "Unit.h"
#include "Dino.h"
#include "Player.h"

#include "PhysX_Mgr.h"

USING(Engine)

CBullet::CBullet(LPDIRECT3DDEVICE9 _pDevice)
	: CUnit(_pDevice)
{
}

CBullet::CBullet(const CBullet & _rhs)
	: CUnit(_rhs)
	, m_tBulletInfo(_rhs.m_tBulletInfo)
	, m_dTime(_rhs.m_dTime)
{
}

HRESULT CBullet::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBullet::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	m_tBulletInfo = *(BULLET_DESC*)_pArg;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eUnitType = UNIT_BULLET;

	m_pTransformCom->Set_Pos(m_tBulletInfo.vPos);
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	
	pDynamic = CPhysX_Mgr::GetInstance()->Get_Physics()->createRigidDynamic(PxTransform(m_tBulletInfo.vPos.x, m_tBulletInfo.vPos.y, m_tBulletInfo.vPos.z));
	pDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	pDynamic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*pDynamic, PxSphereGeometry(0.01f), *CPhysX_Mgr::GetInstance()->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f));
	CPhysX_Mgr::GetInstance()->Get_PxScene()->addActor(*pDynamic);
	// 물리 적용 안함.

	// 캡슐 피직스
	//PxCapsuleControllerDesc cCapusuledesc;
	//cCapusuledesc.height = 0.2f;
	//cCapusuledesc.radius = 0.2f;
	//cCapusuledesc.material = CPhysX_Mgr::GetInstance()->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
	//cCapusuledesc.position = PxExtendedVec3(m_tBulletInfo.vPos.x, m_tBulletInfo.vPos.y, m_tBulletInfo.vPos.z);
	//m_pCtrl = static_cast<PxCapsuleController*>(CPhysX_Mgr::GetInstance()->Get_Manager()->createController(cCapusuledesc));

	switch (m_tBulletInfo.eBulletType)
	{
	case BULLET_BOLA:
		m_pTransformCom->Set_IsYawPitchRoll(true);
		m_pTransformCom->Set_YawPitchRoll(0.f, 0.f, 0.f);

		// 좌우
		m_fYaw = D3DXToRadian(m_tBulletInfo.fAngleY);
		// 상하
		m_fPitch = D3DXToRadian(m_tBulletInfo.fAngleX);

		//
		m_tStat.fAtk = 0.f;
		break;
	case BULLET_ARROW_TORPID:
		m_tStat.fTorpidRate = 2.5f;
	case BULLET_ARROW:
		m_pTransformCom->Set_IsYawPitchRoll(true);
		m_pTransformCom->Set_YawPitchRoll(0.f, 0.f, 0.f);

		// 좌우
		m_fYaw = D3DXToRadian(m_tBulletInfo.fAngleY - 90.f);
		// 상하
		m_fRoll = -D3DXToRadian(m_tBulletInfo.fAngleX);

		//
		m_tStat.fAtk = 20.f;
	default:
		break;
	}

	m_pTransformCom->Set_YawPitchRoll(m_fYaw, m_fPitch, m_fRoll);
	//m_pTransformCom->Update_Component(0);

	return S_OK;
}

_int CBullet::Update_GameObject(const _double & _dTimeDelta)
{
	m_eUnitType = CUnit::UNIT_BULLET;
	if (m_bDead)
		return OBJECT_DEAD;

	CGameObject::Update_GameObject(_dTimeDelta);

	if (m_pParentMat)
	{
		// 
		m_pTransformCom->Set_WorldMatrix(&m_matLoaclMat);
		m_pTransformCom->Set_ParentMatrix(m_pParentMat);
	}

	//_mat matWorld = m_pTransformCom->Get_WorldMatrix();
	Check_DinoHit();

	if (!m_bIsHit)
	{
		// 투사체 이동
		_vec3 vPos, vLook;
		vPos = *m_pTransformCom->Get_Pos();

		if (BULLET_BOLA == m_tBulletInfo.eBulletType)
		{
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			vPos += -vLook * m_tBulletInfo.fSpeed * (_float)_dTimeDelta;

			m_fRoll += (_float)_dTimeDelta * 5.f;						// 회전
			m_fPitch -= (_float)_dTimeDelta * m_tBulletInfo.fGravity;	// 낙하
		}
		else if (BULLET_ARROW == m_tBulletInfo.eBulletType || BULLET_ARROW_TORPID == m_tBulletInfo.eBulletType)
		{
			m_pTransformCom->Get_Info(INFO_RIGHT, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			vPos += -vLook * m_tBulletInfo.fSpeed * (_float)_dTimeDelta;

			m_fRoll += (_float)_dTimeDelta * m_tBulletInfo.fGravity;	// 낙하
		}

		m_pTransformCom->Set_YawPitchRoll(m_fYaw, m_fPitch, m_fRoll);

		// 피직스 이동
		if (pDynamic)
		{
			vPos += vLook * 0.1f;
			m_pTransformCom->Set_Pos(vPos);
			vPos -= vLook * 0.5f;
			m_pSphereColliderCom->Set_Pos(vPos);

			pDynamic->setGlobalPose(PxTransform(vPos.x, vPos.y, vPos.z));
		}

		PxVec3 pxPos((_float)pDynamic->getGlobalPose().p.x, (_float)pDynamic->getGlobalPose().p.y, (_float)pDynamic->getGlobalPose().p.z);
		if (CPhysX_Mgr::GetInstance()->IsOverlap_Sphere_StaticObject(0.1f, pxPos))
		{
			// 피직스 지형과 충돌됨
			if (BULLET_ARROW == m_tBulletInfo.eBulletType || BULLET_ARROW_TORPID == m_tBulletInfo.eBulletType)
			{
				m_bIsHit = true;
				if (pDynamic)
				{
					pDynamic->release();
					pDynamic = nullptr;
				}
			}
		}
	}

	if (m_bIsHit)
	{
		_vec3 asd;
		m_pTransformCom->Get_Info(INFO_POS, &asd);
		m_pSphereColliderCom->Set_Pos(asd);
	}

	return NO_EVENT;
}

_int CBullet::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	m_dTime += _dTimeDelta;
	if (10 < m_dTime)
		Set_Dead();

	// 랜더그룹 추가
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	//_mat matWorld = m_pTransformCom->Get_WorldMatrix();

	return NO_EVENT;
}

void CBullet::Render_GameObject()
{
	//_mat matWorld = m_pTransformCom->Get_WorldMatrix();

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	if (BULLET_BOLA == m_tBulletInfo.eBulletType)
	{
		if (m_bIsHit)
			m_pMeshBola->Render_Meshes(pEffect);
		else
			m_pMeshCom->Render_Meshes(pEffect);
	}
	else
		m_pMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	Safe_Release(pEffect);

#ifdef _DEBUG
	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Render_SphereCollider();
#endif // _DEBUG
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CBullet*	pInstance = new CBullet(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CBullet::Clone(void * _pArg/* = nullptr*/)
{
	CBullet*	pClone = new CBullet(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CBullet::Free()
{
	if (m_bIsClone)
	{
		Safe_Release(m_pAttcker);

		if (m_pCtrl)
		{
			m_pCtrl->release();
			m_pCtrl = nullptr;
		}
	}

	CGameObject::Free();
}

HRESULT CBullet::Add_Component()
{
	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	Add_MeshComponent();

	/// 트랜스폼 컴포넌트
	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	/// 쉐이더 컴포넌트
	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Normal_Shadow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	/// 충돌체 컴포넌트
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_ATTACK;
	tInfo.fRadius = 0.01f;
	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);


	return S_OK;
}

HRESULT CBullet::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat matWorld, matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	matWorld = m_pTransformCom->Get_WorldMatrix();

	// 상수테이블 세팅
	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

HRESULT CBullet::Add_MeshComponent()
{
	CComponent*		pComponent = nullptr;

	switch (m_tBulletInfo.eBulletType)
	{
	case BULLET_BOLA:
		pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Bola_Thrown"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

		pComponent = m_pMeshBola = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Bola_Wrapped"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshBola", pComponent);

		break;
	case BULLET_SPEAR:
		break;
	case BULLET_ARROW_TORPID:
	case BULLET_ARROW:
		pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Bow_Arrow"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);
		break;
	default:
		return E_FAIL;
	}
	return S_OK;
}

void CBullet::Check_DinoHit()
{
	if (!m_bIsHit)
	{
		CSphereCollider::SPHEREINFO tColInfo = m_pSphereColliderCom->Get_listCollider(CSphereCollider::COL_ATTACK)->front();
		_vec3 vWorldPos;
		memcpy(&vWorldPos, &tColInfo.matColMatrix.m[INFO_POS][0], sizeof(_vec3));

		CGameObject* pHitObj = nullptr;
		if (CCollisionFunc::AttackOnUnit(this, &vWorldPos, &tColInfo.fRadius, L"Layer_Dino", CSphereCollider::COL_HIT, m_tBulletInfo.eBulletType, nullptr, nullptr, true, &m_pParentMat, &m_pTargetObj) ||
			CCollisionFunc::AttackOnUnit(this, &vWorldPos, &tColInfo.fRadius, L"Layer_Dino_Tamed", CSphereCollider::COL_HIT, m_tBulletInfo.eBulletType, nullptr, nullptr, true, &m_pParentMat, &m_pTargetObj))
		{
			if (BULLET_BOLA != m_tBulletInfo.eBulletType)
			{
				m_bIsHit = true;

				Set_ParentMat(m_pParentMat);

				//_mat matTarget;
				//D3DXMatrixInverse(&matTarget, NULL, m_pParentMat);
				//
				//m_matLoaclMat = m_pTransformCom->Get_WorldMatrix() * matTarget;
				//
				//m_pTransformCom->Set_WorldMatrix(&m_matLoaclMat);
				//
				//m_pTransformCom->Set_ParentMatrix(m_pParentMat);
				//
				//// 피직스 구체 지우기
				//if (pDynamic)
				//{
				//	pDynamic->release();
				//	pDynamic = nullptr;
				//}
			}
			else if (BULLET_BOLA == m_tBulletInfo.eBulletType)
			{
				m_pParentMat = nullptr;
			}
		}
	}

	if (m_pTargetObj)
	{
		if (static_cast<CDino*>(m_pTargetObj)->Get_Dead())
			Set_Dead();
	}
}

void CBullet::TobeBolaWrapped(const _mat& _matTargetWorld, const _float& _fScale)
{
	if (BULLET_BOLA != m_tBulletInfo.eBulletType)
		return;

	m_pTransformCom->Set_IsYawPitchRoll(false);
	m_pParentMat = nullptr;
	m_bIsHit = true;

	_vec3 vPos, vScale;
	_float fSize = _fScale * 0.05f;

	memcpy(&vPos, &_matTargetWorld.m[INFO_POS][0], sizeof(_vec3));
	vScale = _vec3(fSize, fSize, fSize);

	vPos.y += _fScale * 0.5f;

	m_pTransformCom->Set_Scale(vScale/*_vScale*/);
	m_pTransformCom->Set_Pos(vPos);

	m_dTime = 0.0;
}

void CBullet::Set_ParentMat(_mat * _pParentMat)
{
	m_pParentMat = _pParentMat;

	_mat matTarget;
	D3DXMatrixInverse(&matTarget, NULL, m_pParentMat);

	m_matLoaclMat = m_pTransformCom->Get_WorldMatrix() * matTarget;

	m_pTransformCom->Set_WorldMatrix(&m_matLoaclMat);

	m_pTransformCom->Set_ParentMatrix(m_pParentMat);

	// 피직스 구체 지우기
	if (pDynamic)
	{
		pDynamic->release();
		pDynamic = nullptr;
	}
}
