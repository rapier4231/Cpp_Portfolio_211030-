#include "stdafx.h"
#include "..\Headers\ShadowPosTest2.h"

#include "Export_Function.h"

USING(Engine)


CShadowPosTest2::CShadowPosTest2(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CShadowPosTest2::CShadowPosTest2(const CShadowPosTest2 & _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CShadowPosTest2::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CShadowPosTest2::Ready_GameObject(void * _pArg)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vColor = { 0.f, 0.f, 0.f };
	m_vTexUV = { 0.f, 0.1f };
	m_dCount = 0.f;
	m_dTime = 0.f;
	m_iTextureX = 1;
	m_iTextureY = 1;
	m_fScale = 1.f;
	m_fAlpha = 0.7f;

	m_pTransformCom->Set_Scale(_vec3(m_fScale, m_fScale, 0.001f));

	m_iCount = 0;

	return S_OK;
}

_int CShadowPosTest2::Update_GameObject(const _double & _dTimeDelta)
{
	m_pTransformCom->Set_Scale(_vec3(m_fScale, m_fScale, 0.001f));
	m_pTransformCom->Set_AccRotate(ROT_Y, (_float)_dTimeDelta * 3.f);

	_vec3 vTargetPos = Engine::Get_ShadowLight().vLightAt;
	m_pTransformCom->Set_Pos(vTargetPos);
	
	CGameObject::Update_GameObject(_dTimeDelta);

	Compute_ViewZ(m_pTransformCom->Get_Pos());

	return NO_EVENT;
}

_int CShadowPosTest2::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return NO_EVENT;
}

void CShadowPosTest2::Render_GameObject()
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

CShadowPosTest2 * CShadowPosTest2::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CShadowPosTest2*	pInstance = new CShadowPosTest2(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CShadowPosTest2::Clone(void * _pArg/* = nullptr*/)
{
	CShadowPosTest2*	pClone = new CShadowPosTest2(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

HRESULT CShadowPosTest2::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_EffectObjectFunnel"));
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

HRESULT CShadowPosTest2::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
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

void CShadowPosTest2::Free()
{
	CGameObject::Free();
}
