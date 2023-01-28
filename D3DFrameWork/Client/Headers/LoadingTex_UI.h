#pragma once
#ifndef __LoadingTex_UI_H__
#define __LoadingTex_UI_H__

#include "Value.h"
#include "OrthoUI.h"
#include "Text_Input.h"

#define FinalLoadNum 194
//#define FinalLoadNum 87

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END
class CUnit;
class CLoading;
enum EDinoState {EDinoState_Stay, EDinoState_Walk, EDinoState_Run, EDinoState_Stun, EDinoState_End};

class CLoadingTex_UI : public COrthoUI
{
private:
	typedef struct stLoadingTex_UI_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { WINCX*FHALF, WINCX*FHALF };
		_float fPosY[ESNRT_END] = { WINCY*FHALF ,WINCY*FHALF };
		_float fSizeX[ESNRT_END] = { WINCX,  WINCX };
		_float fSizeY[ESNRT_END] = { WINCY, WINCY };

		_float fLogoPosX[ESNRT_END] = { WINCY,  12.88f*ScrnRatio };
		_float fLogoPosY[ESNRT_END] = { WINCY,  5.94f*ScrnRatio };
		_float fLogoSizeX[ESNRT_END] = { WINCY,  3.78f*ScrnRatio };
		_float fLogoSizeY[ESNRT_END] = { WINCY,  3.78f*ScrnRatio };

		_float fLoading_BackGroundPosX[ESNRT_END] = { WINCX*FHALF,   WINCX*FHALF };
		_float fLoading_BackGroundPosY[ESNRT_END] = { 15.f*ScrnRatio,  8.7f*ScrnRatio };
		_float fLoading_BackGroundSizeX[ESNRT_END] = { 15.f*ScrnRatio, 15.f*ScrnRatio };
		_float fLoading_BackGroundSizeY[ESNRT_END] = { 15.f*ScrnRatio,  0.4f*ScrnRatio };

		_float fLoading_BarPosX[ESNRT_END] = { 15.f*ScrnRatio, WINCX*FHALF };
		_float fLoading_BarPosY[ESNRT_END] = { 15.f*ScrnRatio, 8.7f*ScrnRatio };
		_float fLoading_BarSizeX[ESNRT_END] = { 15.f*ScrnRatio, 15.f*ScrnRatio };
		_float fLoading_BarSizeY[ESNRT_END] = { 15.f*ScrnRatio,  0.4f*ScrnRatio };

		_float fCromeDino_FixPosX[ESNRT_END] = { 15.f*ScrnRatio,  0.18f*ScrnRatio };
		_float fCromeDino_PosY[ESNRT_END] = { 15.f*ScrnRatio,  8.3f*ScrnRatio };
		_float fCromeDino_SizeXY[ESNRT_END] = { 15.f*ScrnRatio,  0.3f*ScrnRatio };
		_float fCromeRunDino_SizeX[ESNRT_END] = { 15.f*ScrnRatio,  0.4f*ScrnRatio };
		_float fCromeRunDino_SizeY[ESNRT_END] = { 15.f*ScrnRatio,  0.2f*ScrnRatio };
		
		_float fFont_GameStart_PosX[ESNRT_END] = { 15.f*ScrnRatio,  13.44f*ScrnRatio };
		_float fFont_GameStart_PosY[ESNRT_END] = { 15.f*ScrnRatio,  8.51f*ScrnRatio };
		_float fFont_GameStart_IntervalPosY[ESNRT_END] = { 15.f*ScrnRatio,  0.8f*ScrnRatio };

		_float fStart_Rect_SizeX[ESNRT_END] = { 15.f*ScrnRatio,  2.1f*ScrnRatio };
		_float fStart_Rect_SizeY[ESNRT_END] = { 15.f*ScrnRatio,  0.5f*ScrnRatio };

		_float fRect_PosX[ESNRT_END] = { 15.f*ScrnRatio,  14.44f*ScrnRatio };
		_float fStart_Rect_PosY[ESNRT_END] = { 15.f*ScrnRatio,  8.73f*ScrnRatio };
		_float fRect_Interval_PosY[ESNRT_END] = { 15.f*ScrnRatio,  0.8f*ScrnRatio };

	}STLoadingTex_UIURI;

	STLoadingTex_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	explicit CLoadingTex_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CLoadingTex_UI(const CLoadingTex_UI& rhs);
	virtual ~CLoadingTex_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;


public:
	static	CLoadingTex_UI* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0);

private:
	CTexture*				m_pLoadingTex1_Com = nullptr;
	CTexture*				m_pLoadingTex2_Com = nullptr;
	CTexture*				m_pCromeDinoTex_Com = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CShader*				m_pPuShaderCom = nullptr;
	CShader*				m_pGageShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

private:
	_bool					m_bLoadingStart = false;
	_bool					m_bLoadingEnd = false;
	_int					m_iTexNum = 4;
	_float					m_fLoadingBackTexNum = 0.f;
	_int					m_iDinoTexNum = NULL;
	_float					m_fDinoPosX = FNULL;
	stSC*					m_pstSC = nullptr;
	_float					m_fPerLoading = FNULL;
	_float					m_fDrawPerLoading = FNULL;
	_float					m_fDinoMoveChange = 0.f;
	EDinoState				m_eDinoState = EDinoState_End;
public:
	void					Set_pstSC(stSC* _pstSC) { m_pstSC = _pstSC; }

private:
	ORTHOUI_INFO			m_tStart_Rect;
	ORTHOUI_INFO			m_tSetting_Rect;
	ORTHOUI_INFO			m_tEnd_Rect;

private:
	void					Mouse_Button_Func();
	void					Button_Click_Func(_bool _bOk);

private:
	CLoading*				m_pLoading = nullptr;
};

#endif // !__LoadingTex_UI_H__