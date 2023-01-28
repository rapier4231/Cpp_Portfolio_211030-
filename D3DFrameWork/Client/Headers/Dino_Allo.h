#pragma once
#ifndef __Dino_Allo_H__
#define __Dino_Allo_H__

#include "Dino.h"

class CDino_Allo : public CDino
{
public:
	enum Allo_ANIMATION
	{
		Allo_AIM_CENTER,
		Allo_AIM_DOWN,
		Allo_AIM_DOWN_L,
		Allo_AIM_DOWN_R,
		Allo_AIM_L,
		Allo_AIM_R,
		Allo_AIM_UP,
		Allo_AIM_UP_L,
		Allo_AIM_UP_R,
		//////////////////////////////////////////////////////////////////////////
		Allo_ANIM_IDLE,
		Allo_ANIM_MOVE_FORWARD,
		Allo_ANIM_MOVE_L,
		Allo_ANIM_MOVE_R,
		Allo_ANIM_CHARGE_FORWARD,
		Allo_ANIM_CHARGE_L,
		Allo_ANIM_CHARGE_R,
		Allo_ANIM_TORPID_IN,
		Allo_ANIM_TORPID_EAT,
		Allo_ANIM_TORPID_IDLE,
		Allo_ANIM_TORPID_OUT_TEIMED,
		Allo_ANIM_TORPID_OUT_WILD,
		Allo_ANIM_SMALL_HEART,
		Allo_ANIM_ATTACK_BITE,
		Allo_ANIM_ATTACK_GORE,
		Allo_ANIM_CUDDLE,
		Allo_ANIM_STARLED,
		Allo_ANIM_BACKWARD,
		Allo_ANIM_EAT,
		Allo_ANIM_END
	};

private:
	explicit CDino_Allo(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Allo(const CDino_Allo& rhs);
	virtual ~CDino_Allo() = default;

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
	static CDino_Allo*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;

private:
	virtual void				IsRide_Set_SaddleMat() override;

	//애니메이션 함수
	void						Check_State() override;

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	/// 테스트용
	_uint						m_iIndex = 0;

};

#endif // !__Dino_Allo_H__
