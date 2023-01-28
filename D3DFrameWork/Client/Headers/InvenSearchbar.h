#pragma once
#ifndef __InvenSearchbar_H__
#define __InvenSearchbar_H__

#include "Value.h"
#include "InvenIcon.h"

class CInvenSearchbar : public CInvenIcon
{
private:
	typedef struct stInvenToolTip_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { 0.37f*ScrnRatio ,   /*1.58f*ScrnRatio */1.6f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 0.9f*ScrnRatio ,  /* 2.82f*ScrnRatio*/ 2.825f*ScrnRatio };
		_float fSizeX[ESNRT_END] = { 0.28f*ScrnRatio ,   1.3f*ScrnRatio };
		_float fSizeY[ESNRT_END] = { 0.07f*ScrnRatio, 0.32f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { { 0.005f ,0.03f  , 0.98f, 0.9f },
										 { 0.004f ,0.05f  , 0.99f, 0.98f } };
	}STInvenToolTipURI;

	STInvenToolTipURI stURI;
	virtual HRESULT			Set_Screen_UI();

private:
	explicit CInvenSearchbar(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInvenSearchbar(const CInvenSearchbar& rhs);
	virtual ~CInvenSearchbar() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static CInvenSearchbar*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);

private:
	CTexture*				m_pBoxNomalTextureCom = nullptr;
	CTexture*				m_pBoxHighlightTextureCom = nullptr;
	
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	COrthoUI*				m_pInventory = nullptr;

};

#endif // !__InvenSearchbar_H__