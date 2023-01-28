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

	//Ȥ�ø� ����üũ
	if (m_listInvincible.size())
	{
		ERR_MSG(L"CUnit::Ready_GameObject - m_listInvincible is not empty");
	}

	FAILED_CHECK_RETURN(CUnit::Add_Component(), E_FAIL);	//Ʈ������ ȹ��
	return S_OK;
}

_int CUnit::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	// ���� ������Ʈ
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

	// �˺���, ����
	// �ؽ�ó���, ���ؽ����, ź��Ʈ, ���̳��
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

	// Ʈ������
	if (m_pTransformCom)
		return E_FAIL;
	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// �κ� ������Ʈ
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
	//�ù����̸� true ��ȯ
	if (m_bInvincibleAll)
		return true;
	//����Ʈ ��ȸ�ϸ� ������ ���ݹ��� ����� ���������� true ��ȯ
	for (auto tChecker : m_listInvincible)
	{
		if (pAttacker == tChecker.pAttacker)
			return true;
	}
	//�ش� ������ false ��ȯ (���� �ƴմϴ�)
	return false;
}

HRESULT CUnit::Add_Invincible(const CUnit * pAttacker)
{
	//�ߺ�üũ ���� (Is_Invincible()���� pAttacker ���翩�θ� üũ�ϹǷ�)
	m_listInvincible.emplace_back(CHECKER_INVINCIBLE(pAttacker));
	return S_OK;
}

_bool CUnit::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	// �����ð� ó�� //////////////////////////////////////////////////////////////////////////
	if (Is_Invincible(pAttacker))
		return false;

	Add_Invincible(pAttacker);

	// �ǰ�����Ʈ ó�� //////////////////////////////////////////////////////////////////////////
	vCollisionPos;	//���⼭ ó���� ���� ������ ������ü���� ����Ʈ�� �ٸ��Ƿ� ���� �ڽİ�ü���� ó�� ����

	// ü�� ���� ó�� //////////////////////////////////////////////////////////////////////////
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

	//////////////////////////////////////////////��Ʈ ����
	//wstring wstrPrintFont = L"";
	//wstrPrintFont += to_wstring((_int)pAttacker->Get_Stat(STAT_ATTACK));
	//_vec3 vAddPos = {0.f,1.5f,0.f};
	//static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(m_pTransformCom->Get_Pos(), wstrPrintFont, CWorldFont_UI::World_Font_Red, CWorldFont_UI::World_FontType_Up, vAddPos,3.f);
	//
	//wstrPrintFont = L"���� ��";
	//vAddPos = { 0.f,2.5f,0.f };
	//static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(m_pTransformCom->Get_Pos(), wstrPrintFont, CWorldFont_UI::World_Font_Yellow, CWorldFont_UI::World_FontType_Fix, vAddPos, 10.f);

	//wstrPrintFont = L"����"; 
	//vAddPos = { 0.f,2.5f,0.f };
	//static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(m_pTransformCom->Get_Pos(), wstrPrintFont, CWorldFont_UI::World_Font_Green, CWorldFont_UI::World_FontType_Follow, vAddPos, 100.f);
	///////////////////////////////////////////////

	return true;
}

HRESULT CUnit::Add_Item(ITEM_INFO & tItem)
{
	//���԰� ��á���� E_FAIL ��ȯ ó���ؾ� �� <�Ұ�>
	m_pInvenCom->Add_Item(tItem);
	
	return S_OK;
}

ITEM_INFO CUnit::Remove_Item(ITEMID _eItemID, _int _iAmount)
{
	//���ϴ¾�����, ���ǿ��´� ������ ��� �˻��� ������ �� �ֵ��� �ؾ� �� <�Ұ�>
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
			// ���� ��
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

	// �̹� ������ ������
	if (iter_find != m_mapBuff.end())
	{
		// ���� �ð� �ʱ�ȭ
		iter_find->second.fBuffTime = 0.f;
	}
	// ó�� ���� ������
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

	// ������̺� ����
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

			Add_ExpToLevel(tInfo.dwLevel);	//�Լ� ���� ����

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
	//��ǥ������ �� �������� ���ų� ������ ����
	if (dwLevel <= (_ulong)m_tStat.fLevel)
		return;

	//��ǥ������ ����ġ���̺� ������ ����ġ ���� <�Ұ�>

	//������ �ݺ������� ������ <- �����ս� ���� ���ɼ� ����. �����ұ�? �� ������Ʈ�� 1���� �ص� ����ҵ�
	_bool bLevelGained = true;
	while (bLevelGained)
	{
		bLevelGained = Check_LevelUp();	//���̻� �������� �� �� ������ ������
	}
}

_bool CUnit::Add_RenderGroup_Culling(RENDERID eID, CGameObject * pGameObject)
{
	return _bool();
}

void CUnit::Check_Invincible(const _double & _dTimeDelta)
{
	//�ù����̰ų� ����Ʈ ����� 0�̸� ����
	if (m_bInvincibleAll || 0 == m_listInvincible.size())
		return;

	//����Ʈ ��ȸ
	auto iter = m_listInvincible.begin();
	for (; iter != m_listInvincible.end();)
	{
		if (m_dInvincibleMax <= iter->dTime)
		{
			//�ִ�ð� �ʰ��� üĿ�� ���� (iter �������� ����)
			iter = m_listInvincible.erase(iter);
		}
		else
		{
			//üĿ���� ��ŸŸ�� ���� �� iter++
			iter++->dTime += _dTimeDelta;
		}
	}
}

void CUnit::Check_Dying(const _double & _dTimeDelta)
{
	if (!m_bDying)
		return;

	m_dDyingTime += _dTimeDelta;
	
	m_bInvincibleAll = true;	//��� �� ���ι���

	Set_Scale(Get_Scale() * 0.9f);	//���׶���

	if (m_dDyingMax <= m_dDyingTime)
	{
		m_dDyingTime = 0;
		//m_bDying = false; //�ʿ�����?
		Set_Dead();
	}
}

void CUnit::Check_Stat(const _double & _dTimeDelta)
{
	if (!m_bIsStatUpdate)
		return;

	// �����
	if (0.f >= m_tStat.fHunger)					
	{
		//m_tStat.fHp -= 2.f * (_float)_dTimeDelta;		// ������� 0�̸� HP �ʴ� 2 ����
	}
	else
	{
		m_tStat.fHunger -= 0.5f * (_float)_dTimeDelta;			// ����� �ʴ� 0.5 ����
		//m_tStat.fHp += 1.f * (_float)_dTimeDelta;
		//if (m_tStat.fMaxHp <= m_tStat.fHp)
		//	m_tStat.fHp = m_tStat.fMaxHp;

		if (0.f >= m_tStat.fHunger)						// ����� 0 ~ max ����
			m_tStat.fHunger = 0.f;
		else if (m_tStat.fMaxHunger <= m_tStat.fHunger)
			m_tStat.fHunger = m_tStat.fMaxHunger;
		else if (m_tStat.fMaxHp > m_tStat.fHp)
		{
			// ������� HP�� ��ȯ
			m_tStat.fHp += (_float)_dTimeDelta;
			m_tStat.fHunger -= (_float)_dTimeDelta;

			if (m_tStat.fHp > m_tStat.fMaxHp)
				m_tStat.fHp = m_tStat.fMaxHp;
		}
	}

	// ���� ���� �� ��� ����
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
