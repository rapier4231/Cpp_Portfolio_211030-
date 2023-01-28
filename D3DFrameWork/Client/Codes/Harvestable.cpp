#include "stdafx.h"
#include "..\Headers\Dino.h"

#include "Export_Function.h"
#include "SphereCollider.h"
#include "..\Headers\Harvestable.h"
#include "LR_Inventory.h"
USING(Engine)

CHarvestable::CHarvestable(LPDIRECT3DDEVICE9 _pDevice)
	: CUnit(_pDevice)
{
	ZeroMemory(m_vBoundingCenter, sizeof(_vec3));
	ZeroMemory(m_vBoundingBox, sizeof(_vec3) * 8);
}

CHarvestable::CHarvestable(const CHarvestable & rhs)
	: CUnit(rhs)
	, m_pMeshCom(rhs.m_pMeshCom)
	, m_pSphereColliderCom(rhs.m_pSphereColliderCom)
	, m_fBoundingRadius(rhs.m_fBoundingRadius)
	, m_vBoundingCenter(rhs.m_vBoundingCenter)
{
	memcpy(m_vBoundingBox, rhs.m_vBoundingBox, sizeof(_vec3) * 8);
}

HRESULT CHarvestable::Ready_GameObject_Prototype()
{
	CUnit::Ready_GameObject_Prototype();

	CHarvestable::Add_Component_Prototype();

	return S_OK;
}

HRESULT CHarvestable::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CUnit::Ready_GameObject(_pArg);	//Ʈ������ ȹ��

	CHarvestable::Add_Component();

	return S_OK;
}

_int CHarvestable::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	CUnit::Update_GameObject(_dTimeDelta);

	//////////////////////////////////////////////////////////////////////////
	//�׽�Ʈ
	//_vec3 vPos = *m_pTransformCom->Get_Pos();
	//if(m_pSphereColliderCom)
	//	m_pSphereColliderCom->Set_Pos(_vec3(vPos.x, vPos.y, vPos.z));
	//////////////////////////////////////////////////////////////////////////
	return NO_EVENT;
}

_int CHarvestable::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Set_ShadowMap(Engine::Get_ShadowLight().vLightPos, Engine::Get_ShadowLight().vLightAt);

	return NO_EVENT;
}

void CHarvestable::Render_GameObject()
{
	//CUnit::Render_GameObject();

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

#endif // !_DEBUG

}

CGameObject * CHarvestable::Clone(void * _pArg/* = nullptr*/)
{
	//CHarvestable*	pClone = new CHarvestable(*this);
	//
	//if (FAILED(pClone->Ready_GameObject()))
	//	Safe_Release(pClone);
	//
	//return pClone;
	return nullptr;
}

void CHarvestable::Free()
{
	CUnit::Free();
}

HRESULT CHarvestable::Add_Component_Prototype()
{
	return S_OK;
}

HRESULT CHarvestable::Add_Component()
{
	return S_OK;
}

HRESULT CHarvestable::Init_Mesh(wstring strMeshTag)
{
	if (m_pMeshCom || !m_pTransformCom)
		return E_FAIL;

	//�޽��±׸� TCHAR�� �ٲ� �� �ش��ϴ� �޽� �˻��� ����
	TCHAR szMeshName[MAX_PATH] = L"";
	wsprintf(szMeshName, strMeshTag.c_str());
	m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(szMeshName));
	if (!m_pMeshCom)
	{
		ERR_MSG(L"CHarvestable : MeshTag not found");
		return E_FAIL;
	}
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);
	
	//�ν��Ͻ� �� �±׷� ����� �� �ֵ��� �޽� �̸� ����
	m_wstrMeshName = strMeshTag;

	return S_OK;
}

HRESULT CHarvestable::Init_DynamicMesh(wstring strMeshTag, _vec3 vScale, _vec3 vRot, _vec3 vPos)
{
	if (m_pDynamicMeshCom)
		return E_FAIL;

	TCHAR szMeshName[MAX_PATH] = L"";
	wsprintf(szMeshName, strMeshTag.c_str());

	m_pDynamicMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(szMeshName));
	if (!m_pDynamicMeshCom)
	{
		ERR_MSG(L"CHarvestable : MeshTag(DM) not found");
		return E_FAIL;
	}

	m_mapComponent[ID_STATIC].emplace(L"Com_DynamicMesh", m_pDynamicMeshCom);

	// <<������>>


	return S_OK;
}

_bool CHarvestable::Add_RenderGroup_Culling(RENDERID eID, CGameObject * pGameObject)
{
	return _bool();
}

_bool CHarvestable::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	// �⺻ �ǰ�ó�� //////////////////////////////////////////////////////////////////////////
	if (Is_Invincible (pAttacker))
		return false;
	Add_Invincible(pAttacker);

	// �ǰ�����Ʈó�� //////////////////////////////////////////////////////////////////////////
	vCollisionPos; //���⼭ ó���� ���� ������ ������ü���� ����Ʈ�� �ٸ��Ƿ� ���� �ڽİ�ü���� ó�� ����

	// HP ���� //////////////////////////////////////////////////////////////////////////
	_float fAttackPoint = pAttacker->Get_Stat(STAT_ATTACK);
	m_tStat.fHp -= fAttackPoint;
	if (m_tStat.fHp <= 0.f)
		m_bDying = true;

	//pAttacker->Add_Item(Remove_Item(ITEMID::ITEMID_Meat, pAttacker->Get_Stat(STAT_ATTACK)));

	/// ???
	//static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Set_InvenOwner(this);
	//static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Match_ItemTex_Info();
	//static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Match_ItemTex_Info();
	return true;
}
