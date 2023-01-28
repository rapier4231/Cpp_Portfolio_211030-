//////////////////////////////////////////////////////////////////////////
//	현재 사용하지 않음(CTile로 전용함)										//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\Headers\Terrain.h"

#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CTerrain::CTerrain(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain::Ready_GameObject_Prototype()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

HRESULT CTerrain::Ready_GameObject(void* _pArg/* = nullptr*/)
{
	
	return S_OK;
}

_int CTerrain::Update_GameObject(const _double & _dTimeDelta)
{
	CGameObject::Update_GameObject(_dTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CTerrain::Render_GameObject()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//m_pBufferCom->Copy_Indices(m_pIndex, m_dwTriCnt);

	LPD3DXEFFECT		pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();
	
	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );
	
	_uint iPassMax = 0;
	
	pEffect->Begin(&iPassMax, 0);	// 1인자 : 현재 쉐이더 파일이 갖고 있는 최대 pass개수, 2인자 : 시작 기본 값
	pEffect->BeginPass(0);
	
	m_pBufferCom->Render_VIBuffer();
	
	pEffect->EndPass();
	pEffect->End();
	
	Safe_Release(pEffect);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CTerrain::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	// 버퍼
	pComponent = m_pBufferCom = static_cast<CVIBuffer_Terrain*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// 텍스처
	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);
	
	//// 필터
	//pComponent = m_pFilterCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Filter"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Filter", pComponent);
	//// 필터
	//pComponent = m_pAuraCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Aura"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Aura", pComponent);
	//
	// 트랜스폼
	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// 쉐이더
	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CTerrain::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat			matWorld, matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(_pEffect, "g_BaseTexture", 0);
	//m_pTextureCom->Set_Texture(_pEffect, "g_BaseTexture1", 1);
	//m_pFilterCom->Set_Texture(_pEffect, "g_FilterTexture", 0);

	_pEffect->CommitChanges();

	//D3DMATERIAL9		tMtrl;			// 메쉬를 사용할 경우 재질의 정보들은 거의 대부분 default 값인 1을 넣은 상태로 사용
	//ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));
	//
	//tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	//tMtrl.Power = 20.f;

	//_pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrl.Diffuse);
	//_pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrl.Specular);
	//_pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrl.Ambient);

	//_pEffect->SetFloat("g_fPower", tMtrl.Power);

	float	fDetail = 20.f;
	_pEffect->SetFloat("g_fDetail", fDetail);


	//const D3DLIGHT9*		pLightInfo = Engine::Get_Light(0);
	//NULL_CHECK_RETURN(pLightInfo, S_OK);
	//
	//_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	//
	//_pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//_pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	//_pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	//
	//D3DXMatrixInverse(&matView, NULL, &matView);
	//_pEffect->SetVector("g_vCamPos", (_vec4*)&matView._41);


	return S_OK;
}

HRESULT CTerrain::SetUp_Material(void)
{
	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 0.f;

	m_pDevice->SetMaterial(&tMtrl);

	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CTerrain*	pInstance = new CTerrain(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CTerrain::Clone(void* _pArg/* = nullptr*/)
{
	CTerrain*	pClone = new CTerrain(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CTerrain::Free()
{
	CGameObject::Free();
}
