#include "stdafx.h"
#include "Dino_Ptero.h"
#include "Export_Function.h"
#include "Inven.h"
#include "LR_Inventory.h"
#include "QuickSlot_UI.h"
#include "PhysX_Mgr.h"
#include "RandomFunc.h"
#include "Harvestable.h"
#include "Player.h"
#include "CollisionFunc.h"

USING(Engine)

CDino_Ptero::CDino_Ptero(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Ptero::CDino_Ptero(const CDino_Ptero & rhs)
	: CDino(rhs)
{
}

HRESULT CDino_Ptero::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Ptero::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// �̱��Ƴ뵷 ���� ����
	/////////////////////////////
	m_wstrDinoSpecies = L"Ptero";
	m_wstrDinoSpeciesKor = L"���׶�뵷";
	/////////////////////////////
	m_iAsddleIndex = 1;								// �Ž��󿡼��� ���� ����� ��ȣ
	m_pSaddleBoneName = "c_back3";	// ���� �� �̸�
	m_pNeckBoneName = "c_neck1";


	// ����¸�ŭ bool �� �����Ҵ�
	m_bIsSaddleIndex = new _bool[m_pMeshCom->Get_MeshList()->size()];
	memset(m_bIsSaddleIndex, 1, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsSaddleIndex[m_iAsddleIndex] = false;		// ��ü�� �׸���

	//// �⺻ ����
	m_iAnimation = PTERO_ANIM_GROUND_IDLE;
	m_bLandDino = false;

	//// ���� �θ� ��Ʈ����
	m_pSaddleFrame = m_pMeshCom->Get_FrameByName(m_pSaddleBoneName);

	// �̱��Ƴ뵷 �ɷ�ġ
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

	// ���̹� ����
	m_eTamingFood = ITEMID_AmarBerry;
	//

	//////////////////////////////////////////////////////////////////////////

	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;
	m_iUpperAnimation = PTERO_ANIM_GROUND_IDLE;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_MOVE;
	m_eNextRandomAnim[2] = STATE_IDLE;
	m_dIdleMaxTime = rand() % 5;

	m_vfootPoint.y = -1.3f;

	/*_float fRandY[16];
	for(int i=0; i<16; i++)
		fRandY[i] = (_float)CRandomFunc::Rand_Int(-60, 60);*/

	m_vecBorder[0]  = { 80.f,	-60.f,	376.f };
	m_vecBorder[1]  = { -480.f,	-60.f,	350.f };
	m_vecBorder[2]  = { 70.f,	-60.f,	638.f };
	m_vecBorder[3]  = { -430.f,	-60.f,	700.f };

	m_vecBorder[4]  = { 80.f,	  0.f,	376.f };
	m_vecBorder[5]  = { -480.f,	  0.f,	350.f };
	m_vecBorder[6]  = { 70.f,	  0.f,	638.f };
	m_vecBorder[7]  = { -430.f,	  0.f,	700.f };

	m_vecBorder[8]  = { -400.f, -60.f,	366.f };
	m_vecBorder[9]  = { -240.f, -60.f,	400.f };
	m_vecBorder[10] = { -442.f, -60.f,	430.f };
	m_vecBorder[11] = { -270.f, -60.f,	436.f };

	m_vecBorder[12] =  { -400.f,  0.f,	366.f };
	m_vecBorder[13] =  { -240.f,  0.f,	400.f };
	m_vecBorder[14] =  { -442.f,  0.f,	430.f };
	m_vecBorder[15] =  { -270.f,  0.f,	436.f };

	m_iNextIndex = CRandomFunc::Rand_Int(0, 15);

	Set_Scale(0.012f);
	Set_Pos(m_vecBorder[m_iNextIndex]);
	m_vMovePos = Get_Pos();

	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(2.f, 0.8f, Get_Pos());
	if (nullptr == m_pCtrl)
		return E_FAIL;

	Load_Collider(L"../Resources/Data/Collider/Ptero_Collider.dat");

	Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"PTERO");

	m_bJump = true;

	m_QuatDirection = Quaternion::Identity();

	m_fCenterPos = 1.f;

	return S_OK;
}

_int CDino_Ptero::Update_GameObject(const _double & _dTimeDelta)
{
	//m_vMovePos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos());

	if(!m_bIsRide && !m_bIsTamed)
	{
		m_bAttackMode = false;

		if (m_ePreUnitState == STATE_LANDING && !m_bIsTorpid)
			m_eUnitState = STATE_LANDING;
		else if (m_bIsTorpid)
		{
			m_bJump = false;
			m_bFalling = true;
			m_fFlyTime = 0.f;
		}
		else
		{
			m_fFlyTime += (_float)_dTimeDelta;

			if (m_fFlyTime >= 30.f && !m_bFalling)
			{
				m_fFlyTime = 0.f;
				m_bFalling = true;
			}

			if (m_fFlyTime >= 5.f && m_eUnitState == STATE_IDLE)
			{
				m_fFlyTime = 0.f;
				m_eUnitState = STATE_MOVE;
				m_bFalling = false;
				m_bJump = true;
			}
		}
	}

	return CDino::Update_GameObject(_dTimeDelta);
}

_int CDino_Ptero::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CDino::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CDino_Ptero::Render_GameObject()
{
	CDino::Render_GameObject();
}

void CDino_Ptero::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Ptero::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDino_Ptero::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	//_bool bHit = CDino::HitBy(pAttacker, dwAttackType, vCollisionPos);
	if (false == CDino::HitBy(pAttacker, dwAttackType, vCollisionPos))
		return false;

	if (!m_bIsTorpid && !m_bDying && STATE_HOLDING != m_eUnitState && !m_bIsTamed)
	{
		m_eUnitState = STATE_SPRINT;

		if (!m_bJump || m_bFalling)
		{
			m_bJump = true;
			m_bFalling = false;
			m_fFlyTime = 0.f;
		}

		m_iNextIndex = CRandomFunc::Rand_Int(0, 15);
		m_vMovePos = m_vecBorder[m_iNextIndex];
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

		if (0 >= m_pInvenCom->Now_Have_ItemAmount(ITEMID_Meat))
			Set_Dead();
	}

	return true;
}

CDino_Ptero * CDino_Ptero::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Ptero*	pInstance = new CDino_Ptero(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Ptero::Clone(void * _pArg)
{
	CDino_Ptero*	pClone = new CDino_Ptero(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Ptero::Free()
{
	CDino::Free();
}


void CDino_Ptero::IsRide_Set_SaddleMat()
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
	D3DXMatrixTranslation(&matRotZ, -20.f, 0.f, -70.f);

	m_matSaddle = matScale * matRotX * matRotY * matRotZ * matSaddle * m_pTransformCom->Get_WorldMatrix();
	//m_pTransformCom->Set_WorldMatrix(&matSaddle);
}

_bool CDino_Ptero::Dino_Move_Ride_Custum(const _double & _dTimeDelta)
{
	//m_tStat.fSpeed = 10.f;
	if (m_bJump)
	{
		if (m_bGrab && !m_pGrabObject)
		{
			_double dPlayTime = m_pMeshCom->Get_AccTime();
			if (dPlayTime > 1.1 && dPlayTime < 1.3 && m_eUnitState == STATE_ATTACK)
			{
				list<CSphereCollider::SPHEREINFO>* pAttackCollist = m_pSphereColliderCom->Get_listCollider(CSphereCollider::COL_ATTACK);

				if (nullptr == pAttackCollist)
					return false;

				for (auto& pAttackCol : *pAttackCollist)
				{
					_vec3 vWorldPos;
					memcpy(&vWorldPos, &pAttackCol.matColMatrix.m[INFO_POS][0], sizeof(_vec3));

					CCollisionFunc::AttackOnUnit(this, &vWorldPos, &pAttackCol.fRadius, L"Layer_Dino_Tamed", CSphereCollider::COL_HIT, ATTACK_OPTION_DINO_GRAB, nullptr, nullptr, false, nullptr, &m_pGrabObject);
				}
			}

			m_eUnitState = STATE_ATTACK;
			m_tStat.fSpeed = 0.f;

			return false;
		}

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
		else if (m_eUnitState == STATE_IDLE || m_eUnitState == STATE_ATTACK)
			m_tStat.fSpeed = 0.f;
		else if (m_eUnitMoveDir == UNIT_DIR_DOWN)
			m_tStat.fSpeed = 0.f;

		PxVec3 PxPos(0);
		PxPos -= vLook * _float(m_tStat.fSpeed  * _dTimeDelta);
		PxPos.y -= -fY * (_float)_dTimeDelta;

		m_pCtrl->move(PxPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());

		return false;
	}
	
	if (m_eUnitState == STATE_MOVE || m_eUnitState == STATE_SPRINT)
		m_tStat.fSpeed = 5.f;

	return true;
}

void CDino_Ptero::Ride_Key_Input_Custum(const _double & _dTimeDelta)
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
		if(!m_pGrabObject)
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

void CDino_Ptero::Dino_Move_Fly(const _double & _dTimeDelta)
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
			//�ٴڿ��� �ٽ� ������ ��
			if (m_eUnitState == STATE_MOVE || m_eUnitState == STATE_SPRINT)
			{
				m_bJump = true;
				m_bFalling = false;
			}
			// �ٴڿ� ����
			else if (m_eUnitState == STATE_IDLE)
				return;
		}
	}

	if (!m_bIsTamed && !m_bIsRide)
		m_fSpeed = 10.f;

	m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(180.f));

	PxVec3 PxPos(0);
	_vec3 vPos = *m_pTransformCom->Get_Pos();

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	_float fMaxDis = 10.f;
	// ���� �ð����� ���ٰ� �ٴڿ� �������� Ÿ��
	if (m_bFalling)
	{
		m_vMovePos.y = -200.f;
		fMaxDis = 0.1f;
	}

	_vec3 vDir = m_vMovePos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	m_eUnitMoveDir = UNIT_DIR_UP;

	float fDis = m_pCalculatorCom->Get_Distance(&vPos, &m_vMovePos);

	if (fDis <= fMaxDis)
	{
		m_iNextIndex = CRandomFunc::Rand_Int(0, 15);
		// ���� ������ ����
		m_vMovePos = m_vecBorder[m_iNextIndex];
	}

	// Ÿ�� ���� ���ʹϾ� ����
	Vector3 targetDir = { (_double)vDir.x, (_double)vDir.y,(_double)vDir.z };
	Quaternion lookRotation = Quaternion::LookRotation(targetDir);

	// �� ���� �̵� ���� ���ʹϾ� ����
	m_QuatDirection = Quaternion::Slerp(m_QuatDirection, lookRotation, 5.f * _dTimeDelta);

	// �� �̵� ���� ���ʹϾ��� ���忡 ����
	D3DXQUATERNION QuatRot = { (_float)m_QuatDirection.X, (_float)m_QuatDirection.Y, (_float)m_QuatDirection.Z, (_float)m_QuatDirection.W };
	_mat matRotWorld;
	D3DXMatrixRotationQuaternion(&matRotWorld, &QuatRot);

	m_pTransformCom->Set_IsYawPitchRoll(true);
	m_pTransformCom->Set_YawPitchRoll(matRotWorld);

	PxPos -= vLook * _float(m_tStat.fSpeed * _dTimeDelta);

	m_pCtrl->move(PxPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());
	
	Ground_Check();
}

void CDino_Ptero::Check_State()
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
			m_iAnimation = m_bJump ? PTERO_ANIM_FLY_IDE : PTERO_ANIM_GROUND_IDLE;
			break;
		case CUnit::STATE_MOVE:
			if (!m_bJump && !m_bIsRide && !m_bIsTamed)
				m_eUnitState = STATE_IDLE;

			m_tStat.fSpeed = m_fSpeed;
			m_iNextIndex = CRandomFunc::Rand_Int(0, 15);
			// ���� ������ ����
			m_vMovePos = m_vecBorder[m_iNextIndex];
			break;
		case CUnit::STATE_SPRINT:
			m_tStat.fSpeed = m_fSprint;
			m_dAnimationSpeed = 0.4;
			break;
		case CUnit::STATE_ATTACK:
			m_iAnimation = m_bJump ? PTERO_ANIM_FLY_ATTACK_BITE : PTERO_ANIM_GROUND_ATACK;
			if(m_bGrab && m_bJump)
				m_iAnimation = PTERO_ANIM_FLY_ATTACK_FEET_OUT;

			m_bIsAttack = true;
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:			// ���� ����
			m_iAnimation = PTERO_ANIM_TORPID_IN;
			RotationY();
			break;
		case CUnit::STATE_TORPID_IDLE:		// ������
			m_iAnimation = PTERO_ANIM_TORPID_IDLE;
			break;
		case CUnit::STATE_TORPID_TAMED:		// ������ �Ͼ�� (���̹� �Ϸ�)
			m_iAnimation = PTERO_ANIM_TORPID_OUT_TAMED;
			break;
		case CUnit::STATE_TORPID_WILD:		// ������ �Ͼ�� (���̹� ����)
			m_iAnimation = PTERO_ANIM_TORPID_OUT_WILD;
			break;
		case CUnit::STATE_TORPID_EAT:		// ������ ���ĸԱ�
			m_iAnimation = PTERO_ANIM_TORPID_EAT;
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = PTERO_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			//m_iAnimation = PTERO_ANIM_HARVEST;
			break;
		case CUnit::STATE_RIDE:
			m_iAnimation = PTERO_ANIM_TAKE_OFF;
			break;
		case CUnit::STATE_ROAR:
			//m_iAnimation = PARA_ANIM_ROAR_ALERT;
			break;
		/// ���� �¾��� �� 
		case CUnit::STATE_HOLDING:
			m_iAnimation = PTERO_ANIM_GROUND_IDLE;
			m_bIsHolding = true;
			break;
		case CUnit::STATE_LANDING:
			m_iAnimation = PTERO_ANIM_LAND;
			if(m_bIsRide)
				m_pTransformCom->Set_Rotate(ROT_X, D3DXToRadian(0.f));
			else 
				RotationY();
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
			m_iAnimation = m_bJump ? PTERO_ANIM_FLY_IDE : PTERO_ANIM_GROUND_BACKWARD;
			break;
			//case CUnit::UNIT_DIR_DOWN_L:
			//	break;
			//case CUnit::UNIT_DIR_DOWN_R:
			//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState || STATE_SPRINT == m_eUnitState)
				m_iAnimation = m_bJump ? PTERO_ANIM_FLY_FWD : PTERO_ANIM_GROUND_MOVE_FWD;
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
			m_iUpperAnimation = m_bJump ? PTERO_ANIM_FLY_ATTACK_BITE : PTERO_ANIM_GROUND_ATACK;
			if (m_bGrab && m_bJump)
				m_iUpperAnimation = PTERO_ANIM_FLY_ATTACK_FEET_OUT;
			m_bIsAttack = true;
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_HARVEST:
			//m_iUpperAnimation = PTERO_ANIM_HARVEST;
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

HRESULT CDino_Ptero::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// �Ž� ������Ʈ
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Ptero"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test��
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 40);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Ptero::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}

void CDino_Ptero::RotationY()
{
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	vLook.y = 0.f;
	//vLook.z = 0.f;

	vLook *= -1.f;

	Vector3 targetDir = { (_double)vLook.x, (_double)vLook.y,(_double)vLook.z };
	Quaternion lookRotation = Quaternion::LookRotation(targetDir);

	// �� ���� �̵� ���� ���ʹϾ� ����
	m_QuatDirection = Quaternion::Slerp(m_QuatDirection, lookRotation, 1);

	// �� �̵� ���� ���ʹϾ��� ���忡 ����
	D3DXQUATERNION QuatRot = { (_float)m_QuatDirection.X, (_float)m_QuatDirection.Y, (_float)m_QuatDirection.Z, (_float)m_QuatDirection.W };
	_mat matRotWorld;
	D3DXMatrixRotationQuaternion(&matRotWorld, &QuatRot);

	m_pTransformCom->Set_YawPitchRoll(matRotWorld);
}

_bool CDino_Ptero::Ground_Check()
{
	_float fRad = static_cast<PxCapsuleController*>(m_pCtrl)->getRadius();
	_float fHeight = static_cast<PxCapsuleController*>(m_pCtrl)->getHeight();
	PxVec3 vPos = PxVec3((_float)m_pCtrl->getPosition().x, (_float)m_pCtrl->getPosition().y, (_float)m_pCtrl->getPosition().z);

	// 1. ����ƽ ��ü���� ����
	// 2. �¾��� ���� ���� x(SPRINT)
	if (CPhysX_Mgr::GetInstance()->IsOverlap_Capsule_StaticObject(fRad, fHeight, vPos) && m_bFalling)
	{ 
		m_bJump = false;
		m_eUnitState = STATE_LANDING;
		m_iNextIndex = CRandomFunc::Rand_Int(0, 15);
		// ���� ������ ����
		m_vMovePos = m_vecBorder[m_iNextIndex];

		return true;
	}

	return false;
}
