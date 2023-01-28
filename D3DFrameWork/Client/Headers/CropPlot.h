#pragma once
#ifndef __CROPPLOT_H__
#define __CROPPLOT_H__

#include "Structure.h"

class CCropPlot : public CStructure
{
private:
	explicit CCropPlot(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCropPlot(const CCropPlot& _rhs);
	virtual ~CCropPlot() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CCropPlot*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

};

#endif // !__CROPPLOT_H__
