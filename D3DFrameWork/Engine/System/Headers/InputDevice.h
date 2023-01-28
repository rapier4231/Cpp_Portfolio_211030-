#pragma once
#ifndef __INPUTDEVICE_H__
#define __INPUTDEVICE_H__

#include "Engine_Define.h"
#include "Base.h"

#define KeyCheck_Lbutton (1 << 0)
#define KeyCheck_Rbutton (1 << 1)
#define KeyCheck_I		 (1 << 2)
#define KeyCheck_O		 (1 << 3)
#define KeyCheck_E		 (1 << 4)
#define KeyCheck_F		 (1 << 5)
#define KeyCheck_Q		 (1 << 6)
#define KeyCheck_W		 (1 << 7)
#define KeyCheck_R		 (1 << 8)
#define KeyCheck_T		 (1 << 9)
#define KeyCheck_Y		 (1 << 10)
#define KeyCheck_U		 (1 << 11)
#define KeyCheck_P		 (1 << 12)
#define KeyCheck_A		 (1 << 13)
#define KeyCheck_S		 (1 << 14)
#define KeyCheck_D		 (1 << 15)
#define KeyCheck_G		 (1 << 16)
#define KeyCheck_H		 (1 << 17)
#define KeyCheck_J		 (1 << 18)
#define KeyCheck_K		 (1 << 19)
#define KeyCheck_L		 (1 << 20)
#define KeyCheck_Z		 (1 << 21)
#define KeyCheck_X		 (1 << 22)
#define KeyCheck_C		 (1 << 23)
#define KeyCheck_V		 (1 << 24)
#define KeyCheck_B		 (1 << 25)
#define KeyCheck_N		 (1 << 26)
#define KeyCheck_M		 (1 << 27)
#define KeyCheck_SpaceBar (1 << 28)
#define KeyCheck_BackSpaceBar (1 << 29)
#define KeyCheck_Enter (1 << 30)

BEGIN(Engine)

class ENGINE_DLL CInputDevice : public CBase
{
	DECLARE_SINGLETON(CInputDevice)

private:
	explicit CInputDevice();
	virtual ~CInputDevice();

public:
	HRESULT		Ready_InputDevice(HINSTANCE _hInst, HWND _hWnd);
	void		Update_InputDevice(HWND _hWnd);

public:
	_bool		Get_DIKeyDown(_ubyte _byKeyID);
	_bool		Get_BIMouseDown(MOUSEKEYSTATE _eMouseKey);

	_byte		Get_DIKeyState(_ubyte _byKeyID) { return m_byKeyState[_byKeyID]; }
	_byte		Get_DIMouseState(MOUSEKEYSTATE _eMouseKey) { return m_tMouseState.rgbButtons[_eMouseKey]; }
	_long		Get_DIMouseMove(MOUSEMOVESTATE _eMouseMove) { return *(((_long*)&m_tMouseState) + _eMouseMove); }
		
public:
	virtual void Free() override;

private:
	void			KeyUpdate();

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

	_byte					m_byKeyState[256];
	_byte					m_byKeyDown[256];
	DIMOUSESTATE			m_tMouseState;
	DIMOUSESTATE			m_tMouseDown;

public:
	_bool			KeyDown(unsigned long int _Key);
	_bool			KeyUp(unsigned long int _Key);
	void			Get_pMousePos(POINT* _pOutMousePos);


private:
	unsigned long int		m_chKeyInput = NULL;
	unsigned long int		m_chPreKeyCheck = NULL;
	POINT			m_MousePoint;
	POINT			m_ClickMousePoint;
	HWND			m_hWnd;

private:
	_bool			m_bHangle = false;
public:
	void			HanYoung_Key_Change() {m_bHangle = !m_bHangle;}
	_bool			Now_HanYoung() { return m_bHangle; }
};
END
#endif // !__INPUTDEVICE_H__
