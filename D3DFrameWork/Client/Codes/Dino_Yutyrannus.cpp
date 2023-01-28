#include "stdafx.h"
#include "Dino_Yutyrannus.h"

#include "Export_Function.h"

#include "TPV_Camera.h"

#include "Inven.h"
#include "LR_Inventory.h"
#include "QuickSlot_UI.h"

#include "PhysX_Mgr.h"

#include "RandomFunc.h"
#include "Player.h"

USING(Engine)

CDino_Yutyrannus::CDino_Yutyrannus(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Yutyrannus::CDino_Yutyrannus(const CDino_Yutyrannus & rhs)
	: CDino(rhs)
	, m_iIndex(rhs.m_iIndex)
{
}

HRESULT CDino_Yutyrannus::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Yutyrannus::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	// �˷λ��罺 ���� ����
	/////////////////////////////
	m_wstrDinoSpecies = L"Yutyra";
	m_wstrDinoSpeciesKor = L"��Ƽ�󴩽�";
	/////////////////////////////
	m_iAsddleIndex = 1;					// �Ž��󿡼��� ���� ����� ��ȣ
	m_pSaddleBoneName = "Spine_02";		// ���� �� �̸�
	m_pNeckBoneName = "Neck_01";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// ����¸�ŭ bool �� �����Ҵ�
	m_bIsSaddleIndex = new _bool[m_pMeshCom->Get_MeshList()->size()];
	memset(m_bIsSaddleIndex, 1, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsSaddleIndex[m_iAsddleIndex] = false;		// ��ü�� �׸���


	//���� �Ǻ� ����
	m_bCanivore = true;
	
	// Ʈ������ ����
	Set_Scale(0.012f);
	Set_Pos(_vec3(-80.f, 0.f, -21.f));
	m_vMovePos = Get_Pos();

	//// �⺻ ����
	m_iAnimation = YUTYRANNUS_ANIM_IDLE;

	//// ���� �θ� ��Ʈ����
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
	
	// ���̹� ����
	m_eTamingFood = ITEMID_Meat;
	//

	m_tStat.fSpeed = 0.f;
	m_fBackwardSpeed = 5.f;
	m_tStat.fSize = 3.f;

	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;
	m_iUpperAnimation = YUTYRANNUS_ANIM_IDLE;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_IDLE2;
	m_eNextRandomAnim[2] = STATE_IDLE3;
	m_dIdleMaxTime = rand() % 5;

	m_vfootPoint.y = -3.5f;

	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(3.f, 2.f, Get_Pos());
	if (nullptr == m_pCtrl)
		return E_FAIL;

	Load_Collider(L"../Resources/Data/Collider/Yutyrannus_Collider.dat");

	Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"YUTYRANNUS");

	m_fDustScale = 0.006f;

	m_fCenterPos = 8.f;

	return S_OK;
}

_int CDino_Yutyrannus::Update_GameObject(const _double & _dTimeDelta)
{
	//Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"YUTYRANNUS");


	if (!m_bIsTamed && !m_bIsRide)
	{
		m_bAttackMode = true;

		if (m_eUnitState == STATE_HOLDING)
			m_eUnitState = STATE_MOVE;
	}

	//��ȿ���� �ƴѵ� �ξ�����Ʈ�� ������ ����
	if (!m_bBuffRoar && m_pRoarEffect)
	{
		Engine::Delete_GameObject(L"Layer_Effect", m_pRoarEffect);
		m_pRoarEffect = nullptr;
		m_bRoarEffect = false;
	}

	if (m_pRoarEffect)
	{
		_vec3 vPos = Get_Pos();
		vPos.y += 9.f;
		static_cast<CTransform*>(m_pRoarEffect->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);
	}

	return CDino::Update_GameObject(_dTimeDelta);
}

_int CDino_Yutyrannus::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CDino::LateUpdate_GameObject(_dTimeDelta);

	return NO_EVENT;
}

void CDino_Yutyrannus::Render_GameObject()
{
	CDino::Render_GameObject();
}

void CDino_Yutyrannus::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Yutyrannus::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDino_Yutyrannus::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
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

		if (0 >= static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Now_Have_ItemAmount(ITEMID_Meat))
			Set_Dead();

		//if (0 >= m_pInvenCom->Get_pInvenItem()->size())
		//	Set_Dead();
	}

	return true;
}

CDino_Yutyrannus * CDino_Yutyrannus::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Yutyrannus*	pInstance = new CDino_Yutyrannus(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Yutyrannus::Clone(void * _pArg)
{
	CDino_Yutyrannus*	pClone = new CDino_Yutyrannus(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Yutyrannus::Free()
{
	CDino::Free();
}

void CDino_Yutyrannus::Ride_Key_Input_Custum(const _double & _dTimeDelta)
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

void CDino_Yutyrannus::Check_IsUpperEndAnimation()
{
	switch (m_ePreUpperDinoState)
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
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.2))
		{
			// ���� ����
			m_bIsAttack = false;
			m_eUpperDinoState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_HURT:
		break;
	case CUnit::STATE_EAT:
		break;
	case CUnit::STATE_HARVEST:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.2))
			m_eUpperDinoState = STATE_IDLE;
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
	case CUnit::STATE_ROAR:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.2))
		{
			m_bBuffRoar = false;
			m_eUpperDinoState = STATE_IDLE;
		}

		if (m_pMeshCom->Is_UpperAnimationsetEnd(2.8) && !m_bRoarEffect)
		{
			m_bRoarEffect = true;
			_float fSize = 20.f;
			m_pRoarEffect = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Roar", &fSize);
			NULL_CHECK_RETURN(m_pRoarEffect, );
			FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", m_pRoarEffect), );

			list<CGameObject*>* pLayer = Engine::Get_LayerList(L"Layer_Dino_Tamed");
			
			for (auto& pGameObject : *pLayer)
			{
				CDino* pDino = static_cast<CDino*>(pGameObject);

				if(STATE_DEAD == pDino->Get_UnitState() || pDino->Get_IsBaby())
					continue;

				pDino->Add_Buff(L"Proto_Obj_Effect_Buff", 20.f, 5.f);
			}

			//�÷��̾ ����
			static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player"))->Add_Buff(L"Proto_Obj_Effect_Buff", 20.f, 5.f);
		}
		break;
	default:
		break;
	}
}

void CDino_Yutyrannus::IsRide_Set_SaddleMat()
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
	//
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
	D3DXMatrixTranslation(&matRotZ, 10.f , 0.f , 50.f);

	m_matSaddle = matScale * matRotX * matRotY * matRotZ * matSaddle * m_pTransformCom->Get_WorldMatrix();
	//m_pTransformCom->Set_WorldMatrix(&matSaddle);
}

void CDino_Yutyrannus::Check_State()
{
	CDino::Check_IsEndAnimation();
	Check_IsUpperEndAnimation();

	_vec3 vPos = *m_pTransformCom->Get_Pos();

	if (m_ePreUnitState != m_eUnitState)
	{
		m_ePreUnitState = m_eUnitState;

		switch (m_eUnitState)
		{
		case CUnit::STATE_IDLE:
			m_iAnimation = YUTYRANNUS_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE2:
			m_iAnimation = YUTYRANNUS_ANIM_CUDDLE;
			break;
		case CUnit::STATE_IDLE3:
			m_iAnimation = YUTYRANNUS_ANIM_STARLED;
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
			m_iAnimation = rand() % 2 ? YUTYRANNUS_ANIM_ATTACK_BITE : YUTYRANNUS_ANIM_ATTACK_HEADBUTT;
			m_bIsAttack = true;
			if(m_iAnimation == YUTYRANNUS_ANIM_ATTACK_BITE)
			{
				m_dAttackStartTime = 0.7;
				m_dAttackEndTime = 0.9;
			}
			else
			{
				m_dAttackStartTime = 0.5;
				m_dAttackEndTime = 0.7;
			}
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:			// ���� ����
			m_iAnimation = YUTYRANNUS_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_TORPID_IDLE:		// ������
			m_iAnimation = YUTYRANNUS_ANIM_TORPID_IDLE;
			break;
		case CUnit::STATE_TORPID_TAMED:		// ������ �Ͼ�� (���̹� �Ϸ�)
			m_iAnimation = YUTYRANNUS_ANIM_TORPID_OUT_TEIMED;
			break;
		case CUnit::STATE_TORPID_WILD:		// ������ �Ͼ�� (���̹� ����)
			m_iAnimation = YUTYRANNUS_ANIM_TORPID_OUT_WILD;
			break;
		case CUnit::STATE_TORPID_EAT:		// ������ ���ĸԱ�
			m_iAnimation = YUTYRANNUS_ANIM_TORPID_EAT;
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = YUTYRANNUS_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			break;
		case CUnit::STATE_RIDE:
			break;
		case CUnit::STATE_ROAR:
			m_iAnimation = YUTYRANNUS_ANIM_ROAR_BUFF;
			break;
		/// ���� �¾��� �� 
		case CUnit::STATE_HOLDING:
			m_iAnimation = YUTYRANNUS_ANIM_IDLE;
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
			m_iAnimation = YUTYRANNUS_ANIM_BACKWARD;
			break;
		//case CUnit::UNIT_DIR_DOWN_L:
		//	break;
		//case CUnit::UNIT_DIR_DOWN_R:
		//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = YUTYRANNUS_ANIM_MOVE_FORWARD;
			else
				m_iAnimation = YUTYRANNUS_ANIM_CHARGE_FORWARD;
			break;
		case CUnit::UNIT_DIR_UP_L:
				m_iAnimation = YUTYRANNUS_ANIM_MOVE_L;
			break;
		case CUnit::UNIT_DIR_UP_R:
				m_iAnimation = YUTYRANNUS_ANIM_MOVE_R;
			break;
		}
	}


	// �κ� �ִϸ��̼� üũ
	if (m_ePreUpperDinoState != m_eUpperDinoState)
	{
		m_ePreUpperDinoState = m_eUpperDinoState;

		switch (m_eUpperDinoState)
		{
		case CUnit::STATE_ATTACK:
			m_tStat.fKnockBack = 2.f;
			m_iUpperAnimation = YUTYRANNUS_ANIM_MOVE_ATTACK;
			m_bIsAttack = true;
			break;
		case CUnit::STATE_ROAR:
			m_iUpperAnimation = YUTYRANNUS_ANIM_ROAR_BUFF;
			break;
		default:
			break;
		}

	}
}

HRESULT CDino_Yutyrannus::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// �Ž� ������Ʈ
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Yutyrannus"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test��
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 70);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Yutyrannus::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}
