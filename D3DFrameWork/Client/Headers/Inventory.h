#pragma once
#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

class CInventory : public COrthoUI
{
private:
	typedef struct stInventory_Resolution_Info : STOURI
	{
		_float fLeftPosX[ESNRT_END] = { 0.9f*ScrnRatio ,  3.34f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 1.45f*ScrnRatio , 4.5f*ScrnRatio };
		_float fSizeX[ESNRT_END] = { 1.15f*ScrnRatio, 5.f*ScrnRatio };
		_float fSizeY[ESNRT_END] = { 2.25f*ScrnRatio, 7.5f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { { 0.004f ,0.002f  , 0.9914f, 0.996f },
										/* { 0.002f ,0.001f  , 0.997f, 0.997f }*/{ 0.002f ,0.001f  , 0.999f, 0.999f } };
	}STInventoryURI;

	STInventoryURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	explicit CInventory(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInventory(const CInventory& rhs);
	virtual ~CInventory() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static CInventory*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	virtual	_bool			UI_RenderOn() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);
	void					InvenOpenAni(const _double & _dTimeDelta);
	void					InvenOpen();

private:
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pFilter_TextureCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	_float					m_fFilterTex_PosY = 0.f;
	_float					m_fTexRatioX = 0.f;
	_float					m_fTexRatioY = 0.f;

	_bool					m_bInvenOpenAni = false;

	_bool					m_bLeftInventory = false;

};

#endif // !__INVENTORY_H__