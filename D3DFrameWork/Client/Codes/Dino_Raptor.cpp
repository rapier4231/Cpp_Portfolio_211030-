#include "stdafx.h"
#include "Dino_Raptor.h"

#include "Export_Function.h"

#include "Inven.h"
#include "LR_Inventory.h"
#include "QuickSlot_UI.h"

#include "PhysX_Mgr.h"

#include "RandomFunc.h"

USING(Engine)

CDino_Raptor::CDino_Raptor(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Raptor::CDino_Raptor(const CDino_Raptor & rhs)
	: CDino(rhs)
	, m_iIndex(rhs.m_iIndex)
{
}

HRESULT CDino_Raptor::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Raptor::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	/////////////////////////////
	m_wstrDinoSpecies = L"Rap";
	m_wstrDinoSpeciesKor = L"����";
	/////////////////////////////

	// ���� ���� ����
	m_iAsddleIndex = 1;								// �Ž��󿡼��� ���� ����� ��ȣ
	m_pSaddleBoneName = "Cnt_Spine_001_JNT_SKL";	// ���� �� �̸�
	m_pNeckBoneName = "Cnt_Neck_000_JNT_SKL";
	//m_pNeckBoneName = "Cnt_Head_JNT_SKL"/*"Cnt_Neck_001_JNT_SKL"*//*"Cnt_Spine_004_JNT_SKL"*/;		// �� �� �̸�

	//���� �Ǻ� ����
	m_bCanivore = true;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// ����¸�ŭ bool �� �����Ҵ�
	m_bIsSaddleIndex = new _bool[m_pMeshCom->Get_MeshList()->size()];
	memset(m_bIsSaddleIndex, 1, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsSaddleIndex[m_iAsddleIndex] = false;		// ��ü�� �׸���


	// Ʈ������ ����
	Set_Scale(0.017f);
	m_vMovePos = Get_Pos();
	//// �⺻ ����
	m_iAnimation = RAPTOR_ANIM_IDLE;

	//// ���� �θ� ��Ʈ����
	m_pSaddleFrame = m_pMeshCom->Get_FrameByName(m_pSaddleBoneName);

	// ���� �ɷ�ġ
	//m_tStat.fMaxHp = 200.f;
	//m_tStat.fHp = m_tStat.fMaxHp;
	//m_tStat.fAtk = 15.f;
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
	m_tStat.fSize = 0.7f;

	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;
	m_iUpperAnimation = RAPTOR_ANIM_IDLE;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_IDLE2;
	m_eNextRandomAnim[2] = STATE_IDLE3;
	m_dIdleMaxTime = rand() % 5;

	m_vfootPoint.y = -2.2f;

	m_dAttackStartTime = 0.2;
	m_dAttackEndTime = 0.5;

	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(3.f, 0.6f, Get_Pos());
	if (nullptr == m_pCtrl)
		return E_FAIL;

	Load_Collider(L"../Resources/Data/Collider/Raptor_Collider.dat");

	Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"RAPTOR");

	m_fDustScale = 0.002f;

	return S_OK;
}

_int CDino_Raptor::Update_GameObject(const _double & _dTimeDelta)
{
	m_fCenterPos = 1.f;

	if (!m_bIsTamed && !m_bIsRide)
		m_bAttackMode = true;

	return CDino::Update_GameObject(_dTimeDelta);
}

_int CDino_Raptor::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CDino::LateUpdate_GameObject(_dTimeDelta);
	
	PxVec3 vPos(0);
	m_fJumpPower = 3.f;
	if (m_bJump)
	{
		m_fJumpTime += (_float)_dTimeDelta * 0.8f;
		vPos.y = (m_fJumpPower * m_fJumpTime - m_fJumpGravity * m_fJumpTime * m_fJumpTime * 0.5f);
		
		const PxU32 flags = m_pCtrl->move(vPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());

		if (flags & PxControllerCollisionFlag::eCOLLISION_DOWN || flags & PxControllerCollisionFlag::eCOLLISION_SIDES)
		{
			if (STATE_FALLING == m_eUnitState || STATE_JUMPING == m_eUnitState)
				m_eUnitState = STATE_LANDING;

			m_bJump = false;
		}
	}

	return NO_EVENT;
}

void CDino_Raptor::Render_GameObject()
{
	CDino::Render_GameObject();
}

void CDino_Raptor::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Raptor::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDino_Raptor::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
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

CDino_Raptor * CDino_Raptor::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Raptor*	pInstance = new CDino_Raptor(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Raptor::Clone(void * _pArg)
{
	CDino_Raptor*	pClone = new CDino_Raptor(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Raptor::Free()
{
	CDino::Free();
}

void CDino_Raptor::Ride_Key_Input_Custum(const _double & _dTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80 && !m_bJump)
	{
		m_bJump = true;
		m_fJumpTime = 0.f;
		m_eUnitState = STATE_JUMPING;
	}

	if (m_ePreUnitState == STATE_JUMPING)
		m_eUnitState = STATE_JUMPING;
	else if (m_ePreUnitState == STATE_FALLING)
		m_eUnitState = STATE_FALLING;
	else if (m_ePreUnitState == STATE_LANDING)
		m_eUnitState = STATE_LANDING;

}

void CDino_Raptor::IsRide_Set_SaddleMat()
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

	_mat matSaddle, matScale, matRotX, matRotY, matRotZ, matAxisY, matTrans;
	matSaddle = m_pSaddleFrame->CombinedTransformMatrix;

	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(180.f));
	D3DXMatrixRotationAxis(&matRotY, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	D3DXMatrixRotationAxis(&matAxisY, &_vec3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(90.f));
	D3DXMatrixTranslation(&matTrans, 0.f, 30.f , 0.f);

	m_matSaddle = matScale * matRotX * matRotY * matAxisY * matRotZ * matTrans * matSaddle * m_pTransformCom->Get_WorldMatrix();
	//m_pTransformCom->Set_WorldMatrix(&matSaddle);
}

void CDino_Raptor::Check_State()
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
			m_iAnimation = RAPTOR_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE2:
			m_iAnimation = RAPTOR_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE3:
			m_iAnimation = RAPTOR_ANIM_IDLE;
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
			m_iAnimation = RAPTOR_ANIM_ATTACK;
			m_bIsAttack = true;
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:			// ���� ����
			m_iAnimation = RAPTOR_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_TORPID_IDLE:		// ������
			m_iAnimation = RAPTOR_ANIM_TORPID_IDLE;
			break;
		case CUnit::STATE_TORPID_TAMED:		// ������ �Ͼ�� (���̹� �Ϸ�)
			m_iAnimation = RAPTOR_ANIM_TORPID_OUT_TEIMMED;
			break;
		case CUnit::STATE_TORPID_WILD:		// ������ �Ͼ�� (���̹� ����)
			m_iAnimation = RAPTOR_ANIM_TORPID_OUT_WILD;
			break;
		case CUnit::STATE_TORPID_EAT:		// ������ ���ĸԱ�
			m_iAnimation = RAPTOR_ANIM_TORPID_EAT;
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = RAPTOR_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			break;
		case CUnit::STATE_RIDE:
			break;
		case CUnit::STATE_ROAR:
			//m_iAnimation = PARA_ANIM_ROAR_ALERT;
			break;
		/// ���� �¾��� �� 
		case CUnit::STATE_HOLDING:
			m_iAnimation = RAPTOR_ANIM_IDLE;
			m_bIsHolding = true;
			break;
		case CUnit::STATE_JUMPING:
			m_iAnimation = RAPTOR_ANIM_JUMP_IN;
			break;
		case CUnit::STATE_FALLING:
			m_iAnimation = RAPTOR_ANIM_JUMP_IDLE;
			break;
		case CUnit::STATE_LANDING:
			m_iAnimation = RAPTOR_ANIM_JUMP_OUT;
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
			m_iAnimation = RAPTOR_ANIM_BACKWARD;
			break;
		//case CUnit::UNIT_DIR_DOWN_L:
		//	break;
		//case CUnit::UNIT_DIR_DOWN_R:
		//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = RAPTOR_ANIM_MOVE_FORWARD;
			else
				m_iAnimation = RAPTOR_ANIM_CHARGE_FORWARD;
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
			m_iUpperAnimation = RAPTOR_ANIM_ATTACK;
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

HRESULT CDino_Raptor::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// �Ž� ������Ʈ
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Raptor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test��
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 20);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Raptor::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}
