#include "stdafx.h"
#include "..\Headers\EffectObject.h"

#include "Export_Function.h"

USING(Engine)


CEffectObject::CEffectObject(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CEffectObject::CEffectObject(const CEffectObject & _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CEffectObject::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CEffectObject::Ready_GameObject(void * _pArg)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vColor = { 1.f, 1.f, 1.f };
	m_vTexUV = { 0.f, 0.1f };		// 0.4f ?׸?
	m_dCount = 0.f;
	m_dTime = 0.f;
	m_iTextureX = 1;
	m_iTextureY = 1;
	m_fScale = 0.f;
	m_fAlpha = 0.7f;

	m_pTransformCom->Set_Scale(_vec3(m_fScale, 1.f, m_fScale));

	m_iCount = 0;

	return S_OK;
}

_int CEffectObject::Update_GameObject(const _double & _dTimeDelta)
{
	if (0.01f < m_fScale)
	{
		m_fScale = 0.f;
		m_fAlpha = 0.7f;
		m_iCount++;
	}

	m_fScale += 0.01f * (_float)_dTimeDelta;
	m_fAlpha -= 0.8f * (_float)_dTimeDelta;

	if (1 < m_iCount)
		return OBJECT_DEAD;

	m_pTransformCom->Set_Scale(_vec3(m_fScale, 1.f, m_fScale));
	m_pTransformCom->Set_AccRotate(ROT_Y, (_float)_dTimeDelta * 3.f);

	if (m_pTarget != nullptr)
	{
		_vec3 vTargetPos = *static_cast<CTransform*>(m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos();
		m_pTransformCom->Set_Pos(vTargetPos);
	}

	CGameObject::Update_GameObject(_dTimeDelta);

	Compute_ViewZ(m_pTransformCom->Get_Pos());

	return NO_EVENT;
}

_int CEffectObject::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_EFFECT, this);

	return NO_EVENT;
}

void CEffectObject::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect, false, 4);

	pEffect->End();

	Safe_Release(pEffect);
}

CEffectObject * CEffectObject::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CEffectObject*	pInstance = new CEffectObject(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CEffectObject::Clone(void * _pArg/* = nullptr*/)
{
	CEffectObject*	pClone = new CEffectObject(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

HRESULT CEffectObject::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// ?Ž? ??????Ʈ
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_EffectObject"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

HRESULT CEffectObject::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat	matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);

	_pEffect->SetFloat("g_fAlpha", m_fAlpha);
	_pEffect->SetInt("g_iTextureX", m_iTextureX);
	_pEffect->SetInt("g_iTextureY", m_iTextureY);
	_pEffect->SetValue("g_vObjColor", &m_vColor, sizeof(_vec3));
	//_pEffect->SetValue("g_vTexUV", &m_vecTexUV[(_uint)m_dCount], sizeof(_vec2));
	_pEffect->SetValue("g_vTexUV", &m_vTexUV, sizeof(_vec2));

	Engine::Get_RenderTargetTexture(_pEffect, L"Depth", "g_DepthTexture");

	return S_OK;
}

void CEffectObject::Free()
{
	CGameObject::Free();
}
