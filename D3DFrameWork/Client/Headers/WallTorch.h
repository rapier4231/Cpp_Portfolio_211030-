#pragma once
#ifndef __WALLTORCH_H__
#define __WALLTORCH_H__

#include "Structure.h"

class CWallTorch : public CStructure
{
private:
	explicit CWallTorch(LPDIRECT3DDEVICE9 _pDevice);
	explicit CWallTorch(const CWallTorch& _rhs);
	virtual ~CWallTorch() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CWallTorch*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

};

#endif // !__WALLTORCH_H__
