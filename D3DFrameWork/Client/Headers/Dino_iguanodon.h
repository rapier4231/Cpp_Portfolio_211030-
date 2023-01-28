#pragma once
#ifndef __Dino_Iguanodon_H__
#define __Dino_Iguanodon_H__

#include "Dino.h"

class CDino_Iguanodon : public CDino
{
public:
	enum IGUANODON_ANIMATION
	{
		IGUANODON_AIM_CENTER,
		IGUANODON_AIM_DOWN,
		IGUANODON_AIM_DOWN_L,
		IGUANODON_AIM_DOWN_R,
		IGUANODON_AIM_L,
		IGUANODON_AIM_R,
		IGUANODON_AIM_UP,
		IGUANODON_AIM_UP_L,
		IGUANODON_AIM_UP_R,
		//////////////////////////////////////////////////////////////////////////
		IGUANODON_ANIM_IDLE,
		IGUANODON_ANIM_MOVE_FORWARD,
		IGUANODON_ANIM_MOVE_L,
		IGUANODON_ANIM_MOVE_R,
		IGUANODON_ANIM_CHARGE_FORWARD,
		IGUANODON_ANIM_CHARGE_L,
		IGUANODON_ANIM_CHARGE_R,
		IGUANODON_ANIM_TORPID_IN,
		IGUANODON_ANIM_STOP_ATTACK,
		IGUANODON_ANIM_STOP_ATTACK2,
		IGUANODON_ANIM_MOVE_ATTACK,
		IGUANODON_ANIM_MOVE_ATTACK2,
		IGUANODON_ANIM_JUMP_START,
		IGUANODON_ANIM_JUMP_IDLE,
		IGUANODON_ANIM_JUMP_END,
		IGUANODON_ANIM_BACKWARD,
		IGUANODON_ANIM_TORPID_EAT,
		IGUANODON_ANIM_TORPID_IDLE,
		IGUANODON_ANIM_TORPID_OUT_TAMED,
		IGUANODON_ANIM_TORPID_OUT_WILD,
		IGUANODON_ANIM_HARVEST,
		IGUANODON_ANIM_END
	};

private:
	explicit CDino_Iguanodon(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Iguanodon(const CDino_Iguanodon& rhs);
	virtual ~CDino_Iguanodon() = default;

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
	static CDino_Iguanodon*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *			Clone(void * _pArg = nullptr) override;
	virtual void					Free() override;

private:
	virtual void					IsRide_Set_SaddleMat() override;
	virtual _bool					Harvest_Action(wstring& wstrTargetLayer, CGameObject* pLookObject) override;

	void							Check_State() override;

private:
	HRESULT							Add_Component();
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);


private:
	/// 테스트용
	_uint						m_iIndex = 0;

};

#endif // !__Dino_Iguanodon_H__
