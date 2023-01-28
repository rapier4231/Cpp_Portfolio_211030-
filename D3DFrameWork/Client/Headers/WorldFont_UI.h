#pragma once
#ifndef __WorldFont_UI_H__
#define __WorldFont_UI_H__

#include "Value.h"
#include "OrthoUI.h"


class CWorldFont_UI : public COrthoUI
{
private:
	virtual HRESULT			Set_Screen_UI()override;


public:
	enum  EWorld_Font_Color { World_Font_Red, World_Font_Green, World_Font_Yellow, World_Font_Translucent_Red, World_Font_Translucent_Green, World_Font_Translucent_Yellow, World_Font_Big_Blue, World_Font_Big_Green, World_Font_Color_END };
	enum  EWorld_FontType { World_FontType_Fix, World_FontType_Up, World_FontType_Follow, World_FontType_END };

	typedef struct tWorldFontInfo
	{
		const _vec3*			m_pvWorldPos = nullptr;
		_vec2					m_v2AddScreenPos = {};
		_vec3					m_vPrintWorldPos = {};
		_vec3					m_vAddPos = {};
		_vec4					m_v4FontColor = {};
		wstring					m_wstrFont = L"";
		EWorld_FontType			m_eFontType = World_FontType_END;
		EWorld_Font_Color		m_eFontColor = World_Font_Color_END;
		_float					m_fTime = 0.f;
		_int					m_iWF_ID = 0;
		_bool					m_bInfiniteShow = false;
	}WFINFO;
private:
	explicit CWorldFont_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CWorldFont_UI(const CWorldFont_UI& rhs);
	virtual ~CWorldFont_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) { return; };
	virtual void			Render_GameObject(_mat _matOrtho, _mat matOldProj) override;

public:
	static	CWorldFont_UI*   Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	_int					Print_WorldFont_UI(const _vec3* _pWorldPos, const wstring& _wstrFont, EWorld_Font_Color _eColor, EWorld_FontType _eFontType, const _vec3& _vAddPos = _vec3{0.f, 0.f, 0.f }, _float _fTime = 3.f, _vec2 _v2AddSrceenPos = _vec2{0.f, 0.f});
	_bool					Remove_WorldFont_UI(_int _iFontNum);
	_int					Get_Font_ID() {return m_tWorldFontInfo.m_iWF_ID;}
	void					Dead_Font() {m_tWorldFontInfo.m_fTime = 0.f; m_tWorldFontInfo.m_bInfiniteShow = false;}
private:
	virtual HRESULT			Add_Component(void) override;
private:
	_vec2					ChangePos_World_To_Screen(_mat matOldProj);
	void					Fix_Font_Pos(const _double& _dTimeDelta);
	void					Render_Font(_mat _matOrtho, _vec2 _v2ScreenPos);

private:
	WFINFO					m_tWorldFontInfo;
	_bool					m_bWFClone = false;
	_int					m_iCteateWorldFontNum = 0;

};

#endif // !__WorldFont_UI_H__