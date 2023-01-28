#pragma once
#ifndef __FIREPLACE_H__
#define __FIREPLACE_H__

#include "Structure.h"

class CFireplace : public CStructure
{
private:
	explicit CFireplace(LPDIRECT3DDEVICE9 _pDevice);
	explicit CFireplace(const CFireplace& _rhs);
	virtual ~CFireplace() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CFireplace*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

};

#endif // !__FIREPLACE_H__
