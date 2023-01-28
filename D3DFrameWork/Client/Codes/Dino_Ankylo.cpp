#include "stdafx.h"
#include "Dino_Ankylo.h"
#include "Export_Function.h"
#include "TPV_Camera.h"
#include "Inven.h"
#include "LR_Inventory.h"
#include "QuickSlot_UI.h"
#include "PhysX_Mgr.h"
#include "RandomFunc.h"
#include "Harvestable.h"

USING(Engine)

CDino_Ankylo::CDino_Ankylo(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Ankylo::CDino_Ankylo(const CDino_Ankylo & rhs)
	: CDino(rhs)
	, m_iIndex(rhs.m_iIndex)
{
}

HRESULT CDino_Ankylo::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Ankylo::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	/////////////////////////////
	m_wstrDinoSpecies = L"Ankylo";
	m_wstrDinoSpeciesKor = L"안킬로사우르스";
	/////////////////////////////
	m_iAsddleIndex = 1;								// 매쉬상에서의 안장 서브셋 번호
	m_pSaddleBoneName = "c_back2";	// 안장 뼈 이름
	m_pNeckBoneName = "c_neck2";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 서브셋만큼 bool 값 동적할당
	m_bIsSaddleIndex = new _bool[m_pMeshCom->Get_MeshList()->size()];
	memset(m_bIsSaddleIndex, 1, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsSaddleIndex[m_iAsddleIndex] = false;		// 몸체만 그리기


	Set_Scale(0.017f);
	Set_Pos(_vec3(-65.f, 0.f, -21.f));
	m_vMovePos = Get_Pos();

	//// 기본 세팅
	m_iAnimation = ANKYLO_ANIM_IDLE;

	//// 안장 부모 매트릭스
	m_pSaddleFrame = m_pMeshCom->Get_FrameByName(m_pSaddleBoneName);

	// 이구아노돈 능력치
	//m_tStat.fMaxHp = 250.f;
	//m_tStat.fHp = m_tStat.fMaxHp;
	//m_tStat.fAtk = 25.f;
	m_tStat.fMaxStamina = 100.f;
	m_tStat.fStamina = m_tStat.fMaxStamina;
	m_tStat.fKnockBack = 2.f;
	m_tStat.fMaxWeight = 2000.f;
	m_tStat.fWeight = 0.f;
	m_tStat.fMaxHunger = 200.f;
	m_tStat.fHunger = m_tStat.fMaxHunger;
	m_tStat.fMaxThirst = 50.f;
	m_tStat.fThirst = m_tStat.fMaxThirst;
	//m_tStat.fTorpidRate = 5.f;
	//m_tStat.fTorpidRate = 10.f;

	m_tStat.fSpeed = 0.f;
	m_fBackwardSpeed = 5.f;
	m_tStat.fSize = 1.5f;

	// 테이밍 먹이
	m_eTamingFood = ITEMID_AmarBerry;
	//

	//////////////////////////////////////////////////////////////////////////

	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;
	m_iUpperAnimation = ANKYLO_ANIM_IDLE;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_IDLE2;
	m_eNextRandomAnim[2] = STATE_IDLE3;
	m_dIdleMaxTime = rand() % 5;

	m_vfootPoint.y = -2.3f;

	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(3.f, 0.8f, Get_Pos());
	if (nullptr == m_pCtrl)
		return E_FAIL;

	Load_Collider(L"../Resources/Data/Collider/Ankylo_Collider.dat");

	Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"ANKYLO");

	m_fDustScale = 0.007f;

	m_fCenterPos = 1.f;

	return S_OK;
}

_int CDino_Ankylo::Update_GameObject(const _double & _dTimeDelta)
{
	m_eUpperDinoState = STATE_IDLE;
	if (m_eUnitState == STATE_HARVEST)
	{
		_double dPlayTime = m_pMeshCom->Get_AccTime();
		if (dPlayTime > m_dAttackStartTime && dPlayTime < m_dAttackEndTime)
		{
			if (m_pLookObject)
			{
				static_cast<CHarvestable*>(m_pLookObject)->HitBy(this, WEAPON_DINO, _vec3(static_cast<CHarvestable*>(m_pLookObject)->Get_Pos()));
				m_pLookObject = nullptr;
			}
		}
	}

	return CDino::Update_GameObject(_dTimeDelta);
}

_int CDino_Ankylo::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CDino::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CDino_Ankylo::Render_GameObject()
{
	CDino::Render_GameObject();
}

void CDino_Ankylo::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Ankylo::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDino_Ankylo::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	//_bool bHit = CDino::HitBy(pAttacker, dwAttackType, vCollisionPos);
	if (false == CDino::HitBy(pAttacker, dwAttackType, vCollisionPos))
		return false;

	if (!m_bAttackMode && !m_bIsTorpid && STATE_HOLDING != m_eUnitState)
	{
		CComponent* pComponent = pAttacker->Get_Component(L"Com_Transform", ID_DYNAMIC);
		if (nullptr == pComponent)
			return false;

		m_pTartgetTransform = static_cast<CTransform*>(pComponent);
		m_bAttackMode = true;
	}
	else if (m_bDying)
	{
		_int iMeatAmount = 0, iHideAmount = 0;
		_int iAmount = (_int)CRandomFunc::Shake_Float(pAttacker->Get_Stat(STAT_ATTACK), 0.2f);

		if (0 > iAmount)
			iAmount = 1;

		if (WEAPON_HATCHET == dwAttackType)
		{
			// 도끼는 고기를 많이 준다.
			iMeatAmount = _int(iAmount * 0.5f);
			iHideAmount = _int(iAmount * 0.2f);
		}
		else if (WEAPON_STONEPICK == dwAttackType)
		{
			// 곡괭이는 가죽를 많이 준다.
			iMeatAmount = _int(iAmount * 0.2f);
			iHideAmount = _int(iAmount * 0.5f);
		}
		else if (WEAPON_HAND == dwAttackType)
		{
			// 맨손은 고기를 조금준다
			iMeatAmount = _int(iAmount * 0.5f);
			iHideAmount = _int(iAmount * 0.2f);
		}
		else if (WEAPON_BOLA == dwAttackType)
		{
			// 타격 안됌
			return false;
		}
		else
		{
			// 보통 무기
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

		if (0 >= m_pInvenCom->Now_Have_ItemAmount(ITEMID_Meat))
			Set_Dead();
	}

	return true;
}

CDino_Ankylo * CDino_Ankylo::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Ankylo*	pInstance = new CDino_Ankylo(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Ankylo::Clone(void * _pArg)
{
	CDino_Ankylo*	pClone = new CDino_Ankylo(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Ankylo::Free()
{
	CDino::Free();
}

void CDino_Ankylo::PeaceTaming(const _double & _dTimeDelta)
{
	if (m_bPeaceTaming)
	{
		m_fPeaceTamingTime += (_float)_dTimeDelta;

		if (m_fPeaceTamingTime >= 1.0)
		{
			m_fPeaceTamingTime = 0.f;
			m_bPeaceTaming = false;
		}
	}
}

void CDino_Ankylo::IsRide_Set_SaddleMat()
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

	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.7f);
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90.f));
	D3DXMatrixRotationAxis(&matRotY, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	D3DXMatrixTranslation(&matRotZ, 0.f, 5.f, -10.f);

	m_matSaddle = matScale * matRotX * matRotY * matRotZ * matSaddle * m_pTransformCom->Get_WorldMatrix();
	//m_pTransformCom->Set_WorldMatrix(&matSaddle);
}

_bool CDino_Ankylo::Dino_Move_Ride_Custum(const _double & _dTimeDelta)
{
	if (m_ePreUnitState == STATE_ATTACK || m_eUpperDinoState == STATE_ATTACK)
	{
		m_eUnitState = STATE_ATTACK;
		m_tStat.fSpeed = 0.f;
		return false;
	}

	return true;
}

_bool CDino_Ankylo::Harvest_Action(wstring & wstrTargetLayer, CGameObject * pLookObject)
{
	if (pLookObject)
	{
		if (!static_cast<CUnit*>(pLookObject)->Get_Dying())
		{
			if (wstrTargetLayer == L"Layer_Harvest_Rock")
			{
				m_eUpperDinoState = STATE_HARVEST;
				if (m_eUnitState == STATE_SPRINT)
					m_eUnitState = STATE_SPRINT;
				else
					m_eUnitState = m_eUnitState == STATE_MOVE ? STATE_MOVE : STATE_HARVEST;

				m_pLookObject = pLookObject;
				return true;
			}
		}
	}

	return false;
}

void CDino_Ankylo::Check_State()
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
			m_iAnimation = ANKYLO_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE2:
			m_iAnimation = ANKYLO_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE3:
			m_iAnimation = ANKYLO_ANIM_IDLE;
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
			m_iAnimation = rand() % 2 ? ANKYLO_ANIM_ATTACK_TAIL_FOUND : ANKYLO_ANIM_ATTACK_TAIL_SWEEP;
			m_iAnimation = m_bIsRide ? ANKYLO_ANIM_ATTACK_TAIL_SWEEP : m_iAnimation;
			m_bIsAttack = true;
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:			// 기절 시작
			m_iAnimation = ANKYLO_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_TORPID_IDLE:		// 기절중
			m_iAnimation = ANKYLO_ANIM_TORPID_IDLE;
			break;
		case CUnit::STATE_TORPID_TAMED:		// 기절중 일어나기 (테이밍 완료)
			m_iAnimation = ANKYLO_ANIM_TORPID_OUT_TAMED;
			break;
		case CUnit::STATE_TORPID_WILD:		// 기절중 일어나기 (테이밍 실패)
			m_iAnimation = ANKYLO_ANIM_TORPID_OUT_WILD;
			break;
		case CUnit::STATE_TORPID_EAT:		// 기절중 음식먹기
			m_iAnimation = ANKYLO_ANIM_TORPID_EAT;
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = ANKYLO_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			m_iAnimation = ANKYLO_ANIM_ATTACK_TAIL_SWEEP;
			break;
		case CUnit::STATE_RIDE:
			break;
		case CUnit::STATE_ROAR:
			//m_iAnimation = PARA_ANIM_ROAR_ALERT;
			break;

		/// 볼라 맞았을 때 
		case CUnit::STATE_HOLDING:
			m_iAnimation = ANKYLO_ANIM_IDLE;
			m_bIsHolding = true;
		}
	}

	//달리는 방향에 따라서 목 돌리기 (임시)
	if (STATE_MOVE == m_eUnitState || STATE_SPRINT == m_eUnitState)
	{
		switch (m_eUnitMoveDir)
		{
			//case CUnit::UNIT_DIR_CENTER:
			//	break;
		case CUnit::UNIT_DIR_DOWN:
			m_iAnimation = ANKYLO_ANIM_BACKWARD;
			break;
			//case CUnit::UNIT_DIR_DOWN_L:
			//	break;
			//case CUnit::UNIT_DIR_DOWN_R:
			//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = ANKYLO_ANIM_MOVE_FORWARD;
			else
				m_iAnimation = ANKYLO_ANIM_CHARGE_FORWARD;
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
}

HRESULT CDino_Ankylo::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Ankylo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test용
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 40);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Ankylo::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}
