#pragma once
#ifndef __SKINNEDMESH_H__
#define __SKINNEDMESH_H__

#include "Component.h"

BEGIN(Engine)
// 스키닝 뼈대 라인 그리기용
typedef struct tagVertexLine
{
	tagVertexLine() {}
	tagVertexLine(_vec3 vPos, D3DCOLOR col) { vPosition = vPos; color = col; }

	_vec3 vPosition;
	D3DCOLOR color;
	static const _ulong FVF_LINE;
}VTX_LINE;

class CAniCtrl;
class CAllocateHierarchy;
class ENGINE_DLL CSkinnedMesh : public CComponent
{
private:
	explicit CSkinnedMesh(LPDIRECT3DDEVICE9 _pDevice, const wstring& _pFilePath, const wstring& _pFileName);
	explicit CSkinnedMesh(const CSkinnedMesh& rhs);
	virtual ~CSkinnedMesh(void) = default;

public:
	virtual HRESULT				Ready_Component_Prototype() override;
	virtual HRESULT				Ready_Component(void * _pArg = nullptr) override;
	void						Render_Meshes(LPD3DXEFFECT& _pEffect);
	void						Render_Meshes(LPD3DXEFFECT& _pEffect, _uint _iSaddleIndex);
	void						Render_Meshes(LPD3DXEFFECT& _pEffect, D3DXFRAME_DERIVED* _pFrame);

	void						Render_Skeleton(D3DXFRAME_DERIVED* _pFrame, D3DXFRAME_DERIVED* _pParentFrame, _mat world);
	void						Render_Skeleton(D3DXFRAME_DERIVED* _pFrame);

public:
	void						Set_Animation(const _uint& iIndex);
	void						Play_Animation(const _double& _dTimeDelta);

	void						Reset_Animtion();

	_double						Get_CurrentTime();

public:
	D3DXFRAME*					Get_RootFrame() { return m_pRootFrame; }
	void						Get_BoneNames(D3DXFRAME_DERIVED* pFrame, list<_vec3>* listBonsPos);

public: 
	const D3DXFRAME_DERIVED*	Get_FrameByName(const char* pFrameName);
	_bool						Is_AnimationsetEnd(_double _dRemoveTime = 0.1);
	LPD3DXANIMATIONCONTROLLER	Get_AnimationCtrl();

	list<D3DXMESHCONTAINER_DERIVED*>*	Get_MeshList() { return &m_MeshContainerList; }
		
public:
	static CSkinnedMesh*		Create(LPDIRECT3DDEVICE9 _pDevice, const wstring& _strFilePath, const wstring& _strFileName);
	virtual CComponent *		Clone(void * pArg = nullptr) override;
	virtual void				Free(void) override;

private:
	void						Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _mat* pParentMatrix);
	void						SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);
	
///
	void	CalculateWorldMatrices(D3DXFRAME_DERIVED* _pFrame, _mat* _pParentMat);
	void	SetupBoneMatrixPointers(D3DXFRAME_DERIVED* _pFrame);
///
private:
	D3DXFRAME*								m_pRootFrame;

	CAniCtrl*								m_pAniCtrl;
	CAllocateHierarchy*						m_pLoader = nullptr;
	list<D3DXMESHCONTAINER_DERIVED*>		m_MeshContainerList;

	wstring									m_strFilePath;
	wstring									m_strFileName;

	map<_ulong, const char*>				m_mapBoneName;


	/// 
	LPD3DXMESH		m_pSphereMesh;
};
END
#endif // !__SKINNEDMESH_H__

