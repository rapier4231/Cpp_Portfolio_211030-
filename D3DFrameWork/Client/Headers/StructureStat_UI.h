#pragma once
#ifndef __StructureStat_UI_H__
#define __StructureStat_UI_H__

#include "Value.h"
#include "Center_UI.h"

class CStructureStat_UI : public CCenter_UI
{

private:
	explicit CStructureStat_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStructureStat_UI(const CStructureStat_UI& rhs);
	virtual ~CStructureStat_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	CUnit*					Get_pStructure() {return m_pStructure;}

public:
	virtual void			StructureStat_UI_Close();

public:
	static CStructureStat_UI*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	virtual void			Inventory_Render_On(InvenUserUnitTypeID _InvenID , CUnit* _pStructureStat = nullptr);
	virtual void			Inventory_Render_Off(_bool _bDelete_pUnit = true);

private:
	CUnit*					m_pStructure = nullptr;
};

#endif // !__StructureStat_UI_H__