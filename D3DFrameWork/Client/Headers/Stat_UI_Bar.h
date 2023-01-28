#pragma once
#ifndef __Stat_UI_Bar_H__
#define __Stat_UI_Bar_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END
class CUnit;

class CStat_UI_Bar : public COrthoUI
{
private:
	typedef struct stStat_UI_Bar_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { WINCX*FHALF - 0.07f*ScrnRatio, WINCX*FHALF - 0.2f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 1.39f*ScrnRatio , 4.6f*ScrnRatio };
		_float fIntervalPosY[ESNRT_END] = { 0.033f*ScrnRatio, 0.06f*ScrnRatio };
		_float fSizeX[ESNRT_END] = { 0.78f*ScrnRatio, 3.2f*ScrnRatio };
		_float fSizeY[ESNRT_END] = { 0.09f*ScrnRatio, 0.32f*ScrnRatio };
		
		_float fGlowSizeXY[ESNRT_END] = { 0.15f*ScrnRatio, 0.4f*ScrnRatio };
		_float fGlowMaxSizeXY[ESNRT_END] = { 0.225f*ScrnRatio, 0.9f*ScrnRatio };
		_float fGlow_IntervalPosX[ESNRT_END] = { 0.465f*ScrnRatio,1.9f*ScrnRatio };

		_float fStatIconSizeXY[ESNRT_END] = { 0.08f*ScrnRatio,0.32f*ScrnRatio };
		_float fStatIcon_IntervalPosX[ESNRT_END] = { 0.34f*ScrnRatio,1.34f*ScrnRatio };
		_float fPlusIconSizeXY[ESNRT_END] = {0.08f*ScrnRatio,0.28f*ScrnRatio };
		_float fPlusIcon_IntervalPosX[ESNRT_END] = { 0.47f*ScrnRatio,1.9f*ScrnRatio  };


		_vec4  v4Hp_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.01f ,0.9f ,0.9f },{ 0.005f, 0.03f ,0.995f ,0.985f } };
		_vec4  v4Hp_Plus_EdgeSize[ESNRT_END] = { { 0.006f, 0.01f ,0.99f ,0.9f },{ 0.05f, 0.03f ,0.994f ,0.97f } };
		_vec4  v4Stamina_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.06f ,0.9f ,0.93f },{ 0.006f, 0.04f ,0.995f ,0.96f } };
		_vec4  v4Stamina_Plus_EdgeSize[ESNRT_END] = { { 0.006f, 0.02f ,0.99f ,0.93f },{ 0.05f, 0.06f ,0.99f ,0.93f } };
		_vec4  v4Food_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.06f ,0.9f ,0.95f },{ 0.006f, 0.04f ,0.995f ,0.97f } };
		_vec4  v4Food_Plus_EdgeSize[ESNRT_END] = { { 0.006f, 0.05f ,0.99f ,0.95f },{ 0.05f, 0.05f ,0.99f ,0.95f } };
		_vec4  v4Water_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.05f ,0.9f ,0.9f },{ 0.006f, 0.05f ,0.995f ,0.96f } };
		_vec4  v4Water_Plus_EdgeSize[ESNRT_END] = { { 0.006f, 0.01f ,0.99f ,0.95f },{ 0.05f, 0.01f ,0.99f ,0.95f } };
		_vec4  v4Weight_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.05f ,0.9f ,0.95f },{ 0.006f, 0.03f ,0.995f ,0.95f } };
		_vec4  v4Weight_Plus_EdgeSize[ESNRT_END] = { { 0.006f, 0.05f ,0.99f ,0.95f },{ 0.05f, 0.05f ,0.99f ,0.95f } };
		_vec4  v4Atk_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.01f ,0.9f ,0.9f },{ 0.006f, 0.04f ,0.995f ,0.97f } };
		_vec4  v4Atk_Plus_EdgeSize[ESNRT_END] = { { 0.006f, 0.01f ,0.99f ,0.9f },{ 0.05f, 0.01f ,0.99f ,0.96f } };
		_vec4  v4Speed_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.05f ,0.9f ,0.95f },{ 0.006f, 0.05f ,0.995f ,0.95f } };
		_vec4  v4Speed_Plus_EdgeSize[ESNRT_END] = { { 0.006f, 0.05f ,0.99f ,0.95f },{ 0.05f, 0.05f ,0.99f ,0.96f } };
		_vec4  v4Engram_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.06f ,0.9f ,0.95f },{ 0.006f, 0.02f ,0.995f ,0.97f } };
		_vec4  v4Engram_Plus_EdgeSize[ESNRT_END] = { { 0.006f, 0.05f ,0.99f ,0.95f },{ 0.05f, 0.05f ,0.99f ,0.95f } };
		_vec4  v4Endure_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.02f ,0.9f ,0.93f },{ 0.006f, 0.02f ,0.995f ,0.96f } };
		_vec4  v4Endure_Plus_EdgeSize[ESNRT_END] = { { 0.006f, 0.02f ,0.99f ,0.95f },{ 0.05f, 0.05f ,0.99f ,0.97f } };
		_vec4  v4Paralysis_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.05f ,0.99f ,0.95f },{ 0.006f, 0.03f ,0.995f ,0.97f } };
		_vec4  v4EXP_Bar_EdgeSize[ESNRT_END] = { { 0.006f, 0.05f ,0.99f ,0.95f },{ 0.005f, 0.05f ,0.995f ,0.96f } };
	
		_float fEXP_Bar_IntervalPosX[ESNRT_END] = { 0.14f*ScrnRatio , 0.4f*ScrnRatio };
		_float fEXP_Bar_IntervalPosY[ESNRT_END] = { 0.03f*ScrnRatio , 0.03f*ScrnRatio };
	
		_float fFont_HP_Stat_IntervalPosX[ESNRT_END] = { 0.41f*ScrnRatio ,1.6f*ScrnRatio };
		_float fFont_HP_Stat_IntervalPosY[ESNRT_END] = { 0.03f*ScrnRatio ,0.058f*ScrnRatio };
		_float fFont_Stamina_Stat_IntervalPosX[ESNRT_END] = { 0.41f*ScrnRatio ,1.6f*ScrnRatio };
		_float fFont_Stamina_Stat_IntervalPosY[ESNRT_END] = { 0.025f*ScrnRatio ,0.07f*ScrnRatio };
		_float fFont_Food_Stat_IntervalPosX[ESNRT_END] = { 0.41f*ScrnRatio ,1.6f*ScrnRatio };
		_float fFont_Food_Stat_IntervalPosY[ESNRT_END] = { 0.025f*ScrnRatio ,0.025f*ScrnRatio };
		_float fFont_Water_Stat_IntervalPosX[ESNRT_END] = { 0.41f*ScrnRatio ,1.6f*ScrnRatio };
		_float fFont_Water_Stat_IntervalPosY[ESNRT_END] = { 0.025f*ScrnRatio ,0.025f*ScrnRatio };
		_float fFont_Weight_Stat_IntervalPosX[ESNRT_END] = { 0.41f*ScrnRatio ,1.6f*ScrnRatio };
		_float fFont_Weight_Stat_IntervalPosY[ESNRT_END] = { 0.025f*ScrnRatio ,0.025f*ScrnRatio };
		_float fFont_Atk_Stat_IntervalPosX[ESNRT_END] = { 0.41f*ScrnRatio ,1.6f*ScrnRatio };
		_float fFont_Atk_Stat_IntervalPosY[ESNRT_END] = { 0.025f*ScrnRatio ,0.02f*ScrnRatio };
		_float fFont_Speed_Stat_IntervalPosX[ESNRT_END] = { 0.41f*ScrnRatio ,1.6f*ScrnRatio };
		_float fFont_SpeedTypeDown_Stat_IntervalPosY[ESNRT_END] = { 0.02f*ScrnRatio ,0.07f*ScrnRatio };
		_float fFont_Speed_Stat_IntervalPosY[ESNRT_END] = { 0.02f*ScrnRatio ,0.06f*ScrnRatio };
		_float fFont_Engram_Stat_IntervalPosX[ESNRT_END] = { 0.41f*ScrnRatio ,1.6f*ScrnRatio };
		_float fFont_Engram_Stat_IntervalPosY[ESNRT_END] = { 0.02f*ScrnRatio ,0.05f*ScrnRatio };
		_float fFont_Endure_Stat_IntervalPosX[ESNRT_END] = { 0.41f*ScrnRatio ,1.6f*ScrnRatio };
		_float fFont_Endure_Stat_IntervalPosY[ESNRT_END] = { 0.02f*ScrnRatio ,0.05f*ScrnRatio };
		_float fFont_Paralysis_Stat_IntervalPosX[ESNRT_END] = { 0.41f*ScrnRatio ,1.6f*ScrnRatio };
		_float fFont_Paralysis_Stat_IntervalPosY[ESNRT_END] = { 0.02f*ScrnRatio ,0.05f*ScrnRatio };
		_float fFont_EXP_Stat_IntervalPosX[ESNRT_END] = { 0.49f*ScrnRatio ,1.8f*ScrnRatio };
		_float fFont_EXP_Stat_IntervalPosY[ESNRT_END] = { 0.02f*ScrnRatio ,0.06f*ScrnRatio };

		_float fFont_Stat_Name_IntervalPosX[ESNRT_END] = { 0.27f*ScrnRatio ,1.f*ScrnRatio };
		_float fFont_Stat_EXP_Name_IntervalPosX[ESNRT_END] = { 0.44f*ScrnRatio ,1.68f*ScrnRatio };

	}STStat_UI_BarURI;

	STStat_UI_BarURI stURI;
	virtual HRESULT			Set_Screen_UI() override;


private:
	explicit CStat_UI_Bar(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStat_UI_Bar(const CStat_UI_Bar& rhs);
	virtual ~CStat_UI_Bar() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static	CStat_UI_Bar*   Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0);

private:
	CTexture*				m_pStatTextureCom = nullptr;
	CTexture*				m_pPlusTextureCom = nullptr;
	CTexture*				m_pPanelTabTextGlowTextureCom = nullptr;
	CTexture*				m_pArrowUpTextureCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	CUI*					m_pBackStat_UI = nullptr;

private:
	ESTATbar				m_eStatType = STATbar_End;
	CUnit*					m_pPlayer = nullptr;
	_float					m_fMaxPlayerStat = FNULL;
	_float					m_fNowPlayerStat = FNULL;
	_float					m_fPerPlayerStat = FNULL;
	_float					m_fStatUp_CoolTime = 0.f;
	_vec2					m_fGlowSizeXY;
	_vec4					m_v4PlusEdge;

	_bool					m_bCanRiseStat = false;
private:
	void					Render_Font(_mat _matOrtho);

private:
	void					PlusMouseCheck(const _double & _dTimeDelta);
private:
	void					PlayerStatControll();

};

#endif // !__Stat_UI_Bar_H__