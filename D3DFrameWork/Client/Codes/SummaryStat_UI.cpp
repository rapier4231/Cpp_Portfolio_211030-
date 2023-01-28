#include "stdafx.h"
#include "..\Headers\SummaryStat_UI.h"
#include "Export_Function.h"
#include "Unit.h"
#include "GuideFont_UI.h"
#include "Player.h"

//#ifndef ICONEDGESIZELEFT
//#define ICONEDGESIZELEFT 0.03f
//#define ICONEDGESIZERIGHT 0.97f
//#define ICONEDGESIZETOP 0.03f
//#define ICONEDGESIZEBOTTOM 0.97f
//#endif
//
//#define SUMMARYSTATPOSX 780.f
//#define SUMMARYSTATSIZEX 30.f
//#define SUMMARYSTATSIZEY 30.f



USING(Engine)

HRESULT CSummaryStat_UI::Set_Screen_UI()
{
	STSummaryStat_UIURI stURI;

	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution] - (((_int)m_eSummaryType * stURI.fIntervalPosY[EScrnResolution]));
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	return S_OK;
}

CSummaryStat_UI::CSummaryStat_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CSummaryStat_UI::CSummaryStat_UI(const CSummaryStat_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CSummaryStat_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CSummaryStat_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);
	wstring wstrTexName = L"Proto_Texture_SummaryStat_";
	wstrTexName = wstrTexName + (static_cast<ITEM_INFO*>(_pArg))->tchName;

	CComponent*		pComponent = nullptr;
	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(wstrTexName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	wstrTexName = wstrTexName + L"_Fill";
	pComponent = m_pFill_TextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(wstrTexName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Fill_Texture", pComponent);

	////////////////////////////////////////////////////

	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution] - ((static_cast<ITEM_INFO*>(_pArg)->iPosition * stURI.fIntervalPosY[EScrnResolution])) ;
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = SUMMARYSTATNOMALALPHA;
	m_tORTHOUI_INFO.bUpdate = true;
	m_tORTHOUI_INFO.bRender = true;

	m_eSummaryType = (ESummaryStat)(static_cast<ITEM_INFO*>(_pArg)->iPosition);
	if (ESummaryStat::SummaryStat_XP == m_eSummaryType)
		m_iShaderPass = 3;
	return S_OK;
}

_int CSummaryStat_UI::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);

	if (!m_pPlayer)
	{
		m_pPlayer = static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player"));
	}

	if (m_bBlinkChange)
	{
		m_dBlinkTime += _dTimeDelta;
		
		if (BlinkTrueTime1 <= m_dBlinkTime&& BlinkFalseTime2 > m_dBlinkTime)
			m_bBlink = true;
		else if(BlinkFalseTime2 <= m_dBlinkTime&& BlinkTrueTime3 > m_dBlinkTime)
			m_bBlink = false;
		else if (BlinkTrueTime3 <= m_dBlinkTime&& BlinkFalseTime4 > m_dBlinkTime)
			m_bBlink = true;
		else if (BlinkFalseTime4 <= m_dBlinkTime&& BlinkTrueTime5 > m_dBlinkTime)
			m_bBlink = false;
		else
			m_bBlinkChange = false;
	}
	return S_OK;
}

_int CSummaryStat_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	_float fNowStat = 0.f;
	switch (m_eSummaryType)
	{
		
	case CSummaryStat_UI::SummaryStat_Health:
		m_fMaxPlayerStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_MAXHP);
		fNowStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_HP);
		if (!m_bBlinkChange)
		{
			if (fNowStat < m_fNowPlayerStat)
			{
				m_dBlinkTime = BlinkTrueTime1;
				m_bBlinkChange = true;
			}
			else
			{
				m_bBlink = false;
				m_bBlinkChange = false;
			}
		}
		m_fNowPlayerStat = fNowStat;
		m_fPerPlayerStat = m_fNowPlayerStat / m_fMaxPlayerStat;
		if(Health_Warning_Ratio > m_fPerPlayerStat)
			static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Red_LessHealth);
		break;
	case CSummaryStat_UI::SummaryStat_Stamina:
		m_fMaxPlayerStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_MAXSTAMINA);
		fNowStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_STAMINA);
		if (!m_bBlinkChange)
		{
			if (Stamina_Warning_Ratio > m_fPerPlayerStat)
			{
				m_dBlinkTime = BlinkTrueTime1;
				m_bBlinkChange = true;
			}
			else
			{
				m_bBlink = false;
				m_bBlinkChange = false;
			}
		}
		m_fNowPlayerStat = fNowStat;
		m_fPerPlayerStat = m_fNowPlayerStat / m_fMaxPlayerStat;
		if (Stamina_Warning_Ratio > m_fPerPlayerStat)
			static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Red_LessStamina);
		break;
	case CSummaryStat_UI::SummaryStat_Food:
		m_fMaxPlayerStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_MAXHUNGER);
		fNowStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_HUNGER);
		if (!m_bBlinkChange)
		{
			if (Food_Warning_Ratio > m_fPerPlayerStat)
			{
				m_dBlinkTime = BlinkTrueTime1;
				m_bBlinkChange = true;
				static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Red_LessHunger);
			}
			else
			{
				m_bBlink = false;
				m_bBlinkChange = false;
			}
		}
		m_fNowPlayerStat = fNowStat;
		m_fPerPlayerStat = m_fNowPlayerStat / m_fMaxPlayerStat;
		break;
	case CSummaryStat_UI::SummaryStat_Water:
		m_fMaxPlayerStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_MAXTHIRST);
		fNowStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_THIRST);
		if (!m_bBlinkChange)
		{
			if (fNowStat < m_fNowPlayerStat)
			{
				m_dBlinkTime = BlinkTrueTime1;
				m_bBlinkChange = true;
			}
			else
			{
				m_bBlink = false;
				m_bBlinkChange = false;
			}
		}
		m_fNowPlayerStat = fNowStat;
		m_fPerPlayerStat = m_fNowPlayerStat / m_fMaxPlayerStat;
		break;
	case CSummaryStat_UI::SummaryStat_Weight:
		m_fMaxPlayerStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_MAXWEIGHT);
		fNowStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_WEIGHT);
		m_fNowPlayerStat = fNowStat; //얘는 어느정도 차면 깜빡이는거로 하자
		m_fPerPlayerStat = m_fNowPlayerStat / m_fMaxPlayerStat;
		if (!m_bBlinkChange)
		{
			if (Weight_Warning_Ratio < m_fPerPlayerStat)
			{
				m_dBlinkTime = BlinkTrueTime1;
				m_bBlinkChange = true;
				static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Red_OverWeight);
			}
			else
			{
				m_bBlink = false;
				m_bBlinkChange = false;
			}
		}
		break;
	case CSummaryStat_UI::SummaryStat_XP:
		m_fMaxPlayerStat = (_float)static_cast<CPlayer*>(m_pPlayer)->Get_NextExp();
		m_fNowPlayerStat = (_float)m_pPlayer->Get_Stat(CUnit::STAT_EXP);

		if ((_int)m_pPlayer->Get_Stat(CUnit::STAT_STATPOINT))
		{
			//m_tORTHOUI_INFO.bRender = true;
			m_fPerPlayerStat = 1.f;
			m_dBlinkTime += _dTimeDelta;

			if (XP_BlinkTime1 <= m_dBlinkTime && XP_BlinkTime2 > m_dBlinkTime)
				m_tORTHOUI_INFO.fAlpha = SUMMARYSTATDANGEROUSALPHA;
			if (XP_BlinkTime2 <= m_dBlinkTime)
			{
				m_tORTHOUI_INFO.fAlpha = SUMMARYSTATNOMALALPHA;
				m_dBlinkTime = BlinkTrueTime1;
			}

			static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Green_StatUp);
		}
		else
		{
			//m_tORTHOUI_INFO.bRender = false;
			m_fPerPlayerStat = m_fNowPlayerStat / m_fMaxPlayerStat;
			m_dBlinkTime = BlinkTrueTime1;
		}
		break;
	}

	if (m_tORTHOUI_INFO.bRender)
		Engine::Add_RenderGroup(RENDER_ORTHOBACKUI,this);

	return _int();
}

void CSummaryStat_UI::Render_GameObject(_mat _matOrtho)
{
	if (!m_tORTHOUI_INFO.bRender)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 4;

	pEffect->Begin(&iPassMax, 0);

	pEffect->CommitChanges();

	pEffect->BeginPass(m_iShaderPass);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}


CSummaryStat_UI * CSummaryStat_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CSummaryStat_UI*	pInstance = new CSummaryStat_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CSummaryStat_UI::Clone(void * _pArg/* = nullptr*/)
{
	CSummaryStat_UI*	pClone = new CSummaryStat_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CSummaryStat_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_InvenIcon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);
	
	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CSummaryStat_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect , _mat _matOrtho)
{
	m_pTextureCom->Set_Texture(_pEffect, "g_BaseTexture");
	m_pFill_TextureCom->Set_Texture(_pEffect, "g_BoxTexture");

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

	_pEffect->SetFloat("g_PercentStat", (FTRUE - m_fPerPlayerStat)); //UV의 V값은 반대니까
	_pEffect->SetBool("g_Blink", m_bBlink);
	return S_OK;
}

void CSummaryStat_UI::Free()
{

	COrthoUI::Free();
}

_bool CSummaryStat_UI::UI_RenderOn()
{
	return COrthoUI::UI_RenderOn();
}
