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

	/// 테스트용
	m_tStat.fMaxTamingRate = 5.f;	// 최대 조련수치
	m_tStat.fTamingRate = 0.f;		// 조련수치
	
	// 공룡 색상
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
	Teemp.wstrCommand_Text = L"인벤토리";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"[행동명령]";
	Teemp.bSubRing = true;
	Teemp.wstrRingKey = L"Moving";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"[설정]";
	Teemp.bSubRing = true;
	Teemp.wstrRingKey = L"Setting";
	vecRing.emplace_back(Teemp);

	m_mapRingCommand.emplace(L"Main", vecRing);
	vecRing.clear();

	Teemp.bSubRing = false;
	Teemp.wstrCommand_Text = L"따르기";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"따라갈 대상 거리 수치 \n ( 현재 : 중간 )";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"짝짓기 사용";
	vecRing.emplace_back(Teemp);
	Teemp.wstrCommand_Text = L"그룹 휘파람 명령 무시";
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

	if (!(m_dwFlagSkip & SKIP_UPDATE))	//스킵이 켜져있으면 중력 적용 이외의 다른 업데이트들을 모두 스킵
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

		// 버프 체크
		Check_Buff(_dTimeDelta);

		/// 기절 후 상태 확인
		if (DINO_TAMED == Check_Torpid(_dTimeDelta))
			return DINO_TAMED;

		// 볼라 체크
		Check_IsHolding(_dTimeDelta);

		/// 기절 후 상태 확인
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

	//// 피직스 중력 적용 ////////////////////////////////////////////////////////////////////////
	_vec3 vPos = { 0.f,0.f,0.f };
	_float fAddGravity = 1.f;
	if (m_eUnitState == STATE_SPRINT && !m_bJump)
		fAddGravity = 5.f;

	//매달려있으면 리턴
	if (m_bHanging)
		return NO_EVENT;

	//육지공룡이거나 익룡인데 날지않을경우는 중력
	if (m_bLandDino || (!m_bLandDino && !m_bJump))
	{
		vPos.y = (_float)(GRAVITY * _dTimeDelta * fAddGravity);
		m_pCtrl->move(vPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());
	}

	//육지공룡이 쩜프할때
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

	// 플레이어 탑승중일 때 스테이트 변경
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

	//// 애니메이션 세팅
	m_pMeshCom->Set_Animation(m_iAnimation);				// 전체 애니메이션

	// Upper가 IDLE이면 BaseAni와 동기화하기
	if (STATE_IDLE != m_eUpperDinoState)
		m_pMeshCom->Set_UpperAnimation(m_iUpperAnimation);	// 부분 애니메이션	
	else
		m_pMeshCom->Set_UpperAnimation(m_iAnimation, 1.0);

	m_dAnimationTime = _dTimeDelta;

#pragma region CULLING
	// 컬링 시작 //
	//컬라이더 좌표는 로컬이므로 월드로 변환해준다
	_vec3 vSpherePos = m_pSphereColliderCom->Get_InteractPos();
	D3DXVec3TransformCoord(&vSpherePos, &vSpherePos, &m_pTransformCom->Get_WorldMatrix());
	_float fSphereRadius = m_pSphereColliderCom->Get_InteractRadius() * Get_Scale().x;

	//그림자 렌더그룹 추가 : 경계구체가 빛 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	//if (Engine::Check_Sphere(vSpherePos, fSphereRadius, CULL_LIGHT))

	// 그림자맵 가장자리쪽에 렌더될 경우 찢어지는 현상(Clamp) 때문에
	// 물체가 일정 거리에 들어올 때부터 그림자맵에 그려준다.
	_vec3	vecPlayerPos = *static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos();
	_float	fLength = D3DXVec3Length(&(*m_pTransformCom->Get_Pos() - vecPlayerPos));

	if (45.f > fLength)
	{
		Set_ShadowMap(Engine::Get_ShadowLight().vLightPos, Engine::Get_ShadowLight().vLightAt);	//그림자맵 위치 잡아줌
		Engine::Add_RenderGroup(RENDER_SHADOWMAP, this);
	}

	//렌더그룹 추가 : Interaction 컬라이더가 절두체 안에 들어갈 경우에만 렌더그룹에 추가한다
	if (m_pSphereColliderCom)
	{
		//컬라이더 반경은 공룡 스케일값으로 나눠진 상태이므로 스케일을 도로 곱해 월드로 변환해준다
		if (Engine::Check_Sphere(vSpherePos, fSphereRadius))
			Engine::Add_RenderGroup(RENDER_NONALPHA, this);
		else
			//컬링될 경우 렌더는 생략하지만 애니메이션만 따로 돌려준다
		{
			// 애니메이션 플레이
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

	//공중탈것만 적용됨
	if (m_eUnitState == STATE_RIDE || m_eUnitState == STATE_LANDING)
	{
		m_tStat.fSpeed = 0.f;
		return false;
	}

	/// 앞뒤 이동
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
	// 애니메이션 플레이
	_double dPlayerTime = 1.0 + m_dAnimationSpeed;
	if (!Engine::Get_ShadowOnOff())
		dPlayerTime = 0.5 + m_dAnimationSpeed / 2.0;

	Dino_Play_Animation(dPlayerTime);

	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Update_Component(m_dAnimationTime);

	//// 버프 체크
	//Check_Buff(m_dAnimationTime);

	// 쉐이더 컴객체 생성
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

	// ShadowMap Pass 0 기본, 1 디졸브, 2 알파테스트
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

	// Shadow Pass 0 기본, 1 디졸브, 2 알파테스트
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
		// 볼라 맞았을 때
		static_cast<CBullet*>(_pAttacker)->TobeBolaWrapped(m_pTransformCom->Get_WorldMatrix(), m_tStat.fSize);

		m_eUnitState = STATE_HOLDING;
		m_bAttackMode = true;
	}
	else if (m_bDying)
	{
		if (0 >= m_pInvenCom->Get_pInvenItem()->size())
		{
			//이름 지우기!!!!!!!!!
			static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Remove_WorldFont_UI(m_iNameFontID);
			Set_Dead();
		}
	}
	else if(STATE_TORPID		!=	m_eUnitState	&& 
			STATE_TORPID_IDLE	!=	m_eUnitState	&&
			STATE_TORPID_EAT	!=	m_eUnitState)
	{
		// 기절치 증가 처리
		_float fTorpid = (pAttacker->Get_Stat(STAT_TORPIDRATE) + fAddTorpid) * pAttacker->Get_Stat(STAT_ATTACK);

		m_tStat.fParalysis += fTorpid;

		if (m_tStat.fParalysis > m_tStat.fMaxParalysis && !m_bIsTorpid)
		{
			// 기절 시작
			m_bIsTorpid = true;

			m_tStat.fParalysis = m_tStat.fMaxParalysis;
			m_eUnitState = STATE_TORPID;
			m_bIsStatUpdate = true;			// 배고픔 달게 하기
			m_bAttackMode = false;			// 공격중 해제
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
	// 이미 테이밍 상태면 리턴 false
	if (m_bIsTamed)
		return false;
		
	// 레이어 변경
	if (FAILED(Engine::Swap_GameObjectInLayer(L"Layer_Dino", L"Layer_Dino_Tamed", this)))
		return false;

	// 수치 변경
	m_tStat.fParalysis = 0.f;		// 기절수치 초기화
	m_tStat.fAtk = m_fTamingAttack;

	//임시로 테이밍됐을 때 피 풀로
	m_tStat.fHp = m_fTamingHp;
	m_tStat.fMaxHp = m_fTamingHp;
	m_tStat.fHp = m_tStat.fMaxHp;	// 체력 만땅

	m_bIsTorpid = false;
	m_bIsTamed = true;
	m_bIsEquipSaddle = true;
	m_bAttackMode = false;

	// 안장 그리기
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
	// 충돌테스트
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

	// 상수테이블 세팅
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

			//Set_Scale(tInfo.vScale);	//우선은 공룡별 스케일로
			Set_Rot(tInfo.vRot.x, tInfo.vRot.y, tInfo.vRot.z);
			Set_Pos(tInfo.vPos.x, tInfo.vPos.y + 1.f, tInfo.vPos.z);

			Add_ExpToLevel(tInfo.dwLevel);	//현재 빈 함수

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

	// MOVE 일 때는 m_vMovePos = 랜덤 좌표
	// SPRINT 일 때는 m_vMovePos = 맞았을 당시 좌표
	m_vMovePos.y = 0.f;
	vPos.y = 0.f;

	_vec3 vDir = m_vMovePos - vPos;
	_float fDis = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vCross;
	_float fAngle = m_pCalculatorCom->Get_Angle(&vPos, &vLook, &m_vMovePos, &vCross);
	m_eUnitMoveDir = UNIT_DIR_UP;

	// 일단 파라 맞았을 때 앞으로 달려가는 거 테스트용
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
		// 원하는 위치에 도착
		if (m_fAttackRange >= fDis)
		{
			//공격모드가 아니면 각도 상관없이 정지
			if (!m_bAttackMode)
			{
				m_eUnitState = STATE_IDLE;
				return;
			}

			//공격모드일 때는 각도까지 판단 대상이 각도에 없으면 회전까지해준다.
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

		// 기본 이동 시에도 회전한다.
		if (vCross.y > 0.f)
			m_pTransformCom->Set_AccRotate(ROT_Y, fAngle * (_float)_dTimeDelta * -m_fRotationSpeed);
		else
			m_pTransformCom->Set_AccRotate(ROT_Y, fAngle * (_float)_dTimeDelta * m_fRotationSpeed);
	
	}

	PxPos += vDir * _float(m_tStat.fSpeed * _dTimeDelta);

	const PxU32 flags = m_pCtrl->move(PxPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters());
	// 움직이다가 벽에 부딪혔을 때 다른 곳으로 이동
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
	
	// 아기 육성 산책 거리 올려주기
	if (m_bBaby && m_eTakeCare == WALK && m_bTakeCare)
		m_fWalkDis += 0.1f;

	if (m_fAttackRange >= fDis)
	{
		//공격모드가 아니면 각도 상관없이 정지
		if (!m_bAttackMode)
		{
			m_eUnitState = STATE_IDLE;
			return;
		}

		//공격모드일 때는 각도까지 판단 대상이 각도에 없으면 회전까지해준다.
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

// 인자로 선공 범위 설정
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

	Setting_Ring(nullptr, m_bFollow ? L"멈추기" : L"따르기", &iter_find->second[RINGMOVING_FOLLOW]);
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
			// 바닥과 충돌중일 때
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
	case CUnit::STATE_TORPID:						// 기절 시작
		if (m_pMeshCom->Is_AnimationsetEnd(0.2))
			m_eUnitState = STATE_TORPID_IDLE;
		break;
	case CUnit::STATE_TORPID_IDLE:
		break;
	case CUnit::STATE_TORPID_TAMED:					// 기절에서 테이밍완료
		if (m_pMeshCom->Is_AnimationsetEnd(0.2))
		{
			m_eUnitState = STATE_IDLE;
			m_bIsTorpid = false;
		}
		break;
	case CUnit::STATE_TORPID_WILD:					// 기절에서 야생상태로 일어나기
		if (m_pMeshCom->Is_AnimationsetEnd(0.2))
		{
			m_eUnitState = STATE_SPRINT;
			m_bIsTorpid = false;
		}
		break;
	case CUnit::STATE_TORPID_EAT:					// 기절중 음식 먹기
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
			// 알
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

	// 첫 프레임에 바로 들어가지 않게 Pre 스테이트를 사용
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
			// 공격 종료
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
			// 공격 종료
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
			// 볼라 해제
			m_dHoldingTime = 0;
			m_bIsHolding = false;

			m_eUnitState = STATE_IDLE;
		}
		else if (m_bIsTorpid)
		{
			// 홀딩중 기절
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

		/// 기절중인 상태
		// 기절수치 조금씩 깍기
		m_tStat.fParalysis -= 0.2f * (_float)_dTimeDelta;

		if (0.f >= m_tStat.fParalysis)
		{
			// 기절에서 깨어나기 (테이밍 실패)
			m_tStat.fParalysis = 0.f;
			m_eUnitState = STATE_TORPID_WILD;

			return NO_EVENT;
		}

		// 테이밍 먹이주기
		return Taming_Eat(_dTimeDelta);
	}

	return NO_EVENT;
}

_int CDino::Taming_Eat(const _double & _dTimeDelta)
{
	m_dTamingEatTime += _dTimeDelta;

	// 배고픔이 맥스가 아닐 때 먹이 먹기
	if (3 < m_dTamingEatTime && m_tStat.fMaxHunger - 1.f > m_tStat.fHunger)
	{
		if (0 < m_pShow_InvenCom->Now_Have_ItemAmount(m_eTamingFood))
		{
			m_dTamingEatTime = 0.0;

			// 테이밍 먹이 먹기
			m_pShow_InvenCom->Remove_Item(m_eTamingFood, 1);
			static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Match_ItemTex_Info();
			
			m_eUnitState = STATE_TORPID_EAT;

			// 조련수치 올리기 (조련수치를 임시로 fParalysis 사용)
			m_tStat.fTamingRate += 1.f;

			// 조련수치가 일정이상 올라가면 테이밍 완료
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

	// 짝짓기 모드, 저항하지 않는 모드 등
	if(!Dino_Action_By_Mode(_dTimeDelta))
		return;

	if (m_bAttackMode && m_pTagetObject)
	{
		// 죽거나 기절 중이면 공격x
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

		// 플레이어가 공룡타고 있으면 공룡의 좌표로 따라옴
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

	// 비행애들은 따로 관리
	if (!m_bLandDino && m_bFollow)
	{
		Dino_Move_Fly(_dTimeDelta);
		return;
	}

	if (m_eUnitState != STATE_IDLE)
	{
		Dino_Move_Tamed(_dTimeDelta);

		// 걷거나 뛸때 일정 시간마다 흙먼지 생성
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

	// 짐벌락 방지
	fabsf(vDinoLookY.y);
	if (0.f > vDinoLookY.z)
		vDinoLookY *= -1.f;


	_float fRatioX = D3DXVec3Dot(&vDinoLookX, &_vec3(0.f, 0.f, 1.f));		// 허리 각도
	//_float fRatioY = D3DXVec3Dot(&vDinoLookY, &_vec3(0.f, 0.f, 1.f));		// 목 각도

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

		//ChaseMode는 처음 공격 받았을 때 판정용
		//bChaseNotDetected는 n초 뒤에는 거리 판별로 공격 대상 찾는 용  
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

		// 공격할 대상이 없을 경우
		if (m_ListTargetUnit.empty())
		{
			m_bAttackMode = false;
			m_pTartgetTransform = nullptr;

			//어그로 끌렸던 포스가 있으면 그곳으로 이동
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

		//바뀐스테이트 한번 적용 시켜주기위해서 리턴
		return;
	}

	if (m_eUnitState == STATE_MOVE || m_eUnitState == STATE_SPRINT)
	{
		m_dIdleTime = 0.f;
		if (m_bLandDino || (!m_bLandDino && !m_bJump))
		{
			Dino_Move(_dTimeDelta);

			// 걷거나 뛸때 일정 시간마다 흙먼지 생성
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
	//플레이어 리스트에 추가, 공룡에게 공격 지시했을 때 플레이어도 어그로에 포함시켜야해서
	CGameObject* pPlayer = Engine::Get_GameObject(L"Layer_Player");
	_vec3 vPos = *m_pTransformCom->Get_Pos();

	// 플레이어 먼저 검사
	if (pPlayer)
	{
		_vec3 vPlayerPos = *static_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos();
		_float fTargetDis = D3DXVec3Length(&(vPos - vPlayerPos));

		//공격 범위내에 플레이어 있으면 추가하는데 이미 리스트에 있으면 패스
		if (fTargetDis <= fDetectRange && m_ListTargetUnit.size() < 2)
		{
			auto& iter_find = find(m_ListTargetUnit.begin(), m_ListTargetUnit.end(), pPlayer);

			if (iter_find == m_ListTargetUnit.end())
			{
				m_ListTargetUnit.emplace_back(pPlayer);

				// 추가했을 때 공룡이 FRONT면 뒤집기
				// 플레이어가 공룡을 타고있으면 공룡이 front일 수 있음
				if (m_ListTargetUnit.front() != pPlayer)
					m_ListTargetUnit.reverse();
			}

			// 플레이어가 공격받을 때 공룡에 타면 리스트에 안들어가서 추가
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
	//처음 n초는 무조건 쫓고 그 이후에는 리스트돌면서 삭제를 시켜야한다..
	else if (m_ListTargetUnit.size() == 1 && static_cast<CUnit*>(m_ListTargetUnit.front())->Get_UnitType() == CUnit::UNIT_PLAYER && !m_bChaseNotDetected && !m_bChaseMode)
	{
		m_pTartgetTransform = static_cast<CTransform*>(m_ListTargetUnit.front()->Get_Component(L"Com_Transform", ID_DYNAMIC));
		m_bChaseMode = true;
		m_dChaseTime = 0.f;
		m_bChaseNotDetected = true;

		// 만약 플레이어가 공룡을 타고 있으면 걔도 공격 대상에 포함
		//CDino* pRideDino = static_cast<CPlayer*>(m_ListTargetUnit.front())->Get_RideDino();
		//if (pRideDino)
		//	m_ListTargetUnit.emplace_back(pRideDino);

		return;
	}

	//먼저 리스트 돌면서 죽은애들 삭제, 거리먼애들 삭제
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

	// 일단 범위안에 플레이어와 길들여진 공룡만 체크
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
	//테이밍 된 공룡 이름 띄우기
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
	// 아기일 경우 이 루프를 탄다.
	if (!m_bBaby)
		return true;

	// 아직 이름 각인을 하지 않은 상태
	if (!m_bNaming)
	{
		// 여기서 배고픔이 빨리달아서 죽는 거 처리
		m_wstrBabyMsg = L"[E] : " + m_wstrDinoSpeciesKor + L"에게 당신을 각인 시킵니다.";
		return false;
	}

	if (m_eUnitState == STATE_HAPPY)
		return false;

	m_bAttackMode = false;

	m_fGrowthTime += (_float)(_dTimeDelta);
	
	// 성장기별 공룡 데이터 정리는 자식 클래스에서 개별 처리
	Set_Growth_Dino_Data();

	if (!m_bTakeCare)
	{
		m_fTakeCareTime -= (_float)_dTimeDelta;

		if (m_fTakeCareTime <= 0.f)
		{
			m_bTakeCare = true;
			m_fTakeCareTime = 15.f;

			m_eTakeCare = (TAKE_CARE)(rand() % 3);

			m_wstrBabyMsg = L"[E] : " + m_wstrDinoSpeciesKor + L"은(는) ";
			switch (m_eTakeCare)
			{
			case CDino::FOOD:
				m_wstrBabyMsg += L"간식을 먹고 싶어합니다. 0번에 넣고 먹여주세요";
				break;
			case CDino::HUG:
				m_wstrBabyMsg += L"당신과 포옹하기를 원합니다.";
				break;
			case CDino::WALK:
				m_wstrBabyMsg = m_wstrDinoSpeciesKor + L"은(는) 당신과 산책하기를 원합니다.";
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

			m_wstrBabyMsg = L"보살핌이 필요 (00:00:" + wstrTime + L")";
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
	// 저항하기가 꺼져있으면 
	if (!m_bResistnace)
	{
		m_bAttackMode = false;
		m_pTartgetTransform = nullptr;
	}

	// 짝짓기 모드
	if (m_bMatingMode)
	{
		// 각 공룡별이라 자식 클래스에서 처리
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
		wstring strDis = L"따라갈 대상 거리 수치 \n ( 현재 : ";
		switch (eIndex)
		{
		case CDino::RINGMOVING_FOLLOW:
			Setting_Ring(&m_bFollow, m_bFollow ? L"따르기" : L"멈추기", &iter_find->second[_iSelectCommand]);
			break;
		case CDino::RINGMOVING_DISTANCE:
			m_eDistance = (DISTANCE)((_int)m_eDistance + 1);
			if (m_eDistance == DISTANCE_END)
				m_eDistance = DISTANCE_CLOSE;

			if (m_eDistance == DISTANCE_CLOSE)
				strDis += L" 가까이 )";
			if (m_eDistance == DISTANCE_MIDDLE)
				strDis += L" 중간 )";
			if (m_eDistance == DISTANCE_FAR)
				strDis += L" 멀리 )";

			Setting_Ring(nullptr, strDis, &iter_find->second[_iSelectCommand]);
			break;
		case CDino::RINGMOVING_MATING:
			Setting_Ring(&m_bMatingMode, m_bMatingMode ? L"짝짓기 사용" : L"짝짓기 사용 안함", &iter_find->second[_iSelectCommand]);
			break;
		case CDino::RINGMOVING_GROUP_WHITLE:
			Setting_Ring(&m_bGroupWhitle, m_bGroupWhitle ? L"그룹 휘파람 명령 따름" : L"그룹 휘파람 명령 무시", &iter_find->second[_iSelectCommand]);
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
