#include "stdafx.h"
#include "..\Headers\InvenItemSort.h"

#include "Export_Function.h"

//#define ICON_ItemSortPOSX	15.f
//#define ICON_ItemSortPOSY	-180.f

USING(Engine)

HRESULT CInvenItemSort::Set_Screen_UI()
{
	STInvenItemSortURI stURI;
	m_tORTHOUI_INFO.fPosX = m_pInventory->Get_ORTHOUI_INFO()->fPosX + stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = m_pInventory->Get_ORTHOUI_INFO()->fPosY - stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = stIconURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stIconURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stIconURI.v4EdgeSize[EScrnResolution];
	return S_OK;
}

CInvenItemSort::CInvenItemSort(LPDIRECT3DDEVICE9 _pDevice)
	: CInvenIcon(_pDevice)
{
}

CInvenItemSort::CInvenItemSort(const CInvenItemSort & rhs)
	: CInvenIcon(rhs)
{
}

HRESULT CInvenItemSort::Ready_GameObject_Prototype()
{
	CInvenIcon::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CInvenItemSort::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CInvenIcon::Ready_GameObject(_pArg);

	m_pInventory = static_cast<COrthoUI*>(_pArg);

	m_tORTHOUI_INFO.fPosX = m_pInventory->Get_ORTHOUI_INFO()->fPosX + stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = m_pInventory->Get_ORTHOUI_INFO()->fPosY - stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = stIconURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stIconURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stIconURI.v4EdgeSize[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = ICONNOMALALPHA;
	m_tORTHOUI_INFO.fEdgeAlpha = ICONEDGENOMALALPHA;
	m_tORTHOUI_INFO.bUpdate = true;
	return S_OK;
}

_int CInvenItemSort::Update_GameObject(const _double & _dTimeDelta)
{
	CInvenIcon::Update_GameObject(_dTimeDelta);

	return S_OK;
}

_int CInvenItemSort::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CInvenIcon::LateUpdate_GameObject(_dTimeDelta);

	if (m_tORTHOUI_INFO.bRender)
	{
		Engine::Add_RenderGroup(RENDER_ORTHOUI, this);
	}

	return _int();
}

void CInvenItemSort::Render_GameObject(_mat _matOrtho)
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


CInvenItemSort * CInvenItemSort::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CInvenItemSort*	pInstance = new CInvenItemSort(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CInvenItemSort::Clone(void * _pArg/* = nullptr*/)
{
	CInvenItemSort*	pClone = new CInvenItemSort(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CInvenItemSort::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_InvenIcon_ItemSort"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

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

HRESULT CInvenItemSort::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho)
{
	m_pTextureCom->Set_Texture(_pEffect, "g_BaseTexture");

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

void CInvenItemSort::Free()
{

	CInvenIcon::Free();
}