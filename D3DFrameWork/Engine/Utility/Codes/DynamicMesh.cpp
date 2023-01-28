#include "..\Headers\DynamicMesh.h"

#include "AniCtrl.h"
#include "HierarchyLoader.h"


CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 _pDevice, const wstring& _pFilePath, const wstring& _pFileName)
	: CComponent(_pDevice)
	, m_strFilePath(_pFilePath)
	, m_strFileName(_pFileName)
{
}

CDynamicMesh::CDynamicMesh(const CDynamicMesh & rhs)
	: CComponent(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
	, m_strFilePath(rhs.m_strFilePath)
	, m_strFileName(rhs.m_strFileName)
	, m_bIsUpper(rhs.m_bIsUpper)
{
	m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);
	m_pUpperAniCtrl = CAniCtrl::Create(*rhs.m_pUpperAniCtrl);
}

HRESULT CDynamicMesh::Ready_Component_Prototype()
{
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, m_strFilePath.c_str());
	lstrcat(szFullPath, m_strFileName.c_str());

	m_pLoader = CHierarchyLoader::Create(m_pDevice, m_strFilePath);
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

	m_pUpperAniCtrl = CAniCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pUpperAniCtrl, E_FAIL);

	//m_pWaistAniCtrl = CAniCtrl::Create(pAniCtrl);
	//NULL_CHECK_RETURN(m_pWaistAniCtrl, E_FAIL);
	//
	//m_pNeckAniCtrl = CAniCtrl::Create(pAniCtrl);
	//NULL_CHECK_RETURN(m_pNeckAniCtrl, E_FAIL);

	Safe_Release(pAniCtrl);

	_mat		matTemp;

	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp));
	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

HRESULT CDynamicMesh::Ready_Component(void * _pArg/* = nullptr*/)
{
	return S_OK;
}

void CDynamicMesh::Render_Meshes(void)
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
			m_pDevice->SetTexture(0, *pDerivedMeshContainer->ppTexture[i]);
			pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
		pDerivedMeshContainer->pOriMesh->UnlockVertexBuffer();
	}
}

void CDynamicMesh::Render_Meshes(LPD3DXEFFECT & _pEffect, _bool bDesolve, _bool* _bSubsetRender/* = nullptr*/, SHADER_PASS_MESH ePass)
{
	_uint iSkipSubset = 0;
	for (auto& iter : m_MeshContainerList)
	{
		if (_bSubsetRender && !_bSubsetRender[iSkipSubset++])
			continue;
		//++iSkipSubset;

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
			_int	iPass = 0;
			// 아무것도 안넣어주면 기존거 그대로
			if (ePass == SHADER_PASS_NORMAL)
				iPass = bDesolve ? SHADER_PASS_DISSOLVE : SHADER_PASS_NORMAL; // 쉐이더 패스 지정
			else if(ePass == SHADER_PASS_NORMAL_CUL)
				iPass = bDesolve ? SHADER_PASS_DISSOLVE : SHADER_PASS_NORMAL_CUL; // 쉐이더 패스 지정

			if (Find_Alpha(iter->pMaterials[i].pTextureFilename))
				iPass = bDesolve ? SHADER_PASS_DISSOLVE_A : SHADER_PASS_ALPHATEST;

			// 쉐이더 텍스처 세팅
			_pEffect->SetTexture("g_DiffuseTexture", pDerivedMeshContainer->ppTexture[TEXTURE_DIFFUSE][i]);

			// 노멀 텍스쳐가 없으면 정점 정보로 노멀 생성
			if (pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i] == nullptr)
				_pEffect->SetBool("g_bNoneNormalTexture", true);

			// 노말 텍스쳐가 있다면 해당 텍스쳐로 노멀 생성
			else
			{
				_pEffect->SetBool("g_bNoneNormalTexture", false);
				_pEffect->SetTexture("g_NormalTexture", pDerivedMeshContainer->ppTexture[TEXTURE_NORMAL][i]);
			}

			// 마스크 텍스쳐
			if (pDerivedMeshContainer->ppTexture[TEXTURE_MASK][i] == nullptr)
			{
				_pEffect->SetBool("g_bMaskTexture", false);
			}
			else
			{
				_pEffect->SetBool("g_bMaskTexture", true);
				_pEffect->SetTexture("g_MaskTexture", pDerivedMeshContainer->ppTexture[TEXTURE_MASK][i]);
			}

			if (iPass == 0)
				_pEffect->BeginPass(iPass);
			else
				_pEffect->BeginPass(iPass);

			// 텍스처 정보 갱신
			_pEffect->CommitChanges();
			pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);

			_pEffect->EndPass();
		}

		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
		pDerivedMeshContainer->pOriMesh->UnlockVertexBuffer();
	}
}

void CDynamicMesh::Render_MeshesShadowMap(LPD3DXEFFECT & _pEffect, _bool* _bSubsetRender/* = nullptr*/)
{
	_uint iSkipSubset = 0;
	for (auto& iter : m_MeshContainerList)
	{
		if (_bSubsetRender && !_bSubsetRender[iSkipSubset++])
			continue;
		//++iSkipSubset;

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
		}

		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
		pDerivedMeshContainer->pOriMesh->UnlockVertexBuffer();
	}
}

void CDynamicMesh::Render_MeshesShadow(LPD3DXEFFECT & _pEffect, _bool* _bSubsetRender/* = nullptr*/)
{
	_uint iSkipSubset = 0;
	for (auto& iter : m_MeshContainerList)
	{
		if (_bSubsetRender && !_bSubsetRender[iSkipSubset++])
			continue;
		//++iSkipSubset;

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
			_vec4	vCol;

			vCol.x = pDerivedMeshContainer->pMaterials->MatD3D.Diffuse.r;
			vCol.y = pDerivedMeshContainer->pMaterials->MatD3D.Diffuse.g;
			vCol.z = pDerivedMeshContainer->pMaterials->MatD3D.Diffuse.b;
			vCol.w = pDerivedMeshContainer->pMaterials->MatD3D.Diffuse.a;

			_pEffect->SetVector("g_vCol", &vCol);

			// 텍스처 정보 갱신
			_pEffect->CommitChanges();
			pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
		pDerivedMeshContainer->pOriMesh->UnlockVertexBuffer();
	}
}

void CDynamicMesh::Set_Animation(const _uint & iIndex)
{
	m_pAniCtrl->Set_Animation(iIndex);
}

void CDynamicMesh::Set_UpperAnimation(const _uint & iIndex, const _double& _dTime)
{
	if (_dTime == 1.0)
		m_pUpperAniCtrl->Set_Animation(iIndex, m_pAniCtrl->Get_AccTime());
	else
		m_pUpperAniCtrl->Set_Animation(iIndex, 0.0);
}

void CDynamicMesh::Play_Animation(const _double& _dTimeDelta, _uint _iUpdateType/* = 0*/, const char* _pBoneName/* = nullptr*/, _float _fAngleY, _float _fAngleX, 
								  _bool bBaby, const char* pBoneHead, const char* pBoneTail)
{
	m_pAniCtrl->Play_Animation(_dTimeDelta);
	
	_mat matTemp;

	switch (_iUpdateType)
	{
	case 0:
		// 하나의 애니메이션 전체 동작
		Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp));
		break;
	case 1:
		// 두개의 애니메이션 블랜드
		Update_FrameMatrices_Blend((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp), _pBoneName, _dTimeDelta, _dTimeDelta, bBaby, pBoneHead, pBoneTail);
		m_bIsUpper = false;
		break;
	case 2:
		Update_FrameMatrices_Mix((D3DXFRAME_DERIVED*)m_pRootFrame,  D3DXMatrixIdentity(&matTemp), _pBoneName, _fAngleY, _fAngleX, _dTimeDelta);
		m_bIsUpper = false;
		m_iWaistIndex = 0;
		m_iNeckIndex = 0;
		break;
	default:
		break;
	}
}

void CDynamicMesh::Play_BlendAnimation(const _double & _dTimeDelta, const _double & _dUpperTimeDelta, const char * _pBoneName)
{
	m_pAniCtrl->Play_Animation(_dTimeDelta);

	_mat matTemp;
	// 두개의 애니메이션 블랜드
	Update_FrameMatrices_Blend((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp), _pBoneName, _dTimeDelta, _dUpperTimeDelta, false, nullptr, nullptr);
	m_bIsUpper = false;
}

void CDynamicMesh::Play_Animation_End(const _double & dTimeDelta, const char* _pBoneName, _bool bBaby, const char* pBoneHead, const char* pBoneTail)
{
	m_pAniCtrl->Play_Animation_End(dTimeDelta);

	_mat		matTemp;

	Update_FrameMatrices_Blend((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp), _pBoneName, dTimeDelta, dTimeDelta, bBaby, pBoneHead, pBoneTail);
}

void CDynamicMesh::Play_CustomAnimation(_int iIndex, _mat* pCombineMatix)
{
	for (auto& data : m_MeshContainerList)
	{
		const char* pBoneName = data->pSkinInfo->GetBoneName(iIndex);
		D3DXFRAME_DERIVED*	 pDerivedFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);

		pDerivedFrame->CombinedTransformMatrix = *pCombineMatix;
	}
}

void CDynamicMesh::Reset_Animtion()
{
	m_pAniCtrl->Reset_Animation();
}

void CDynamicMesh::Reset_UpperAnimtion()
{
	m_pUpperAniCtrl->Reset_Animation();
}

void CDynamicMesh::Get_BoneNames(D3DXFRAME_DERIVED* pFrame, list<_vec3>* listBonsPos)
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

_double CDynamicMesh::Get_CurrentTime()
{
	return m_pAniCtrl->Get_CurrentTime();
}

_double CDynamicMesh::Get_AccTime()
{
	return m_pAniCtrl->Get_AccTime();
}

_double CDynamicMesh::Get_UpperCurrentTime()
{
	return m_pUpperAniCtrl->Get_CurrentTime();
}

_double CDynamicMesh::Get_UpperAccTime()
{
	return m_pUpperAniCtrl->Get_AccTime();
}

const D3DXFRAME_DERIVED * CDynamicMesh::Get_FrameByName(const char * pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

_bool CDynamicMesh::Is_AnimationsetEnd(_double _dRemoveTime/* = 0.1f*/)
{
	return m_pAniCtrl->Is_AnimationsetEnd(_dRemoveTime);
}

_bool CDynamicMesh::Is_UpperAnimationsetEnd(_double _dRemoveTime/* = 0.1f*/)
{
	return m_pUpperAniCtrl->Is_AnimationsetEnd(_dRemoveTime);
}

LPD3DXANIMATIONCONTROLLER CDynamicMesh::Get_AnimationCtrl()
{
	return m_pAniCtrl->Get_AnimationCtrl();
}

CDynamicMesh * CDynamicMesh::Create(LPDIRECT3DDEVICE9 _pDevice, const wstring & _strFilePath, const wstring & _strFileName)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(_pDevice, _strFilePath, _strFileName);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CDynamicMesh::Clone(void * pArg/* = nullptr*/)
{
	CDynamicMesh*	pClone = new CDynamicMesh(*this);

	if (FAILED(pClone->Ready_Component()))
		Safe_Release(pClone);

	return pClone;
}

void CDynamicMesh::Free(void)
{
	Safe_Release(m_pAniCtrl);
	Safe_Release(m_pUpperAniCtrl);
	//Safe_Release(m_pWaistAniCtrl);
	//Safe_Release(m_pNeckAniCtrl);

	m_vecWaistMat.clear();
	m_vecWaistMat.shrink_to_fit();
	m_vecNeckMat.clear();
	m_vecNeckMat.shrink_to_fit();

	if (false == m_bIsClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}

	m_MeshContainerList.clear();

	CComponent::Free();
}

void CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED * pFrame, const _mat * pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix);
}

void CDynamicMesh::Update_FrameMatrices_Blend(D3DXFRAME_DERIVED * pFrame, const _mat * pParentMatrix, const char* _pBoneName, const _double& _dTimeDelta,const _double& _dUpperTimeDelta,
											  _bool bBaby, const char* pBoneHead, const char* pBoneTail)
{
	if (nullptr == pFrame)
		return;

	char* pName = pFrame->Name;

	if (!m_bIsUpper)
	{
		if (nullptr != pName)
		{
			if (!strcmp(_pBoneName, pName))
			{
				m_bIsUpper = true;
				m_pUpperAniCtrl->Play_Animation(_dUpperTimeDelta);
			}
		}
	}
	
	_mat matBoneMatrix = pFrame->TransformationMatrix;

	if (bBaby && pName && pBoneHead && pBoneTail)
	{
		_mat matScale;

		if (!strcmp(pBoneHead, pName))
		{
			D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);
			matBoneMatrix = matScale * matBoneMatrix;
		}

		if (!strcmp(pBoneTail, pName))
		{
			D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);
			matBoneMatrix = matScale * matBoneMatrix;
		}
	}

	pFrame->CombinedTransformMatrix = matBoneMatrix * *pParentMatrix;

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices_Blend((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix, _pBoneName, _dTimeDelta, _dUpperTimeDelta, bBaby, pBoneHead, pBoneTail);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices_Blend((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix, _pBoneName, _dTimeDelta, _dUpperTimeDelta, bBaby, pBoneHead, pBoneTail);

}

void CDynamicMesh::Update_FrameMatrices_Mix(D3DXFRAME_DERIVED * pFrame, const _mat * pParentMatrix, const char* _pBoneName, _float _fAngleY, _float _fAngleZ, const _double& _dTimeDelta)
{
	if (nullptr == pFrame)
		return;
	
	_bool	bIsMix = false;
	_mat matRotate;
	D3DXMatrixIdentity(&matRotate);

	char* pName = pFrame->Name;
	if (nullptr != pName)
	{
		if (m_iWaistIndex < m_vecWaistMat.size())
		{
			if (!strcmp(pName, m_vecWaistMat[m_iWaistIndex]))
			{
				D3DXMatrixRotationY(&matRotate, _fAngleY);
				++m_iWaistIndex;
			}
		}

		if (m_iNeckIndex < m_vecNeckMat.size())
		{
			if (!strcmp(pName, m_vecNeckMat[m_iNeckIndex]))
			{
				D3DXMatrixRotationZ(&matRotate, _fAngleZ);
				++m_iNeckIndex;
			}
		}
	}

	if (nullptr != _pBoneName && !m_bIsUpper)
	{
		char* pName = pFrame->Name;
		if (nullptr != pName)
		{
			if (!strcmp(_pBoneName, pName))
			{
				m_bIsUpper = true;
				m_pUpperAniCtrl->Play_Animation(_dTimeDelta * 1);
			}
		}
	}

	pFrame->CombinedTransformMatrix = matRotate * pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices_Mix((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix, _pBoneName, _fAngleY, _fAngleZ, _dTimeDelta);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices_Mix((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix, _pBoneName, _fAngleY, _fAngleZ, _dTimeDelta);

}

void CDynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED * pFrame)
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

void CDynamicMesh::Set_WaistMat(const vector<const char*>& _vecWaistBoneName)
{
	for (const char* iter : _vecWaistBoneName)
		m_vecWaistMat.emplace_back(iter);
}

void CDynamicMesh::Set_NeckMat(const vector<const char*>& _vecNeckBoneName)
{
	for (const char* iter : _vecNeckBoneName)
		m_vecNeckMat.emplace_back(iter);
}

_bool Engine::CDynamicMesh::Find_Alpha(const char* pFileName)
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