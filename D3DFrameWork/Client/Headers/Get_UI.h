#pragma once
#ifndef __Get_UI_H__
#define __Get_UI_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END
class CGet_UI : public COrthoUI
{
private:
	explicit CGet_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CGet_UI(const CGet_UI& rhs);
	virtual ~CGet_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static	CGet_UI* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0);

public:
	void					Player_Hit_On();

private:
	CTexture*				m_pHitTexCom = nullptr;
	CShader*				m_pShaderCom = nullptr; 
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

private:
	_float					m_fHit_UI_Time = 0.f;
	_float					m_fHitspread = 0.f;

};

#endif // !__Get_UI_H__