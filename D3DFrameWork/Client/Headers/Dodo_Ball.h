#pragma once
#ifndef __Dodo_Ball_H__
#define __Dodo_Ball_H__

#include "Dino.h"

class CDodo_Ball : public CDino
{
public:
	enum DODO_ANIMATION
	{
		DODO_AIM_CENTER,
		DODO_AIM_DOWN,
		DODO_AIM_DOWN_L,
		DODO_AIM_DOWN_R,
		DODO_AIM_L,
		DODO_AIM_R,
		DODO_AIM_UP,
		DODO_AIM_UP_L,
		DODO_AIM_UP_R,
		//////////////////////////////////////////////////////////////////////////
		DODO_ANIM_ATTACK,
		DODO_ANIM_CHARGE_FOWARD,
		DODO_ANIM_CHARGE_LEFT,
		DODO_ANIM_CHARGE_RIGHT,
		DODO_ANIM_CUDDLE,
		DODO_ANIM_EAT,
		DODO_ANIM_GRAZE,
		DODO_ANIM_HURT_BIG,
		DODO_ANIM_STARLED,
		DODO_ANIM_MOVE_BACKWARD,
		DODO_ANIM_MOVE_FWORD,
		DODO_ANIM_IDLE,
		DODO_ANIM_TORPID_IN, // 사망
		DODO_ANIM_TORPID_EAT,
		DODO_ANIM_TORPID_IDLE,
		DODO_ANIM_TORPID_OUT_TAMED,
		DODO_ANIM_TORPID_OUT_WILD,
		DODO_ANIM_END
	};

private:
	explicit CDodo_Ball(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDodo_Ball(const CDodo_Ball& rhs);
	virtual ~CDodo_Ball() = default;

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
	static CDodo_Ball*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;
	void						Throw_Ball();

private:
	//애니메이션 함수
	void						Check_State() override;

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	/// 테스트용
	_uint						m_iIndex = 0;

	_bool						m_bThrow = false;
	PxRigidDynamic*				m_pDynamic = nullptr;

	const _mat*					m_pBoneMatrix = nullptr;
	const _mat*					m_pParentWorld = nullptr;
};

#endif // !__Dodo_Ball_H__
