#include "stdafx.h"
#include "Dino_Rex.h"

#include "Export_Function.h"

#include "Inven.h"
#include "LR_Inventory.h"
#include "QuickSlot_UI.h"

#include "PhysX_Mgr.h"

#include "RandomFunc.h"

USING(Engine)

CDino_Rex::CDino_Rex(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Rex::CDino_Rex(const CDino_Rex & rhs)
	: CDino(rhs)
{
}

HRESULT CDino_Rex::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();	

	// ���� ���� ����
	m_iAsddleIndex = 1;								// �Ž��󿡼��� ���� ����� ��ȣ
	m_pSaddleBoneName = "Cnt_Spine_003_JNT_SKL";	// ���� �� �̸�
	m_pNeckBoneName = "Cnt_Neck_002_JNT_SKL"/*"Cnt_Head_JNT_SKL"*//*"Cnt_Neck_001_JNT_SKL"*//*"Cnt_Spine_004_JNT_SKL"*/;		// �� �� �̸�

	//m_vecWaistBoneName.reserve(4);
	//m_vecWaistBoneName.emplace_back("Cnt_Spine_001_JNT_SKL");
	//m_vecWaistBoneName.emplace_back("Cnt_Spine_002_JNT_SKL");
	//m_vecWaistBoneName.emplace_back("Cnt_Spine_003_JNT_SKL");
	//m_vecWaistBoneName.emplace_back("Cnt_Spine_004_JNT_SKL");
	//
	//m_vecNeckBoneName.reserve(3);
	//m_vecNeckBoneName.emplace_back("Cnt_Neck_001_JNT_SKL");
	//m_vecNeckBoneName.emplace_back("Cnt_Neck_002_JNT_SKL");
	//m_vecNeckBoneName.emplace_back("Cnt_Neck_003_JNT_SKL");

	return S_OK;
}

HRESULT CDino_Rex::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	/////////////////////////////
	m_wstrDinoSpecies = L"Rex";
	/////////////////////////////

	m_iAsddleIndex = 1;


	// ����¸�ŭ bool �� �����Ҵ�
	m_bIsSaddleIndex = new _bool[m_pMeshCom->Get_MeshList()->size()];
	memset(m_bIsSaddleIndex, 1, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsSaddleIndex[m_iAsddleIndex] = false;		// ��ü�� �׸���

	m_pSaddleFrame = m_pMeshCom->Get_FrameByName(m_pSaddleBoneName); // ���� �θ� ��Ʈ����

	if (m_bBaby)
	{
		m_bIsTamed = true;
		m_bCanbeRide = false;
		m_wstrDinoSpeciesKor = L"���Ʊ� ����";
		m_pHeadBoneName = "Cnt_Head_JNT_SKL";
		m_pTailBoneName = "Cnt_Tail_000_JNT_SKL";

		// Ʈ������ ����
		Set_Scale(0.003f);

		// ���� �ɷ�ġ
		m_tStat.fMaxStamina = 100.f;
		m_tStat.fStamina = m_tStat.fMaxStamina;
		m_tStat.fKnockBack = 3.f;
		m_tStat.fMaxWeight = 2000.f;
		m_tStat.fWeight = 0.f;
		m_tStat.fMaxHunger = 200.f;
		m_tStat.fHunger = m_tStat.fMaxHunger;
		m_tStat.fMaxThirst = 50.f;
		m_tStat.fThirst = m_tStat.fMaxThirst;
		m_tStat.fSize = 10.f;

		m_vfootPoint = { 0.f, -0.3f, 0.f };
		m_fDustScale = 0.0008f;

		m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(0.2f, 0.2f, Get_Pos());
		if (nullptr == m_pCtrl)
			return E_FAIL;

		Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"BABY_REX");
	}
	else
	{
		m_wstrDinoSpeciesKor = L"����";

		if (FAILED(Rex_Setting(true))) return E_FAIL;
	}

	// �⺻ ����
	m_iAnimation = REX_A_IDLE;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_IDLE2;
	m_eNextRandomAnim[2] = STATE_IDLE3;
	m_dIdleMaxTime = rand() % 5;

	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;
	m_iAnimation = REX_A_IDLE;
	m_iUpperAnimation = REX_A_IDLE;

	Load_Collider(L"../Resources/Data/Collider/Rex_Collider.dat");

	return S_OK;
}

_int CDino_Rex::Update_GameObject(const _double & _dTimeDelta)
{
	m_fCenterPos = 10.f;

	if (!m_bIsTamed && !m_bIsRide)
	{
		m_bAttackMode = true;

		if (m_eUnitState == STATE_HOLDING)
			m_eUnitState = STATE_MOVE;
	}

	if (Engine::Get_DIKeyDown(DIK_K) && m_bIsTamed && !m_bBaby)
		m_eUnitState = STATE_EGG;

	if (Engine::Get_DIKeyState(DIK_N) & 0x80 && m_bIsTamed && !m_bBaby && !m_bPostpartumCare)
		m_bMatingMode = true;

	return CDino::Update_GameObject(_dTimeDelta);
}

_int CDino_Rex::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CDino::LateUpdate_GameObject(_dTimeDelta);

	// �÷��̾� ž������ �� ������Ʈ ����
	if (m_bIsRide)
	{
		Check_State();

		Dino_Move_Ride(_dTimeDelta);

		CDino::Is_Attack(L"Layer_Dino");
	}


	//�����׷� �߰� : Interaction �ö��̴��� ����ü �ȿ� �� ��쿡�� �����׷쿡 �߰��Ѵ�
	//�ö��̴� ��ǥ�� �����̹Ƿ� ����� ��ȯ���ش�
	_vec3 vSpherePos = m_pSphereColliderCom->Get_InteractPos();	
	D3DXVec3TransformCoord(&vSpherePos, &vSpherePos, &m_pTransformCom->Get_WorldMatrix());
	//�ö��̴� �ݰ��� ���� �����ϰ����� ������ �����̹Ƿ� �������� ���� ���� ����� ��ȯ���ش�
	if (Engine::Check_Sphere(vSpherePos, m_pSphereColliderCom->Get_InteractRadius() * Get_Scale().x))
		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	else
		//�ø��� ��� ������ ���������� �ִϸ��̼Ǹ� ���� �����ش�
	{
		if (m_bIsRide)
			DinoHead_ToCamLook();

		// �ִϸ��̼� �÷���
		if (m_bDying && m_ePreUnitState == STATE_DEAD)
			m_pMeshCom->Play_Animation_End(m_dAnimationTime, m_pNeckBoneName);
		else
			m_pMeshCom->Play_Animation(m_dAnimationTime, 1, m_pNeckBoneName);

		if (m_pSphereColliderCom)
			m_pSphereColliderCom->Update_Component(m_dAnimationTime);
	}

	return NO_EVENT;
}

void CDino_Rex::Render_GameObject()
{
	CDino::Render_GameObject();

	//if (m_pSkinnedCom)
	//{
	//	LPD3DXEFFECT pEffect = m_pSkinnedShaderCom->Get_EffectHandle();
	//	NULL_CHECK(pEffect);
	//	pEffect->AddRef();
	//
	//	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );
	//
	//	//m_pSkinnedCom->Render_Meshes(pEffect, nullptr);
	//
	//	Safe_Release(pEffect);
	//
	//	m_pSkinnedCom->Render_Skeleton(nullptr, nullptr, m_pTransformCom->Get_WorldMatrix());
	//}
}

_bool CDino_Rex::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
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

void CDino_Rex::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Rex::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

CDino_Rex * CDino_Rex::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Rex*	pInstance = new CDino_Rex(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Rex::Clone(void * _pArg)
{
	CDino_Rex*	pClone = new CDino_Rex(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Rex::Free()
{
	CDino::Free();
}

void CDino_Rex::IsRide_Set_SaddleMat()
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

	D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0.3f);
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(180.f));
	D3DXMatrixRotationAxis(&matRotY, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	D3DXMatrixTranslation(&matRotZ, 15.f, -55.f, 0.f);

	m_matSaddle = matScale * matRotX * matRotY * matRotZ * matSaddle * m_pTransformCom->Get_WorldMatrix();
}

void CDino_Rex::Set_Growth_Dino_Data()
{
	// ���̹� �ۼ�Ʈ
	_int iPer = (_int)(m_fGrowthTime / GROWTHTIME);

	if (iPer >= 100 && m_bBaby)
	{
		m_bBaby = false;
		Rex_Setting(false, 1.f);
		m_wstrDinoSpeciesKor.erase(0, 4);
		Set_DinoSpeciesKor(m_wstrDinoSpeciesKor);
		m_vfootPoint.y -= 0.5f;
	}
	else if (iPer >= 70 && !m_bFirstName[2])
	{
		m_pTailBoneName = nullptr;
		m_pHeadBoneName = nullptr;
		m_wstrDinoSpeciesKor.erase(0, 4);
		Set_DinoSpeciesKor(L"������ " + m_wstrDinoSpeciesKor, &m_bFirstName[2]);
		Rex_Setting(false, 0.6f);
		m_vfootPoint.y -= 0.3f;
	}
	else if (iPer >= 30 && !m_bFirstName[1])
	{
		m_pTailBoneName = nullptr;
		m_pHeadBoneName = nullptr;
		m_wstrDinoSpeciesKor.erase(0, 4);
		Set_DinoSpeciesKor(L"����� " + m_wstrDinoSpeciesKor, &m_bFirstName[1]);
		Rex_Setting(false, 0.3f);
		m_vfootPoint.y -= 0.2f;
	}
	else if (!m_bFirstName[0])
		Set_DinoSpeciesKor(L"���Ʊ� " + m_wstrDinoSpeciesKor, &m_bFirstName[0]);

}

void CDino_Rex::Dino_Mating(const _double & _dTimeDelta)
{
	if (m_eGender == FEMALE)
	{
		// �ӽ� �� �̰ų� �������� ���̸� �н�
		if (m_bPregnant || m_bPostpartumCare)
			return;

		list<CGameObject*>* listGameObjects = Get_LayerList(L"Layer_Dino_Tamed");
		if (listGameObjects->empty())
			return;

		_bool bMating = false;
		for (auto & pGameObject : *listGameObjects)
		{
			CDino* pDino = static_cast<CDino*>(pGameObject);
			//������, �Ʊ�� �ȵ�
			//����͸� �ȵ� ¦�����忩����, �����̾����
			if (pDino->Get_DinoSpecies()== L"Rex"	&&
				pDino->Get_IsBaby()		== false	&&
				pDino->Get_Follow()		== false	&&
				pDino->Get_MatingMode() == true		&&
				pDino->Get_Gender()		== CDino::MALE)
			{
				_float fDis = m_pCalculatorCom->Get_Distance(&pDino->Get_Pos(), &Get_Pos());

				if (fDis <= 10.f)
				{
					bMating = true;
					break;
				}
			}
		}

		if (bMating)
		{
			m_fMatingTime += (_float)_dTimeDelta;
			_int iPer = (_int)(m_fMatingTime / MATINGTIME);

			if (iPer >= 100)
			{
				m_fMatingTime = 0.f;
				m_bPregnant = true;
			}
		}
		else
			m_fMatingTime = 0.f;
	}
}

void CDino_Rex::Check_State()
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
		case CUnit::STATE_IDLE2:
		case CUnit::STATE_IDLE3:
			m_iAnimation = REX_A_IDLE;
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
			m_iAnimation = REX_A_BITE;
			m_bIsAttack = true;
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:			// ���� ����
			m_iAnimation = REX_A_TORPID_IN;
			break;
		case CUnit::STATE_TORPID_IDLE:		// ������
			m_iAnimation = REX_A_TORPID_IDLE;
			break;
		case CUnit::STATE_TORPID_TAMED:		// ������ �Ͼ�� (���̹� �Ϸ�)
			m_iAnimation = REX_A_TORPID_OUT_TAMED;
			break;
		case CUnit::STATE_TORPID_WILD:		// ������ �Ͼ�� (���̹� ����)
			m_iAnimation = REX_A_TORPID_OUT_WILD;
			break;
		case CUnit::STATE_TORPID_EAT:		// ������ ���ĸԱ�
			m_iAnimation = REX_A_TORPID_EAT;
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = REX_A_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			break;
		case CUnit::STATE_RIDE:
			break;
		case CUnit::STATE_EGG:
			m_iAnimation = REX_A_EGG;
			m_dAnimationSpeed = -0.5;
			break;
		case  CUnit::STATE_HAPPY:
			m_iAnimation = REX_A_CUDDLE;
			break;
		/// ���� �¾��� �� 
		case CUnit::STATE_HOLDING:
			m_iAnimation = REX_A_IDLE;
			m_bIsHolding = true;
		}
	}

	// �޸��� ���⿡ ���� �� ������ (�ӽ�)
	if (STATE_MOVE == m_eUnitState || STATE_SPRINT == m_eUnitState)
	{
		switch (m_eUnitMoveDir)
		{
		//case CUnit::UNIT_DIR_CENTER:
		//	break;
		case CUnit::UNIT_DIR_DOWN:
			m_iAnimation = REX_A_MOVE_BWD;
			break;
		//case CUnit::UNIT_DIR_DOWN_L:
		//	break;
		//case CUnit::UNIT_DIR_DOWN_R:
		//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = REX_A_MOVE_FWD;
			else
				m_iAnimation = REX_A_CHARGE_FWD;
			break;
		case CUnit::UNIT_DIR_UP_L:
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
			m_iUpperAnimation = REX_A_BITE;
			m_bIsAttack = true;
			break;
		}
	}
}

HRESULT CDino_Rex::Rex_Setting(_bool bCreate, _float fDivide)
{
	// Ʈ������ ����
	Set_Scale(0.03f * fDivide);

	// ���� �ɷ�ġ
	m_tStat.fMaxStamina = 100.f * fDivide;
	m_tStat.fStamina = m_tStat.fMaxStamina * fDivide;
	m_tStat.fKnockBack = 6.f * fDivide;
	m_tStat.fMaxWeight = 2000.f * fDivide;
	m_tStat.fWeight = 0.f;
	m_tStat.fMaxHunger = 200.f * fDivide;
	m_tStat.fHunger = m_tStat.fMaxHunger;
	m_tStat.fMaxThirst = 50.f * fDivide;
	m_tStat.fThirst = m_tStat.fMaxThirst;
	m_tStat.fSize = 10.f * fDivide;

	m_fBackwardSpeed = 5.f * fDivide;

	m_vfootPoint = { 0.f, -4.3f * fDivide, 0.f };
	m_fDustScale = 0.008f * fDivide;

	if (bCreate)
	{
		m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(3.5f, 2.5f, Get_Pos());
		if (nullptr == m_pCtrl)
			return E_FAIL;
	}
	else
	{
		static_cast<PxCapsuleController*>(m_pCtrl)->setRadius(3.5f * fDivide);
		static_cast<PxCapsuleController*>(m_pCtrl)->setHeight(2.5f * fDivide);
		m_pCtrl->resize(2.5f * fDivide);
	}

	Load_DinoInfo(L"../Resources/Data/DinoInfo/DinoData.ini", L"REX", fDivide);

	return S_OK;
}

HRESULT CDino_Rex::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	///// �Ž� ������Ʈ
	//pComponent = m_pSkinnedCom = static_cast<CSkinnedMesh*>(Engine::Clone_ComponentPrototype(L"Proto_SkinnedMesh_Dino_Rex"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_SkinnedMesh", pComponent);
	//
	///// ��Ű�׽��̴� ������Ʈ
	//pComponent = m_pSkinnedShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Skinning"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_SkinnedShader", pComponent);


	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Rex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test��
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 100);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Rex::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}

