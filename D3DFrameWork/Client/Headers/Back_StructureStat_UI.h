#pragma once
#ifndef __Back_StructureStat_UI_H__
#define __Back_StructureStat_UI_H__

#include "Value.h"
#include "Center_Back_UI.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_RectTex;
END
class CUnit;
class CBack_StructureStat_UI : public CCenter_Back_UI
{
private:
	typedef struct stBack_StructureStat_UI_Resolution_Info : STOURI
	{
		_float fSizeY[ESNRT_END] = { 2.25f*ScrnRatio,  5.2f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 1.45f*ScrnRatio ,3.3516f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { { 0.004f ,0.002f  , 0.9914f, 0.996f },
			/* { 0.001f ,0.001f  , 0.995f, 0.997f }*/{ 0.003f ,0.001f  , 0.995f, 0.997f } };

		_float fButton_Rect_SizeX[ESNRT_END] = { 1.f*ScrnRatio, 1.7f*ScrnRatio };
		_float fButton_Rect_SizeY[ESNRT_END] = { 1.f*ScrnRatio, 0.38f*ScrnRatio };
		_float fButton_Rect_PosY[ESNRT_END] = { 1.45f*ScrnRatio ,5.52f*ScrnRatio };
		_vec4  v4Button_Rect_EdgeSize[ESNRT_END] = { { 0.004f ,0.002f  , 0.9914f, 0.996f },
			/* { 0.001f ,0.001f  , 0.995f, 0.997f }*/{ 0.005f ,0.01f  , 0.995f, 0.99f } };

		_float fFont_Button_PosX[ESNRT_END] = { WINCX*FHALF, WINCX*FHALF };
		_float fFont_Button_PosY[ESNRT_END] = { WINCX*FHALF, 5.46f*ScrnRatio };
	}STBack_StructureStat_UIURI;

	STBack_StructureStat_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	explicit CBack_StructureStat_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBack_StructureStat_UI(const CBack_StructureStat_UI& rhs);
	virtual ~CBack_StructureStat_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static CBack_StructureStat_UI*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	virtual	_bool			UI_RenderOn() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);
	virtual void			Center_Back_UIOpenAni(const _double & _dTimeDelta) override;
	void					Back_StructureStat_UIOpen();

private:
	CShader*				m_pButtonShaderCom = nullptr;
	ORTHOUI_INFO			m_tButton_Rect;
public:
	ORTHOUI_INFO*			Get_Button_Rect_Info() { return &m_tButton_Rect; }

private:
	void					Button_Click_Func();
	void					BranchRender_Font();
	CUnit*					m_pUnit = nullptr;
	InvenUserUnitTypeID		m_eType = InvenUserUnitTypeID_END;
public:
	void					Set_pUnit(CUnit* _pUnit);
};

#endif // !__Back_StructureStat_UI_H__