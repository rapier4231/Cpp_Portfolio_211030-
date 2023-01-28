#pragma once
#ifndef __GATEFRAME_H__
#define __GATEFRAME_H__

#include "Structure.h"

class CGateFrame : public CStructure
{
private:
	explicit CGateFrame(LPDIRECT3DDEVICE9 _pDevice);
	explicit CGateFrame(const CGateFrame& _rhs);
	virtual ~CGateFrame() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CGateFrame*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

public:
	virtual const _vec3		Get_PivotDir(_uint _eCorDir);

private:
	HRESULT					Add_Component();

};

#endif // !__GATEFRAME_H__
