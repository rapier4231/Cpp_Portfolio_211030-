#pragma once
#ifndef __GRAPHICDEVICE_H__
#define __GRAPHICDEVICE_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDevice : public CBase
{
	DECLARE_SINGLETON(CGraphicDevice)

public:
	enum ESCREEN_RESOULTION { ESCREEN_RESOULTION_4To3, ESCREEN_RESOULTION_16To9, ESCREEN_RESOULTION_END };

private:
	explicit CGraphicDevice();
	virtual ~CGraphicDevice();

public:
	HRESULT	Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION _eScreenResolution, const _float& fScreenRatio);
	HRESULT	Ready_GraphicDevice(HWND hWnd,
								WINMODE _eMode,
								CGraphicDevice** ppGraphicDevice);

	void					Render_Begin(D3DXCOLOR Color);
	void					Render_End(HWND _hWnd = nullptr);

	LPDIRECT3DDEVICE9		GetDevice() { return m_pDevice; }

	_float					Get_WinCX();
	_float					Get_WinCY();
	_float					Get_Screen_Ratio();
	CGraphicDevice::ESCREEN_RESOULTION		Get_Screen_Resolution();

public:
	virtual void			Free();

private:
	LPDIRECT3D9				m_pSDK = nullptr;
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;

	_float					m_fWinCX = 0.f;
	_float					m_fWinCY = 0.f;
	_float					m_fScreen_Ratio = 0.f;
	CGraphicDevice::ESCREEN_RESOULTION		m_eScreen_Resolution = ESCREEN_RESOULTION_END;
};
END

#endif // !__GRAPHICDEVICE_H__
