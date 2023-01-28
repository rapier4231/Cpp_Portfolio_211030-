#pragma once
#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CLight(void) = default;

public:
	HRESULT				Ready_Light(const D3DLIGHT9* _pLightInfo, const _uint& _iIndex);
	void				Render_Light(LPD3DXEFFECT& pEffect, _bool _bBlur = false);

public:
	//const D3DLIGHT9*	Get_Light(void) const { return &m_tLightInfo; }
	void				Set_LightStat(SETLIGHTSTAT _eSetLightStat, _float _fPower);

public:
	static CLight*		Create(LPDIRECT3DDEVICE9 _pDevice, const D3DLIGHT9* _pLightInfo, const _uint& _iIndex);
	virtual void		Free() override;

private:
	LPDIRECT3DDEVICE9			m_pDevice;
	D3DLIGHT9					m_tLightInfo;
	_uint						m_iIndex;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

};
END
#endif // !__LIGHT_H__
