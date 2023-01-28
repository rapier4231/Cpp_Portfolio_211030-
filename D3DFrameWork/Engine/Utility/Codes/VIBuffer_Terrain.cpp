#include "VIBuffer_Terrain.h"

USING(Engine)

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 _pDevice, const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv, _bool _bIsHMap/* = true*/)
	: CVIBuffer(_pDevice)
	, m_dwCntX(_dwCntX)
	, m_dwCntZ(_dwCntZ)
	, m_dwVtxItv(_dwVtxItv)
	, m_bIsHMap(_bIsHMap)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_dwCntX(rhs.m_dwCntX)
	, m_dwCntZ(rhs.m_dwCntZ)
	, m_dwVtxItv(rhs.m_dwVtxItv)
	, m_bIsHMap(rhs.m_bIsHMap)
	, m_hFile(rhs.m_hFile)
	, m_fileH(rhs.m_fileH)
	, m_infoH(rhs.m_infoH)
	, m_pPos(rhs.m_pPos)
{
}

HRESULT CVIBuffer_Terrain::Ready_Component_Prototype()
{
	m_iTriCount = (m_dwCntX - 1) * (m_dwCntZ - 1) * 2;
	m_iVertexCount = m_dwCntX * m_dwCntZ;
	//m_iVertexSize = sizeof(VTXTEX);
	m_iVertexSize = sizeof(VTX_COLOR);
	m_iIndexSize = sizeof(INDEX32);
	m_IndexFmt = D3DFMT_INDEX32;
	//m_iFVF = FVF_TEX;
	m_iFVF = FVF_VTX_COLOR;

	m_pPos = new _vec3[m_iVertexCount];

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Component_Prototype(), E_FAIL);



	/// 높이맵 설정
	_ulong*		pPixel = nullptr;

	if (m_bIsHMap)
	{
		_ulong dwByte = 0;

		m_hFile = CreateFile(L"../Bin/Resources/Texture/Terrain/Height.bmp",
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0);

		ReadFile(m_hFile, &m_infoH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
		ReadFile(m_hFile, &m_infoH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

		pPixel = new _ulong[m_infoH.biWidth * m_infoH.biHeight];

		ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_infoH.biWidth * m_infoH.biHeight, &dwByte, NULL);
		CloseHandle(m_hFile);
	}

	// 정점정보 세팅
	VTX_COLOR*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong	dwIndex = 0;

	for (_ulong i = 0; i < m_dwCntZ; ++i)
	{
		for (_ulong j = 0; j < m_dwCntX; ++j)
		{
			dwIndex = i * m_dwCntX + j;

			// 높이 맵 사용 여부 체크
			if (m_bIsHMap)
				pVertex[dwIndex].vPosition = _vec3(_float(j) * m_dwVtxItv, (pPixel[dwIndex] & 0x000000ff) / 20.f, _float(i) * m_dwVtxItv);
			else
				pVertex[dwIndex].vPosition = _vec3(_float(j) * m_dwVtxItv, 0.f, _float(i) * m_dwVtxItv);

			m_pPos[dwIndex] = pVertex[dwIndex].vPosition;
			pVertex[dwIndex].iColor = 0xff505050;
			//pVertex[dwIndex].vNormal = _vec3(0.f, 0.f, 0.f);

			//pVertex[dwIndex].vTexUV = _vec2(_float(j) / (m_dwCntX - 1) * 20.f,
			//	_float(i) / (m_dwCntZ - 1)* 20.f);
		}
	}


	Safe_Delete_Array(pPixel);

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

			//pVertex[pIndex[dwTriIndex]._1].vNormal += vNormal;
			//pVertex[pIndex[dwTriIndex]._2].vNormal += vNormal;
			//pVertex[pIndex[dwTriIndex]._3].vNormal += vNormal;

			dwTriIndex++;

			// 왼쪽 아래
			pIndex[dwTriIndex]._1 = dwIndex + m_dwCntX;
			pIndex[dwTriIndex]._2 = dwIndex + 1;
			pIndex[dwTriIndex]._3 = dwIndex;

			vDest = pVertex[pIndex[dwTriIndex]._2].vPosition - pVertex[pIndex[dwTriIndex]._1].vPosition;
			vSour = pVertex[pIndex[dwTriIndex]._3].vPosition - pVertex[pIndex[dwTriIndex]._2].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			//pVertex[pIndex[dwTriIndex]._1].vNormal += vNormal;
			//pVertex[pIndex[dwTriIndex]._2].vNormal += vNormal;
			//pVertex[pIndex[dwTriIndex]._3].vNormal += vNormal;

			dwTriIndex++;
		}
	}

	//for (_ulong i = 0; i < m_iVertexCount; ++i)
	//	D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Ready_Component(void * pArg/* = nullptr*/)
{
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Component(), E_FAIL);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Render_VIBuffer()
{
	CVIBuffer::Render_VIBuffer();

	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 _pDevice, const _ulong & _dwCntX, const _ulong & _dwCntZ, const _ulong & _dwVtxItv, _bool _bIsHMap/* = true*/)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(_pDevice, _dwCntX, _dwCntZ, _dwVtxItv, _bIsHMap);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg/* = nullptr*/)
{
	return new CVIBuffer_Terrain(*this);
}

void CVIBuffer_Terrain::Free()
{
	if (false == m_bIsClone)
		Safe_Delete_Array(m_pPos);

	CVIBuffer::Free();
}
