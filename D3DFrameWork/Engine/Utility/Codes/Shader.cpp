#include "..\Headers\Shader.h"



CShader::CShader(LPDIRECT3DDEVICE9 _pDevice, const wstring& _strFilePath)
	: CComponent(_pDevice)
	, m_strFilePath(_strFilePath)
{
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
{
	m_pEffect->AddRef();
}


HRESULT CShader::Ready_Component_Prototype()
{
	if (FAILED(D3DXCreateEffectFromFile(m_pDevice,
		m_strFilePath.c_str(),
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&m_pEffect,
		&m_pErrMsg)))
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}
	else if (NULL != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_warning", MB_OK);
	}

	return S_OK;
}

HRESULT CShader::Ready_Component(void * _pArg/* = nullptr*/)
{
	return S_OK;
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 _pDevice, const wstring & _strFilePath)
{
	CShader*	pInstance = new CShader(_pDevice, _strFilePath);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CShader::Clone(void * pArg/* = nullptr*/)
{
	CShader*	pClone = new CShader(*this);

	if (FAILED(pClone->Ready_Component(pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CShader::Free()
{
	Safe_Release(m_pEffect);

	CComponent::Free();
}
