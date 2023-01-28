#pragma once
#ifndef __HIERACHYLOADER_H__
#define __HIERACHYLOADER_H__

#include "Engine_Define.h"

BEGIN(Engine)

class CHierarchyLoader : public ID3DXAllocateHierarchy
{
public:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 _pDevice, const wstring& pPath);
	virtual ~CHierarchyLoader(void) = default;

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name,
								CONST D3DXMESHDATA *pMeshData,
								CONST D3DXMATERIAL *pMaterials,
								CONST D3DXEFFECTINSTANCE *pEffectInstances,
								DWORD NumMaterials,
								CONST DWORD *pAdjacency,
								LPD3DXSKININFO pSkinInfo,
								LPD3DXMESHCONTAINER *ppNewMeshContainer);

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

public:
	static CHierarchyLoader*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & pPath);
	_ulong						Release(void);

private:
	void						Allocate_Name(char** ppName, const char* pFrameName);

private:
	LPDIRECT3DDEVICE9			m_pDevice;
	wstring						m_pPath;
};
END
#endif // !__HIERACHYLOADER_H__
