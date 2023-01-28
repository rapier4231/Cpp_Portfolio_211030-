#pragma once
#ifndef __THATCH_DOORFRAME_H__
#define __THATCH_DOORFRAME_H__

#include "Structure.h"

class CThatch_DoorFrame : public CStructure
{
private:
	explicit CThatch_DoorFrame(LPDIRECT3DDEVICE9 _pDevice);
	explicit CThatch_DoorFrame(const CThatch_DoorFrame& _rhs);
	virtual ~CThatch_DoorFrame() = default;

public:
	virtual HRESULT				Ready_GameObject_Prototype() override;
	virtual HRESULT				Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int				Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int				LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void				Render_GameObject() override;

public:
	static CThatch_DoorFrame*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*		Clone(void * _pArg = nullptr) override;

public:
	virtual const _vec3			Get_PivotDir(_uint _eCorDir);

private:
	HRESULT						Add_Component();
};

#endif // !__THATCH_DOORFRAME_H__
