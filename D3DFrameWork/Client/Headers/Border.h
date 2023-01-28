#pragma once
#ifndef __Border_H__
#define __Border_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_RectTex;
class CTexture;
END

class CBorder : public COrthoUI
{
private:
	virtual HRESULT			Set_Screen_UI() { return S_OK; };

private:
	explicit CBorder(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBorder(const CBorder& rhs);
	virtual ~CBorder() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static CBorder*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);
private:
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	CTexture*				m_pOrderUseTex = nullptr;

	ORTHOUI_INFO*			m_pMasterUIInfo = nullptr;

private:
	//void					OrderCheck();

public:
	void					Set_BorderRenderID(RENDERID _eRenderID) { m_eMasterRenderID = _eRenderID;}
private:
	RENDERID				m_eMasterRenderID = RENDER_END;




public:
	void					Set_Scroll_Show(_float _fScroll_MoveY, _float _fShow_TopY, _float _fShow_BottomY);
	_float					m_fScroll_FixY = 0.f;
	_float					m_fDrawCutRatio = 0.f;
	_int					m_iDrawCutUp = 0; //0은 안하는거, 1은 위 , 2는 아래
	_bool					m_bInvenItemSlot = false;


};

#endif // !__Border_H__