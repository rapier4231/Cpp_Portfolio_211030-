#pragma once
#ifndef __InvenDropAll_H__
#define __InvenDropAll_H__

#include "Value.h"
#include "InvenIcon.h"

class CInvenDropAll : public CInvenIcon
{
private:
	typedef struct stInvenDropAll_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { 0.025f*ScrnRatio ,   0.215f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 0.9f*ScrnRatio , /* 2.84f*ScrnRatio */ 2.845f*ScrnRatio };
	}STInvenDropAllURI;

	STInvenDropAllURI stURI;
	virtual HRESULT			Set_Screen_UI();

private:
	explicit CInvenDropAll(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInvenDropAll(const CInvenDropAll& rhs);
	virtual ~CInvenDropAll() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;
public:
	static CInvenDropAll*		Create(LPDIRECT3DDEVICE9 _pDevice);
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

#endif // !__InvenDropAll_H__