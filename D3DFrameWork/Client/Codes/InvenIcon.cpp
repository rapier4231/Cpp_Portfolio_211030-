#include "stdafx.h"
#include "..\Headers\InvenIcon.h"

#include "Export_Function.h"

USING(Engine)

CInvenIcon::CInvenIcon(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CInvenIcon::CInvenIcon(const CInvenIcon & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CInvenIcon::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CInvenIcon::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	return S_OK;
}

_int CInvenIcon::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);

	MouseCheck();
	if (m_bBoxSelect)
	{
		if ((MouseClickUI & m_tORTHOUI_INFO.uchSelectUI))
		{
			m_bBoxSelect = false;
			m_tORTHOUI_INFO.fAlpha = ICONMOUSEALPHA;
			m_tORTHOUI_INFO.fEdgeAlpha = ICONEDGEMOUSEALPHA;
		}
	}
	else
	{
		if (!(MouseOnUI & m_tORTHOUI_INFO.uchSelectUI))
		{
			m_bBoxSelect = false;
			m_tORTHOUI_INFO.fAlpha = ICONNOMALALPHA;
			m_tORTHOUI_INFO.fEdgeAlpha = ICONEDGENOMALALPHA;
		}
		if ((MouseOnUI & m_tORTHOUI_INFO.uchSelectUI))
		{
			m_bBoxSelect = false;
			m_tORTHOUI_INFO.fAlpha = ICONMOUSEALPHA;
			m_tORTHOUI_INFO.fEdgeAlpha = ICONEDGEMOUSEALPHA;
		}
		if ((MouseClickUI & m_tORTHOUI_INFO.uchSelectUI))
		{
			m_bBoxSelect = true;
			m_tORTHOUI_INFO.fAlpha = ICONSELECTALPHA;
			m_tORTHOUI_INFO.fEdgeAlpha = ICONEDGESELECALPHA;
		}
	}

	return S_OK;
}

_int CInvenIcon::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	return _int();
}

void CInvenIcon::Render_GameObject(_mat _matOrtho)
{

}

HRESULT CInvenIcon::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	
	return S_OK;
}

void CInvenIcon::Free()
{

	COrthoUI::Free();
}

void CInvenIcon::Set_LeftInvenIcon(_bool _LeftInvenIcon)
{
	m_bLeftInven = _LeftInvenIcon;
}
