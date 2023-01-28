#include "..\Headers\Scene.h"
#include "Frustum.h"

USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
{
	m_pDevice->AddRef();
}

HRESULT CScene::Ready_Scene(void)
{
	return S_OK;
}

_int CScene::Update_Scene(const _double & dTimeDelta)
{
	_int	iResult = 0;

	for (auto& iter : m_mapLayer)
	{
		iResult = iter.second->Update_Layer(dTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
		else if (1 == iResult)
		{
			// ������Ʈ ����

		}
		else if (0 != iResult)	// ��Ÿ �̺�Ʈ
			return iResult;
	}

	CFrustum::GetInstance()->Update_Frustum();
	CFrustum::GetInstance()->Update_LightFrustum_Ortho();

	for (auto& iter : m_mapLayer)
	{
		iResult = iter.second->LateUpdate_Layer(dTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CScene::Render_Scene(void)
{
}

CLayer * CScene::Get_Layer(const wstring & _wstrGameLayerTag)
{
	auto pair = m_mapLayer.find(_wstrGameLayerTag);

	if (pair == m_mapLayer.end())
		return nullptr;

	return pair->second;
}

CGameObject * CScene::Get_GameObject(const wstring & _wstrLayerTag, const _int& _iObjIdx)
{
	auto pair = m_mapLayer.find(_wstrLayerTag);

	if (pair == m_mapLayer.end())
		return nullptr;

	return pair->second->Get_GameObject(_iObjIdx);
}

CComponent * CScene::Get_Component(const wstring & _wstrLayerTag, const _int& _iObjIdx, const wstring & _wstrComponentTag, COMPONENTID eID)
{
	auto pair = m_mapLayer.find(_wstrLayerTag);

	if (pair == m_mapLayer.end())
		return nullptr;

	return pair->second->Get_Component(_iObjIdx, _wstrComponentTag, eID);
}

HRESULT CScene::Add_GameObjectInLayer(const wstring & _wstrGameLayerTag, CGameObject * _pGameObject)
{
	// ���̾� Ž��, ã�� ���̾ ���� ��� ����
	auto pair = m_mapLayer.find(_wstrGameLayerTag);

	if(pair == m_mapLayer.end())
		return E_FAIL;

	// ���� ������Ʈ �߰�
	return pair->second->Add_GameObject(_pGameObject);
}

HRESULT CScene::Swap_GameObjectInLayer(const wstring & _wstrPreGameLayerTag, const wstring & _wstrToGameLayerTag, _uint _iIndex)
{
	/// �ӽ� ������Ʈ ���̾� ���� �Լ� (�ε����� ã��)

	// ���� ���̾��� �ش� �ε��� ������Ʈ ��������
	if(_iIndex >= m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->size())
		return E_FAIL;

	CGameObject* pGameObject = m_mapLayer[_wstrPreGameLayerTag]->Get_GameObject(_iIndex);

	if (nullptr == pGameObject)
		return E_FAIL;

	// ���� ���̾���� ����Ʈ ���� ����
	auto& iter = m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->begin();
	for(_uint i = 0; i < _iIndex; ++i)
		++iter;
	m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->erase(iter);

	// �̵��� ���̾ ������ ������Ʈ �߰�
	m_mapLayer[_wstrToGameLayerTag]->Add_GameObject(pGameObject);

	return S_OK;
}

HRESULT CScene::Swap_GameObjectInLayer(const wstring & _wstrPreGameLayerTag, const wstring & _wstrToGameLayerTag, CGameObject * _pGameObject)
{
	/// �ӽ� ������Ʈ ���̾� ���� �Լ� (���ӿ�����Ʈ�� ã��)

	if (nullptr == _pGameObject)
		return E_FAIL;

	// �ش� ���� ������Ʈ ���� ���̾�� ����
	auto iter = m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->begin();
	for (; iter != m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->end(); ++iter)
	{
		if (*iter == _pGameObject)
		{
			iter = m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->erase(iter);

			// �� ���̾ �߰�
			m_mapLayer[_wstrToGameLayerTag]->Add_GameObject(_pGameObject);

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CScene::RayCast_OnMesh(_vec3 * pOut, const _vec3 & vRayPos, const _vec3 & vRayDir, const wstring & _wstrGameLayerTag)
{
	// ���̾� Ž��, ã�� ���̾ ���� ��� ����
	auto pair = m_mapLayer.find(_wstrGameLayerTag);

	if (pair == m_mapLayer.end())
		return E_FAIL;

	// ����ĳ��Ʈ
	return pair->second->RayCast_OnMesh(pOut, vRayPos, vRayDir);
}

list<CGameObject*>* CScene::Get_LayerList(const wstring & _wstrGameLayerTag)
{
	auto pair = m_mapLayer.find(_wstrGameLayerTag);

	if (pair == m_mapLayer.end())
		return nullptr;

	return pair->second->Get_ListGameObject();
}

void CScene::Set_ShadowLight(_vec3 _vLightPos, _vec3 _vLightAt)
{
	m_sShadowLight.vLightPos = _vLightPos;
	m_sShadowLight.vLightAt = _vLightAt;
}

const SHADOWLIGHT & CScene::Get_ShadowLight()
{
	return m_sShadowLight;
}

void CScene::Set_ShadowOnOff(_bool _bShadow)
{
	m_bShadow = _bShadow;
}

const _bool & CScene::Get_ShadowOnOff()
{
	return m_bShadow;
}

void CScene::Free()
{
	for(auto& pair : m_mapLayer)
		Safe_Release(pair.second);
	m_mapLayer.clear();

	Safe_Release(m_pDevice);
}
