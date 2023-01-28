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
	// ��, �浹�� �˻��� ���̾�
	//static _bool Collision_Sphere_Layer(const _vec3 * pDst, const _float fDstRadius, wstring strTargetLayer);

	//pAttacker			= ������ ���
	//pAttackPos		= ������ ����� Pos, attacker���� �޾ƿ͵� �ɵ�
	//fAttackRadius		= ������ ����� ���ݱ�ü ������
	//strTargetLayer	= ������ ���̾�, �ڿ� pTargetList�� ���� ������ ����
	//eColType			= ������ ���̾��� ��ü Ÿ��
	//dwOption			= ���� �ɼ�
	//pOut_CollisionPos = �浹 ���� �� ������ ����?
	//pTargetList		= �浹 �˻��� ����Ʈ�� ���� ������ strTagLayer�� �˻����
	//bWeapon			= �÷��̾� ���� ���� ���⸦ ��� �ִ� �� ( ���� ������ ������ ���� �ʹ� �۾����� )
	static _bool AttackOnUnit( class CUnit* pAttacker, const _vec3 * pAttackPos, const _float* fAttackRadius,
								const wstring& strTargetLayer, CSphereCollider::COL_TYPE eColType, const _ulong& dwOption = 0,
								_vec3* pOut_CollisionPos = nullptr, list<CGameObject*>* pTargetList = nullptr, _bool bWeapon = false, _mat** _pTargetMat = nullptr, CGameObject** pHitObject = nullptr);

	static _bool Collision_Sphere_Layer(wstring strSrcLayer, wstring strDstLayer);

	static _float Collision_Terrain(_vec3& vPlayerPos);

	static void  Get_ListRangeDino(list<CGameObject*>& pTargetList, _vec3& vMyPos, _float fDis);

};

#endif
