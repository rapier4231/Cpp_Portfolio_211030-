#pragma once
#ifndef __Setting_UI_H__
#define __Setting_UI_H__

#include "Value.h"
#include "OrthoUI.h"
#include "Text_Input.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

class CSetting_UI : public COrthoUI
{
private:
	typedef struct stSetting_UI_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { WINCX*FHALF, WINCX*FHALF };
		_float fPosY[ESNRT_END] = { 1.45f*ScrnRatio ,4.5f*ScrnRatio };
		_float fSizeX[ESNRT_END] = { 1.f*ScrnRatio, 5.2f*ScrnRatio };
		_float fSizeY[ESNRT_END] = { 2.25f*ScrnRatio, 3.8f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { { 0.004f ,0.002f  , 0.9914f, 0.996f },
			/* { 0.001f ,0.001f  , 0.995f, 0.997f }*/{ 0.003f ,0.001f  , 0.996f, 0.999f } };
	}STSetting_UIURI;

	STSetting_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	explicit CSetting_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSetting_UI(const CSetting_UI& rhs);
	virtual ~CSetting_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static	CSetting_UI*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0);

private:
	CTexture*				m_pTexture_Com = nullptr;
	CShader*				m_pShaderCom = nullptr; 
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
};

#endif // !__Setting_UI_H__