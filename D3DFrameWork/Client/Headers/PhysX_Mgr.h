#pragma once
#ifndef __PhysX_Mgr_H__
#define __PhysX_Mgr_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CPhysX_Mgr : public CBase
{
	DECLARE_SINGLETON(CPhysX_Mgr)
private:
	explicit CPhysX_Mgr();
	virtual ~CPhysX_Mgr();

public:
	HRESULT	Ready_PhysX_Mgr();
	HRESULT Update_PhysX_Mgr_Simulate(const _double & _dTimeDelta);

public:
	PxCooking*					Get_Cooking() { return m_pCooking; }
	PxPhysics*					Get_Physics() { return m_pPhysics; }

	PxScene*					Get_PxScene() { return m_pPxScene; }
	PxControllerManager*		Get_Manager() { return m_pManager; }

	PxMaterial*					Get_PxMaterial() { return m_pPxMaterial; }

public:
	PxController*				CreateCapsuleController(_float fRad, _float fHeight, _vec3 vPos, _float fSlopeLimit = 30.f);
	PxRigidDynamic*				CreateSphereRigidDynamic(_vec3 vPos, _float fRad);

	_bool 						RayCast(PxVec3& vPos, PxVec3& vDis, _float _fMaxDis, _float* _pOutPickDis = nullptr, _vec3* _pOutPickPos = nullptr);

	_bool						IsOverlap_Capsule_StaticObject(_float fRad, _float fHeight, PxVec3& vPos);
	_bool						IsOverlap_Sphere_StaticObject(_float fRad, PxVec3& vPos);

public:
	virtual void				Free();

private:
	PxPhysics*					m_pPhysics = nullptr;
	PxFoundation*				m_pFoundation = nullptr;
	PxPvd*						m_pPvd = nullptr;
	PxScene*					m_pPxScene = nullptr;
	PxDefaultCpuDispatcher*		m_pDisPatcher = nullptr;
	PxCooking*					m_pCooking = nullptr;
	PxControllerManager*		m_pManager = nullptr;

	//////////////////////////////////////////////////////////////////////////
	PxMaterial*		m_pPxMaterial = nullptr;
	PxRigidStatic*	m_pGround = nullptr;
};
END

#endif // !__PhysX_Mgr_H__
