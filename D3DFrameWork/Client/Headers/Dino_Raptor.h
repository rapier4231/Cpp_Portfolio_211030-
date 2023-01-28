#pragma once
#ifndef __Dino_Raptor_H__
#define __Dino_Raptor_H__

#include "Dino.h"

class CDino_Raptor : public CDino
{
public:
	enum RAPTOR_ANIMATION
	{
		RAPTOR_AIM_CENTER,
		RAPTOR_AIM_DOWN,
		RAPTOR_AIM_DOWN_L,
		RAPTOR_AIM_DOWN_R,
		RAPTOR_AIM_L,
		RAPTOR_AIM_R,
		RAPTOR_AIM_UP,
		RAPTOR_AIM_UP_L,
		RAPTOR_AIM_UP_R,
		//////////////////////////////////////////////////////////////////////////
		RAPTOR_ANIM_IDLE,
		RAPTOR_ANIM_MOVE_FORWARD,
		RAPTOR_ANIM_CHARGE_FORWARD,
		RAPTOR_ANIM_TORPID_IN,
		RAPTOR_ANIM_TORPID_IDLE,
		RAPTOR_ANIM_TORPID_EAT,
		RAPTOR_ANIM_TORPID_OUT_TEIMMED,
		RAPTOR_ANIM_TORPID_OUT_WILD,
		RAPTOR_ANIM_EAT,
		RAPTOR_ANIM_ATTACK,
		RAPTOR_ANIM_CALL,
		RAPTOR_ANIM_CUDDLE,
		RAPTOR_ANIM_FONCE_IN,
		RAPTOR_ANIM_FONCE_IDLE,
		RAPTOR_ANIM_FONCE_OUT,
		RAPTOR_ANIM_FONCE_ATTACK,
		RAPTOR_ANIM_HEART_BIG,
		RAPTOR_ANIM_JUMP_IN,
		RAPTOR_ANIM_JUMP_OUT,
		RAPTOR_ANIM_JUMP_IDLE,
		RAPTOR_ANIM_MOVE_ATTACK,
		RAPTOR_ANIM_BACKWARD,
		RAPTOR_ANIM_END
	};

private:
	explicit CDino_Raptor(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Raptor(const CDino_Raptor& rhs);
	virtual ~CDino_Raptor() = default;

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
	static CDino_Raptor*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *			Clone(void * _pArg = nullptr) override;
	virtual void					Free() override;
	virtual void					Ride_Key_Input_Custum(const _double & _dTimeDelta) override;

private:
	virtual void				IsRide_Set_SaddleMat() override;

	//애니메이션 함수
	void						Check_State() override;

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);


private:
	/// 테스트용
	_uint					m_iIndex = 0;
};

#endif // !__Dino_Raptor_H__
