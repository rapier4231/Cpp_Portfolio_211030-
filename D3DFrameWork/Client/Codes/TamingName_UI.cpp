#include "stdafx.h"
#include "..\Headers\TamingName_UI.h"

#include "Export_Function.h"
#include "Dino.h"

#define BACKGROUND_TEX_RATIOXY 0.6f
#define BACKGROUND_TEX_ALPHA 0.6f
#define NAME_RECT_ALPHA 0.4f
#define NAME_RECT_COLOR { 0.3f,0.5f,0.6f,0.f }
#define Gudie_FONT_COLOR { 0.3f,0.8f,1.f,1.f }
#define Button_FONT_COLOR { 0.6f,0.8f,1.f,1.f }
#define Button_Selet_FONT_COLOR { 0.8f,0.9f,1.f,1.f }
#define NAME_FONT_COLOR	{ 0.6f,0.9f,0.6f,1.f }
#define Button_Alpha 0.7f
#define Button_Select_Alpha 1.f
USING(Engine)

HRESULT CTamingName_UI::Set_Screen_UI()
{
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	return S_OK;
}

CTamingName_UI::CTamingName_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CTamingName_UI::CTamingName_UI(const CTamingName_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CTamingName_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CTamingName_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	m_tORTHOUI_INFO.fEdgeAlpha = BACKGROUND_TEX_ALPHA;
	m_tORTHOUI_INFO.fAlpha = BACKGROUND_TEX_ALPHA;


	m_tName_Rect.fPosX = stURI.fPosX[EScrnResolution];
	m_tName_Rect.fSizeX = stURI.fName_Rect_SizeX[EScrnResolution];
	m_tName_Rect.fSizeY = stURI.fName_Rect_SizeY[EScrnResolution];
	m_tName_Rect.fPosY = stURI.fName_Rect_PosY[EScrnResolution];
	m_tName_Rect.v4EdgeSize = stURI.v4Name_Rect_EdgeSize[EScrnResolution];
	m_tName_Rect.fEdgeAlpha = BACKGROUND_TEX_ALPHA;
	m_tName_Rect.fAlpha = NAME_RECT_ALPHA;

	m_tOk_Rect.fPosX = stURI.fPosX[EScrnResolution];
	m_tOk_Rect.fSizeX = stURI.fOk_Rect_SizeX[EScrnResolution];
	m_tOk_Rect.fSizeY = stURI.fOk_Rect_SizeY[EScrnResolution];
	m_tOk_Rect.fPosY = stURI.fOk_Rect_PosY[EScrnResolution];
	m_tOk_Rect.v4EdgeSize = stURI.v4Ok_Rect_EdgeSize[EScrnResolution];
	m_tOk_Rect.fEdgeAlpha = Button_Alpha;
	m_tOk_Rect.fAlpha = Button_Alpha;

	m_tCancel_Rect.fPosX = stURI.fPosX[EScrnResolution];
	m_tCancel_Rect.fSizeX = stURI.fCancel_Rect_SizeX[EScrnResolution];
	m_tCancel_Rect.fSizeY = stURI.fCancel_Rect_SizeY[EScrnResolution];
	m_tCancel_Rect.fPosY = stURI.fCancel_Rect_PosY[EScrnResolution];
	m_tCancel_Rect.v4EdgeSize = stURI.v4Cancel_Rect_EdgeSize[EScrnResolution];
	m_tCancel_Rect.fEdgeAlpha = Button_Alpha;
	m_tCancel_Rect.fAlpha = Button_Alpha;
	return S_OK;
}

_int CTamingName_UI::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;
	COrthoUI::Update_GameObject(_dTimeDelta);

	Key_Input_Update();	
	Mouse_Button_Func();
	return NO_EVENT;
}

_int CTamingName_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	Engine::Add_RenderGroup(RENDER_LASTUI, this);
	return NO_EVENT;
}

void CTamingName_UI::Render_GameObject(_mat _matOrtho)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 6;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(0); /////

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();

	//////////////////////////	
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho,1);
	pEffect->BeginPass(1);    ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////
	//////////////////////////	
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, 2);
	pEffect->BeginPass(1);    ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////
	//////////////////////////	
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, 3);
	pEffect->BeginPass(1);    ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////
	

	pEffect->End();

	Safe_Release(pEffect);

	wstring Font = m_wstrDinoName;
	Font += L"의 이름을 지어줍니다!";
	Engine::Render_Font(L"Font_TopRedGuide", Font, &_vec2(stURI.fPosX[EScrnResolution], stURI.fFont_Guide_PosY[EScrnResolution]), Gudie_FONT_COLOR,4);

	Font = Get_NowText();
	Engine::Render_Font(L"Font_WorldFont", Font, &_vec2(stURI.fFont_Name_PosX[EScrnResolution], stURI.fFont_Name_PosY[EScrnResolution]), NAME_FONT_COLOR);

	Font = L"확인";
	if(Button_Select_Alpha == m_tOk_Rect.fAlpha)
		Engine::Render_Font(L"Font_WorldFont", Font, &_vec2(stURI.fFont_Ok_PosX[EScrnResolution], stURI.fFont_Ok_PosY[EScrnResolution]), Button_Selet_FONT_COLOR);
	else
		Engine::Render_Font(L"Font_WorldFont", Font, &_vec2(stURI.fFont_Ok_PosX[EScrnResolution], stURI.fFont_Ok_PosY[EScrnResolution]), Button_FONT_COLOR);

	Font = L"취소";
	if(Button_Select_Alpha == m_tCancel_Rect.fAlpha)
		Engine::Render_Font(L"Font_WorldFont", Font, &_vec2(stURI.fFont_Cancel_PosX[EScrnResolution], stURI.fFont_Cancel_PosY[EScrnResolution]), Button_Selet_FONT_COLOR);
	else
		Engine::Render_Font(L"Font_WorldFont", Font, &_vec2(stURI.fFont_Cancel_PosX[EScrnResolution], stURI.fFont_Cancel_PosY[EScrnResolution]), Button_FONT_COLOR);
}

HRESULT CTamingName_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBackGround_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Inventory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Pubilc_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CTamingName_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	if (1 == _bTwo) //이름 렉트
	{
		matWorld._11 = m_tName_Rect.fSizeX;
		matWorld._22 = m_tName_Rect.fSizeY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tName_Rect.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tName_Rect.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;

		_pEffect->SetFloat("g_Alpha", m_tName_Rect.fAlpha);
		_pEffect->SetVector("g_vecColor", &_vec4 NAME_RECT_COLOR);
	}
	else if (2 == _bTwo) //Ok버튼
	{
		matWorld._11 = m_tOk_Rect.fSizeX;
		matWorld._22 = m_tOk_Rect.fSizeY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tOk_Rect.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tOk_Rect.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;

		_pEffect->SetFloat("g_Alpha", m_tOk_Rect.fAlpha);
		_pEffect->SetVector("g_vecColor", &_vec4 NAME_RECT_COLOR);
	}
	else if (3==_bTwo)//Cancel 버튼
	{
		matWorld._11 = m_tCancel_Rect.fSizeX;
		matWorld._22 = m_tCancel_Rect.fSizeY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tCancel_Rect.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tCancel_Rect.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;

		_pEffect->SetFloat("g_Alpha", m_tCancel_Rect.fAlpha);
		_pEffect->SetVector("g_vecColor", &_vec4 NAME_RECT_COLOR);
	}
	else
	{
		matWorld._11 = m_tORTHOUI_INFO.fSizeX;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;

		m_pBackGround_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_Alpha", m_tORTHOUI_INFO.fAlpha);
		_pEffect->SetFloat("g_Ratio_SizeX", BACKGROUND_TEX_RATIOXY);
		_pEffect->SetFloat("g_Ratio_SizeY", BACKGROUND_TEX_RATIOXY);
	}

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);

	return S_OK;
}

void CTamingName_UI::Mouse_Button_Func()
{
	if(KeyDown(KeyCheck_Enter))
		Button_Click_Func(true);

	Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
	_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;
	_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

	_float fHalfX = stURI.fOk_Rect_SizeX[EScrnResolution] * FHALF;
	_float fHalfY = stURI.fOk_Rect_SizeY[EScrnResolution] * FHALF;
	if (MouseX >= m_tOk_Rect.fPosX - fHalfX
		&& MouseX <= m_tOk_Rect.fPosX + fHalfX
		&& MouseY >= m_tOk_Rect.fPosY - fHalfY
		&& MouseY <= m_tOk_Rect.fPosY + fHalfY)
	{
		m_tOk_Rect.fAlpha = m_tOk_Rect.fEdgeAlpha = Button_Select_Alpha;
		if (Engine::KeyDown(KeyCheck_Lbutton))
			Button_Click_Func(true);
	}
	else
		m_tOk_Rect.fAlpha = m_tOk_Rect.fEdgeAlpha = Button_Alpha;


	if (MouseX >= m_tCancel_Rect.fPosX - fHalfX
		&& MouseX <= m_tCancel_Rect.fPosX + fHalfX
		&& MouseY >= m_tCancel_Rect.fPosY - fHalfY
		&& MouseY <= m_tCancel_Rect.fPosY + fHalfY)
	{
		m_tCancel_Rect.fAlpha = m_tCancel_Rect.fEdgeAlpha = Button_Select_Alpha;
		if (Engine::KeyDown(KeyCheck_Lbutton))
			Button_Click_Func(false);
	}
	else
		m_tCancel_Rect.fAlpha = m_tCancel_Rect.fEdgeAlpha = Button_Alpha;
}

void CTamingName_UI::Button_Click_Func(_bool _bOk)
{
	if (_bOk)
	{
		m_pDino->Set_DinoSpeciesKor(Get_NowText());
		m_pDino->Set_IsNaming(true);
		Text_Input_Reset();
		m_pDino = nullptr;
		m_tCancel_Rect.fAlpha = m_tCancel_Rect.fEdgeAlpha = Button_Alpha;
		m_tOk_Rect.fAlpha = m_tOk_Rect.fEdgeAlpha = Button_Alpha;
		m_wstrDinoName = L"";
		m_tORTHOUI_INFO.bRender = false;
		m_tName_Rect.bRender = false;
		m_tOk_Rect.bRender = false;
		m_tCancel_Rect.bRender = false;
	}
	else
	{
		Text_Input_Reset();
		m_pDino = nullptr;
		m_tCancel_Rect.fAlpha = m_tCancel_Rect.fEdgeAlpha = Button_Alpha;
		m_tOk_Rect.fAlpha = m_tOk_Rect.fEdgeAlpha = Button_Alpha;
		m_wstrDinoName = L"";
		m_tORTHOUI_INFO.bRender = false;
		m_tName_Rect.bRender = false;
		m_tOk_Rect.bRender = false;
		m_tCancel_Rect.bRender = false;
	}
}

CTamingName_UI * CTamingName_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CTamingName_UI*	pInstance = new CTamingName_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CTamingName_UI::Clone(void * _pArg)
{
	CTamingName_UI*	pClone = new CTamingName_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CTamingName_UI::Free()
{
	Text_Input_Reset();
	COrthoUI::Free();
}

void CTamingName_UI::Making_a_Name(CDino* _pDino, const wstring & _wstrDinoName)
{
	m_pDino = _pDino;
	m_wstrDinoName = _pDino->Get_DinoSpeciesKor();
	m_tORTHOUI_INFO.bRender = true;
	m_tName_Rect.bRender = true;
	m_tOk_Rect.bRender = true;
	m_tCancel_Rect.bRender = true;
}

