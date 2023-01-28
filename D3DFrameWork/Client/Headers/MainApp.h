#pragma once
#ifndef __MAINAPP_H__
#define __MAINAPP_H__

#include "Engine_Define.h"
#include "Base.h"

#include "Value.h"

BEGIN(Engine)

class CGraphicDevice;
class CManagement;

END

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Ready_MainApp();
	_int		Update_MainApp(const _double _dTimeDelta);
	void		Render_MainApp();

private:
	HRESULT		Setup_DefaultSetting(LPDIRECT3DDEVICE9* ppDevice);
	HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 _pDevice, CManagement** ppManagementInstance);
	HRESULT		Ready_Management(LPDIRECT3DDEVICE9 _pDevice, CManagement** ppManagementInstance);
//	HRESULT		Render_MainImage();
public:
	static	CMainApp*	Create();
	virtual void Free() override;

private:
	CGraphicDevice*		m_pGraphicDevice = nullptr;
	CManagement*		m_pManagement = nullptr;
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;

	_double				m_FPSTime = 0;
	_uint				m_iCountFPS = 0;
	_uint				m_iFPS = 0;
	wstring				m_wstrFPS = L"";
};

#endif // !__MAINAPP_H__
