#include "stdafx.h"
#include "OrthoUI.h"
#include "Export_Function.h"

USING(Engine)

COrthoUI::COrthoUI(LPDIRECT3DDEVICE9 _pDevice)
	: CUI(_pDevice)
{
}

COrthoUI::COrthoUI(const COrthoUI & rhs)
	: CUI(rhs)
{
}

HRESULT COrthoUI::Ready_GameObject_Prototype()
{
	CUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT COrthoUI::Ready_GameObject(void * _pArg)
{
	CUI::Ready_GameObject(_pArg);

	ZeroMemory(&m_tORTHOUI_INFO, sizeof(ORTHOUI_INFO));
	Add_Component();

	return S_OK;
}

_int COrthoUI::Update_GameObject(const _double & _dTimeDelta)
{
	CUI::Update_GameObject(_dTimeDelta);
#ifdef _DEBUG
	//Set_Screen_UI();
#endif
	return NULL;
}

_int COrthoUI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CUI::LateUpdate_GameObject(_dTimeDelta);

	return NULL;
}

void COrthoUI::Render_GameObject()
{
}

void COrthoUI::Set_IdentifyNumber(_int _iNum)
{
	m_tORTHOUI_INFO.iIdentifyNumber = _iNum;
}

ORTHOUI_INFO * COrthoUI::Get_ORTHOUI_INFO()
{
	return &m_tORTHOUI_INFO;
}

void COrthoUI::Plus_Pos_UI(_float _fPlusX, _float _fPlusY)
{
	m_tORTHOUI_INFO.fPosX += _fPlusX;
	m_tORTHOUI_INFO.fPosY += _fPlusY;
}

void COrthoUI::Set_Pos_UI(_float _fPlusX, _float _fPlusY)
{
	m_tORTHOUI_INFO.fPosX = _fPlusX;
	m_tORTHOUI_INFO.fPosY = _fPlusY;
}

_bool COrthoUI::ExecutionUpdateCheck()
{
	return m_tORTHOUI_INFO.bUpdate;
}

_bool COrthoUI::Question_Now_MouseIsUrRange()
{
	Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
	_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;
	_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

	_float fHalfX = m_tORTHOUI_INFO.fSizeX * FHALF;
	_float fHalfY = m_tORTHOUI_INFO.fSizeY * FHALF;

	if (MouseX >= m_tORTHOUI_INFO.fPosX - fHalfX
		&& MouseX <= m_tORTHOUI_INFO.fPosX + fHalfX
		&& MouseY >= m_tORTHOUI_INFO.fPosY - fHalfY
		&& MouseY <= m_tORTHOUI_INFO.fPosY + fHalfY)
		return true;

	return false;
}

_bool COrthoUI::UI_RenderOn()
{
	m_tORTHOUI_INFO.bRender = true;
	return m_tORTHOUI_INFO.bRender;
}

_bool COrthoUI::UI_RenderOff()
{
	m_tORTHOUI_INFO.bRender = false;
	return m_tORTHOUI_INFO.bRender;
}

void COrthoUI::MouseCheck()
{
	if (m_tORTHOUI_INFO.bRender)
	{
		Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
		_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;
		_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

		_float fHalfX = m_tORTHOUI_INFO.fSizeX * FHALF;
		_float fHalfY = m_tORTHOUI_INFO.fSizeY * FHALF;
		if (MouseX >= m_tORTHOUI_INFO.fPosX - fHalfX
			&& MouseX <= m_tORTHOUI_INFO.fPosX + fHalfX
			&& MouseY >= m_tORTHOUI_INFO.fPosY - fHalfY
			&& MouseY <= m_tORTHOUI_INFO.fPosY + fHalfY)
			m_tORTHOUI_INFO.uchSelectUI |= MouseOnUI;
		else
			m_tORTHOUI_INFO.uchSelectUI &= ~MouseOnUI;
		///////////////////////////// uchSelectUI의 첫번째 비트는 마우스가 올라왔는지 아닌지만

		m_tORTHOUI_INFO.uchSelectUI &= ~(MouseClickUI);
		if (MouseOnUI & m_tORTHOUI_INFO.uchSelectUI)
		{
			if (Engine::KeyDown(KeyCheck_Lbutton))
			{
				m_tORTHOUI_INFO.uchSelectUI |= MouseClickUI;
			}
		}
		//////////////////////////// uchSelectUI의 두번째 비트는 마우스가 올라왔을 때 클릭하면 On, 다음 프레임에서 Off
	}
}

HRESULT COrthoUI::Add_Component(void)
{
	return S_OK;
}

void COrthoUI::Free()
{
	CUI::Free();
}
