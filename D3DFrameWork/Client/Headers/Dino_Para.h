#pragma once
#ifndef __DINO_PARA_H__
#define __DINO_PARA_H__

#include "Dino.h"

class CDino_Para : public CDino
{
public:
	enum PARA_ANIMATION
	{
		PARA_AIM_CENTER,
		PARA_AIM_DOWN,
		PARA_AIM_DOWN_L,
		PARA_AIM_DOWN_R,
		PARA_AIM_L,
		PARA_AIM_R,
		PARA_AIM_UP,
		PARA_AIM_UP_L,
		PARA_AIM_UP_R,
		//////////////////////////////////////////////////////////////////////////
		PARA_ATTACK,
		PARA_CHARGE_FOWARD,
		PARA_MOVE_BACKWARD,
		PARA_MOVE_FWORD,
		PARA_ANIM_IDLE,
		PARA_ANIM_TORPID_IN, // 사망
		PARA_ANIM_ROAR_ALERT,
		PARA_ANIM_TERRET_CHECK,
		PARA_ANIM_IDLE_GRAZE,
		PARA_ANIM_TORPID_EAT,
		PARA_ANIM_TORPID_IDLE,
		PARA_ANIM_TORPID_OUT_TAMED,
		PARA_ANIM_TORPID_OUT_WILD,
		PARA_ANIM_END
	};

private:
	explicit CDino_Para(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Para(const CDino_Para& rhs);
	virtual ~CDino_Para() = default;

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
	static CDino_Para*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;
	virtual void				Ride_Key_Input_Custum(const _double & _dTimeDelta) override;

private:
	void						IsRide_Set_SaddleMat();
	virtual _bool				Harvest_Action(wstring& wstrTargetLayer, CGameObject* pLookObject) override;
	//애니메이션 함수
	void						Check_State() override;

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	/// 테스트용
	_uint						m_iIndex = 0;

};

#endif // !__Dino_Para_H__
