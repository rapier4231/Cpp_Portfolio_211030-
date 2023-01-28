#include "stdafx.h"
#include "..\Headers\CollisionFunc.h"
#include "SphereCollider.h"
#include "Unit.h"
#include "Export_Function.h"

#include "Unit.h"
#include "Bullet.h"

CCollisionFunc::CCollisionFunc()
{
}


CCollisionFunc::~CCollisionFunc()
{
}
//
////
//_bool CCollisionFunc::Collision_Sphere_Layer(const _vec3 * pSrc, const _float fSrcRad, wstring strTargetLayer)
//{
//	map<wstring, CGameObject*>* pList = Engine::Get_LayerList(strTargetLayer);
//
//	if (nullptr == pList)
//		return false;
//
//	_float fDis = 0.f, fTargetRad = 0.f;
//	for (auto& pGameObject : *pList)
//	{
//		CComponent* pComponent = pGameObject.second->Get_Component(L"Com_SphereCollider", ID_DYNAMIC);
//		if (nullptr == pComponent)
//			return false;
//
//		CSphereCollider* pTargetCollider = static_cast<CSphereCollider*>(pComponent);
//
//		_vec3 vTargetPos = pTargetCollider->Get_Pos();
//		_float fTargetRad = pTargetCollider->Get_Radius();
//
//		fDis = D3DXVec3Length(&(vTargetPos - *pSrc));
//		
//		if (fDis <= fTargetRad + fSrcRad)
//			return true;
//	}
//
//	return false;
//}

_bool CCollisionFunc::AttackOnUnit( class CUnit* pAttacker, const _vec3 * pAttackPos, const _float* fAttackRadius,
									const wstring& strTargetLayer, CSphereCollider::COL_TYPE eColType, const _ulong& dwOption,
									_vec3* pOut_CollisionPos, list<CGameObject*>* pListAtackObejct, _bool bWeapon, _mat** _pTargetMat, CGameObject** pHitObejct)
{
	list<CGameObject*>* pTargetList = nullptr;

	// List사용하면 Layer에서 검색안하고 받아온 list로 충돌 검사
	if (pListAtackObejct)
		pTargetList = pListAtackObejct;
	else
		pTargetList = Engine::Get_LayerList(strTargetLayer);

	if (pTargetList == nullptr)
		return false;

	if (pTargetList->empty())
		return false;

	_float vAttackScale = static_cast<CTransform*>(pAttacker->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Scale()->x;
	if (bWeapon)
		vAttackScale = 1.f;

	_float fDis = 0.f, fTargetRad = 0.f, fAttackRad = 0.f;
	_bool bCollided = false;
	for (auto& pTargetObject : *pTargetList)
	{
		// 자기 자신은 패스
		if (pTargetObject == pAttacker)
			continue;

		CComponent* pComponent = pTargetObject->Get_Component(L"Com_SphereCollider", ID_DYNAMIC);
		if (nullptr == pComponent)
			continue;

		CSphereCollider* pTargetCollider = static_cast<CSphereCollider*>(pComponent);

		pComponent = pTargetObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
		if (nullptr == pComponent)
			continue;
		CTransform* pTargetTransform = static_cast<CTransform*>(pComponent);

		_vec3 vTargetScale = *pTargetTransform->Get_Scale();
		list<CSphereCollider::SPHEREINFO>* tTargetSpheres = pTargetCollider->Get_listCollider(eColType);

		if (tTargetSpheres->empty())
			continue;

		for (auto& tTargetSphere : *tTargetSpheres)
		{
			_vec3 vTargetPos;
			memcpy(vTargetPos, &tTargetSphere.matColMatrix.m[3][0], sizeof(_vec3));

			_float fScale = vTargetScale.x;
			if (tTargetSphere.strBoneName == L"")
				fScale = 1.f;

			fTargetRad = tTargetSphere.fRadius * fScale;
			fAttackRad = *fAttackRadius * vAttackScale;

			fDis = D3DXVec3Length(&(vTargetPos - *pAttackPos));

			if (fDis <= fTargetRad + fAttackRad)
			{
				_vec3 vCollisionPos, vCollisionDir;
				D3DXVec3Normalize(&vCollisionDir, &(*pAttackPos - vTargetPos));		//공격자를 향하는 방향
				vCollisionPos = vTargetPos + vCollisionDir * fTargetRad;			//으로 자신의 충돌체 반지름만큼 이동 = 최초충돌한위치
				
				bCollided = static_cast<CUnit*>(pTargetObject)->HitBy(pAttacker, dwOption, vCollisionPos);
				
				if (nullptr != pHitObejct)
				{
					*pHitObejct = pTargetObject;
					if (nullptr != _pTargetMat)
					{
						*_pTargetMat = &tTargetSphere.matColMatrix;
						return true;
					}
					return bCollided;
				}

				if (nullptr != pOut_CollisionPos)
				{
					*pOut_CollisionPos = vCollisionPos;
				}

			}
		}
	}

	return bCollided;
}

_bool CCollisionFunc::Collision_Sphere_Layer(wstring strSrcLayer, wstring strDstLayer)
{
	//list<CGameObject*>* pListSrc = Engine::Get_LayerList(strSrcLayer);
	//list<CGameObject*>* pListDst = Engine::Get_LayerList(strDstLayer);

	/*map<wstring, CGameObject*>* pSrcGameObjects = Engine::Get_LayerMap(strSrcLayer);
	map<wstring, CGameObject*>* pDstGameObjects = Engine::Get_LayerMap(strDstLayer);

	if (nullptr == pListSrc || nullptr == pListDst)
		return false;

	_float fDis = 0.f, fTargetRad = 0.f, fSrcRad = 0.f;
	_vec3  vSrcPos;

	for (auto& pSrcGameObject : *pListSrc)
	{
		CComponent*	pComponent = pSrcGameObject->Get_Component(L"Com_SphereCollider", ID_DYNAMIC);
		if (nullptr == pComponent)
			return false;

		CSphereCollider* pSrcCollider = static_cast<CSphereCollider*>(pComponent);
		fSrcRad = pSrcCollider->Get_Radius();
		vSrcPos = pSrcCollider->Get_Pos();

		for (auto& pDstGameObject : *pListDst)
		{
			pComponent = pDstGameObject->Get_Component(L"Com_SphereCollider", ID_DYNAMIC);
			if (nullptr == pComponent)
				return false;

			CSphereCollider* pTargetCollider = static_cast<CSphereCollider*>(pComponent);

			_vec3 vTargetPos = pTargetCollider->Get_Pos();
			_float fTargetRad = pTargetCollider->Get_Radius();

			fDis = D3DXVec3Length(&(vTargetPos - vSrcPos));

			if (fDis <= fTargetRad + fSrcRad)
				return true;
		}
	}*/

	return false;
}

_float CCollisionFunc::Collision_Terrain(_vec3& vPlayerPos)
{
	list<CGameObject*>* pTiles = Engine::Get_LayerList(L"Layer_Tile");

	if (nullptr == pTiles)
		return false;

	for (auto& pTile : *pTiles)
	{
		CComponent* pComponent = pTile->Get_Component(L"Com_Mesh", ID_STATIC);
		if (nullptr == pComponent)
			return false;

		CStaticMesh* pTargetMesh = static_cast<CStaticMesh*>(pComponent);

		pComponent = pTile->Get_Component(L"Com_Transform", ID_DYNAMIC);
		if (nullptr == pComponent)
			return false;

		_mat matTargetWorld = static_cast<CTransform*>(pComponent)->Get_WorldMatrix();

		// 플레이어 포스가 레이포스
		_vec3 vRayPos = _vec3(vPlayerPos.x, vPlayerPos.y + 3.f, vPlayerPos.z);

		// 수직
		_vec3 vRayDir = {0.f, -1.f, 0.f};

		_ulong dwVtxNum = pTargetMesh->Get_VtxNum();
		const _vec3* pStaticPos = pTargetMesh->Get_VtxPos();

		_vec3 v1, v2, v3;
		_float	fU, fV, fDist;
		for (_ulong i = 0; i < dwVtxNum-2; ++i)
		{
			// 정점들을 월드좌표로 변환
			D3DXVec3TransformCoord(&v1, &pStaticPos[i], &matTargetWorld);
			D3DXVec3TransformCoord(&v2, &pStaticPos[i + 1], &matTargetWorld);
			D3DXVec3TransformCoord(&v3, &pStaticPos[i + 2], &matTargetWorld);
		
			if (D3DXIntersectTri(&v1, &v2, &v3, &vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				_vec3 vPos = v1 + fU * (v2 - v1) + fV * (v3 - v1);
		
				return vPos.y;
			}
		}

	}

	return vPlayerPos.y;
}

void CCollisionFunc::Get_ListRangeDino(list<CGameObject*>& pTargetList, _vec3& vMyPos, _float fDis)
{
	// 일단 범위안에 플레이어와 길들여진 공룡만 체크
	list<CGameObject*>* pDinos = Get_LayerList(L"Layer_Dino");

	//공룡이 없으면 그냥 리턴
	if (pDinos->empty())
		return;

	for (auto& pDino : *pDinos)
	{
		CTransform* pTargetTransform = static_cast<CTransform*>(pDino->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 pDstPos = *pTargetTransform->Get_Pos();

		_float fTargetDis = D3DXVec3Length(&(vMyPos - pDstPos));

		if (fTargetDis > fDis)
			continue;

		pTargetList.emplace_back(pDino);
	}
}
