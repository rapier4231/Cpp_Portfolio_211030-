#include "..\Headers\Frustum.h"
#include "Component.h"

#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
{
}

CFrustum::~CFrustum()
{
	Free();
}

HRESULT CFrustum::Ready_Frustum(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (FAILED(Set_Device(pGraphicDev)))
		return E_FAIL;

	m_vPointProj[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPointProj[1] = _vec3(1.f, 1.f, 0.f);
	m_vPointProj[2] = _vec3(1.f, -1.f, 0.f);
	m_vPointProj[3] = _vec3(-1.f, -1.f, 0.f);
	m_vPointProj[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPointProj[5] = _vec3(1.f, 1.f, 1.f);
	m_vPointProj[6] = _vec3(1.f, -1.f, 1.f);
	m_vPointProj[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

HRESULT CFrustum::Update_Frustum()
{
	//드로/컬 카운터 초기화
	m_iDrawCount = 0;
	m_iCullCount = 0;
	
	//절두체 갱신
	_mat matProj, matView;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXMatrixInverse(&matView, NULL, &matView);
	matProj *= matView;

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPointProj[i], &matProj);
		//D3DXVec3TransformCoord(&m_vPoint[i], &m_vPointProj[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// z+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	return S_OK;
}

HRESULT CFrustum::Update_LightFrustum_Ortho()
{
	//드로/컬 카운트 초기화
	m_iDrawShadowCount = 0;
	m_iCullShadowCount = 0;

	//절두체 갱신
	_mat matProj, matView;


	//투영행렬은 직교투영행렬인데, 가로세로 1000.f의 납작판(큐브?) 형태
	D3DXMatrixOrthoLH(&matProj, 100.f, 100.f, 0.f, 2000.f); //큐브형
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	//뷰행렬은 빛의 방향을 따라 플레이어 전방+Adj 지점을 바라보도록 회전시키는 행렬이므로
	//1. 빛의 방향 얻기
	_vec3 vLightDir = _vec3(1.f, -10.f, -15.f);		//변수 받아와서 고치기 <할것>
	D3DXVec3Normalize(&vLightDir, &vLightDir);

	//2. 플레이어 위치와 방향(룩벡터) 얻기
	CTransform* pPlayerTransform = (CTransform*)Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC);
	if (!pPlayerTransform)
		return E_FAIL;
	_vec3 vPlayerPos, vPlayerDir;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerDir);
	vPlayerDir.y = 0.f;	//이때 룩벡터는 y값을 제거 <-오차 줄어들거같은 느낌적인느낌??이어서
	D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);
	vPlayerPos -= vPlayerDir * m_fLightFrustumCenterAdj;	//At은 플레이어 전방+adj 지점

	//Eye는 At에서 빛의 역방향으로 적당량 후퇴한 지점. Eye.y == vLightPos.y 가 성립하는 지점까지 후퇴 (지금은 막장으로 100때림)
	//Up은 Up
	D3DXMatrixLookAtLH(&matView, &(vPlayerPos - vLightDir * 100.f), &(vPlayerPos), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixInverse(&matView, NULL, &matView);

	//구해진 두 역행렬을 이용해 절두체 원본을 월드로 변환
	matProj *= matView;

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matProj);
		//D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_PlaneLight[0], &m_vPointLight[1], &m_vPointLight[5], &m_vPointLight[6]);

	// x-
	D3DXPlaneFromPoints(&m_PlaneLight[1], &m_vPointLight[4], &m_vPointLight[0], &m_vPointLight[3]);

	// z+
	D3DXPlaneFromPoints(&m_PlaneLight[2], &m_vPointLight[7], &m_vPointLight[6], &m_vPointLight[5]);

	// z-
	D3DXPlaneFromPoints(&m_PlaneLight[3], &m_vPointLight[0], &m_vPointLight[1], &m_vPointLight[2]);

	// y+
	D3DXPlaneFromPoints(&m_PlaneLight[4], &m_vPointLight[4], &m_vPointLight[5], &m_vPointLight[1]);

	// y-
	D3DXPlaneFromPoints(&m_PlaneLight[5], &m_vPointLight[3], &m_vPointLight[2], &m_vPointLight[6]);

	return S_OK;




	////드로/컬 카운트 초기화
	//m_iDrawShadowCount = 0;
	//m_iCullShadowCount = 0;

	////절두체 갱신
	//_mat matProj, matView;


	////투영행렬은 직교투영행렬인데, 가로세로 1000.f의 납작판(큐브?) 형태
	//D3DXMatrixOrthoLH(&matProj, 1000.f, 1000.f, 0.f, 200.f);	//납작판형
	//															//D3DXMatrixOrthoLH(&matProj, 1000.f, 1000.f, 0.f, 2000.f); //큐브형
	//D3DXMatrixInverse(&matProj, NULL, &matProj);

	////뷰행렬은 플레이어 전방 +250.f 지점을 중심으로 플레이어 전방에 수직이 되도록 z회전시키는 행렬이므로
	////플레이어 위치와 룩벡터 얻기
	//CTransform* pPlayerTransform = (CTransform*)Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC);
	//if (!pPlayerTransform)
	//	return E_FAIL;
	//_vec3 vPlayerPos, vPlayerDir;
	//pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	//pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerDir);
	//D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);
	////빛의 Eye, At, Up 구하기
	////빛의 Eye (위치) : 플레이어의 위치에서 ~ 룩벡터 방향 +250.f 지점에서 ~ 빛의 반대방향으로 100.f(투영행렬 far평면의 절반)만큼 전진한 위치 
	//_vec3 vLightPos = _vec3(-400.f + vPlayerPos.x, 100.f + vPlayerPos.y, 600.f + vPlayerPos.z);
	//vLightPos += m_fLightFrustumCenterAdj * vPlayerDir;
	////빛의 At : 플레이어의 위치에서 ~ 룩벡터 방향 +250.f 지점
	//_vec3 vLightAt = vLightPos + _vec3(1.f, -10.f, -15.f);
	////빛의 Up : 플레이어의 룩벡터 <- 이거맞?는듯?

	////얻은 값들을 바탕으로 뷰행렬 얻기
	////D3DXMatrixLookAtLH(&m_matLightView, &m_vecLightPos, &m_vecLightAt, &m_vecLightUp);
	//D3DXMatrixLookAtLH(&matView, &vLightPos, &vLightAt, &vPlayerDir);
	//D3DXMatrixInverse(&matView, NULL, &matView);
	//matProj *= matView;

	//for (_uint i = 0; i < 8; ++i)
	//{
	//	D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matProj);
	//	//D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matView);
	//}

	//// x+
	//D3DXPlaneFromPoints(&m_PlaneLight[0], &m_vPointLight[1], &m_vPointLight[5], &m_vPointLight[6]);

	//// x-
	//D3DXPlaneFromPoints(&m_PlaneLight[1], &m_vPointLight[4], &m_vPointLight[0], &m_vPointLight[3]);

	//// z+
	//D3DXPlaneFromPoints(&m_PlaneLight[2], &m_vPointLight[7], &m_vPointLight[6], &m_vPointLight[5]);

	//// z-
	//D3DXPlaneFromPoints(&m_PlaneLight[3], &m_vPointLight[0], &m_vPointLight[1], &m_vPointLight[2]);

	//// y+
	//D3DXPlaneFromPoints(&m_PlaneLight[4], &m_vPointLight[4], &m_vPointLight[5], &m_vPointLight[1]);

	//// y-
	//D3DXPlaneFromPoints(&m_PlaneLight[5], &m_vPointLight[3], &m_vPointLight[2], &m_vPointLight[6]);

	//return S_OK;
}

HRESULT CFrustum::Update_LightFrustum_Ortho(_vec3 vLightDir, _vec3 vPlayerPos, _vec3 vPlayerDir)
{
	//드로/컬 카운트 초기화
	m_iDrawShadowCount = 0;
	m_iCullShadowCount = 0;

	//절두체 갱신
	_mat matProj, matView;


	//투영행렬은 직교투영행렬인데, 가로세로 1000.f의 납작판(큐브?) 형태
	D3DXMatrixOrthoLH(&matProj, 1000.f, 1000.f, 0.f, 200.f);	//납작판형
	//D3DXMatrixOrthoLH(&matProj, 1000.f, 1000.f, 0.f, 2000.f); //큐브형
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	
	//뷰행렬은 플레이어 전방 +250.f 지점을 중심으로 플레이어 전방에 수직이 되도록 z회전시키는 행렬이므로
	//플레이어 위치와 룩벡터 얻기
	/*CTransform* pPlayerTransform = (CTransform*)Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC);
	_vec3 vPlayerPos, vPlayerDir;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerDir);
	D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);*/
	//빛의 Eye, At, Up 구하기
	//빛의 Eye (위치) : 플레이어의 위치에서 ~ 룩벡터 방향 +250.f 지점에서 ~ 빛의 반대방향으로 100.f(투영행렬 far평면의 절반)만큼 전진한 위치 
	/*_vec3 vLightPos = _vec3(-400.f + vPlayerPos.x, 100.f + vPlayerPos.y, 600.f + vPlayerPos.z);
	vLightPos += m_fLightFrustumCenterAdj * vPlayerDir;*/
	//빛의 At : 플레이어의 위치에서 ~ 룩벡터 방향 +250.f 지점
	//_vec3 vLightAt = vLightPos + _vec3(1.f, -10.f, -15.f);
	//빛의 Up : 플레이어의 룩벡터 <- 이거맞?는듯?


	//빛의 Eye, At, Up 구하기
	//빛의 Eye (위치) : 플레이어의 위치에서 ~ 룩벡터 방향 +250.f 지점에서 ~ 빛의 반대방향으로 100.f(투영행렬 far평면의 절반)만큼 전진한 위치 
	vPlayerPos += vPlayerDir * m_fLightFrustumCenterAdj;
	_vec3 vEye = vPlayerPos - vLightDir * 100.f;
	//빛의 At : 플레이어의 위치에서 ~ 룩벡터 방향 +250.f 지점
	_vec3 vAt = vPlayerPos;
	//빛의 Up : 플레이어의 룩벡터 <- 이거맞?는듯?
	_vec3 vUp = vPlayerDir;
	
	//얻은 값들을 바탕으로 뷰행렬 얻기
	//D3DXMatrixLookAtLH(&m_matLightView, &m_vecLightPos, &m_vecLightAt, &m_vecLightUp);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	D3DXMatrixInverse(&matView, NULL, &matView);		
	matProj *= matView;

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matProj);
		//D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_PlaneLight[0], &m_vPointLight[1], &m_vPointLight[5], &m_vPointLight[6]);

	// x-
	D3DXPlaneFromPoints(&m_PlaneLight[1], &m_vPointLight[4], &m_vPointLight[0], &m_vPointLight[3]);

	// z+
	D3DXPlaneFromPoints(&m_PlaneLight[2], &m_vPointLight[7], &m_vPointLight[6], &m_vPointLight[5]);

	// z-
	D3DXPlaneFromPoints(&m_PlaneLight[3], &m_vPointLight[0], &m_vPointLight[1], &m_vPointLight[2]);

	// y+
	D3DXPlaneFromPoints(&m_PlaneLight[4], &m_vPointLight[4], &m_vPointLight[5], &m_vPointLight[1]);

	// y-
	D3DXPlaneFromPoints(&m_PlaneLight[5], &m_vPointLight[3], &m_vPointLight[2], &m_vPointLight[6]);

	return S_OK;
}

_uint CFrustum::Get_DrawCount()
{
	return m_iDrawCount;
}

_uint CFrustum::Get_CullCount()
{
	return m_iCullCount;
}

_uint CFrustum::Get_DrawShadowCount()
{
	return m_iDrawShadowCount;
}

_uint CFrustum::Get_CullShadowCount()
{
	return m_iCullShadowCount;
}

void CFrustum::Set_LightFrustumCulling(_bool bTorF)
{
	m_bEnable_LightFrustumCulling = bTorF;
}

HRESULT CFrustum::Set_Device(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pDevice = pGraphicDev;
	m_pDevice->AddRef();

	return S_OK;
}

bool CFrustum::CheckPoint(_vec3 vPos, CULL_TYPE eCulltype)
{
	if (eCulltype == CULL_VIEW)
	{
		for (_uint i = 0; i < 6; ++i)
		{
			//삼각형을 시계방향으로 감았으므로 법선 방향이 바깥쪽이다.
			//결과값이 0보다 크면 -> 평면 바깥이므로 실패, false 반환
			if (D3DXPlaneDotCoord(&m_Plane[i], &vPos) > 0.f)
			{
				m_iCullCount++;
				return false;
			}
		}

		//모든 점이 평면 안쪽이면 true 반환
		m_iDrawCount++;
		return true;
	}
	else if (eCulltype == CULL_LIGHT)
	{
		if (!m_bEnable_LightFrustumCulling)
		{
			m_iDrawShadowCount = -1;
			m_iCullShadowCount = -1;
			return true;
		}

		for (_uint i = 0; i < 6; ++i)
		{
			//삼각형을 시계방향으로 감았으므로 법선 방향이 바깥쪽이다.
			//결과값이 0보다 크면 -> 평면 바깥이므로 실패, false 반환
			if (D3DXPlaneDotCoord(&m_PlaneLight[i], &vPos) > 0.f)
			{
				m_iCullShadowCount++;
				return false;
			}
		}

		//모든 점이 평면 안쪽이면 true 반환
		m_iDrawShadowCount++;
		return true;
	}

	ERR_MSG(L"CFrustrum::CheckPoint() : wrong CULL_TYPE");
	return false;
}

bool CFrustum::CheckPoint(float x, float y, float z, CULL_TYPE eCulltype)
{
	return CheckPoint(_vec3(x, y, z), eCulltype);
}

bool CFrustum::CheckSphere(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype)
{
	if (eCulltype == CULL_VIEW)
	{
		for (_uint i = 0; i < 6; ++i)
		{
			//삼각형을 시계방향으로 감았으므로 법선 방향이 바깥쪽이다.
			//결과값이 점과의 수선 길이이므로 이것이 (양수값이면서) 반지름보다 크면 
			//	-> 구가 평면 밖으로 완전히 빠져나간 것이므로 실패, false 반환
			if (D3DXPlaneDotCoord(&m_Plane[i], &vCenter) > fRadius)
			{
				m_iCullCount++;
				return false;
			}
		}

		m_iDrawCount++;
		return true;
	}
	else if (eCulltype == CULL_LIGHT)
	{
		if (!m_bEnable_LightFrustumCulling)
		{
			m_iDrawShadowCount = -1;
			m_iCullShadowCount = -1;
			return true;
		}

		for (_uint i = 0; i < 6; ++i)
		{
			//삼각형을 시계방향으로 감았으므로 법선 방향이 바깥쪽이다.
			//결과값이 점과의 수선 길이이므로 이것이 (양수값이면서) 반지름보다 크면 
			//	-> 구가 평면 밖으로 완전히 빠져나간 것이므로 실패, false 반환
			if (D3DXPlaneDotCoord(&m_PlaneLight[i], &vCenter) > fRadius)
			{
				m_iCullShadowCount++;
				return false;
			}
		}

		m_iDrawShadowCount++;
		return true;
	}

	ERR_MSG(L"CFrustrum::CheckSphere() : wrong CULL_TYPE");
	return false;
}

bool CFrustum::CheckSphere(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype)
{
	return CheckSphere(_vec3(xCenter, yCenter, zCenter), fRadius, eCulltype);
}

_bool CFrustum::CheckTree(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype)
{
	if (eCulltype == CULL_VIEW)
	{
		for (_uint i = 0; i < 4; ++i)	//y값(4번, 5번 평면)의 검출을 실시하지 않음
		{
			//삼각형을 시계방향으로 감았으므로 법선 방향이 바깥쪽이다.
			//결과값이 점과의 수선 길이이므로 이것이 (양수값이면서) 반지름보다 크면 
			//	-> 구가 평면 밖으로 완전히 빠져나간 것이므로 실패, false 반환
			if (D3DXPlaneDotCoord(&m_Plane[i], &vCenter) > fRadius)
			{
				m_iCullCount++;
				return false;
			}
		}

		m_iDrawCount++;
		return true;
	}
	else if (eCulltype == CULL_LIGHT)
	{
		if (!m_bEnable_LightFrustumCulling)
		{
			m_iDrawShadowCount = -1;
			m_iCullShadowCount = -1;
			return true;
		}

		for (_uint i = 0; i < 4; ++i)	//y값(4번, 5번 평면)의 검출을 실시하지 않음
		{
			//삼각형을 시계방향으로 감았으므로 법선 방향이 바깥쪽이다.
			//결과값이 점과의 수선 길이이므로 이것이 (양수값이면서) 반지름보다 크면 
			//	-> 구가 평면 밖으로 완전히 빠져나간 것이므로 실패, false 반환
			if (D3DXPlaneDotCoord(&m_PlaneLight[i], &vCenter) > fRadius)
			{
				m_iCullShadowCount++;
				return false;
			}
		}

		m_iDrawShadowCount++;
		return true;
	}

	ERR_MSG(L"CFrustrum::CheckSphere() : wrong CULL_TYPE");
	return false;
}

_bool CFrustum::CheckTree(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype)
{
	return CheckTree(_vec3(xCenter, yCenter, zCenter), fRadius, eCulltype);
}

//
//bool CFrustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
//{
//	// 뷰 프러스 텀에 큐브의 한 점이 있는지 확인합니다.
//	for (int i = 0; i<6; i++)
//	{
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f)
//			continue;
//
//		return false;
//	}
//
//	return true;
//}
//
//
//bool CFrustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
//{
//	// 사각형의 6 개의 평면 중 하나가 뷰 frustum 안에 있는지 확인합니다.
//	for (int i = 0; i<6; i++)
//	{
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
//			continue;
//		
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		return false;
//	}
//
//	return true;
//}

void CFrustum::Free()
{
	Safe_Release(m_pDevice);
}
