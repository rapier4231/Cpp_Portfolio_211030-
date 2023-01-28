#include "GraphicDevice.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDevice)

CGraphicDevice::CGraphicDevice()
{
}


CGraphicDevice::~CGraphicDevice()
{
	Free();
}

HRESULT CGraphicDevice::Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION _eScreenResolution, const _float & fScreenRatio)
{
	if (ESCREEN_RESOULTION_END < _eScreenResolution || 0 > fScreenRatio)
		return E_FAIL;

	m_eScreen_Resolution = _eScreenResolution;
	m_fScreen_Ratio = fScreenRatio;

	switch (m_eScreen_Resolution)
	{
	case Engine::CGraphicDevice::ESCREEN_RESOULTION_4To3:
		m_fWinCX = 4.f;
		m_fWinCY = 3.f;
		break;
	case Engine::CGraphicDevice::ESCREEN_RESOULTION_16To9:
		m_fWinCX = 16.f;
		m_fWinCY = 9.f;
		break;
	case Engine::CGraphicDevice::ESCREEN_RESOULTION_END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CGraphicDevice::Ready_GraphicDevice(HWND hWnd, WINMODE _eMode, CGraphicDevice ** _ppGraphicDevice)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);						  

	if (nullptr == m_pSDK)
		return E_FAIL;

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
		return E_FAIL;

	_ulong		uFlag = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		uFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		uFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS			Present_Parameter;
	ZeroMemory(&Present_Parameter, sizeof(D3DPRESENT_PARAMETERS));

	Present_Parameter.BackBufferWidth =  (UINT)(m_fWinCX * m_fScreen_Ratio);
	Present_Parameter.BackBufferHeight = (UINT)(m_fWinCY * m_fScreen_Ratio);
	Present_Parameter.BackBufferFormat = D3DFMT_A8R8G8B8;
	Present_Parameter.BackBufferCount = 1;

	//DWORD qual;
	//for (int i = D3DMULTISAMPLE_16_SAMPLES; i > D3DMULTISAMPLE_NONMASKABLE; i--) {
	//	if (SUCCEEDED(m_pSDK->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, TRUE, (D3DMULTISAMPLE_TYPE)i, &qual)))
	//	{
	//		Present_Parameter.MultiSampleType = (D3DMULTISAMPLE_TYPE)i;
	//		Present_Parameter.MultiSampleQuality = qual - 1;
	//		break;
	//	}
	//}
	Present_Parameter.MultiSampleType = D3DMULTISAMPLE_NONE;
	Present_Parameter.MultiSampleQuality = 0;

	Present_Parameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Present_Parameter.hDeviceWindow = hWnd;

	Present_Parameter.Windowed = _eMode;
	Present_Parameter.EnableAutoDepthStencil = TRUE;
	Present_Parameter.AutoDepthStencilFormat = D3DFMT_D24S8;

	Present_Parameter.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	Present_Parameter.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, uFlag, &Present_Parameter, &m_pDevice)))
		return E_FAIL;

	*_ppGraphicDevice = this;

	return S_OK;
}

void CGraphicDevice::Render_Begin(D3DXCOLOR _Color)
{
	m_pDevice->Clear(0,
		nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		_Color,
		1.f,
		0);

	m_pDevice->BeginScene();
}

void CGraphicDevice::Render_End(HWND _hWnd/* = nullptr*/)
{
	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, _hWnd, NULL);
}

_float CGraphicDevice::Get_WinCX()
{
	return (m_fWinCX * m_fScreen_Ratio);
}

_float CGraphicDevice::Get_WinCY()
{
	return (m_fWinCY * m_fScreen_Ratio);
}

_float CGraphicDevice::Get_Screen_Ratio()
{
	return m_fScreen_Ratio;
}

CGraphicDevice::ESCREEN_RESOULTION CGraphicDevice::Get_Screen_Resolution()
{
	return m_eScreen_Resolution;
}

void CGraphicDevice::Free()
{
	size_t iChecker = 0;
	if (iChecker = Safe_Release(m_pDevice))
	{
		MSG_BOX("m_pDevice Release Failed");
	}

	if (iChecker = Safe_Release(m_pSDK))
	{
		MSG_BOX("m_pSDK Release Failed");
	}
}
