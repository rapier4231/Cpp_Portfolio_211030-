#pragma once

#ifndef __RANDOMFUNC_H__
#define __RANDOMFUNC_H__

#include "Export_Function.h"
#include "Base.h"

USING(Engine)


class CRandomFunc
{
public:
	CRandomFunc();
	~CRandomFunc();

public:
	static _float Rand_Float(const _float& fMin, const _float& fMax, const _uint& iDivide);
	static _float Shake_Float(const _float& fValue, const _float& fPercent);
	static _int Rand_Int(const _int& iMin, const _int& iMax);
	static _vec3 Rand_Pos_InCircle(const _float& fRadius, const _vec3 vCenter = _vec3(0.f, 0.f, 0.f));
	static _vec3 Rand_Pos_InSphere(const _float& fRadius, const _vec3 vCenter = _vec3(0.f, 0.f, 0.f));

	static _vec3 Rand_Dir();
	static _vec3 Rand_Dir_2D();
};

#endif
