#ifndef __EXPORT_SYSTEM_H__
#define __EXPORT_SYSTEM_H__

#include "GraphicDevice.h"
#include "Timer_Mgr.h"
#include "Frame_Mgr.h"
#include "Font_Mgr.h"
#include "InputDevice.h"

BEGIN(Engine)
/// GraphicDevice
inline HRESULT								Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION _eScreenResolution, const _float& fScreenRatio);
inline HRESULT								Ready_GraphicDevice(HWND hWnd, WINMODE _eMode, CGraphicDevice** _ppGraphicDev);
inline _float								Get_Screen_Ratio();
inline CGraphicDevice::ESCREEN_RESOULTION	Get_Screen_Resolution();
inline	_float		Get_WinCX();
inline	_float		Get_WinCY();
inline void			Render_Begin(D3DXCOLOR _Color);
inline void			Render_End(HWND _hWnd = nullptr);

/// Timer_Mgr
inline HRESULT		Ready_Timer(const wstring _strTimerTag);
inline _double		Get_TimeDelta(const wstring _strTimerTag);
inline void			Set_TimeDelta(const wstring _strTimerTag);

/// Frame_Mgr
inline	HRESULT		Ready_Frame(const wstring& _strFrameTag, const _double& _dCallLimit);
inline _bool		IsPermit_Call(const wstring& _strFrameTag, const _double& _dTimeDelta);

/// Font
inline HRESULT		Ready_Font(LPDIRECT3DDEVICE9 _pDevice, const wstring & _wstrFontTag, const wstring & _wstrFontType, const _uint & _iWidth, const _uint & _iHeight, const _uint & _iWeight, const _uint& _iWinCX);
inline void			Render_Font(const wstring & _wstrFontTag, const wstring & _wstrString, const _vec2 * _pPos, D3DXCOLOR _Color, _int _iFontType = 0);
inline void			Render_Font(const wstring & _wstrFontTag, const list<LEFTGUIDEFONT_INFO>& _vecGuideFont);
inline void			Render_Font(const wstring& _wstrGreenFontTag, const wstring& _wstrRedFontTag, const list<TOPGUIDEFONT_INFO>& _vecGuideFont, _int _iRedboundary);

/// InputDevice
inline	HRESULT		Ready_InputDevice(HINSTANCE _hInst, HWND _hWnd);
inline	void		Update_InputDevice(HWND _hWnd);
inline _byte		Get_DIKeyState(_ubyte _byKeyID);
inline _byte		Get_DIMouseState(MOUSEKEYSTATE _eMouseKey);
inline _long		Get_DIMouseMove(MOUSEMOVESTATE _eMouseMove);
inline _bool		Get_DIKeyDown(_ubyte _byKeyID);
inline _bool		Get_BIMouseDown(MOUSEKEYSTATE _eMouseKey);
inline _bool		KeyDown(unsigned long int _Key);
inline _bool		KeyUp(unsigned long int _Key);
inline void			Get_pMousePos(POINT* _pOutMousePos);
inline void			HanYoung_Key_Change();
inline _bool		Now_HanYoung();



inline void			Release_System();

#include "Export_System.inl"

END

#endif // !__EXPORT_SYSTEM_H__
