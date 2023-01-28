#pragma once
#ifndef __THATCHBED_H__
#define __THATCHBED_H__

#include "Structure.h"

class CThatchBed : public CStructure
{
private:
	explicit CThatchBed(LPDIRECT3DDEVICE9 _pDevice);
	explicit CThatchBed(const CThatchBed& _rhs);
	virtual ~CThatchBed() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CThatchBed*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

public:
	void					TurnOn_Structure();		//기능 작동 시작
	void					TurnOff_Structure();	//작동 종료 -> 소멸(디졸브연출)

	_int					Get_BedID() { return m_iBedID; }

private:
	HRESULT					Add_Component();

private:
	_int					m_iBedID = -1;
};

#endif // !__THATCHBED_H__
