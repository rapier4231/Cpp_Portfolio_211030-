#include "stdafx.h"
#include "..\Headers\InvenSearchbar.h"

#include "Export_Function.h"

//#define ICON_SearchbarPOSX	-74.f
//#define ICON_SearchbarPOSY	-180.f
//#define ICON_SearchbarSizeX 56.f
//#define ICON_SearchbarSizeY 14.f
//
//#define ICONSearchbarEDGESIZELEFT 0.005f
//#define ICONSearchbarEDGESIZERIGHT 0.98f
//#define ICONSearchbarEDGESIZETOP 0.03f
//#define ICONSearchbarEDGESIZEBOTTOM 0.9f
USING(Engine)

HRESULT CInvenSearchbar::Set_Screen_UI()
{
	STInvenToolTipURI stURI;

	m_tORTHOUI_INFO.fPosX = m_pInventory->Get_ORTHOUI_INFO()->fPosX - stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = m_pInventory->Get_ORTHOUI_INFO()->fPosY - stURI.fPosY[EScrnResolution];

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	return S_OK;
}

CInvenSearchbar::CInvenSearchbar(LPDIRECT3DDEVICE9 _pDevice)
	: CInvenIcon(_pDevice)
{
}

CInvenSearchbar::CInvenSearchbar(const CInvenSearchbar & rhs)
	: CInvenIcon(rhs)
{
}

HRESULT CInvenSearchbar::Ready_GameObject_Prototype()
{
	CInvenIcon::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CInvenSearchbar::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CInvenIcon::Ready_GameObject(_pArg);

	m_pInventory = static_cast<COrthoUI*>(_pArg);

	m_tORTHOUI_INFO.fPosX = m_pInventory->Get_ORTHOUI_INFO()->fPosX - stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = m_pInventory->Get_ORTHOUI_INFO()->fPosY - stURI.fPosY[EScrnResolution];

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = ICONNOMALALPHA;
	m_tORTHOUI_INFO.fEdgeAlpha = ICONEDGENOMALALPHA;
	m_tORTHOUI_INFO.bUpdate = true;

	return S_OK;
}

_int CInvenSearchbar::Update_GameObject(const _double & _dTimeDelta)
{
	CInvenIcon::Update_GameObject(_dTimeDelta);

	return S_OK;
}

_int CInvenSearchbar::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CInvenIcon::LateUpdate_GameObject(_dTimeDelta);

	if (m_tORTHOUI_INFO.bRender)
	{
		Engine::Add_RenderGroup(RENDER_ORTHOUI, this);
	}

	return _int();
}

void CInvenSearchbar::Render_GameObject(_mat _matOrtho)
{
	if (!m_tORTHOUI_INFO.bRender)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 2;

	pEffect->Begin(&iPassMax, 0);

	pEffect->CommitChanges();

	pEffect->BeginPass(1);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}


CInvenSearchbar * CInvenSearchbar::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CInvenSearchbar*	pInstance = new CInvenSearchbar(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CInvenSearchbar::Clone(void * _pArg/* = nullptr*/)
{
	CInvenSearchbar*	pClone = new CInvenSearchbar(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CInvenSearchbar::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBoxNomalTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_InvenIcon_Box_Nomal"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BoxNomalTexture", pComponent);
	
	pComponent = m_pBoxHighlightTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_InvenIcon_Box_Highlight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BoxHighlightTexture", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_InvenIcon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);
	return S_OK;
}

HRESULT CInvenSearchbar::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho)
{
	if(m_bBoxSelect)
		m_pBoxHighlightTextureCom->Set_Texture(_pEffect, "g_BoxTexture");
	else
		m_pBoxNomalTextureCom->Set_Texture(_pEffect, "g_BoxTexture");

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

	_pEffect->SetFloat("g_Alpha", m_tORTHOUI_INFO.fAlpha);

	return S_OK;
}

void CInvenSearchbar::Free()
{

	CInvenIcon::Free();
}