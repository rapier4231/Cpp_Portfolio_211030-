#include "..\Headers\Texture.h"
#include "..\Headers\Prototype_Mgr.h"
//#include <PathCch.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi")

USING(Engine)

CTexture::CTexture(LPDIRECT3DDEVICE9 _pDevice, const wstring& _strPath, TEXTURETYPE _eTextureType, const _uint& _iCnt/* = 1*/)
	: CComponent(_pDevice)
	, m_strPath(_strPath)
	, m_eTextureType(_eTextureType)
	, m_iCnt(_iCnt)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_strPath(rhs.m_strPath)
	, m_eTextureType(rhs.m_eTextureType)
	, m_iCnt(rhs.m_iCnt)
{
	/// vector 깊은복사
	_uint	iContainerSize = (_int)rhs.m_vecTexture.size();

	m_vecTexture.reserve(iContainerSize);
	m_vecTexture = rhs.m_vecTexture;

	for (auto& iter : m_vecTexture)		// vector 안에 값이 컴객체이므로 레퍼런스 카운트 추가
		iter->AddRef();
}

HRESULT CTexture::Ready_Component_Prototype()
{
	m_vecTexture.reserve(m_iCnt);

	IDirect3DBaseTexture9* pTexture = nullptr;

	for (_uint i = 0; i < m_iCnt; ++i)
	{
		TCHAR szFullPath[MAX_PATH] = L"";
		wsprintf(szFullPath, m_strPath.c_str(), i);

		TCHAR szFileName[MAX_PATH] = L"";
		wsprintf(szFileName, m_strPath.c_str(), i);
		PathStripPath(szFileName);

		switch (m_eTextureType)
		{
		case Engine::TEX_NORMAL:
			//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture)))
			if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, szFileName, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		case Engine::TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.emplace_back(pTexture);
	}

	return S_OK;
}

HRESULT CTexture::Ready_Component(void * _pArg/* = nullptr*/)
{
	return S_OK;
}

void CTexture::Set_Texture(const _uint & _iIndex)
{
	if (m_vecTexture.size() < _iIndex)
		return;

	m_pDevice->SetTexture(0, m_vecTexture[_iIndex]);
}

void CTexture::Set_Texture(LPD3DXEFFECT & _pEffect, const char * _pConstantTable, const _uint & _iIndex)
{
	if (m_vecTexture.size() < _iIndex)
		return;

	// 텍스쳐를 그리기 하는 함수
	_pEffect->SetTexture(_pConstantTable, m_vecTexture[_iIndex]);
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 _pDevice, const wstring & _strPath, TEXTURETYPE _eTextureType, const _uint & _iCut)
{
	CTexture* pInstance = new CTexture(_pDevice, _strPath, _eTextureType, _iCut);
	
	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTexture::Clone(void * pArg/* = nullptr*/)
{
	CTexture* pClone = new CTexture(*this);

	if (FAILED(pClone->Ready_Component()))
		Safe_Release(pClone);

	return pClone;
}

void CTexture::Free()
{
	for (auto& iter : m_vecTexture)
		Safe_Release(iter);

	m_vecTexture.clear();
	m_vecTexture.shrink_to_fit();

	CComponent::Free();
}
