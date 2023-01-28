#pragma once
#ifndef __VIBUFFER_TERRAIN_H__
#define __VIBUFFER_TERRAIN_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain : public CVIBuffer
{
private:
	explicit CVIBuffer_Terrain(LPDIRECT3DDEVICE9 _pDevice, const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv, _bool _bIsHMap = true);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT				Ready_Component_Prototype() override;
	virtual HRESULT				Ready_Component(void * pArg = nullptr) override;
	virtual HRESULT				Render_VIBuffer() override;

public:
	static CVIBuffer_Terrain*	Create(LPDIRECT3DDEVICE9 _pDevice, const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv, _bool _bIsHMap = true);
	virtual CComponent *		Clone(void * pArg = nullptr) override;
	virtual void				Free() override;

public:
	const _vec3*				Get_VtxPos(void) const { return m_pPos; }
	_ulong						Get_VtxCntX(void) const { return m_dwCntX; }
	_ulong						Get_VtxCntZ(void) const { return m_dwCntZ; }
	//void						Copy_Indices(INDEX32* pIndex, const _ulong& dwTriCnt);		// 나중에 사용될 코드

private:
	_ulong						m_dwCntX;
	_ulong						m_dwCntZ;
	_ulong						m_dwVtxItv;

	_bool						m_bIsHMap;		// 높이맵 사용 여부
	HANDLE						m_hFile;		// 높이맵
	BITMAPFILEHEADER			m_fileH;		// 높이맵
	BITMAPINFOHEADER			m_infoH;		// 높이맵

	_vec3*						m_pPos = nullptr;
};
END
#endif // !__VIBUFFER_TERRAIN_H__
