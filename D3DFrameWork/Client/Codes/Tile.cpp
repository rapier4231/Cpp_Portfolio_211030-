#include "stdafx.h"

#include "Tile.h"

#include "Export_Function.h"
//#include "StaticMesh.h"
//#include "Shader.h"
#include "Calculator.h"
#include "PhysX_Mgr.h"

USING(Engine)

CTile::CTile(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	D3DXMatrixIdentity(&m_matWorld);
}

CTile::CTile(const CTile & rhs)
	: CGameObject(rhs)
	, m_matWorld(rhs.m_matWorld)
	, m_fDetail(rhs.m_fDetail)
	, m_fDetailCover(rhs.m_fDetailCover)
	, m_iPass(rhs.m_iPass)
{
	for (_uint i = 0; i < TEXTURE_END; ++i)
		m_ppTexture[i] = new LPDIRECT3DTEXTURE9[TILETEXTURE_END];

	for (_uint i = 0; i < TEXTURE_END; ++i)
	{
		for (_ulong j = 0; j < TILETEXTURE_END; ++j)
		{
			if (rhs.m_ppTexture[i][j] == nullptr)
			{
				m_ppTexture[i][j] = nullptr;
				continue;
			}

			m_ppTexture[i][j] = rhs.m_ppTexture[i][j];
			m_ppTexture[i][j]->AddRef();
		}
	}
}

HRESULT CTile::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	for (_uint i = 0; i < TEXTURE_END; ++i)
	{
		m_ppTexture[i] = new LPDIRECT3DTEXTURE9[TILETEXTURE_END];
		for (size_t j = 0; j < TILETEXTURE_END; j++)
			m_ppTexture[i][j] = nullptr;
	}

	return S_OK;
}

HRESULT CTile::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

_int CTile::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;
	
	CGameObject::Update_GameObject(_dTimeDelta);

	return NO_EVENT;
}

_int CTile::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	//Engine::Add_RenderGroup(RENDER_SHADOWMAP, this);

	return NO_EVENT;
}

void CTile::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(m_iPass);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

//void CTile::Render_GameObjectShadowMap()
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

void CTile::Render_GameObjectShadow()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowCom->Get_EffectHandle();
	pEffect->AddRef();

	m_pDevice->GetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);

	m_matWVP = m_pTransformCom->Get_WorldMatrix() * m_matView * m_matProj;
	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	m_matWLVB = m_matWLV * m_matScaleBias;

	pEffect->SetMatrix("g_matWorldViewProj", &m_matWVP);
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);
	pEffect->SetMatrix("g_matWorldLightViewProjBias", &m_matWLVB);

	D3DXMatrixInverse(&m_matWLVB, NULL, &m_pTransformCom->Get_WorldMatrix());
	D3DXVec3Transform(&m_vDir, &m_vecLightPos, &m_matWLVB);
	D3DXVec4Normalize(&m_vDir, &m_vDir);
	m_vDir.w = 0;

	pEffect->SetVector("g_vLightDir", &m_vDir);

	Engine::Get_RenderTargetTexture(pEffect, L"ShadowMap", "g_ShadowMapTexture");
	Engine::Get_RenderTargetTexture(pEffect, L"ShadowBlurY", "g_ShadowBlurTexture");
	pEffect->SetBool("g_bTerrain", true);

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_MeshesShadow(pEffect);

	pEffect->EndPass();
	pEffect->End();

	pEffect->SetBool("g_bTerrain", false);
	Safe_Release(pEffect);
}

CTile * CTile::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CTile*	pInstance = new CTile(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CTile::Clone(void * _pArg/* = nullptr*/)
{
	CTile*	pClone = new CTile(*this);

	if (FAILED(pClone->Ready_GameObject()))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CTile::Free()
{
	CGameObject::Free();

	for (size_t i = 0; i < TEXTURE_END; i++)
	{
		for (size_t j = 0; j < TILETEXTURE_END; j++)
		{
			if (m_ppTexture[i][j])
				Engine::Safe_Release(m_ppTexture[i][j]);
		}
		Engine::Safe_Delete_Array(m_ppTexture[i]);
	}
}

HRESULT CTile::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	/*/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_Tile_Cave01"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);*/
	
	/// 쉐이더 컴포넌트
	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);
	
	pComponent = m_pShaderShadowMapCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_ShadowMap"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_ShaderShadowMap", pComponent);

	//트랜스폼 컴포넌트
	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//m_pTransformCom->Set_Pos(0.f, -202.5f, 0.f);
	//m_pTransformCom->Set_Scale(_vec3(0.05f, 0.05f, 0.05f));
	///// 기타 컴포넌트
	//pComponent = m_pCalculatorCom = static_cast<CCalculator*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Calculator"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);

	return S_OK;
}

HRESULT CTile::Init_Mesh(_int iMeshIdx, _vec3 vScale, _vec3 vRot, _vec3 vPos)
{
	if (m_pMeshCom || !m_pTransformCom)
		return E_FAIL;

	//태그에 해당하는 메시컴포넌트 불러와 추가
	TCHAR szMeshName[MAX_PATH] = L"";
	wsprintf(szMeshName, L"Proto_StaticMesh_Bio_Tile_%d", iMeshIdx);

	m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(szMeshName));
	if (!m_pMeshCom)
	{
		ERR_MSG(L"CTile : MeshTag not found");
		return E_FAIL;
	}
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);

	//기본지형 텍스처와 패스 셋업(0)
	Setup_Tile(0);

	//인자로 받은 위치값 트랜스폼에 반영
	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Rotate(ROT_X, vRot.x);
	m_pTransformCom->Set_Rotate(ROT_Y, vRot.y);
	m_pTransformCom->Set_Rotate(ROT_Z, vRot.z);
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Update_Component(0);

	//피직스 처리 (장애물&지형타기 기능)
	//버텍스와 인덱스 정보 저장
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = m_pMeshCom->Get_VtxNum();
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = m_pMeshCom->Get_VtxPos();

	meshDesc.triangles.count = m_pMeshCom->Get_IndexNum();
	meshDesc.triangles.stride = m_pMeshCom->Get_dwStride();
	meshDesc.triangles.data = m_pMeshCom->Get_Index();
	meshDesc.flags = PxMeshFlag::e16_BIT_INDICES;

	// 데이터를 직렬화 시키기
	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	CPhysX_Mgr::GetInstance()->Get_Cooking()->cookTriangleMesh(meshDesc, writeBuffer, &result);
	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	// 직렬화된 데이터 읽어와서 삼각형 메쉬를 만든다.
	PxTriangleMesh* m_TriangleMesh = CPhysX_Mgr::GetInstance()->Get_Physics()->createTriangleMesh(readBuffer);

	// 잘 모르겠음 스태틱 리지드바디로 생성해서 씬에 넣어주는 것 같다.
	PxTriangleMeshGeometry triGeom;
	triGeom.triangleMesh = m_TriangleMesh;
	triGeom.scale = PxMeshScale(PxVec3(vScale.x, vScale.y, vScale.z));

	_mat matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, vRot.y, vRot.x, vRot.z);	//각 축 회전값으로 3x3매트릭스를 생성
	PxMat33 mat33(PxVec3(matRot._11, matRot._12, matRot._13)			//이때 요피치롤이 yxz 순서임에 유의
		, PxVec3(matRot._21, matRot._22, matRot._23)
		, PxVec3(matRot._31, matRot._32, matRot._33));

	PxTransform pxTransmform;
	pxTransmform.p = vPos;
	pxTransmform.q = PxQuat(mat33);		//쿼터니언 생성자 중 매트릭스를 인자로 받는 생성자 활용

	PxRigidStatic* actor = CPhysX_Mgr::GetInstance()->Get_Physics()->createRigidStatic(pxTransmform);

	PxMaterial* paxMat = CPhysX_Mgr::GetInstance()->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
	PxShape* shape = PxRigidActorExt::createExclusiveShape(*actor, triGeom, *paxMat);

	//PX_UNUSED(shape);

	CPhysX_Mgr::GetInstance()->Get_PxScene()->addActor(*actor);

	return S_OK;
}

HRESULT CTile::Init_Mesh(TCHAR * szMeshTag, _vec3 vScale, _vec3 vRot, _vec3 vPos)
{
	if (m_pMeshCom || !m_pTransformCom)
		return E_FAIL;

	//태그에 해당하는 메시컴포넌트 불러와 추가
	m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(szMeshTag));
	if (!m_pMeshCom)
	{
		ERR_MSG(L"CTile : MeshTag not found");
		return E_FAIL;
	}
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);

	//보스아레나용 텍스처와 패스 셋업(1)
	Setup_Tile(1);

	//인자로 받은 위치값 트랜스폼에 반영
	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Set_Rotate(ROT_X, vRot.x);
	m_pTransformCom->Set_Rotate(ROT_Y, vRot.y);
	m_pTransformCom->Set_Rotate(ROT_Z, vRot.z);
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Update_Component(0);

	//피직스 처리 (장애물&지형타기 기능)
	//버텍스와 인덱스 정보 저장
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = m_pMeshCom->Get_VtxNum();
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = m_pMeshCom->Get_VtxPos();

	meshDesc.triangles.count = m_pMeshCom->Get_IndexNum();
	meshDesc.triangles.stride = m_pMeshCom->Get_dwStride();
	meshDesc.triangles.data = m_pMeshCom->Get_Index();
	meshDesc.flags = PxMeshFlag::e16_BIT_INDICES;

	// 데이터를 직렬화 시키기
	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	CPhysX_Mgr::GetInstance()->Get_Cooking()->cookTriangleMesh(meshDesc, writeBuffer, &result);
	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	// 직렬화된 데이터 읽어와서 삼각형 메쉬를 만든다.
	PxTriangleMesh* m_TriangleMesh = CPhysX_Mgr::GetInstance()->Get_Physics()->createTriangleMesh(readBuffer);

	// 잘 모르겠음 스태틱 리지드바디로 생성해서 씬에 넣어주는 것 같다.
	PxTriangleMeshGeometry triGeom;
	triGeom.triangleMesh = m_TriangleMesh;
	triGeom.scale = PxMeshScale(PxVec3(vScale.x, vScale.y, vScale.z));

	_mat matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, vRot.y, vRot.x, vRot.z);	//각 축 회전값으로 3x3매트릭스를 생성
	PxMat33 mat33(PxVec3(matRot._11, matRot._12, matRot._13)			//이때 요피치롤이 yxz 순서임에 유의
		, PxVec3(matRot._21, matRot._22, matRot._23)
		, PxVec3(matRot._31, matRot._32, matRot._33));

	PxTransform pxTransmform;
	pxTransmform.p = vPos;
	pxTransmform.q = PxQuat(mat33);		//쿼터니언 생성자 중 매트릭스를 인자로 받는 생성자 활용

	PxRigidStatic* actor = CPhysX_Mgr::GetInstance()->Get_Physics()->createRigidStatic(pxTransmform);

	PxMaterial* paxMat = CPhysX_Mgr::GetInstance()->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
	PxShape* shape = PxRigidActorExt::createExclusiveShape(*actor, triGeom, *paxMat);

	//PX_UNUSED(shape);

	CPhysX_Mgr::GetInstance()->Get_PxScene()->addActor(*actor);

	return S_OK;
}

HRESULT CTile::Setup_Tile(const _uint & iPass/* = 0*/)
{
	_bool bFailed = false;

	m_iPass = iPass;


	if (0 == m_iPass)
	{
		m_fDetail = 400.f;
		m_fDetailCover = 90.f;

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/Rock/T_Rock_D.tga", &m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_COVER]))) bFailed = true;
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/Rock/T_Rock_N.tga", &m_ppTexture[TEXTURE_NORMAL][TILETEXTURE_COVER]))) bFailed = true;

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/T_BioLumRock_D.tga", &m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_SLOPE]))) bFailed = true;
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/T_BioLumRock_N.tga", &m_ppTexture[TEXTURE_NORMAL][TILETEXTURE_SLOPE]))) bFailed = true;

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/T_Cliff_1_D.tga", &m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_TOP]))) bFailed = true;
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/T_Cliff_1_N.tga", &m_ppTexture[TEXTURE_NORMAL][TILETEXTURE_TOP]))) bFailed = true;
	}
	else
	{
		m_fDetail = 80.f;
		m_fDetailCover = 18.f;

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/Graphite/T_Graphite_D.tga", &m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_COVER]))) bFailed = true;
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/Graphite/T_Graphite_ANR.tga", &m_ppTexture[TEXTURE_NORMAL][TILETEXTURE_COVER]))) bFailed = true;

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/T_BioLumRock_D.tga", &m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_SLOPE]))) bFailed = true;
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/T_BioLumRock_N.tga", &m_ppTexture[TEXTURE_NORMAL][TILETEXTURE_SLOPE]))) bFailed = true;

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/T_Cliff_1_D.tga", &m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_TOP]))) bFailed = true;
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/T_Cliff_1_N.tga", &m_ppTexture[TEXTURE_NORMAL][TILETEXTURE_TOP]))) bFailed = true;
	}
	
	if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/T_Perlin_Noise_M.tga", &m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_FILTER]))) bFailed = true;

	if (FAILED(D3DXCreateTextureFromFile(m_pDevice, L"../Resources/Texture/Terrain/T_Perlin_Noise_M.tga", &m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_NOISE]))) bFailed = true;

	if (bFailed)
	{
		ERR_MSG(L"CTile::Ready_GameObject_Prototype() : Failed to Create TerrainTexture");
	}

	return S_OK;
}

HRESULT CTile::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	Set_ConstantTable_Normal(_pEffect, m_pTransformCom->Get_WorldMatrix());

	_pEffect->SetTexture("g_DiffuseTextureSlope", m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_SLOPE]);
	_pEffect->SetTexture("g_NormalTextureSlope", m_ppTexture[TEXTURE_NORMAL][TILETEXTURE_SLOPE]);

	_pEffect->SetTexture("g_DiffuseTextureCover", m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_COVER]);
	_pEffect->SetTexture("g_NormalTextureCover", m_ppTexture[TEXTURE_NORMAL][TILETEXTURE_COVER]);

	_pEffect->SetTexture("g_DiffuseTextureTop", m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_TOP]);
	_pEffect->SetTexture("g_NormalTextureTop", m_ppTexture[TEXTURE_NORMAL][TILETEXTURE_TOP]);

	_pEffect->SetTexture("g_FilterTexture", m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_FILTER]);
	_pEffect->SetTexture("g_NoiseTexture", m_ppTexture[TEXTURE_DIFFUSE][TILETEXTURE_NOISE]);

	// 디테일맵
	_pEffect->SetFloat("g_fDetail", m_fDetail);
	_pEffect->SetFloat("g_fDetailCover", m_fDetailCover);
	_pEffect->SetBool("m_bTerrain", true);

	_pEffect->CommitChanges();

	return S_OK;
}
