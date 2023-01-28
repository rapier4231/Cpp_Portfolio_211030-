#include "..\Headers\VIBuffer_RectTex.h"

USING(Engine)


CVIBuffer_RectTex::CVIBuffer_RectTex(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_RectTex::CVIBuffer_RectTex(const CVIBuffer_RectTex & rhs)
	: CVIBuffer(rhs)
{
}


HRESULT CVIBuffer_RectTex::Ready_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_TEX);
	m_iIndexSize = sizeof(INDEX16);
	m_iFVF = FVF_VTX_TEX;
	m_IndexFmt = D3DFMT_INDEX16;
	m_iVertexCount = 4;
	m_iTriCount = 2;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	VTX_TEX* pVertex = nullptr;

	// 
	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	//
	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_RectTex::Ready_Component(void * pArg/* = nullptr*/)
{
	CVIBuffer::Ready_Component(pArg);

	return S_OK;
}

HRESULT CVIBuffer_RectTex::Render_VIBuffer()
{
	return CVIBuffer::Render_VIBuffer();
}

CVIBuffer_RectTex * CVIBuffer_RectTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_RectTex* pInstance = new CVIBuffer_RectTex(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		//ERR_MSG(L"Error", L"Failed To Create CVIBuffer_CubeTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_RectTex::Clone(void * pArg/* = nullptr*/)
{
	CVIBuffer_RectTex* pClone = new CVIBuffer_RectTex(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		//ERR_MSG(L"Error", L"Failed To Clone CVIBuffer_CubeTexture");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVIBuffer_RectTex::Free()
{
	CVIBuffer::Free();
}
