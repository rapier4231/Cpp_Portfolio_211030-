#pragma once
#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include "Component.h"

BEGIN(Engine)
class CCollider;
class ENGINE_DLL CCalculator : public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * _pArg = nullptr) override;

public:
	_float		Get_Angle(const _vec3* srcPos, const _vec3* srcDir, const _vec3* DstPos, _vec3* vCross = nullptr);
	_float		Get_Distance(const _vec3* srcPos, const _vec3* DstPos);

	const _mat& VectorToRotationMatrix(_vec3* vDir, const _vec3* vUp);

public:
	static CCalculator* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__CALCULATOR_H__
