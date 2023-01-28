#pragma once
#ifndef __DinoList_UI_H__
#define __DinoList_UI_H__

#include "Value.h"
#include "OrthoUI.h"
#include "Text_Input.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END
class CUnit;

class CDinoList_UI : public COrthoUI, public CText_Input
{
public:
	typedef struct stRing_Command
	{
		WCHAR * pwchCommand_Text = nullptr;
		_bool	bCharging_Click = false;
	}STRCD;

private:
	typedef struct stDinoList_UI_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { WINCX*FHALF, WINCX*FHALF };
		_float fPosY[ESNRT_END] = { WINCY*FHALF ,WINCY*FHALF };
		_float fSizeX[ESNRT_END] = { 3,   7.4f *ScrnRatio };
		_float fSizeY[ESNRT_END] = { WINCY, 7.4f *ScrnRatio };

	}STDinoList_UIURI;

	STDinoList_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

	typedef struct stSplit_Angles
	{
		_float fRadin = 0.f;
		_float fLeft_Angle = 0.f;
		_float fRight_Angle = 0.f;
		_float fDis_Fill = 0.f;
	}STSAs;

private:
	explicit CDinoList_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDinoList_UI(const CDinoList_UI& rhs);
	virtual ~CDinoList_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static	CDinoList_UI* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void					Open_DinoList(vector<STRCD> _vecCommand);

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0);
	HRESULT					SetUp_ConstantTable_To_EffectUI(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0);

private:
	CTexture*				m_pRing_Command_Tex = nullptr;
	CShader*				m_pShaderCom = nullptr; 
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

private:
	_int					Mouse_Func(const _double& _dTimeDelta);
	void					Select_Func(_int _iSelectCommand);
private:
	void					Split_To_Circle();
private:
	void					Render_Font();
private:
	void					Ef_Func(const _double& _dTimeDelta);
private:
	CUnit*					m_pUnit = nullptr;
	_float					m_fOpen = 0.f;
	_float					m_fRotate[3] = {0.f,0.f,0.f};
	_float					m_fEf_UI_Alpha = 0.f;
	_float					m_fEf_UI_Size = 0.f;
	vector<STSAs>			m_vecSAs;
	_int					m_iCommandAmount = 0;
	_float					m_fDis_Command_HalfGap = 1.f;
	vector<STRCD>			m_vecCommand;
};

#endif // !__DinoList_UI_H__