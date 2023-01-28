#include "stdafx.h"
#include "Back_StructureStat_UI.h"
#include "Export_Function.h"

#include "Border.h"
#include "Inven.h"
#include "Unit.h"
#include "Campfire.h"
#include "Forge.h"

//#define UISTATSIZEY 450.f
#define UISTATTEXRATIOX	0.3f
#define UISTATTEXRATIOY	0.8f

#define NAME_RECT_COLOR { 0.3f,0.8f,0.7f,0.f }
#define Button_FONT_COLOR { 0.8f,0.9f,1.f,0.9f }
#define Button_Selet_FONT_COLOR { 1.f,1.f,1.f,1.f }
#define Button_Alpha 0.6f
#define Button_Select_Alpha 0.76f

USING(Engine)

HRESULT CBack_StructureStat_UI::Set_Screen_UI()
{
	CCenter_Back_UI::Set_Screen_UI();
	m_tORTHOUI_INFO.fSizeY = 6.36f*ScrnRatio;
	m_tORTHOUI_INFO.fPosY = 3.93f*ScrnRatio;
	m_tORTHOUI_INFO.v4EdgeSize = { 0.003f ,0.001f  , 0.995f, 0.997f };
	return S_OK;
}

CBack_StructureStat_UI::CBack_StructureStat_UI(LPDIRECT3DDEVICE9 _pDevice)
	: CCenter_Back_UI(_pDevice)
{
}

CBack_StructureStat_UI::CBack_StructureStat_UI(const CBack_StructureStat_UI & rhs)
	: CCenter_Back_UI(rhs)
{
}

HRESULT CBack_StructureStat_UI::Ready_GameObject_Prototype()
{
	CCenter_Back_UI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBack_StructureStat_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CCenter_Back_UI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	m_fTexRatioX = UISTATTEXRATIOX;
	m_fTexRatioY = UISTATTEXRATIOY;

	m_tButton_Rect.fPosX = CCenter_Back_UI::stURI.fPosX[EScrnResolution];
	m_tButton_Rect.fSizeX = stURI.fButton_Rect_SizeX[EScrnResolution];
	m_tButton_Rect.fSizeY = stURI.fButton_Rect_SizeY[EScrnResolution];
	m_tButton_Rect.fPosY = stURI.fButton_Rect_PosY[EScrnResolution];
	m_tButton_Rect.v4EdgeSize = stURI.v4Button_Rect_EdgeSize[EScrnResolution];
	m_tButton_Rect.fEdgeAlpha = Button_Alpha;
	m_tButton_Rect.fAlpha = Button_Alpha;

	return S_OK;
}

_int CBack_StructureStat_UI::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_pUnit)
		return NO_EVENT;

	CCenter_Back_UI::Update_GameObject(_dTimeDelta);

	if (m_eType == InvenUserUnitTypeID_Campfire || m_eType == InvenUserUnitTypeID_Brazier)
	{

		if (m_tORTHOUI_INFO.bRender)
			m_tButton_Rect.bRender = true;
		else
			m_tButton_Rect.bRender = false;

		Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
		_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;
		_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

		_float fHalfX = stURI.fButton_Rect_SizeX[EScrnResolution] * FHALF;
		_float fHalfY = stURI.fButton_Rect_SizeY[EScrnResolution] * FHALF;
		if (MouseX >= m_tButton_Rect.fPosX - fHalfX
			&& MouseX <= m_tButton_Rect.fPosX + fHalfX
			&& MouseY >= m_tButton_Rect.fPosY - fHalfY
			&& MouseY <= m_tButton_Rect.fPosY + fHalfY)
		{
			m_tButton_Rect.fAlpha = m_tButton_Rect.fEdgeAlpha = Button_Select_Alpha;
			if (Engine::KeyDown(KeyCheck_Lbutton))
				Button_Click_Func();
		}
		else
			m_tButton_Rect.fAlpha = m_tButton_Rect.fEdgeAlpha = Button_Alpha;
	}

	return S_OK;
}

_int CBack_StructureStat_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_pUnit)
		return NO_EVENT;

	CCenter_Back_UI::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CBack_StructureStat_UI::Render_GameObject(_mat _matOrtho)
{
	CCenter_Back_UI::Render_GameObject(_matOrtho);

	if (!m_tORTHOUI_INFO.bRender)
		return;

	LPD3DXEFFECT pEffect = m_pButtonShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_tButton_Rect.fSizeX;
	matWorld._22 = m_tButton_Rect.fSizeY;
	matWorld._33 = FTRUE;

	matWorld._41 = m_tButton_Rect.fPosX - WINCX * FHALF;
	matWorld._42 = -m_tButton_Rect.fPosY + WINCY * FHALF;
	matWorld._43 = FTRUE;

	pEffect->SetFloat("g_Alpha", m_tButton_Rect.fAlpha);
	pEffect->SetVector("g_vecColor", &_vec4 NAME_RECT_COLOR);


	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &_matOrtho);

	_uint iPassMax = 2;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(1); /////

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

	BranchRender_Font();
}


CBack_StructureStat_UI * CBack_StructureStat_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CBack_StructureStat_UI*	pInstance = new CBack_StructureStat_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CBack_StructureStat_UI::Clone(void * _pArg/* = nullptr*/)
{
	CBack_StructureStat_UI*	pClone = new CBack_StructureStat_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CBack_StructureStat_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pButtonShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Pubilc_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BShader", pComponent);

	return CCenter_Back_UI::Add_Component();
}

HRESULT CBack_StructureStat_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect , _mat _matOrtho)
{
	return CCenter_Back_UI::SetUp_ConstantTable(_pEffect, _matOrtho);
}

void CBack_StructureStat_UI::Center_Back_UIOpenAni(const _double & _dTimeDelta)
{
	if (m_bCenter_Back_UIOpenAni)
	{
		m_tORTHOUI_INFO.v4EdgeSize.x -= (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.v4EdgeSize.y -= (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.v4EdgeSize.z += (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.v4EdgeSize.w += (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.fEdgeAlpha += (_float)(_dTimeDelta)* FHALF;

		if (stURI.v4EdgeSize[EScrnResolution].x >= m_tORTHOUI_INFO.v4EdgeSize.x)
			m_tORTHOUI_INFO.v4EdgeSize.x = stURI.v4EdgeSize[EScrnResolution].x;
		if (stURI.v4EdgeSize[EScrnResolution].y >= m_tORTHOUI_INFO.v4EdgeSize.y)
			m_tORTHOUI_INFO.v4EdgeSize.y = stURI.v4EdgeSize[EScrnResolution].y;
		if (stURI.v4EdgeSize[EScrnResolution].z <= m_tORTHOUI_INFO.v4EdgeSize.z)
			m_tORTHOUI_INFO.v4EdgeSize.z = stURI.v4EdgeSize[EScrnResolution].z;
		if (stURI.v4EdgeSize[EScrnResolution].w <= m_tORTHOUI_INFO.v4EdgeSize.w)
			m_tORTHOUI_INFO.v4EdgeSize.w = stURI.v4EdgeSize[EScrnResolution].w;
		if (Center_Back_UIEDGEALPHA <= m_tORTHOUI_INFO.fEdgeAlpha)
			m_tORTHOUI_INFO.fEdgeAlpha = Center_Back_UIEDGEALPHA;

		if (stURI.v4EdgeSize[EScrnResolution].x == m_tORTHOUI_INFO.v4EdgeSize.x &&stURI.v4EdgeSize[EScrnResolution].y == m_tORTHOUI_INFO.v4EdgeSize.y && stURI.v4EdgeSize[EScrnResolution].z == m_tORTHOUI_INFO.v4EdgeSize.z&& stURI.v4EdgeSize[EScrnResolution].w == m_tORTHOUI_INFO.v4EdgeSize.w
			&&Center_Back_UIEDGEALPHA == m_tORTHOUI_INFO.fEdgeAlpha)
			m_bCenter_Back_UIOpenAni = false;
	}
}

void CBack_StructureStat_UI::Free()
{

	CCenter_Back_UI::Free();
}

_bool CBack_StructureStat_UI::UI_RenderOn()
{
	return CCenter_Back_UI::UI_RenderOn();
}

void CBack_StructureStat_UI::Back_StructureStat_UIOpen()
{
	CCenter_Back_UI::Center_Back_UIOpen();
}

void CBack_StructureStat_UI::Button_Click_Func()
{
	if (m_eType == InvenUserUnitTypeID_Campfire)
		if (static_cast<CCampfire*>(m_pUnit)->Get_Ignition())
			static_cast<CCampfire*>(m_pUnit)->Set_Ignition(false);
		else
			static_cast<CCampfire*>(m_pUnit)->Set_Ignition(true);

	//else if (m_eType == InvenUserUnitTypeID_Brazier)

}

void CBack_StructureStat_UI::BranchRender_Font()
{
	wstring Font = L"";

	

	if (m_eType == InvenUserUnitTypeID_Campfire)
		if(static_cast<CCampfire*>(m_pUnit)->Get_Ignition())
			Font = L"소화";
		else
			Font = L"점화";
	//else if (m_eType == InvenUserUnitTypeID_Brazier)
	//	if (static_cast<CForge*>(m_pUnit)->Get_Ignition())
	//		Font = L"ㅁㄴㅇㅁㄹ";
	//	else
	//		Font = L"ㅁㄻㄴㄻㄴㄹ";

	if (Button_Select_Alpha == m_tButton_Rect.fAlpha)
		Engine::Render_Font(L"Font_ItemGuide", Font, &_vec2(stURI.fFont_Button_PosX[EScrnResolution], stURI.fFont_Button_PosY[EScrnResolution]), Button_Selet_FONT_COLOR, 2);
	else
		Engine::Render_Font(L"Font_ItemGuide", Font, &_vec2(stURI.fFont_Button_PosX[EScrnResolution], stURI.fFont_Button_PosY[EScrnResolution]), Button_FONT_COLOR, 2);
}

void CBack_StructureStat_UI::Set_pUnit(CUnit * _pUnit)
{
	if (!_pUnit)
	{
		m_eType = InvenUserUnitTypeID_END;
		return;
	}

	m_pUnit = _pUnit;

	CInven*	pInven = nullptr;
	if (m_pUnit->Get_Component(L"Com_Show_Inven", ID_DYNAMIC))
		pInven = static_cast<CInven*>(m_pUnit->Get_Component(L"Com_Show_Inven", ID_DYNAMIC));
	else
		pInven = static_cast<CInven*>(m_pUnit->Get_Component(L"Com_Inven", ID_DYNAMIC));

	m_eType = pInven->Get_InvenUserUnitTypeID();
}
