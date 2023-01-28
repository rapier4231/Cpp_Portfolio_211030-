#include "stdafx.h"
#include "..\Headers\Unit.h"

#include "Export_Function.h"
#include "Inven.h"
#include "WorldFont_UI.h"

USING(Engine)

CUnit::CUnit(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	ZeroMemory(&m_tStat, sizeof(UNITSTAT));
}

CUnit::CUnit(const CUnit & rhs)
	: CGameObject(rhs)
	, m_tStat(rhs.m_tStat)
	, m_pShaderCom(rhs.m_pShaderCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pInvenCom(rhs.m_pInvenCom)
	, m_listInvincible(rhs.m_listInvincible)
	, m_dInvincibleMax(rhs.m_dInvincibleMax)
	, m_bDying(rhs.m_bDying)
	, m_dDyingTime(rhs.m_dDyingTime)
	, m_dDyingMax(rhs.m_dDyingMax)
	, m_pShaderShadowCom(rhs.m_pShaderShadowCom)
	, m_pShaderShadowMapCom(rhs.m_pShaderShadowMapCom)
	, m_pDissolveTextureCom(rhs.m_pDissolveTextureCom)
	, m_wstrMeshName(rhs.m_wstrMeshName)
{
}

HRESULT CUnit::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();
	FAILED_CHECK_RETURN(CUnit::Add_Component_Prototype(), E_FAIL);
	return S_OK;
}

HRESULT CUnit::Ready_GameObject(void * _pArg)
{
	CGameObject::Ready_GameObject();

	//혹시모를 에러체크
	if (m_listInvincible.size())
	{
		ERR_MSG(L"CUnit::Ready_GameObject - m_listInvincible is not empty");
	}

	FAILED_CHECK_RETURN(CUnit::Add_Component(), E_FAIL);	//트랜스폼 획득
	return S_OK;
}

_int CUnit::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	// 스탯 업데이트
	Check_Stat(_dTimeDelta);
	Check_Invincible(_dTimeDelta);
	//Check_LevelUp();
	//Check_Dying(_dTimeDelta);

	return CGameObject::Update_GameObject(_dTimeDelta);
}

void CUnit::Free()
{
	CGameObject::Free();
}

HRESULT CUnit::Add_Component_Prototype()
{
	CComponent*		pComponent = nullptr;

	// 알베도, 뎁스
	// 텍스처노멀, 버텍스노멀, 탄젠트, 바이노멀
	if (m_pShaderCom || m_pShaderShadowMapCom || m_pShaderShadowCom)
		return E_FAIL;

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Normal_Shadow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pShaderShadowMapCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_ShadowMap"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_ShaderShadowMap", pComponent);

	pComponent = m_pDissolveTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_DissolveTexture", pComponent);

	return S_OK;
}

HRESULT CUnit::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// 트랜스폼
	if (m_pTransformCom)
		return E_FAIL;
	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// 인벤 컴포넌트
	InvenUserUnitTypeID eInvenID = InvenUserUnitTypeID_Harvestable;
	pComponent = m_pInvenCom = static_cast<CInven*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Inven", &eInvenID));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Inven", pComponent);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////


	return S_OK;
}

_float CUnit::Get_Stat(STAT _eStat)
{
	_float	fStat = 0;

	memcpy(&fStat, (_float*)&m_tStat + _eStat, sizeof(_float));

	return fStat;
}

void CUnit::Set_Stat(STAT _eStat, _float _fValue)
{
	_float*	fStat = 0;

	fStat = (_float*)&m_tStat + _eStat;

	*fStat += _fValue;
}

_vec3 CUnit::Get_Pos()
{
	if (m_pTransformCom)
		return *m_pTransformCom->Get_Pos();
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
		return _vec3(0.f, 0.f, 0.f);
	}
}

void CUnit::Set_Pos(const _vec3 & _vPos)
{
	if (m_pTransformCom)
		m_pTransformCom->Set_Pos(_vPos);
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
	}
}

void CUnit::Set_Pos(const _float & _fX, const _float & _fY, const _float & _fZ)
{
	if (m_pTransformCom)
		m_pTransformCom->Set_Pos(_fX, _fY, _fZ);
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
	}
}

_float CUnit::Get_RotX()
{
	if (m_pTransformCom)
		return m_pTransformCom->Get_Rotate(ROT_X);
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
		return 0.f;
	}
}

_float CUnit::Get_RotY()
{
	if (m_pTransformCom)
		return m_pTransformCom->Get_Rotate(ROT_Y);
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
		return 0.f;
	}
}

_float CUnit::Get_RotZ()
{
	if (m_pTransformCom)
		return m_pTransformCom->Get_Rotate(ROT_Z);
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
		return 0.f;
	}
}

void CUnit::Set_RotX(const _float & _vRot)
{
	if (m_pTransformCom)
		m_pTransformCom->Set_Rotate(ROT_X, _vRot);
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
	}
}

void CUnit::Set_RotY(const _float & _vRot)
{
	if (m_pTransformCom)
		m_pTransformCom->Set_Rotate(ROT_Y, _vRot);
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
	}
}

void CUnit::Set_RotZ(const _float & _vRot)
{
	if (m_pTransformCom)
		m_pTransformCom->Set_Rotate(ROT_Z, _vRot);
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
	}
}

void CUnit::Set_Rot(const _float & _fX, const _float & _fY, const _float & _fZ)
{
	if (m_pTransformCom)
	{
		m_pTransformCom->Set_Rotate(ROT_X, _fX);
		m_pTransformCom->Set_Rotate(ROT_Y, _fY);
		m_pTransformCom->Set_Rotate(ROT_Z, _fZ);
	}
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
	}
}

_vec3 CUnit::Get_Scale()
{
	if (m_pTransformCom)
		return *m_pTransformCom->Get_Scale();
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
		return _vec3(1.f, 1.f, 1.f);
	}
}

void CUnit::Set_Scale(const _vec3 & _vScale)
{
	if (m_pTransformCom)
		m_pTransformCom->Set_Scale(_vScale);
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
	}
}

void CUnit::Set_Scale(const _float & _fScale)
{
	if (m_pTransformCom)
		m_pTransformCom->Set_Scale(_fScale, _fScale, _fScale);
	else
	{
		ERR_MSG(L"m_pTransformCom is nullptr");
	}
}

_bool CUnit::Is_Invincible(const CUnit* pAttacker)
{
	//올무적이면 true 반환
	if (m_bInvincibleAll)
		return true;
	//리스트 순회하며 이전에 공격받은 기록이 남아있으면 true 반환
	for (auto tChecker : m_listInvincible)
	{
		if (pAttacker == tChecker.pAttacker)
			return true;
	}
	//해당 없으면 false 반환 (무적 아닙니다)
	return false;
}

HRESULT CUnit::Add_Invincible(const CUnit * pAttacker)
{
	//중복체크 생략 (Is_Invincible()에서 pAttacker 존재여부를 체크하므로)
	m_listInvincible.emplace_back(CHECKER_INVINCIBLE(pAttacker));
	return S_OK;
}

_bool CUnit::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	// 무적시간 처리 //////////////////////////////////////////////////////////////////////////
	if (Is_Invincible(pAttacker))
		return false;

	Add_Invincible(pAttacker);

	// 피격이펙트 처리 //////////////////////////////////////////////////////////////////////////
	vCollisionPos;	//여기서 처리할 수도 있으나 개별객체마다 이펙트가 다르므로 개별 자식객체에서 처리 권장

	// 체력 감소 처리 //////////////////////////////////////////////////////////////////////////
	_float fAttackPoint = pAttacker->Get_Stat(STAT_ATTACK);
	m_tStat.fHp -= fAttackPoint;
	//m_tStat.fParalysis -= fAttackPoint;
	
	//if (m_tStat.iParalysis <= 0)
	//	m_eUnitState = STATE_TORPID;

	if (!m_bDying && m_tStat.fHp <= 0)
	{
		m_bDying = true;
		m_tStat.fHp = 0;
		m_eUnitState = STATE_DEAD;

		return false;
	}

	//////////////////////////////////////////////폰트 띄우기
	//wstring wstrPrintFont = L"";
	//wstrPrintFont += to_wstring((_int)pAttacker->Get_Stat(STAT_ATTACK));
	//_vec3 vAddPos = {0.f,1.5f,0.f};
	//static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(m_pTransformCom->Get_Pos(), wstrPrintFont, CWorldFont_UI::World_Font_Red, CWorldFont_UI::World_FontType_Up, vAddPos,3.f);
	//
	//wstrPrintFont = L"때린 곳";
	//vAddPos = { 0.f,2.5f,0.f };
	//static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(m_pTransformCom->Get_Pos(), wstrPrintFont, CWorldFont_UI::World_Font_Yellow, CWorldFont_UI::World_FontType_Fix, vAddPos, 10.f);

	//wstrPrintFont = L"도도"; 
	//vAddPos = { 0.f,2.5f,0.f };
	//static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(m_pTransformCom->Get_Pos(), wstrPrintFont, CWorldFont_UI::World_Font_Green, CWorldFont_UI::World_FontType_Follow, vAddPos, 100.f);
	///////////////////////////////////////////////

	return true;
}

HRESULT CUnit::Add_Item(ITEM_INFO & tItem)
{
	//무게가 꽉찼으면 E_FAIL 반환 처리해야 함 <할것>
	m_pInvenCom->Add_Item(tItem);
	
	return S_OK;
}

ITEM_INFO CUnit::Remove_Item(ITEMID _eItemID, _int _iAmount)
{
	//원하는아이템, 조건에맞는 아이템 등등 검색해 지정할 수 있도록 해야 함 <할것>
	ITEM_INFO tOut_RemoveItem;
	tOut_RemoveItem = m_pInvenCom->Remove_Item(_eItemID, _iAmount);

	return tOut_RemoveItem;
}

void CUnit::Close_Ring_Command()
{
	static_cast<CRingCommand_UI*>(Engine::Get_GameObject(L"RingCommand_UI"))->Close_RingCommand();
}

void CUnit::Check_Buff(const _double & _dTimeDelta)
{
	if (!m_mapBuff.empty())
	{
		for (auto& iter = m_mapBuff.begin(); iter != m_mapBuff.end();)
		{
			(*iter).second.fBuffTime += (_float)_dTimeDelta;
			// 버프 끝
			if ((*iter).second.fBuffTime >= (*iter).second.fMaxBuffTime)
			{
				Engine::Delete_GameObject(L"Layer_Effect", (*iter).second.pEffect);
				m_mapBuff.erase(iter++);
			}
			else
			{
				_vec3 vPos = Get_Pos();
				vPos.y += m_fCenterPos;
				static_cast<CTransform*>((*iter).second.pEffect->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);
				++iter;
			}
		}
	}
}

void CUnit::Add_Buff(wstring wstrBuffName, _float fBuffMaxTime, _float fScale)
{
	auto& iter_find = m_mapBuff.find(wstrBuffName);

	// 이미 버프가 있으면
	if (iter_find != m_mapBuff.end())
	{
		// 버프 시간 초기화
		iter_find->second.fBuffTime = 0.f;
	}
	// 처음 버프 들어오면
	else
	{
		BUFF tBuff;
		tBuff.fBuffTime = 0.f;
		tBuff.fMaxBuffTime = fBuffMaxTime;

		tBuff.pEffect = Engine::Clone_GameObjectPrototype(wstrBuffName);
		NULL_CHECK_RETURN(tBuff.pEffect, );
		FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", tBuff.pEffect), );

		m_mapBuff.emplace(wstrBuffName, tBuff);
	}
}

HRESULT CUnit::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat matWorld, matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	// 상수테이블 세팅
	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);
	_pEffect->SetFloat("g_fAmount", m_fDissolveAmount);
	_pEffect->SetVector("g_vSetColor", &_vec4(m_vColor, 1.f));

	m_pDissolveTextureCom->Set_Texture(_pEffect, "g_DissolveTexture");

	return S_OK;
}

_int CUnit::Check_Arg(void * _pArg)
{
	if (_pArg)
	{
		if (dynamic_cast<OBJINFO*>((OBJINFO*)_pArg))
		{
			OBJINFO tInfo;
			memcpy(&tInfo, _pArg, sizeof(OBJINFO));

			Set_Scale(tInfo.vScale);
			Set_Rot(tInfo.vRot.x, tInfo.vRot.y, tInfo.vRot.z);
			Set_Pos(tInfo.vPos);

			Add_ExpToLevel(tInfo.dwLevel);	//함수 내용 없음

			m_vColor = tInfo.vColor;

			return tInfo.dwOption;
		}
		else
		{
			ERR_MSG(L"Wrong Format Argument");
			return 0;
		}
	}
	return 0;
}

void CUnit::Add_ExpToLevel(_ulong dwLevel)
{
	//목표레벨이 내 레벨보다 낮거나 같으면 리턴
	if (dwLevel <= (_ulong)m_tStat.fLevel)
		return;

	//목표레벨의 경험치테이블 참조해 경험치 변경 <할것>

	//강제로 반복문돌려 레벨업 <- 퍼포먼스 부하 가능성 있음. 생략할까? 매 업데이트당 1업씩 해도 충분할듯
	_bool bLevelGained = true;
	while (bLevelGained)
	{
		bLevelGained = Check_LevelUp();	//더이상 레벨업이 안 될 때까지 레벨업
	}
}

_bool CUnit::Add_RenderGroup_Culling(RENDERID eID, CGameObject * pGameObject)
{
	return _bool();
}

void CUnit::Check_Invincible(const _double & _dTimeDelta)
{
	//올무적이거나 리스트 사이즈가 0이면 생략
	if (m_bInvincibleAll || 0 == m_listInvincible.size())
		return;

	//리스트 순회
	auto iter = m_listInvincible.begin();
	for (; iter != m_listInvincible.end();)
	{
		if (m_dInvincibleMax <= iter->dTime)
		{
			//최대시간 초과한 체커는 삭제 (iter 증가하지 않음)
			iter = m_listInvincible.erase(iter);
		}
		else
		{
			//체커마다 델타타임 증가 후 iter++
			iter++->dTime += _dTimeDelta;
		}
	}
}

void CUnit::Check_Dying(const _double & _dTimeDelta)
{
	if (!m_bDying)
		return;

	m_dDyingTime += _dTimeDelta;
	
	m_bInvincibleAll = true;	//사망 중 전부무적

	Set_Scale(Get_Scale() * 0.9f);	//오그라든다

	if (m_dDyingMax <= m_dDyingTime)
	{
		m_dDyingTime = 0;
		//m_bDying = false; //필요한지?
		Set_Dead();
	}
}

void CUnit::Check_Stat(const _double & _dTimeDelta)
{
	if (!m_bIsStatUpdate)
		return;

	// 배고픔
	if (0.f >= m_tStat.fHunger)					
	{
		//m_tStat.fHp -= 2.f * (_float)_dTimeDelta;		// 배고픔이 0이면 HP 초당 2 감소
	}
	else
	{
		m_tStat.fHunger -= 0.5f * (_float)_dTimeDelta;			// 배고픔 초당 0.5 감소
		//m_tStat.fHp += 1.f * (_float)_dTimeDelta;
		//if (m_tStat.fMaxHp <= m_tStat.fHp)
		//	m_tStat.fHp = m_tStat.fMaxHp;

		if (0.f >= m_tStat.fHunger)						// 배고음 0 ~ max 제한
			m_tStat.fHunger = 0.f;
		else if (m_tStat.fMaxHunger <= m_tStat.fHunger)
			m_tStat.fHunger = m_tStat.fMaxHunger;
		else if (m_tStat.fMaxHp > m_tStat.fHp)
		{
			// 배고픔을 HP로 전환
			m_tStat.fHp += (_float)_dTimeDelta;
			m_tStat.fHunger -= (_float)_dTimeDelta;

			if (m_tStat.fHp > m_tStat.fMaxHp)
				m_tStat.fHp = m_tStat.fMaxHp;
		}
	}

	// 쉬고 있을 때 기력 증가
	if (!m_bRecoveryStamina)
	{
		m_dRecoveryStaminaTime += _dTimeDelta;
		if (2 < m_dRecoveryStaminaTime)
		{
			m_dRecoveryStaminaTime = 0.0;
			m_bRecoveryStamina = true;
		}
	}

	if (m_bRecoveryStamina && m_tStat.fMaxStamina > m_tStat.fStamina)
	{
		m_tStat.fStamina += 20.f * (_float)_dTimeDelta;

		if (0.f >= m_tStat.fStamina)
			m_tStat.fStamina = 0.f;
		else if (m_tStat.fMaxStamina <= m_tStat.fStamina)
			m_tStat.fStamina = m_tStat.fMaxStamina;
	}
}
