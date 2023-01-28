#include "..\Headers\SkinnedMesh.h"

#include "AniCtrl.h"
#include "AllocateHierarchy.h"

const _ulong	VTX_LINE::FVF_LINE = D3DFVF_XYZ | D3DFVF_DIFFUSE;

CSkinnedMesh::CSkinnedMesh(LPDIRECT3DDEVICE9 _pDevice, const wstring& _pFilePath, const wstring& _pFileName)
	: CComponent(_pDevice)
	, m_strFilePath(_pFilePath)
	, m_strFileName(_pFileName)
{
}

CSkinnedMesh::CSkinnedMesh(const CSkinnedMesh & rhs)
	: CComponent(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
	, m_strFilePath(rhs.m_strFilePath)
	, m_strFileName(rhs.m_strFileName)
	, m_pSphereMesh(rhs.m_pSphereMesh)
{
	m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);
}

HRESULT CSkinnedMesh::Ready_Component_Prototype()
{
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, m_strFilePath.c_str());
	lstrcat(szFullPath, m_strFileName.c_str());

	m_pLoader = CAllocateHierarchy::Create(m_pDevice, m_strFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED/*D3DXMESH_DYNAMIC*/,
		m_pDevice,
		m_pLoader,	// HierarchyLoader
		NULL,
		&m_pRootFrame,
		&pAniCtrl)))	// AniCtrl
		return E_FAIL;

	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);

	Safe_Release(pAniCtrl);

	_mat		matTemp;

	//Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp));
	CalculateWorldMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp));
	SetupBoneMatrixPointers((D3DXFRAME_DERIVED*)m_pRootFrame);
	//SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);



	///
	D3DXCreateSphere(m_pDevice, 0.02f, 10, 10, &m_pSphereMesh, NULL);

	return S_OK;
}

HRESULT CSkinnedMesh::Ready_Component(void * _pArg/* = nullptr*/)
{
	return S_OK;
}

void CSkinnedMesh::Render_Meshes(LPD3DXEFFECT & _pEffect)
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = iter;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			pDerivedMeshContainer->pRenderingMatrix[i] = pDerivedMeshContainer->pFrameOffsetMatrix[i] *
				*pDerivedMeshContainer->ppFrameCombinedMatrix[i];
		}
		void*		pSrcVtx = nullptr;
		void*		pDestVtx = nullptr;

		pDerivedMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pDerivedMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		// 소프트 웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈와 정점 정보들을 모두 변경을 해주기도 함)
		pDerivedMeshContainer->pSkinInfo->UpdateSkinnedMesh(pDerivedMeshContainer->pRenderingMatrix, NULL, pSrcVtx, pDestVtx);

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			// 쉐이더 텍스처 세팅
			_pEffect->SetTexture("g_DiffuseTexture", pDerivedMeshContainer->ppTexture[TEXTURE_DIFFUSE][i]);

			// 텍스처 정보 갱신
			_pEffect->CommitChanges();
			pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);

			// 노멀 텍스쳐가 없으면 정점 정보로 노멀 생성
			if (pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i] == nullptr)
			{
				_pEffect->SetBool("g_bNoneNormalTexture", true);
				_pEffect->CommitChanges();

				pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);
			}

			// 노말 텍스쳐가 있다면 해당 텍스쳐로 노멀 생성
			else
			{
				_pEffect->SetBool("g_bNoneNormalTexture", false);
				_pEffect->SetTexture("g_NormalTexture", pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i]);
				_pEffect->CommitChanges();

				pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);
			}
		}

		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
		pDerivedMeshContainer->pOriMesh->UnlockVertexBuffer();
	}
}

void CSkinnedMesh::Render_Meshes(LPD3DXEFFECT & _pEffect, _uint _iSaddleIndex)
{
	_uint iSkipSubset = 0;
	for (auto& iter : m_MeshContainerList)
	{
		if (iSkipSubset == _iSaddleIndex)
			continue;
		++iSkipSubset;

		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = iter;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			pDerivedMeshContainer->pRenderingMatrix[i] = pDerivedMeshContainer->pFrameOffsetMatrix[i] *
				*pDerivedMeshContainer->ppFrameCombinedMatrix[i];
		}
		void*		pSrcVtx = nullptr;
		void*		pDestVtx = nullptr;

		pDerivedMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pDerivedMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		// 소프트 웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈와 정점 정보들을 모두 변경을 해주기도 함)
		pDerivedMeshContainer->pSkinInfo->UpdateSkinnedMesh(pDerivedMeshContainer->pRenderingMatrix, NULL, pSrcVtx, pDestVtx);

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			// 쉐이더 텍스처 세팅
			_pEffect->SetTexture("g_DiffuseTexture", pDerivedMeshContainer->ppTexture[TEXTURE_DIFFUSE][i]);

			// 텍스처 정보 갱신
			_pEffect->CommitChanges();
			pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);

			// 노멀 텍스쳐가 없으면 정점 정보로 노멀 생성
			if (pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i] == nullptr)
			{
				_pEffect->SetBool("g_bNoneNormalTexture", true);
				_pEffect->CommitChanges();

				pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);
			}

			// 노말 텍스쳐가 있다면 해당 텍스쳐로 노멀 생성
			else
			{
				_pEffect->SetBool("g_bNoneNormalTexture", false);
				_pEffect->SetTexture("g_NormalTexture", pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i]);
				_pEffect->CommitChanges();

				pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);
			}
		}

		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
		pDerivedMeshContainer->pOriMesh->UnlockVertexBuffer();
	}
}

void CSkinnedMesh::Render_Meshes(LPD3DXEFFECT & _pEffect, D3DXFRAME_DERIVED * _pFrame)
{
	// 프레임이 nullptr일 경우는 처음 함수 호출됐을 때
	if (nullptr == _pFrame)
		_pFrame = (D3DXFRAME_DERIVED*)m_pRootFrame;

	// 랜더할 메쉬가 있다면 그린다.
	if (_pFrame->pMeshContainer != NULL)
	{
		D3DXMESHCONTAINER_DERIVED* pBoneMesh = (D3DXMESHCONTAINER_DERIVED*)_pFrame->pMeshContainer;

		if (NULL != pBoneMesh->pSkinInfo)
		{
			// 본 컴비네이션 테이블을 가져온다.
			LPD3DXBONECOMBINATION pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pBoneMesh->pBoneCombiantionBuf->GetBufferPointer());

			// 모든 속성 그룹에 대해
			for (_ulong i = 0; i < pBoneMesh->dwNumAttributeGroups; ++i)
			{
				// 본 컴비네이션 테이블로부터 본들의 아이디를 얻고, 행렬 팔레트를 구성한다.
				for (_int iPaletteEntry = 0; iPaletteEntry < 30; ++iPaletteEntry)
				{
					_int iMatrixIndex = pBoneComb[i].BoneId[iPaletteEntry];
					if (UINT_MAX != iMatrixIndex)
					{
						D3DXMatrixMultiply(&pBoneMesh->pRenderingMatrix[iPaletteEntry],
							&pBoneMesh->pFrameOffsetMatrix[iMatrixIndex],
							pBoneMesh->ppFrameCombinedMatrix[iMatrixIndex]);
					}
				}

				// 쉐이더에게 행렬 팔레트를 업로드한다.
				_pEffect->SetMatrixArray("g_matPalette", pBoneMesh->pRenderingMatrix, 80);
				_pEffect->SetInt("g_iNumBoneInfluences", pBoneMesh->dwMaxVertInfluences);

				// 쉐이더에게 텍스처, 메테리얼 세팅
				_int mtrlIndex = pBoneComb[i].AttribId;

				_pEffect->Begin(NULL, NULL);
				_pEffect->BeginPass(0);

				pBoneMesh->MeshData.pMesh->DrawSubset(mtrlIndex);

				_pEffect->EndPass();
				_pEffect->End();
			}
		}
	}

	// 현제, 자식 뼈도 재귀하면서 랜더링한다.
	if (NULL != _pFrame->pFrameSibling)
		Render_Meshes(_pEffect, (D3DXFRAME_DERIVED*)_pFrame->pFrameSibling);
	if (NULL != _pFrame->pFrameFirstChild)
		Render_Meshes(_pEffect, (D3DXFRAME_DERIVED*)_pFrame->pFrameFirstChild);
}

void CSkinnedMesh::Render_Skeleton(D3DXFRAME_DERIVED * _pFrame, D3DXFRAME_DERIVED * _pParentFrame, _mat world)
{
	if (nullptr == _pFrame)
		_pFrame = (D3DXFRAME_DERIVED*)m_pRootFrame;

	_mat r, s, t;
	D3DXMatrixRotationYawPitchRoll(&r, -D3DX_PI * 0.5f, 0.f, 0.f);

	if (nullptr != _pParentFrame && nullptr != _pFrame->Name &&  nullptr != _pParentFrame->Name)
	{
		/// 구를 그린다
		// 본 행렬계산
		s = r * _pFrame->CombinedTransformMatrix * world;
		D3DXMatrixTranslation(&t, s(3,0), s(3,1), s(3,2));
		m_pDevice->SetTransform(D3DTS_WORLD, &t);
		m_pSphereMesh->DrawSubset(0);

		/// 라인을 그린다.
		_mat w1 = _pFrame->CombinedTransformMatrix;
		_mat w2 = _pParentFrame->CombinedTransformMatrix;
		
		_vec3 vThisBone = _vec3(w1(3,0), w1(3,1), w1(3,2));
		_vec3 vParentBone = _vec3(w2(3, 0), w2(3, 1), w2(3, 2));
		
		m_pDevice->SetTransform(D3DTS_WORLD, &world);
		VTX_LINE vert[] = { VTX_LINE(vParentBone, 0xffff0000), VTX_LINE(vThisBone, 0xff00ff00)};
		m_pDevice->SetFVF(VTX_LINE::FVF_LINE);
		m_pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &vert[0], sizeof(VTX_LINE));
	}

	if (_pFrame->pFrameSibling)
		Render_Skeleton((D3DXFRAME_DERIVED*)_pFrame->pFrameSibling, _pParentFrame, world);

	if (_pFrame->pFrameFirstChild)
		Render_Skeleton((D3DXFRAME_DERIVED*)_pFrame->pFrameFirstChild, _pFrame, world);
}

void CSkinnedMesh::Render_Skeleton(D3DXFRAME_DERIVED * _pFrame)
{
	if (_pFrame == NULL)
		_pFrame = (D3DXFRAME_DERIVED*)m_pRootFrame;

	if (_pFrame->pMeshContainer != NULL)
	{
		SKINNEDMESHCONTAINER_DERIVED* boneMesh = (SKINNEDMESHCONTAINER_DERIVED*)_pFrame->pMeshContainer;

		if (boneMesh->pSkinInfo != NULL)
		{
			_int numBones = boneMesh->pSkinInfo->GetNumBones();

			for (_int i = 0; i < numBones; ++i)
			{
				D3DXMatrixMultiply(&boneMesh->pCurrentBoneMatrices[i],
					&boneMesh->pBoneOffsetMatrices[i],
					boneMesh->ppBoneMatrixPtrs[i]);
			}

			BYTE *src = NULL, *dest = NULL;
			boneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (VOID**)&src);
			boneMesh->MeshData.pMesh->LockVertexBuffer(0, (VOID**)&dest);

			boneMesh->pSkinInfo->UpdateSkinnedMesh(boneMesh->pCurrentBoneMatrices,
				NULL,
				src,
				dest);

			boneMesh->MeshData.pMesh->UnlockVertexBuffer();
			boneMesh->pOrigMesh->UnlockVertexBuffer();

			for (_ulong i = 0; i < boneMesh->dwNumAttributeGroups; ++i)
			{
				_ulong mtrl = boneMesh->pAttributeTable[i].AttribId;
				//m_pDevice->SetMaterial
				boneMesh->MeshData.pMesh->DrawSubset(mtrl);
			}
		}
	}

	if (_pFrame->pFrameSibling != NULL)
		Render_Skeleton((D3DXFRAME_DERIVED*)_pFrame->pFrameSibling);
	if (_pFrame->pFrameFirstChild != NULL)
		Render_Skeleton((D3DXFRAME_DERIVED*)_pFrame->pFrameFirstChild);
}

void CSkinnedMesh::Set_Animation(const _uint & iIndex)
{
	m_pAniCtrl->Set_Animation(iIndex);
}

void CSkinnedMesh::Play_Animation(const _double& _dTimeDelta)
{
	m_pAniCtrl->Play_Animation(_dTimeDelta);

	_mat matTemp;

	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp));
}

void CSkinnedMesh::Reset_Animtion()
{
	m_pAniCtrl->Reset_Animation();
}

void CSkinnedMesh::Get_BoneNames(D3DXFRAME_DERIVED* pFrame, list<_vec3>* listBonsPos)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			const char* pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);	// 뼈의 이름을 반환하는 함수

			D3DXFRAME_DERIVED*	 pDerivedFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName); // 뼈의 이름을 근거로 뼈를 찾아주는 함수

			//뼈마다 지닌 주소
			_mat BoneMatrix =  pDerivedFrame->CombinedTransformMatrix;

			_vec3 vBonPos;
			memcpy(vBonPos, &BoneMatrix.m[3][0], sizeof(_vec3));

			listBonsPos->emplace_back(vBonPos);

			//_tchar		szBuff[MAX_PATH] = L"";
			//_tchar		szBone[MAX_PATH] = L"";
			//MultiByteToWideChar(CP_ACP, 0, pBoneName, strlen(pBoneName), szBuff, MAX_PATH);
			//
			//lstrcpy(szBone, szBuff);

			//wstring strBone = szBone;
			//Bones->emplace_back(strBone);
		}
	}

	if (nullptr != pFrame->pFrameSibling)
		Get_BoneNames((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, listBonsPos);

	if (nullptr != pFrame->pFrameFirstChild)
		Get_BoneNames((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, listBonsPos);

}

_double CSkinnedMesh::Get_CurrentTime()
{
	return m_pAniCtrl->Get_CurrentTime();
}

const D3DXFRAME_DERIVED * CSkinnedMesh::Get_FrameByName(const char * pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

_bool CSkinnedMesh::Is_AnimationsetEnd(_double _dRemoveTime/* = 0.1f*/)
{
	return m_pAniCtrl->Is_AnimationsetEnd(_dRemoveTime);
}

LPD3DXANIMATIONCONTROLLER CSkinnedMesh::Get_AnimationCtrl()
{
	return m_pAniCtrl->Get_AnimationCtrl();
}

CSkinnedMesh * CSkinnedMesh::Create(LPDIRECT3DDEVICE9 _pDevice, const wstring & _strFilePath, const wstring & _strFileName)
{
	CSkinnedMesh*	pInstance = new CSkinnedMesh(_pDevice, _strFilePath, _strFileName);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CSkinnedMesh::Clone(void * pArg/* = nullptr*/)
{
	CSkinnedMesh*	pClone = new CSkinnedMesh(*this);

	if (FAILED(pClone->Ready_Component()))
		Safe_Release(pClone);

	return pClone;
}

void CSkinnedMesh::Free(void)
{

	Safe_Release(m_pAniCtrl);

	if (false == m_bIsClone)
	{
		///
		Safe_Release(m_pSphereMesh);
		///
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}

	m_MeshContainerList.clear();

	CComponent::Free();
}

void CSkinnedMesh::Update_FrameMatrices(D3DXFRAME_DERIVED * pFrame, const _mat * pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix);
}

void CSkinnedMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED * pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;
	
		// 이 부분은 충돌 툴을 만들 시에 매우 요긴하게 사용될 코드들
		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			const char* pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);	// 뼈의 이름을 반환하는 함수
			D3DXFRAME_DERIVED*	 pDerivedFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName); // 뼈의 이름을 근거로 뼈를 찾아주는 함수
	
			m_mapBoneName.emplace(i, pBoneName);
	
			pDerivedMeshContainer->ppFrameCombinedMatrix[i] = &pDerivedFrame->CombinedTransformMatrix;
		}
		m_MeshContainerList.push_back(pDerivedMeshContainer);
	}
	
	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);
	
	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSkinnedMesh::CalculateWorldMatrices(D3DXFRAME_DERIVED * _pFrame, _mat * _pParentMat)
{
	if (nullptr == _pFrame)
		return;

	// 부모 변환 행렬과 복합된 월드 변환 행렬을 계산한다.
	D3DXMatrixMultiply(&_pFrame->CombinedTransformMatrix,
						&_pFrame->TransformationMatrix,
						_pParentMat);
	//_pFrame->CombinedTransformMatrix = _pFrame->TransformationMatrix * (*_pParentMat);

	// 형제 뼈 계산
	if (_pFrame->pFrameSibling)
		CalculateWorldMatrices((D3DXFRAME_DERIVED*)_pFrame->pFrameSibling, _pParentMat);

	// 자식 뼈 계산
	if (_pFrame->pFrameFirstChild)
		CalculateWorldMatrices((D3DXFRAME_DERIVED*)_pFrame->pFrameFirstChild, &_pFrame->CombinedTransformMatrix);
}

void CSkinnedMesh::SetupBoneMatrixPointers(D3DXFRAME_DERIVED * _pFrame)
{
	if (_pFrame->pMeshContainer != NULL)
	{
		SKINNEDMESHCONTAINER_DERIVED* boneMesh = (SKINNEDMESHCONTAINER_DERIVED*)_pFrame->pMeshContainer;
		
		if (boneMesh->pSkinInfo != NULL)
		{
			_int NumBones = boneMesh->pSkinInfo->GetNumBones();
			boneMesh->ppBoneMatrixPtrs = new _mat*[NumBones];

			for (_int i = 0; i < NumBones; ++i)
			{
				D3DXFRAME_DERIVED* b = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, boneMesh->pSkinInfo->GetBoneName(i));

				if (b != NULL)
					boneMesh->ppBoneMatrixPtrs[i] = &b->CombinedTransformMatrix;
				else
					boneMesh->ppBoneMatrixPtrs[i] = NULL;
			}
		}
	}

	if (_pFrame->pFrameSibling != NULL)
		SetupBoneMatrixPointers((D3DXFRAME_DERIVED*)_pFrame->pFrameSibling);
	if (_pFrame->pFrameFirstChild != NULL)
		SetupBoneMatrixPointers((D3DXFRAME_DERIVED*)_pFrame->pFrameFirstChild);
}
