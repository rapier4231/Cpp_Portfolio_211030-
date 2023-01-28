#pragma once
#ifndef __CenterTap_H__
#define __CenterTap_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END
class CUnit;
class CCenterTap : public COrthoUI
{
private:
	typedef struct stCenterTap_Resolution_Info : STOURI
	{
		//_float fPosX[ESNRT_END] = { WINCX*FHALF , WINCX*FHALF };
		_float fPosY[ESNRT_END] = { 1.05f*ScrnRatio ,3.45f*ScrnRatio };
		_float fMonsterPosY[ESNRT_END] = { 1.05f*ScrnRatio ,2.8f*ScrnRatio };
		_float fSizeY[ESNRT_END] = { 0.15f*ScrnRatio, 0.6f*ScrnRatio };
		_float fFont_Inventory_FixPosX[ESNRT_END] = { 0.44f*ScrnRatio, 1.9f*ScrnRatio - 0.2f*ScrnRatio };
		_float fFont_Inventory_FixPosY[ESNRT_END] = { 0.375f*ScrnRatio, 0.96f*ScrnRatio - 0.05f*ScrnRatio };
		_float fFont_PlayerName_FixPosY[ESNRT_END] = { 0.17f*ScrnRatio, 0.68f*ScrnRatio };
		_float fFont_PlayerLevel_FixPosY[ESNRT_END] = { 0.3f*ScrnRatio, 1.1f*ScrnRatio };
		_float fFont_PlayerTribal_FixPosY[ESNRT_END] = { 0.42f*ScrnRatio, 1.65f*ScrnRatio };
		_float fFont_TribalContent_FixPosY[ESNRT_END] = { 0.48f*ScrnRatio, 1.9f*ScrnRatio };
		_float fFont_PlayerState_FixPosY[ESNRT_END] = { 0.62f*ScrnRatio, 2.45f*ScrnRatio };
		_float fFont_StateContent_FixPosY[ESNRT_END] = { 0.68f*ScrnRatio, 2.7f*ScrnRatio };

	}STCenterTapURI;

	STCenterTapURI stURI;
	virtual HRESULT			Set_Screen_UI() override;

private:
	explicit CCenterTap(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCenterTap(const CCenterTap& rhs);
	virtual ~CCenterTap() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static CCenterTap*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);
private:
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureCom2 = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	COrthoUI*				m_pCentertory = nullptr;

private:
	void					InvenTapSwitching();

private:
	_bool					m_bSelect_Tap_Item = true;

private:
	void					RenderFont();
private:
	_int					m_iInvenTapOwner = NULL;
	_int					m_iInvenTapPreOwner = NULL;
	CUnit*					m_pPreUnit = nullptr;
public:
	void					Set_TapOwner(_int _iOwner = 0, CUnit* _pPreUnit = nullptr);

private:
	_bool					m_bDaeSang = false;
public:
	void					Reset_DaeSangTap() { m_bDaeSang = false; }
	void					Set_DaeSangTap() { m_bDaeSang = true; }
};

#endif // !__CenterTap_H__