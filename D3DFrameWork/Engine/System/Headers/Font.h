#ifndef __FONT_H__
#define __FONT_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFont : public CBase
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CFont() = default;

public:
	HRESULT				Ready_Font(	const wstring& _wstrFontType,
									const _uint& _iWidth,
									const _uint& _iHeight,
									const _uint& _iWeight);

	void				Render_Font(const wstring& _wstrString,
									const _vec2* _pPos,
									D3DXCOLOR _Color , _int _iFontType = 0);

public:
	static CFont*		Create(		LPDIRECT3DDEVICE9 _pDevice,
									const wstring& _pFontType,
									const _uint& _iWidth,
									const _uint& _iHeight,
									const _uint& _iWeight);

public:
	void				Render_Font(const list<LEFTGUIDEFONT_INFO>& _vecGuideFont);
	void				Render_Font(const list<TOPGUIDEFONT_INFO>& _vecGuideFont, _int _iRedboundary, _bool _bGreenGuide);

private:
	virtual void Free();

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPD3DXFONT				m_pFont;
	LPD3DXSPRITE			m_pSprite;
};
END
#endif // !__FONT_H__
