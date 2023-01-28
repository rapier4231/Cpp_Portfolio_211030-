#pragma once
#ifndef __THATCH_FLOOR_H__
#define __THATCH_FLOOR_H__

#include "Structure.h"

class CThatch_Floor : public CStructure
{
private:
	explicit CThatch_Floor(LPDIRECT3DDEVICE9 _pDevice);
	explicit CThatch_Floor(const CThatch_Floor& _rhs);
	virtual ~CThatch_Floor() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CThatch_Floor*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

public:
	virtual	void			Move_Pivot() override;

public:
	virtual const _vec3		Get_PivotDir(_uint _eCorDir);

private:
	HRESULT					Add_Component();
};

#endif // !__THATCH_FLOOR_H__
