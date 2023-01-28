#pragma once
#ifndef __LIGHT_MGR_H__
#define __LIGHT_MGR_H__

#include "Engine_Define.h"
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLight_Mgr : public CBase
{
	DECLARE_SINGLETON(CLight_Mgr)

private:
	explicit CLight_Mgr(void);
	virtual ~CLight_Mgr(void);

public:
	HRESULT				Ready_Light(LPDIRECT3DDEVICE9 _pDevice, const D3DLIGHT9* _pLineInfo, const _uint& _iIndex);

public:
	CLight*				Get_Light(const _uint& _iIndex) const;
	void				Render_Light(LPD3DXEFFECT& pEffect, _bool _bBlur = false);
	_int				Get_LightListSize() { return (_int)m_listLight.size(); }

public:
	virtual void		Free() override;

private:
	list<CLight*>		m_listLight;
};
END
#endif // !__LIGHT_MGR_H__
