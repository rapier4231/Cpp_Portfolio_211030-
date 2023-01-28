#pragma once
#ifndef __STATICMESH_H__
#define __STATICMESH_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CComponent
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 _pDevice, const wstring& pFilePath, const wstring& pFileName);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh() = default;

public:
	virtual HRESULT			Ready_Component_Prototype() override;
	virtual HRESULT			Ready_Component(void * _pArg = nullptr) override;

public:
	void					Render_Meshes(void);
	void					Render_Meshes(LPD3DXEFFECT& _pEffect, _bool _bDissolve = false, _int _iPass = 0);
	void					Render_MeshesShadowMap(LPD3DXEFFECT& _pEffect, _bool _bDissolve = false);
	void					Render_MeshesShadow(LPD3DXEFFECT& _pEffect, _bool _bDissolve = false);
	void					Render_Subset_ForInstancing(LPD3DXEFFECT& _pEffect, _uint iSubset, _ulong iPass = 0);
		
public:
	LPD3DXMESH				Get_Mesh() { return m_pMesh; }

	const _vec3*			Get_VtxPos(void) { return m_pVtxPos; }
	const _ulong&			Get_VtxNum(void) { return m_dwVtxNum; }
	const _ulong&			Get_VtxSize(void) { return m_dwVtxSize; }
	_bool					Find_Alpha(const char* pFileName);

	const DWORD&			Get_IndexNum() { return m_dwIndexNum; }
	const DWORD&			Get_dwStride() { return m_dwStride; }
	const LPBYTE&			Get_Index() { return m_pIndex; }
	const _ulong&			Get_SubsetCount(void) { return m_dwSubsetCnt; }

	HRESULT					RayCast_OnMesh(_vec3 * pOut, const _vec3 & vRayPos, const _vec3 & vRayDir, const _mat & matWorld);

public:
	static CStaticMesh*		Create(LPDIRECT3DDEVICE9 _pDevice, const wstring& pFilePath, const wstring& pFileName);
	virtual CComponent *	Clone(void * pArg = nullptr) override;
	virtual void			Free() override;

private:
	LPD3DXMESH				m_pMesh;			// 변형(노말정보가 삽입)된 메쉬 컴객체

	LPD3DXBUFFER			m_pAdjacency;		// 인접한 폴리곤의 첫 번째 정보를 저장해두기 위함 자료형
	LPD3DXBUFFER			m_pSubset;			// 서브셋 정보를 저장하기 위한 자료형
	_ulong					m_dwSubsetCnt;		// 꼭 기억하자! 서브셋의 개수 == 재질의 개수 == 텍스쳐의 개수
	D3DXATTRIBUTERANGE*		m_pAttTable;		// 각 서브셋 정보를 얻어올 수 있는 특성테이블. 메시 최적화 이후 생성됨
	_ulong					m_dwAttTableCnt;	// 특성테이블 개수 (== 서브셋개수?)

	LPD3DXMATERIAL			m_pMtrl;				// 메쉬의 재질 정보를 보관하기 위한 구조체 포인터
	//vector<LPDIRECT3DTEXTURE9>	m_vecTexture[TEXTURE_END];	// 0 디퓨즈, 1 노말, 2 스펙큘러, 3 알파 텍스처

	LPDIRECT3DTEXTURE9*		m_ppTexture[TEXTURE_END];		// 여러 장의 텍스처를 보관하기 위해 이중 포인터로 선언

	LPD3DXMESH				m_pOriMesh;			// 변형되지 않은 최초 로드 상태의 메쉬 컴객체

	_ulong					m_dwVtxNum;
	_ulong					m_dwVtxSize;		// 메쉬가 지닌 정점의 크기를 저장하는 변수

	_vec3*					m_pVtxPos;

	wstring					m_strFilePath;
	wstring					m_strFileName;

	LPBYTE					m_pIndex;
	DWORD					m_dwIndexNum;
	DWORD					m_dwStride;

	//_bool					m_bIsAlpha = false;
};
END
#endif // !__STATICMESH_H__
