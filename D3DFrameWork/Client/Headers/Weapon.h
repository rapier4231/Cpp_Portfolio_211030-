#pragma once
#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Value.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CDynamicMesh;
class CStaticMesh;
class CSphereCollider;
END

class CPlayer;
class CWeapon : public CGameObject
{
private:
	explicit CWeapon(LPDIRECT3DDEVICE9 _pDevice);
	explicit CWeapon(const CWeapon& _rhs);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double & _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CWeapon*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void					Is_Attack(const wstring& _wstrTargetLayerTag, list<CGameObject*>* pListAtackObejct = nullptr);
	void					Is_FPVAttack(const wstring& _wstrTargetLayerTag, const _vec3& _vColPos, const _float& _fColRadius);

	void					Set_FPVPlayer(const _bool& _bIsFPV) { m_bIsFPV = _bIsFPV; }
	void					Set_WeaponAnimation(const _uint & iIndex);
	void					Set_WeaponYawPitchRoll(_float _fYaw, _float _fPitch, _float _fRoll);

	const _float&			Get_WeaponAtt() { return m_fWeaponAtt; }
	const _float&			Get_WeaponTorpid() { return m_fWeaponTorpid; }

private:
	HRESULT					Add_Component();
	HRESULT					Add_MeshComponent();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

	CDynamicMesh*			m_pDMeshTPVCom = nullptr;
	CStaticMesh*			m_pMeshTPVCom = nullptr;		// 3인칭 매쉬
	CStaticMesh*			m_pMeshFPVCom = nullptr;		// 1인칭 매쉬
	CSphereCollider*		m_pSphereColliderCom = nullptr;	// 콜리더

	WEAPON_TYPE				m_eWeaponType = WEAPON_END;		// 무기 타입

	_bool					m_bIsTPV = false;				// 3인칭인지 여부
	_bool					m_bIsDynamic = false;			// 다이나믹인지 여부

	CPlayer*				m_pPlayer = nullptr;

	const _mat*				m_pParentBoneMatrix = nullptr;
	const _mat*				m_pFPVParentBoneMatrix = nullptr;
	const _mat*				m_pBoneMatrix = nullptr;


	_float					m_fWeaponAtt = 0.f;
	_float					m_fWeaponTorpid = 0.f;

	_bool					m_bIsFPV = false;

	_uint					m_iBulletNum = 0;
};

#endif // !__WEAPON_H__
