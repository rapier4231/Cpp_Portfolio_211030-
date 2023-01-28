#pragma once
#ifndef __Dino_Ptero_H__
#define __Dino_Ptero_H__

#include "Dino.h"
#include "Quaternion.hpp"
class CDino_Ptero : public CDino
{
public:
	enum PTERO_ANIMATION
	{
		//////////////////////////////////////////////////////////////////////////
		PTERO_ANIM_BARREL_ROLL_RIT,
		PTERO_ANIM_FLY_ATTACK_BITE,
		PTERO_ANIM_FLY_ATTACK_FEET_OUT,
		PTERO_ANIM_FLY_ATTACK_SPRINT,
		PTERO_ANIM_FLY_ATTACK_SWOOP_OUT,
		PTERO_ANIM_FLY_FWD,
		PTERO_ANIM_FLY_HURT,
		PTERO_ANIM_FLY_IDE,
		PTERO_ANIM_FLY_LEFT,
		PTERO_ANIM_FLY_RIGHT,
		PTERO_ANIM_GROUND_ATACK,
		PTERO_ANIM_GROUND_IDLE,
		PTERO_ANIM_GROUND_MOVE_FWD,
		PTERO_ANIM_LAND,
		PTERO_ANIM_TAKE_OFF,
		PTERO_ANIM_TORPID_EAT,
		PTERO_ANIM_TORPID_IDLE,
		PTERO_ANIM_TORPID_IN,
		PTERO_ANIM_TORPID_OUT_TAMED,
		PTERO_ANIM_TORPID_OUT_WILD,
		PTERO_ANIM_GROUND_BACKWARD,
		PTERO_ANIM_END
	};

private:
	explicit CDino_Ptero(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Ptero(const CDino_Ptero& rhs);
	virtual ~CDino_Ptero() = default;

public:
	virtual HRESULT					Ready_GameObject_Prototype() override;
	virtual HRESULT					Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int					Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int					LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void					Render_GameObject() override;
	virtual void					Render_GameObjectShadowMap() override;
	virtual void					Render_GameObjectShadow() override;
	virtual _bool					HitBy(CUnit* pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos) override;

public:
	static CDino_Ptero*				Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *			Clone(void * _pArg = nullptr) override;
	virtual void					Free() override;

private:
	virtual void					IsRide_Set_SaddleMat() override;
	virtual _bool					Dino_Move_Ride_Custum(const _double & _dTimeDelta) override;
	virtual void					Ride_Key_Input_Custum(const _double & _dTimeDelta) override;
	virtual void					Dino_Move_Fly(const _double & _dTimeDelta) override;
	void							Check_State() override;

private:
	HRESULT							Add_Component();
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

	void							RotationY();
	_bool							Ground_Check();
private:
	/// 테스트용
	_uint							m_iIndex = 0;
	PxU32							m_Pxflags = 0;

	_vec3							m_vecBorder[16];

	Quaternion						m_QuatDirection;
	_int							m_iNextIndex;

	_float							m_fFlyTime = 0.f;
	_bool							m_bFalling = false;
};

#endif // !__Dino_Ptero_H__
