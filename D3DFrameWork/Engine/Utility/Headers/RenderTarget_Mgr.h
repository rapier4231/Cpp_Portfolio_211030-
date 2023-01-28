#pragma once
#ifndef __RENDERTARGET_MGR_H__
#define __RENDERTARGET_MGR_H__

#include "RenderTarget.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget_Mgr : public CBase
{
	DECLARE_SINGLETON(CRenderTarget_Mgr)

private:
	CRenderTarget_Mgr();
	virtual ~CRenderTarget_Mgr();

public:
	HRESULT		Ready_RenderTarget(LPDIRECT3DDEVICE9 _pDevice, D3DFORMAT _Format, D3DXCOLOR _Color, const wstring& _wstrTargetTag, const _uint& _iWidth = 800, const _uint& _iHeight = 600, _bool _bShadow = false);
	HRESULT		Ready_MRT(const wstring& _wstrMRTTag, const wstring& _wstrTargetTag);

	HRESULT		Begin_MRT(const wstring& _wstrMRTTag, _bool _bMotionBlur);
	HRESULT		End_MRT(const wstring& _wstrMRTTag);

	HRESULT		Ready_DebugBuffer(const wstring& _wstrTargetTag, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);
	void		Render_DebugBuffer(const wstring& _wstrMRTTag);

	void		Get_RenderTargetTexture(LPD3DXEFFECT& pEffect, const wstring& _strTargetTag, const char* _pConstantName);

public:
	virtual void Free() override;

private:
	CRenderTarget*			Find_RenderTarget(const wstring& _wstrTargetTag);
	list<CRenderTarget*>*	Find_MRT(const wstring& _wstrMRTTag);

private:
	map<wstring, CRenderTarget*>			m_mapRenderTareget;
	map<wstring, list<CRenderTarget*>>		m_mapMRT;
};
END

#endif // !__RENDERTARGET_MGR_H__
