#include "stdafx.h"
#include "Dino_Dodo.h"

#include "Export_Function.h"

#include "Inven.h"
#include "LR_Inventory.h"
#include "QuickSlot_UI.h"

#include "PhysX_Mgr.h"

#include "RandomFunc.h"
#include "Player.h"

USING(Engine)

CDino_Dodo::CDino_Dodo(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Dodo::CDino_Dodo(const CDino_Dodo & rhs)
	: CDino(rhs)
	, m_iIndex(rhs.m_iIndex)
{
}

HRESULT CDino_Dodo::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Dodo::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	m_wstrDinoSpecies = L"Dodo";
	m_wstrDinoSpeciesKor = L"도도";

	// 렉스 전용 변수
	m_iAsddleIndex = 1;								// 매쉬상에서의 안장 서브셋 번호
	m_pSaddleBoneName = "c_back3";	// 안장 뼈 이름
	m_pNeckBoneName = "c_neck1";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_Scale(0.015f);
	m_vMovePos = Get_Pos();
	//// 기본 세팅
	m_iAnimation = DODO_ANIM_IDLE;

	//// 안장 부모 매트릭스
	m_pSaddleFrame = m_pMeshCom->Get_FrameByName(m_pSaddleBoneName);

	// 렉스 능력치
	//m_tStat.fMaxHp = 100.f;
	//m_tStat.fHp = m_tStat.fMaxHp;
	//m_tStat.fAtk = 50.f;
	//m_tStat.fMaxParalysis = 30.f;
	//m_tStat.fParalysis = 50.f;
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

	m_tStat.fSize = 0.5f;

	m_bCanbeRide = false;

	// 테이밍 먹이
	m_eTamingFood = ITEMID_AmarBerry;

	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;
	m_iAnimation = DODO_ANIM_IDLE;
	m_iUpperAnimation = DODO_ANIM_IDLE;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_IDLE2;
	m_eNextRandomAnim[2] = STATE_IDLE3;
	m_dIdleMaxTime = rand() % 5;

	m_vfootPoint.y = -1.5f;
	
	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(1.f, 0.8f, Get_Pos());
	if (nullptr == m_pCtrl)
		return E_FAIL;

	Load_Collider(L"../Resources/Data/Collider/DODO_Collider.dat");

	Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"DODO");

	m_fDustScale = 0.f;

	return S_OK;
}

_int CDino_Dodo::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead && m_pAttacker)
	{
		m_pAttacker->Delete_UnitList(this);
		m_pAttacker->Set_PeaceTaming(true);
		m_pAttacker = nullptr;
	}

	if (!m_bIsRide && !m_bIsTamed)
		m_bAttackMode = false;

	return CDino::Update_GameObject(_dTimeDelta);
}

_int CDino_Dodo::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CDino::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CDino_Dodo::Render_GameObject()
{
	CDino::Render_GameObject();

	//IsRide_Set_SaddleMat();
}

void CDino_Dodo::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Dodo::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDino_Dodo::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3 & vCollisionPos)
{
	if (false == CDino::HitBy(pAttacker, dwAttackType, vCollisionPos))
		return false;

	if (!m_bIsTorpid && !m_bDying && STATE_HOLDING != m_eUnitState)
		m_eUnitState = STATE_SPRINT;
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

		// 공룡 공격일 경우
		if (dwAttackType == ATTACK_OPTION_DINO_ATTACK)
		{
			if (static_cast<CDino*>(pAttacker)->Get_NowTamingState())
				Reduce_Item(iMeatAmount, iHideAmount, pAttacker, dwAttackType);
			// 야생끼리 잡아먹었을 때
			else
			{
				Remove_Item(ITEMID::ITEMID_Meat, iMeatAmount).iAmount;

				if (0 >= m_pInvenCom->Now_Have_ItemAmount(ITEMID_Meat))
				{
					Set_Dead();

					if (dwAttackType == ATTACK_OPTION_DINO_ATTACK)
					{
						if (L"Theri" == static_cast<CDino*>(pAttacker)->Get_DinoSpecies())
							m_pAttacker = static_cast<CDino*>(pAttacker);
					}
				}
			}
		}
		else
		{
			Reduce_Item(iMeatAmount, iHideAmount, pAttacker, dwAttackType);
		}
	}

	return true;
}

CDino_Dodo * CDino_Dodo::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Dodo*	pInstance = new CDino_Dodo(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Dodo::Clone(void * _pArg)
{
	CDino_Dodo*	pClone = new CDino_Dodo(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Dodo::Free()
{
	CDino::Free();
}

void CDino_Dodo::HangingMatrix()
{
	m_pCtrl->setPosition(PxExtendedVec3(0.f, 0.f, 0.f));

	// 플레이어 손뼈 포인터 가져오기
	if (m_pBoneMatrix == nullptr)
	{
		CDynamicMesh*	pPlayerMeshCom = static_cast<CDynamicMesh*>(Engine::CManagement::GetInstance()->Get_Component(L"Layer_Player", 0, L"Com_Mesh", ID_STATIC));
		const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("c_R_weapon_jnt");
		m_pBoneMatrix = &pFrame->CombinedTransformMatrix;

		CTransform*	pPlayerTransformCom = static_cast<CTransform*>(static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0))->Get_Component(L"Com_Transform", ID_DYNAMIC));
		m_pParentWorld = &pPlayerTransformCom->Get_WorldMatrix();
	}

	_mat matWorld, matScale, matTrans;
	matWorld = *m_pBoneMatrix * *m_pParentWorld;

	m_pTransformCom->Set_ParentMatrix(&matWorld);

	 _vec3 vPos;
	memcpy(&vPos, &matWorld.m[INFO_POS][0], sizeof(_vec3));
	Set_Pos(vPos);

	m_pTransformCom->Update_Component(0);
}

void CDino_Dodo::Drop()
{
	m_pBoneMatrix = nullptr;
	m_bHanging = false;

	_vec3 vPos;
	memcpy(&vPos, &m_pTransformCom->Get_WorldMatrix().m[INFO_POS][0], sizeof(_vec3));
	m_pCtrl->setPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
}

void CDino_Dodo::IsRide_Set_SaddleMat()
{
	_mat matSaddle, matScale, matRotX, matRotY, matRotZ;
	matSaddle = m_pSaddleFrame->CombinedTransformMatrix;
	
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90.f));
	D3DXMatrixRotationAxis(&matRotY, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	D3DXMatrixTranslation(&matRotZ, 20.f , 0.f, 0.f);
	
	m_matSaddle = matScale * matRotX * matRotY * matRotZ * matSaddle * m_pTransformCom->Get_WorldMatrix();
}

void CDino_Dodo::Reduce_Item(_int iMeat, _int iHide, CUnit* pAttacker, _ulong dwAttackType)
{
	if (0 < iMeat)
	{
		_int iMeatRemoved = Remove_Item(ITEMID::ITEMID_Meat, iMeat).iAmount;
		static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_Meat, iMeatRemoved);
		static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(iMeatRemoved * 2.f);
	}
	if (0 < iHide)
	{
		static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_Hide, iHide);
		static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(iHide * 2.f);
	}

	if (0 >= m_pInvenCom->Now_Have_ItemAmount(ITEMID_Meat))
		Set_Dead();
}

void CDino_Dodo::Check_State()
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
			m_iAnimation = DODO_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE2:
			m_iAnimation = DODO_ANIM_CUDDLE;
			break;
		case CUnit::STATE_IDLE3:
			m_iAnimation = DODO_ANIM_GRAZE;
			break;
		case CUnit::STATE_MOVE:
			m_tStat.fSpeed = m_fSpeed;
			vPos.x += CRandomFunc::Rand_Int(-60, 60);
			vPos.z += CRandomFunc::Rand_Int(-60, 60);
			m_vMovePos = vPos;
			break;
		case CUnit::STATE_SPRINT:
			m_fSprint = 5.f;
			m_tStat.fSpeed = m_fSprint;
			break;
		//case CUnit::STATE_ATTACK:
		//	m_iAnimation = REX_A_BITE;
		//	m_bIsAttack = true;
		//	break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:
			m_iAnimation = DODO_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_TORPID_IDLE:
			m_iAnimation = DODO_ANIM_TORPID_IDLE;
			break;
		case CUnit::STATE_TORPID_WILD:
			m_iAnimation = DODO_ANIM_TORPID_OUT_WILD;
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = DODO_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			break;
		case CUnit::STATE_RIDE:
			break;
		case CUnit::STATE_ROAR:
			break;
		/// 볼라 맞았을 때 
		case CUnit::STATE_HOLDING:
			m_iAnimation = DODO_ANIM_IDLE;
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
			m_iAnimation = DODO_ANIM_MOVE_BACKWARD;
			break;
		//case CUnit::UNIT_DIR_DOWN_L:
		//	break;
		//case CUnit::UNIT_DIR_DOWN_R:
		//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = DODO_ANIM_MOVE_FWORD;
			else
				m_iAnimation = DODO_ANIM_CHARGE_FOWARD;
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
		case CUnit::STATE_IDLE:
			break;
		case CUnit::STATE_MOVE:
			break;
		case CUnit::STATE_SPRINT:
			break;
		case CUnit::STATE_JUMPING:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_LANDING:
			break;
		case CUnit::STATE_ATTACK:
			m_iUpperAnimation = DODO_ANIM_ATTACK;
			m_bIsAttack = true;
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_HARVEST:
			break;
		case CUnit::STATE_INVENTORY:
			break;
		case CUnit::STATE_DEAD:
			break;
		case CUnit::STATE_TORPID:
			break;
		case CUnit::STATE_RIDE:
			break;
		case CUnit::STATE_END:
			break;
		default:
			break;
		}

	}
}

HRESULT CDino_Dodo::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Dodo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test용
	//고기
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 10);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Dodo::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}
