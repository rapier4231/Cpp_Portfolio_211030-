#include "stdafx.h"
#include "..\Headers\Dino.h"

#include "Export_Function.h"
#include "SphereCollider.h"
#include "LR_Inventory.h"
#include "Inven.h"

#include "Bullet.h"

#include "QuickSlot_UI.h"
#include "Right_Inventory.h"
#include "TamingName_UI.h"
#include "PhysX_Mgr.h"
#include "CollisionFunc.h"
#include "Player.h"
#include "WorldFont_UI.h"
#include "GuideFont_UI.h"
#include "RandomFunc.h"
#include "Harvestable.h"
#include "CameraMrg.h"

USING(Engine)

#define CHASETIME 10.f

CDino::CDino(LPDIRECT3DDEVICE9 _pDevice)
	: CUnit(_pDevice)
{
}

CDino::CDino(const CDino & rhs)
	: CUnit(rhs)
	//, m_pSkinnedCom(rhs.m_pSkinnedCom)
	, m_pMeshCom(rhs.m_pMeshCom)
	, m_iAnimation(rhs.m_iAnimation)
	, m_iUpperAnimation(rhs.m_iUpperAnimation)
	, m_dAnimationTime(rhs.m_dAnimationTime)
	, m_pSaddleFrame(rhs.m_pSaddleFrame)
	, m_pSaddleBoneName(rhs.m_pSaddleBoneName)
	, m_pNeckBoneName(rhs.m_pNeckBoneName)
	, m_bIsTamed(rhs.m_bIsTamed)
	, m_bIsRide(rhs.m_bIsRide)
	, m_bIsSaddle(rhs.m_bIsSaddle)
	, m_bIsEquipSaddle(rhs.m_bIsEquipSaddle)
	//, m_iAsddleIndex(rhs.m_iAsddleIndex)
	//, m_vecWaistBoneName(rhs.m_vecWaistBoneName)
	//, m_vecNeckBoneName(rhs.m_vecNeckBoneName)
	, m_mapColor1(rhs.m_mapColor1)
	, m_mapColor2(rhs.m_mapColor2)
	, m_mapColor3(rhs.m_mapColor3)
{
}

HRESULT CDino::Ready_GameObject_Prototype()
{
	CUnit::Ready_GameObject_Prototype();

	_int i = 0;
	m_mapColor1.emplace(i++, _vec3(REDBROWN));
	m_mapColor1.emplace(i++, _vec3(BROWN));
	m_mapColor1.emplace(i++, _vec3(SIENNA));
	m_mapColor1.emplace(i++, _vec3(SITBROWN));
	m_mapColor1.emplace(i++, _vec3(PERU));
	m_mapColor1.emplace(i++, _vec3(GALJO));
	m_mapColor1.emplace(i++, _vec3(SANDYBROWN));

	i = 0;
	m_mapColor2.emplace(i++, _vec3(HWANGGAL));
	m_mapColor2.emplace(i++, _vec3(BURLYWOOD));
	m_mapColor2.emplace(i++, _vec3(MEAL));
	m_mapColor2.emplace(i++, _vec3(NAVAHO));
	m_mapColor2.emplace(i++, _vec3(BISQUE));
	m_mapColor2.emplace(i++, _vec3(BLANCHEDALMOND));
	m_mapColor2.emplace(i++, _vec3(CORNSILK));

	i = 0;
	m_mapColor3.emplace(i++, _vec3(REDBROWN));
	m_mapColor3.emplace(i++, _vec3(BROWN));
	m_mapColor3.emplace(i++, _vec3(SIENNA));
	m_mapColor3.emplace(i++, _vec3(SITBROWN));
	m_mapColor3.emplace(i++, _vec3(PERU));
	m_mapColor3.emplace(i++, _vec3(GALJO));
	m_mapColor3.emplace(i++, _vec3(SANDYBROWN));
	m_mapColor3.emplace(i++, _vec3(HWANGGAL));
	m_mapColor3.emplace(i++, _vec3(BURLYWOOD));
	m_mapColor3.emplace(i++, _vec3(MEAL));
	m_mapColor3.emplace(i++, _vec3(NAVAHO));
	m_mapColor3.emplace(i++, _vec3(BISQUE));
	m_mapColor3.emplace(i++, _vec3(BLANCHEDALMOND));
	m_mapColor3.emplace(i++, _vec3(CORNSILK));

	return S_OK;
}

HRESULT CDino::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CUnit::Ready_GameObject(_pArg);
	
	_int dwOption = Check_Arg(_pArg);
	if (dwOption < 0)
		return E_FAIL;

	m_eUnitType = CUnit::UNIT_DINO;

	/// �׽�Ʈ��
	m_tStat.fMaxTamingRate = 5.f;	// �ִ� ���ü�ġ
	m_tStat.fTamingRate = 0.f;		// ���ü�ġ
	
	// ���� ����
	m_iRandColor[0] = CRandomFunc::Rand_Int(0, (_int)m_mapColor1.size());
	m_iRandColor[1] = CRandomFunc::Rand_Int(0, (_int)m_mapColor2.size());
	m_iRandColor[2] = CRandomFunc::Rand_Int(0, (_int)m_mapColor3.size());

	m_vColor = _vec3(1.f, 1.f, 1.f);

	m_eGender = (GENDER)CRandomFunc::Rand_Int(0, 1);

	m_fFllowDis[DISTANCE_CLOSE] = 5.f;
	m_fFllowDis[DISTANCE_MIDDLE] = 10.f;
	m_fFllowDis[DISTANCE_FAR] = 15.f;

	vector<CRingCommand_UI::STRCD> vecRing;
	CRingCommand_UI::STRCD Teemp;
	Teemp.wstrCommand_Text = L"�κ��丮";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"[�ൿ���]";
	Teemp.bSubRing = true;
	Teemp.wstrRingKey = L"Moving";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"[����]";
	Teemp.bSubRing = true;
	Teemp.wstrRingKey = L"Setting";
	vecRing.emplace_back(Teemp);

	m_mapRingCommand.emplace(L"Main", vecRing);
	vecRing.clear();

	Teemp.bSubRing = false;
	Teemp.wstrCommand_Text = L"������";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"���� ��� �Ÿ� ��ġ \n ( ���� : �߰� )";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"¦���� ���";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"�׷� ���Ķ� ��� ����";
	vecRing.emplace_back(Teemp);

	m_mapRingCommand.emplace(L"Moving", vecRing);
	vecRing.clear();

	Teemp.wstrCommand_Text = L"11";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"22";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"33";
	vecRing.emplace_back(Teemp);

	m_mapRingCommand.emplace(L"Setting", vecRing);

	return S_OK;
}

_int CDino::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
	{
		m_bDissolve = true;
		m_fDissolveAmount += 0.02f;

		if (1.f < m_fDissolveAmount)
			return OBJECT_DEAD;
	}

	if (!(m_dwFlagSkip & SKIP_UPDATE))	//��ŵ�� ���������� �߷� ���� �̿��� �ٸ� ������Ʈ���� ��� ��ŵ
	{
		CUnit::Update_GameObject(_dTimeDelta);

		/*if (m_bDissolve && !m_bDead)
		{
			m_fDissolveAmount -= (_float)_dTimeDelta;

			if (0.f >= m_fDissolveAmount)
				m_bDissolve = false;

			_vec3 vPos(0.f,0.f,0.f);
			vPos.y = (_float)(GRAVITY * _dTimeDelta);
			m_pCtrl->move(vPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());

			PxExtendedVec3 PxPos = m_pCtrl->getPosition();
			m_pTransformCom->Set_Pos(_vec3((_float)PxPos.x + m_vfootPoint.x, (_float)PxPos.y + m_vfootPoint.y, (_float)PxPos.z + m_vfootPoint.z));

			return NO_EVENT;
		}*/

		Dino_UI();

		if (m_bIsRide)
			IsRide_Set_SaddleMat();

		// ���� üũ
		Check_Buff(_dTimeDelta);

		/// ���� �� ���� Ȯ��
		if (DINO_TAMED == Check_Torpid(_dTimeDelta))
			return DINO_TAMED;

		// ���� üũ
		Check_IsHolding(_dTimeDelta);

		/// ���� �� ���� Ȯ��
		if (DINO_TAMED == Check_Torpid(_dTimeDelta))
			return DINO_TAMED;

		if (m_bDying && m_ePreUnitState == STATE_DEAD)
			return NO_EVENT;

		if (!m_bIsRide && !m_bDying && !m_bIsTorpid && m_eUnitState != STATE_HOLDING)
		{
			PeaceTaming(_dTimeDelta);

			if (!m_bIsTamed)
			{
				Refresh_TargetList(_dTimeDelta);

				Wild_Action(_dTimeDelta);

				Is_Attack(L"Layer_Player", m_ListTargetUnit.empty() ? nullptr : &m_ListTargetUnit);
			}
			else
			{
				if (m_bHanging)
				{
					_vec3 vPos;
					memcpy(&vPos, &m_pHangingMatrix->m[INFO_POS][0], sizeof(_vec3));

					Set_Pos(vPos);
					m_pCtrl->setPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
				}
				else
					Tamed_Action(_dTimeDelta);
			}
		}

		Check_State();

		m_fCreateIntv += (_float)_dTimeDelta;

	}

	//// ������ �߷� ���� ////////////////////////////////////////////////////////////////////////
	_vec3 vPos = { 0.f,0.f,0.f };
	_float fAddGravity = 1.f;
	if (m_eUnitState == STATE_SPRINT && !m_bJump)
		fAddGravity = 5.f;

	//�Ŵ޷������� ����
	if (m_bHanging)
		return NO_EVENT;

	//���������̰ų� �ͷ��ε� ������������ �߷�
	if (m_bLandDino || (!m_bLandDino && !m_bJump))
	{
		vPos.y = (_float)(GRAVITY * _dTimeDelta * fAddGravity);
		m_pCtrl->move(vPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());
	}

	//���������� �����Ҷ�
	PxExtendedVec3 PxPos = m_pCtrl->getPosition();
	m_pTransformCom->Set_Pos(_vec3((_float)PxPos.x + m_vfootPoint.x, (_float)PxPos.y + m_vfootPoint.y, (_float)PxPos.z + m_vfootPoint.z));

	return NO_EVENT;
}

_int CDino::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (m_bIsDeadBody || (m_dwFlagSkip & SKIP_RENDER))
		return NO_EVENT;

	if (m_bHanging)
		HangingMatrix();

	// �÷��̾� ž������ �� ������Ʈ ����
	if (m_bIsRide)
	{
		DinoHead_ToCamLook();
 
		Dino_Move_Ride(_dTimeDelta);

		Check_State();

		if(!m_bGrab)
			CDino::Is_Attack(L"Layer_Dino");

	//	PxExtendedVec3 PxPos = m_pCtrl->getPosition();
	//	m_pTransformCom->Set_Pos(_vec3((_float)PxPos.x + m_vfootPoint.x, (_float)PxPos.y + m_vfootPoint.y, (_float)PxPos.z + m_vfootPoint.z));
	}

	//// �ִϸ��̼� ����
	m_pMeshCom->Set_Animation(m_iAnimation);				// ��ü �ִϸ��̼�

	// Upper�� IDLE�̸� BaseAni�� ����ȭ�ϱ�
	if (STATE_IDLE != m_eUpperDinoState)
		m_pMeshCom->Set_UpperAnimation(m_iUpperAnimation);	// �κ� �ִϸ��̼�	
	else
		m_pMeshCom->Set_UpperAnimation(m_iAnimation, 1.0);

	m_dAnimationTime = _dTimeDelta;

#pragma region CULLING
	// �ø� ���� //
	//�ö��̴� ��ǥ�� �����̹Ƿ� ����� ��ȯ���ش�
	_vec3 vSpherePos = m_pSphereColliderCom->Get_InteractPos();
	D3DXVec3TransformCoord(&vSpherePos, &vSpherePos, &m_pTransformCom->Get_WorldMatrix());
	_float fSphereRadius = m_pSphereColliderCom->Get_InteractRadius() * Get_Scale().x;

	//�׸��� �����׷� �߰� : ��豸ü�� �� ����ü �ȿ� �� ��쿡�� �����׷쿡 �߰��Ѵ�
	//if (Engine::Check_Sphere(vSpherePos, fSphereRadius, CULL_LIGHT))

	// �׸��ڸ� �����ڸ��ʿ� ������ ��� �������� ����(Clamp) ������
	// ��ü�� ���� �Ÿ��� ���� ������ �׸��ڸʿ� �׷��ش�.
	_vec3	vecPlayerPos = *static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos();
	_float	fLength = D3DXVec3Length(&(*m_pTransformCom->Get_Pos() - vecPlayerPos));

	if (45.f > fLength)
	{
		Set_ShadowMap(Engine::Get_ShadowLight().vLightPos, Engine::Get_ShadowLight().vLightAt);	//�׸��ڸ� ��ġ �����
		Engine::Add_RenderGroup(RENDER_SHADOWMAP, this);
	}

	//�����׷� �߰� : Interaction �ö��̴��� ����ü �ȿ� �� ��쿡�� �����׷쿡 �߰��Ѵ�
	if (m_pSphereColliderCom)
	{
		//�ö��̴� �ݰ��� ���� �����ϰ����� ������ �����̹Ƿ� �������� ���� ���� ����� ��ȯ���ش�
		if (Engine::Check_Sphere(vSpherePos, fSphereRadius))
			Engine::Add_RenderGroup(RENDER_NONALPHA, this);
		else
			//�ø��� ��� ������ ���������� �ִϸ��̼Ǹ� ���� �����ش�
		{
			// �ִϸ��̼� �÷���
			if (m_bDying && m_ePreUnitState == STATE_DEAD)
				m_pMeshCom->Play_Animation_End(m_dAnimationTime, m_pNeckBoneName);
			else
				m_pMeshCom->Play_Animation(m_dAnimationTime, 1, m_pNeckBoneName);

			if (m_pSphereColliderCom)
				m_pSphereColliderCom->Update_Component(m_dAnimationTime);
		}
	}
#pragma endregion CULLING

	//Engine::Add_RenderGroup(RENDER_SHADOWMAP, this);

	return NO_EVENT;
}

void CDino::Set_IsRide(_bool _bIsRide)
{
	m_bIsRide = _bIsRide;

	if (m_bIsRide && !m_bLandDino)
	{
		m_eUnitState = STATE_RIDE;
		m_bJump = true;
	}
	else if (!m_bIsRide && !m_bLandDino)
	{
		m_pTransformCom->Set_Rotate(ROT_X, D3DXToRadian(0.f));
	}
}

_bool CDino::IsRide_Key_Input(const _double & _dTimeDelta, wstring& wstrTargetLayer, CGameObject* pLookObject)
{
 	_bool bIsInput = false;

	//����Ż�͸� �����
	if (m_eUnitState == STATE_RIDE || m_eUnitState == STATE_LANDING)
	{
		m_tStat.fSpeed = 0.f;
		return false;
	}

	/// �յ� �̵�
	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		m_eUnitState = STATE_MOVE;
		m_eUnitMoveDir = UNIT_DIR_UP;
		m_tStat.fSpeed = m_fSpeed;

		if (Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80)
		{
			m_eUnitState = STATE_SPRINT;
			m_tStat.fSpeed = m_fSprint;
		}
		
		m_pTransformCom->Set_AccRotate(ROT_Y, -m_fWaistAngle * (_float)_dTimeDelta * m_fRotationSpeed);
		//static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0))->Set_AccAngleY(-m_fWaistAngle * (_float)_dTimeDelta * 2.f);

		bIsInput = true;
	}
	else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		m_eUnitState = STATE_MOVE;
		m_eUnitMoveDir = UNIT_DIR_DOWN;
		m_tStat.fSpeed = -m_fBackwardSpeed;

		bIsInput = true;
	}
	else if (m_eUnitState != STATE_ATTACK  && m_eUnitState != STATE_ROAR && m_eUnitState != STATE_HARVEST)
	{
		m_eUnitState = STATE_IDLE;
		m_eUnitMoveDir = UNIT_DIR_CENTER;
		m_tStat.fSpeed = 0.f;
	}

	if (m_eUpperDinoState != STATE_ATTACK && !m_bBuffRoar)
	{
		if (Engine::Get_BIMouseDown(DIM_LB))
		{
			if (!Harvest_Action(wstrTargetLayer, pLookObject))
			{
				m_eUpperDinoState = STATE_ATTACK;
				if (m_eUnitState == STATE_SPRINT)
					m_eUnitState = STATE_SPRINT;
				else
					m_eUnitState = m_eUnitState == STATE_MOVE ? STATE_MOVE : STATE_ATTACK;
			}
		}
	}
	
	Ride_Key_Input_Custum(_dTimeDelta);

	return bIsInput;
}

void CDino::Render_GameObject()
{
	// �ִϸ��̼� �÷���
	_double dPlayerTime = 1.0 + m_dAnimationSpeed;
	if (!Engine::Get_ShadowOnOff())
		dPlayerTime = 0.5 + m_dAnimationSpeed / 2.0;

	Dino_Play_Animation(dPlayerTime);

	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Update_Component(m_dAnimationTime);

	//// ���� üũ
	//Check_Buff(m_dAnimationTime);

	// ���̴� �İ�ü ����
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect, m_bDissolve, m_bIsSaddleIndex, m_eDinoPass);

	pEffect->End();

	Safe_Release(pEffect);

#ifdef _DEBUG
	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Render_SphereCollider();
#endif // !_DEBUG
}

void CDino::Render_GameObjectShadowMap()
{
	_double dPlayerTime = 0.5 + m_dAnimationSpeed / 2.0;

	Dino_Play_Animation(dPlayerTime);
	
	LPD3DXEFFECT pEffect = m_pShaderShadowMapCom->Get_EffectHandle();
	pEffect->AddRef();

	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);

	pEffect->Begin(NULL, 0);

	// ShadowMap Pass 0 �⺻, 1 ������, 2 �����׽�Ʈ
	if (m_bDissolve)
		pEffect->BeginPass(1);
	else
		pEffect->BeginPass(0);


	m_pMeshCom->Render_MeshesShadowMap(pEffect, m_bIsSaddleIndex);


	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

void CDino::Render_GameObjectShadow()
{
	_double dPlayerTime = 0.5 + m_dAnimationSpeed / 2.0;

	Dino_Play_Animation(dPlayerTime);

	LPD3DXEFFECT pEffect = m_pShaderShadowCom->Get_EffectHandle();
	pEffect->AddRef();

	m_pDevice->GetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);

	m_matWVP = m_pTransformCom->Get_WorldMatrix() * m_matView * m_matProj;
	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	m_matWLVB = m_matWLV * m_matScaleBias;

	pEffect->SetMatrix("g_matWorldViewProj", &m_matWVP);
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);
	pEffect->SetMatrix("g_matWorldLightViewProjBias", &m_matWLVB);

	D3DXMatrixInverse(&m_matWLVB, NULL, &m_pTransformCom->Get_WorldMatrix());
	D3DXVec3Transform(&m_vDir, &m_vecLightPos, &m_matWLVB);
	D3DXVec4Normalize(&m_vDir, &m_vDir);
	m_vDir.w = 0;

	pEffect->SetVector("g_vLightDir", &m_vDir);

	Engine::Get_RenderTargetTexture(pEffect, L"ShadowMap", "g_ShadowMapTexture");

	pEffect->Begin(NULL, 0);

	// Shadow Pass 0 �⺻, 1 ������, 2 �����׽�Ʈ
	if (m_bDissolve)
		pEffect->BeginPass(1);
	else
		pEffect->BeginPass(0);


	m_pMeshCom->Render_MeshesShadow(pEffect, m_bIsSaddleIndex);


	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

_bool CDino::HitBy(CUnit * _pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	if (dwAttackType == ATTACK_OPTION_DINO_GRAB && !m_pHangingMatrix)
	{
		m_bHanging = true;
		m_pHangingMatrix = &static_cast<CTransform*>(_pAttacker->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_WorldMatrix();

		return true;
	}
		
	if (false == CUnit::HitBy(_pAttacker, dwAttackType, vCollisionPos))
	{
		if (m_eUnitState == STATE_DEAD && !m_bCreateDeadBlood)
		{
			m_bCreateDeadBlood = true;

			CGameObject* pGameObject = nullptr;
			//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Blood");
			//NULL_CHECK_RETURN(pGameObject, false);
			//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), false);
			//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vCollisionPos);

			//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_BloodTest");
			//NULL_CHECK_RETURN(pGameObject, false);
			//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), false);
			//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vCollisionPos);

			pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_BloodParticle");
			NULL_CHECK_RETURN(pGameObject, false);
			FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), false);
			static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vCollisionPos);
		}

		return false;
	}

	CUnit* pAttacker = _pAttacker;
	_float fAddTorpid = 0.f;
	if (pAttacker->Get_UnitType() == CUnit::UNIT_BULLET)
	{
		CGameObject* pPlayer = Engine::Get_GameObject(L"Layer_Player");
		pAttacker = static_cast<CUnit*>(pPlayer);

		//if (BULLET_ARROW_TORPID == static_cast<CBullet*>(_pAttacker)->Get_BulletInfo().eBulletType)
		//	fAddTorpid = 2.5f;
	}

	auto& iter_find = find(m_ListTargetUnit.begin(), m_ListTargetUnit.end(), pAttacker);
	
	if (iter_find == m_ListTargetUnit.end())
		m_ListTargetUnit.emplace_back(pAttacker);

	_vec3 vPos = *static_cast<CTransform*>(_pAttacker->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos();
	_vec3 vDir = vCollisionPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	_float fLength = D3DXVec3Length(&(vCollisionPos - vPos));

	CGameObject* pGameObject = nullptr;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Blood");
	//NULL_CHECK_RETURN(pGameObject, false);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), false);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vCollisionPos);

	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_BloodTest");
	//NULL_CHECK_RETURN(pGameObject, false);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), false);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vCollisionPos);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_BloodParticle");
	NULL_CHECK_RETURN(pGameObject, false);
	FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), false);
	static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vCollisionPos);

	wstring wstrPrintFont = L"";
	wstrPrintFont += to_wstring((_int)_pAttacker->Get_Stat(STAT_ATTACK));
	_vec3 vAddPos = { 0.f, 0.f, 0.f };
	if (m_bIsTamed)
	{
		if(_pAttacker->Get_UnitType() == CUnit::UNIT_PLAYER || (_pAttacker->Get_UnitType()==CUnit::UNIT_DINO && static_cast<CDino*>(_pAttacker)->Get_NowTamingState()))
			static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(&vCollisionPos, wstrPrintFont, CWorldFont_UI::World_Font_Yellow, CWorldFont_UI::World_FontType_Up, vAddPos, 3.f);
		else
			static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(&vCollisionPos, wstrPrintFont, CWorldFont_UI::World_Font_Red, CWorldFont_UI::World_FontType_Up, vAddPos, 3.f);
	}
	else
		static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(&vCollisionPos, wstrPrintFont, CWorldFont_UI::World_Font_Green, CWorldFont_UI::World_FontType_Up, vAddPos, 3.f);

	if (BULLET_BOLA == dwAttackType)
	{
		// ���� �¾��� ��
		static_cast<CBullet*>(_pAttacker)->TobeBolaWrapped(m_pTransformCom->Get_WorldMatrix(), m_tStat.fSize);

		m_eUnitState = STATE_HOLDING;
		m_bAttackMode = true;
	}
	else if (m_bDying)
	{
		if (0 >= m_pInvenCom->Get_pInvenItem()->size())
		{
			//�̸� �����!!!!!!!!!
			static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Remove_WorldFont_UI(m_iNameFontID);
			Set_Dead();
		}
	}
	else if(STATE_TORPID		!=	m_eUnitState	&& 
			STATE_TORPID_IDLE	!=	m_eUnitState	&&
			STATE_TORPID_EAT	!=	m_eUnitState)
	{
		// ����ġ ���� ó��
		_float fTorpid = (pAttacker->Get_Stat(STAT_TORPIDRATE) + fAddTorpid) * pAttacker->Get_Stat(STAT_ATTACK);

		m_tStat.fParalysis += fTorpid;

		if (m_tStat.fParalysis > m_tStat.fMaxParalysis && !m_bIsTorpid)
		{
			// ���� ����
			m_bIsTorpid = true;

			m_tStat.fParalysis = m_tStat.fMaxParalysis;
			m_eUnitState = STATE_TORPID;
			m_bIsStatUpdate = true;			// ����� �ް� �ϱ�
			m_bAttackMode = false;			// ������ ����
		}
	}

	return true;
}

void CDino::Dino_Play_Animation(const _double & _time)
{
	if (m_bDying && m_ePreUnitState == STATE_DEAD)
		m_pMeshCom->Play_Animation_End(m_dAnimationTime * _time, m_pNeckBoneName, m_bBaby, m_pHeadBoneName, m_pTailBoneName);
	else
		m_pMeshCom->Play_Animation(m_dAnimationTime * _time, 1, m_pNeckBoneName, 0.f, 0.f, m_bBaby, m_pHeadBoneName, m_pTailBoneName);
}

CGameObject * CDino::Clone(void * _pArg/* = nullptr*/)
{
	return nullptr;
}

void CDino::Free()
{
	if (m_bIsClone)
	{
		Engine::Safe_Delete_Array(m_bIsSaddleIndex);
	}
	
	if (m_pCtrl)
		m_pCtrl->release();

	CUnit::Free();
}

_bool CDino::Set_IsTamed()
{
	// �̹� ���̹� ���¸� ���� false
	if (m_bIsTamed)
		return false;
		
	// ���̾� ����
	if (FAILED(Engine::Swap_GameObjectInLayer(L"Layer_Dino", L"Layer_Dino_Tamed", this)))
		return false;

	// ��ġ ����
	m_tStat.fParalysis = 0.f;		// ������ġ �ʱ�ȭ
	m_tStat.fAtk = m_fTamingAttack;

	//�ӽ÷� ���ֵ̹��� �� �� Ǯ��
	m_tStat.fHp = m_fTamingHp;
	m_tStat.fMaxHp = m_fTamingHp;
	m_tStat.fHp = m_tStat.fMaxHp;	// ü�� ����

	m_bIsTorpid = false;
	m_bIsTamed = true;
	m_bIsEquipSaddle = true;
	m_bAttackMode = false;

	// ���� �׸���
	if (m_bIsSaddleIndex)
		m_bIsSaddleIndex[m_iAsddleIndex] = true;

	return true;
}

void CDino::Set_TakeCare(_bool bTakeCare)
{
	m_bTakeCare = false;
	m_eUnitState = STATE_HAPPY;
	
	m_fGrowthTime += 120.f;

	m_wstrBabyMsg = L"";
}

HRESULT CDino::Add_Component()
{
	CComponent*		pComponent = nullptr;

	InvenUserUnitTypeID eInvenID = InvenUserUnitTypeID_Monster;
	pComponent = m_pShow_InvenCom = static_cast<CInven*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Inven", &eInvenID));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Show_Inven", pComponent);

	pComponent = m_pCalculatorCom = static_cast<CCalculator*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);

	//////////////////////////////////////////////////////////////////////////
	// �浹�׽�Ʈ
	//////////////////////////////////////////////////////////////////////////
	/*CSphereCollider::SPHEREINFO tSphereInfo;
	tSphereInfo.fRadius = 3.f;
	tSphereInfo.pBoneName = nullptr;
	tSphereInfo.eColliderType = CSphereCollider::COL_BODY;

	tSphereInfo.pTartgetMesh = m_pMeshCom;

	tSphereInfo.pTargetWorldMatix = &m_pTransformCom->Get_WorldMatrix();

	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tSphereInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);

	//////////////////////////////////////////////////////////////////////////
	*/

	return S_OK;
}

HRESULT CDino::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	_mat matWorld, matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	// ������̺� ����
	_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &matProj);
	_pEffect->SetFloat("g_fAmount", m_fDissolveAmount);
	_pEffect->SetVector("g_vSetColor", &_vec4(m_vColor, 1.f));

	_pEffect->SetVector("g_DinoColor1", &D3DXVECTOR4(m_mapColor1[m_iRandColor[0]] / 255.f, 0.f));
	_pEffect->SetVector("g_DinoColor2", &D3DXVECTOR4(m_mapColor2[m_iRandColor[1]] / 255.f, 0.f));
	_pEffect->SetVector("g_DinoColor3", &D3DXVECTOR4(m_mapColor3[m_iRandColor[2]] / 255.f, 0.f));
	_pEffect->SetFloat("g_fShadowBias", 0.01f);

	m_pDissolveTextureCom->Set_Texture(_pEffect, "g_DissolveTexture");

	return S_OK;
}

_int CDino::Check_Arg(void * _pArg)
{
	if (_pArg)
	{
		if (dynamic_cast<OBJINFO*>((OBJINFO*)_pArg))
		{
			OBJINFO tInfo;
			memcpy(&tInfo, _pArg, sizeof(OBJINFO));

			//Set_Scale(tInfo.vScale);	//�켱�� ���溰 �����Ϸ�
			Set_Rot(tInfo.vRot.x, tInfo.vRot.y, tInfo.vRot.z);
			Set_Pos(tInfo.vPos.x, tInfo.vPos.y + 1.f, tInfo.vPos.z);

			Add_ExpToLevel(tInfo.dwLevel);	//���� �� �Լ�

			m_vColor = tInfo.vColor;

			if(tInfo.dwOption == 99)
				m_bBaby = true;

			return tInfo.dwOption;
		}
		else
		{
			ERR_MSG(L"Wrong Format Argument");
			return 0;
		}
	}
	return 0;
}

void CDino::Dino_Move(const _double & _dTimeDelta)
{
	PxVec3 PxPos(0);

	_vec3 vPos = *m_pTransformCom->Get_Pos();
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	// MOVE �� ���� m_vMovePos = ���� ��ǥ
	// SPRINT �� ���� m_vMovePos = �¾��� ��� ��ǥ
	m_vMovePos.y = 0.f;
	vPos.y = 0.f;

	_vec3 vDir = m_vMovePos - vPos;
	_float fDis = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vCross;
	_float fAngle = m_pCalculatorCom->Get_Angle(&vPos, &vLook, &m_vMovePos, &vCross);
	m_eUnitMoveDir = UNIT_DIR_UP;

	// �ϴ� �Ķ� �¾��� �� ������ �޷����� �� �׽�Ʈ��
	if (m_eUnitState == STATE_SPRINT)
	{
		vDir = -vLook;

		if (70.f <= fDis)
		{
			m_eUnitState = STATE_IDLE;
			return;
		}
	}
	else
	{
		// ���ϴ� ��ġ�� ����
		if (m_fAttackRange >= fDis)
		{
			//���ݸ�尡 �ƴϸ� ���� ������� ����
			if (!m_bAttackMode)
			{
				m_eUnitState = STATE_IDLE;
				return;
			}

			//���ݸ���� ���� �������� �Ǵ� ����� ������ ������ ȸ���������ش�.
			if (m_bAttackMode && D3DXToDegree(fAngle) < 15.f /*vCross.y >= -0.3f && vCross.y <= 0.3f*/)
			{
				m_eUnitState = STATE_ATTACK;
				m_eUpperDinoState = STATE_ATTACK;
			}
			else
			{
				if (vCross.y > 0.f)
					m_pTransformCom->Set_AccRotate(ROT_Y, fAngle * (_float)_dTimeDelta * -m_fRotationSpeed);
				else
					m_pTransformCom->Set_AccRotate(ROT_Y, fAngle * (_float)_dTimeDelta * m_fRotationSpeed);
			}

			return;
		}

		// �⺻ �̵� �ÿ��� ȸ���Ѵ�.
		if (vCross.y > 0.f)
			m_pTransformCom->Set_AccRotate(ROT_Y, fAngle * (_float)_dTimeDelta * -m_fRotationSpeed);
		else
			m_pTransformCom->Set_AccRotate(ROT_Y, fAngle * (_float)_dTimeDelta * m_fRotationSpeed);
	
	}

	PxPos += vDir * _float(m_tStat.fSpeed * _dTimeDelta);

	const PxU32 flags = m_pCtrl->move(PxPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());
	// �����̴ٰ� ���� �ε����� �� �ٸ� ������ �̵�
	if (flags & PxControllerCollisionFlag::eCOLLISION_SIDES && m_eUnitState != STATE_ATTACK)
	{
		vPos.x += rand() % 40 - 20;
		vPos.z += rand() % 40 - 20;
		m_vMovePos = vPos;
	}
}

void CDino::Dino_Move_Ride(const _double & _dTimeDelta)
{
	if (!Dino_Move_Ride_Custum(_dTimeDelta))
		return;

	PxVec3 PxPos(0);

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);	

	PxPos -= vLook * _float(m_tStat.fSpeed  * _dTimeDelta * (m_eUnitState == STATE_SPRINT ? 1.5f : 1.f));

	const PxU32 flags = m_pCtrl->move(PxPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());
}

void CDino::Dino_Move_Tamed(const _double & _dTimeDelta)
{
	if (m_pTartgetTransform && !m_bIsAttack && !m_bFixedMove)
		m_vMovePos = *m_pTartgetTransform->Get_Pos();

	PxVec3 PxPos(0);

	_vec3 vPos = *m_pTransformCom->Get_Pos();
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	if (m_bLandDino)
	{
		m_vMovePos.y = 0.f;
		vPos.y = 0.f;
	}

	_vec3 vDir = m_vMovePos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vCross;
	_float fAngle = m_pCalculatorCom->Get_Angle(&vPos, &vLook, &m_vMovePos, &vCross);
	m_eUnitMoveDir = UNIT_DIR_UP;
	_float fDis = m_pCalculatorCom->Get_Distance(&vPos, &m_vMovePos);
	
	// �Ʊ� ���� ��å �Ÿ� �÷��ֱ�
	if (m_bBaby && m_eTakeCare == WALK && m_bTakeCare)
		m_fWalkDis += 0.1f;

	if (m_fAttackRange >= fDis)
	{
		//���ݸ�尡 �ƴϸ� ���� ������� ����
		if (!m_bAttackMode)
		{
			m_eUnitState = STATE_IDLE;
			return;
		}

		//���ݸ���� ���� �������� �Ǵ� ����� ������ ������ ȸ���������ش�.
		if (m_bAttackMode && D3DXToDegree(fAngle) < 15.f )
		{
			m_eUnitState = STATE_ATTACK;
			m_eUpperDinoState = STATE_ATTACK;
		}
		else
		{
			if (vCross.y > 0.f)
				m_pTransformCom->Set_AccRotate(ROT_Y, fAngle * (_float)_dTimeDelta * -m_fRotationSpeed);
			else
				m_pTransformCom->Set_AccRotate(ROT_Y, fAngle * (_float)_dTimeDelta * m_fRotationSpeed);
		}

		return;
	}

	if (vCross.y > 0.f)
		m_pTransformCom->Set_AccRotate(ROT_Y, fAngle * (_float)_dTimeDelta * -m_fRotationSpeed);
	else
		m_pTransformCom->Set_AccRotate(ROT_Y, fAngle * (_float)_dTimeDelta * m_fRotationSpeed);

	PxPos -= vLook * _float(m_tStat.fSpeed * _dTimeDelta);
	m_pCtrl->move(PxPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());
	
}

// ���ڷ� ���� ���� ����
void CDino::Attack_OnPlayer(_float fRange)
{
	CGameObject* pPlayer = Engine::Get_GameObject(L"Layer_Player");

	if (pPlayer)
	{
		CTransform* pPlayerTransform = static_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_float fDis = m_pCalculatorCom->Get_Distance(m_pTransformCom->Get_Pos(), pPlayerTransform->Get_Pos());

		if (fDis <= fRange)
		{
			m_pTartgetTransform = pPlayerTransform;
			m_bAttackMode = true;
		}
		else
		{
			m_bAttackMode = false;
			m_eUnitState = STATE_IDLE;
			m_iRandNum = rand() % 3;
			m_dIdleMaxTime = rand() % 5 + 5;
		}
	}
}

void CDino::Set_Follow(_bool bFollow)
{
	m_bFollow = bFollow;

	if (m_bFollow && m_bFixedMove)
		m_bFixedMove = false;

	auto& iter_find = m_mapRingCommand.find(L"Moving");

	if (iter_find == m_mapRingCommand.end())
		return;

	Setting_Ring(nullptr, m_bFollow ? L"���߱�" : L"������", &iter_find->second[RINGMOVING_FOLLOW]);
}

void CDino::Attack_OnTarget(CGameObject* pTarget)
{
	m_bAttackMode = true;
	m_pTagetObject = pTarget;
	CTransform* vPos = static_cast<CTransform*>(pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC));
	m_pTartgetTransform = vPos;
}

void CDino::MoveToTargetPos(const _vec3 & vTargetPos)
{
	m_vMovePos = vTargetPos;
}

void CDino::Set_SkipFlag(const _ulong & dwFlag)
{
	m_dwFlagSkip = dwFlag;
}

void CDino::Check_IsEndAnimation()
{
	//if (m_ePreUnitState == STATE_MOVE && m_eUnitState == STATE_ATTACK)
	//{
	//	m_eUnitState = STATE_MOVE;
	//	if (m_pMeshCom->Is_AnimationsetEnd(0.1))
	//		m_eUnitState = STATE_ATTACK;
	//}

	if (m_ePreUnitState != m_eUnitState)
		return;

	switch (m_eUnitState)
	{
	case CUnit::STATE_IDLE:
		break;
	case CUnit::STATE_IDLE2:
	case CUnit::STATE_IDLE3:
		if (m_pMeshCom->Is_AnimationsetEnd(0.1))
			m_eUnitState = STATE_IDLE;
		break;
	case CUnit::STATE_MOVE:
		break;
	case CUnit::STATE_SPRINT:
		break;
	case CUnit::STATE_JUMPING:
		if (m_pMeshCom->Is_AnimationsetEnd(0.1))
			m_eUnitState = STATE_FALLING;
		break;
	case CUnit::STATE_FALLING:
		break;
	case CUnit::STATE_LANDING:
		if (m_pMeshCom->Is_AnimationsetEnd(0.2))
		{
			m_eUnitState = STATE_IDLE;
			// �ٴڰ� �浹���� ��
			m_fJumpTime = 0.f;
			m_bJump = false;
		}
		break;
	case CUnit::STATE_ATTACK:
			if (m_pMeshCom->Is_AnimationsetEnd(0.2))
			{
				m_eUnitState = STATE_IDLE;
				m_eUpperDinoState = STATE_IDLE;
				m_bIsAttack = false;
			}
		break;
	case CUnit::STATE_HURT:
		break;
	case CUnit::STATE_EAT:
		break;
	case CUnit::STATE_HARVEST:
		if (m_pMeshCom->Is_AnimationsetEnd(0.2))
		{
			m_eUnitState = STATE_IDLE;
			m_eUpperDinoState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_INVENTORY:
		break;
	case CUnit::STATE_DEAD:
		break;
	case CUnit::STATE_TORPID:						// ���� ����
		if (m_pMeshCom->Is_AnimationsetEnd(0.2))
			m_eUnitState = STATE_TORPID_IDLE;
		break;
	case CUnit::STATE_TORPID_IDLE:
		break;
	case CUnit::STATE_TORPID_TAMED:					// �������� ���ֿ̹Ϸ�
		if (m_pMeshCom->Is_AnimationsetEnd(0.2))
		{
			m_eUnitState = STATE_IDLE;
			m_bIsTorpid = false;
		}
		break;
	case CUnit::STATE_TORPID_WILD:					// �������� �߻����·� �Ͼ��
		if (m_pMeshCom->Is_AnimationsetEnd(0.2))
		{
			m_eUnitState = STATE_SPRINT;
			m_bIsTorpid = false;
		}
		break;
	case CUnit::STATE_TORPID_EAT:					// ������ ���� �Ա�
		if (m_pMeshCom->Is_AnimationsetEnd(0.2))
			m_eUnitState = STATE_TORPID_IDLE;
		break;
	case CUnit::STATE_RIDE:
		if (m_pMeshCom->Is_AnimationsetEnd(0.2))
			m_eUnitState = STATE_IDLE;
		break;
	case CUnit::STATE_EGG:
		if (m_pMeshCom->Is_AnimationsetEnd(0.7) && !m_bLayEgg)
		{
			m_bLayEgg = true;
			m_bPostpartumCare = true;
			// ��
			OBJINFO tInfo;
			tInfo.vPos = Get_Pos();
			tInfo.vRot = { 0.f,0.f,0.f };
			tInfo.vScale = { 0.01f,0.01f,0.01f };
			//tInfo.dwOption = 99;
			CGameObject* pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Egg", &tInfo));
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Egg", pGameObject), );
		}

		if (m_pMeshCom->Is_AnimationsetEnd(0.1))
		{
			m_eUnitState = STATE_IDLE;
			m_dAnimationSpeed = 0.0;
			m_bLayEgg = false;
		}
		break;
	case CUnit::STATE_ROAR:
		if (m_pMeshCom->Is_AnimationsetEnd(0.2) && m_wstrDinoSpecies == L"Para")
		{
			m_eUnitMoveDir = UNIT_DIR_UP;
			m_eUnitState = STATE_SPRINT;
			m_vMovePos = *m_pTransformCom->Get_Pos();
			m_bIsAttack = false;
		}
		else if (m_pMeshCom->Is_AnimationsetEnd(0.2) && m_wstrDinoSpecies == L"Yutyra")
		{
			m_eUnitState = STATE_IDLE;
			m_bBuffRoar = false;
		}
		break;
	case  CUnit::STATE_HAPPY:
		if (m_pMeshCom->Is_AnimationsetEnd(0.1))
			m_eUnitState = STATE_IDLE;
		break;
	default:
		break;
	}
}

void CDino::Check_IsUpperEndAnimation()
{
	//if (m_ePreUpperDinoState == STATE_HARVEST)
	//{
	//	m_eUnitState = STATE_MOVE;
	//	if (m_pMeshCom->Is_AnimationsetEnd(0.1))
	//		m_eUnitState = STATE_ATTACK;
	//}

	// ù �����ӿ� �ٷ� ���� �ʰ� Pre ������Ʈ�� ���
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
			// ���� ����
			m_bBuffRoar = false;
			m_eUpperDinoState = STATE_IDLE;
		}
		break;
	default:
		break;
	}
}

void CDino::Check_IsHolding(const _double& _dTimeDelta)
{
	if (m_bIsHolding)
	{
		m_dHoldingTime += _dTimeDelta;

		if (10 < m_dHoldingTime)
		{
			// ���� ����
			m_dHoldingTime = 0;
			m_bIsHolding = false;

			m_eUnitState = STATE_IDLE;
		}
		else if (m_bIsTorpid)
		{
			// Ȧ���� ����
			m_dHoldingTime = 0;
			m_bIsHolding = false;
		}
	}
}

_int CDino::Check_Torpid(const _double & _dTimeDelta)
{
	if (!m_bIsTamed && m_bIsTorpid)
	{
		CGameObject* pPlayer = Engine::Get_GameObject(L"Layer_Player");
		_vec3 vPos = *m_pTransformCom->Get_Pos();

		_vec3 vPlayerPos = *static_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos();
		_float fTargetDis = D3DXVec3Length(&(vPos - vPlayerPos));

		if(20.f >= fTargetDis)
			static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Green_GoTaming);

		/// �������� ����
		// ������ġ ���ݾ� ���
		m_tStat.fParalysis -= 0.2f * (_float)_dTimeDelta;

		if (0.f >= m_tStat.fParalysis)
		{
			// �������� ����� (���̹� ����)
			m_tStat.fParalysis = 0.f;
			m_eUnitState = STATE_TORPID_WILD;

			return NO_EVENT;
		}

		// ���̹� �����ֱ�
		return Taming_Eat(_dTimeDelta);
	}

	return NO_EVENT;
}

_int CDino::Taming_Eat(const _double & _dTimeDelta)
{
	m_dTamingEatTime += _dTimeDelta;

	// ������� �ƽ��� �ƴ� �� ���� �Ա�
	if (3 < m_dTamingEatTime && m_tStat.fMaxHunger - 1.f > m_tStat.fHunger)
	{
		if (0 < m_pShow_InvenCom->Now_Have_ItemAmount(m_eTamingFood))
		{
			m_dTamingEatTime = 0.0;

			// ���̹� ���� �Ա�
			m_pShow_InvenCom->Remove_Item(m_eTamingFood, 1);
			static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Match_ItemTex_Info();
			
			m_eUnitState = STATE_TORPID_EAT;

			// ���ü�ġ �ø��� (���ü�ġ�� �ӽ÷� fParalysis ���)
			m_tStat.fTamingRate += 1.f;

			// ���ü�ġ�� �����̻� �ö󰡸� ���̹� �Ϸ�
			if (m_tStat.fMaxTamingRate <= m_tStat.fTamingRate)
			{
				m_tStat.fTamingRate = m_tStat.fMaxTamingRate;

				if (Set_IsTamed())
				{
					m_eUnitState = STATE_TORPID_TAMED;
					m_bAttackMode = false;
					static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Green_TamingSuccess);
					static_cast<CTamingName_UI*>(Engine::Get_GameObject(L"Taming_Name_UI", 0))->Making_a_Name(this, m_wstrDinoSpeciesKor);
					return DINO_TAMED;
				}
			}
		}
	}

	return NO_EVENT;
}

void CDino::Wild_Action(const _double & _dTimeDelta)
{
	if (m_bAttackMode && m_pTartgetTransform && !m_bIsAttack)
	{
		_mat vWorldMat = m_pTartgetTransform->Get_WorldMatrix();
		memcpy(&m_vMovePos, &vWorldMat.m[3][0], sizeof(_vec3));	

		m_eUnitState = STATE_MOVE;
	}

	if (m_eUnitState != STATE_DEAD || m_eUnitState != STATE_HOLDING || !m_bIsTorpid)
		CDino::Dino_Idle_Move(_dTimeDelta);

}

void CDino::Tamed_Action(const _double & _dTimeDelta)
{
	if (m_bDying || 
		m_eUnitState == STATE_TORPID_TAMED	|| m_eUnitState == STATE_EGG	||
		m_eUnitState == STATE_HAPPY			|| m_eUnitState == STATE_LANDING)
		return;

	if (!Dino_Baby_Parenting(_dTimeDelta))
		return;

	// ¦���� ���, �������� �ʴ� ��� ��
	if(!Dino_Action_By_Mode(_dTimeDelta))
		return;

	if (m_bAttackMode && m_pTagetObject)
	{
		// �װų� ���� ���̸� ����x
		if (static_cast<CUnit*>(m_pTagetObject)->Get_Dying() ||
			static_cast<CUnit*>(m_pTagetObject)->Get_UnitState() == CUnit::STATE_TORPID ||
			static_cast<CUnit*>(m_pTagetObject)->Get_UnitState() == CUnit::STATE_TORPID_IDLE)
			m_bAttackMode = false;
		else
		{
			m_eUnitState = STATE_SPRINT;
			Is_Attack(L"Layer_Dino");
		}
	}
	else if (m_bFixedMove)
	{
		m_bFollow = false;
		m_eUnitState = STATE_SPRINT;
		
		m_vMovePos.y = 0;
		_vec3 vPos = Get_Pos();
		vPos.y = 0;
		_float fDis = m_pCalculatorCom->Get_Distance(&m_vMovePos, &vPos);

		if (fDis <= 5.f)
		{
			m_eUnitState = STATE_IDLE;
			m_bFixedMove = false;
		}
	}
	else if (m_bFollow)
	{
		CGameObject* pPlayer = Engine::Get_GameObject(L"Layer_Player");
		if (pPlayer == nullptr)
			return;

		// �÷��̾ ����Ÿ�� ������ ������ ��ǥ�� �����
		CTransform* pPlayerTransform = nullptr;
		if (static_cast<CPlayer*>(pPlayer)->Get_IsRide())
		{
			CDino* pDino = static_cast<CPlayer*>(pPlayer)->Get_RideDino();
			pPlayerTransform = static_cast<CTransform*>(pDino->Get_Component(L"Com_Transform", ID_DYNAMIC));
		}
		else
			pPlayerTransform = static_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));

		m_pTartgetTransform = pPlayerTransform;
		_float fDis = m_pCalculatorCom->Get_Distance(m_pTransformCom->Get_Pos(), pPlayerTransform->Get_Pos());

		if (fDis >= m_fFllowDis[m_eDistance] + 15.f)
			m_eUnitState = STATE_SPRINT;
		else if (fDis >= m_fFllowDis[m_eDistance] + 2.f && m_eUnitState != STATE_SPRINT)
			m_eUnitState = STATE_MOVE;
		else if (fDis <= m_fFllowDis[m_eDistance])
			m_eUnitState = STATE_IDLE;
	}
	else
		m_eUnitState = STATE_IDLE;

	// ����ֵ��� ���� ����
	if (!m_bLandDino && m_bFollow)
	{
		Dino_Move_Fly(_dTimeDelta);
		return;
	}

	if (m_eUnitState != STATE_IDLE)
	{
		Dino_Move_Tamed(_dTimeDelta);

		// �Ȱų� �۶� ���� �ð����� ����� ����
		if (1.f < m_fCreateIntv)
		{
			m_fCreateIntv = 0.f;

			CGameObject* pGameObject = nullptr;

			pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Dust", &m_fDustScale);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), );
			_vec3 vPos = *m_pTransformCom->Get_Pos() + _vec3(0.f, 0.1f, 0.f);
			static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);
		}
	}
}

void CDino::Check_Dying(const _double & _dTimeDelta)
{
	if (!m_bDying)
		return;

	m_dDyingTime += _dTimeDelta;
}

void CDino::DinoHead_ToCamLook()
{
	_vec3 vDinoLook, vDinoLookX, vDinoLookY;
	m_pTransformCom->Get_Info(INFO_LOOK, &vDinoLook);
	vDinoLook *= -1.f;
	
	_mat matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	
	D3DXVec3TransformNormal(&vDinoLook, &vDinoLook, &matView);

	vDinoLookX = vDinoLook;
	//vDinoLookY = vDinoLookX = vDinoLook;

	vDinoLookX.y = 0.f;
	//vDinoLookY.x = 0.f;

	D3DXVec3Normalize(&vDinoLookX, &vDinoLookX);
	//D3DXVec3Normalize(&vDinoLookY, &vDinoLookY);

	// ������ ����
	fabsf(vDinoLookY.y);
	if (0.f > vDinoLookY.z)
		vDinoLookY *= -1.f;


	_float fRatioX = D3DXVec3Dot(&vDinoLookX, &_vec3(0.f, 0.f, 1.f));		// �㸮 ����
	//_float fRatioY = D3DXVec3Dot(&vDinoLookY, &_vec3(0.f, 0.f, 1.f));		// �� ����

	m_fWaistAngle = acosf(fRatioX);
	m_fWaistAngle *= 0.15f;

	if (0 > vDinoLookX.x)
		m_fWaistAngle *= -1.f;

	if (0.4f < m_fWaistAngle)
		m_fWaistAngle = 0.4f;
	else if(-0.4f > m_fWaistAngle)
		m_fWaistAngle = -0.4f;

	//m_fNectAngle = acosf(fRatioY);
	//m_fNectAngle *= 0.5f;
	//if(0 > vDinoLookY.y)
	//	m_fNectAngle *= -1.f;
	//
	//m_fNectAngle -= 0.2f;
}

void CDino::Is_Attack(const wstring& LayerTag, list<CGameObject*>* pTargetObject)
{		
	if (m_bIsAttack)
	{
		_double dAnimationtime = m_pMeshCom->Get_UpperAccTime();
		if (dAnimationtime > m_dAttackStartTime && dAnimationtime < m_dAttackEndTime)
		{
			list<CSphereCollider::SPHEREINFO>* pAttackCollist = m_pSphereColliderCom->Get_listCollider(CSphereCollider::COL_ATTACK);

			if (nullptr == pAttackCollist)
				return;

			for (auto& pAttackCol : *pAttackCollist)
			{
				_vec3 vWorldPos;
				memcpy(&vWorldPos, &pAttackCol.matColMatrix.m[INFO_POS][0], sizeof(_vec3));

				CCollisionFunc::AttackOnUnit(this, &vWorldPos, &pAttackCol.fRadius, LayerTag, CSphereCollider::COL_HIT, ATTACK_OPTION_DINO_ATTACK, nullptr, pTargetObject);
			}
		}
	}
}

_bool CDino::Refresh_TargetList(const _double & _dTimeDelta)
{
	if (m_bAttackMode)
	{
		if (Before_Sort_TagetList())
			return false;

		//ChaseMode�� ó�� ���� �޾��� �� ������
		//bChaseNotDetected�� n�� �ڿ��� �Ÿ� �Ǻ��� ���� ��� ã�� ��  
		if (m_bChaseMode && m_bChaseNotDetected)
		{
			m_dChaseTime += _dTimeDelta;

			if (m_dChaseTime >= CHASETIME)
			{
				m_bChaseNotDetected = false;
				m_dChaseTime = 0.f;
			}
		}
		
		Sort_TargetList(m_fDetectedRange);

		// ������ ����� ���� ���
		if (m_ListTargetUnit.empty())
		{
			m_bAttackMode = false;
			m_pTartgetTransform = nullptr;

			//��׷� ���ȴ� ������ ������ �װ����� �̵�
			if (m_vReturnPos.x != 0.f)
			{
				m_vMovePos = m_vReturnPos;
				m_vReturnPos.x = 0.f;
			}

			if (m_eUnitState != STATE_IDLE)
				m_eUnitState = STATE_MOVE;

		}
		else
		{
			if (m_vReturnPos.x == 0.f)
				m_vReturnPos = Get_Pos();

			m_eUnitState = STATE_MOVE;
		}
	}

	return true;
}

void CDino::Dino_Idle_Move(const _double & _dTimeDelta)
{
	if (m_eUnitState == STATE_IDLE)
		m_dIdleTime += _dTimeDelta;

	if (m_dIdleTime >= m_dIdleMaxTime && m_eUnitState == STATE_IDLE && m_dIdleMaxTime > 0.f)
	{
		m_dIdleTime = 0.f;
		m_iRandNum = rand() % 3;
		m_dIdleMaxTime = rand() % 5 + 5;
		m_eUnitState = m_eNextRandomAnim[m_iRandNum];

		//�ٲｺ����Ʈ �ѹ� ���� �����ֱ����ؼ� ����
		return;
	}

	if (m_eUnitState == STATE_MOVE || m_eUnitState == STATE_SPRINT)
	{
		m_dIdleTime = 0.f;
		if (m_bLandDino || (!m_bLandDino && !m_bJump))
		{
			Dino_Move(_dTimeDelta);

			// �Ȱų� �۶� ���� �ð����� ����� ����
			if (1.f < m_fCreateIntv)
			{
				m_fCreateIntv = 0.f;

				CGameObject* pGameObject = nullptr;

				//m_fDustScale = 0.001f;

				pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Dust", &m_fDustScale);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), );
				_vec3 vPos = *m_pTransformCom->Get_Pos() + _vec3(0.f, 0.1f, 0.f);
				static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);
			}
		}
		else
			Dino_Move_Fly(_dTimeDelta);
	}
}

void CDino::Load_Collider(const wstring & FilePath)
{
	m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider"));
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", m_pSphereColliderCom);

	HANDLE hFile = CreateFile(FilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwstrByte = 0;
	DWORD dwByte = 0;
	TCHAR* pBoneName = nullptr;

	_float	fRadius = 1.f;
	_vec3	vPos = { 0.f,0.f,0.f };
	_bool	bHead = false;
	_int	iSize = 0;
	while (true)
	{
		for (_int i = 0; i < 3; ++i)
		{
			ReadFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);
			for (_int j = 0; j < iSize; ++j)
			{
				ReadFile(hFile, &dwstrByte, sizeof(DWORD), &dwByte, nullptr);
				pBoneName = new TCHAR[dwstrByte];
				ReadFile(hFile, pBoneName, dwstrByte, &dwByte, nullptr);

				ReadFile(hFile, &fRadius, sizeof(_float), &dwByte, nullptr);
				ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
				ReadFile(hFile, &bHead, sizeof(_bool), &dwByte, nullptr);

				m_pSphereColliderCom->Add_SphereCollider(pBoneName, this, (CSphereCollider::COL_TYPE)i, vPos, fRadius);

				Safe_Delete_Array(pBoneName);
			}
		}
		break;
	}

	CloseHandle(hFile);
}

void CDino::Load_DinoInfo(const wstring & FilePath, const wstring & DinoName, _float fDivide)
{
	WCHAR str[255] = L"";

	GetPrivateProfileString(DinoName.c_str(), L"fMaxHp", 0, str, 255, FilePath.c_str());
	wstring ss = str;
	m_tStat.fMaxHp = stof(ss) * fDivide;
	m_tStat.fHp = m_tStat.fMaxHp;
	m_tStat.fMaxParalysis = m_tStat.fMaxHp;

	GetPrivateProfileString(DinoName.c_str(), L"fTamingHp", 0, str, 255, FilePath.c_str());
	ss = str;
	m_fTamingHp = stof(ss) * fDivide;

	GetPrivateProfileString(DinoName.c_str(), L"fAttack", 0, str, 255, FilePath.c_str());
	ss = str;
	m_tStat.fAtk = stof(ss) * fDivide;

	GetPrivateProfileString(DinoName.c_str(), L"fTamingAttack", 0, str, 255, FilePath.c_str());
	ss = str;
	m_fTamingAttack = stof(ss) * fDivide;

	GetPrivateProfileString(DinoName.c_str(), L"fRotationSpeed", 0, str, 255, FilePath.c_str());
	ss = str;
	m_fRotationSpeed = stof(ss) * fDivide;

	GetPrivateProfileString(DinoName.c_str(), L"fAttackRange", 0, str, 255, FilePath.c_str());
	ss = str;
	m_fAttackRange = stof(ss) * fDivide;

	GetPrivateProfileString(DinoName.c_str(), L"fSpeed", 0, str, 255, FilePath.c_str());
	ss = str;
	m_fSpeed = stof(ss) * fDivide;

	GetPrivateProfileString(DinoName.c_str(), L"fSprint", 0, str, 255, FilePath.c_str());
	ss = str;
	m_fSprint = stof(ss) * fDivide;

}

void CDino::Sort_TargetList(_float fDetectRange)
{
	//�÷��̾� ����Ʈ�� �߰�, ���濡�� ���� �������� �� �÷��̾ ��׷ο� ���Խ��Ѿ��ؼ�
	CGameObject* pPlayer = Engine::Get_GameObject(L"Layer_Player");
	_vec3 vPos = *m_pTransformCom->Get_Pos();

	// �÷��̾� ���� �˻�
	if (pPlayer)
	{
		_vec3 vPlayerPos = *static_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos();
		_float fTargetDis = D3DXVec3Length(&(vPos - vPlayerPos));

		//���� �������� �÷��̾� ������ �߰��ϴµ� �̹� ����Ʈ�� ������ �н�
		if (fTargetDis <= fDetectRange && m_ListTargetUnit.size() < 2)
		{
			auto& iter_find = find(m_ListTargetUnit.begin(), m_ListTargetUnit.end(), pPlayer);

			if (iter_find == m_ListTargetUnit.end())
			{
				m_ListTargetUnit.emplace_back(pPlayer);

				// �߰����� �� ������ FRONT�� ������
				// �÷��̾ ������ Ÿ�������� ������ front�� �� ����
				if (m_ListTargetUnit.front() != pPlayer)
					m_ListTargetUnit.reverse();
			}

			// �÷��̾ ���ݹ��� �� ���濡 Ÿ�� ����Ʈ�� �ȵ��� �߰�
			if (static_cast<CPlayer*>(m_ListTargetUnit.front())->Get_IsRide())
			{
				CGameObject* pDino = static_cast<CPlayer*>(m_ListTargetUnit.front())->Get_RideDino();
				auto& iter_find = find(m_ListTargetUnit.begin(), m_ListTargetUnit.end(), pDino);

				if (iter_find == m_ListTargetUnit.end())
					m_ListTargetUnit.emplace_back(pDino);
			}
		}
	}
	
	if (m_ListTargetUnit.empty())
	{
		m_bChaseMode = false;
		return;
	}

	if (m_ListTargetUnit.size() >= 2)
	{
		m_bChaseMode = false;
		m_bChaseNotDetected = false;
	}
	//ó�� n�ʴ� ������ �Ѱ� �� ���Ŀ��� ����Ʈ���鼭 ������ ���Ѿ��Ѵ�..
	else if (m_ListTargetUnit.size() == 1 && static_cast<CUnit*>(m_ListTargetUnit.front())->Get_UnitType() == CUnit::UNIT_PLAYER && !m_bChaseNotDetected && !m_bChaseMode)
	{
		m_pTartgetTransform = static_cast<CTransform*>(m_ListTargetUnit.front()->Get_Component(L"Com_Transform", ID_DYNAMIC));
		m_bChaseMode = true;
		m_dChaseTime = 0.f;
		m_bChaseNotDetected = true;

		// ���� �÷��̾ ������ Ÿ�� ������ �µ� ���� ��� ����
		//CDino* pRideDino = static_cast<CPlayer*>(m_ListTargetUnit.front())->Get_RideDino();
		//if (pRideDino)
		//	m_ListTargetUnit.emplace_back(pRideDino);

		return;
	}

	//���� ����Ʈ ���鼭 �����ֵ� ����, �Ÿ��վֵ� ����
	if (!m_bChaseNotDetected)
	{
		auto& iter = m_ListTargetUnit.begin();
		for (; iter != m_ListTargetUnit.end();)
		{
			if (static_cast<CUnit*>((*iter))->Get_UnitState() == STATE_DEAD)
				iter = m_ListTargetUnit.erase(iter);
			else
			{
				CTransform* pTargetTransform = static_cast<CTransform*>((*iter)->Get_Component(L"Com_Transform", ID_DYNAMIC));
				_vec3 pDstPos = *pTargetTransform->Get_Pos();

				_float fTargetDis = D3DXVec3Length(&(vPos - pDstPos));
				if (fTargetDis > fDetectRange)
					iter = m_ListTargetUnit.erase(iter);
				else
					iter++;
			}
		}
	}

	if (m_ListTargetUnit.empty())
		return;

	m_ListTargetUnit.sort([&](auto& a, auto& b)->_bool{
		_vec3 v1 = *static_cast<CTransform*>(a->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos();
		_vec3 v2 = *static_cast<CTransform*>(b->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos();
		
		return D3DXVec3Length(&(v1 - vPos)) < D3DXVec3Length(&(v2 - vPos));
	});

	m_pTartgetTransform = static_cast<CTransform*>(m_ListTargetUnit.front()->Get_Component(L"Com_Transform", ID_DYNAMIC));

	// �ϴ� �����ȿ� �÷��̾�� ��鿩�� ���游 üũ
	//list<CGameObject*>* pDinos = Get_LayerList(L"Layer_Dino_Tamed");

	//for (auto& pDino : *pDinos)
	//{
	//	if(static_cast<CDino*>(pDino)->Get_Dying() || pDino == this)
	//		continue;
	//
	//	CTransform* pTargetTransform = static_cast<CTransform*>(pDino->Get_Component(L"Com_Transform", ID_DYNAMIC));
	//	_vec3 pDstPos = *pTargetTransform->Get_Pos();
	//
	//	_float fTargetDis = D3DXVec3Length(&(vPos - pDstPos));
	//
	//	if (fTargetDis > fDetectRange)
	//		continue;
	//
	//	pTargetList.emplace_back(pDino);
	//}
}

void CDino::Dino_UI()
{	
	//���̹� �� ���� �̸� ����
	if (m_bIsTamed && !m_iNameFontID && !m_bDying)
	{
		//wstring wstrPrintFont = m_wstrDinoSpeciesKor;
		_vec3 vAddPos = DINONAMEADDWORLDPOS;
		m_iNameFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(m_pTransformCom->Get_Pos(), m_wstrDinoSpeciesKor, CWorldFont_UI::World_Font_Translucent_Green, CWorldFont_UI::World_FontType_Follow, vAddPos, -1000.f);
	}

	CGameObject* pPlayer = Engine::Get_GameObject(L"Layer_Player", 0);
	if (pPlayer == nullptr)
		return;

	_vec3 vDTPLength;
	vDTPLength = (*m_pTransformCom->Get_Pos() - *static_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos());
	if (50.f < D3DXVec3Length(&vDTPLength) || m_bIsRide || m_bDying)
	{
		static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Remove_WorldFont_UI(m_iNameFontID);
		m_iNameFontID = 0;
	}
}

_bool CDino::Dino_Baby_Parenting(const _double & _dTimeDelta)
{
	// �Ʊ��� ��� �� ������ ź��.
	if (!m_bBaby)
		return true;

	// ���� �̸� ������ ���� ���� ����
	if (!m_bNaming)
	{
		// ���⼭ ������� �����޾Ƽ� �״� �� ó��
		m_wstrBabyMsg = L"[E] : " + m_wstrDinoSpeciesKor + L"���� ����� ���� ��ŵ�ϴ�.";
		return false;
	}

	if (m_eUnitState == STATE_HAPPY)
		return false;

	m_bAttackMode = false;

	m_fGrowthTime += (_float)(_dTimeDelta);
	
	// ����⺰ ���� ������ ������ �ڽ� Ŭ�������� ���� ó��
	Set_Growth_Dino_Data();

	if (!m_bTakeCare)
	{
		m_fTakeCareTime -= (_float)_dTimeDelta;

		if (m_fTakeCareTime <= 0.f)
		{
			m_bTakeCare = true;
			m_fTakeCareTime = 15.f;

			m_eTakeCare = (TAKE_CARE)(rand() % 3);

			m_wstrBabyMsg = L"[E] : " + m_wstrDinoSpeciesKor + L"��(��) ";
			switch (m_eTakeCare)
			{
			case CDino::FOOD:
				m_wstrBabyMsg += L"������ �԰� �;��մϴ�. 0���� �ְ� �Կ��ּ���";
				break;
			case CDino::HUG:
				m_wstrBabyMsg += L"��Ű� �����ϱ⸦ ���մϴ�.";
				break;
			case CDino::WALK:
				m_wstrBabyMsg = m_wstrDinoSpeciesKor + L"��(��) ��Ű� ��å�ϱ⸦ ���մϴ�.";
				m_fWalkDis = 0.f;
				break;
			}
		}
		else
		{
			_int iTime = (_int)m_fTakeCareTime;

			wstring wstrTime = L"0";
			if (iTime < 10)
				wstrTime += to_wstring(iTime);
			else
				wstrTime = to_wstring(iTime);

			m_wstrBabyMsg = L"�������� �ʿ� (00:00:" + wstrTime + L")";
		}
	}	
	else
	{
		if (m_eTakeCare == WALK && m_fWalkDis >= 20.f)
			Set_TakeCare(true);
	}

	return true;
}

_bool CDino::Dino_Action_By_Mode(const _double & _dTimeDelta)
{
	// �����ϱⰡ ���������� 
	if (!m_bResistnace)
	{
		m_bAttackMode = false;
		m_pTartgetTransform = nullptr;
	}

	// ¦���� ���
	if (m_bMatingMode)
	{
		// �� ���溰�̶� �ڽ� Ŭ�������� ó��
		Dino_Mating(_dTimeDelta);
	}

	if (m_bPregnant)
	{
		m_fPregnantTime += (_float)_dTimeDelta;
		_int iPer = (_int)(m_fPregnantTime / PREGTIME);

		if (iPer >= 100)
		{
			m_fPregnantTime = 0.f;
			m_bPregnant = false;
			m_eUnitState = STATE_EGG;
			return false;
		}
	}

	if (m_bPostpartumCare)
	{
		m_fPostpartumCareTime += (_float)_dTimeDelta;
		_int iPer = (_int)(m_fPostpartumCareTime / PREGTIME);

		if (iPer >= 100)
		{
			m_fPostpartumCareTime = 0.f;
			m_bPostpartumCare = false;
		}
	}

	return true;
}

void CDino::Set_DinoSpeciesKor(wstring _wstrDinoName, _bool* bNaming)
{
	if (bNaming)
		*bNaming = true;

	m_wstrDinoSpeciesKor = L"";
	static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Remove_WorldFont_UI(m_iNameFontID);
	m_iNameFontID = 0;
	m_wstrDinoSpeciesKor = _wstrDinoName;
	wstring wstrPrintFont = m_wstrDinoSpeciesKor;
	_vec3 vAddPos = DINONAMEADDWORLDPOS;
	m_iNameFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(m_pTransformCom->Get_Pos(), wstrPrintFont, CWorldFont_UI::World_Font_Translucent_Green, CWorldFont_UI::World_FontType_Follow, vAddPos, -1000.f);
}

void CDino::Set_PeaceTaming(_bool bPeaceTaming)
{
	m_bPeaceTaming = bPeaceTaming;

	if (m_bPeaceTaming)
	{
		m_tStat.fTamingRate += 2.f;

		if (m_tStat.fTamingRate >= m_tStat.fMaxTamingRate)
		{
			Set_IsTamed();
			m_eUnitState = STATE_TORPID_TAMED;
			m_bAttackMode = false;
			static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Green_TamingSuccess);
			static_cast<CTamingName_UI*>(Engine::Get_GameObject(L"Taming_Name_UI", 0))->Making_a_Name(this, m_wstrDinoSpeciesKor);
		}
	}
}

void CDino::Open_Ring_Command(CUnit * pUnit)
{
	if (m_wstrRing == L"")
		m_wstrRing = L"Main";

	static_cast<CRingCommand_UI*>(Engine::Get_GameObject(L"RingCommand_UI"))->Open_RingCommand(m_mapRingCommand[m_wstrRing], pUnit);
}

void CDino::Set_Ring_Command(_int _iSelectCommand)
{
	auto& iter_find = m_mapRingCommand.find(m_wstrRing);
	
	if (iter_find == m_mapRingCommand.end())
		return;

	if (iter_find->second[_iSelectCommand].bSubRing)
	{
		m_wstrRing = iter_find->second[_iSelectCommand].wstrRingKey;
		Open_Ring_Command(this);
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_FixMouseCursor(false);
		return;
	}

	_int index = _iSelectCommand;
	if (m_wstrRing == L"Main")
	{
		RINGMAIN eIndex = (RINGMAIN)index;
		switch (eIndex)
		{
		case CDino::RINGMAIN_INVEN:
			CGameObject* pGameObject = Get_GameObject(L"Layer_Player");
			if (pGameObject != nullptr)
				static_cast<CPlayer*>(pGameObject)->Open_Inventory(this);
			break;
		}
	}
	else if (m_wstrRing == L"Moving")
	{
		RINGMOVING eIndex = (RINGMOVING)index;
		wstring strDis = L"���� ��� �Ÿ� ��ġ \n ( ���� : ";
		switch (eIndex)
		{
		case CDino::RINGMOVING_FOLLOW:
			Setting_Ring(&m_bFollow, m_bFollow ? L"������" : L"���߱�", &iter_find->second[_iSelectCommand]);
			break;
		case CDino::RINGMOVING_DISTANCE:
			m_eDistance = (DISTANCE)((_int)m_eDistance + 1);
			if (m_eDistance == DISTANCE_END)
				m_eDistance = DISTANCE_CLOSE;

			if (m_eDistance == DISTANCE_CLOSE)
				strDis += L" ������ )";
			if (m_eDistance == DISTANCE_MIDDLE)
				strDis += L" �߰� )";
			if (m_eDistance == DISTANCE_FAR)
				strDis += L" �ָ� )";

			Setting_Ring(nullptr, strDis, &iter_find->second[_iSelectCommand]);
			break;
		case CDino::RINGMOVING_MATING:
			Setting_Ring(&m_bMatingMode, m_bMatingMode ? L"¦���� ���" : L"¦���� ��� ����", &iter_find->second[_iSelectCommand]);
			break;
		case CDino::RINGMOVING_GROUP_WHITLE:
			Setting_Ring(&m_bGroupWhitle, m_bGroupWhitle ? L"�׷� ���Ķ� ��� ����" : L"�׷� ���Ķ� ��� ����", &iter_find->second[_iSelectCommand]);
			break;
		}
	}
	else if (m_wstrRing == L"Setting")
	{
		RINGSETTING eIndex = (RINGSETTING)index;
		switch (eIndex)
		{
		case CDino::SETTING_1:
			break;
		case CDino::SETTING_2:
			break;
		case CDino::SETTING_3:
			break;
		}
	}

	m_wstrRing = L"Main";
}

void CDino::Delete_UnitList(CGameObject * pObject)
{
	for (auto& iter = m_ListTargetUnit.begin(); iter != m_ListTargetUnit.end();)
	{
		if ((*iter) == pObject)
			iter = m_ListTargetUnit.erase(iter);
		else
			iter++;
	}
}

void CDino::Setting_Ring(_bool * bBool, wstring  wstrChangeText, CRingCommand_UI::STRCD* pStrcd)
{
	if(bBool)
		*bBool = !(*bBool);

	pStrcd->wstrCommand_Text = wstrChangeText;
}
