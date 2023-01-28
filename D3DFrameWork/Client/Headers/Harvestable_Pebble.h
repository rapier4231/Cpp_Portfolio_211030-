#pragma once
#ifndef __HARVESTABLE_PEBBLE_H__
#define __HARVESTABLE_PEBBLE_H__

#include "Harvestable.h"

class CHarvestable_Pebble : public CHarvestable
{
private:
	explicit CHarvestable_Pebble(LPDIRECT3DDEVICE9 _pDevice);
	explicit CHarvestable_Pebble(const CHarvestable_Pebble& rhs);
	virtual ~CHarvestable_Pebble() = default;

public:
	virtual HRESULT				Ready_GameObject_Prototype() override;
	virtual HRESULT				Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int				Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int				LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void				Render_GameObject() override;
	virtual void				Render_GameObjectShadowMap() override;
	virtual void				Render_GameObjectShadow() override;
	virtual void				Setup_Instance(LPD3DXEFFECT & _pEffect) override;

public:
	static CHarvestable_Pebble*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;

	virtual _int				Check_Arg(void * _pArg) override;
public:
	virtual _bool				HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos) override;

private:
	HRESULT						Add_Component(_ulong dwOption = 0);
};

#endif // !__HARVESTABLE_PEBBLE_H__
