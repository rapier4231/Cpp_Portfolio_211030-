#pragma once
#ifndef __LAYER_H__
#define __LAYER_H__

#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Ready_Layer();
	_int	Update_Layer(const _double& _dTimeDelta);
	_int	LateUpdate_Layer(const _double& _dTimeDelta);
	void	Render_Layer();

public:
	HRESULT Add_GameObject(CGameObject* _pGameObject);
	HRESULT Delete_GameObject(CGameObject* _pGameObject);
	HRESULT Delete_GameObject(const _int& iIdx);

	list<CGameObject*>* Get_ListGameObject() { return &m_listGameObject; }
	CGameObject* Get_GameObject(const _int& iIdx);
	CComponent*	Get_Component(const _int& iIdx, const wstring & _wstrComponentTag, COMPONENTID eID);

	HRESULT RayCast_OnMesh(_vec3* pOut, const _vec3& vRayPos, const _vec3& vRayDir);

public:
	static	CLayer*	Create();
	virtual void Free() override;

private:
	list<CGameObject*>	m_listGameObject;
};
END

#endif // !__LAYER_H__
