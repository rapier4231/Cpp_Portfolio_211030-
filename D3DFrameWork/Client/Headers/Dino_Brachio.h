#pragma once
#ifndef __Dino_Brachio_H__
#define __Dino_Brachio_H__

#include "Dino.h"

//class CPlatformSaddle_Brachio;
class CDino_Brachio : public CDino
{
public:
	enum BRACHIO_ANIMATION
	{
		BRACHIO_AIM_CENTER,
		BRACHIO_AIM_DOWN,
		BRACHIO_AIM_DOWN_L,
		BRACHIO_AIM_DOWN_R,
		BRACHIO_AIM_L,
		BRACHIO_AIM_R,
		BRACHIO_AIM_UP,
		BRACHIO_AIM_UP_L,
		BRACHIO_AIM_UP_R,
		//////////////////////////////////////////////////////////////////////////
		BRACHIO_ANIM_IDLE,
		BRACHIO_ANIM_MOVE_FORWARD,
		BRACHIO_ANIM_MOVE_L,
		BRACHIO_ANIM_MOVE_R,
		BRACHIO_ANIM_CHARGE_FORWARD,
		BRACHIO_ANIM_CHARGE_L,
		BRACHIO_ANIM_CHARGE_R,
		BRACHIO_ANIM_TORPID_IN,
		BRACHIO_ANIM_TAIL_ATTACK_L,
		BRACHIO_ANIM_TAIL_ATTACK_R,
		BRACHIO_ANIM_BACKWARD,
		BRACHIO_ANIM_CUDDLE,
		BRACHIO_ANIM_GRAZE,
		BRACHIO_ANIM_TORPID_EAT,
		BRACHIO_ANIM_TORPID_OUT_TAMED,
		BRACHIO_ANIM_TORPID_OUT_WILD,
		BRACHIO_ANIM_TAIL_ATTACK_FOOTSTAMP,
		BRACHIO_ANIM_TAIL_STARLED_LEFT,
		BRACHIO_ANIM_TAIL_STARLED_RIGHT,
		BRACHIO_ANIM_END
	};

private:
	explicit CDino_Brachio(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Brachio(const CDino_Brachio& rhs);
	virtual ~CDino_Brachio() = default;

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
	static CDino_Brachio*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;

private:
	void						IsRide_Set_SaddleMat();

	//애니메이션 함수
	void						Check_State() override;

	void						Set_PlatformSaddle();

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	//CPlatformSaddle_Brachio*	m_pPlatformSaddle = nullptr;
	_bool						m_bIsSaddle = false;

	/// 테스트용
	_uint						m_iIndex = 0;
};

#endif // !__Dino_Brachio_H__
