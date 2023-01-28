#pragma once
#ifndef __OPTIMIZATION_H__
#define __OPTIMIZATION_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL COptimization : public CComponent
{
private:
	explicit COptimization(LPDIRECT3DDEVICE9 _pDevice);
	explicit COptimization(const COptimization& rhs);
	virtual ~COptimization() = default;

public:
	virtual HRESULT			Ready_Component_Prototype() override;
	virtual HRESULT			Ready_Component(void * _pArg = nullptr) override;

public:
	_bool					Isin_Frustum_ForObject(const _vec3& _vMin, const _vec3& _vMax, const _vec3* _pPos);

public:
	static COptimization*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent *	Clone(void * pArg = nullptr) override;
	virtual void			Free() override;

private:
	_vec3					m_vPoint[8];
	D3DXPLANE				m_Plane[6];

};
END
#endif //!__OPTIMIZATION_H__
