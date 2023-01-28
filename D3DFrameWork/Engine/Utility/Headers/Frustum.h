#pragma once
#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

#include "Engine_Define.h"
#include "Base.h"
BEGIN(Engine)

class ENGINE_DLL CFrustum : public CBase
{
	DECLARE_SINGLETON(CFrustum)
private:
	explicit CFrustum();
	virtual	~CFrustum();

public:
	HRESULT Ready_Frustum(LPDIRECT3DDEVICE9 pGraphicDev);	//클라에서 최초1회 호출할 것
	HRESULT Update_Frustum();			//CScene이 업데이트 ~ 레잇업데이트 사이에 호출해줌
	HRESULT Update_LightFrustum_Ortho();			//CScene이 업데이트 ~ 레잇업데이트 사이에 호출해줌
	HRESULT Update_LightFrustum_Ortho(_vec3 vLightDir, _vec3 vPlayerPos, _vec3 vPlayerDir);			//클라이언트에서 불러올 생각이면 이걸로

private:
	virtual void Free() override;

public:
	//점이 절두체 안에 있는지 확인해 반환
	_bool CheckPoint(_vec3 vPos, CULL_TYPE eCulltype);
	_bool CheckPoint(float x, float y, float z, CULL_TYPE eCulltype);

	//구가 절두체 안에 있는지 확인해 반환
	_bool CheckSphere(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype);
	_bool CheckSphere(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype);

	//나무의 구가 절두체 안에 있는지 확인해 반환. 이때 y값 판정은 실시하지 않음
	_bool CheckTree(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype);
	_bool CheckTree(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype);

	//bool CheckCube(float, float, float, float);
	//bool CheckRectangle(float, float, float, float, float, float);

	_uint Get_DrawCount();
	_uint Get_CullCount();
	_uint Get_DrawShadowCount();
	_uint Get_CullShadowCount();

	void Set_LightFrustumCulling(_bool bTorF);

private:
	HRESULT Set_Device(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
	_vec3				m_vPointProj[8];	//투영행렬 원본 저장 (-1 ~ 1, -1 ~ 1, 0 ~ 1)

	//뷰 프러스텀 컬링
	_vec3				m_vPoint[8];
	D3DXPLANE			m_Plane[6];
	_uint				m_iDrawCount = 0;
	_uint				m_iCullCount = 0;
	
	//라이트 프러스텀 컬링
	_bool				m_bEnable_LightFrustumCulling = true;
	_float				m_fLightFrustumCenterAdj = 25.f;	//플레이어 전방으로 n만큼 전진한 지점을 절두체의 중심으로 두겠다
	_vec3				m_vPointLight[8];
	D3DXPLANE			m_PlaneLight[6];
	_uint				m_iDrawShadowCount = 0;
	_uint				m_iCullShadowCount = 0;
};
END
#endif // !__FRUSTUM_H__