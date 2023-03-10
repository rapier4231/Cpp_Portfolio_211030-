#include "stdafx.h"

#include "Dust.h"

#include "Export_Function.h"
#include "StaticMesh.h"
#include "Shader.h"

USING(Engine)

CDust::CDust(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{

}

CDust::CDust(const CDust & rhs)
	: CGameObject(rhs)
{

}

HRESULT CDust::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDust::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Set_Pos(_vec3(-25.f, 2.f, -25.f));

	m_vColor = { 1.f, 1.f, 1.f };
	m_vTexUV = { 0.f, 0.f };
	m_dCount = 0.f;
	m_dTime = 0;
	//m_iTextureX = 4;
	//m_iTextureY = 4;
	m_iTextureX = 1;
	m_iTextureY = 1;
	m_fScale = *(_float*)_pArg;
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

	m_pBufferCom->SetAni(1);

	return S_OK;
}

_int CDust::Update_GameObject(const _double & _dTimeDelta)
{
	if (0.f > m_fAlpha)
		Set_Dead();

	if (m_bDead)
		return OBJECT_DEAD;

	m_pTransformCom->Set_Scale(_vec3(m_fScale, m_fScale, m_fScale));

	CGameObject::Update_GameObject(_dTimeDelta);

	Compute_ViewZ(m_pTransformCom->Get_Pos());

	if (m_iTextureX * m_iTextureY > m_dCount + 1)
	{
		m_dCount += _dTimeDelta * 10.f;
		m_fScale += 0.05f;
	}

	// 가끔 알탭같은거 하고나면 dTimeDelta 값 오지게 커져서 에러 터지는거 방지용
	if (m_iTextureX * m_iTextureY <= m_dCount + 1)
	{
		m_dCount = m_iTextureX * m_iTextureY - 1;
	}

	//if(8 < m_dCount)
		m_fAlpha -= (_float)_dTimeDelta * 2.f/*(_float)1 / (m_iTextureX * m_iTextureY)*/;

	m_pBufferCom->SetAvgTime(2.f);
	m_pBufferCom->Update_Particle((_float)_dTimeDelta);

	m_bUpdated = true;

	return NO_EVENT;
}

_int CDust::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_EFFECT, this);

	return NO_EVENT;
}

void CDust::Render_GameObject()
{
	if (!m_bUpdated)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);
	pEffect->CommitChanges();

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

CDust * CDust::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDust*	pInstance = new CDust(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDust::Clone(void * _pArg/* = nullptr*/)
{
	CDust*	pClone = new CDust(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CDust::Free()
{
	CGameObject::Free();
}

HRESULT CDust::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CVIBuffer_ParticleTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_ParticleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Dust"));
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

HRESULT CDust::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
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
	_pEffect->SetValue("g_vTexUV", &_vec2(0.f, 0.f), sizeof(_vec2));
	//_pEffect->SetValue("g_vTexUV", &m_vecTexUV[(_uint)m_dCount], sizeof(_vec2));

	m_pTextureCom->Set_Texture(_pEffect, "g_DiffuseTexture", 0);

	Engine::Get_RenderTargetTexture(_pEffect, L"Depth", "g_DepthTexture");

	return S_OK;
}
