#pragma once
#ifndef __OrthoUI_H__
#define __OrthoUI_H__

#include "UI.h"

#define MouseOnUI		(1 << 0)
#define MouseClickUI	(1 << 1)
#define MousePressing	(1 << 2)

class COrthoUI abstract : public CUI
{
protected:
	typedef struct stOrthoUI_Resolution_Info
	{

	}STOURI;
	virtual HRESULT			Set_Screen_UI() PURE;

protected:
	explicit COrthoUI(LPDIRECT3DDEVICE9 _pDevice);
	explicit COrthoUI(const COrthoUI& rhs);
	virtual ~COrthoUI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() PURE;
	virtual HRESULT			Ready_GameObject(void* _pArg = nullptr) PURE;
	virtual _int			Update_GameObject(const _double& _dTimeDelta) PURE;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) PURE;
	virtual void			Render_GameObject()override;
	virtual void			Render_GameObject(_mat _matOrtho) PURE;

public:
	void					Set_IdentifyNumber(_int _iNum);
	ORTHOUI_INFO*			Get_ORTHOUI_INFO();

public:
	void					Plus_Pos_UI(_float _fPlusX, _float _fPlusY);
	void					Set_Pos_UI(_float _fPlusX, _float _fPlusY);

public:
	_bool					ExecutionUpdateCheck();

public:
	_bool					Question_Now_MouseIsUrRange();

public:
	virtual	_bool			UI_RenderOn();
	virtual	_bool			UI_RenderOff();

protected:
	void					MouseCheck();
	virtual HRESULT			Add_Component(void) PURE;

protected:
	ORTHOUI_INFO			m_tORTHOUI_INFO;

public:
	virtual CUI*			Clone(void* _pArg = nullptr) PURE;

public:
	virtual void			Free() PURE;
};

#endif // !__OrthoUI_H__
