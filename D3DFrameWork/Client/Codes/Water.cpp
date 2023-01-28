#include "stdafx.h"

#include "Water.h"

#include "Export_Function.h"
#include "StaticMesh.h"
#include "Shader.h"

USING(Engine)

CWater::CWater(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{

}

CWater::CWater(const CWater & rhs)
	: CGameObject(rhs)
{

}

HRESULT CWater::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CWater::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vColor = { 1.f, 1.f, 1.f };
	m_fTime = 0.f;
	m_dCount = 0.f;
	m_dTime = 0;
	m_fScale = 1.f;
	m_fAlpha = 1.f;

	m_pTransformCom->Set_Scale(4.f, 1.f, 2.7f);
	m_pTransformCom->Set_Pos(-450.f, -140.f, 680.f);
	//m_pTransformCom->Set_Pos(-400.f, -86.f, 500.f);
	//m_pTransformCom->Set_Rotate(ROT_X, D3DXToRadian(180.f));

	//m_pTransformCom->Set_Scale(5.f, 5.f, 5.f);
	//m_pTransformCom->Set_Pos(-400.f, -80.f, 500.f);

	return S_OK;
}

_int CWater::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;


	CGameObject::Update_GameObject(_dTimeDelta);

	m_fTime += (_float)_dTimeDelta;

	return NO_EVENT;
}

_int CWater::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return NO_EVENT;
}

void CWater::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

CWater * CWater::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CWater*	pInstance = new CWater(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CWater::Clone(void * _pArg/* = nullptr*/)
{
	CWater*	pClone = new CWater(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CWater::Free()
{
	CGameObject::Free();
}

HRESULT CWater::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CVIBuffer_WaterTerrain*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_WaterTerrain"));
	//pComponent = m_pBufferCom = dynamic_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	//pComponent = m_pBufferCom = dynamic_cast<CVIBuffer_CubeTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Water"));
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_CubeTest"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pNormalTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_WaterNormal2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_TextureSub", pComponent);

	pComponent = m_pNoiseTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_WaterNoise"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_TextureNoise", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Water"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

HRESULT CWater::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat	matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);
	_pEffect->SetVector("g_vSetColor", &_vec4(m_vColor, 1.f));
	_pEffect->SetFloat("g_fNoiseAcc", m_fTime);

	m_pTextureCom->Set_Texture(_pEffect, "g_DiffuseCubeTexture");
	m_pNormalTextureCom->Set_Texture(_pEffect, "g_NormalTexture");
	//m_pNoiseTextureCom->Set_Texture(_pEffect, "g_NoiseTexture");
	//m_pNoiseTextureCom->Set_Texture(_pEffect, "g_DiffuseTexture");

	//Engine::Get_RenderTargetTexture(_pEffect, L"Blend", "g_DiffuseCubeTexture");

	_pEffect->CommitChanges();

	return S_OK;
}
