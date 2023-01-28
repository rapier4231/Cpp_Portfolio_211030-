#pragma once
#ifndef __DINO_REX_H__
#define __DINO_REX_H__

#include "Dino.h"

class CDino_Rex : public CDino
{
public:
	enum REX_ANIMATION
	{
		REX_A_AIM_CENTER,
		REX_A_AIM_DOWN,
		REX_A_AIM_DOWN_L,
		REX_A_AIM_DOWN_R,
		REX_A_AIM_L,
		REX_A_AIM_R,
		REX_A_AIM_UP,
		REX_A_AIM_UP_L,
		REX_A_AIM_UP_R,
		REX_A_BITE,
		REX_A_CHARGE_FWD,
		REX_A_CHARGE_LFT,
		REX_A_CHARGE_RIT,
		REX_A_CUDDLE,
		REX_A_EAT,
		REX_A_FALLING,
		REX_A_HURT,
		REX_A_IDLE,
		REX_A_EGG,
		REX_A_MOVE_FWD,
		REX_A_MOVE_LFT,
		REX_A_MOVE_RIT,
		REX_A_ROAR,
		REX_A_STARTLED_LFT,
		REX_A_STARTLED_RIT,
		REX_A_TORPID_IDLE,
		REX_A_TORPID_EAT,
		REX_A_TORPID_IN,
		REX_A_TORPID_OUT_TAMED,
		REX_A_TORPID_OUT_WILD,
		REX_A_MOVE_BWD,
		REX_A_END
	};

private:
	explicit CDino_Rex(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Rex(const CDino_Rex& rhs);
	virtual ~CDino_Rex() = default;

public:
	virtual HRESULT				Ready_GameObject_Prototype() override;
	virtual HRESULT				Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int				Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int				LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void				Render_GameObject() override;
	virtual void				Render_GameObjectShadowMap() override;
	virtual void				Render_GameObjectShadow() override;
	
public:
	virtual _bool				HitBy(CUnit* pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos) override;
	virtual void				IsRide_Set_SaddleMat() override;
	virtual void				Set_Growth_Dino_Data() override;
	virtual void				Dino_Mating(const _double & _dTimeDelta) override;
public:
	static CDino_Rex*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;

private:
	void						Check_State() override;
	HRESULT						Rex_Setting(_bool bCreate, _float fDivide = 1.f);

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
};

#endif // !__DINO_REX_H__
