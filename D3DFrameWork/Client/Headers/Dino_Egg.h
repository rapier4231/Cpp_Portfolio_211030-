#pragma once
#ifndef __Dino_Egg_H__
#define __Dino_Egg_H__

#include "Unit.h"

BEGIN(Engine)
class CStaticMesh;
class CSphereCollider;
END

class CDino_Egg : public CUnit
{
private:
	enum PARENTS
	{
		MOTHER, FATHER
	};

protected:
	explicit CDino_Egg(LPDIRECT3DDEVICE9 _pDevice);
	explicit CDino_Egg(const CDino_Egg& _rhs);
	virtual ~CDino_Egg() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double & _dTimeDelta) override;

	virtual void			Render_GameObject() override;

public:
	static CDino_Egg*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *	Clone(void * _pArg = nullptr) override;
	virtual void			Free();

public:
	void					Set_ParentsName(wstring& wstrFater, wstring& wstrMother);
	_float					Get_HatchingTime() { return m_fHatchingTime; }

	wstring&				Get_Msg() { return m_wstrMsg; }
protected:
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수

protected:
	CStaticMesh*			m_pMeshCom = nullptr;
	CSphereCollider*		m_pSphereColliderCom = nullptr;

	_bool					m_bIsRender = false;
	PxController*			m_pCtrl = nullptr;

	
	wstring					m_wstrParents[FATHER];
	wstring					m_wstrMsg = L"";
	_float					m_fHatchingTime = 60.f;
};

#endif // !__Dino_Egg_H__
