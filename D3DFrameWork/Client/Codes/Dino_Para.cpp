#include "stdafx.h"
#include "Dino_Para.h"

#include "Export_Function.h"

#include "Inven.h"
#include "LR_Inventory.h"
#include "QuickSlot_UI.h"

#include "PhysX_Mgr.h"

#include "RandomFunc.h"
#include "Harvestable.h"

USING(Engine)

CDino_Para::CDino_Para(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Para::CDino_Para(const CDino_Para & rhs)
	: CDino(rhs)
	, m_iIndex(rhs.m_iIndex)
{
}

HRESULT CDino_Para::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Para::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	// �Ķ� ���� ����
	/////////////////////////////
	m_wstrDinoSpecies = L"Para";
	m_wstrDinoSpeciesKor = L"�Ķ��츣��";
	/////////////////////////////
	m_iAsddleIndex = 1;								// �Ž��󿡼��� ���� ����� ��ȣ
	m_pSaddleBoneName = "c_back2";	// ���� �� �̸�
	m_pNeckBoneName = "c_neck3";	// �� �� �̸�

	//_uint* ui = (_uint*)_pArg;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	// ����¸�ŭ bool �� �����Ҵ�
	m_bIsSaddleIndex = new _bool[m_pMeshCom->Get_MeshList()->size()];
	memset(m_bIsSaddleIndex, 1, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsSaddleIndex[m_iAsddleIndex] = false;		// ��ü�� �׸���


	Set_Scale(0.01f);
	m_vMovePos = Get_Pos();
	//// �⺻ ����
	m_iAnimation = PARA_ANIM_IDLE;

	//// ���� �θ� ��Ʈ����
	m_pSaddleFrame = m_pMeshCom->Get_FrameByName(m_pSaddleBoneName);

	// �Ķ� �ɷ�ġ
	//m_tStat.fMaxHp = 200.f;
	//m_tStat.fHp = m_tStat.fMaxHp;
	//m_tStat.fAtk = 50.f;
	m_tStat.fMaxStamina = 100.f;
	m_tStat.fStamina = m_tStat.fMaxStamina;
	m_tStat.fKnockBack = 2.f;
	m_tStat.fMaxWeight = 2000.f;
	m_tStat.fWeight = 0.f;
	m_tStat.fMaxHunger = 200.f;
	m_tStat.fHunger = m_tStat.fMaxHunger;
	m_tStat.fMaxThirst = 50.f;
	m_tStat.fThirst = m_tStat.fMaxThirst;
	m_tStat.fSpeed = 0.f;
	m_tStat.fSize = 1.5f;


	// ���̹� ����
	m_eTamingFood = ITEMID_AmarBerry;

	m_iAnimation = PARA_ANIM_IDLE;
	m_iUpperAnimation = PARA_ANIM_IDLE;

	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_IDLE2;
	m_eNextRandomAnim[2] = STATE_IDLE3;
	m_dIdleMaxTime = rand() % 5;

	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(1.f, 1.f, Get_Pos());
	if (nullptr == m_pCtrl)
		return E_FAIL;

	Load_Collider(L"../Resources/Data/Collider/Para_Collider.dat");

	Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"PARA");

	m_fDustScale = 0.005f;

	m_fCenterPos = 2.f;

	return S_OK;
}

_int CDino_Para::Update_GameObject(const _double & _dTimeDelta)
{

	m_vfootPoint = { 0.f, -1.7f, 0.f };

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	m_vfootPoint += vLook * 1.f;
	//Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"PARA");
	//_uint iResult = NO_EVENT;

	if (m_eUnitState == STATE_IDLE && !m_bIsTamed)
		m_bRoar = false;

	if(!m_bIsRide && !m_bIsTamed)
		m_bAttackMode = false;

	return CDino::Update_GameObject(_dTimeDelta);
}

_int CDino_Para::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CDino::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CDino_Para::Render_GameObject()
{
	CDino::Render_GameObject();
}

void CDino_Para::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Para::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDino_Para::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	//_bool bHit = CDino::HitBy(pAttacker, dwAttackType, vCollisionPos);
	if (false == CDino::HitBy(pAttacker, dwAttackType, vCollisionPos))
		return false;

	if (!m_bRoar && !m_bIsTorpid && !m_bDying && STATE_HOLDING != m_eUnitState)
	{
		m_bRoar = true;
		m_eUnitState = STATE_ROAR;
	}
	else if (m_bDying)
	{
		_int iMeatAmount = 0, iHideAmount = 0;
		_int iAmount = (_int)CRandomFunc::Shake_Float(pAttacker->Get_Stat(STAT_ATTACK), 0.2f);

		if (0 > iAmount)
			iAmount = 1;

		if (WEAPON_HATCHET == dwAttackType)
		{
			// ������ ��⸦ ���� �ش�.
			iMeatAmount = _int(iAmount * 0.5f);
			iHideAmount = _int(iAmount * 0.2f);
		}
		else if (WEAPON_STONEPICK == dwAttackType)
		{
			// ��̴� ���׸� ���� �ش�.
			iMeatAmount = _int(iAmount * 0.2f);
			iHideAmount = _int(iAmount * 0.5f);
		}
		else if (WEAPON_HAND == dwAttackType)
		{
			// �Ǽ��� ��⸦ �����ش�
			iMeatAmount = _int(iAmount * 0.5f);
			iHideAmount = _int(iAmount * 0.2f);
		}
		else if (WEAPON_BOLA == dwAttackType)
		{
			// Ÿ�� �ȉ�
			return false;
		}
		else
		{
			// ���� ����
			iMeatAmount = _int(iAmount * 0.5f);;
			iHideAmount = _int(iAmount * 0.2f);;
		}

		if (0 < iMeatAmount)
		{
			_int iMeatRemoved = Remove_Item(ITEMID::ITEMID_Meat, iMeatAmount).iAmount;
			static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_Meat, iMeatRemoved);
			static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(iMeatRemoved * 2.f);
		}
		if (0 < iHideAmount)
		{
			static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_Hide, iHideAmount);
			static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(iHideAmount * 2.f);
		}

		if(0 >= m_pInvenCom->Now_Have_ItemAmount(ITEMID_Meat))
			Set_Dead();
	}

	return true;
}

CDino_Para * CDino_Para::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Para*	pInstance = new CDino_Para(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Para::Clone(void * _pArg)
{
	CDino_Para*	pClone = new CDino_Para(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Para::Free()
{
	CDino::Free();
}

void CDino_Para::Ride_Key_Input_Custum(const _double & _dTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_C) & 0x80 && !m_bBuffRoar)
	{
		m_bBuffRoar = true;
		if (m_eUnitState == STATE_SPRINT)
			m_eUnitState = STATE_SPRINT;
		else
			m_eUnitState = m_eUnitState == STATE_MOVE ? STATE_MOVE : STATE_ROAR;

		m_eUpperDinoState = STATE_ROAR;
	}
}


void CDino_Para::IsRide_Set_SaddleMat()
{
	//WCHAR str[255] = L"";
	//GetPrivateProfileString(L"RECT", L"x", 0, str, 255, L"../Resources/text.ini");
	//wstring ss = str;
	//float x = stof(ss);
	//GetPrivateProfileString(L"RECT", L"y", 0, str, 255, L"../Resources/text.ini");
	//ss = str;
	//float y = stof(ss);
	//GetPrivateProfileString(L"RECT", L"z", 0, str, 255, L"../Resources/text.ini");
	//ss = str;
	//float z = stof(ss);

	//GetPrivateProfileString(L"RECT", L"q", 0, str, 255, L"../Resources/text.ini");
	//ss = str;
	//float q = stof(ss);
	//GetPrivateProfileString(L"RECT", L"w", 0, str, 255, L"../Resources/text.ini");
	//ss = str;
	//float w = stof(ss);

	_mat matSaddle, matScale, matRotX, matRotY, matRotZ;
	matSaddle = m_pSaddleFrame->CombinedTransformMatrix;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90.f));
	D3DXMatrixRotationAxis(&matRotY, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	D3DXMatrixTranslation(&matRotZ, 20.f , 0.f, -20.f);

	m_matSaddle = matScale * matRotX * matRotY * matRotZ * matSaddle * m_pTransformCom->Get_WorldMatrix();
}

_bool CDino_Para::Harvest_Action(wstring & wstrTargetLayer, CGameObject * pLookObject)
{
	if (pLookObject)
	{
		if (!static_cast<CUnit*>(pLookObject)->Get_Dying())
		{
			if (wstrTargetLayer == L"Layer_Harvest_Bush")
			{
				m_eUpperDinoState = STATE_ATTACK;
				if (m_eUnitState == STATE_SPRINT)
					m_eUnitState = STATE_SPRINT;
				else
					m_eUnitState = m_eUnitState == STATE_MOVE ? STATE_MOVE : STATE_ATTACK;

				static_cast<CHarvestable*>(pLookObject)->HitBy(this, WEAPON_DINO, _vec3(static_cast<CHarvestable*>(pLookObject)->Get_Pos()));
			}
			else if (wstrTargetLayer == L"Layer_Harvest_Tree")
			{
				m_eUpperDinoState = STATE_ATTACK;
				if (m_eUnitState == STATE_SPRINT)
					m_eUnitState = STATE_SPRINT;
				else
					m_eUnitState = m_eUnitState == STATE_MOVE ? STATE_MOVE : STATE_ATTACK;

				static_cast<CHarvestable*>(pLookObject)->HitBy(this, WEAPON_DINO, _vec3(static_cast<CHarvestable*>(pLookObject)->Get_Pos()));
			}
			else
				return false;

			return true;
		}
	}

	return false;
}

void CDino_Para::Check_State()
{
	CDino::Check_IsEndAnimation();
	CDino::Check_IsUpperEndAnimation();

	_vec3 vPos = *m_pTransformCom->Get_Pos();

	if (m_ePreUnitState != m_eUnitState)
	{
		m_ePreUnitState = m_eUnitState;

		switch (m_eUnitState)
		{
		case CUnit::STATE_IDLE:
			m_iAnimation = PARA_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE2:
			m_iAnimation = PARA_ANIM_IDLE_GRAZE;
			break;
		case CUnit::STATE_IDLE3:
			m_iAnimation = PARA_ANIM_TERRET_CHECK;
			break;
		case CUnit::STATE_MOVE:
			m_tStat.fSpeed = m_fSpeed;
			vPos.x += CRandomFunc::Rand_Int(-60, 60);
			vPos.z += CRandomFunc::Rand_Int(-60, 60);
			m_vMovePos = vPos;
			break;
		case CUnit::STATE_SPRINT:
			m_tStat.fSpeed = m_fSprint;
			break;
		case CUnit::STATE_ATTACK:
			m_iAnimation = PARA_ATTACK;
			m_bIsAttack = true;
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:
			m_iAnimation = PARA_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_TORPID_IDLE:
			m_iAnimation = PARA_ANIM_TORPID_IDLE;
			break;
		case CUnit::STATE_TORPID_TAMED:		// ������ �Ͼ�� (���̹� �Ϸ�)
			m_iAnimation = PARA_ANIM_TORPID_OUT_TAMED;
			break;
		case CUnit::STATE_TORPID_WILD:
			m_iAnimation = PARA_ANIM_TORPID_OUT_WILD;
			break;
		case CUnit::STATE_TORPID_EAT:		// ������ ���ĸԱ�
			m_iAnimation = PARA_ANIM_TORPID_EAT;
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = PARA_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			break;
		case CUnit::STATE_RIDE:
			break;
		case CUnit::STATE_ROAR:
			m_iAnimation = PARA_ANIM_ROAR_ALERT;
			break;
		/// ���� �¾��� �� 
		case CUnit::STATE_HOLDING:
			m_iAnimation = PARA_ANIM_IDLE;
			m_bIsHolding = true;
			break;
		}
	}

	//�޸��� ���⿡ ���� �� ������ (�ӽ�)
	if (STATE_MOVE == m_eUnitState || STATE_SPRINT == m_eUnitState)
	{
		switch (m_eUnitMoveDir)
		{
		//case CUnit::UNIT_DIR_CENTER:
		//	break;
		case CUnit::UNIT_DIR_DOWN:
			m_iAnimation = PARA_MOVE_BACKWARD;
			break;
		//case CUnit::UNIT_DIR_DOWN_L:
		//	break;
		//case CUnit::UNIT_DIR_DOWN_R:
		//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = PARA_MOVE_FWORD;
			else
				m_iAnimation = PARA_CHARGE_FOWARD;
			break;
		/*case CUnit::UNIT_DIR_UP_L:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = REX_A_MOVE_LFT;
			else
				m_iAnimation = REX_A_CHARGE_LFT;
			break;
		case CUnit::UNIT_DIR_UP_R:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = REX_A_MOVE_RIT;
			else
				m_iAnimation = REX_A_CHARGE_RIT;
			break;*/
		}
	}


	// �κ� �ִϸ��̼� üũ
	if (m_ePreUpperDinoState != m_eUpperDinoState)
	{
		m_ePreUpperDinoState = m_eUpperDinoState;

		switch (m_eUpperDinoState)
		{
		case CUnit::STATE_ATTACK:
			m_iUpperAnimation = PARA_ATTACK;
			m_bIsAttack = true;
			break;
		case CUnit::STATE_ROAR:
			m_iUpperAnimation = PARA_ANIM_ROAR_ALERT;
			break;
		}

	}
}

HRESULT CDino_Para::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// �Ž� ������Ʈ
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Para"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test��
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 30);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Para::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}
