#pragma once
#ifndef __InvenFolder_H__
#define __InvenFolder_H__

#include "Value.h"
#include "InvenIcon.h"

class CInvenFolder : public CInvenIcon
{
private:
	typedef struct stInvenFolder_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { 0.175f*ScrnRatio , 0.595f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 0.9f*ScrnRatio ,  /* 2.84f*ScrnRatio*/ 2.845f*ScrnRatio };
	}STInvenFolderURI;

	STInvenFolderURI stURI;
	virtual HRESULT			Set_Screen_UI();

private:
	explicit CInvenFolder(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInvenFolder(const CInvenFolder& rhs);
	virtual ~CInvenFolder() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;
public:
	static CInvenFolder*		Create(LPDIRECT3DDEVICE9 _pDevice);
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

#endif // !__InvenFolder_H__