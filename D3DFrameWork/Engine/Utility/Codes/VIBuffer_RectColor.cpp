#include "..\Headers\VIBuffer_RectColor.h"

USING(Engine)


CVIBuffer_RectColor::CVIBuffer_RectColor(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_RectColor::CVIBuffer_RectColor(const CVIBuffer_RectColor & rhs)
	: CVIBuffer(rhs)
{
}


HRESULT CVIBuffer_RectColor::Ready_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_COLOR);
	m_iIndexSize = sizeof(INDEX16);
	m_iFVF = FVF_VTX_COLOR;
	m_IndexFmt = D3DFMT_INDEX16;
	m_iVertexCount = 3;
	m_iTriCount = 1;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	VTX_COLOR* pVertex = nullptr;

	// 
	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);

	pVertex[0].vPosition = _vec3(0.f, 1.f, 0.f);
	pVertex[0].iColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPosition = _vec3(1.f, -1.f, 0.f);
	pVertex[1].iColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[2].vPosition = _vec3(-1.f, -1.f, 0.f);
	pVertex[2].iColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pVB->Unlock();

	//
	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_RectColor::Ready_Component(void * pArg/* = nullptr*/)
{
	CVIBuffer::Ready_Component(pArg);

	return S_OK;
}

HRESULT CVIBuffer_RectColor::Render_VIBuffer()
{
	if (FAILED(CVIBuffer::Render_VIBuffer()))
		return E_FAIL;

	/* 장치에게 인덱스버퍼 전달 */
	if (FAILED(m_pDevice->SetIndices(m_pIB)))
	{
		//ERR_MSG(L"Error", L"Failed To SetIndices");
		return E_FAIL;
	}

	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);

}

CVIBuffer_RectColor * CVIBuffer_RectColor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_RectColor* pInstance = new CVIBuffer_RectColor(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		//ERR_MSG(L"Error", L"Failed To Create CVIBuffer_CubeTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_RectColor::Clone(void * pArg/* = nullptr*/)
{
	CVIBuffer_RectColor* pClone = new CVIBuffer_RectColor(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		//ERR_MSG(L"Error", L"Failed To Clone CVIBuffer_CubeTexture");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVIBuffer_RectColor::Free()
{
	CVIBuffer::Free();
}
