#pragma once
#ifndef __SummaryStat_UI_Monster_H__
#define __SummaryStat_UI_Monster_H__

#include "Value.h"
#include "OrthoUI.h"

class CUnit;
class CDino;
BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

class CSummaryStat_UI_Monster : public COrthoUI
{
private:
	typedef struct stSummaryStat_UI_Monster_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { 3.9f*ScrnRatio, 3.9f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 0.105f*ScrnRatio , 0.105f*ScrnRatio };
		_float fIntervalPosY[ESNRT_END] = { 0.175f*ScrnRatio , 0.175f*ScrnRatio };
		_float fSizeXY[ESNRT_END] = { 0.15f*ScrnRatio, 0.15f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { { 0.03f ,0.03f ,0.97f ,0.97f },{ 0.03f ,0.03f ,0.97f ,0.97f } };
	}STSummaryStat_UI_MonsterURI;

	STSummaryStat_UI_MonsterURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

public:
	enum ESummaryStat_Monster {SummaryStat_Health, SummaryStat_Stamina, SummaryStat_Food, SummaryStat_Weight, SummaryStat_XP , SummaryStat_END};
private:
	explicit CSummaryStat_UI_Monster(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSummaryStat_UI_Monster(const CSummaryStat_UI_Monster& rhs);
	virtual ~CSummaryStat_UI_Monster() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static CSummaryStat_UI_Monster*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void					Set_RideMonster(CUnit* _pDino);

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

	CUnit*					m_pRidingMonster = nullptr;
	ESummaryStat_Monster	m_eSummaryType = SummaryStat_END;
	_float					m_fMaxMonsterStat = FNULL;
	_float					m_fNowMonsterStat = FNULL;
	_float					m_fPerMonsterStat = FNULL;
	_int					m_iShaderPass = 2;
	_bool					m_bBlink = false;
	_bool					m_bBlinkChange = false;
	_double					m_dBlinkTime = FNULL;
};

#endif // !__SummaryStat_UI_Monster_H__