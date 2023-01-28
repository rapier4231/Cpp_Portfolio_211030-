#pragma once
#ifndef __Item_Info_View_UI_H__
#define __Item_Info_View_UI_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

#define ItemInfoViewSizeX 192.f 
#define ItemInfoViewSizeY 96.f 
#define ItemInfoViewAlpha 0.7f;
#define ItemTexAddPosX -66.f
#define ItemTexAddPosY -5.f

class CItem_Info_View_UI : public COrthoUI
{
private:
	typedef struct stItem_Info_View_UI_Resolution_Info : STOURI
	{
		_float fSizeX[ESNRT_END] = { 0.96f*ScrnRatio, 2.8f*ScrnRatio };
		_float fSizeY[ESNRT_END] = { 0.48f*ScrnRatio, 1.6f*ScrnRatio };
		_float fItemSizeXY[ESNRT_END] = { 0.18f*ScrnRatio, 0.5f*ScrnRatio };
		_float fTextureFixPosX[ESNRT_END] = { 0.33f*ScrnRatio, 0.94f*ScrnRatio };
		_float fTextureFixPosY[ESNRT_END] = { 0.025f*ScrnRatio, 0.08f*ScrnRatio };

		_float fMaking_Ingredient_Bg_FixSizeX[ESNRT_END] = { 0.45f*ScrnRatio, 1.2f*ScrnRatio };
		_float fMaking_Ingredient_Bg_IntervalX[ESNRT_END] = { 0.02f*ScrnRatio, 0.06f*ScrnRatio };
		_float fMaking_Ingredient_Item_SizeXY[ESNRT_END] = { 0.085f*ScrnRatio, 0.2f*ScrnRatio };
		_float fMaking_Ingredient_Item_IntervalX[ESNRT_END] = { 0.175f*ScrnRatio, 0.53f*ScrnRatio };
		_float fMaking_Ingredient_Item_IntervalY[ESNRT_END] = { 0.09f*ScrnRatio, 0.28f*ScrnRatio };
		_float fMaking_Ingredient_Item_FixY[ESNRT_END] = { 0.11f*ScrnRatio, 0.3f*ScrnRatio };

		_float fFont_ItemName_FixPosX[ESNRT_END] = { 0.09f*ScrnRatio, 0.38f*ScrnRatio };
		_float fFont_ItemName_FixPosY[ESNRT_END] = { 0.055f*ScrnRatio, 0.04f*ScrnRatio };
		_float fFont_ItemToolTip_FixPosX[ESNRT_END] = { 0.14f*ScrnRatio, 0.4f*ScrnRatio };
		_float fFont_ItemToolTip_FixPosY[ESNRT_END] = { 0.15f*ScrnRatio, 0.32f*ScrnRatio };
		_float fFont_ItemDurability_FixPosX[ESNRT_END] = { 0.1f*ScrnRatio, 0.36f*ScrnRatio };
		_float fFont_ItemDurability_FixPosY[ESNRT_END] = { 0.36f*ScrnRatio, 1.1f*ScrnRatio };
		_float fFont_Required_FixPosX[ESNRT_END] = { 0.91f*ScrnRatio, 2.6f*ScrnRatio };
		_float fFont_Required_FixPosY[ESNRT_END] = { 0.04f*ScrnRatio,  0.04f*ScrnRatio };
		_float fFont_IngredientRequired_FixPosX[ESNRT_END] = { 0.95f*ScrnRatio, 2.72f*ScrnRatio };
		_float fFont_IngredientRequired_FixPosY[ESNRT_END] = { 0.12f*ScrnRatio, 0.33f*ScrnRatio };
		_float fFont_IngredientRequired_IntervalY[ESNRT_END] = { 0.09f*ScrnRatio, 0.28f*ScrnRatio };
	}STItem_Info_View_UIURI;

	STItem_Info_View_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	explicit CItem_Info_View_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CItem_Info_View_UI(const CItem_Info_View_UI& rhs);
	virtual ~CItem_Info_View_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;


public:
	static	CItem_Info_View_UI* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;		

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0, _int _IngredientItemNum = 0);

public:
	void					On_Item_Info_View(ITEM_INFO _tItem_Info, _vec2 ItemSlotPos);
	void					On_Making_Info_View(MAKING_INFO _tMaking_Info, _vec2 ItemSlotPos, MII_INFO _tMII);

private:
	void					Set_MyPos(_vec2 ItemSlotPos);

private:
	CTexture*				m_pItemInfoView_BgCom = nullptr;
	CTexture*				m_pItemTex_Com = nullptr;
	CTexture*				m_pMakingTex_Com[4] = {nullptr,nullptr ,nullptr ,nullptr };
	CShader*				m_pShaderCom = nullptr; 
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

private:
	ITEM_INFO				m_tView_Item_Info;
	MAKING_INFO				m_tView_Making_Info;
	MII_INFO				m_tMII;
private:
	void					Render_Font();
};

#endif // !__Item_Info_View_UI_H__