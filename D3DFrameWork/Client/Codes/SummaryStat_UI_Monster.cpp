#include "stdafx.h"
#include "..\Headers\SummaryStat_UI_Monster.h"
#include "Export_Function.h"
#include "Unit.h"
#include "GuideFont_UI.h"
#include "SummaryStat_UI.h"

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

HRESULT CSummaryStat_UI_Monster::Set_Screen_UI()
{
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution] - (((_int)m_eSummaryType * stURI.fIntervalPosY[EScrnResolution]));
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	return S_OK;
}

CSummaryStat_UI_Monster::CSummaryStat_UI_Monster(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CSummaryStat_UI_Monster::CSummaryStat_UI_Monster(const CSummaryStat_UI_Monster & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CSummaryStat_UI_Monster::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CSummaryStat_UI_Monster::Ready_GameObject(void * _pArg/* = nullptr*/)
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
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution] + ((static_cast<ITEM_INFO*>(_pArg)->iPosition * stURI.fIntervalPosY[EScrnResolution])) ;
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = SUMMARYSTATNOMALALPHA;
	m_tORTHOUI_INFO.bUpdate = false;
	m_tORTHOUI_INFO.bRender = false;

	m_eSummaryType = (ESummaryStat_Monster)(static_cast<ITEM_INFO*>(_pArg)->iPosition);
	if (ESummaryStat_Monster::SummaryStat_XP == m_eSummaryType)
		m_iShaderPass = 3;
	return S_OK;
}

_int CSummaryStat_UI_Monster::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_pRidingMonster)
		return NO_EVENT;

	COrthoUI::Update_GameObject(_dTimeDelta);

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

_int CSummaryStat_UI_Monster::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_pRidingMonster)
		return NO_EVENT;

	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	_float fNowStat = 0.f;
	switch (m_eSummaryType)
	{
		
	case CSummaryStat_UI_Monster::SummaryStat_Health:
		m_fMaxMonsterStat = (_float)m_pRidingMonster->Get_Stat(CUnit::STAT_MAXHP);
		fNowStat = (_float)m_pRidingMonster->Get_Stat(CUnit::STAT_HP);
		if (!m_bBlinkChange)
		{
			if (fNowStat < m_fNowMonsterStat)
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
		m_fNowMonsterStat = fNowStat;
		m_fPerMonsterStat = m_fNowMonsterStat / m_fMaxMonsterStat;
		break;
	case CSummaryStat_UI_Monster::SummaryStat_Stamina:
		m_fMaxMonsterStat = (_float)m_pRidingMonster->Get_Stat(CUnit::STAT_MAXSTAMINA);
		fNowStat = (_float)m_pRidingMonster->Get_Stat(CUnit::STAT_STAMINA);
		if (!m_bBlinkChange)
		{
			if (Stamina_Warning_Ratio > m_fPerMonsterStat)
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
		m_fNowMonsterStat = fNowStat;
		m_fPerMonsterStat = m_fNowMonsterStat / m_fMaxMonsterStat;
		break;
	case CSummaryStat_UI_Monster::SummaryStat_Food:
		m_fMaxMonsterStat = (_float)m_pRidingMonster->Get_Stat(CUnit::STAT_MAXHUNGER);
		fNowStat = (_float)m_pRidingMonster->Get_Stat(CUnit::STAT_HUNGER);
		m_fNowMonsterStat = fNowStat;
		m_fPerMonsterStat = m_fNowMonsterStat / m_fMaxMonsterStat;
		if (!m_bBlinkChange)
		{
			if (Food_Warning_Ratio > m_fPerMonsterStat)
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
		break;
	case CSummaryStat_UI_Monster::SummaryStat_Weight:
		m_fMaxMonsterStat = (_float)m_pRidingMonster->Get_Stat(CUnit::STAT_MAXWEIGHT);
		fNowStat = (_float)m_pRidingMonster->Get_Stat(CUnit::STAT_WEIGHT);
		m_fNowMonsterStat = fNowStat; //얘는 어느정도 차면 깜빡이는거로 하자
		m_fPerMonsterStat = m_fNowMonsterStat / m_fMaxMonsterStat;
		if (!m_bBlinkChange)
		{
			if (Weight_Warning_Ratio < m_fPerMonsterStat)
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
		break;
	case CSummaryStat_UI_Monster::SummaryStat_XP:
		m_fMaxMonsterStat = FNULL; //남은 스텟 포인트는 없어야 하므로
		m_fNowMonsterStat = (_float)m_pRidingMonster->Get_Stat(CUnit::STAT_STATPOINT);
		if (m_fNowMonsterStat)
		{
			m_tORTHOUI_INFO.bRender = true;
		}
		else
		{
			m_tORTHOUI_INFO.bRender = false;
			m_dBlinkTime = BlinkTrueTime1;
		}

		if (m_tORTHOUI_INFO.bRender)
		{
			m_dBlinkTime += _dTimeDelta;

			if (XP_BlinkTime1 <= m_dBlinkTime && XP_BlinkTime2 > m_dBlinkTime)
				m_tORTHOUI_INFO.fAlpha = SUMMARYSTATDANGEROUSALPHA;
			if (XP_BlinkTime2 <= m_dBlinkTime)
			{
				m_tORTHOUI_INFO.fAlpha = SUMMARYSTATNOMALALPHA;
				m_dBlinkTime = BlinkTrueTime1;
			}

		}
		break;
	}

	if (m_tORTHOUI_INFO.bRender)
		Engine::Add_RenderGroup(RENDER_ORTHOBACKUI,this);

	return _int();
}

void CSummaryStat_UI_Monster::Render_GameObject(_mat _matOrtho)
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


CSummaryStat_UI_Monster * CSummaryStat_UI_Monster::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CSummaryStat_UI_Monster*	pInstance = new CSummaryStat_UI_Monster(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CSummaryStat_UI_Monster::Clone(void * _pArg/* = nullptr*/)
{
	CSummaryStat_UI_Monster*	pClone = new CSummaryStat_UI_Monster(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CSummaryStat_UI_Monster::Add_Component(void)
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

HRESULT CSummaryStat_UI_Monster::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect , _mat _matOrtho)
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

	_pEffect->SetFloat("g_PercentStat", (FTRUE - m_fPerMonsterStat)); //UV의 V값은 반대니까
	_pEffect->SetBool("g_Blink", m_bBlink);
	return S_OK;
}

void CSummaryStat_UI_Monster::Free()
{

	COrthoUI::Free();
}

void CSummaryStat_UI_Monster::Set_RideMonster(CUnit * _pDino)
{
	if (_pDino)
		m_tORTHOUI_INFO.bRender = true;
	else
		m_tORTHOUI_INFO.bRender = false;

	m_pRidingMonster = _pDino;
	m_bBlink = false;
	m_bBlinkChange = false;
	m_dBlinkTime = FNULL;
	m_fMaxMonsterStat = FNULL;
	m_fNowMonsterStat = FNULL;
	m_fPerMonsterStat = FNULL;
}

_bool CSummaryStat_UI_Monster::UI_RenderOn()
{
	return COrthoUI::UI_RenderOn();
}
