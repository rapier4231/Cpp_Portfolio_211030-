#pragma once
#ifndef __SCENE_H__
#define __SCENE_H__

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CScene() = default;

public:
	virtual	HRESULT		Ready_Scene(void) PURE;
	virtual	HRESULT		Late_Ready_Scene(void) { return S_OK; }
	virtual	_int		Update_Scene(const _double& dTimeDelta) PURE;
	virtual	void		Render_Scene(void) PURE;

public:
	CLayer*				Get_Layer(const wstring& _wstrGameLayerTag);
	CGameObject*		Get_GameObject(const wstring& _wstrLayerTag, const _int& _iObjIdx);
	CComponent*			Get_Component(const wstring & _wstrLayerTag, const _int& _iObjIdx, const wstring & _wstrComponentTag, COMPONENTID eID);
	HRESULT				Add_GameObjectInLayer(const wstring& _wstrGameLayerTag, CGameObject* _pGameObject);
	HRESULT				Swap_GameObjectInLayer(const wstring& _wstrPreGameLayerTag, const wstring& _wstrToGameLayerTag, _uint _iIndex);
	HRESULT				Swap_GameObjectInLayer(const wstring& _wstrPreGameLayerTag, const wstring& _wstrToGameLayerTag, CGameObject* _pGameObject);
	HRESULT				RayCast_OnMesh(_vec3* pOut, const _vec3& vRayPos, const _vec3& vRayDir, const wstring& _wstrGameLayerTag);

public:
	list<CGameObject*>*		Get_LayerList(const wstring& _wstrGameLayerTag);
	void					Set_ShadowLight(_vec3 _vLightPos, _vec3 _vLightAt);
	const SHADOWLIGHT&		Get_ShadowLight();
	void					Set_ShadowOnOff(_bool _bShadow);
	const _bool&			Get_ShadowOnOff();

public:
	virtual void		Free() PURE;

protected:
	LPDIRECT3DDEVICE9				m_pDevice;
	map<wstring, CLayer*>			m_mapLayer;

	SHADOWLIGHT						m_sShadowLight;

	// 그림자 기능 On/Off
	_bool							m_bShadow = false;
};
END

#endif // !__SCENE_H__
