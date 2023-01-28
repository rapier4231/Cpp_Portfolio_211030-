#include "..\Headers\HierarchyLoader.h"
#include "..\Headers\Prototype_Mgr.h"


USING(Engine)

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 _pDevice, const wstring & pPath)
	: m_pDevice(_pDevice)
	, m_pPath(pPath)
{
	m_pDevice->AddRef();
}

STDMETHODIMP CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED*	pDerivedFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pDerivedFrame, sizeof(D3DXFRAME_DERIVED));

	pDerivedFrame->CombinedTransformMatrix = *D3DXMatrixIdentity(&pDerivedFrame->TransformationMatrix);

	Allocate_Name(&pDerivedFrame->Name, Name);

	*ppNewFrame = pDerivedFrame;

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	_int iChecker = 0;
	D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pDerivedMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	if (nullptr != Name)
		Allocate_Name(&pDerivedMeshContainer->Name, Name);
	else
		iChecker++;

	pDerivedMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH		pMesh = pMeshData->pMesh;

	_ulong	dwNumFaces = pMesh->GetNumFaces();	// �޽��� ���� ������ ������ ��ȯ

	pDerivedMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pDerivedMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	_ulong	dwFVF = pMesh->GetFVF();

	//���� FVF�� �ٸ��� ź��Ʈ ������ ������ �ְ� ��
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	// Mesh Export ���� �� decl�� üũ ���� ���(FVF�� 0�� �鰨)
	if (dwFVF == 0)
		pMesh->CloneMesh(pMesh->GetOptions(), decl, m_pDevice, &pDerivedMeshContainer->MeshData.pMesh);
	// Mesh Export ���� �� decl�� üũ ������ ���(FVF���)
	else
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pDevice, &pDerivedMeshContainer->MeshData.pMesh);

	//////////////////////////////////////////////////////////////////////////
	void*			pVertex = nullptr;

	pDerivedMeshContainer->dwVtxNum = pMesh->GetNumVertices();	// �޽��� ���� ������ ������ ��ȯ


	D3DVERTEXELEMENT9			Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) *MAX_FVF_DECL_SIZE);
	pMesh->GetDeclaration(Decl);
	pDerivedMeshContainer->dwVtxSize = D3DXGetDeclVertexSize(Decl, 0);  //44;//D3DXGetFVFVertexSize(dwFVF);
	_ubyte			byOffset = 0;
	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (_ubyte)Decl[i].Offset;
			break;
		}
	}

	//pVtxPos : ��ġ���� �ѵ� ��Ƴ���
	pDerivedMeshContainer->pVtxPos = new _vec3[pDerivedMeshContainer->dwVtxNum];
	pMesh->LockVertexBuffer(0, &pVertex);
	for (_ulong i = 0; i < pDerivedMeshContainer->dwVtxNum; ++i)
	{
		pDerivedMeshContainer->pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (pDerivedMeshContainer->dwVtxSize * i + byOffset)));
	}
	pMesh->UnlockVertexBuffer();

	//pIndex : �ε����� �ѵ� ������
	LPDIRECT3DINDEXBUFFER9 pIB = nullptr;
	pMesh->GetIndexBuffer(&pIB);	//�ε������� ���� (������ �ʿ�)
	
	D3DINDEXBUFFER_DESC desc;
	pIB->GetDesc(&desc);
	pDerivedMeshContainer->dwStride = desc.Format == D3DFMT_INDEX16 ? 3 * sizeof(WORD) : 3 * sizeof(DWORD);

	pDerivedMeshContainer->dwIndexNum = pMesh->GetNumFaces();

	pDerivedMeshContainer->pIndex = new BYTE[pDerivedMeshContainer->dwStride * pDerivedMeshContainer->dwIndexNum];
	void* pBits = nullptr;
	pMesh->LockIndexBuffer(D3DLOCK_READONLY, &pBits);
	memcpy(pDerivedMeshContainer->pIndex, pBits, pDerivedMeshContainer->dwStride * pDerivedMeshContainer->dwIndexNum);
	pMesh->UnlockIndexBuffer();

	Safe_Release(pIB);

	//////////////////////////////////////////////////////////////////////////

	// ��Ȥ���� ������ �޽��� ���� ������ ���� ��츦 ����� �ڵ�
	pDerivedMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);
	for (_uint i = 0; i < TEXTURE_END; ++i)
	{
		pDerivedMeshContainer->ppTexture[i] = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
		ZeroMemory(pDerivedMeshContainer->ppTexture[i], sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);
	}

	// ���� ������ ����� �ִ� ���
	if (0 != NumMaterials)
	{
		pDerivedMeshContainer->pMaterials = new D3DXMATERIAL[pDerivedMeshContainer->NumMaterials];
		ZeroMemory(pDerivedMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

		//if (sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials == _msize(const_cast<D3DXMATERIAL*>(pMaterials)))
		memcpy(pDerivedMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);
		//else if (sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials > sizeof(*pMaterials))
			/*iChecker++;
		else
			iChecker++;*/
		for (size_t i = 0; i < pDerivedMeshContainer->NumMaterials; i++)
		{
			iChecker = sizeof(pMaterials[i]);
			iChecker++;
		}

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			_tchar		szFullPath[MAX_PATH] = L"";
			_tchar		szFileName[MAX_PATH] = L"";

			MultiByteToWideChar(CP_ACP, 0, pDerivedMeshContainer->pMaterials[i].pTextureFilename, (_uint)strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename), szFileName, MAX_PATH);

			Allocate_Name(&pDerivedMeshContainer->pMaterials[i].pTextureFilename, pDerivedMeshContainer->pMaterials[i].pTextureFilename);

			lstrcpy(szFullPath, m_pPath.c_str());	//wstring���� �������� ���ڿ���
			lstrcat(szFullPath, szFileName);	//��ο� ���ϸ� ��ħ
			wstring wstrFileName = szFileName;
			_uint iLength = (_int)wstrFileName.length();
			wstrFileName = wstrFileName.substr(0, iLength - 5);
			wstring wstrNormal = wstrFileName + L"N.tga";
			wstring wstrSpecular = wstrFileName + L"S.tga";
			wstring wstrAlpha = wstrFileName + L"A.tga";
			wstring wstrMask = wstrFileName + L"M.tga";

			//D3DXIMAGE_INFO tImageInfo;
			//if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice,
			//	szFullPath,
			//	0,
			//	0,
			//	0,
			//	D3DUSAGE_DYNAMIC,
			//	D3DFMT_UNKNOWN,
			//	D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
			//	D3DX_DEFAULT,
			//	D3DX_DEFAULT,
			//	0,
			//	&tImageInfo,
			//	NULL,
			//	&pDerivedMeshContainer->ppTexture[TEXTURE_DIFFUSE][i])))
			if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, szFileName, szFullPath, &pDerivedMeshContainer->ppTexture[TEXTURE_DIFFUSE][i])))
			{
				MSG_BOX("DynamicMesh's Texture Create Failed");
				return E_FAIL;
			}
			//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pDerivedMeshContainer->ppTexture[TEXTURE_DIFFUSE][i])))
			//{
			//	MSG_BOX("DynamicMesh's Texture Create Failed");
			//	return E_FAIL;
			//}

			// �븻 �ؽ�ó �߰�
			lstrcpy(szFullPath, m_pPath.c_str());
			lstrcat(szFullPath, wstrNormal.c_str());

			//if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice,
			//	szFullPath,
			//	0,
			//	0,
			//	0,
			//	D3DUSAGE_DYNAMIC,
			//	D3DFMT_UNKNOWN,
			//	D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
			//	D3DX_DEFAULT,
			//	D3DX_DEFAULT,
			//	0,
			//	&tImageInfo,
			//	NULL, 
			//	&pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i])))
			if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, wstrNormal.c_str(), szFullPath, &pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i])))
			{
				pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i] = nullptr;
				//MSG_BOX("DynamicMesh's Texture Create Failed");
				//return E_FAIL;
			}
			//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i])))
			//	pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i] = nullptr;

			//// ����ŧ�� �ؽ�ó �߰�
			//lstrcpy(szFullPath, m_pPath.c_str());
			//lstrcat(szFullPath, wstrNormal.c_str());

			//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pDerivedMeshContainer->ppTexture[TEXTURE_SPECULAR][i])))
			//	pDerivedMeshContainer->ppTexture[TEXTURE_SPECULAR][i] = nullptr;

			// ����ũ �ؽ�ó �߰�
			lstrcpy(szFullPath, m_pPath.c_str());
			lstrcat(szFullPath, wstrMask.c_str());

			//if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice,
			//	szFullPath,
			//	0,
			//	0,
			//	0,
			//	D3DUSAGE_DYNAMIC,
			//	D3DFMT_UNKNOWN,
			//	D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
			//	D3DX_DEFAULT,
			//	D3DX_DEFAULT,
			//	0,
			//	&tImageInfo,
			//	NULL, 
			//	&pDerivedMeshContainer->ppTexture[TEXTURE_MASK][i])))
			if (FAILED(CPrototype_Mgr::GetInstance()->Create_Texture(m_pDevice, wstrMask.c_str(), szFullPath, &pDerivedMeshContainer->ppTexture[TEXTURE_MASK][i])))
			{
				pDerivedMeshContainer->ppTexture[TEXTURE_MASK][i] = nullptr;
				//MSG_BOX("DynamicMesh's Texture Create Failed");
				//return E_FAIL;
			}
			//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pDerivedMeshContainer->ppTexture[TEXTURE_MASK][i])))
			//	pDerivedMeshContainer->ppTexture[TEXTURE_MASK][i] = nullptr;

		}
		
	}
	// ���� ������ ��� ������ ������ �� �� ������ ���
	else
	{//�����Ÿ����
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pDerivedMeshContainer->ppTexture[0] = NULL;
	}

	// ��Ű�� �ִϸ��̼��� �����ϴ� �� ��ü�� ���� ���� ���̳��� �޽��� ������� ���ϰ� �ִٰ� ���ص� ������ �ƴϴ�.
	if (nullptr == pSkinInfo)
		return S_OK;

	pDerivedMeshContainer->pSkinInfo = pSkinInfo;
	pDerivedMeshContainer->pSkinInfo->AddRef();

	// �ִϸ��̼� ���� ������ ������ ��ȯ���� ���� ���� �޽� ������ �����صд�.
	if (dwFVF == 0)
		pDerivedMeshContainer->MeshData.pMesh->CloneMesh(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), decl, m_pDevice, &pDerivedMeshContainer->pOriMesh);
	else//�����Ż��
		pDerivedMeshContainer->MeshData.pMesh->CloneMeshFVF(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), pDerivedMeshContainer->MeshData.pMesh->GetFVF(), m_pDevice, &pDerivedMeshContainer->pOriMesh);

	// ���� �޽� �����̳ʰ� ������ ��ġ�� ���� ������ ���´�.
	pDerivedMeshContainer->dwNumBones = pDerivedMeshContainer->pSkinInfo->GetNumBones();

	pDerivedMeshContainer->pFrameOffsetMatrix = new _mat[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pFrameOffsetMatrix, sizeof(_mat) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->ppFrameCombinedMatrix = new _mat*[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->ppFrameCombinedMatrix, sizeof(_mat*) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->pRenderingMatrix = new _mat[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pRenderingMatrix, sizeof(_mat) * pDerivedMeshContainer->dwNumBones);

	for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
	{
		pDerivedMeshContainer->pFrameOffsetMatrix[i] = *pDerivedMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	}
	
	*ppNewMeshContainer = pDerivedMeshContainer;

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
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

STDMETHODIMP CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	//for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
	//	Safe_Release(pDerivedMeshContainer->ppTexture[i]);

	//Safe_Delete_Array(pDerivedMeshContainer->ppTexture);

	// �ؽ��ĵ� ����
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

	Safe_Release(pDerivedMeshContainer->MeshData.pMesh);
	Safe_Release(pDerivedMeshContainer->pOriMesh);
	Safe_Release(pDerivedMeshContainer->pSkinInfo);

	Safe_Delete_Array(pDerivedMeshContainer->Name);
	Safe_Delete_Array(pDerivedMeshContainer->pAdjacency);
	for (size_t i = 0; i < pDerivedMeshContainer->NumMaterials; i++)
	{
		Safe_Delete_Array(pDerivedMeshContainer->pMaterials[i].pTextureFilename);
	}
	Safe_Delete_Array(pDerivedMeshContainer->pMaterials);

	Safe_Delete_Array(pDerivedMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->ppFrameCombinedMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->pRenderingMatrix);

	Safe_Delete_Array(pDerivedMeshContainer->pVtxPos);
	Safe_Delete_Array(pDerivedMeshContainer->pIndex);

	Safe_Delete(pDerivedMeshContainer);

	return S_OK;
}

CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & pPath)
{
	return new CHierarchyLoader(pGraphicDev, pPath);
}

_ulong CHierarchyLoader::Release(void)
{
	Safe_Release(m_pDevice);

	delete this;

	return 0;
}

void CHierarchyLoader::Allocate_Name(char ** ppName, const char * pFrameName)		// char�� �� ������ ���� �ڷ����� �ƴ� ��� ������ �� �� �ֱ⶧��
{
	if (nullptr == pFrameName)
		return;

	_uint		iLength = (_uint)strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);
}
