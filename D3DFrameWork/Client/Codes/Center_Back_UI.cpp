#include "stdafx.h"
#include "..\Headers\Center_Back_UI.h"
#include "Export_Function.h"

#include "Border.h"

//#define Center_Back_UIPOSX	400.f
//#define Center_Back_UIPOSY	290.f
//#define Center_Back_UISIZEX	200.f
//#define Center_Back_UIALPHA	0.4f
//
//#define FILTERTEXSPEEDCORRECTION 0.15f
//#define OPENEDGEANISPEEDCORRECTION 1.f
//
////#define Center_Back_UIEDGESIZELEFT 0.004f
////#define Center_Back_UIEDGESIZERIGHT 0.9914f
////#define Center_Back_UIEDGESIZETOP 0.002f
////#define Center_Back_UIEDGESIZEBOTTOM 0.996f
//#define Center_Back_UIEDGEALPHA	0.6f

USING(Engine)

HRESULT CCenter_Back_UI::Set_Screen_UI()
{
	STCenter_Back_UIURI stURI;

	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];

	return S_OK;
}

CCenter_Back_UI::CCenter_Back_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CCenter_Back_UI::CCenter_Back_UI(const CCenter_Back_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CCenter_Back_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CCenter_Back_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = Center_Back_UIALPHA;
	m_tORTHOUI_INFO.fEdgeAlpha = Center_Back_UIALPHA;
	m_tORTHOUI_INFO.bUpdate = true;

	return S_OK;
}

_int CCenter_Back_UI::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);

	m_fFilterTex_PosY += (_float)_dTimeDelta * FILTERTEXSPEEDCORRECTION;
	if (FTRUE <= m_fFilterTex_PosY)
		m_fFilterTex_PosY = FNULL;

	Center_Back_UIOpenAni(_dTimeDelta);

	return S_OK;
}

_int CCenter_Back_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	if (m_tORTHOUI_INFO.bRender)
	{
		Engine::Add_RenderGroup(RENDER_ORTHOBACKUI,this);
	}

	return _int();
}

void CCenter_Back_UI::Render_GameObject(_mat _matOrtho)
{
	if (!m_tORTHOUI_INFO.bRender)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = NULL;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(NULL);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CCenter_Back_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent= m_pTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Inventory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pFilter_TextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Inventory_Filter"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_FilterTexture", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Inventory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);
	
	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CCenter_Back_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect , _mat _matOrtho)
{
	m_pTextureCom->Set_Texture(_pEffect, "g_BaseTexture");
	m_pFilter_TextureCom->Set_Texture(_pEffect, "g_FilterTexture");

	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_tORTHOUI_INFO.fSizeX;
	matWorld._22 = m_tORTHOUI_INFO.fSizeY;
	matWorld._33 = FTRUE;

	matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
	matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
	matWorld._43 = FTRUE;

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);

	_pEffect->SetFloat("g_FilterPosY", m_fFilterTex_PosY);
	_pEffect->SetFloat("g_Alpha", m_tORTHOUI_INFO.fAlpha);
	
	_pEffect->SetFloat("g_RatioX", m_fTexRatioX);
	_pEffect->SetFloat("g_RatioY", m_fTexRatioY);

	return S_OK;
}

void CCenter_Back_UI::Free()
{

	COrthoUI::Free();
}

_bool CCenter_Back_UI::UI_RenderOn()
{
	Center_Back_UIOpen();

	return COrthoUI::UI_RenderOn();
}

void CCenter_Back_UI::Center_Back_UIOpen()
{
	if (!m_tORTHOUI_INFO.bRender)
	{
		m_bCenter_Back_UIOpenAni = true;
		m_tORTHOUI_INFO.v4EdgeSize = { FHALF , FHALF  ,FHALF  ,FHALF };
		m_tORTHOUI_INFO.fEdgeAlpha = FNULL;
	}
}
