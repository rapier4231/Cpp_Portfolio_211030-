#include "stdafx.h"
#include "..\Headers\MainApp.h"

#include "Export_Function.h"

#include "Logo.h"

#include "Sound_Mgr.h"
#include "PhysX_Mgr.h"
#include "Frustum.h"

#define Ratio_800To600 200.f
#define RrtioX_4To3 4.f

#define Ratio_1366To768 85.375f
#define RrtioX_16To9 16.f

CMainApp::CMainApp()
{
}


CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	FAILED_CHECK_RETURN(Setup_DefaultSetting(&m_pDevice), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Renderer(m_pDevice, false), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Frustum(m_pDevice), E_FAIL);
	Engine::Set_ClientSize((LONG)WINCX, (LONG)WINCY); // UI Á÷±³Åõ¿µÀ» À§ÇÑ Å¬¶ó »çÀÌÁî ¹Þ±â
	FAILED_CHECK_RETURN(Ready_Management(m_pDevice, &m_pManagement), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pDevice, &m_pManagement), E_FAIL);
	CPhysX_Mgr::GetInstance()->Ready_PhysX_Mgr();
	g_GlobalTime = 1.0;

	return S_OK;
}

_int CMainApp::Update_MainApp(const _double _dTimeDelta)
{
	_double TIME = _dTimeDelta >= 0.06 ? 0.06 : _dTimeDelta;

	NULL_CHECK_RETURN(m_pManagement, -1);

	_uint iResult = m_pManagement->Update_Scene(TIME * g_GlobalTime);
	Engine::Update_InputDevice(g_hWnd);

	if (0 != iResult)
		return iResult;

	// FPS
	++m_iCountFPS;
	m_FPSTime += TIME;
	if (1 < m_FPSTime)
	{
		m_iFPS = m_iCountFPS;
		m_FPSTime = 0;
		m_iCountFPS = 0;
	}

	pSMgr->Update_Sound_Mgr();
	Render_MainApp();

	return iResult;
}

void CMainApp::Render_MainApp()
{
	Engine::Render_Begin(D3DXCOLOR(1.0f, 0.0f, 1.f, 1.f));

	if (nullptr == m_pGraphicDevice)
		return;

	Engine::Render_Scene();

	// FPS
//#ifdef _DEBUG
	m_wstrFPS = L"FPS : " + to_wstring(m_iFPS);
	Engine::Render_Font(L"Font_FPS", m_wstrFPS, &_vec2(WINCX - 100.f, 10.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	m_wstrFPS = L"Draw : " + to_wstring(Engine::Get_DrawCount());
	Engine::Render_Font(L"Font_FPS", m_wstrFPS, &_vec2(WINCX - 100.f, 40.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	m_wstrFPS = L"Cull : " + to_wstring(Engine::Get_CullCount());
	Engine::Render_Font(L"Font_FPS", m_wstrFPS, &_vec2(WINCX - 100.f, 70.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));

	m_wstrFPS = L"SDraw : " + to_wstring(Engine::Get_DrawShadowCount());
	Engine::Render_Font(L"Font_FPS", m_wstrFPS, &_vec2(WINCX - 110.f, 100.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	m_wstrFPS = L"SCull : " + to_wstring(Engine::Get_CullShadowCount());
	Engine::Render_Font(L"Font_FPS", m_wstrFPS, &_vec2(WINCX - 110.f, 130.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
//#endif //_DEBUG

	Engine::Render_End();
}

HRESULT CMainApp::Setup_DefaultSetting(LPDIRECT3DDEVICE9 * ppDevice)
{
#ifdef _DEBUG
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDevice(g_hWnd, /*MODE_FULL*/MODE_WIN, &m_pGraphicDevice), E_FAIL);
#else
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDevice(g_hWnd, MODE_WIN/*MODE_FULL*/, &m_pGraphicDevice), E_FAIL);
#endif // _DEBUG

	m_pGraphicDevice->AddRef();

	(*ppDevice) = m_pGraphicDevice->GetDevice();
	(*ppDevice)->AddRef();

	// InputDev
	FAILED_CHECK_RETURN(Engine::Ready_InputDevice(g_hInst, g_hWnd), E_FAIL);

	///////////////////////////////////////////////////////////////////////////////////800:600
	// Font
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_Default", L"°íµñ", (_int)(0.075f* Ratio_800To600), (_int)(0.1f* Ratio_800To600), FW_HEAVY,(_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_FPS", L"°íµñ", (_int)(0.05f* Ratio_800To600), (_int)(0.1f* Ratio_800To600), FW_NORMAL, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	//UI
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_TapName", L"IBMPlexSansKR-Medium", (_int)(0.04f* Ratio_800To600), (_int)(0.08f* Ratio_800To600), FW_HEAVY, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_ItemGuide", L"IBMPlexSansKR-Medium", (_int)(0.025f* Ratio_800To600), (_int)(0.05f* Ratio_800To600), FW_NORMAL, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_StatGuide", L"IBMPlexSansKR-Medium", (_int)(0.03f* Ratio_800To600), (_int)(0.06f* Ratio_800To600), FW_NORMAL, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_PlayerInfoGuide", L"IBMPlexSansKR-Medium", (_int)(0.035f* Ratio_800To600), (_int)(0.07f* Ratio_800To600), FW_NORMAL, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_StatGuideSmall", L"IBMPlexSansKR-Medium", (_int)(0.025f* Ratio_800To600), (_int)(0.05f* Ratio_800To600), FW_LIGHT, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_LeftGuide", L"IBMPlexSansKR-Medium", (_int)(0.03f* Ratio_800To600), (_int)(0.06f* Ratio_800To600), FW_NORMAL, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_TopGreenGuide", L"IBMPlexSansKR-Medium", (_int)(0.07f* Ratio_800To600), (_int)(0.1f* Ratio_800To600), FW_HEAVY, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_TopRedGuide", L"IBMPlexSansKR-Medium", (_int)(0.06f* Ratio_800To600), (_int)(0.08f* Ratio_800To600), FW_HEAVY, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_WorldFont", L"EliceDigitalBaeum_Bold", (_int)(0.07f* Ratio_800To600), (_int)(0.14f* Ratio_800To600), FW_HEAVY, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_WorldFont_Translucent", L"EliceDigitalBaeum_Bold", (_int)(0.045f* Ratio_800To600), (_int)(0.09f* Ratio_800To600), FW_HEAVY, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_WorldFont_Big", L"EliceDigitalBaeum_Bold", (_int)(0.075f* Ratio_800To600), (_int)(0.15f* Ratio_800To600), FW_HEAVY, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////1366To768
	// Font
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_Default", L"°íµñ", (_int)(0.12* Ratio_1366To768), (_int)(0.24* Ratio_1366To768), FW_HEAVY, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_FPS", L"°íµñ", (_int)(0.11f* Ratio_1366To768), (_int)(0.22f* Ratio_1366To768), FW_NORMAL, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	//UI
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_Main", L"IBMPlexSansKR-Medium", (_int)(0.24f* Ratio_1366To768), (_int)(0.5f* Ratio_1366To768), FW_HEAVY, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_TapName", L"IBMPlexSansKR-Medium", (_int)(0.18f* Ratio_1366To768), (_int)(0.36f* Ratio_1366To768), FW_HEAVY, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_ItemGuide", L"IBMPlexSansKR-Medium", (_int)(0.08f* Ratio_1366To768), (_int)(0.15f* Ratio_1366To768), FW_NORMAL, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_StatGuide", L"IBMPlexSansKR-Medium", (_int)(0.09f* Ratio_1366To768), (_int)(0.17f* Ratio_1366To768), FW_NORMAL, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_PlayerInfoGuide", L"IBMPlexSansKR-Medium", (_int)(0.1f* Ratio_1366To768), (_int)(0.18f* Ratio_1366To768), FW_NORMAL, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_StatGuideSmall", L"IBMPlexSansKR-Medium", (_int)(0.08f* Ratio_1366To768), (_int)(0.15f* Ratio_1366To768), FW_LIGHT, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_LeftGuide", L"IBMPlexSansKR-Medium", (_int)(0.09f* Ratio_1366To768), (_int)(0.17f* Ratio_1366To768), FW_NORMAL, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_TopGreenGuide", L"IBMPlexSansKR-Medium", (_int)(0.18f* Ratio_1366To768), (_int)(0.34f* Ratio_1366To768), FW_HEAVY, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_TopRedGuide", L"IBMPlexSansKR-Medium", (_int)(0.16f* Ratio_1366To768), (_int)(0.3f* Ratio_1366To768), FW_HEAVY, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_WorldFont", L"EliceDigitalBaeum_Bold", (_int)(0.14f* Ratio_1366To768), (_int)(0.26f* Ratio_1366To768), FW_HEAVY, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_WorldFont_Translucent", L"EliceDigitalBaeum_Bold", (_int)(0.12f* Ratio_1366To768), (_int)(0.22f* Ratio_1366To768), FW_HEAVY, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_WorldFont_Big", L"EliceDigitalBaeum_Bold", (_int)(0.18f* Ratio_1366To768), (_int)(0.34f* Ratio_1366To768), FW_HEAVY, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	//////////////////////////////////////////////////////////////////////////////////////////
	CSound_Mgr::GetInstance()->Ready_Sound_Mgr();

	ShowCursor(false);
	// µð¹ö±×¿ë
#ifdef _DEBUG
	////////////////////////////////////////////////////////////////////////////////////800:600

	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_Debug", L"°íµñ", (_int)(0.035f* Ratio_800To600), (_int)(0.07f* Ratio_800To600), /*FW_BOLD*/FW_NORMAL, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_Debug_Bold", L"°íµñ", (_int)(0.035f* Ratio_800To600), (_int)(0.07f* Ratio_800To600), FW_BOLD, (_int)(RrtioX_4To3*Ratio_800To600)), E_FAIL);

	//////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////1366To768

	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_Debug", L"°íµñ", (_int)(0.09f* Ratio_1366To768), (_int)(0.17f* Ratio_1366To768), /*FW_BOLD*/FW_NORMAL, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pDevice, L"Font_Debug_Bold", L"°íµñ", (_int)(0.09f* Ratio_1366To768), (_int)(0.17f* Ratio_1366To768), FW_BOLD, (_int)(RrtioX_16To9*Ratio_1366To768)), E_FAIL);

	//////////////////////////////////////////////////////////////////////////////////////
	ShowCursor(true);
#endif

	// Ä¿¼­ °¨Ãß±â

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 _pDevice, CManagement ** ppManagementInstance)
{
	CScene*		pScene = nullptr;
	pScene = CLogo::Create(m_pDevice);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN((*ppManagementInstance)->SetUp_Scene(pScene), E_FAIL);
	return S_OK;
}

HRESULT CMainApp::Ready_Management(LPDIRECT3DDEVICE9 _pDevice, CManagement ** ppManagementInstance)
{
	FAILED_CHECK_RETURN(Engine::Create_Management(_pDevice, ppManagementInstance), E_FAIL);
	(*ppManagementInstance)->AddRef();

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainApp::Free()
{
	CSound_Mgr::DestroyInstance();

	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pDevice);

	Engine::Release_Utility();
	Engine::Release_System();

	CPhysX_Mgr::DestroyInstance();
}
