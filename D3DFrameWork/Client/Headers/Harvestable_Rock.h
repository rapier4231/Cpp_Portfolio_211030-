	#pragma once
#ifndef __HARVESTABLE_ROCK_H__
#define __HARVESTABLE_ROCK_H__

#include "Harvestable.h"

BEGIN(Engine)
class CDynamicMesh;
END

class CHarvestable_Rock : public CHarvestable
{
private:
	explicit CHarvestable_Rock(LPDIRECT3DDEVICE9 _pDevice);
	explicit CHarvestable_Rock(const CHarvestable_Rock& rhs);
	virtual ~CHarvestable_Rock() = default;

public:
	virtual HRESULT				Ready_GameObject_Prototype() override;
	virtual HRESULT				Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int				Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int				LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void				Render_GameObject() override;
	virtual void				Render_GameObjectShadowMap() override;
	virtual void				Render_GameObjectShadow() override;
	virtual	void				Check_Dying(const _double& _dTimeDelta) override;

public:
	static CHarvestable_Rock*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;

	virtual _int				Check_Arg(void * _pArg) override;

public:
	virtual _bool				HitBy(CUnit* pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos) override;

private:
	HRESULT						Add_Component(_ulong dwOption = 0);
	_bool						m_bPowerBreak = false;

	//파괴 시 다이나믹메시+피직스 적용
	_bool									m_bBreak = false;
	_float									m_fDeleteTime = 0.f;
	PxRigidStatic*							m_pBaseActor;
	vector<tuple<PxRigidDynamic*, _mat>>	m_tupleBones;
};

#endif // !__HARVESTABLE_ROCK_H__
