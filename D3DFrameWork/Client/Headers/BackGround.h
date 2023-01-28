#pragma once
#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "Value.h"
#include "GameObject.h"

BEGIN(Engine)
// 전방선언
class CTexture;
class CVIBuffer_RectTex;
class CTransform;
class CShader;
END

class CBackGround : public CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;

public:
	virtual HRESULT		Ready_GameObject_Prototype() override;
	virtual HRESULT		Ready_GameObject(void* _pArg = nullptr) override;
	virtual _int		Update_GameObject(const _double & _dTimeDelta) override;
	virtual void		Render_GameObject() override;

public:
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone(void* _pArg = nullptr) override;
	virtual void Free() override;

private:
	CVIBuffer_RectTex*	m_pVIBuffer = nullptr;
	CTransform*			m_pTransform = nullptr;
	CTexture*			m_pTexture = nullptr;
	CShader*			m_pShader = nullptr;

	_mat				m_matScreenProj;
};

#endif // !__BACKGROUND_H__
