#pragma once
#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "Component.h"
#include "DynamicMesh.h"

BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CSphereCollider : public	CComponent
{
public:
	enum COL_TYPE
	{
		// HIT은 서로 밀어내는 것 까지 처리할 거임
		// attack은 자기가 공격할 때 쓸 콜라이더
		COL_HIT, COL_ATTACK, COL_INTERACTION, COL_BUILD_PIVOT, COL_END
	};

	enum COL_DIR
	{
		// 건축 피벗용 (앞,뒤,왼쪽,오른쪽,위,아래)
		COL_FWD, COL_BWD, COL_LEFT, COL_RIGHT, COL_UP, COL_DOWN, COL_DIR_END
	};

	typedef struct SpereInfo
	{
		wstring				strBoneName = L"";
		_float				fRadius = 3.f;
		_vec3				vSpherePos = { 0.f,0.f,0.f };
		_bool				bHead = false;

		LPD3DXMESH			pSphere = nullptr;
		_mat				matColMatrix;
		CDynamicMesh*		pTartgetMesh = nullptr;
		const _mat*			pTargetWorldMatix = nullptr;

		const _mat*			pParentBoneMatrix = nullptr;
		const _mat*			pParentWorldMatrix = nullptr;

		// 수동으로 값 넣어줄 때 사용하는 용도
		COL_TYPE		eColType;
		COL_DIR			eColDir;
	}SPHEREINFO;

private:
	explicit CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereCollider(const CSphereCollider& rhs);
	virtual ~CSphereCollider(void);

public:
	HRESULT				Ready_SphereCollider(wstring strBoneName, CGameObject* pObject, COL_TYPE eType, _vec3 vPos, _float fRadius, COL_DIR _eColDir = COL_DIR_END);
	void				Render_SphereCollider();
	virtual _int		Update_Component(const _double& _dTimeDelta) override;
	void				Set_Pos(_vec3& vPos);
	list<SPHEREINFO>*	Get_listCollider(const COL_TYPE& eType) { return &m_listCollider[eType]; }
	_float*				Get_Radius();
	_vec3*				Get_Pos();
	_vec3*				Get_Pos(COL_TYPE eType);
	_float				Get_InteractRadius();
	_vec3				Get_InteractPos();
	HRESULT				Add_SphereCollider(SPHEREINFO* tInfo, CGameObject* pObject);

	//툴용
	HRESULT				Add_SphereCollider(const wstring& strBoneName, CGameObject* pObject, COL_TYPE eType, _vec3 vPos = { 0.f,0.f,0.f }, _float fRadius = 100.f);
	void				Delete_SphereCollider(wstring strBoneName);
	void				Delete_AllCollider();
	void				Set_Radius(wstring strBoneName, _float fRad);
	void				Set_Radius(_float fRad);
	void				Set_Pos(wstring strBoneName, _vec3& vPos);
	void				Set_Head(wstring strBoneName, _bool bHead);
	SPHEREINFO*			Get_SphereInfo(wstring strBoneName, _int* iColType = nullptr);
	void				Get_BoneName(list<wstring>* pBoneList);
	list<SPHEREINFO>*   Get_listCollider() { if (m_listCollider->empty()) return nullptr;  else return m_listCollider; }

private:
	list<SPHEREINFO>	m_listCollider[COL_END];

#ifdef _DEBUG
	_bool				m_bIsRender = false;
#endif

public:
	static CSphereCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CSphereCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring strBoneName, CGameObject* pObject, COL_TYPE eType, _vec3 vPos = { 0.f,0.f,0.f }, _float fRadius = 100.f);
	virtual CComponent*			Clone(void * pArg = nullptr) override;
	virtual HRESULT				Ready_Component_Prototype() override;
	virtual HRESULT				Ready_Component(void * _pArg = nullptr) override;
	virtual void				Free(void);

};

END
#endif // SphereCollider_h__
