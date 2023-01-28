#pragma once
#ifndef __FONT_MGR_H__
#define __FONT_MGR_H__

#include "Engine_Define.h"
#include "Base.h"
#include "Font.h"

BEGIN(Engine)

class ENGINE_DLL CFont_Mgr : public CBase
{
	DECLARE_SINGLETON(CFont_Mgr)
private:
	explicit CFont_Mgr();
	virtual ~CFont_Mgr();

public:
	HRESULT		Ready_Font(LPDIRECT3DDEVICE9 _pDevice,
							const wstring& _wstrFontTag,
							const wstring& _wstrFontType,
							const _uint& _iWidth,
							const _uint& _iHeight,
							const _uint& _iWeight,
							const _uint& _iWinCX);

	void		Render_Font(const wstring& _wstrFontTag,
							const wstring& _wstrString,
							const _vec2* _pPos,
							D3DXCOLOR _Color, _int _iFontType = 0);

	void				Render_Font(const wstring& _wstrFontTag, const list<LEFTGUIDEFONT_INFO>& _vecGuideFont);
	void				Render_Font(const wstring& _wstrGreenFontTag, const wstring& _wstrRedFontTag, const list<TOPGUIDEFONT_INFO>& _vecGuideFont, _int _iRedboundary);
public:
	CFont*				Find_Font(const wstring _wstrFontTag, const _uint& _iWinCX);
public:
	virtual	void Free();

private:
	map<wstring,map<_int ,CFont*>>		m_mapFont;
};
END
#endif // !__FONT_MGR_H__
