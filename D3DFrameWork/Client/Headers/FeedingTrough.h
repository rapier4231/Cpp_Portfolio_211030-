#pragma once
#ifndef __FEEDINGTOUGH_H__
#define __FEEDINGTOUGH_H__

#include "Structure.h"

class CFeedingTrough : public CStructure
{
private:
	explicit CFeedingTrough(LPDIRECT3DDEVICE9 _pDevice);
	explicit CFeedingTrough(const CFeedingTrough& _rhs);
	virtual ~CFeedingTrough() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CFeedingTrough*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

private:
	HRESULT					Add_Component();

private:
	CStaticMesh*			m_pMeshEmpty = nullptr;
};

#endif // !__FEEDINGTOUGH_H__
