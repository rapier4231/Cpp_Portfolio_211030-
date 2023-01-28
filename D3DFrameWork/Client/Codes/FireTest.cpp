#include "stdafx.h"

#include "FireTest.h"

#include "Export_Function.h"
#include "StaticMesh.h"
#include "Shader.h"

USING(Engine)

CFireTest::CFireTest(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{

}

CFireTest::CFireTest(const CFireTest & rhs)
	: CGameObject(rhs)
{

}

HRESULT CFireTest::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CFireTest::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vColor = { 1.5f, 1.f, 1.f };
	m_vTexUV = { 0.f, 0.f };
	m_dCount = 0.f;
	m_dTime = 0;
	m_iTextureX = 1;
	m_iTextureY = 1;
	m_fScale = 1.f;
	m_fAlpha = 1.f;

	m_bIgnition = true;

	return S_OK;
}

_int CFireTest::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	m_pTransformCom->Set_Scale(1.f, 1.5f, 1.f);

	CGameObject::Update_GameObject(_dTimeDelta);

	m_vTexUV.y += (_float)_dTimeDelta;

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

	m_bUpdated = true;

	return NO_EVENT;
}

_int CFireTest::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_EFFECT, this);

	return NO_EVENT;
}

void CFireTest::Render_GameObject()
{
	if (!m_bUpdated)
		return;

	if (m_bIgnition)
	{
		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);
		pEffect->AddRef();

		FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

		_uint iPassMax = 0;

		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(2);
		pEffect->CommitChanges();

		m_pBufferCom->Render_VIBuffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

CFireTest * CFireTest::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CFireTest*	pInstance = new CFireTest(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CFireTest::Clone(void * _pArg/* = nullptr*/)
{
	CFireTest*	pClone = new CFireTest(*this);

	if (FAILED(pClone->Ready_GameObject()))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CFireTest::Free()
{
	CGameObject::Free();
}

HRESULT CFireTest::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_FireTest"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pNoiseTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_FireNoise"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_TextureNoise", pComponent);

	pComponent = m_pSubTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_FireSub"));
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

HRESULT CFireTest::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
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
	_pEffect->SetValue("g_vTexUV", &m_vTexUV, sizeof(_vec2));

	m_pTextureCom->Set_Texture(_pEffect, "g_DiffuseTexture");
	m_pNoiseTextureCom->Set_Texture(_pEffect, "g_NoiseTexture");
	m_pSubTextureCom->Set_Texture(_pEffect, "g_SubTexture");

	Engine::Get_RenderTargetTexture(_pEffect, L"Depth", "g_DepthTexture");

	return S_OK;
}
