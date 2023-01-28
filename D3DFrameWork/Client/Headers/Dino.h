#pragma once
#ifndef __DINO_H__
#define __DINO_H__

#include "Unit.h"

#define DINONAMEADDWORLDPOS {0.f,2.5f,0.f}

BEGIN(Engine)
class CDynamicMesh;
class CSphereCollider;
class CCalculator;
class CSkinnedMesh;
END

class CDino : public CUnit
{
public:
	enum RIDE_INPUT
	{
		RIDE_MOVEFWD,
		RIDE_MOVEBWD,
		RIDE_END
	};

	enum TAKE_CARE
	{
		FOOD,
		HUG,
		WALK,
		TAKE_CARE_END
	};

	enum GENDER
	{
		MALE, FEMALE, GENDER_END
	};

	//링커맨드 별 설정
	enum RINGMAIN
	{
		RINGMAIN_INVEN, RINGMAIN_MOVING, RINGMAIN_SETTING, RINGMAIN_END
	};

	enum RINGMOVING
	{
		RINGMOVING_FOLLOW, RINGMOVING_DISTANCE, RINGMOVING_MATING, RINGMOVING_GROUP_WHITLE, RINGMOVING_END
	};

	enum RINGSETTING
	{
		SETTING_1, SETTING_2, SETTING_3, SETTING_END
	};

	enum DISTANCE
	{
		DISTANCE_CLOSE , DISTANCE_MIDDLE, DISTANCE_FAR, DISTANCE_END
	};

protected:
	explicit CDino(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino(const CDino& rhs);
	virtual ~CDino() = default;

public:
	virtual HRESULT				Ready_GameObject_Prototype() override;
	virtual HRESULT				Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int				Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int				LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void				Render_GameObject() override;
	virtual void				Render_GameObjectShadowMap() override;
	virtual void				Render_GameObjectShadow() override;

public:
	//// 탑승 전용 함수 ///////
	_bool						Get_CanbeRide() { return m_bCanbeRide; }
	void						Set_IsRide(_bool _bIsRide);
	virtual _bool				IsRide_Key_Input(const _double& _dTimeDelta, wstring& wstrTargetLayer, CGameObject* pLookObject);
	virtual void				Ride_Key_Input_Custum(const _double & _dTimeDelta) { return; }
	virtual _bool				HitBy(CUnit* pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos) override;
	virtual void				IsRide_Set_SaddleMat() { return; }
	virtual void				Check_Dying(const _double & _dTimeDelta) override;


protected:
	virtual void				Check_State() PURE;

private:
	void						Dino_Play_Animation(const _double& _time = 1.0);

public:
	virtual CGameObject *		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;

public:
	const D3DXFRAME_DERIVED*	Get_SaddleFrame() const { return m_pSaddleFrame; }
	const _mat&					Get_SaddleMat() const { return m_matSaddle; }
	_bool						Set_IsTamed();

	void						Set_Attack(_bool bAttack) { m_bAttackMode = bAttack; }

	void						Set_Resistance(_bool bResistance) { m_bResistnace = bResistance; }
	void						Set_Follow(_bool bFollow);
	_bool						Get_Follow() { return m_bFollow; }
	void						Set_FixedMove(_bool bFixedMove, _vec3 vMovePos) { m_bFixedMove = bFixedMove; m_vMovePos = vMovePos; }

	_bool						Get_GropWhitle() { return m_bGroupWhitle; }

	void						Attack_OnTarget(CGameObject* pTarget);
	CInven*						Get_Inven() { return m_pShow_InvenCom; }

	//Area에서 호출
public:
	void						MoveToTargetPos(const _vec3& vTargetPos);
	void						Set_SkipFlag(const _ulong& dwFlag);

	//유아기
public:
	virtual void				Set_Growth_Dino_Data() { return; }
	_bool						Get_TakeCare() { return m_bTakeCare; }
	_float						Get_TakeCareTime() { return m_fTakeCareTime; }
	void						Set_TakeCare(_bool bTakeCare);

	TAKE_CARE					Get_TakeCareNum() { return m_eTakeCare; }
	wstring&					Get_BabyMsg() { return m_wstrBabyMsg; }

	//짝짓기 관련
public:
	_bool						Get_MatingMode() { return m_bMatingMode; }
	_float						Get_MatingTime() { return m_fMatingTime; }

protected:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수
	_int						Check_Arg(void * _pArg);							//클론 시 인자 확인해 트랜스폼 및 옵션 부여

	void						Load_Collider(const wstring& FilePath);
	void						Load_DinoInfo(const wstring& FilePath, const wstring& DinoName, _float fDivide = 1.f);

	void						DinoHead_ToCamLook();

protected:
	void						Dino_Idle_Move(const _double & _dTimeDelta);
	void						Dino_Move(const _double & _dTimeDelta);
	// 자식에서 관리
	virtual void				Dino_Move_Fly(const _double & _dTimeDelta) { return; }

	void						Dino_Move_Ride(const _double & _dTimeDelta);
	virtual	_bool				Dino_Move_Ride_Custum(const _double & _dTimeDelta) { return true; };

	void						Dino_Move_Tamed(const _double & _dTimeDelta);

	void						Attack_OnPlayer(_float fRange);

	void						Check_IsEndAnimation();
	virtual	void				Check_IsUpperEndAnimation();
	void						Check_IsHolding(const _double& _dTimeDelta);
	_int						Check_Torpid(const _double& _dTimeDelta);

	virtual _int				Taming_Eat(const _double& _dTimeDelta);

	void						Wild_Action(const _double & _dTimeDelta);
	void						Tamed_Action(const _double & _dTimeDelta);

	void						Is_Attack(const wstring& LayerTag, list<CGameObject*>* pTargetObject = nullptr);
	_bool						Refresh_TargetList(const _double & _dTimeDelta);
	void						Sort_TargetList(_float fDis);

	virtual _bool				Harvest_Action(wstring& wstrTargetLayer, CGameObject* pLookObject) { return false; }

private:
	void						Dino_UI();
	_bool						Dino_Baby_Parenting(const _double & _dTimeDelta);

	_bool						Dino_Action_By_Mode(const _double & _dTimeDelta);
	virtual	void				Dino_Mating(const _double & _dTimeDelta) { return; };

protected:
	CDynamicMesh*				m_pMeshCom = nullptr;
	CSphereCollider*			m_pSphereColliderCom = nullptr;
	CCalculator*				m_pCalculatorCom = nullptr;
	CInven*						m_pShow_InvenCom = nullptr; //채집용 인벤은 유닛 부모클래스에서 넣어줬고, 여기서는 진짜 인벤토리.

	// 애니메이션 관련 변수
	_uint						m_iAnimation = 0;			// 전체 애니메이션
	_uint						m_iUpperAnimation = 0;		// 부분 애니메이션
	_double						m_dAnimationTime = 0;

	UNIT_STATE					m_eUpperDinoState = STATE_END;
	UNIT_STATE					m_ePreUpperDinoState = STATE_END;
	_bool						m_bIsAttack = false;				// 공격중

	// 공룡 전용 변수
	const D3DXFRAME_DERIVED*	m_pSaddleFrame = nullptr;
	const char*					m_pSaddleBoneName = nullptr;
	const char*					m_pNeckBoneName = nullptr;
	const char*					m_pHeadBoneName = nullptr;
	const char*					m_pTailBoneName = nullptr;

public:
	void						Set_Hanging(_bool bHanging) { m_bHanging = bHanging; }
	void						Set_HangingMatrix(const _mat* pMat) { m_pHangingMatrix = pMat; }
	virtual void				HangingMatrix() { return; }
protected:
	// 그랩용
	// 그랩하는 대상
	_bool						m_bGrab = false;
	CGameObject*				m_pGrabObject = nullptr;

	// 그랩당하는 대상
	const _mat*					m_pHangingMatrix = nullptr;
	_bool						m_bHanging = false;

	////////////////////////////////////Hun길들임 여부 전달 함수
public:
	_bool						Get_NowTamingState() { return m_bIsTamed; }
	_bool						Get_NowTorpidState() { return m_bIsTorpid; }
	_bool						Get_NowRidingState() { return m_bIsRide; }
	const wstring&				Get_DinoSpecies() { return m_wstrDinoSpecies; }
	const wstring&				Get_DinoSpeciesKor() { return m_wstrDinoSpeciesKor; }
	void						Set_DinoSpeciesKor(wstring _wstrDinoName, _bool* bNaming = nullptr);
	_vec3						Get_DinoNameAddPos() { return _vec3(DINONAMEADDWORLDPOS); }

	_bool						Get_IsBaby() { return m_bBaby; }
	_bool						Get_IsNaming() { return m_bNaming; }
	void						Set_IsNaming(_bool bNaming) { m_bNaming = bNaming; }
	_float						Get_BabyGrowthTime() { return m_fGrowthTime; }

	GENDER						Get_Gender() { return m_eGender; }
	_bool						Get_Pregnant() {return m_bPregnant;}
	_float						Get_PregnatTime() { return m_fPregnantTime; }
	_bool						Get_PostpartumCare() {	return m_bPostpartumCare; }
	_float						Get_PostpartumCareTime() { return m_fPostpartumCareTime; }

protected:
	wstring						m_wstrDinoSpecies = L"";
	wstring						m_wstrDinoSpeciesKor = L"";
	_int						m_iNameFontID = 0; //죽을 때 없앰욥

	///////////////////////////////////////////////////////////
	_bool						m_bIsTamed = false;			// 길들임 여부
	_bool						m_bIsRide = false;			// 플레이어가 탑승중인지 여부
	_bool						m_bLandDino = true;

	_bool						m_bIsSaddle = false;		// 탈 수 있는지 여부
	_bool						m_bIsEquipSaddle = false;	// 안장 착용상태
	_bool*						m_bIsSaddleIndex = nullptr;
	_uint						m_iAsddleIndex = 0;			// 안장 서브셋 번호
	_mat						m_matSaddle;

	_ulong						m_dwFlagSkip = 0;				//업데이트와 렌더 생략시키는 플래그

	// 애니메이션
	wstring						m_wstrDinoName = L"";

	_bool						m_bRideMove = false;		// 조종으로 움직임
	_float						m_fTurnAngle = 0.f;

	//vector<const char*>		m_vecWaistBoneName;
	//vector<const char*>		m_vecNeckBoneName;

	_float						m_fWaistAngle = 0.f;		// 등뼈 앵글
	_float						m_fNectAngle = 0.f;			// 목뼈 앵글

	// 공룡 변수
	SHADER_PASS_MESH			m_eDinoPass = SHADER_PASS_NORMAL;
	_float						m_fTamingAttack = 0.f;
	_float						m_fTamingHp = 0.f;
	_float						m_fAttackRange = 3.f;
	_float						m_fDetectedRange = 30.f;
	_float						m_fRotationSpeed = 0.f;
	_float						m_fSpeed = 5.f;
	_float						m_fSprint = 5.f;
	_float						m_fBackwardSpeed = 5.f;

	_double						m_dAttackStartTime = 0.5;
	_double						m_dAttackEndTime = 0.7;
	_vec3						m_vfootPoint = {0.f,0.f, 0.f};

	list<CGameObject*>			m_ListTargetUnit;

	_bool						m_bAttackMode = false;
	_bool						m_bBuffRoar = false;
	_double						m_dAnimationSpeed = 0.0;

	// 원거리 공격 당했을 때 플레이어 쫓기
	_bool						m_bChaseMode = false;
	_bool						m_bChaseNotDetected = false;
	_double						m_dChaseTime = 0.0;

	_vec3						m_vMovePos;
	_bool						m_bRoar = false;

	// 육식
	_bool						m_bCanivore = false;

	CTransform*					m_pTartgetTransform = nullptr;
	_vec3						m_vReturnPos = { 0.f,0.f, 0.f };

	// 테이밍 관련변수
	_bool						m_bIsHolding = false;
	_double						m_dHoldingTime = 0.0;

	_bool						m_bIsTorpid = false;
	_double						m_dTamingEatTime = 0.0;

	ITEMID						m_eTamingFood = ITEMID_Meat;		// 나중에 배열로 만들기

	//테이밍 이후 변수
	_bool						m_bCanbeRide = true;	// 탑승 할 수 있는 지
	_bool						m_bGroupWhitle = true;
	_bool						m_bFollow = false;
	_bool						m_bFixedMove = false;
	_bool						m_bResistnace = true;
	CGameObject*				m_pTagetObject = nullptr;

	// IDLE 세팅
	_int						m_iRandNum = 0;
	_double						m_dIdleTime = 0.0, m_dIdleMaxTime = 0.0;
	UNIT_STATE					m_eNextRandomAnim[3];
	
	map<_int, _vec3>			m_mapColor1;
	map<_int, _vec3>			m_mapColor2;
	map<_int, _vec3>			m_mapColor3;
	_int						m_iRandColor[3];

	//////////////////////////////////////////////////////////////////////////
	PxController*				m_pCtrl = nullptr;
	_bool						m_bJump = false;
	_float						m_fJumpTime = 0.f;
	_float						m_fJumpPower = 1.2f;
	_float						m_fJumpGravity = 9.8f;
	//////////////////////////////////////////////////////////////////////////

	// Effect 관련
	_bool						m_bCreateDeadBlood = false;
	_float						m_fCreateIntv = 0.f;

	//////////////////////////////////////////////////////////////////////////
	// 짝짓기
	GENDER						m_eGender = GENDER_END;
	_bool						m_bMatingMode = false;
	_bool						m_bMating = false;
	_float						m_fMatingTime = 0.f;

	_bool						m_bPregnant = false;
	_float						m_fPregnantTime = 0.f;
	_bool						m_bPostpartumCare = false;
	_float						m_fPostpartumCareTime = 0.f;

	// 육아 관련 변수
	_bool						m_bLayEgg = false;

	_bool						m_bBaby = false;
	_bool						m_bTakeCare = false;
	_bool						m_bNaming = false;
	_bool						m_bFirstName[3] = { false, false, false };
	_float						m_fTakeCareTime = 15.f;
	_float						m_fWalkDis = 0.f;
	_float						m_fGrowthTime = 0.f;

	TAKE_CARE					m_eTakeCare = TAKE_CARE_END;
	wstring						m_wstrBabyMsg = L"";


	_bool						m_bPeaceTaming = false;
	_float						m_fPeaceTamingTime = 0.f;
public:
	void						Set_PeaceTaming(_bool bPeaceTaming);
	_bool						Get_PeaceTaming() { return m_bPeaceTaming; }
	virtual	void				PeaceTaming(const _double & _dTimeDelta) { return; }

	virtual	void				Drop() { return; }
	virtual _bool				Before_Sort_TagetList(){ return false; }
		
public:
	virtual void				Open_Ring_Command(CUnit* pUnit) override;
	virtual	void				Set_Ring_Command(_int _iSelectCommand) override;
	void						Delete_UnitList(CGameObject* pObject);
	_bool						Get_Hanging() { return m_bHanging; }
private:
	void						Setting_Ring(_bool* bBool, wstring wstrChangeText, CRingCommand_UI::STRCD* pStrcd);

private:
	//////////////////////////////////////////////////////////////////////////
	map<wstring, vector<CRingCommand_UI::STRCD>>	m_mapRingCommand;
	DISTANCE										m_eDistance = DISTANCE_MIDDLE;
	_float											m_fFllowDis[DISTANCE_END];

};

#endif // !__DINO_H__
