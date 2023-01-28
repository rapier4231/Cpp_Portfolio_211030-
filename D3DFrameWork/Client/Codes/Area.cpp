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
	
	//반지름
	_vec3 vScale = Get_Scale();
	m_fRadius = vScale.x;

	//스케일조정
	_vec3 vMin, vMax;
	D3DXComputeBoundingBox(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), sizeof(_vec3), &vMin, &vMax);
	_float fMeshRadius = (vMax.x - vMin.x) * 0.5f;	//메쉬의 반지름은 툴에서 조치할테지만 한번더..
	_float fMeshHeight = (vMax.y - vMin.y);			//높이는 툴에서 조정할 수 없음. 여기서 조정해야함
	_vec3 vScaleNormal = _vec3(1.f / fMeshRadius, 1.f / fMeshHeight, 1.f / fMeshRadius);	//단위스케일
	Set_Scale(_vec3(vScaleNormal.x * m_fRadius, vScaleNormal.y * 40.f, vScaleNormal.z * m_fRadius));	//xz는 단위스케일*반지름, y는 임의의 상수

	m_pTransformCom->Update_Component(0);	//마지막 업데이트임! 이 객체는 컴포넌트 업데이트 수행하지 않음

	return S_OK;
}

_int CArea::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	if (AREASTATE_DISABLE == m_eState)
		return NO_EVENT;

	//플레이어 위치 확인 및 갱신
	if (FAILED(Update_PlayerPointer()))
		return NO_EVENT;
	Update_DistanceToPlayer();

	// 테스트보드 //////////////////////////////////////////////////////////////////////////
	//m_fDistance;
	//m_eState;
	//_int iChecker = m_listDino.size();
	//m_fRadiusReady = 80.f;
	//m_fRadiusWait = 100.f;
	//m_iDinoCountMax = 3;
	//////////////////////////////////////////////////////////////////////////

	//거리에 따라 상태 갱신
	Update_State();

	//상태에 따른 동작 수행
	return Update_Actions(_dTimeDelta);
}

_int CArea::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	//공룡렌더에 간섭?

#ifdef _DEBUG
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)	//영역 자체는 컨트롤키 누르는 중에만 렌더
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

			//dwOption으로 받은 MeshID값을 참조해 스폰할 공룡 지정
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
			
			m_tStat.fLevel = (_float)tInfo.dwLevel;		//생성될 공룡들의 레벨 (Shake 20퍼)
			m_iDinoCountMax = (_int)D3DXToDegree(tInfo.vRot.x);		//X축회전값을 최대공룡수로 전용
	
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
	if (!m_pPlayer)	//플레이어가 삭제되는 일이 없으므로
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
	//거리에 따라서 상태 판단
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

	//상태가 바뀌었다면 그에 따른 조치
	if (m_eState != m_eStatePast)
	{
		m_eStatePast = m_eState;

		switch (m_eState)
		{
		case AREASTATE_ACTIVE:
			//소속 공룡 업데이트 시작시킴
			//소속 공룡 렌더 시작시킴
			m_vColor = _vec3(1.f, 1.f, 1.f);
			m_fAlpha = 0.2f;
			for (auto& pDino : m_listDino)
				pDino->Set_SkipFlag(SKIP_NOTHING);
			break;
		case AREASTATE_READY:
			//소속 공룡 업데이트 중단시킴
			//소속 공룡 렌더 시작시킴
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
			//소속 공룡 업데이트 중단시킴
			//소속 공룡 렌더 중단시킴
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

	ERR_MSG(L"CArea::Update_Actions() : 어쩌다 이런 곳까지 오셨소?");
	return S_OK;
}

HRESULT CArea::Action_Ready(const _double & dTimeDelta)
{
	//보유공룡수가 최대공룡수보다 적은 경우
	if (m_listDino.size() < m_iDinoCountMax)
	{
		//1초 간격으로 1마리씩 스폰
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
		//시체가 된 공룡, 테이밍된 공룡이면 리스트에서 삭제
		if (STATE_DEAD == (*iter)->Get_UnitState() || (*iter)->Get_NowTamingState())
		{
			(*iter)->Set_SkipFlag(SKIP_NOTHING);	//테이밍공룡에 영향주지 않도록 모든 스킵플래그 해제
			iter = m_listDino.erase(iter);
			continue;	//iter 증가시키지 않음
		}

		//선을 넘은 공룡이라면 랜덤좌표 안으로 복귀하도록 요청 (2D좌표)
		if (m_fRadius < D3DXVec3Length(&_vec3(Get_Pos().x - (*iter)->Get_Pos().x, 0.f, Get_Pos().z - (*iter)->Get_Pos().z)))
		{
			(*iter)->MoveToTargetPos(CRandomFunc::Rand_Pos_InCircle(m_fRadius * 0.8f, Get_Pos()));	//반지름 * 0.8f인 원 안쪽 임의지점으로 이동
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
