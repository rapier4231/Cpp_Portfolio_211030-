#pragma once
#ifndef __Dino_Quetzal_H__
#define __Dino_Quetzal_H__

#include "Dino.h"
#include "Quaternion.hpp"

class CDino_Quetzal : public CDino
{
public:
	enum QUETZAL_ANIMATION
	{
		QUETZAL_CUDDLE,
		QUETZAL_ATTACK_BITE,
		QUETZAL_FLAP_FLY_FWD,
		QUETZAL_FLY_FWD,
		QUETZAL_GROUND_IDLE,
		QUETZAL_GROUND_IDLE_PLATFORM,
		QUETZAL_GROUND_MOVE_PLATFORM,
		QUETZAL_GROUND_STARLED,
		QUETZAL_GROUND_LAND_PLATFORM,
		QUETZAL_TAKE_OFF_PLATFORM,
		QUETZAL_TORPID_EAT,
		QUETZAL_TORPID_IDLE,
		QUETZAL_TORPID_IN,
		QUETZAL_TORPID_OUT_TAMED,
		QUETZAL_TORPID_OUT_WILD,
		QUETZAL_GROUND_BACKWARD,
		QUETZAL_ANIM_END
	};

private:
	explicit CDino_Quetzal(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Quetzal(const CDino_Quetzal& rhs);
	virtual ~CDino_Quetzal() = default;

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
	static CDino_Quetzal*			Create(LPDIRECT3DDEVICE9 _pDevice);
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
	_uint						m_iIndex = 0;
	_vec3						m_vecBorder[6];


	Quaternion					m_QuatDirection;
	_int						m_iNextIndex;

	_float						m_fFlyTime = 0.f;
	_bool						m_bFalling = false;
	PxU32						m_Pxflags = 0;
};

#endif // !__Dino_Quetzal_H__
