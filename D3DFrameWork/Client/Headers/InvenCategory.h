#pragma once
#ifndef __InvenCategory_H__
#define __InvenCategory_H__

#include "Value.h"
#include "InvenIcon.h"

class CInvenCategory : public CInvenIcon
{
private:
	typedef struct stInvenCategory_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { 0.325f*ScrnRatio ,  /*1.22f*ScrnRatio*/1.215f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 0.9f*ScrnRatio ,  /* 2.84f*ScrnRatio*/ 2.845f*ScrnRatio };
	}STInvenCategoryURI;

	STInvenCategoryURI stURI;
	virtual HRESULT			Set_Screen_UI();

private:
	explicit CInvenCategory(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInvenCategory(const CInvenCategory& rhs);
	virtual ~CInvenCategory() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;
public:
	static CInvenCategory*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);

private:
	CTexture*				m_pBoxNomalTextureCom = nullptr;
	CTexture*				m_pBoxHighlightTextureCom = nullptr;

	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	COrthoUI*					m_pInventory = nullptr;


};

#endif // !__InvenCategory_H__