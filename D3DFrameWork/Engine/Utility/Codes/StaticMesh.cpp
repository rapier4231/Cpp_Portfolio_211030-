#include "..\Headers\StaticMesh.h"
#include "..\Headers\Prototype_Mgr.h"

CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 _pDevice, const wstring& pFilePath, const wstring& pFileName)
	: CComponent(_pDevice)
	, m_strFilePath(pFilePath)
	, m_strFileName(pFileName)
{
}

CStaticMesh::CStaticMesh(const CStaticMesh & rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubset(rhs.m_pSubset)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pMtrl(rhs.m_pMtrl)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_dwVtxNum(rhs.m_dwVtxNum)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_strFilePath(rhs.m_strFilePath)
	, m_strFileName(rhs.m_strFileName)
	, m_dwIndexNum(rhs.m_dwIndexNum)
	, m_dwStride(rhs.m_dwStride)
	, m_pIndex(rhs.m_pIndex)
	, m_dwAttTableCnt(rhs.m_dwAttTableCnt)
{
	for (_uint i = 0; i < TEXTURE_END; ++i)
		m_ppTexture[i] = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_uint i = 0; i < TEXTURE_END; ++i)
	{
		for (_ulong j = 0; j < rhs.m_dwSubsetCnt; ++j)
		{
			if (rhs.m_ppTexture[i][j] == nullptr)
			{
				m_ppTexture[i][j] = nullptr;
				continue;
			}

			m_ppTexture[i][j] = rhs.m_ppTexture[i][j];
			m_ppTexture[i][j]->AddRef();
		}
	}

	m_pAttTable = new D3DXATTRIBUTERANGE[m_dwAttTableCnt];
	m_pMesh->GetAttributeTable(m_pAttTable, 0);

	// 컴객체 레퍼런스카운트 증가
	m_pAdjacency->AddRef();
	m_pSubset->AddRef();

	m_pOriMesh->AddRef();
	m_pMesh->AddRef();
}

HRESULT CStaticMesh::Ready_Component_Prototype()
{
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, m_strFilePath.c_str());
	lstrcat(szFullPath, m_strFileName.c_str());

	if (FAILED(D3DXLoadMeshFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pDevice,
		&m_pAdjacency,
		&m_pSubset,
		NULL,
		&m_dwSubsetCnt,
		&m_pOriMesh)))
		return E_FAIL;

	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	_ulong	dwFVF = m_pOriMesh->GetFVF();
	if (dwFVF == 0)		//Decl 정보 기반의 메시
		m_pOriMesh->CloneMesh(m_pOriMesh->GetOptions(), decl, m_pDevice, &m_pMesh);
	else
	{
		// FVF 정보 기반의 메시
		if (!(dwFVF & D3DFVF_NORMAL))
		{
			m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pDevice, &m_pMesh);
			D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
		}
		else
		{
			m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pDevice, &m_pMesh);
		}
	}

	//클론한 메시 최적화
	m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)m_pAdjacency->GetBufferPointer()
		, 0 /*(DWORD*)m_pAdjacency->GetBufferPointer()*/, 0, 0);
	//특성테이블 저장
	m_dwAttTableCnt;
	m_pMesh->GetAttributeTable(0, &m_dwAttTableCnt);
	if (0 == m_dwAttTableCnt)
	{
		ERR_MSG(L"CStaticMesh::Ready_Component_Prototype() : 메시 최적화를 마쳤으나 특성테이블의 사이즈가 0입니다.");
	}
	else
	{
		m_pAttTable = new D3DXATTRIBUTERANGE[m_dwAttTableCnt];
		m_pMesh->GetAttributeTable(m_pAttTable, 0);
		if (m_dwAttTableCnt != m_dwSubsetCnt)
		{
			ERR_MSG(L"서브셋 개수와 특성테이블 개수가 다를 수도 있네요 신기합니다.");
		}
	}

	// 메쉬가 지닌 정점 정보를 기준으로 충돌 박스를 만들어주자

	void*			pVertex = nullptr;

	m_dwVtxNum = m_pMesh->GetNumVertices();	// 메쉬가 지닌 정점의 개수를 반환

	m_pVtxPos = new _vec3[m_dwVtxNum];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9			Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) *MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);

	_ubyte			byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (_ubyte)Decl[i].Offset;
			break;
		}
	}
	
	m_dwVtxSize = D3DXGetDeclVertexSize(decl, 0);  //44;//D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwVtxNum; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (m_dwVtxSize * i + byOffset)));
	}

	// D3DXBUFFER가 갖고 있는 정보 중 가장 앞 주소를 리턴
	m_pMtrl = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	//m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	// 서브셋 개수만큼 텍스처 백터 사이즈 잡기 
	
	//// 0 디퓨즈, 1 노말, 2 스펙큘러 텍스처
	//for (_uint i = 0; i < TEXTURE_END; ++i)
	//	m_vecTexture[i].reserve(m_dwSubsetCnt);

	for (_uint i = 0; i < TEXTURE_END; ++i)
		m_ppTexture[i] = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar		szFileName[256] = L"";

		MultiByteToWideChar(CP_ACP, 0, m_pMtrl[i].pTextureFilename, (_int)strlen(m_pMtrl[i].pTextureFilename), szFileName, 256);

		lstrcpy(szFullPath, m_strFilePath.c_str());
		lstrcat(szFullPath, szFileName);
		wstring wstrFileName = szFileName;
		_uint iLength = (_int)wstrFileName.length();
		wstrFileName = wstrFileName.substr(0, iLength - 5);
		wstring wstrNormal = wstrFileName + L"N.tga";
		wstring wstrSpecular = wstrFileName + L"S.tga";
		wstring wstrAlpha = wstrFileName + L"A.tga";
		wstring wstrMask = wstrFileName + L"M.tga";


		// 디퓨즈 텍스처 추가
		//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[TEXTURE_DIFFUSE][i])))
		if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, szFileName, szFullPath, &m_ppTexture[TEXTURE_DIFFUSE][i])))
			return E_FAIL;

		// 노말 텍스처 추가
		if (1 == m_dwSubsetCnt)
		{
			wstring wstrSub = m_strFileName;				//노멀텍스처의 서브셋이 1일 경우 폴더명을 우선적으로 참고하여 가져올 것
			_uint iLength = (_uint)m_strFileName.length();
			wstrSub = wstrSub.substr(0, iLength - 2);		//MeshName.X -> MeshName
			wstring wstrTemp = wstrSub + L"_N.tga";			//MeshName -> MeshName_N.tga

			_tchar	szTempPath[256] = L"";
			lstrcpy(szTempPath, m_strFilePath.c_str());
			lstrcat(szTempPath, wstrTemp.c_str());			//StaticMesh\\MeshName\\MeshName_N.tga

			if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, wstrTemp.c_str(), szTempPath, &m_ppTexture[TEXTURE_NORMAL][i])))
				//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/DefaultNormal_Uncompressed.tga", &m_ppTexture[TEXTURE_NORMAL][i])))
			{
				//폴더명 검색에 실패하면 파일명 기준으로 찾는다
				lstrcpy(szFullPath, m_strFilePath.c_str());
				lstrcat(szFullPath, wstrNormal.c_str());
				if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, wstrNormal.c_str(), szFullPath, &m_ppTexture[TEXTURE_NORMAL][i])))
				{
					if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, L"DefaultNormal", L"../Resources/Texture/Terrain/DefaultNormal_Uncompressed.tga", &m_ppTexture[TEXTURE_NORMAL][i])))
						m_ppTexture[TEXTURE_NORMAL][i] = nullptr;
				}
			}
		}
		else
		{
			//서브셋이 2이상이면 바로 파일명 기준으로 찾는다
			lstrcpy(szFullPath, m_strFilePath.c_str());
			lstrcat(szFullPath, wstrNormal.c_str());
			if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, wstrNormal.c_str(), szFullPath, &m_ppTexture[TEXTURE_NORMAL][i])))
			{
				if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, L"DefaultNormal", L"../Resources/Texture/Terrain/DefaultNormal_Uncompressed.tga", &m_ppTexture[TEXTURE_NORMAL][i])))
					m_ppTexture[TEXTURE_NORMAL][i] = nullptr;
			}
		}

		// 스펙큘러 텍스처 추가
		//lstrcpy(szFullPath, m_strFilePath.c_str());
		//lstrcat(szFullPath, wstrSpecular.c_str());
		//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pTexture)))
		//{
		//	m_vecTexture[TEXTURE_SEPCULAR].emplace_back(nullptr);
		//}
		//else
		//	m_vecTexture[TEXTURE_SEPCULAR].emplace_back(pTexture);

		//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[TEXTURE_SPECULAR][i])))
		//	m_ppTexture[TEXTURE_SPECULAR][i] = nullptr;


		// 마스크 텍스처 추가
		lstrcpy(szFullPath, m_strFilePath.c_str());
		lstrcat(szFullPath, wstrMask.c_str());
		//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[TEXTURE_MASK][i])))
		if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, wstrMask.c_str(), szFullPath, &m_ppTexture[TEXTURE_MASK][i])))
			m_ppTexture[TEXTURE_MASK][i] = nullptr;

	}

	m_pMesh->UnlockVertexBuffer();

	m_dwIndexNum = m_pMesh->GetNumFaces();

	LPDIRECT3DINDEXBUFFER9 pIB = nullptr;
	m_pMesh->GetIndexBuffer(&pIB);
	D3DINDEXBUFFER_DESC desc;
	pIB->GetDesc(&desc);

	m_dwStride = desc.Format == D3DFMT_INDEX16 ? 3 * sizeof(WORD) : 3 * sizeof(DWORD);

	Safe_Release(pIB);

	m_pIndex = new BYTE[m_dwStride * m_dwIndexNum];
	void* pBits = nullptr;

	m_pMesh->LockIndexBuffer(D3DLOCK_READONLY, &pBits);

	memcpy(m_pIndex, pBits, m_dwStride * m_dwIndexNum);

	m_pMesh->UnlockIndexBuffer();

	return S_OK;
}

HRESULT CStaticMesh::Ready_Component(void * _pArg/* = nullptr*/)
{
	return S_OK;
}

void CStaticMesh::Render_Meshes(void)
{
	//D3DMATERIAL9 pMtrl;
	//ZeroMemory(&pMtrl, sizeof(D3DMATERIAL9));
	//pMtrl.Diffuse = { 1.f, 1.f, 1.f, 1.f };

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pDevice->SetTexture(0, m_ppTexture[TEXTURE_DIFFUSE][i]);
		//m_pDevice->SetMaterial(&pMtrl);
		m_pMesh->DrawSubset(i);
	}
}

void CStaticMesh::Render_Meshes(LPD3DXEFFECT & _pEffect, _bool _bDissolve, _int _iPass)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		// 기본값
		_int	iPass = SHADER_PASS_NORMAL;

		// 알파 테스트 텍스쳐 && 디졸브 ON
		if (Find_Alpha(m_pMtrl[i].pTextureFilename) && _bDissolve)
			iPass = SHADER_PASS_DISSOLVE_A;

		// 알파 테스트 텍스쳐 && 디졸브 OFF
		else if (Find_Alpha(m_pMtrl[i].pTextureFilename) && !_bDissolve)
			iPass = SHADER_PASS_ALPHATEST;

		// 기본값 && 디졸브 ON
		else if (_bDissolve)
			iPass = SHADER_PASS_DISSOLVE;

		_pEffect->SetTexture("g_DiffuseTexture", m_ppTexture[TEXTURE_DIFFUSE][i]);
		_pEffect->SetTexture("g_NormalTexture", m_ppTexture[TEXTURE_NORMAL][i]);
		//_pEffect->SetTexture("g_SpecularTexture", m_ppTexture[TEXTURE_SPECULAR][i]);
		_pEffect->SetBool("g_bMaskTexture", false);
		_pEffect->CommitChanges();

		if(_iPass == 0)
			_pEffect->BeginPass(iPass);
		else
			_pEffect->BeginPass(_iPass);

		m_pMesh->DrawSubset(i);

		_pEffect->EndPass();
	}
}

void CStaticMesh::Render_MeshesShadowMap(LPD3DXEFFECT & _pEffect, _bool _bDissolve)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_int	iPass = 0;

		// 알파 테스트 텍스쳐 && 디졸브 OFF
		if (Find_Alpha(m_pMtrl[i].pTextureFilename) && !_bDissolve)
			iPass = 2;

		// 기본값 && 디졸브 ON
		else if (_bDissolve)
			iPass = 1;

		_pEffect->SetTexture("g_DiffuseTexture", m_ppTexture[TEXTURE_DIFFUSE][i]);

		_pEffect->CommitChanges();

		_pEffect->BeginPass(iPass);

		m_pMesh->DrawSubset(i);

		_pEffect->EndPass();
	}

}

void CStaticMesh::Render_MeshesShadow(LPD3DXEFFECT & _pEffect, _bool _bDissolve)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_vec4	vCol;
		vCol.x = m_pMtrl->MatD3D.Diffuse.r;
		vCol.y = m_pMtrl->MatD3D.Diffuse.g;
		vCol.z = m_pMtrl->MatD3D.Diffuse.b;
		vCol.w = m_pMtrl->MatD3D.Diffuse.a;

		_pEffect->SetVector("g_vCol", &vCol);

		_int	iPass = 0;

		// 알파 테스트 텍스쳐 && 디졸브 OFF
		if (Find_Alpha(m_pMtrl[i].pTextureFilename) && !_bDissolve)
			iPass = 2;

		// 기본값 && 디졸브 ON
		else if (_bDissolve)
			iPass = 1;

		_pEffect->SetTexture("g_DiffuseTexture", m_ppTexture[TEXTURE_DIFFUSE][i]);

		_pEffect->CommitChanges();

		_pEffect->BeginPass(iPass);

		m_pMesh->DrawSubset(i);

		_pEffect->EndPass();
	}
}

void CStaticMesh::Render_Subset_ForInstancing(LPD3DXEFFECT & _pEffect, _uint i/*Subset*/, _ulong iPass/*=0*/)
{
	//해당 서브셋의 패스 결정
	/*_int	iPass = SHADER_PASS_NORMAL;*/	//디폴트인자일 경우 기본패스
	
	if (Find_Alpha(m_pMtrl[i].pTextureFilename))
		++iPass;

	//해당 서브셋의 텍스처 세팅
	_pEffect->SetTexture("g_DiffuseTexture", m_ppTexture[TEXTURE_DIFFUSE][i]);
	_pEffect->SetTexture("g_NormalTexture", m_ppTexture[TEXTURE_NORMAL][i]);
	if(4 == iPass)
		_pEffect->SetTexture("g_Diffuse1Texture", m_ppTexture[TEXTURE_MASK][i]);
	_pEffect->CommitChanges();

	_pEffect->BeginPass(iPass);

	//특성테이블에서 해당 서브셋의 버텍스, 인덱스 정보 확인
	//_ulong dwTriCount = m_pAttTable[i].FaceCount;		//이번 서브셋에서 그릴 삼각형 숫자.
	//_ulong dwTriDrawn = m_pAttTable[i].FaceStart;		//그리기 완료한 삼각형 숫자가 누적됨. = 이번 서브셋에서 그릴 삼각형 인덱스(오프셋)
	//_ulong dwIndexOffset = dwTriDrawn * 3;			//여기에 삼각형의 정점 수(3)를 곱하면 이번 서브셋에서 그릴 정점 인덱스(오프셋)이 됨
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxNum, m_pAttTable[i].FaceStart * 3, m_pAttTable[i].FaceCount);	//m_pMesh->DrawSubset(i) 역할

	_pEffect->EndPass();
}

_bool CStaticMesh::Find_Alpha(const char * pFileName)
{
	size_t	iLength = strlen(pFileName);

	for (size_t i = 0; i < iLength + 1; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == 'A')
				return true;
		}
	}

	return false;
}

HRESULT CStaticMesh::RayCast_OnMesh(_vec3 * pOut, const _vec3 & vRayPos, const _vec3 & vRayDir, const _mat & matWorld)
{
	BOOL bHit = false;
	_float fDistance = -1.f;
	_float fU = 0.f, fV = 0.f, fDist = 0.f;
	_vec3 vRayPosLocal, vRayDirLocal;
	_mat matInvWorld;

	//월드 -> 로컬
	D3DXMatrixInverse(&matInvWorld, 0, &matWorld);
	D3DXVec3TransformCoord(&vRayPosLocal, &vRayPos, &matInvWorld);
	D3DXVec3TransformNormal(&vRayDirLocal, &vRayDir, &matInvWorld);

	//로컬공간에서 충돌 검출 시도
	D3DXIntersect(m_pMesh, &vRayPosLocal, &vRayDirLocal, &bHit, 0, &fU, &fV, &fDist, 0, 0);
	if (bHit)
	{
		//충돌점의 좌표를 월드로 돌려서 반환
		D3DXVec3TransformCoord(pOut, &(vRayPosLocal + vRayDirLocal * fDist), &matWorld);
		return S_OK;
	}
	else
		return E_FAIL;
}

CStaticMesh * CStaticMesh::Create(LPDIRECT3DDEVICE9 _pDevice, const wstring & pFilePath, const wstring & pFileName)
{
	CStaticMesh*	pInstance = new CStaticMesh(_pDevice, pFilePath, pFileName);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CStaticMesh::Clone(void * pArg/* = nullptr*/)
{
	CStaticMesh* pClone = new CStaticMesh(*this);

	if (FAILED(pClone->Ready_Component()))
		Safe_Release(pClone);

	return pClone;
}

void CStaticMesh::Free()
{
	// 디퓨즈 텍스쳐 제거
	for (_uint i = 0; i < TEXTURE_END; ++i)
	{
		for (_ulong j = 0; j < m_dwSubsetCnt; ++j)
		{
			if (m_ppTexture[i][j] == nullptr)
				continue;

			Safe_Release(m_ppTexture[i][j]);
		}

		Safe_Delete_Array(m_ppTexture[i]);
	}

	if (false == m_bIsClone)
		Safe_Delete_Array(m_pVtxPos);
	if (false == m_bIsClone)
		Safe_Delete_Array(m_pIndex);

	Safe_Release(m_pSubset);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pOriMesh);
	Safe_Release(m_pMesh);

	delete[] m_pAttTable;
	m_pAttTable = nullptr;

	CComponent::Free();
}
