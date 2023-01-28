#include "stdafx.h"
#include "..\Headers\Scroll_UI.h"
#include "Export_Function.h"
#include "Scroll.h"

#define ScrollBarAlpha 0.4f
USING(Engine)

HRESULT CScroll_UI::Set_Screen_UI()
{

	return S_OK;
}

CScroll_UI::CScroll_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CScroll_UI::CScroll_UI(const CScroll_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CScroll_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CScroll_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fAlpha = ScrollBarAlpha;
	return S_OK;
}

_int CScroll_UI::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);
	if (!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;
	Scroll_Bar_Move_Check();
	return NO_EVENT;
}

_int CScroll_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);
	if (!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;
	Scroll_Func();
	Engine::Add_RenderGroup(RENDER_ITEMUI, this);
	return NO_EVENT;
}

void CScroll_UI::Render_GameObject(_mat _matOrtho)
{
	if (!m_tORTHOUI_INFO.bRender)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = NULL;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(NULL); /////

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CScroll_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Scroll"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CScroll_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_tORTHOUI_INFO.fSizeX;
	matWorld._22 = m_tORTHOUI_INFO.fSizeY;
	matWorld._33 = FTRUE;

	matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
	matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
	matWorld._43 = FTRUE;

	_pEffect->SetFloat("g_Rect_Start_Ratio", m_fScroll_Rect_Start_Ratio);
	_pEffect->SetFloat("g_Rect_End_Ratio", m_fScroll_Rect_End_Ratio);
	_pEffect->SetFloat("g_Alpha", m_tORTHOUI_INFO.fAlpha);
	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

void CScroll_UI::Set_Scroll_Master_Info(CScroll * _Master, _float _fPosX, _float _fPosY, _float _fSizeX, _float _fSizeY)
{
	m_pMaster = _Master;
	m_fMasterPosX = _fPosX;
	m_fMasterPosY = _fPosY;
	m_fMasterSizeX = _fSizeX;
	m_fMasterSizeY = _fSizeY;
}

void CScroll_UI::Scroll_Bar_Move_Check()
{
	if (m_bScroll_Rect_Click)
	{
		Mouse_Click_Func();
		if (KeyUp(KeyCheck_Lbutton))
		{
			m_bScroll_Rect_Click = false;
			m_fClick_RatioY = 0.f;
			m_stMouse_Click_PosY = 0.f;
		}
		return;
	}

	if (m_tORTHOUI_INFO.bRender)
	{
		Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
		_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;
		_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

		////////////////////////////////////////마우스 휠 무브
		_float fHalfX = m_fMasterSizeX * FHALF;
		_float fHalfY = m_fMasterSizeY * FHALF;
		if (MouseX >= m_fMasterPosX - fHalfX
			&& MouseX <= m_fMasterPosX + fHalfX
			&& MouseY >= m_fMasterPosY - fHalfY
			&& MouseY <= m_fMasterPosY + fHalfY) //마우스가 휠 범위 안에 있다
			Mouse_Wheel_Func();

		///////////////////////////////////////마우스 클릭 무브
		//스크롤 렉트 SizeY 구하기
		_float Scroll_RectSizeY = m_fScroll_Rect_End_PosY - m_fScroll_Rect_Start_PosY;
		_float Scroll_RectPosY = m_fScroll_Rect_Start_PosY + Scroll_RectSizeY*FHALF;
		fHalfX = m_tORTHOUI_INFO.fSizeX * FHALF;
		fHalfY = Scroll_RectSizeY * FHALF;

		if (MouseX >= m_tORTHOUI_INFO.fPosX - fHalfX
			&& MouseX <= m_tORTHOUI_INFO.fPosX + fHalfX
			&& MouseY >= Scroll_RectPosY - fHalfY
			&& MouseY <= Scroll_RectPosY + fHalfY) //마우스가 스크롤 렉트 범위 안에 있다
			if (KeyDown(KeyCheck_Lbutton))
			{
				m_bScroll_Rect_Click = true;
				m_stMouse_Click_PosY = (_float)MouseY;
				m_fClick_RatioY = m_pMaster->Get_Now_RatioY();

			}
	}
}

void CScroll_UI::Mouse_Wheel_Func()
{
	LONG Wheel = Get_DIMouseMove(DIMS_Z);
	if(NULL != Wheel)
		m_pMaster->Scroll_Move_Func(Wheel);
}

void CScroll_UI::Mouse_Click_Func()
{
	Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
	//_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;

	//_float MouseMovePosY = MouseY - m_stMouse_Click_PosY ;
	//m_fScroll_Rect_Start_PosY += (_float)MouseMovePosY;
	//m_fScroll_Rect_End_PosY += (_float)MouseMovePosY;

	//if (m_fScroll_Rect_End_PosY > (m_tORTHOUI_INFO.fPosY + (m_tORTHOUI_INFO.fSizeY*FHALF)))
	//{
	//	m_fScroll_Rect_Start_PosY -= (_float)MouseMovePosY;
	//	m_fScroll_Rect_End_PosY -= (_float)MouseMovePosY;

	//	_float FixY = (m_tORTHOUI_INFO.fPosY + (m_tORTHOUI_INFO.fSizeY*FHALF)) - m_fScroll_Rect_End_PosY;
	//	m_fScroll_Rect_Start_PosY += FixY;
	//	m_fScroll_Rect_End_PosY += FixY;
	//}
	//if (m_fScroll_Rect_Start_PosY < (m_tORTHOUI_INFO.fPosY - (m_tORTHOUI_INFO.fSizeY*FHALF)))
	//{
	//	m_fScroll_Rect_Start_PosY -= (_float)MouseMovePosY;
	//	m_fScroll_Rect_End_PosY -= (_float)MouseMovePosY;

	//	_float FixY = m_fScroll_Rect_Start_PosY - (m_tORTHOUI_INFO.fPosY - (m_tORTHOUI_INFO.fSizeY*FHALF));
	//	m_fScroll_Rect_Start_PosY += FixY;
	//	m_fScroll_Rect_End_PosY += FixY;
	//}

	///////////////////////////////////////////////스타트 포스랑 엔드 포스로 비율 구하기 - m_fScroll_Rect_Start_PosY를 0.f로 봄
	//_float fScroll_Rect_PosY = (m_fScroll_Rect_Start_PosY + m_fScroll_Rect_End_PosY)*FHALF;

	////비율 -> 현재 위치 / 사이즈 
	////비율 자체가 렉트 사이즈 Y 빼고 구한거기 때문에 
	////아하. 맨 위가 0.f고 맨 마지막이 1.f군
	////
	//_float RatioPosY = (fScroll_Rect_PosY - (m_tORTHOUI_INFO.fPosY-(m_tORTHOUI_INFO.fSizeY*FHALF)) - ((m_fScroll_Rect_End_PosY - m_fScroll_Rect_Start_PosY)*FHALF))/ (m_tORTHOUI_INFO.fSizeY - (m_fScroll_Rect_End_PosY - m_fScroll_Rect_Start_PosY));


	//////////////////////////////////////////////////////////////////////////////////
	//_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;
	////m_stMouse_Click_PosY



	//_float fRectHalfSizeY = ((m_fScroll_Rect_End_PosY - m_fScroll_Rect_Start_PosY)*FHALF);
	//_float fScrollBarMaxTopPosY = m_tORTHOUI_INFO.fPosY - (m_tORTHOUI_INFO.fSizeY*FHALF);
	//_float fScrollBarMaxBottomPosY = m_tORTHOUI_INFO.fPosY + (m_tORTHOUI_INFO.fSizeY*FHALF);

	//if (fScrollBarMaxTopPosY + fRectHalfSizeY >= MouseY)
	//	MouseY = fScrollBarMaxTopPosY + fRectHalfSizeY;
	//else if (fScrollBarMaxBottomPosY  - fRectHalfSizeY <= MouseY)
	//	MouseY = fScrollBarMaxBottomPosY - fRectHalfSizeY;

	//_float RatioPosY = (MouseY - (fScrollBarMaxTopPosY + fRectHalfSizeY)) / (m_tORTHOUI_INFO.fSizeY - (fRectHalfSizeY*2.f));

	//m_pMaster->Scroll_Move_Func(RatioPosY);
	//반은 되는 거
	//////////////////////////////////////////////////////////////////////////////////
	_float fRectHalfSizeY = ((m_fScroll_Rect_End_PosY - m_fScroll_Rect_Start_PosY)*FHALF);
	_float fScrollBarMaxTopPosY = m_tORTHOUI_INFO.fPosY - (m_tORTHOUI_INFO.fSizeY*FHALF);
	_float fScrollBarMaxBottomPosY = m_tORTHOUI_INFO.fPosY + (m_tORTHOUI_INFO.fSizeY*FHALF);
	//클릭한 곳이 X ( 안 움직인다, 즉 현재 비율 받아서.)
	// X에다 마우스가 움직인 거리에 따라 비율 증감을 해준다.
	// 증감한 비율이 0~1 사이가 되도록 최대 최소값을 맞춰준다.
	_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y - m_stMouse_Click_PosY;
	_float fShow_RatioY = m_fClick_RatioY;
	_float RatioPosY = MouseY / (m_tORTHOUI_INFO.fSizeY - (fRectHalfSizeY*2.f));
	fShow_RatioY += RatioPosY;
	m_pMaster->Scroll_Move_Func(fShow_RatioY);
}

void CScroll_UI::Set_Scroll_Info(_float _fPosX, _float _fPosY, _float _fSizeX, _float _fSizeY)
{
	m_tORTHOUI_INFO.fPosX = _fPosX;
	m_tORTHOUI_INFO.fPosY = _fPosY;
	m_tORTHOUI_INFO.fSizeX = _fSizeX;
	m_tORTHOUI_INFO.fSizeY = _fSizeY;
}

void CScroll_UI::Set_Scroll_Rect(_float _fMaxTopPosY, _float _fMaxBottomPosY, _float _fShowTopPosY, _float _fShowBottomPosY)
{
	stSRInfo.fMaxTopPosY = _fMaxTopPosY;
	stSRInfo.fMaxBottomPosY = _fMaxBottomPosY;
	stSRInfo.fShowTopPosY = _fShowTopPosY;
	stSRInfo.fShowBottomPosY = _fShowBottomPosY;
}

void CScroll_UI::Scroll_Func()
{
	//스크롤 시작지점부터 스크롤 렉트 시작지점 까지 비율 구하기
	//						   실제 스크린 Y 사이즈  :  스크롤 Y 사이즈
	//stSRInfo.fMaxBottomPosY - stSRInfo.fMaxTopPosY : m_tORTHOUI_INFO.fSizeY

	//					실제 스크린 Show Top Pos Y : 알고싶은 값
	//stSRInfo.fShowTopPosY - stSRInfo.fMaxTopPosY : 스크롤 시작지점 부터 스크롤 렉트 시작하는 부분 (X로 치환)
	//X(stSRInfo.fMaxBottomPosY - stSRInfo.fMaxTopPosY) = m_tORTHOUI_INFO.fSizeY * (stSRInfo.fShowTopPosY - stSRInfo.fMaxTopPosY)
	//X =  (m_tORTHOUI_INFO.fSizeY * (stSRInfo.fShowTopPosY - stSRInfo.fMaxTopPosY)) / (stSRInfo.fMaxBottomPosY - stSRInfo.fMaxTopPosY)
	_float fDis_Scroll_Top_PosY_To_Scroll_Rect_Top_PosY;
	_float fDis_Scroll_Top_PosY_To_Scroll_Rect_Bottom_PosY;
	fDis_Scroll_Top_PosY_To_Scroll_Rect_Top_PosY = (m_tORTHOUI_INFO.fSizeY * (stSRInfo.fShowTopPosY - stSRInfo.fMaxTopPosY)) / (stSRInfo.fMaxBottomPosY - stSRInfo.fMaxTopPosY);
	//스크롤 렉트 시작 위치 (스크린)
	m_fScroll_Rect_Start_PosY = m_tORTHOUI_INFO.fPosY - (m_tORTHOUI_INFO.fSizeY*FHALF) + fDis_Scroll_Top_PosY_To_Scroll_Rect_Top_PosY;
	//스크롤 렉트 시작 비율
	m_fScroll_Rect_Start_Ratio = fDis_Scroll_Top_PosY_To_Scroll_Rect_Top_PosY / m_tORTHOUI_INFO.fSizeY;

	// 스크롤 시작지점 부터  스크롤 렉트 끝 지점까지 비율 구하기
	//						   실제 스크린 Y 사이즈  :  스크롤 Y 사이즈
	//stSRInfo.fMaxBottomPosY - stSRInfo.fMaxTopPosY : m_tORTHOUI_INFO.fSizeY

	//	       실제 스크린 Show Bottom Pos Y : 알고싶은 값
	//stSRInfo.fShowBottomPosY - stSRInfo.fMaxTopPosY : 스크롤 시작지점 부터 스크롤 렉트 시작하는 부분 (X로 치환)

	//ShowTopY를 ShowBottmY로 바꿔만 주면 됨
	fDis_Scroll_Top_PosY_To_Scroll_Rect_Bottom_PosY = (m_tORTHOUI_INFO.fSizeY * (stSRInfo.fShowBottomPosY - stSRInfo.fMaxTopPosY)) / (stSRInfo.fMaxBottomPosY - stSRInfo.fMaxTopPosY);
	//스크롤 렉트 끝 위치 (스크린)
	m_fScroll_Rect_End_PosY = m_tORTHOUI_INFO.fPosY - (m_tORTHOUI_INFO.fSizeY*FHALF) + fDis_Scroll_Top_PosY_To_Scroll_Rect_Bottom_PosY;
	//스크롤 렉트 끝 비율
	m_fScroll_Rect_End_Ratio = fDis_Scroll_Top_PosY_To_Scroll_Rect_Bottom_PosY / m_tORTHOUI_INFO.fSizeY;

}

CScroll_UI * CScroll_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CScroll_UI*	pInstance = new CScroll_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CScroll_UI::Clone(void * _pArg)
{
	CScroll_UI*	pClone = new CScroll_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CScroll_UI::Free()
{
	COrthoUI::Free();
}