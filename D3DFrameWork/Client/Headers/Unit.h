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
		STATE_CAMERAWORK,			// ī�޶� ����
		STATE_IDLE,					// �⺻����1
		STATE_IDLE2,				// �⺻����2
		STATE_IDLE3,				// �⺻����3
		STATE_MOVE,					// ������
		STATE_SPRINT,				// ��������
		STATE_JUMPING,				// �������� ��
		STATE_FALLING,				// ������ ��
		STATE_LANDING,				// ������ ��

		STATE_ATTACK,				// ����
		STATE_HURT,					// ������
		STATE_EAT,					// ���� ��
		
		STATE_HARVEST,				// �ݱ�
		STATE_INVENTORY,			// �κ��丮 �� ��
		STATE_ATCTIVATECONSOLE,		// ��� ��ȣ�ۿ�
		STATE_WHISTLE,				// ���Ķ�
		STATE_WEAPON_DRAW,			// ���� ������
		STATE_WEAPON_PUTAWAY,		// ���� �ֱ�

		STATE_WEAPON_THROW,			// ���� ������ (����, Ȱ, ����, â ���..)
		STATE_WEAPON_PULLBACK,		// ���� ������ �� �غ� (����, Ȱ, ����, â ���..)
		STATE_WEAPON_RELOAD,		// ���� ������

		STATE_HOLDING,				// ���� �¾��� ��

		STATE_DEAD,					// ����

		// ���̹�
		STATE_TORPID,				// ����1
		STATE_TORPID_IDLE,			// ����2
		STATE_TORPID_TAMED,			// ���̹� ����
		STATE_TORPID_WILD,			// ���̹� ����
		STATE_TORPID_EAT,			// ���̹� �� ���� �Ա�

		STATE_RIDE,					// ���� ž����
		STATE_ROAR,					// ���� ��ȿ
		STATE_EGG,					// ���� �� ����
		STATE_HAPPY,				// ���� �����ϴ� �ִ�
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

	// �����Ҷ� enum STAT�� UNITSTAT ����ü ������ ������ ���� �Ȱ��� ������� Get�̶� Set �Լ� ����� ����
	typedef struct
	{
		// ����ġ
		_float  fEXP;
		_float	fLevel;
		// ü��
		_float	fMaxHp;
		_float	fHp;

		// ���
		_float	fMaxStamina;
		_float	fStamina;

		// �����
		_float	fMaxHunger;
		_float	fHunger;

		// �񸶸�
		_float	fMaxThirst;
		_float	fThirst;

		// ����
		_float	fMaxWeight;
		_float	fWeight;

		// (����)���ݷ�
		_float	fAtk;
		_float	fKnockBack;

		//�ӵ�
		_float	fSpeed;

		//���� ���
		_float	fEngram;

		//�γ�
		_float	fEndure;

		//����
		_float	fMaxParalysis;
		_float	fParalysis;

		//���� ���� ����Ʈ
		_float	fStatPoint;

		// ���� ���� (���ݷ� * �������� = Ÿ�ݽ� ������ ����ġ)
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
	//���̴�, �ؽ�ó ȹ��
	virtual HRESULT			Ready_GameObject_Prototype() override;
	//Ʈ������ ȹ��
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual HRESULT			Late_Ready_GameObject(void* _pArg = nullptr) { return S_OK; }	//////////////////////////////////////////////////////////////////////////
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;		//	�Ҹ�(m_bDead), �ǰݹ���, ��ü���� ������Ʈ
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) { return 0; }	//
	virtual void			Render_GameObject() PURE;										//	���߿� ����

public:
	virtual CGameObject*	Clone(void * _pArg = nullptr) PURE;
	virtual void			Free();

	//���̴�, �ؽ�ó ȹ��
	HRESULT					Add_Component_Prototype();
	//Ʈ������ ȹ��
	HRESULT					Add_Component();

public:
	UNIT_TYPE				Get_UnitType() { return m_eUnitType; }
	// ���� ���� �Լ� //////////////////////////////////////////////////////////////////////////
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

	//�������¸� true ��ȯ, �¾ƾߵǸ� false ��ȯ
	_bool					Is_Invincible(const CUnit* pAttacker);
	//�����ڸ� �������� ����Ʈ�� �߰� - �ش� �����ڿ��Դ� �����ð����� ����
	HRESULT					Add_Invincible(const CUnit* pAttacker);

	_bool					Get_Dying() { return m_bDying; }

	// ��ȣ�ۿ� ���� �Լ� //////////////////////////////////////////////////////////////////////////
	//���������� ����(ä��)�� ������ ��� ȣ��(������, ��������)
	virtual _bool			HitBy(CUnit* pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos);
	
	HRESULT					Add_Item(ITEM_INFO& tItem);
	ITEM_INFO				Remove_Item(ITEMID _eItemID, _int _iAmount);

	void					Add_Exp(_float fExp) { m_tStat.fEXP += fExp; }

	///////////////�� Ŀ��� ��� �޴� �Լ�//////////////////////////////////////////////////////////
	virtual void			Open_Ring_Command(CUnit* pUnit) { return; }
	virtual	void			Set_Ring_Command(_int _iSelectCommand) { return; }
	void					Close_Ring_Command();
	void					Init_Ring_Command() { m_wstrRing = L""; }

	//���� �Լ�
	protected:
		void					Check_Buff(const _double & _dTimeDelta);

	public:
		void					Add_Buff(wstring wstrBuffName, _float fBuffMaxTime, _float fScale);

	protected:
		map<wstring, BUFF>		m_mapBuff;
		_float					m_fCenterPos = 0.f;

protected:
	virtual HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// ���̴� ��� ���̺� �����Լ�

	//Ŭ�� ���� �� ȣ�� : ���� �˻��� Ʈ�������� �����ϰ� dwOption�� ��ȯ
	virtual _int			Check_Arg(void * _pArg);
	

	//Ư�������� �� ������ ����ġ�� �־� ��������Ű�� �Լ�. ���緹���� �� ���ٸ� ��ȿ
	void					Add_ExpToLevel(_ulong dwLevel);
	//���� ����ġ�� �������� ��ŭ �����ߴ��� üũ�� ��������Ŵ
	virtual _bool			Check_LevelUp() { return false; }

	virtual _bool			Add_RenderGroup_Culling(RENDERID eID, CGameObject* pGameObject);

private:
	//�ǰ� �������� ���� ������Ʈ
	virtual void			Check_Invincible(const _double& _dTimeDelta);
	//��ü���� ���� ������Ʈ
	virtual void			Check_Dying(const _double& _dTimeDelta);
	// ���� ������Ʈ(��Ⱘ�� ���)
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

	//���ݴ����� �� �����ð�
	list<CHECKER_INVINCIBLE> m_listInvincible;			//�����ں� �����ð� üũ�� ����Ʈ
	_bool					 m_bInvincibleAll = false;	//��� �����ڿ��� ���� (�ƽ�, ������� ��� �� �� on�� ��)
	_double					 m_dInvincibleMax = 0.6;		//�� �����ð��� �����Ǵ� �ð�

	// ��� �� ����ð�
	_bool					m_bDying = false;
	_bool					m_bIsDeadBody = false;
	_double					m_dDyingTime = 0;
	_double					m_dDyingMax = 1.0;

	_bool					m_bIsStatUpdate = false;
	_double					m_dStatUpdateTime = 0.0;

	// Dissolve ����
	_float					m_fDissolveAmount = 0.f;
	_bool					m_bDissolve = false;

	UNIT_TYPE				m_eUnitType = UNIT_END;

	// ���� ���� ����
	_bool					m_bRecoveryStamina = false;
	_double					m_dRecoveryStaminaTime = 0.0;

	// �����϶� ����� ũ��
	_float					m_fDustScale = 0.f;

	//�޽��̸�
	wstring					m_wstrMeshName;
	
	// ��Ǻ� ����
	_mat					m_matLastWorldView;

	//��Ŀ�ǵ�
	wstring					m_wstrRing = L"";
};

#endif // !__UNIT_H__