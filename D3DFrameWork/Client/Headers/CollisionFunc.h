#pragma once

#ifndef __CCollisionFunc_H__
#define __CCollisionFunc_H__

#include "Export_Function.h"
#include "Base.h"

USING(Engine)


class CCollisionFunc
{
public:
	CCollisionFunc();
	~CCollisionFunc();

public:
	// 나, 충돌할 검색할 레이어
	//static _bool Collision_Sphere_Layer(const _vec3 * pDst, const _float fDstRadius, wstring strTargetLayer);

	//pAttacker			= 공격한 대상
	//pAttackPos		= 공격한 대상의 Pos, attacker에서 받아와도 될듯
	//fAttackRadius		= 공격한 대상의 공격구체 반지름
	//strTargetLayer	= 공격할 레이어, 뒤에 pTargetList가 값이 있으면 무시
	//eColType			= 공격할 레이어의 구체 타입
	//dwOption			= 공격 옵션
	//pOut_CollisionPos = 충돌 했을 때 포스값 리턴?
	//pTargetList		= 충돌 검사할 리스트를 직접 보내면 strTagLayer로 검사안함
	//bWeapon			= 플레이어 전용 변수 무기를 들고 있는 지 ( 뼈가 없으면 스케일 값이 너무 작아져서 )
	static _bool AttackOnUnit( class CUnit* pAttacker, const _vec3 * pAttackPos, const _float* fAttackRadius,
								const wstring& strTargetLayer, CSphereCollider::COL_TYPE eColType, const _ulong& dwOption = 0,
								_vec3* pOut_CollisionPos = nullptr, list<CGameObject*>* pTargetList = nullptr, _bool bWeapon = false, _mat** _pTargetMat = nullptr, CGameObject** pHitObject = nullptr);

	static _bool Collision_Sphere_Layer(wstring strSrcLayer, wstring strDstLayer);

	static _float Collision_Terrain(_vec3& vPlayerPos);

	static void  Get_ListRangeDino(list<CGameObject*>& pTargetList, _vec3& vMyPos, _float fDis);

};

#endif
