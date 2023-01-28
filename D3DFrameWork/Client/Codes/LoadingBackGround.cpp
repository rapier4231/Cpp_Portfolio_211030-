#include "stdafx.h"
#include "LoadingBackGround.h"

#include "Export_Function.h"

CLoadingBackGround::CLoadingBackGround(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CLoadingBackGround::CLoadingBackGround(const CLoadingBackGround & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLoadingBackGround::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CLoadingBackGround::Ready_GameObject(void* _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	CComponent*		pComponent = nullptr;

	// VIBuffer 컴포넌트
	pComponent = m_pVIBuffer = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// 텍스처
	pComponent = m_pTexture = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_LoadingScreen"));
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

	// VIBuffer 컴포넌트
	pComponent = m_pVIBufferIcon = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BufferIcon", pComponent);

	// 텍스처
	pComponent = m_pTextureIcon = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_LoadingIcon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_TextureIcon", pComponent);

	m_dFrame = 0.f;

	return S_OK;
}

_int CLoadingBackGround::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	CGameObject::Update_GameObject(_dTimeDelta);

	D3DXMatrixOrthoLH(&m_matScreenProj, WINCX, WINCY, 0.f, 1.f);

	if (15 <= m_dFrame)
		m_dFrame = 0.f;
	else
		m_dFrame += _dTimeDelta * 10.f;

	Engine::Add_RenderGroup(RENDER_BACKUI, this);

	return NO_EVENT;
}

void CLoadingBackGround::Render_GameObject()
{
	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint iPassMax = 0;

	// 로딩 백그라운드 (검은 화면)
	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(1);

	m_pVIBuffer->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	//// 로딩 아이콘 (불)
	//FAILED_CHECK_RETURN(SetUp_ConstantTableIcon(pEffect), );
	//
	//pEffect->Begin(&iPassMax, 0);
	//pEffect->BeginPass(2);
	//
	//m_pVIBufferIcon->Render_VIBuffer();
	//
	//pEffect->EndPass();
	//pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CLoadingBackGround::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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

HRESULT CLoadingBackGround::SetUp_ConstantTableIcon(LPD3DXEFFECT & pEffect)
{
	_mat	matWorld, matView;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = 256.f;
	matWorld._22 = 256.f;
	matWorld._33 = 1.f;

	matWorld._41 = -(WINCX * 0.5f - 100.f);
	matWorld._42 = -(WINCY * 0.5f - 100.f);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matScreenProj);
	pEffect->SetFloat("g_fAlpha", 1.f);

	m_pTextureIcon->Set_Texture(pEffect, "g_BaseTexture", (_uint)m_dFrame);

	return S_OK;
}

CLoadingBackGround * CLoadingBackGround::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CLoadingBackGround* pInstance = new CLoadingBackGround(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CLoadingBackGround::Clone(void* _pArg/* = nullptr*/)
{
	CLoadingBackGround* pClone = new CLoadingBackGround(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CLoadingBackGround::Free()
{
	CGameObject::Free();
}
