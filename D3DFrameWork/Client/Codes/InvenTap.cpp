#include "stdafx.h"
#include "..\Headers\InvenTap.h"
#include "Export_Function.h"
#include "Left_Inventory.h"
#include "Right_Inventory.h"
#include "Player.h"
#include "Inven.h"

//#define INVENTAPPOSY	-210.f
//#define INVENTAPSIZEY	30.f
#define INVENTAPALPHA	0.5f

//#define INVENTORYFONTPOSX	78.f
//#define MAKINGFONTPOSX		230.f
//#define INVENTORYPOSY		75.f
//#define INVENTORYMINORPOSY  79.f
//
//#define SELECMOVEPOSX 4.f

#define INVENSELECCOLOR {0.8f,0.8f,1.f,1.f}
#define INVENNONESELECCOLOR {0.5f,0.5f,0.7f,0.8f}

USING(Engine)

HRESULT CInvenTap::Set_Screen_UI()
{
	STInvenTapURI stURI;

	m_tORTHOUI_INFO.fPosX = m_pInventory->Get_ORTHOUI_INFO()->fPosX;
	m_tORTHOUI_INFO.fPosY = m_pInventory->Get_ORTHOUI_INFO()->fPosY - stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = m_pInventory->Get_ORTHOUI_INFO()->fSizeX;
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	return S_OK;
}

CInvenTap::CInvenTap(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CInvenTap::CInvenTap(const CInvenTap & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CInvenTap::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CInvenTap::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_pInventory = static_cast<COrthoUI*>(_pArg);
	
	m_tORTHOUI_INFO.fPosX = m_pInventory->Get_ORTHOUI_INFO()->fPosX;
	m_tORTHOUI_INFO.fPosY = m_pInventory->Get_ORTHOUI_INFO()->fPosY - stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = m_pInventory->Get_ORTHOUI_INFO()->fSizeX;
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = INVENTAPALPHA;
	m_tORTHOUI_INFO.bUpdate = true;

	return S_OK;
}

_int CInvenTap::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);
	m_bForge = false;
	InvenTapSwitching();

	return S_OK;
}

_int CInvenTap::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	if (m_tORTHOUI_INFO.bRender)
	{
		Engine::Add_RenderGroup(RENDER_ORTHOUI, this);
	}

	return _int();
}

void CInvenTap::Render_GameObject(_mat _matOrtho)
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

	RenderFont();
}


CInvenTap * CInvenTap::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CInvenTap*	pInstance = new CInvenTap(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CInvenTap::Clone(void * _pArg/* = nullptr*/)
{
	CInvenTap*	pClone = new CInvenTap(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CInvenTap::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent= m_pTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_InvenTap_Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pTextureCom2 = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_InvenTap_Right"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture2", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_InvenTap"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);
	
	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CInvenTap::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho)
{
	if (m_bForge)
	{
		if (m_bSelect_Tap_Item)
			m_pTextureCom2->Set_Texture(_pEffect, "g_BaseTexture");
		else
			m_pTextureCom->Set_Texture(_pEffect, "g_BaseTexture");
	}

	else
	{
		if (m_bSelect_Tap_Item)
			m_pTextureCom->Set_Texture(_pEffect, "g_BaseTexture");
		else
			m_pTextureCom2->Set_Texture(_pEffect, "g_BaseTexture");
	}

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

	_pEffect->SetFloat("g_Alpha", INVENTAPALPHA);
	
	return S_OK;
}

void CInvenTap::RenderFont()
{
	STInvenTapURI stURI;

	switch (m_iInvenTapOwner)
	{
	case 0: //오른쪽 인벤토리
		if(m_bForge)
		{
				if (m_bSelect_Tap_Item)
			{
				Engine::Render_Font(L"Font_TapName", L"인벤토리", &_vec2(stURI.fFont_Right_Inventory_PosX[EScrnResolution] - stURI.fFontSelectMovePosX[EScrnResolution], stURI.fFontPosY[EScrnResolution] - stURI.fFontSelectMovePosY[EScrnResolution]), INVENSELECCOLOR);
				Engine::Render_Font(L"Font_TapName", L"제작", &_vec2(stURI.fFont_Right_Making_PosX[EScrnResolution], stURI.fFontPosY[EScrnResolution]), INVENNONESELECCOLOR);
			}
			else
			{
				Engine::Render_Font(L"Font_TapName", L"인벤토리", &_vec2(stURI.fFont_Right_Inventory_PosX[EScrnResolution], stURI.fFontPosY[EScrnResolution]), INVENNONESELECCOLOR);
				Engine::Render_Font(L"Font_TapName", L"제작", &_vec2(stURI.fFont_Right_Making_PosX[EScrnResolution] + stURI.fFontSelectMovePosX[EScrnResolution], stURI.fFontPosY[EScrnResolution] - stURI.fFontSelectMovePosY[EScrnResolution]), INVENSELECCOLOR);
			}
		}
		else
			Engine::Render_Font(L"Font_TapName", L"인벤토리", &_vec2(stURI.fFont_Right_Inventory_PosX[EScrnResolution] - stURI.fFontSelectMovePosX[EScrnResolution], stURI.fFontPosY[EScrnResolution] - stURI.fFontSelectMovePosY[EScrnResolution]), INVENSELECCOLOR);
		break;
	case 1 : //왼쪽 인벤토리
		if (m_bSelect_Tap_Item)
		{
			Engine::Render_Font(L"Font_TapName", L"인벤토리", &_vec2(stURI.fFont_Left_Inventory_PosX[EScrnResolution] + stURI.fFontSelectMovePosX[EScrnResolution], stURI.fFontPosY[EScrnResolution] - stURI.fFontSelectMovePosY[EScrnResolution]), INVENSELECCOLOR);
			Engine::Render_Font(L"Font_TapName", L"제작", &_vec2(stURI.fFont_Left_Making_PosX[EScrnResolution], stURI.fFontPosY[EScrnResolution] ), INVENNONESELECCOLOR);
		}
		else
		{
			Engine::Render_Font(L"Font_TapName", L"인벤토리", &_vec2(stURI.fFont_Left_Inventory_PosX[EScrnResolution], stURI.fFontPosY[EScrnResolution] ), INVENNONESELECCOLOR);
			Engine::Render_Font(L"Font_TapName", L"제작", &_vec2(stURI.fFont_Left_Making_PosX[EScrnResolution] - stURI.fFontSelectMovePosX[EScrnResolution], stURI.fFontPosY[EScrnResolution] - stURI.fFontSelectMovePosY[EScrnResolution]), INVENSELECCOLOR);
		}
			break;
	default:
		break;
	}
}

void CInvenTap::Free()
{


	COrthoUI::Free();
}

void CInvenTap::InvenTapSwitching()
{
	if (m_tORTHOUI_INFO.bRender)
	{
		if (m_iInvenTapOwner)
		{
			MouseCheck();
			if (m_tORTHOUI_INFO.uchSelectUI & MouseClickUI)
			{
				m_bSelect_Tap_Item = !m_bSelect_Tap_Item;
				static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->ChangeInvenTap();
				static_cast<CScroll*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Scroll_RatioY_Swap(CScroll::EScroll_SwapRatioY::EScroll_Swap_SaveY_1Slot);
			}
		}
		else
		{
			CGameObject* pLookGameObject = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0))->Get_pPlayerLookUnit();
			if (pLookGameObject)
			{
				CInven* pLookInven = static_cast<CInven*>(pLookGameObject->Get_Component(L"Com_Inven", ID_DYNAMIC));
				if (pLookInven)
				{
					if (pLookInven->Get_InvenUserUnitTypeID() == InvenUserUnitTypeID_Brazier)
						m_bForge = true;
				}
			}
			if (m_bForge)
			{
				MouseCheck();
				if (m_tORTHOUI_INFO.uchSelectUI & MouseClickUI)
				{
					m_bSelect_Tap_Item = !m_bSelect_Tap_Item;
					static_cast<CRight_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->ChangeInvenTap();
					static_cast<CScroll*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Scroll_RatioY_Swap(CScroll::EScroll_SwapRatioY::EScroll_Swap_SaveY_1Slot);
				}
			}
			else
				m_bSelect_Tap_Item = false;
		}
	}
}
