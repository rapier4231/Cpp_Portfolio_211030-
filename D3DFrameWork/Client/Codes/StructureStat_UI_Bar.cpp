#include "stdafx.h"
#include "..\Headers\StructureStat_UI_Bar.h"

#include "StructureStat_UI.h"
#include "Export_Function.h"
#include "Player.h"
#include "Left_Inventory.h"

//#define StatUIPosX (WINCX*0.5f - 14.f)
//#define StatUIPosY (278.f)
//#define StatUISizeX (156.f)
//#define StatUISizeY (18.f)
//#define EStructureSTATbar_EXP	10 원래 주석이였음

#define StatUINOMALALPHA 0.9f
#define StatFontColor D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
USING(Engine)

HRESULT CStructureStat_UI_Bar::Set_Screen_UI()
{
	STStructureStat_UI_BarURI stURI;

 	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution] + ((stURI.fSizeY[EScrnResolution] + stURI.fIntervalPosY[EScrnResolution]) *m_eStatType);
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	switch (m_eStatType)
	{
	case Engine::EStructureSTATbar_HP:
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Hp_Bar_EdgeSize[EScrnResolution];
		break;
	case Engine::EStructureSTATbar_Amount:
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Amount_Bar_EdgeSize[EScrnResolution];
		break;
	case Engine::EStructureSTATbar_Weight:
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Weight_Bar_EdgeSize[EScrnResolution];
		break;
	}
	return S_OK;
}

CStructureStat_UI_Bar::CStructureStat_UI_Bar(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CStructureStat_UI_Bar::CStructureStat_UI_Bar(const CStructureStat_UI_Bar & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CStructureStat_UI_Bar::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CStructureStat_UI_Bar::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	ITEM_INFO* tStatInfo = static_cast<ITEM_INFO*>(_pArg);
	wstring wstrStatType = L"Proto_Texture_";
	m_eStatType = (EStructureSTATbar)tStatInfo->iPosition;

	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution] + ((stURI.fSizeY[EScrnResolution] + stURI.fIntervalPosY[EScrnResolution]) *tStatInfo->iPosition);
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = StatUINOMALALPHA;
	m_tORTHOUI_INFO.fEdgeAlpha = StatUINOMALALPHA;
	m_tORTHOUI_INFO.bUpdate = false;
	m_tORTHOUI_INFO.bRender = false;

	switch (m_eStatType)
	{
	case Engine::EStructureSTATbar_HP:
		wstrStatType += L"SummaryStat_Health";
		//							  	 왼쪽   위   오른쪽  아래
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Hp_Bar_EdgeSize[EScrnResolution];
		break;
	case Engine::EStructureSTATbar_Amount:
		wstrStatType += L"SummaryStat_Amount";
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Amount_Bar_EdgeSize[EScrnResolution];
		break;
	case Engine::EStructureSTATbar_Weight:
		wstrStatType += L"SummaryStat_Weight";
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Weight_Bar_EdgeSize[EScrnResolution];
		break;
	}

	CComponent*		pComponent = nullptr;
	pComponent = m_pStatTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(wstrStatType));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);
	return S_OK;
}

_int CStructureStat_UI_Bar::Update_GameObject(const _double & _dTimeDelta)
{
	m_pStructure = static_cast<CStructureStat_UI*>(Get_GameObject(L"Center_UI", 2))->Get_pStructure();
	if (!m_pStructure)
		return NO_EVENT;
	COrthoUI::Update_GameObject(_dTimeDelta);
	Set_Screen_UI();
	return S_OK;
}

_int CStructureStat_UI_Bar::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_pStructure)
		return NO_EVENT;

	COrthoUI::LateUpdate_GameObject(_dTimeDelta);
	if (m_tORTHOUI_INFO.bRender)
	{
		_float fNowStat; //가지고 있던 now스텟을 잠깐동안 pre스텟으로 만들기 위해
		switch (m_eStatType)
		{
		case Engine::EStructureSTATbar_HP:
			m_fMaxPlayerStat = (_float)m_pStructure->Get_Stat(CUnit::STAT_MAXHP);
			fNowStat = (_float)m_pStructure->Get_Stat(CUnit::STAT_HP);
			break;
		case Engine::EStructureSTATbar_Amount: 
			//인벤에서 확인
			break;
		case Engine::EStructureSTATbar_Weight:
			m_fMaxPlayerStat = (_float)m_pStructure->Get_Stat(CUnit::STAT_MAXWEIGHT);
			fNowStat = (_float)m_pStructure->Get_Stat(CUnit::STAT_WEIGHT);
			break;
		}

		//m_fNowPlayerStat = fNowStat;
		//m_fPerPlayerStat = m_fNowPlayerStat / m_fMaxPlayerStat;
		Engine::Add_RenderGroup(RENDER_ITEMUI, this);
	}
	return _int();
}

void CStructureStat_UI_Bar::Render_GameObject(_mat _matOrtho)
{
	if (!m_tORTHOUI_INFO.bRender)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 4;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(NULL); /////현재 수치 바

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();

	//////////////////////////
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, 1);
	pEffect->BeginPass(1);      ////그림
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	////////////////////////////

	pEffect->End();

	Safe_Release(pEffect);

	Render_Font(_matOrtho);
}

HRESULT CStructureStat_UI_Bar::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	
	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Stat_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CStructureStat_UI_Bar::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	STStructureStat_UI_BarURI stURI;

	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	if (1 == _bTwo) //스텟 아이콘 놓을 자리
	{
		matWorld._11 = stURI.fStatIconSizeXY[EScrnResolution];
		matWorld._22 = stURI.fStatIconSizeXY[EScrnResolution];
		matWorld._33 = FTRUE;

		matWorld._41 = (m_tORTHOUI_INFO.fPosX - stURI.fStatIcon_IntervalPosX[EScrnResolution]) - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pStatTextureCom->Set_Texture(_pEffect, "g_StatTexture");
	}
	else
	{
		matWorld._11 = m_tORTHOUI_INFO.fSizeX;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;
	}

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);

	_pEffect->SetFloat("g_Alpha", m_tORTHOUI_INFO.fAlpha);

	_pEffect->SetFloat("g_PercentStat", (m_fPerPlayerStat));
	return S_OK;
}

void CStructureStat_UI_Bar::Render_Font(_mat _matOrtho)
{
	STStructureStat_UI_BarURI stURI;

	wstring wstrNum = L"";
	switch (m_eStatType)
	{
	case Engine::EStructureSTATbar_HP:
		wstrNum += to_wstring((_int)m_fNowPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fNowPlayerStat * 10.f)) - (((_int)m_fNowPlayerStat) * 10)); //10은 소숫점 하나 올려줄려고 쓴거라 디파인 안해도 됨
		wstrNum += L" / ";
		wstrNum += to_wstring((_int)m_fMaxPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fMaxPlayerStat * 10.f)) - (((_int)m_fMaxPlayerStat) * 10));
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_HP_Stat_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_HP_Stat_IntervalPosY[EScrnResolution]), StatFontColor,1);
		wstrNum = L"체력";
		break;
	case Engine::EStructureSTATbar_Amount:
		wstrNum += to_wstring((_int)m_fNowPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fNowPlayerStat * 10.f)) - (((_int)m_fNowPlayerStat) * 10));
		wstrNum += L" / ";
		wstrNum += to_wstring((_int)m_fMaxPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fMaxPlayerStat * 10.f)) - (((_int)m_fMaxPlayerStat) * 10));
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Amount_Stat_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_Amount_Stat_IntervalPosY[EScrnResolution]), StatFontColor, 1);
		wstrNum = L"보관함";
		break;
	case Engine::EStructureSTATbar_Weight:
		wstrNum += to_wstring((_int)m_fNowPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fNowPlayerStat * 10.f)) - (((_int)m_fNowPlayerStat) * 10));
		wstrNum += L" / ";
		wstrNum += to_wstring((_int)m_fMaxPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fMaxPlayerStat * 10.f)) - (((_int)m_fMaxPlayerStat) * 10));
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Weight_Stat_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_Weight_Stat_IntervalPosY[EScrnResolution]), StatFontColor, 1);
		wstrNum = L"무게";
		break;
	}

	Engine::Render_Font(L"Font_StatGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_Stat_Name_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_HP_Stat_IntervalPosY[EScrnResolution]), StatFontColor);
}

CStructureStat_UI_Bar * CStructureStat_UI_Bar::Create(LPDIRECT3DDEVICE9 _pDevice)
{

	CStructureStat_UI_Bar*	pInstance = new CStructureStat_UI_Bar(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CStructureStat_UI_Bar::Clone(void * _pArg)
{
	CStructureStat_UI_Bar*	pClone = new CStructureStat_UI_Bar(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CStructureStat_UI_Bar::Free()
{
	COrthoUI::Free();
}