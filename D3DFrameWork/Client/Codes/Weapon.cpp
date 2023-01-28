#include "stdafx.h"
#include "..\Headers\Weapon.h"

#include "Export_Function.h"
#include "CollisionFunc.h"

#include "Player.h"

USING(Engine)

CWeapon::CWeapon(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CWeapon::CWeapon(const CWeapon & _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CWeapon::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CWeapon::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	m_eWeaponType = *(WEAPON_TYPE*)_pArg;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 플레이어 참조
	m_pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0));
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);
	m_pPlayer->AddRef();

	// 플레이어 손뼈 포인터 가져오기
	CDynamicMesh*	pPlayerMeshCom = static_cast<CDynamicMesh*>(Engine::CManagement::GetInstance()->Get_Component(L"Layer_Player", 0, L"Com_Mesh", ID_STATIC));
	const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("c_R_weapon_jnt");
	m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

	// 무기별 능력치
	switch (m_eWeaponType)
	{
	case WEAPON_HATCHET:
		m_fWeaponAtt = 15.f;
		m_fWeaponTorpid = 0.3f;
		
		// 1인칭 손뼈 위치
		pPlayerMeshCom = static_cast<CDynamicMesh*>(Engine::CManagement::GetInstance()->Get_Component(L"Layer_Player", 0, L"Com_MeshFPV", ID_STATIC));
		pFrame = pPlayerMeshCom->Get_FrameByName("hatchet_jnt");
		m_pFPVParentBoneMatrix = &pFrame->CombinedTransformMatrix;
		break;
	case WEAPON_STONEPICK:
		m_fWeaponAtt = 15.f;
		m_fWeaponTorpid = 0.3f;

		// 1인칭 손뼈 위치
		pPlayerMeshCom = static_cast<CDynamicMesh*>(Engine::CManagement::GetInstance()->Get_Component(L"Layer_Player", 0, L"Com_MeshFPV", ID_STATIC));
		pFrame = pPlayerMeshCom->Get_FrameByName("hatchet_jnt");
		m_pFPVParentBoneMatrix = &pFrame->CombinedTransformMatrix;
		break;
	case WEAPON_WOODCLUB:
		m_fWeaponAtt = 5.f;
		m_fWeaponTorpid = 3.f;

		// 1인칭 손뼈 위치
		pPlayerMeshCom = static_cast<CDynamicMesh*>(Engine::CManagement::GetInstance()->Get_Component(L"Layer_Player", 0, L"Com_MeshFPV", ID_STATIC));
		pFrame = pPlayerMeshCom->Get_FrameByName("hatchet_jnt");
		m_pFPVParentBoneMatrix = &pFrame->CombinedTransformMatrix;
		break;
	case WEAPON_SPEAR:
		m_fWeaponAtt = 35.f;
		m_fWeaponTorpid = 0.2f;

		pPlayerMeshCom = static_cast<CDynamicMesh*>(Engine::CManagement::GetInstance()->Get_Component(L"Layer_Player", 0, L"Com_MeshFPV", ID_STATIC));
		pFrame = pPlayerMeshCom->Get_FrameByName("spear_jnt");
		m_pFPVParentBoneMatrix = &pFrame->CombinedTransformMatrix;
		break;
	case WEAPON_BOLA:
		m_fWeaponAtt = 0.f;
		m_fWeaponTorpid = 0.f;

		m_pBoneMatrix = &m_pDMeshTPVCom->Get_FrameByName("bola_rope_root_exp")->CombinedTransformMatrix;

		m_pTransformCom->Set_IsYawPitchRoll(true);
		m_pTransformCom->Set_YawPitchRoll(D3DXToRadian(220.f), D3DXToRadian(90.f), D3DXToRadian(90.f));

		m_pDMeshTPVCom->Set_Animation(1);
		break;
	case WEAPON_BOW:
		m_fWeaponAtt = 10.f;
		m_fWeaponTorpid = 0.2f;

		pFrame = pPlayerMeshCom->Get_FrameByName("c_L_weapon_jnt");
		m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

		m_pBoneMatrix = &m_pDMeshTPVCom->Get_FrameByName("d_bow_root_jnt")->CombinedTransformMatrix;
		
		m_pTransformCom->Set_Rotate(ROT_Z, D3DXToRadian(300.f));


		m_pDMeshTPVCom->Set_Animation(2);
		break;
	}


	return S_OK;
}

_int CWeapon::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	CGameObject::Update_GameObject(_dTimeDelta);

	//_vec3 vPos = *m_pTransformCom->Get_Pos();
	//m_pSphereColliderCom->Set_Pos(vPos);

	return NO_EVENT;
}

_int CWeapon::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CTransform*	pPlayerTransformCom = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	_mat mat, matScale, matTrans;
	
	if (!m_bIsDynamic && m_bIsFPV)
	{
		// 1인칭 무기 위치
		pPlayerTransformCom = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_TransformFPV", ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransformCom, -1);

		m_pTransformCom->Set_Rotate(ROT_X, D3DXToRadian(90.f));

		//if (!m_bIsDynamic)
		//{
		//	mat = *m_pBoneMatrix * (*m_pFPVParentBoneMatrix) * pPlayerTransformCom->Get_WorldMatrix();
		//}
		//else
		//{
			mat = *m_pFPVParentBoneMatrix * pPlayerTransformCom->Get_WorldMatrix();
		//}
	}
	else
	{
		// 3인칭 무기 위치
		pPlayerTransformCom = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransformCom, -1);

		m_pTransformCom->Set_Rotate(ROT_X, D3DXToRadian(0.f));

		if (m_bIsDynamic)
		{
			mat = *m_pBoneMatrix * (*m_pParentBoneMatrix) * pPlayerTransformCom->Get_WorldMatrix();
		}
		else
		{
			mat = *m_pParentBoneMatrix * pPlayerTransformCom->Get_WorldMatrix();
		}
	}

	
	m_pTransformCom->Set_ParentMatrix(&mat);

	_vec3 vPos, vLook;
	memcpy(&vPos, &mat.m[INFO_POS][0], sizeof(_vec3));

	if (WEAPON_SPEAR == m_eWeaponType)
	{
		// 창일 경우 충돌체 위치 위로 옮김
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);

		vPos += vLook * 2.f;
	}

	m_pSphereColliderCom->Set_Pos(vPos);

	// 랜더그룹 추가
	if(m_pPlayer)
		Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	if (m_bIsDynamic)
	{
		m_pDMeshTPVCom->Play_Animation(_dTimeDelta);
	}

	return NO_EVENT;
}

void CWeapon::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	if (m_bIsFPV)
	{
		//if (m_bIsDynamic)
		//	m_pDMeshTPVCom->Render_Meshes(pEffect, false);
		//else
		//	m_pMeshTPVCom->Render_Meshes(pEffect);

		if(!m_bIsDynamic)
			m_pMeshFPVCom->Render_Meshes(pEffect);
	}
	else
	{
		if (m_bIsDynamic)
			m_pDMeshTPVCom->Render_Meshes(pEffect, false);
		else
			m_pMeshTPVCom->Render_Meshes(pEffect);
	}

	pEffect->End();

	Safe_Release(pEffect);

#ifdef _DEBUG
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	wstring wstrEye = L"Weapon Pos		: " + to_wstring(vPos.x) + L" / " + to_wstring(vPos.y) + L" / " + to_wstring(vPos.z);
	Engine::Render_Font(L"Font_Debug_Bold", wstrEye, &_vec2(10.f, 165.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Render_SphereCollider();
#endif // _DEBUG
}

CWeapon * CWeapon::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CWeapon*	pInstance = new CWeapon(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CWeapon::Clone(void * _pArg/* = nullptr*/)
{
	CWeapon*	pClone = new CWeapon(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CWeapon::Free()
{
	if(m_bIsClone)
		Safe_Release(m_pPlayer);

	CGameObject::Free();
}

void CWeapon::Is_Attack(const wstring& _wstrTargetLayerTag, list<CGameObject*>* pListAtackObejct)
{
	CCollisionFunc::AttackOnUnit(m_pPlayer, m_pSphereColliderCom->Get_Pos(), m_pSphereColliderCom->Get_Radius(),_wstrTargetLayerTag, CSphereCollider::COL_HIT, m_eWeaponType, nullptr, pListAtackObejct, true);
}

void CWeapon::Is_FPVAttack(const wstring & _wstrTargetLayerTag, const _vec3 & _vColPos, const _float & _fColRadius)
{
	CCollisionFunc::AttackOnUnit(m_pPlayer, &_vColPos, &_fColRadius, _wstrTargetLayerTag, CSphereCollider::COL_HIT, m_eWeaponType, nullptr, nullptr, true);
}

void CWeapon::Set_WeaponAnimation(const _uint & iIndex)
{
	if (!m_bIsDynamic)
		return;

	m_pDMeshTPVCom->Set_Animation(iIndex);
}

void CWeapon::Set_WeaponYawPitchRoll(_float _fYaw, _float _fPitch, _float _fRoll)
{
	m_pTransformCom->Set_YawPitchRoll(D3DXToRadian(_fYaw), D3DXToRadian(_fPitch), D3DXToRadian(_fRoll));
}

HRESULT CWeapon::Add_Component()
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
	tInfo.fRadius = 2.f;
	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);

	return S_OK;
}

HRESULT CWeapon::Add_MeshComponent()
{
	CComponent*		pComponent = nullptr;

	switch (m_eWeaponType)
	{
	case WEAPON_HATCHET:
		pComponent = m_pMeshTPVCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Hatchet_TPV"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshTPV", pComponent);

		pComponent = m_pMeshFPVCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Hatchet_FPV"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshFPV", pComponent);
		break;
	case WEAPON_STONEPICK:
		pComponent = m_pMeshTPVCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_StonePick_TPV"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshTPV", pComponent);

		pComponent = m_pMeshFPVCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_StonePick_FPV"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshFPV", pComponent);
		break;
	case WEAPON_WOODCLUB:
		pComponent = m_pMeshTPVCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_WoodClub_TPV"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshTPV", pComponent);

		pComponent = m_pMeshFPVCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_WoodClub_FPV"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshFPV", pComponent);
		break;
	case WEAPON_SPEAR:
		pComponent = m_pMeshTPVCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Spear_TPV"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshTPV", pComponent);

		pComponent = m_pMeshFPVCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Spear_FPV"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshFPV", pComponent);
		break;

		// 다이나믹 무기
	case WEAPON_BOLA:
		pComponent = m_pDMeshTPVCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Weapon_Bola_TPV"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshTPV", pComponent);

		m_bIsDynamic = true;
		break;
	case WEAPON_BOW:
		pComponent = m_pDMeshTPVCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Weapon_Bow_TPV"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_MeshTPV", pComponent);

		m_bIsDynamic = true;
		break;
	default:
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CWeapon::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
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
