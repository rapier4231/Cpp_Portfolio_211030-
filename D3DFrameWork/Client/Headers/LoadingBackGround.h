#pragma once
#ifndef __LOADINGBACKGROUND_H__
#define __LOADINGBACKGROUND_H__

#include "Value.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer_RectTex;
class CTransform;
class CShader;

END

class CLoadingBackGround : public CGameObject
{
private:
	explicit CLoadingBackGround(LPDIRECT3DDEVICE9 _pDevice);
	explicit CLoadingBackGround(const CLoadingBackGround& rhs);
	virtual ~CLoadingBackGround() = default;

public:
	virtual HRESULT		Ready_GameObject_Prototype() override;
	virtual HRESULT		Ready_GameObject(void* _pArg = nullptr) override;
	virtual _int		Update_GameObject(const _double & _dTimeDelta) override;
	virtual void		Render_GameObject() override;

public:
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect);			// 로딩 백그라운드 스크린
	HRESULT				SetUp_ConstantTableIcon(LPD3DXEFFECT& pEffect);		// 로딩 백그라운드 아이콘

public:
	static CLoadingBackGround* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone(void* _pArg = nullptr) override;
	virtual void Free() override;

private:
	CVIBuffer_RectTex*	m_pVIBuffer = nullptr;
	CVIBuffer_RectTex*	m_pVIBufferIcon = nullptr;
	CTransform*			m_pTransform = nullptr;
	CTexture*			m_pTexture = nullptr;
	CTexture*			m_pTextureIcon = nullptr;
	CShader*			m_pShader = nullptr;

	_mat				m_matScreenProj;
	_double				m_dFrame;
};

#endif // !__LOADINGBACKGROUND_H__
