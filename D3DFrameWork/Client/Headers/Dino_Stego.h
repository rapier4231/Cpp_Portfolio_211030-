#pragma once
#ifndef __Dino_Stego_H__
#define __Dino_Stego_H__

#include "Dino.h"

class CDino_Stego : public CDino
{
public:
	enum STEGO_ANIMATION
	{
		STEGO_AIM_CENTER,
		STEGO_AIM_DOWN,
		STEGO_AIM_DOWN_L,
		STEGO_AIM_DOWN_R,
		STEGO_AIM_L,
		STEGO_AIM_R,
		STEGO_AIM_UP,
		STEGO_AIM_UP_L,
		STEGO_AIM_UP_R,
		//////////////////////////////////////////////////////////////////////////
		STEGO_ANIM_IDLE,
		STEGO_ANIM_MOVE_FORWARD,
		STEGO_ANIM_CHARGE_FORWARD,
		STEGO_ANIM_CHARGE_LEFT,
		STEGO_ANIM_CHARGE_RIGHT,
		STEGO_ANIM_TORPID_IN,
		STEGO_ANIM_TORPID_IDLE,
		STEGO_ANIM_TORPID_EAT,
		STEGO_ANIM_TORPID_OUT_TEIMMED,
		STEGO_ANIM_TORPID_OUT_WILD,
		STEGO_ANIM_CUDDLE,
		STEGO_ANIM_IDLE2,
		STEGO_ANIM_IDLE3,
		STEGO_ANIM_GRAZE,
		STEGO_ANIM_HURT_BIG,
		STEGO_ANIM_SHAKE,
		STEGO_ANIM_ATTACK_L,
		STEGO_ANIM_ATTACK_R,
		STEGO_ANIM_TURN_L,
		STEGO_ANIM_TURN_R,
		STEGO_ANIM_BACKWARD,
		STEGO_ANIM_END
	};

private:
	explicit CDino_Stego(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Stego(const CDino_Stego& rhs);
	virtual ~CDino_Stego() = default;

public:
	virtual HRESULT				Ready_GameObject_Prototype() override;
	virtual HRESULT				Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int				Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int				LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void				Render_GameObject() override;
	virtual void				Render_GameObjectShadowMap() override;
	virtual void				Render_GameObjectShadow() override;
	virtual _bool				HitBy(CUnit* pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos) override;

public:
	static CDino_Stego*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *			Clone(void * _pArg = nullptr) override;
	virtual void					Free() override;

private:
	virtual void				IsRide_Set_SaddleMat() override;
	virtual _bool				Dino_Move_Ride_Custum(const _double & _dTimeDelta) override;
	//애니메이션 함수
	void						Check_State() override;

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);


private:
	/// 테스트용
	_uint						m_iIndex = 0;

};

#endif // !__Dino_Stego_H__
