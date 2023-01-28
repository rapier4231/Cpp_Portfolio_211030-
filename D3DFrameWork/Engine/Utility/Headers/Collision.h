//////////////////////////////////////////////////////////////////////////
// ¾È¾¹´Ï´Ù
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollision : public CComponent
{
private:
	explicit CCollision(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCollision(const CCollision& rhs);
	virtual ~CCollision() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * _pArg = nullptr) override;
	virtual HRESULT	Render_VIBuffer();

public:
	inline const _float&	Get_Radius();
	inline void				Set_Radius(const _float& fRadius);
	inline void				Set_Pos(const _vec3& vPos);
	void					Set_WorldMatrixPos(const _mat& matWorld);
	//_bool					IsMeshPicking(_vec3* _pOutVec, const POINT& _pt);

public:
	static CCollision* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	LPD3DXMESH		m_pMesh = nullptr;
	LPD3DXBUFFER	m_pBuffer = nullptr;
	_mat			m_matWorld;
	_float			m_fRadius = 0.f;
	COLLISIONSTATE	m_eCollisionState;
};
END
#endif // !__COLLISION_H__
