#include "stdafx.h"
#include "..\Headers\Thatch_Roof.h"

#include "Export_Function.h"

#include "SphereCollider.h"

USING(Engine)

CThatch_Roof::CThatch_Roof(LPDIRECT3DDEVICE9 _pDevice)
	: CStructure(_pDevice)
{
}

CThatch_Roof::CThatch_Roof(const CThatch_Roof & _rhs)
	: CStructure(_rhs)
{
}

HRESULT CThatch_Roof::Ready_GameObject_Prototype()
{
	CStructure::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CThatch_Roof::Ready_GameObject(void * _pArg)
{
	CStructure::Ready_GameObject(_pArg);

	m_tStructurInfo.fInteractRad = 2.0f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));

	// _pArg�� Pos���� ���� ���(��ġ �Ϸ� ��� ��)
	if (!m_tStructurInfo.bIsPreview)
	{
		m_tStructurInfo.eStructureType = STRUCTURE_THATCH_ROOF;
		m_pTransformCom->Set_Pos(m_tStructurInfo.vPos);
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_tStructurInfo.fAngleY));
		m_pTransformCom->Update_Component(0);

		/// �ǹ� �߰�
		_vec3 vLook, vRight;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		
		CSphereCollider::SPHEREINFO tInfo;
		tInfo.eColType = CSphereCollider::COL_BUILD_PIVOT;
		tInfo.fRadius = 1.f;
		
		tInfo.vSpherePos = vLook * 1.5f;
		tInfo.eColDir = CSphereCollider::COL_FWD;
		m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);
		
		tInfo.vSpherePos = vLook * -1.5f;
		tInfo.eColDir = CSphereCollider::COL_BWD;
		m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);
		
		tInfo.vSpherePos = vRight * 1.5f;
		tInfo.eColDir = CSphereCollider::COL_RIGHT;
		m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);
		
		tInfo.vSpherePos = vRight * -1.5f;
		tInfo.eColDir = CSphereCollider::COL_LEFT;
		m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);


		Set_PhysXCook();

		m_vColor = { 1.f, 1.f, 1.f };

		m_bDissolve = true;
		m_fDissolveAmount = 1.f;
	}

	return S_OK;
}

_int CThatch_Roof::Update_GameObject(const _double & _dTimeDelta)
{
	if (OBJECT_DEAD == CStructure::Update_GameObject(_dTimeDelta))
		return OBJECT_DEAD;

	//if (m_pBaseActor)
	//{
	//	m_pTransformCom->Move_Pos(_vec3(-1.f, 0.f, 0.f), 0.5f, _dTimeDelta);
	//	m_pTransformCom->Set_AccRotate(ROT_Y, 0.05f);
	//
	//	_vec3 vPos = *m_pTransformCom->Get_Pos();
	//	_vec3 vScale = *m_pTransformCom->Get_Scale();
	//	_mat matRot;
	//	D3DXMatrixRotationYawPitchRoll(&matRot, Get_RotY(), Get_RotX(), Get_RotZ());	//�� �� ȸ�������� 3x3��Ʈ������ ����
	//	PxMat33 mat33(PxVec3(matRot._11, matRot._12, matRot._13)			//�̶� ����ġ���� yxz �����ӿ� ����
	//		, PxVec3(matRot._21, matRot._22, matRot._23)
	//		, PxVec3(matRot._31, matRot._32, matRot._33));
	//	PxTransform px = PxTransform(vPos.x, vPos.y, vPos.z, PxQuat(mat33));
	//
	//	m_pBaseActor->setGlobalPose(px);
	//}

	return NO_EVENT;
}

_int CThatch_Roof::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CStructure::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CThatch_Roof::Render_GameObject()
{
	CStructure::Render_GameObject();
}

CThatch_Roof * CThatch_Roof::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CThatch_Roof*	pInstance = new CThatch_Roof(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CThatch_Roof::Clone(void * _pArg)
{
	CThatch_Roof*	pClone = new CThatch_Roof(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CThatch_Roof::Move_Pivot()
{
	/// �ǹ� �߰�
	_vec3 vLook, vRight;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	m_pSphereColliderCom->Get_listCollider(CSphereCollider::COL_BUILD_PIVOT);

	for (auto& iter : *m_pSphereColliderCom->Get_listCollider(CSphereCollider::COL_BUILD_PIVOT))
	{
		if (CSphereCollider::COL_FWD == iter.eColDir)
		{
			iter.vSpherePos = vLook * 1.5f;
		}
		else if (CSphereCollider::COL_BWD == iter.eColDir)
		{
			iter.vSpherePos = vLook * -1.5f;
		}
		else if (CSphereCollider::COL_RIGHT == iter.eColDir)
		{
			iter.vSpherePos = vRight * 1.5f;
		}
		else if (CSphereCollider::COL_LEFT == iter.eColDir)
		{
			iter.vSpherePos = vRight * -1.5f;
		}
	}
}

const _vec3 CThatch_Roof::Get_PivotDir(_uint _eCorDir)
{
	_vec3 vLook, vRight;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	switch (_eCorDir)
	{
	case CSphereCollider::COL_FWD:
		return vLook * 1.5f;
	case CSphereCollider::COL_BWD:
		return vLook * -1.5f;
	case CSphereCollider::COL_LEFT:
		return vRight * -1.5f;
	case CSphereCollider::COL_RIGHT:
		return vRight * 1.5f;
	case CSphereCollider::COL_UP:
		return _vec3();
	case CSphereCollider::COL_DOWN:
		return _vec3();
	default:
		return _vec3();
	}
	return _vec3();
}

HRESULT CThatch_Roof::Add_Component()
{
	CComponent*		pComponent = nullptr;

	/// �Ž� ������Ʈ
	if (STRUCTURE_THATCH_ROOF == m_tStructurInfo.eStructureType)
		pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_ThatchRoof"));
	else if (STRUCTURE_STONE_ROOF == m_tStructurInfo.eStructureType)
		pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Structure_StoneRoof"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	/// �浹ü ������Ʈ
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	tInfo.fRadius = m_tStructurInfo.fInteractRad;
	tInfo.vSpherePos = m_tStructurInfo.vPos;

	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);

	return S_OK;
}
