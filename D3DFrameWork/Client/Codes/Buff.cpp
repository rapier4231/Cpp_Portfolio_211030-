#include "stdafx.h"

#include "Buff.h"

#include "Export_Function.h"
#include "StaticMesh.h"
#include "Shader.h"

USING(Engine)

CBuff::CBuff(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{

}

CBuff::CBuff(const CBuff & rhs)
	: CGameObject(rhs)
{

}

HRESULT CBuff::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBuff::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vColor = { 1.f, 0.518983f, 0.f };
	m_vTexUV = { 0.f, 0.f };
	m_dCount = 0.f;
	m_dTime = 0;
	m_iTextureX = 1;
	m_iTextureY = 1;

	if(_pArg)
		m_fDefaultScale = *(_float*)_pArg;
	else
		m_fDefaultScale = 4.f;

	m_fScale = m_fDefaultScale;
	m_fAlpha = 1.f;

	return S_OK;
}

_int CBuff::Update_GameObject(const _double & _dTimeDelta)
{
	//if (0.f > m_fAlpha)
	//	Set_Dead();

	if (m_bDead)
		return OBJECT_DEAD;

	if (0.f > m_fAlpha)
	{
		m_fAlpha = 1.f;
		m_fScale = m_fDefaultScale;
	}

	else
	{
		m_fAlpha -= (_float)_dTimeDelta;
		m_fScale += 0.1f;
	}

	m_pTransformCom->Set_Scale(_vec3(m_fScale, m_fScale, m_fScale));

	CGameObject::Update_GameObject(_dTimeDelta);

	Compute_ViewZ(m_pTransformCom->Get_Pos());

	_mat	matWorld, matView, matBill;
	matWorld = m_pTransformCom->Get_WorldMatrix();

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._12 = matView._12;
	matBill._13 = matView._13;
	matBill._21 = matView._21;
	matBill._22 = matView._22;
	matBill._23 = matView._23;
	matBill._31 = matView._31;
	matBill._32 = matView._32;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

	m_dTime += _dTimeDelta;

	m_bUpdated = true;

	return NO_EVENT;
}

_int CBuff::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_EFFECT, this);

	return NO_EVENT;
}

void CBuff::Render_GameObject()
{
	if (!m_bUpdated)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(5);
	pEffect->CommitChanges();

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTableSub(pEffect), );

	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(5);
	pEffect->CommitChanges();

	m_pBufferSubCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

CBuff * CBuff::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CBuff*	pInstance = new CBuff(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CBuff::Clone(void * _pArg/* = nullptr*/)
{
	CBuff*	pClone = new CBuff(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CBuff::Free()
{
	CGameObject::Free();
}

HRESULT CBuff::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pBufferSubCom = dynamic_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BufferSub", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_BuffRing"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pTextureSubCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_BuffEffect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_TextureSub", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

HRESULT CBuff::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat	matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);

	_pEffect->SetFloat("g_fAlpha", m_fAlpha);
	_pEffect->SetFloat("g_fTime",(_float) m_dTime);
	_pEffect->SetInt("g_iTextureX", m_iTextureX);
	_pEffect->SetInt("g_iTextureY", m_iTextureY);
	_pEffect->SetValue("g_vObjColor", &m_vColor, sizeof(_vec3));
	_pEffect->SetValue("g_vTexUV", &_vec2(0.f, 0.f), sizeof(_vec2));

	m_pTextureCom->Set_Texture(_pEffect, "g_DiffuseTexture", 0);
	m_pTextureCom->Set_Texture(_pEffect, "g_NoiseTexture", 1);

	Engine::Get_RenderTargetTexture(_pEffect, L"Depth", "g_DepthTexture");

	return S_OK;
}

HRESULT CBuff::SetUp_ConstantTableSub(LPD3DXEFFECT & _pEffect)
{
	_mat	matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);

	_pEffect->SetFloat("g_fAlpha", m_fAlpha);
	_pEffect->SetFloat("g_fTime", (_float)m_dTime);
	_pEffect->SetInt("g_iTextureX", m_iTextureX);
	_pEffect->SetInt("g_iTextureY", m_iTextureY);
	_pEffect->SetValue("g_vObjColor", &m_vColor, sizeof(_vec3));
	_pEffect->SetValue("g_vTexUV", &_vec2(0.f, 0.f), sizeof(_vec2));

	m_pTextureSubCom->Set_Texture(_pEffect, "g_DiffuseTexture", 0);
	m_pTextureSubCom->Set_Texture(_pEffect, "g_NoiseTexture", 1);

	Engine::Get_RenderTargetTexture(_pEffect, L"Depth", "g_DepthTexture");

	return S_OK;
}
