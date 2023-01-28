#pragma once
#ifndef __Back_MonsterStat_UI_H__
#define __Back_MonsterStat_UI_H__

#include "Value.h"
#include "Center_Back_UI.h"

class CBack_MonsterStat_UI : public CCenter_Back_UI
{
private:
	typedef struct stBack_MonsterStat_UI_Resolution_Info : STOURI
	{
		_float fSizeY[ESNRT_END] = { 2.25f*ScrnRatio, 6.44f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 1.45f*ScrnRatio ,4.f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { { 0.004f ,0.002f  , 0.9914f, 0.996f },
			/* { 0.001f ,0.001f  , 0.995f, 0.997f }*/{ 0.003f ,0.002f  , 0.995f, 0.997f } };
	}STBack_MonsterStat_UIURI;

	STBack_MonsterStat_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	explicit CBack_MonsterStat_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBack_MonsterStat_UI(const CBack_MonsterStat_UI& rhs);
	virtual ~CBack_MonsterStat_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static CBack_MonsterStat_UI*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	virtual	_bool			UI_RenderOn() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);
	virtual void			Center_Back_UIOpenAni(const _double & _dTimeDelta) override;
	void					Back_MonsterStat_UIOpen();
};

#endif // !__Back_MonsterStat_UI_H__