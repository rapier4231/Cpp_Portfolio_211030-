#include "AllocateHierarchy.h"

USING(Engine)

CAllocateHierarchy::CAllocateHierarchy(LPDIRECT3DDEVICE9 _pDevice, const wstring & pPath)
	: m_pDevice(_pDevice)
	, m_pPath(pPath)
{
	m_pDevice->AddRef();
}

STDMETHODIMP CAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED*	pDerivedFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pDerivedFrame, sizeof(D3DXFRAME_DERIVED));

	pDerivedFrame->CombinedTransformMatrix = *D3DXMatrixIdentity(&pDerivedFrame->TransformationMatrix);

	Allocate_Name(&pDerivedFrame->Name, Name);

	pDerivedFrame->pMeshContainer = NULL;
	pDerivedFrame->pFrameFirstChild = NULL;
	pDerivedFrame->pFrameSibling = NULL;

	*ppNewFrame = pDerivedFrame;

	return S_OK;
}

STDMETHODIMP CAllocateHierarchy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	SKINNEDMESHCONTAINER_DERIVED* boneMesh = new SKINNEDMESHCONTAINER_DERIVED;
	memset(boneMesh, 0, sizeof(SKINNEDMESHCONTAINER_DERIVED));

	boneMesh->pOrigMesh = pMeshData->pMesh;
	boneMesh->MeshData.pMesh = pMeshData->pMesh;
	boneMesh->MeshData.Type = pMeshData->Type;

	pMeshData->pMesh->AddRef();

	for (size_t i = 0; i < NumMaterials; ++i)
	{
		_tchar		szFullPath[256] = L"";
		_tchar		szFileName[256] = L"";

		//MultiByteToWideChar(CP_ACP, 0, boneMesh->pMaterials[i].pTextureFilename, (_uint)strlen(boneMesh->pMaterials[i].pTextureFilename), szFileName, 256);
		//
		//lstrcpy(szFullPath, m_pPath.c_str());
		//lstrcat(szFullPath, szFileName);
		//wstring wstrFileName = szFileName;
		//_uint iLength = (_int)wstrFileName.length();
		//wstrFileName = wstrFileName.substr(0, iLength - 5);
		//wstring wstrNormal = wstrFileName + L"N.tga";
		//wstring wstrSpecular = wstrFileName + L"S.tga";
		//wstring wstrAlpha = wstrFileName + L"A.tga";


		//D3DXMATERIAL mtrl;
		//memcpy(&mtrl, &pMaterials[i], sizeof(D3DXMATERIAL));
		//
		//IDirect3DTexture9* newTexture = NULL;
		//if (mtrl.pTextureFilename != NULL)
		//{
		//	char textureFname[200];
		//	strcpy(textureFname, "meshes/");
		//	strcat(textureFname, mtrl.pTextureFilename);
		//	MultiByteToWideChar(CP_ACP, 0, textureFname, (_uint)strlen(textureFname), szFileName, 256);
		//
		//	// 텍스처 로드
		//	D3DXCreateTextureFromFile(m_pDevice, textureFname, &newTexture);
		//}
	}
	
	if (pSkinInfo != NULL)
	{
		boneMesh->pSkinInfo = pSkinInfo;

		pSkinInfo->AddRef();

		pMeshData->pMesh->CloneMeshFVF(D3DXMESH_MANAGED,
			pMeshData->pMesh->GetFVF(),
			m_pDevice,
			&boneMesh->MeshData.pMesh);

		boneMesh->MeshData.pMesh->GetAttributeTable(NULL, &boneMesh->dwNumAttributeGroups);
		boneMesh->pAttributeTable = new D3DXATTRIBUTERANGE[boneMesh->dwNumAttributeGroups];
		boneMesh->MeshData.pMesh->GetAttributeTable(boneMesh->pAttributeTable, NULL);

		_int NumBones = pSkinInfo->GetNumBones();
		boneMesh->pBoneOffsetMatrices = new _mat[NumBones];
		boneMesh->pCurrentBoneMatrices = new _mat[NumBones];

		for (_int i = 0; i < NumBones; ++i)
			boneMesh->pBoneOffsetMatrices[i] = *(boneMesh->pSkinInfo->GetBoneOffsetMatrix(i));
	}


	///////////////
	*ppNewMeshContainer = boneMesh;

	return S_OK;

	//D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	//ZeroMemory(pDerivedMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	//Allocate_Name(&pDerivedMeshContainer->Name, Name);

	//pDerivedMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	//LPD3DXMESH		pMesh = pMeshData->pMesh;

	//_ulong	dwNumFaces = pMesh->GetNumFaces();	// 메쉬가 지닌 폴리곤 개수를 반환

	//pDerivedMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	//memcpy(pDerivedMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	//_ulong	dwFVF = pMesh->GetFVF();

	//////기존 FVF와 다르게 탄젠트 값까지 얻을수 있게 함
	////D3DVERTEXELEMENT9 decl[] =
	////{
	////	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	////	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	////	{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
	////	{ 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	////	D3DDECL_END()
	////};
	////
	////// Mesh Export 했을 때 decl에 체크 했을 경우(FVF에 0이 들감)
	////if (dwFVF == 0)
	////	pMesh->CloneMesh(pMesh->GetOptions(), decl, m_pDevice, &pDerivedMeshContainer->MeshData.pMesh);
	////// Mesh Export 했을 때 decl에 체크 안했을 경우(FVF모드)
	////else
	////	pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pDevice, &pDerivedMeshContainer->MeshData.pMesh);

	////////////////////////////////////////////////////////////////////////////
	//void*			pVertex = nullptr;

	//pDerivedMeshContainer->dwVtxNum = pMesh->GetNumVertices();	// 메쉬가 지닌 정점의 개수를 반환
	//pDerivedMeshContainer->pVtxPos = new _vec3[pDerivedMeshContainer->dwVtxNum];
	//pMesh->LockVertexBuffer(0, &pVertex);

	//D3DVERTEXELEMENT9			Decl[MAX_FVF_DECL_SIZE];
	//ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) *MAX_FVF_DECL_SIZE);

	//pMesh->GetDeclaration(Decl);

	//_ubyte			byOffset = 0;

	//for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	//{
	//	if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
	//	{
	//		byOffset = (_ubyte)Decl[i].Offset;
	//		break;
	//	}
	//}

	////pDerivedMeshContainer->dwVtxSize = D3DXGetDeclVertexSize(decl, 0);  //44;//D3DXGetFVFVertexSize(dwFVF);

	//for (_ulong i = 0; i < pDerivedMeshContainer->dwVtxNum; ++i)
	//{
	//	pDerivedMeshContainer->pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (pDerivedMeshContainer->dwVtxSize * i + byOffset)));
	//}
	//
	//pMesh->UnlockVertexBuffer();

	//pDerivedMeshContainer->dwIndexNum = pMesh->GetNumFaces();

	//LPDIRECT3DINDEXBUFFER9 pIB = nullptr;
	//pMesh->GetIndexBuffer(&pIB);
	//D3DINDEXBUFFER_DESC desc;
	//pIB->GetDesc(&desc);

	//pDerivedMeshContainer->dwStride = desc.Format == D3DFMT_INDEX16 ? 3 * sizeof(WORD) : 3 * sizeof(DWORD);

	//Safe_Release(pIB);

	//pDerivedMeshContainer->pIndex = new BYTE[pDerivedMeshContainer->dwStride * pDerivedMeshContainer->dwIndexNum];
	//void* pBits = nullptr;

	//pMesh->LockIndexBuffer(D3DLOCK_READONLY, &pBits);

	//memcpy(pDerivedMeshContainer->pIndex, pBits, pDerivedMeshContainer->dwStride * pDerivedMeshContainer->dwIndexNum);

	//pMesh->UnlockIndexBuffer();

	////////////////////////////////////////////////////////////////////////////

	//// 간혹가다 추출한 메쉬의 재질 정보가 없는 경우를 대비한 코드
	//pDerivedMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	//pDerivedMeshContainer->pMaterials = new D3DXMATERIAL[pDerivedMeshContainer->NumMaterials];
	//ZeroMemory(pDerivedMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

	//for (_uint i = 0; i < TEXTURE_END; ++i)
	//{
	//	pDerivedMeshContainer->ppTexture[i] = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	//	ZeroMemory(pDerivedMeshContainer->ppTexture[i], sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);
	//}

	//// 재질 정보가 제대로 있는 경우
	//if (0 != NumMaterials)
	//{
	//	memcpy(pDerivedMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

	//	for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
	//	{
	//		_tchar		szFullPath[256] = L"";
	//		_tchar		szFileName[256] = L"";

	//		MultiByteToWideChar(CP_ACP, 0, pDerivedMeshContainer->pMaterials[i].pTextureFilename, (_uint)strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename), szFileName, 256);

	//		lstrcpy(szFullPath, m_pPath.c_str());
	//		lstrcat(szFullPath, szFileName);
	//		wstring wstrFileName = szFileName;
	//		_uint iLength = (_int)wstrFileName.length();
	//		wstrFileName = wstrFileName.substr(0, iLength - 5);
	//		wstring wstrNormal = wstrFileName + L"N.tga";
	//		wstring wstrSpecular = wstrFileName + L"S.tga";
	//		wstring wstrAlpha = wstrFileName + L"A.tga";

	//		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pDerivedMeshContainer->ppTexture[TEXTURE_DIFFUSE][i])))
	//		{
	//			MSG_BOX("DynamicMesh's Texture Create Failed");
	//			return E_FAIL;
	//		}

	//		// 노말 텍스처 추가
	//		lstrcpy(szFullPath, m_pPath.c_str());
	//		lstrcat(szFullPath, wstrNormal.c_str());

	//		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i])))
	//			pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i] = nullptr;

	//		// 스펙큘러 텍스처 추가
	//		lstrcpy(szFullPath, m_pPath.c_str());
	//		lstrcat(szFullPath, wstrNormal.c_str());

	//		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pDerivedMeshContainer->ppTexture[TEXTURE_SPECULAR][i])))
	//			pDerivedMeshContainer->ppTexture[TEXTURE_SPECULAR][i] = nullptr;

	//	}
	//	
	//}
	//// 재질 정보가 없어서 강제로 재질을 한 개 생성한 경우
	//else
	//{
	//	pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	//	pDerivedMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	//	pDerivedMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	//	pDerivedMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	//	pDerivedMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

	//	pDerivedMeshContainer->ppTexture[0] = NULL;
	//}

	//// 스키닝 애니메이션을 구동하는 컴 객체가 없는 경우면 다이나믹 메쉬를 사용하지 못하고 있다고 말해도 과언이 아니다.
	//if (nullptr == pSkinInfo)
	//	return S_OK;

	//pDerivedMeshContainer->pSkinInfo = pSkinInfo;
	//pDerivedMeshContainer->pSkinInfo->AddRef();

	//_ulong dwMaxVertInfluences = 0;
	//_ulong dwNumBoneComboEntries = 0;

	////pDerivedMeshContainer->pSkinInfo->ConvertToBlendedMesh(pDerivedMeshContainer->pOriMesh,
	////	D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
	////	pDerivedMeshContainer->pAdjacency,
	////	NULL, NULL, NULL,
	////	&pDerivedMeshContainer->dwMaxVertInfluences,
	////	&pDerivedMeshContainer->dwNumAttributeGroups,
	////	&pDerivedMeshContainer->pBoneCombiantionBuf,
	////	&pDerivedMeshContainer->MeshData.pMesh);

	//pSkinInfo->ConvertToIndexedBlendedMesh(pMeshData->pMesh,
	//	D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
	//	30,
	//	NULL, NULL, NULL, NULL,
	//	&dwMaxVertInfluences,
	//	&dwNumBoneComboEntries,
	//	&pDerivedMeshContainer->pBoneCombiantionBuf,
	//	&pDerivedMeshContainer->MeshData.pMesh);
	//
	//pDerivedMeshContainer->dwMaxVertInfluences = dwMaxVertInfluences;
	//
	//pDerivedMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pDerivedMeshContainer->dwNumAttributeGroups);
	//pDerivedMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pDerivedMeshContainer->dwNumAttributeGroups];
	//pDerivedMeshContainer->MeshData.pMesh->GetAttributeTable(pDerivedMeshContainer->pAttributeTable, NULL);

	////// 애니메이션 적용 이전에 정점이 변환되지 않은 원본 메쉬 정보를 복제해둔다.
	////if (dwFVF == 0)
	////{
	////	pDerivedMeshContainer->MeshData.pMesh->CloneMesh(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), decl, m_pDevice, &pDerivedMeshContainer->pOriMesh);
	////}
	////else
	////	pDerivedMeshContainer->MeshData.pMesh->CloneMeshFVF(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), pDerivedMeshContainer->MeshData.pMesh->GetFVF(), m_pDevice, &pDerivedMeshContainer->pOriMesh);

	//// 현재 메쉬 컨테이너가 영향을 끼치는 뼈의 개수를 얻어온다.
	//pDerivedMeshContainer->dwNumBones = pDerivedMeshContainer->pSkinInfo->GetNumBones();

	//pDerivedMeshContainer->pFrameOffsetMatrix = new _mat[pDerivedMeshContainer->dwNumBones];
	//ZeroMemory(pDerivedMeshContainer->pFrameOffsetMatrix, sizeof(_mat) * pDerivedMeshContainer->dwNumBones);

	//pDerivedMeshContainer->ppFrameCombinedMatrix = new _mat*[pDerivedMeshContainer->dwNumBones];
	//ZeroMemory(pDerivedMeshContainer->ppFrameCombinedMatrix, sizeof(_mat*) * pDerivedMeshContainer->dwNumBones);

	//pDerivedMeshContainer->pRenderingMatrix = new _mat[pDerivedMeshContainer->dwNumBones];
	//ZeroMemory(pDerivedMeshContainer->pRenderingMatrix, sizeof(_mat) * pDerivedMeshContainer->dwNumBones);

	//for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
	//{
	//	pDerivedMeshContainer->pFrameOffsetMatrix[i] = *pDerivedMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	//}
	//
	//*ppNewMeshContainer = pDerivedMeshContainer;

	//return S_OK;
}

STDMETHODIMP CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	// 텍스쳐들 제거
	for (_int i = 0; i < TEXTURE_END; ++i)
	{
		for (_ulong j = 0; j < pDerivedMeshContainer->NumMaterials; ++j) 
		{
			if (pDerivedMeshContainer->ppTexture[i][j] == nullptr)
				continue;

			Safe_Release(pDerivedMeshContainer->ppTexture[i][j]);
		}

		Safe_Delete_Array(pDerivedMeshContainer->ppTexture[i]);
	}
	///
	Safe_Release(pDerivedMeshContainer->pBoneCombiantionBuf);
	///
	Safe_Release(pDerivedMeshContainer->MeshData.pMesh);
	Safe_Release(pDerivedMeshContainer->pOriMesh);
	Safe_Release(pDerivedMeshContainer->pSkinInfo);

	Safe_Delete_Array(pDerivedMeshContainer->Name);
	Safe_Delete_Array(pDerivedMeshContainer->pAdjacency);
	Safe_Delete_Array(pDerivedMeshContainer->pMaterials);

	Safe_Delete_Array(pDerivedMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->ppFrameCombinedMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->pRenderingMatrix);

	Safe_Delete_Array(pDerivedMeshContainer->pVtxPos);
	Safe_Delete_Array(pDerivedMeshContainer->pIndex);

	Safe_Delete(pDerivedMeshContainer);

	return S_OK;
}

CAllocateHierarchy * CAllocateHierarchy::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & pPath)
{
	return new CAllocateHierarchy(pGraphicDev, pPath);
}

_ulong CAllocateHierarchy::Release(void)
{
	Safe_Release(m_pDevice);

	delete this;

	return 0;
}

void CAllocateHierarchy::Allocate_Name(char ** ppName, const char * pFrameName)		// char를 쓴 이유는 원시 자료형이 아닐 경우 오류가 날 수 있기때문
{
	if (nullptr == pFrameName)
		return;

	_uint		iLength = (_uint)strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);
}
