#include "stdafx.h"

#include "Obstacle.h"

#include "Export_Function.h"
//#include "StaticMesh.h"
//#include "Shader.h"
#include "Calculator.h"
#include "PhysX_Mgr.h"

USING(Engine)

CObstacle::CObstacle(LPDIRECT3DDEVICE9 _pDevice)
	: CUnit(_pDevice)
{
	ZeroMemory(m_vBoundingBox, sizeof(_vec3) * 8);
	ZeroMemory(m_vBoundingCenter, sizeof(_vec3));
}

CObstacle::CObstacle(const CObstacle & rhs)
	: CUnit(rhs)
	//, m_vColor(rhs.m_vColor)
	, m_fDetail(rhs.m_fDetail)
	, m_fBoundingRadius(rhs.m_fBoundingRadius)
	, m_vBoundingCenter(rhs.m_vBoundingCenter)
{
	memcpy(m_vBoundingBox, rhs.m_vBoundingBox, sizeof(_vec3) * 8);
}

HRESULT CObstacle::Ready_GameObject_Prototype()
{
	CUnit::Ready_GameObject_Prototype();

	//m_iID = UNITID_OBSTACLE;
	//m_iOption = 1;

	m_vColor = { 1.f, 1.f, 1.f };
	m_fDetail = 1.f;

	return S_OK;
}

HRESULT CObstacle::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CUnit::Ready_GameObject(_pArg);

	_int iOption = Check_Arg(_pArg);
	if (iOption < 0)
		return E_FAIL;

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

	



	return S_OK;
}

_int CObstacle::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;
	
	CUnit::Update_GameObject(_dTimeDelta);

	return NO_EVENT;
}

_int CObstacle::LateUpdate_GameObject(const _double & _dTimeDelta)
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
		g_bInstancing ?
			Engine::Add_Instance(m_wstrMeshName, RENDER_NONALPHA, m_pTransformCom->Get_WorldMatrix()) :
			Engine::Add_RenderGroup(RENDER_NONALPHA, this);	
	}

	return NO_EVENT;
}

void CObstacle::Render_GameObject()
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
}

//void CObstacle::Render_GameObjectShadowMap()
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

//void CObstacle::Render_GameObjectShadow()
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

CObstacle * CObstacle::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CObstacle*	pInstance = new CObstacle(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CObstacle::Clone(void * _pArg/* = nullptr*/)
{
	CObstacle*	pClone = new CObstacle(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CObstacle::Free()
{
	CUnit::Free();
}

HRESULT CObstacle::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

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

HRESULT CObstacle::Init_Mesh(wstring strMeshTag, _vec3 vScale, _vec3 vRot, _vec3 vPos)
{
	if (m_pMeshCom || !m_pTransformCom)
		return E_FAIL;

	//�±׿� �ش��ϴ� �޽�������Ʈ �ҷ��� ����
	TCHAR szMeshName[MAX_PATH] = L"";
	wsprintf(szMeshName, strMeshTag.c_str());
	m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(szMeshName));
	if (!m_pMeshCom)
	{
		ERR_MSG(L"CObstacle : MeshTag not found");
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

HRESULT CObstacle::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	Set_ConstantTable_Normal(_pEffect, m_pTransformCom->Get_WorldMatrix());

	_pEffect->SetVector("g_vSetColor", &_vec4(m_vColor, 1.f));
	_pEffect->SetFloat("g_fDetail", m_fDetail);

	_pEffect->CommitChanges();

	return S_OK;
}

_int CObstacle::Check_Arg(void * _pArg)
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