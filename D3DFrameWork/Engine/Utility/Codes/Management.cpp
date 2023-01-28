#include "..\Headers\Management.h"

#include "Export_Function.h"

#include "Shader.h"

//
USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pScene(nullptr)
	, m_dWorldTime(1)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

Engine::CManagement::~CManagement(void)
{
	Free();
}

HRESULT CManagement::SetUp_Scene(CScene * pScene)
{
	Safe_Release(m_pScene);

	m_pScene = pScene;

	return S_OK;
}

_int CManagement::Update_Scene(const _double & _dTimeDelta)
{
	NULL_CHECK_RETURN(m_pScene, -1);

	Update_Fade(_dTimeDelta);

	_uint iResult = m_pScene->Update_Scene(_dTimeDelta * m_dWorldTime);
	if(iResult)
		return iResult;
	return iResult;
}

void CManagement::Render_Scene()
{
	Engine::Render_GameObject(m_fFade);

	NULL_CHECK(m_pScene);

	m_pScene->Render_Scene();
}

HRESULT CManagement::Ready_Shader(LPDIRECT3DDEVICE9 & _pDevice)
{
	/// 랜더타겟
	D3DVIEWPORT9	ViewPort;
	_pDevice->GetViewport(&ViewPort);

	// 랜더타겟 추가
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"Albedo", ViewPort.Width, ViewPort.Height), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"Depth", ViewPort.Width, ViewPort.Height), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"NormalResult", ViewPort.Width, ViewPort.Height), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 0.f), L"MotionBlur", ViewPort.Width, ViewPort.Height, true), E_FAIL);
	
	// 빛
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"Shade", ViewPort.Width, ViewPort.Height), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"Specular", ViewPort.Width, ViewPort.Height), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"Bloom", ViewPort.Width, ViewPort.Height), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"BlurX", ViewPort.Width, ViewPort.Height), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"BlurY", ViewPort.Width, ViewPort.Height), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 0.f), L"ShadowMap", SHADOW_WIDTH, SHADOW_HEIGHT, true), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 0.f), L"Shadow", ViewPort.Width, ViewPort.Height), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 0.f), L"ShadowBlurX", ViewPort.Width, ViewPort.Height), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 0.f), L"ShadowBlurY", ViewPort.Width, ViewPort.Height), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"Blend", ViewPort.Width, ViewPort.Height), E_FAIL);

	// HDR 관련
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"HDRSource", ViewPort.Width, ViewPort.Height, true), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"HDRScaledSource", 32, 32, true), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(_pDevice, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), L"HDRBrightFilter", ViewPort.Width, ViewPort.Height, true), E_FAIL);

	// 멀티 랜더 타겟 추가
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred", L"Albedo") ,E_FAIL);						// 알베도 타겟 : 디퓨즈값을 담은 랜더타겟
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred", L"Depth"), E_FAIL);						// 깊이 타겟 : 깊이 값을 담은 랜더타겟
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred", L"NormalResult"), E_FAIL);				// 노말/탄젠트/바이노말 연산 결과
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred", L"Shadow"), E_FAIL);						// 그림자 타겟

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_ShadowBlur", L"ShadowBlurX"), E_FAIL);				// 그림자 블러 타겟
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_ShadowBlur", L"ShadowBlurY"), E_FAIL);				// 그림자 블러 타겟

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc", L"Shade"), E_FAIL);						// 쉐이드 타겟
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc", L"Specular"), E_FAIL);					// 스펙큘러 타겟

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightBloom", L"Bloom"), E_FAIL);					// 블룸 타겟
	
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightBlur", L"BlurX"), E_FAIL);						// 블러 타겟
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightBlur", L"BlurY"), E_FAIL);						// 블러 타겟

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_ShadowMap", L"ShadowMap"), E_FAIL);					// 그림자맵 타겟

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_HDRSource", L"HDRSource"), E_FAIL);					// HDR 소스
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_HDRScaledSource", L"HDRScaledSource"), E_FAIL);		// HDR 스케일용
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_HDRBrightFilter", L"HDRBrightFilter"), E_FAIL);		// HDR 스케일용

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Blend", L"Blend"), E_FAIL);							// 블렌드 타겟

	// 디버그 버퍼 추가
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Albedo", 0.f, 0.f, 150.f, 150.f), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"NormalResult", 0.f, 150.f, 150.f, 150.f), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Depth", 0.f, 300.f, 150.f, 150.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Shade", 150.f, 0.f, 150.f, 150.f), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Specular", 150.f, 150.f, 150.f, 150.f), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Bloom", 150.f, 300.f, 150.f, 150.f), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"BlurY", 150.f, 450.f, 150.f, 150.f), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"BlurX", 800.f, 600.f, 0.f, 0.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"ShadowMap", 450.f, 0.f, 250.f, 250.f), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Shadow", 300.f, 0.f, 150.f, 150.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"ShadowBlurX", 300.f, 150.f, 150.f, 150.f), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"ShadowBlurY", 300.f, 300.f, 150.f, 150.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"HDRSource", 0.f, 0.f, 150.f, 150.f), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"HDRScaledSource", 0.f, 150.f, 150.f, 150.f), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"HDRBrightFilter", 0.f, 300.f, 150.f, 150.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Blend", 1400.f, 0.f, 150.f, 150.f), E_FAIL);

	// 쉐이더 프로토타입 추가
	CShader* pShader = nullptr;

	/// 스키닝 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Skinning.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Skinning", pShader), E_FAIL);

	// 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Sample", pShader), E_FAIL);

	// 매쉬 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Mesh", pShader), E_FAIL);

	// 명암 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Shade.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Shade", pShader), E_FAIL);
	
	// 블랜드 쉐이더 (최종 화면)
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Blend.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Blend", pShader), E_FAIL);

	// 터레인
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Terrain", pShader), E_FAIL);

	//스카이박스
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_SkyDome.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_SkyDome", pShader), E_FAIL);
	
	//// 노말맵 쉐이더	//이제 사용하지 않습니다. 아래의 Shader_Normal_Shadow를 이용해주세용
	//pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Normal.hpp");
	//NULL_CHECK_RETURN(pShader, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Normal", pShader), E_FAIL);

	// 노말맵+그림자 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Normal_Shadow.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Normal_Shadow", pShader), E_FAIL);
	
	//인스턴싱 쉐이더 (스태틱메시 전용) : 월드행렬을 정점정보로 받아 내부에서 조립함. 그림자 포함
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Instance.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Instance", pShader), E_FAIL);

	//인스턴싱 쉐도우맵 (스태틱메시 전용)
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Instance_ShadowMap.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Instance_ShadowMap", pShader), E_FAIL);

	// Blur 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Blur.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Blur", pShader), E_FAIL);

	// ShadowMap 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_ShadowMap.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_ShadowMap", pShader), E_FAIL);

	//// Shadow 쉐이더		//이제 사용하지 않습니다. 위의 Shader_Normal_Shadow를 이용해주세용
	//pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_ShadowMap.hpp");
	//NULL_CHECK_RETURN(pShader, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_ShadowMap", pShader), E_FAIL);

	// ShadowBlur 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_ShadowBlur.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_ShadowBlur", pShader), E_FAIL);

	// Water 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Water.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Water", pShader), E_FAIL);

	// HDR 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_HDRSource.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_HDRSource", pShader), E_FAIL);

	// Effect 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Effect.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Effect", pShader), E_FAIL);

	// Bloom 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Bloom.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Bloom", pShader), E_FAIL);

	// Result 쉐이더
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Result.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Result", pShader), E_FAIL);

	// UI 쉐이더 
	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Inventory.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Inventory", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_InvenTap.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_InvenTap", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_InvenIcon.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_InvenIcon", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Border.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Border", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_ItemSlot.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_ItemSlot", pShader), E_FAIL);

	//pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Font.hpp");
	//NULL_CHECK_RETURN(pShader, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Font", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Stat_UI.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Stat_UI", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_InfoView_UI.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_InfoView_UI", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Loading.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Loading", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_ItemInfo_View.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_ItemInfo_View", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_ScrollBar.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Scroll", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Pubilc_UI.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Pubilc_UI", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_RingCommand_UI.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_RingCommand_UI", pShader), E_FAIL);

	pShader = CShader::Create(_pDevice, L"../../Reference/Shader/Shader_Collider.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Shader_Collider", pShader), E_FAIL);
	////////////////////////////////////////

	return S_OK;
}

CLayer * CManagement::Get_Layer(const wstring & _wstrLayerTag)
{
	if (!m_pScene)
		return nullptr;

	return m_pScene->Get_Layer(_wstrLayerTag);
}

CGameObject * CManagement::Get_GameObject(const wstring & _wstrGameLayerTag, const _int& iObjIdx)
{
	if(!m_pScene)
		return nullptr;

	return m_pScene->Get_GameObject(_wstrGameLayerTag, iObjIdx);
}

CComponent * CManagement::Get_Component(const wstring & _wstrLayerTag, const _int& iObjIdx, const wstring & _wstrComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(_wstrLayerTag, iObjIdx, _wstrComponentTag, eID);
}

HRESULT CManagement::Add_GameObjectInLayer(const wstring & _wstrGameLayerTag, CGameObject * _pGameObject)
{
	if (nullptr == m_pScene)
		return E_FAIL;

	return m_pScene->Add_GameObjectInLayer(_wstrGameLayerTag, _pGameObject);
}

HRESULT CManagement::Swap_GameObjectInLayer(const wstring & _wstrPreGameLayerTag, const wstring & _wstrToGameLayerTag, _uint _iIndex)
{
	return m_pScene->Swap_GameObjectInLayer(_wstrPreGameLayerTag, _wstrToGameLayerTag, _iIndex);
}

HRESULT CManagement::Swap_GameObjectInLayer(const wstring & _wstrPreGameLayerTag, const wstring & _wstrToGameLayerTag, CGameObject * _pGameObject)
{
	return m_pScene->Swap_GameObjectInLayer(_wstrPreGameLayerTag, _wstrToGameLayerTag, _pGameObject);
}

HRESULT CManagement::RayCast_OnMesh(_vec3 * pOut, const _vec3 & vRayPos, const _vec3 & vRayDir, const wstring & _wstrGameLayerTag)
{
	if (nullptr == m_pScene)
		return E_FAIL;

	return m_pScene->RayCast_OnMesh(pOut, vRayPos, vRayDir, _wstrGameLayerTag);
}

HRESULT CManagement::Delete_GameObject(const wstring & _wstrLayerTag, CGameObject * _pGameObject)
{
	CLayer* pLayer = Get_Layer(_wstrLayerTag);
	if (nullptr == pLayer)
		return E_FAIL;

	return pLayer->Delete_GameObject(_pGameObject);
}

HRESULT CManagement::Update_Fade(const _double& _dTimeDelta)
{
	if (m_bFadeIn)
	{
		m_fFade += m_fFadeSpeed * (_float)_dTimeDelta;
		if (1.f < m_fFade)
			m_fFade = 1.f;
	}
	else
	{
		m_fFade -= m_fFadeSpeed * (_float)_dTimeDelta;
		if (m_fFade < 0.f)
			m_fFade = 0.f;
	}
	return S_OK;
}

void CManagement::Set_Fade(const _float & fFade, const _bool& bFadeIn)
{
	m_fFade = fFade;
	m_bFadeIn = bFadeIn;
}

HRESULT CManagement::Fade_In(const _float& fSec)
{
	if (m_bFadeIn)
		return E_FAIL;

	m_fFadeSpeed = 1 / fSec;
	m_bFadeIn = true;

	return S_OK;
}

HRESULT CManagement::Fade_Out(const _float& fSec)
{
	if (!m_bFadeIn)
		return E_FAIL;

	m_fFadeSpeed = 1 / fSec;
	m_bFadeIn = false;

	return S_OK;
}

list<CGameObject*>* CManagement::Get_LayerList(const wstring & _wstrGameLayerTag)
{
	return m_pScene->Get_LayerList(_wstrGameLayerTag);
}

void CManagement::Set_ShadowLight(_vec3 _vLightPos, _vec3 _vLightAt)
{
	m_pScene->Set_ShadowLight(_vLightPos, _vLightAt);
}

const SHADOWLIGHT & CManagement::Get_ShadowLight()
{
	SHADOWLIGHT pEmpty;
	pEmpty.vLightAt = _vec3(0.f, 0.f, 0.f);
	pEmpty.vLightPos = _vec3(0.f, 0.f, 0.f);
	pEmpty.vLightDir = _vec3(0.f, 0.f, 0.f);

	if (m_pScene != nullptr)
		return m_pScene->Get_ShadowLight();
	else
		return pEmpty;
}

void CManagement::Set_ShadowOnOff(_bool _bShadow)
{
	m_pScene->Set_ShadowOnOff(_bShadow);
}

const _bool & CManagement::Get_ShadowOnOff()
{
	return m_pScene->Get_ShadowOnOff();
}

void CManagement::Free(void)
{
	Safe_Release(m_pScene);
}

