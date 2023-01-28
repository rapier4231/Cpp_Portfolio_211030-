#include "SphereCollider.h"

#include "Export_Function.h"

USING(Engine)

Engine::CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

Engine::CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
	: CComponent(rhs)
{
	for (int i = 0; i < COL_END; ++i)
		m_listCollider[i] = rhs.m_listCollider[i];
}

Engine::CSphereCollider::~CSphereCollider(void)
{

}

HRESULT Engine::CSphereCollider::Ready_SphereCollider(wstring strBoneName, CGameObject* pObject, COL_TYPE eType, _vec3 vPos, _float fRad, COL_DIR _eColDir/* = COL_DIR_END*/)
{
	SPHEREINFO tInfo{};
	ZeroMemory(&tInfo, sizeof(SPHEREINFO));
	tInfo.fRadius = fRad;
	tInfo.strBoneName = strBoneName;
	tInfo.eColType = eType;			// 건축용
	tInfo.eColDir = _eColDir;		// 건축용

	CComponent* pComponent = pObject->Get_Component(L"Com_Mesh", ID_STATIC);
	if (nullptr == pComponent)
		return E_FAIL;
	tInfo.pTartgetMesh = static_cast<CDynamicMesh*>(pComponent);

	tInfo.vSpherePos = vPos;
	pComponent = pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	if (nullptr == pComponent)
		return E_FAIL;

	tInfo.pTargetWorldMatix = &static_cast<CTransform*>(pComponent)->Get_WorldMatrix();

	D3DXCreateSphere(m_pDevice, 1.f, 10, 10, &tInfo.pSphere, nullptr);

	m_listCollider[eType].emplace_back(tInfo);

	Update_Component(0.f);

	return S_OK;
}

void CSphereCollider::Render_SphereCollider()
{
#ifdef _DEBUG
	//if (Engine::Get_DIKeyDown(DIK_M))
	//	m_bIsRender = !m_bIsRender;

 	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
 	{
		CShader*	pShader = static_cast<CShader*>(Clone_ComponentPrototype(L"Proto_Shader_Collider"));
		NULL_CHECK(pShader);

		LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
		pEffect->AddRef();

		_mat matView, matProj;
		m_pDevice->GetTransform(D3DTS_VIEW, &matView);
		m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
		pEffect->SetMatrix("g_matProj", &matProj);
		pEffect->SetMatrix("g_matView", &matView);

		for (int i = 0; i < COL_END; ++i)
		{
			for (auto& data : m_listCollider[i])
			{
				if (data.pSphere)
				{
					pEffect->SetMatrix("g_matWorld", &data.matColMatrix);

					pEffect->Begin(0, 0);
					pEffect->BeginPass(i);

					data.pSphere->DrawSubset(0);

					pEffect->EndPass();
					pEffect->End();
				}
			}
		}

		Safe_Release(pEffect);
		Safe_Release(pShader);
	}
#endif // _DEBUG
}

_int CSphereCollider::Update_Component(const _double& _dTimeDelta)
{
	for (int i = 0; i < COL_END; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			if (data.strBoneName == L"")
			{
				D3DXMatrixScaling(&data.matColMatrix, data.fRadius, data.fRadius, data.fRadius);

				_mat matTrans;
				D3DXMatrixTranslation(&matTrans, data.vSpherePos.x, data.vSpherePos.y, data.vSpherePos.z);

				data.matColMatrix *= matTrans;


				if (COL_BUILD_PIVOT == data.eColType && data.pTargetWorldMatix)
				{
					_mat matParent = *data.pTargetWorldMatix;
					matParent._11 =
					matParent._22 =
					matParent._33 = data.fRadius;
				
					data.matColMatrix *= matParent;
				
				}
			}
			else
			{
				if (data.pParentBoneMatrix == nullptr)
				{
					char szBuff[MAX_PATH];
					WideCharToMultiByte(CP_ACP, 0, data.strBoneName.c_str(), -1, szBuff, MAX_PATH, 0, 0);
					const D3DXFRAME_DERIVED* pFrame = data.pTartgetMesh->Get_FrameByName(szBuff);

					data.pParentBoneMatrix = &pFrame->CombinedTransformMatrix;
					data.pParentWorldMatrix = data.pTargetWorldMatix;

					data.matColMatrix = *data.pParentBoneMatrix * *data.pParentWorldMatrix;
				}
				else
				{
					D3DXMatrixIdentity(&data.matColMatrix);

					_mat matWorld, matScale, matTrans;
					D3DXMatrixScaling(&matScale, data.fRadius, data.fRadius, data.fRadius);
					D3DXMatrixTranslation(&matTrans, data.vSpherePos.x, data.vSpherePos.y, data.vSpherePos.z);

					data.matColMatrix = matScale * matTrans;
					data.matColMatrix *= *data.pParentBoneMatrix * *data.pParentWorldMatrix;
				}
			}
		}
	}

	return _int();
}

HRESULT CSphereCollider::Add_SphereCollider(const wstring& strBoneName, CGameObject * pObject, COL_TYPE eType, _vec3 vPos, _float fRadius)
{
	SPHEREINFO* tInfo = Get_SphereInfo(strBoneName);

	if (tInfo)
		return E_FAIL;

	if (FAILED(Ready_SphereCollider(strBoneName, pObject, eType, vPos, fRadius)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSphereCollider::Add_SphereCollider(SPHEREINFO* tInfo, CGameObject* pObject)
{
	if (FAILED(Ready_SphereCollider(L"", pObject, tInfo->eColType, tInfo->vSpherePos, tInfo->fRadius, tInfo->eColDir)))
		return E_FAIL;

	return S_OK;
}

void CSphereCollider::Delete_SphereCollider(wstring strBoneName)
{
	for (_int i = 0; i < COL_END; ++i)
	{
		auto iter = m_listCollider[i].begin();
		for (; iter != m_listCollider[i].end();)
		{
			if (iter->strBoneName == strBoneName)
			{
				Safe_Release(iter->pSphere);
				iter = m_listCollider->erase(iter);
			}
			else
				iter++;
		}
	}
}

void CSphereCollider::Delete_AllCollider()
{
	for (_int i = 0; i < COL_END; ++i)
	{
		auto iter = m_listCollider[i].begin();
		for (; iter != m_listCollider[i].end();)
		{
			Safe_Release(iter->pSphere);
		}
	}

	m_listCollider->clear();
}

Engine::CSphereCollider::SPHEREINFO*  CSphereCollider::Get_SphereInfo(wstring strBoneName, _int* iColType)
{
	if (m_listCollider[COL_HIT].empty() && m_listCollider[COL_ATTACK].empty() && m_listCollider[COL_INTERACTION].empty())
		return nullptr;

	for (_int i = 0; i < COL_END; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			if (data.strBoneName == strBoneName)
			{
				if (iColType)
					*iColType = i;

				return &data;
			}
		}
	}

	return nullptr;
}

void CSphereCollider::Get_BoneName(list<wstring>* pBoneList)
{
	for (int i = 0; i < COL_END; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			pBoneList->emplace_back(data.strBoneName);
		}
	}
}

void CSphereCollider::Set_Radius(wstring strBoneName, _float fRad)
{
	for (int i = 0; i < COL_END; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			if (data.strBoneName == strBoneName)
			{
				data.fRadius = fRad;
				return;
			}
		}
	}
}

void CSphereCollider::Set_Radius(_float fRad)
{
	for (int i = 0; i < COL_END; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			if (data.pSphere)
			{
				data.fRadius = fRad;
				return;
			}
		}
	}
}

void CSphereCollider::Set_Pos(wstring strBoneName, _vec3 & vPos)
{
	for (int i = 0; i < COL_END; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			if (data.strBoneName == strBoneName)
			{
				data.vSpherePos = vPos;
				return;
			}
		}
	}

}

void CSphereCollider::Set_Pos(_vec3 & vPos)
{
	// 뼈에 추가안하는 애들용
	for (int i = 0; i < COL_BUILD_PIVOT; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			if (data.pSphere)
			{
				data.vSpherePos = vPos;
				break;
			}
		}
	}
}

_float * CSphereCollider::Get_Radius()
{
	for (int i = 0; i < COL_END; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			if (data.pSphere)
				return &data.fRadius;
		}
	}

	return nullptr;
}

_vec3 * CSphereCollider::Get_Pos()
{
	// 뼈에 추가안하는 애들용, 하나만 있는 애들
	for (int i = 0; i < COL_END; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			if (data.pSphere)
				return &data.vSpherePos;
		}
	}

	return nullptr;
}

_float CSphereCollider::Get_InteractRadius()
{
	if (0 < m_listCollider[COL_INTERACTION].size())
		return m_listCollider[COL_INTERACTION].front().fRadius;
	else
	{
		ERR_MSG(L"No InteractionCollider Found");
		return -1.f;
	}
}

_vec3 CSphereCollider::Get_InteractPos()
{
	if (0 < m_listCollider[COL_INTERACTION].size())
		return m_listCollider[COL_INTERACTION].front().vSpherePos;
	else
	{
		ERR_MSG(L"No InteractionCollider Found");
		return _vec3(0.f, 0.f, 0.f);
	}
}

void CSphereCollider::Set_Head(wstring strBoneName, _bool bHead)
{
	for (int i = 0; i < COL_END; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			if (data.strBoneName == strBoneName)
			{
				data.bHead = bHead;
				return;
			}
		}
	}
}

CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphereCollider*	pInstance = new CSphereCollider(pGraphicDev);

	return pInstance;
}

Engine::CSphereCollider* Engine::CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring strBoneName, CGameObject* pObject, COL_TYPE eType, _vec3 vPos, _float fRadius)
{
	CSphereCollider*	pInstance = new CSphereCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_SphereCollider(strBoneName, pObject, eType, vPos, fRadius)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CSphereCollider::Free(void)
{
	for (int i = 0; i < COL_END; ++i)
	{
		for (auto& data : m_listCollider[i])
		{
			if (data.pSphere)
				Safe_Release(data.pSphere);
		}
	}

	CComponent::Free();
}

HRESULT CSphereCollider::Ready_Component_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CSphereCollider::Ready_Component(void * _pArg)
{
	SPHEREINFO* pInfo = (SPHEREINFO*)_pArg;
	pInfo->strBoneName = L"";

	D3DXCreateSphere(m_pDevice, 1.f, 10, 10, &pInfo->pSphere, nullptr);

	m_listCollider[pInfo->eColType].emplace_back(*pInfo);

	return S_OK;
}

Engine::CComponent* Engine::CSphereCollider::Clone(void * pArg)
{
	CSphereCollider* pClone = new CSphereCollider(*this);
	
	if (pArg)
	{
		if (FAILED(pClone->Ready_Component(pArg)))
			Safe_Release(pClone);
	}

	return pClone;
}

