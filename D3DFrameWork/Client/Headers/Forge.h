#pragma once
#ifndef __FORGE_H__
#define __FORGE_H__

#include "Structure.h"

class CForge : public CStructure
{
private:
	explicit CForge(LPDIRECT3DDEVICE9 _pDevice);
	explicit CForge(const CForge& _rhs);
	virtual ~CForge() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CForge*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

};

#endif // !__FORGE_H__
