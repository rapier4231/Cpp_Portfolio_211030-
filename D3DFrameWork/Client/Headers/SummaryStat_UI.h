#pragma once
#ifndef __SummaryStat_UI_H__
#define __SummaryStat_UI_H__

#include "Value.h"
#include "OrthoUI.h"

#define SUMMARYSTATNOMALALPHA 0.9f
#define SUMMARYSTATDANGEROUSALPHA 0.7f
#define SUMMARYSTATEMERGENCYALPHA 0.3f
#define BlinkTrueTime1 0.0
#define BlinkFalseTime2 3.0
#define BlinkTrueTime3 6.0
#define BlinkFalseTime4 9.0
#define BlinkTrueTime5 12.0
#define Health_Warning_Ratio 0.3f
#define Stamina_Warning_Ratio 0.1f
#define Food_Warning_Ratio 0.1f
#define Weight_Warning_Ratio 0.95f

#define XP_BlinkTime1 8.0   //몬지 잘 몰겠다!!
#define XP_BlinkTime2 16.0
//몬스터도 공용으로 쓰기 위해 헤더로 옮김

class CUnit;
BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

class CSummaryStat_UI : public COrthoUI
{
private:
	typedef struct stSummaryStat_UI_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { 3.9f*ScrnRatio, 15.7f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 2.895f*ScrnRatio , 8.68f*ScrnRatio };
		_float fIntervalPosY[ESNRT_END] = { 0.175f*ScrnRatio , 0.54f*ScrnRatio };
		_float fSizeXY[ESNRT_END] = { 0.15f*ScrnRatio, 0.4f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { { 0.03f ,0.03f ,0.97f ,0.97f },{ 0.03f ,0.03f ,0.97f ,0.97f } };
	}STSummaryStat_UIURI;

	STSummaryStat_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

public:
	enum ESummaryStat { SummaryStat_Health, SummaryStat_Stamina, SummaryStat_Food, SummaryStat_Water, SummaryStat_Weight, SummaryStat_XP, SummaryStat_END };

private:
	explicit CSummaryStat_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSummaryStat_UI(const CSummaryStat_UI& rhs);
	virtual ~CSummaryStat_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static CSummaryStat_UI*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	virtual	_bool			UI_RenderOn() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);

private:
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pFill_TextureCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

	CUnit*					m_pPlayer = nullptr;
	ESummaryStat			m_eSummaryType = SummaryStat_END;
	_float					m_fMaxPlayerStat = FNULL;
	_float					m_fNowPlayerStat = FNULL;
	_float					m_fPerPlayerStat = FNULL;
	_int					m_iShaderPass = 2;
	_bool					m_bBlink = false;
	_bool					m_bBlinkChange = false;
	_double					m_dBlinkTime = FNULL;
};

#endif // !__SummaryStat_UI_H__