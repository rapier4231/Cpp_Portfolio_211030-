#include "stdafx.h"
#include "Dino_Quetzal.h"
#include "Export_Function.h"
#include "PhysX_Mgr.h"
#include "RandomFunc.h"
#include "QuickSlot_UI.h"
#include "Player.h"
#include "LR_Inventory.h"
#include "Inven.h"
#include "CollisionFunc.h"

USING(Engine)

CDino_Quetzal::CDino_Quetzal(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Quetzal::CDino_Quetzal(const CDino_Quetzal & rhs)
	: CDino(rhs)
	, m_iIndex(rhs.m_iIndex)
{
}

HRESULT CDino_Quetzal::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Quetzal::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	/////////////////////////////
	m_wstrDinoSpecies = L"Quetzal";
	m_wstrDinoSpeciesKor = L"케찰";
	/////////////////////////////
	m_iAsddleIndex = 1;								// 매쉬상에서의 안장 서브셋 번호
	m_pSaddleBoneName = "c_back2";	// 안장 뼈 이름
	m_pNeckBoneName = "c_neck1";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 서브셋만큼 bool 값 동적할당
	m_bIsSaddleIndex = new _bool[m_pMeshCom->Get_MeshList()->size()];
	memset(m_bIsSaddleIndex, 1, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsSaddleIndex[m_iAsddleIndex] = false;		// 몸체만 그리기

	Set_Scale(0.035f);

	//// 기본 세팅
	m_iAnimation = QUETZAL_FLY_FWD;
	m_eDinoPass = SHADER_PASS_NORMAL_CUL;

	//// 안장 부모 매트릭스
	m_pSaddleFrame = m_pMeshCom->Get_FrameByName(m_pSaddleBoneName);

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
	//m_tStat.fTorpidRate = 10.f;

	m_tStat.fSpeed = 0.f;
	m_fBackwardSpeed = 5.f;

	m_tStat.fSize = 1.5f;

	// 테이밍 먹이
	m_eTamingFood = ITEMID_AmarBerry;

	//m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(180.f));
	//////////////////////////////////////////////////////////////////////////
	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;
	m_iUpperAnimation = QUETZAL_FLY_FWD;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_MOVE;
	m_eNextRandomAnim[2] = STATE_MOVE;
	m_dIdleMaxTime = rand() % 1 + 1;

	m_vecBorder[0] = _vec3(340.f, 600.f, 550.f);			
	m_vecBorder[1] = _vec3(-300.f, 400.f, 400.f);
	m_vecBorder[2] = _vec3(230.f, 500.f, 1170.f);
	m_vecBorder[3] = _vec3(-350.f, 450.f, 1200.f);
	m_vecBorder[4] = _vec3(480.f, 580.f, 1630.f);
	m_vecBorder[5] = _vec3(-145.f, 660.f, 1900.f);

	m_iIndex = CRandomFunc::Rand_Int(0, 5);
	Set_Pos(m_vecBorder[m_iIndex]);
	//////////////////////////////////////////////////////////////////////////

	//Set_Pos(_vec3(20.f, 12.f, -40.f));
	Set_Scale(0.03f);
	
	m_bLandDino = false;

	m_vMovePos = Get_Pos();
	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(2.f, 0.8f, Get_Pos());
	if (nullptr == m_pCtrl)
		return E_FAIL;

	//m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(180.f));

	m_QuatDirection = Quaternion::Identity();

	m_bJump = true;

	Load_Collider(L"../Resources/Data/Collider/Quetzal_Collider.dat");
	Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"QUETZAL");

	return S_OK;
}

_int CDino_Quetzal::Update_GameObject(const _double & _dTimeDelta)
{
	//CDino::Update_GameObject(_dTimeDelta);

	return CDino::Update_GameObject(_dTimeDelta);
}

_int CDino_Quetzal::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CDino::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CDino_Quetzal::Render_GameObject()
{
	CDino::Render_GameObject();
}

void CDino_Quetzal::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Quetzal::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDino_Quetzal::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	_bool bHit = CDino::HitBy(pAttacker, dwAttackType, vCollisionPos);
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

CDino_Quetzal * CDino_Quetzal::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Quetzal*	pInstance = new CDino_Quetzal(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Quetzal::Clone(void * _pArg)
{
	CDino_Quetzal*	pClone = new CDino_Quetzal(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Quetzal::Free()
{
	CDino::Free();
}


void CDino_Quetzal::IsRide_Set_SaddleMat()
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
	D3DXMatrixTranslation(&matRotZ, 0.f, 0.f, 0.f);

	m_matSaddle = matScale * matRotX * matRotY * matRotZ * matSaddle * m_pTransformCom->Get_WorldMatrix();
	//m_pTransformCom->Set_WorldMatrix(&matSaddle);
}

_bool CDino_Quetzal::Dino_Move_Ride_Custum(const _double & _dTimeDelta)
{
	if (m_bJump)
	{
		m_tStat.fSpeed = m_eUnitState == STATE_SPRINT ? m_fSprint : m_fSpeed;

		_vec3 vLook{ 0.f,0.f,0.f };
		_float fY = 0.f, RotX = 0.f;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);

		CGameObject* pPlayer = Engine::Get_GameObject(L"Layer_Player");
		if (pPlayer)
			RotX = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player"))->Get_AngleX();

		m_pTransformCom->Set_Rotate(ROT_X, D3DXToRadian(RotX));

		if (m_eUnitState == STATE_RIDE || m_eUnitState == STATE_LANDING)
		{
			vLook = { 0.f, 0.f, 0.f };
			fY += 0.5f;
		}
		else if (m_eUnitState == STATE_IDLE || m_eUnitState == STATE_ATTACK || m_eUnitMoveDir == UNIT_DIR_DOWN)
			m_tStat.fSpeed = 0.f;

		PxVec3 PxPos(0);
		PxPos -= vLook * _float(m_tStat.fSpeed  * _dTimeDelta);
		PxPos.y -= -fY * (_float)_dTimeDelta;

		m_pCtrl->move(PxPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());

		return false;
	}

	if(m_eUnitState == STATE_MOVE || m_eUnitState ==STATE_SPRINT)
		m_tStat.fSpeed = 5.f;

	return true;
}

void CDino_Quetzal::Ride_Key_Input_Custum(const _double & _dTimeDelta)
{
	if (m_ePreUnitState == STATE_LANDING)
		m_eUnitState = STATE_LANDING;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (m_bJump && m_eUnitState != STATE_LANDING)
		{
			if (Ground_Check())
			{
				m_eUnitState = STATE_LANDING;
				m_bJump = false;
			}
		}
		else if (!m_bJump && m_eUnitState != STATE_LANDING)
		{
			m_bJump = true;
			m_eUnitState = STATE_RIDE;
		}
	}

	if (Engine::Get_DIMouseState(DIM_RB) && m_bJump)
	{
		m_bGrab = true;
		if (!m_pGrabObject)
			m_eUnitState = STATE_ATTACK;
	}
	else
	{
		m_bGrab = false;
		if (m_pGrabObject)
		{
			static_cast<CDino*>(m_pGrabObject)->Set_Hanging(false);
			static_cast<CDino*>(m_pGrabObject)->Set_HangingMatrix(nullptr);
			m_pGrabObject = nullptr;
		}
	}
}

void CDino_Quetzal::Dino_Move_Fly(const _double & _dTimeDelta)
{
	if (m_bIsTamed)
	{
		if (m_bJump)
		{
			if (m_pTartgetTransform && !m_bIsAttack)
				m_vMovePos = *m_pTartgetTransform->Get_Pos();

			if (m_eUnitState == STATE_LANDING)
			{
				m_bJump = false;
				return;
			}
			else if (m_eUnitState == STATE_IDLE)
			{
				m_vMovePos = Get_Pos();
				m_bFalling = true;
				m_tStat.fSpeed = 5.f;
			}
			else
			{
				m_vMovePos.y += 7.f;
				m_tStat.fSpeed = m_eUnitState == STATE_SPRINT ? m_fSpeed : 5.f;
				m_bJump = true;
				m_bFalling = false;
			}
		}
		else
		{
			//바닥에서 다시 도약할 때
			if (m_eUnitState == STATE_MOVE || m_eUnitState == STATE_SPRINT)
			{
				m_bJump = true;
				m_bFalling = false;
			}
			// 바닥에 착지
			else if (m_eUnitState == STATE_IDLE)
				return;
		}
	}

	m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(180.f));

	PxVec3 PxPos(0);
	_vec3 vPos = *m_pTransformCom->Get_Pos();

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3 vDir = m_vMovePos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	m_eUnitMoveDir = UNIT_DIR_UP;

	float fDis = m_pCalculatorCom->Get_Distance(&vPos, &m_vMovePos);

	if (fDis <= 10.f)
	{
		CGameObject* pPlayer = Engine::Get_GameObject(L"Layer_Player");
		if (nullptr == pPlayer)
			m_iIndex = CRandomFunc::Rand_Int(0, 5);
		else
		{
			if (static_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos()->z >= 1000.f)
				m_iIndex = CRandomFunc::Rand_Int(2, 5);
			else
				m_iIndex = CRandomFunc::Rand_Int(0, 3);
		}

		// 다음 움직일 포스
		m_vMovePos = m_vecBorder[m_iIndex];
	}
	// 타겟 방향 쿼터니언 생성
	Vector3 targetDir = { (_double)vDir.x, (_double)vDir.y,(_double)vDir.z };
	Quaternion lookRotation = Quaternion::LookRotation(targetDir);

	// 내 현재 이동 방향 쿼터니언 보간
	m_QuatDirection = Quaternion::Slerp(m_QuatDirection, lookRotation, 5.f * _dTimeDelta);

	// 내 이동 방향 쿼터니언을 월드에 적용
	D3DXQUATERNION QuatRot = { (_float)m_QuatDirection.X, (_float)m_QuatDirection.Y, (_float)m_QuatDirection.Z, (_float)m_QuatDirection.W };
	_mat matRotWorld;
	D3DXMatrixRotationQuaternion(&matRotWorld, &QuatRot);

	m_pTransformCom->Set_IsYawPitchRoll(true);
	m_pTransformCom->Set_YawPitchRoll(matRotWorld);

	PxPos -= vLook * _float(m_tStat.fSpeed * _dTimeDelta);

	m_pCtrl->move(PxPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());

	//Ground_Check();
}

void CDino_Quetzal::Check_State()
{
	CDino::Check_IsEndAnimation();
	CDino::Check_IsUpperEndAnimation();

	_vec3 vPos = *m_pTransformCom->Get_Pos();

	if (m_ePreUnitState != m_eUnitState)
	{
		m_ePreUnitState = m_eUnitState;
		m_dAnimationSpeed = 0.0;

		switch (m_eUnitState)
		{
		case CUnit::STATE_IDLE:
		case CUnit::STATE_IDLE2:
		case CUnit::STATE_IDLE3:
			m_iAnimation = m_bJump ? QUETZAL_FLY_FWD : QUETZAL_GROUND_IDLE_PLATFORM;
			break;
		case CUnit::STATE_MOVE:
			if (!m_bJump && !m_bIsRide && !m_bIsTamed)
				m_eUnitState = STATE_IDLE;

			m_tStat.fSpeed = m_fSpeed;
			m_iNextIndex = CRandomFunc::Rand_Int(0, 5);
			// 다음 움직일 포스
			m_vMovePos = m_vecBorder[m_iNextIndex];
			break;
		case CUnit::STATE_SPRINT:
			m_tStat.fSpeed = m_fSprint;
			m_dAnimationSpeed = 0.4;
			break;
		case CUnit::STATE_ATTACK:
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:			// 기절 시작
			m_iAnimation = QUETZAL_TORPID_IN;
			RotationY();
			break;
		case CUnit::STATE_TORPID_IDLE:		// 기절중
			m_iAnimation = QUETZAL_TORPID_IDLE;
			break;
		case CUnit::STATE_TORPID_TAMED:		// 기절중 일어나기 (테이밍 완료)
			m_iAnimation = QUETZAL_TORPID_OUT_TAMED;
			break;
		case CUnit::STATE_TORPID_WILD:		// 기절중 일어나기 (테이밍 실패)
			m_iAnimation = QUETZAL_TORPID_OUT_WILD;
			break;
		case CUnit::STATE_TORPID_EAT:		// 기절중 음식먹기
			m_iAnimation = QUETZAL_TORPID_EAT;
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = QUETZAL_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			//m_iAnimation = QUETZALHARVEST;
			break;
		case CUnit::STATE_RIDE:
			m_iAnimation = QUETZAL_TAKE_OFF_PLATFORM;
			m_bFalling = true;
			break;
		case CUnit::STATE_ROAR:
			//m_iAnimation = PARA_ANIM_ROAR_ALERT;
			break;
			/// 볼라 맞았을 때 
		case CUnit::STATE_HOLDING:
			m_iAnimation = QUETZAL_GROUND_IDLE_PLATFORM;
			m_bIsHolding = true;
			break;
		case CUnit::STATE_LANDING:
			m_iAnimation = QUETZAL_GROUND_LAND_PLATFORM;
			m_bFalling = false;
			if (m_bIsRide)
				m_pTransformCom->Set_Rotate(ROT_X, D3DXToRadian(0.f));
			else
				RotationY();
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
			m_iAnimation = m_bJump ? QUETZAL_FLY_FWD : QUETZAL_GROUND_BACKWARD;
			break;
			//case CUnit::UNIT_DIR_DOWN_L:
			//	break;
			//case CUnit::UNIT_DIR_DOWN_R:
			//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState || STATE_SPRINT == m_eUnitState)
				m_iAnimation = m_bJump ? QUETZAL_FLY_FWD : QUETZAL_GROUND_MOVE_PLATFORM;
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
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_HARVEST:
			//m_iUpperAnimation = QUETZALHARVEST;
			break;
		case CUnit::STATE_INVENTORY:
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

HRESULT CDino_Quetzal::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Quetzal"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test용
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 40);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Quetzal::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}

void CDino_Quetzal::RotationY()
{
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	vLook.y = 0.f;
	//vLook.z = 0.f;

	vLook *= -1.f;

	Vector3 targetDir = { (_double)vLook.x, (_double)vLook.y,(_double)vLook.z };
	Quaternion lookRotation = Quaternion::LookRotation(targetDir);

	// 내 현재 이동 방향 쿼터니언 보간
	m_QuatDirection = Quaternion::Slerp(m_QuatDirection, lookRotation, 1);

	// 내 이동 방향 쿼터니언을 월드에 적용
	D3DXQUATERNION QuatRot = { (_float)m_QuatDirection.X, (_float)m_QuatDirection.Y, (_float)m_QuatDirection.Z, (_float)m_QuatDirection.W };
	_mat matRotWorld;
	D3DXMatrixRotationQuaternion(&matRotWorld, &QuatRot);

	m_pTransformCom->Set_YawPitchRoll(matRotWorld);
}

_bool CDino_Quetzal::Ground_Check()
{
	_float fRad = static_cast<PxCapsuleController*>(m_pCtrl)->getRadius();
	_float fHeight = static_cast<PxCapsuleController*>(m_pCtrl)->getHeight();
	PxVec3 vPos = PxVec3((_float)m_pCtrl->getPosition().x, (_float)m_pCtrl->getPosition().y, (_float)m_pCtrl->getPosition().z);

	// 1. 스태틱 물체에만 착지
	// 2. 맞았을 때는 착지 x(SPRINT)
	if (CPhysX_Mgr::GetInstance()->IsOverlap_Capsule_StaticObject(fRad, fHeight, vPos) && m_bFalling)
	{
		m_bJump = false;
		m_eUnitState = STATE_LANDING;

		return true;
	}

	return false;
}
