#pragma once
#ifndef __InvenIcon_H__
#define __InvenIcon_H__

#include "Value.h"
#include "OrthoUI.h"

//#define ICONSIZEX	15.f
//#define ICONSIZEY	15.f
#define ICONNOMALALPHA	0.3f
#define ICONMOUSEALPHA	0.65f
#define ICONSELECTALPHA	0.85f

//#define ICONEDGESIZELEFT 0.03f
//#define ICONEDGESIZERIGHT 0.97f
//#define ICONEDGESIZETOP 0.03f
//#define ICONEDGESIZEBOTTOM 0.97f
#define ICONEDGENOMALALPHA	0.3f
#define ICONEDGEMOUSEALPHA	0.45f
#define ICONEDGESELECALPHA	0.7f


BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

class CInvenIcon : public COrthoUI
{
protected:
	typedef struct stInvenIcon_Resolution_Info : STOURI
	{
		_float fSizeXY[ESNRT_END] = { 0.075f*ScrnRatio, 0.35f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { { 0.03f ,0.03f  , 0.97f, 0.97f },
			/* { 0.02f ,0.01f  , 0.95f, 0.95f }*/{ 0.03f ,0.03f  , 0.97f, 0.985f } };
	}STInvenIconURI;

	STInvenIconURI stIconURI;
	virtual HRESULT			Set_Screen_UI()PURE;

protected:
	explicit CInvenIcon(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInvenIcon(const CInvenIcon& rhs);
	virtual ~CInvenIcon() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() PURE;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) PURE;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) PURE;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) PURE;
	virtual void			Render_GameObject(_mat _matOrtho) PURE;

public:
	virtual CUI*			Clone(void * _pArg = nullptr) PURE;
	virtual void			Free() PURE;

public:
	void					Set_LeftInvenIcon(_bool _LeftInvenIcon);

protected:
	virtual HRESULT			Add_Component(void) PURE;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

protected:
	CTexture*				m_pTextureCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	CUI*					m_pInventory = nullptr;

	_bool					m_bBoxSelect = false;
	_bool					m_bLeftInven = true;
};

#endif // !__InvenIcon_H__