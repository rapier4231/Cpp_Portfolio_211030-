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
	///////////////////////////////////////// �׽�Ʈ ��
	m_pInvenCom->Set_InvenUserUnitTypeID(InvenUserUnitTypeID_Player);
	//�÷��̾�� ����, ����, ����, �� ������ �κ��� ������ Ű����� unit ��ü����
	// static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", L"Right_Inventory"))->Set_InvenOwner(this);
	//�̰� �ҷ��ָ� ��

	//�κ��� ������ �ֱ�
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

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Campfire, 1));			// ��ں�
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_SleepingBag, 1));			// 1ȸ�� ħ��
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_MortarPestle, 1));		// �����Ͱ���
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Forge, 1));				// ȭ��
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_Smithy, 1));				// ���尣
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchBed, 1));			// ¤ ħ��

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

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchFloor, 1));			// ¤ ���
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchWall, 1));			// ¤ ��
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchRoof, 1));			// ¤ õ��
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchDoorFrame, 1));		// ¤ �� ������
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_ThatchDoor, 1));			// ¤ ��

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneFloor, 1));			// ¤ ���
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneWall, 1));			// ¤ ��
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneRoof, 1));			// ¤ õ��
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneDoorFrame, 1));		// ¤ �� ������
	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneDoor, 1));			// ¤ ��


	// ��
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

	m_pInvenCom->Add_Item(Engine::Clone_Item_Prototype(ITEMID_StoneGateFrame, 1));			// ���� ����Ʈ ������
	/////////////////////////////////////////

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vPlayerPos = _vec3(20.f, 14.6f, -15.f);

	// 1��Ī
	m_pTransformFPVCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformFPVCom->Set_Pos(m_vPlayerPos);
	m_pTransformFPVCom->Update_Component(0.0);

	// 3��Ī
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(m_vPlayerPos);
	m_pTransformCom->Update_Component(0.0);

	// ����¸�ŭ bool �� �����Ҵ�
	m_bIsFPVSubsetRender = new _bool[m_pMeshFPVCom->Get_MeshList()->size()];
	ZeroMemory(m_bIsFPVSubsetRender, sizeof(_bool) * m_pMeshFPVCom->Get_MeshList()->size());
	m_bIsFPVSubsetRender[0] = true;		// ��ü�� �׸���

	// 3��Ī
	m_bIsTPVSubsetRender = new _bool[m_pMeshCom->Get_MeshList()->size()];
	ZeroMemory(m_bIsTPVSubsetRender, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsTPVSubsetRender[0] = true;		// ��

	m_bIsTPVSubsetRender[1] = true;		// �������
	//m_bIsTPVSubsetRender[2] = false;		// ���� �Ź�
	//m_bIsTPVSubsetRender[3] = false;		// ���� �尩
	//m_bIsTPVSubsetRender[4] = false;		// ���� ����
	//m_bIsTPVSubsetRender[5] = false;		// ���� ����
	//m_bIsTPVSubsetRender[6] = false;		// ��ź �Ź�
	//m_bIsTPVSubsetRender[7] = false;		// ��ź �尩
	//m_bIsTPVSubsetRender[8] = false;		// ��ź ����
	//m_bIsTPVSubsetRender[9] = false;		// ��ź ����

	/// �÷��̾� ���� ����
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

	/// �÷��̾� ���� ����
	/// ù���� ���۽� ī�޶� ����
	m_bIsFPVCam = true;
	m_eUnitState = STATE_IDLE;
	m_eUpperPlayerState = STATE_IDLE;
	/////////
	// ����
	//m_eUnitState = STATE_CAMERAWORK;
	//m_eUpperPlayerState = STATE_CAMERAWORK;

	// �ִϸ��̼�
	// ���ӽ��۽� 1��Ī ���

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

	// ���������� �ӽ��Լ� ////////////////////////////////////////////////////////////////////////
	//Init_LevelUpTable();
	//////////////////////////////////////////////////////////////////////////

	//static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_CameraType(CCameraMrg::CAMERA_FPV);

	////////////////////////////////////���� �Ŵ��� �÷��̾� ��ġ
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


	// ġƮ�ڵ� //////////////////////////////////////////////////////////////////////////
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
		g_bInstancing = !g_bInstancing;	//�ν��Ͻ� �Ѱ� ����
	//////////////////////////////////////////////////////////////////////////

	/// �÷��̾� ����, ��Ȱ ó��
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

	/// �÷��̾� ��ġ ��������
	m_pTransformCom->Get_Info(INFO_POS, &m_vPlayerPos);

	/// ���濡 ����� ������Ʈ ���̾� Ȯ��
	//if (!m_bIsFPVCam)
	Check_PlayerLookLayer();

	/// Ű�Է�
	if (!m_bIsRide)
	{
		/// �����Ž
		// Ű�Է�
		if (STATE_DEAD != m_eUnitState)
			Key_Input(_dTimeDelta);

		/// Ư�� �ִϸ��̼� �ص� üũ
		Check_IsEndAnimation();
		Check_IsEndUpperAnimation(_dTimeDelta);


		// �÷��̾� ���� üũ
		Check_PlayerState(_dTimeDelta);

		// ����
		Is_Attack();

		// ����
		Is_Building();

		/// ���� �� ���⿡ �°� �÷��̾� �ִϸ��̼� ����
		m_eAimation = PlayerAnimationByStance();					// ���⺰ �ִϸ��̼�
		m_eUpperAnimation = PlayerUpperAnimationByStance();			// ���⺰ �κ� �ִϸ��̼�
		m_eFPVAnimation = PlayerFPVAnimationByStance();
		m_eFPVUpperAnimation = PlayerFPVUpperAnimationByStance();

		/// �÷��̾� ��ġ �����ϱ� (�̵�)
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
		// �κ��丮
		Key_Input_Inventory(_dTimeDelta);

		/// �������� �� ���� Ű�Է�
		IsRide_Key_Input(_dTimeDelta);
	}

	m_fCreateIntv += (_float)_dTimeDelta * 2.f;

	m_fTime = (_float)_dTimeDelta;

	// �÷��̾� ��ġ �������� ShadowMap ���� ��ġ ���
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

				//������ ���� ž���� ���� ü�� ��� ����
				for (_int i = 0; i < (_int)CSummaryStat_UI_Monster::SummaryStat_END; ++i)
					static_cast<CSummaryStat_UI_Monster*>(Engine::Get_GameObject(L"SummaryStat_UI", ((_int)CSummaryStat_UI::SummaryStat_END) * 2 + (i)))->Set_RideMonster(nullptr);
			}
		}
	}

	// �ִϸ��̼� ����
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
			m_pMeshCom->Set_Animation(m_eAimation);				// ��ü �ִϸ��̼�
		
		if (STATE_IDLE != m_eUpperPlayerState)
			m_pMeshCom->Set_UpperAnimation(m_eUpperAnimation);	// �κ� �ִϸ��̼�
		else
			m_pMeshCom->Set_UpperAnimation(m_eAimation);
	}



	// �����׷� �߰�
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
	// ����ü �ִϸ��̼� ���ÿ� ������
	//m_pMeshCom->Play_Animation(m_dAnimationTime, 1, "Cnt_Spine_002_JNT_SKL");

	////////////////////////////////////////////////////////////////////
	// �ִϸ��̼� �÷���
	//m_pMeshCom->Play_Animation(m_dAnimationTime, 2);

	//////////////////////////////////////////////////////////////////
	//if (P_A_BASIC_IDLE != m_eUpperAimation)
	//	m_pMeshCom->Play_Animation(m_dAnimationTime, 1, "Cnt_Spine_002_JNT_SKL");		// �ִϸ��̼� ���� �÷���
	//else
	//	m_pMeshCom->Play_Animation(m_dAnimationTime);		// ���� �ִϸ��̼� �÷���
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
	//////////////////////////////////////////////////////////////������//////

#ifdef _DEBUG
	_vec3 vPos, vLook, vRight, vEye, vAt, vCamLook;

	if (isnan(vPos.x))
		int a = 0;
	// �÷��̾�
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

	//ī�޶�
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

	//���콺
	POINT Mouse;
	Engine::Get_pMousePos(&Mouse);
	wstring wstrMouse = L"MouseX: " + to_wstring(Mouse.x) + L" / MouseY: " + to_wstring(Mouse.y);
	Engine::Render_Font(L"Font_Debug", wstrMouse, &_vec2(500.f, 5.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


	// HP
	wstring wstrHp = L"HP : " + to_wstring(m_tStat.fHp);
	Engine::Render_Font(L"Font_Debug_Bold", wstrHp, &_vec2(10.f, 550.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	// ����ĳ����
	if (m_bIsPick)
	{
		Engine::Render_Font(L"Font_Debug_Bold", m_wstrTargetLayer, &_vec2(400.f, 300.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		if (m_wstrTargetLayer == L"Layer_Dino" || m_wstrTargetLayer == L"Layer_Dino_Tamed")
		{
			wstring wstrHp = L"HP : " + to_wstring(static_cast<CUnit*>(m_pLookObject)->Get_Stat(CUnit::STAT_HP));
			Engine::Render_Font(L"Font_Debug_Bold", wstrHp, &_vec2(400.f, 250.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
	}

	// �ִϸ��̼�
	wstring wstrAni = L"Ani : " + to_wstring(m_AniTest);
	Engine::Render_Font(L"Font_Debug_Bold", wstrAni, &_vec2(600.f, 300.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	// �ָ� �浹ü
	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Render_SphereCollider();
	// 1��Ī �浹ü
	if (m_pFPVSphereColliderCom)
		m_pFPVSphereColliderCom->Render_SphereCollider();

	// ����
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

	/// �Ž� ������Ʈ
	// 1��Ī
	pComponent = m_pMeshFPVCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Player_Male_FPV"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_MeshFPV", pComponent);

	pComponent = m_pTransformFPVCom = static_cast<CTransform*>(Engine::Clone_ComponentPrototype(L"Proto_Com_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_TransformFPV", pComponent);

	// 3��Ī
	pComponent = m_pMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Player_Male_TPV"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);


	////////////////////////////////////////////////////////////////////////////
	/// �浹ü
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
			// ��� �κ��丮�� ���� ����
			static_cast<CRight_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Set_InvenOwner(static_cast<CUnit*>(pTargetInven));
			static_cast<CMonsterStat_UI*>(Engine::Get_GameObject(L"Center_UI", 1))->Stat_UI_Open(static_cast<CUnit*>(pTargetInven));
			m_bIsOpenInventory = true;
		}
		else
		{
			// �÷��̾� �κ��丮 ����
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
		static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->Stat_UI_Close(); //�κ� ���� �� ����
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

	//m_bIsTPVSubsetRender[1] = true;		// �������
	//m_bIsTPVSubsetRender[2] = false;		// ���� �Ź�
	//m_bIsTPVSubsetRender[3] = false;		// ���� �尩
	//m_bIsTPVSubsetRender[4] = false;		// ���� ����
	//m_bIsTPVSubsetRender[5] = false;		// ���� ����
	//m_bIsTPVSubsetRender[6] = false;		// ��ź �Ź�
	//m_bIsTPVSubsetRender[7] = false;		// ��ź �尩
	//m_bIsTPVSubsetRender[8] = false;		// ��ź ����
	//m_bIsTPVSubsetRender[9] = false;		// ��ź ����
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
	// ������̺� ����
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
		//  ���� Ÿ�� �ִ� ������ Lay���� ����
		if (bDinoRide)
		{
			if (static_cast<CDino*>(pObj)->Get_NowRidingState())
				continue;
		}

		// �ݸ��� ���η� 1�� ���͸�
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
			// �θ���� ���� ��� Ÿ���� ����� ���� �����ش�.
			fColRadius *= pTransform->Get_Scale()->x;
			memcpy(&fColPos, &tColInfo.matColMatrix.m[INFO_POS][0], sizeof(_vec3));
		}
		else
			fColPos = pCollider->Get_InteractPos();

		// �Ÿ��� 2�� ���͸�		
		if (0.f >= fColRadius)
			continue;

		/// ���⼭���� ������ ��ü�浹 ����
		// �� ����� ��������
		_mat matView;
		m_pDevice->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, NULL, &matView);

		// ��ŷ ������ ������ ���� �����
		_vec3 vCamPos, vCamLook;
		memcpy(&vCamPos, &matView.m[INFO_POS][0], sizeof(_vec3));
		memcpy(&vCamLook, &matView.m[INFO_LOOK][0], sizeof(_vec3));
		D3DXVec3Normalize(&vCamLook, &vCamLook);

		vCamPos += vCamLook * fCamDis;

		// ������ ���� ���ϱ� ���� cos �� ���ϱ�
		_vec3 v1 = fColPos - vCamPos;					// ������ �������� ��ü�߽��� �̾��ִ� ����
		_float f2 = D3DXVec3Length(&v1);				// ������ �������� ��ü �߽��� �Ÿ��� ���Ѵ�.

														// �Ÿ��� ���͸�
		if (f2 > _fRayDis + fColRadius)
			continue;

		_float f1 = D3DXVec3Dot(&vCamLook, &v1);		// ������ ���� ���� ������ ���̸� ���Ѵ�.

		if (0.f > f1)
			continue;

		_float fDis = sqrtf((f2 * f2) - (f1 * f1));

		if (fDis < fColRadius)
		{
			if (0.f > fTargetDis)
			{
				// ó�� Ÿ���� ã���� ��
				pTargetObj = pObj;
				fTargetDis = f2;
			}
			else
			{
				// ���� Ÿ���� ã���� ��
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
	// ���� �� ������Ʈ ����
	Safe_Release(m_pLookObject);

	// ī�޶� �־����� �������� �� ä�������� ������
	// �ϴ� ����� �� �� 10.f �־����� 10.f
	// ���� �� ī�޶� �־����� �Ÿ� ������ �װɷ� ����
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
		//���� ���̹��������� ���� �߿� ���̹� ����� �ٸ��ֵ�
		wstring wstrDinoName = static_cast<CDino*>(m_pLookObject)->Get_DinoSpecies();

		if (L"Ankylo" == wstrDinoName)
		{
			if (!m_iTopSngFontID && !static_cast<CDino*>(m_pLookObject)->Get_Dying())
			{
				if (!static_cast<CDino*>(m_pLookObject)->Get_PeaceTaming())
					wstrMsg = L"[E] : ����̱� ���� �Ƹ��� ������ ���Դϴ�.";
				else
				{
					wstrMsg = L"���� �� ������ ���� �ʽ��ϴ�.";
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
		/////////////////////////////////////////ž�� �����ϴٴ� ��Ʈ ����ֱ�
		if (!static_cast<CDino*>(m_pLookObject)->Get_NowRidingState() && static_cast<CDino*>(m_pLookObject)->Get_CanbeRide()) //�÷��̾ Ÿ�� �ִ� ģ���� �ƴ϶��
		{
			if (!m_iTopSngFontID)//���� ž�� �� ����ٸ�
			{
				wstrMsg = L"[E] : [ž��] \n ������ ������ �� ���� �ɼ�";
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
		/////////////////////////////////////////ä�� �����ϴٴ� ��Ʈ ����ֱ�
		if (!m_iTopSngFontID)//���� ä�� �� ����ٸ�
		{
			wstrMsg = L"[E] : [ä��]";
			m_iTopSngFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((static_cast<CDino*>(m_pLookObject))->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrMsg, CWorldFont_UI::World_Font_Big_Blue, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f, AddScreenPos);
		}
		//////////////////////////////////////////////////////////////////////
	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_Harvest_Bush", 1.f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_Harvest_Bush";
		m_pLookObject->AddRef();
		/////////////////////////////////////////ä�� �����ϴٴ� ��Ʈ ����ֱ�
		if (!m_iTopSngFontID && !static_cast<CUnit*>(m_pLookObject)->Get_Dying())//���� ä�� �� ����ٸ�
		{
			wstrMsg = L"[E] : [�ݱ�]";
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
			wstrMsg = L"[E] : [�ݱ�]";
			m_iTopSngFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((static_cast<CDino*>(m_pLookObject))->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrMsg, CWorldFont_UI::World_Font_Big_Blue, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f, AddScreenPos);
		}
	}
	else if (m_pLookObject = Ray_PlayerLook(L"Layer_BasketballHoop", 2.5f + fAddDis))
	{
		m_wstrTargetLayer = L"Layer_BasketballHoop";
		m_pLookObject->AddRef();
		if (!m_bGrabBall)
		{
			wstrMsg = L"[E] : [������ �ֱ�] ";
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

		// �÷��� �������� üũ
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
			// �÷��̾�� ī�޶� ������ �Ÿ� ��������
			_float fCamDis = static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Get_TPVCam_Dis();

			// �� ����� ��������
			_mat matView;
			m_pDevice->GetTransform(D3DTS_VIEW, &matView);
			D3DXMatrixInverse(&matView, NULL, &matView);

			// ��ŷ ������ ������ ���� �����
			_vec3 vCamPos, vCamLook;
			memcpy(&vCamPos, &matView.m[INFO_POS][0], sizeof(_vec3));
			memcpy(&vCamLook, &matView.m[INFO_LOOK][0], sizeof(_vec3));
			D3DXVec3Normalize(&vCamLook, &vCamLook);

			vCamPos += vCamLook * fCamDis;

			vRayPos = PxVec3(vCamPos.x, vCamPos.y, vCamPos.z);
			vRayDir = PxVec3(vCamLook.x, vCamLook.y, vCamLook.z);

			if (!CPhysX_Mgr::GetInstance()->RayCast(vRayPos, vRayDir, 10.f, &vOutPickDis, &vOutPickPos))
			{
				// ����ĳ���ÿ� �������� ���
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


		// ������ �����ֱ�
		if(pStructure)
			Preview_Structure(vOutPickPos, bIsBulidingOK, &fAngleY);
		else
			Preview_Structure(vOutPickPos, bIsBulidingOK);


		// �ǹ����� Ȯ��
		if (Engine::Get_BIMouseDown(DIM_LB))
		{
			if (bIsBulidingOK)
			{
				// �ǹ� ����
				if (pStructure)
					Build_Structure(vOutPickPos, pPlatform, &fAngleY);
				else
					Build_Structure(vOutPickPos, pPlatform);

				// ī�޶� ����ġ
				static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2);

				// ������ �Ҹ�
				//CGameObject* pQuickSlot = Engine::Get_GameObject(L"QuickSlot_UI");
				//static_cast<CQuickSlot_UI*>(pQuickSlot)->UisngRemove_Item(m_iQuickSlot, 1);
			}
			else
			{
				// �ǹ� ���� ���
				// ������ �����
				Is_BuildingCancle();
			}
		}
		else if (Engine::Get_BIMouseDown(DIM_RB))
		{
			// �ǹ� ���� ���
			Is_BuildingCancle();
		}
	}
}

void CPlayer::Is_BuildingCancle()
{
	// ������ �����
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

	// ù �����ӿ� �ٷ� ���� �ʰ� Pre ������Ʈ�� ���
	switch (m_ePreUpperPlayerState)
	{
	case CUnit::STATE_ATTACK:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.1))
		{
			// ���� ����
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
			// ���� �ѹ��ϰ� idle ����
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
	case CUnit::STATE_WEAPON_DRAW:		// ���� ������
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.3))
		{
			m_eUpperPlayerState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_WEAPON_PUTAWAY:	// ���� �ֱ�
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.3))
		{
			// ���� ���� ����ְ� ���� ���Ⱑ ���� ��� STATE_WEAPON_DRAW �� ����
			if (Swap_Weapon())
				m_eUpperPlayerState = STATE_WEAPON_DRAW;
			else
				m_eUpperPlayerState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_WEAPON_THROW:
		if (m_pMeshCom->Is_UpperAnimationsetEnd(0.1))
		{
			/// ������ ���Ⱑ ������ ���
			if (P_STANCE_BOAL == m_ePlayerStance)
			{
				// ���� ������ ����
				Engine::Get_GameObject(m_wstrLeftHand, 0)->Set_Dead();
				m_bIsGrapLeftHand = false;

				// ���� ����ü ����
				Shot_Bullet(BULLET_BOLA);

				// �����Կ� ���� �������� ��� ���ο� ���� ������
				if (ITEMID_END != static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI"))->Get_Item_To_Number(m_iQuickSlot).iItemID)
				{
					Swap_Weapon();
				}
				else
				{
					// ���� ���� ��� ������� ���ƿ���
					m_eUpperPlayerState = STATE_IDLE;
					m_ePlayerStance = P_STANCE_BASIC;
					m_wstrPreLeftHand = m_wstrLeftHand = L"";
				}
			}
			else if (P_STANCE_BOW == m_ePlayerStance)
			{
				// ȭ�� �߻�
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
			// ������ �Ϸ�
			if (P_STANCE_BOW == m_ePlayerStance)
			{
				// Ȱ�� ȭ�� �߰�

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
	// ī�޶� �������̸� �ٷ� ����
	if (STATE_CAMERAWORK == m_eUnitState)
	{
		return;
	}

	// ���콺 ���� (ī�޶� �� �÷��̾� ����)
	Key_Input_MouseMove(_dTimeDelta);

	// �κ��丮
	Key_Input_Inventory(_dTimeDelta);

	/// ��� �÷��̾� �ӵ� ����
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

	/// �̵� (�÷��̾��� �ִϸ��̼� ���� �ʱ�ȭ)
	Key_Input_Move(_dTimeDelta);

	/// �������� �ƴ� �� �ڼ� ���� (C) : ���ֱ� > �ɱ� > ����
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

		/// ��ü ������Ʈ ����
		// ��ȣ�ۿ�
		Key_Input_Interact(_dTimeDelta);

		// ���Ķ�
		Key_Input_Whistle();

		// ���� (���콺 ��Ŭ��) : �������� ��, ���� ��, �ֿ� ��, ������â �� �� �� ���� ����
		Key_Input_Attack(_dTimeDelta);

		// ������ : (������ ����)
		Key_Input_QuickSlot(_dTimeDelta);

		/// �׽�Ʈ
		if (Engine::Get_DIKeyDown(DIK_F1))
		{
			// �ӽ÷� ������� �Լ� (������ �ƴ� ������Ʈ���� ȣ���� ��� ����)
			if (m_pLookObject)
				static_cast<CDino*>(m_pLookObject)->Set_IsTamed();
		}

		//if (Engine::Get_DIKeyState(DIK_R))
		//	g_GlobalTime = 0.2;
		//else
		//	g_GlobalTime = 1.0;
	}

#ifdef _DEBUG
	//// �ִϸ��̼� üũ��
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

	/// ����Ű �Է�
	if (!m_bIsOpenInventory)
	{
		if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			// ������ �̵�
			if (STATE_JUMPING != m_eUnitState)
				m_eUnitState = STATE_MOVE;
			m_eUnitMoveDir = UNIT_DIR_UP;

			_vec3 vLook;
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			vPos -= vLook * (_float)_dTimeDelta * m_fPlayerSpeed;

			// ��������

			if (P_POSE_STAND == m_ePlayerPose && Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80)
			{
				m_dRecoveryStaminaTime = 0.0;
				m_bRecoveryStamina = false;

				if (0.f < m_tStat.fStamina)
				{
					m_eUnitState = STATE_SPRINT;
					//vPos -= vLook * (_float)_dTimeDelta * m_fPlayerSpeed * 1.2f;
					vPos -= vLook * (_float)_dTimeDelta * m_fPlayerSpeed * 10.f;

					// ��°���
					//m_tStat.fStamina -= 10.f * (_float)_dTimeDelta;

					if (0.f >= m_tStat.fStamina)
						m_tStat.fStamina = 0.f;
				}
			}
		}
		else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			// �ڷ� �̵�
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
			// �������� �̵�
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
			// ���������� �̵�
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
		/// ���� Ű �Է�
		if (Get_DIKeyDown(DIK_SPACE) && !m_bJump && STATE_FALLING != m_eUnitState)
		{
			// ���� ����
			m_bJump = true;
			m_fJumpTime = 0.f;
			m_eUnitState = STATE_JUMPING;
		}
	}

	/// �������� �� y �� ����
	m_fJumpPower = 1.6f;
	if (m_bJump)
	{
		m_fJumpTime += (_float)_dTimeDelta * 0.6f;
		vPos.y = (m_fJumpPower * m_fJumpTime - m_fJumpGravity * m_fJumpTime * m_fJumpTime * 0.5f);
	}
	else
	{
		// �������� �ƴ� �� �÷��̾� �Ʒ��� �б�
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
	
	//m_fJumpPower = 4.2f;					// �÷��� Ȯ�ο� ���� �Ŀ�
	//if (Engine::Get_DIKeyState(DIK_F6))
	//	vPos.y += 1.f;
	//
	//if (Engine::Get_DIKeyState(DIK_F7))
	//	vPos.y -= 1.f;
	//m_fJumpPower = 4.2f;					// �÷��� Ȯ�ο� ���� �Ŀ�

	if (Engine::Get_DIKeyState(DIK_F6))
		vPos.y += 1.f;
	
	if (Engine::Get_DIKeyState(DIK_F7))
		vPos.y -= 1.f;

	/// �عٴ� �浹 üũ
	PxFilterData test;
	const PxU32 flags = m_pCtrl->move(vPos, 0.f, (PxF32)_dTimeDelta, PxControllerFilters(&test));

	if (flags & PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		// �ٴڰ� �浹���� ��
		m_fJumpTime = 0.f;
		m_bJump = false;
		m_bTakeOff = false;

		// �������� �� �������� �� �������·� ����
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
		// �ٴڿ� ���� �ȴ���� ��, ���������� ����
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
	
	// ��Ŀ�ǵ� ����ϴ� ���̾��
	if (L"Layer_BasketballHoop" == m_wstrTargetLayer || L"Layer_Dino_Tamed" == m_wstrTargetLayer)
	{
		if (m_dPressETime >= 0.5)
		{
			m_bPressE = false;
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_FixMouseCursor(false);
			m_pRingUnit = static_cast<CUnit*>(m_pLookObject);
			m_pRingUnit->Open_Ring_Command(static_cast<CUnit*>(m_pRingUnit));
		}
		// ��ȣ�ۿ� (E)
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
				// ���� �����ִ� ���� �����Ͱ� ���� ��� ����
				if (nullptr != m_pDino)
					Safe_Release(m_pDino);

				// ���� Ž��
				m_pDino = static_cast<CDino*>(m_pLookObject);

				// ž���� ������ ���� ��� ž��
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

						//������ ���� ž���� ���� ü�� ��� Ű��
						for (_int i = 0; i < (_int)CSummaryStat_UI_Monster::SummaryStat_END; ++i)
							static_cast<CSummaryStat_UI_Monster*>(Engine::Get_GameObject(L"SummaryStat_UI", ((_int)CSummaryStat_UI::SummaryStat_END) * 2 + (i)))->Set_RideMonster(static_cast<CUnit*>(m_pLookObject));
					}
					// ���Ʊ� ����� �䱸���� ����ֱ�
					else if (m_pDino->Get_IsBaby())
					{
						// ó�� �̸� ���� ��Ű��
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
			/// ä��
			// �Ǽ��� ��쿡�� ä�� ����
			if (!m_bIsGrapLeftHand && !static_cast<CUnit*>(m_pLookObject)->Get_Dying())
			{
				// ä�� ����
				m_eUpperPlayerState = STATE_HARVEST;
			}
			else
			{
				// �޼��� ��� (�Ǽ����θ� ä���� �����մϴ�.)
			}
		}
		else if(L"Layer_Ball" == m_wstrTargetLayer && !m_bGrabBall)
		{
			if (nullptr != m_pDino)
				Safe_Release(m_pDino);

			// ���� Ž��
			m_pDino = static_cast<CDino*>(m_pLookObject);
			m_pDino->AddRef();

			m_bGrabBall = true;

			m_pDino->Set_Hanging(true);
			m_pDino->Set_HangingMatrix(&m_pTransformCom->Get_WorldMatrix());
		}
		// ��ȭ���̹��ϴ� ��
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
	// Ư�� ������ �� �ٷ� ����
	if (STATE_ATTACK == m_eUpperPlayerState ||
		STATE_EAT == m_eUpperPlayerState ||
		STATE_HARVEST == m_eUpperPlayerState ||
		STATE_INVENTORY == m_eUpperPlayerState ||
		STATE_ATCTIVATECONSOLE == m_eUpperPlayerState ||
		STATE_WHISTLE == m_eUpperPlayerState ||
		STATE_WEAPON_DRAW == m_eUpperPlayerState ||
		STATE_WEAPON_PUTAWAY == m_eUpperPlayerState)
		return;

	/// ���Ĺ� ��ȣ�ۿ�
	if (Engine::Get_DIKeyDown(DIK_T))
	{
		// ���� ������� �ϱ�
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
		// ���� ������� �ʰ� �ϱ�
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
		// ���׾���
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
		// ��Ÿ��ȿ� ���� ���� �θ���
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
		// ��Ÿ��ȿ� ���� ���� ���߱�
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
		// ũ�ν�����
	}
	else if(m_bAttackCrossHair)
	{
		//����
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
		// ��Ÿ��ȿ� ���� ������ ��ġ�� �̵� (�̿ϼ�)
		
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.2);
	}
	else if (m_bMoveCrossHair)
	{
		m_bMoveCrossHair = false;

		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2);
		// �÷��̾�� ī�޶� ������ �Ÿ� ��������
		_float fCamDis = static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Get_TPVCam_Dis();

		// �� ����� ��������
		_mat matView;
		m_pDevice->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, NULL, &matView);

		// ��ŷ ������ ������ ���� �����
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

	// �κ��丮 (I)
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

		// ��� �κ��丮�� ���� ����
		if (!m_bIsOpenInventory)
		{
			if (L"Layer_Dino" == m_wstrTargetLayer && m_pLookObject)
			{
				// ������ ���� ���̰ų� ���̹� ������ ���� �κ��丮 �� �� �ֵ���
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
				// ������ ����
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
					// ������ �غ�
					m_fThrowPower += (_float)_dTimeDelta;

					if (2.f < m_fThrowPower)
						m_fThrowPower = 2.f;
				}

				m_eUpperPlayerState = STATE_WEAPON_PULLBACK;
				m_bIsFullBack = true;

			}
			else if (m_bIsFullBack)
			{
				// ������
				m_dUpperAnimationSpeed = 0.8;

				Switch_Camera_TPVToFPV(1);
				//static_cast<CTPV_Camera*>(Engine::Get_GameObject(L"Layer_Camera_TPV"))->Set_MoveFov(60.f, 0.4);

				m_eUpperPlayerState = STATE_WEAPON_THROW;

				// ȭ�� �߻�
				if (P_STANCE_BOW == m_ePlayerStance)
					Shot_Bullet(BULLET_ARROW);

				m_bIsFullBack = false;
			}
		}
		else if (Engine::Get_BIMouseDown(DIM_LB))
		{
			m_eUpperPlayerState = STATE_ATTACK;

			//////////////////////////////////////////////////////////////////////////
			// �� �׽�Ʈ
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

	// �Ǽ����� ��ȯ
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

	// ������ ����
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

	/// ���⼭ ���� �������� ������ ��
	// �������� ��� ���� ���
	if (m_bIsBuilding)
	{
		Is_BuildingCancle();
		if (iPreQuickSlot == m_iQuickSlot)
			return;
	}

	// ���� ���õ� ������ ������ ���� ��������
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
		m_wstrLeftHand = L"Layer_Weapon_Hatchet";		// ������
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;
	case ITEMID_StonePick:
		if (L"Layer_Weapon_StonePick" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_StonePick";		// �����
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;
	case ITEMID_Club:
		if (L"Layer_Weapon_WoodClub" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_WoodClub";		// ���������
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;
	case ITEMID_Spear:
		if (L"Layer_Weapon_Spear" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_Spear";			// ��â
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;
	case ITEMID_Bolas:
		if (L"Layer_Weapon_Bola" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_Bola";			// ����
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;
	case ITEMID_Bow:
		if (L"Layer_Weapon_Bow" == m_wstrLeftHand) return;
		m_wstrLeftHand = L"Layer_Weapon_Bow";			// Ȱ
		m_eUpperPlayerState = STATE_WEAPON_PUTAWAY;
		break;

		// ������
	case ITEMID_Meat:
	case ITEMID_CookedMeat:
	case ITEMID_AmarBerry:
		Is_EatItem(tPickItemInfo);				// �ش� ������ �Ա�
		static_cast<CQuickSlot_UI*>(pQuickSlot)->UisngRemove_Item(m_iQuickSlot, 1);
		break;

		/// ��ġ��
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

		/// ���๰
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

	// ī�޶� ���� ������
	if (dwMouseMoveY = Get_DIMouseMove(DIMS_Y))
	{
		m_fAngleX -= dwMouseMoveY/* * (_float)_dTimeDelta*/ * 0.05f;

		// ���� ���� ����
		if (-80.f >= m_fAngleX)
			m_fAngleX = -80.f;
		else if (80.f <= m_fAngleX)
			m_fAngleX = 80.f;
	}

	if (dwMouseMoveX = Get_DIMouseMove(DIMS_X))
	{
		m_fAngleY += dwMouseMoveX/* * (_float)_dTimeDelta*/ * 0.05f;

	}

	// �¿� ���� �ʱ�ȭ
	if (-360.f >= m_fAngleY)
		m_fAngleY += 360.f;
	else if (360.f < m_fAngleY)
		m_fAngleY -= 360.f;


	/// �÷��̾� ȸ��
	// ���� ž������ �� ȸ�� ����
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

		// �÷��̾� ȸ��
		m_pTransformCom->Set_Rotate(ROT_Y, D3DXToRadian(m_fAngleY));
		m_pTransformFPVCom->Set_Rotate(ROT_Y, D3DXToRadian(m_fAngleY));
		m_pTransformFPVCom->Set_Rotate(ROT_X, D3DXToRadian(m_fAngleX));
	}



	// 1,3 ��Ī ����
	if (dwMouseMoveZ = Get_DIMouseMove(DIMS_Z))
	{
		if (0 > dwMouseMoveZ)
		{
			// 3��Ī���� ����
			if (m_bIsFPVCam)
			{
				Switch_Camera(1);
			}
		}
		else if (0 < dwMouseMoveZ)
		{
			// 1��Ī���� ����
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
	// ���콺 ���� (ī�޶� �� �÷��̾� ����)
	Key_Input_MouseMove(_dTimeDelta);

	/// ����Ÿ��
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

	// ���� �����̱�
	// ���� ����
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

	// ��ȣ�ۿ� (E) ���� ������
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

			//������ ���� ž���� ���� ü�� ��� ����
			for (_int i = 0; i < (_int)CSummaryStat_UI_Monster::SummaryStat_END; ++i)
				static_cast<CSummaryStat_UI_Monster*>(Engine::Get_GameObject(L"SummaryStat_UI", ((_int)CSummaryStat_UI::SummaryStat_END) * 2 + (i)))->Set_RideMonster(nullptr);
		}
	}

	// ���Ķ� �׼�
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

		// �ɷ�ġ ��ȯ
		m_tStat.fAtk -= pWeapon->Get_WeaponAtt();
		m_tStat.fTorpidRate = 0.2f;

		// ���� ������ ����
		pWeapon->Set_Dead();
		m_bIsGrapLeftHand = false;
		m_eUpperPlayerState = STATE_IDLE;
		m_ePlayerStance = P_STANCE_BASIC;

		ZeroMemory(m_bIsFPVSubsetRender, sizeof(_bool)*m_pMeshFPVCom->Get_MeshList()->size());
		m_bIsFPVSubsetRender[0] = true;
	}

	WEAPON_TYPE		eWeaponType = WEAPON_END;


	// ���� �ų� ���Ⱑ ������� ���� ����
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

		// ���� �ɷ�ġ��ŭ ����
		m_tStat.fAtk += static_cast<CWeapon*>(pWeapon)->Get_WeaponAtt();
		m_tStat.fTorpidRate = static_cast<CWeapon*>(pWeapon)->Get_WeaponTorpid();
		static_cast<CWeapon*>(pWeapon)->Set_FPVPlayer(m_bIsFPVCam);

		// ���⿡ ���� ī�޶� ���� ����
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

	/// �÷��̾� ��ü ������Ʈ
	if (m_ePreUnitState != m_eUnitState)
	{
		// �̵�, ����, ����, ����Ÿ��, ���� ��� ���� ����
		m_ePreUnitState = m_eUnitState;

		switch (m_eUnitState)
		{
			//case CUnit::STATE_CAMERAWORK:
			//	break;
		case CUnit::STATE_IDLE:
			m_eAimation = MoveAnimationByPose(P_A_BASIC_IDLE);

			/// 1��Ī
			m_eFPVAnimation = P_A_FPV_BASIC_JUMPIDLE;
			break;
		case CUnit::STATE_MOVE:
			// �÷��̾� �̵� �ִϸ��̼� ���� (��� ���� �ٸ��� ����)

			/// 1��Ī
			m_eFPVAnimation = P_A_FPV_BASIC_MOVE;
			break;
		case CUnit::STATE_SPRINT:
			m_eAimation = P_A_BASIC_SPRINT;

			/// 1��Ī
			m_eFPVAnimation = P_A_FPV_BASIC_SPRINT;
			break;
		case CUnit::STATE_JUMPING:
			m_eAimation = P_A_BASIC_JUMP_UP;

			/// 1��Ī
			m_eFPVAnimation = P_A_FPV_BASIC_JUMPIDLE;
			break;
		case CUnit::STATE_FALLING:
			m_eAimation = P_A_BASIC_JUMP_FALL;

			/// 1��Ī
			m_eFPVAnimation = P_A_FPV_BASIC_JUMPIDLE;
			break;
		case CUnit::STATE_LANDING:
			m_eAimation = P_A_BASIC_JUMP_LAND;

			/// 1��Ī
			m_eFPVAnimation = P_A_FPV_BASIC_JUMPIDLE;
			break;
		case CUnit::STATE_TORPID:
			break;
		case CUnit::STATE_DEAD:
			/// �÷��̾� �����
			m_bDead = true;

			m_eAimation = P_A_BASIC_HURT;
			m_eUpperPlayerState = STATE_IDLE;
			g_GlobalTime = 0.4;
			m_bDissolve = true;
			m_bIsFullBack = false;

			// ���� ����
			if (m_bIsGrapLeftHand)
			{
				pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrPreLeftHand, 0));

				// �ɷ�ġ ��ȯ
				m_tStat.fAtk -= pWeapon->Get_WeaponAtt();
				m_tStat.fTorpidRate = 0.2f;

				// ���� ������ ����
				pWeapon->Set_Dead();

				m_bIsGrapLeftHand = false;
				m_bIsGrapRightHand = false;
				m_wstrPreLeftHand = m_wstrLeftHand = L"";
				m_eUpperPlayerState = STATE_IDLE;
				m_ePlayerStance = P_STANCE_BASIC;
			}
			// �κ��丮 �ݱ�
			if (m_bIsOpenInventory)
			{
				//static_cast<CRight_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Right_InventoryClose();
				static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->Stat_UI_Close();//�κ� ���� �� ����
				m_bIsOpenInventory = !m_bIsOpenInventory;
			}
			break;
		case CUnit::STATE_RIDE:
			m_bIsRide = true;						// ž�� ����

			if (L"Rex" == m_pDino->Get_DinoSpecies())
			{
				//m_eAimation = P_A_RIDE_REX_IDLE;		// ���� ž�� �ִϸ��̼�
				static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Set_TPVCam_MaxDis(30.f);
			}
			else if (L"Para" == m_pDino->Get_DinoSpecies())
			{
				//m_eAimation = P_A_RIDE_PARA_IDLE;		// ���� ž�� �ִϸ��̼�
				static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Set_TPVCam_MaxDis(10.f);
			}
			else
			{
				//m_eAimation = P_A_RIDE_IDLE;			// ���� ž�� �ִϸ��̼�
				static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera", 0))->Set_TPVCam_MaxDis(10.f);
			}

			m_eUpperPlayerState = STATE_IDLE;			// �κ� ������Ʈ �⺻���·� ����

														// �÷��̾� look ���� �������� ����
			m_pTransformCom->Set_Rotate(ROT_Y, static_cast<CTransform*>(m_pDino->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Rotate(ROT_Y));
			break;
		}
	}

	// �÷��̾� ��� �⺻���� �ִϸ��̼� ����
	if (STATE_IDLE == m_eUnitState)
	{
		m_eAimation = MoveAnimationByPose(P_A_BASIC_IDLE);
	}

	// �÷��̾� ���⺰ �޸��� �ִϸ��̼� ����
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

		// �Ȱų� �۶� ���� �ð����� ����� ����
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


	/// �÷��̾� �κ� ������Ʈ(��ü)
	if (m_ePreUpperPlayerState != m_eUpperPlayerState)
	{
		// ����, ����, ���⽺��, ��ȣ�ۿ� ��� ���� ����
		m_ePreUpperPlayerState = m_eUpperPlayerState;

		switch (m_eUpperPlayerState)
		{
		case CUnit::STATE_IDLE:				// �⺻ ���� (�⺻������ ��� ��ü �ִϸ��̼��� ����)
			m_eUpperAnimation = P_A_BASIC_IDLE;

			/// 1��Ī
			m_eFPVUpperAnimation = P_A_FPV_BASIC_JUMPIDLE;
			break;
		case CUnit::STATE_ATTACK:			// ����
			m_eUpperAnimation = P_A_BASIC_ATTACK;
			m_bIsAttack = true;

			/// 1��Ī
			m_eFPVUpperAnimation = PLAYER_ANIMATION_FPV(P_A_FPV_BASIC_ATTACK_LEFT_1 + rand() % 4);
			break;
		case CUnit::STATE_HURT:				// �¾��� ��
			m_eUpperAnimation = P_A_BASIC_HURT;
			break;
		case CUnit::STATE_EAT:				// ���� ��
			m_eUpperAnimation = P_A_BASIC_EAT;
			m_bIsInteract = true;
			break;
		case CUnit::STATE_HARVEST:			// ������ �ݱ�
			m_eUpperAnimation = P_A_BASIC_HARVEST;
			m_bIsInteract = true;
			// ä�� ������ �߰�
			static_cast<CHarvestable*>(m_pLookObject)->HitBy(this, WEAPON_HAND_HARVEST, _vec3(static_cast<CHarvestable*>(m_pLookObject)->Get_Pos()));

			/// 1��Ī
			m_eFPVUpperAnimation = P_A_FPV_BASIC_HARVESTING;
			break;
		case CUnit::STATE_INVENTORY:		// �κ��丮 ����
			m_eUpperAnimation = P_A_BASIC_INVENTORY;

			/// 1��Ī
			m_eFPVUpperAnimation = P_A_FPV_BASIC_IMPLANT_EQUIP;
			break;
		case CUnit::STATE_ATCTIVATECONSOLE:	// ��� �κ��丮 ����, ��ȣ�ۿ�
			m_eUpperAnimation = P_A_BASIC_ATCTIVATECONSOLE;

			/// 1��Ī
			m_eFPVUpperAnimation = P_A_FPV_BASIC_IMPLANT_EQUIP;
			break;
		case CUnit::STATE_WHISTLE:			// ���Ķ� �ұ�
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
		case CUnit::STATE_WEAPON_DRAW:		// ���� ������
			m_eUpperAnimation = P_A_BASIC_EQUIP_COPY;

			m_eFPVUpperAnimation = P_A_FPV_BASIC_FISTSEQUIP;
			break;
		case CUnit::STATE_WEAPON_PUTAWAY:	// ���� �ֱ�
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
				ERR_MSG(L"�߸��� �÷��̾� ������ ������Ʈ ȣ��");
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

		// ���Ⱑ ���� ��� ������ ���� ȣ��
		if (m_bIsGrapLeftHand)
		{
			switch (m_ePlayerStance)
			{
			case CPlayer::P_STANCE_HATCHET:
				// ���� �ڼ��϶�
				if (0.5 < dAnimationtime && 0.7 > dAnimationtime)
				{
					// ���⸦ ��� ���� ���� ����
					CWeapon* pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrLeftHand.c_str()));
					if (pWeapon)
						pWeapon->Is_Attack(m_wstrTargetLayer, nullptr);
				}
				break;
			case CPlayer::P_STANCE_SPEAR:
				// â �ڼ��϶�
				if (0.3 < dAnimationtime && 0.5 > dAnimationtime)
				{
					// ���⸦ ��� ���� ���� ����
					CWeapon* pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrLeftHand.c_str()));
					if (pWeapon)
						pWeapon->Is_Attack(m_wstrTargetLayer, nullptr);
				}
				break;
			}
		}
		else
		{
			////�Ǽ��� ��
			m_dUpperAnimationSpeed = 0.7;

			if (0.4 < dAnimationtime && 0.6 > dAnimationtime)
			{
				CSphereCollider::SPHEREINFO tColInfo = m_pSphereColliderCom->Get_listCollider(CSphereCollider::COL_ATTACK)->front();
				_vec3 vWorldPos;
				memcpy(&vWorldPos, &tColInfo.matColMatrix.m[INFO_POS][0], sizeof(_vec3));

				//_vec3 vCollPos;
				if (true == CCollisionFunc::AttackOnUnit(this, &vWorldPos, &tColInfo.fRadius, m_wstrTargetLayer, CSphereCollider::COL_HIT, WEAPON_HAND, nullptr, nullptr/*, &vCollPos*/))
				{
					// �Ǽ� ������ �¾��� �� �÷��̾� HP 2 ����
					m_tStat.fHp -= 2.f;
					if (0.f >= m_tStat.fHp)
						m_tStat.fHp = 0.f;



					//vCollPos ��ġ�� ����Ʈ ����ؾ��ϴµ�
					//���� ���� �浹ü�� �� ���� �浹�� ��� �������� ������ �浹ü���� �浹�������� ��ȯ��. (�ϳ��� ������ ���� �浹ü, ���� ������ ���� �浹ü)
					//�ذ��� 1) �������°� �ƴ� � �浹ü���� �浹 ���� �� ��ȸ�� ���߰� �浹�Լ� ���� -> ��ó�� ����� ������ ���濡 �� 1ȸ �߻�
					//�ذ��� 2) �������°� �ƴ� ��� �浹 �˻� �� ���� �����(��) �浹�� �����Ͽ� 1ȸ�� �浹 ó�� -> ���� ����� ���濡 �� 1ȸ �߻�
					//�ذ��� 3) ����� �浹���� �ش� ��ü�� Hitby �Լ��� �Ѱ� �ű⼭ ������ 1ȸ�� ��Ƣ�⵵�� ó���ϴ� ��� -> �ɸ� ��� ������ ���� �� 1ȸ �߻�
					//	->	�ذ��� 3���� ����. �� ��ü�� Hitby �Լ����� �ǰ�����Ʈ ����ϸ� �˴ϴ�.
					//		(����(��), ����(����), ����(�ɰ���) �� ������ü���� ����Ʈ�� �ٸ��Ƿ� �̰� �� �����ѵ�...)
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
			// ���� ����
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
	case CUnit::STATE_WEAPON_DRAW:		// ���� ������
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.3))
		{
			m_eUpperPlayerState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_WEAPON_PUTAWAY:	// ���� �ֱ�
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.3))
		{
			// ���� ���� ����ְ� ���� ���Ⱑ ���� ��� STATE_WEAPON_DRAW �� ����
			if (Swap_Weapon())
				m_eUpperPlayerState = STATE_WEAPON_DRAW;
			else
				m_eUpperPlayerState = STATE_IDLE;
		}
		break;
	case CUnit::STATE_WEAPON_THROW:
		if (m_pMeshFPVCom->Is_AnimationsetEnd(0.1))
		{
			/// ������ ���Ⱑ ������ ���
			if (P_STANCE_BOAL == m_ePlayerStance)
			{
				// ���� ������ ����
				Engine::Get_GameObject(m_wstrLeftHand, 0)->Set_Dead();
				m_bIsGrapLeftHand = false;
				//static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI"))->UisngRemove_Item(m_iQuickSlot, 1);

				// ���� ����ü ����
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

				// �����Կ� ���� �������� ��� ���ο� ���� ������
				if (ITEMID_END != static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI"))->Get_Item_To_Number(m_iQuickSlot).iItemID)
				{
					Swap_Weapon();
				}
				else
				{
					// ���� ���� ��� ������� ���ƿ���
					m_eUpperPlayerState = STATE_IDLE;
					m_ePlayerStance = P_STANCE_BASIC;
					m_wstrPreLeftHand = m_wstrLeftHand = L"";
				}
			}
			else if (P_STANCE_BOW == m_ePlayerStance)
			{
				// ȭ�� �߻� �� ������
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

		// ���Ⱑ ���� ��� ������ ���� ȣ��
		if (m_bIsGrapLeftHand)
		{
			switch (m_ePlayerStance)
			{
			case CPlayer::P_STANCE_HATCHET:
				// �����ڼ��϶�
				if (0.5 < dAnimationtime && 0.7 > dAnimationtime)
				{
					// ���⸦ ��� ���� ���� ����
					CWeapon* pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrLeftHand.c_str()));
					if (pWeapon)
						pWeapon->Is_FPVAttack(m_wstrTargetLayer, *m_pFPVSphereColliderCom->Get_Pos(), *m_pFPVSphereColliderCom->Get_Radius());
				}
				break;
			case CPlayer::P_STANCE_SPEAR:
				// �����ڼ��϶�
				if (0.3 < dAnimationtime && 0.5 > dAnimationtime)
				{
					// ���⸦ ��� ���� ���� ����
					CWeapon* pWeapon = static_cast<CWeapon*>(Engine::Get_GameObject(m_wstrLeftHand.c_str()));
					if (pWeapon)
						pWeapon->Is_Attack(m_wstrTargetLayer, nullptr);
				}
				break;
			}
		}
		else
		{
			////�Ǽ��� ��
			m_dUpperAnimationSpeed = 0.1;

			if (0.4 < dAnimationtime && 0.6 > dAnimationtime)
			{
				CSphereCollider::SPHEREINFO tColInfo = m_pFPVSphereColliderCom->Get_listCollider(CSphereCollider::COL_ATTACK)->front();
				_vec3 vWorldPos;
				memcpy(&vWorldPos, &tColInfo.matColMatrix.m[INFO_POS][0], sizeof(_vec3));

				//_vec3 vCollPos;
				if (true == CCollisionFunc::AttackOnUnit(this, &vWorldPos, &tColInfo.fRadius, m_wstrTargetLayer, CSphereCollider::COL_HIT, WEAPON_HAND, nullptr, nullptr/*, &vCollPos*/, true))
				{
					// �Ǽ� ������ �¾��� �� �÷��̾� HP 2 ����
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

	// ���� ĳ���� �������� ��, �ʷϻ� ǥ��
	if (_bRayCast)
		pPreView->Set_Color(0.f, 1.f, 0.f);

	// ���� ĳ���� �������� ��, ������ ǥ��
	else
		pPreView->Set_Color(1.f, 0.f, 0.f);
}

void CPlayer::Build_Structure(const _vec3& _vBuildPos, CGameObject* pPlatform, _float* _pAngleY)
{
	// ������ �����
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

	// Ŭ�� ����
	CGameObject* pStructure = Engine::Clone_GameObjectPrototype(m_wstrPreviewStructure, &tStructInfo/*_vPos*/);
	Engine::Add_GameObjectInLayer(L"Layer_Structure", pStructure);

	// ������ ����
	if (pPlatform)
	{
		static_cast<CStructure*>(pStructure)->Set_TartgetPlatform(pPlatform);
	}

	// Ư�� �������� �Լ� ȣ��
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
	/// �ǹ��� �ʿ� ���� �������� nullptr ��ȯ
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
		// �ݸ��� ���η� 1�� ���͸�
		CSphereCollider* pCollider = static_cast<CSphereCollider*>(pObj->Get_Component(L"Com_SphereCollider", ID_DYNAMIC));
		CTransform*	pTransform = static_cast<CTransform*>(pObj->Get_Component(L"Com_Transform", ID_DYNAMIC));

		if (nullptr == pCollider)
			continue;

		CSphereCollider::SPHEREINFO tCol = pCollider->Get_listCollider(CSphereCollider::COL_INTERACTION)->front();

		_vec3 fColPos;
		memcpy(&fColPos, &tCol.matColMatrix.m[INFO_POS][0], sizeof(_vec3));

		// �Ÿ��� 2�� ���͸�		
		if (10.f <= D3DXVec3Length(&_vec3(m_vPlayerPos - fColPos)))
			continue;

		// �ǹ� ��ü ��ȸ�ϱ�
		for (auto iter : *pCollider->Get_listCollider(CSphereCollider::COL_BUILD_PIVOT))
		{
			// ��� ���๰ �ǹ��� ������ ũ��� 1.f;
			_float fColRadius;
			fColRadius = 1.f;

			// �ǹ� ��ġ ��������
			memcpy(&fColPos, &iter.matColMatrix.m[INFO_POS][0], sizeof(_vec3));


			/// ���⼭���� ������ ��ü�浹 ����
			// �� ����� ��������
			_mat matView;
			m_pDevice->GetTransform(D3DTS_VIEW, &matView);
			D3DXMatrixInverse(&matView, NULL, &matView);

			// ��ŷ ������ ������ ���� �����
			_vec3 vCamPos, vCamLook;
			memcpy(&vCamPos, &matView.m[INFO_POS][0], sizeof(_vec3));
			memcpy(&vCamLook, &matView.m[INFO_LOOK][0], sizeof(_vec3));
			D3DXVec3Normalize(&vCamLook, &vCamLook);

			vCamPos += vCamLook * fCamDis;

			// ������ ���� ���ϱ� ���� cos �� ���ϱ�
			_vec3 v1 = fColPos - vCamPos;					// ������ �������� ��ü�߽��� �̾��ִ� ����
			_float f2 = D3DXVec3Length(&v1);				// ������ �������� ��ü �߽��� �Ÿ��� ���Ѵ�.
			_float f1 = D3DXVec3Dot(&vCamLook, &v1);		// ������ ���� ���� ������ ���̸� ���Ѵ�.

			if (0.f > f1)
				continue;

			_float fDis = sqrtf((f2 * f2) - (f1 * f1));

			if (fDis < fColRadius)
			{
				if (0.f > fTargetDis)
				{
					// ó�� Ÿ���� ã���� ��
					bIsPicking = true;

					fTargetDis = f2;

					memcpy(_vPos, &iter.matColMatrix.m[INFO_POS][0], sizeof(_vec3));
					*_iDirIndex = iter.eColDir;
					pTargetObj = static_cast<CStructure*>(pObj);

					// Y ȸ����
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
					// ���� Ÿ���� ã���� ��
					if (fTargetDis > f2)
					{
						fTargetDis = f2;

						memcpy(_vPos, &iter.matColMatrix.m[INFO_POS][0], sizeof(_vec3));
						*_iDirIndex = iter.eColDir;
						pTargetObj = static_cast<CStructure*>(pObj);

						// Y ȸ����
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
		/// ��� �ǹ�
		if (L"Proto_Obj_Thatch_Wall" == m_wstrPreviewStructure ||
			L"Proto_Obj_Thatch_DoorFrame" == m_wstrPreviewStructure)
		{
			// ���� ��� ���⿡ ���� ȸ����Ű��
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
		/// �� �ǹ�
		if (L"Proto_Obj_Thatch_Roof" == m_wstrPreviewStructure)
		{
			// �������� ����� �÷��̾��� �������� �յ� �Ǵ�
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
		/// ���� �ǹ�
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

	// ���� �ʱ�ȭ
	m_tStat.fHp = m_tStat.fMaxHp;
	m_tStat.fHunger = m_tStat.fMaxHunger;
	m_tStat.fStamina = m_tStat.fMaxStamina;
	m_tStat.fThirst = m_tStat.fMaxThirst;

	m_pAttacker = nullptr;
	m_iKnockCount = 0;

	// ������ ����
	m_bDissolve = false;
	m_fDissolveAmount = 0.f;

	//���̵�ƿ� ����
	Engine::Fade_In(1.f);

	// ���� �ʱ�ȭ
	/// ��Ȱ�� ī�޶� ����
	Switch_Camera(0);
	m_bIsFPVCam = true;
	m_bIsLockCamera = true;
	m_eFPVAnimation = P_A_FPV_BASIC_CAMERA_SHORT;
	m_eFPVUpperAnimation = P_A_FPV_BASIC_CAMERA_SHORT;

	m_eUnitState = STATE_CAMERAWORK;
	m_eUpperPlayerState = STATE_CAMERAWORK;
	m_ePlayerPose = P_POSE_STAND;
	m_ePlayerStance = P_STANCE_BASIC;

	// ��Ȱ�� ��ġ�� ����
	m_pTransformCom->Set_Pos(_vSpawnPos);
	PxExtendedVec3 vSavePos = PxExtendedVec3(_vSpawnPos.x, _vSpawnPos.y, _vSpawnPos.z);
	m_pCtrl->setPosition(vSavePos);

	// ī�޶� �����̱�
	static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2);


	// ���� �ʱ�ȭ
	m_bIsGrapLeftHand = false;
	m_bIsGrapRightHand = false;
	m_wstrLeftHand = L"";
	m_wstrPreLeftHand = L"";

	ZeroMemory(m_bIsFPVSubsetRender, sizeof(_bool) * m_pMeshFPVCom->Get_MeshList()->size());
	m_bIsFPVSubsetRender[0] = true;		// ��ü�� �׸���

	ZeroMemory(m_bIsTPVSubsetRender, sizeof(_bool) * m_pMeshCom->Get_MeshList()->size());
	m_bIsTPVSubsetRender[0] = true;		// ��ü�� �׸���
	m_bIsTPVSubsetRender[1] = true;		// �������



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
		// ���⿡ ���� ī�޶� ���� ����
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
		// 3��Ī���� 1��Ī���� �ڿ������� �ٲٱ�
		static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(0.f, 0.f, 50.f, 0.2, false, true);
	}
	else if (1 == _iIndex)
	{
		// 1��Ī���� 3��Ī���� �ڿ������� �ٲٱ�
		// ���⿡ ���� ī�޶� ���� ����
		if (L"Layer_Weapon_Bola" == m_wstrLeftHand ||
			L"Layer_Weapon_Bow" == m_wstrLeftHand)
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, MAXCAMRIGHT, 50.f, 0.1, true);
		else
			static_cast<CCameraMrg*>(Engine::Get_GameObject(L"Layer_Camera"))->Set_TPVMoveCamera(MAXCAMDIS, 0.f, MAXCAMFOV, 0.2, true);
	}
}

void CPlayer::Is_EatItem(const ITEM_INFO& _tItemInfo)
{
	// �÷��̾� �ִϸ��̼� ����
	m_eUpperPlayerState = STATE_EAT;

	// �����ƺ� �߰� ȿ��
	if (ITEMID_Meat == _tItemInfo.iItemID)
		m_tStat.fHp -= 3.f;		// ����⸦ ���� ��� HP 3 ����


								// ������ �ɷ�ġ ��ŭ ��� ä���
	if (m_tStat.fMaxHunger <= m_tStat.fHunger)
	{
		// ������� �ƽ��� �� ���� ȸ��ġ�� ���ݸ�ŭ HP ȸ��
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
		// ���������� �̵�
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
		// �������� �̵�
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
		// �տ��������� �̵�
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
		// �տ������� �̵�
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
		// �������� �̵�
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
		// �ڿ��������� �̵�
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
		// �ڿ������� �̵�
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
		// �������� �̵�
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
			return P_A_HATCHET_HIT;		// ä�� ���
		return P_A_HATCHET_SWING;			// ����� �ֵθ���
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
			return P_A_FPV_HATCHET_HIT;		// ä�� ���
		return P_A_FPV_HATCHET_SWING;		// ����� �ֵθ���
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
		//ȭ���½�̴�����Ʈ //<<�ǰ�����Ʈ���>>_vec3 _vPos,_vec3 _vAtkPos
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

	// �����
	if (0.f >= m_tStat.fHunger)							// ������� 0�̸� HP �ʴ� 2 ����
	{
		//m_tStat.fHp -= 2.f * (_float)_dTimeDelta;
	}
	else
	{
		m_tStat.fHunger -= 0.5f * (_float)_dTimeDelta;			// ����� �ʴ� 0.5 ����
		m_tStat.fHp += 1.f * (_float)_dTimeDelta;
		if (m_tStat.fMaxHp <= m_tStat.fHp)
			m_tStat.fHp = m_tStat.fMaxHp;

		if (0.f >= m_tStat.fHunger)						// ����� 0 ~ max ����
			m_tStat.fHunger = 0.f;
		else if (m_tStat.fMaxHunger <= m_tStat.fHunger)
			m_tStat.fHunger = m_tStat.fMaxHunger;
		else if (m_tStat.fMaxHp > m_tStat.fHp)
		{
			// ������� HP�� ��ȯ
			m_tStat.fHp += (_float)_dTimeDelta;
			m_tStat.fHunger -= (_float)_dTimeDelta;

			if (m_tStat.fHp > m_tStat.fMaxHp)
				m_tStat.fHp = m_tStat.fMaxHp;
		}
	}

	// ���� ���� �� ��� ����
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
	//���� ���� 1
	// 1 -> 2���� �ʰ� 50 //���� 50
	m_vecExpRequire.emplace_back(0.f);
	m_vecExpStack.emplace_back(0.f);	//0����

	m_vecExpRequire.emplace_back(0.f);
	m_vecExpStack.emplace_back(0.f);	//1

	m_vecExpRequire.emplace_back(50.f);
	m_vecExpStack.emplace_back(m_vecExpRequire.back());	//1 -> 2����

	_int i = 3;
	_float fStack = m_vecExpRequire.back();
	_float fValue = 0.6f;
	for (; i < 100; i++)
	{
		_float fBefore = m_vecExpRequire.back();	//�������� �ʿ����ġ
		_float fRequire = fStack + fBefore * (fValue * 0.8f);	//���緹�� �ʿ����ġ = ��������ġ + �������� �ʿ����ġ�� ������ ���� ��ġ
		m_vecExpRequire.emplace_back(fRequire);	//���緹�� �ʿ����ġ ����
		fStack += fRequire;
		m_vecExpStack.emplace_back(fStack);	//���緹�� ��������ġ ����
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
