#include "stdafx.h"

#include "SkyDome.h"

#include "Export_Function.h"
//#include "StaticMesh.h"
//#include "Shader.h"

USING(Engine)

CSkyDome::CSkyDome(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	D3DXMatrixIdentity(&m_matWorld);
}

CSkyDome::CSkyDome(const CSkyDome & rhs)
	: CGameObject(rhs)
	, m_matWorld(rhs.m_matWorld)
	, m_fPosYAdj(rhs.m_fPosYAdj)
{
}

HRESULT CSkyDome::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CSkyDome::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fPosYAdj = 6.f;

	//_vec3 vScale = _vec3(0.009f, 0.009f, 0.009);	//보스룸 사이즈... 깊이포그에 가리는
	_vec3 vScale = _vec3(0.004f, 0.004f, 0.004f);	//일반맵 사이즈
	m_pTransformCom->Set_Scale(vScale);

	return S_OK;
}

_int CSkyDome::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;
	
	CGameObject::Update_GameObject(_dTimeDelta);
	return NO_EVENT;
}

_int CSkyDome::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	_mat matWorld, matCamera;

	//월드 초기화
	//_vec3 vScale = *m_pTransformCom->Get_Scale();
	//D3DXMatrixScaling(&matWorld, vScale.x, vScale.y, vScale.z);
	
	/*CTransform* pTransform = (CTransform*)Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC);
	_mat matPlayer = pTransform->Get_WorldMatrix();
	_mat matWorld;
	D3DXMatrixScaling(&matWorld, 0.01f, 0.01f, 0.01f);
	matWorld._41 = matPlayer._41;
	matWorld._42 = matPlayer._42;
	matWorld._43 = matPlayer._43;
	//m_pTransformCom->Set_Pos(_vec3(matWorld._41, matWorld._42, matWorld._43));
	//m_pTransformCom->Update_Component(0);
	memcpy(&matWorld._41, &matPlayer._41, sizeof(_vec3));
	*/

	matWorld = m_pTransformCom->Get_WorldMatrix();

	//카메라행렬 얻기
	m_pDevice->GetTransform(D3DTS_VIEW, &matCamera);
	D3DXMatrixInverse(&matCamera, 0, &matCamera);

	//카메라행렬의 위치값 내 월드로 복사
	memcpy(&matWorld._41, &matCamera._41, sizeof(_vec3));
	matWorld._42 += m_fPosYAdj;	//Y보정값 더해줌

	//완성된 행렬 트랜스폼에 세팅
	m_pTransformCom->Set_WorldMatrix(&matWorld);

	Engine::Add_RenderGroup(RENDER_PRIORITY, this);

	return NO_EVENT;
}

void CSkyDome::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect, false, 7);

	pEffect->End();

	Engine::Safe_Release(pEffect);
}

CSkyDome * CSkyDome::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CSkyDome*	pInstance = new CSkyDome(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CSkyDome::Clone(void * _pArg/* = nullptr*/)
{
	CSkyDome*	pClone = new CSkyDome(*this);

	if (FAILED(pClone->Ready_GameObject()))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CSkyDome::Free()
{
	CGameObject::Free();
}

HRESULT CSkyDome::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 메시 컴포넌트
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_SkyDome"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// 셰이더 컴포넌트
	//pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_SkyDome"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Normal_Shadow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	//트랜스폼 컴포넌트
	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

HRESULT CSkyDome::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat matWorld, matView, matProj;

	// 행렬
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);
	_pEffect->SetVector("g_vSetColor", &_vec4(m_vColor, 1.f));

	_pEffect->CommitChanges();

	return S_OK;
}
