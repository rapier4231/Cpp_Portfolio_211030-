#include "..\Headers\Layer.h"
#include "Transform.h"
#include "StaticMesh.h"

USING(Engine)

CLayer::CLayer()
{
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _double & _dTimeDelta)
{
	_int	iResult = 0;

	//// Update
	//for (auto& iter : m_listGameObject)
	//{
	//	iResult = iter.second->Update_GameObject(_dTimeDelta);
	//	if (iResult & 0x80000000)		// iResult �� �������� üũ
	//		return iResult;
	//}

	auto iter = m_listGameObject.begin();

	for (; iter != m_listGameObject.end(); )
	{
		iResult = (*iter)->Update_GameObject(_dTimeDelta);

		if (iResult == OBJECT_DEAD)
		{
			Safe_Release(*iter);
			iter = m_listGameObject.erase(iter);
			size_t iSize = m_listGameObject.size();
			iSize = 0;
		}
		else if (iResult == DINO_TAMED)
			return DINO_TAMED;
		else
			iter++;
	}

	return iResult;
}

_int CLayer::LateUpdate_Layer(const _double & _dTimeDelta)
{
	_int	iResult = 0;

	// LateUpdate
	for (auto& pObj : m_listGameObject)
	{
		iResult = pObj->LateUpdate_GameObject(_dTimeDelta);
		if (iResult & 0x80000000)		// iResult �� �������� üũ
			return iResult;
	}

	return iResult;
}

void CLayer::Render_Layer()
{
	for (auto& pObj : m_listGameObject)
		pObj->Render_GameObject();
}

HRESULT CLayer::Add_GameObject(CGameObject* _pGameObject)
{
	if (nullptr == _pGameObject)
		return E_FAIL;

	m_listGameObject.emplace_back(_pGameObject);

	return S_OK;
}

HRESULT CLayer::Delete_GameObject(CGameObject* _pGameObject)
{
	auto iter = m_listGameObject.begin();
	for (; iter != m_listGameObject.end(); iter++)
	{
		if (*iter == _pGameObject)
		{
			Safe_Release(*iter);
			iter = m_listGameObject.erase(iter);
			return S_OK;
		}
	}
	return S_FALSE;
}

HRESULT CLayer::Delete_GameObject(const _int & iIdx)
{
	if (m_listGameObject.size() <= iIdx)
		return E_FAIL;
	auto iter = m_listGameObject.begin();
	for (_int i = 0; i < iIdx; i++)
	{
		if (++iter == m_listGameObject.end())
			return S_FALSE;
	}

	Safe_Release(*iter);
	iter = m_listGameObject.erase(iter);
	return S_OK;
}

CGameObject * CLayer::Get_GameObject(const _int& iIdx)
{
	if (m_listGameObject.size () <= iIdx)
		return nullptr;

	auto iter = m_listGameObject.begin();
	for (_int i = 0; i < iIdx; i++)
	{
		if (++iter == m_listGameObject.end())
			return nullptr;
	}
	return *iter;
}

CComponent * CLayer::Get_Component(const _int & iIdx, const wstring & _wstrComponentTag, COMPONENTID eID)
{
	if (m_listGameObject.size() <= iIdx)
		return nullptr;
	auto iter = m_listGameObject.begin();
	for (_int i = 0; i < iIdx; i++)
	{
		if (++iter == m_listGameObject.end())
			return nullptr;
	}

	return (*iter)->Get_Component(_wstrComponentTag, eID);
}

HRESULT CLayer::RayCast_OnMesh(_vec3 * pOut, const _vec3 & vRayPos, const _vec3 & vRayDir)
{
	HRESULT hr = S_FALSE;
	CStaticMesh* pMeshCom = nullptr;
	CTransform* pTransformCom = nullptr;
	_mat matWorld;
	_float fDistance = -1.f;
	_vec3 vCollisionPoint;

	for (auto pObj : m_listGameObject)
	{
		//�� ������Ʈ�� ���� ����� ���
		pTransformCom = (CTransform*)pObj->Get_Component(L"Com_Transform", ID_DYNAMIC);
		if (!pTransformCom)
			continue;
		matWorld = pTransformCom->Get_WorldMatrix();
		
		////���� -> ����
		//D3DXMatrixInverse(&matWorld, 0, &pTransformCom->Get_WorldMatrix());
		//D3DXVec3TransformCoord(&vRayLocalPos, &vRayPos, &matWorld);
		//D3DXVec3TransformNormal(&vRayLocalDir, &vRayDir, &matWorld);

		//�� ������Ʈ�� �޽ÿ� �浹 �õ� (�Լ� ���ο��� ���� -> ���� ��ȯ���� �浹, ������� ���� ��ȯ)
		pMeshCom = static_cast<CStaticMesh*>(pObj->Get_Component(L"Com_Mesh", ID_STATIC));
		hr = pMeshCom->RayCast_OnMesh(&vCollisionPoint, vRayPos, vRayDir, matWorld);
		if (S_OK == hr)
		{
			//�浹 ���⿡ �������� ��
			if (fDistance < 0.f)
			{
				//ù ���̴� �״�� ���
				fDistance = D3DXVec3Length(&(vRayPos - vCollisionPoint));
				*pOut = vCollisionPoint;
			}
			else
			{
				//�� ��°���ʹ� ���̸� ���� �� ª�� ��츸 ���
				float fDistTemp = D3DXVec3Length(&(vRayPos - vCollisionPoint));
				if (fDistTemp < fDistance)
				{
					fDistance = fDistTemp;
					*pOut = vCollisionPoint;
				}
			}
		}
		//���̾� ���� ��� ������Ʈ ��ȸ
	}

	if (0.f < fDistance)
		//�� ���̶� �浹 ���⿡ ������ ���
		return S_OK;
	else
		//�浹 ���⿡ ������ ��� ���ʹ� �ʱ�ȭ�ϰ� E_FAIL ��ȯ
		*pOut = _vec3(0.f, 0.f, 0.f);

	return E_FAIL;
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pObj : m_listGameObject)
		Safe_Release(pObj);

	m_listGameObject.clear();
}
