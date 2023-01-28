#pragma once
#ifndef __WATERWELL_H__
#define __WATERWELL_H__

#include "Structure.h"

class CWaterWell : public CStructure
{
private:
	explicit CWaterWell(LPDIRECT3DDEVICE9 _pDevice);
	explicit CWaterWell(const CWaterWell& _rhs);
	virtual ~CWaterWell() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CWaterWell*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

};

#endif // !__WATERWELL_H__
