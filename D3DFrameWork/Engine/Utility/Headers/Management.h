#pragma once
#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__

#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement();
	virtual ~CManagement();

public:
	HRESULT			SetUp_Scene(CScene* pScene);
	_int			Update_Scene(const _double& _dTimeDelta);
	void			Render_Scene();

public:
	HRESULT			Ready_Shader(LPDIRECT3DDEVICE9& _pDevice);

public:
	CLayer*			Get_Layer(const wstring & _wstrLayerTag);
	CGameObject*	Get_GameObject(const wstring & _wstrLayerTag, const _int& iObjIdx);
	CComponent*		Get_Component(const wstring & _wstrLayerTag, const _int& iObjIdx, const wstring & _wstrComponentTag, COMPONENTID eID);
	HRESULT			Add_GameObjectInLayer(const wstring& _wstrGameLayerTag, CGameObject* _pGameObject);
	HRESULT			Swap_GameObjectInLayer(const wstring& _wstrPreGameLayerTag, const wstring& _wstrToGameLayerTag, _uint _iIndex);
	HRESULT			Swap_GameObjectInLayer(const wstring& _wstrPreGameLayerTag, const wstring& _wstrToGameLayerTag, CGameObject* _pGameObject);
	HRESULT			RayCast_OnMesh(_vec3* pOut, const _vec3& vRayPos, const _vec3& vRayDir, const wstring& _wstrGameLayerTag);
	HRESULT			Delete_GameObject(const wstring & _wstrLayerTag, CGameObject* _pGameObject);

	//페이드인효과
private:
	HRESULT			Update_Fade(const _double& _dTimeDelta);
public:
	void			Set_Fade(const _float& fFade, const _bool& bFadeIn);
	HRESULT			Fade_In(const _float& fSec);
	HRESULT			Fade_Out(const _float& fSec);

public:
	list<CGameObject*>* Get_LayerList(const wstring& _wstrGameLayerTag);

public:
	void					Set_WorldTime(const _double _dWorldTime) { m_dWorldTime = _dWorldTime; }
	void					Set_ShadowLight(_vec3 _vLightPos, _vec3 _vLightAt);
	const SHADOWLIGHT&		Get_ShadowLight();
	void					Set_ShadowOnOff(_bool _bShadow);
	const _bool&			Get_ShadowOnOff();

public:
	virtual void	Free(void);

private:
	CScene*				m_pScene;
	_double				m_dWorldTime = 1;

	//페이드인효과
	_float				m_fFade = 1.f;
	_float				m_fFadeSpeed = 0.f;
	_bool				m_bFadeIn = true;
};
END

#endif // !__MANAGEMENT_H__
