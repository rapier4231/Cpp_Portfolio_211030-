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
			// 오브젝트 데드

		}
		else if (0 != iResult)	// 기타 이벤트
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
	// 레이어 탐색, 찾는 레이어가 없을 경우 리턴
	auto pair = m_mapLayer.find(_wstrGameLayerTag);

	if(pair == m_mapLayer.end())
		return E_FAIL;

	// 게임 오브젝트 추가
	return pair->second->Add_GameObject(_pGameObject);
}

HRESULT CScene::Swap_GameObjectInLayer(const wstring & _wstrPreGameLayerTag, const wstring & _wstrToGameLayerTag, _uint _iIndex)
{
	/// 임시 오브젝트 레이어 스왑 함수 (인덱스로 찾기)

	// 이전 레이어의 해당 인덱스 오브젝트 가져오기
	if(_iIndex >= m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->size())
		return E_FAIL;

	CGameObject* pGameObject = m_mapLayer[_wstrPreGameLayerTag]->Get_GameObject(_iIndex);

	if (nullptr == pGameObject)
		return E_FAIL;

	// 이전 레이어에서의 리스트 원소 삭제
	auto& iter = m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->begin();
	for(_uint i = 0; i < _iIndex; ++i)
		++iter;
	m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->erase(iter);

	// 이동할 레이어에 가져온 오브젝트 추가
	m_mapLayer[_wstrToGameLayerTag]->Add_GameObject(pGameObject);

	return S_OK;
}

HRESULT CScene::Swap_GameObjectInLayer(const wstring & _wstrPreGameLayerTag, const wstring & _wstrToGameLayerTag, CGameObject * _pGameObject)
{
	/// 임시 오브젝트 레이어 스왑 함수 (게임오브젝트로 찾기)

	if (nullptr == _pGameObject)
		return E_FAIL;

	// 해당 게임 오브젝트 이전 레이어에서 삭제
	auto iter = m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->begin();
	for (; iter != m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->end(); ++iter)
	{
		if (*iter == _pGameObject)
		{
			iter = m_mapLayer[_wstrPreGameLayerTag]->Get_ListGameObject()->erase(iter);

			// 새 레이어에 추가
			m_mapLayer[_wstrToGameLayerTag]->Add_GameObject(_pGameObject);

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CScene::RayCast_OnMesh(_vec3 * pOut, const _vec3 & vRayPos, const _vec3 & vRayDir, const wstring & _wstrGameLayerTag)
{
	// 레이어 탐색, 찾는 레이어가 없을 경우 리턴
	auto pair = m_mapLayer.find(_wstrGameLayerTag);

	if (pair == m_mapLayer.end())
		return E_FAIL;

	// 레이캐스트
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
