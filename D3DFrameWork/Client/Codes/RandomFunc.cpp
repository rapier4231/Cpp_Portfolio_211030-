#include "stdafx.h"
#include "..\Headers\RandomFunc.h"

#include "Unit.h"

CRandomFunc::CRandomFunc()
{
}

CRandomFunc::~CRandomFunc()
{
}

//1.f 99.f min ~ max를 iDivide만큼 등분
//난수가 등분되어 나옴
_float CRandomFunc::Rand_Float(const _float& fMin, const _float& fMax, const _uint& iDivide)
{
	if (iDivide <= 1)
	{
		ERR_MSG(L"CRandomFunc::Rand_Float() - Invalid value");
	}
	/*_float f = (fMax - fMin) / iDivide;
	_int iRand = rand() % iDivide;
	return _float(fMin + f * iRand);*/
	return _float(fMin + ((fMax - fMin) / iDivide) * (rand() % iDivide));
}

// 1.f, 0.2f -> 최소 0.8 ~ 1.2f
_float CRandomFunc::Shake_Float(const _float& fValue, const _float& fPercent)
{
	/*if (fPercent <= 0)
	{
		ERR_MSG(L"CRandomFunc::Shake_Float() - Invalid value");
	}*/
	/*_float fMin = fValue * (100.f - fPercent);
	_float fMax = fValue * (100.f + fPercent);
	return Rand_Float(fMin, fMax, 100);*/
	return fValue * Rand_Float(1.f - fPercent, 1.f + fPercent, 100);
}

_int CRandomFunc::Rand_Int(const _int& iMin, const _int& iMax)
{
	/*_int i = iMax - iMin;
	_int iRand = rand() % i;
	return _int(iMin + iRand);*/
	return _int(iMin + (rand() % (iMax - iMin)));
}

_vec3 CRandomFunc::Rand_Pos_InCircle(const _float & fRadius, const _vec3 vCenter)
{
	return vCenter + Rand_Dir_2D() * Rand_Float(0.f, fRadius, (_int)fRadius * 100);
}

_vec3 CRandomFunc::Rand_Pos_InSphere(const _float & fRadius, const _vec3 vCenter)
{
	return vCenter + Rand_Dir() * Rand_Float(0.f, fRadius, (_int)fRadius * 100);
}

_vec3 CRandomFunc::Rand_Dir()
{
	_vec3 vDir = { Rand_Float(-1.f, 1.f, 100), Rand_Float(-1.f, 1.f, 100), Rand_Float(-1.f, 1.f, 100) };
	return *D3DXVec3Normalize(&vDir, &vDir);
}

_vec3 CRandomFunc::Rand_Dir_2D()
{
	_vec3 vDir = { Rand_Float(-1.f, 1.f, 100), 0.f, Rand_Float(-1.f, 1.f, 100) };
	return *D3DXVec3Normalize(&vDir, &vDir);
}
