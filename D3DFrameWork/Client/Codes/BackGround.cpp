#include "stdafx.h"
#include "BackGround.h"

#include "Export_Function.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBackGround::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBackGround::Ready_GameObject(void* _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	CComponent*		pComponent = nullptr;

	// VIBuffer 컴포넌트
	pComponent = m_pVIBuffer = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// 텍스처
	pComponent = m_pTexture = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform 컴포넌트
	pComponent = m_pTransform = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// 쉐이더
	pComponent = m_pShader = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Sample"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

_int CBackGround::Update_GameObject(const _double & _dTimeDelta)
{
	CGameObject::Update_GameObject(_dTimeDelta);

	D3DXMatrixOrthoLH(&m_matScreenProj, WINCX, WINCY, 0.f, 1.f);

	Engine::Add_RenderGroup(RENDER_BACKUI, this);

	return _int();
}

void CBackGround::Render_GameObject()
{
	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);	// 1인자 : 현재 쉐이더 파일이 갖고 있는 최대 pass개수, 2인자 : 시작 기본 값
	pEffect->BeginPass(1);

	m_pVIBuffer->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CBackGround::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_mat	matWorld, matView;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = WINCX;
	matWorld._22 = WINCY;
	matWorld._33 = 1.f;

	matWorld._41 = 0.f;
	matWorld._42 = 0.f;

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matScreenProj);
	pEffect->SetFloat("g_fAlpha", 1.f);

	m_pTexture->Set_Texture(pEffect, "g_BaseTexture");

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CBackGround* pInstance = new CBackGround(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CBackGround::Clone(void* _pArg/* = nullptr*/)
{
	CBackGround* pClone = new CBackGround(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CBackGround::Free()
{
	CGameObject::Free();
}
