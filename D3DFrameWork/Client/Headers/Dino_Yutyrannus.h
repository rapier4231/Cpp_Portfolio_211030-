#pragma once
#ifndef __Dino_Yutyrannus_H__
#define __Dino_Yutyrannus_H__

#include "Dino.h"

class CDino_Yutyrannus : public CDino
{
public:
	enum YUTYRANNUS_ANIMATION
	{
		YUTYRANNUS_AIM_CENTER,
		YUTYRANNUS_AIM_DOWN,
		YUTYRANNUS_AIM_DOWN_L,
		YUTYRANNUS_AIM_DOWN_R,
		YUTYRANNUS_AIM_L,
		YUTYRANNUS_AIM_R,
		YUTYRANNUS_AIM_UP,
		YUTYRANNUS_AIM_UP_L,
		YUTYRANNUS_AIM_UP_R,
		//////////////////////////////////////////////////////////////////////////
		YUTYRANNUS_ANIM_ATTACK_BITE,
		YUTYRANNUS_ANIM_ATTACK_HEADBUTT,
		YUTYRANNUS_ANIM_CHARGE_FORWARD,
		YUTYRANNUS_ANIM_CHARGE_L,
		YUTYRANNUS_ANIM_CHARGE_R,
		YUTYRANNUS_ANIM_CUDDLE,
		YUTYRANNUS_ANIM_EAT,
		YUTYRANNUS_ANIM_HURT,
		YUTYRANNUS_ANIM_IDLE,
		YUTYRANNUS_ANIM_MOVE_FORWARD,
		YUTYRANNUS_ANIM_MOVE_L,
		YUTYRANNUS_ANIM_MOVE_R,
		YUTYRANNUS_ANIM_MOVE_ATTACK,
		YUTYRANNUS_ANIM_ROAR_BUFF,
		YUTYRANNUS_ANIM_ROAR_SCARED,
		YUTYRANNUS_ANIM_STARLED,
		YUTYRANNUS_ANIM_TORPID_IDLE,
		YUTYRANNUS_ANIM_TORPID_IN,
		YUTYRANNUS_ANIM_TORPID_EAT,
		YUTYRANNUS_ANIM_TORPID_OUT_TEIMED,
		YUTYRANNUS_ANIM_TORPID_OUT_WILD,
		YUTYRANNUS_ANIM_BACKWARD,
		YUTYRANNUS_ANIM_END
	};

private:
	explicit CDino_Yutyrannus(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Yutyrannus(const CDino_Yutyrannus& rhs);
	virtual ~CDino_Yutyrannus() = default;

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
	static CDino_Yutyrannus*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *			Clone(void * _pArg = nullptr) override;
	virtual void					Free() override;
	virtual void					Ride_Key_Input_Custum(const _double & _dTimeDelta) override;
	virtual void					Check_IsUpperEndAnimation() override;

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

	_bool						m_bRoarEffect = false;
	CGameObject*				m_pRoarEffect = nullptr;

};

#endif // !__Dino_Yutyrannus_H__
