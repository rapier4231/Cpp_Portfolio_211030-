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
	LPD3DXMESH				m_pMesh;			// ����(�븻������ ����)�� �޽� �İ�ü

	LPD3DXBUFFER			m_pAdjacency;		// ������ �������� ù ��° ������ �����صα� ���� �ڷ���
	LPD3DXBUFFER			m_pSubset;			// ����� ������ �����ϱ� ���� �ڷ���
	_ulong					m_dwSubsetCnt;		// �� �������! ������� ���� == ������ ���� == �ؽ����� ����
	D3DXATTRIBUTERANGE*		m_pAttTable;		// �� ����� ������ ���� �� �ִ� Ư�����̺�. �޽� ����ȭ ���� ������
	_ulong					m_dwAttTableCnt;	// Ư�����̺� ���� (== ����°���?)

	LPD3DXMATERIAL			m_pMtrl;				// �޽��� ���� ������ �����ϱ� ���� ����ü ������
	//vector<LPDIRECT3DTEXTURE9>	m_vecTexture[TEXTURE_END];	// 0 ��ǻ��, 1 �븻, 2 ����ŧ��, 3 ���� �ؽ�ó

	LPDIRECT3DTEXTURE9*		m_ppTexture[TEXTURE_END];		// ���� ���� �ؽ�ó�� �����ϱ� ���� ���� �����ͷ� ����

	LPD3DXMESH				m_pOriMesh;			// �������� ���� ���� �ε� ������ �޽� �İ�ü

	_ulong					m_dwVtxNum;
	_ulong					m_dwVtxSize;		// �޽��� ���� ������ ũ�⸦ �����ϴ� ����

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
