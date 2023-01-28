#include "stdafx.h"
#include "Dino_Therizino.h"

#include "Export_Function.h"

#include "Inven.h"
#include "LR_Inventory.h"
#include "QuickSlot_UI.h"

#include "PhysX_Mgr.h"

#include "RandomFunc.h"
#include "Harvestable.h"

USING(Engine)

CDino_Therizino::CDino_Therizino(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Therizino::CDino_Therizino(const CDino_Therizino & rhs)
	: CDino(rhs)
	, m_iIndex(rhs.m_iIndex)
{
}

HRESULT CDino_Therizino::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Therizino::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	// 테리지노 전용 변수
	/////////////////////////////
	m_wstrDinoSpecies = L"Theri";
	m_wstrDinoSpeciesKor = L"테리지노";
	/////////////////////////////
	m_iAsddleIndex = 1;								// 매쉬상에서의 안장 서브셋 번호
	m_pSaddleBoneName = "c_back2";	// 안장 뼈 이름
	m_pNeckBoneName = "c_neck1";
	//m_pNeckBoneName = "Cnt_Head_JNT_SKL"/*"Cnt_Neck_001_JNT_SKL"*//*"Cnt_Spine_004_JNT_SKL"*/;		// 목 뼈 이름

	// 초식이지만 거의 육식급
	m_bCanivore = true;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 서브셋만큼 bool 값 동적할당
	m_bIsSaddleIndex = new _bool[m_pMeshCom->Get_MeshList()->size()];
	memset(m_bIsSaddleIndex, 1, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsSaddleIndex[m_iAsddleIndex] = false;		// 몸체만 그리기

	// 트랜스폼 세팅
	Set_Scale(0.013f);
	m_vMovePos = Get_Pos();
	//// 기본 세팅
	m_iAnimation = THERIZINO_ANIM_IDLE;

	//// 안장 부모 매트릭스
	m_pSaddleFrame = m_pMeshCom->Get_FrameByName(m_pSaddleBoneName);

	// 테리지노 능력치
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
	m_fBackwardSpeed = 5.f;
	m_tStat.fSize = 3.f;

	// 테이밍 먹이
	m_eTamingFood = ITEMID_AmarBerry;
	//

	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;
	m_iUpperAnimation = THERIZINO_ANIM_IDLE;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_IDLE2;
	m_eNextRandomAnim[2] = STATE_IDLE3;
	m_dIdleMaxTime = rand() % 5;

	m_vfootPoint.y = -2.6f;

	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(3.f, 1.f, Get_Pos());
	if (nullptr == m_pCtrl)
		return E_FAIL;

	Load_Collider(L"../Resources/Data/Collider/Therizino_Collider.dat");
	Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"THERIZINO");

	m_fDustScale = 0.005f;

	m_fCenterPos = 3.f;

	return S_OK;	
}

_int CDino_Therizino::Update_GameObject(const _double & _dTimeDelta)
{
	//Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"THERIZINO");

	if (!m_bIsTamed && !m_bIsRide)
	{
		m_fDetectedRange = 10.f;
		if (m_vReturnPos.x != 0.f)
			m_fDetectedRange = 40.f;
	
		m_bAttackMode = true;
	
		if (m_eUnitState == STATE_HOLDING)
			m_eUnitState = STATE_MOVE;
	}

	return CDino::Update_GameObject(_dTimeDelta);;
}

_int CDino_Therizino::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (m_bIsTamed)
		m_bEatDodo = false;

	CDino::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CDino_Therizino::Render_GameObject()
{
	CDino::Render_GameObject();
}

void CDino_Therizino::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Therizino::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDino_Therizino::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
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

CDino_Therizino * CDino_Therizino::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Therizino*	pInstance = new CDino_Therizino(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Therizino::Clone(void * _pArg)
{
	CDino_Therizino*	pClone = new CDino_Therizino(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Therizino::Free()
{
	CDino::Free();
}

_bool CDino_Therizino::Before_Sort_TagetList()
{
	if (!m_bPeaceTaming)
	{
		_bool bIsDodo = false;
		CDino* pDodo = nullptr;
		// 공격리스트에 도도 있는 지 확인
		if (!m_ListTargetUnit.empty())
		{
			for (auto& data : m_ListTargetUnit)
			{
				if(static_cast<CUnit*>(data)->Get_UnitType() == CUnit::UNIT_PLAYER)
					continue;

				if (L"Dodo" == static_cast<CDino*>(data)->Get_DinoSpecies())
				{
					bIsDodo = true;
					pDodo = static_cast<CDino*>(data);
					break;
				}
			}
		}

		//만약 도도가 없다면 추가
		if (!bIsDodo)
		{
			list<CGameObject*> plist = *Get_LayerList(L"Layer_Dino_Tamed");
			if (!plist.empty())
			{
				for (auto& Dino : plist)
				{
					if (L"Dodo" == static_cast<CDino*>(Dino)->Get_DinoSpecies())
					{
						CTransform* pDodo = static_cast<CTransform*>(Dino->Get_Component(L"Com_Transform", ID_DYNAMIC));

						_float fDis = m_pCalculatorCom->Get_Distance(pDodo->Get_Pos(), &Get_Pos());

						if (fDis <= 10.f)
						{
							auto& iter_find = find(m_ListTargetUnit.begin(), m_ListTargetUnit.end(), Dino);

							if (iter_find == m_ListTargetUnit.end())
							{
								m_ListTargetUnit.emplace_back(Dino);
								m_pTartgetTransform = pDodo;
							}

							m_bEatDodo = true;
							return true;
						}
					}
				}
			}
		}
		else
		// 도도가 있다면
		{
			// 도도가 죽지않았을때
			if (!pDodo->Get_Dead())
				return true;
		}
	}

	m_bEatDodo = false;

	return false;
}

void CDino_Therizino::PeaceTaming(const _double & _dTimeDelta)
{
	if (m_bPeaceTaming)
	{
		m_fPeaceTamingTime += (_float)_dTimeDelta;

		if (m_fPeaceTamingTime >= 5.0)
		{
			m_fPeaceTamingTime = 0.f;
			m_bPeaceTaming = false;
		}
	}
}

void CDino_Therizino::IsRide_Set_SaddleMat()
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
	D3DXMatrixTranslation(&matRotZ, 50.f, 0.f, -55.f);

	m_matSaddle = matScale * matRotX * matRotY * matRotZ * matSaddle * m_pTransformCom->Get_WorldMatrix();
	//m_pTransformCom->Set_WorldMatrix(&matSaddle);
}

_bool CDino_Therizino::Harvest_Action(wstring& wstrTargetLayer, CGameObject* pLookObject)
{
	if (pLookObject)
	{
		if (!static_cast<CUnit*>(pLookObject)->Get_Dying())
		{
			if (wstrTargetLayer == L"Layer_Harvest_Bush")
			{
				m_eUpperDinoState = STATE_HARVEST;
				if (m_eUnitState == STATE_SPRINT)
					m_eUnitState = STATE_SPRINT;
				else
					m_eUnitState = m_eUnitState == STATE_MOVE ? STATE_MOVE : STATE_HARVEST;

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

void CDino_Therizino::Check_State()
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
			m_iAnimation = THERIZINO_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE2:
			m_iAnimation = THERIZINO_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE3:
			m_iAnimation = THERIZINO_ANIM_IDLE;
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
			m_iAnimation = rand() % 3;
			if (m_bEatDodo)
				m_iAnimation = THERIZINO_ANIM_ATTACK_BITE;
			else
			{
				if (m_iAnimation == 0)
				{
					m_iAnimation = THERIZINO_ANIM_ATTACK_BITE;
					m_tStat.fKnockBack = 2.f;
				}
				else if (m_iAnimation == 1)
				{
					m_iAnimation = THERIZINO_ANIM_ATTACK_CLAW1;
					m_tStat.fKnockBack = 4.f;
				}
				else
				{
					m_iAnimation = THERIZINO_ANIM_ATTACK_CLAW2;
					m_tStat.fKnockBack = 4.f;
				}
			}
			m_bIsAttack = true;
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:			// 기절 시작
			m_iAnimation = THERIZINO_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_TORPID_IDLE:		// 기절중
			m_iAnimation = THERIZINO_ANIM_TORPID_IDLE;
			break;
		case CUnit::STATE_TORPID_TAMED:		// 기절중 일어나기 (테이밍 완료)
			m_iAnimation = THERIZINO_ANIM_TORPID_OUT_TAMED;
			break;
		case CUnit::STATE_TORPID_WILD:		// 기절중 일어나기 (테이밍 실패)
			m_iAnimation = THERIZINO_ANIM_TORPID_OUT_WILD;
			break;
		case CUnit::STATE_TORPID_EAT:		// 기절중 음식먹기
			m_iAnimation = THERIZINO_ANIM_TORPID_EAT;
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = THERIZINO_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			m_iAnimation = THERIZINO_ANIM_STOP_HARVEST;
			break;
		case CUnit::STATE_RIDE:
			break;
		case CUnit::STATE_ROAR:
			break;

		/// 볼라 맞았을 때 
		case CUnit::STATE_HOLDING:
			m_iAnimation = THERIZINO_ANIM_IDLE;
			m_bIsHolding = true;
			break;
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
			m_iAnimation = THERIZINO_ANIM_BACKWARD;
			break;
		//case CUnit::UNIT_DIR_DOWN_L:
		//	break;
		//case CUnit::UNIT_DIR_DOWN_R:
		//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = THERIZINO_ANIM_MOVE_FORWARD;
			else
				m_iAnimation = THERIZINO_ANIM_CHARGE_FORWARD;
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


	// 부분 애니메이션 체크
	if (m_ePreUpperDinoState != m_eUpperDinoState)
	{
		m_ePreUpperDinoState = m_eUpperDinoState;

		switch (m_eUpperDinoState)
		{
		case CUnit::STATE_ATTACK:
			m_tStat.fKnockBack = 2.f;
			m_iUpperAnimation = rand() % 3;
			if (m_bEatDodo)
				m_iUpperAnimation = THERIZINO_ANIM_ATTACK_BITE;
			else
			{
				if (m_iUpperAnimation == 0)
					m_iUpperAnimation = THERIZINO_ANIM_ATTACK_BITE;
				else if (m_iUpperAnimation == 1)
				{
					m_iUpperAnimation = THERIZINO_ANIM_ATTACK_CLAW1;
					m_tStat.fKnockBack = 4.f;
				}
				else
				{
					m_iUpperAnimation = THERIZINO_ANIM_ATTACK_CLAW2;
					m_tStat.fKnockBack = 4.f;
				}
			}

			m_bIsAttack = true;
			break;
		case CUnit::STATE_HARVEST:
			m_iUpperAnimation = THERIZINO_ANIM_MOVE_HARVEST;
			break;
		default:
			break;
		}
	}
}

HRESULT CDino_Therizino::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Therizino"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test용
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 60);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Therizino::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}
