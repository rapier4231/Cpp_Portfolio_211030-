#pragma once
#ifndef __UNIT_H__
#define __UNIT_H__

#include "Value.h"
#include "GameObject.h"
#include "RingCommand_UI.h"

BEGIN(Engine)
class CInven;
class CTransform;
class CShader;
class CTexture;
END

class CUnit : public CGameObject
{
public:
	enum STAT {STAT_EXP, STAT_LEVEL, STAT_MAXHP, STAT_HP, STAT_MAXSTAMINA, STAT_STAMINA, STAT_MAXHUNGER, STAT_HUNGER, STAT_MAXTHIRST, STAT_THIRST, STAT_MAXWEIGHT, STAT_WEIGHT
		, STAT_ATTACK, STAT_KNOCKBACK, STAT_SPEED, STAT_ENGRAM, STAT_ENDURE, STAT_MAXPARALYSIS, STAT_PARALYSIS, STAT_STATPOINT, STAT_TORPIDRATE, STAT_MAXTAMEDRATE, STAT_TAMEDRATE, STAT_SIZE, STAT_END};

	enum UNIT_STATE
	{
		STATE_CAMERAWORK,			// 카메라 연출
		STATE_IDLE,					// 기본상태1
		STATE_IDLE2,				// 기본상태2
		STATE_IDLE3,				// 기본상태3
		STATE_MOVE,					// 움직임
		STATE_SPRINT,				// 전력질주
		STATE_JUMPING,				// 점프중일 때
		STATE_FALLING,				// 떨어질 때
		STATE_LANDING,				// 착지할 때

		STATE_ATTACK,				// 공격
		STATE_HURT,					// 맞을때
		STATE_EAT,					// 먹을 때
		
		STATE_HARVEST,				// 줍기
		STATE_INVENTORY,			// 인벤토리 볼 때
		STATE_ATCTIVATECONSOLE,		// 대상 상호작용
		STATE_WHISTLE,				// 휘파람
		STATE_WEAPON_DRAW,			// 무기 꺼내기
		STATE_WEAPON_PUTAWAY,		// 무기 넣기

		STATE_WEAPON_THROW,			// 무기 던지기 (볼라, 활, 새총, 창 등등..)
		STATE_WEAPON_PULLBACK,		// 무기 던지기 전 준비 (볼라, 활, 새총, 창 등등..)
		STATE_WEAPON_RELOAD,		// 무기 재장전

		STATE_HOLDING,				// 볼라 맞았을 때

		STATE_DEAD,					// 죽음

		// 테이밍
		STATE_TORPID,				// 기절1
		STATE_TORPID_IDLE,			// 기절2
		STATE_TORPID_TAMED,			// 테이밍 성공
		STATE_TORPID_WILD,			// 테이밍 실패
		STATE_TORPID_EAT,			// 테이밍 중 먹이 먹기

		STATE_RIDE,					// 공룡 탑승중
		STATE_ROAR,					// 공룡 포효
		STATE_EGG,					// 공룡 알 낳기
		STATE_HAPPY,				// 육아 좋아하는 애님
		STATE_END
	};

	enum UNIT_DIR
	{
		UNIT_DIR_CENTER,
		UNIT_DIR_DOWN,
		UNIT_DIR_DOWN_L,
		UNIT_DIR_DOWN_R,
		UNIT_DIR_L,
		UNIT_DIR_R,
		UNIT_DIR_UP,
		UNIT_DIR_UP_L,
		UNIT_DIR_UP_R,
		UNIT_DIR_END,
	};

	enum UNIT_TYPE
	{
		UNIT_DINO,
		UNIT_PLAYER,
		UNIT_BULLET,
		UNIT_STRUCTURE,
		UNIT_END
	};

	// 수정할때 enum STAT과 UNITSTAT 구조체 내부의 변수들 순서 똑같이 맞춰줘야 Get이랑 Set 함수 제대로 먹음
	typedef struct
	{
		// 경험치
		_float  fEXP;
		_float	fLevel;
		// 체력
		_float	fMaxHp;
		_float	fHp;

		// 기력
		_float	fMaxStamina;
		_float	fStamina;

		// 배고픔
		_float	fMaxHunger;
		_float	fHunger;

		// 목마름
		_float	fMaxThirst;
		_float	fThirst;

		// 무게
		_float	fMaxWeight;
		_float	fWeight;

		// (근접)공격력
		_float	fAtk;
		_float	fKnockBack;

		//속도
		_float	fSpeed;

		//제작 기술
		_float	fEngram;

		//인내
		_float	fEndure;

		//마비
		_float	fMaxParalysis;
		_float	fParalysis;

		//남은 스텟 포인트
		_float	fStatPoint;

		// 기절 비율 (공격력 * 기절비율 = 타격시 오르는 기절치)
		_float	fTorpidRate;
		
		_float	fMaxTamingRate;
		_float	fTamingRate;

		_float	fSize;
	}UNITSTAT;

	typedef struct tChecker_Invincible
	{
		explicit tChecker_Invincible(const CUnit* _pAttacker) { pAttacker = _pAttacker; }
		const CUnit* pAttacker = nullptr;
		_double dTime = 0;
	}CHECKER_INVINCIBLE;

	typedef struct tagBuff
	{
		_float			fBuffTime = 0.f;
		_float			fMaxBuffTime = 20.f;
		CGameObject*	pEffect = nullptr;
	}BUFF;

protected:
	explicit CUnit(LPDIRECT3DDEVICE9 _pDevice);
	explicit CUnit(const CUnit& rhs);
	virtual ~CUnit() = default;

public:
	//쉐이더, 텍스처 획득
	virtual HRESULT			Ready_GameObject_Prototype() override;
	//트랜스폼 획득
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual HRESULT			Late_Ready_GameObject(void* _pArg = nullptr) { return S_OK; }	//////////////////////////////////////////////////////////////////////////
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;		//	소멸(m_bDead), 피격무적, 시체상태 업데이트
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) { return 0; }	//
	virtual void			Render_GameObject() PURE;										//	나중에 통합

public:
	virtual CGameObject*	Clone(void * _pArg = nullptr) PURE;
	virtual void			Free();

	//쉐이더, 텍스처 획득
	HRESULT					Add_Component_Prototype();
	//트랜스폼 획득
	HRESULT					Add_Component();

public:
	UNIT_TYPE				Get_UnitType() { return m_eUnitType; }
	// 정보 관련 함수 //////////////////////////////////////////////////////////////////////////
	UNIT_STATE				Get_UnitState() {return m_eUnitState;}

	_float					Get_Stat(STAT _eStat);
	void					Set_Stat(STAT _eStat, _float _fValue);

	_vec3					Get_Pos();
	void					Set_Pos(const _vec3& _vPos);
	void					Set_Pos(const _float& _fX, const _float& _fY, const _float& _fZ);

	_float					Get_RotX();
	_float					Get_RotY();
	_float					Get_RotZ();
	void					Set_RotX(const _float& _vRot);
	void					Set_RotY(const _float& _vRot);
	void					Set_RotZ(const _float& _vRot);
	void					Set_Rot(const _float& _fX, const _float& _fY, const _float& _fZ);

	_vec3					Get_Scale();
	void					Set_Scale(const _vec3& _vScale);
	void					Set_Scale(const _float& _fScale);

	//무적상태면 true 반환, 맞아야되면 false 반환
	_bool					Is_Invincible(const CUnit* pAttacker);
	//공격자를 무적판정 리스트에 추가 - 해당 공격자에게는 일정시간동안 무적
	HRESULT					Add_Invincible(const CUnit* pAttacker);

	_bool					Get_Dying() { return m_bDying; }

	// 상호작용 관련 함수 //////////////////////////////////////////////////////////////////////////
	//누군가에게 공격(채집)을 당했을 경우 호출(공격자, 공격종류)
	virtual _bool			HitBy(CUnit* pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos);
	
	HRESULT					Add_Item(ITEM_INFO& tItem);
	ITEM_INFO				Remove_Item(ITEMID _eItemID, _int _iAmount);

	void					Add_Exp(_float fExp) { m_tStat.fEXP += fExp; }

	///////////////링 커멘드 명령 받는 함수//////////////////////////////////////////////////////////
	virtual void			Open_Ring_Command(CUnit* pUnit) { return; }
	virtual	void			Set_Ring_Command(_int _iSelectCommand) { return; }
	void					Close_Ring_Command();
	void					Init_Ring_Command() { m_wstrRing = L""; }

	//버프 함수
	protected:
		void					Check_Buff(const _double & _dTimeDelta);

	public:
		void					Add_Buff(wstring wstrBuffName, _float fBuffMaxTime, _float fScale);

	protected:
		map<wstring, BUFF>		m_mapBuff;
		_float					m_fCenterPos = 0.f;

protected:
	virtual HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수

	//클론 생성 시 호출 : 인자 검사해 트랜스폼에 적용하고 dwOption값 반환
	virtual _int			Check_Arg(void * _pArg);
	

	//특정레벨이 될 때까지 경험치를 주어 레벨업시키는 함수. 현재레벨이 더 높다면 무효
	void					Add_ExpToLevel(_ulong dwLevel);
	//현재 경험치가 레벨업할 만큼 도달했는지 체크해 레벨업시킴
	virtual _bool			Check_LevelUp() { return false; }

	virtual _bool			Add_RenderGroup_Culling(RENDERID eID, CGameObject* pGameObject);

private:
	//피격 무적상태 관련 업데이트
	virtual void			Check_Invincible(const _double& _dTimeDelta);
	//시체상태 관련 업데이트
	virtual void			Check_Dying(const _double& _dTimeDelta);
	// 스탯 업데이트(허기감소 등등)
	virtual	void			Check_Stat(const _double& _dTimeDelta);

protected:
	UNITSTAT				m_tStat;
	UNIT_STATE				m_eUnitState;
	UNIT_STATE				m_ePreUnitState;
	UNIT_DIR				m_eUnitMoveDir;

	CInven*					m_pInvenCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderShadowMapCom = nullptr;
	CShader*				m_pShaderShadowCom = nullptr;
	CTexture*				m_pDissolveTextureCom = nullptr;
	//CUnionMesh*				m_pMeshCom = nullptr;

	//공격당했을 시 무적시간
	list<CHECKER_INVINCIBLE> m_listInvincible;			//공격자별 무적시간 체크할 리스트
	_bool					 m_bInvincibleAll = false;	//모든 공격자에게 무적 (컷신, 사망연출 등등 할 때 on할 것)
	_double					 m_dInvincibleMax = 0.6;		//각 무적시간이 해제되는 시간

	// 사망 시 연출시간
	_bool					m_bDying = false;
	_bool					m_bIsDeadBody = false;
	_double					m_dDyingTime = 0;
	_double					m_dDyingMax = 1.0;

	_bool					m_bIsStatUpdate = false;
	_double					m_dStatUpdateTime = 0.0;

	// Dissolve 관련
	_float					m_fDissolveAmount = 0.f;
	_bool					m_bDissolve = false;

	UNIT_TYPE				m_eUnitType = UNIT_END;

	// 스탯 관련 변수
	_bool					m_bRecoveryStamina = false;
	_double					m_dRecoveryStaminaTime = 0.0;

	// 움질일때 흙먼지 크기
	_float					m_fDustScale = 0.f;

	//메쉬이름
	wstring					m_wstrMeshName;
	
	// 모션블러 관련
	_mat					m_matLastWorldView;

	//링커맨드
	wstring					m_wstrRing = L"";
};

#endif // !__UNIT_H__