#include "InputDevice.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDevice)

CInputDevice::CInputDevice()
{
}


CInputDevice::~CInputDevice()
{
	Free();
}

HRESULT CInputDevice::Ready_InputDevice(HINSTANCE _hInst, HWND _hWnd)
{
	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create(_hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL), E_FAIL);

	// 키보드 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();

	return S_OK;
}

void CInputDevice::Update_InputDevice(HWND _hWnd)
{
 	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	m_hWnd = _hWnd;
	KeyUpdate();
}

_bool CInputDevice::Get_DIKeyDown(_ubyte _byKeyID)
{
	//
	if ((m_byKeyState[_byKeyID] & 0x80) && !(m_byKeyDown[_byKeyID] & 0x80))
	{
		m_byKeyDown[_byKeyID] |= 0x80;
		return true;
	}
	else if (!(m_byKeyState[_byKeyID] & 0x80) && (m_byKeyDown[_byKeyID] & 0x80))
	{
		m_byKeyDown[_byKeyID] ^= 0x80;
		return false;
	}
	return false;
}

_bool CInputDevice::Get_BIMouseDown(MOUSEKEYSTATE _eMouseKey)
{
	if (m_tMouseState.rgbButtons[_eMouseKey] & 0x80 && !(m_tMouseDown.rgbButtons[_eMouseKey] & 0x80))
	{
		m_tMouseDown.rgbButtons[_eMouseKey] |= 0x80;
		return true;
	}
	else if (!(m_tMouseState.rgbButtons[_eMouseKey] & 0x80) && (m_tMouseDown.rgbButtons[_eMouseKey] & 0x80))
	{
		m_tMouseDown.rgbButtons[_eMouseKey] ^= 0x80;
		return false;
	}
	return false;
}

void CInputDevice::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

void CInputDevice::KeyUpdate()
{
	m_chPreKeyCheck = m_chKeyInput;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_chKeyInput |= KeyCheck_Lbutton;
	else
		m_chKeyInput &= ~KeyCheck_Lbutton;

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_chKeyInput |= KeyCheck_Rbutton;
	else
		m_chKeyInput &= ~KeyCheck_Rbutton;

	if (GetAsyncKeyState('I') & 0x8000)
		m_chKeyInput |= KeyCheck_I;
	else
		m_chKeyInput &= ~KeyCheck_I;

	if (GetAsyncKeyState('O') & 0x8000)
		m_chKeyInput |= KeyCheck_O;
	else
		m_chKeyInput &= ~KeyCheck_O;

	if (GetAsyncKeyState('E') & 0x8000)
		m_chKeyInput |= KeyCheck_E;
	else
		m_chKeyInput &= ~KeyCheck_E;

	if (GetAsyncKeyState('F') & 0x8000)
		m_chKeyInput |= KeyCheck_F;
	else
		m_chKeyInput &= ~KeyCheck_F;

	if (GetAsyncKeyState('W') & 0x8000)
		m_chKeyInput |= KeyCheck_W;
	else
		m_chKeyInput &= ~KeyCheck_W;

	if (GetAsyncKeyState('Q') & 0x8000)
		m_chKeyInput |= KeyCheck_Q;
	else
		m_chKeyInput &= ~KeyCheck_Q;

	if (GetAsyncKeyState('R') & 0x8000)
		m_chKeyInput |= KeyCheck_R;
	else
		m_chKeyInput &= ~KeyCheck_R;

	if (GetAsyncKeyState('T') & 0x8000)
		m_chKeyInput |= KeyCheck_T;
	else
		m_chKeyInput &= ~KeyCheck_T;

	if (GetAsyncKeyState('Y') & 0x8000)
		m_chKeyInput |= KeyCheck_Y;
	else
		m_chKeyInput &= ~KeyCheck_Y;

	if (GetAsyncKeyState('U') & 0x8000)
		m_chKeyInput |= KeyCheck_U;
	else
		m_chKeyInput &= ~KeyCheck_U;

	if (GetAsyncKeyState('P') & 0x8000)
		m_chKeyInput |= KeyCheck_P;
	else
		m_chKeyInput &= ~KeyCheck_P;

	if (GetAsyncKeyState('A') & 0x8000)
		m_chKeyInput |= KeyCheck_A;
	else
		m_chKeyInput &= ~KeyCheck_A;

	if (GetAsyncKeyState('S') & 0x8000)
		m_chKeyInput |= KeyCheck_S;
	else
		m_chKeyInput &= ~KeyCheck_S;

	if (GetAsyncKeyState('D') & 0x8000)
		m_chKeyInput |= KeyCheck_D;
	else
		m_chKeyInput &= ~KeyCheck_D;

	if (GetAsyncKeyState('G') & 0x8000)
		m_chKeyInput |= KeyCheck_G;
	else
		m_chKeyInput &= ~KeyCheck_G;

	if (GetAsyncKeyState('H') & 0x8000)
		m_chKeyInput |= KeyCheck_H;
	else
		m_chKeyInput &= ~KeyCheck_H;

	if (GetAsyncKeyState('J') & 0x8000)
		m_chKeyInput |= KeyCheck_J;
	else
		m_chKeyInput &= ~KeyCheck_J;

	if (GetAsyncKeyState('K') & 0x8000)
		m_chKeyInput |= KeyCheck_K;
	else
		m_chKeyInput &= ~KeyCheck_K;

	if (GetAsyncKeyState('L') & 0x8000)
		m_chKeyInput |= KeyCheck_L;
	else
		m_chKeyInput &= ~KeyCheck_L;

	if (GetAsyncKeyState('Z') & 0x8000)
		m_chKeyInput |= KeyCheck_Z;
	else
		m_chKeyInput &= ~KeyCheck_Z;

	if (GetAsyncKeyState('X') & 0x8000)
		m_chKeyInput |= KeyCheck_X;
	else
		m_chKeyInput &= ~KeyCheck_X;

	if (GetAsyncKeyState('C') & 0x8000)
		m_chKeyInput |= KeyCheck_C;
	else
		m_chKeyInput &= ~KeyCheck_C;

	if (GetAsyncKeyState('V') & 0x8000)
		m_chKeyInput |= KeyCheck_V;
	else
		m_chKeyInput &= ~KeyCheck_V;

	if (GetAsyncKeyState('B') & 0x8000)
		m_chKeyInput |= KeyCheck_B;
	else
		m_chKeyInput &= ~KeyCheck_B;

	if (GetAsyncKeyState('N') & 0x8000)
		m_chKeyInput |= KeyCheck_N;
	else
		m_chKeyInput &= ~KeyCheck_N;

	if (GetAsyncKeyState('M') & 0x8000)
		m_chKeyInput |= KeyCheck_M;
	else
		m_chKeyInput &= ~KeyCheck_M;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_chKeyInput |= KeyCheck_SpaceBar;
	else
		m_chKeyInput &= ~KeyCheck_SpaceBar;

	if (GetAsyncKeyState(VK_BACK) & 0x8000)
		m_chKeyInput |= KeyCheck_BackSpaceBar;
	else
		m_chKeyInput &= ~KeyCheck_BackSpaceBar;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_chKeyInput |= KeyCheck_Enter;
	else
		m_chKeyInput &= ~KeyCheck_Enter;
}

_bool CInputDevice::KeyDown(unsigned long int _Key)
{
	if ((m_chKeyInput&_Key) > (m_chPreKeyCheck&_Key))
		return true;
	return false;
}

_bool CInputDevice::KeyUp(unsigned long int _Key)
{
	if ((m_chKeyInput&_Key) < (m_chPreKeyCheck&_Key))
		return true;
	return false;
}

void CInputDevice::Get_pMousePos(POINT * _pOutMousePos)
{
	GetCursorPos(&m_MousePoint);
	ScreenToClient(m_hWnd, &m_MousePoint);
	_pOutMousePos->x = m_MousePoint.x;
	_pOutMousePos->y = m_MousePoint.y;
}
