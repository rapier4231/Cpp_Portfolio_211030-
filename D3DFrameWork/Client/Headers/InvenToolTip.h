#pragma once
#ifndef __InvenToolTip_H__
#define __InvenToolTip_H__

#include "Value.h"
#include "InvenIcon.h"

class CInvenToolTip : public CInvenIcon
{
private:
	typedef struct stInvenToolTip_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { 0.425f*ScrnRatio , /* 1.62f*ScrnRatio */ 1.615f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 0.9f*ScrnRatio , /* 2.84f*ScrnRatio */ 2.845f*ScrnRatio };
	}STInvenToolTipURI;

	STInvenToolTipURI stURI;
	virtual HRESULT			Set_Screen_UI();

private:
	explicit CInvenToolTip(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInvenToolTip(const CInvenToolTip& rhs);
	virtual ~CInvenToolTip() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;
public:
	static CInvenToolTip*		Create(LPDIRECT3DDEVICE9 _pDevice);
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

#endif // !__InvenToolTip_H__