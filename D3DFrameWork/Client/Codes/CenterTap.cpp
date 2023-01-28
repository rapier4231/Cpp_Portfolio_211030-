#include "stdafx.h"
#include "..\Headers\CenterTap.h"
#include "Export_Function.h"
#include "Unit.h"
#include "Dino.h"
#include "MonsterStat_UI.h"
#include "Stat_UI.h"
#include "StructureStat_UI.h"
//#define CenterTAPPOSX	0.f
//#define CenterTAPPOSY	-210.f
//#define CenterTAPSIZEY	30.f
//#define INVENTORYPOSY		75.f

#define CenterTAPALPHA	0.5f
#define INVENSELECCOLOR {0.8f,0.8f,1.f,1.f}
#define PlayerIntroduceColor (D3DXCOLOR(0.7f, 0.9f, 1.f, 1.f))
USING(Engine)

HRESULT CCenterTap::Set_Screen_UI()

{
	STCenterTapURI stURI;

	m_tORTHOUI_INFO.fPosX = m_pCentertory->Get_ORTHOUI_INFO()->fPosX;
	m_tORTHOUI_INFO.fPosY = m_pCentertory->Get_ORTHOUI_INFO()->fPosY - stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = m_pCentertory->Get_ORTHOUI_INFO()->fSizeX;
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	return S_OK;
}

CCenterTap::CCenterTap(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CCenterTap::CCenterTap(const CCenterTap & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CCenterTap::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CCenterTap::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_pCentertory = static_cast<COrthoUI*>(_pArg);
	
	m_tORTHOUI_INFO.fPosX = m_pCentertory->Get_ORTHOUI_INFO()->fPosX;
	m_tORTHOUI_INFO.fPosY = m_pCentertory->Get_ORTHOUI_INFO()->fPosY - stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = m_pCentertory->Get_ORTHOUI_INFO()->fSizeX;
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = CenterTAPALPHA;
	m_tORTHOUI_INFO.bUpdate = true;

	return S_OK;
}

_int CCenterTap::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
	{
		m_bDaeSang = false;
		return NO_EVENT;
	}
	COrthoUI::Update_GameObject(_dTimeDelta);

	InvenTapSwitching();
	return S_OK;
}

_int CCenterTap::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	if (m_tORTHOUI_INFO.bRender)
	{
		Engine::Add_RenderGroup(RENDER_ORTHOUI, this);
	}

	return _int();
}

void CCenterTap::Render_GameObject(_mat _matOrtho)
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


CCenterTap * CCenterTap::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CCenterTap*	pInstance = new CCenterTap(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CCenterTap::Clone(void * _pArg/* = nullptr*/)
{
	CCenterTap*	pClone = new CCenterTap(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CCenterTap::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent= m_pTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_CenterTap_Left"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pTextureCom2 = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_CenterTap_Right"));
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

HRESULT CCenterTap::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho)
{
	if(m_bSelect_Tap_Item)
		m_pTextureCom->Set_Texture(_pEffect, "g_BaseTexture");
	else
		m_pTextureCom2->Set_Texture(_pEffect, "g_BaseTexture");

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

void CCenterTap::InvenTapSwitching()
{
	if (m_tORTHOUI_INFO.bRender)
	{
		if (!m_pPreUnit) //대상이 없어졌으면 
		{
			m_bDaeSang = false;
			return;
		}

		MouseCheck();
		if (m_tORTHOUI_INFO.uchSelectUI & MouseClickUI)
		{
			if (m_bSelect_Tap_Item && !m_bDaeSang)
				return;

			m_bSelect_Tap_Item = !m_bSelect_Tap_Item;
			if (m_bSelect_Tap_Item) //플레이어 라면
			{
				if((_int)InvenUserUnitTypeID::InvenUserUnitTypeID_Monster == m_iInvenTapOwner)
					static_cast<CMonsterStat_UI*>(Engine::Get_GameObject(L"Center_UI", 1))->Inventory_Render_Off(false);
				else
					static_cast<CStructureStat_UI*>(Engine::Get_GameObject(L"Center_UI", 2))->Inventory_Render_Off(false);
				
				static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->Inventory_Render_On();
				return;
			}
			else //대상으로 탭이라면
			{
				//플레이어 지우고
				static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->Inventory_Render_Off();

				if ((_int)InvenUserUnitTypeID::InvenUserUnitTypeID_Monster == m_iInvenTapPreOwner)// 현재 대상이 몬스터면
					static_cast<CMonsterStat_UI*>(Engine::Get_GameObject(L"Center_UI", 1))->Inventory_Render_On(m_pPreUnit);
				else
					static_cast<CStructureStat_UI*>(Engine::Get_GameObject(L"Center_UI", 2))->Inventory_Render_On((InvenUserUnitTypeID)m_iInvenTapPreOwner,m_pPreUnit);

				m_iInvenTapPreOwner = NULL;
			}
		}
	}
}

void CCenterTap::RenderFont()
{
	STCenterTapURI stURI;
	wstring wstrPlayerLv = L"";
	wstring wstrFont = L"";
	CUnit* pUnitMonster = static_cast<CMonsterStat_UI*>(Engine::Get_GameObject(L"Center_UI", 1))->Get_pMonster();
	CDino* pMonster = static_cast<CDino*>(static_cast<CMonsterStat_UI*>(Engine::Get_GameObject(L"Center_UI", 1))->Get_pMonster());

	switch (m_iInvenTapOwner)
	{
	case 0:
		m_bSelect_Tap_Item = true;
		//플레이어 인벤토리
		Engine::Render_Font(L"Font_TapName", L"플레이어", &_vec2(WINCX*0.5f - stURI.fFont_Inventory_FixPosX[EScrnResolution], stURI.fFont_Inventory_FixPosY[EScrnResolution]), INVENSELECCOLOR);
		if (m_bDaeSang)
		{
			if(1 == m_iInvenTapOwner)
				Engine::Render_Font(L"Font_TapName", L"대상", &_vec2(WINCX*0.5f + 1.f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution] + 0.05f*ScrnRatio), INVENSELECCOLOR);
			else
				Engine::Render_Font(L"Font_TapName", L"구조물", &_vec2(WINCX*0.5f + 0.7f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution] + 0.05f*ScrnRatio), INVENSELECCOLOR);
		}
		//플레이어 이름, 레벨
		Engine::Render_Font(L"Font_StatGuide", L"글로리 Jo", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerName_FixPosY[EScrnResolution]), PlayerIntroduceColor,2);
		wstrPlayerLv = L"레벨: ";
		wstrPlayerLv += to_wstring((_int)static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Get_Stat(CUnit::STAT_LEVEL));
		Engine::Render_Font(L"Font_StatGuide", wstrPlayerLv, &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerLevel_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);

		Engine::Render_Font(L"Font_ItemGuide", L"부족", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerTribal_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"Jusin-115A 부족민", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_TribalContent_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"상태", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerState_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"분노, 복수, 도피", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_StateContent_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		break;
	case 1:
		m_bSelect_Tap_Item = false;
		Engine::Render_Font(L"Font_TapName", L"플레이어", &_vec2(WINCX*0.5f - stURI.fFont_Inventory_FixPosX[EScrnResolution] - 0.2f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution] + 0.05f*ScrnRatio), INVENSELECCOLOR);
		Engine::Render_Font(L"Font_TapName", L"대상", &_vec2(WINCX*0.5f + 0.8f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution]), INVENSELECCOLOR);
		//몬스터 이름
		if (!pMonster)
			return;
		wstrFont = pMonster->Get_DinoSpeciesKor();
		Engine::Render_Font(L"Font_StatGuide", wstrFont, &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerName_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);

		wstrPlayerLv = L"레벨: ";
		wstrPlayerLv += to_wstring((_int)pUnitMonster->Get_Stat(CUnit::STAT_LEVEL));
		Engine::Render_Font(L"Font_StatGuide", wstrPlayerLv, &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerLevel_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);

		Engine::Render_Font(L"Font_ItemGuide", L"부족", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerTribal_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"Jusin-115A 부족", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_TribalContent_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"상태", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerState_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"조련", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_StateContent_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		break;
	case (_int)InvenUserUnitTypeID::InvenUserUnitTypeID_Campfire:
		m_bSelect_Tap_Item = false;
		Engine::Render_Font(L"Font_TapName", L"플레이어", &_vec2(WINCX*0.5f - stURI.fFont_Inventory_FixPosX[EScrnResolution] - 0.2f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution] + 0.05f*ScrnRatio), INVENSELECCOLOR);
		Engine::Render_Font(L"Font_TapName", L"구조물", &_vec2(WINCX*0.5f + 0.5f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution]), INVENSELECCOLOR);
		
		Engine::Render_Font(L"Font_StatGuide", L"모닥불", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerLevel_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"주인", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerTribal_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"글로리Jo의 부족", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_TribalContent_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"불을 붙이려면 나무나 짚같은 연료가 필요합니다.", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerState_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"불을 붙이면 날 음식을 조리할 수 있습니다.", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_StateContent_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		break;
	case (_int)InvenUserUnitTypeID::InvenUserUnitTypeID_Smithy:
		m_bSelect_Tap_Item = false;
		Engine::Render_Font(L"Font_TapName", L"플레이어", &_vec2(WINCX*0.5f - stURI.fFont_Inventory_FixPosX[EScrnResolution] - 0.2f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution] + 0.05f*ScrnRatio), INVENSELECCOLOR);
		Engine::Render_Font(L"Font_TapName", L"구조물", &_vec2(WINCX*0.5f + 0.5f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution]), INVENSELECCOLOR);
		
		Engine::Render_Font(L"Font_StatGuide", L"대장간", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerLevel_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"주인", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerTribal_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"글로리Jo의 부족", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_TribalContent_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"철제 아이템을 제작하려면 여기서.", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerState_FixPosY[EScrnResolution]), PlayerIntroduceColor, 4);
		break;
	case (_int)InvenUserUnitTypeID::InvenUserUnitTypeID_Mortar:
		m_bSelect_Tap_Item = false;
		Engine::Render_Font(L"Font_TapName", L"플레이어", &_vec2(WINCX*0.5f - stURI.fFont_Inventory_FixPosX[EScrnResolution] - 0.2f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution] + 0.05f*ScrnRatio), INVENSELECCOLOR);
		Engine::Render_Font(L"Font_TapName", L"구조물", &_vec2(WINCX*0.5f + 0.5f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution]), INVENSELECCOLOR);
		
		Engine::Render_Font(L"Font_StatGuide", L"절구와 공이", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerLevel_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"주인", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerTribal_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"글로리Jo의 부족", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_TribalContent_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"자원을 갈아 새로운 아이템을 만듭니다.", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerState_FixPosY[EScrnResolution]), PlayerIntroduceColor, 4);
		break;
	case (_int)InvenUserUnitTypeID::InvenUserUnitTypeID_Locker:
		Engine::Render_Font(L"Font_TapName", L"플레이어", &_vec2(WINCX*0.5f - stURI.fFont_Inventory_FixPosX[EScrnResolution] - 0.2f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution] + 0.05f*ScrnRatio), INVENSELECCOLOR);
		Engine::Render_Font(L"Font_TapName", L"구조물", &_vec2(WINCX*0.5f + 0.5f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution]), INVENSELECCOLOR);
		
		Engine::Render_Font(L"Font_StatGuide", L"보관함", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerLevel_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"주인", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerTribal_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"글로리Jo의 부족", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_TribalContent_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"이곳에 아이템을 보관하세요.", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerState_FixPosY[EScrnResolution]), PlayerIntroduceColor, 4);
		break;
	case (_int)InvenUserUnitTypeID::InvenUserUnitTypeID_Brazier:
		Engine::Render_Font(L"Font_TapName", L"플레이어", &_vec2(WINCX*0.5f - stURI.fFont_Inventory_FixPosX[EScrnResolution] - 0.2f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution] + 0.05f*ScrnRatio), INVENSELECCOLOR);
		Engine::Render_Font(L"Font_TapName", L"구조물", &_vec2(WINCX*0.5f + 0.5f*ScrnRatio, stURI.fFont_Inventory_FixPosY[EScrnResolution]), INVENSELECCOLOR);
		
		Engine::Render_Font(L"Font_StatGuide", L"공업용 화로", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerLevel_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"주인", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerTribal_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"글로리Jo의 부족", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_TribalContent_FixPosY[EScrnResolution]), PlayerIntroduceColor, 2);
		Engine::Render_Font(L"Font_ItemGuide", L"정제되지 않은 자원을 넣어 이 곳에서 정제시킬 수 있습니다.", &_vec2(WINCX*0.5f, stURI.fFont_Inventory_FixPosY[EScrnResolution] + stURI.fFont_PlayerState_FixPosY[EScrnResolution]), PlayerIntroduceColor, 4);
		break;
	default:
		break;
	}
}

void CCenterTap::Set_TapOwner(_int _iOwner, CUnit* _pPreUnit)
{
	m_iInvenTapPreOwner = m_iInvenTapOwner;
	m_iInvenTapOwner = _iOwner;
	if (0 == m_iInvenTapOwner)//플레이어
	{
		m_bSelect_Tap_Item = true;
	}
	else if (1 == m_iInvenTapOwner) //몬스터
	{
		m_bSelect_Tap_Item = false;
		m_pPreUnit = _pPreUnit;
	}
	else //스트럭쳐
	{
		m_bSelect_Tap_Item = false;
		m_pPreUnit = _pPreUnit;
	}
	return;
}

void CCenterTap::Free()
{
	COrthoUI::Free();
}