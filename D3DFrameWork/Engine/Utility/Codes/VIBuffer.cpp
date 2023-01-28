#include "..\Headers\VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_iVertexSize(rhs.m_iVertexSize)
	, m_iVertexCount(rhs.m_iVertexCount)
	, m_iFVF(rhs.m_iFVF)
	, m_iIndexSize(rhs.m_iIndexSize)
	, m_iTriCount(rhs.m_iTriCount)
	, m_IndexFmt(rhs.m_IndexFmt)
	, m_pVertices(rhs.m_pVertices)
	, m_pConvertVertices(rhs.m_pConvertVertices)
	, m_pIndices(rhs.m_pIndices)
	, m_bParticle(rhs.m_bParticle)
{
	if (!m_bParticle)
	{
		m_pVB->AddRef();
		m_pIB->AddRef();
	}
	//Safe_AddRef(m_pVB);
	//Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Ready_Component_Prototype()
{
	// 버텍스 버퍼 생성
	if (FAILED(m_pDevice->CreateVertexBuffer(
		m_iVertexSize * m_iVertexCount,
		0,
		m_iFVF,
		D3DPOOL_MANAGED,
		&m_pVB, nullptr)))
	{
		//ERR_MSG(L"Error", L"Failed To CreateVertexBuffer");
		return E_FAIL;
	}

	// 인덱스 버퍼 생성
	if (0 != m_iIndexSize)
	{
		if (FAILED(m_pDevice->CreateIndexBuffer(
			m_iIndexSize * m_iTriCount,
			0,
			m_IndexFmt,
			D3DPOOL_MANAGED,
			&m_pIB, nullptr)))
		{
			//ERR_MSG(L"Error", L"Failed To CreateIndexBuffer");
			return E_FAIL;
		}
	}

	// 버퍼 크기만큼 동적할당
	m_pVertices = new char[m_iVertexSize * m_iVertexCount];
	m_pConvertVertices = new char[m_iVertexSize * m_iVertexCount];
	m_pIndices = new char[m_iIndexSize * m_iTriCount];

	return S_OK;
}

HRESULT CVIBuffer::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render_VIBuffer()
{
	if (FAILED(m_pDevice->SetStreamSource(0, m_pVB, 0, m_iVertexSize)))
	{
		//ERR_MSG(L"Error", L"Failed To SetStreamSource");
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetFVF(m_iFVF)))
	{
		//ERR_MSG(L"Error", L"Failed To SetFVF");
		return E_FAIL;
	}

	// 랜더러 만들기 전 임시
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);

	return S_OK;
}

void CVIBuffer::Free()
{
	if (!m_bParticle)
	{
		if (!m_bIsClone)
		{
			Safe_Delete_Array(m_pVertices);
			Safe_Delete_Array(m_pConvertVertices);
			Safe_Delete_Array(m_pIndices);
		}

		Safe_Release(m_pVB);
		Safe_Release(m_pIB);
	}

	CComponent::Free();
}
