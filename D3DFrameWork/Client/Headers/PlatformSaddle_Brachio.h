#pragma once
#ifndef __PLATFORMSADDLE_BRACHIO_H__
#define __PLATFORMSADDLE_BRACHIO_H__

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CTransform;
class CShader;
class CSphereCollider;
END

class CPlatformSaddle_Brachio : public CGameObject
{
private:
	explicit CPlatformSaddle_Brachio(LPDIRECT3DDEVICE9 _pDevice);
	explicit CPlatformSaddle_Brachio(const CPlatformSaddle_Brachio& rhs);
	virtual ~CPlatformSaddle_Brachio() = default;

public:
	virtual HRESULT					Ready_GameObject_Prototype() override;
	virtual HRESULT					Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int					Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int					LateUpdate_GameObject(const _double & _dTimeDelta) override;
	virtual void					Render_GameObject() override;
	virtual void					Render_GameObjectShadowMap() override;
	virtual void					Render_GameObjectShadow() override;

public:
	static CPlatformSaddle_Brachio* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject *			Clone(void * _pArg = nullptr) override;
	virtual void					Free() override;

public:
	void							Set_TargetDino(CGameObject* _pTargetDino);

	CGameObject*					Get_TargetDino() { return m_pTargetDino; }

private:
	HRESULT							Add_Component();
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);

private:
	HRESULT							Set_PhysXCook();
	void							Move_PhysX();

private:
	CStaticMesh*					m_pMeshCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CSphereCollider*				m_pSphereColliderCom = nullptr;

	CGameObject*					m_pTargetDino = nullptr;

	_mat							m_matParent;

	//
	PxRigidStatic*					m_pBaseActor = nullptr;
};

#endif // !__PLATFORMSADDLE_BRACHIO_H__
