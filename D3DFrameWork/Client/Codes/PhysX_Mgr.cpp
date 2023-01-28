#include "stdafx.h"
#include "PhysX_Mgr.h"
#include "Export_Utility.h"
#include "Value.h"

USING(Engine)
IMPLEMENT_SINGLETON(CPhysX_Mgr)

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;

CPhysX_Mgr::CPhysX_Mgr()
{
}


CPhysX_Mgr::~CPhysX_Mgr()
{
	Free();
}

HRESULT CPhysX_Mgr::Ready_PhysX_Mgr()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (!m_pFoundation)
	{
		MessageBox(NULL, L"PhysX Foundation Error", L"System Message", MB_OK);
		return E_FAIL;
	}

	m_pPvd = PxCreatePvd(*m_pFoundation);

	PxPvdTransport* pPvdTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pPvd->connect(*pPvdTransport, PxPvdInstrumentationFlag::eALL);

	//PxTolerancesScale mPxTolerancesScale;
	//mPxTolerancesScale.length = 5;
	//mPxTolerancesScale.speed = 981;

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPvd);
	if (!m_pPhysics)
	{
		MessageBox(NULL, L"PhysX Create Error", L"System Message", MB_OK);
		return E_FAIL;
	}

	// 관절 사용하기위해
	PxInitExtensions(*m_pPhysics, m_pPvd);

	//메쉬 구워주는 객체?
	PxCookingParams params(m_pPhysics->getTolerancesScale());
	params.meshWeldTolerance = 0.001f;
	params.meshPreprocessParams = PxMeshPreprocessingFlags(PxMeshPreprocessingFlag::eWELD_VERTICES);
	params.buildGPUData = true;
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, PxCookingParams(params));
	if (!m_pCooking)
	{
		MessageBox(NULL, L"PhysX Cooking Error", L"System Message", MB_OK);
		return E_FAIL;
	}

	// 씬에 중력추가
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.f, GRAVITY, 0.f);

	m_pDisPatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDisPatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_pPxScene = m_pPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = m_pPxScene->getScenePvdClient();

	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// 기본 물리 속성 정의
	// 정지 마찰 계수 - 물체가 가만히 있었을 때
	// 운동 마찰 계수 - 움직일 때 계수
	// 반사 계수	- 통 통
	m_pPxMaterial = m_pPhysics->createMaterial(0.f, 0.f, 0.f);
	//m_pGround = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 1), *m_pPxMaterial);
	//m_pPxScene->addActor(*m_pGround);

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 메쉬에 리지드바디를 추가한다.
	// 바디에 모양을 붙인다. ( attachShape ) 
	// RigidBodyExt : 

	m_pManager = PxCreateControllerManager(*m_pPxScene);
	if (!m_pManager)
	{
		MessageBox(NULL, L"PhysX m_pManager Error", L"System Message", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPhysX_Mgr::Update_PhysX_Mgr_Simulate(const _double & _dTimeDelta)
{
	_double dTimeDebug = 0;
	0.06 < _dTimeDelta ? dTimeDebug = 0.06 : dTimeDebug = _dTimeDelta;

	m_pPxScene->simulate((_float)_dTimeDelta);
	m_pPxScene->fetchResults(true);

	return S_OK;
}

PxController * CPhysX_Mgr::CreateCapsuleController(_float fRad, _float fHeight, _vec3 vPos, _float fSlopeLimit)
{
	PxCapsuleControllerDesc cCapusuledesc;
	cCapusuledesc.height = fRad;
	cCapusuledesc.radius = fHeight;
	cCapusuledesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	cCapusuledesc.position = PxExtendedVec3(vPos.x, vPos.y, vPos.z);
	cCapusuledesc.material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);
	// 오를 수 있는 경사각
	cCapusuledesc.slopeLimit = D3DXToRadian(fSlopeLimit);

	return static_cast<PxCapsuleController*>(m_pManager->createController(cCapusuledesc));
}

PxRigidDynamic * CPhysX_Mgr::CreateSphereRigidDynamic(_vec3 vPos, _float fRad)
{
	PxRigidDynamic * pDynamic = nullptr;

	pDynamic = m_pPhysics->createRigidDynamic(PxTransform(vPos.x, vPos.y, vPos.z));
	PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*pDynamic, PxSphereGeometry(fRad), *m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f));
	m_pPxScene->addActor(*pDynamic);

	return pDynamic;
}

_bool  CPhysX_Mgr::RayCast(PxVec3& vPos, PxVec3& vDir, _float _fMaxDis, _float* _pOutPickDis/* = nullptr*/, _vec3* _pOutPickPos/* = nullptr*/)
{
	PxVec3 origin = vPos;                // [in] Ray origin
	PxVec3 unitDir = vDir;               // [in] Normalized ray direction
	PxReal maxDistance = _fMaxDis;          // [in] Raycast max distance
	PxRaycastBuffer hit;                 // [out] Raycast results

										 // Raycast against all static & dynamic objects (no filtering)
										 // The main result from this call is the closest hit, stored in the 'hit.block' structure

	//hit.block.flags = PxHitFlag::ePOSITION;
	PxQueryFilterData  filterData(PxQueryFlag::eANY_HIT | PxQueryFlag::eSTATIC);

	bool status = m_pPxScene->raycast(origin, unitDir, maxDistance, hit, PxHitFlag::eDEFAULT, filterData);

	if (nullptr != _pOutPickDis)
	{
		*_pOutPickDis = hit.block.distance;
	}
	if (nullptr != _pOutPickPos)
	{
		*_pOutPickPos = _vec3(hit.block.position.x, hit.block.position.y, hit.block.position.z);
	}

	return status;	
}

_bool CPhysX_Mgr::IsOverlap_Capsule_StaticObject(_float fRad, _float fHeight, PxVec3& vPos)
{
	PxCapsuleGeometry geom(fRad, fHeight);
	PxVec3 pos(vPos.x, vPos.y, vPos.z);
	PxQuat orientation(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f));
	PxOverlapBuffer hit;
	
	if (m_pPxScene->overlap(geom, PxTransform(pos, orientation), hit, PxQueryFilterData(PxQueryFlag::eANY_HIT | PxQueryFlag::eSTATIC)))
		return true;

	return false;
}

_bool CPhysX_Mgr::IsOverlap_Sphere_StaticObject(_float fRad, PxVec3& vPos)
{
	PxSphereGeometry geom(fRad);
	PxVec3 pos(vPos.x, vPos.y, vPos.z);
	PxQuat orientation(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f));
	PxOverlapBuffer hit;

	if (m_pPxScene->overlap(geom, PxTransform(pos, orientation), hit, PxQueryFilterData(PxQueryFlag::eANY_HIT | PxQueryFlag::eSTATIC)))
		return true;

	return false;
}


void CPhysX_Mgr::Free()
{
	//////////////////////////////////////////////////////////////////////////
	//PX_RELEASE(m_pGround);
	PX_RELEASE(m_pManager);
	PX_RELEASE(m_pPxMaterial);
	//////////////////////////////////////////////////////////////////////////

	PX_RELEASE(m_pPxScene);
	PX_RELEASE(m_pDisPatcher);

	PX_RELEASE(m_pCooking);
	PxCloseExtensions();
	PX_RELEASE(m_pPhysics);
	if (m_pPvd)
	{
		PxPvdTransport* transport = m_pPvd->getTransport();
		m_pPvd->release();
		m_pPvd = NULL;
		PX_RELEASE(transport);
	}

	PX_RELEASE(m_pFoundation);
}
