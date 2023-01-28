#include "stdafx.h"
#include "BasketballHoop.h"
#include "Export_Function.h"
#include "Calculator.h"
#include "PhysX_Mgr.h"
#include "CollisionFunc.h"
#include "RingCommand_UI.h"

USING(Engine)

CBasketballHoop::CBasketballHoop(LPDIRECT3DDEVICE9 _pDevice)
	: CUnit(_pDevice)
{
	ZeroMemory(m_vBoundingBox, sizeof(_vec3) * 8);
	ZeroMemory(m_vBoundingCenter, sizeof(_vec3));
}

CBasketballHoop::CBasketballHoop(const CBasketballHoop & rhs)
	: CUnit(rhs)
	//, m_vColor(rhs.m_vColor)
	, m_fDetail(rhs.m_fDetail)
	, m_fBoundingRadius(rhs.m_fBoundingRadius)
	, m_vBoundingCenter(rhs.m_vBoundingCenter)
{
	memcpy(m_vBoundingBox, rhs.m_vBoundingBox, sizeof(_vec3) * 8);
}

HRESULT CBasketballHoop::Ready_GameObject_Prototype()
{
	CUnit::Ready_GameObject_Prototype();

	//m_iID = UNITID_BasketballHoop;
	//m_iOption = 1;

	m_vColor = { 1.f, 1.f, 1.f };
	m_fDetail = 1.f;

	return S_OK;
}

HRESULT CBasketballHoop::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CUnit::Ready_GameObject(_pArg);

	//_int iOption = Check_Arg(_pArg);
	//if (iOption < 0)
	//	return E_FAIL;
	Init_Mesh(L"Proto_StaticMesh_BasketballHoop", _vec3(0.01f, 0.01f, 0.01f), _vec3(0.f, -45.f, 0.f), _vec3(-315.f, -68.f, 54.f));

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//�޽ø� ���� �� ������(����) �ۼ�
	D3DXComputeBoundingSphere(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), sizeof(_vec3), &m_vBoundingCenter, &m_fBoundingRadius);
	//������ ���ؼ� ����� ��ȯ
	m_pTransformCom->Update_Component(0);
	D3DXVec3TransformCoord(&m_vBoundingCenter, &m_vBoundingCenter, &m_pTransformCom->Get_WorldMatrix());
	m_fBoundingRadius *= Get_Scale().x;

	////�޽ø� ���� �� ������(����) �ۼ�
	//_vec3 vMin, vMax;
	//D3DXComputeBoundingBox(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), sizeof(_vec3), &vMin, &vMax);
	//m_vBoundingBox[0] = _vec3(vMin.x, vMin.y, vMin.z);	//	- - -
	//m_vBoundingBox[1] = _vec3(vMax.x, vMin.y, vMin.z);	//	+ - -
	//m_vBoundingBox[2] = _vec3(vMin.x, vMax.y, vMin.z);	//	- + -
	//m_vBoundingBox[3] = _vec3(vMax.x, vMax.y, vMin.z);	//	+ + -
	//m_vBoundingBox[4] = _vec3(vMin.x, vMin.y, vMax.z);	//	- - +
	//m_vBoundingBox[5] = _vec3(vMax.x, vMin.y, vMax.z);	//	+ - +
	//m_vBoundingBox[6] = _vec3(vMin.x, vMax.y, vMax.z);	//	- + +
	//m_vBoundingBox[7] = _vec3(vMax.x, vMax.y, vMax.z);	//	+ + +

	////������(����)�� ����� ��ȯ�� ����
	//m_pTransformCom->Update_Component(0);
	//D3DXVec3TransformCoordArray(m_vBoundingBox, sizeof(_vec3), m_vBoundingBox, sizeof(_vec3), &m_pTransformCom->Get_WorldMatrix(), 8);

	m_vBallSpawnPos = { -300.f, -50.f, 50.f };

	CRingCommand_UI::STRCD Teemp;
	Teemp.wstrCommand_Text = L"�̴ϰ��� ������ ����(15��)";
	m_vRingCommand.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"�̴ϰ��� ������ ����(30��)";
	m_vRingCommand.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"�̴ϰ��� ������ ����(45��)";
	m_vRingCommand.emplace_back(Teemp);

	return S_OK;
}

_int CBasketballHoop::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	m_vBallSpawnPos = { -300.f, -60.f, 50.f };
	
	CUnit::Update_GameObject(_dTimeDelta);

	_vec3 vPos = Get_Pos();
	m_pSphereColliderCom->Set_Radius(0.2f);
	m_pSphereColliderCom->Set_Pos(_vec3(vPos.x + 1.f, vPos.y + 4.5f, vPos.z - 0.5f));

	_bool bCol = CCollisionFunc::AttackOnUnit(this, m_pSphereColliderCom->Get_Pos(), m_pSphereColliderCom->Get_Radius(), L"Layer_Ball", CSphereCollider::COL_HIT, 0, nullptr, nullptr, true);
	
	if (Engine::Get_DIKeyDown(DIK_PGUP) || bCol)
	{
		OBJINFO tInfo;
		tInfo.vPos = m_vBallSpawnPos;
		tInfo.vRot = { 0.f,0.f,0.f };
		tInfo.dwOption = 0;
		CGameObject* pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dodo_Ball", &tInfo));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Ball", pGameObject), E_FAIL);
	}


	return NO_EVENT;
}

_int CBasketballHoop::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	//�׸��� �����׷� �߰� : ��豸ü�� �� ����ü �ȿ� �� ��쿡�� �����׷쿡 �߰��Ѵ�
	//if (Engine::Check_Sphere(m_vBoundingCenter, m_fBoundingRadius, CULL_LIGHT))
	//{
	//	g_bInstancing ?
	//		Engine::Add_Instance(m_wstrMeshName, RENDER_SHADOWMAP, m_pTransformCom->Get_WorldMatrix()) :
	//		Engine::Add_RenderGroup(RENDER_SHADOWMAP, this);
	//}
	
	//�����׷� �߰� : ��豸ü�� ����ü �ȿ� �� ��쿡�� �����׷쿡 �߰��Ѵ�

	if (Engine::Check_Sphere(m_vBoundingCenter, m_fBoundingRadius))
	{
		Engine::Add_RenderGroup(RENDER_NONALPHA, this);	
	}

	return NO_EVENT;
}

void CBasketballHoop::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

#ifdef _DEBUG
	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Render_SphereCollider();
#endif // _DEBUG

}

//void CBasketballHoop::Render_GameObjectShadowMap()
//{
//	LPD3DXEFFECT pEffect = m_pShaderShadowMapCom->Get_EffectHandle();
//	pEffect->AddRef();
//
//	Set_ConstantTable_ShadowMap(pEffect, m_pTransformCom->Get_WorldMatrix());
//
//	pEffect->Begin(NULL, 0);
//	pEffect->BeginPass(0);
//
//	m_pMeshCom->Render_MeshesShadowMap(pEffect);
//
//	pEffect->EndPass();
//	pEffect->End();
//
//	Safe_Release(pEffect);
//}

//void CBasketballHoop::Render_GameObjectShadow()
//{
//	LPD3DXEFFECT pEffect = m_pShaderShadowCom->Get_EffectHandle();
//	pEffect->AddRef();
//
//	m_pDevice->GetTransform(D3DTS_VIEW, &m_matView);
//	m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);
//
//	m_matWVP = m_pTransformCom->Get_WorldMatrix() * m_matView * m_matProj;
//	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
//	m_matWLVB = m_matWLV * m_matScaleBias;
//
//	pEffect->SetMatrix("g_matWorldViewProj", &m_matWVP);
//	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);
//	pEffect->SetMatrix("g_matWorldLightViewProjBias", &m_matWLVB);
//
//	D3DXMatrixInverse(&m_matWLVB, NULL, &m_pTransformCom->Get_WorldMatrix());
//	D3DXVec3Transform(&m_vDir, &m_vecLightPos, &m_matWLVB);
//	D3DXVec4Normalize(&m_vDir, &m_vDir);
//	m_vDir.w = 0;
//
//	pEffect->SetVector("g_vLightDir", &m_vDir);
//
//	Engine::Get_RenderTargetTexture(pEffect, L"ShadowMap", "g_ShadowMapTexture");
//	Engine::Get_RenderTargetTexture(pEffect, L"ShadowBlurY", "g_ShadowBlurTexture");
//	pEffect->SetBool("g_bTerrain", true);
//
//	pEffect->Begin(NULL, 0);
//	pEffect->BeginPass(0);
//
//	m_pMeshCom->Render_MeshesShadow(pEffect);
//
//	pEffect->EndPass();
//	pEffect->End();
//
//	Safe_Release(pEffect);
//}

CBasketballHoop * CBasketballHoop::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CBasketballHoop*	pInstance = new CBasketballHoop(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CBasketballHoop::Clone(void * _pArg/* = nullptr*/)
{
	CBasketballHoop*	pClone = new CBasketballHoop(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CBasketballHoop::Free()
{
	CUnit::Free();
}

void CBasketballHoop::Open_Ring_Command(CUnit* pUnit)
{
	static_cast<CRingCommand_UI*>(Engine::Get_GameObject(L"RingCommand_UI"))->Open_RingCommand(m_vRingCommand, pUnit);
}

void CBasketballHoop::Set_Ring_Command(_int _iSelectCommand)
{
	// 0 : 15��
	// 1 : 30��
	// 2 : 45��
	switch (_iSelectCommand)
	{
	case 0:
		m_dTimeSetting = 15.0;
		break;
	case 1:
		m_dTimeSetting = 30.0;
		break;
	case 2:
		m_dTimeSetting = 45.0;
		break;
	}

	m_bGameStart = true;
}

HRESULT CBasketballHoop::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	/// �浹ü ������Ʈ
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_HIT;
	tInfo.fRadius = 0.1f;
	tInfo.vSpherePos = Get_Pos();

	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);

	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	tInfo.fRadius = 2.f;
	tInfo.vSpherePos = Get_Pos();
	m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);

	//�޽�������Ʈ - Init_Mesh�� ����
	
	///// ���̴� ������Ʈ - Ŭ���̾�Ʈ����
	//pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Terrain"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);
	//
	//Ʈ������ ������Ʈ - Init_Mesh���� ���� ȣ���� ��
	//pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Transform", pComponent);

	///// ��Ÿ ������Ʈ
	//pComponent = m_pCalculatorCom = static_cast<CCalculator*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Calculator"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);

	return S_OK;
}

HRESULT CBasketballHoop::Init_Mesh(wstring strMeshTag, _vec3 vScale, _vec3 vRot, _vec3 vPos)
{
	if (m_pMeshCom || !m_pTransformCom)
		return E_FAIL;

	//�±׿� �ش��ϴ� �޽�������Ʈ �ҷ��� ����
	TCHAR szMeshName[MAX_PATH] = L"";
	wsprintf(szMeshName, strMeshTag.c_str());
	m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(szMeshName));
	if (!m_pMeshCom)
	{
		ERR_MSG(L"CBasketballHoop : MeshTag not found");
		return E_FAIL;
	}
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);

	//�ν��Ͻ� �� �±׷� ����� �� �ֵ��� �޽� �̸� ����
	m_wstrMeshName = strMeshTag;
	
	//���ڷ� ���� ��ġ�� Ʈ�������� �ݿ�
	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Rotate(ROT_X, vRot.x);
	m_pTransformCom->Set_Rotate(ROT_Y, vRot.y);
	m_pTransformCom->Set_Rotate(ROT_Z, vRot.z);
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Update_Component(0);
	
	//������ ó�� (��ֹ�&����Ÿ�� ���)
	//���ؽ��� �ε��� ���� ����
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = m_pMeshCom->Get_VtxNum();
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = m_pMeshCom->Get_VtxPos();

	meshDesc.triangles.count = m_pMeshCom->Get_IndexNum();
	meshDesc.triangles.stride = m_pMeshCom->Get_dwStride();
	meshDesc.triangles.data = m_pMeshCom->Get_Index();
	meshDesc.flags = PxMeshFlag::e16_BIT_INDICES;

	// �����͸� ����ȭ ��Ű��
	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	CPhysX_Mgr::GetInstance()->Get_Cooking()->cookTriangleMesh(meshDesc, writeBuffer, &result);
	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	// ����ȭ�� ������ �о�ͼ� �ﰢ�� �޽��� �����.
	PxTriangleMesh* m_TriangleMesh = CPhysX_Mgr::GetInstance()->Get_Physics()->createTriangleMesh(readBuffer);

	// �� �𸣰��� ����ƽ ������ٵ�� �����ؼ� ���� �־��ִ� �� ����.
	PxTriangleMeshGeometry triGeom;
	triGeom.triangleMesh = m_TriangleMesh;
	triGeom.scale = PxMeshScale(PxVec3(vScale.x, vScale.y, vScale.z));

	_mat matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, vRot.y, vRot.x, vRot.z);	//�� �� ȸ�������� 3x3��Ʈ������ ����
	PxMat33 mat33(PxVec3(matRot._11, matRot._12, matRot._13)			//�̶� ����ġ���� yxz �����ӿ� ����
		, PxVec3(matRot._21, matRot._22, matRot._23)
		, PxVec3(matRot._31, matRot._32, matRot._33));

	PxTransform pxTransmform;
	pxTransmform.p = vPos;
	pxTransmform.q = PxQuat(mat33);		//���ʹϾ� ������ �� ��Ʈ������ ���ڷ� �޴� ������ Ȱ��

	PxRigidStatic* actor = CPhysX_Mgr::GetInstance()->Get_Physics()->createRigidStatic(pxTransmform);
	PxMaterial* paxMat = CPhysX_Mgr::GetInstance()->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
	PxShape* shape = PxRigidActorExt::createExclusiveShape(*actor, triGeom, *paxMat);

	//PX_UNUSED(shape);

	CPhysX_Mgr::GetInstance()->Get_PxScene()->addActor(*actor);

	return S_OK;
}

HRESULT CBasketballHoop::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	Engine::Set_ConstantTable_Normal(_pEffect, m_pTransformCom->Get_WorldMatrix());

	_pEffect->SetVector("g_vSetColor", &_vec4(m_vColor, 1.f));
	_pEffect->SetFloat("g_fDetail", m_fDetail);

	_pEffect->CommitChanges();

	return S_OK;
}

_int CBasketballHoop::Check_Arg(void * _pArg)
{
	if (_pArg)
	{
		if (dynamic_cast<OBJINFO*>((OBJINFO*)_pArg))
		{
			OBJINFO tInfo;
			memcpy(&tInfo, _pArg, sizeof(OBJINFO));

			//dwOption���� ���� MeshID�������� �����޽� �Ǵ�
			TCHAR szText[MAX_PATH];
			Engine::Get_MeshTag_Obstacle(szText, tInfo.dwOption);

			if (E_FAIL == Init_Mesh(szText, tInfo.vScale, tInfo.vRot, tInfo.vPos))
				return -1;

			Set_Scale(tInfo.vScale);
			Set_Rot(tInfo.vRot.x, tInfo.vRot.y, tInfo.vRot.z);
			Set_Pos(tInfo.vPos);
			m_vColor = tInfo.vColor;
	
			Add_ExpToLevel(tInfo.dwLevel); //����ġ ���������. ���� �� �Լ�


			return tInfo.dwOption;

		}
		else
		{
			ERR_MSG(L"Wrong Format Argument");
			return 0;
		}
	}
	return 0;
}