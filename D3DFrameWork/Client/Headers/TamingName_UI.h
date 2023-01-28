#pragma once
#ifndef __TamingName_UI_H__
#define __TamingName_UI_H__

#include "Value.h"
#include "OrthoUI.h"
#include "Text_Input.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END
class CDino;

class CTamingName_UI : public COrthoUI, public CText_Input
{
private:
	typedef struct stTamingName_UI_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { WINCX*FHALF, WINCX*FHALF };
		_float fPosY[ESNRT_END] = { 1.45f*ScrnRatio ,4.5f*ScrnRatio };
		_float fSizeX[ESNRT_END] = { 1.f*ScrnRatio, 5.2f*ScrnRatio };
		_float fSizeY[ESNRT_END] = { 2.25f*ScrnRatio, 3.8f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { { 0.004f ,0.002f  , 0.9914f, 0.996f },
			/* { 0.001f ,0.001f  , 0.995f, 0.997f }*/{ 0.003f ,0.001f  , 0.996f, 0.999f } };

		_float fName_Rect_SizeX[ESNRT_END] = { 1.f*ScrnRatio, 4.2f*ScrnRatio };
		_float fName_Rect_SizeY[ESNRT_END] = { 1.f*ScrnRatio, 0.46f*ScrnRatio };
		_float fName_Rect_PosY[ESNRT_END] = { 1.45f*ScrnRatio ,3.7f*ScrnRatio };
		_vec4  v4Name_Rect_EdgeSize[ESNRT_END] = { { 0.004f ,0.002f  , 0.9914f, 0.996f },
			/* { 0.001f ,0.001f  , 0.995f, 0.997f }*/{ 0.003f ,0.03f  , 0.999f, 0.99f } };

		_float fOk_Rect_SizeX[ESNRT_END] = { 1.f*ScrnRatio, 1.6f*ScrnRatio };
		_float fOk_Rect_SizeY[ESNRT_END] = { 1.f*ScrnRatio, 0.6f*ScrnRatio };
		_float fOk_Rect_PosY[ESNRT_END] = { 1.45f*ScrnRatio ,4.8f*ScrnRatio };
		_vec4  v4Ok_Rect_EdgeSize[ESNRT_END] = { { 0.004f ,0.002f  , 0.9914f, 0.996f },
			/* { 0.001f ,0.001f  , 0.995f, 0.997f }*/{ 0.003f ,0.01f  , 0.995f, 0.99f } };

		_float fCancel_Rect_SizeX[ESNRT_END] = { 1.f*ScrnRatio, 1.6f*ScrnRatio };
		_float fCancel_Rect_SizeY[ESNRT_END] = { 1.f*ScrnRatio, 0.6f*ScrnRatio };
		_float fCancel_Rect_PosY[ESNRT_END] = { 1.45f*ScrnRatio ,5.6f*ScrnRatio };
		_vec4  v4Cancel_Rect_EdgeSize[ESNRT_END] = { { 0.004f ,0.002f  , 0.9914f, 0.996f },
			/* { 0.001f ,0.001f  , 0.995f, 0.997f }*/{ 0.003f ,0.03f  , 0.995f, 0.99f } };

		_float fFont_Guide_PosY[ESNRT_END] = { 6.f*ScrnRatio, 3.4f*ScrnRatio };

		_float fFont_Name_PosX[ESNRT_END] = { 6.f*ScrnRatio, 6.f*ScrnRatio};
		_float fFont_Name_PosY[ESNRT_END] = { 3.58f*ScrnRatio,3.58f*ScrnRatio };

		_float fFont_Ok_PosX[ESNRT_END] = { 3.58f*ScrnRatio, 7.74f*ScrnRatio };
		_float fFont_Ok_PosY[ESNRT_END] = { 3.58f*ScrnRatio,4.67f*ScrnRatio };
		_float fFont_Cancel_PosX[ESNRT_END] = { 3.58f*ScrnRatio,7.74f*ScrnRatio };
		_float fFont_Cancel_PosY[ESNRT_END] = { 3.58f*ScrnRatio,5.48f*ScrnRatio };
	}STTamingName_UIURI;

	STTamingName_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	explicit CTamingName_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CTamingName_UI(const CTamingName_UI& rhs);
	virtual ~CTamingName_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;


public:
	static	CTamingName_UI* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void					Making_a_Name(CDino* _pDino, const wstring& _wstrDinoName);

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0);

private:
	void					Mouse_Button_Func();
	void					Button_Click_Func(_bool _bOk);

private:
	CTexture*				m_pBackGround_Com = nullptr;
	CShader*				m_pShaderCom = nullptr; 
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

private:
	CDino*					m_pDino = nullptr;
	wstring					m_wstrDinoName = L"";

private:
	ORTHOUI_INFO			m_tName_Rect;
	ORTHOUI_INFO			m_tOk_Rect;
	ORTHOUI_INFO			m_tCancel_Rect;

public:
	ORTHOUI_INFO*			Get_Name_Rect_Info() { return &m_tName_Rect; }
	ORTHOUI_INFO*			Get_Ok_Rect_Info() { return &m_tOk_Rect; }
	ORTHOUI_INFO*			Get_Cancel_Rect_Info() { return &m_tCancel_Rect; }
};

#endif // !__TamingName_UI_H__