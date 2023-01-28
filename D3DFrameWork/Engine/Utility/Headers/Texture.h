#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 _pDevice, const wstring& _strPath, TEXTURETYPE _eTextureType, const _uint& _iCnt = 1);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT			Ready_Component_Prototype() override;
	virtual HRESULT			Ready_Component(void * _pArg = nullptr) override;

public:
	void					Set_Texture(const _uint& _iIndex = 0);
	void					Set_Texture(LPD3DXEFFECT& _pEffect, const char* _pConstantTable, const _uint& _iIndex = 0);

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 _pDevice, const wstring& _strPath, TEXTURETYPE _eTextureType, const _uint& _iCnt = 1);
	virtual CComponent*		Clone(void * pArg = nullptr) override;
	virtual void			Free() override;

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	wstring								m_strPath;
	TEXTURETYPE							m_eTextureType;
	_uint								m_iCnt;
};
END
#endif // !__TEXTURE_H__
