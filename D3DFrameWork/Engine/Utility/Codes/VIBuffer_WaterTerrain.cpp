#include "VIBuffer_WaterTerrain.h"

USING(Engine)

CVIBuffer_WaterTerrain::CVIBuffer_WaterTerrain(LPDIRECT3DDEVICE9 _pDevice, const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv)
	: CVIBuffer(_pDevice)
	, m_dwCntX(_dwCntX)
	, m_dwCntZ(_dwCntZ)
	, m_dwVtxItv(_dwVtxItv)
	, m_pPos(nullptr)
{
}

CVIBuffer_WaterTerrain::CVIBuffer_WaterTerrain(const CVIBuffer_WaterTerrain & rhs)
	: CVIBuffer(rhs)
	, m_dwCntX(rhs.m_dwCntX)
	, m_dwCntZ(rhs.m_dwCntZ)
	, m_dwVtxItv(rhs.m_dwVtxItv)
	, m_pPos(rhs.m_pPos)
{
}

HRESULT CVIBuffer_WaterTerrain::Ready_Component_Prototype()
{
	m_iTriCount = (m_dwCntX - 1) * (m_dwCntZ - 1) * 2;
	m_iVertexCount = m_dwCntX * m_dwCntZ;
	m_iVertexSize = sizeof(VTX_TEX);
	m_iIndexSize = sizeof(INDEX32);
	m_IndexFmt = D3DFMT_INDEX32;
	m_iFVF = FVF_VTX_TEX;

	m_pPos = new _vec3[m_iVertexCount];

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Component_Prototype(), E_FAIL);
	
	VTX_TEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong	dwIndex = 0;

	for (_ulong i = 0; i < m_dwCntZ; ++i)
	{
		for (_ulong j = 0; j < m_dwCntX; ++j)
		{
			dwIndex = i * m_dwCntX + j;

			pVertex[dwIndex].vPosition = _vec3(_float(j) * m_dwVtxItv, 0.f, _float(i) * m_dwVtxItv);
			pVertex[dwIndex].vTexUV = _vec2(_float(j) / (m_dwCntX - 1),	_float(i) / (m_dwCntZ - 1));

			m_pPos[dwIndex] = pVertex[dwIndex].vPosition;
		}
	}

	INDEX32*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong	dwTriIndex = 0;

	for (_ulong i = 0; i < m_dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < m_dwCntX - 1; ++j)
		{
			dwIndex = i * m_dwCntX + j;

			_vec3		vDest, vSour, vNormal;

			// 오른쪽 위
			pIndex[dwTriIndex]._1 = dwIndex + m_dwCntX;
			pIndex[dwTriIndex]._2 = dwIndex + m_dwCntX + 1;
			pIndex[dwTriIndex]._3 = dwIndex + 1;

			vDest = pVertex[pIndex[dwTriIndex]._2].vPosition - pVertex[pIndex[dwTriIndex]._1].vPosition;
			vSour = pVertex[pIndex[dwTriIndex]._3].vPosition - pVertex[pIndex[dwTriIndex]._2].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			dwTriIndex++;

			// 왼쪽 아래
			pIndex[dwTriIndex]._1 = dwIndex + m_dwCntX;
			pIndex[dwTriIndex]._2 = dwIndex + 1;
			pIndex[dwTriIndex]._3 = dwIndex;

			vDest = pVertex[pIndex[dwTriIndex]._2].vPosition - pVertex[pIndex[dwTriIndex]._1].vPosition;
			vSour = pVertex[pIndex[dwTriIndex]._3].vPosition - pVertex[pIndex[dwTriIndex]._2].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			dwTriIndex++;
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_WaterTerrain::Ready_Component(void * pArg/* = nullptr*/)
{
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Component(), E_FAIL);

	return S_OK;
}

HRESULT CVIBuffer_WaterTerrain::Render_VIBuffer()
{
	CVIBuffer::Render_VIBuffer();

	return S_OK;
}

CVIBuffer_WaterTerrain * CVIBuffer_WaterTerrain::Create(LPDIRECT3DDEVICE9 _pDevice, const _ulong & _dwCntX, const _ulong & _dwCntZ, const _ulong & _dwVtxItv)
{
	CVIBuffer_WaterTerrain*	pInstance = new CVIBuffer_WaterTerrain(_pDevice, _dwCntX, _dwCntZ, _dwVtxItv);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CVIBuffer_WaterTerrain::Clone(void * pArg/* = nullptr*/)
{
	return new CVIBuffer_WaterTerrain(*this);
}

void CVIBuffer_WaterTerrain::Free()
{
	if (false == m_bIsClone)
		Safe_Delete_Array(m_pPos);

	CVIBuffer::Free();
}
