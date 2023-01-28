#pragma once
#ifndef __COMPOSTBIN_H__
#define __COMPOSTBIN_H__

#include "Structure.h"

class CCompostBin : public CStructure
{
private:
	explicit CCompostBin(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCompostBin(const CCompostBin& _rhs);
	virtual ~CCompostBin() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CCompostBin*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

};

#endif // !__COMPOSTBIN_H__
