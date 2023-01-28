#pragma once
#ifndef __InvenTap_H__
#define __InvenTap_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

class CInvenTap : public COrthoUI
{
private:
	typedef struct stInvenTap_Resolution_Info : STOURI
	{
		_float fPosY[ESNRT_END] = { 1.05f*ScrnRatio ,  3.45f*ScrnRatio };
		_float fSizeY[ESNRT_END] = { 0.15f*ScrnRatio, 0.6f*ScrnRatio };
		_float fFont_Left_Inventory_PosX[ESNRT_END] = { 0.39f*ScrnRatio, 0.98f*ScrnRatio };
		_float fFont_Right_Inventory_PosX[ESNRT_END] = { 3.22f*ScrnRatio, 13.3f*ScrnRatio };
		_float fFont_Left_Making_PosX[ESNRT_END] = { 1.15f*ScrnRatio , 4.6f*ScrnRatio };
		_float fFont_Right_Making_PosX[ESNRT_END] = { 1.15f*ScrnRatio , 10.4f*ScrnRatio };
		_float fFontPosY[ESNRT_END] = { 0.375f*ScrnRatio,0.96f*ScrnRatio };
		_float fFontSelectMovePosX[ESNRT_END] = { 0.02f*ScrnRatio ,0.2f*ScrnRatio };
		_float fFontSelectMovePosY[ESNRT_END] = { 0.02f*ScrnRatio ,0.05f*ScrnRatio };
	}STInvenTapURI;

	STInvenTapURI stURI;
	virtual HRESULT			Set_Screen_UI();

private:
	explicit CInvenTap(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInvenTap(const CInvenTap& rhs);
	virtual ~CInvenTap() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static CInvenTap*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	void					InvenTapSwitching();

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);
private:
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureCom2 = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	COrthoUI*				m_pInventory = nullptr;

private:
	_bool					m_bSelect_Tap_Item = true;

private:
	_int					m_iInvenTapOwner = NULL;
public:
	void					Set_TapOwner(_int _iOwner) { m_iInvenTapOwner = _iOwner; return; }
private:
	void					RenderFont();

	_bool					m_bForge = false;
};

#endif // !__InvenTap_H__