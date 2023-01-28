#include "Font_Mgr.h"
#include "GraphicDevice.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFont_Mgr)

CFont_Mgr::CFont_Mgr()
{
}


CFont_Mgr::~CFont_Mgr()
{
	Free();
}

HRESULT CFont_Mgr::Ready_Font(LPDIRECT3DDEVICE9 _pDevice, const wstring & _wstrFontTag, const wstring & _wstrFontType, const _uint & _iWidth, const _uint & _iHeight, const _uint & _iWeight, const _uint& _iWinCX)
{
	CFont*		pFont = nullptr;

	pFont = Find_Font(_wstrFontTag, _iWinCX);
	if (nullptr != pFont)
		return E_FAIL;

	pFont = CFont::Create(_pDevice, _wstrFontType, _iWidth, _iHeight, _iWeight);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	auto iter = m_mapFont.find(_wstrFontTag);
	if (iter == m_mapFont.end())
	{
		map<_int, CFont*> mapInput;
		mapInput.emplace(_iWinCX, pFont);
		m_mapFont.emplace(_wstrFontTag, mapInput);
	}
	else
	{
		iter->second.emplace(_iWinCX,pFont);
	}

	return S_OK;
}

void CFont_Mgr::Render_Font(const wstring & _wstrFontTag, const wstring & _wstrString, const _vec2 * _pPos, D3DXCOLOR _Color, _int _iFontType)
{
	CFont*	pFont = Find_Font(_wstrFontTag, (_int)CGraphicDevice::GetInstance()->Get_WinCX());
	NULL_CHECK(pFont);

	pFont->Render_Font(_wstrString, _pPos, _Color, _iFontType);
}

void CFont_Mgr::Render_Font(const wstring & _wstrFontTag, const list<LEFTGUIDEFONT_INFO>& _vecGuideFont)
{
	CFont*	pFont = Find_Font(_wstrFontTag, (_int)CGraphicDevice::GetInstance()->Get_WinCX());
	NULL_CHECK(pFont);

	pFont->Render_Font(_vecGuideFont);
}

void CFont_Mgr::Render_Font(const wstring & _wstrGreenFontTag, const wstring & _wstrRedFontTag, const list<TOPGUIDEFONT_INFO>& _vecGuideFont, _int _iRedboundary)
{
	CFont*	pFont = Find_Font(_wstrGreenFontTag, (_int)CGraphicDevice::GetInstance()->Get_WinCX());
	NULL_CHECK(pFont);
	pFont->Render_Font(_vecGuideFont, _iRedboundary,true);

	pFont = Find_Font(_wstrRedFontTag, (_int)CGraphicDevice::GetInstance()->Get_WinCX());
	NULL_CHECK(pFont);
	pFont->Render_Font(_vecGuideFont, _iRedboundary,false);
}

CFont * CFont_Mgr::Find_Font(const wstring _wstrFontTag, const _uint& _iWinCX)
{
	auto iter = m_mapFont.find(_wstrFontTag);

	if (iter == m_mapFont.end())
		return nullptr;

	auto iiter = iter->second.find(_iWinCX);
	if (iiter == iter->second.end())
		return nullptr;

	return iiter->second;
}

void CFont_Mgr::Free()
{
	for (auto& pair : m_mapFont)
		for (auto& ppair : pair.second)
			Safe_Release(ppair.second);
	m_mapFont.clear();
}
