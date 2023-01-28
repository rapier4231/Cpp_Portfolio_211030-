#pragma once
#ifndef __SHADER_H__
#define __SHADER_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 _pDevice, const wstring& _strFilePath);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * _pArg = nullptr) override;

public:
 	LPD3DXEFFECT	Get_EffectHandle() { return m_pEffect; }

public:
	static CShader* Create(LPDIRECT3DDEVICE9 _pDevice, const wstring& _strFilePath);
	virtual CComponent * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	LPD3DXEFFECT		m_pEffect = nullptr;
	LPD3DXBUFFER		m_pErrMsg;
	wstring				m_strFilePath;
};
END
#endif // !__SHADER_H__
