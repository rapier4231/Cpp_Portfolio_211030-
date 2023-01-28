#pragma once
#ifndef __VIBUFFER_WATERTERRAIN_H__
#define __VIBUFFER_WATERTERRAIN_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_WaterTerrain : public CVIBuffer
{
private:
	explicit CVIBuffer_WaterTerrain(LPDIRECT3DDEVICE9 _pDevice, const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv);
	explicit CVIBuffer_WaterTerrain(const CVIBuffer_WaterTerrain& rhs);
	virtual ~CVIBuffer_WaterTerrain() = default;

public:
	virtual HRESULT				Ready_Component_Prototype() override;
	virtual HRESULT				Ready_Component(void * pArg = nullptr) override;
	virtual HRESULT				Render_VIBuffer() override;

public:
	static CVIBuffer_WaterTerrain*	Create(LPDIRECT3DDEVICE9 _pDevice, const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv);
	virtual CComponent *		Clone(void * pArg = nullptr) override;
	virtual void				Free() override;

public:
	const _vec3*				Get_VtxPos(void) const { return m_pPos; }
	_ulong						Get_VtxCntX(void) const { return m_dwCntX; }
	_ulong						Get_VtxCntZ(void) const { return m_dwCntZ; }
	//void						Copy_Indices(INDEX32* pIndex, const _ulong& dwTriCnt);

private:
	_ulong						m_dwCntX;
	_ulong						m_dwCntZ;
	_ulong						m_dwVtxItv;

	_vec3*						m_pPos = nullptr;
};
END
#endif // !__VIBUFFER_WATERTERRAIN_H__
