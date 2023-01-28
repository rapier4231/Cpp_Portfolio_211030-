#pragma once
#ifndef __DYNAMICMESH_H__
#define __DYNAMICMESH_H__

#include "Component.h"

BEGIN(Engine)

class CAniCtrl;
class CHierarchyLoader;
class ENGINE_DLL CDynamicMesh : public CComponent
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 _pDevice, const wstring& _pFilePath, const wstring& _pFileName);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void) = default;

public:
	virtual HRESULT				Ready_Component_Prototype() override;
	virtual HRESULT				Ready_Component(void * _pArg = nullptr) override;
	void						Render_Meshes(void);
	void						Render_Meshes(LPD3DXEFFECT& _pEffect, _bool bDesolve, _bool* _bSubsetRender = nullptr, SHADER_PASS_MESH ePass = SHADER_PASS_NORMAL);
	void                        Render_MeshesShadowMap(LPD3DXEFFECT& _pEffect, _bool* _bSubsetRender = nullptr);
	void                        Render_MeshesShadow(LPD3DXEFFECT& _pEffect, _bool* _bSubsetRender = nullptr);

public:
	void						Set_Animation(const _uint& iIndex);
	void						Set_UpperAnimation(const _uint& iIndex, const _double& _dTime = 0.0);
	void						Play_Animation(const _double& _dTimeDelta, _uint _iUpdateType = 0, const char* _pBoneName = nullptr, _float _fAngleY = 0.f, _float _fAngleX = 0.f, 
												_bool bBaby = false, const char* pBoneHead = nullptr, const char* pBoneTail = nullptr);
	void						Play_BlendAnimation(const _double& _dTimeDelta, const _double& _dUpperTimeDelta, const char* _pBoneName = nullptr);
	void						Play_Animation_End(const _double & fTimeDelta, const char* _pBoneName, _bool bBaby = false, const char* pBoneHead = nullptr, const char* pBoneTail = nullptr);
	void						Play_CustomAnimation(_int iIndex, _mat* pCombineMatix);

	void						Reset_Animtion();
	void						Reset_UpperAnimtion();

	_double						Get_CurrentTime();
	_double						Get_UpperCurrentTime();
	_double						Get_AccTime();
	_double						Get_UpperAccTime();

public:
	D3DXFRAME*					Get_RootFrame() { return m_pRootFrame; }
	void						Get_BoneNames(D3DXFRAME_DERIVED* pFrame, list<_vec3>* listBonsPos);

public: 
	const D3DXFRAME_DERIVED*	Get_FrameByName(const char* pFrameName);
	_bool						Is_AnimationsetEnd(_double _dRemoveTime = 0.1);
	_bool						Is_UpperAnimationsetEnd(_double _dRemoveTime = 0.1);
	LPD3DXANIMATIONCONTROLLER	Get_AnimationCtrl();

	list<D3DXMESHCONTAINER_DERIVED*>*	Get_MeshList() { return &m_MeshContainerList; }
		

	// 허리, 목 매트릭스 세팅
	void						Set_WaistMat(const vector<const char*>& _vecWaistBoneName);
	void						Set_NeckMat(const vector<const char*>& _vecNeckBoneName);

private:
	_bool						Find_Alpha(const char* pFileName);

public:
	static CDynamicMesh*		Create(LPDIRECT3DDEVICE9 _pDevice, const wstring& _strFilePath, const wstring& _strFileName);
	virtual CComponent *		Clone(void * pArg = nullptr) override;
	virtual void				Free(void) override;

private:
	void						Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _mat* pParentMatrix);
	void						Update_FrameMatrices_Blend(D3DXFRAME_DERIVED* pFrame, const _mat* pParentMatrix, const char* _pBoneName, const _double& _dTimeDelta, const _double& _dUpperTimeDelta, 
														  _bool bBaby, const char* pBoneHead, const char* pBoneTail);
	void						Update_FrameMatrices_Mix(D3DXFRAME_DERIVED* pFrame1, const _mat* pParentMatrix, const char* _pBoneName, _float _fAngleY, _float _fAngleX, const _double& _dTimeDelta);
	void						SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);


private:
	D3DXFRAME*								m_pRootFrame;

	CAniCtrl*								m_pAniCtrl;
	CHierarchyLoader*						m_pLoader = nullptr;
	list<D3DXMESHCONTAINER_DERIVED*>		m_MeshContainerList;

	wstring									m_strFilePath;
	wstring									m_strFileName;

	// 추가 애니메이션 컨트롤러
	_bool									m_bIsUpper = false;
	CAniCtrl*								m_pUpperAniCtrl;

	// 허리
	vector<const char*>						m_vecWaistMat;
	vector<const char*>						m_vecNeckMat;
	_uint									m_iWaistIndex = 0;
	_uint									m_iNeckIndex = 0;

	map<_ulong, const char*>				m_mapBoneName;
};
END
#endif // !__DYNAMICMESH_H__
