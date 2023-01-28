#include "stdafx.h"
#include "..\Headers\LoadingTex_UI.h"

#include "Export_Function.h"
#include "Loading.h"

#define  Loading_RECT_Alpha 0.4f
#define  Loading_RECT_COLOR { 0.3f,0.5f,0.6f,0.f }
#define  Loading_Back_RECT_COLOR { 0.1f,0.1f,0.1f,0.f }
#define  DinoMoveChangTime 0.3f
#define  LoadingGageSpeed 0.2f
#define  GameStartFontColor { 0.9f,0.2f,0.2f,1.f }
#define  GameStartSelecFontColor { 1.f,1.f,1.f,1.f }

USING(Engine)

HRESULT CLoadingTex_UI::Set_Screen_UI()
{
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	return S_OK;
}

CLoadingTex_UI::CLoadingTex_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CLoadingTex_UI::CLoadingTex_UI(const CLoadingTex_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CLoadingTex_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CLoadingTex_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_pLoading = static_cast<CLoading*>(_pArg);

	m_tStart_Rect.fPosX = stURI.fRect_PosX[EScrnResolution];
	m_tStart_Rect.fPosY = stURI.fStart_Rect_PosY[EScrnResolution];
	m_tSetting_Rect.fPosX = stURI.fRect_PosX[EScrnResolution];
	m_tSetting_Rect.fPosY = stURI.fStart_Rect_PosY[EScrnResolution] + stURI.fRect_Interval_PosY[EScrnResolution];
	m_tEnd_Rect.fPosX = stURI.fRect_PosX[EScrnResolution];
	m_tEnd_Rect.fPosY = stURI.fStart_Rect_PosY[EScrnResolution] + stURI.fRect_Interval_PosY[EScrnResolution] + stURI.fRect_Interval_PosY[EScrnResolution];
	
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];

	m_eDinoState = EDinoState_Stay;
	return S_OK;
}

_int CLoadingTex_UI::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);
	if (!m_bLoadingEnd && (1.f <= m_fDrawPerLoading) && KeyDown(KeyCheck_Enter))
		m_bLoadingEnd = true;
	return NO_EVENT;
}

_int CLoadingTex_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	if (m_bLoadingStart)
	{
		m_fLoadingBackTexNum += 0.25f;
		if (99.7f < m_fLoadingBackTexNum)
			m_fLoadingBackTexNum = 0;

	if (1 == m_pstSC->iLoadingBarCount)
		m_eDinoState = EDinoState_Walk;

	m_fPerLoading = (_float)m_pstSC->iLoadingBarCount / (_float)FinalLoadNum;
	m_fDrawPerLoading += (_float)_dTimeDelta*LoadingGageSpeed;
	if (m_fDrawPerLoading > m_fPerLoading)
		m_fDrawPerLoading = m_fPerLoading;

	if (0.5f < m_fDrawPerLoading)
		m_eDinoState = EDinoState_Run;

	if (1.f == m_fDrawPerLoading)
		m_eDinoState = EDinoState_Stun;

	m_fDinoPosX = stURI.fLoading_BarPosX[EScrnResolution] - (stURI.fLoading_BarSizeX[EScrnResolution] * 0.5f);
	m_fDinoPosX += (stURI.fLoading_BarSizeX[EScrnResolution] * m_fDrawPerLoading);

	m_fDinoMoveChange += (_float)_dTimeDelta;
	if (DinoMoveChangTime < m_fDinoMoveChange)
	{
		m_fDinoMoveChange = 0.f;
		switch (m_eDinoState)
		{
		case EDinoState_Stay:
			if (0 == m_iDinoTexNum)
				m_iDinoTexNum = 1;
			else
				m_iDinoTexNum = 0;
			break;
		case EDinoState_Walk:
			if (2 == m_iDinoTexNum)
				m_iDinoTexNum = 3;
			else
				m_iDinoTexNum = 2;
			break;
		case EDinoState_Run:
			if (6 == m_iDinoTexNum)
				m_iDinoTexNum = 7;
			else
				m_iDinoTexNum = 6;
			break;
		case EDinoState_Stun:
			if (4 == m_iDinoTexNum)
				m_iDinoTexNum = 5;
			else
				m_iDinoTexNum = 4;
			break;
		case EDinoState_End:
			break;
		default:
			break;
		}
	}
}
	else ////엔터 누르기 전
	{
	}
	Engine::Add_RenderGroup(RENDER_ORTHOBACKUI, this);
	return NO_EVENT;
}

void CLoadingTex_UI::Render_GameObject(_mat _matOrtho)
{

	if(m_bLoadingStart)
	{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho,5);

	_uint iPassMax = 2;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(NULL); /////

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();

	//////////////////////////	로고
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho);
	pEffect->BeginPass(NULL);    ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////

	pEffect->End();
	Safe_Release(pEffect);
	//여기까지 사진
	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	pEffect = m_pPuShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	//////////////////////////	로딩바 잘 보이게 하기 위한 반투명 검은 사각형
	//SetUp_ConstantTable(pEffect, _matOrtho,1);

	iPassMax = 3;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	//pEffect->BeginPass(1); /////

	//m_pBufferCom->Render_VIBuffer();

	//pEffect->EndPass();

	//////////////////////////	크롬 다이노
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, 2);
	pEffect->BeginPass(2);    ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////

	pEffect->End();
	Safe_Release(pEffect);
	///////////////////////////////여기까지 백 그라운드 바
	////////////////////////////////////////////////

	pEffect = m_pGageShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho , 3);

	iPassMax = 5;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(4); /////

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();

	pEffect->End();
	Safe_Release(pEffect);

	////////////////////////////////////////////////////////////
	if (!m_bLoadingEnd)
	{
		Engine::Render_Font(L"Font_StatGuide", m_pLoading->Get_String(), &_vec2(WINCX*0.04f, WINCY - 31.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		wstring Test = L"";
		Test = to_wstring(m_pLoading->Get_pSC()->iLoadingBarCount);
		Engine::Render_Font(L"Font_StatGuide", Test, &_vec2(WINCX*0.04f, WINCY - 70.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	if(1.f <= m_fDrawPerLoading)
	{
		Engine::Render_Font(L"Font_StatGuide", L"Enter ▶", &_vec2(WINCX*0.928f, WINCY - 32.f), GameStartSelecFontColor);
	}
	////////////////////////////////////////////////
	}
	else //////////////////엔터 누르기 전 이미지
	{
		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);
		pEffect->AddRef();

		SetUp_ConstantTable(pEffect, _matOrtho,4);

		_uint iPassMax = 2;

		pEffect->Begin(&iPassMax, NULL);

		pEffect->CommitChanges();

		pEffect->BeginPass(NULL); /////

		m_pBufferCom->Render_VIBuffer();

		pEffect->EndPass();

		//////////////////////////	로고 222
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho , 6);
		pEffect->BeginPass(NULL);    ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
		//////////////////////////

		pEffect->End();
		Safe_Release(pEffect);

		Mouse_Button_Func();
	}
}

HRESULT CLoadingTex_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pLoadingTex1_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Loading"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pLoadingTex2_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_LodingBackGround"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture2", pComponent);

	pComponent = m_pCromeDinoTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_CromeDino"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture3", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Loading"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pPuShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Pubilc_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader2", pComponent);

	pComponent = m_pGageShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Stat_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader3", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CLoadingTex_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	if (1 == _bTwo)
	{
		//로딩 뒤 백 렉트
		matWorld._11 = stURI.fLoading_BackGroundSizeX[EScrnResolution];
		matWorld._22 = stURI.fLoading_BackGroundSizeY[EScrnResolution];
		matWorld._33 = FTRUE;

		matWorld._41 = stURI.fLoading_BackGroundPosX[EScrnResolution] - WINCX * FHALF;
		matWorld._42 = -stURI.fLoading_BackGroundPosY[EScrnResolution] + WINCY * FHALF;
		matWorld._43 = FTRUE;

		_pEffect->SetFloat("g_Alpha", Loading_RECT_Alpha);
		_pEffect->SetVector("g_vecColor", &_vec4 Loading_Back_RECT_COLOR);
	}
	else if (2 == _bTwo)
	{
		//크롬 다이노
		if (EDinoState_Run == m_eDinoState)
		{
			matWorld._11 = stURI.fCromeRunDino_SizeX[EScrnResolution];
			matWorld._22 = stURI.fCromeRunDino_SizeY[EScrnResolution];
		}
		else
		{
			matWorld._11 = stURI.fCromeDino_SizeXY[EScrnResolution];
			matWorld._22 = stURI.fCromeDino_SizeXY[EScrnResolution];
		}
		matWorld._33 = FTRUE;

		matWorld._41 = m_fDinoPosX - WINCX * FHALF;
		matWorld._42 = -stURI.fCromeDino_PosY[EScrnResolution] + WINCY * FHALF;
		matWorld._43 = FTRUE;

		m_pCromeDinoTex_Com->Set_Texture(_pEffect, "g_Bg_Tex", m_iDinoTexNum);
		_pEffect->SetFloat("g_Alpha", 1.f);
	}
	else if (3 == _bTwo)
	{
		//게이지
		matWorld._11 = stURI.fLoading_BarSizeX[EScrnResolution];
		matWorld._22 = stURI.fLoading_BarSizeY[EScrnResolution];
		matWorld._33 = FTRUE;

		matWorld._41 = stURI.fLoading_BarPosX[EScrnResolution] - WINCX * FHALF;
		matWorld._42 = -stURI.fLoading_BarPosY[EScrnResolution] + WINCY * FHALF;
		matWorld._43 = FTRUE;

		_pEffect->SetFloat("g_Alpha", Loading_RECT_Alpha);
		_pEffect->SetFloat("g_PercentStat", (m_fDrawPerLoading));
	}
	else if (4 == _bTwo)
	{
		//메인 화면

		matWorld._11 = WINCX;
		matWorld._22 = WINCY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pLoadingTex1_Com->Set_Texture(_pEffect, "g_Bg_Tex", 5);
	}
	else if (5 == _bTwo)
	{
		//로딩 뒤에 사진
		matWorld._11 = WINCX;
		matWorld._22 = WINCY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pLoadingTex2_Com->Set_Texture(_pEffect, "g_Bg_Tex", (_int)m_fLoadingBackTexNum);
	}
	else if (6 == _bTwo)
	{
		//로고2
		matWorld._11 = 3.f*ScrnRatio;
		matWorld._22 = 3.f*ScrnRatio;
		matWorld._33 = FTRUE;

		matWorld._41 = WINCX * 0.5f - WINCX * FHALF;
		matWorld._42 = -100.f + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pLoadingTex1_Com->Set_Texture(_pEffect, "g_Bg_Tex", m_iTexNum);
	}
	else
	{
		//로고
		matWorld._11 = stURI.fLogoSizeX[EScrnResolution];
		matWorld._22 = stURI.fLogoSizeY[EScrnResolution];
		matWorld._33 = FTRUE;

		matWorld._41 = stURI.fLogoPosX[EScrnResolution] - WINCX * FHALF;
		matWorld._42 = -stURI.fLogoPosY[EScrnResolution] + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pLoadingTex1_Com->Set_Texture(_pEffect, "g_Bg_Tex", m_iTexNum);
	}
	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

void CLoadingTex_UI::Mouse_Button_Func()
{
	if (KeyDown(KeyCheck_Enter))
		Button_Click_Func(true);

	Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
	_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;
	_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

	_float fHalfX = stURI.fStart_Rect_SizeX[EScrnResolution] * FHALF;
	_float fHalfY = stURI.fStart_Rect_SizeY[EScrnResolution] * FHALF;

	if (MouseX >= m_tStart_Rect.fPosX - fHalfX
		&& MouseX <= m_tStart_Rect.fPosX + fHalfX
		&& MouseY >= m_tStart_Rect.fPosY - fHalfY
		&& MouseY <= m_tStart_Rect.fPosY + fHalfY)
	{
		Engine::Render_Font(L"Font_Main", L"게임 시작▷", &_vec2(stURI.fFont_GameStart_PosX[EScrnResolution], stURI.fFont_GameStart_PosY[EScrnResolution]), GameStartSelecFontColor);
		if (Engine::KeyDown(KeyCheck_Lbutton))
			Button_Click_Func(true);
	}
	else
	{
		Engine::Render_Font(L"Font_Main", L"게임 시작▷", &_vec2(stURI.fFont_GameStart_PosX[EScrnResolution], stURI.fFont_GameStart_PosY[EScrnResolution]), GameStartFontColor);
	}


	//if (MouseX >= m_tSetting_Rect.fPosX - fHalfX
	//	&& MouseX <= m_tSetting_Rect.fPosX + fHalfX
	//	&& MouseY >= m_tSetting_Rect.fPosY - fHalfY
	//	&& MouseY <= m_tSetting_Rect.fPosY + fHalfY)
	//{
	//	Engine::Render_Font(L"Font_Main", L"환경 설정", &_vec2(stURI.fFont_GameStart_PosX[EScrnResolution], stURI.fFont_GameStart_PosY[EScrnResolution] + stURI.fFont_GameStart_IntervalPosY[EScrnResolution]), GameStartSelecFontColor);
	//	/*	if (Engine::KeyDown(KeyCheck_Lbutton))
	//			Button_Click_Func(false);*/
	//}
	//else
	//	Engine::Render_Font(L"Font_Main", L"환경 설정", &_vec2(stURI.fFont_GameStart_PosX[EScrnResolution], stURI.fFont_GameStart_PosY[EScrnResolution] + stURI.fFont_GameStart_IntervalPosY[EScrnResolution]), GameStartFontColor);


	//if (MouseX >= m_tEnd_Rect.fPosX - fHalfX
	//	&& MouseX <= m_tEnd_Rect.fPosX + fHalfX
	//	&& MouseY >= m_tEnd_Rect.fPosY - fHalfY
	//	&& MouseY <= m_tEnd_Rect.fPosY + fHalfY)
	//{
	//	Engine::Render_Font(L"Font_Main", L"게임 종료", &_vec2(stURI.fFont_GameStart_PosX[EScrnResolution], stURI.fFont_GameStart_PosY[EScrnResolution] + stURI.fFont_GameStart_IntervalPosY[EScrnResolution] + stURI.fFont_GameStart_IntervalPosY[EScrnResolution]), GameStartSelecFontColor);
	//	//if (Engine::KeyDown(KeyCheck_Lbutton))
	//	//	Button_Click_Func(false);
	//}
	//else
	//	Engine::Render_Font(L"Font_Main", L"게임 종료", &_vec2(stURI.fFont_GameStart_PosX[EScrnResolution], stURI.fFont_GameStart_PosY[EScrnResolution] + stURI.fFont_GameStart_IntervalPosY[EScrnResolution] + stURI.fFont_GameStart_IntervalPosY[EScrnResolution]), GameStartFontColor);
}

void CLoadingTex_UI::Button_Click_Func(_bool _bOk)
{
	m_bLoadingStart = true;
}

CLoadingTex_UI * CLoadingTex_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CLoadingTex_UI*	pInstance = new CLoadingTex_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CLoadingTex_UI::Clone(void * _pArg)
{
	CLoadingTex_UI*	pClone = new CLoadingTex_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CLoadingTex_UI::Free()
{
	COrthoUI::Free();
}