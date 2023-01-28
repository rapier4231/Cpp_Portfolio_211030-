#pragma once
#ifndef __Dino_Therizino_H__
#define __Dino_Therizino_H__

#include "Dino.h"

class CDino_Therizino : public CDino
{
public:
	enum THERIZINO_ANIMATION
	{
		THERIZINO_AIM_CENTER,
		THERIZINO_AIM_DOWN,
		THERIZINO_AIM_DOWN_L,
		THERIZINO_AIM_DOWN_R,
		THERIZINO_AIM_L,
		THERIZINO_AIM_R,
		THERIZINO_AIM_UP,
		THERIZINO_AIM_UP_L,
		THERIZINO_AIM_UP_R,
		//////////////////////////////////////////////////////////////////////////
		THERIZINO_ANIM_IDLE,
		THERIZINO_ANIM_MOVE_FORWARD,
		THERIZINO_ANIM_MOVE_L,
		THERIZINO_ANIM_MOVE_R,
		THERIZINO_ANIM_CHARGE_FORWARD,
		THERIZINO_ANIM_CHARGE_L,
		THERIZINO_ANIM_CHARGE_R,
		THERIZINO_ANIM_TORPID_IN,
		THERIZINO_ANIM_STOP_HARVEST,
		THERIZINO_ANIM_MOVE_HARVEST,
		THERIZINO_ANIM_ATTACK_BITE,
		THERIZINO_ANIM_ATTACK_CLAW1,
		THERIZINO_ANIM_ATTACK_CLAW2,
		THERIZINO_ANIM_BACKWARD,
		THERIZINO_ANIM_MOVE_ATTACK,
		THERIZINO_ANIM_TORPID_IDLE,
		THERIZINO_ANIM_TORPID_OUT_TAMED,
		THERIZINO_ANIM_TORPID_OUT_WILD,
		THERIZINO_ANIM_STRLED,
		THERIZINO_ANIM_TORPID_EAT,
		THERIZINO_ANIM_END
	};

private:
	explicit CDino_Therizino(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Therizino(const CDino_Therizino& rhs);
	virtual ~CDino_Therizino() = default;

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
	static CDino_Therizino*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *			Clone(void * _pArg = nullptr) override;
	virtual void					Free() override;
	virtual	_bool					Before_Sort_TagetList() override;
	virtual void					PeaceTaming(const _double & _dTimeDelta) override;

private:
	virtual void				IsRide_Set_SaddleMat() override;
	virtual _bool				Harvest_Action(wstring& wstrTargetLayer, CGameObject* pLookObject) override;
	//애니메이션 함수
	void						Check_State() override;

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);


private:
	/// 테스트용
	_uint						m_iIndex = 0;
	
	_bool						m_bEatDodo = false;
};

#endif // !__Dino_Therizino_H__
