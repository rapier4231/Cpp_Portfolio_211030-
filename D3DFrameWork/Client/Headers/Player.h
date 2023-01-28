#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Unit.h"

BEGIN(Engine)
class CDynamicMesh;
class CCalculator;
class CInven;
class CSphereCollider;
class CSkinnedMesh;
END

class CDino;
class CWeapon;
class CBullet;
class CStructure;
class CPlayer : public CUnit
{
#pragma region PlayerAnimation
	enum PLAYER_ANIMATION_FPV
	{
		P_A_FPV_BASIC_IMPLANT_EQUIP,		// �κ��丮 ����
		P_A_FPV_BASIC_IMPLANT_IDLE,			// �κ��丮
		P_A_FPV_BASIC_CAMERA_SHORT,			/// ��Ȱ�� ī�޶� ����
		P_A_FPV_BASIC_FISTSEQUIP,				//
		P_A_FPV_BASIC_FISTSUNEQUIP,				//
		P_A_FPV_BASIC_ATTACK_LEFT_1,		// ���� 1
		P_A_FPV_BASIC_ATTACK_LEFT_2,		// ���� 2
		P_A_FPV_BASIC_ATTACK_RIGHT_1,		// ���� 3
		P_A_FPV_BASIC_ATTACK_RIGHT_2,		// ���� 4
		P_A_FPV_BASIC_HARVESTING,			// ��Ȯ
		P_A_FPV_BASIC_MOVE,					/// �̵�
		P_A_FPV_BASIC_PETPUG,				/// ���ٵ��
		P_A_FPV_BASIC_CAMERA_SPWANINTRO,	/// ���ӽ��۽� ī�޶� ����
		P_A_FPV_BASIC_TWOHANDHOLDING,
		P_A_FPV_BASIC_JUMPIDLE,				// �⺻����
		P_A_FPV_BASIC_SPRINT,				// ��������

		P_A_FPV_HATCHET_HIT,
		P_A_FPV_HATCHET_DRAW,
		P_A_FPV_HATCHET_IDLE,
		P_A_FPV_HATCHET_MOVE,
		P_A_FPV_HATCHET_PUTAWAY,
		P_A_FPV_HATCHET_SPRINT,
		P_A_FPV_HATCHET_SWING,

		P_A_FPV_SPEAR_AIMLOOP,
		P_A_FPV_SPEAR_AIMSTART,
		P_A_FPV_SPEAR_ATTACK,
		P_A_FPV_SPEAR_BREAK,
		P_A_FPV_SPEAR_EQUIP,
		P_A_FPV_SPEAR_IDLE,
		P_A_FPV_SPEAR_SPRINT,
		P_A_FPV_SPEAR_THROW,
		P_A_FPV_SPEAR_UNEQUIP,

		P_A_FPV_BOLA_EQUIP,			//
		P_A_FPV_BOLA_IDLE,			//
		P_A_FPV_BOLA_MELEE,			//
		P_A_FPV_BOLA_THROWSTART,
		P_A_FPV_BOLA_THROWLOOP,
		P_A_FPV_BOLA_RUN,			//
		P_A_FPV_BOLA_THROW,
		P_A_FPV_BOLA_UNEQUIP,		//
		P_A_FPV_BOLA_WALK,			//

		P_A_FPV_BOW_AIM,
		P_A_FPV_BOW_AIMLOOP,
		P_A_FPV_BOW_DRAW,			//
		P_A_FPV_BOW_FIRE,
		P_A_FPV_BOW_IDLE,			//
		P_A_FPV_BOW_MELEE,			//
		P_A_FPV_BOW_MOVE,			//
		P_A_FPV_BOW_PUTAWAY,
		P_A_FPV_BOW_RELOAD,
		P_A_FPV_BOW_RELOAD_NONOTIFY,
		P_A_FPV_BOW_SPRINT,			//


		P_A_FPV_END
	};

	enum PLAYER_ANIMATION
	{
		P_A_BASIC_AIM_CENTER,
		P_A_BASIC_AIM_DOWN,
		P_A_BASIC_AIM_DOWN_L,
		P_A_BASIC_AIM_DOWN_R,
		P_A_BASIC_AIM_L,
		P_A_BASIC_AIM_R,
		P_A_BASIC_AIM_UP,
		P_A_BASIC_AIM_UP_L,
		P_A_BASIC_AIM_UP_R,
		P_A_BASIC_CROUCH_BWD,
		P_A_BASIC_CROUCH_BWD_L,
		P_A_BASIC_CROUCH_BWD_R,
		P_A_BASIC_CROUCH_FWD,
		P_A_BASIC_CROUCH_FWD_L,
		P_A_BASIC_CROUCH_FWD_R,
		P_A_BASIC_CROUCH_IDLE,
		P_A_BASIC_CROUCH_L,
		P_A_BASIC_CROUCH_R,
		P_A_BASIC_MOVE_BWD,
		P_A_BASIC_MOVE_BWD_L,
		P_A_BASIC_MOVE_BWD_R,
		P_A_BASIC_MOVE_FWD,
		P_A_BASIC_MOVE_FWD_L,
		P_A_BASIC_MOVE_FWD_R,
		P_A_BASIC_MOVE_L,
		P_A_BASIC_MOVE_R,
		P_A_BASIC_JUMP_UP,
		P_A_BASIC_JUMP_FALL,
		P_A_BASIC_JUMP_LAND,
		P_A_BASIC_PRONE_IDLE,
		P_A_BASIC_PRONE_IN,
		P_A_BASIC_PRONE_MOVE_FWD,
		P_A_BASIC_PRONE_OUT,
		P_A_BASIC_SLEEP_LOOP,
		P_A_BASIC_SLEEP_OUT,
		P_A_BASIC_SLEEP_FIRSTSPAWN,
		P_A_BASIC_ATTACK,
		P_A_BASIC_DRINK,
		P_A_BASIC_EAT,
		P_A_BASIC_EAT_NOSOUND,
		P_A_BASIC_EQUIP_COPY,
		P_A_BASIC_HARVEST,
		P_A_BASIC_HURT,
		P_A_BASIC_IDLE,
		P_A_BASIC_INVENTORY,
		P_A_BASIC_PICKUP,
		P_A_BASIC_POOP,
		P_A_BASIC_SPRINT,
		P_A_BASIC_TURN_RIGHT,
		P_A_BASIC_WHISTLE,					// ���Ķ�
		P_A_BASIC_ATCTIVATECONSOLE,
		P_A_BASIC_WHISTLE_ATTACK,			// ���� ���Ķ�
		P_A_BASIC_WHISTLE_MOVE,				// �̵� ���Ķ�
		P_A_BASIC_STUN,
		P_A_BASIC_STUNING,
		P_A_BASIC_TWOHANDHOLDING,			// 55

		P_A_HATCHET_AIM_CENTER,
		P_A_HATCHET_AIM_DOWN,
		P_A_HATCHET_AIM_DOWN_L,
		P_A_HATCHET_AIM_DOWN_R,
		P_A_HATCHET_AIM_L,
		P_A_HATCHET_AIM_R,
		P_A_HATCHET_AIM_UP,
		P_A_HATCHET_AIM_UP_L,
		P_A_HATCHET_AIM_UP_R,
		P_A_HATCHET_COVER_SHIELD,
		P_A_HATCHET_CROUCH_COVER_SHIELD,
		P_A_HATCHET_DRAW,
		P_A_HATCHET_HIT,
		P_A_HATCHET_IDLE,
		P_A_HATCHET_IDLE_SHIELD,
		P_A_HATCHET_JUMPFALL,
		P_A_HATCHET_MOVE_BWD,
		P_A_HATCHET_MOVE_BWD_R,
		P_A_HATCHET_MOVE_BWD_L,
		P_A_HATCHET_MOVE_FWD,
		P_A_HATCHET_MOVE_FWD_L,
		P_A_HATCHET_MOVE_FWD_R,
		P_A_HATCHET_MOVE_L,
		P_A_HATCHET_MOVE_R,
		P_A_HATCHET_PRONE_IDLE,
		P_A_HATCHET_PRONE_IN,
		P_A_HATCHET_PRONE_MOVE_FWD,
		P_A_HATCHET_PRONE_OUT,
		P_A_HATCHET_PUTAWAY,
		P_A_HATCHET_SPRINT,
		P_A_HATCHET_SWING,
		P_A_HATCHET_TURN_RIGHT,
		P_A_HATCHET_IDLE_SHIELD2,
		P_A_HATCHET_CROUCH_SHIELD,
		P_A_HATCHET_PAINTBRUSH,

		P_A_BOW_AIM_CENTER,
		P_A_BOW_AIM_DOWN,
		P_A_BOW_AIM_DOWN_L,
		P_A_BOW_AIM_DOWN_R,
		P_A_BOW_AIM_L,
		P_A_BOW_AIM_R,
		P_A_BOW_AIM_UP,
		P_A_BOW_AIM_UP_L,
		P_A_BOW_AIM_UP_R,
		P_A_BOW_CROUCH_BWD,
		P_A_BOW_CROUCH_BWD_L,
		P_A_BOW_CROUCH_BWD_R,
		P_A_BOW_CROUCH_FWD,
		P_A_BOW_CROUCH_FWD_L,
		P_A_BOW_CROUCH_FWD_R,
		P_A_BOW_CROUCH_IDLE,
		P_A_BOW_CROUCH_L,
		P_A_BOW_CROUCH_R,
		P_A_BOW_DRAW,
		P_A_BOW_FIRE,
		P_A_BOW_IDLE,
		P_A_BOW_JUMPFALL,
		P_A_BOW_MELEE,
		P_A_BOW_MOVE_FWD,
		P_A_BOW_MOVE_FWD_L,
		P_A_BOW_MOVE_FWD_R,
		P_A_BOW_MOVE_BWD,
		P_A_BOW_MOVE_BWD_L,
		P_A_BOW_MOVE_BWD_R,
		P_A_BOW_MOVE_L,
		P_A_BOW_MOVE_R,
		P_A_BOW_PULLSTRING,
		P_A_BOW_PULLSTRINGCENTER_CENTER,
		P_A_BOW_PULLSTRINGCENTER_DOWN,
		P_A_BOW_PULLSTRINGCENTER_DOWN_L,
		P_A_BOW_PULLSTRINGCENTER_DOWN_R,
		P_A_BOW_PULLSTRINGCENTER_L,
		P_A_BOW_PULLSTRINGCENTER_R,
		P_A_BOW_PULLSTRINGCENTER_UP,
		P_A_BOW_PULLSTRINGCENTER_UP_L,
		P_A_BOW_PULLSTRINGCENTER_UP_R,
		P_A_BOW_PULLSTRING_LOOP,
		P_A_BOW_PUTAWAY,
		P_A_BOW_RELOAD,
		P_A_BOW_SPRINT,
		P_A_BOW_TURN_RIGHT,
		P_A_BOW_JUMP,
		P_A_BOW_JUMPLAND,
		P_A_BOW_PRONE_IDLE,
		P_A_BOW_PRONE_IN,
		P_A_BOW_PRONE_FWD,
		P_A_BOW_PRONE_OUT,

		P_A_RIFLE_AIM_CENTER,
		P_A_RIFLE_AIM_DOWN,
		P_A_RIFLE_AIM_DOWN_L,
		P_A_RIFLE_AIM_DOWN_R,
		P_A_RIFLE_AIM_L,
		P_A_RIFLE_AIM_R,
		P_A_RIFLE_AIM_UP,
		P_A_RIFLE_AIM_UP_L,
		P_A_RIFLE_AIM_UP_R,
		P_A_RIFLE_CROUCH_BWD,
		P_A_RIFLE_CROUCH_BWD_L,
		P_A_RIFLE_CROUCH_BWD_R,
		P_A_RIFLE_CROUCH_FWD,
		P_A_RIFLE_CROUCH_FWD_L,
		P_A_RIFLE_CROUCH_FWD_R,
		P_A_RIFLE_CROUCH_IDLE,
		P_A_RIFLE_CROUCH_L,
		P_A_RIFLE_CROUCH_R,
		P_A_RIFLE_IDLE,
		P_A_RIFLE_JUMP,
		P_A_RIFLE_JUMPFALL,
		P_A_RIFLE_JUMPLAND,
		P_A_RIFLE_MELEE,
		P_A_RIFLE_MOVE_BWD,
		P_A_RIFLE_MOVE_BWD_L,
		P_A_RIFLE_MOVE_BWD_R,
		P_A_RIFLE_MOVE_FWD,
		P_A_RIFLE_MOVE_FWD_L,
		P_A_RIFLE_MOVE_FWD_R,
		P_A_RIFLE_MOVE_L,
		P_A_RIFLE_MOVE_R,
		P_A_RIFLE_RELOAD,
		P_A_RIFLE_RIFLE_SHOOT,
		P_A_RIFLE_SPRINT,
		P_A_RIFLE_SINGLE_RELOAD,
		P_A_RIFLE_PRONE_IDLE,
		P_A_RIFLE_PRONE_IN,
		P_A_RIFLE_PRONE_FWD,
		P_A_RIFLE_PRONE_OUT,
		P_A_RIFLE_TURN_RIGHT,
		P_A_RIFLE_SHOTGUN_RELOAD,

		P_A_ETC_SPYGLASS,
		P_A_ETC_SWORD_ATTACK,
		P_A_ETC_GPS_IDLE,
		P_A_ETC_GPS_PRONE_IDLE,
		P_A_ETC_GPS_PRONE_IN,
		P_A_ETC_GPS_PRONE_FWD,
		P_A_ETC_GPS_PRONE_OUT,
		P_A_ETC_GPS_GPSANDMAP,
		P_A_ETC_GPS_MAP,
		P_A_ETC_MAGNIFYINGGLASS,
		P_A_ETC_BOLA_LOOP,				// ���� ������ �ִϸ��̼�
		P_A_RIDE_BIGFOOT,

		P_A_EMOTE_AIRDRUM,
		P_A_EMOTE_AIRGUITAR,
		P_A_EMOTE_VDAY_SHY,
		P_A_EMOTE_BALLET,
		P_A_EMOTE_CHEERUP,
		P_A_EMOTE_LAUGH,

		P_A_SPEAR_CROUCH_BWD,
		P_A_SPEAR_CROUCH_BWD_L,
		P_A_SPEAR_CROUCH_BWD_R,
		P_A_SPEAR_CROUCH_FWD,
		P_A_SPEAR_CROUCH_FWD_L,
		P_A_SPEAR_CROUCH_FWD_R,
		P_A_SPEAR_CROUCH_IDLE,
		P_A_SPEAR_CROUCH_L,
		P_A_SPEAR_CROUCH_R,
		P_A_SPEAR_AIM_CENTER,
		P_A_SPEAR_AIM_DOWN,
		P_A_SPEAR_AIM_DOWN_L,
		P_A_SPEAR_AIM_DOWN_R,
		P_A_SPEAR_AIM_L,
		P_A_SPEAR_AIM_R,
		P_A_SPEAR_AIM_UP,
		P_A_SPEAR_AIM_UP_L,
		P_A_SPEAR_AIM_UP_R,
		P_A_SPEAR_IDLE,
		P_A_SPEAR_JUMPFALL,
		P_A_SPEAR_MELEE,
		P_A_SPEAR_MOVE_FWD,
		P_A_SPEAR_MOVE_FWD_L,
		P_A_SPEAR_MOVE_FWD_R,
		P_A_SPEAR_MOVE_BWD,
		P_A_SPEAR_MOVE_BWD_L,
		P_A_SPEAR_MOVE_BWD_R,
		P_A_SPEAR_MOVE_L,
		P_A_SPEAR_MOVE_R,
		P_A_SPEAR_SPRINT,
		P_A_SPEAR_PULLBACK_AIM_CENTER,
		P_A_SPEAR_PULLBACK_AIM_DOWN,
		P_A_SPEAR_PULLBACK_AIM_DOWN_L,
		P_A_SPEAR_PULLBACK_AIM_DOWN_R,
		P_A_SPEAR_PULLBACK_AIM_L,
		P_A_SPEAR_PULLBACK_AIM_R,
		P_A_SPEAR_PULLBACK_AIM_UP,
		P_A_SPEAR_PULLBACK_AIM_UP_L,
		P_A_SPEAR_PULLBACK_AIM_UP_R,
		P_A_SPEAR_PULLBACK_LOOP,
		P_A_SPEAR_PULLBACK_RELOAD,
		P_A_SPEAR_PULLBACK_START,
		P_A_SPEAR_PULLBACK_THROW,		// ������ (â, ����)
		P_A_SPEAR_TURN_RIGHT,
		P_A_SPEAR_PRONE_IDLE,
		P_A_SPEAR_PRONE_IN,
		P_A_SPEAR_PRONE_FWD,
		P_A_SPEAR_PRONE_OUT,

		P_A_RIDE_REX_IDLE,				// ���� ž��
		P_A_RIDE_REX_MOVE,				// ���� ž�� ������
		P_A_RIDE_KNEELING_IDLE,			//
		P_A_RIDE_KNEELING_MOVE,			// ���� �ڼ� ž��
		P_A_RIDE_IDLE,					//
		P_A_RIDE_MOVE,					// ���� ž��
		P_A_RIDE_PARA_IDLE,				//
		P_A_RIDE_PARA_MOVE,				// �Ķ� ž��
		P_A_RIDE_WIED_IDLE,				//
		P_A_RIDE_WIED_MOVE,				// ���� ž��
		P_A_RIDE_TROPE_IDLE,
		P_A_RIDE_WYVERN_IDLE,
		
		P_A_FISHING_CAST,
		P_A_FISHING_IDLECAST,
		P_A_FISHING_IDLENOCAST,
		P_A_FISHING_PULLINLOOPCAST,

		P_A_END
	};
#pragma endregion
	enum PLAYER_POSE		// �÷��̾� �ڼ�
	{
		P_POSE_STAND,		// ���ִ� �ڼ�
		P_POSE_CROUCH,		// �ɾ��ִ� �ڼ�
		P_POSE_PRONE,		// ���帰 �ڼ�
		P_POSE_END
	};

	enum PLAYER_STANCE
	{
		P_STANCE_BASIC,			// �Ǽ�
		P_STANCE_HATCHET,		// ����, ���, �б�
		P_STANCE_SPEAR,			// â
		P_STANCE_BOAL,			// ����
		P_STANCE_BOW,			// Ȱ
		P_STANCE_END
	};

	enum PLAYER_WHISTLE
	{
		P_WHISTLE_NORMAL,
		P_WHISTLE_ATTACK,
		P_WHISTLE_MOVE,
		P_WHISTLE_END
	};

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 _pDevice);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;
	virtual void			Render_GameObjectShadowMap() override;
	virtual void			Render_GameObjectShadow() override;

public:
	static CPlayer*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUnit*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	_bool					Get_IsRide() { return m_bIsRide; }
	CDino*					Get_RideDino() { return m_pDino; }
	const _float&			Get_AngleX() { return m_fAngleX; }
	const _float&			Get_AngleY() { return m_fAngleY; }

	void					Set_AngleX(const _float& _fAngleX) { m_fAngleX = _fAngleX; }
	void					Set_AngleY(const _float& _fAngleY) { m_fAngleY = _fAngleY; }
	void					Set_AccAngleX(const _float& _fAccAngleX) { m_fAngleX += _fAccAngleX; }
	void					Set_AccAngleY(const _float& _fAccAngleY) { m_fAngleY += _fAccAngleY; }

public:
	// ��Ȱ
	void					Player_Respawn(const _vec3 _vSpawnPos);

	// ī�޶� ��ȯ
	void					Switch_Camera(_uint _iIndex);
	void					Switch_Camera_TPVToFPV(_uint _iIndex);

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// ���̴� ��� ���̺� �����Լ�
	void					Load_Collider_Player(const wstring & FilePath);

public:
	void					Open_Inventory(CGameObject* pTargetInven = nullptr);
	void					Close_Inventory();

	void					Equip_Armor(EQUIP_Position _ePosType, ITEMID _eArmorType);
	void					Unequip_Armor(EQUIP_Position _ePosType);

	//////////////////////////////////////////////////////////////////////////
private:
	CGameObject*			Ray_PlayerLook(const wstring& _wstrLayerTag, _float _fRayDis = 0.f, _bool bDinoRide = false);
	void					Check_PlayerLookLayer();

	/////////////////////// �÷��̾� �� ���� ������Ʈ ������ �Ѱ��ִ� �Լ�
public:
	CGameObject*			Get_pPlayerLookUnit() { return m_pLookObject; }
	/////////////////////////////////////////////////////////////////////

private:
	void					Check_PlayerState(const _double& _dTimeDelta);
	void					Check_IsEndAnimation();
	void					Check_IsEndUpperAnimation(const _double& _dTimeDelta);

	void					Key_Input(const _double& _dTimeDelta);
	void					Key_Input_Move(const _double& _dTimeDelta);
	void					Key_Input_Interact(const _double& _dTimeDelta);
	void					Key_Input_Whistle();
	void					Key_Input_Inventory(const _double& _dTimeDelta);
	void					Key_Input_Attack(const _double& _dTimeDelta);
	void					Key_Input_QuickSlot(const _double& _dTimeDelta);

	void					Key_Input_MouseMove(const _double& _dTimeDelta);

	void					Shot_Bullet(BULLET_TYPE _eBulletType);

	///////// ���� ž������ �� Ű�Է�
	void					IsRide_Key_Input(const _double& _dTimeDelta);
	void					IsRide_SetPos();
	/////////
	void					Is_Attack();		// �÷��̾� ����

	void					Is_Building();
	void					Is_BuildingCancle();
	//
	void					Is_EatItem(const ITEM_INFO& _tItemInfo);		// ������ ��� (�Դ� ���)

	_bool					Swap_Weapon();			// ���� �ٲٱ� (�ٲ� ���Ⱑ ���� ��� ���� ���⸸ ����)

	void					Set_ShadowLight();
	
	/// FPV ���� ����Ǵ� �Լ�
	void					FPV_Check_IsEndAnimation(const _double& _dTimeDelta);
	void					FPV_Is_Attack();

	/// ������� �Լ�
	void					Preview_Structure(const _vec3& _vPreviewPos, _bool _bRayCast, _float* _pAngleY = nullptr);
	void					Build_Structure(const _vec3& _vBuildPos, CGameObject* pPlatform, _float* _pAngleY = nullptr);
	CStructure*				Ray_FindPivot(_vec3* _vPos, _float* _fAngleY, _uint* _iDirIndex);
	_bool					Pivot_Relocation(CStructure* _pStructure, _vec3& _vPivotPos, _float& _fAngle, _uint& _iColDir);

private:
	PLAYER_ANIMATION		MoveAnimationByPose(PLAYER_ANIMATION _eAnimation);
	PLAYER_ANIMATION		PlayerAnimationByStance();				// ���⺰ �ִϸ��̼�
	PLAYER_ANIMATION		PlayerUpperAnimationByStance();			// ���⺰ �κ� �ִϸ��̼�

	PLAYER_ANIMATION_FPV	PlayerFPVAnimationByStance();			// ���⺰ 1��Ī �ִϸ��̼�
	PLAYER_ANIMATION_FPV	PlayerFPVUpperAnimationByStance();		// ���⺰ 1��Ī �κ� �ִϸ��̼�


	// ����,���, �б�, ��
	PLAYER_ANIMATION		PlayerAnimation_ToHatchet();
	PLAYER_ANIMATION		PlayerUpperAnimation_ToHatchet();
	PLAYER_ANIMATION_FPV	PlayerFPVAnimation_ToHatchet();
	PLAYER_ANIMATION_FPV	PlayerFPVUpperAnimation_ToHatchet();

	// â
	PLAYER_ANIMATION		PlayerAnimation_ToSpear();
	PLAYER_ANIMATION		PlayerUpperAnimation_ToSpear();
	PLAYER_ANIMATION_FPV	PlayerFPVAnimation_ToSpear();
	PLAYER_ANIMATION_FPV	PlayerFPVUpperAnimation_ToSpear();

	// ����
	PLAYER_ANIMATION_FPV	PlayerFPVAnimation_ToBola();
	PLAYER_ANIMATION_FPV	PlayerFPVUpperAnimation_ToBola();

	// Ȱ
	PLAYER_ANIMATION		PlayerAnimation_ToBow();
	PLAYER_ANIMATION		PlayerUpperAnimation_ToBow();
	PLAYER_ANIMATION_FPV	PlayerFPVAnimation_ToBow();
	PLAYER_ANIMATION_FPV	PlayerFPVUpperAnimation_ToBow();

private:
	CDynamicMesh*			m_pMeshFPVCom = nullptr;
	CTransform*				m_pTransformFPVCom = nullptr;

	CDynamicMesh*			m_pMeshCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;	
	CSphereCollider*		m_pFPVSphereColliderCom = nullptr;	// �ָ� �浹ü
	CSphereCollider*		m_pSphereColliderCom = nullptr;		//�浹ü �ӽ� �߰�<����>

	/// ī�޶�
	//
	_bool					m_bIsFPVCam = true;

	// 3��Ī ī�޶� ����

	// �ޱ�
	_bool					m_bIsLockCamera = false;	// ī�޶� ȸ�� ����
	_float					m_fAngleX = 0.f;			// ī�޶� ���� ���� 
	_float					m_fAngleY = 0.f;			// ī�޶� �¿� ����

	// �÷��̾� ����
	_vec3					m_vPlayerPos;
	_float					m_fPlayerSpeed = 10.f;
	wstring					m_wstrTargetLayer = L"";
	_bool					m_bCheatMode = false;
	_bool*					m_bIsFPVSubsetRender = nullptr;
	_bool*					m_bIsTPVSubsetRender = nullptr;

	// �÷��̾� �ִϸ��̼�
	PLAYER_ANIMATION_FPV	m_eFPVAnimation = P_A_FPV_END;			// 1��Ī �ִϸ��̼�
	PLAYER_ANIMATION_FPV	m_eFPVUpperAnimation = P_A_FPV_END;		// 1��Ī �ִϸ��̼�
	PLAYER_ANIMATION		m_eAimation = P_A_END;					// ��ü �ִϸ��̼�
	PLAYER_ANIMATION		m_eUpperAnimation = P_A_END;			// �κ� �ִϸ��̼�
	PLAYER_POSE				m_ePlayerPose = P_POSE_END;				// �÷��̾� �ڼ� (���ֱ�, �ɱ�, ���帮��)
	PLAYER_STANCE			m_ePlayerStance = P_STANCE_END;			// �÷��̾� ���⺰ �ڼ�
	PLAYER_WHISTLE			m_ePlayerWhistle = P_WHISTLE_END;		// �÷��̾� ���Ķ�

	_double					m_dAnimationTime = 0.0;

	_double					m_dAnimationSpeed = 1.0;				// �ִϸ��̼� �ӵ�
	_double					m_dUpperAnimationSpeed = 1.0;			// �κ� �ִϸ��̼� �ӵ�

	// �ִϸ��̼� ���ѿ� bool
	// �κ� ������Ʈ
	UNIT_STATE				m_eUpperPlayerState = STATE_END;
	UNIT_STATE				m_ePreUpperPlayerState = STATE_END;

	_bool					m_bIsAttack = false;				// ������
	_bool					m_bIsJumping = false;				// ������
	_bool					m_bIsInteract = false;				// ��ȣ�ۿ��� (ä��, ������ ����, �����κ��丮 ����)
	_bool					m_bIsOpenInventory = false;			// �κ��丮 ����� ��
	_bool					m_bIsBuilding = false;				// �ǹ�����
	_bool					m_bGrabBall = false;				// ������ ���
	_bool					m_bGrabObejct = false;				// ���
	_bool					m_bTakeOff = false;					// ���� ������
	_vec3					m_vTakeOffDir;						// ������ ����

	_double					m_dPressETime = 0.f;
	_bool					m_bPressE = false;
	CUnit*					m_pRingUnit = nullptr;
	_bool					m_bAttackCrossHair = false;
	_bool					m_bMoveCrossHair = false;

	// ž�°���
	_bool					m_bIsRide = false;
	CDino*					m_pDino = nullptr;

	// ����
	_uint					m_iQuickSlot = 99;				// ���� ������� ������
	_bool					m_bIsGrapLeftHand = false;		// �޼�
	_bool					m_bIsGrapRightHand = false;		// ������ (����)
	wstring					m_wstrLeftHand = L"";			// ���� �������� ���� ���̾�(����϶��� L"")
	wstring					m_wstrPreLeftHand = L"";

	_bool					m_bIsFullBack = false;			// ���� ������ ����

	_float					m_fThrowPower = 1.f;

	// ����
	wstring					m_wstrPreviewStructure = L"";	// ������ ������Ʈ
	STRUCTURE_TYPE			m_eBuilding = STRUCTURE_END;

	//////////////////////////////////////////////////////////////////////////
	vector<tuple<PxRigidDynamic*, _mat*, _bool>>	m_tupleBones;
	_bool											m_bRagdoll = false;
	map<_mat*, PxArticulationLink*>					m_plistLink;
	//////////////////////////////////////////////////////////////////////////
	PxCapsuleController*	m_pCtrl = nullptr;
	_bool					m_bJump = false;
	_float					m_fJumpTime = 0.f;
	_float					m_fJumpPower = 1.2f;
	_float					m_fJumpGravity = 9.8f;
	//////////////////////////////////////////////////////////////////////////
	CUnit*					m_pAttacker = nullptr;
	_float					m_fKnockBack = 0.f;
	_int					m_iKnockCount = 0;

#ifdef _DEBUG
	_bool					m_bIsPick = true;
	_int					m_AniTest = 0;
#endif // _DEBUG
	virtual _bool			HitBy(CUnit* pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos);

	CGameObject*			m_pLookObject = nullptr;
	CGameObject*			m_pTargetInvecObject = nullptr;
	_double					m_dOpenDelayInven = 0.0;

	// Effect ����
	_float					m_fCreateIntv = 0.f;

	//ž�� Font
	_int					m_iTopSngFontID = 0;

	_float					m_fTime = 0.f;
	_float					m_fStructureDustScale = 0.f;

private:	//���������� �ӽ��Լ�
	virtual void	Check_Stat(const _double& _dTimeDelta) override;
	void			Init_LevelUpTable();
	virtual _bool	Check_LevelUp() override;
	vector<_float>	m_vecExpRequire;
	vector<_float>	m_vecExpStack;

public:
	_float			Get_NextExp();
};

#endif // !__PLAYER_H__