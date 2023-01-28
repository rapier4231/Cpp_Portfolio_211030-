#pragma once
#ifndef __Dino_Ankylo_H__
#define __Dino_Ankylo_H__

#include "Dino.h"

class CDino_Ankylo : public CDino
{
public:
	enum ANKYLO_ANIMATION
	{
		ANKYLO_AIM_CENTER,
		ANKYLO_AIM_DOWN,
		ANKYLO_AIM_DOWN_L,
		ANKYLO_AIM_DOWN_R,
		ANKYLO_AIM_L,
		ANKYLO_AIM_R,
		ANKYLO_AIM_UP,
		ANKYLO_AIM_UP_L,
		ANKYLO_AIM_UP_R,
		//////////////////////////////////////////////////////////////////////////
		ANKYLO_ANIM_ATTACK_TAIL_FOUND,
		ANKYLO_ANIM_ATTACK_TAIL_SWEEP,
		ANKYLO_ANIM_CHARGE_FORWARD,
		ANKYLO_ANIM_CHARGE_L,
		ANKYLO_ANIM_CHARGE_R,
		ANKYLO_ANIM_GRAZE,
		ANKYLO_ANIM_HURT_RIT,
		ANKYLO_ANIM_IDLE,
		ANKYLO_ANIM_MOVE_FORWARD,
		ANKYLO_ANIM_STRLED,
		ANKYLO_ANIM_TORPID_IDLE,
		ANKYLO_ANIM_TORPID_EAT,
		ANKYLO_ANIM_TORPID_IN,
		ANKYLO_ANIM_TORPID_OUT_TAMED,
		ANKYLO_ANIM_TORPID_OUT_WILD,
		ANKYLO_ANIM_MOVE_L,
		ANKYLO_ANIM_MOVE_R,
		ANKYLO_ANIM_BACKWARD,
		ANKYLO_ANIM_END
	};

private:
	explicit CDino_Ankylo(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Ankylo(const CDino_Ankylo& rhs);
	virtual ~CDino_Ankylo() = default;

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
	static CDino_Ankylo*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *			Clone(void * _pArg = nullptr) override;
	virtual void					Free() override;
	virtual	void					PeaceTaming(const _double & _dTimeDelta) override;

private:
	virtual void					IsRide_Set_SaddleMat() override;
	virtual	_bool					Dino_Move_Ride_Custum(const _double & _dTimeDelta) override;
	virtual	_bool					Harvest_Action(wstring& wstrTargetLayer, CGameObject* pLookObject) override;

	void							Check_State() override;

private:
	HRESULT							Add_Component();
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	/// 테스트용
	_uint						m_iIndex = 0;
	CGameObject*				m_pLookObject = nullptr;
};

#endif // !__Dino_Ankylo_H__
