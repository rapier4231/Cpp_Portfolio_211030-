
/// GraphicDevice
HRESULT		Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION _eScreenResolution, const _float& fScreenRatio)
{
	return CGraphicDevice::GetInstance()->Set_ScreenSize(_eScreenResolution, fScreenRatio);
}
HRESULT			Ready_GraphicDevice(HWND hWnd, WINMODE _eMode, CGraphicDevice** _ppGraphicDev)
{
	return CGraphicDevice::GetInstance()->Ready_GraphicDevice(hWnd, _eMode, _ppGraphicDev);
}

_float		Get_WinCX()
{
	return CGraphicDevice::GetInstance()->Get_WinCX();
}
_float		Get_WinCY()
{
	return CGraphicDevice::GetInstance()->Get_WinCY();
}
_float		Get_Screen_Ratio()
{
	return CGraphicDevice::GetInstance()->Get_Screen_Ratio();
}
CGraphicDevice::ESCREEN_RESOULTION	Get_Screen_Resolution()
{
	return CGraphicDevice::GetInstance()->Get_Screen_Resolution();
}
void			Render_Begin(D3DXCOLOR _Color)
{
	CGraphicDevice::GetInstance()->Render_Begin(_Color);
}
void			Render_End(HWND _hWnd/* = nullptr*/)
{
	CGraphicDevice::GetInstance()->Render_End(_hWnd);
}

/// Timer
_double		Get_TimeDelta(const wstring _strTimerTag)
{
	return CTimer_Mgr::GetInstance()->Get_TimeDelta(_strTimerTag);
}
void			Set_TimeDelta(const wstring _strTimerTag)
{
	CTimer_Mgr::GetInstance()->Set_TimeDelta(_strTimerTag);
}
HRESULT		Ready_Timer(const wstring _strTimerTag)
{
	return CTimer_Mgr::GetInstance()->Ready_Timer(_strTimerTag);
}

/// Frame_Mgr
HRESULT		Ready_Frame(const wstring& _strFrameTag, const _double& _dCallLimit)
{
	return CFrame_Mgr::GetInstance()->Ready_Frame(_strFrameTag, _dCallLimit);
}
_bool		IsPermit_Call(const wstring& _strFrameTag, const _double& _dTimeDelta)
{
	return CFrame_Mgr::GetInstance()->IsPermit_Call(_strFrameTag, _dTimeDelta);
}

/// Font
HRESULT Ready_Font(LPDIRECT3DDEVICE9 _pDevice, const wstring & _wstrFontTag, const wstring & _wstrFontType, const _uint & _iWidth, const _uint & _iHeight, const _uint & _iWeight, const _uint& _iWinCX)
{
	return CFont_Mgr::GetInstance()->Ready_Font(_pDevice, _wstrFontTag, _wstrFontType, _iWidth, _iHeight, _iWeight, _iWinCX);
}
void Render_Font(const wstring & _wstrFontTag, const wstring & _wstrString, const _vec2 * _pPos, D3DXCOLOR _Color, _int _iFontType)
{
	CFont_Mgr::GetInstance()->Render_Font(_wstrFontTag, _wstrString, _pPos, _Color, _iFontType);
}
void Render_Font(const wstring & _wstrFontTag, const list<LEFTGUIDEFONT_INFO>& _vecGuideFont)
{
	CFont_Mgr::GetInstance()->Render_Font(_wstrFontTag, _vecGuideFont);
}
void Render_Font(const wstring& _wstrGreenFontTag, const wstring& _wstrRedFontTag, const list<TOPGUIDEFONT_INFO>& _vecGuideFont, _int _iRedboundary)
{
	CFont_Mgr::GetInstance()->Render_Font(_wstrGreenFontTag, _wstrRedFontTag, _vecGuideFont, _iRedboundary);
}

/// InputDevice
HRESULT Ready_InputDevice(HINSTANCE _hInst, HWND _hWnd)
{
	return CInputDevice::GetInstance()->Ready_InputDevice(_hInst, _hWnd);
}
void	Update_InputDevice(HWND _hWnd)
{
	CInputDevice::GetInstance()->Update_InputDevice(_hWnd);
}
_byte	Get_DIKeyState(_ubyte _byKeyID)
{
	return CInputDevice::GetInstance()->Get_DIKeyState(_byKeyID);
}
_byte	Get_DIMouseState(MOUSEKEYSTATE _eMouseKey)
{
	return CInputDevice::GetInstance()->Get_DIMouseState(_eMouseKey);
}
_long	Get_DIMouseMove(MOUSEMOVESTATE _eMouseMove)
{
	return CInputDevice::GetInstance()->Get_DIMouseMove(_eMouseMove);
}
_bool		Get_DIKeyDown(_ubyte _byKeyID)
{
	return CInputDevice::GetInstance()->Get_DIKeyDown(_byKeyID);
}
_bool		Get_BIMouseDown(MOUSEKEYSTATE _eMouseKey)
{
	return CInputDevice::GetInstance()->Get_BIMouseDown(_eMouseKey);
}
_bool		KeyDown(unsigned long int _Key)
{
	return CInputDevice::GetInstance()->KeyDown(_Key);
}
_bool		KeyUp(unsigned long int _Key)
{
	return CInputDevice::GetInstance()->KeyUp(_Key);
}
void		Get_pMousePos(POINT* _pOutMousePos)
{
	return  CInputDevice::GetInstance()->Get_pMousePos(_pOutMousePos);
}

void		HanYoung_Key_Change()
{
	return  CInputDevice::GetInstance()->HanYoung_Key_Change();
}
_bool		Now_HanYoung()
{
	return  CInputDevice::GetInstance()->Now_HanYoung();
}

/// System Release
void Release_System(void)
{
	CInputDevice::DestroyInstance();
	CFont_Mgr::DestroyInstance();
	CFrame_Mgr::DestroyInstance();
	CTimer_Mgr::DestroyInstance();
	CGraphicDevice::DestroyInstance();
}