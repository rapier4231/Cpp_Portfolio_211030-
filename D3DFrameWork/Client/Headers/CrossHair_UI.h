#pragma once
#ifndef __CrossHair_UI_H__
#define __CrossHair_UI_H__

#include "Value.h"
#include "OrthoUI.h"
#include "Text_Input.h"

#define HitMakerFullAlpha 0.8f

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

class CCrossHair_UI : public COrthoUI
{
private:
	typedef struct stCrossHair_UI_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { WINCX*FHALF, WINCX*FHALF };
		_float fPosY[ESNRT_END] = { WINCY*FHALF ,WINCY*FHALF };
		_float fSizeXY[ESNRT_END] = { 1.f*ScrnRatio,  0.5f*ScrnRatio };
		_float fHitMakerSizeXY[ESNRT_END] = { 1.f*ScrnRatio,  0.9f*ScrnRatio };
	}STCrossHair_UIURI;

	STCrossHair_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	explicit CCrossHair_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCrossHair_UI(const CCrossHair_UI& rhs);
	virtual ~CCrossHair_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;


public:
	static	CCrossHair_UI*  Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0);

private:
	CTexture*				m_pCrossHair1_Com = nullptr;
	CTexture*				m_pCrossHair2_Com = nullptr;
	CShader*				m_pShaderCom = nullptr; 
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

	_float					m_fHitMakerAlpha = 0.f;

public:
	void					Render_CrossHair(_bool _bRender) { m_tORTHOUI_INFO.bRender = _bRender; }
	void					HitRender_On() { m_fHitMakerAlpha = HitMakerFullAlpha; }
};

#endif // !__CrossHair_UI_H__