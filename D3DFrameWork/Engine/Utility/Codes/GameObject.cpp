#include "..\Headers\GameObject.h"
#include "Camera.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice),
	m_bDead(false),
	m_bInit(false),
	m_bIsClone(false),
	m_vColor({ 1.f, 1.f, 1.f })
{
	m_pDevice->AddRef();
}

CGameObject::CGameObject(const CGameObject & _rhs)
	: m_pDevice(_rhs.m_pDevice),
	m_bDead(_rhs.m_bDead),
	m_bInit(_rhs.m_bInit),
	m_bIsClone(true),
	m_vColor(_rhs.m_vColor)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		m_mapComponent[i] = _rhs.m_mapComponent[i];
		for (auto& iter : m_mapComponent[i])
		{
			if (nullptr != iter.second)
				iter.second->AddRef();
		}
	}

	m_pDevice->AddRef();
}

HRESULT CGameObject::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Ready_GameObject(void* _pArg/* = nullptr*/)
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _double & _dTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(_dTimeDelta);

	return NO_EVENT;
}

void CGameObject::Render_GameObject()
{
}

void CGameObject::Set_ShadowMap(_vec3 _vLightPos, _vec3 _vLightAt)
{
	m_vecLightPos = _vLightPos;
	m_vecLightAt = _vLightAt;

	//if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
	//	m_fPosX -= 1.f;
	//if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
	//	m_fPosX += 1.f;
	//if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
	//	m_fPosY -= 1.f;
	//if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
	//	m_fPosY += 1.f;
	//if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
	//	m_fPosZ -= 1.f;
	//if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
	//	m_fPosZ += 1.f;

	//m_vecLightPos = _vec3(-400.f + m_fPosX, 100.f + m_fPosY, 600.f + m_fPosZ);
	//m_vecLightAt = m_vecLightPos + _vec3(1.f, -10.f, -15.f);
	m_vecLightUp = _vec3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&m_matLightView, &m_vecLightPos, &m_vecLightAt, &m_vecLightUp);
	D3DXMatrixOrthoLH(&m_matLightProj, 100.f, 100.f, 0.f, 10000.f);

	float fOffsetX = 0.5f + (0.5f / SHADOW_WIDTH);
	float fOffsetY = 0.5f + (0.5f / SHADOW_HEIGHT);
	m_matScaleBias = _mat(0.5f, 0.0f, 0.0f, 0.0f,
							0.0f, -0.5f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f,
							fOffsetX, fOffsetY, 0.0f, 1.0f);
}

void CGameObject::Compute_ViewZ(const _vec3 * pPos)
{
	_mat			matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3		vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}

void CGameObject::Set_Color(const _float & _fX, const _float & _fY, const _float & _fZ)
{
	m_vColor = { _fX, _fY, _fZ };
}

_vec3 CGameObject::Get_Color()
{
	return m_vColor;
}

CComponent * CGameObject::Get_Component(const wstring & _strComponentTag, COMPONENTID _eComponentID)
{
	auto iter = m_mapComponent[_eComponentID].find(_strComponentTag);

	if (iter == m_mapComponent[_eComponentID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		int iTest = 0;
		for (auto& iter : m_mapComponent[i])
		{
			if (NULL != iter.second)
				++iTest;
		}
		if (iTest)
			iTest = 0;	//여기는 Clone()할때 주소값복사로 AddRef()된 컴포넌트가 있으면 양수뜰수있음
		m_mapComponent[i].clear();
	}
	//for (_uint i = 0; i < ID_END; ++i)
	//{
	//	for (auto& pair : m_mapComponent[i])
	//		Safe_Release(pair.second);
	//	m_mapComponent[i].clear();
	//}
	Safe_Release(m_pDevice);
}
