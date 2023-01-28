#include "stdafx.h"
#include "Harvestable_Pebble.h"

#include "Export_Function.h"
#include "Inven.h"
#include "LR_Inventory.h"

#include "QuickSlot_UI.h"

USING(Engine)

CHarvestable_Pebble::CHarvestable_Pebble(LPDIRECT3DDEVICE9 _pDevice)
	: CHarvestable(_pDevice)
{
}

CHarvestable_Pebble::CHarvestable_Pebble(const CHarvestable_Pebble & rhs)
	: CHarvestable(rhs)
{
}

HRESULT CHarvestable_Pebble::Ready_GameObject_Prototype()
{
	CHarvestable::Ready_GameObject_Prototype();

	//m_iID = UNITID_PEBBLE;

	m_tStat.fHp = 10.f;

	return S_OK;
}

HRESULT CHarvestable_Pebble::Ready_GameObject(void * _pArg)
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

	return S_OK;
}

_int CHarvestable_Pebble::Update_GameObject(const _double & _dTimeDelta)
{
	return CHarvestable::Update_GameObject(_dTimeDelta);
}

_int CHarvestable_Pebble::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	//그림자 렌더그룹 추가 : 경계구체가 빛 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	//if (Engine::Check_Sphere(m_vBoundingCenter, m_fBoundingRadius, CULL_LIGHT))
	//{
	//	g_bInstancing ? 
	//		Engine::Add_Instance(m_wstrMeshName, RENDER_SHADOWMAP, m_pTransformCom->Get_WorldMatrix()) :
	//		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	//}

	//렌더그룹 추가 : 경계구체가 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	if (Engine::Check_Sphere(m_vBoundingCenter, m_fBoundingRadius))
	{
		g_bInstancing ?
			Engine::Add_Instance(m_wstrMeshName, RENDER_NONALPHA, m_pTransformCom->Get_WorldMatrix()) :
			Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	}

	return NO_EVENT;
}

void CHarvestable_Pebble::Render_GameObject()
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
}

void CHarvestable_Pebble::Setup_Instance(LPD3DXEFFECT & pEffect)
{
	// 상수테이블 세팅
	_mat matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	m_pDissolveTextureCom->Set_Texture(pEffect, "g_DissolveTexture");

	//pEffect->SetFloat("g_fAmount", m_fDissolveAmount);	//<- 인스턴스별로 다른 데이터 : 인스턴스데이터로 옮겨야함

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	//m_pMeshCom->Render_Meshes(pEffect, m_bDissolve);
	m_pMeshCom->Render_Meshes(pEffect, false);

	pEffect->End();
}

void CHarvestable_Pebble::Render_GameObjectShadowMap()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowMapCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Render_MeshesShadowMap(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);
}

void CHarvestable_Pebble::Render_GameObjectShadow()
{
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
	Engine::Get_RenderTargetTexture(pEffect, L"ShadowBlurY", "g_ShadowBlurTexture");
	pEffect->SetBool("g_bTerrain", false);

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Render_MeshesShadow(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);
}

CHarvestable_Pebble * CHarvestable_Pebble::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CHarvestable_Pebble*	pInstance = new CHarvestable_Pebble(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CHarvestable_Pebble::Clone(void * _pArg)
{
	CHarvestable_Pebble*	pClone = new CHarvestable_Pebble(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CHarvestable_Pebble::Free()
{
	CHarvestable::Free();
}

_int CHarvestable_Pebble::Check_Arg(void * _pArg)
{
	if (_pArg)
	{
		if (dynamic_cast<OBJINFO*>((OBJINFO*)_pArg))
		{
			OBJINFO tInfo;
			memcpy(&tInfo, _pArg, sizeof(OBJINFO));

			//dwOption으로 받은 MeshID값에따라 넣을메쉬 판단
			TCHAR szText[MAX_PATH];
			Engine::Get_MeshTag_Pebble(szText, tInfo.dwOption);

			if (!szText || E_FAIL == Init_Mesh(szText))
			{
				ERR_MSG(L"CHarvestable_Pebble : Failed to Find MeshTag");
				return -1;
			}

			Set_Scale(tInfo.vScale);
			Set_Rot(tInfo.vRot.x, tInfo.vRot.y, tInfo.vRot.z);
			Set_Pos(tInfo.vPos);
			m_vColor = tInfo.vColor;

			//wstring strTagDM = szText + wstring(L"_DM");
			//if (E_FAIL == Init_DynamicMesh(szText, tInfo.vScale, tInfo.vRot, tInfo.vPos))
			//{
			//	ERR_MSG(L"CHarvestable_Bush : Failed to Find MeshTag_DM");
			//	return -1;
			//}

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
		Init_Mesh(L"Proto_StaticMesh_Pebble_A");
	}
	return 0;
}

HRESULT CHarvestable_Pebble::Add_Component(_ulong dwOption)
{
	CComponent*		pComponent = nullptr;

	//pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Pebble_A"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	/// 충돌체
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	tInfo.fRadius = 0.8f;
	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);
	
	//충돌체 월드위치 세팅
	m_pSphereColliderCom->Set_Pos(Get_Pos());

	return S_OK;
}

_bool CHarvestable_Pebble::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	if (WEAPON_HAND_HARVEST != dwAttackType)
		return false;

	_bool bHit = CHarvestable::HitBy(pAttacker, dwAttackType, vCollisionPos);

	//별도 출력이펙트 없음

	static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_Stone, 1);
	static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(1.f);
	Set_Dead();

	return bHit;
}