#include "stdafx.h"
#include "Logo.h"

#include "Export_Function.h"

#include "Stage.h"
#include "Loading.h"

#include "BackGround.h"
#include "LoadingBackGround.h"
#include "LoadingTex_UI.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 _pDevice)
	: CScene(_pDevice)
{
}

HRESULT CLogo::Ready_Scene(void)
{
	/////////////////////////사운드 테스트
	pSMgr->PlayBGM(L"Main_Theme");

	::SetWindowText(g_hWnd, L"Logo");

	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);

	// 스테이지 리소스 로드
	m_pLoading = CLoading::Create(m_pDevice, LOADINGID::LOADING_STAGE, &m_pStage);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	// 로고 리소스
	FAILED_CHECK_RETURN(Ready_Environment_Layer(), E_FAIL);

	m_pLoadingTex_UI->Set_pstSC(m_pLoading->Get_pSC());

	return S_OK;
}

_int CLogo::Update_Scene(const _double & dTimeDelta)
{
	_int iExit = CScene::Update_Scene(dTimeDelta);
	if (true == m_pLoading->Get_Finish())
	{
		if (Get_DIKeyState(DIK_RETURN) & 0x80)
		{
			FAILED_CHECK_RETURN(SetUp_Scene(m_pStage), E_FAIL);
			Engine::Set_Fade(0.f, false);
			Engine::Fade_In(2.f);

			/////////////////////////사운드 테스트
			pSMgr->PlayBGM(L"s_amb_forest_day");

			return iExit;
		}
	}

	return iExit;
}

void CLogo::Render_Scene(void)
{
	CScene::Render_Scene();
}

HRESULT CLogo::Ready_Environment_Layer()
{
	Engine::Add_Component_Prototype(L"Proto_Com_Transform", CTransform::Create(m_pDevice));
	Engine::Add_Component_Prototype(L"Proto_Buffer_RectTex", CVIBuffer_RectTex::Create(m_pDevice));

	// LoadingScreen 전용 프로토타입들
	Engine::Add_Component_Prototype(L"Proto_Texture_LoadingIcon", CTexture::Create(m_pDevice, L"../Resources/Texture/LoadingIcon/LoadingIcon%d.jpg", TEX_NORMAL, 16));
	Engine::Add_Component_Prototype(L"Proto_Texture_LoadingScreen", CTexture::Create(m_pDevice, L"../Resources/Texture/LoadingScreen/LoadingScreen%d.png", TEX_NORMAL));
	Engine::Add_GameObject_Prototype(L"Proto_Obj_LoadingBackGround", CLoadingBackGround::Create(m_pDevice));

	Engine::Add_Component_Prototype(L"Proto_Texture_Loading", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Loading/Loading%d.tga", TEX_NORMAL, 6));
	Engine::Add_Component_Prototype(L"Proto_Texture_CromeDino", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Loading/LoadingDino%d.png", TEX_NORMAL, 8));
	Engine::Add_Component_Prototype(L"Proto_Texture_LodingBackGround", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Loading/outImgs/%d.png", TEX_NORMAL, 100));
	Engine::Add_GameObject_Prototype(L"Proto_LoadingTex_UI", CLoadingTex_UI::Create(m_pDevice));

	//테스트
	//FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_RingCommand", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Ring_Command/RingCommand%d.png", TEX_NORMAL, 3)), E_FAIL);
	//Engine::Add_GameObject_Prototype(L"Proto_Test_UI", CRingCommand_UI::Create(m_pDevice));


	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_LoadingBackGround");
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_LoadingTex_UI", m_pLoading);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_pLoadingTex_UI = static_cast<CLoadingTex_UI*>(pGameObject);

	//테스트
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Test_UI");
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(L"Environment", pLayer);

	return S_OK;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CLogo* pInstance = new CLogo(_pDevice);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free()
{
	Engine::Delete_GameObject_Prototype(L"Proto_Com_Transform");
	Engine::Delete_GameObject_Prototype(L"Proto_Buffer_RectTex");
	Engine::Delete_GameObject_Prototype(L"Proto_Texture_LoadingIcon");
	Engine::Delete_GameObject_Prototype(L"Proto_Texture_LoadingScreen");
	Engine::Delete_GameObject_Prototype(L"Proto_Obj_LoadingBackGround");
	Engine::Delete_GameObject_Prototype(L"Proto_Texture_Loading");
	Engine::Delete_GameObject_Prototype(L"Proto_Texture_CromeDino");
	Engine::Delete_GameObject_Prototype(L"Proto_Texture_LodingBackGround");
	Engine::Delete_GameObject_Prototype(L"Proto_LoadingTex_UI");

	Safe_Release(m_pLoading);

	CScene::Free();
}
