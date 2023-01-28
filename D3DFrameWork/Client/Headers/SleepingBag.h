#pragma once
#ifndef __SLEEPINGBAG_H__
#define __SLEEPINGBAG_H__

#include "Structure.h"

class CSleepingBag : public CStructure
{
private:
	explicit CSleepingBag(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSleepingBag(const CSleepingBag& _rhs);
	virtual ~CSleepingBag() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CSleepingBag*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

public:
	void					TurnOn_Structure();		//��� �۵� ����
	void					TurnOff_Structure();	//�۵� ���� -> �Ҹ�(�����꿬��)

	_int					Get_BedID() { return m_iBedID; }

private:
	HRESULT					Add_Component();

private:
	_int					m_iBedID = -1;
};

#endif // !__CAMPFIRE_H__
