#include "..\Headers\VIBuffer_CubeTex.h"

USING(Engine)


CVIBuffer_CubeTex::CVIBuffer_CubeTex(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_CubeTex::CVIBuffer_CubeTex(const CVIBuffer_CubeTex & rhs)
	: CVIBuffer(rhs)
{
}


HRESULT CVIBuffer_CubeTex::Ready_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_CUBE);
	m_iIndexSize = sizeof(INDEX16);
	m_iFVF = FVF_CUBE;
	m_IndexFmt = D3DFMT_INDEX16;
	m_iVertexCount = 8;
	m_iTriCount = 12;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	VTX_CUBE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);

	// ¾Õ¸é
	pVertex[0].vPosition = _vec3(-1.f, 1.f, -1.f);
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(1.f, 1.f, -1.f);
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(1.f, -1.f, -1.f);
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(-1.f, -1.f, -1.f);
	pVertex[3].vTexUV = pVertex[3].vPosition;

	// µÞ¸é
	pVertex[4].vPosition = _vec3(-1.f, 1.f, 1.f);
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = _vec3(1.f, 1.f, 1.f);
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = _vec3(1.f, -1.f, 1.f);
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = _vec3(-1.f, -1.f, 1.f);
	pVertex[7].vTexUV = pVertex[7].vPosition;

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// X-
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// y+
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// y-
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// z+
	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;

	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;

	// z-
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_CubeTex::Ready_Component(void * pArg/* = nullptr*/)
{
	CVIBuffer::Ready_Component(pArg);

	return S_OK;
}

HRESULT CVIBuffer_CubeTex::Render_VIBuffer()
{
	return CVIBuffer::Render_VIBuffer();
}

CVIBuffer_CubeTex * CVIBuffer_CubeTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_CubeTex* pInstance = new CVIBuffer_CubeTex(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		//ERR_MSG(L"Error", L"Failed To Create CVIBuffer_CubeTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_CubeTex::Clone(void * pArg/* = nullptr*/)
{
	CVIBuffer_CubeTex* pClone = new CVIBuffer_CubeTex(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		//ERR_MSG(L"Error", L"Failed To Clone CVIBuffer_CubeTexture");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVIBuffer_CubeTex::Free()
{
	CVIBuffer::Free();
}
