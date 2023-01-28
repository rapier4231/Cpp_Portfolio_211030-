#include "stdafx.h"
#include "..\Headers\MonsterStat_UI_Bar.h"

#include "MonsterStat_UI.h"
#include "Export_Function.h"
#include "Player.h"
#include "Left_Inventory.h"

//#define StatUIPosX (WINCX*0.5f - 14.f)
//#define StatUIPosY (278.f)
//#define StatUISizeX (156.f)
//#define StatUISizeY (18.f)
//#define EMonsterSTATbar_EXP	10 원래 주석이였음

#define StatUINOMALALPHA 0.9f
#define StatUpCoolTime 1.f
#define FinalStatUpCoolTime 0.25f //? 모름
#define ParalysisMaxStat 200.f
#define GlowFixTimeRatio /* 20.f*/ 30.f
#define StatFontColor D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
USING(Engine)

HRESULT CMonsterStat_UI_Bar::Set_Screen_UI()
{
	STMonsterStat_UI_BarURI stURI;

 	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution] + ((stURI.fSizeY[EScrnResolution] + stURI.fIntervalPosY[EScrnResolution]) *m_eStatType);
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_fGlowSizeXY = { stURI.fGlowSizeXY[EScrnResolution],stURI.fGlowSizeXY[EScrnResolution] };
	switch (m_eStatType)
	{
	case Engine::EMonsterSTATbar_HP:
		m_v4PlusEdge = stURI.v4Hp_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Hp_Bar_EdgeSize[EScrnResolution];
		break;
	case Engine::EMonsterSTATbar_Stamina:
		m_v4PlusEdge = stURI.v4Stamina_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Stamina_Bar_EdgeSize[EScrnResolution];

		break;
	case Engine::EMonsterSTATbar_Food:
		m_v4PlusEdge = stURI.v4Food_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Food_Bar_EdgeSize[EScrnResolution];
		break;
	case Engine::EMonsterSTATbar_Weight:
		m_v4PlusEdge = stURI.v4Weight_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Weight_Bar_EdgeSize[EScrnResolution];

		break;
	case Engine::EMonsterSTATbar_Atk:
		m_v4PlusEdge = stURI.v4Atk_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Atk_Bar_EdgeSize[EScrnResolution];
		break;
	case Engine::EMonsterSTATbar_Speed:
		m_v4PlusEdge = stURI.v4Speed_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Speed_Bar_EdgeSize[EScrnResolution];

		break;
	case Engine::EMonsterSTATbar_Paralysis:
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Paralysis_Bar_EdgeSize[EScrnResolution];
		m_fMaxPlayerStat = ParalysisMaxStat;
		break;

	case Engine::EMonsterSTATbar_EXP:
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EXP_Bar_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.fPosX = WINCX * FHALF;
		m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution] + stURI.fEXP_Bar_IntervalPosX[EScrnResolution];
		m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution] - ((stURI.fSizeY[EScrnResolution] + stURI.fIntervalPosY[EScrnResolution]));
		break;
	case /*Engine::EMonsterSTATbar_End*/11:
		break;
	default:
		break;
	}
	return S_OK;
}

CMonsterStat_UI_Bar::CMonsterStat_UI_Bar(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CMonsterStat_UI_Bar::CMonsterStat_UI_Bar(const CMonsterStat_UI_Bar & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CMonsterStat_UI_Bar::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CMonsterStat_UI_Bar::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	ITEM_INFO* tStatInfo = static_cast<ITEM_INFO*>(_pArg);
	wstring wstrStatType = L"Proto_Texture_";
	m_eStatType = (EMonsterSTATbar)tStatInfo->iPosition;

	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution] + ((stURI.fSizeY[EScrnResolution] + stURI.fIntervalPosY[EScrnResolution]) *tStatInfo->iPosition);
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_fGlowSizeXY = { stURI.fGlowSizeXY[EScrnResolution],stURI.fGlowSizeXY[EScrnResolution] };
	m_tORTHOUI_INFO.fAlpha = StatUINOMALALPHA;
	m_tORTHOUI_INFO.fEdgeAlpha = StatUINOMALALPHA;
	m_tORTHOUI_INFO.bUpdate = false;
	m_tORTHOUI_INFO.bRender = false;
	m_fStatUp_CoolTime = StatUpCoolTime;
	switch (m_eStatType)
	{
	case Engine::EMonsterSTATbar_HP:
		wstrStatType += L"SummaryStat_Health";
		//							  	 왼쪽   위   오른쪽  아래
		m_v4PlusEdge = stURI.v4Hp_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Hp_Bar_EdgeSize[EScrnResolution];
		break;
	case Engine::EMonsterSTATbar_Stamina:
		wstrStatType += L"SummaryStat_Stamina";
		m_v4PlusEdge = stURI.v4Stamina_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Stamina_Bar_EdgeSize[EScrnResolution];

		break;
	case Engine::EMonsterSTATbar_Food:
		wstrStatType += L"SummaryStat_Food";
		m_v4PlusEdge = stURI.v4Food_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Food_Bar_EdgeSize[EScrnResolution];

		break;
	case Engine::EMonsterSTATbar_Weight:
		wstrStatType += L"SummaryStat_Weight";
		m_v4PlusEdge = stURI.v4Weight_Bar_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Weight_Bar_EdgeSize[EScrnResolution];

		break;
	case Engine::EMonsterSTATbar_Atk:
		wstrStatType += L"Melee";
		m_v4PlusEdge = stURI.v4Atk_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Atk_Bar_EdgeSize[EScrnResolution];
		break;
	case Engine::EMonsterSTATbar_Speed:
		wstrStatType += L"Speed";
		m_v4PlusEdge = stURI.v4Speed_Plus_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Speed_Bar_EdgeSize[EScrnResolution];

		break;
	case Engine::EMonsterSTATbar_Paralysis:
		wstrStatType += L"Torpid";
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4Paralysis_Bar_EdgeSize[EScrnResolution];
		m_fMaxPlayerStat = ParalysisMaxStat;
		break;

	case Engine::EMonsterSTATbar_EXP:
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EXP_Bar_EdgeSize[EScrnResolution];
		m_tORTHOUI_INFO.fPosX = WINCX * FHALF;
		m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution] + stURI.fEXP_Bar_IntervalPosX[EScrnResolution];
		m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution] - ((stURI.fSizeY[EScrnResolution] + stURI.fIntervalPosY[EScrnResolution]));
		break;
	case /*Engine::EMonsterSTATbar_End*/11:
		break;
	default:
		break;
	}

	if (EMonsterSTATbar::EMonsterSTATbar_EXP != m_eStatType)
	{
		CComponent*		pComponent = nullptr;
		pComponent = m_pStatTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(wstrStatType));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);
	}

	return S_OK;
}

_int CMonsterStat_UI_Bar::Update_GameObject(const _double & _dTimeDelta)
{
	m_pMonster = static_cast<CMonsterStat_UI*>(Get_GameObject(L"Center_UI", 1))->Get_pMonster();
	if (!m_pMonster)
		return NO_EVENT;
	COrthoUI::Update_GameObject(_dTimeDelta);
	return S_OK;
}

_int CMonsterStat_UI_Bar::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_pMonster)
		return NO_EVENT;

	COrthoUI::LateUpdate_GameObject(_dTimeDelta);
	if (m_tORTHOUI_INFO.bRender)
	{
		if (EMonsterSTATbar::EMonsterSTATbar_Paralysis != m_eStatType)
		{
			if (0 < m_pMonster->Get_Stat(CUnit::STAT_STATPOINT))
				m_bCanRiseStat = true;
			else
				m_bCanRiseStat = false;
			m_fStatUp_CoolTime += (_float)_dTimeDelta;
			m_tORTHOUI_INFO.uchSelectUI &= ~(MouseClickUI); //클릭 해줬다는 건 계속 지워줘야 함 (KeyDown같은 느낌)
			if (m_bCanRiseStat)
			{	///////////////////스텟이 찍을 게 있다면
				PlusMouseCheck(_dTimeDelta);
				if (0.5f > _dTimeDelta) //디버그 중단점시 개판나는 거 방지
				{
					_float GlowSizeFix = (_float)_dTimeDelta*GlowFixTimeRatio;
					if (m_tORTHOUI_INFO.iMasterdentifyNumber)
						m_fGlowSizeXY -= {GlowSizeFix, GlowSizeFix};
					else
						m_fGlowSizeXY += {GlowSizeFix, GlowSizeFix};

					if (stURI.fGlowMaxSizeXY[EScrnResolution] <= m_fGlowSizeXY.x)
						m_tORTHOUI_INFO.iMasterdentifyNumber = 1;
					else if (stURI.fGlowSizeXY[EScrnResolution] >= m_fGlowSizeXY.x)
						m_tORTHOUI_INFO.iMasterdentifyNumber = 0;
				}
				//////////////////////////////////
			}
		}
		_float fNowStat; //가지고 있던 now스텟을 잠깐동안 pre스텟으로 만들기 위해
		switch (m_eStatType)
		{
		case Engine::EMonsterSTATbar_HP:
			m_fMaxPlayerStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_MAXHP);
			fNowStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_HP);
			break;
		case Engine::EMonsterSTATbar_Stamina:
			m_fMaxPlayerStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_MAXSTAMINA);
			fNowStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_STAMINA);
			break;
		case Engine::EMonsterSTATbar_Food:
			m_fMaxPlayerStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_MAXHUNGER);
			fNowStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_HUNGER);
			break;
		case Engine::EMonsterSTATbar_Weight:
			m_fMaxPlayerStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_MAXWEIGHT);
			fNowStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_WEIGHT);
			break;
		case Engine::EMonsterSTATbar_Atk:
			if (!m_fMaxPlayerStat)
				m_fMaxPlayerStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_ATTACK);
			fNowStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_ATTACK);
			break;
		case Engine::EMonsterSTATbar_Speed:
			if (!m_fMaxPlayerStat)
				m_fMaxPlayerStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_SPEED);
			fNowStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_SPEED);
			break;
		case Engine::EMonsterSTATbar_Paralysis:
			fNowStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_PARALYSIS);
			break;
		case Engine::EMonsterSTATbar_EXP:
			m_fMaxPlayerStat = (_float)static_cast<CPlayer*>(m_pMonster)->Get_NextExp(); //<<>>
			fNowStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_EXP);
			break;
		case Engine::EMonsterSTATbar_End:
			break;
		default:
			break;
		}

		m_fNowPlayerStat = fNowStat;
		m_fPerPlayerStat = m_fNowPlayerStat / m_fMaxPlayerStat;
		if (EMonsterSTATbar::EMonsterSTATbar_Atk <= m_eStatType && EMonsterSTATbar::EMonsterSTATbar_EXP != m_eStatType)
			m_fPerPlayerStat *= 100.f; //100퍼센트로 나와야 하므로. 디파인 걸 필요는 없을 듯. 걍 100이니

		Engine::Add_RenderGroup(RENDER_ITEMUI, this);
		//렌더 중이라면
		PlayerStatControll();
	}
	return _int();
}

void CMonsterStat_UI_Bar::Render_GameObject(_mat _matOrtho)
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

	if (EMonsterSTATbar::EMonsterSTATbar_EXP > m_eStatType)
	{
		//////////////////////////
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, 1);
		pEffect->BeginPass(1);      ////그림
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
		////////////////////////////
		if (EMonsterSTATbar::EMonsterSTATbar_Paralysis > m_eStatType)
		{
			if (m_bCanRiseStat)
			{
				pEffect->CommitChanges();
				SetUp_ConstantTable(pEffect, _matOrtho, 3);
				pEffect->BeginPass(3);    ////글로리
				m_pBufferCom->Render_VIBuffer();
				pEffect->EndPass();
			}
			////////////////////////////
			pEffect->CommitChanges();
			SetUp_ConstantTable(pEffect, _matOrtho, 2);
			pEffect->BeginPass(2);  ////더하기 버튼
			m_pBufferCom->Render_VIBuffer();
			pEffect->EndPass();
			////////////////////////
		}
	}
	pEffect->End();

	Safe_Release(pEffect);

	Render_Font(_matOrtho);
}

HRESULT CMonsterStat_UI_Bar::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pPlusTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_PlusStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture2", pComponent);

	pComponent = m_pPanelTabTextGlowTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_PanelTabTextGlow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture3", pComponent);
	
	pComponent = m_pArrowUpTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_ArrowUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture4", pComponent);
	
	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Stat_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CMonsterStat_UI_Bar::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	STMonsterStat_UI_BarURI stURI;

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
	else if (2 == _bTwo)
	{
		matWorld._11 = stURI.fPlusIconSizeXY[EScrnResolution];
		matWorld._22 = stURI.fPlusIconSizeXY[EScrnResolution];
		matWorld._33 = FTRUE;

		matWorld._41 = (m_tORTHOUI_INFO.fPosX + stURI.fPlusIcon_IntervalPosX[EScrnResolution]) - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pPlusTextureCom->Set_Texture(_pEffect, "g_StatTexture");
		_pEffect->SetVector("g_EdgeSize", &m_v4PlusEdge);

		///////////찍을 스텟이 있을 때 또는 0.25 이하일 떄 쿨탐이 (마지막 스텟 찍었다는 뜻)
		if (FinalStatUpCoolTime > m_fStatUp_CoolTime) //마우스가 클릭 했으면
		{
			_pEffect->SetFloat("g_fStatUpCollTime", m_fStatUp_CoolTime);
			m_pArrowUpTextureCom->Set_Texture(_pEffect, "g_ArrowUpTexture");
			_pEffect->SetInt("g_iMouse", 2);
		}
		else if ((m_tORTHOUI_INFO.uchSelectUI & MouseOnUI)&& m_pMonster->Get_Stat(CUnit::STAT_STATPOINT)) //마우스가 올라왔으면
			_pEffect->SetInt("g_iMouse", 1);
		else
		{
			_pEffect->SetInt("g_iMouse", 0);
			_pEffect->SetBool("g_bCanRiseStat", m_bCanRiseStat);
		}
		////////////////////
	}
	else if (3 == _bTwo)
	{
		matWorld._11 = m_fGlowSizeXY.x;
		matWorld._22 = m_fGlowSizeXY.y;
		matWorld._33 = FTRUE;

		matWorld._41 = (m_tORTHOUI_INFO.fPosX + stURI.fGlow_IntervalPosX[EScrnResolution]) - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pPanelTabTextGlowTextureCom->Set_Texture(_pEffect, "g_GlowTexture");
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

void CMonsterStat_UI_Bar::Render_Font(_mat _matOrtho)
{
	STMonsterStat_UI_BarURI stURI;

	wstring wstrNum = L"";
	switch (m_eStatType)
	{
	case Engine::EMonsterSTATbar_HP:
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
	case Engine::EMonsterSTATbar_Stamina:
		wstrNum += to_wstring((_int)m_fNowPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fNowPlayerStat * 10.f)) - (((_int)m_fNowPlayerStat) * 10));
		wstrNum += L" / ";
		wstrNum += to_wstring((_int)m_fMaxPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fMaxPlayerStat * 10.f)) - (((_int)m_fMaxPlayerStat) * 10));
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Stamina_Stat_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_Stamina_Stat_IntervalPosY[EScrnResolution]), StatFontColor, 1);
		wstrNum = L"기력";
		break;
	case Engine::EMonsterSTATbar_Food:
		wstrNum += to_wstring((_int)m_fNowPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fNowPlayerStat * 10.f)) - (((_int)m_fNowPlayerStat) * 10));
		wstrNum += L" / ";
		wstrNum += to_wstring((_int)m_fMaxPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fMaxPlayerStat * 10.f)) - (((_int)m_fMaxPlayerStat) * 10));
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Food_Stat_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_Food_Stat_IntervalPosY[EScrnResolution]), StatFontColor, 1);
		wstrNum = L"음식";
		break;
	case Engine::EMonsterSTATbar_Weight:
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
	case Engine::EMonsterSTATbar_Atk:
		wstrNum += to_wstring((_int)m_fPerPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fPerPlayerStat * 10.f)) - (((_int)m_fPerPlayerStat) * 10));
		wstrNum += L"%";
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Atk_Stat_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_Atk_Stat_IntervalPosY[EScrnResolution]), StatFontColor, 1);
		wstrNum = L"근접 공격력";
		break;
	case Engine::EMonsterSTATbar_Speed:
		wstrNum += to_wstring((_int)m_fPerPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fPerPlayerStat * 10.f)) - (((_int)m_fPerPlayerStat) * 10));
		wstrNum += L"%";
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Speed_Stat_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_Speed_Stat_IntervalPosY[EScrnResolution]), StatFontColor, 1);
		wstrNum = L"속도";
		break;
	case Engine::EMonsterSTATbar_Paralysis:
		wstrNum += to_wstring((_int)m_fNowPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fNowPlayerStat * 10.f)) - (((_int)m_fNowPlayerStat) * 10));
		wstrNum += L" / ";
		wstrNum += to_wstring((_int)m_fMaxPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fMaxPlayerStat * 10.f)) - (((_int)m_fMaxPlayerStat) * 10));
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Paralysis_Stat_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_Paralysis_Stat_IntervalPosY[EScrnResolution]), StatFontColor, 1);
		wstrNum = L"마비수치";
		break;
	case Engine::EMonsterSTATbar_EXP:
		wstrNum = L"남은 포인트 ";
		wstrNum += to_wstring((_int)m_pMonster->Get_Stat(CUnit::STAT_STATPOINT));
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_EXP_Stat_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_EXP_Stat_IntervalPosY[EScrnResolution]), StatFontColor, 1);

		wstrNum = L"경험치(XP) : ";
		wstrNum += to_wstring((_int)m_fNowPlayerStat);
		wstrNum += L".";
		wstrNum += to_wstring(((_int)(m_fNowPlayerStat * 10.f)) - (((_int)m_fNowPlayerStat) * 10));
		wstrNum += L" / ";
		wstrNum += to_wstring((_int)m_fMaxPlayerStat);
		break;
	case Engine::EMonsterSTATbar_End:
		break;
	default:
		break;
	}
	if(EMonsterSTATbar::EMonsterSTATbar_HP == m_eStatType)
		Engine::Render_Font(L"Font_StatGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_Stat_Name_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_HP_Stat_IntervalPosY[EScrnResolution]), StatFontColor);
	else if (EMonsterSTATbar::EMonsterSTATbar_Speed > m_eStatType)
		Engine::Render_Font(L"Font_StatGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_Stat_Name_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_Stamina_Stat_IntervalPosY[EScrnResolution]), StatFontColor);
	else if (EMonsterSTATbar::EMonsterSTATbar_EXP > m_eStatType)
		Engine::Render_Font(L"Font_StatGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_Stat_Name_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_SpeedTypeDown_Stat_IntervalPosY[EScrnResolution]), StatFontColor);
	else
		Engine::Render_Font(L"Font_StatGuideSmall", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_Stat_EXP_Name_IntervalPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_EXP_Stat_IntervalPosY[EScrnResolution]), StatFontColor);

}

void CMonsterStat_UI_Bar::PlusMouseCheck(const _double & _dTimeDelta)
{
	//스텟 찍을 수 있는 상황에만 함수 들어옴
	//m_tORTHOUI_INFO.uchSelectUI &= ~(MouseClickUI); //클릭 해줬다는 건 계속 지워줘야 함 (KeyDown같은 느낌)
	//밖으로 빼줌 (마지막 스텟 찍었을 때를 위해, 이 함수를 스텟 찍을 수 있는 상황이 아닐떄도 돌리는건 부담이 ㅅ있을 듯
	if (EMonsterSTATbar::EMonsterSTATbar_Paralysis > m_eStatType)
	{
		if (m_tORTHOUI_INFO.bRender)
		{
			if (FinalStatUpCoolTime < m_fStatUp_CoolTime) //누르면 0되고 ㅇㅇ..
			{
				_float PlusPosX = (m_tORTHOUI_INFO.fPosX + stURI.fPlusIcon_IntervalPosX[EScrnResolution]);
				_float PlusPosY = m_tORTHOUI_INFO.fPosY;
				_float PlusSizeXY = stURI.fPlusIconSizeXY[EScrnResolution];

				Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
				_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;
				_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

				_float fHalfX = PlusSizeXY * FHALF;
				_float fHalfY = PlusSizeXY * FHALF;
				if (MouseX >= PlusPosX - fHalfX
					&& MouseX <= PlusPosX + fHalfX
					&& MouseY >= PlusPosY - fHalfY
					&& MouseY <= PlusPosY + fHalfY)
					m_tORTHOUI_INFO.uchSelectUI |= MouseOnUI;
				else
					m_tORTHOUI_INFO.uchSelectUI &= ~MouseOnUI;
				///////////////////////////// uchSelectUI의 첫번째 비트는 마우스가 올라왔는지 아닌지만

				if (MouseOnUI & m_tORTHOUI_INFO.uchSelectUI)
				{
					if (Engine::KeyDown(KeyCheck_Lbutton))
					{
						m_fStatUp_CoolTime = 0.f;
						m_pMonster->Set_Stat(CUnit::STAT_STATPOINT, (_int)-1);
						m_tORTHOUI_INFO.uchSelectUI |= MouseClickUI;
					}
				}
				//////////////////////////// uchSelectUI의 두번째 비트는 마우스가 올라왔을 때 클릭하면 On, 다음 프레임에서 Off
			}
		}
	}
}

void CMonsterStat_UI_Bar::PlayerStatControll()
{
	if (m_tORTHOUI_INFO.uchSelectUI & (MouseClickUI)) //클릭 했다면
	{
		switch (m_eStatType)
		{
		case Engine::EMonsterSTATbar_HP:
			m_pMonster->Set_Stat(CUnit::STAT_MAXHP, IncreasePlayerMaxHp);
			break;
		case Engine::EMonsterSTATbar_Stamina:
			m_pMonster->Set_Stat(CUnit::STAT_MAXSTAMINA, IncreasePlayerMaxStamina);
			break;
		case Engine::EMonsterSTATbar_Food:
			m_pMonster->Set_Stat(CUnit::STAT_MAXHUNGER, IncreasePlayerMaxFood);
			break;
		case Engine::EMonsterSTATbar_Weight:
			m_pMonster->Set_Stat(CUnit::STAT_MAXWEIGHT,  IncreasePlayerMaxWeight);
			break;
		case Engine::EMonsterSTATbar_Atk:
			m_pMonster->Set_Stat(CUnit::STAT_ATTACK, IncreasePlayerMaxAtk);
			break;
		case Engine::EMonsterSTATbar_Speed:
			m_pMonster->Set_Stat(CUnit::STAT_SPEED, IncreasePlayerMaxSpeed);
			break;
		case Engine::EMonsterSTATbar_Paralysis:
			break;
		case Engine::EMonsterSTATbar_End:
			break;
		default:
			break;
		}
	}
}

CMonsterStat_UI_Bar * CMonsterStat_UI_Bar::Create(LPDIRECT3DDEVICE9 _pDevice)
{

	CMonsterStat_UI_Bar*	pInstance = new CMonsterStat_UI_Bar(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CMonsterStat_UI_Bar::Clone(void * _pArg)
{
	CMonsterStat_UI_Bar*	pClone = new CMonsterStat_UI_Bar(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CMonsterStat_UI_Bar::Free()
{
	COrthoUI::Free();
}