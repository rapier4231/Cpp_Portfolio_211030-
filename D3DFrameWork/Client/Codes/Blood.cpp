#include "stdafx.h"

#include "Blood.h"

#include "Export_Function.h"
#include "StaticMesh.h"
#include "Shader.h"

USING(Engine)

CBlood::CBlood(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{

}

CBlood::CBlood(const CBlood & rhs)
	: CGameObject(rhs)
{

}

HRESULT CBlood::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBlood::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(_vec3(-25.f, 2.f, -25.f));

	m_vTexUV = { 0.f, 0.f };
	m_dCount = 0.f;
	m_dTime = 0;
	m_iTextureX = 4;
	m_iTextureY = 8;
	m_fScale = 1.f;
	m_fAlpha = 1.f;

	m_vecTexUV.reserve(sizeof(_vec2) * m_iTextureX * m_iTextureY);

	for (_int y = 0; y < m_iTextureY; y++)
	{
		for (_int x = 0; x < m_iTextureX; x++)
		{
			m_vTexUV = { 1.f / m_iTextureX * x, 1.f / m_iTextureY * y };

			m_vecTexUV.emplace_back(m_vTexUV);
		}
	}

	return S_OK;
}

_int CBlood::Update_GameObject(const _double & _dTimeDelta)
{
	if (0.f > m_fAlpha)
		Set_Dead();

	if (m_bDead)
		return OBJECT_DEAD;

	CGameObject::Update_GameObject(_dTimeDelta);

	Compute_ViewZ(m_pTransformCom->Get_Pos());

	if (m_iTextureX * m_iTextureY > m_dCount + 1)
	{
		m_dCount += _dTimeDelta * 50.f;
		m_fScale += 0.03f;
	}

	// 가끔 알탭같은거 하고나면 dTimeDelta 값 오지게 커져서 에러 터지는거 방지용
	if (m_iTextureX * m_iTextureY <= m_dCount + 1)
	{
		m_dCount = m_iTextureX * m_iTextureY - 1;
	}

	if(16 < m_dCount)
		m_fAlpha -= (_float)_dTimeDelta * 2.f;

	m_pTransformCom->Set_Scale(_vec3(m_fScale, m_fScale, m_fScale));

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

	m_vColor = { 1.f, 0.f, 0.f };

	return NO_EVENT;
}

_int CBlood::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_EFFECT, this);

	return NO_EVENT;
}

void CBlood::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(3);
	pEffect->CommitChanges();

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

CBlood * CBlood::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CBlood*	pInstance = new CBlood(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CBlood::Clone(void * _pArg/* = nullptr*/)
{
	CBlood*	pClone = new CBlood(*this);

	if (FAILED(pClone->Ready_GameObject()))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CBlood::Free()
{
	CGameObject::Free();
}

HRESULT CBlood::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Blood"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

HRESULT CBlood::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
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
	_pEffect->SetValue("g_vTexUV", &m_vecTexUV[(_uint)m_dCount], sizeof(_vec2));

	m_pTextureCom->Set_Texture(_pEffect, "g_DiffuseTexture", 0);
	m_pTextureCom->Set_Texture(_pEffect, "g_NormalTexture", 1);

	Engine::Get_RenderTargetTexture(_pEffect, L"Depth", "g_DepthTexture");

	return S_OK;
}
