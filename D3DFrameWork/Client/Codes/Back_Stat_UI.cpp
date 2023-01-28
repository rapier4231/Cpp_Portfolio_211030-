#include "stdafx.h"
#include "..\Headers\Back_Stat_UI.h"
#include "Export_Function.h"

#include "Border.h"

//#define UISTATSIZEY 450.f
#define UISTATTEXRATIOX	0.3f
#define UISTATTEXRATIOY	0.8f

USING(Engine)

HRESULT CBack_Stat_UI::Set_Screen_UI()
{
	CCenter_Back_UI::Set_Screen_UI();
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	return S_OK;
}

CBack_Stat_UI::CBack_Stat_UI(LPDIRECT3DDEVICE9 _pDevice)
	: CCenter_Back_UI(_pDevice)
{
}

CBack_Stat_UI::CBack_Stat_UI(const CBack_Stat_UI & rhs)
	: CCenter_Back_UI(rhs)
{
}

HRESULT CBack_Stat_UI::Ready_GameObject_Prototype()
{
	CCenter_Back_UI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBack_Stat_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CCenter_Back_UI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	m_fTexRatioX = UISTATTEXRATIOX;
	m_fTexRatioY = UISTATTEXRATIOY;

	return S_OK;
}

_int CBack_Stat_UI::Update_GameObject(const _double & _dTimeDelta)
{
	CCenter_Back_UI::Update_GameObject(_dTimeDelta);

	return S_OK;
}

_int CBack_Stat_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CCenter_Back_UI::LateUpdate_GameObject(_dTimeDelta);

	return _int();
}

void CBack_Stat_UI::Render_GameObject(_mat _matOrtho)
{
	CCenter_Back_UI::Render_GameObject(_matOrtho);
}


CBack_Stat_UI * CBack_Stat_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CBack_Stat_UI*	pInstance = new CBack_Stat_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CBack_Stat_UI::Clone(void * _pArg/* = nullptr*/)
{
	CBack_Stat_UI*	pClone = new CBack_Stat_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CBack_Stat_UI::Add_Component(void)
{
	return CCenter_Back_UI::Add_Component();
}

HRESULT CBack_Stat_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect , _mat _matOrtho)
{
	return CCenter_Back_UI::SetUp_ConstantTable(_pEffect, _matOrtho);
}

void CBack_Stat_UI::Center_Back_UIOpenAni(const _double & _dTimeDelta)
{
	if (m_bCenter_Back_UIOpenAni)
	{
		m_tORTHOUI_INFO.v4EdgeSize.x -= (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.v4EdgeSize.y -= (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.v4EdgeSize.z += (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.v4EdgeSize.w += (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.fEdgeAlpha += (_float)(_dTimeDelta)* FHALF;

		if (stURI.v4EdgeSize[EScrnResolution].x >= m_tORTHOUI_INFO.v4EdgeSize.x)
			m_tORTHOUI_INFO.v4EdgeSize.x = stURI.v4EdgeSize[EScrnResolution].x;
		if (stURI.v4EdgeSize[EScrnResolution].y >= m_tORTHOUI_INFO.v4EdgeSize.y)
			m_tORTHOUI_INFO.v4EdgeSize.y = stURI.v4EdgeSize[EScrnResolution].y;
		if (stURI.v4EdgeSize[EScrnResolution].z <= m_tORTHOUI_INFO.v4EdgeSize.z)
			m_tORTHOUI_INFO.v4EdgeSize.z = stURI.v4EdgeSize[EScrnResolution].z;
		if (stURI.v4EdgeSize[EScrnResolution].w <= m_tORTHOUI_INFO.v4EdgeSize.w)
			m_tORTHOUI_INFO.v4EdgeSize.w = stURI.v4EdgeSize[EScrnResolution].w;
		if (Center_Back_UIEDGEALPHA <= m_tORTHOUI_INFO.fEdgeAlpha)
			m_tORTHOUI_INFO.fEdgeAlpha = Center_Back_UIEDGEALPHA;

		if (stURI.v4EdgeSize[EScrnResolution].x == m_tORTHOUI_INFO.v4EdgeSize.x &&stURI.v4EdgeSize[EScrnResolution].y == m_tORTHOUI_INFO.v4EdgeSize.y && stURI.v4EdgeSize[EScrnResolution].z == m_tORTHOUI_INFO.v4EdgeSize.z&& stURI.v4EdgeSize[EScrnResolution].w == m_tORTHOUI_INFO.v4EdgeSize.w
			&&Center_Back_UIEDGEALPHA == m_tORTHOUI_INFO.fEdgeAlpha)
			m_bCenter_Back_UIOpenAni = false;
	}
}

void CBack_Stat_UI::Free()
{

	CCenter_Back_UI::Free();
}

_bool CBack_Stat_UI::UI_RenderOn()
{
	return CCenter_Back_UI::UI_RenderOn();
}

void CBack_Stat_UI::Back_Stat_UIOpen()
{
	CCenter_Back_UI::Center_Back_UIOpen();
}
