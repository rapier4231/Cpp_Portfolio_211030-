#pragma once
#ifndef __UI_H__
#define __UI_H__

#include "GameObject.h"

#define MouseOnUI		(1 << 0)
#define MouseClickUI	(1 << 1)
#define MousePressing	(1 << 2)

class CUI abstract : public CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() PURE;
	virtual HRESULT			Ready_GameObject(void* _pArg = nullptr) PURE;
	virtual _int			Update_GameObject(const _double& _dTimeDelta) PURE;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) PURE;
	virtual void			Render_GameObject();
	
public:
	virtual CUI*			Clone(void* _pArg = nullptr) PURE;

public:
	virtual void			Free() PURE;
};

#endif // !__UI_H__
