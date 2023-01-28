#include "stdafx.h"
#include "..\Headers\Player.h"

#include "Value.h"

#include "Export_Function.h"
#include "Sound_Mgr.h"

#include "Dino.h"

#include "Camera.h"

#include "Weapon.h"
#include "Bullet.h"
#include "Structure.h"
#include "Campfire.h"
#include "SleepingBag.h"

#include "Inven.h"
#include "LR_Inventory.h"
#include "QuickSlot_UI.h"
#include "Right_Inventory.h"
#include "Stat_UI.h"
#include "MonsterStat_UI.h"
#include "PlayerHit_UI.h"
#include "MiniMap_UI.h"
#include "GuideFont_UI.h"
#include "InfoView_UI.h"

#include "PhysX_Mgr.h"

#include "SphereCollider.h"
#include "CollisionFunc.h"

#include "Harvestable.h"

#include "WorldFont_UI.h"

#include "SummaryStat_UI_Monster.h"
#include "SummaryStat_UI.h"

#include "CameraMrg.h"
#include "TamingName_UI.h"
#include "Dino_Egg.h"
#include "Dodo_Ball.h"
#include "Dino_Ankylo.h"

USING(Engine)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pDevice)
	: CUnit(_pDevice)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CUnit(rhs)
{
}

HRESULT CPlayer::Ready_GameObject_Prototype()
{
	CUnit::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CPlayer::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CUnit::Ready_GameObject(_pArg);
	///////////////////////////////////////// 테스트 용
	m_pInvenCom->Set_InvenUserUnitTypeID(InvenUserUnitTypeID_Player);
	//플레이어는 고정, 몬스터, 상자, 등 오른쪽 인벤에 띄우려면 키고싶은 unit 객체에서
	// static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", L"Right_Inventory"))->Set_InvenOwner(this);
	//이걸 불러주면 됨

	//인벤에 아이템 넣기
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_AmarBerry, 100));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Meat, 40));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Wood, 100));

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Arrow, 100));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_TranqArrow, 100));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Club, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneAx, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StonePick, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Spear, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Bolas, 10));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Bow, 1));

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Campfire, 1));			// 모닥불
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_SleepingBag, 1));			// 1회용 침낭
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_MortarPestle, 1));		// 절구와공이
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Forge, 1));				// 화로
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Smithy, 1));				// 대장간
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchBed, 1));			// 짚 침대

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_WallTorch, 1));			//
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_WaterWell, 1));			//
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Barricade, 1));			//
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_CompostBin, 1));			//
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_CropPlot, 1));			//
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_FeedingTrough, 1));		//
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Fireplace, 1));			//
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_S_Storage, 1));			//
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_L_Storage, 1));			//
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_CookingCampFire, 1));		//

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchFloor, 1));			// 짚 토대
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchWall, 1));			// 짚 벽
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchRoof, 1));			// 짚 천장
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchDoorFrame, 1));		// 짚 문 프레임
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchDoor, 1));			// 짚 문

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneFloor, 1));			// 짚 토대
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneWall, 1));			// 짚 벽
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneRoof, 1));			// 짚 천장
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneDoorFrame, 1));		// 짚 문 프레임
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneDoor, 1));			// 짚 문


	// 방어구
	//m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Leather_Hat, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Leather_TOP, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Leather_Legs, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Leather_Gloves, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Leather_Boots, 1));

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Metal_Hat, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Metal_TOP, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Metal_Legs, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Metal_Gloves, 1));
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Metal_Boots, 1));

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneGateFrame, 1));			// 석재 게이트 프레임
	/////////////////////////////////////////

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vPlayerPos = _vec3(20.f, 14.6f, -15.f);

	// 1인칭
	m_pTransformFPVCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformFPVCom->Set_Pos(m_vPlayerPos);
	m_pTransformFPVCom->Update_Component(0.0);

	// 3인칭
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(m_vPlayerPos);
	m_pTransformCom->Update_Component(0.0);

	// 서브셋만큼 bool 값 동적할당
	m_bIsFPVSubsetRender = new _bool[m_pMeshFPVCom->Get_MeshList()->size()];
	ZeroMemory(m_bIsFPVSubsetRender, sizeof(_bool) * m_pMeshFPVCom->Get_MeshList()->size());
	m_bIsFPVSubsetRender[0] = true;		// 몸체만 그리기

	// 3인칭
	m_bIsTPVSubsetRender = new _bool[m_pMeshCom->Get_MeshList()->size()];
	ZeroMemory(m_bIsTPVSubsetRender, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsTPVSubsetRender[0] = true;		// 몸

	m_bIsTPVSubsetRender[1] = true;		// 언더웨어
	//m_bIsTPVSubsetRender[2] = false;		// 가죽 신발
	//m_bIsTPVSubsetRender[3] = false;		// 가죽 장갑
	//m_bIsTPVSubsetRender[4] = false;		// 가죽 바지
	//m_bIsTPVSubsetRender[5] = false;		// 가죽 상의
	//m_bIsTPVSubsetRender[6] = false;		// 방탄 신발
	//m_bIsTPVSubsetRender[7] = false;		// 방탄 장갑
	//m_bIsTPVSubsetRender[8] = false;		// 방탄 바지
	//m_bIsTPVSubsetRender[9] = false;		// 방탄 상의

	/// 플레이어 스탯 세팅
	m_bIsStatUpdate = true;

	m_tStat.fMaxHp = 100;
	m_tStat.fHp = m_tStat.fMaxHp;
	m_tStat.fMaxStamina = 100;
	m_tStat.fStamina = m_tStat.fMaxStamina;
	m_tStat.fAtk = 5;
	m_tStat.fKnockBack = 0;
	m_tStat.fMaxWeight = 1000;
	m_tStat.fWeight = 0;
	m_tStat.fMaxHunger = 100;
	m_tStat.fHunger = 30.f;
	m_tStat.fMaxThirst = 100;
	m_tStat.fThirst = m_tStat.fMaxThirst;
	m_tStat.fSpeed = 3;
	m_tStat.fParalysis = 0;
	m_tStat.fEngram = 1;
	m_tStat.fEndure = 0;
	m_tStat.fStatPoint = 0;
	m_tStat.fTorpidRate = 0.5f;
	m_tStat.fEXP = 0.f;
	m_tStat.fLevel = 1.f;
	m_tStat.fSize = 1.f;

	/// 플레이어 상태 세팅
	/// 첫게임 시작시 카메라 연출
	m_bIsFPVCam = true;
	m_eUnitState = STATE_IDLE;
	m_eUpperPlayerState = STATE_IDLE;
	/////////
	// 상태
	//m_eUnitState = STATE_CAMERAWORK;
	//m_eUpperPlayerState = STATE_CAMERAWORK;

	// 애니메이션
	// 게임시작시 1인칭 모드

	//m_bIsFPVCam = true;
	//m_bIsLockCamera = true;
	//m_eFPVAnimation = P_A_FPV_BASIC_CAMERA_SPWANINTRO;
	//m_eFPVUpperAnimation = P_A_FPV_BASIC_CAMERA_SPWANINTRO;
	///////////

	m_eUpperAnimation = P_A_BASIC_IDLE;
	m_ePlayerPose = P_POSE_STAND;

	m_eUnitType = CUnit::UNIT_PLAYER;

	m_pCtrl = static_cast<PxCapsuleController*>(CPhysX_Mgr::GetInstance()->CreateCapsuleController(0.2f, 0.5f, Get_Pos()));
	if (nullptr == m_pCtrl)
		return E_FAIL;

	// 프로토빌드용 임시함수 ////////////////////////////////////////////////////////////////////////
	//Init_LevelUpTable();
	//////////////////////////////////////////////////////////////////////////

	//static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_CameraType(CCameraMrg::CAMERA_FPV);

	////////////////////////////////////사운드 매니저 플레이어 위치
	pSMgr->Set_PlayerPos(&m_vPlayerPos);
	/////////////////////////////////////////////////////////

	D3DXMatrixIdentity(&m_matLastWorldView);

	m_fCenterPos = 1.f;

	return S_OK;
}

_int CPlayer::Update_GameObject(const _double & _dTimeDelta)
{
	//m_pTransformFPVCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));

	//if (Engine::Get_DIKeyDown(DIK_UP))
	//{
	//	Equip_Armor(EQUIP_TOP, ITEMID_Metal_TOP);
	//	Equip_Armor(EQUIP_LEGS, ITEMID_Metal_Legs);
	//	Equip_Armor(EQUIP_GLOVES, ITEMID_Metal_Gloves);
	//	Equip_Armor(EQUIP_BOOTS, ITEMID_Metal_Boots);
	//}
	//else if (Engine::Get_DIKeyDown(DIK_DOWN))
	//{
	//	Unequip_Armor(EQUIP_TOP);
	//	Unequip_Armor(EQUIP_LEGS);
	//}


	// 치트코드 //////////////////////////////////////////////////////////////////////////
	if (Get_DIKeyDown(DIK_F5))
		m_bCheatMode = !m_bCheatMode;
	if (m_bCheatMode)
	{
		m_tStat.fStamina = m_tStat.fMaxStamina;
		if (m_tStat.fMaxHp >= m_tStat.fHp)
			m_tStat.fHp = m_tStat.fMaxHp;

		if (m_tStat.fMaxHunger >= m_tStat.fHunger)
			m_tStat.fHunger = m_tStat.fMaxHunger;
	}

	if (Get_DIKeyDown(DIK_F8))
		g_bInstancing = !g_bInstancing;	//인스턴싱 켜고 끄기
	//////////////////////////////////////////////////////////////////////////

	/// 플레이어 죽음, 부활 처리
	if (m_bDead)
	{
		Engine::Fade_Out(1.f);
		//ShowCursor(true);
		if (Engine::Get_DIKeyDown(DIK_SPACE))
		{
			if (0 < static_cast<CMiniMap_UI*>(Engine::Get_GameObject(L"MiniMap_UI", 0))->Get_BedCount())
				static_cast<CMiniMap_UI*>(Engine::Get_GameObject(L"MiniMap_UI", 0))->MiniMap_Move(true);
			else
				Player_Respawn(m_vPlayerPos = _vec3(20.f, 11.f, -15.f));
		}

		return NO_EVENT;
	}

	CUnit::Update_GameObject(_dTimeDelta);

	/// 플레이어 위치 가져오기
	m_pTransformCom->Get_Info(INFO_POS, &m_vPlayerPos);

	/// 전방에 가까운 오브젝트 레이어 확인
	//if (!m_bIsFPVCam)
	Check_PlayerLookLayer();

	/// 키입력
	if (!m_bIsRide)
	{
		/// 공룡안탐
		// 키입력
		if (STATE_DEAD != m_eUnitState)
			Key_Input(_dTimeDelta);

		/// 특정 애니메이션 앤드 체크
		Check_IsEndAnimation();
		Check_IsEndUpperAnimation(_dTimeDelta);


		// 플레이어 상태 체크
		Check_PlayerState(_dTimeDelta);

		// 공격
		Is_Attack();

		// 건축
		Is_Building();

		/// 포즈 및 무기에 맞게 플레이어 애니메이션 변경
		m_eAimation = PlayerAnimationByStance();					// 무기별 애니메이션
		m_eUpperAnimation = PlayerUpperAnimationByStance();			// 무기별 부분 애니메이션
		m_eFPVAnimation = PlayerFPVAnimationByStance();
		m_eFPVUpperAnimation = PlayerFPVUpperAnimationByStance();

		/// 플레이어 위치 세팅하기 (이동)
		PxExtendedVec3 vPos = m_pCtrl->getPosition();

		_float fFoot = m_bTakeOff ? 0.f : 1.3f;
		if (m_ePlayerPose == P_POSE_CROUCH)
		{
			m_pCtrl->setHeight(0.2f);
			m_pCtrl->resize(0.2f);
			fFoot = 0.5f/*1.f*/;
		}
		else
		{
			m_pCtrl->setHeight(1.5f);
			m_pCtrl->resize(1.5f);
		}

		m_pTransformCom->Set_Pos(_vec3((_float)vPos.x, (_float)vPos.y - fFoot, (_float)vPos.z));
		m_pTransformFPVCom->Set_Pos(_vec3((_float)vPos.x, (_float)vPos.y - fFoot + 1.5f, (_float)vPos.z));
	}
	else
	{
		// 인벤토리
		Key_Input_Inventory(_dTimeDelta);

		/// 공룡탔을 때 공룡 키입력
		IsRide_Key_Input(_dTimeDelta);
	}

	m_fCreateIntv += (_float)_dTimeDelta * 2.f;

	m_fTime = (_float)_dTimeDelta;

	// 플레이어 위치 기준으로 ShadowMap 광원 위치 잡기
	Set_ShadowLight();


	return NO_EVENT;
}

_int CPlayer::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	m_dAnimationTime = _dTimeDelta;

	if (!m_bDying && m_tStat.fHp <= 0.f)
	{
		m_bDying = true;
		m_tStat.fHp = 0.f;
		m_eUnitState = STATE_DEAD;

		if (m_bIsRide)
		{
			if (m_pDino)
			{
				_vec3 vPos;
				_mat matSaddle = m_pDino->Get_SaddleMat();
				memcpy(&vPos, &matSaddle.m[INFO_POS][0], sizeof(_vec3));
				m_pCtrl->setPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
				m_pTransformCom->Set_Pos(vPos);
				m_vPlayerPos = vPos;
				m_pTransformCom->Update_Component(0.f);

				m_pDino->Set_IsRide(false);
				Safe_Release(m_pDino);
				m_pDino = nullptr;
				m_bIsRide = false;

				//오른쪽 위에 탑승한 공룡 체력 등등 끄기
				for (_int i = 0; i < (_int)CSummaryStat_UI_Monster::SummaryStat_END; ++i)
					static_cast<CSummaryStat_UI_Monster*>(Engine::Get_GameObject(L"SummaryStat_UI", ((_int)CSummaryStat_UI::SummaryStat_END) * 2 + (i)))->Set_RideMonster(nullptr);
			}
		}
	}

	// 애니메이션 세팅
	if (m_bIsFPVCam)
	{
		//m_eFPVAnimation = PLAYER_ANIMATION_FPV(47);
		//m_pMeshFPVCom->Set_Animation(m_eFPVAnimation);

		if (STATE_IDLE == m_eUpperPlayerState)
			m_pMeshFPVCom->Set_Animation(m_eFPVAnimation);
		else
			m_pMeshFPVCom->Set_Animation(m_eFPVUpperAnimation);
	}
	else
	{
//#ifdef _DEBUG
		//if (Engine::Get_DIKeyDown(DIK_UP))
		//{
		//	++m_AniTest;
		//	if (P_A_END <= m_AniTest)
		//		m_AniTest = P_A_FISHING_PULLINLOOPCAST;
		//}
		//else if (Engine::Get_DIKeyDown(DIK_DOWN))
		//{
		//	--m_AniTest;
		//	if (0 > m_AniTest)
		//		m_AniTest = 0;
		//}
//#endif
		//m_pMeshCom->Set_Animation(m_AniTest);
		//m_pMeshCom->Set_UpperAnimation(m_AniTest);

		if (STATE_IDLE == m_eUnitState && (STATE_HARVEST == m_eUpperPlayerState))
			m_pMeshCom->Set_Animation(m_eUpperAnimation);
		else
			m_pMeshCom->Set_Animation(m_eAimation);				// 전체 애니메이션
		
		if (STATE_IDLE != m_eUpperPlayerState)
			m_pMeshCom->Set_UpperAnimation(m_eUpperAnimation);	// 부분 애니메이션
		else
			m_pMeshCom->Set_UpperAnimation(m_eAimation);
	}



	// 랜더그룹 추가
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	Engine::Add_RenderGroup(RENDER_SHADOWMAP, this);

	if (m_bIsFPVCam)
	{
		//if (m_bDead)
		//	m_pMeshFPVCom->Play_Animation_End(_dTimeDelta, nullptr);
		//else
		m_pMeshFPVCom->Play_Animation(_dTimeDelta);
	}
	else
	{
		if (m_bDead)
			m_pMeshCom->Play_Animation_End(_dTimeDelta, "Cnt_Spine_002_JNT_SKL");
		else
			m_pMeshCom->Play_BlendAnimation(_dTimeDelta * m_dAnimationSpeed, _dTimeDelta * m_dUpperAnimationSpeed, "Cnt_Spine_002_JNT_SKL");
	}

	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Update_Component(m_dAnimationTime);
	if (m_pFPVSphereColliderCom)
	{
		_vec3 vColPos = m_vPlayerPos;
		_vec3 vColLook;
		m_pTransformFPVCom->Get_Info(INFO_LOOK, &vColLook);
		D3DXVec3Normalize(&vColLook, &vColLook);
		vColPos -= vColLook * 1.0f;
		vColPos.y += 1.5f;

		m_pFPVSphereColliderCom->Set_Pos(vColPos);
		m_pFPVSphereColliderCom->Update_Component(m_dAnimationTime);
	}

	return NO_EVENT;
}

void CPlayer::Render_GameObject()
{
	// 상하체 애니메이션 동시에 돌리기
	//m_pMeshCom->Play_Animation(m_dAnimationTime, 1, "Cnt_Spine_002_JNT_SKL");

	////////////////////////////////////////////////////////////////////
	// 애니메이션 플레이
	//m_pMeshCom->Play_Animation(m_dAnimationTime, 2);

	//////////////////////////////////////////////////////////////////
	//if (P_A_BASIC_IDLE != m_eUpperAimation)
	//	m_pMeshCom->Play_Animation(m_dAnimationTime, 1, "Cnt_Spine_002_JNT_SKL");		// 애니메이션 블랜딩 플레이
	//else
	//	m_pMeshCom->Play_Animation(m_dAnimationTime);		// 단일 애니메이션 플레이
	//////////////////////////////////////////////////////////////////

	IsRide_SetPos();

	Check_Buff(m_dAnimationTime);

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	if (m_bDissolve)
	{
		m_fDissolveAmount += _float(m_dAnimationTime * 1.f);
		if (1.f <= m_fDissolveAmount)
			m_fDissolveAmount = 1.f;
	}
	else
		m_fDissolveAmount = 0.f;

	if (m_bIsFPVCam)
	{
		m_pMeshFPVCom->Render_Meshes(pEffect, m_bDissolve, m_bIsFPVSubsetRender);
	}
	else
	{
		m_pMeshCom->Render_Meshes(pEffect, m_bDissolve, m_bIsTPVSubsetRender);
	}

	pEffect->End();

	Engine::Safe_Release(pEffect);
	//////////////////////////////////////////////////////////////ㅎㅇㅈ//////

#ifdef _DEBUG
	_vec3 vPos, vLook, vRight, vEye, vAt, vCamLook;

	if (isnan(vPos.x))
		int a = 0;
	// 플레이어
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	wstring wstrPos = L"Player Pos		: " + to_wstring(vPos.x) + L" / " + to_wstring(vPos.y) + L" / " + to_wstring(vPos.z);
	Engine::Render_Font(L"Font_Debug_Bold", wstrPos, &_vec2(10.f, 5.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	wstring wstrLook = L"Player Look	: " + to_wstring(vLook.x) + L" / " + to_wstring(vLook.y) + L" / " + to_wstring(vLook.z);
	Engine::Render_Font(L"Font_Debug_Bold", wstrLook, &_vec2(10.f, 25.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);
	wstring wstrRight = L"Player Right	: " + to_wstring(vRight.x) + L" / " + to_wstring(vRight.y) + L" / " + to_wstring(vRight.z);
	Engine::Render_Font(L"Font_Debug_Bold", wstrRight, &_vec2(10.f, 45.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//카메라
	_mat matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	memcpy(&vCamLook, &matView.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vCamLook, &vCamLook);

	wstrPos = L"Pos		: " + to_wstring(vCamLook.x) + L" / " + to_wstring(vCamLook.y) + L" / " + to_wstring(vCamLook.z);
	Engine::Render_Font(L"Font_Debug_Bold", wstrPos, &_vec2(10.f, 65.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


	wstring wstrAngleX = L"Player AngleX	: " + to_wstring(m_fAngleX);
	Engine::Render_Font(L"Font_Debug_Bold", wstrAngleX, &_vec2(10.f, 65.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	wstring wstrAngleY = L"Player AngleY	: " + to_wstring(m_fAngleY);
	Engine::Render_Font(L"Font_Debug_Bold", wstrAngleY, &_vec2(10.f, 85.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	wstring wstrPower = L"Player AngleY	: " + to_wstring(m_fThrowPower);
	Engine::Render_Font(L"Font_Debug_Bold", wstrPower, &_vec2(10.f, 105.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	//마우스
	POINT Mouse;
	Engine::Get_pMousePos(&Mouse);
	wstring wstrMouse = L"MouseX: " + to_wstring(Mouse.x) + L" / MouseY: " + to_wstring(Mouse.y);
	Engine::Render_Font(L"Font_Debug", wstrMouse, &_vec2(500.f, 5.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


	// HP
	wstring wstrHp = L"HP : " + to_wstring(m_tStat.fHp);
	Engine::Render_Font(L"Font_Debug_Bold", wstrHp, &_vec2(10.f, 550.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	// 레이캐스팅
	if (m_bIsPick)
	{
		Engine::Render_Font(L"Font_Debug_Bold", m_wstrTargetLayer, &_vec2(400.f, 300.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		if (m_wstrTargetLayer == L"Layer_Dino" || m_wstrTargetLayer == L"Layer_Dino_Tamed")
		{
			wstring wstrHp = L"HP : " + to_wstring(static_cast<CUnit*>(m_pLookObject)->Get_Stat(CUnit::STAT_HP));
			Engine::Render_Font(L"Font_Debug_Bold", wstrHp, &_vec2(400.f, 250.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
	}

	// 애니메이션
	wstring wstrAni = L"Ani : " + to_wstring(m_AniTest);
	Engine::Render_Font(L"Font_Debug_Bold", wstrAni, &_vec2(600.f, 300.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	// 주먹 충돌체
	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Render_SphereCollider();
	// 1인칭 충돌체
	if (m_pFPVSphereColliderCom)
		m_pFPVSphereColliderCom->Render_SphereCollider();

	// 조명
	wstring wstrLightPos = L"Light Pos : " + to_wstring(m_vecLightPos.x) + L" / " + to_wstring(m_vecLightPos.y) + L" / " + to_wstring(m_vecLightPos.z);
	Engine::Render_Font(L"Font_Debug_Bold", wstrLightPos, &_vec2(10.f, 165.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	wstring wstrLightLook = L"Light Look : " + to_wstring(m_vecLightAt.x) + L" / " + to_wstring(m_vecLightAt.y) + L" / " + to_wstring(m_vecLightAt.z);
	Engine::Render_Font(L"Font_Debug_Bold", wstrLightLook, &_vec2(10.f, 185.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


#endif // _DEBUG
}

void CPlayer::Render_GameObjectShadowMap()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowMapCom->Get_EffectHandle();
	pEffect->AddRef();

	if (m_bIsFPVCam)
		m_matWLV = m_pTransformFPVCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	else
		m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(NULL, 0);

	if (m_bDissolve)
		pEffect->BeginPass(1);
	else
		pEffect->BeginPass(0);

	if (m_bIsFPVCam)
	{
		m_pMeshFPVCom->Render_MeshesShadowMap(pEffect, m_bIsFPVSubsetRender);
	}
	else
	{
		m_pMeshCom->Render_MeshesShadowMap(pEffect, m_bIsTPVSubsetRender);
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CPlayer::Render_GameObjectShadow()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowCom->Get_EffectHandle();
	pEffect->AddRef();

	m_pDevice->GetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);

	if (m_bIsFPVCam)
	{
		m_matWVP = m_pTransformFPVCom->Get_WorldMatrix() * m_matView * m_matProj;
		m_matWLV = m_pTransformFPVCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	}
	else
	{
		m_matWVP = m_pTransformCom->Get_WorldMatrix() * m_matView * m_matProj;
		m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	}

	m_matWLVB = m_matWLV * m_matScaleBias;

	pEffect->SetMatrix("g_matWorldViewProj", &m_matWVP);
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);
	pEffect->SetMatrix("g_matWorldLightViewProjBias", &m_matWLVB);

	if (m_bIsFPVCam)
		D3DXMatrixInverse(&m_matWLVB, NULL, &m_pTransformFPVCom->Get_WorldMatrix());
	else
		D3DXMatrixInverse(&m_matWLVB, NULL, &m_pTransformCom->Get_WorldMatrix());
	D3DXVec3Transform(&m_vDir, &m_vecLightPos, &m_matWLVB);
	D3DXVec4Normalize(&m_vDir, &m_vDir);
	m_vDir.w = 0;

	pEffect->SetVector("g_vLightDir", &m_vDir);

	Engine::Get_RenderTargetTexture(pEffect, L"ShadowMap", "g_ShadowMapTexture");
	Engine::Get_RenderTargetTexture(pEffect, L"ShadowBlurY", "g_ShadowBlurTexture");

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(NULL, 0);

	if (m_bDissolve)
		pEffect->BeginPass(1);

	else
		pEffect->BeginPass(0);

	if (m_bIsFPVCam)
	{
		m_pMeshFPVCom->Render_MeshesShadow(pEffect, m_bIsFPVSubsetRender);
	}
	else
	{
		m_pMeshCom->Render_MeshesShadow(pEffect, m_bIsTPVSubsetRender);
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CPlayer*	pInstance = new CPlayer(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CUnit * CPlayer::Clone(void * _pArg/* = nullptr*/)
{
	CPlayer*	pClone = new CPlayer(*this);

	if (FAILED(pClone->Ready_GameObject()))
		Engine::Safe_Release(pClone);

	return pClone;
}

void CPlayer::Free()
{
	Engine::Safe_Delete_Array(m_bIsFPVSubsetRender);
	Engine::Safe_Delete_Array(m_bIsTPVSubsetRender);
	Engine::Safe_Release(m_pLookObject);
	Engine::Safe_Release(m_pDino);

	if (m_pCtrl)
		m_pCtrl->release();

	CUnit::Free();
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	/// 매쉬 컴포넌트
	// 1인칭
	pComponent = m_pMeshFPVCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Player_Male_FPV"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_MeshFPV", pComponent);

	pComponent = m_pTransformFPVCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_TransformFPV", pComponent);

	// 3인칭
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Player_Male_TPV"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);


	////////////////////////////////////////////////////////////////////////////
	/// 충돌체
	Load_Collider_Player(L"../Resources/Data/Collider/Player_Collider.dat");

	//////////////////////////////////
	pComponent = m_pFPVSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider"));
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereColliderFPV", m_pFPVSphereColliderCom);
	//
	CSphereCollider::SPHEREINFO tFPVCol;
	tFPVCol.eColType = CSphereCollider::COL_ATTACK;
	tFPVCol.vSpherePos = _vec3(0.f, 0.f, 0.f);
	tFPVCol.fRadius = 0.2f;
	//tFPVCol.pTartgetMesh = m_pMeshFPVCom;
	//tFPVCol.pTargetWorldMatix = &m_pTransformFPVCom->Get_WorldMatrix();
	//tFPVCol.strBoneName = L"camera";
	D3DXCreateSphere(m_pDevice, 1.f, 10, 10, &tFPVCol.pSphere, nullptr);
	//
	m_pFPVSphereColliderCom->Get_listCollider(CSphereCollider::COL_ATTACK)->emplace_back(tFPVCol);
	//m_pFPVSphereColliderCom->Update_Component(0.f);
	//
	//m_pFPVSphereColliderCom->Add_SphereCollider(&tFPVCol, this);
	//m_pSphereColliderCom->Add_SphereCollider(L"camera", this, CSphereCollider::COL_ATTACK, _vec3(0.f, 0.f, 2.f), 3.f);
	////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

void CPlayer::Load_Collider_Player(const wstring & FilePath)
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
				//m_pHandSphereColliderCom->Add_SphereCollider(L"tset");
				Safe_Delete_Array(pBoneName);
			}
		}
		break;
	}

	CloseHandle(hFile);
}

void CPlayer::Open_Inventory(CGameObject* pTargetInven/* = nullptr*/)
{
	if (!m_bIsOpenInventory)
	{
		if (pTargetInven)
		{
			// 대상 인벤토리랑 같이 열기
			static_cast<CRight_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Set_InvenOwner(static_cast<CUnit*>(pTargetInven));
			static_cast<CMonsterStat_UI*>(Engine::Get_GameObject(L"Center_UI", 1))->Stat_UI_Open(static_cast<CUnit*>(pTargetInven));
			m_bIsOpenInventory = true;
		}
		else
		{
			// 플레이어 인벤토리 열기
			static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->Stat_UI_Open();
			m_bIsOpenInventory = true;
		}

		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_FixMouseCursor(false);
		ShowCursor(true);
		m_bIsLockCamera = true;
	}
}

void CPlayer::Close_Inventory()
{
	if (m_bIsOpenInventory)
	{
		m_eUpperPlayerState = STATE_IDLE;
		//static_cast<CRight_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Right_InventoryClose();
		static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->Stat_UI_Close(); //인벤 관련 다 꺼짐
		m_bIsOpenInventory = false;
		m_pTargetInvecObject = nullptr;

		m_dOpenDelayInven = 0.0;

		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_FixMouseCursor(true);
		ShowCursor(false);
		m_bIsLockCamera = false;
	}
}

void CPlayer::Equip_Armor(EQUIP_Position _ePosType, ITEMID _eArmorType)
{
	switch (_ePosType)
	{
	case Engine::EQUIP_HAT:
		break;
	case Engine::EQUIP_TOP:
		if (ITEMID_Leather_TOP == _eArmorType)
			m_bIsTPVSubsetRender[5] = true;
		else if (ITEMID_Metal_TOP == _eArmorType)
			m_bIsTPVSubsetRender[9] = true;
		break;
	case Engine::EQUIP_LEGS:
		m_bIsTPVSubsetRender[1] = false;
		if(ITEMID_Leather_Legs == _eArmorType)
			m_bIsTPVSubsetRender[4] = true;
		else if(ITEMID_Metal_Legs == _eArmorType)
			m_bIsTPVSubsetRender[8] = true;
		break;
	case Engine::EQUIP_GLOVES:
		if (ITEMID_Leather_Gloves == _eArmorType)
			m_bIsTPVSubsetRender[3] = true;
		else if (ITEMID_Metal_Gloves == _eArmorType)
			m_bIsTPVSubsetRender[7] = true;
		break;
	case Engine::EQUIP_BOOTS:
		if (ITEMID_Leather_Boots == _eArmorType)
			m_bIsTPVSubsetRender[2] = true;
		else if (ITEMID_Metal_Boots == _eArmorType)
			m_bIsTPVSubsetRender[6] = true;
		break;
	case Engine::EQUIP_SUB:
		break;
	default:
		break;
	}

	//m_bIsTPVSubsetRender[1] = true;		// 언더웨어
	//m_bIsTPVSubsetRender[2] = false;		// 가죽 신발
	//m_bIsTPVSubsetRender[3] = false;		// 가죽 장갑
	//m_bIsTPVSubsetRender[4] = false;		// 가죽 바지
	//m_bIsTPVSubsetRender[5] = false;		// 가죽 상의
	//m_bIsTPVSubsetRender[6] = false;		// 방탄 신발
	//m_bIsTPVSubsetRender[7] = false;		// 방탄 장갑
	//m_bIsTPVSubsetRender[8] = false;		// 방탄 바지
	//m_bIsTPVSubsetRender[9] = false;		// 방탄 상의
}

void CPlayer::Unequip_Armor(EQUIP_Position _ePosType)
{
	switch (_ePosType)
	{
	case Engine::EQUIP_HAT:
		break;
	case Engine::EQUIP_TOP:
		m_bIsTPVSubsetRender[5] = false;
		m_bIsTPVSubsetRender[9] = false;
		break;
	case Engine::EQUIP_LEGS:
		m_bIsTPVSubsetRender[1] = true;
		m_bIsTPVSubsetRender[4] = false;
		m_bIsTPVSubsetRender[8] = false;
		break;
	case Engine::EQUIP_GLOVES:
		m_bIsTPVSubsetRender[3] = false;
		m_bIsTPVSubsetRender[7] = false;
		break;
	case Engine::EQUIP_BOOTS:
		m_bIsTPVSubsetRender[2] = false;
		m_bIsTPVSubsetRender[6] = false;
		break;
	case Engine::EQUIP_SUB:
		break;
	default:
		break;
	}
}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect)
{
	// 상수테이블 세팅
	if (m_bIsFPVCam)
	{
		_pEffect->SetMatrix("g_matWorld", &m_pTransformFPVCom->Get_WorldMatrix());
		Set_ConstantTable_Normal(_pEffect, m_pTransformFPVCom->Get_WorldMatrix());
	}
	else
	{
		_pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());
		Set_ConstantTable_Normal(_pEffect, m_pTransformCom->Get_WorldMatrix());
	}

	_pEffect->SetFloat("g_fAmount", m_fDissolveAmount);
	_pEffect->SetVector("g_vSetColor", &_vec4(m_vColor, 1.f));
	_pEffect->SetFloat("g_fShadowBias", 0.0001f);

	m_pDissolveTextureCom->Set_Texture(_pEffect, "g_DissolveTexture");

	return S_OK;
}

CGameObject * CPlayer::Ray_PlayerLook(const wstring & _wstrLayerTag, _float _fRayDis, _bool bDinoRide)
{
	auto pLayer = Engine::Get_LayerList(_wstrLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	if (0 >= pLayer->size())
		return nullptr;

	_float fCamDis = 0.f;
	if (!m_bIsFPVCam)
		fCamDis = MAXCAMDIS;

	CGameObject* pTargetObj = nullptr;
	_float fTargetDis = -1.f;

	for (auto pObj : *pLayer)
	{
		//  현재 타고 있는 공룡은 Lay에서 제외
		if (bDinoRide)
		{
			if (static_cast<CDino*>(pObj)->Get_NowRidingState())
				continue;
		}

		// 콜리전 여부로 1차 필터링
		CSphereCollider* pCollider = static_cast<CSphereCollider*>(pObj->Get_Component(L"Com_SphereCollider", ID_DYNAMIC));
		CTransform*	pTransform = static_cast<CTransform*>(pObj->Get_Component(L"Com_Transform", ID_DYNAMIC));

		if (nullptr == pCollider)
			continue;

		CSphereCollider::SPHEREINFO tColInfo = pCollider->Get_listCollider(CSphereCollider::COL_INTERACTION)->front();

		_float fColRadius;
		_vec3 fColPos;

		fColRadius = pCollider->Get_InteractRadius();

		if (nullptr != tColInfo.pParentBoneMatrix)
		{
			// 부모뼈가 있을 경우 타겟의 월드와 뼈를 곱해준다.
			fColRadius *= pTransform->Get_Scale()->x;
			memcpy(&fColPos, &tColInfo.matColMatrix.m[INFO_POS][0], sizeof(_vec3));
		}
		else
			fColPos = pCollider->Get_InteractPos();

		// 거리로 2차 필터링		
		if (0.f >= fColRadius)
			continue;

		/// 여기서부터 직선과 구체충돌 시작
		// 뷰 역행렬 가져오기
		_mat matView;
		m_pDevice->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, NULL, &matView);

		// 피킹 시작할 포스와 방향 만들기
		_vec3 vCamPos, vCamLook;
		memcpy(&vCamPos, &matView.m[INFO_POS][0], sizeof(_vec3));
		memcpy(&vCamLook, &matView.m[INFO_LOOK][0], sizeof(_vec3));
		D3DXVec3Normalize(&vCamLook, &vCamLook);

		vCamPos += vCamLook * fCamDis;

		// 수선의 발을 구하기 위한 cos 값 구하기
		_vec3 v1 = fColPos - vCamPos;					// 직선의 시작점과 구체중심을 이어주는 백터
		_float f2 = D3DXVec3Length(&v1);				// 직선의 시작점과 구체 중심의 거리를 구한다.

														// 거리로 필터링
		if (f2 > _fRayDis + fColRadius)
			continue;

		_float f1 = D3DXVec3Dot(&vCamLook, &v1);		// 수선의 발을 내린 빗변의 길이를 구한다.

		if (0.f > f1)
			continue;

		_float fDis = sqrtf((f2 * f2) - (f1 * f1));

		if (fDis < fColRadius)
		{
			if (0.f > fTargetDis)
			{
				// 처음 타겟을 찾았을 때
				pTargetObj = pObj;
				fTargetDis = f2;
			}
			else
			{
				// 다음 타겟을 찾았을 때
				if (fTargetDis > f2)
				{
					pTargetObj = pObj;
					fTargetDis = f2;
				}
			}
		}
	}

	return pTargetObj;
}

void CPlayer::Check_PlayerLookLayer()
{
	// 기존 룩 오브젝트 삭제
	Safe_Release(m_pLookObject);

	// 카메라가 멀어져서 공룡탔을 때 채집물들이 안잡힘
	// 일단 숄더뷰 할 때 10.f 멀어져서 10.f
	// 공룡 별 카메라 멀어지는 거리 있으면 그걸로 변경
	_float fAddDis = 0.f;
	if (m_bIsRide)
		fAddDis = 10.f;

	if (m_iTopSngFontID)
	{
		static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Remove_WorldFont_UI(m_iTopSngFontID);
		m_iTopSngFontID = 0;
	}

	_vec3 AddPos = DINONAMEADDWORLDPOS;
	_vec2 AddScreenPos = { 0.f,12.f };
	wstring wstrMsg;
	if (m_pLookObject = Ray_PlayerLook(L"Layer_Dino", 5.f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_Dino";
		m_pLookObject->AddRef();
		//////////////////////////////////////////////////////////////////////////
		//아직 테이밍하지않은 공룡 중에 테이밍 방식이 다른애들
		wstring wstrDinoName = static_cast<CDino*>(m_pLookObject)->Get_DinoSpecies();

		if (L"Ankylo" == wstrDinoName)
		{
			if (!m_iTopSngFontID && !static_cast<CDino*>(m_pLookObject)->Get_Dying())
			{
				if (!static_cast<CDino*>(m_pLookObject)->Get_PeaceTaming())
					wstrMsg = L"[E] : 길들이기 위해 아마르 베리를 먹입니다.";
				else
				{
					wstrMsg = L"아직 뭘 먹으려 하지 않습니다.";
					AddPos = DinoInfoViewFixWorldPos;
					if (Engine::CGraphicDevice::ESCREEN_RESOULTION_4To3 == EScrnResolution)
						AddScreenPos = { 0.f,0.23f*ScrnRatio };
					else
						AddScreenPos = { 0.f, -1.4f*ScrnRatio };
				}

				m_iTopSngFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((static_cast<CDino*>(m_pLookObject))->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrMsg, CWorldFont_UI::World_Font_Big_Blue, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f, AddScreenPos);
			}
		}
	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_Dino_Tamed", 3.f + fAddDis, true))
	{
		m_wstrTargetLayer = L"Layer_Dino_Tamed";
		m_pLookObject->AddRef();
		/////////////////////////////////////////탑승 가능하다는 폰트 띄워주기
		if (!static_cast<CDino*>(m_pLookObject)->Get_NowRidingState() && static_cast<CDino*>(m_pLookObject)->Get_CanbeRide()) //플레이어가 타고 있는 친구가 아니라면
		{
			if (!m_iTopSngFontID)//만약 탑승 안 띄웠다면
			{
				wstrMsg = L"[E] : [탑승] \n 누르고 있으면 더 많은 옵션";
				m_iTopSngFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((static_cast<CDino*>(m_pLookObject))->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrMsg, CWorldFont_UI::World_Font_Big_Blue, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f, AddScreenPos);
			}
		}
		else if (static_cast<CDino*>(m_pLookObject)->Get_IsBaby())
		{
			if (!m_iTopSngFontID)
			{
				wstrMsg = static_cast<CDino*>(m_pLookObject)->Get_BabyMsg();

				AddPos = DinoInfoViewFixWorldPos;
				if (Engine::CGraphicDevice::ESCREEN_RESOULTION_4To3 == EScrnResolution)
					AddScreenPos = { 0.f, 0.23f * ScrnRatio };
				else
					AddScreenPos = { 0.f, -1.4f * ScrnRatio };

				m_iTopSngFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((static_cast<CDino*>(m_pLookObject))->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrMsg, CWorldFont_UI::World_Font_Big_Blue, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f, AddScreenPos);
			}
		}
	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_Structure", 3.f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_Structure", 1.f;
		m_pLookObject->AddRef();
	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_Harvest_Tree", 0.f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_Harvest_Tree";
		m_pLookObject->AddRef();
	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_Harvest_Rock", 2.5f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_Harvest_Rock";
		m_pLookObject->AddRef();
	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_Harvest_Pebble", 3.f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_Harvest_Pebble";
		m_pLookObject->AddRef();
		/////////////////////////////////////////채집 가능하다는 폰트 띄워주기
		if (!m_iTopSngFontID)//만약 채집 안 띄웠다면
		{
			wstrMsg = L"[E] : [채집]";
			m_iTopSngFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((static_cast<CDino*>(m_pLookObject))->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrMsg, CWorldFont_UI::World_Font_Big_Blue, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f, AddScreenPos);
		}
		//////////////////////////////////////////////////////////////////////
	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_Harvest_Bush", 1.f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_Harvest_Bush";
		m_pLookObject->AddRef();
		/////////////////////////////////////////채집 가능하다는 폰트 띄워주기
		if (!m_iTopSngFontID && !static_cast<CUnit*>(m_pLookObject)->Get_Dying())//만약 채집 안 띄웠다면
		{
			wstrMsg = L"[E] : [줍기]";
			m_iTopSngFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((static_cast<CDino*>(m_pLookObject))->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrMsg, CWorldFont_UI::World_Font_Big_Blue, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f, AddScreenPos);
		}
		//////////////////////////////////////////////////////////////////////
	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_Egg", 2.5f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_Egg";
		m_pLookObject->AddRef();

		wstrMsg = static_cast<CDino_Egg*>(m_pLookObject)->Get_Msg();
		m_iTopSngFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((static_cast<CDino*>(m_pLookObject))->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrMsg, CWorldFont_UI::World_Font_Big_Blue, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f, AddScreenPos);

	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_Ball", 2.5f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_Ball";
		m_pLookObject->AddRef();
		if (!m_bGrabBall)
		{
			wstrMsg = L"[E] : [줍기]";
			m_iTopSngFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((static_cast<CDino*>(m_pLookObject))->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrMsg, CWorldFont_UI::World_Font_Big_Blue, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f, AddScreenPos);
		}
	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_BasketballHoop", 2.5f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_BasketballHoop";
		m_pLookObject->AddRef();
		if (!m_bGrabBall)
		{
			wstrMsg = L"[E] : [누르고 있기] ";
			m_iTopSngFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((static_cast<CDino*>(m_pLookObject))->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrMsg, CWorldFont_UI::World_Font_Big_Blue, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f, AddScreenPos);
		}
	}
	else
	{
		m_wstrTargetLayer = L"Nothing";		
		m_pLookObject = nullptr;
	}
}

void CPlayer::Is_Building()
{
	if (m_bIsBuilding)
	{
		_bool bIsBulidingOK = false;
		CStructure* pStructure = nullptr;

		PxVec3 vRayPos, vRayDir;

		_float vOutPickDis;
		_vec3 vOutPickPos;
		_float fAngleY = 0.f;
		_uint iColDir;

		// 플랫폼 안장인지 체크
		CGameObject* pPlatform = nullptr;
		if (pPlatform = Engine::Get_GameObject(L"Layer_PlatformSaddle_Brachio"))
		{
			_vec3 vFlatform = static_cast<CSphereCollider*>(pPlatform->Get_Component(L"Com_SphereCollider", ID_DYNAMIC))->Get_InteractPos();
			vFlatform = vFlatform - m_vPlayerPos;
			_float fDis = D3DXVec3Length(&vFlatform);

			if (static_cast<CSphereCollider*>(pPlatform->Get_Component(L"Com_SphereCollider", ID_DYNAMIC))->Get_InteractRadius() < fDis)
				pPlatform = nullptr;
		}

		///
		if (pStructure = Ray_FindPivot(&vOutPickPos, &fAngleY, &iColDir))
		{
			if (bIsBulidingOK = Pivot_Relocation(pStructure, vOutPickPos, fAngleY, iColDir))
			{
				pPlatform = pStructure->Get_TargetPlatform();
			}
		}
		else
		{
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
			// 플레이어과 카메라 사이의 거리 가져오기
			_float fCamDis = static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Get_TPVCam_Dis();

			// 뷰 역행렬 가져오기
			_mat matView;
			m_pDevice->GetTransform(D3DTS_VIEW, &matView);
			D3DXMatrixInverse(&matView, NULL, &matView);

			// 피킹 시작할 포스와 방향 만들기
			_vec3 vCamPos, vCamLook;
			memcpy(&vCamPos, &matView.m[INFO_POS][0], sizeof(_vec3));
			memcpy(&vCamLook, &matView.m[INFO_LOOK][0], sizeof(_vec3));
			D3DXVec3Normalize(&vCamLook, &vCamLook);

			vCamPos += vCamLook * fCamDis;

			vRayPos = PxVec3(vCamPos.x, vCamPos.y, vCamPos.z);
			vRayDir = PxVec3(vCamLook.x, vCamLook.y, vCamLook.z);

			if (!CPhysX_Mgr::GetInstance()->RayCast(vRayPos, vRayDir, 10.f, &vOutPickDis, &vOutPickPos))
			{
				// 레이캐스팅에 실패했을 경우
				vOutPickPos = vCamPos + (vCamLook * 3.f);
			}
			else
			{
				bIsBulidingOK = true;
			}
		}

		if (L"Proto_Obj_GateFrame" == m_wstrPreviewStructure)
		{
			vOutPickPos.y += 3.5f;
		}


		// 프리뷰 보여주기
		if(pStructure)
			Preview_Structure(vOutPickPos, bIsBulidingOK, &fAngleY);
		else
			Preview_Structure(vOutPickPos, bIsBulidingOK);


		// 건물짓기 확정
		if (Engine::Get_BIMouseDown(DIM_LB))
		{
			if (bIsBulidingOK)
			{
				// 건물 짓기
				if (pStructure)
					Build_Structure(vOutPickPos, pPlatform, &fAngleY);
				else
					Build_Structure(vOutPickPos, pPlatform);

				// 카메라 원위치
				static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2);

				// 아이탬 소모
				//CGameObject* pQuickSlot = Engine::Get_GameObject(L"QuickSlot_UI");
				//static_cast<CQuickSlot_UI*>(pQuickSlot)->UisngRemove_Item(m_iQuickSlot, 1);
			}
			else
			{
				// 건물 짓기 취소
				// 프리뷰 지우기
				Is_BuildingCancle();
			}
		}
		else if (Engine::Get_BIMouseDown(DIM_RB))
		{
			// 건물 짓기 취소
			Is_BuildingCancle();
		}
	}
}

void CPlayer::Is_BuildingCancle()
{
	// 프리뷰 지우기
	CGameObject* pPreView = Engine::Get_GameObject(L"Layer_PreViewStructure");

	if (nullptr == pPreView)
		return;

	pPreView->Set_Dead();

	static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2);

	m_bIsBuilding = false;
	m_wstrPreviewStructure = L"";
	m_eBuilding = STRUCTURE_END;
}

void CPlayer::Check_IsEndAnimation()
{
	switch (m_eUnitState)
	{
	case CUnit::STATE_JUMPING:
		if (m_pMeshCom->Is_AnimationsetEnd(0.3))
		{
			m_eUnitState = STATE_FALLING;
		}
		break;
	case CUnit::STATE_LANDING:
		if (m_pMeshCom->Is_AnimationsetEnd(0.3))
		{
			m_eUnitState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_DEAD:
		break;
	default:
		break;
	}
}

void CPlayer::Check_IsEndUpperAnimation(const _double& _dTimeDelta)
{
	if (m_bIsFPVCam)
	{
		FPV_Check_IsEndAnimation(_dTimeDelta);
		return;
	}

	// 첫 프레임에 바로 들어가지 않게 Pre 스테이트를 사용
	switch (m_ePreUpperPlayerState)
	{
	case CUnit::STATE_ATTACK:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.1))
		{
			// 공격 종료
			m_eUpperPlayerState = STATE_IDLE;
			m_bIsAttack = false;
			m_dUpperAnimationSpeed = 1;
		}
		break;
	case CUnit::STATE_HURT:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.1))
		{
			m_eUpperPlayerState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_EAT:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.1))
		{
			m_eUpperPlayerState = STATE_IDLE;
			m_bIsInteract = false;
		}
		break;
	case CUnit::STATE_HARVEST:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.1))
		{
			m_eUpperPlayerState = STATE_IDLE;
			m_bIsInteract = false;
		}
		break;
	case CUnit::STATE_INVENTORY:
		m_dOpenDelayInven += _dTimeDelta;
		if (!m_bIsOpenInventory && 0.2 < m_dOpenDelayInven)
			Open_Inventory();
		break;
	case CUnit::STATE_ATCTIVATECONSOLE:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.3))
		{
			// 손짓 한번하고 idle 상태
			m_eUpperAnimation = P_A_BASIC_IDLE;
			Open_Inventory(m_pTargetInvecObject);
			m_dUpperAnimationSpeed = 1;
		}
		break;
	case CUnit::STATE_WHISTLE:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.1))
		{
			m_eUpperPlayerState = STATE_IDLE;
			m_ePlayerWhistle = P_WHISTLE_END;
		}
	case CUnit::STATE_WEAPON_DRAW:		// 무기 꺼내기
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.3))
		{
			m_eUpperPlayerState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_WEAPON_PUTAWAY:	// 무기 넣기
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.3))
		{
			// 현재 무기 집어넣고 다음 무기가 있을 경우 STATE_WEAPON_DRAW 로 변경
			if (Swap_Weapon())
				m_eUpperPlayerState = STATE_WEAPON_DRAW;
			else
				m_eUpperPlayerState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_WEAPON_THROW:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.1))
		{
			/// 던지는 무기가 볼라일 경우
			if (P_STANCE_BOAL == m_ePlayerStance)
			{
				// 기존 아이탬 삭제
				Engine::Get_GameObject(m_wstrLeftHand, 0)->Set_Dead();
				m_bIsGrapLeftHand = false;

				// 볼라 투사체 생성
				Shot_Bullet(BULLET_BOLA);

				// 퀵슬롯에 볼라가 남아있을 경우 새로운 볼라 꺼내기
				if (ITEMID_END != static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI"))->Get_Item_To_Number(m_iQuickSlot).iItemID)
				{
					Swap_Weapon();
				}
				else
				{
					// 볼라가 없을 경우 빈손으로 돌아오기
					m_eUpperPlayerState = STATE_IDLE;
					m_ePlayerStance = P_STANCE_BASIC;
					m_wstrPreLeftHand = m_wstrLeftHand = L"";
				}
			}
			else if (P_STANCE_BOW == m_ePlayerStance)
			{
				// 화살 발사
				//Shot_Bullet(BULLET_ARROW);

				m_eUpperPlayerState = STATE_WEAPON_RELOAD;
			}
			else
				m_eUpperPlayerState = STATE_IDLE;

			m_dUpperAnimationSpeed = 1;
		}
		break;
	case CUnit::STATE_WEAPON_PULLBACK:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.1))
		{
			if (P_STANCE_BOW == m_ePlayerStance)
			{
				m_eUpperAnimation = P_A_BOW_PULLSTRING_LOOP;
			}
		}
		break;
	case CUnit::STATE_WEAPON_RELOAD:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.1))
		{
			// 재장전 완료
			if (P_STANCE_BOW == m_ePlayerStance)
			{
				// 활에 화살 추가

			}
			m_eUpperPlayerState = STATE_IDLE;
		}
		break;
	default:
		break;
	}
}

void CPlayer::Key_Input(const _double& _dTimeDelta)
{
	// 카메라 연출중이면 바로 리턴
	if (STATE_CAMERAWORK == m_eUnitState)
	{
		return;
	}

	// 마우스 무브 (카메라 및 플레이어 각도)
	Key_Input_MouseMove(_dTimeDelta);

	// 인벤토리
	Key_Input_Inventory(_dTimeDelta);

	/// 포즈별 플레이어 속도 조절
	switch (m_ePlayerPose)
	{
	case CPlayer::P_POSE_STAND:
		m_fPlayerSpeed = 10.f;
		//static_cast<CTPV_Camera*>(Engine::Get_GameObject(L"Layer_Camera_TPV", 0))->Set_EyeHeight(1.8f);
		break;
	case CPlayer::P_POSE_CROUCH:
		//static_cast<CTPV_Camera*>(Engine::Get_GameObject(L"Layer_Camera_TPV", 0))->Set_EyeHeight(1.2f);
		m_fPlayerSpeed = 5.f;
		break;
	case CPlayer::P_POSE_PRONE:
		//static_cast<CTPV_Camera*>(Engine::Get_GameObject(L"Layer_Camera_TPV", 0))->Set_EyeHeight(0.8f);
		m_fPlayerSpeed = 3.f;
		break;
	}

	/// 이동 (플레이어의 애니메이션 상태 초기화)
	Key_Input_Move(_dTimeDelta);

	/// 점프중이 아닐 때 자세 변경 (C) : 서있기 > 앉기 > 눕기
	if (!m_bIsOpenInventory)
	{
		if (STATE_JUMPING != m_eUnitState || STATE_LANDING != m_eUnitState)
		{
			if (Engine::Get_DIKeyDown(DIK_C))
			{
				if (P_POSE_CROUCH == m_ePlayerPose)
					m_ePlayerPose = P_POSE_STAND;
				else
					m_ePlayerPose = P_POSE_CROUCH;
			}
			else if (Engine::Get_DIKeyDown(DIK_X))
			{
				if (P_POSE_PRONE == m_ePlayerPose)
					m_ePlayerPose = P_POSE_STAND;
				else
					m_ePlayerPose = P_POSE_PRONE;
			}
		}

		/// 상체 스테이트 변경
		// 상호작용
		Key_Input_Interact(_dTimeDelta);

		// 휘파람
		Key_Input_Whistle();

		// 공격 (마우스 좌클릭) : 공격중일 때, 먹을 때, 주울 때, 아이탬창 볼 때 는 공격 안함
		Key_Input_Attack(_dTimeDelta);

		// 퀵슬롯 : (아이탬 선택)
		Key_Input_QuickSlot(_dTimeDelta);

		/// 테스트
		if (Engine::Get_DIKeyDown(DIK_F1))
		{
			// 임시로 사용중인 함수 (공룡이 아닌 오브젝트에서 호출할 경우 터짐)
			if (m_pLookObject)
				static_cast<CDino*>(m_pLookObject)->Set_IsTamed();
		}

		//if (Engine::Get_DIKeyState(DIK_R))
		//	g_GlobalTime = 0.2;
		//else
		//	g_GlobalTime = 1.0;
	}

#ifdef _DEBUG
	//// 애니메이션 체크용
	//if(Engine::Get_DIKeyDown(DIK_9))
	//{
	//	if(0 < m_iAnimation)
	//		--m_iAnimation;
	//}
	//else if (Engine::Get_DIKeyDown(DIK_0))
	//{
	//	if (MAX_ANI > m_iAnimation)
	//		++m_iAnimation;
	//}
#endif // _DEBUG
}

void CPlayer::Key_Input_Move(const _double & _dTimeDelta)
{
	PxVec3 vPos(0);
	if (STATE_JUMPING != m_eUnitState &&
		STATE_FALLING != m_eUnitState &&
		STATE_LANDING != m_eUnitState &&
		STATE_INVENTORY != m_eUnitState)
		m_eUnitState = STATE_END;

	/// 방향키 입력
	if (!m_bIsOpenInventory)
	{
		if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			// 앞으로 이동
			if (STATE_JUMPING != m_eUnitState)
				m_eUnitState = STATE_MOVE;
			m_eUnitMoveDir = UNIT_DIR_UP;

			_vec3 vLook;
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			vPos -= vLook * (_float)_dTimeDelta * m_fPlayerSpeed;

			// 전력질주

			if (P_POSE_STAND == m_ePlayerPose && Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80)
			{
				m_dRecoveryStaminaTime = 0.0;
				m_bRecoveryStamina = false;

				if (0.f < m_tStat.fStamina)
				{
					m_eUnitState = STATE_SPRINT;
					//vPos -= vLook * (_float)_dTimeDelta * m_fPlayerSpeed * 1.2f;
					vPos -= vLook * (_float)_dTimeDelta * m_fPlayerSpeed * 10.f;

					// 기력감소
					//m_tStat.fStamina -= 10.f * (_float)_dTimeDelta;

					if (0.f >= m_tStat.fStamina)
						m_tStat.fStamina = 0.f;
				}
			}
		}
		else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			// 뒤로 이동
			if (STATE_JUMPING != m_eUnitState)
				m_eUnitState = STATE_MOVE;
			m_eUnitMoveDir = UNIT_DIR_DOWN;

			_vec3 vLook;
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			vPos += vLook * (_float)_dTimeDelta * m_fPlayerSpeed;
		}

		if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			// 왼쪽으로 이동
			if (STATE_JUMPING != m_eUnitState)
				m_eUnitState = STATE_MOVE;
			if (STATE_SPRINT != m_eUnitState)
			{
				m_eUnitState = STATE_MOVE;
				if (UNIT_DIR_UP == m_eUnitMoveDir)
					m_eUnitMoveDir = UNIT_DIR_UP_L;
				else if (UNIT_DIR_DOWN == m_eUnitMoveDir)
					m_eUnitMoveDir = UNIT_DIR_DOWN_L;
				else
					m_eUnitMoveDir = UNIT_DIR_L;
			}

			_vec3 vRight;
			m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
			D3DXVec3Normalize(&vRight, &vRight);
			vPos += vRight * (_float)_dTimeDelta * m_fPlayerSpeed;
		}
		else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			// 오른쪽으로 이동
			if (STATE_JUMPING != m_eUnitState)
				m_eUnitState = STATE_MOVE;
			if (STATE_SPRINT != m_eUnitState)
			{
				m_eUnitState = STATE_MOVE;
				if (UNIT_DIR_UP == m_eUnitMoveDir)
					m_eUnitMoveDir = UNIT_DIR_UP_R;
				else if (UNIT_DIR_DOWN == m_eUnitMoveDir)
					m_eUnitMoveDir = UNIT_DIR_DOWN_R;
				else
					m_eUnitMoveDir = UNIT_DIR_R;
			}

			_vec3 vRight;
			m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
			D3DXVec3Normalize(&vRight, &vRight);
			vPos -= vRight * (_float)_dTimeDelta * m_fPlayerSpeed;
		}
		//////////////////////////////////////////////////////////////////////////
		/// 점프 키 입력
		if (Get_DIKeyDown(DIK_SPACE) && !m_bJump && STATE_FALLING != m_eUnitState)
		{
			// 점프 시작
			m_bJump = true;
			m_fJumpTime = 0.f;
			m_eUnitState = STATE_JUMPING;
		}
	}

	/// 점프중일 때 y 값 변경
	m_fJumpPower = 1.6f;
	if (m_bJump)
	{
		m_fJumpTime += (_float)_dTimeDelta * 0.6f;
		vPos.y = (m_fJumpPower * m_fJumpTime - m_fJumpGravity * m_fJumpTime * m_fJumpTime * 0.5f);
	}
	else
	{
		// 점프중이 아닐 때 플레이어 아래로 밀기
		vPos.y = (_float)(GRAVITY * _dTimeDelta * 2.f);
	}

	if (!Is_Invincible(m_pAttacker))
	{
		if (m_pAttacker)
			m_fKnockBack = (_float)m_pAttacker->Get_Stat(CUnit::STAT_KNOCKBACK);

		m_pAttacker = nullptr;
		m_iKnockCount = 0;
	}

	if (m_pAttacker)
	{
		_vec3 vTargetDir;
		static_cast<CTransform*>(m_pAttacker->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Info(INFO_LOOK, &vTargetDir);
		D3DXVec3Normalize(&vTargetDir, &vTargetDir);

		m_iKnockCount++;
		_float fReduce = 0.f;
		for (_int i = 0; i < m_iKnockCount; ++i)
			fReduce -= 0.005f * i;

		m_fKnockBack += fReduce;

		if (m_fKnockBack <= 0.f)
			m_fKnockBack = 0.f;

		vPos -= vTargetDir * (_float)_dTimeDelta * m_fKnockBack * m_fPlayerSpeed;
	}

	if (m_bTakeOff)
		vPos -= m_vTakeOffDir * (_float)_dTimeDelta * 5.f;
	
	//m_fJumpPower = 4.2f;					// 플레폼 확인용 점프 파워
	//if (Engine::Get_DIKeyState(DIK_F6))
	//	vPos.y += 1.f;
	//
	//if (Engine::Get_DIKeyState(DIK_F7))
	//	vPos.y -= 1.f;
	//m_fJumpPower = 4.2f;					// 플레폼 확인용 점프 파워

	if (Engine::Get_DIKeyState(DIK_F6))
		vPos.y += 1.f;
	
	if (Engine::Get_DIKeyState(DIK_F7))
		vPos.y -= 1.f;

	/// 밑바닥 충돌 체크
	PxFilterData test;
	const PxU32 flags = m_pCtrl->move(vPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters(&test));

	if (flags & PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		// 바닥과 충돌중일 때
		m_fJumpTime = 0.f;
		m_bJump = false;
		m_bTakeOff = false;

		// 떨어지는 중 착지했을 때 착지상태로 변경
		if (STATE_FALLING == m_eUnitState || STATE_JUMPING == m_eUnitState)
			m_eUnitState = STATE_LANDING;
		else if (STATE_JUMPING != m_eUnitState &&
			STATE_LANDING != m_eUnitState &&
			STATE_MOVE != m_eUnitState &&
			STATE_SPRINT != m_eUnitState &&
			STATE_INVENTORY != m_eUnitState)
		{
			m_eUnitState = STATE_IDLE;
			m_eUnitMoveDir = UNIT_DIR_CENTER;
		}
	}
	else if (STATE_JUMPING != m_eUnitState)
	{
		// 바닥에 땅이 안닿았을 때, 떨어지는중 상태
		m_eUnitState = STATE_FALLING;
	}
}

void CPlayer::Key_Input_Interact(const _double & _dTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_E) & 0x80)
	{
		m_bPressE = true;
		m_dPressETime += _dTimeDelta;
	}
	else
		m_dPressETime = 0.0;
	
	// 링커맨드 사용하는 레이어들
	if (L"Layer_BasketballHoop" == m_wstrTargetLayer || L"Layer_Dino_Tamed" == m_wstrTargetLayer)
	{
		if (m_dPressETime >= 0.5)
		{
			m_bPressE = false;
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_FixMouseCursor(false);
			m_pRingUnit = static_cast<CUnit*>(m_pLookObject);
			m_pRingUnit->Open_Ring_Command(static_cast<CUnit*>(m_pRingUnit));
		}
		// 상호작용 (E)
		else if (m_bPressE && m_dPressETime == 0.0)
		{
			m_bPressE = false;
			if (m_pRingUnit)
			{
				static_cast<CUnit*>(m_pRingUnit)->Init_Ring_Command();
				m_pRingUnit = nullptr;
				Close_Ring_Command();
			}

			if (L"Layer_Dino_Tamed" == m_wstrTargetLayer)
			{
				// 현재 보고있는 공룡 포인터가 있을 경우 삭제
				if (nullptr != m_pDino)
					Safe_Release(m_pDino);

				// 공룡 탐색
				m_pDino = static_cast<CDino*>(m_pLookObject);

				// 탑승할 공룡이 있을 경우 탑승
				if (nullptr != m_pDino)
				{
					m_pDino->AddRef();

					if (m_pDino->Get_CanbeRide() && !m_bTakeOff)
					{
						m_pDino->Set_IsRide(true);

						m_eUnitState = STATE_RIDE;

						m_wstrLeftHand = L"";
						Swap_Weapon();
						Switch_Camera(1);

						static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2);
						//static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVIsPlayerShoulderView(false);

						//오른쪽 위에 탑승한 공룡 체력 등등 키기
						for (_int i = 0; i < (_int)CSummaryStat_UI_Monster::SummaryStat_END; ++i)
							static_cast<CSummaryStat_UI_Monster*>(Engine::Get_GameObject(L"SummaryStat_UI", ((_int)CSummaryStat_UI::SummaryStat_END) * 2 + (i)))->Set_RideMonster(static_cast<CUnit*>(m_pLookObject));
					}
					// 유아기 공룡들 요구사항 들어주기
					else if (m_pDino->Get_IsBaby())
					{
						// 처음 이름 각인 시키기
						if (!m_pDino->Get_IsNaming())
							static_cast<CTamingName_UI*>(Engine::Get_GameObject(L"Taming_Name_UI", 0))->Making_a_Name(m_pDino, m_pDino->Get_DinoSpeciesKor());

						if (m_pDino->Get_TakeCare())
						{
							if (m_pDino->Get_TakeCareNum() == CDino::FOOD)
							{
								CGameObject* pQuickSlot = Engine::Get_GameObject(L"QuickSlot_UI");
								ITEM_INFO tPickItemInfo = static_cast<CQuickSlot_UI*>(pQuickSlot)->Get_Item_To_Number(9);
								if (tPickItemInfo.iItemID == ITEMID_Meat)
								{
									static_cast<CQuickSlot_UI*>(pQuickSlot)->UisngRemove_Item(9, 1);
									m_pDino->Set_TakeCare(true);
								}
							}
							else if (m_pDino->Get_TakeCareNum() == CDino::HUG)
								m_pDino->Set_TakeCare(true);
						}
					}
					else if (L"Dodo" == m_pDino->Get_DinoSpecies() && !m_bGrabObejct)
					{
						m_bGrabObejct = true;
						m_pDino->Set_Hanging(true);
						const D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("c_R_weapon_jnt");
						m_pDino->Set_HangingMatrix(&(*pFrame->CombinedTransformMatrix * m_pTransformCom->Get_WorldMatrix()));
					}
				}
			}
		}
	}
	else if(m_bPressE)
	{
		m_bPressE = false;
		if (L"Layer_Harvest_Bush" == m_wstrTargetLayer ||
			L"Layer_Harvest_Pebble" == m_wstrTargetLayer)
		{
			/// 채집
			// 맨손일 경우에만 채집 가능
			if (!m_bIsGrapLeftHand && !static_cast<CUnit*>(m_pLookObject)->Get_Dying())
			{
				// 채집 시작
				m_eUpperPlayerState = STATE_HARVEST;
			}
			else
			{
				// 메세지 출력 (맨손으로만 채집이 가능합니다.)
			}
		}
		else if(L"Layer_Ball" == m_wstrTargetLayer && !m_bGrabBall)
		{
			if (nullptr != m_pDino)
				Safe_Release(m_pDino);

			// 공룡 탐색
			m_pDino = static_cast<CDino*>(m_pLookObject);
			m_pDino->AddRef();

			m_bGrabBall = true;

			m_pDino->Set_Hanging(true);
			m_pDino->Set_HangingMatrix(&m_pTransformCom->Get_WorldMatrix());
		}
		// 평화테이밍하는 애
		else if (L"Layer_Dino" == m_wstrTargetLayer)
		{
			CDino* pDino =  static_cast<CDino*>(m_pLookObject);

			if (L"Ankylo" == pDino->Get_DinoSpecies() && !pDino->Get_PeaceTaming())
			{
				CGameObject* pQuickSlot = Engine::Get_GameObject(L"QuickSlot_UI");
				ITEM_INFO tPickItemInfo = static_cast<CQuickSlot_UI*>(pQuickSlot)->Get_Item_To_Number(9);
				if (tPickItemInfo.iItemID == ITEMID_AmarBerry)
				{
					static_cast<CQuickSlot_UI*>(pQuickSlot)->UisngRemove_Item(9, 1);
					pDino->Set_PeaceTaming(true);
				}
			}
		}
		//m_eUpperPlayerState = STATE_ATCTIVATECONSOLE;
	}
}

void CPlayer::Key_Input_Whistle()
{
	// 특정 상태일 때 바로 리턴
	if (STATE_ATTACK == m_eUpperPlayerState ||
		STATE_EAT == m_eUpperPlayerState ||
		STATE_HARVEST == m_eUpperPlayerState ||
		STATE_INVENTORY == m_eUpperPlayerState ||
		STATE_ATCTIVATECONSOLE == m_eUpperPlayerState ||
		STATE_WHISTLE == m_eUpperPlayerState ||
		STATE_WEAPON_DRAW == m_eUpperPlayerState ||
		STATE_WEAPON_PUTAWAY == m_eUpperPlayerState)
		return;

	/// 휘파발 상호작용
	if (Engine::Get_DIKeyDown(DIK_T))
	{
		// 공룡 따라오게 하기
		if (L"Layer_Dino_Tamed" == m_wstrTargetLayer)
		{
			static_cast<CDino*>(m_pLookObject)->Set_Follow(true);
			static_cast<CDino*>(m_pLookObject)->Set_Resistance(true);
		}

		if (!m_bIsRide)
		{
			m_eUpperPlayerState = STATE_WHISTLE;
			m_ePlayerWhistle = P_WHISTLE_NORMAL;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_Y))
	{
		// 공룡 따라오지 않게 하기
		if (L"Layer_Dino_Tamed" == m_wstrTargetLayer)
			static_cast<CDino*>(m_pLookObject)->Set_Follow(false);

		if (!m_bIsRide)
		{
			m_eUpperPlayerState = STATE_WHISTLE;
			m_ePlayerWhistle = P_WHISTLE_NORMAL;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_SEMICOLON))
	{
		// 저항안함
		list<CGameObject*>* listGameObject = Engine::Get_LayerList(L"Layer_Dino_Tamed");

		if (listGameObject->empty())
			return;

		for (auto& Dino : *listGameObject)
			static_cast<CDino*>(Dino)->Set_Resistance(false);

		if (!m_bIsRide)
		{
			m_eUpperPlayerState = STATE_WHISTLE;
			m_ePlayerWhistle = P_WHISTLE_NORMAL;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_J))
	{
		// 사거리안에 공룡 전부 부르기
		list<CGameObject*>* listGameObject = Engine::Get_LayerList(L"Layer_Dino_Tamed");

		if (listGameObject->empty())
			return;

		for (auto& Dino : *listGameObject)
		{
			CDino* pDino = static_cast<CDino*>(Dino);

			if (!pDino->Get_GropWhitle())
				continue;

			pDino->Set_Follow(true);
			pDino->Set_Resistance(true);
		}

		if (!m_bIsRide)
		{
			m_eUpperPlayerState = STATE_WHISTLE;
			m_ePlayerWhistle = P_WHISTLE_NORMAL;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_U))
	{
		// 사거리안에 공룡 전부 멈추기
		list<CGameObject*>* listGameObject = Engine::Get_LayerList(L"Layer_Dino_Tamed");

		if (listGameObject->empty())
			return;

		for (auto& Dino : *listGameObject)
		{
			CDino* pDino = static_cast<CDino*>(Dino);

			if (!pDino->Get_GropWhitle())
				continue;

			pDino->Set_Follow(false);
		}

		if (!m_bIsRide)
		{
			m_eUpperPlayerState = STATE_WHISTLE;
			m_ePlayerWhistle = P_WHISTLE_NORMAL;
		}
	}

	if (Engine::Get_DIKeyState(DIK_PERIOD) & 0x80)
	{
		m_bAttackCrossHair = true;
		// 크로스헤어온
	}
	else if(m_bAttackCrossHair)
	{
		//해제
		m_bAttackCrossHair = false;

		//////////////////////////////////////////////////////////////////////////
		CGameObject* pLookObject = nullptr;

		if (pLookObject = Ray_PlayerLook(L"Layer_Dino", 50.f))
		{
			//pLookObject->AddRef();
			list<CGameObject*>* listGameObject = Engine::Get_LayerList(L"Layer_Dino_Tamed");

			if (listGameObject->empty())
				return;

			for (auto& Dino : *listGameObject)
			{
				_bool bFollow = static_cast<CDino*>(Dino)->Get_Follow();

				if (bFollow)
				{
					static_cast<CDino*>(Dino)->Attack_OnTarget(pLookObject);
				}
			}

			CGameObject* pGameObject = nullptr;

			pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_EffectObject");
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), );
			pGameObject->Set_Target(pLookObject);

			pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_EffectObjectRev");
			NULL_CHECK_RETURN(pGameObject, );
			pGameObject->Set_Target(pLookObject);
			FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), );

			pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_EffectObjectArrow");
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), );
			pGameObject->Set_Target(pLookObject);
		}
		if (!m_bIsRide)
		{
			m_eUpperPlayerState = STATE_WHISTLE;
			m_ePlayerWhistle = P_WHISTLE_ATTACK;
		}
	}

	if (Engine::Get_DIKeyState(DIK_COMMA) & 0x80)
	{
		m_bMoveCrossHair = true;
		// 사거리안에 공룡 지정한 위치로 이동 (미완성)
		
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
	}
	else if (m_bMoveCrossHair)
	{
		m_bMoveCrossHair = false;

		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2);
		// 플레이어과 카메라 사이의 거리 가져오기
		_float fCamDis = static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Get_TPVCam_Dis();

		// 뷰 역행렬 가져오기
		_mat matView;
		m_pDevice->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, NULL, &matView);

		// 피킹 시작할 포스와 방향 만들기
		_vec3 vCamPos, vCamLook;
		memcpy(&vCamPos, &matView.m[INFO_POS][0], sizeof(_vec3));
		memcpy(&vCamLook, &matView.m[INFO_LOOK][0], sizeof(_vec3));
		D3DXVec3Normalize(&vCamLook, &vCamLook);

		vCamPos += vCamLook * fCamDis;

		PxVec3 vRayPos = PxVec3(vCamPos.x, vCamPos.y, vCamPos.z);
		PxVec3 vRayDir = PxVec3(vCamLook.x, vCamLook.y, vCamLook.z);

		_float vOutPickDis;
		_vec3 vOutPickPos;
		if (CPhysX_Mgr::GetInstance()->RayCast(vRayPos, vRayDir, 40.f, &vOutPickDis, &vOutPickPos))
		{
			list<CGameObject*>* listGameObject = Engine::Get_LayerList(L"Layer_Dino_Tamed");

			if (listGameObject->empty())
				return;

			for (auto& Dino : *listGameObject)
			{
				CDino* pDino = static_cast<CDino*>(Dino);

				if (!pDino->Get_GropWhitle())
					continue;

				static_cast<CDino*>(Dino)->Set_FixedMove(true, vOutPickPos);
			}
		}

		//CGameObject* pGameObject = nullptr;

		//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_EffectObject");
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), );
		//pGameObject->Set_Target(pLookObject);

		//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_EffectObjectRev");
		//NULL_CHECK_RETURN(pGameObject, );
		//pGameObject->Set_Target(pLookObject);
		//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), );

		//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_EffectObjectArrow");
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), );
		//pGameObject->Set_Target(pLookObject);

		if (!m_bIsRide)
		{
			m_eUpperPlayerState = STATE_WHISTLE;
			m_ePlayerWhistle = P_WHISTLE_MOVE;
		}
	}
}

void CPlayer::Key_Input_Inventory(const _double & _dTimeDelta)
{
	if (Engine::Get_DIKeyDown(DIK_ESCAPE))
	{
		if (m_pRingUnit)
		{
			static_cast<CUnit*>(m_pRingUnit)->Init_Ring_Command();
			m_pRingUnit = nullptr;
			Close_Ring_Command();
		}

		Close_Inventory();
	}

	// 인벤토리 (I)
	if (Engine::Get_DIKeyDown(DIK_I))
	{
		if (!m_bIsOpenInventory)
		{
			if (m_bIsRide)
			{
				Open_Inventory();
			}
			else
			{
				m_eUpperPlayerState = STATE_INVENTORY;
				m_dOpenDelayInven = 0.0;
			}
		}
		else
		{
			Close_Inventory();
		}

	}
	else if (Engine::Get_DIKeyDown(DIK_F))
	{
		if (m_bIsRide)
			return;

		// 대상 인벤토리랑 같이 열기
		if (!m_bIsOpenInventory)
		{
			if (L"Layer_Dino" == m_wstrTargetLayer && m_pLookObject)
			{
				// 공룡은 기절 중이거나 테이밍 상태일 때만 인벤토리 열 수 있도록
				if (static_cast<CDino*>(m_pLookObject)->Get_NowTamingState() ||
					static_cast<CDino*>(m_pLookObject)->Get_NowTorpidState())
				{
					m_eUpperPlayerState = STATE_ATCTIVATECONSOLE;
					m_pTargetInvecObject = m_pLookObject;
					m_dUpperAnimationSpeed = 1.5;
				}
			}
			else if (L"Layer_Dino_Tamed" == m_wstrTargetLayer ||
				L"Layer_Structure" == m_wstrTargetLayer)
			{
				if (m_pLookObject)
				{
					m_eUpperPlayerState = STATE_ATCTIVATECONSOLE;
					m_pTargetInvecObject = m_pLookObject;
					m_dUpperAnimationSpeed = 1.5;
				}
			}
		}
		else
		{
			Close_Inventory();
		}
	}
}

void CPlayer::Key_Input_Attack(const _double & _dTimeDelta)
{
	if (!m_bIsBuilding &&
		(P_POSE_STAND == m_ePlayerPose) &&
		(STATE_ATTACK != m_eUpperPlayerState ||
			STATE_EAT != m_eUpperPlayerState ||
			STATE_HARVEST != m_eUpperPlayerState ||
			STATE_INVENTORY != m_eUpperPlayerState ||
			STATE_WEAPON_DRAW != m_eUpperPlayerState ||
			STATE_WEAPON_PUTAWAY != m_eUpperPlayerState ||
			STATE_WEAPON_THROW != m_eUpperPlayerState ||
			STATE_WEAPON_PULLBACK != m_eUpperPlayerState))
	{
		//if (L"Layer_Weapon_Bola" == m_wstrLeftHand)
		if (P_STANCE_BOAL == m_ePlayerStance || P_STANCE_BOW == m_ePlayerStance)
		{
			if (STATE_WEAPON_THROW != m_eUpperPlayerState && Engine::Get_DIMouseState(DIM_LB) & 0x80)
			{
				// 던지기 시작
				if (!m_bIsFullBack)
				{
					m_dUpperAnimationSpeed = 2.f;
					Switch_Camera_TPVToFPV(0);
					//static_cast<CTPV_Camera*>(Engine::Get_GameObject(L"Layer_Camera_TPV"))->Set_MoveFov(40.f, 0.2);

					if (P_STANCE_BOAL == m_ePlayerStance)
					{
						CWeapon* pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(L"Layer_Weapon_Bola"));
						if (pWeapon) pWeapon->Set_WeaponYawPitchRoll(180.f, 0.f, 90.f);
					}
				}
				else
				{
					// 던지기 준비
					m_fThrowPower += (_float)_dTimeDelta;

					if (2.f < m_fThrowPower)
						m_fThrowPower = 2.f;
				}

				m_eUpperPlayerState = STATE_WEAPON_PULLBACK;
				m_bIsFullBack = true;

			}
			else if (m_bIsFullBack)
			{
				// 던지기
				m_dUpperAnimationSpeed = 0.8;

				Switch_Camera_TPVToFPV(1);
				//static_cast<CTPV_Camera*>(Engine::Get_GameObject(L"Layer_Camera_TPV"))->Set_MoveFov(60.f, 0.4);

				m_eUpperPlayerState = STATE_WEAPON_THROW;

				// 화살 발사
				if (P_STANCE_BOW == m_ePlayerStance)
					Shot_Bullet(BULLET_ARROW);

				m_bIsFullBack = false;
			}
		}
		else if (Engine::Get_BIMouseDown(DIM_LB))
		{
			m_eUpperPlayerState = STATE_ATTACK;

			//////////////////////////////////////////////////////////////////////////
			// 농구 테스트
			if (m_bGrabBall)
			{
				if (m_pDino)
				{
					m_bGrabBall = false;
					static_cast<CDodo_Ball*>(m_pDino)->Throw_Ball();
					Safe_Release(m_pDino);
					m_pDino = nullptr;
				}
			}
			else if (m_bGrabObejct)
			{
				if (m_pDino)
				{
					m_bGrabObejct = false;
					static_cast<CDino*>(m_pDino)->Drop();
					Safe_Release(m_pDino);
					m_pDino = nullptr;
				}
			}
		}
	}
}

void CPlayer::Key_Input_QuickSlot(const _double & _dTimeDelta)
{
	if (STATE_ATTACK == m_eUpperPlayerState ||
		STATE_WEAPON_DRAW == m_eUpperPlayerState ||
		STATE_WEAPON_PUTAWAY == m_eUpperPlayerState ||
		STATE_WEAPON_THROW == m_eUpperPlayerState ||
		STATE_WEAPON_PULLBACK == m_eUpperPlayerState)
		return;

	CGameObject* pQuickSlot = Engine::Get_GameObject(L"QuickSlot_UI");

	// 맨손으로 전환
	if (Engine::Get_DIKeyDown(DIK_Q))
	{
		if (99 != m_iQuickSlot)
		{
			m_iQuickSlot = 99;

			m_wstrLeftHand = L"";
			m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;

			if (!m_bIsFPVCam)
				static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2);
			return;
		}
		else
			return;
	}

	// 퀵슬롯 선택
	_uint iPreQuickSlot = m_iQuickSlot;

	if (Engine::Get_DIKeyDown(DIK_1))
	{
		if (0 != m_iQuickSlot)
		{
			m_iQuickSlot = 0;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_2))
	{

		if (1 != m_iQuickSlot)
		{
			m_iQuickSlot = 1;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_3))
	{
		if (2 != m_iQuickSlot)
		{
			m_iQuickSlot = 2;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_4))
	{
		if (3 != m_iQuickSlot)
		{
			m_iQuickSlot = 3;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_5))
	{
		if (4 != m_iQuickSlot)
		{
			m_iQuickSlot = 4;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_6))
	{
		if (5 != m_iQuickSlot)
		{
			m_iQuickSlot = 5;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_7))
	{
		if (6 != m_iQuickSlot)
		{
			m_iQuickSlot = 6;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_8))
	{
		if (7 != m_iQuickSlot)
		{
			m_iQuickSlot = 7;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_9))
	{
		if (8 != m_iQuickSlot)
		{
			m_iQuickSlot = 8;
		}
	}
	else if (Engine::Get_DIKeyDown(DIK_0))
	{
		if (9 != m_iQuickSlot)
		{
			m_iQuickSlot = 9;
		}
	}
	else
		return;

	/// 여기서 부터 퀵슬롯이 눌렸을 때
	// 빌딩중일 경우 빌딩 취소
	if (m_bIsBuilding)
	{
		Is_BuildingCancle();
		if (iPreQuickSlot == m_iQuickSlot)
			return;
	}

	// 현재 선택된 퀵슬롯 아이탬 정보 가져오기
	ITEM_INFO tPickItemInfo = static_cast<CQuickSlot_UI*>(pQuickSlot)->Get_Item_To_Number(m_iQuickSlot);


	if (NULL == tPickItemInfo.tchName)
	{
		if (L"" == m_wstrLeftHand)
			return;
		else
		{
			m_wstrLeftHand = L"";
			m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;

			return;
		}
	}

	switch (tPickItemInfo.iItemID)
	{
	case ITEMID_StoneAx:
		if (L"Layer_Weapon_Hatchet" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_Hatchet";		// 돌도끼
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;
	case ITEMID_StonePick:
		if (L"Layer_Weapon_StonePick" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_StonePick";		// 돌곡괭이
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;
	case ITEMID_Club:
		if (L"Layer_Weapon_WoodClub" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_WoodClub";		// 나무방망이
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;
	case ITEMID_Spear:
		if (L"Layer_Weapon_Spear" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_Spear";			// 투창
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;
	case ITEMID_Bolas:
		if (L"Layer_Weapon_Bola" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_Bola";			// 볼라
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;
	case ITEMID_Bow:
		if (L"Layer_Weapon_Bow" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_Bow";			// 활
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;

		// 먹을거
	case ITEMID_Meat:
	case ITEMID_CookedMeat:
	case ITEMID_AmarBerry:
		Is_EatItem(tPickItemInfo);				// 해당 아이탬 먹기
		static_cast<CQuickSlot_UI*>(pQuickSlot)->UisngRemove_Item(m_iQuickSlot, 1);
		break;

		/// 설치물
	case ITEMID_Campfire:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Campfire";
		m_eBuilding = STRUCTURE_CAMPFIRE;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.002f;
		break;
	case ITEMID_SleepingBag:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_SleepingBag";
		m_eBuilding = STRUCTURE_SLEEPINGBAG;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_MortarPestle:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_MortarPestal";
		m_eBuilding = STRUCTURE_MORTARPESTAL;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.001f;
		break;
	case ITEMID_Forge:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Forge";
		m_eBuilding = STRUCTURE_FORGE;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_Smithy:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Smithy";
		m_eBuilding = STRUCTURE_SMITHY;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_ThatchBed:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_ThatchBed";
		m_eBuilding = STRUCTURE_THATCHBED;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_WallTorch:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_WallTorch";
		m_eBuilding = STRUCTURE_WALLTORCH;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.001f;
		break;
	case ITEMID_WaterWell:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_WaterWell";
		m_eBuilding = STRUCTURE_WATERWELL;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.006f;
		break;
	case ITEMID_Barricade:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Barricade";
		m_eBuilding = STRUCTURE_BARRICADE;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_CompostBin:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_CompostBin";
		m_eBuilding = STRUCTURE_COMPOSTBIN;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_CropPlot:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_CropPlot";
		m_eBuilding = STRUCTURE_CROPPLOT;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_FeedingTrough:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_FeedingTrough";
		m_eBuilding = STRUCTURE_FEEDINGTROUGH;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.002f;
		break;
	case ITEMID_Fireplace:
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Fireplace";
		m_eBuilding = STRUCTURE_FIREPLACE;
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_S_Storage:
		m_eBuilding = STRUCTURE_S_STORAGE;
	case ITEMID_L_Storage:
		if (STRUCTURE_END == m_eBuilding)
			m_eBuilding = STRUCTURE_L_STORAGE;
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Storage";
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_CookingCampFire:
		m_eBuilding = STRUCTURE_COOKINGCAMPFIRE;
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_CookingCampFire";
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;

		/// 건축물
	case ITEMID_ThatchFloor:
		m_eBuilding = STRUCTURE_THATCH_FLOOR;
	case ITEMID_StoneFloor:
		if(STRUCTURE_END == m_eBuilding)
			m_eBuilding = STRUCTURE_STONE_FLOOR;
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Thatch_Floor";
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.006f;
		break;
	case ITEMID_ThatchWall:
		m_eBuilding = STRUCTURE_THATCH_WALL;
	case ITEMID_StoneWall:
		if (STRUCTURE_END == m_eBuilding)
			m_eBuilding = STRUCTURE_STONE_WALL;
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Thatch_Wall";
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.004f;
		break;
	case ITEMID_ThatchRoof:
		m_eBuilding = STRUCTURE_THATCH_ROOF;
	case ITEMID_StoneRoof:		
		if (STRUCTURE_END == m_eBuilding)
			m_eBuilding = STRUCTURE_STONE_ROOF;
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Thatch_Roof";
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.006f;
		break;
	case ITEMID_ThatchDoorFrame:
		m_eBuilding = STRUCTURE_THATCH_DOORFRAME;
	case ITEMID_StoneDoorFrame:
		if (STRUCTURE_END == m_eBuilding)
			m_eBuilding = STRUCTURE_STONE_DOORFRAME;
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Thatch_DoorFrame";
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_ThatchDoor:
		m_eBuilding = STRUCTURE_THATCH_DOOR;
	case ITEMID_StoneDoor:		
		if (STRUCTURE_END == m_eBuilding)
			m_eBuilding = STRUCTURE_STONE_DOOR;
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_Thatch_Door";
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	case ITEMID_StoneGateFrame:
		m_eBuilding = STRUCTURE_STONE_GATEFRAME;
		m_bIsBuilding = true;
		m_wstrPreviewStructure = L"Proto_Obj_GateFrame";
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		m_fStructureDustScale = 0.003f;
		break;
	default:
		break;
	}
}

void CPlayer::Key_Input_MouseMove(const _double & _dTimeDelta)
{
	if (m_bIsLockCamera)
		return;

	_long	dwMouseMoveY, dwMouseMoveX, dwMouseMoveZ;
	dwMouseMoveY = dwMouseMoveX = dwMouseMoveZ = 0;

	// 카메라 상하 움직임
	if (dwMouseMoveY = Get_DIMouseMove(DIMS_Y))
	{
		m_fAngleX -= dwMouseMoveY/* * (_float)_dTimeDelta*/ * 0.05f;

		// 상하 각도 제한
		if (-80.f >= m_fAngleX)
			m_fAngleX = -80.f;
		else if (80.f <= m_fAngleX)
			m_fAngleX = 80.f;
	}

	if (dwMouseMoveX = Get_DIMouseMove(DIMS_X))
	{
		m_fAngleY += dwMouseMoveX/* * (_float)_dTimeDelta*/ * 0.05f;

	}

	// 좌우 각도 초기화
	if (-360.f >= m_fAngleY)
		m_fAngleY += 360.f;
	else if (360.f < m_fAngleY)
		m_fAngleY -= 360.f;


	/// 플레이어 회전
	// 공룡 탑승중일 때 회전 제한
	if (!m_bIsRide)
	{
		//_vec3 vCamLook = m_vTPVCamLook;
		//vCamLook.y = 0.f;
		//D3DXVec3Normalize(&vCamLook, &vCamLook);

		//_float fRatio = D3DXVec3Dot(&_vec3(0.f, 0.f, -1.f), &vCamLook);
		//_float fRdian = acosf(fRatio);
		//
		//if (0.f < vCamLook.x)
		//	fRdian *= -1.f;

		// 플레이어 회전
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_fAngleY));
		m_pTransformFPVCom->Set_Rotate(ROT_Y, D3DXToRadian(m_fAngleY));
		m_pTransformFPVCom->Set_Rotate(ROT_X, D3DXToRadian(m_fAngleX));
	}



	// 1,3 인칭 변경
	if (dwMouseMoveZ = Get_DIMouseMove(DIMS_Z))
	{
		if (0 > dwMouseMoveZ)
		{
			// 3인칭으로 변경
			if (m_bIsFPVCam)
			{
				Switch_Camera(1);
			}
		}
		else if (0 < dwMouseMoveZ)
		{
			// 1인칭으로 변경
			if (!m_bIsFPVCam)
			{
				Switch_Camera(0);

				//if (L"" != m_wstrLeftHand)
				//{
				//	CWeapon* pWeapon = nullptr;
				//	pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrPreLeftHand, 0));
				//	if (pWeapon)
				//		pWeapon->Set_FPVPlayer(m_bIsFPVCam);
				//}
			}
		}
	}
}

void CPlayer::Shot_Bullet(BULLET_TYPE _eBulletType)
{
	BULLET_DESC tBulletInfo;
	ZeroMemory(&tBulletInfo, sizeof(BULLET_DESC));
	_mat matWorld = m_pTransformFPVCom->Get_WorldMatrix();

	_vec3 vLook;

	tBulletInfo.fAngleX = m_fAngleX;
	tBulletInfo.fAngleY = m_fAngleY;

	switch (_eBulletType)
	{
	case BULLET_BOLA:
		tBulletInfo.eBulletType = BULLET_BOLA;

		memcpy(&tBulletInfo.vPos, &matWorld.m[INFO_POS][0], sizeof(_vec3));
		memcpy(&vLook, &matWorld.m[INFO_LOOK][0], sizeof(_vec3));
		D3DXVec3Normalize(&vLook, &vLook);

		tBulletInfo.vPos -= vLook * 0.5f;

		tBulletInfo.fGravity = 0.5f / m_fThrowPower;
		tBulletInfo.fSpeed = 5.f * m_fThrowPower;
		m_fThrowPower = 1.f;
		break;
	case BULLET_SPEAR:
		break;
	case BULLET_ARROW_TORPID:
		tBulletInfo.eBulletType = BULLET_ARROW_TORPID;
	case BULLET_ARROW:
		if (BULLET_ARROW_TORPID != tBulletInfo.eBulletType)
			tBulletInfo.eBulletType = BULLET_ARROW;

		memcpy(&tBulletInfo.vPos, &matWorld.m[INFO_POS][0], sizeof(_vec3));
		memcpy(&vLook, &matWorld.m[INFO_LOOK][0], sizeof(_vec3));
		D3DXVec3Normalize(&vLook, &vLook);

		tBulletInfo.vPos -= vLook * 0.5f;

		tBulletInfo.fGravity = 0.5f / m_fThrowPower;
		tBulletInfo.fSpeed = 10.0f * m_fThrowPower;
		m_fThrowPower = 1.f;
		break;
	default:
		return;
	}

	CGameObject* pBullet = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Bullet", &tBulletInfo);
	Engine::Add_GameObjectInLayer(L"Layer_Bullet", pBullet);
}

void CPlayer::IsRide_Key_Input(const _double & _dTimeDelta)
{
	// 마우스 무브 (카메라 및 플레이어 각도)
	Key_Input_MouseMove(_dTimeDelta);

	/// 공룡타기
	if (m_bIsOpenInventory)
		return;

	if (m_pDino->Get_Dying())
	{
		_vec3 vPos;
		_mat matSaddle = m_pDino->Get_SaddleMat();
		memcpy(&vPos, &matSaddle.m[INFO_POS][0], sizeof(_vec3));
		m_pCtrl->setPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
		m_pTransformCom->Set_Pos(vPos);
		m_vPlayerPos = vPos;
		m_pTransformCom->Update_Component(0.f);

		m_pDino->Set_IsRide(false);
		Safe_Release(m_pDino);
		m_pDino = nullptr;
		m_bIsRide = false;

		m_eUnitState = STATE_IDLE;

		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Set_TPVCam_MaxDis(MAXCAMDIS);

		return;
	}

	// 공룡 움직이기
	// 랩터 무브
	if (m_pDino->IsRide_Key_Input(_dTimeDelta, m_wstrTargetLayer, m_pLookObject))
	{
		if (L"Rex" == m_pDino->Get_DinoSpecies() || L"Stego" == m_pDino->Get_DinoSpecies())
			m_eAimation = P_A_RIDE_REX_MOVE;
		else if (L"Rap" == m_pDino->Get_DinoSpecies())
			m_eAimation = P_A_RIDE_MOVE;
		else if (L"Yutyra" == m_pDino->Get_DinoSpecies())
			m_eAimation = P_A_RIDE_WIED_MOVE;
		else
			m_eAimation = P_A_RIDE_PARA_MOVE;
	}
	else
	{
		if (L"Rex" == m_pDino->Get_DinoSpecies() || L"Stego" == m_pDino->Get_DinoSpecies())
			m_eAimation = P_A_RIDE_REX_IDLE;
		else if (L"Rap" == m_pDino->Get_DinoSpecies())
			m_eAimation = P_A_RIDE_IDLE;
		else if (L"Yutyra" == m_pDino->Get_DinoSpecies())
			m_eAimation = P_A_RIDE_WIED_IDLE;
		else
			m_eAimation = P_A_RIDE_PARA_IDLE;
	}

	// 상호작용 (E) 공룡 내리기
	if (Engine::Get_DIKeyDown(DIK_E))
	{
		if (m_pDino)
		{
			_vec3 vPos;
			_mat matSaddle = m_pDino->Get_SaddleMat();
			memcpy(&vPos, &matSaddle.m[INFO_POS][0], sizeof(_vec3));

			m_pTransformCom->Set_Pos(vPos);
			m_vPlayerPos = vPos;
			m_pCtrl->setPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
			m_pTransformCom->Update_Component(0.f);

			memcpy(&m_vTakeOffDir, &matSaddle.m[INFO_RIGHT][0], sizeof(_vec3));
			D3DXVec3Normalize(&m_vTakeOffDir, &m_vTakeOffDir);
			m_bJump = true;
			m_bTakeOff = true;

			m_pDino->Set_IsRide(false);
			Safe_Release(m_pDino);
			m_pDino = nullptr;
			m_bIsRide = false;

			m_eUnitState = STATE_IDLE;

			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Set_TPVCam_MaxDis(MAXCAMDIS);

			//오른쪽 위에 탑승한 공룡 체력 등등 끄기
			for (_int i = 0; i < (_int)CSummaryStat_UI_Monster::SummaryStat_END; ++i)
				static_cast<CSummaryStat_UI_Monster*>(Engine::Get_GameObject(L"SummaryStat_UI", ((_int)CSummaryStat_UI::SummaryStat_END) * 2 + (i)))->Set_RideMonster(nullptr);
		}
	}

	// 휘파람 액션
	Key_Input_Whistle();

}

void CPlayer::IsRide_SetPos()
{
	if (nullptr != m_pDino && m_bIsRide)
	{
		_mat matSaddle = m_pDino->Get_SaddleMat();
		m_pTransformCom->Set_WorldMatrix(&matSaddle);

		_vec3 vPos;
		memcpy(&vPos, &matSaddle.m[INFO_POS][0], sizeof(_vec3));
		m_pTransformCom->Set_Pos(vPos);
		m_pTransformFPVCom->Set_Pos(vPos);

		m_pCtrl->setPosition(PxExtendedVec3(0.f, 0.f, 0.f));
	}
}

_bool CPlayer::Swap_Weapon()
{
	if (m_bIsGrapLeftHand)
	{
		CWeapon* pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrPreLeftHand, 0));

		// 능력치 반환
		m_tStat.fAtk -= pWeapon->Get_WeaponAtt();
		m_tStat.fTorpidRate = 0.2f;

		// 기존 아이탬 삭제
		pWeapon->Set_Dead();
		m_bIsGrapLeftHand = false;
		m_eUpperPlayerState = STATE_IDLE;
		m_ePlayerStance = P_STANCE_BASIC;

		ZeroMemory(m_bIsFPVSubsetRender, sizeof(_bool)*m_pMeshFPVCom->Get_MeshList()->size());
		m_bIsFPVSubsetRender[0] = true;
	}

	WEAPON_TYPE		eWeaponType = WEAPON_END;


	// 다음 거낼 무기가 있을경우 무기 스폰
	if (L"" != m_wstrLeftHand)
	{
		if (L"Layer_Weapon_Hatchet" == m_wstrLeftHand)
		{
			eWeaponType = WEAPON_HATCHET;
			m_ePlayerStance = P_STANCE_HATCHET;

		}
		else if (L"Layer_Weapon_StonePick" == m_wstrLeftHand)
		{
			eWeaponType = WEAPON_STONEPICK;
			m_ePlayerStance = P_STANCE_HATCHET;

		}
		else if (L"Layer_Weapon_WoodClub" == m_wstrLeftHand)
		{
			eWeaponType = WEAPON_WOODCLUB;
			m_ePlayerStance = P_STANCE_HATCHET;
		}
		else if (L"Layer_Weapon_Spear" == m_wstrLeftHand)
		{
			eWeaponType = WEAPON_SPEAR;
			m_ePlayerStance = P_STANCE_SPEAR;
		}
		else if (L"Layer_Weapon_Bola" == m_wstrLeftHand)
		{
			eWeaponType = WEAPON_BOLA;
			m_ePlayerStance = P_STANCE_BOAL;
			m_bIsFPVSubsetRender[1] = true;
		}
		else if (L"Layer_Weapon_Bow" == m_wstrLeftHand)
		{
			eWeaponType = WEAPON_BOW;
			m_ePlayerStance = P_STANCE_BOW;
			m_bIsFPVSubsetRender[2] = true;
		}

		m_bIsGrapLeftHand = true;
		m_eUpperPlayerState = STATE_WEAPON_DRAW;

		CGameObject* pWeapon = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Weapon", &eWeaponType);

		//_mat matParent = m_pMeshCom->Get_FrameByName("c_R_attatch_jnt")->CombinedTransformMatrix;

		if (FAILED(Engine::Add_GameObjectInLayer(m_wstrLeftHand, pWeapon)))
		{
			ERR_MSG(L"Failed to Create Weapon !!!");
			Safe_Release(pWeapon);
			m_bIsGrapLeftHand = false;
			m_eUpperPlayerState = STATE_IDLE;
			m_ePlayerStance = P_STANCE_BASIC;

			m_wstrLeftHand = L"";
			m_wstrPreLeftHand = L"";

			return false;
		}

		m_wstrPreLeftHand = m_wstrLeftHand;

		// 무기 능력치만큼 증가
		m_tStat.fAtk += static_cast<CWeapon*>(pWeapon)->Get_WeaponAtt();
		m_tStat.fTorpidRate = static_cast<CWeapon*>(pWeapon)->Get_WeaponTorpid();
		static_cast<CWeapon*>(pWeapon)->Set_FPVPlayer(m_bIsFPVCam);

		// 무기에 따라서 카메라 시점 변경
		if (L"Layer_Weapon_Bola" == m_wstrLeftHand ||
			L"Layer_Weapon_Bow" == m_wstrLeftHand)
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
		else
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2);

		return true;
	}


	return false;
}

void CPlayer::Check_PlayerState(const _double & _dTimeDelta)
{
	if (STATE_CAMERAWORK == m_eUpperPlayerState)
	{
		m_ePreUpperPlayerState = m_eUpperPlayerState;
		return;
	}

	CWeapon* pWeapon = nullptr;

	/// 플레이어 전체 스테이트
	if (m_ePreUnitState != m_eUnitState)
	{
		// 이동, 점프, 기절, 공룡타기, 죽음 등등 상태 관리
		m_ePreUnitState = m_eUnitState;

		switch (m_eUnitState)
		{
			//case CUnit::STATE_CAMERAWORK:
			//	break;
		case CUnit::STATE_IDLE:
			m_eAimation = MoveAnimationByPose(P_A_BASIC_IDLE);

			/// 1인칭
			m_eFPVAnimation = P_A_FPV_BASIC_JUMPIDLE;
			break;
		case CUnit::STATE_MOVE:
			// 플레이어 이동 애니메이션 세팅 (포즈에 따라서 다르게 세팅)

			/// 1인칭
			m_eFPVAnimation = P_A_FPV_BASIC_MOVE;
			break;
		case CUnit::STATE_SPRINT:
			m_eAimation = P_A_BASIC_SPRINT;

			/// 1인칭
			m_eFPVAnimation = P_A_FPV_BASIC_SPRINT;
			break;
		case CUnit::STATE_JUMPING:
			m_eAimation = P_A_BASIC_JUMP_UP;

			/// 1인칭
			m_eFPVAnimation = P_A_FPV_BASIC_JUMPIDLE;
			break;
		case CUnit::STATE_FALLING:
			m_eAimation = P_A_BASIC_JUMP_FALL;

			/// 1인칭
			m_eFPVAnimation = P_A_FPV_BASIC_JUMPIDLE;
			break;
		case CUnit::STATE_LANDING:
			m_eAimation = P_A_BASIC_JUMP_LAND;

			/// 1인칭
			m_eFPVAnimation = P_A_FPV_BASIC_JUMPIDLE;
			break;
		case CUnit::STATE_TORPID:
			break;
		case CUnit::STATE_DEAD:
			/// 플레이어 사망시
			m_bDead = true;

			m_eAimation = P_A_BASIC_HURT;
			m_eUpperPlayerState = STATE_IDLE;
			g_GlobalTime = 0.4;
			m_bDissolve = true;
			m_bIsFullBack = false;

			// 무기 제거
			if (m_bIsGrapLeftHand)
			{
				pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrPreLeftHand, 0));

				// 능력치 반환
				m_tStat.fAtk -= pWeapon->Get_WeaponAtt();
				m_tStat.fTorpidRate = 0.2f;

				// 기존 아이탬 삭제
				pWeapon->Set_Dead();

				m_bIsGrapLeftHand = false;
				m_bIsGrapRightHand = false;
				m_wstrPreLeftHand = m_wstrLeftHand = L"";
				m_eUpperPlayerState = STATE_IDLE;
				m_ePlayerStance = P_STANCE_BASIC;
			}
			// 인벤토리 닫기
			if (m_bIsOpenInventory)
			{
				//static_cast<CRight_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Right_InventoryClose();
				static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->Stat_UI_Close();//인벤 관련 다 꺼짐
				m_bIsOpenInventory = !m_bIsOpenInventory;
			}
			break;
		case CUnit::STATE_RIDE:
			m_bIsRide = true;						// 탑승 시작

			if (L"Rex" == m_pDino->Get_DinoSpecies())
			{
				//m_eAimation = P_A_RIDE_REX_IDLE;		// 공룡 탑승 애니메이션
				static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Set_TPVCam_MaxDis(30.f);
			}
			else if (L"Para" == m_pDino->Get_DinoSpecies())
			{
				//m_eAimation = P_A_RIDE_PARA_IDLE;		// 공룡 탑승 애니메이션
				static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Set_TPVCam_MaxDis(10.f);
			}
			else
			{
				//m_eAimation = P_A_RIDE_IDLE;			// 공룡 탑승 애니메이션
				static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Set_TPVCam_MaxDis(10.f);
			}

			m_eUpperPlayerState = STATE_IDLE;			// 부분 스테이트 기본상태로 변경

														// 플레이어 look 공룡 기준으로 변경
			m_pTransformCom->Set_Rotate(ROT_Y, static_cast<CTransform*>(m_pDino->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Rotate(ROT_Y));
			break;
		}
	}

	// 플레이어 포즈별 기본상태 애니메이션 변경
	if (STATE_IDLE == m_eUnitState)
	{
		m_eAimation = MoveAnimationByPose(P_A_BASIC_IDLE);
	}

	// 플레이어 방향별 달리는 애니메이션 변경
	if (STATE_MOVE == m_eUnitState)
	{
		switch (m_eUnitMoveDir)
		{
		case CUnit::UNIT_DIR_DOWN:
			m_eAimation = MoveAnimationByPose(P_A_BASIC_MOVE_BWD);
			break;
		case CUnit::UNIT_DIR_DOWN_L:
			m_eAimation = MoveAnimationByPose(P_A_BASIC_MOVE_BWD_L);
			break;
		case CUnit::UNIT_DIR_DOWN_R:
			m_eAimation = MoveAnimationByPose(P_A_BASIC_MOVE_BWD_R);
			break;
		case CUnit::UNIT_DIR_L:
			m_eAimation = MoveAnimationByPose(P_A_BASIC_MOVE_L);
			break;
		case CUnit::UNIT_DIR_R:
			m_eAimation = MoveAnimationByPose(P_A_BASIC_MOVE_R);
			break;
		case CUnit::UNIT_DIR_UP:
			m_eAimation = MoveAnimationByPose(P_A_BASIC_MOVE_FWD);
			break;
		case CUnit::UNIT_DIR_UP_L:
			m_eAimation = MoveAnimationByPose(P_A_BASIC_MOVE_FWD_L);
			break;
		case CUnit::UNIT_DIR_UP_R:
			m_eAimation = MoveAnimationByPose(P_A_BASIC_MOVE_FWD_R);
			break;
		}

		// 걷거나 뛸때 일정 시간마다 흙먼지 생성
		if (P_POSE_STAND == m_ePlayerPose &&
			1.f < m_fCreateIntv)
		{
			m_fCreateIntv = 0.f;

			CGameObject* pGameObject = nullptr;

			m_fDustScale = 0.002f;

			pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Dust", &m_fDustScale);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), );
			_vec3 vPos = *m_pTransformCom->Get_Pos() + _vec3(0.f, 0.1f, 0.f);
			static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);
			static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3(0.f, 0.f, 0.f));
		}
	}


	/// 플레이어 부분 스테이트(상체)
	if (m_ePreUpperPlayerState != m_eUpperPlayerState)
	{
		// 에임, 공격, 무기스왑, 상호작용 등등 상태 관리
		m_ePreUpperPlayerState = m_eUpperPlayerState;

		switch (m_eUpperPlayerState)
		{
		case CUnit::STATE_IDLE:				// 기본 상태 (기본상태일 경우 하체 애니메이션을 따라감)
			m_eUpperAnimation = P_A_BASIC_IDLE;

			/// 1인칭
			m_eFPVUpperAnimation = P_A_FPV_BASIC_JUMPIDLE;
			break;
		case CUnit::STATE_ATTACK:			// 공격
			m_eUpperAnimation = P_A_BASIC_ATTACK;
			m_bIsAttack = true;

			/// 1인칭
			m_eFPVUpperAnimation = PLAYER_ANIMATION_FPV(P_A_FPV_BASIC_ATTACK_LEFT_1 + rand() % 4);
			break;
		case CUnit::STATE_HURT:				// 맞았을 때
			m_eUpperAnimation = P_A_BASIC_HURT;
			break;
		case CUnit::STATE_EAT:				// 먹을 때
			m_eUpperAnimation = P_A_BASIC_EAT;
			m_bIsInteract = true;
			break;
		case CUnit::STATE_HARVEST:			// 아이탬 줍기
			m_eUpperAnimation = P_A_BASIC_HARVEST;
			m_bIsInteract = true;
			// 채집 아이탬 추가
			static_cast<CHarvestable*>(m_pLookObject)->HitBy(this, WEAPON_HAND_HARVEST, _vec3(static_cast<CHarvestable*>(m_pLookObject)->Get_Pos()));

			/// 1인칭
			m_eFPVUpperAnimation = P_A_FPV_BASIC_HARVESTING;
			break;
		case CUnit::STATE_INVENTORY:		// 인벤토리 보기
			m_eUpperAnimation = P_A_BASIC_INVENTORY;

			/// 1인칭
			m_eFPVUpperAnimation = P_A_FPV_BASIC_IMPLANT_EQUIP;
			break;
		case CUnit::STATE_ATCTIVATECONSOLE:	// 대상 인벤토리 보기, 상호작용
			m_eUpperAnimation = P_A_BASIC_ATCTIVATECONSOLE;

			/// 1인칭
			m_eFPVUpperAnimation = P_A_FPV_BASIC_IMPLANT_EQUIP;
			break;
		case CUnit::STATE_WHISTLE:			// 휘파람 불기
			switch (m_ePlayerWhistle)
			{
			case CPlayer::P_WHISTLE_NORMAL:
				m_eUpperAnimation = P_A_BASIC_WHISTLE;
				break;
			case CPlayer::P_WHISTLE_ATTACK:
				m_eUpperAnimation = P_A_BASIC_WHISTLE_ATTACK;
				break;
			case CPlayer::P_WHISTLE_MOVE:
				m_eUpperAnimation = P_A_BASIC_WHISTLE_MOVE;
				break;
			default:
				break;
			}
			break;
		case CUnit::STATE_WEAPON_DRAW:		// 무기 꺼내기
			m_eUpperAnimation = P_A_BASIC_EQUIP_COPY;

			m_eFPVUpperAnimation = P_A_FPV_BASIC_FISTSEQUIP;
			break;
		case CUnit::STATE_WEAPON_PUTAWAY:	// 무기 넣기
			m_eUpperAnimation = P_A_BASIC_EQUIP_COPY;

			m_eFPVUpperAnimation = P_A_FPV_BASIC_FISTSUNEQUIP;
			break;
		case CUnit::STATE_WEAPON_THROW:
			if (P_STANCE_BOAL == m_ePlayerStance)
			{
				m_eUpperAnimation = P_A_SPEAR_PULLBACK_THROW;
				m_eFPVUpperAnimation = P_A_FPV_BOLA_THROW;
			}
			else if (P_STANCE_BOW == m_ePlayerStance)
			{
				m_eUpperAnimation = P_A_BOW_FIRE;
				m_eFPVUpperAnimation = P_A_FPV_BOW_FIRE;
			}
			break;
		case CUnit::STATE_WEAPON_PULLBACK:
			if (P_STANCE_BOAL == m_ePlayerStance)
			{
				m_eUpperAnimation = P_A_ETC_BOLA_LOOP;
				if (P_A_FPV_BOLA_THROWLOOP != m_eFPVUpperAnimation)
					m_eFPVUpperAnimation = P_A_FPV_BOLA_THROWSTART;
			}
			else if (P_STANCE_BOW == m_ePlayerStance)
			{
				if (P_A_BOW_PULLSTRING_LOOP != m_eUpperAnimation)
					m_eUpperAnimation = P_A_BOW_PULLSTRING;
				if (P_A_FPV_BOW_AIMLOOP != m_eFPVUpperAnimation)
					m_eFPVUpperAnimation = P_A_FPV_BOW_AIM;
			}
			break;
		case CUnit::STATE_WEAPON_RELOAD:
			if (P_STANCE_BOW == m_ePlayerStance)
			{
				m_eUpperAnimation = P_A_BOW_RELOAD;
				m_eFPVUpperAnimation = P_A_FPV_BOW_RELOAD;
			}
			else
			{
				ERR_MSG(L"잘못된 플레이어 재장전 스테이트 호출");
				//m_eUpperPlayerState = STATE_IDLE;
				//m_eUpperAnimation;
				//m_eFPVUpperAnimation;
			}
			break;
		}
	}
}

void CPlayer::Is_Attack()
{
	if (m_bIsAttack)
	{
		if (m_bIsFPVCam)
		{
			FPV_Is_Attack();
			return;
		}

		_double dAnimationtime = m_pMeshCom->Get_UpperCurrentTime();

		// 무기가 있을 경우 무기의 어택 호출
		if (m_bIsGrapLeftHand)
		{
			switch (m_ePlayerStance)
			{
			case CPlayer::P_STANCE_HATCHET:
				// 도끼 자세일때
				if (0.5 < dAnimationtime && 0.7 > dAnimationtime)
				{
					// 무기를 들고 있을 때만 공격
					CWeapon* pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrLeftHand.c_str()));
					if (pWeapon)
						pWeapon->Is_Attack(m_wstrTargetLayer, nullptr);
				}
				break;
			case CPlayer::P_STANCE_SPEAR:
				// 창 자세일때
				if (0.3 < dAnimationtime && 0.5 > dAnimationtime)
				{
					// 무기를 들고 있을 때만 공격
					CWeapon* pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrLeftHand.c_str()));
					if (pWeapon)
						pWeapon->Is_Attack(m_wstrTargetLayer, nullptr);
				}
				break;
			}
		}
		else
		{
			////맨손일 때
			m_dUpperAnimationSpeed = 0.7;

			if (0.4 < dAnimationtime && 0.6 > dAnimationtime)
			{
				CSphereCollider::SPHEREINFO tColInfo = m_pSphereColliderCom->Get_listCollider(CSphereCollider::COL_ATTACK)->front();
				_vec3 vWorldPos;
				memcpy(&vWorldPos, &tColInfo.matColMatrix.m[INFO_POS][0], sizeof(_vec3));

				//_vec3 vCollPos;
				if (true == CCollisionFunc::AttackOnUnit(this, &vWorldPos, &tColInfo.fRadius, m_wstrTargetLayer, CSphereCollider::COL_HIT, WEAPON_HAND, nullptr, nullptr/*, &vCollPos*/))
				{
					// 맨손 공격이 맞았을 때 플레이어 HP 2 감소
					m_tStat.fHp -= 2.f;
					if (0.f >= m_tStat.fHp)
						m_tStat.fHp = 0.f;



					//vCollPos 위치에 이펙트 출력해야하는데
					//현재 여러 충돌체에 한 번에 충돌할 경우 마지막에 연산한 충돌체와의 충돌지점만을 반환함. (하나의 공룡의 여러 충돌체, 여러 공룡의 여러 충돌체)
					//해결방안 1) 무적상태가 아닌 어떤 충돌체와의 충돌 검출 시 순회를 멈추고 충돌함수 종료 -> 맨처음 연산된 운좋은 공룡에 피 1회 발생
					//해결방안 2) 무적상태가 아닌 모든 충돌 검사 후 가장 가까운(먼) 충돌을 선별하여 1회만 충돌 처리 -> 가장 가까운 공룡에 피 1회 발생
					//해결방안 3) 검출된 충돌점을 해당 객체의 Hitby 함수로 넘겨 거기서 루프당 1회만 피튀기도록 처리하는 방법 -> 걸린 모든 공룡이 각각 피 1회 발생
					//	->	해결방안 3으로 했음. 각 객체의 Hitby 함수에서 피격이펙트 출력하면 됩니다.
					//		(공룡(피), 바위(먼지), 덤불(꽃가루) 등 개별객체마다 이펙트가 다르므로 이게 더 적절한듯...)
				}
			}
		}
	}
}

void CPlayer::Set_ShadowLight()
{
	//_vec3 vLightAt = *m_pTransformCom->Get_Pos();

	if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
		m_fPosX -= 1.f;
	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
		m_fPosX += 1.f;
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
		m_fPosY -= 1.f;
	if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
		m_fPosY += 1.f;
	if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
		m_fPosZ -= 1.f;
	if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
		m_fPosZ += 1.f;

	_vec3 vLightPos = *m_pTransformCom->Get_Pos() + _vec3(0.f + m_fPosX, 100.f + m_fPosY, 150.f + m_fPosZ);
	_vec3 vLightAt = m_vecLightPos + _vec3(0.f, -10.f, -15.f);
	_vec3 vLightDir = vLightAt - vLightPos;

	//m_vecLightPos = _vec3(-400.f + m_fPosX, 100.f + m_fPosY, 600.f + m_fPosZ);
	//m_vecLightAt = m_vecLightPos + _vec3(1.f, -10.f, -15.f) * 20.f;

	Engine::Set_ShadowLight(vLightPos, vLightAt);

	Set_ShadowMap(vLightPos, vLightAt);
}

void CPlayer::FPV_Check_IsEndAnimation(const _double& _dTimeDelta)
{
	switch (m_ePreUpperPlayerState)
	{
	case CUnit::STATE_CAMERAWORK:
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.1))
		{
			if (m_bIsFPVCam)
				Switch_Camera(1);
			m_eUnitState = STATE_IDLE;
			m_eUpperPlayerState = STATE_IDLE;
			m_bIsLockCamera = false;
		}
		break;
	case CUnit::STATE_IDLE:
		break;
	case CUnit::STATE_IDLE2:
		break;
	case CUnit::STATE_IDLE3:
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
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.1))
		{
			// 공격 종료
			m_eUpperPlayerState = STATE_IDLE;
			m_bIsAttack = false;
			m_dUpperAnimationSpeed = 1;
		}
		break;
	case CUnit::STATE_HURT:
		break;
	case CUnit::STATE_EAT:
		break;
	case CUnit::STATE_HARVEST:
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.1))
		{
			m_eUpperPlayerState = STATE_IDLE;
			m_bIsInteract = false;
		}
		break;
	case CUnit::STATE_INVENTORY:
	case CUnit::STATE_ATCTIVATECONSOLE:
		m_dOpenDelayInven += _dTimeDelta;
		if (!m_bIsOpenInventory && 0.2 < m_dOpenDelayInven)
		{
			Open_Inventory();
			m_eFPVUpperAnimation = P_A_FPV_BASIC_IMPLANT_IDLE;
		}
		break;
	case CUnit::STATE_WHISTLE:
		break;
	case CUnit::STATE_WEAPON_DRAW:		// 무기 꺼내기
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.3))
		{
			m_eUpperPlayerState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_WEAPON_PUTAWAY:	// 무기 넣기
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.3))
		{
			// 현재 무기 집어넣고 다음 무기가 있을 경우 STATE_WEAPON_DRAW 로 변경
			if (Swap_Weapon())
				m_eUpperPlayerState = STATE_WEAPON_DRAW;
			else
				m_eUpperPlayerState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_WEAPON_THROW:
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.1))
		{
			/// 던지는 무기가 볼라일 경우
			if (P_STANCE_BOAL == m_ePlayerStance)
			{
				// 기존 아이탬 삭제
				Engine::Get_GameObject(m_wstrLeftHand, 0)->Set_Dead();
				m_bIsGrapLeftHand = false;
				//static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI"))->UisngRemove_Item(m_iQuickSlot, 1);

				// 볼라 투사체 생성
				Shot_Bullet(BULLET_BOLA);
				//_mat matWorld = m_pTransformCom->Get_WorldMatrix();
				
				//// fGravity 0.3, fSpeed 10 ~ fGravity 0.1, fSpeed 20
				//BULLET_DESC tBulletInfo;
				//tBulletInfo.eBulletType = BULLET_BOLA;
				//tBulletInfo.pShooter = this;
				//_vec3 vLook;
				//memcpy(&tBulletInfo.vPos, &matWorld.m[INFO_POS][0], sizeof(_vec3));
				//memcpy(&vLook, &matWorld.m[INFO_LOOK][0], sizeof(_vec3));
				//D3DXVec3Normalize(&vLook, &vLook);
				//tBulletInfo.vPos -= vLook * 0.5f;
				//tBulletInfo.vPos.y += 1.5f;
				//_mat matView;
				//m_pDevice->GetTransform(D3DTS_VIEW, &matView);
				//D3DXMatrixInverse(&matView, NULL, &matView);
				//memcpy(&tBulletInfo.vDir, &matView.m[INFO_LOOK][0], sizeof(_vec3));
				//tBulletInfo.fGravity = 0.f;
				////tBulletInfo.fGravity = 0.5f / m_bBolaPower;
				//tBulletInfo.fSpeed = 1.f * m_bBolaPower;
				//m_bBolaPower = 1.f;

				//CGameObject* pBullet = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Bullet", &tBulletInfo);
				//Engine::Add_GameObjectInLayer(L"Layer_Bullet", pBullet);

				// 퀵슬롯에 볼라가 남아있을 경우 새로운 볼라 꺼내기
				if (ITEMID_END != static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI"))->Get_Item_To_Number(m_iQuickSlot).iItemID)
				{
					Swap_Weapon();
				}
				else
				{
					// 볼라가 없을 경우 빈손으로 돌아오기
					m_eUpperPlayerState = STATE_IDLE;
					m_ePlayerStance = P_STANCE_BASIC;
					m_wstrPreLeftHand = m_wstrLeftHand = L"";
				}
			}
			else if (P_STANCE_BOW == m_ePlayerStance)
			{
				// 화살 발사 후 재장전
				//Shot_Bullet(BULLET_ARROW);

				m_eUpperPlayerState = STATE_WEAPON_RELOAD;


				//m_eUpperPlayerState = STATE_IDLE;
			}
			else
				m_eUpperPlayerState = STATE_IDLE;

			m_dUpperAnimationSpeed = 1;
		}
		break;
	case CUnit::STATE_WEAPON_PULLBACK:
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.1))
		{
			if (P_STANCE_BOAL == m_ePlayerStance)
			{
				m_eFPVUpperAnimation = P_A_FPV_BOLA_THROWLOOP;

			}
			else if (P_STANCE_BOW == m_ePlayerStance)
			{
				m_eFPVUpperAnimation = P_A_FPV_BOW_AIMLOOP;
			}
		}
		break;
	case CUnit::STATE_WEAPON_RELOAD:
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.3))
		{
			if (P_STANCE_BOW == m_ePlayerStance)
			{
				m_eUpperPlayerState = STATE_IDLE;;
			}
		}
		break;
	case CUnit::STATE_HOLDING:
		break;
	case CUnit::STATE_DEAD:
		m_bDead = true;
		break;
	case CUnit::STATE_TORPID:
		break;
	case CUnit::STATE_TORPID_IDLE:
		break;
	case CUnit::STATE_TORPID_TAMED:
		break;
	case CUnit::STATE_TORPID_WILD:
		break;
	case CUnit::STATE_TORPID_EAT:
		break;
	case CUnit::STATE_RIDE:
		break;
	default:
		break;
	}
}

void CPlayer::FPV_Is_Attack()
{
	if (m_bIsAttack)
	{
		_double dAnimationtime = m_pMeshFPVCom->Get_CurrentTime();

		// 무기가 있을 경우 무기의 어택 호출
		if (m_bIsGrapLeftHand)
		{
			switch (m_ePlayerStance)
			{
			case CPlayer::P_STANCE_HATCHET:
				// 도끼자세일때
				if (0.5 < dAnimationtime && 0.7 > dAnimationtime)
				{
					// 무기를 들고 있을 때만 공격
					CWeapon* pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrLeftHand.c_str()));
					if (pWeapon)
						pWeapon->Is_FPVAttack(m_wstrTargetLayer, *m_pFPVSphereColliderCom->Get_Pos(), *m_pFPVSphereColliderCom->Get_Radius());
				}
				break;
			case CPlayer::P_STANCE_SPEAR:
				// 도끼자세일때
				if (0.3 < dAnimationtime && 0.5 > dAnimationtime)
				{
					// 무기를 들고 있을 때만 공격
					CWeapon* pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrLeftHand.c_str()));
					if (pWeapon)
						pWeapon->Is_Attack(m_wstrTargetLayer, nullptr);
				}
				break;
			}
		}
		else
		{
			////맨손일 때
			m_dUpperAnimationSpeed = 0.1;

			if (0.4 < dAnimationtime && 0.6 > dAnimationtime)
			{
				CSphereCollider::SPHEREINFO tColInfo = m_pFPVSphereColliderCom->Get_listCollider(CSphereCollider::COL_ATTACK)->front();
				_vec3 vWorldPos;
				memcpy(&vWorldPos, &tColInfo.matColMatrix.m[INFO_POS][0], sizeof(_vec3));

				//_vec3 vCollPos;
				if (true == CCollisionFunc::AttackOnUnit(this, &vWorldPos, &tColInfo.fRadius, m_wstrTargetLayer, CSphereCollider::COL_HIT, WEAPON_HAND, nullptr, nullptr/*, &vCollPos*/, true))
				{
					// 맨손 공격이 맞았을 때 플레이어 HP 2 감소
					m_tStat.fHp -= 2.f;
					if (0.f >= m_tStat.fHp)
						m_tStat.fHp = 0.f;
				}
			}
		}
	}
}

void CPlayer::Preview_Structure(const _vec3& _vPreviewPos, _bool _bRayCast, _float* _pAngleY)
{
	CGameObject* pPreView = Engine::Get_GameObject(L"Layer_PreViewStructure");

	if (!pPreView)
	{
		STRUCTURE_DESC tStructInfo;
		tStructInfo.bIsPreview = true;
		tStructInfo.eStructureType = m_eBuilding;
		pPreView = Engine::Clone_GameObjectPrototype(m_wstrPreviewStructure, &tStructInfo);
		Engine::Add_GameObjectInLayer(L"Layer_PreViewStructure", pPreView);
	}

	if (!pPreView)
		return;

	static_cast<CStructure*>(pPreView)->Set_PreviewPos(_vPreviewPos);
	if(_pAngleY)
		static_cast<CStructure*>(pPreView)->Set_PreviewAngle(*_pAngleY);
	else
		static_cast<CStructure*>(pPreView)->Set_PreviewAngle(m_fAngleY + 180.f);

	// 레이 캐스팅 성공했을 때, 초록색 표시
	if (_bRayCast)
		pPreView->Set_Color(0.f, 1.f, 0.f);

	// 레이 캐스팅 실패했을 때, 빨간색 표시
	else
		pPreView->Set_Color(1.f, 0.f, 0.f);
}

void CPlayer::Build_Structure(const _vec3& _vBuildPos, CGameObject* pPlatform, _float* _pAngleY)
{
	// 프리뷰 지우기
	CGameObject* pPreView = Engine::Get_GameObject(L"Layer_PreViewStructure");
	pPreView->Set_Dead();

	// Campfire Point Light Pos
	STRUCTURE_DESC tStructInfo;
	tStructInfo.bIsPreview = false;
	tStructInfo.eStructureType = m_eBuilding;
	tStructInfo.vPos = _vBuildPos;
	if (_pAngleY)
		tStructInfo.fAngleY = *_pAngleY;
	else
		tStructInfo.fAngleY = m_fAngleY + 180.f;

	// 클론 생성
	CGameObject* pStructure = Engine::Clone_GameObjectPrototype(m_wstrPreviewStructure, &tStructInfo/*_vPos*/);
	Engine::Add_GameObjectInLayer(L"Layer_Structure", pStructure);

	// 구조물 생성
	if (pPlatform)
	{
		static_cast<CStructure*>(pStructure)->Set_TartgetPlatform(pPlatform);
	}

	// 특정 구조물의 함수 호출
	if (L"Proto_Obj_SleepingBag" == m_wstrPreviewStructure)
	{
		static_cast<CSleepingBag*>(pStructure)->TurnOn_Structure();
	}

	m_bIsBuilding = false;
	m_wstrPreviewStructure = L"";
	m_eBuilding = STRUCTURE_END;

	CGameObject* pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_StructDust", &m_fStructureDustScale);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), );
	static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(_vBuildPos);
}

CStructure* CPlayer::Ray_FindPivot(_vec3* _vPos, _float* _fAngleY, _uint* _iDirIndex)
{
	/// 피벗이 필요 없는 구조물은 nullptr 반환
	if (L"Proto_Obj_Campfire" == m_wstrPreviewStructure ||
		L"Proto_Obj_SleepingBag" == m_wstrPreviewStructure ||
		L"Proto_Obj_Forge" == m_wstrPreviewStructure || 
		L"Proto_Obj_MortarPestal" == m_wstrPreviewStructure ||
		L"Proto_Obj_Smithy" == m_wstrPreviewStructure)
	{
		return nullptr;
	}

	CStructure* pTargetObj = nullptr;
	_bool bIsPicking = false;

	auto pLayer = Engine::Get_LayerList(L"Layer_Structure");

	if (nullptr == pLayer)
		return nullptr;

	if (0 >= pLayer->size())
		return nullptr;

	_float fCamDis = 0.f;
	if (!m_bIsFPVCam)
		fCamDis = MAXCAMDIS;

	_float fTargetDis = -1.f;

	for (auto pObj : *pLayer)
	{
		// 콜리전 여부로 1차 필터링
		CSphereCollider* pCollider = static_cast<CSphereCollider*>(pObj->Get_Component(L"Com_SphereCollider", ID_DYNAMIC));
		CTransform*	pTransform = static_cast<CTransform*>(pObj->Get_Component(L"Com_Transform", ID_DYNAMIC));

		if (nullptr == pCollider)
			continue;

		CSphereCollider::SPHEREINFO tCol = pCollider->Get_listCollider(CSphereCollider::COL_INTERACTION)->front();

		_vec3 fColPos;
		memcpy(&fColPos, &tCol.matColMatrix.m[INFO_POS][0], sizeof(_vec3));

		// 거리로 2차 필터링		
		if (10.f <= D3DXVec3Length(&_vec3(m_vPlayerPos - fColPos)))
			continue;

		// 피벗 구체 순회하기
		for (auto iter : *pCollider->Get_listCollider(CSphereCollider::COL_BUILD_PIVOT))
		{
			// 모든 건축물 피벗의 반지름 크기는 1.f;
			_float fColRadius;
			fColRadius = 1.f;

			// 피벗 위치 가져오기
			memcpy(&fColPos, &iter.matColMatrix.m[INFO_POS][0], sizeof(_vec3));


			/// 여기서부터 직선과 구체충돌 시작
			// 뷰 역행렬 가져오기
			_mat matView;
			m_pDevice->GetTransform(D3DTS_VIEW, &matView);
			D3DXMatrixInverse(&matView, NULL, &matView);

			// 피킹 시작할 포스와 방향 만들기
			_vec3 vCamPos, vCamLook;
			memcpy(&vCamPos, &matView.m[INFO_POS][0], sizeof(_vec3));
			memcpy(&vCamLook, &matView.m[INFO_LOOK][0], sizeof(_vec3));
			D3DXVec3Normalize(&vCamLook, &vCamLook);

			vCamPos += vCamLook * fCamDis;

			// 수선의 발을 구하기 위한 cos 값 구하기
			_vec3 v1 = fColPos - vCamPos;					// 직선의 시작점과 구체중심을 이어주는 백터
			_float f2 = D3DXVec3Length(&v1);				// 직선의 시작점과 구체 중심의 거리를 구한다.
			_float f1 = D3DXVec3Dot(&vCamLook, &v1);		// 수선의 발을 내린 빗변의 길이를 구한다.

			if (0.f > f1)
				continue;

			_float fDis = sqrtf((f2 * f2) - (f1 * f1));

			if (fDis < fColRadius)
			{
				if (0.f > fTargetDis)
				{
					// 처음 타겟을 찾았을 때
					bIsPicking = true;

					fTargetDis = f2;

					memcpy(_vPos, &iter.matColMatrix.m[INFO_POS][0], sizeof(_vec3));
					*_iDirIndex = iter.eColDir;
					pTargetObj = static_cast<CStructure*>(pObj);

					// Y 회전값
					CTransform* pTrans = static_cast<CTransform*>(pObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
					_vec3 vLook;
					pTrans->Get_Info(INFO_LOOK, &vLook);
					vLook.y = 0.f;
					D3DXVec3Normalize(&vLook, &vLook);

					_float fRadian = D3DXVec3Dot(&vLook, &_vec3(0.f, 0.f, 1.f));
					
					fRadian = acosf(fRadian);

					if (0.f > vLook.x)
						fRadian *= -1.f;
					*_fAngleY = D3DXToDegree(fRadian);
				}
				else
				{
					// 다음 타겟을 찾았을 때
					if (fTargetDis > f2)
					{
						fTargetDis = f2;

						memcpy(_vPos, &iter.matColMatrix.m[INFO_POS][0], sizeof(_vec3));
						*_iDirIndex = iter.eColDir;
						pTargetObj = static_cast<CStructure*>(pObj);

						// Y 회전값
						CTransform* pTrans = static_cast<CTransform*>(pObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
						_vec3 vLook;
						pTrans->Get_Info(INFO_LOOK, &vLook);
						vLook.y = 0.f;
						D3DXVec3Normalize(&vLook, &vLook);

						_float fRadian = D3DXVec3Dot(&vLook, &_vec3(0.f, 0.f, 1.f));

						fRadian = acosf(fRadian);

						if (0.f > vLook.x)
							fRadian *= -1.f;
						*_fAngleY = D3DXToDegree(fRadian);
					}
				}
			}
		}
	}

	return pTargetObj;
}

_bool CPlayer::Pivot_Relocation(CStructure * _pStructure, _vec3 & _vPivotPos, _float & _fAngle, _uint & _iColDir)
{
	STRUCTURE_TYPE eStructType = _pStructure->Get_StructType();

	if (STRUCTURE_THATCH_FLOOR == eStructType ||
		STRUCTURE_STONE_FLOOR == eStructType)
	{
		/// 토대 피벗
		if (L"Proto_Obj_Thatch_Wall" == m_wstrPreviewStructure ||
			L"Proto_Obj_Thatch_DoorFrame" == m_wstrPreviewStructure)
		{
			// 벽일 경우 방향에 따라서 회전시키기
			switch (_iColDir)
			{
			case CSphereCollider::COL_FWD:
				_fAngle += 90.f;
				break;
			case CSphereCollider::COL_BWD:
				_fAngle += 270.f;
				break;
			case CSphereCollider::COL_RIGHT:
				_fAngle += 180.f;
				break;
			case CSphereCollider::COL_LEFT:
			default:
				break;
			}

			_vPivotPos.y += 1.5f;
			return true;
		}
		else if (L"Proto_Obj_Thatch_Floor" == m_wstrPreviewStructure)
		{
			_vPivotPos += _pStructure->Get_PivotDir(_iColDir);
			return true;
		}
	}
	else if (STRUCTURE_THATCH_WALL == eStructType ||
		STRUCTURE_THATCH_DOORFRAME == eStructType)
	{
		/// 벽 피벗
		if (L"Proto_Obj_Thatch_Roof" == m_wstrPreviewStructure)
		{
			// 구조물의 방향과 플레이어의 방향으로 앞뒤 판단
			CTransform* pTransCom = static_cast<CTransform*>(_pStructure->Get_Component(L"Com_Transform", ID_DYNAMIC));
			if (!pTransCom)
				return false;

			_vec3 vLook, vPos, vPlayerPos;

			pTransCom->Get_Info(INFO_POS, &vPos);
			m_pTransformCom->Get_Info(INFO_POS, &vPlayerPos);
			vPlayerPos.y = 0.f;

			vPos = vPlayerPos - vPos;
			D3DXVec3Normalize(&vPos, &vPos);
			pTransCom->Get_Info(INFO_RIGHT, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);

			_float fDot = D3DXVec3Dot(&vPos, &vLook);

			if(0.f < fDot)
			{
				_vPivotPos += vLook * 1.5f;
			}
			else
				_vPivotPos -= vLook * 1.5f;

			_vPivotPos.y -= 0.2f;

			return true;
		}
		else if (L"Proto_Obj_Thatch_Wall" == m_wstrPreviewStructure ||
			L"Proto_Obj_Thatch_DoorFrame" == m_wstrPreviewStructure)
		{
			_vPivotPos += _pStructure->Get_PivotDir(_iColDir);
			return true;
		}
		else if (L"Proto_Obj_Thatch_Door" == m_wstrPreviewStructure &&
			STRUCTURE_THATCH_DOORFRAME == eStructType &&
			CSphereCollider::COL_DOWN == _iColDir)
		{
			CTransform* pTransCom = static_cast<CTransform*>(_pStructure->Get_Component(L"Com_Transform", ID_DYNAMIC));
			if (!pTransCom)
				return false;
			_vec3 vLook;
			pTransCom->Get_Info(INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);

			_vPivotPos -= vLook * 0.6f;
			return true;
		}
	}
	else if (STRUCTURE_THATCH_ROOF == eStructType)
	{
		/// 지붕 피벗
		if (L"Proto_Obj_Thatch_Roof" == m_wstrPreviewStructure)
		{
			_vPivotPos += _pStructure->Get_PivotDir(_iColDir);
			return true;
		}
	}

	return false;
}

void CPlayer::Player_Respawn(const _vec3 _vSpawnPos)
{
	g_GlobalTime = 1;

	m_bDead = false;
	m_bDying = false;

	// 스탯 초기화
	m_tStat.fHp = m_tStat.fMaxHp;
	m_tStat.fHunger = m_tStat.fMaxHunger;
	m_tStat.fStamina = m_tStat.fMaxStamina;
	m_tStat.fThirst = m_tStat.fMaxThirst;

	m_pAttacker = nullptr;
	m_iKnockCount = 0;

	// 디졸드 해제
	m_bDissolve = false;
	m_fDissolveAmount = 0.f;

	//페이드아웃 해제
	Engine::Fade_In(1.f);

	// 상태 초기화
	/// 부활시 카메라 연출
	Switch_Camera(0);
	m_bIsFPVCam = true;
	m_bIsLockCamera = true;
	m_eFPVAnimation = P_A_FPV_BASIC_CAMERA_SHORT;
	m_eFPVUpperAnimation = P_A_FPV_BASIC_CAMERA_SHORT;

	m_eUnitState = STATE_CAMERAWORK;
	m_eUpperPlayerState = STATE_CAMERAWORK;
	m_ePlayerPose = P_POSE_STAND;
	m_ePlayerStance = P_STANCE_BASIC;

	// 부활할 위치값 세팅
	m_pTransformCom->Set_Pos(_vSpawnPos);
	PxExtendedVec3 vSavePos = PxExtendedVec3(_vSpawnPos.x, _vSpawnPos.y, _vSpawnPos.z);
	m_pCtrl->setPosition(vSavePos);

	// 카메라 움직이기
	static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2);


	// 무기 초기화
	m_bIsGrapLeftHand = false;
	m_bIsGrapRightHand = false;
	m_wstrLeftHand = L"";
	m_wstrPreLeftHand = L"";

	ZeroMemory(m_bIsFPVSubsetRender, sizeof(_bool) * m_pMeshFPVCom->Get_MeshList()->size());
	m_bIsFPVSubsetRender[0] = true;		// 몸체만 그리기

	ZeroMemory(m_bIsTPVSubsetRender, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsTPVSubsetRender[0] = true;		// 몸체만 그리기
	m_bIsTPVSubsetRender[1] = true;		// 언더웨어



	ShowCursor(false);
}

void CPlayer::Switch_Camera(_uint _iIndex)
{
	static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_CameraType(CCameraMrg::CAMERA_TYPE(_iIndex), m_fAngleX, m_fAngleY);

	if (0 == _iIndex)
		m_bIsFPVCam = true;
	else if (1 == _iIndex)
	{
		m_bIsFPVCam = false;
		// 무기에 따라서 카메라 시점 변경
		if (L"Layer_Weapon_Bola" == m_wstrLeftHand ||
			L"Layer_Weapon_Bow" == m_wstrLeftHand)
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2, true);
		else
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2, true);
	}

	if (L"" != m_wstrLeftHand)
	{
		CWeapon* pWeapon = nullptr;
		pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrPreLeftHand, 0));
		if (pWeapon)
			pWeapon->Set_FPVPlayer(m_bIsFPVCam);
	}
}

void CPlayer::Switch_Camera_TPVToFPV(_uint _iIndex)
{
	//static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_CameraType(CCameraMrg::CAMERA_TYPE(_iIndex), m_fAngleX, m_fAngleY, true);

	if (0 == _iIndex)
	{
		// 3인칭에서 1인칭으로 자연스럽게 바꾸기
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(0.f, 0.f, 50.f, 0.2, false, true);
	}
	else if (1 == _iIndex)
	{
		// 1인칭에서 3인칭으로 자연스럽게 바꾸기
		// 무기에 따라서 카메라 시점 변경
		if (L"Layer_Weapon_Bola" == m_wstrLeftHand ||
			L"Layer_Weapon_Bow" == m_wstrLeftHand)
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.1, true);
		else
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2, true);
	}
}

void CPlayer::Is_EatItem(const ITEM_INFO& _tItemInfo)
{
	// 플레이어 애니메이션 제한
	m_eUpperPlayerState = STATE_EAT;

	// 아이탬별 추가 효과
	if (ITEMID_Meat == _tItemInfo.iItemID)
		m_tStat.fHp -= 3.f;		// 생고기를 먹을 경우 HP 3 감소


								// 아이탬 능력치 만큼 허기 채우기
	if (m_tStat.fMaxHunger <= m_tStat.fHunger)
	{
		// 배고픔이 맥스일 때 음식 회복치의 절반만큼 HP 회복
		m_tStat.fHp += _tItemInfo.iStat * 0.5f;
		if (m_tStat.fMaxHp <= m_tStat.fHp)
			m_tStat.fHp = m_tStat.fMaxHp;
	}
	else if (0 > m_tStat.fMaxHunger - (m_tStat.fHunger + _tItemInfo.iStat))
		m_tStat.fHunger = m_tStat.fMaxHunger;
	else
		m_tStat.fHunger += _tItemInfo.iStat;
}

CPlayer::PLAYER_ANIMATION CPlayer::MoveAnimationByPose(PLAYER_ANIMATION _eAnimation)
{
	switch (_eAnimation)
	{
	case CPlayer::P_A_BASIC_IDLE:
		switch (m_ePlayerPose)
		{
		case CPlayer::P_POSE_STAND:
			return P_A_BASIC_IDLE;
		case CPlayer::P_POSE_CROUCH:
			return P_A_BASIC_CROUCH_IDLE;
		case CPlayer::P_POSE_PRONE:
			return P_A_BASIC_PRONE_IDLE;
		}
		break;
	case CPlayer::P_A_BASIC_MOVE_R:
		// 오른쪽으로 이동
		switch (m_ePlayerPose)
		{
		case CPlayer::P_POSE_STAND:
			return P_A_BASIC_MOVE_R;
		case CPlayer::P_POSE_CROUCH:
			return P_A_BASIC_CROUCH_R;
		case CPlayer::P_POSE_PRONE:
			return P_A_BASIC_PRONE_MOVE_FWD;
		}
		break;
	case CPlayer::P_A_BASIC_MOVE_L:
		// 왼쪽으로 이동
		switch (m_ePlayerPose)
		{
		case CPlayer::P_POSE_STAND:
			return P_A_BASIC_MOVE_L;
		case CPlayer::P_POSE_CROUCH:
			return P_A_BASIC_CROUCH_L;
		case CPlayer::P_POSE_PRONE:
			return P_A_BASIC_PRONE_MOVE_FWD;
		}
		break;
	case CPlayer::P_A_BASIC_MOVE_FWD_R:
		// 앞오른쪽으로 이동
		switch (m_ePlayerPose)
		{
		case CPlayer::P_POSE_STAND:
			return P_A_BASIC_MOVE_FWD_R;
		case CPlayer::P_POSE_CROUCH:
			return P_A_BASIC_CROUCH_FWD_R;
		case CPlayer::P_POSE_PRONE:
			return P_A_BASIC_PRONE_MOVE_FWD;
		}
		break;
	case CPlayer::P_A_BASIC_MOVE_FWD_L:
		// 앞왼쪽으로 이동
		switch (m_ePlayerPose)
		{
		case CPlayer::P_POSE_STAND:
			return P_A_BASIC_MOVE_FWD_L;
		case CPlayer::P_POSE_CROUCH:
			return P_A_BASIC_CROUCH_FWD_L;
		case CPlayer::P_POSE_PRONE:
			return P_A_BASIC_PRONE_MOVE_FWD;
		}
		break;
	case CPlayer::P_A_BASIC_MOVE_FWD:
		// 앞쪽으로 이동
		switch (m_ePlayerPose)
		{
		case CPlayer::P_POSE_STAND:
			return P_A_BASIC_MOVE_FWD;
		case CPlayer::P_POSE_CROUCH:
			return P_A_BASIC_CROUCH_FWD;
		case CPlayer::P_POSE_PRONE:
			return P_A_BASIC_PRONE_MOVE_FWD;
		}
		break;
	case CPlayer::P_A_BASIC_MOVE_BWD_R:
		// 뒤오른쪽으로 이동
		switch (m_ePlayerPose)
		{
		case CPlayer::P_POSE_STAND:
			return P_A_BASIC_MOVE_BWD_R;
		case CPlayer::P_POSE_CROUCH:
			return P_A_BASIC_CROUCH_BWD_R;
		case CPlayer::P_POSE_PRONE:
			return P_A_BASIC_PRONE_MOVE_FWD;
		}
		break;
	case CPlayer::P_A_BASIC_MOVE_BWD_L:
		// 뒤왼쪽으로 이동
		switch (m_ePlayerPose)
		{
		case CPlayer::P_POSE_STAND:
			return P_A_BASIC_MOVE_BWD_L;
		case CPlayer::P_POSE_CROUCH:
			return P_A_BASIC_CROUCH_BWD_L;
		case CPlayer::P_POSE_PRONE:
			return P_A_BASIC_PRONE_MOVE_FWD;
		}
		break;
	case CPlayer::P_A_BASIC_MOVE_BWD:
		// 뒤쪽으로 이동
		switch (m_ePlayerPose)
		{
		case CPlayer::P_POSE_STAND:
			return P_A_BASIC_MOVE_BWD;
		case CPlayer::P_POSE_CROUCH:
			return P_A_BASIC_CROUCH_BWD;
		case CPlayer::P_POSE_PRONE:
			return P_A_BASIC_PRONE_MOVE_FWD;
		}
		break;
	}
	return P_A_END;
}

CPlayer::PLAYER_ANIMATION CPlayer::PlayerAnimationByStance()
{
	switch (m_ePlayerStance)
	{
	case CPlayer::P_STANCE_BASIC:
		return m_eAimation;
	case CPlayer::P_STANCE_HATCHET:
	case CPlayer::P_STANCE_BOAL:
		return PlayerAnimation_ToHatchet();
	case CPlayer::P_STANCE_SPEAR:
		return PlayerAnimation_ToSpear();
	case CPlayer::P_STANCE_BOW:
		return PlayerAnimation_ToBow();
	}
	return m_eAimation;
}

CPlayer::PLAYER_ANIMATION CPlayer::PlayerUpperAnimationByStance()
{
	switch (m_ePlayerStance)
	{
	case CPlayer::P_STANCE_BASIC:
		return m_eUpperAnimation;
	case CPlayer::P_STANCE_HATCHET:
	case CPlayer::P_STANCE_BOAL:
		return PlayerUpperAnimation_ToHatchet();
	case CPlayer::P_STANCE_SPEAR:
		return PlayerUpperAnimation_ToSpear();
	case CPlayer::P_STANCE_BOW:
		return PlayerUpperAnimation_ToBow();
	}
	return m_eUpperAnimation;
}

CPlayer::PLAYER_ANIMATION_FPV CPlayer::PlayerFPVAnimationByStance()
{
	switch (m_ePlayerStance)
	{
	case CPlayer::P_STANCE_BASIC:
		return P_A_FPV_BASIC_JUMPIDLE;
	case CPlayer::P_STANCE_HATCHET:
		return PlayerFPVAnimation_ToHatchet();
	case CPlayer::P_STANCE_SPEAR:
		return PlayerFPVAnimation_ToSpear();
	case CPlayer::P_STANCE_BOAL:
		return PlayerFPVAnimation_ToBola();
	case CPlayer::P_STANCE_BOW:
		return PlayerFPVAnimation_ToBow();
	default:
		break;
	}
	return m_eFPVAnimation;
}

CPlayer::PLAYER_ANIMATION_FPV CPlayer::PlayerFPVUpperAnimationByStance()
{
	switch (m_ePlayerStance)
	{
	case CPlayer::P_STANCE_BASIC:
		return m_eFPVUpperAnimation;
	case CPlayer::P_STANCE_HATCHET:
		return PlayerFPVUpperAnimation_ToHatchet();
	case CPlayer::P_STANCE_SPEAR:
		return PlayerFPVUpperAnimation_ToSpear();
	case CPlayer::P_STANCE_BOAL:
		return PlayerFPVUpperAnimation_ToBola();
	case CPlayer::P_STANCE_BOW:
		return PlayerFPVUpperAnimation_ToBow();
	default:
		break;
	}
	return m_eFPVUpperAnimation;
}

CPlayer::PLAYER_ANIMATION CPlayer::PlayerAnimation_ToHatchet()
{
	switch (m_eAimation)
	{
	case CPlayer::P_A_BASIC_AIM_CENTER:
		return P_A_HATCHET_AIM_CENTER;
	case CPlayer::P_A_BASIC_AIM_DOWN:
		return P_A_HATCHET_AIM_DOWN;
	case CPlayer::P_A_BASIC_AIM_DOWN_L:
		return P_A_HATCHET_AIM_DOWN_L;
	case CPlayer::P_A_BASIC_AIM_DOWN_R:
		return P_A_HATCHET_AIM_DOWN_R;
	case CPlayer::P_A_BASIC_AIM_L:
		return P_A_HATCHET_AIM_L;
	case CPlayer::P_A_BASIC_AIM_R:
		return P_A_HATCHET_AIM_R;
	case CPlayer::P_A_BASIC_AIM_UP:
		return P_A_HATCHET_AIM_UP;
	case CPlayer::P_A_BASIC_AIM_UP_L:
		return P_A_HATCHET_AIM_UP_L;
	case CPlayer::P_A_BASIC_AIM_UP_R:
		return P_A_HATCHET_AIM_UP_R;

		//case CPlayer::P_A_BASIC_CROUCH_BWD:
		//	return P_A_SPEAR_CROUCH_BWD;
		//case CPlayer::P_A_BASIC_CROUCH_BWD_L:
		//	return P_A_SPEAR_CROUCH_BWD_L;
		//case CPlayer::P_A_BASIC_CROUCH_BWD_R:
		//	return P_A_SPEAR_CROUCH_BWD_R;
		//case CPlayer::P_A_BASIC_CROUCH_FWD:
		//	return P_A_SPEAR_CROUCH_FWD;
		//case CPlayer::P_A_BASIC_CROUCH_FWD_L:
		//	return P_A_SPEAR_CROUCH_FWD_L;
		//case CPlayer::P_A_BASIC_CROUCH_FWD_R:
		//	return P_A_SPEAR_CROUCH_FWD_R;
		//case CPlayer::P_A_BASIC_CROUCH_IDLE:
		//	return P_A_SPEAR_CROUCH_IDLE;
		//case CPlayer::P_A_BASIC_CROUCH_L:
		//	return P_A_SPEAR_CROUCH_L;
		//case CPlayer::P_A_BASIC_CROUCH_R:
		//	return P_A_SPEAR_CROUCH_R;

	case CPlayer::P_A_BASIC_MOVE_BWD:
		return P_A_HATCHET_MOVE_BWD;
	case CPlayer::P_A_BASIC_MOVE_BWD_L:
		return P_A_HATCHET_MOVE_BWD_L;
	case CPlayer::P_A_BASIC_MOVE_BWD_R:
		return P_A_HATCHET_MOVE_BWD_R;
	case CPlayer::P_A_BASIC_MOVE_FWD:
		return P_A_HATCHET_MOVE_FWD;
	case CPlayer::P_A_BASIC_MOVE_FWD_L:
		return P_A_HATCHET_MOVE_FWD_L;
	case CPlayer::P_A_BASIC_MOVE_FWD_R:
		return P_A_HATCHET_MOVE_FWD_R;
	case CPlayer::P_A_BASIC_MOVE_L:
		return P_A_HATCHET_MOVE_L;
	case CPlayer::P_A_BASIC_MOVE_R:
		return P_A_HATCHET_MOVE_R;

		//case CPlayer::P_A_BASIC_JUMP_UP:
		//	return;
	case CPlayer::P_A_BASIC_JUMP_FALL:
		return P_A_HATCHET_JUMPFALL;
		//case CPlayer::P_A_BASIC_JUMP_LAND:
		//	return;

	case CPlayer::P_A_BASIC_PRONE_IDLE:
		return P_A_HATCHET_PRONE_IDLE;
	case CPlayer::P_A_BASIC_PRONE_IN:
		return P_A_HATCHET_PRONE_IN;
	case CPlayer::P_A_BASIC_PRONE_MOVE_FWD:
		return P_A_HATCHET_PRONE_MOVE_FWD;
	case CPlayer::P_A_BASIC_PRONE_OUT:
		return P_A_HATCHET_PRONE_OUT;
	case CPlayer::P_A_BASIC_HURT:
		return P_A_HATCHET_HIT;
	case CPlayer::P_A_BASIC_IDLE:
		return P_A_HATCHET_IDLE;
	case CPlayer::P_A_BASIC_SPRINT:
		return P_A_HATCHET_SPRINT;
	case CPlayer::P_A_BASIC_TURN_RIGHT:
		return P_A_HATCHET_TURN_RIGHT;

	default:
		break;
	}

	return m_eAimation;
}

CPlayer::PLAYER_ANIMATION CPlayer::PlayerUpperAnimation_ToHatchet()
{
	switch (m_eUpperAnimation)
	{
	case CPlayer::P_A_BASIC_AIM_CENTER:
		return P_A_HATCHET_AIM_CENTER;
	case CPlayer::P_A_BASIC_AIM_DOWN:
		return P_A_HATCHET_AIM_DOWN;
	case CPlayer::P_A_BASIC_AIM_DOWN_L:
		return P_A_HATCHET_AIM_DOWN_L;
	case CPlayer::P_A_BASIC_AIM_DOWN_R:
		return P_A_HATCHET_AIM_DOWN_R;
	case CPlayer::P_A_BASIC_AIM_L:
		return P_A_HATCHET_AIM_L;
	case CPlayer::P_A_BASIC_AIM_R:
		return P_A_HATCHET_AIM_R;
	case CPlayer::P_A_BASIC_AIM_UP:
		return P_A_HATCHET_AIM_UP;
	case CPlayer::P_A_BASIC_AIM_UP_L:
		return P_A_HATCHET_AIM_UP_L;
	case CPlayer::P_A_BASIC_AIM_UP_R:
		return P_A_HATCHET_AIM_UP_R;

	case P_A_BASIC_ATTACK:
		if ((L"Layer_Harvest_Tree" == m_wstrTargetLayer || L"Layer_Harvest_Rock" == m_wstrTargetLayer) &&
			(L"Layer_Weapon_Hatchet" == m_wstrLeftHand || L"Layer_Weapon_StonePick" == m_wstrLeftHand))
			return P_A_HATCHET_HIT;		// 채집 모션
		return P_A_HATCHET_SWING;			// 허공에 휘두르기
	case CPlayer::P_A_BASIC_EQUIP_COPY:
		if (STATE_WEAPON_DRAW == m_eUpperPlayerState)
			return P_A_HATCHET_DRAW;
		else if (STATE_WEAPON_PUTAWAY == m_eUpperPlayerState)
			return P_A_HATCHET_PUTAWAY;
	default:
		break;
	}

	return m_eUpperAnimation;
}

CPlayer::PLAYER_ANIMATION_FPV CPlayer::PlayerFPVAnimation_ToHatchet()
{
	switch (m_eFPVAnimation)
	{
	case CPlayer::P_A_FPV_BASIC_MOVE:
	case CPlayer::P_A_FPV_SPEAR_SPRINT:
		return P_A_FPV_HATCHET_MOVE;
	case CPlayer::P_A_FPV_BASIC_JUMPIDLE:
	case CPlayer::P_A_FPV_SPEAR_IDLE:
		return P_A_FPV_HATCHET_IDLE;
	case CPlayer::P_A_FPV_BASIC_SPRINT:
		return P_A_FPV_HATCHET_SPRINT;
	default:
		break;
	}
	return m_eFPVAnimation;
}

CPlayer::PLAYER_ANIMATION_FPV CPlayer::PlayerFPVUpperAnimation_ToHatchet()
{
	switch (m_eFPVUpperAnimation)
	{
	case CPlayer::P_A_FPV_BASIC_FISTSEQUIP:
		return P_A_FPV_HATCHET_DRAW;
	case CPlayer::P_A_FPV_BASIC_FISTSUNEQUIP:
		return P_A_FPV_HATCHET_PUTAWAY;
	case CPlayer::P_A_FPV_BASIC_ATTACK_LEFT_1:
	case CPlayer::P_A_FPV_BASIC_ATTACK_LEFT_2:
	case CPlayer::P_A_FPV_BASIC_ATTACK_RIGHT_1:
	case CPlayer::P_A_FPV_BASIC_ATTACK_RIGHT_2:
		if ((L"Layer_Harvest_Tree" == m_wstrTargetLayer || L"Layer_Harvest_Rock" == m_wstrTargetLayer) &&
			(L"Layer_Weapon_Hatchet" == m_wstrLeftHand || L"Layer_Weapon_StonePick" == m_wstrLeftHand))
			return P_A_FPV_HATCHET_HIT;		// 채집 모션
		return P_A_FPV_HATCHET_SWING;		// 허공에 휘두르기
	default:
		break;
	}
	return m_eFPVUpperAnimation;
}

CPlayer::PLAYER_ANIMATION CPlayer::PlayerAnimation_ToSpear()
{
	switch (m_eAimation)
	{
	case CPlayer::P_A_BASIC_AIM_CENTER:
		return P_A_SPEAR_AIM_CENTER;
	case CPlayer::P_A_BASIC_AIM_DOWN:
		return P_A_SPEAR_AIM_DOWN;
	case CPlayer::P_A_BASIC_AIM_DOWN_L:
		return P_A_SPEAR_AIM_DOWN_L;
	case CPlayer::P_A_BASIC_AIM_DOWN_R:
		return P_A_SPEAR_AIM_DOWN_R;
	case CPlayer::P_A_BASIC_AIM_L:
		return P_A_SPEAR_AIM_L;
	case CPlayer::P_A_BASIC_AIM_R:
		return P_A_SPEAR_AIM_R;
	case CPlayer::P_A_BASIC_AIM_UP:
		return P_A_SPEAR_AIM_UP;
	case CPlayer::P_A_BASIC_AIM_UP_L:
		return P_A_SPEAR_AIM_UP_L;
	case CPlayer::P_A_BASIC_AIM_UP_R:
		return P_A_SPEAR_AIM_UP_R;

	case CPlayer::P_A_BASIC_CROUCH_BWD:
		return P_A_SPEAR_CROUCH_BWD;
	case CPlayer::P_A_BASIC_CROUCH_BWD_L:
		return P_A_SPEAR_CROUCH_BWD_L;
	case CPlayer::P_A_BASIC_CROUCH_BWD_R:
		return P_A_SPEAR_CROUCH_BWD_R;
	case CPlayer::P_A_BASIC_CROUCH_FWD:
		return P_A_SPEAR_CROUCH_FWD;
	case CPlayer::P_A_BASIC_CROUCH_FWD_L:
		return P_A_SPEAR_CROUCH_FWD_L;
	case CPlayer::P_A_BASIC_CROUCH_FWD_R:
		return P_A_SPEAR_CROUCH_FWD_R;
	case CPlayer::P_A_BASIC_CROUCH_IDLE:
		return P_A_SPEAR_CROUCH_IDLE;
	case CPlayer::P_A_BASIC_CROUCH_L:
		return P_A_SPEAR_CROUCH_L;
	case CPlayer::P_A_BASIC_CROUCH_R:
		return P_A_SPEAR_CROUCH_R;

	case CPlayer::P_A_BASIC_MOVE_BWD:
		return P_A_SPEAR_MOVE_BWD;
	case CPlayer::P_A_BASIC_MOVE_BWD_L:
		return P_A_SPEAR_MOVE_BWD_L;
	case CPlayer::P_A_BASIC_MOVE_BWD_R:
		return P_A_SPEAR_MOVE_BWD_R;
	case CPlayer::P_A_BASIC_MOVE_FWD:
		return P_A_SPEAR_MOVE_FWD;
	case CPlayer::P_A_BASIC_MOVE_FWD_L:
		return P_A_SPEAR_MOVE_FWD_L;
	case CPlayer::P_A_BASIC_MOVE_FWD_R:
		return P_A_SPEAR_MOVE_FWD_R;
	case CPlayer::P_A_BASIC_MOVE_L:
		return P_A_SPEAR_MOVE_L;
	case CPlayer::P_A_BASIC_MOVE_R:
		return P_A_SPEAR_MOVE_R;

	case CPlayer::P_A_BASIC_JUMP_UP:
		return P_A_RIFLE_JUMP;
	case CPlayer::P_A_BASIC_JUMP_FALL:
		return P_A_SPEAR_JUMPFALL;
	case CPlayer::P_A_BASIC_JUMP_LAND:
		return P_A_RIFLE_JUMPLAND;

	case CPlayer::P_A_BASIC_PRONE_IDLE:
		return P_A_SPEAR_PRONE_IDLE;
	case CPlayer::P_A_BASIC_PRONE_IN:
		return P_A_SPEAR_PRONE_IN;
	case CPlayer::P_A_BASIC_PRONE_MOVE_FWD:
		return P_A_SPEAR_PRONE_FWD;
	case CPlayer::P_A_BASIC_PRONE_OUT:
		return P_A_SPEAR_PRONE_OUT;

		//case CPlayer::P_A_BASIC_HURT:
		//	return P_A_SPEAR_HIT;
	case CPlayer::P_A_BASIC_IDLE:
		return P_A_SPEAR_IDLE;
	case CPlayer::P_A_BASIC_SPRINT:
		return P_A_SPEAR_SPRINT;
	case CPlayer::P_A_BASIC_TURN_RIGHT:
		return P_A_SPEAR_TURN_RIGHT;

	default:
		break;
	}

	return m_eAimation;
}

CPlayer::PLAYER_ANIMATION CPlayer::PlayerUpperAnimation_ToSpear()
{
	switch (m_eUpperAnimation)
	{
	case CPlayer::P_A_BASIC_AIM_CENTER:
		return P_A_SPEAR_AIM_CENTER;
	case CPlayer::P_A_BASIC_AIM_DOWN:
		return P_A_SPEAR_AIM_DOWN;
	case CPlayer::P_A_BASIC_AIM_DOWN_L:
		return P_A_SPEAR_AIM_DOWN_L;
	case CPlayer::P_A_BASIC_AIM_DOWN_R:
		return P_A_SPEAR_AIM_DOWN_R;
	case CPlayer::P_A_BASIC_AIM_L:
		return P_A_SPEAR_AIM_L;
	case CPlayer::P_A_BASIC_AIM_R:
		return P_A_SPEAR_AIM_R;
	case CPlayer::P_A_BASIC_AIM_UP:
		return P_A_SPEAR_AIM_UP;
	case CPlayer::P_A_BASIC_AIM_UP_L:
		return P_A_SPEAR_AIM_UP_L;
	case CPlayer::P_A_BASIC_AIM_UP_R:
		return P_A_SPEAR_AIM_UP_R;

	case P_A_BASIC_ATTACK:
		return P_A_SPEAR_MELEE;

	case CPlayer::P_A_BASIC_EQUIP_COPY:
		if (STATE_WEAPON_DRAW == m_eUpperPlayerState)
			return P_A_SPEAR_PULLBACK_RELOAD;
		else if (STATE_WEAPON_PUTAWAY == m_eUpperPlayerState)
			return P_A_HATCHET_PUTAWAY;
	default:
		break;
	}

	return m_eUpperAnimation;
}

CPlayer::PLAYER_ANIMATION_FPV CPlayer::PlayerFPVAnimation_ToSpear()
{
	switch (m_eFPVAnimation)
	{
	case CPlayer::P_A_FPV_HATCHET_MOVE:
	case CPlayer::P_A_FPV_BASIC_MOVE:
		return P_A_FPV_SPEAR_SPRINT;
	case CPlayer::P_A_FPV_HATCHET_IDLE:
	case CPlayer::P_A_FPV_BASIC_JUMPIDLE:
		return P_A_FPV_SPEAR_IDLE;
	case CPlayer::P_A_FPV_HATCHET_SPRINT:
	case CPlayer::P_A_FPV_BASIC_SPRINT:
		return P_A_FPV_SPEAR_SPRINT;
	default:
		break;
	}

	return m_eFPVAnimation;
}

CPlayer::PLAYER_ANIMATION_FPV CPlayer::PlayerFPVUpperAnimation_ToSpear()
{
	switch (m_eFPVUpperAnimation)
	{
	case CPlayer::P_A_FPV_BASIC_FISTSEQUIP:
		return P_A_FPV_SPEAR_EQUIP;
	case CPlayer::P_A_FPV_BASIC_FISTSUNEQUIP:
		return P_A_FPV_SPEAR_UNEQUIP;
	case CPlayer::P_A_FPV_BASIC_ATTACK_LEFT_1:
	case CPlayer::P_A_FPV_BASIC_ATTACK_LEFT_2:
	case CPlayer::P_A_FPV_BASIC_ATTACK_RIGHT_1:
	case CPlayer::P_A_FPV_BASIC_ATTACK_RIGHT_2:
		return P_A_FPV_SPEAR_ATTACK;
	default:
		break;
	}
	return m_eFPVUpperAnimation;
}

CPlayer::PLAYER_ANIMATION_FPV CPlayer::PlayerFPVAnimation_ToBola()
{
	switch (m_eFPVAnimation)
	{
	case CPlayer::P_A_FPV_HATCHET_MOVE:
	case CPlayer::P_A_FPV_BASIC_MOVE:
		return P_A_FPV_BOLA_WALK;
	case CPlayer::P_A_FPV_HATCHET_IDLE:
	case CPlayer::P_A_FPV_BASIC_JUMPIDLE:
		return P_A_FPV_BOLA_IDLE;
	case CPlayer::P_A_FPV_HATCHET_SPRINT:
	case CPlayer::P_A_FPV_BASIC_SPRINT:
		return P_A_FPV_BOLA_RUN;
	default:
		break;
	}

	return m_eFPVAnimation;
}

CPlayer::PLAYER_ANIMATION_FPV CPlayer::PlayerFPVUpperAnimation_ToBola()
{
	switch (m_eFPVUpperAnimation)
	{
	case CPlayer::P_A_FPV_BASIC_FISTSEQUIP:
		return P_A_FPV_BOLA_EQUIP;
	case CPlayer::P_A_FPV_BASIC_FISTSUNEQUIP:
		return P_A_FPV_BOLA_UNEQUIP;
	case CPlayer::P_A_FPV_BASIC_ATTACK_LEFT_1:
	case CPlayer::P_A_FPV_BASIC_ATTACK_LEFT_2:
	case CPlayer::P_A_FPV_BASIC_ATTACK_RIGHT_1:
	case CPlayer::P_A_FPV_BASIC_ATTACK_RIGHT_2:
		return P_A_FPV_BOLA_MELEE;
	default:
		break;
	}
	return m_eFPVUpperAnimation;
}

CPlayer::PLAYER_ANIMATION CPlayer::PlayerAnimation_ToBow()
{
	switch (m_eAimation)
	{
	case CPlayer::P_A_BASIC_AIM_CENTER:
		return P_A_BOW_AIM_CENTER;
	case CPlayer::P_A_BASIC_AIM_DOWN:
		return P_A_BOW_AIM_DOWN;
	case CPlayer::P_A_BASIC_AIM_DOWN_L:
		return P_A_BOW_AIM_DOWN_L;
	case CPlayer::P_A_BASIC_AIM_DOWN_R:
		return P_A_BOW_AIM_DOWN_R;
	case CPlayer::P_A_BASIC_AIM_L:
		return P_A_BOW_AIM_L;
	case CPlayer::P_A_BASIC_AIM_R:
		return P_A_BOW_AIM_R;
	case CPlayer::P_A_BASIC_AIM_UP:
		return P_A_BOW_AIM_UP;
	case CPlayer::P_A_BASIC_AIM_UP_L:
		return P_A_BOW_AIM_UP_L;
	case CPlayer::P_A_BASIC_AIM_UP_R:
		return P_A_BOW_AIM_UP_R;

	case CPlayer::P_A_BASIC_CROUCH_BWD:
		return P_A_BOW_CROUCH_BWD;
	case CPlayer::P_A_BASIC_CROUCH_BWD_L:
		return P_A_BOW_CROUCH_BWD_L;
	case CPlayer::P_A_BASIC_CROUCH_BWD_R:
		return P_A_BOW_CROUCH_BWD_R;
	case CPlayer::P_A_BASIC_CROUCH_FWD:
		return P_A_BOW_CROUCH_FWD;
	case CPlayer::P_A_BASIC_CROUCH_FWD_L:
		return P_A_BOW_CROUCH_FWD_L;
	case CPlayer::P_A_BASIC_CROUCH_FWD_R:
		return P_A_BOW_CROUCH_FWD_R;
	case CPlayer::P_A_BASIC_CROUCH_IDLE:
		return P_A_BOW_CROUCH_IDLE;
	case CPlayer::P_A_BASIC_CROUCH_L:
		return P_A_BOW_CROUCH_L;
	case CPlayer::P_A_BASIC_CROUCH_R:
		return P_A_BOW_CROUCH_R;

	case CPlayer::P_A_BASIC_MOVE_BWD:
		return P_A_BOW_MOVE_BWD;
	case CPlayer::P_A_BASIC_MOVE_BWD_L:
		return P_A_BOW_MOVE_BWD_L;
	case CPlayer::P_A_BASIC_MOVE_BWD_R:
		return P_A_BOW_MOVE_BWD_R;
	case CPlayer::P_A_BASIC_MOVE_FWD:
		return P_A_BOW_MOVE_FWD;
	case CPlayer::P_A_BASIC_MOVE_FWD_L:
		return P_A_BOW_MOVE_FWD_L;
	case CPlayer::P_A_BASIC_MOVE_FWD_R:
		return P_A_BOW_MOVE_FWD_R;
	case CPlayer::P_A_BASIC_MOVE_L:
		return P_A_BOW_MOVE_L;
	case CPlayer::P_A_BASIC_MOVE_R:
		return P_A_BOW_MOVE_R;

	case CPlayer::P_A_BASIC_JUMP_UP:
		return P_A_BOW_JUMP;			//
	case CPlayer::P_A_BASIC_JUMP_FALL:
		return P_A_BOW_JUMPFALL;
	case CPlayer::P_A_BASIC_JUMP_LAND:
		return P_A_BOW_JUMPLAND;		//

	case CPlayer::P_A_BASIC_PRONE_IDLE:
		return P_A_BOW_PRONE_IDLE;
	case CPlayer::P_A_BASIC_PRONE_IN:
		return P_A_BOW_PRONE_IN;
	case CPlayer::P_A_BASIC_PRONE_MOVE_FWD:
		return P_A_BOW_PRONE_FWD;
	case CPlayer::P_A_BASIC_PRONE_OUT:
		return P_A_BOW_PRONE_OUT;

		//case CPlayer::P_A_BASIC_HURT:
		//	return P_A_SPEAR_HIT;
	case CPlayer::P_A_BASIC_IDLE:
		return P_A_BOW_IDLE;
	case CPlayer::P_A_BASIC_SPRINT:
		return P_A_BOW_SPRINT;
	case CPlayer::P_A_BASIC_TURN_RIGHT:
		return P_A_BOW_TURN_RIGHT;

	default:
		break;
	}

	return m_eAimation;
}

CPlayer::PLAYER_ANIMATION CPlayer::PlayerUpperAnimation_ToBow()
{
	switch (m_eUpperAnimation)
	{
	case CPlayer::P_A_BASIC_AIM_CENTER:
		return P_A_BOW_AIM_CENTER;
	case CPlayer::P_A_BASIC_AIM_DOWN:
		return P_A_BOW_AIM_DOWN;
	case CPlayer::P_A_BASIC_AIM_DOWN_L:
		return P_A_BOW_AIM_DOWN_L;
	case CPlayer::P_A_BASIC_AIM_DOWN_R:
		return P_A_BOW_AIM_DOWN_R;
	case CPlayer::P_A_BASIC_AIM_L:
		return P_A_BOW_AIM_L;
	case CPlayer::P_A_BASIC_AIM_R:
		return P_A_BOW_AIM_R;
	case CPlayer::P_A_BASIC_AIM_UP:
		return P_A_BOW_AIM_UP;
	case CPlayer::P_A_BASIC_AIM_UP_L:
		return P_A_BOW_AIM_UP_L;
	case CPlayer::P_A_BASIC_AIM_UP_R:
		return P_A_BOW_AIM_UP_R;

	case P_A_BASIC_ATTACK:
		return P_A_BOW_MELEE;

	case CPlayer::P_A_BASIC_EQUIP_COPY:
		if (STATE_WEAPON_DRAW == m_eUpperPlayerState)
			return P_A_BOW_DRAW;
		else if (STATE_WEAPON_PUTAWAY == m_eUpperPlayerState)
			return P_A_BOW_PUTAWAY;
	default:
		break;
	}

	return m_eUpperAnimation;
}

CPlayer::PLAYER_ANIMATION_FPV CPlayer::PlayerFPVAnimation_ToBow()
{
	switch (m_eFPVAnimation)
	{
	case CPlayer::P_A_FPV_HATCHET_MOVE:
	case CPlayer::P_A_FPV_BASIC_MOVE:
		return P_A_FPV_BOW_MOVE;
	case CPlayer::P_A_FPV_HATCHET_IDLE:
	case CPlayer::P_A_FPV_BASIC_JUMPIDLE:
		return P_A_FPV_BOW_IDLE;
	case CPlayer::P_A_FPV_HATCHET_SPRINT:
	case CPlayer::P_A_FPV_BASIC_SPRINT:
		return P_A_FPV_BOW_SPRINT;
	default:
		break;
	}

	return m_eFPVAnimation;
}

CPlayer::PLAYER_ANIMATION_FPV CPlayer::PlayerFPVUpperAnimation_ToBow()
{
	switch (m_eFPVUpperAnimation)
	{
	case CPlayer::P_A_FPV_BASIC_FISTSEQUIP:
		return P_A_FPV_BOW_DRAW;
	case CPlayer::P_A_FPV_BASIC_FISTSUNEQUIP:
		return P_A_FPV_BOW_PUTAWAY;
	case CPlayer::P_A_FPV_BASIC_ATTACK_LEFT_1:
	case CPlayer::P_A_FPV_BASIC_ATTACK_LEFT_2:
	case CPlayer::P_A_FPV_BASIC_ATTACK_RIGHT_1:
	case CPlayer::P_A_FPV_BASIC_ATTACK_RIGHT_2:
		return P_A_FPV_BOW_MELEE;
	default:
		break;
	}
	return m_eFPVUpperAnimation;
}

_bool CPlayer::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	if (m_bIsRide || STATE_DEAD == m_eUnitState)
		return false;

	_bool bHit = CUnit::HitBy(pAttacker, dwAttackType, vCollisionPos);

	if (bHit)
	{
		//화면번쩍이는이펙트 //<<피격이펙트출력>>_vec3 _vPos,_vec3 _vAtkPos
		_vec3 pPos,pAtkPos = { 0.f,0.f,0.f };
		pPos = *m_pTransformCom->Get_Pos();
		pAtkPos = vCollisionPos;
		static_cast<CPlayerHit_UI*>(Engine::Get_GameObject(L"PlayerHit_UI", 0))->Player_Hit_On(pPos, pAtkPos);
		m_pAttacker = pAttacker;

		if (STATE_IDLE == m_eUpperPlayerState)
			m_eUpperPlayerState = STATE_HURT;
	}
	return bHit;
}

void CPlayer::Check_Stat(const _double & _dTimeDelta)
{
	if (!m_bIsStatUpdate)
		return;

	//m_tStat.fHunger = 100.f;
	if (0.3f > m_tStat.fHp / m_tStat.fMaxHp)
		static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Red_LessHealth);
	else if (30.f > m_tStat.fHunger)
		static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Red_LessHunger);
	else if (20.f > m_tStat.fStamina)
		static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Red_LessStamina);

	// 배고픔
	if (0.f >= m_tStat.fHunger)							// 배고픔이 0이면 HP 초당 2 감소
	{
		//m_tStat.fHp -= 2.f * (_float)_dTimeDelta;
	}
	else
	{
		m_tStat.fHunger -= 0.5f * (_float)_dTimeDelta;			// 배고픔 초당 0.5 감소
		m_tStat.fHp += 1.f * (_float)_dTimeDelta;
		if (m_tStat.fMaxHp <= m_tStat.fHp)
			m_tStat.fHp = m_tStat.fMaxHp;

		if (0.f >= m_tStat.fHunger)						// 배고음 0 ~ max 제한
			m_tStat.fHunger = 0.f;
		else if (m_tStat.fMaxHunger <= m_tStat.fHunger)
			m_tStat.fHunger = m_tStat.fMaxHunger;
		else if (m_tStat.fMaxHp > m_tStat.fHp)
		{
			// 배고픔을 HP로 전환
			m_tStat.fHp += (_float)_dTimeDelta;
			m_tStat.fHunger -= (_float)_dTimeDelta;

			if (m_tStat.fHp > m_tStat.fMaxHp)
				m_tStat.fHp = m_tStat.fMaxHp;
		}
	}

	// 쉬고 있을 때 기력 증가
	if (!m_bRecoveryStamina)
	{
		m_dRecoveryStaminaTime += _dTimeDelta;
		if (2 < m_dRecoveryStaminaTime)
		{
			m_dRecoveryStaminaTime = 0.0;
			m_bRecoveryStamina = true;
		}
	}

	if (m_bRecoveryStamina && m_tStat.fMaxStamina > m_tStat.fStamina)
	{
		m_tStat.fStamina += 5.f * (_float)_dTimeDelta;

		if (0.f >= m_tStat.fStamina)
			m_tStat.fStamina = 0.f;
		else if (m_tStat.fMaxStamina <= m_tStat.fStamina)
			m_tStat.fStamina = m_tStat.fMaxStamina;
	}
}

void CPlayer::Init_LevelUpTable()
{
	//최초 레벨 1
	// 1 -> 2레벨 필경 50 //누적 50
	m_vecExpRequire.emplace_back(0.f);
	m_vecExpStack.emplace_back(0.f);	//0레벨

	m_vecExpRequire.emplace_back(0.f);
	m_vecExpStack.emplace_back(0.f);	//1

	m_vecExpRequire.emplace_back(50.f);
	m_vecExpStack.emplace_back(m_vecExpRequire.back());	//1 -> 2레벨

	_int i = 3;
	_float fStack = m_vecExpRequire.back();
	_float fValue = 0.6f;
	for (; i < 100; i++)
	{
		_float fBefore = m_vecExpRequire.back();	//이전레벨 필요경험치
		_float fRequire = fStack + fBefore * (fValue * 0.8f);	//현재레벨 필요경험치 = 누적경험치 + 이전레벨 필요경험치에 증가폭 곱한 수치
		m_vecExpRequire.emplace_back(fRequire);	//현재레벨 필요경험치 갱신
		fStack += fRequire;
		m_vecExpStack.emplace_back(fStack);	//현재레벨 누적경험치 갱신
	}
	return;
}

_bool CPlayer::Check_LevelUp()
{
	_int iLevel = (_int)m_tStat.fLevel;
	if (99 <= iLevel)
		return false;
	_float fNextLevel = m_vecExpStack[iLevel + 1];
	if (fNextLevel < m_tStat.fEXP)
	{
		m_tStat.fLevel = _float(iLevel + 1);
		m_tStat.fStatPoint += 1.f;
		return true;
	}
	return false;
}

_float CPlayer::Get_NextExp()
{
	if (m_vecExpStack.size() <= _int(m_tStat.fLevel) + 1)
		return -1.f;

	return m_vecExpStack[_int(m_tStat.fLevel) + 1];
}
