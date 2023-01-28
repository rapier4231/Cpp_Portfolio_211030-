#include "stdafx.h"
#include "..\Headers\EffectObjectRev.h"

#include "Export_Function.h"

USING(Engine)


CEffectObjectRev::CEffectObjectRev(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CEffectObjectRev::CEffectObjectRev(const CEffectObjectRev & _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CEffectObjectRev::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CEffectObjectRev::Ready_GameObject(void * _pArg)
{
	CGameObject::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vColor = { 1.f, 1.f, 1.f };
	m_vTexUV = { 0.f, 0.1f };
	m_dCount = 0.f;
	m_dTime = 0.f;
	m_iTextureX = 1;
	m_iTextureY = 1;
	m_fScale = 0.05f;
	m_fAlpha = 1.f;

	m_pTransformCom->Set_Scale(_vec3(m_fScale, 0.001f - m_fScale * 2.f, m_fScale));

	m_iCount = 0;

	return S_OK;
}

_int CEffectObjectRev::Update_GameObject(const _double & _dTimeDelta)
{
	if (0.f > m_fScale)
	{
		m_fScale = 0.05f;
		m_fAlpha = 1.f;
		m_iCount++;
	}

	m_fScale -= 0.045f * (_float)_dTimeDelta;
	m_fAlpha -= 1.1f * (_float)_dTimeDelta;

	//if (m_bDead)
	if (1 < m_iCount)
		return OBJECT_DEAD;

	//if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
	//{
	//	m_fScale -= 0.01f * _dTimeDelta;
	//	m_fAlpha += 0.8f * _dTimeDelta;
	//}

	//if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	//{
	//	m_fScale += 0.01f * _dTimeDelta;
	//	m_fAlpha -= 0.8f * _dTimeDelta;
	//}

	m_pTransformCom->Set_Scale(_vec3(m_fScale, 0.001f - m_fScale * 2.f, m_fScale));
	m_pTransformCom->Set_AccRotate(ROT_Y, (_float)_dTimeDelta * 3.f);

	if (m_pTarget != nullptr)
	{
		_vec3 vTargetPos = *static_cast<CTransform*>(m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos() + _vec3(0.f, 1.f, 0.f);
		m_pTransformCom->Set_Pos(vTargetPos);
	}

	CGameObject::Update_GameObject(_dTimeDelta);

	Compute_ViewZ(m_pTransformCom->Get_Pos());

	return NO_EVENT;
}

_int CEffectObjectRev::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_EFFECT, this);

	return NO_EVENT;
}

void CEffectObjectRev::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect, false, 4);

	pEffect->End();

	Safe_Release(pEffect);
}

CEffectObjectRev * CEffectObjectRev::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CEffectObjectRev*	pInstance = new CEffectObjectRev(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CEffectObjectRev::Clone(void * _pArg/* = nullptr*/)
{
	CEffectObjectRev*	pClone = new CEffectObjectRev(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Engine::Safe_Release(pClone);

	return pClone;
}

HRESULT CEffectObjectRev::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_EffectObject"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

HRESULT CEffectObjectRev::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat	matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);

	_pEffect->SetFloat("g_fAlpha", m_fAlpha);
	_pEffect->SetInt("g_iTextureX", m_iTextureX);
	_pEffect->SetInt("g_iTextureY", m_iTextureY);
	_pEffect->SetValue("g_vObjColor", &m_vColor, sizeof(_vec3));
	//_pEffect->SetValue("g_vTexUV", &m_vecTexUV[(_uint)m_dCount], sizeof(_vec2));
	_pEffect->SetValue("g_vTexUV", &m_vTexUV, sizeof(_vec2));

	Engine::Get_RenderTargetTexture(_pEffect, L"Depth", "g_DepthTexture");

	return S_OK;

	return S_OK;
}

void CEffectObjectRev::Free()
{
	CGameObject::Free();
}
