#pragma once
#ifndef __COOKINGCAMPFIRE_H__
#define __COOKINGCAMPFIRE_H__

//#include "Unit.h"
#include "Structure.h"

BEGIN(Engine)
//class CTransform;
//class CShader;
//class CStaticMesh;
//class CSphereCollider;
END

class CFire;
class CFireTest;
class CFireSmoke;
class CCookingCampfire : public CStructure
{
private:
	explicit CCookingCampfire(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCookingCampfire(const CCookingCampfire& _rhs);
	virtual ~CCookingCampfire() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CCookingCampfire*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;

public:
	_bool					Set_Ignition(_bool _bIsIgnition);
	_bool					Get_Ignition() { return m_bIsIgnition; }

private:
	HRESULT					Add_Component();

private:
	void					Check_Roast(const _double & _dTimeDelta);
	_int					Get_BedID() { return -1; }

private:
	_bool					m_bIsIgnition = false;
	_double					m_dIgnitionTime = 0.0;
	_double					m_dRoastTime = 0.0;

	// 모닥불에 붙은 불
	//CFire*					pFire = nullptr;
	CFireTest*				pFire = nullptr;
	CFireSmoke*				pFireSmoke = nullptr;

	_float					m_fLightRangeMin, m_fLightRangeMax;
};

#endif // !__COOKINGCAMPFIRE_H__
