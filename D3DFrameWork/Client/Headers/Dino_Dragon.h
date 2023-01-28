#pragma once
#ifndef __Dino_Dragon_H__
#define __Dino_Dragon_H__

#include "Dino.h"

class CDino_Dragon : public CDino
{
public:
	enum DRAGON_ANIMATION
	{
		DRAGON_ANIM_FLY_ATTACK_FIRE,
		DRAGON_ANIM_FLY_ATTACK_SPIT_FIRE,
		DRAGON_ANIM_FLY_ATTACK_SWOOP_OUT,
		DRAGON_ANIM_FLY_FWD,
		DRAGON_ANIM_FLY_IDLE,
		DRAGON_ANIM_FLY_LEFT,
		DRAGON_ANIM_FLY_RIGHT,
		DRAGON_ANIM_GROUND_ATTACK_BITE,
		DRAGON_ANIM_GROUND_ATTACK_WING,
		DRAGON_ANIM_GROUND_ATTACK_FIRE_ALT,
		DRAGON_ANIM_GROUND_ATTACK_FIRE,
		DRAGON_ANIM_GROUND_CHARGE_FWD,
		DRAGON_ANIM_GROUND_MOVE_FWD,
		DRAGON_ANIM_GROUND_DIE,
		DRAGON_ANIM_GROUND_IDLE,
		DRAGON_ANIM_GROUND_LAND,
		DRAGON_ANIM_GROUND_TAKE_OFF,
		DRAGON_ANIM_END
	};

private:
	explicit CDino_Dragon(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Dragon(const CDino_Dragon& rhs);
	virtual ~CDino_Dragon() = default;

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
	static CDino_Dragon*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *			Clone(void * _pArg = nullptr) override;
	virtual void					Free() override;

private:
	virtual void					IsRide_Set_SaddleMat() override;

	void							Check_State() override;

private:
	HRESULT							Add_Component();
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	_uint							m_iIndex = 0;
};

#endif // !__Dino_Dragon_H__
