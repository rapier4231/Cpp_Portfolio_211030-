#pragma once
#ifndef __Center_Back_UI_H__
#define __Center_Back_UI_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END
#define Center_Back_UIALPHA	0.4f

#define FILTERTEXSPEEDCORRECTION 0.15f
#define OPENEDGEANISPEEDCORRECTION 1.f

#define Center_Back_UIEDGEALPHA	0.4f

class CCenter_Back_UI : public COrthoUI
{
protected:
	typedef struct stCenter_Back_UI_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { WINCX*FHALF , WINCX*FHALF };
		_float fSizeX[ESNRT_END] = { 1.f*ScrnRatio, 4.f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { { 0.004f ,0.002f  , 0.9914f, 0.996f },
			/* { 0.001f ,0.001f  , 0.995f, 0.997f }*/{ 0.003f ,0.001f  , 0.995f, 0.999f } };
	}STCenter_Back_UIURI;

	STCenter_Back_UIURI stURI;
	virtual HRESULT			Set_Screen_UI() override;

protected:
	explicit CCenter_Back_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCenter_Back_UI(const CCenter_Back_UI& rhs);
	virtual ~CCenter_Back_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() PURE;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) PURE;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) PURE;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) PURE;
	virtual void			Render_GameObject(_mat _matOrtho) PURE;

public:
	virtual CUI*			Clone(void * _pArg = nullptr) PURE;
	virtual void			Free() override;

public:
	virtual	_bool			UI_RenderOn() PURE;

protected:
	virtual HRESULT			Add_Component(void);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);
	virtual void			Center_Back_UIOpenAni(const _double & _dTimeDelta) PURE;
	void					Center_Back_UIOpen();

protected:
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pFilter_TextureCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	_float					m_fFilterTex_PosY = 0.f;
	_float					m_fTexRatioX = 0.f;
	_float					m_fTexRatioY = 0.f;

	_bool					m_bCenter_Back_UIOpenAni = false;

};

#endif // !__Center_Back_UI_H__