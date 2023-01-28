#pragma once
#ifndef __MORTARPESTAL_H__
#define __MORTARPESTAL_H__

#include "Structure.h"

class CMortarPestal : public CStructure
{
private:
	explicit CMortarPestal(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMortarPestal(const CMortarPestal& _rhs);
	virtual ~CMortarPestal() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CMortarPestal*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

};

#endif // !__MORTARPESTAL_H__
