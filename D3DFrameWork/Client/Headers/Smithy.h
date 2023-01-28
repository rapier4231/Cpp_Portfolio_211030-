#pragma once
#ifndef __SMITHY_H__
#define __SMITHY_H__

#include "Structure.h"

class CSmithy : public CStructure
{
private:
	explicit CSmithy(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSmithy(const CSmithy& _rhs);
	virtual ~CSmithy() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CSmithy*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

};

#endif // !__SMITHY_H__
