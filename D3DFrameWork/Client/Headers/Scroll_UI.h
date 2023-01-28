#pragma once
#ifndef __Scroll_UI_H__
#define __Scroll_UI_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_RectTex;
END
class CScroll;

class CScroll_UI : public COrthoUI
{
private:
	typedef struct stScroll_UI_Resolution_Info : STOURI
	{
	
	}STScroll_UIURI;

	STScroll_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	typedef struct stScroll_Rect_Info
	{
		_float	fMaxTopPosY = 0.f;
		_float fMaxBottomPosY = 0.f;
		_float fShowTopPosY = 0.f;
		_float fShowBottomPosY = 0.f;
	}SCROLL_RECT_INFO;

	SCROLL_RECT_INFO stSRInfo;

private:
	explicit CScroll_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CScroll_UI(const CScroll_UI& rhs);
	virtual ~CScroll_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho);

public:
	static	CScroll_UI*     Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0);

private:
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

public:								// 마우스 휠 먹을 때 공간 알기 위해서 ㅇㅇ
	void					Set_Scroll_Master_Info(CScroll* _Master, _float _fPosX, _float _fPosY, _float _fSizeX, _float _fSizeY);
private:
	CScroll*				m_pMaster = nullptr;
	_float					m_fMasterPosX = 0.f;
	_float					m_fMasterPosY = 0.f;
	_float					m_fMasterSizeX = 0.f;
	_float					m_fMasterSizeY = 0.f;

private:
	void					Scroll_Bar_Move_Check();
	void					Mouse_Wheel_Func();
	void					Mouse_Click_Func();
private:
	_bool					m_bScroll_Rect_Click = false;
	_float					m_stMouse_Click_PosY = 0.f;
	_float					m_fClick_RatioY = 0.f;
public:
	void					Set_Scroll_Info(_float _fPosX, _float _fPosY, _float _fSizeX, _float _fSizeY);
	void					Set_Scroll_Rect(_float _fMaxTopPosY, _float _fMaxBottomPosY, _float _fShowTopPosY, _float _fShowBottomPosY);

private:
	void					Scroll_Func();
private:
	_float					m_fScroll_Rect_Start_PosY = 0.f;
	_float					m_fScroll_Rect_End_PosY = 0.f;
	_float					m_fScroll_Rect_Start_Ratio = 0.f;
	_float					m_fScroll_Rect_End_Ratio = 0.f;
};

#endif // !__Scroll_UI_H__