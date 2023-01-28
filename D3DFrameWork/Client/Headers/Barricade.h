#pragma once
#ifndef __BARRICADE_H__
#define __BARRICADE_H__

#include "Structure.h"

class CBarricade : public CStructure
{
private:
	explicit CBarricade(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBarricade(const CBarricade& _rhs);
	virtual ~CBarricade() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CBarricade*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

};

#endif // !__WOODBARRICADE_H__
