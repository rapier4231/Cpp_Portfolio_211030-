	#pragma once
#ifndef __HARVESTABLE_TREE_H__
#define __HARVESTABLE_TREE_H__

#include "Harvestable.h"

BEGIN(Engine)
END

class CHarvestable_Tree : public CHarvestable
{
private:
	explicit CHarvestable_Tree(LPDIRECT3DDEVICE9 _pDevice);
	explicit CHarvestable_Tree(const CHarvestable_Tree& rhs);
	virtual ~CHarvestable_Tree() = default;

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
	static CHarvestable_Tree*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *		Clone(void * _pArg = nullptr) override;
	virtual void				Free() override;

	virtual _int				Check_Arg(void * _pArg) override;

public:
	virtual _bool				HitBy(CUnit* pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos) override;

private:
	HRESULT						Add_Component(_ulong dwOption = 0);

	//파괴 시 수동 애니메이션 적용
	_bool						m_bBreak = false;
	_vec3						m_vDirBreak;
	_float						m_fDeleteTime = 0.f;
	
	//피직스 캡슐충돌
	PxCapsuleController*		m_pBaseActor;

	//나무의 키! 그림자 드리우는 길이 추정에 사용
	_float						m_fHeight = 0.f;

	_vec4						m_vColorInstancing;
};

#endif // !__HARVESTABLE_TREE_H__
