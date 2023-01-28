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
		P_A_FPV_BASIC_IMPLANT_EQUIP,		// 인벤토리 시작
		P_A_FPV_BASIC_IMPLANT_IDLE,			// 인벤토리
		P_A_FPV_BASIC_CAMERA_SHORT,			/// 부활시 카메라 연출
		P_A_FPV_BASIC_FISTSEQUIP,				//
		P_A_FPV_BASIC_FISTSUNEQUIP,				//
		P_A_FPV_BASIC_ATTACK_LEFT_1,		// 공격 1
		P_A_FPV_BASIC_ATTACK_LEFT_2,		// 공격 2
		P_A_FPV_BASIC_ATTACK_RIGHT_1,		// 공격 3
		P_A_FPV_BASIC_ATTACK_RIGHT_2,		// 공격 4
		P_A_FPV_BASIC_HARVESTING,			// 수확
		P_A_FPV_BASIC_MOVE,					/// 이동
		P_A_FPV_BASIC_PETPUG,				/// 쓰다듬기
		P_A_FPV_BASIC_CAMERA_SPWANINTRO,	/// 게임시작시 카메라 연출
		P_A_FPV_BASIC_TWOHANDHOLDING,
		P_A_FPV_BASIC_JUMPIDLE,				// 기본상태
		P_A_FPV_BASIC_SPRINT,				// 전력질주

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
		P_A_BASIC_WHISTLE,					// 휘파람
		P_A_BASIC_ATCTIVATECONSOLE,
		P_A_BASIC_WHISTLE_ATTACK,			// 공격 휘파람
		P_A_BASIC_WHISTLE_MOVE,				// 이동 휘파람
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
		P_A_ETC_BOLA_LOOP,				// 볼라 돌리는 애니메이션
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
		P_A_SPEAR_PULLBACK_THROW,		// 던지기 (창, 볼라)
		P_A_SPEAR_TURN_RIGHT,
		P_A_SPEAR_PRONE_IDLE,
		P_A_SPEAR_PRONE_IN,
		P_A_SPEAR_PRONE_FWD,
		P_A_SPEAR_PRONE_OUT,

		P_A_RIDE_REX_IDLE,				// 렉스 탑승
		P_A_RIDE_REX_MOVE,				// 렉스 탑승 움직임
		P_A_RIDE_KNEELING_IDLE,			//
		P_A_RIDE_KNEELING_MOVE,			// 앉은 자세 탑승
		P_A_RIDE_IDLE,					//
		P_A_RIDE_MOVE,					// 보통 탑승
		P_A_RIDE_PARA_IDLE,				//
		P_A_RIDE_PARA_MOVE,				// 파라 탑승
		P_A_RIDE_WIED_IDLE,				//
		P_A_RIDE_WIED_MOVE,				// 넓은 탑승
		P_A_RIDE_TROPE_IDLE,
		P_A_RIDE_WYVERN_IDLE,
		
		P_A_FISHING_CAST,
		P_A_FISHING_IDLECAST,
		P_A_FISHING_IDLENOCAST,
		P_A_FISHING_PULLINLOOPCAST,

		P_A_END
	};
#pragma endregion
	enum PLAYER_POSE		// 플레이어 자세
	{
		P_POSE_STAND,		// 서있는 자세
		P_POSE_CROUCH,		// 앉아있는 자세
		P_POSE_PRONE,		// 엎드린 자세
		P_POSE_END
	};

	enum PLAYER_STANCE
	{
		P_STANCE_BASIC,			// 맨손
		P_STANCE_HATCHET,		// 도끼, 곡괭이, 둔기
		P_STANCE_SPEAR,			// 창
		P_STANCE_BOAL,			// 볼라
		P_STANCE_BOW,			// 활
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
	// 부활
	void					Player_Respawn(const _vec3 _vSpawnPos);

	// 카메라 전환
	void					Switch_Camera(_uint _iIndex);
	void					Switch_Camera_TPVToFPV(_uint _iIndex);

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수
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

	/////////////////////// 플레이어 앞 게임 오브젝트 포인터 넘겨주는 함수
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

	///////// 공룡 탑승중일 떄 키입력
	void					IsRide_Key_Input(const _double& _dTimeDelta);
	void					IsRide_SetPos();
	/////////
	void					Is_Attack();		// 플레이어 공격

	void					Is_Building();
	void					Is_BuildingCancle();
	//
	void					Is_EatItem(const ITEM_INFO& _tItemInfo);		// 사이탬 사용 (먹는 모션)

	_bool					Swap_Weapon();			// 무기 바꾸기 (바꿀 무기가 없을 경우 현재 무기만 삭제)

	void					Set_ShadowLight();
	
	/// FPV 에서 변경되는 함수
	void					FPV_Check_IsEndAnimation(const _double& _dTimeDelta);
	void					FPV_Is_Attack();

	/// 건축관련 함수
	void					Preview_Structure(const _vec3& _vPreviewPos, _bool _bRayCast, _float* _pAngleY = nullptr);
	void					Build_Structure(const _vec3& _vBuildPos, CGameObject* pPlatform, _float* _pAngleY = nullptr);
	CStructure*				Ray_FindPivot(_vec3* _vPos, _float* _fAngleY, _uint* _iDirIndex);
	_bool					Pivot_Relocation(CStructure* _pStructure, _vec3& _vPivotPos, _float& _fAngle, _uint& _iColDir);

private:
	PLAYER_ANIMATION		MoveAnimationByPose(PLAYER_ANIMATION _eAnimation);
	PLAYER_ANIMATION		PlayerAnimationByStance();				// 무기별 애니메이션
	PLAYER_ANIMATION		PlayerUpperAnimationByStance();			// 무기별 부분 애니메이션

	PLAYER_ANIMATION_FPV	PlayerFPVAnimationByStance();			// 무기별 1인칭 애니메이션
	PLAYER_ANIMATION_FPV	PlayerFPVUpperAnimationByStance();		// 무기별 1인칭 부분 애니메이션


	// 도끼,곡괭이, 둔기, 검
	PLAYER_ANIMATION		PlayerAnimation_ToHatchet();
	PLAYER_ANIMATION		PlayerUpperAnimation_ToHatchet();
	PLAYER_ANIMATION_FPV	PlayerFPVAnimation_ToHatchet();
	PLAYER_ANIMATION_FPV	PlayerFPVUpperAnimation_ToHatchet();

	// 창
	PLAYER_ANIMATION		PlayerAnimation_ToSpear();
	PLAYER_ANIMATION		PlayerUpperAnimation_ToSpear();
	PLAYER_ANIMATION_FPV	PlayerFPVAnimation_ToSpear();
	PLAYER_ANIMATION_FPV	PlayerFPVUpperAnimation_ToSpear();

	// 볼라
	PLAYER_ANIMATION_FPV	PlayerFPVAnimation_ToBola();
	PLAYER_ANIMATION_FPV	PlayerFPVUpperAnimation_ToBola();

	// 활
	PLAYER_ANIMATION		PlayerAnimation_ToBow();
	PLAYER_ANIMATION		PlayerUpperAnimation_ToBow();
	PLAYER_ANIMATION_FPV	PlayerFPVAnimation_ToBow();
	PLAYER_ANIMATION_FPV	PlayerFPVUpperAnimation_ToBow();

private:
	CDynamicMesh*			m_pMeshFPVCom = nullptr;
	CTransform*				m_pTransformFPVCom = nullptr;

	CDynamicMesh*			m_pMeshCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;	
	CSphereCollider*		m_pFPVSphereColliderCom = nullptr;	// 주먹 충돌체
	CSphereCollider*		m_pSphereColliderCom = nullptr;		//충돌체 임시 추가<도훈>

	/// 카메라
	//
	_bool					m_bIsFPVCam = true;

	// 3인칭 카메라 설정

	// 앵글
	_bool					m_bIsLockCamera = false;	// 카메라 회전 멈춤
	_float					m_fAngleX = 0.f;			// 카메라 상하 각도 
	_float					m_fAngleY = 0.f;			// 카메라 좌우 각도

	// 플레이어 인포
	_vec3					m_vPlayerPos;
	_float					m_fPlayerSpeed = 10.f;
	wstring					m_wstrTargetLayer = L"";
	_bool					m_bCheatMode = false;
	_bool*					m_bIsFPVSubsetRender = nullptr;
	_bool*					m_bIsTPVSubsetRender = nullptr;

	// 플레이어 애니메이션
	PLAYER_ANIMATION_FPV	m_eFPVAnimation = P_A_FPV_END;			// 1인칭 애니메이션
	PLAYER_ANIMATION_FPV	m_eFPVUpperAnimation = P_A_FPV_END;		// 1인칭 애니메이션
	PLAYER_ANIMATION		m_eAimation = P_A_END;					// 전체 애니메이션
	PLAYER_ANIMATION		m_eUpperAnimation = P_A_END;			// 부분 애니메이션
	PLAYER_POSE				m_ePlayerPose = P_POSE_END;				// 플레이어 자세 (서있기, 앉기, 엎드리기)
	PLAYER_STANCE			m_ePlayerStance = P_STANCE_END;			// 플레이어 무기별 자세
	PLAYER_WHISTLE			m_ePlayerWhistle = P_WHISTLE_END;		// 플레이어 휘파람

	_double					m_dAnimationTime = 0.0;

	_double					m_dAnimationSpeed = 1.0;				// 애니메이션 속도
	_double					m_dUpperAnimationSpeed = 1.0;			// 부분 애니메이션 속도

	// 애니메이션 제한용 bool
	// 부분 스테이트
	UNIT_STATE				m_eUpperPlayerState = STATE_END;
	UNIT_STATE				m_ePreUpperPlayerState = STATE_END;

	_bool					m_bIsAttack = false;				// 공격중
	_bool					m_bIsJumping = false;				// 점프중
	_bool					m_bIsInteract = false;				// 상호작용중 (채집, 구조물 열기, 공룡인벤토리 열기)
	_bool					m_bIsOpenInventory = false;			// 인벤토리 열어보는 중
	_bool					m_bIsBuilding = false;				// 건물짓기
	_bool					m_bGrabBall = false;				// 도도볼 잡기
	_bool					m_bGrabObejct = false;				// 잡기
	_bool					m_bTakeOff = false;					// 공룡 내리기
	_vec3					m_vTakeOffDir;						// 내리는 방향

	_double					m_dPressETime = 0.f;
	_bool					m_bPressE = false;
	CUnit*					m_pRingUnit = nullptr;
	_bool					m_bAttackCrossHair = false;
	_bool					m_bMoveCrossHair = false;

	// 탑승공룡
	_bool					m_bIsRide = false;
	CDino*					m_pDino = nullptr;

	// 무기
	_uint					m_iQuickSlot = 99;				// 현재 사용중인 퀵슬롯
	_bool					m_bIsGrapLeftHand = false;		// 왼손
	_bool					m_bIsGrapRightHand = false;		// 오른손 (방패)
	wstring					m_wstrLeftHand = L"";			// 현재 장착중인 무기 레이어(빈손일때는 L"")
	wstring					m_wstrPreLeftHand = L"";

	_bool					m_bIsFullBack = false;			// 무기 당기기중 여부

	_float					m_fThrowPower = 1.f;

	// 건축
	wstring					m_wstrPreviewStructure = L"";	// 건축할 오브젝트
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

	// Effect 관련
	_float					m_fCreateIntv = 0.f;

	//탑승 Font
	_int					m_iTopSngFontID = 0;

	_float					m_fTime = 0.f;
	_float					m_fStructureDustScale = 0.f;

private:	//프로토빌드용 임시함수
	virtual void	Check_Stat(const _double& _dTimeDelta) override;
	void			Init_LevelUpTable();
	virtual _bool	Check_LevelUp() override;
	vector<_float>	m_vecExpRequire;
	vector<_float>	m_vecExpStack;

public:
	_float			Get_NextExp();
};

#endif // !__PLAYER_H__