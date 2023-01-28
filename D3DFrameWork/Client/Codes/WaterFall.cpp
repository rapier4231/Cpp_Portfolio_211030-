#include "stdafx.h"

#include "WaterFall.h"

#include "Export_Function.h"

USING(Engine)

CWaterFall::CWaterFall(LPDIRECT3DDEVICE9 _pDevice)
	: CUnit(_pDevice)
{
	ZeroMemory(m_vBoundingBox, sizeof(_vec3) * 8);
	ZeroMemory(m_vBoundingCenter, sizeof(_vec3));
}

CWaterFall::CWaterFall(const CWaterFall & rhs)
	: CUnit(rhs)
	, m_fBoundingRadius(rhs.m_fBoundingRadius)
	, m_vBoundingCenter(rhs.m_vBoundingCenter)
{
	memcpy(m_vBoundingBox, rhs.m_vBoundingBox, sizeof(_vec3) * 8);
}

HRESULT CWaterFall::Ready_GameObject_Prototype()
{
	//CUnit::Ready_GameObject_Prototype();	//CUnit 범용 셰이더와 다른 특수한 셰이더 사용
	CComponent*		pComponent = nullptr;

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Water"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pDissolveTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_DissolveTexture", pComponent);

	return S_OK;
}

HRESULT CWaterFall::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CUnit::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//메시를 받은 후 경계상자(로컬) 작성
	D3DXComputeBoundingSphere(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), sizeof(_vec3), &m_vBoundingCenter, &m_fBoundingRadius);

	//스케일 곱해서 월드로 변환
	m_pTransformCom->Update_Component(0);
	D3DXVec3TransformCoord(&m_vBoundingCenter, &m_vBoundingCenter, &m_pTransformCom->Get_WorldMatrix());
	m_fBoundingRadius *= Get_Scale().x;

	////메시를 받은 후 경계상자(로컬) 작성
	//_vec3 vMin, vMax;
	//D3DXComputeBoundingBox(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), sizeof(_vec3), &vMin, &vMax);
	//m_vBoundingBox[0] = _vec3(vMin.x, vMin.y, vMin.z);	//	- - -
	//m_vBoundingBox[1] = _vec3(vMax.x, vMin.y, vMin.z);	//	+ - -
	//m_vBoundingBox[2] = _vec3(vMin.x, vMax.y, vMin.z);	//	- + -
	//m_vBoundingBox[3] = _vec3(vMax.x, vMax.y, vMin.z);	//	+ + -
	//m_vBoundingBox[4] = _vec3(vMin.x, vMin.y, vMax.z);	//	- - +
	//m_vBoundingBox[5] = _vec3(vMax.x, vMin.y, vMax.z);	//	+ - +
	//m_vBoundingBox[6] = _vec3(vMin.x, vMax.y, vMax.z);	//	- + +
	//m_vBoundingBox[7] = _vec3(vMax.x, vMax.y, vMax.z);	//	+ + +

	////경계상자(로컬)을 월드로 변환해 저장
	//m_pTransformCom->Update_Component(0);
	//D3DXVec3TransformCoordArray(m_vBoundingBox, sizeof(_vec3), m_vBoundingBox, sizeof(_vec3), &m_pTransformCom->Get_WorldMatrix(), 8);

	m_vColor = { 1.f, 1.f, 1.f };

	m_pTransformCom->Set_Scale(0.2f, 0.2f, 0.2f);
	m_pTransformCom->Set_Pos(-550.f, -220.f, 850.f);
	//Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(30.f));

	return S_OK;
}

_int CWaterFall::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;
	
	CUnit::Update_GameObject(_dTimeDelta);

	m_fNoiseAcc += (_float)_dTimeDelta;

	return NO_EVENT;
}

_int CWaterFall::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	//렌더그룹 추가 : 경계구체가 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	if (Engine::Check_Sphere(m_vBoundingCenter, m_fBoundingRadius))
	{
		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
		Set_ShadowMap(Engine::Get_ShadowLight().vLightPos, Engine::Get_ShadowLight().vLightAt);
	}

	//Engine::Add_RenderGroup(RENDER_SHADOWMAP, this);

	return NO_EVENT;
}

void CWaterFall::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect, false, 1);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CWaterFall::Render_GameObjectShadowMap()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowMapCom->Get_EffectHandle();
	pEffect->AddRef();

	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_MeshesShadowMap(pEffect);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

void CWaterFall::Render_GameObjectShadow()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowCom->Get_EffectHandle();
	pEffect->AddRef();

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
	pEffect->SetBool("g_bTerrain", true);

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_MeshesShadow(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CWaterFall * CWaterFall::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CWaterFall*	pInstance = new CWaterFall(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CWaterFall::Clone(void * _pArg/* = nullptr*/)
{
	CWaterFall*	pClone = new CWaterFall(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CWaterFall::Free()
{
	CUnit::Free();
}

HRESULT CWaterFall::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_WaterFall"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pNoiseTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_WaterFallNoise"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_NoiseTexture", pComponent);

	pComponent = m_pGradientTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_WaterFallGradient"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_GradientTexture", pComponent);

	return S_OK;
}

HRESULT CWaterFall::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat matWorld, matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);
	_pEffect->SetVector("g_vSetColor", &_vec4(m_vColor, 1.f));
	_pEffect->SetFloat("g_fNoiseAcc", m_fNoiseAcc);

	m_pNoiseTextureCom->Set_Texture(_pEffect, "g_NoiseTexture");
	m_pGradientTextureCom->Set_Texture(_pEffect, "g_GradientTexture");

	_pEffect->CommitChanges();

	return S_OK;
}