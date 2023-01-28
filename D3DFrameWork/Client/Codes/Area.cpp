#include "stdafx.h"

#include "Area.h"

#include "Dino.h"
#include "RandomFunc.h"
#include "Export_Function.h"

USING(Engine)

CArea::CArea(LPDIRECT3DDEVICE9 _pDevice)
	: CUnit(_pDevice)
{
	//ZeroMemory(Get_Pos(), sizeof(_vec3));
}

CArea::CArea(const CArea & rhs)
	: CUnit(rhs)
	, m_fRadius(rhs.m_fRadius)
	, m_fRadiusReady(rhs.m_fRadiusReady)
	, m_fRadiusWait(rhs.m_fRadiusWait)
	, m_fAlpha(rhs.m_fAlpha)
	, m_pPlayer(nullptr)
	, m_fDistance(rhs.m_fDistance)
	, m_bPlayerSteppedIn(rhs.m_bPlayerSteppedIn)
	, m_iDinoCountMax(rhs.m_iDinoCountMax)
	, m_dTimeSpawn(rhs.m_dTimeSpawn)
{
	if (0 < m_listDino.size())
	{
		m_listDino.clear();
		list<CDino*>().swap(m_listDino);
	}
}

HRESULT CArea::Ready_GameObject_Prototype()
{
	//CUnit::Ready_GameObject_Prototype();
	
	CComponent*		pComponent = nullptr;

	if (m_pShaderCom || m_pMeshCom || m_pTransformCom)
	{
		ERR_MSG(L"CArea::Ready() : Components already exist");
		return E_FAIL;
	}

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);


	m_vColor = { 1.f, 1.f, 1.f };
	m_fAlpha = 0.7f;

	return S_OK;
}

HRESULT CArea::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	//CUnit::Ready_GameObject(_pArg);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	_int iOption = Check_Arg(_pArg);
	if (iOption < 0)
		return E_FAIL;
	
	//������
	_vec3 vScale = Get_Scale();
	m_fRadius = vScale.x;

	//����������
	_vec3 vMin, vMax;
	D3DXComputeBoundingBox(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), sizeof(_vec3), &vMin, &vMax);
	_float fMeshRadius = (vMax.x - vMin.x) * 0.5f;	//�޽��� �������� ������ ��ġ�������� �ѹ���..
	_float fMeshHeight = (vMax.y - vMin.y);			//���̴� ������ ������ �� ����. ���⼭ �����ؾ���
	_vec3 vScaleNormal = _vec3(1.f / fMeshRadius, 1.f / fMeshHeight, 1.f / fMeshRadius);	//����������
	Set_Scale(_vec3(vScaleNormal.x * m_fRadius, vScaleNormal.y * 40.f, vScaleNormal.z * m_fRadius));	//xz�� ����������*������, y�� ������ ���

	m_pTransformCom->Update_Component(0);	//������ ������Ʈ��! �� ��ü�� ������Ʈ ������Ʈ �������� ����

	return S_OK;
}

_int CArea::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	if (AREASTATE_DISABLE == m_eState)
		return NO_EVENT;

	//�÷��̾� ��ġ Ȯ�� �� ����
	if (FAILED(Update_PlayerPointer()))
		return NO_EVENT;
	Update_DistanceToPlayer();

	// �׽�Ʈ���� //////////////////////////////////////////////////////////////////////////
	//m_fDistance;
	//m_eState;
	//_int iChecker = m_listDino.size();
	//m_fRadiusReady = 80.f;
	//m_fRadiusWait = 100.f;
	//m_iDinoCountMax = 3;
	//////////////////////////////////////////////////////////////////////////

	//�Ÿ��� ���� ���� ����
	Update_State();

	//���¿� ���� ���� ����
	return Update_Actions(_dTimeDelta);
}

_int CArea::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	//���添���� ����?

#ifdef _DEBUG
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)	//���� ��ü�� ��Ʈ��Ű ������ �߿��� ����
		Engine::Add_RenderGroup(RENDER_EFFECT, this);
#endif
	return NO_EVENT;
}

void CArea::Render_GameObject()
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

CArea * CArea::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CArea*	pInstance = new CArea(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CArea::Clone(void * _pArg/* = nullptr*/)
{
	CArea*	pClone = new CArea(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CArea::Free()
{
	m_listDino.clear();
	list<CDino*>().swap(m_listDino);
	CUnit::Free();
}

HRESULT CArea::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_EffectObject"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

HRESULT CArea::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	Set_ConstantTable_Normal(_pEffect, m_pTransformCom->Get_WorldMatrix());

	_pEffect->SetFloat("g_fAlpha", m_fAlpha);
	_pEffect->SetInt("g_iTextureX", 1);
	_pEffect->SetInt("g_iTextureY", 1);
	_pEffect->SetValue("g_vObjColor", &m_vColor, sizeof(_vec3));
	_pEffect->SetValue("g_vTexUV", &_vec2(0.f, 0.f), sizeof(_vec2));

	Engine::Get_RenderTargetTexture(_pEffect, L"Depth", "g_DepthTexture");

	return S_OK;
}

_int CArea::Check_Arg(void * _pArg)
{
	if (_pArg)
	{
		if (dynamic_cast<OBJINFO*>((OBJINFO*)_pArg))
		{
			OBJINFO tInfo;
			memcpy(&tInfo, _pArg, sizeof(OBJINFO));

			//dwOption���� ���� MeshID���� ������ ������ ���� ����
			TCHAR szText[MAX_PATH];
			if (FAILED(Engine::Get_ProtoTag_Dino(szText, tInfo.dwOption)))
			{
				m_iDinoCountMax = -1;
				m_strDinoTag = L"";
				//m_eDinoTag = DINOTAG_END;
			}
			else
				m_strDinoTag = szText;
				//m_eDinoTag = (DINOTAG)tInfo.dwOption;

			Set_Scale(tInfo.vScale);
			Set_Rot(0.f, 0.f, 0.f);
			Set_Pos(tInfo.vPos);
			m_vColor = tInfo.vColor;
			
			m_tStat.fLevel = (_float)tInfo.dwLevel;		//������ ������� ���� (Shake 20��)
			m_iDinoCountMax = (_int)D3DXToDegree(tInfo.vRot.x);		//X��ȸ������ �ִ������� ����
	
			return tInfo.dwOption;
		}
		else
		{
			ERR_MSG(L"CArea::Check_Arg() : Wrong Format Argument");
			return 0;
		}
	}
	return 0;
}

HRESULT CArea::Update_PlayerPointer()
{
	if (!m_pPlayer)	//�÷��̾ �����Ǵ� ���� �����Ƿ�
	{
		m_pPlayer = (CUnit*)Engine::Get_GameObject(L"Layer_Player");
		if (!m_pPlayer)
			return E_FAIL;
	}

	return S_OK;
}

void CArea::Update_DistanceToPlayer()
{
	m_fDistance = D3DXVec3Length(&_vec3(Get_Pos().x - m_pPlayer->Get_Pos().x, 0.f, Get_Pos().z - m_pPlayer->Get_Pos().z));
}

_int CArea::Update_State()
{
	//�Ÿ��� ���� ���� �Ǵ�
	if (m_fRadiusWait < m_fDistance)
		m_eState = AREASTATE_WAIT;
	else if (m_fRadiusReady < m_fDistance)
		m_eState = AREASTATE_READY;
	else
		m_eState = AREASTATE_ACTIVE;

	if (m_fDistance < m_fRadius)
	{
		m_fAlpha = 0.7f;
		m_bPlayerSteppedIn = true;
	}
	else
	{
		m_fAlpha = 0.2f;
		m_bPlayerSteppedIn = false;
	}

	//���°� �ٲ���ٸ� �׿� ���� ��ġ
	if (m_eState != m_eStatePast)
	{
		m_eStatePast = m_eState;

		switch (m_eState)
		{
		case AREASTATE_ACTIVE:
			//�Ҽ� ���� ������Ʈ ���۽�Ŵ
			//�Ҽ� ���� ���� ���۽�Ŵ
			m_vColor = _vec3(1.f, 1.f, 1.f);
			m_fAlpha = 0.2f;
			for (auto& pDino : m_listDino)
				pDino->Set_SkipFlag(SKIP_NOTHING);
			break;
		case AREASTATE_READY:
			//�Ҽ� ���� ������Ʈ �ߴܽ�Ŵ
			//�Ҽ� ���� ���� ���۽�Ŵ
			m_vColor = _vec3(0.f, 1.f, 0.f);
			m_fAlpha = 0.5f;
			for (auto& pDino : m_listDino)
				pDino->Set_SkipFlag(SKIP_UPDATE);
			break;
		case AREASTATE_WAIT:
		case AREASTATE_DISABLE:
		case AREASTATE_END:
		default:
			m_vColor = _vec3(0.f, 0.f, 1.f);
			m_fAlpha = 0.7f;
			//�Ҽ� ���� ������Ʈ �ߴܽ�Ŵ
			//�Ҽ� ���� ���� �ߴܽ�Ŵ
			for (auto& pDino : m_listDino)
				pDino->Set_SkipFlag(SKIP_UPDATE | SKIP_RENDER);
			break;
		}
	}

	return (_int)m_eState;
}

HRESULT CArea::Update_Actions(const _double & dTimeDelta)
{
	switch (m_eState)
	{
	case AREASTATE_ACTIVE:
		return Action_Active(dTimeDelta);
		break;
	case AREASTATE_READY:
		return Action_Ready(dTimeDelta);
		break;
	case AREASTATE_WAIT:
		//return Action_Wait(dTimeDelta);
		return NO_EVENT;
		break;
	case AREASTATE_DISABLE:
	case AREASTATE_END:
	default:
		return NO_EVENT;
		break;
	}

	ERR_MSG(L"CArea::Update_Actions() : ��¼�� �̷� ������ ���̼�?");
	return S_OK;
}

HRESULT CArea::Action_Ready(const _double & dTimeDelta)
{
	//����������� �ִ��������� ���� ���
	if (m_listDino.size() < m_iDinoCountMax)
	{
		//1�� �������� 1������ ����
		if (m_dTimeSpawn < 1.f)
			m_dTimeSpawn += dTimeDelta;
		else
		{
			m_dTimeSpawn = 0;
			Spawn_Dino();
		}
	}

	return S_OK;
}

HRESULT CArea::Action_Active(const _double & dTimeDelta)
{
	auto iter = m_listDino.begin();
	for (; iter != m_listDino.end ();)
	{
		//��ü�� �� ����, ���ֵ̹� �����̸� ����Ʈ���� ����
		if (STATE_DEAD == (*iter)->Get_UnitState() || (*iter)->Get_NowTamingState())
		{
			(*iter)->Set_SkipFlag(SKIP_NOTHING);	//���ְ̹��濡 �������� �ʵ��� ��� ��ŵ�÷��� ����
			iter = m_listDino.erase(iter);
			continue;	//iter ������Ű�� ����
		}

		//���� ���� �����̶�� ������ǥ ������ �����ϵ��� ��û (2D��ǥ)
		if (m_fRadius < D3DXVec3Length(&_vec3(Get_Pos().x - (*iter)->Get_Pos().x, 0.f, Get_Pos().z - (*iter)->Get_Pos().z)))
		{
			(*iter)->MoveToTargetPos(CRandomFunc::Rand_Pos_InCircle(m_fRadius * 0.8f, Get_Pos()));	//������ * 0.8f�� �� ���� ������������ �̵�
		}

		iter++;
	}

	return S_OK;
}

HRESULT CArea::Spawn_Dino()
{
	OBJINFO tInfo;
	ZeroMemory(&tInfo, sizeof(OBJINFO));
	tInfo.vPos = _vec3(10.f, 2.f, 10.f);
	tInfo.dwOption = DINOTAG_PARA;
	tInfo.dwLevel = (_ulong)CRandomFunc::Shake_Float(m_tStat.fLevel, 0.2f);

	CDino* pGameObject = static_cast<CDino*>(Engine::Clone_GameObjectPrototype(m_strDinoTag, &tInfo));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Add_GameObjectInLayer(L"Layer_Dino", pGameObject);
	m_listDino.emplace_back(pGameObject);
	pGameObject->Set_Pos(CRandomFunc::Rand_Pos_InCircle(m_fRadius * 0.8f, Get_Pos()));

	return S_OK;
}

HRESULT CArea::Set_State(const _int & eState)
{
	if (eState < (_int)AREASTATE_END)
		m_eState = (AREASTATE)eState;
	else
		return E_FAIL;

	return S_OK;
}
