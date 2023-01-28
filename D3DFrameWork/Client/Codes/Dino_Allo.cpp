#include "stdafx.h"
#include "Dino_Allo.h"

#include "Export_Function.h"

#include "Inven.h"
#include "LR_Inventory.h"
#include "QuickSlot_UI.h"

#include "PhysX_Mgr.h"

#include "RandomFunc.h"

USING(Engine)

CDino_Allo::CDino_Allo(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Allo::CDino_Allo(const CDino_Allo & rhs)
	: CDino(rhs)
	, m_iIndex(rhs.m_iIndex)
{
}

HRESULT CDino_Allo::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Allo::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	// 알로사우루스 전용 변수
	/////////////////////////////
	m_wstrDinoSpecies = L"Allo";
	m_wstrDinoSpeciesKor = L"알로사우르스";
	/////////////////////////////
	m_iAsddleIndex = 1;					// 매쉬상에서의 안장 서브셋 번호
	m_pSaddleBoneName = "c_back4";		// 안장 뼈 이름
	m_pNeckBoneName = "c_neck1";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 서브셋만큼 bool 값 동적할당
	m_bIsSaddleIndex = new _bool[m_pMeshCom->Get_MeshList()->size()];
	memset(m_bIsSaddleIndex, 1, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsSaddleIndex[m_iAsddleIndex] = false;		// 몸체만 그리기

	//육식 판별 변수
	m_bCanivore = true;
	
	// 트랜스폼 세팅
	Set_Scale(0.012f);

	m_vMovePos = Get_Pos();
	//// 기본 세팅
	m_iAnimation = Allo_ANIM_IDLE;

	//// 안장 부모 매트릭스
	m_pSaddleFrame = m_pMeshCom->Get_FrameByName(m_pSaddleBoneName);

	//m_tStat.fMaxHp = 200;
	//m_tStat.fHp = m_tStat.fMaxHp;
	//m_tStat.fAtk = 50.f;
	m_tStat.fMaxStamina = 100;
	m_tStat.fStamina = m_tStat.fMaxStamina;
	m_tStat.fKnockBack = 2.f;
	m_tStat.fMaxWeight = 2000.f;
	m_tStat.fWeight = 20.f;
	m_tStat.fMaxHunger = 200.f;
	m_tStat.fHunger = m_tStat.fMaxHunger;
	m_tStat.fMaxThirst = 50.f;
	m_tStat.fThirst = m_tStat.fMaxThirst;
	

	m_tStat.fSpeed = 0.f;
	m_fBackwardSpeed = 5.f;
	m_tStat.fSize = 3.f;

	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;
	m_iUpperAnimation = Allo_ANIM_IDLE;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_IDLE2;
	m_eNextRandomAnim[2] = STATE_IDLE3;
	m_dIdleMaxTime = rand() % 5;

	m_vfootPoint.y = -2.6f;
	
	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(3.f, 1.f, Get_Pos());
	if (nullptr == m_pCtrl)
		return E_FAIL;

	Load_Collider(L"../Resources/Data/Collider/Allo_Collider.dat");

	Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"ALLO");

	m_fDustScale = 0.005f;

	return S_OK;
}

_int CDino_Allo::Update_GameObject(const _double & _dTimeDelta)
{
	m_fCenterPos = 3.f;
	//Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"ALLO");

	if (!m_bIsTamed && !m_bIsRide)
	{
		m_bAttackMode = true;

		if (m_eUnitState == STATE_HOLDING)
			m_eUnitState = STATE_MOVE;
	}

	return CDino::Update_GameObject(_dTimeDelta);;
}

_int CDino_Allo::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CDino::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CDino_Allo::Render_GameObject()
{
	CDino::Render_GameObject();
}

void CDino_Allo::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Allo::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDino_Allo::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
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

		if (0 >= static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Now_Have_ItemAmount(ITEMID_Meat))
			Set_Dead();

		//if (0 >= m_pInvenCom->Get_pInvenItem()->size())
		//	Set_Dead();
	}

	return true;
}

CDino_Allo * CDino_Allo::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Allo*	pInstance = new CDino_Allo(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Allo::Clone(void * _pArg)
{
	CDino_Allo*	pClone = new CDino_Allo(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Allo::Free()
{
	CDino::Free();
}


void CDino_Allo::IsRide_Set_SaddleMat()
{
	/*WCHAR str[255] = L"";
	GetPrivateProfileString(L"RECT", L"x", 0, str, 255, L"../Resources/text.ini");
	wstring ss = str;
	float x = stof(ss);
	GetPrivateProfileString(L"RECT", L"y", 0, str, 255, L"../Resources/text.ini");
	ss = str;
	float y = stof(ss);
	GetPrivateProfileString(L"RECT", L"z", 0, str, 255, L"../Resources/text.ini");
	ss = str;
	float z = stof(ss);

	GetPrivateProfileString(L"RECT", L"q", 0, str, 255, L"../Resources/text.ini");
	ss = str;
	float q = stof(ss);
	GetPrivateProfileString(L"RECT", L"w", 0, str, 255, L"../Resources/text.ini");
	ss = str;
	float w = stof(ss);*/

	_mat matSaddle, matScale, matRotX, matRotY, matRotZ;
	matSaddle = m_pSaddleFrame->CombinedTransformMatrix;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90.f));
	D3DXMatrixRotationAxis(&matRotY, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	D3DXMatrixTranslation(&matRotZ, 0.f, 0.f , -40.f);

	m_matSaddle = matScale * matRotX * matRotY * matRotZ * matSaddle * m_pTransformCom->Get_WorldMatrix();
	//m_pTransformCom->Set_WorldMatrix(&matSaddle);
}

void CDino_Allo::Check_State()
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
			m_iAnimation = Allo_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE2:
			m_iAnimation = Allo_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE3:
			m_iAnimation = Allo_ANIM_IDLE;
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
			m_tStat.fKnockBack = 2.f;
			m_iAnimation = rand() % 2 ? Allo_ANIM_ATTACK_BITE: Allo_ANIM_ATTACK_GORE;
			if (m_iAnimation == Allo_ANIM_ATTACK_GORE)
				m_tStat.fKnockBack = 5.f;
			m_bIsAttack = true;
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:			// 기절 시작
			m_iAnimation = Allo_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_TORPID_IDLE:		// 기절중
			m_iAnimation = Allo_ANIM_TORPID_IDLE;
			break;
		case CUnit::STATE_TORPID_TAMED:		// 기절중 일어나기 (테이밍 완료)
			m_iAnimation = Allo_ANIM_TORPID_OUT_TEIMED;
			break;
		case CUnit::STATE_TORPID_WILD:		// 기절중 일어나기 (테이밍 실패)
			m_iAnimation = Allo_ANIM_TORPID_OUT_WILD;
			break;
		case CUnit::STATE_TORPID_EAT:		// 기절중 음식먹기
			m_iAnimation = Allo_ANIM_TORPID_EAT;
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = Allo_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			break;
		case CUnit::STATE_RIDE:
			break;
		case CUnit::STATE_ROAR:
			break;

		/// 볼라 맞았을 때 
		case CUnit::STATE_HOLDING:
			m_iAnimation = Allo_ANIM_IDLE;
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
			m_iAnimation = Allo_ANIM_BACKWARD;
			break;
		//case CUnit::UNIT_DIR_DOWN_L:
		//	break;
		//case CUnit::UNIT_DIR_DOWN_R:
		//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = Allo_ANIM_MOVE_FORWARD;
			else
				m_iAnimation = Allo_ANIM_CHARGE_FORWARD;
			break;
		case CUnit::UNIT_DIR_UP_L:
				m_iAnimation = Allo_ANIM_MOVE_L;
			break;
		case CUnit::UNIT_DIR_UP_R:
				m_iAnimation = Allo_ANIM_MOVE_R;
			break;
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
			m_tStat.fKnockBack = 2.f;
			m_iUpperAnimation = rand() % 2 ? Allo_ANIM_ATTACK_BITE : Allo_ANIM_ATTACK_GORE;
			if (m_iUpperAnimation == Allo_ANIM_ATTACK_GORE)
				m_tStat.fKnockBack = 5.f;
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

HRESULT CDino_Allo::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Allo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test용
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 70);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Allo::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}
