#include "stdafx.h"
#include "..\Headers\Setting_UI.h"

#include "Export_Function.h"

#define SettingAlpha 0.7f
USING(Engine)

HRESULT CSetting_UI::Set_Screen_UI()
{
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	return S_OK;
}

CSetting_UI::CSetting_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CSetting_UI::CSetting_UI(const CSetting_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CSetting_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CSetting_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	m_tORTHOUI_INFO.fEdgeAlpha = SettingAlpha;
	m_tORTHOUI_INFO.fAlpha = SettingAlpha;

	return S_OK;
}

_int CSetting_UI::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;
	COrthoUI::Update_GameObject(_dTimeDelta);

	return NO_EVENT;
}

_int CSetting_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	Engine::Add_RenderGroup(RENDER_LASTUI, this);
	return NO_EVENT;
}

void CSetting_UI::Render_GameObject(_mat _matOrtho)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 6;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(0); /////

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();

	//////////////////////////	
	//pEffect->CommitChanges();
	//SetUp_ConstantTable(pEffect, _matOrtho,1);
	//pEffect->BeginPass(1);    ////
	//m_pBufferCom->Render_VIBuffer();
	//pEffect->EndPass();
	////////////////////////////

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CSetting_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTexture_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L""));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L""));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CSetting_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);


	matWorld._11 = m_tORTHOUI_INFO.fSizeX;
	matWorld._22 = m_tORTHOUI_INFO.fSizeY;
	matWorld._33 = FTRUE;

	matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
	matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
	matWorld._43 = FTRUE;

	m_pTexture_Com->Set_Texture(_pEffect, "g_Bg_Tex");
	_pEffect->SetFloat("g_Alpha", m_tORTHOUI_INFO.fAlpha);
	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);

	return S_OK;
}

CSetting_UI * CSetting_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CSetting_UI*	pInstance = new CSetting_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CSetting_UI::Clone(void * _pArg)
{
	CSetting_UI*	pClone = new CSetting_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CSetting_UI::Free()
{
	COrthoUI::Free();
}