#pragma once
#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "Structure.h"

class CStorage : public CStructure
{
private:
	explicit CStorage(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStorage(const CStorage& _rhs);
	virtual ~CStorage() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CStorage*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

};

#endif // !__STORAGE_H__
