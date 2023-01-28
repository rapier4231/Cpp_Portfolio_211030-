#include "stdafx.h"
#include "Dino_Brachio.h"

#include "Export_Function.h"

#include "Inven.h"
#include "LR_Inventory.h"
#include "PlatformSaddle_Brachio.h"

#include "PhysX_Mgr.h"

USING(Engine)

CDino_Brachio::CDino_Brachio(LPDIRECT3DDEVICE9 _pDevice)
	: CDino(_pDevice)
{
}

CDino_Brachio::CDino_Brachio(const CDino_Brachio & rhs)
	: CDino(rhs)
	, m_iIndex(rhs.m_iIndex)
{
}

HRESULT CDino_Brachio::Ready_GameObject_Prototype()
{
	CDino::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDino_Brachio::Ready_GameObject(void * _pArg)
{
	CDino::Ready_GameObject(_pArg);

	/////////////////////////////
	m_wstrDinoSpecies = L"Bra";
	m_wstrDinoSpeciesKor = L"���Ű��";
	/////////////////////////////

	// ���� ���� ����
	m_iAsddleIndex = 1;								// �Ž��󿡼��� ���� ����� ��ȣ
	m_pSaddleBoneName = "c_back3";	// ���� �� �̸�
	m_pNeckBoneName = "c_neck1";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// ����¸�ŭ bool �� �����Ҵ�
	m_bIsSaddleIndex = new _bool[m_pMeshCom->Get_MeshList()->size()];
	memset(m_bIsSaddleIndex, 1, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());

	m_bIsSaddleIndex[0] = true;		// ��
	m_bIsSaddleIndex[1] = true;		// �÷��� ����
	m_bIsSaddleIndex[2] = false;	// �Ϲ� ����

	// Ʈ������ ����
	//m_pTransformCom->Set_Scale(0.02f, 0.02f, 0.02f);
	Set_Scale(0.02f);
	m_vMovePos = Get_Pos();
	//// �⺻ ����
	m_iAnimation = BRACHIO_ANIM_IDLE;

	//// ���� �θ� ��Ʈ����
	m_pSaddleFrame = m_pMeshCom->Get_FrameByName(m_pSaddleBoneName);

	// ���� �ɷ�ġ
	m_tStat.fMaxHp = 30.f;
	m_tStat.fHp = m_tStat.fMaxHp;
	m_tStat.fMaxStamina = 100.f;
	m_tStat.fStamina = m_tStat.fMaxStamina;
	m_tStat.fAtk = 50.f;
	m_tStat.fMaxWeight = 2000.f;
	m_tStat.fWeight = 0.f;
	m_tStat.fMaxHunger = 200.f;
	m_tStat.fHunger = m_tStat.fMaxHunger;
	m_tStat.fMaxThirst = 50.f;
	m_tStat.fThirst = m_tStat.fMaxThirst;

	m_tStat.fSpeed = 0.f;

	m_fRotationSpeed = 2.f;
	m_fBackwardSpeed = 5.f;

	///// �׽�Ʈ��
	//m_iIndex = *ui;

	m_eUnitState = STATE_IDLE;
	m_eUpperDinoState = STATE_IDLE;
	m_iUpperAnimation = BRACHIO_ANIM_IDLE;

	m_eNextRandomAnim[0] = STATE_MOVE;
	m_eNextRandomAnim[1] = STATE_IDLE2;
	m_eNextRandomAnim[2] = STATE_IDLE3;
	m_dIdleMaxTime = rand() % 5;

	//////////////////////////////////////////////////////////////////////////
	// ������ �׽�Ʈ 
	m_pCtrl = CPhysX_Mgr::GetInstance()->CreateCapsuleController(0.2f, 0.2f, Get_Pos());
	if (nullptr == m_pCtrl)
		return E_FAIL;

	Load_Collider(L"../Resources/Data/Collider/Brachio_Collider.dat");

	///
	//Set_PlatformSaddle();
	m_bIsSaddle = false;

	m_fCenterPos = 20.f;

	return S_OK;
}

_int CDino_Brachio::Update_GameObject(const _double & _dTimeDelta)
{

	m_bAttackMode = false;

	CDino::Update_GameObject(_dTimeDelta);
	//m_bIsSaddle = false;
	if(!m_bIsSaddle)
		Set_PlatformSaddle();

	//if (!m_pPlatformSaddle)
	//{
	//	// �÷��� ���� �߰� �� ����
	//	m_pPlatformSaddle = static_cast<CPlatformSaddle_Brachio*>(Engine::Get_GameObject(L"Layer_PlatformSaddle_Brachio", 0));
	//
	//	if(!m_pPlatformSaddle)
	//		Set_PlatformSaddle();
	//}
	//else
	//{
	//	_mat matSaddle = m_pMeshCom->Get_FrameByName("c_back4")->CombinedTransformMatrix * m_pTransformCom->Get_WorldMatrix();
	//	m_pPlatformSaddle->Set_SaddleTransform(&matSaddle);
	//}

	return NO_EVENT;
}

_int CDino_Brachio::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	//////////////////////////////////////////////////////////////////////////
	CDino::LateUpdate_GameObject(_dTimeDelta);	//////////////////////////	���Ű���� ���� �浹ü�� ���� CDino���� �ø� �߿� ����.
	//////////////////////////////////////////////////////////////////////////	=> CDino::LateUpdate()���� �ø����븸 ���� �����ؿ���
	//if (m_bIsDeadBody)
	//	return NO_EVENT;

	//// �÷��̾� ž������ �� ������Ʈ ����
	//if (m_bIsRide)
	//{
	//	DinoHead_ToCamLook();

	//	Dino_Move_Ride(_dTimeDelta);

	//	Check_State();

	//	CDino::Is_Attack(L"Layer_Dino");
	//}

	////// �ִϸ��̼� ����
	//m_pMeshCom->Set_Animation(m_iAnimation);				// ��ü �ִϸ��̼�

	//														// Upper�� IDLE�̸� BaseAni�� ����ȭ�ϱ�
	//if (STATE_IDLE != m_eUpperDinoState)
	//	m_pMeshCom->Set_UpperAnimation(m_iUpperAnimation);	// �κ� �ִϸ��̼�	
	//else
	//	m_pMeshCom->Set_UpperAnimation(m_iAnimation, 1.0);

	//m_dAnimationTime = _dTimeDelta;
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	//// �÷��̾� ž������ �� ������Ʈ ����
	//if (m_bIsRide)
	//{
	//	Check_State();
	//	
	//	Dino_Move_Ride(_dTimeDelta);

	//	//CDino::Is_Attack();
	//}

	////�����׷� �߰� : ���Ű���� ���� �浹ü�� �޷����� �����Ƿ� ����ü�ø� �õ����� �ʰ� �ٷ� ����
	////_vec3 vSpherePos = m_pSphereColliderCom->Get_InteractPos();
	////D3DXVec3TransformCoord(&vSpherePos, &vSpherePos, &m_pTransformCom->Get_WorldMatrix());
	////if (Engine::Check_Sphere(vSpherePos, m_pSphereColliderCom->Get_InteractRadius() * Get_Scale().x))
	//	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return NO_EVENT;
}

void CDino_Brachio::Render_GameObject()
{
	CDino::Render_GameObject();
}

void CDino_Brachio::Render_GameObjectShadowMap()
{
	CDino::Render_GameObjectShadowMap();
}

void CDino_Brachio::Render_GameObjectShadow()
{
	CDino::Render_GameObjectShadow();
}

_bool CDino_Brachio::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	//_bool bHit = CDino::HitBy(pAttacker, dwAttackType, vCollisionPos);

	if (!m_bAttackMode)
	{
		CComponent* pComponent = pAttacker->Get_Component(L"Com_Transform", ID_DYNAMIC);
		if (nullptr == pComponent)
			return false;

		m_pTartgetTransform = static_cast<CTransform*>(pComponent);
		m_bAttackMode = true;
	}

	return false; //return bHit;
}

CDino_Brachio * CDino_Brachio::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDino_Brachio*	pInstance = new CDino_Brachio(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDino_Brachio::Clone(void * _pArg)
{
	CDino_Brachio*	pClone = new CDino_Brachio(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDino_Brachio::Free()
{
	CDino::Free();
}


void CDino_Brachio::IsRide_Set_SaddleMat()
{
	/*WCHAR str[255] = L"";
	GetPrivateProfileString(L"RECT", L"x", 0, str, 255, L"../text.ini");
	wstring ss = str;
	float x = stof(ss);
	GetPrivateProfileString(L"RECT", L"y", 0, str, 255, L"../text.ini");
	ss = str;
	float y = stof(ss);
	GetPrivateProfileString(L"RECT", L"z", 0, str, 255, L"../text.ini");
	ss = str;
	float z = stof(ss);*/


	//GetPrivateProfileString(L"RECT", L"q", 0, str, 255, L"../text.ini");
	//ss = str;
	//float q = stof(ss);
	//GetPrivateProfileString(L"RECT", L"w", 0, str, 255, L"../text.ini");
	//ss = str;
	//float w = stof(ss);

	_mat matSaddle, matScale, matRotX, matRotY, matRotZ;
	matSaddle = m_pSaddleFrame->CombinedTransformMatrix;

	D3DXMatrixScaling(&matScale, 0.45f, 0.45f, 0.45f);
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90.f));
	D3DXMatrixRotationAxis(&matRotY, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	D3DXMatrixTranslation(&matRotZ, 220.f, 0.f , 150.f);

	m_matSaddle = matScale * matRotX * matRotY * matRotZ * matSaddle * m_pTransformCom->Get_WorldMatrix();
	//m_pTransformCom->Set_WorldMatrix(&matSaddle);
}

void CDino_Brachio::Check_State()
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
			m_iAnimation = BRACHIO_ANIM_IDLE;
			break;
		case CUnit::STATE_IDLE2:
			m_iAnimation = BRACHIO_ANIM_GRAZE;
			break;
		case CUnit::STATE_IDLE3:
			m_iAnimation = BRACHIO_ANIM_GRAZE;
			break;
		case CUnit::STATE_MOVE:
			m_tStat.fSpeed = 2.f;
			vPos.x += rand() % 40 - 20;
			vPos.z += rand() % 40 - 20;
			m_vMovePos = vPos;
			break;
		case CUnit::STATE_SPRINT:
			m_tStat.fSpeed = 5.f;
			break;
		case CUnit::STATE_ATTACK:
			m_iAnimation = BRACHIO_ANIM_TAIL_ATTACK_L;
			m_bIsAttack = true;
			break;
		case CUnit::STATE_HURT:
			break;
		case CUnit::STATE_FALLING:
			break;
		case CUnit::STATE_EAT:
			break;
		case CUnit::STATE_TORPID:
			break;
		case CUnit::STATE_DEAD:
			m_iAnimation = BRACHIO_ANIM_TORPID_IN;
			break;
		case CUnit::STATE_HARVEST:
			break;
		case CUnit::STATE_RIDE:
			break;
		case CUnit::STATE_ROAR:
			//m_iAnimation = PARA_ANIM_ROAR_ALERT;
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
			m_iAnimation = BRACHIO_ANIM_BACKWARD;
			break;
		//case CUnit::UNIT_DIR_DOWN_L:
		//	break;
		//case CUnit::UNIT_DIR_DOWN_R:
		//	break;
		case CUnit::UNIT_DIR_UP:
			if (STATE_MOVE == m_eUnitState)
				m_iAnimation = BRACHIO_ANIM_MOVE_FORWARD;
			else
				m_iAnimation = BRACHIO_ANIM_CHARGE_FORWARD;
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
			m_iUpperAnimation = BRACHIO_ANIM_TAIL_ATTACK_L;
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

void CDino_Brachio::Set_PlatformSaddle()
{
	CGameObject* pGameObject = nullptr;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_PlatformSaddle_Brachio");
	NULL_CHECK_RETURN(pGameObject,);

	static_cast<CPlatformSaddle_Brachio*>(pGameObject)->Set_TargetDino(this);
	Engine::Add_GameObjectInLayer(L"Layer_PlatformSaddle_Brachio", pGameObject);

	m_bIsSaddle = true;
}

HRESULT CDino_Brachio::Add_Component()
{
	if (FAILED(CDino::Add_Component())) 
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	/// �Ž� ������Ʈ
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Dino_Sauropod"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////Inven Test��
	m_pInvenCom->Add_Item(ITEMID::ITEMID_Meat, 30);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CDino_Brachio::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	CDino::SetUp_ConstantTable(_pEffect);

	return S_OK;
}
